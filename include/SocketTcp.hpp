/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketTcp.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchallat <bchallat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 17:42:34 by bchallat          #+#    #+#             */
/*   Updated: 2026/02/26 15:42:01 by bchallat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef SOCKETTCP_HPP
# define SOCKETTCP_HPP

#include <exception>

#include "webserver.hpp"

//================================== SOCKET ===============================

class Socket {

  public :
    Socket ( void );
    virtual ~Socket ( void );
    Socket (const Socket& other);
    Socket& operator=(const Socket& other);

  public :
    int         getServer_fd ( void )const;
    sockaddr_in getServer_addr ( void )const;

  public :
    void  setServer_fd(int domain, int type, int protocol);
    void  setServer_addr(sa_family_t sin_family, in_port_t sin_port, in_addr_t  s_addr);
  
  private :
    int                 _server_fd;
    struct sockaddr_in  _server_addr;

};
/*
  public :
    class ErrorServerFdException : public std::exception {
    public:
      const char* what() const throw();
    };

    class ErrorBindException : public std::exception {
    public :
      const char* what() const throw();
    };

    class ErrorListenException : public std::exception {
    public:
      const char* what() const throw();
    };
*/
//================================== CLIENTS ===============================

class Clients: public Socket {

  public:
    Clients( void );
    virtual ~Clients( void );
    Clients(const Clients& other);
    Clients& operator=(const Clients& other);

  public:
    void  setClient_fd(int server_fd, struct sockaddr_in  client_addr, socklen_t client_addr_len);
    void  setArray_fd(int server_fd);
    void  setClients_addr_len( void );

  private :
    int                 _client_fd;
    struct pollfd       _array_fd[MAX_CLIENTS + 1];
    struct sockaddr_in  _client_addr;
    socklen_t           _client_addr_len;
};

//================================== LOOP ================================

class EventsLoop: public Clients {

  private :
    EventsLoop( void );
    virtual ~EventsLoop( void );
    EventsLoop(const EventsLoop& other);
    EventsLoop& operator=(const EventsLoop& other);
};

# endif

/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */
