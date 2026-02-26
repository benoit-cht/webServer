/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Clients.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchallat <bchallat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 15:39:18 by bchallat          #+#    #+#             */
/*   Updated: 2026/02/26 15:41:48 by bchallat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/SocketTcp.hpp"

/* ========================================================================== */

Clients::Clients( void )
{ 
  std::cout << "Call constructor clients"<< std::endl;
}

Clients::~Clients( void )
{ 
  std::cout << "Call destructor clients"<< std::endl;
}

Clients::Clients(const Clients& other): _client_fd(other._client_fd){}

Clients& Clients::operator=(const Clients& other)
{
  if( this != &other )
  {
    
  }
  return ( *this );
}


/* ========================================================================== */

void  Clients::setClient_fd(int server_fd, struct sockaddr_in  client_addr, socklen_t client_addr_len)
{
  _client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
  if (_client_fd == -1) 
    std::cerr << "Error: failed accept new connexion" << std::endl;
  else 
    std::cout << "New connexion " << inet_ntoa(client_addr.sin_addr) << std::endl;
}

void  Clients::setArray_fd(int server_fd)
{
  int index = 0;

  _array_fd[0].fd = server_fd;
  _array_fd[0].fd = POLLIN;

  //Initialiser les autre slot a -1
  while (index <= MAX_CLIENTS + 1)
  {
    _array_fd[0].fd = -1;
    index++;
  }
}

void  Clients::setClients_addr_len()
{
  _client_addr_len = sizeof(_client_addr);
}

/* ========================================================================== */

/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */


