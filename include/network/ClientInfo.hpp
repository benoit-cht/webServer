/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientInfo.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcluzan <lcluzan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 06:30:01 by bchallat          #+#    #+#             */
/*   Updated: 2026/06/09 18:54:32 by lcluzan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_INFO_HPP
#define CLIENT_INFO_HPP

#include <string>

class ClientInfo {

  public:
    ClientInfo(int fd, const std::string& ip, int port);
    virtual ~ClientInfo(void);

    std::string request_buffer; // Stores incoming data
    std::string response_buffer; // Stores outgoing data
    bool        is_ready_to_send; // True when we have built the response

  public:
    int                 getFileDescriptor(void) const;
    int                 getPortNumber(void) const;
    const std::string&  getAdressIp(void) const;

  private:
    int         _fileDescriptor;
    int         _portNumber;
    std::string _adressIp;
};

#endif

/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */

