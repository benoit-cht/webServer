/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 17:24:59 by tjacquel          #+#    #+#             */
/*   Updated: 2026/06/01 20:29:41 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONCONFIG_HPP
# define LOCATIONCONFIG_HPP

# include <string>
# include <vector>
# include <iostream>
# include <map>
# include <set>
# include <utility>
# include <cstdlib>
# include "ConfigParsing.hpp"
# include "ConfigParsingUtils.hpp"

class	LocationConfig {

public:
	LocationConfig();
	~LocationConfig();

	void	loadFromEmbedBlock(const t_block& embed_block);
	void	rootDirCheckAndLoad(const std::vector<std::string> rootDirective);
	void	methodsDirCheckAndLoad(const std::vector<std::string> curr_dir);
	void	indexDirCheckAndLoad(const std::vector<std::string> indexDir);
	void	autoindexDirCheckAndLoad(const std::vector<std::string> autoindexDir);
	void	uploadenbableDirCheckAndLoad(const std::vector<std::string> upload_enableDir);
	void	uploadlocDirCheckAndLoad(const std::vector<std::string> upload_locDir);
	void	returnDirCheckAndLoad(const std::vector<std::string> returnDir);
	void	errorpageDirCheckAndLoad(const std::vector<std::string> error_pageDir);

	void	printLocationConfig() const;

private:
	std::string					root;
	std::set<std::string>		methods;
	// USAGE: if (this->methods.find("POST") != this->methogA nd()) { // POST is allowed! [...] }
	std::string					index;
	bool						autoindex;
	bool						upload_enable;
	std::string					upload_loc;
	std::map<int, std::string>	return_route;
	std::map<int, std::string>	error_page;

} ;

#endif