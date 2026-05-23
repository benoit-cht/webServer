/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcluzan <lcluzan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 06:01:32 by bchallat          #+#    #+#             */
/*   Updated: 2026/05/23 18:11:08 by lcluzan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstring>  // Pour memset
#include <fcntl.h>
#include <sys/wait.h>

#include "../../include/network/EventLoop.hpp"
#include "../../include/cgi/CgiHandler.hpp"

EventLoop::EventLoop(const std::vector<int>& ports) { setupServerSockets(ports); }

EventLoop::~EventLoop() { cleanup(); }

/* ************************************************************************** */

void setNonBlocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) return;
    // Sur Linux, O_NONBLOCK suffit amplement.
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void EventLoop::run()
{
    std::cout << COLOR_MAGENTA << "Info : start event loop..." << COLOR_RESET << std::endl;
    while (true)
    {
        // Attendre une activité sur les sockets (timeout infini)
        int ret = poll(&_poll_fds[0], _poll_fds.size(), -1);
        if (ret == -1)
        {
            //throw std::runtime_error("poll() " + std::string(strerror(errno)));
          std::cout << COLOR_RED << "Error : poll() failed interrupted system call ?" << COLOR_RESET << std::endl;
            break;
        }

        // Parcourir tous les descripteurs surveillés
        for (size_t i = 0; i < _poll_fds.size(); i++)
        {
            int current_fd = _poll_fds[i].fd;

            // --- LOGIQUE CGI ASYNCHRONE ---
                if (_cgi_contexts.find(current_fd) != _cgi_contexts.end()) {
                    CgiContext* ctx = _cgi_contexts[current_fd];

            // CAS A: poll() nous dit qu'on peut écrire des données dans le CGI (requête POST)
                if ((_poll_fds[i].revents & POLLOUT) && current_fd == ctx->pipe_in) {

                if (ctx->bytes_written < ctx->request_body.size()) {
                    ssize_t ret = write(ctx->pipe_in,
                                        ctx->request_body.c_str() + ctx->bytes_written,
                                        ctx->request_body.size() - ctx->bytes_written);
                if (ret > 0) ctx->bytes_written += ret;
                }

                // Quand tout est envoyé, on ferme le pipe pour que le PHP comprenne que c'est fini (EOF)
                if (ctx->bytes_written >= ctx->request_body.size()) {
                    close(ctx->pipe_in);
                    _cgi_contexts.erase(ctx->pipe_in);
                    _poll_fds.erase(_poll_fds.begin() + i);
                    i--; // On recule l'index car on a réduit le vecteur
                }
            }
            // CAS B: poll() nous dit qu'on peut lire le résultat généré par le PHP
            else if ((_poll_fds[i].revents & POLLIN) && current_fd == ctx->pipe_out) {
                char buffer[4096];
                ssize_t bytes = read(ctx->pipe_out, buffer, sizeof(buffer) - 1);

                if (bytes > 0) {
                    buffer[bytes] = '\0';
                    ctx->response_buffer += buffer; // On concatène petit à petit
                }else if (bytes == 0 || (bytes < 0 && errno != EAGAIN)) {
                    // Fin de la lecture (EOF) ou erreur critique

                    // WNOHANG est magique sous Linux: il libère le zombie instantanément sans bloquer
                    waitpid(ctx->pid, NULL, WNOHANG);

                    // Re-créer la réponse HTTP depuis le buffer brut
                    t_httpResponse response;
                    size_t header_end = ctx->response_buffer.find("\r\n\r\n");
                    if (header_end != std::string::npos) {
                        response.status = 200;
                        response.body = ctx->response_buffer.substr(header_end + 4);
                    } else {
                        response.status = 502; // Bad Gateway
                        response.body = "<html><body>502 Bad Gateway</body></html>";
                    }

                    std::string raw_response = response.toString();
                    // Envoi (synchronisé pour l'instant, mais idéalement asynchrone aussi à terme)
                    _socket_handler.writeToSocket(ctx->client_fd, raw_response.c_str(), raw_response.size());

                    // Grand nettoyage de la mémoire
                    close(ctx->pipe_out);
                    _cgi_contexts.erase(ctx->pipe_out);
                    _poll_fds.erase(_poll_fds.begin() + i);
                    delete ctx;
                    i--;
                }
            }
            continue;
        }
            if (_poll_fds[i].revents & POLLIN)
            {
                // Vérifier si c'est un socket serveur (nouvelle connexion)
                bool is_server_fd = false;

                for (size_t j = 0; j < _server_fds.size(); j++)
                {
                  if (_poll_fds[i].fd == _server_fds[j])
                  {
                    is_server_fd = true;
                    break;
                  }
                }
                  bool  is_cgi_fd = (_cgi_contexts.find(_poll_fds[i].fd) != _cgi_contexts.end());

                 if (is_server_fd) {
                   handleNewConnection(_poll_fds[i].fd);
                } else {
                    handleClientRequest(_poll_fds[i].fd);
                }
            }
        }
    }
}
/* ************************************************************************** */

void EventLoop::handleNewConnection(int server_fd)
{
   /* struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);*/

    // Accepter la nouvelle connexion
    std::string client_ip;
    int client_port;
    int client_fd = _socket_handler.acceptConnection(server_fd, client_ip, client_port);
    if (client_fd == -1)
    {
        std::cerr << "Erreur lors de l'acceptation de la connexion." << std::endl;
        return;
    }

    // Ajouter le client à ClientManager
    _client_manager.addClient(client_fd, client_ip, client_port);

    // Ajouter le client à la liste des descripteurs surveillés par poll()
    struct pollfd client_pollfd;
    client_pollfd.fd = client_fd;
    client_pollfd.events = POLLIN;
    _poll_fds.push_back(client_pollfd);

    std::cout << COLOR_CYAN << "(Log Client) : new conection accept (fd=" << client_fd
              << ", IP=" << client_ip
              << ", Port=" << client_port << ")" << COLOR_RESET << std::endl;
}

void EventLoop::handleClientRequest(int fd)
{
    // 1. Récupérer le client
    ClientInfo* client = _client_manager.getClient(fd);
    if (!client)
    {
        std::cerr << "Client non trouvé (fd=" << fd << ")." << std::endl;
        _socket_handler.closeSocket(fd);
        return;
    }

    // 2. Lire les données du client
    char buffer[4096];
    memset(buffer, 0, sizeof(buffer));
    ssize_t bytes_read = _socket_handler.readFromSocket(fd, buffer, sizeof(buffer) - 1);
    if (bytes_read <= 0)
    {
        // Client déconnecté ou erreur de lecture
        std::cout << COLOR_CYAN << "(Log Client) : client déconnecté (fd=" << fd << ")." << COLOR_RESET << std::endl;
        _client_manager.removeClient(fd);

        // Retirer le fd de _poll_fds
        for (size_t i = 0; i < _poll_fds.size(); i++)
        {
            if (_poll_fds[i].fd == fd)
            {
                _poll_fds.erase(_poll_fds.begin() + i);
                break;
            }
        }
        return;
    }

    // 3. Afficher les données reçues (pour débogage)
    std::cout << COLOR_BLUE
              << "Données reçues de " << client->getAdressIp()
              << ":" << client->getPortNumber() << " (" << bytes_read << " octets) :\n"
              << COLOR_RESET << std::endl;

    t_httpRequest   request  = HttpHandler::setHttpRequest(buffer);
    if (request.path.find("/cgi-bin/") == 0 || request.path.find(".php") != std::string::npos) {

        // On lance le fork sans attendre le résultat
        CgiContext* ctx = CgiHandler::startCgi(request.path, request, fd);

        if (!ctx) {
             // Sécurité : si le fork ou pipe échoue
             t_httpResponse response(500, std::map<std::string, std::string>(), "500 Internal Error");
             std::string raw = response.toString();
             _socket_handler.writeToSocket(fd, raw.c_str(), raw.size());
             return;
        }
        // On enregistre le contexte pour que le poll() (dans EventLoop::run) s'en occupe
        _cgi_contexts[ctx->pipe_in] = ctx;
        _cgi_contexts[ctx->pipe_out] = ctx;

        struct pollfd pfd_in;
        pfd_in.fd = ctx->pipe_in;
        pfd_in.events = POLLOUT; // Pour envoyer le body (ex: POST)
        _poll_fds.push_back(pfd_in);

        struct pollfd pfd_out;
        pfd_out.fd = ctx->pipe_out;
        pfd_out.events = POLLIN; // Pour lire la réponse du script PHP
        _poll_fds.push_back(pfd_out);

        // TRÈS IMPORTANT : On s'arrête là !
        // On ne génère pas de t_httpResponse tout de suite.
        return;
    }
    t_httpResponse  response = HttpHandler::setHttpResponse(request);

    std::string raw_response = response.toString();
    _socket_handler.writeToSocket(fd, raw_response.c_str(), raw_response.size());
    print_http_response(response);

}

void EventLoop::setupServerSockets(const std::vector<int>& ports)
{
    // Créer un socket serveur pour chaque port
    _server_fds = _socket_handler.createSocketWithPort(ports);

    // Ajouter chaque socket serveur à _poll_fds pour surveillance
    for (size_t i = 0; i < _server_fds.size(); i++)
    {
        struct pollfd pfd;
        pfd.fd = _server_fds[i];
        pfd.events = POLLIN;      // Surveiller les nouvelles connexions
        _poll_fds.push_back(pfd);
    }
}

/* ************************************************************************** */

void EventLoop::cleanup()
{
    // Fermer tous les sockets serveur
    for (size_t i = 0; i < _server_fds.size(); i++)
    {
        _socket_handler.closeSocket(_server_fds[i]);
    }

    // Fermer tous les sockets clients (via ClientManager)
    const std::vector<ClientInfo*>& clients = _client_manager.getClients();
    for (size_t i = 0; i < clients.size(); i++)
    {
        _socket_handler.closeSocket(clients[i]->getFileDescriptor());
    }
}

/* ************************************************************************** */
/*                                                                            */
/* Constructor: Prend une liste de ports à écouter                            */
/* Destructor: nettoye les vector des attribue proprement                     */
/*                                                                            */
/* Methode public                                                             */
/*   run : boucle d'evennement                                                */
/*                                                                            */
/* Methode priver                                                             */
/*   cleanup : Nettoie les ressources (fermeture des sockets)                 */
/*   handleNewConnection : Accepte une nouvelle connexion                     */
/*   handleClientRequest: Traite une requête client                           */
/*   setupServerSockets:  Configure les sockets serveur                       */
/*                                                                            */
/* ************************************************************************** */

