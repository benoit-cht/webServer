/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientInfo.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchallat <bchallat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 14:33:18 by bchallat          #+#    #+#             */
/*   Updated: 2026/03/25 06:45:12 by bchallat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/network/ClientInfo.hpp"

ClientInfo::ClientInfo(int fd, const std::string& ip, int port)
    : _fileDescriptor(fd), _portNumber(port), _adressIp(ip) {}

ClientInfo::~ClientInfo(void) {}

int ClientInfo::getFileDescriptor(void) const 
{
    return _fileDescriptor;
}

int ClientInfo::getPortNumber(void) const 
{
    return _portNumber;
}

const std::string& ClientInfo::getAdressIp(void) const 
{
    return _adressIp;
}

/* ************************************************************************** */
/* Constructeur : Initialise les membres avec les valeurs en paramètre        */
/* Destructeur : Vide (pas de ressources à libérer manuellement)              */
/*                                                                            */
/* Getter Metode:                                                             */
/* getFileDescriptor le descripteur de fichier du socket client               */
/* getPortNumber le numéro de port du client                                  */
/* getAdressIp l'adresse IP du client                                         */
/* (par référence constante pour éviter la copie)                             */
/* ************************************************************************** */
