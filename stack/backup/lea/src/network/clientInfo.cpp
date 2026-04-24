/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clientInfo.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcluzan <lcluzan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 14:33:18 by bchallat          #+#    #+#             */
/*   Updated: 2026/04/23 00:52:27 by lcluzan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "ClientInfo.hpp"
#include "../../include/NetworkTcp.hpp"

// // Constructeur : Initialise les membres avec les valeurs passées en paramètre
// ClientInfo::ClientInfo(int fd, const std::string& ip, int port)
//     : _fileDescriptor(fd), _portNumber(port), _adressIp(ip) {}

// // Destructeur : Vide (pas de ressources à libérer manuellement)
// ClientInfo::~ClientInfo(void) {}

// // Retourne le descripteur de fichier du socket client
// int ClientInfo::getFileDescriptor(void) const {
//     return _fileDescriptor;
// }

// // Retourne le numéro de port du client
// int ClientInfo::getPortNumber(void) const {
//     return _portNumber;
// }

// // Retourne l'adresse IP du client (par référence constante pour éviter la copie)
// const std::string& ClientInfo::getAdressIp(void) const {
//     return _adressIp;
// }


// Ajoute les nouvelles données lues à la suite du buffer existant
void ClientInfo::appendData(const char* data, ssize_t size) {
    _requestBuffer.append(data, size);
}

// Vérifie si la fin des headers HTTP ("\r\n\r\n") a été reçue
bool ClientInfo::isHeaderComplete() const {
    return _requestBuffer.find("\r\n\r\n") != std::string::npos;
}

// Retourne une référence constante vers le buffer
const std::string& ClientInfo::getBuffer() const {
    return _requestBuffer;
}

void ClientInfo::clearBuffer(void) {
    _requestBuffer.clear();
}

// Prépare le client à envoyer une nouvelle réponse
void ClientInfo::setResponse(const std::string& response) {
    _responseBuffer = response;
    _totalSent = 0;
}

// Tente d'envoyer le reste du buffer
ssize_t ClientInfo::sendData() {
    if (isResponseComplete()) return 0;

    // On écrit à partir de là où on s'était arrêté
    const char* data_ptr = _responseBuffer.c_str() + _totalSent;
    size_t data_left = _responseBuffer.size() - _totalSent;

    ssize_t bytes = write(_fileDescriptor, data_ptr, data_left);
    
    if (bytes > 0) {
        _totalSent += bytes; // On met à jour l'avancement
    }
    return bytes;
}

bool ClientInfo::isResponseComplete() const {
    return _totalSent >= _responseBuffer.size();
}

void ClientInfo::clearBuffers() {
    _requestBuffer.clear();
    _responseBuffer.clear();
    _totalSent = 0;
}
/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */


