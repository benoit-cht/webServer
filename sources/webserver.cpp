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

#include <csignal>
#include <cstdlib>
#include <iostream>

# include "../include/webserver.hpp"

volatile sig_atomic_t g_should_exit = 0;

static void signal_handler(int signal)
{
    if (signal == SIGINT) 
    {
        std::cout << " \n [log] Reçu SIGINT (Ctrl+C), arrêt du serveur... " << std::endl;
        g_should_exit = 1;
    }
}

static void initVerctorOfPort(std::vector<int>& ports)
{
  ports.push_back(8080);
  //ports.push_back(8081);
  //ports.push_back(8082);
}

//int main (int av, char *ag[])
int main()
{
  std::vector<int> ports;
  std::signal(SIGINT, signal_handler);// Enregistrer le gestionnaire de signal

  initVerctorOfPort(ports);
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
