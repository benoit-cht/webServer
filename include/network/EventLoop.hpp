/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchallat <bchallat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 06:29:31 by bchallat          #+#    #+#             */
/*   Updated: 2026/03/25 06:46:02 by bchallat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENT_LOOP_HPP
#define EVENT_LOOP_HPP

#include <vector>
#include <poll.h>

#include "SocketHandler.hpp"
#include "ClientManager.hpp"

class EventLoop {

  public:
    EventLoop(const std::vector<int>& ports);  // Prend une liste de ports à écouter
    ~EventLoop();
  
  public:
    void  run();
/* 
* Attendre une activité sur les sockets (timeout infini)
* Parcourir tous les descripteurs surveillés
* Vérifier si c'est un socket serveur (nouvelle connexion)
* Sinon, c'est un client existant (données à lire)
*/

  private:
    void  cleanup();                                          // Nettoie les ressources (fermeture des sockets)
    void  handleNewConnection(int server_fd);                 // Accepte une nouvelle connexion
    void  handleClientRequest(int fd);                        // Traite une requête client
    void  setupServerSockets(const std::vector<int>& ports);  // Configure les sockets serveur

  private:
    SocketHandler               _socket_handler;              // Gère les opérations bas niveau sur les sockets
    ClientManager               _client_manager;              // Gère la liste des clients connectés
    std::vector<int>            _server_fds;                  // Liste des descripteurs de socket serveur (un par port)
    std::vector<struct pollfd>  _poll_fds;                    // Liste des descripteurs à surveiller avec poll()
};

# endif

/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */
