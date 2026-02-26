/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketTcp.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchallat <bchallat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 18:59:46 by bchallat          #+#    #+#             */
/*   Updated: 2026/02/26 14:02:55 by bchallat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/SocketTcp.hpp"

/*=========================================================================== */

SocketTcp::SocketTcp ( void ) 
{
  try {
    setServer_fd(AF_INET, SOCK_STREAM, 0);  
    setServer_addr(AF_INET, htons(PORT), INADDR_ANY);
  }
  catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  if(bind(_server_fd, (struct sockaddr *)&_server_addr, sizeof(_server_addr)) == -1)
  {
    throw SocketTcp::ErrorBindException();
  }
  if(listen(_server_fd, MAX_CLIENTS) == -1)
  {
    throw SocketTcp::ErrorListenException();
  }

  std::cout << "Serveur démarré sur le port " << _server_addr.sin_port << std::endl;
}

SocketTcp::~SocketTcp ( void )
{ 
  close(_server_fd);
}

SocketTcp::SocketTcp (const SocketTcp& other): _server_fd(other._server_fd), _server_addr(other._server_addr){}

SocketTcp&  SocketTcp::operator=(const SocketTcp& other)
{
  if( this != &other )
  {
    _server_fd = other._server_fd;
    _server_addr = other._server_addr;
  }
  return ( *this );
}

/*=========================================================================== */

int         SocketTcp::getServer_fd ( void )const { return (_server_fd);}
sockaddr_in SocketTcp::getServer_addr ( void )const { return(_server_addr);}

/*=========================================================================== */

void SocketTcp::setServer_fd (int domain, int type, int protocol)
{
  this->_server_fd = socket(domain, type, protocol);
  if (_server_fd == -1) 
  {
    throw SocketTcp::ErrorServerFdException(); 
  }
}

void SocketTcp::setServer_addr (sa_family_t sin_family, in_port_t sin_port, in_addr_t  s_addr)
{
  _server_addr.sin_family = sin_family;
  _server_addr.sin_addr.s_addr = s_addr;  // Écouter sur toutes les interfaces
  _server_addr.sin_port = sin_port;        // Convertir le port en network byte order
}

/*=========================================================================== */

const char* SocketTcp::ErrorServerFdException::what() const throw()
{
  return( "Error: exeption to lift socket cannot be created " );
}

const char* SocketTcp::ErrorBindException::what() const throw()
{
  return ("Error: exception to lift the association failure");
}

const char* SocketTcp::ErrorListenException::what() const throw()
{
  return ("Error: exception to lift port listenting failed");
}

/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */

