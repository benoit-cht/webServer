/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchallat <bchallat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 06:30:07 by bchallat          #+#    #+#             */
/*   Updated: 2026/03/25 06:50:11 by bchallat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_MANAGER_HPP
#define CLIENT_MANAGER_HPP

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "ClientInfo.hpp"  // Inclure la définition de ClientInfo

class ClientManager {

  public:
    ClientManager( void );
    virtual ~ClientManager( void );

  public:
    void                              addClient(int fd, const std::string& ip, int port);
    void                              removeClient(int fd);
  
  public :
    ClientInfo*                       getClient(int fd) const;
    const std::vector<ClientInfo*>&   getClients() const;

  private:
    std::vector<ClientInfo*>          _clients;
    std::map<int, ClientInfo*>        _fileDescriptorClient;

};

# endif 
/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */

