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

#include "../../include/NetworkTcp.hpp"

/*=========================================================================== */

Socket::Socket ( void ) 
{
  std::cout << "[log] Call constructor socket " << std::endl;
  try {

    setSocket();
  
  }catch (const std:: exception& e){

    std::cerr << "Error: " << e.what() << std:: endl;
    close(_server_fd);
    setServerFd(-1);
  }
}

Socket::~Socket ( void )
{ 
  close(_server_fd);
  std::cout << "[log] Call destructor socket " << std::endl;
}

Socket::Socket (const Socket& other): _server_fd(other._server_fd){}

Socket&  Socket::operator=(const Socket& other)
{
  if( this != &other )
  {
    _server_fd = other._server_fd;
    //_server_addr = other._server_addr;
  }
  return ( *this );
}

/*=========================================================================== */

int         Socket::getServerFd( void )const { return (_server_fd);}

/*=========================================================================== */

void        Socket::setSocket( void )
{
  int fd = socket(AF_INET, SOCK_STREAM, 0);// [domain, type, protocol]
  setServerFd(fd);

  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;          // 
  server_addr.sin_addr.s_addr = INADDR_ANY;  // Écouter sur toutes les interfaces
  server_addr.sin_port = htons(PORT);        // Convertir le port en network byte order
                                             //
  if (getServerFd() == -1)
    throw Socket::ErrorServerFdException(); 

  if(bind(_server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    throw Socket::ErrorBindException();

  if(listen(_server_fd, MAX_CLIENTS) == -1)
    throw Socket::ErrorListenException();

  std::cout << "[log] Serveur démarré sur le port " << PORT << std::endl;
}

void      Socket::setServerFd(int port)
{
  this->_server_fd = port;
}


/*=========================================================================== */


const char* Socket::ErrorServerFdException::what() const throw()
{
  return( "exeption to lift socket cannot be created " );
}

const char* Socket::ErrorBindException::what() const throw()
{
  return ("exception to lift the association failure");
}

const char* Socket::ErrorListenException::what() const throw()
{
  return ("exception to lift port listenting failed");
}

/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */

