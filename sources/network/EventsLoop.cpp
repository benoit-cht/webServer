/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventsLoop.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchallat <bchallat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 14:12:23 by bchallat          #+#    #+#             */
/*   Updated: 2026/03/17 13:22:58 by ton_utilisate    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/NetworkTcp.hpp"

// ========================================================================== //

EventsLoop::EventsLoop(Socket& socket): _socket(socket)
{
  struct pollfd server_pollfd;
  server_pollfd.fd = _socket.getServerFd();
  server_pollfd.events = POLLIN;  // Surveiller les nouvelles connexions
  _poll_fds.push_back(server_pollfd);
  std::cout << "[log] Call constructor EventsLoop " << std::endl;
}

EventsLoop::~EventsLoop( void )
{
  std::cout << "[log] Call destructor EventsLoop " << std::endl;
}

// ========================================================================== //


void EventsLoop::acceptNewConnection()
{
  struct sockaddr_in client_addr;
  socklen_t client_addr_len = sizeof(client_addr);

  // 1. Accepter la nouvelle connexion
  int client_fd = accept(_socket.getServerFd(), (struct sockaddr *)&client_addr, &client_addr_len);
  if (client_fd == -1) // to do lift exception !!!
  {
    //perror("accept() failed");
    return;
  }

  // 2. Récupérer l'adresse IP du client
  char ip_str[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &(client_addr.sin_addr), ip_str, INET_ADDRSTRLEN);
  std::string client_ip(ip_str);
  //int client_port = ntohs(client_addr.sin_port);

  // 3. Créer un nouvel objet Client
  Client* clients = new Client(_socket);
  _clients.push_back(clients);

  // 4. Ajouter le client à la liste des sockets à surveiller
  struct pollfd client_pollfd;
  client_pollfd.fd = client_fd;
  client_pollfd.events = POLLIN;  // Surveiller les données entrantes
  _poll_fds.push_back(client_pollfd);
}



void EventsLoop::handleClientRequest(int client_fd) 
{
  // 1. Trouver le client correspondant
  Client* client = NULL;
  for (size_t i = 0; i < _clients.size(); i++)
  {
    if (_socket.getServerFd() == client_fd) {
      client = _clients[i];
      break;
    }
  }

  if (!client) {
    closeClient(client_fd);
    return;
  }

  // 2. Lire la requête du client
  char buffer[4096];
  memset(buffer, 0, sizeof(buffer));
  ssize_t bytes_read = client->read(buffer, sizeof(buffer) - 1);

  if (bytes_read <= 0) {
  // Le client a fermé la connexion ou une erreur
    closeClient(client_fd);
    return;
  }

  // 3. Afficher la requête brute (pour débogage)
  //std::cout << "Requête reçue de " << client->getIpAddress() << ":" << client->getPort() << ":\n" << buffer << std::endl;

    // 4. Ici, tu pourrais appeler fonction pour parser la requête
    //    Pour l'instant, on se contente de lire les données.
}



void EventsLoop::closeClient(int client_fd)
{
  // 1. Trouver le client correspondant
  for (size_t i = 0; i < _clients.size(); i++)
  {
    if (_socket.getServerFd() == client_fd)
    {
      // 2. Fermer le socket client
      //_clients[i]->closeSocket();

      // 3. Retirer le client de la liste des sockets à surveiller
      for (size_t j = 1; j < _poll_fds.size(); j++)
      {
        if (_poll_fds[j].fd == client_fd)
        {
          _poll_fds.erase(_poll_fds.begin() + j);
          break;
        }
      }

      // 4. Supprimer le client de la liste
      delete _clients[i];
      _clients.erase(_clients.begin() + i);
      break;
    }
  }
}



// ========================================================================== //


/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */
