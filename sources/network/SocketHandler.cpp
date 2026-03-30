/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchallat <bchallat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 13:33:39 by bchallat          #+#    #+#             */
/*   Updated: 2026/03/25 06:47:14 by bchallat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../include/network/SocketHandler.hpp"

SocketHandler::SocketHandler() {}
SocketHandler::~SocketHandler() {}

/* ************************************************************************** */

std::vector<int> SocketHandler::createServerSockets(const std::vector<int>& ports)
{
    std::vector<int> server_fds;
    for (size_t i = 0; i < ports.size(); i++) 
    {
        int fd = createServerSocket(ports[i]);  // Appelle la méthode pour un seul port
        server_fds.push_back(fd);
    }
    
    return server_fds;
}

int SocketHandler::createServerSocket(int port) 
{
    // 1. Créer le socket
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) 
        throw std::runtime_error("[err](socket()) " + std::string(strerror(errno)));
    
    // 2. Autoriser la réutilisation du port
    /*int opt = 1;
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        close(socket_fd);
        throw std::runtime_error("setsockopt() failed: " + std::string(strerror(errno)));
    }*/

    // 3. Configurer l'adresse du serveur
    struct sockaddr_in socket_addr;
    memset(&socket_addr, 0, sizeof(socket_addr));
    socket_addr.sin_family = AF_INET;
    socket_addr.sin_addr.s_addr = INADDR_ANY;  // Écouter sur toutes les interfaces
    socket_addr.sin_port = htons(port);

    // 4. Associer le socket à l'adresse et au port
    if (bind(socket_fd, (struct sockaddr *)&socket_addr, sizeof(socket_addr)) == -1)
    {
        close(socket_fd);
        throw std::runtime_error("[err](bind()) " + std::string(strerror(errno)));
    }

    // 5. Mettre le socket en mode écoute
    if (listen(socket_fd, SOMAXCONN) == -1) 
    {
        close(socket_fd);
        throw std::runtime_error("[err](listen()) " + std::string(strerror(errno)));
    }

    // 6. Associer le fd au port pour référence future
    //_fd_to_port[socket_fd] = port;

    std::cout << "[log] open socket to a port: " << port << std::endl;
    return socket_fd;
}

void SocketHandler::closeSocket(int fd)
{
    if (close(fd) == -1)
        std::cerr << "[err](close()) " << strerror(errno) << std::endl;
    std::cout << "[log] close socket success " << std::endl;
}

/* ************************************************************************** */

int SocketHandler::acceptConnection(int socket_fd, std::string& client_ip, int& client_port)
{
    int                 client_fd = 0;
    char                ip_str[INET_ADDRSTRLEN];
    socklen_t           client_addr_len = 0;
    struct sockaddr_in  client_addr;

    client_addr_len = sizeof(client_addr);
    client_fd = accept(socket_fd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_fd == -1)
    {
        throw std::runtime_error("[err](accept()) " + std::string(strerror(errno)));
    }
    else 
    {
      inet_ntop(AF_INET, &(client_addr.sin_addr), ip_str, INET_ADDRSTRLEN);
      client_ip = std::string(ip_str);
      client_port = ntohs(client_addr.sin_port);
    }

    std::cout << "[log] new connetion accept to " << client_ip << ":" << client_port << std::endl;
    return client_fd;
}

/* ************************************************************************** */

ssize_t SocketHandler::readFromSocket(int fd, char* buffer, size_t size)
{
    ssize_t bytes_read = read(fd, buffer, size);
    if (bytes_read == -1)
        throw std::runtime_error("[err](read()) " + std::string(strerror(errno)));

    return bytes_read;
}

ssize_t SocketHandler::writeToSocket(int fd, const char* buffer, size_t size)
{
    ssize_t bytes_written = write(fd, buffer, size);
    if (bytes_written == -1)
        throw std::runtime_error("[err](write()) " + std::string(strerror(errno)));
    
    return bytes_written;
}

/* ************************************************************************** */
/*                                                                            */
/*  acceptConnection :   0. Definition des variable                           */
/*                       1. Accepter la connexion                             */
/*                       2. Récupérer l'IP et le port du client               */
/*                       3. renvoyer le file descriptor du client             */
/*                                                                            */
/* ************************************************************************** */

