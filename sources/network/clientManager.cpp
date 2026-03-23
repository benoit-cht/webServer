/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchallat <bchallat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 14:46:05 by bchallat          #+#    #+#             */
/*   Updated: 2026/03/23 17:17:16 by bchallat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// ClientManager.cpp
//#include "ClientManager.hpp"
//#include <algorithm>  // Pour std::find
#include "../../include/NetworkTcp.hpp"

ClientManager::ClientManager(void) {}

ClientManager::~ClientManager(void) 
{
    // Libérer la mémoire allouée pour chaque ClientInfo
    for (size_t i = 0; i < _clients.size(); i++) {
        delete _clients[i];
    }
    _clients.clear();
    _fileDescriptorClient.clear();
}

void ClientManager::addClient(int fd, const std::string& ip, int port)
{
    // 1. Créer un nouvel objet ClientInfo
    ClientInfo* client = new ClientInfo(fd, ip, port);

    // 2. Ajouter à la liste des clients
    _clients.push_back(client);

    // 3. Ajouter à la map pour un accès rapide par fd
    _fileDescriptorClient[fd] = client;

    std::cout << "Client ajouté : fd=" << fd << ", IP=" << ip << ", Port=" << port << std::endl;
}

void ClientManager::removeClient(int fd)
{
    // 1. Trouver le ClientInfo dans la map
    std::map<int, ClientInfo*>::iterator it = _fileDescriptorClient.find(fd);
    if (it == _fileDescriptorClient.end()) {
        return;  // Client non trouvé, rien à faire
    }

    ClientInfo* client = it->second;

    // 2. Supprimer de la map
    _fileDescriptorClient.erase(it);

    // 3. Supprimer de la liste _clients
    std::vector<ClientInfo*>::iterator vec_it = std::find(_clients.begin(), _clients.end(), client);
    if (vec_it != _clients.end()) {
        _clients.erase(vec_it);
    }

    // 4. Libérer la mémoire
    delete client;

    std::cout << "Client supprimé : fd=" << fd << std::endl;
}

ClientInfo* ClientManager::getClient(int fd) const 
{
    // Recherche dans la map par fd
    std::map<int, ClientInfo*>::const_iterator it = _fileDescriptorClient.find(fd);
    if (it == _fileDescriptorClient.end()) {
        return nullptr;  // Client non trouvé
    }
    return it->second;
}

const std::vector<ClientInfo*>& ClientManager::getClients(void) const 
{
    return _clients;  // Retourne la liste complète des clients (par référence constante)
}




/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */

