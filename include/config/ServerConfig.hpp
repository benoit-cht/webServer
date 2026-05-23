/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 19:26:05 by lcluzan           #+#    #+#             */
/*   Updated: 2026/05/23 18:09:06 by tjacquel         ###   ########.fr       */
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

// class	LocationConfig {

// public:
// 	LocationConfig();
// 	~LocationConfig();

// 	void	loadFromBlock(const t_block& curr_block);

// 	void	printLocationConfig() const;

// private:
// 	std::string					root;
// 	std::vector<std::string>	allow_methods;
// } ;

class	ServerConfig {

public:
	ServerConfig();
	~ServerConfig();

	void	loadFromBlock(const t_block& block);

	void	serverDirCheck(const t_block& curr_block);
	void	listenDirCheckAndLoad(const std::vector<std::string>& curr_dir);
	void	server_nameDirCheckAndLoad(const std::vector<std::string>& curr_dir);
	void	cmbsDirCheckAndLoad(const std::vector<std::string>& curr_dir);

	void	printServerConfig(const size_t i) const;

private:
	// int			i;
	int			port;
	std::string	server_name;
	std::string	host;
	int			client_max_body_size;
	std::map<std::string, LocationConfig>	location;

} ;

t_block		parseTokens(const std::vector<std::string>& tokens, size_t& i);
void		printParserBlocks(const t_block& curr_block, const std::string& prefix, int indent_level);
void		printParserEmbedBlocks(const std::vector<t_block>& embed_block, size_t& i);
void		linterBlock(const t_block& curr_block, size_t& i);
void		paramCheck(const t_block& curr_block);

#endif
