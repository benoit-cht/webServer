/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 17:23:36 by tjacquel          #+#    #+#             */
/*   Updated: 2026/05/25 20:57:17 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/LocationConfig.hpp>

LocationConfig::LocationConfig() : autoindex(false), upload_enable(false) { }

LocationConfig::~LocationConfig() { }

void	LocationConfig::printLocationConfig() const {
	std::cout << "    root=" << this->root << std::endl;
	if (this->allow_methods.size() > 0) {
			std::cout << "    ";
		for (size_t i = 0; i < this->allow_methods.size(); i++) {
			std::cout << "allow_methods[" << i << "]=" << this->allow_methods[i];
			if (i != this->allow_methods.size() - 1) {
				std::cout << ", ";
			}
		}
		std::cout << std::endl;
	}
	if (this->index.size() > 0)
		std::cout << "    index=" << this->index << std::endl;
	if (this->autoindex)
		std::cout << "    autoindex=on" << std::endl;
}

void	LocationConfig::autoindexDirCheckAndLoad(const std::vector<std::string> autoindexDir) {
	if (autoindexDir.size() != 2) {
		std::cerr << "Config file error: single mandatory argument expected for autoindex directive, found " << autoindexDir.size() - 1 << std::endl;
		exit (1);
	}

	if		(autoindexDir.at(1) == "on")	{ this->autoindex = true; }
	else if	(autoindexDir.at(1) == "off")	{ this->autoindex = false; }
	else {
		std::cerr << "Config file error: argument `" << autoindexDir.at(1) << "` found in autoindex directive is unsupported" << std::endl;
		exit (1);
	}

}

void	LocationConfig::indexDirCheckAndLoad(const std::vector<std::string> indexDir) {
	if (indexDir.size() != 2) {
		std::cerr << "Config file error: single mandatory argument expected for index directive, found " << indexDir.size() - 1 << std::endl;
		exit (1);
	}
	this->index = indexDir.at(1);
}

void	LocationConfig::methodsDirCheckAndLoad(const std::vector<std::string> curr_dir) {
	// std::cout << "allow_methodsDir.size()=" << curr_dir.size() << std::endl;

	if (curr_dir.size() < 2) {
		std::cerr << "Config file error: at least one argument expected for allow_methods directive, found " << curr_dir.size() - 1 << std::endl;
		exit (1);
	}

	for (size_t i = 1; i < curr_dir.size(); ++i) {
		// std::cout << "allow_methodsDir.at(" << i << ")=" << curr_dir.at(i) << std::endl;
		if (curr_dir.at(i) == "GET") {
			allow_methods.push_back("GET");
		}
		else if (curr_dir.at(i) == "POST") {
			allow_methods.push_back("POST");
		}
		else if (curr_dir.at(i) == "DELETE") {
			allow_methods.push_back("DELETE");
		}
		else if (curr_dir.at(i) == "UPLOAD") {
			allow_methods.push_back("UPLOAD");
		}
		else {
			std::cerr << "Config file error: `" << curr_dir.at(i) << "` found in allow_methods directive is unsupported" << std::endl;
			exit (1);
		}
	}
}

void	LocationConfig::rootDirCheckAndLoad(const std::vector<std::string> rootDirective) {
	// std::cout << "rootDirective.size()=" << rootDirective.size() << std::endl;
	if (rootDirective.size() != 2) {
		std::cerr << "Config file error: single mandatory argument expected for root directive, found " << rootDirective.size() - 1 << std::endl;
		exit (1);
	}
	this->root = rootDirective.at(1);
}

void	LocationConfig::loadFromEmbedBlock(const t_block& embed_block) {
	std::cout << "embed_block.name=" << embed_block.name << std::endl;
	std::cout << "embed_block.parameters.size()=" << embed_block.parameters.size() << std::endl;
	for (size_t i = 0; i < embed_block.parameters.size(); ++i) {
		std::cout << "embed_block.parameters[" << i << "]=" << embed_block.parameters[i] << std::endl;
	}
	if (embed_block.parameters.size() != 1) {
		std::cerr << "Config file error: single mandatory parameter expected for location context directive, found " << embed_block.parameters.size() << std::endl;
		exit (1);
	}

	for (size_t i = 0; i < embed_block.directives.size(); i++) {
		if (embed_block.directives[i][0] == "root")
			{ rootDirCheckAndLoad(embed_block.directives[i]); }
		else if (embed_block.directives[i][0] == "allow_methods") methodsDirCheckAndLoad(embed_block.directives[i]);
		else if (embed_block.directives[i][0] == "index") {
			indexDirCheckAndLoad(embed_block.directives[i]);
		}
		else if (embed_block.directives[i][0] == "autoindex") { autoindexDirCheckAndLoad(embed_block.directives[i]); }

	}
}
