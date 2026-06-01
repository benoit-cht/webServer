/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 19:42:36 by lcluzan           #+#    #+#             */
/*   Updated: 2026/05/29 21:37:00 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/ServerConfig.hpp>

ServerConfig::ServerConfig() : /* i(0), */ip("0.0.0.0"), port(8000), client_max_body_size(8000000) {
	this->server_name.push_back("");
}



ServerConfig::~ServerConfig() {}

void	ServerConfig::printServerConfig(const size_t i) const {
	std::cout << "\nvirtual_servers[" << i << "].ip=" << this->ip << std::endl;
	std::cout << "virtual_servers[" << i << "].port=" << this->port << std::endl;
	for (size_t j = 0; j < this->server_name.size(); ++j) {
		std::cout << "virtual_servers[" << i << "].server_name[" << j << "]=" << this->server_name[j] << std::endl;
	}
	std::cout << "virtual_servers[" << i << "].clients_max_body_size=" << this->client_max_body_size << std::endl;

	// std::cout << "virtual_servers[" << i << "].location.size()=" << this->location.size() << std::endl;
	if (this->location.size() > 0) {
		std::map<std::string, LocationConfig>::const_iterator	it;
		for (it = location.begin();  it != location.end(); ++it) {
			std::cout << "virtual_servers[" << i << "].location.KEY=" << it->first << std::endl;
			it->second.printLocationConfig();
		}
	}
}

void	checkEmptyDirBlock(const t_block& curr_block) {
	if (curr_block.directives.empty()) {
		std::cerr << "Config file error: cannot have empty directive block" << std::endl;
		exit (1);
	}
}

void	ServerConfig::serverDirCheck(const t_block& curr_block) {
	// std::cout << "curr_block.directives.size()=" << curr_block.directives.size() << std::endl;
	// std::cout << "curr_block.embed_block[0].directives.size()=" << curr_block.embed_block[0].directives.size() << std::endl;
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

void	ServerConfig::handleAddress(const std::string& addrStr) {
	std::istringstream	iss(addrStr);
	std::vector<int>	addrVec;
	std::string			addrToken;

	while (std::getline(iss, addrToken, '.')) {
		std::cout << "addrToken=" << addrToken << std::endl;
		if (addrToken.empty()) {
			std::cerr << "Config file error: IPv4 address cannot be empty in " << addrStr << std::endl;
			exit (1);
		}
		addrVec.push_back(ft_stoi(addrToken));
	}

	if (addrVec.size() > 4) {
		std::cerr << "Config file error: IPv4 address format error" << std::endl;
		exit (1);
	}

	for (size_t i = 0; i < addrVec.size(); ++i) {
		if (addrVec.at(i) < 0 || addrVec.at(i) > 255) {
			std::cerr << "Config file error: IPv4 address range must be between 0 and 255, found " << addrVec.at(i) << std::endl;
			exit (1);
		}
	}

	this->ip = addrStr;

}

void	ServerConfig::handlePort(const std::string& portStr) {
	this->port = ft_stoi(portStr);

	if (this->port )
}

void	ServerConfig::listenDirCheckAndLoad(const std::vector<std::string>& curr_dir) {
	// std::cout << "listen curr_dir.size()=" << curr_dir.size() << std::endl;
	if (curr_dir.size() > 2) {
		std::cerr << "Config file error: at least one argument expected for listen directive, found " << curr_dir.size() - 1 << " arguments." << std::endl;
		exit (1);
	}

	try {
		std::cout << curr_dir.at(1) << std::endl;
		size_t	foundDQuote = curr_dir.at(1).find(":");
		if (foundDQuote != std::string::npos) {
			std::string	addrStr = curr_dir.at(1).substr(0, foundDQuote);
			std::string	portStr = curr_dir.at(1).substr(foundDQuote+1);
			handleAddress(addrStr);
			handlePort(portStr);
		}
		else {
			size_t	foundDot = curr_dir.at(1).find(".");
			if (foundDot != std::string::npos) {
				handleAddress(curr_dir.at(1));
			}
			else {
				handlePort(curr_dir.at(1));
			}
		}
		// this->port = ft_stoi(curr_dir[1]);
	}
	catch (std::exception& e) {
		std::cerr << "Config file error: " << e.what() << std::endl;
		exit (1);
	}
	// this->port = atoi((curr_dir[1]).c_str());
}

void	ServerConfig::server_nameDirCheckAndLoad(const std::vector<std::string>& curr_dir) {
	// std::cout << "server_name curr_dir.size()=" << curr_dir.size() << std::endl;
	if (curr_dir.size() != 2) {
		std::cerr << "Config file error: at least one argument expected for server_name directive, found " << curr_dir.size() - 1 << " argument(s)." << std::endl;
		exit(1);
	}
	if (this->server_name[0] != "") {
		std::cout << "Config file error: multiples server_name directives found in server block directive" << std::endl;
		exit(1);
	}
	else {
		for (size_t i = 1; i < curr_dir.size(); ++i) {
			this->server_name.push_back(curr_dir[i]);
		}
		// this->server_name[1] = curr_dir[1];
	}
}

void	ServerConfig::cmbsDirCheckAndLoad(const std::vector<std::string>& curr_dir) {
	if (curr_dir.size() != 2) {
		std::cerr << "Config file error: single mandatory argument expected for client_max_body_size directive, found " << curr_dir.size() - 1 << " argument(s)." << std::endl;
		exit(1);
	}
	// this->client_max_body_size = atoi((curr_dir[1]).c_str());
	try { this->client_max_body_size = ft_stoi(curr_dir.at(1)); }
	catch (std::exception& e) { std::cerr << "Config file error: " << e.what() << std::endl; exit (1); }
}

void	ServerConfig::loadFromBlock(const t_block& curr_block) {
	if (curr_block.name != "server") {
		std::cerr << "Config file error: expected server directive block, found " << curr_block.name << std::endl;
		exit(1);

	}
	serverDirCheck(curr_block);
	for (size_t i = 0; i < curr_block.directives.size(); i++) {
		if (curr_block.directives[i][0] == "listen") {
			listenDirCheckAndLoad(curr_block.directives[i]);
		}
		else if (curr_block.directives[i][0] == "server_name") {
			server_nameDirCheckAndLoad(curr_block.directives[i]);
		}
		else if (curr_block.directives[i][0] == "client_max_body_size") {
			cmbsDirCheckAndLoad(curr_block.directives[i]);
		}

	}

	for (size_t i = 0; i < curr_block.embed_block.size(); i++) {

		if (curr_block.embed_block[i].name == "location") {
			LocationConfig	new_loc;
			new_loc.loadFromEmbedBlock(curr_block.embed_block[i]);

			this->location[curr_block.embed_block[i].parameters[0]] = new_loc;
		}

		else {
			std::cerr << "Config file error: unknown embedded block `" << curr_block.embed_block[i].name << "`" << std::endl;
			exit (1);
		}
	}

}