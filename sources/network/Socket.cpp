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

Socket::Socket (std::vector<int> vectorSocketFd) 
{
  struct  sockaddr_in server_addr;
  int     _socket_fd = -1;
  
  std::cout << "[log] Call constructor socket " << std::endl;

  _socket_fd = socket(AF_INET, SOCK_STREAM, 0);// [domain, type, protocol]
  if (_socket_fd == -1)
    //throw Socket::ErrorServerFdException(); 

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;          // 
  server_addr.sin_addr.s_addr = INADDR_ANY;  // Écouter sur toutes les interfaces
  server_addr.sin_port = htons(PORT);        // Convertir le port en network byte order
  
  if(bind(_socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    //throw Socket::ErrorBindException();

  if(listen(_socket_fd, MAX_CLIENTS) == -1)
    //throw Socket::ErrorListenException();

  std::cout << "[log] Serveur démarré sur le port " << PORT << std::endl;
  vectorSocketFd.push_back(_socket_fd);
}

Socket::~Socket ( void )
{ 
  close(_socket_fd);
  std::cout << "[log] Call destructor socket " << std::endl;
}


/*=========================================================================== */

/*
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
*/

/*
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
*/

/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */

