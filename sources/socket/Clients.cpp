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

Clients::Clients( void ){ std::cout << "Call constructor clients"<< std::endl;}
Clients::~Clients( void ){ std::cout << "Call destructor clients"<< std::endl;}

Clients::Clients(const Clients& other): _client_fd(other._client_fd){}

Clients& Clients::operator=(const Clients& other)
{
  if( this != &other )
  {
    
  }
  return ( *this );
}

/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */


