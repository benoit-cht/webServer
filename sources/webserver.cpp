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

void signal_handler(int signal)
{
    if (signal == SIGINT) 
    {
        std::cout << " \n [log] Reçu SIGINT (Ctrl+C), arrêt du serveur... " << std::endl;
        g_should_exit = 1;
    }
}
//int main (int av, char *ag[])
int main()
{
  std::signal(SIGINT, signal_handler);// Enregistrer le gestionnaire de signal

  
  std::vector<int> ports;

  ports.push_back(8080);
  //ports.push_back(8081);
  //ports.push_back(8082);

  try {
      EventLoop loop(ports);  // Créer une instance de EventLoop
      loop.run();             // Lancer la boucle d'événements
  } catch (const std::exception& e) {
      std::cerr << "Erreur : " << e.what() << std::endl;
      return EXIT_FAILURE ;
  }

  return EXIT_SUCCESS ;
}

/* 
while (true)
{

  Attendre une activité sur les sockets (timeout infini)
  
  Parcourir tous les descripteurs surveillés
  
  Vérifier si c'est un socket serveur (nouvelle connexion)
    Sinon, c'est un client existant (données à lire)
}
*/
