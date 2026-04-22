/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 17:24:16 by bchallat          #+#    #+#             */
/*   Updated: 2026/04/21 20:07:15 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <csignal>
#include <cstdlib>
#include <iostream>

# include "../include/webserver.hpp"

// volatile sig_atomic_t g_should_exit = 0;

// static void signal_handler(int signal)
// {
//   if (signal == SIGINT)
//   {
//     std::cout << COLOR_YELLOW << " \nWarning : Reçu SIGINT (Ctrl+C), arrêt du serveur... " << COLOR_RESET << std::endl;
//     g_should_exit = 1;
//   }
// }

// static void initVerctorOfPort(std::vector<int>& ports)
// {
//   ports.push_back(8080);
//   //ports.push_back(8081);
//   //ports.push_back(8082);
// }

/*void runEventLoop(EventLoop loop)
{
  while(true)
  {
    loop.waitActivityOnSocket();
    loop.monitoredAllDescriptor();
  }
}*/

int main (int av, char *ag[])
// int main()
{
  if (av != 2) {
    std::cout << "c'est pas bon kevin" << std::endl;
    return (1);
  }
  // std::vector<int> ports;
  // std::signal(SIGINT, signal_handler);// Enregistrer le gestionnaire de signal
  std::string filename  = ag[1];

  std::ifstream inFile(filename.c_str());
  if (!inFile.is_open()) {
    std::cerr << "Error: cannot open input file " << filename << std::endl;
    return (1);
  }

  std::string line;
  std::vector<std::string>liiines;
  while (std::getline(inFile, line)) {
    size_t  sharpPos = line.find("#");
    if (sharpPos != std::string::npos) {
      line = line.substr(0, sharpPos);
    }
    liiines.push_back(line);
  }

  std::vector<std::string> tokens;
  for (size_t i = 0; i < liiines.size(); i++) {
	size_t j = 0;
	while (j < liiines[i].size()) {
		while (j < liiines[i].size() && isspace(liiines[i][j])) { // skip first whitespace
			j++;
		}

		if (j >= liiines[i].size()) {
			break ;
		}

		if (liiines[i][j] == '{' || liiines[i][j] == '}' || liiines[i][j] == ';') {
			tokens.push_back(liiines[i].substr(j, 1));
			j++;
			continue ;
		}

		size_t	nbegin = j;
		while (j < liiines[i].size() && !isspace(liiines[i][j]) && liiines[i][j] != '{' && liiines[i][j] != '}' && liiines[i][j] != ';') {
			j++;
		}
		tokens.push_back(liiines[i].substr(nbegin, j - nbegin));


	}

    std::cout << liiines[i] << std::endl;
  }

  for (size_t i = 0; i < tokens.size(); i++) {
	std::cout << "`" << tokens[i] << "`" << std::endl;
  }



  // std::vector<serverConfig> server;
  // server[0].port = 8080;
  // server[1].port = 9200;


  // std::map<std::string, std::string> map;
  // map["port"] = /* ... */ "test";
  // map["server_name"] = /* ...*/ "test2";




  // initVerctorOfPort(ports);
  // try {
  //     EventLoop loop(ports);  // Créer une instance de EventLoop
  //     loop.run();
  //     //runEventLoop(loop);
  // } catch (const std::exception& e) {
  //     std::cerr << COLOR_RED << "Error : " << e.what() << COLOR_RESET << std::endl;
  //     return EXIT_FAILURE ;
  // }
  // return EXIT_SUCCESS ;

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
