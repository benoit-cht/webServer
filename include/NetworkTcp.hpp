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

  public :
    // Methode pour la gestion des socket;


  protected :
    std::vector<int*>  vectorSocketFd;

  private :
  //  std::string       _linkConfigFile;
};

// =========================== Class Socket ================================= //

class Socket: public NetworkTcp{

  public :
    Socket(std::vector<int>  vectorSocketFd);
    virtual ~Socket ( void );

  private :
    int   _socket_fd;
};

// ============================ Class Client =============================== //

class Client: public NetworkTcp{

  public:
    Client(const int& socketFd);
    virtual ~Client( void );
//    Client(const Client& other);
//    Client& operator=(const Client& other);

  public:
    ssize_t   read(char* buffer, size_t size);          // Lire les données du client
    ssize_t   write(const char* buffer, size_t size);   // Écrire des données au client
  
  protected :
    int   _socket_fd;

 };

// =============================== Class EventsLoop ========================= //

class EventsLoop: public NetworkTcp{

  public:
    void  acceptNewConnection();               // Accepter une nouvelle connexion
    void  handleClientRequest(int client_fd);  // Traiter une requête client
    void  closeClient(int client_fd);          // Fermer une connexion client

  private:
    int                        _socket_fd;     // Socket serveur
    std::vector<Client*>       _clients;       // Liste des clients connectés
    std::vector<struct pollfd> _poll_fds;      // Liste des descripteurs à surveiller

/*  public :
*    EventsLoop(Socket& soket);
*    virtual ~EventsLoop( void );
*    EventsLoop(const EventsLoop& other);
*    EventsLoop& operator=(const EventsLoop& other);
*/
};

// ========================================================================== //

#endif // !NETWORKTCP_HPP

/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */

