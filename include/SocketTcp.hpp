/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketTcp.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchallat <bchallat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 17:42:34 by bchallat          #+#    #+#             */
/*   Updated: 2026/02/25 18:36:36 by bchallat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef SOKETTCP_HPP
# define SOCKETTCP_HPP

#include <exception>

#include "webserver.hpp"

class SocketTcp {

  public :
    SocketTcp ( void );
    virtual ~SocketTcp ( void );
    SocketTcp (const& other);
    SocketTcp operator=(const& other);

  public :
    int         getServer_fd ( void )const;
    Sockadd_int getServer_addr ( void )const;

  public :
    void setServer_fd (int domain, int type, int protocol);
    void setServer_addr (sin_family, sin_port, s_addr);

  public :
    class ErrorServerFdException : public std::exception {
    public:
      const char* what() const throw();
    };

    class ErrorBindException : public std::exception {
    public :
      const char* what() const throw();
    };

    class ErrorListenException : public exception {
    public:
      const cha* what() const throw();
    };
  
  private :
    int                 _server_fd;
    struct sockaddr_in  _server_addr;

};

# endif

/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */
