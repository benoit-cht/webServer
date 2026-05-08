/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 19:42:36 by lcluzan           #+#    #+#             */
/*   Updated: 2026/05/08 18:57:47 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ServerConfig.hpp"

ServerConfig::ServerConfig() : /* i(0), */ port(80), client_max_body_size(10000) {}

ServerConfig::~ServerConfig() {}

void	ServerConfig::printServerConfig(const size_t i) const {
	std::cout << "virtual_servers[" << i << "].port=" << this->port << std::endl;
}

void	checkEmptyDirBlock(const t_block& curr_block) {
	if (curr_block.directives.empty()) {
		std::cerr << "Config file error: cannot have empty directive block" << std::endl;
		exit (1);
	}
}

void	ServerConfig::serverDirCheck(const t_block& curr_block) {
	std::cout << "curr_block.directives.size()=" << curr_block.directives.size() << std::endl;
	std::cout << "curr_block.embed_block[0].directives.size()=" << curr_block.embed_block[0].directives.size() << std::endl;
	if (!curr_block.parameters.empty()) {
		std::cerr << "Config file error: no parameter expected for server directive" << std::endl;
		exit (1);
	}
	if (curr_block.directives.empty() && !curr_block.embed_block.empty()) {
		std::cerr << "Config file error: cannot have empty directive block" << std::endl;
		exit (1);
	}
	// this->i =
}

void	ServerConfig::listenDirCheckAndLoad(const std::vector<std::string>& curr_dir) {
	std::cout << "curr_dir.size()=" << curr_dir.size() << std::endl;
	if (curr_dir.size() > 2) {
		std::cerr << "Config file error: single argument expected for listen directive, found " << curr_dir.size() - 1 << " arguments." << std::endl;
		exit (1);
	}
	this->port = atoi((curr_dir[1]).c_str());
}

void	ServerConfig::loadFromBlock(const t_block& curr_block) {
	if (curr_block.name == "server") {
		serverDirCheck(curr_block);
		for (size_t i = 0; i < curr_block.directives.size(); i++) {
			if (curr_block.directives[i][0] == "listen") {
				listenDirCheckAndLoad(curr_block.directives[i]);
			}
		}


	}

}