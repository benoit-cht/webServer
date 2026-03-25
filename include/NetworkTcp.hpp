/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NetworkTcp.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchallat <bchallat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 14:12:23 by bchallat          #+#    #+#             */
/*   Updated: 2026/03/25 06:17:51 by bchallat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NETWORKTCP_HPP
#define NETWORKTCP_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>
#include <vector>
#include <string>
#include <map>
#include <stdexcept>
#include <cstring>
#include <arpa/inet.h>
#include <cerrno>
#include <iostream>

#include "webserver.hpp"

// ============================== Class socketHandler ========================== //

class SocketHandler {
  
  public:
    SocketHandler( void );
    virtual ~SocketHandler( void );
  
  public :
    //int     getPortFromFd(int fd) const;
  
  public:
    int     createServerSocket(int port);
    std::vector<int> createServerSockets(const std::vector<int>& ports);
    int     acceptConnection(int server_fd, std::string& ip, int& port);
    ssize_t readFromSocket(int fd, char* buffer, size_t size);
    ssize_t writeToSocket(int fd, const char* buffer, size_t size);
  
  public :  
    void    closeSocket(int fd);

  /*public :
    std::vector<int> createServerSockets(const std::vector<int>& ports);
  
  private :
    std::map<int, int> _tableFilePort;  // Associe un fd de socket serveur à son port
*/
};

// =========================== Class ClientInfo ================================= //

class ClientInfo {
  
  public:
    ClientInfo(int fd, const std::string& ip, int port);
    virtual ~ClientInfo( void );

  public:
    int                 getFileDescriptor() const;
    int                 getPortNumber() const;
    const std::string&  getAdressIp() const;

  private:
    int                 _fileDescriptor;
    int                 _portNumber;
    std::string         _adressIp;

};

// ========================== Class ClientManager =============================== //

class ClientManager {

  public:
    ClientManager( void );
    virtual ~ClientManager( void );

  public:
    void                              addClient(int fd, const std::string& ip, int port);
    void                              removeClient(int fd);
  
  public :
    ClientInfo*                       getClient(int fd) const;
    const std::vector<ClientInfo*>&   getClients() const;

  private:
    std::vector<ClientInfo*>          _clients;
    std::map<int, ClientInfo*>        _fileDescriptorClient;

};

// =============================== Class EventsLoop ========================= //
/*
class EventLoop {

  public:
    EventLoop(int port);
    virtual ~EventLoop( void );
  
  public:
    void  run();
    void  cleanup();

  private:
    void  setupServerSocket(int port);
    void  handleClientRequest(int fd);
    void  handleNewConnection();

  private:
    int                         _server_fd;
    SocketHandler               _socket_handler;
    ClientManager               _client_manager;
    std::vector<struct pollfd>  _poll_fds;

};
*/

class EventLoop {
public:
    EventLoop(const std::vector<int>& ports);  // Prend une liste de ports à écouter
    ~EventLoop();

    void run();  // Boucle principale du serveur

private:
    std::vector<int> _server_fds;          // Liste des descripteurs de socket serveur (un par port)
    SocketHandler _socket_handler;         // Gère les opérations bas niveau sur les sockets
    ClientManager _client_manager;         // Gère la liste des clients connectés
    std::vector<struct pollfd> _poll_fds;    // Liste des descripteurs à surveiller avec poll()

    void setupServerSockets(const std::vector<int>& ports);  // Configure les sockets serveur
    void handleNewConnection(int server_fd);               // Accepte une nouvelle connexion
    void handleClientRequest(int fd);                      // Traite une requête client
    void cleanup();                                         // Nettoie les ressources (fermeture des sockets)
};

// ========================================================================== //

#endif // !NETWORKTCP_HPP

/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */
