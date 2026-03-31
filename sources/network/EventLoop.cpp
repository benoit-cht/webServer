/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchallat <bchallat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 06:01:32 by bchallat          #+#    #+#             */
/*   Updated: 2026/03/31 11:53:18 by bchallat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstring>  // Pour memset

#include "../../include/network/EventLoop.hpp"

EventLoop::EventLoop(const std::vector<int>& ports)
{
    setupServerSockets(ports);
}

EventLoop::~EventLoop()
{
    cleanup();
}

/* ************************************************************************** */

void  EventLoop::waitActivityOnSocket( void ) // Attendre une activité sur les sockets (timeout infini)
{
// Attendre une activité sur les sockets (timeout infini)
  int ret = poll(&_poll_fds[0], _poll_fds.size(), -1);
  if (ret == -1) 
    throw std::runtime_error("[err](poll()) " + std::string(strerror(errno)));
}

void  EventLoop::monitoredAllDescriptor( void ) //Parcourir tous les descripteurs surveillés
{
  for (size_t i = 0; i < _poll_fds.size(); i++)
  {
    if (_poll_fds[i].revents & POLLIN)
    {
      // Vérifier si c'est un socket serveur (nouvelle connexion)
      bool is_server_fd = false;

      for (size_t j = 0; j < _server_fds.size(); j++) 
      {
        if (_poll_fds[i].fd == _server_fds[j])
        {
          handleNewConnection(_poll_fds[i].fd);
          is_server_fd = true;
          break;
        }
      }

      // Sinon, c'est un client existant (données à lire)
      if (!is_server_fd)
      {
        handleClientRequest(_poll_fds[i].fd);
      }
    }
  }
}

void EventLoop::run() 
{
    std::cout << COLOR_GREEN << "Info : start event loop..." << COLOR_RESET << std::endl;
    while (true) 
    {
        // Attendre une activité sur les sockets (timeout infini)
        int ret = poll(&_poll_fds[0], _poll_fds.size(), -1);
        if (ret == -1) 
        {
            throw std::runtime_error("[err](poll()) " + std::string(strerror(errno)));
            //perror("poll() failed");
            break;
        }

        // Parcourir tous les descripteurs surveillés
        for (size_t i = 0; i < _poll_fds.size(); i++)
        {
          if (_poll_fds[i].revents & POLLIN)
          {
              // Vérifier si c'est un socket serveur (nouvelle connexion)
              bool is_server_fd = false;
                
              for (size_t j = 0; j < _server_fds.size(); j++) 
              {
                if (_poll_fds[i].fd == _server_fds[j])
                {
                  handleNewConnection(_poll_fds[i].fd);
                  is_server_fd = true;
                  break;
                }
              }

                // Sinon, c'est un client existant (données à lire)
              if (!is_server_fd)
              {
                handleClientRequest(_poll_fds[i].fd);
              }
          }
        }
    }
}

/* ************************************************************************** */

void EventLoop::handleNewConnection(int server_fd)
{
   /* struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);*/

    // Accepter la nouvelle connexion
    std::string client_ip;
    int client_port;
    int client_fd = _socket_handler.acceptConnection(server_fd, client_ip, client_port);
    if (client_fd == -1)
    {
        std::cerr << "Erreur lors de l'acceptation de la connexion." << std::endl;
        return;
    }

    // Ajouter le client à ClientManager
    _client_manager.addClient(client_fd, client_ip, client_port);

    // Ajouter le client à la liste des descripteurs surveillés par poll()
    struct pollfd client_pollfd;
    client_pollfd.fd = client_fd;
    client_pollfd.events = POLLIN;
    _poll_fds.push_back(client_pollfd);

    std::cout << "[log] new conection accept (fd=" << client_fd
              << ", IP=" << client_ip
              << ", Port=" << client_port << ")" << std::endl;
}

void EventLoop::handleClientRequest(int fd)
{
    // 1. Récupérer le client
    ClientInfo* client = _client_manager.getClient(fd);
    if (!client)
    {
        std::cerr << "Client non trouvé (fd=" << fd << ")." << std::endl;
        _socket_handler.closeSocket(fd);
        return;
    }

    // 2. Lire les données du client
    char buffer[4096];
    memset(buffer, 0, sizeof(buffer));
    ssize_t bytes_read = _socket_handler.readFromSocket(fd, buffer, sizeof(buffer) - 1);
    if (bytes_read <= 0)
    {
        // Client déconnecté ou erreur de lecture
        std::cout << "Client déconnecté (fd=" << fd << ")." << std::endl;
        _client_manager.removeClient(fd);

        // Retirer le fd de _poll_fds
        for (size_t i = 0; i < _poll_fds.size(); i++)
        {
            if (_poll_fds[i].fd == fd)
            {
                _poll_fds.erase(_poll_fds.begin() + i);
                break;
            }
        }
        return;
    }

    // 3. Afficher les données reçues (pour débogage)
    std::cout << "Données reçues de " << client->getAdressIp()
              << ":" << client->getPortNumber() << " ("
              << bytes_read << " octets) :\n" << buffer << std::endl;

    // 4. Ici, tu pourrais appeler un parseur HTTP pour traiter la requête.
    //    Pour l'instant, on se contente d'afficher les données brutes.
}

void EventLoop::setupServerSockets(const std::vector<int>& ports) 
{
    // Créer un socket serveur pour chaque port
    _server_fds = _socket_handler.createSocketWithPort(ports);

    // Ajouter chaque socket serveur à _poll_fds pour surveillance
    for (size_t i = 0; i < _server_fds.size(); i++) 
    {
        struct pollfd pfd;
        pfd.fd = _server_fds[i];
        pfd.events = POLLIN;      // Surveiller les nouvelles connexions
        _poll_fds.push_back(pfd);
    }
}

/* ************************************************************************** */

void EventLoop::cleanup()
{
    // Fermer tous les sockets serveur
    for (size_t i = 0; i < _server_fds.size(); i++) 
    {
        _socket_handler.closeSocket(_server_fds[i]);
    }

    // Fermer tous les sockets clients (via ClientManager)
    const std::vector<ClientInfo*>& clients = _client_manager.getClients();
    for (size_t i = 0; i < clients.size(); i++) 
    {
        _socket_handler.closeSocket(clients[i]->getFileDescriptor());
    }
}

/* ************************************************************************** */
/*                                                                            */
/* Constructor: Prend une liste de ports à écouter                            */
/* Destructor: nettoye les vector des attribue proprement                     */
/*                                                                            */
/* Methode public                                                             */
/*   run : boucle d'evennement                                                */
/*                                                                            */
/* Methode priver                                                             */
/*   cleanup : Nettoie les ressources (fermeture des sockets)                 */
/*   handleNewConnection : Accepte une nouvelle connexion                     */
/*   handleClientRequest: Traite une requête client                           */
/*   setupServerSockets:  Configure les sockets serveur                       */
/*                                                                            */
/* ************************************************************************** */

