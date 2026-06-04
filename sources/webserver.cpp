/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 17:24:16 by bchallat          #+#    #+#             */
/*   Updated: 2026/06/04 17:43:02 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <csignal>
#include <cstdlib>
#include <iostream>

# include "../include/webserver.hpp"

#if RUN_WITH_SERVER
volatile sig_atomic_t g_should_exit = 1;

static void signal_handler(int signal)
{
  if (signal == SIGINT)
  {
    std::cout << COLOR_YELLOW << " \nWarning : Reçu SIGINT (Ctrl+C), arrêt du serveur... " << COLOR_RESET << std::endl;
    g_should_exit = 1;
  }
}

static void initVerctorOfPort(std::vector<int>& ports)
{
  ports.push_back(8080);
  //ports.push_back(8081);
  //ports.push_back(8082);
}

/* void runEventLoop(EventLoop loop)
{
  while(true)
  {
    loop.waitActivityOnSocket();
    loop.monitoredAllDescriptor();
  }
} */
#endif

int main (int av, char *ag[]) {
	if (av != 2) {
		std::cout << "Usage: ./webserv [configuration file]" << std::endl;
		return (EXIT_FAILURE);
	}
	std::vector<int> ports;
#if RUN_WITH_SERVER
	std::signal(SIGINT, signal_handler);// Enregistrer le gestionnaire de signal
#endif

	try {

	std::vector<ServerConfig>	virtual_servers;
	confParsingHandler(ag[1], virtual_servers);

#if RUN_WITH_SERVER
  initVerctorOfPort(ports);
      EventLoop loop(ports);  // Créer une instance de EventLoop
      loop.run();
      //runEventLoop(loop);
#endif

  } catch (const std::exception& e) {
      std::cerr << COLOR_RED << "Error: " << e.what() << COLOR_RESET << std::endl;
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
