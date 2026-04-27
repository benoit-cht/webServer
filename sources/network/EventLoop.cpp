/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcluzan <lcluzan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 06:01:32 by bchallat          #+#    #+#             */
/*   Updated: 2026/04/27 16:23:29 by lcluzan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstring>  // Pour memset

#include "../../include/network/EventLoop.hpp"

EventLoop::EventLoop(const std::vector<int>& ports) { setupServerSockets(ports); }

EventLoop::~EventLoop() { cleanup(); }

/* ************************************************************************** */

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
              bool  is_cgi_fd = (_cgi_to_client.find(_poll_fds[i].fd) != _cgi_to_client.end());
              
              if (is_server_fd) {
                handleClientRequest(_poll_fds[i].fd);
              }
              else if (is_cgi_fd) {
                handleCgiResponse(_poll_fds[i].fd);
              }
              else {
                handleCgiResponse(_poll_fds[i].fd);
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
              //<< "---------------------------------------------\n"
              //<< buffer 
              //<< "---------------------------------------------\n"
              << COLOR_RESET << std::endl;

    t_httpRequest   request  = HttpHandler::setHttpRequest(buffer); 
    t_httpResponse  response = HttpHandler::setHttpResponse(request);
   
    std::string raw_response = response.toString();
    _socket_handler.writeToSocket(fd, raw_response.c_str(), raw_response.size());
    print_http_response(response); 

}

void EventLoop::handleCgiResponse(int cgi_fd)
{
    // Retrouver à qui appartient cette réponse
    int client_fd = _cgi_to_client[cgi_fd];
    pid_t pid = _cgi_to_pid[cgi_fd];

    // Lire la réponse du processus enfant (PHP/Python)
    char buffer[4096];
    std::string cgi_output;
    int bytes_read;
    
    // On lit tout ce qu'il y a dans le pipe
    while ((bytes_read = read(cgi_fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0';
        cgi_output += buffer;
    }

    // Récupérer le processus enfant pour éviter un zombie
    // Puisque read a renvoyé 0 (ou fini), le script a terminé, waitpid ne bloquera pas
    waitpid(pid, NULL, 0);

    // Formater la réponse pour ton Webserv
    t_httpResponse response;
    size_t header_end = cgi_output.find("\r\n\r\n");
    if (header_end != std::string::npos) {
        response.status = 200;
        // On récupère uniquement le body (on met les headers bruts de côté)
        response.body = cgi_output.substr(header_end + 4);
        
        std::string header_block = cgi_output.substr(0, header_end);

        size_t line_start = 0;
        size_t line_end = header_block.find("\r\n");

        while (line_end != std::string::npos) {
            std::string line = header_block.substr(line_start, line_end - line_start);
            size_t colon_pos = line.find(":");

            if (colon_pos != std::string::npos) {
                std::string key = line.substr(0, colon_pos);
                
                size_t value_start = line.find_first_not_of(" ", colon_pos + 1);
                std::string value = "";
                if (value_start != std::string::npos) {
                    value = line.substr(value_start);
                }
                response.headers[key] = value;
        }
        line_start = line_end + 2;
        line_end = header_block.find("\r\n", line_start);
        }
    } 
    else {
        response.status = 502; // Bad Gateway
        response.body = "<html><body>502 Bad Gateway (Invalid CGI output)</body></html>";
    }

    // Envoyer la réponse finale au vrai client
    std::string raw_response = response.toString();
    _socket_handler.writeToSocket(client_fd, raw_response.c_str(), raw_response.size());
    std::cout << COLOR_GREEN << "✅ CGI Response sent to client (fd=" << client_fd << ")" << COLOR_RESET << std::endl;

    // 🧹 LE GRAND NETTOYAGE 🧹
    close(cgi_fd); // Fermer le pipe de lecture
    _cgi_to_client.erase(cgi_fd); // Enlever des maps
    _cgi_to_pid.erase(cgi_fd);

    // Retirer le FD du CGI de la liste surveillée par poll()
    for (size_t i = 0; i < _poll_fds.size(); i++) {
        if (_poll_fds[i].fd == cgi_fd) {
            _poll_fds.erase(_poll_fds.begin() + i);
            break;
        }
    }
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

