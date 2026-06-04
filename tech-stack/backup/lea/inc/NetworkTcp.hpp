/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NetworkTcp.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcluzan <lcluzan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 14:12:23 by bchallat          #+#    #+#             */
/*   Updated: 2026/04/23 00:52:02 by lcluzan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NETWORKTCP_HPP
#define NETWORKTCP_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
//#include <poll.h>
#include <sys/epoll.h> 
#include <fcntl.h>
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
     //new methods
    void                appendData(const char* data, ssize_t size);
    bool                isHeaderComplete() const;
    const std::string&  getBuffer() const;
    void        setResponse(const std::string& response);
    ssize_t     sendData();
    bool        isResponseComplete() const;
    void        clearBuffers(); // Remplace clearBuffer()

  private:
    int                 _fileDescriptor;
    int                 _portNumber;
    std::string         _adressIp;
    std::string         _requestBuffer;
    std::string         _responseBuffer;
    size_t      _totalSent;      // Combien d'octets déjà envoyés

  void ClientInfo::clearBuffer(void) {
    _requestBuffer.clear();
}

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

class EventLoop {

  public:
    EventLoop(int port);
    virtual ~EventLoop( void );
  
  public:
    void  run();
    void  cleanup();

  private:
    int             _server_fd;
    SocketHandler   _socket_handler;
    ClientManager   _client_manager;

  private:
    int                         _server_fd;
    SocketHandler               _socket_handler;
    ClientManager               _client_manager;
    int                         _epoll_fd;
};

// ========================================================================== //

#endif // !NETWORKTCP_HPP

/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */
