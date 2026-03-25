/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchallat <bchallat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 17:24:16 by bchallat          #+#    #+#             */
/*   Updated: 2026/03/25 06:24:30 by bchallat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
 * createServerSocket
 *    call fonction socket()
 *    set struct sockaddr_in
 *    call fonction bind
 *    call fonction listen
 *
 * init struct pollfd
 * set other slots to -1 value
 * 
 * start infini loop
 *  call fonction poll attendre une activiter
 *  check new conection is comming
 *      accept new conection
 *      find free slot in fd(fd in the struct pollfd)
 *      
 *    
 * 
*/

# include "../include/webserver.hpp"
//# include "../include/SocketTcp.hpp"
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <unistd.h> // pour sleep()

volatile sig_atomic_t g_should_exit = 0;

void signal_handler(int signal) {
    if (signal == SIGINT) {
        std::cout << "\nReçu SIGINT (Ctrl+C), arrêt du serveur..." << std::endl;
        g_should_exit = 1;
    }
}
//int main (int av, char *ag[])
int main()
{
  // Enregistrer le gestionnaire de signal
  std::signal(SIGINT, signal_handler);
  
  /*std::vector<int>  Fd;
  std::vector<struct pollfd> _poll_fds;      // Liste des descripteurs à surveiller

  Socket sock(Fd);
  struct pollfd server_pollfd;
  server_pollfd.fd = Fd.front();
  server_pollfd.events = POLLIN;  // Surveiller les nouvelles connexions
  _poll_fds.push_back(server_pollfd);

  while (!g_should_exit)
  {
    // 1. Attendre une activité sur les sockets
    int ret = poll(&_poll_fds[0], _poll_fds.size(), -1);  // Timeout infini
    if (ret == -1) {
      std::cout << "[ERR] poll fonc fail "<< std::endl; //perror("poll() failed");
      break;
    }

    // 2. Vérifier si une nouvelle connexion arrive
    if (_poll_fds[0].revents)
    {
      std::cout << "[log] new conection " << std::endl;
      //break ;
      //EventsLoop::acceptNewConnection();
    }

    // 3. Traiter les requêtes des clients existants
    for (size_t i = 1; i < _poll_fds.size(); i++) 
    {
      if (_poll_fds[i].fd != -1 && _poll_fds[i].revents & POLLIN)
      {
        //EventsLoop::handleClientRequest(_poll_fds[i].fd);
      }
    }
  }*/

  std::vector<int> ports;
  ports.push_back(8080);
  //ports.push_back(8081);
  //ports.push_back(8082);

  try {
      EventLoop loop(ports);  // Créer une instance de EventLoop
      loop.run();             // Lancer la boucle d'événements
  } catch (const std::exception& e) {
      std::cerr << "Erreur : " << e.what() << std::endl;
      return 1;
  }

  return 0;
}


/*

    while (true) {
        // 1. Attendre une activité sur les sockets
        int ret = poll(&_poll_fds[0], _poll_fds.size(), -1);  // Timeout infini
        if (ret == -1) {
            perror("poll() failed");
            break;
        }

        // 2. Vérifier si une nouvelle connexion arrive
        if (_poll_fds[0].revents & POLLIN) {
        EventsLoop::acceptNewConnection();
        }

        // 3. Traiter les requêtes des clients existants
        for (size_t i = 1; i < _poll_fds.size(); i++) {
            if (_poll_fds[i].fd != -1 && _poll_fds[i].revents & POLLIN) {
                handleClientRequest(_poll_fds[i].fd);
            }
        }
    }
*/
//================================================================================//

/*
 * createServerSocket
 *    call fonction socket()
 *    set struct sockaddr_in
 *    call fonction bind
 *    call fonction listen
 *
 * init struct pollfd
 * set other slots to -1 value
 * 
 * start infini loop
 *  call fonction poll attendre une activiter
 *  check new conection is comming
 *      accept new conection
 *      find free slot in fd(fd in the struct pollfd)
 *      
 *    
 * 
*/

//================================================================================//
