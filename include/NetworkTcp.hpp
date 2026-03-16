/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NetworkTcp.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchallat <bchallat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 14:12:23 by bchallat          #+#    #+#             */
/*   Updated: 2026/02/27 14:15:10 by bchallat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NETWORKTCP_HPP
#define NETWORKTCP_HPP

#include <sys/socket.h>   // Pour les sockets
#include <netinet/in.h>   // Pour sockaddr_in
#include <unistd.h>       // Pour close(), read(), write()
#include <poll.h>         // Pour poll()
#include <fcntl.h>        // Pour fcntl() (non-blocking)
#include <string>         // Pour memset, strerror
#include <iostream>       // Pour les logs
#include <arpa/inet.h>    // Pour inet_ntoa (affichage des IPs)

#include <cerrno>
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

// ============================ Class Clients =============================== //

class Clients: public NetworkTcp{

  public:
    Clients(Socket& socket);
    virtual ~Clients( void );
//    Clients(const Clients& other);
//    Clients& operator=(const Clients& other);

  public:
    ssize_t   read(char* buffer, size_t size);          // Lire les données du client
    ssize_t   write(const char* buffer, size_t size);   // Écrire des données au client

  private:
    Socket& _socket;
};

// =============================== Class EventsLoop ========================= //

class EventsLoop: public NetworkTcp{

  private :
    EventsLoop(Socket& soket);
    virtual ~EventsLoop( void );
//    EventsLoop(const EventsLoop& other);
//    EventsLoop& operator=(const EventsLoop& other);

  private:
    void  acceptNewConnection();               // Accepter une nouvelle connexion
    void  handleClientRequest(int client_fd);  // Traiter une requête client
    void  closeClient(int client_fd);          // Fermer une connexion client

  private:
    Socket& _socket;                           // Socket serveur
    std::vector<struct pollfd> _poll_fds;     // Liste des descripteurs à surveiller
    std::vector<Clients*> _clients;            // Liste des clients connectés

};

// ========================================================================== //

#endif // !NETWORKTCP_HPP

/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */

