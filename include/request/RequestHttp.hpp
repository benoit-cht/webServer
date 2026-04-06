/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHttp.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ton_utilisateur_42 <ton_email@student.42.  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 17:27:34 by ton_utilisate     #+#    #+#             */
/*   Updated: 2026/02/25 18:37:49 by bchallat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef REQUESTHTTP_HPP
# define REQUESTHTTP_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class RequestHttp {

  public:
    RequestHttp ( void );
    virtual ~RequestHttp ( void );
    RequestHttp (const RequestHttp& other);
    RequestHttp operator=(const RequestHttp& other);

  public:
    void          setBuffer(std::string str);
    std::string   getBuffer( void );
    std::string   readFile(const std::string& path);

  public:
    std::string   buffer;
};

/*
  // first ligne in request
    int         _methode ;
    std::string _link;
    double      _version;
  
  // header lingne 
    std::string _host;
    std::string _userAgent;
    std::string _contentType;
    ssize_t     _contentLength;
    std::string _connection;
*/

# endif

/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */

