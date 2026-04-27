/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcluzan <lcluzan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 06:29:31 by bchallat          #+#    #+#             */
/*   Updated: 2026/04/27 15:15:12 by lcluzan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENT_LOOP_HPP
#define EVENT_LOOP_HPP

#include <vector>
#include <poll.h>
#include <map>
#include <sys/types.h>
#include "SocketHandler.hpp"
#include "ClientManager.hpp"
#include "../request/HttpHandler.hpp"
#include "./../log/colorLog.hpp"

class EventLoop {

  public:
    EventLoop(const std::vector<int>& ports);  // Prend une liste de ports à écouter
    ~EventLoop();
  
  public:
    void  run();

  private:
    void  cleanup();                                          // Nettoie les ressources (fermeture des sockets)
    void  handleNewConnection(int server_fd);                 // Accepte une nouvelle connexion
    void  handleClientRequest(int fd);                        // Traite une requête client
    void  handleCgiResponse(int cgi_fd);
    void  setupServerSockets(const std::vector<int>& ports);  // Configure les sockets serveur

  private:
    SocketHandler               _socket_handler;              // Gère les opérations bas niveau sur les sockets
    ClientManager               _client_manager;              // Gère la liste des clients connectés
    std::vector<int>            _server_fds;                  // Liste des descripteurs de socket serveur (un par port)
    std::vector<struct pollfd>  _poll_fds;                    // Liste des descripteurs à surveiller avec poll()
    std::map<int, int>          _cgi_to_client;               // Map des FDs de CGI vers les FDs clients correspondants;
    std::map<int, pid_t>        _cgi_to_pid;                  // Map des FDs clients vers les PIDs des processus CGI correspondants;
};

# endif

/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */
