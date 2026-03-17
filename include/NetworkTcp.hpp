/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NetworkTcp.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchallat <bchallat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 14:12:23 by bchallat          #+#    #+#             */
/*   Updated: 2026/03/17 13:28:50 by ton_utilisate    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NETWORKTCP_HPP
#define NETWORKTCP_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <vector>

#include "webserver.hpp"

// ============================== Class NetworkTcp ========================== //

class NetworkTcp {

  public :
    NetworkTcp( void );
    virtual ~NetworkTcp( void );
//    NetworkTcp(const NetworkTcp& other);
//    NetworkTcp& operator=(const NetworkTcp& other);

  protected :
    int port;
    int type;
    int protocol;


  private :
  //  std::string       _linkConfigFile;
  //  std::vector<int>  _arraySocket

};

// =========================== Class Socket ================================= //

class Socket: public NetworkTcp{

  public :
    Socket( void );
    virtual ~Socket ( void );
    Socket(const Socket& other);
    Socket& operator=(const Socket& other);

  public :
    int     getServerFd( void ) const;
  
  public :
    void    setServerFd(int fd);
    void    setSocket( void );

  public :
    class ErrorServerFdException : public std::exception {
      const char* what() const throw();
    };
    class ErrorBindException : public std::exception {
      const char* what() const throw();
    };
    class ErrorListenException : public std::exception {
      const char* what() const throw();
    };

  private :
    int   _server_fd;

};

// ============================ Class Client =============================== //

class Client: public NetworkTcp{

  public:
    Client(Socket& socket);
    virtual ~Client( void );
//    Client(const Client& other);
//    Client& operator=(const Client& other);

  public:
    ssize_t   read(char* buffer, size_t size);          // Lire les données du client
    ssize_t   write(const char* buffer, size_t size);   // Écrire des données au client

  private:
    Socket& _socket;
};

// =============================== Class EventsLoop ========================= //

class EventsLoop: public NetworkTcp{

  public :
    EventsLoop(Socket& soket);
    virtual ~EventsLoop( void );
//    EventsLoop(const EventsLoop& other);
//    EventsLoop& operator=(const EventsLoop& other);

  public:
    void  acceptNewConnection();               // Accepter une nouvelle connexion
    void  handleClientRequest(int client_fd);  // Traiter une requête client
    void  closeClient(int client_fd);          // Fermer une connexion client

  private:
    Socket& _socket;                           // Socket serveur
    std::vector<struct pollfd> _poll_fds;     // Liste des descripteurs à surveiller
    std::vector<Client*> _clients;            // Liste des clients connectés

};

// ========================================================================== //

#endif // !NETWORKTCP_HPP

/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */

