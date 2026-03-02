/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchallat <bchallat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 17:39:42 by bchallat          #+#    #+#             */
/*   Updated: 2026/02/27 14:13:23 by bchallat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

#define PORT 8080        // Port d'écoute du serveur
#define MAX_CLIENTS 10   // Nombre max de clients simultanés
#define BUFFER_SIZE 4096  // Taille du buffer pour lire les requêtes

#include <sys/socket.h>  // Pour les sockets
#include <netinet/in.h>  // Pour sockaddr_in
#include <unistd.h>      // Pour close(), read(), write()
#include <poll.h>        // Pour poll()
#include <fcntl.h>       // Pour fcntl() (non-blocking)
#include <cstring>       // Pour memset, strerror
#include <iostream>      // Pour les logs
#include <arpa/inet.h>   // Pour inet_ntoa (affichage des IPs)

#include "RequestHttp.hpp"
#include "NetworkTcp.hpp"
//#include "SocketTcp.hpp"
//#include "EventsLoop.hpp"

# endif 

/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */
