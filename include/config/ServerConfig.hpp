/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 19:26:05 by lcluzan           #+#    #+#             */
/*   Updated: 2026/06/04 16:37:31 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include <map>
# include <fstream>
# include <iostream>
# include <sstream>
# include <string>
# include <vector>
# include <cstdlib>
# include "LocationConfig.hpp"
# include "ConfigParsing.hpp"

# define CLIENT_MAX_BODY_SIZE_LIMIT 2147483647

// class	LocationConfig {

// public:
// 	LocationConfig();
// 	~LocationConfig();

// 	void	loadFromBlock(const t_block& curr_block);

// 	void	printLocationConfig() const;

// private:
// 	std::string					root;
// 	std::vector<std::string>	methods;
// } ;

class	ServerConfig {

public:
	ServerConfig();
	~ServerConfig();

	void	loadFromBlock(const t_block& block);

	void	serverDirCheck(const t_block& curr_block);
	void	handleAddress(const std::string& addrStr);
	void	handlePort(const std::string& portStr);
	void	listenDirCheckAndLoad(const std::vector<std::string>& curr_dir);
	void	server_nameDirCheckAndLoad(const std::vector<std::string>& curr_dir);
	void	cmbsDirCheckAndLoad(const std::vector<std::string>& curr_dir);

	void	printServerConfig(const size_t i) const;

private:
	// int									i;
	std::string								host;
	int										port;
	std::vector<std::string>				server_name;
	int										client_max_body_size;
	std::map<std::string, LocationConfig>	location;

} ;

#endif
