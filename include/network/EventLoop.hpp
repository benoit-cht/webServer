/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcluzan <lcluzan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 06:29:31 by bchallat          #+#    #+#             */
/*   Updated: 2026/05/23 17:56:04 by lcluzan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENT_LOOP_HPP
#define EVENT_LOOP_HPP

#include <vector>
#include <poll.h>
#include <map>
#include <sys/types.h>
#include <string>
#include "SocketHandler.hpp"
#include "ClientManager.hpp"
#include "../request/HttpHandler.hpp"
#include "./../log/colorLog.hpp"

struct CgiContext {
    int     client_fd;       // Le client qui a fait la requête
    int     pipe_in;         // FD pour écrire le body de la requête (POST)
    int     pipe_out;        // FD pour lire la réponse du CGI
    pid_t   pid;             // Le PID du script PHP

    std::string request_body;    // Body à envoyer
    size_t      bytes_written;   // Suivi de l'écriture
    std::string response_buffer; // Stockage de la réponse
};

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
    void  setupServerSockets(const std::vector<int>& ports);  // Configure les sockets serveur

  private:
    SocketHandler               _socket_handler;              // Gère les opérations bas niveau sur les sockets
    ClientManager               _client_manager;              // Gère la liste des clients connectés
    std::vector<int>            _server_fds;                  // Liste des descripteurs de socket serveur (un par port)
    std::vector<struct pollfd>  _poll_fds;                    // Liste des descripteurs à surveiller avec poll()
    std::map<int, CgiContext*>  _cgi_contexts;
};

# endif

/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */
