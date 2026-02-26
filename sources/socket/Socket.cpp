/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchallat <bchallat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 18:59:46 by bchallat          #+#    #+#             */
/*   Updated: 2026/02/26 22:00:27 by bchallat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/SocketTcp.hpp"

/*=========================================================================== */

Socket::Socket ( void ) 
{
  
  setServer_fd(AF_INET, SOCK_STREAM, 0);  
  if (getServer_fd() == -1)
    exit(EXIT_FAILURE);
  setServer_addr(AF_INET, htons(PORT), INADDR_ANY);

  if(bind(_server_fd, (struct sockaddr *)&_server_addr, sizeof(_server_addr)) == -1)
  {
    //throw Socket::ErrorBindException();
    perror("bind() failed");
    exit(EXIT_FAILURE);
  }
  if(listen(_server_fd, MAX_CLIENTS) == -1)
  {
   // throw Socket::ErrorListenException();
    perror("listen() failed");
    exit(EXIT_FAILURE);
  }

  std::cout << "Serveur démarré sur le port " << _server_addr.sin_port << std::endl;
}

Socket::~Socket ( void )
{ 
  close(_server_fd);
}

Socket::Socket (const Socket& other): _server_fd(other._server_fd), _server_addr(other._server_addr){}

Socket&  Socket::operator=(const Socket& other)
{
  if( this != &other )
  {
    _server_fd = other._server_fd;
    _server_addr = other._server_addr;
  }
  return ( *this );
}

/*=========================================================================== */

int         Socket::getServer_fd ( void )const { return (_server_fd);}
sockaddr_in Socket::getServer_addr ( void )const { return(_server_addr);}

/*=========================================================================== */

void Socket::setServer_fd (int domain, int type, int protocol)
{
  this->_server_fd = socket(domain, type, protocol);
  if (_server_fd == -1) 
  {
    //throw Socket::ErrorServerFdException(); 
  }
}

void Socket::setServer_addr (sa_family_t sin_family, in_port_t sin_port, in_addr_t  s_addr)
{
  _server_addr.sin_family = sin_family;
  _server_addr.sin_addr.s_addr = s_addr;  // Écouter sur toutes les interfaces
  _server_addr.sin_port = sin_port;        // Convertir le port en network byte order
}

/*=========================================================================== */

/*const char* Socket::ErrorServerFdException::what() const throw()
{
  return( "Error: exeption to lift socket cannot be created " );
}

const char* Socket::ErrorBindException::what() const throw()
{
  return ("Error: exception to lift the association failure");
}

const char* Socket::ErrorListenException::what() const throw()
{
  return ("Error: exception to lift port listenting failed");
}*/

/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */

