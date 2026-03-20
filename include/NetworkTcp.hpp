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

// ============================== Class NetworkTcp ========================== //

class SocketHandler {
  
  public:
    SocketHandler( void );
    virtual ~SocketHandler( void );
  
  public:
    int createServerSocket(int port);
    int acceptConnection(int server_fd, std::string& ip, int& port);
  
    ssize_t readFromSocket(int fd, char* buffer, size_t size);
    ssize_t writeToSocket(int fd, const char* buffer, size_t size);
    
    void closeSocket(int fd);

};

// =========================== Class Socket ================================= //

class ClientInfo {
  
  public:
    ClientInfo(int fd, const std::string& ip, int port);
    virtual ~ClientInfo( void );

  public:
    int                 getFd() const;
    const std::string&  getIp() const;
    int                 getPort() const;

  private:
    int           _fd;
    std::string   _ip;
    int           _port;

};

// ============================ Class Client =============================== //

class ClientManager {

  public:
    ClientManager( void );
    virtual ~ClientManager( void );

  public:
    void                              addClient(int fd, const std::string& ip, int port);
    void                              removeClient(int fd);
    ClientInfo*                       getClient(int fd) const;
    const std::vector<ClientInfo*>&   getClients() const;

  private:
    std::vector<ClientInfo*>    _clients;
    std::map<int, ClientInfo*>  _fd_to_client;

};

// =============================== Class EventsLoop ========================= //

class EventsLoop {

  public:
    EventLoop(int port);
    virtual ~EventLoop( void );
  
  public:
    void run();

  private:
    void  setupServerSocket(int port);
    void  handleNewConnection();
    void  handleClientRequest(int fd);
    void  cleanup();

  private:
    int                         _server_fd;
    SocketHandler               _socket_handler;
    ClientManager               _client_manager;
    std::vector<struct pollfd>  _poll_fds;


};

// ========================================================================== //

#endif // !NETWORKTCP_HPP

/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */
