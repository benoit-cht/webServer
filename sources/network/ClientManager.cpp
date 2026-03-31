/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchallat <bchallat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 14:46:05 by bchallat          #+#    #+#             */
/*   Updated: 2026/03/31 12:26:17 by bchallat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/network/ClientManager.hpp"

ClientManager::ClientManager(void) {}

ClientManager::~ClientManager(void) 
{
    for (size_t index = 0; index < _clients.size(); index++)
        delete _clients[index];

    _clients.clear();
    _fileDescriptorClient.clear();
}

/* ************************************************************************** */

void ClientManager::addClient(int fd, const std::string& ip, int port)
{
    ClientInfo* client = new ClientInfo(fd, ip, port);
    _clients.push_back(client);
    _fileDescriptorClient[fd] = client;

    std::cout << COLOR_CYAN << "(Log Server) : add new client: fd=" << fd << ", IP=" << ip << ", Port=" << port << COLOR_RESET <<std::endl;
}

void ClientManager::removeClient(int fd)
{
    std::map<int, ClientInfo*>::iterator it = _fileDescriptorClient.find(fd);
    if (it == _fileDescriptorClient.end())
    {
        return;  // Client non trouvé, rien à faire ![TO DO lever l'execption][war]
    }

    ClientInfo* client = it->second;
    _fileDescriptorClient.erase(it);
    
    std::vector<ClientInfo*>::iterator vec_it = std::find(_clients.begin(), _clients.end(), client);
    if (vec_it != _clients.end())
    {
        _clients.erase(vec_it);
    }

    delete client;
    std::cout << COLOR_CYAN << "(Log Server) : delete client: fd=" << fd << COLOR_RESET << std::endl;
}

/* ************************************************************************** */

ClientInfo* ClientManager::getClient(int fd) const 
{
    std::map<int, ClientInfo*>::const_iterator it = _fileDescriptorClient.find(fd);
    if (it == _fileDescriptorClient.end())
    {
        return nullptr;  // Client non trouvé ![TO DO lever une exeption][war]
    }
    return it->second;
}

const std::vector<ClientInfo*>& ClientManager::getClients(void) const 
{
    return _clients;
}

/* ************************************************************************** */
/*                                                                            */
/* Constructor NONE                                                           */
/* Destructor libére la mémoire allouée des ClientInfo et clear les atribu    */
/*                                                                            */
/* Methode principal: (add, remove)ClientManager                              */
/*                                                                            */
/* AddClientManager:    cree et ajoute dans liste le clients                  */
/*                      ajout pour un accès rapide a ma map                   */
/*                                                                            */
/* DeleteClientManger:  supprimer de la map                                   */
/*                      supprimer de la liste _clients                        */
/*                      libérer la mémoire                                    */
/*                                                                            */
/* Methode get: (Client, vertor)                                              */
/*                                                                            */
/* getClient :          Recherche dans la map par fd                          */
/*                                                                            */
/* getClients :         Retourne le vecteur clients(par référence constante)  */
/*                                                                            */
/* ************************************************************************** */

