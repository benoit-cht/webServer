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

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <cerrno>
#include <cstring>
#include <iostream>

#include "webserver.hpp"

// ========================================================================== //

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

// ========================================================================== //

class Socket: public NetworkTcp{

  public :
    Socket( void );
    virtual ~Socket ( void );
    Socket(const Socket& other);
    Socket& operator=(const Socket& other);

  public :
    int   getServerFd( void ) const;
    void  setServerFd(int fd);
    void  setSocket( void );

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
    int                 _server_fd;

};

// ========================================================================== //

class Clients: public NetworkTcp{

  public:
    Clients( void );
    virtual ~Clients( void );
    Clients(const Clients& other);
    Clients& operator=(const Clients& other);

};

// ========================================================================== //

class EventsLoop: public NetworkTcp{

  private :
    EventsLoop( void );
    virtual ~EventsLoop( void );
    EventsLoop(const EventsLoop& other);
    EventsLoop& operator=(const EventsLoop& other);
};

// ========================================================================== //

#endif // !NETWORKTCP_HPP

/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */

