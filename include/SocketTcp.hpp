/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketTcp.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchallat <bchallat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 17:42:34 by bchallat          #+#    #+#             */
/*   Updated: 2026/02/26 13:41:39 by bchallat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef SOCKETTCP_HPP
# define SOCKETTCP_HPP

#include <exception>

#include "webserver.hpp"

class SocketTcp {

  public :
    SocketTcp ( void );
    virtual ~SocketTcp ( void );
    SocketTcp (const SocketTcp& other);
    SocketTcp& operator=(const SocketTcp& other);

  public :
    int         getServer_fd ( void )const;
    sockaddr_in getServer_addr ( void )const;

  public :
    void setServer_fd (int domain, int type, int protocol);
    void setServer_addr (sa_family_t sin_family, in_port_t sin_port, in_addr_t  s_addr);

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
  
  private :
    int                 _server_fd;
    struct sockaddr_in  _server_addr;

};

# endif

/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */
