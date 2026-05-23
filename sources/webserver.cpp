/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 17:24:16 by bchallat          #+#    #+#             */
/*   Updated: 2026/05/23 17:28:48 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <csignal>
#include <cstdlib>
#include <iostream>

#define RUN_WITH_SERVER 0

# include "../include/webserver.hpp"

#if RUN_WITH_SERVER
volatile sig_atomic_t g_should_exit = 0;

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

int main (int av, char *ag[])
// int main()
{
	if (av != 2) {
		std::cout << "c'est pas bon kevin" << std::endl;
		return (1);
	}
	std::vector<int> ports;
#if RUN_WITH_SERVER
	std::signal(SIGINT, signal_handler);// Enregistrer le gestionnaire de signal
#endif
	std::string filename  = ag[1];

	std::ifstream inFile(filename.c_str());
	if (!inFile.is_open()) {
		std::cerr << "Error: cannot open input file " << filename << std::endl;
		return (1);
	}

	std::string line;
	std::vector<std::string>liiines;
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
	// std::cout << "directiveblocks_count=" << directiveblocks_count << std::endl;

	// std::vector<t_block>	parser;
	// t_block buffer;
	// in_brackets = false;
	// bracket_count = 0;
	// for (size_t i = 0; i < tokens.size(); i++) {
	// 	if (i + 1 < tokens.size() && tokens[i] == "server") {
	// 		buffer.context = tokens[i];
	// 		parser.push_back(buffer);
	// 	}
	// 	if (i + 2 < tokens.size() && tokens[i] == "location") {
	// 		buffer.context = tokens[i];
	// 		parser.push_back(buffer);
	// 	}
	// 	// else if (i > 0 && tokens[i] == ";" ) {
	// 	// 	size_t sc_pos = i;
	// 	// 	size_t	j = i;
	// 	// 	while (j > 0 && tokens[j] != ";" && tokens[j] != "}" && tokens[j] != "{") {
	// 	// 		j--;
	// 	// 	}
	// 	// 	j++;
	// 	// 	size_t nbegin = j;
	// 	// 	while (j < sc_pos - nbegin) {
	// 	// 		buffer.directives.push_back(tokens[j]);
	// 	// 		// parser[i].directives.push_back(tokens[j]);
	// 	// 		j++;
	// 	// 	}
	// 	// 	parser[i].directives.push_back(buffer.directives[i]);
	// 	// 	// buffer.directives
	// 	// }
	// }

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

	// std::cout << "block.size()=" << block.size() << std::endl;
	// for (size_t i = 0; i < block.size(); i++) {
	// 	std::cout << "block[" << i << "].name=" << block[i].name << std::endl;
	// 	for (size_t j = 0; j < block[i].parameters.size(); j++) {
	// 		std::cout << "block[" << i << "].parameters[" << j << "]=" << block[i].parameters[j] << std::endl;
	// 	}
	// 	for (size_t k = 0; k < block[i].directives.size(); k++) {
	// 		for (size_t ka = 0; ka < block[i].directives[k].size(); ka++) {
	// 			std::cout << "    block[" << i << "].directives[" << k << "][" << ka << "]=" << block[i].directives[k][ka] << std::endl;
	// 		}
	// 	}
	// 	for (size_t l = 0; l < block[i].embed_block.size(); l++) {
	// 		std::cout << "    block[" << i << "].embed_block[" << l << "].name=" << block[i].embed_block[l].name << std::endl;
	// 		for (size_t la = 0; la < block[i].embed_block[l].parameters.size(); la++) {
	// 			std::cout << "    block[" << i << "].embed_block[" << l << "].parameters[" << la << "]=" << block[i].embed_block[l].parameters[la] << std::endl;
	// 		}
	// 		for (size_t lb = 0; lb < block[i].embed_block[l].directives.size(); lb++) {
	// 			for (size_t lba = 0; lba < block[i].embed_block[l].directives[lb].size(); lba++) {
	// 				std::cout << "        block[" << i << "].embed_block[" << l << "].directives[" << lb << "][" << lba << "]=" << block[i].embed_block[l].directives[lb][lba] << std::endl;
	// 			}
	// 		}

	// 	}
	// }

	for (size_t i = 0; i < block.size(); i++) {
		std::stringstream	ss;
		ss << "block[" << i << "]";
		std::string			base_prefix = ss.str();

		printParserBlocks(block[i], base_prefix, 0);
		std::cout << "\n";
	}

/* 	for (size_t i = 0; i < block.size(); i++) {
		// if (block[i].directives.empty() && block[i].embed_block.empty()) {
		// 	std::cerr << "Config file error: cannot have empty directive block" << std::endl;
		// 	return (1);
		// }
		linterBlock(block[i], i);
	}

	for (size_t i = 0; i < block.size(); i++) {
		paramCheck(block[i]);
		// if (block[i].name == "server") {
		// 	if (!block[i].parameters.empty()) {
		// 		std::cerr << "Config file error: no parameter expected for server directive" << std::endl;
		// 		return (1);
		// 	}
		// }
		// if (block[i].name == "location") {
		// 	if (block[i].parameters.empty()) {
		// 		std::cerr << "Config file error: filepath parameter expected for location directive" << std::endl;
		// 		return (1);
		// 	}
		// }
	} */

	std::vector<ServerConfig>	virtual_servers;
	for (size_t i = 0; i < block.size(); i++) {
		ServerConfig		new_server;
		new_server.loadFromBlock(block.at(i));
		virtual_servers.push_back(new_server);
	}

	for (size_t i = 0; i < virtual_servers.size(); i++) {
		virtual_servers[i].printServerConfig(i);
	}



#if RUN_WITH_SERVER
  initVerctorOfPort(ports);
  try {
      EventLoop loop(ports);  // Créer une instance de EventLoop
      loop.run();
      //runEventLoop(loop);
  } catch (const std::exception& e) {
      std::cerr << COLOR_RED << "Error : " << e.what() << COLOR_RESET << std::endl;
      return EXIT_FAILURE ;
  }
  return EXIT_SUCCESS ;
#endif
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
