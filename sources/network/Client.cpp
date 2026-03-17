/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchallat <bchallat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 15:39:18 by bchallat          #+#    #+#             */
/*   Updated: 2026/02/26 15:41:48 by bchallat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/NetworkTcp.hpp"

/* ========================================================================== */

Client::Client(Socket& socket) : _socket(socket)
{ 
  std::cout << "[log] Call constructor clients with socket"<< std::endl;
}

Client::~Client( void )
{ 
  std::cout << "[log] Call destructor clients"<< std::endl;
}

/* ========================================================================== */

ssize_t Client::read(char* buffer, size_t size)
{
  return ::read(_socket.getServerFd(), buffer, size);
}

ssize_t Client::write(const char* buffer, size_t size)
{
  return ::write(_socket.getServerFd(), buffer, size);
}

/* ========================================================================== */

/*Client::Client(const Client& other): _client_fd(other._client_fd){}

Client& Client::operator=(const Client& other)
{
  if( this != &other )
  {

  }
  return ( *this );
}*/


/* ========================================================================== */

/*void  Client::setClient_fd(int server_fd, struct sockaddr_in  client_addr, socklen_t client_addr_len)
{
  _client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
  if (_client_fd == -1) 
    std::cerr << "Error: failed accept new connexion" << std::endl;
  else 
    std::cout << "New connexion " << inet_ntoa(client_addr.sin_addr) << std::endl;
}

void  Client::setInitArray_fd(int server_fd)
{
  int index = 1;

  _array_fd[0].fd = server_fd;
  _array_fd[0].events = POLLIN;

  //Initialiser les autre slot a -1
  while (index <= MAX_CLIENTS)
  {
    _array_fd[index].fd = -1;
    index++;
  }
}
*/

/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */


