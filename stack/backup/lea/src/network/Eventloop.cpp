/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Eventloop.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcluzan <lcluzan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 19:15:57 by lcluzan           #+#    #+#             */
/*   Updated: 2026/04/23 00:53:29 by lcluzan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../include/NetworkTcp.hpp"

extern volatile sig_atomic_t g_should_exit; // Ctrl+C

EventLoop::EventLoop(int port) {
    setupServerSocket(port);
}

EventLoop::~EventLoop(void) {
    cleanup();
}

// Initializes the server socket and sets up the epoll instance
void EventLoop::setupServerSocket(int port) {
    _server_fd = _socket_handler.createServerSocket(port);

    _epoll_fd = epoll_create(10);
    if (_epoll_fd == -1)
        throw std::runtime_error("epoll_create failed");

    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = _server_fd;

    if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, _server_fd, &event) == -1)
        throw std::runtime_error("epoll_ctl failed for server socket");
}

// Accepts a new client and adds it to epoll
void EventLoop::handleNewConnection() {
    std::string client_ip;
    int client_port;

    int client_fd = _socket_handler.acceptConnection(_server_fd, client_ip, client_port);
    if (client_fd == -1) return;

    _client_manager.addClient(client_fd, client_ip, client_port);

    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = client_fd;

    if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, client_fd, &event) == -1) {
        _client_manager.removeClient(client_fd);
        _socket_handler.closeSocket(client_fd);
    }
}

// Main event loop handling all multiplexed I/O with epoll
void EventLoop::run() {
    struct epoll_event events[64];

    std::cout << "Server is running and waiting for connections..." << std::endl;
    while (!g_should_exit) {
        int num_events = epoll_wait(_epoll_fd, events, 64, -1);
        if (num_events == -1 && errno != EINTR) break;

        for (int i = 0; i < num_events; ++i) {
            if (events[i].data.fd == _server_fd)
                handleNewConnection();
            else
                handleClientRequest(events[i].data.fd);
        }
    }
}


// Reads data, handles disconnects, and checks for full HTTP headers
void EventLoop::handleClientRequest(int fd) {
    char buffer[BUFFER_SIZE];

    // Read data from the non-blocking socket
    ssize_t bytes = read(fd, buffer, sizeof(buffer) - 1);

    // Handle client disconnection (0) or actual errors (-1)
    if (bytes <= 0) {
        if (bytes == 0 || (bytes == -1 && errno != EAGAIN && errno != EWOULDBLOCK)) {
            epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, fd, NULL); // Remove from epoll
            _client_manager.removeClient(fd);              // Free your ClientInfo
            _socket_handler.closeSocket(fd);               // Close the fd
        }
        return; // EAGAIN simply returns to the epoll loop
    }

    // Null-terminate to safely convert to std::string
    buffer[bytes] = '\0';
    std::string chunk(buffer);

    // [!] CRITICAL: Here should append 'chunk' to a string inside your ClientInfo!
    // TCP fragments packets:  might not receive the whole request in one read().

    // Check if the HTTP headers are fully received
    if (chunk.find("\r\n\r\n") != std::string::npos) {
        std::cout << "Headers completely received for FD: " << fd << std::endl;

    // Check if we received at least the full HTTP header
    if (client->isHeaderComplete()) {
        std::cout << "Full HTTP Header received for FD " << fd << std::endl;

       // 4. Check if we received at least the full HTTP header
    if (client->isHeaderComplete()) {
        
        RequestHttp request;
        
        // request.parse renverra 'true' SEULEMENT si Headers ET Body sont complets
        if (request.parse(client->getBuffer())) {
            
            std::cout << "Requête 100% complète reçue pour FD " << fd << std::endl;
            
        // Check if we received at least the full HTTP header
    if (client->isHeaderComplete()) {
        
        RequestHttp request;
        
        if (request.parse(client->getBuffer())) {
            ResponseHttp response;
            std::string filepath = "sources/www"; // Le dossier racine de votre site
            
            // Si le client demande "/", on lui donne "/index.html"
            std::string link = request.getLink(); // À ajouter dans RequestHttp.hpp
            if (link == "/") link = "/index.html";
            
            filepath += link;
            std::string fileContent = readFile(filepath);
            
            // Si le fichier est vide ou n'existe pas (Erreur 404)
            if (fileContent.empty()) {
                response.setStatusCode(404, "Not Found");
                response.setHeader("Content-Type", "text/html");
                response.setBody("<html><body><h1>404 - Fichier non trouve</h1></body></html>");
            } else {
                // Succès 200 OK
                response.setStatusCode(200, "OK");
                response.setHeader("Content-Type", "text/html");
                response.setBody(fileContent);
            }
            
            // Génère la chaîne HTTP finale
            std::string final_response = response.buildResponse();
            
            // Envoie au client !
            _socket_handler.writeToSocket(fd, final_response.c_str(), final_response.size());
            
            std::cout << "Réponse envoyée au FD " << fd << " !" << std::endl;
            
            client->clearBuffer(); 
        }
    }
            
            // Préparer le client pour sa potentielle prochaine requête
            client->clearBuffer(); 
            
        } else {
            std::cout << "Téléchargement du Body en cours pour FD " << fd << "..." << std::endl;
        }
    }
        RequestHttp request;
        if (request.parse(client->getBuffer())) {
            // La requête est valide !
            // Prochaine étape : préparer la réponse (ex: lire /index.html)
        } else {
            // Requête malformée (Erreur 400 Bad Request)
        }
        // ------------------------
    }
    }
}

// Gère l'envoi asynchrone des données au client
void EventLoop::handleClientWrite(int fd) {
    ClientInfo* client = _client_manager.getClient(fd);
    if (!client) return;

    // Tenter d'envoyer les données
    ssize_t bytes = client->sendData();

    // Gérer les vraies erreurs (déconnexion)
    if (bytes < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
        epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, fd, NULL);
        _client_manager.removeClient(fd);
        _socket_handler.closeSocket(fd);
        return;
    }

    // Vérifier si on a fini d'envoyer toute la réponse
    if (client->isResponseComplete()) {
        std::cout << "Réponse entièrement envoyée au FD " << fd << std::endl;
        
        client->clearBuffers();

        // On repasse le socket en mode "Lecture" (EPOLLIN) pour attendre la prochaine requête
        struct epoll_event event;
        event.events = EPOLLIN;
        event.data.fd = fd;
        epoll_ctl(_epoll_fd, EPOLL_CTL_MOD, fd, &event);
    }
}

//mettre ailleurs dans un utils ou autre
#include <fstream>

// Lit le contenu complet d'un fichier et le retourne sous forme de string
static std::string readFile(const std::string& filepath) {
    std::ifstream file(filepath.c_str());
    if (!file.is_open()) {
        return ""; // Le fichier n'existe pas ou n'a pas les droits de lecture
    }
    
    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}