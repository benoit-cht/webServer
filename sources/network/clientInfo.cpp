/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clientInfo.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchallat <bchallat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 14:33:18 by bchallat          #+#    #+#             */
/*   Updated: 2026/03/23 14:34:45 by bchallat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "ClientInfo.hpp"
#include "../../include/NetworkTcp.hpp"

// Constructeur : Initialise les membres avec les valeurs passées en paramètre
ClientInfo::ClientInfo(int fd, const std::string& ip, int port)
    : _fileDescriptor(fd), _portNumber(port), _adressIp(ip) {}

// Destructeur : Vide (pas de ressources à libérer manuellement)
ClientInfo::~ClientInfo(void) {}

// Retourne le descripteur de fichier du socket client
int ClientInfo::getFileDescriptor(void) const {
    return _fileDescriptor;
}

// Retourne le numéro de port du client
int ClientInfo::getPortNumber(void) const {
    return _portNumber;
}

// Retourne l'adresse IP du client (par référence constante pour éviter la copie)
const std::string& ClientInfo::getAdressIp(void) const {
    return _adressIp;
}

/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */


