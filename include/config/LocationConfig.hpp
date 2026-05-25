/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 17:24:59 by tjacquel          #+#    #+#             */
/*   Updated: 2026/05/25 20:49:53 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONCONFIG_HPP
# define LOCATIONCONFIG_HPP

# include <string>
# include <vector>
# include <iostream>
# include <map>
# include <cstdlib>
# include "ConfigParsing.hpp"

class	LocationConfig {

public:
	LocationConfig();
	~LocationConfig();

	void	loadFromEmbedBlock(const t_block& embed_block);
	void	rootDirCheckAndLoad(const std::vector<std::string> rootDirective);
	void	methodsDirCheckAndLoad(const std::vector<std::string> curr_dir);
	void	indexDirCheckAndLoad(const std::vector<std::string> indexDir);
	void	autoindexDirCheckAndLoad(const std::vector<std::string> autoindexDir);


	void	printLocationConfig() const;

private:
	std::string					root;
	std::vector<std::string>	allow_methods;
	std::string					index;
	bool						autoindex;
	bool						upload_enable;
	std::string					upload_loc;
	std::map<int, std::string>	return_route;
	std::map<int, std::string>	error_page;

} ;

#endif