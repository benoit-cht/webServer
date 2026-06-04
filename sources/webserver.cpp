/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 17:24:16 by bchallat          #+#    #+#             */
/*   Updated: 2026/06/04 15:31:15 by tjacquel         ###   ########.fr       */
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
	std::string filename  = ag[1];

	// 1
	try {
	std::vector<ServerConfig>	virtual_servers;
	confParsingHandler(ag[1], virtual_servers);
	// 2
	// std::vector<ServerConfig>	virtual_servers = confParsingHandler(ag[1]);

	std::ifstream inFile(filename.c_str());
	if (!inFile.is_open()) {
		std::cerr << "Error: cannot open input file " << filename << std::endl;
		return (1);
	}

	std::string line;
	std::vector<std::string>liiines;

	// skip all comments
	while (std::getline(inFile, line)) {
		size_t  hashPos = line.find("#");
		if (hashPos != std::string::npos) {
		line = line.substr(0, hashPos);
		}
		liiines.push_back(line);
	}
	inFile.close();

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

	if (tokens.empty()) {
		std::cerr << "Config file error: file is empty or contains no valid tokens" << std::endl;
		return (1);
	}

	for (size_t i = 0; i < tokens.size(); i++) {
		std::cout << "token[" << i << "]=" << "`" << tokens[i] << "`" << std::endl;
	}

	int	bracket_count = 0;
	for (size_t i = 0; i < tokens.size(); i++) {

		if (tokens[i] == "{") {
			bracket_count++;
		}
		if (tokens[i] == "}") {
			bracket_count--;
		}
		if (bracket_count < 0) {
			std::cerr << "Error: too many closing brackets in config file" << std::endl;
			return (1);
		}
		}
		if (bracket_count != 0) {
			std::cerr << "Config file error: closing bracket count doest not match open bracket count" << std::endl;
			return (1);
	}

	bracket_count = 0;
	bool	in_brackets = false;
	for (size_t i = 0; i < tokens.size(); i++) {
		if (tokens[i] == "{") {
			bracket_count++;
			in_brackets = true;
		}
		if (tokens[i] == "}") {
			bracket_count--;
			if (bracket_count == 0) {
				in_brackets = false;
			}
		}
		if (in_brackets == false && tokens[i] != "server" && tokens[i] != "}") {
			std::cerr << "Config file error: only `server` token expected outside brackets block but found tokens[" << i << "] == " << tokens[i] << std::endl;
			std::cout << "in_brackets=" << in_brackets << " and bracket_count=" << bracket_count << std::endl;
			return (1);
		}

		if (i > 0 && tokens[i] == ";" && tokens[i - 1] == ";") {
			std::cerr << "Config file error: consecutive separating operator `;` found" << std::endl;
			return (1);
		}

		if (i > 0 && tokens[i] == "{" && tokens[i - 1] == "{") {
			std::cerr << "Config file error: consecutive open bracket found" << std::endl;
			return (1);
		}

		if (i > 0 && tokens[i] == "{" && tokens[i - 1] == ";") {
			std::cerr << "Config file error: unexpected `;` found before opening brackets `{`" << std::endl;
			return (1);
		}
	}

	std::cout << "tokens.back()=" << tokens.back() << std::endl;
	if (tokens.back() != "}") {
		std::cerr << "Config file error: last token is not a closing bracket" << std::endl;
		return (1);
	}

	std::vector<t_block>	block;
	size_t i = 0;
	while (i < tokens.size()) {
		if (tokens[i] == "server") {
			block.push_back(parseTokens(tokens, i));
		}
		else {
			std::cerr << "Config file error: Unexpected token found outside server block: tokens[" << i << "]=" << tokens[i] << std::endl;
			return (1);
		}
	}

	if (block.empty()) {
		std::cerr << "Config file error: no valid block constructed" << std::endl;
		return (1);
	}


	// std::vector<ServerConfig>	virtual_servers;
	for (size_t i = 0; i < block.size(); i++) {
		ServerConfig		new_server;
		new_server.loadFromBlock(block.at(i));
		virtual_servers.push_back(new_server);
	}


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
