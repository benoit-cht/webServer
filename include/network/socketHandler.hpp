/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketHandler.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchallat <bchallat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 06:32:48 by bchallat          #+#    #+#             */
/*   Updated: 2026/03/25 06:48:26 by bchallat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HANDLER_HPP
#define SOCKET_HANDLER_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <stdexcept>
#include <cstring>
#include <map>

//#include "../webserver.hpp"

class SocketHandler {
  
  public:
    SocketHandler( void );
    virtual ~SocketHandler( void );
  
  public :
    //int     getPortFromFd(int fd) const;
  
  public:
    int     createServerSocket(int port);
    std::vector<int> createServerSockets(const std::vector<int>& ports);
    int     acceptConnection(int server_fd, std::string& ip, int& port);
    ssize_t readFromSocket(int fd, char* buffer, size_t size);
    ssize_t writeToSocket(int fd, const char* buffer, size_t size);
  
  public :  
    void    closeSocket(int fd);

  /*public :
    std::vector<int> createServerSockets(const std::vector<int>& ports);
  
  private :
    std::map<int, int> _tableFilePort;  // Associe un fd de socket serveur à son port
*/
};


#endif 
/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */

