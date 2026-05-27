/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 17:23:36 by tjacquel          #+#    #+#             */
/*   Updated: 2026/05/27 15:48:31 by tjacquel         ###   ########.fr       */
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
	if (this->upload_enable)
		std::cout << "    upload_enable=on" << std::endl;
	if (this->upload_loc.size() > 0) {
		std::cout << "    upload_loc=" << this->upload_loc << std::endl;
	}

	if (this->return_route.size() > 0) {
		std::cout << "    return_route ";
		std::map<int, std::string>::const_iterator	it;
		for (it = this->return_route.begin(); it != this->return_route.end(); ++it) {
			std::cout << it->first << "=" << it->second << std::endl;
		}
	}
	if (this->error_page.size() > 0) {
		// std::cout << "    error_page ";
		std::map<int, std::string>::const_iterator it;
		for (it = this->error_page.begin(); it != this->error_page.end(); ++it) {
			std::cout << "    error_page " << it->first << "=" << it->second << std::endl;
		}
	}
}

void	LocationConfig::errorpageDirCheckAndLoad(const std::vector<std::string> error_pageDir) {
	// std::cout << "error_pageDir.size()=" << error_pageDir.size() << std::endl;
	if (error_pageDir.size() < 3) {
		std::cerr << "Config file error: at least two arguments expected for error_page directive, found " << error_pageDir.size() - 1 << std::endl;
		exit (1);
	}

	std::vector<int>	err_code;
	size_t	i = 1;
	for (; i < error_pageDir.size(); i++) {
		if (onlyDigits(error_pageDir[i])) {
			try {
				err_code.push_back(ft_stoi(error_pageDir[i]));
			}
			catch (std::exception& e) {
				std::cerr << "Config file error: " << e.what() << std::endl;
				exit (1);
			}
		}
		else {
			break ;
		}
	}

	if (i != error_pageDir.size() - 1) {
		std::cerr << "Config file error: URI location expected as unique last error_page directive argument, found " << error_pageDir.size() - i << " arguments left" << std::endl;
		exit (1);
	}

	for (size_t j = 0; j < err_code.size(); ++j) {
		// error_code min-max check
		if (err_code.at(j) < 300 || err_code.at(j) > 599) {
			std::cerr << "Config file error: value `" << err_code.at(j) << "` must be between 300 and 599 in error_page directive" << std::endl;
			exit (1);
		}
	}


	i = 0;
	for (; i < err_code.size(); i++) {
		this->error_page[err_code.at(i)] = error_pageDir.at(error_pageDir.size() - 1);

		std::string str;
		std::map<int, std::string> map;
		map[i] = str;
		map.insert(std::pair<int, std::string>(i, str));

		// error_code duplicate check (we keep the first instance like nginx)
		std::map<int, std::string>::const_iterator it;
		// we go through the already existing map container to check if the error_code value is already stored
		it = this->error_page.find(err_code.at(i));
		if (it == this->error_page.end()) {
			// if no duplicate found we store the error_code associated with its URI
			this->error_page.insert(std::pair<int, std::string>(err_code.at(i), error_pageDir.at(error_pageDir.size() - 1)));
		}
		// else we simply ignore/do not store the error_code=URI pair
	}

	// std::cout << "error_pageDir.at(error_pageDir.size() - 1)=" << error_pageDir.at(error_pageDir.size() - 1) << std::endl;



}

void	LocationConfig::returnDirCheckAndLoad(const std::vector<std::string> returnDir) {
	// std::cout << "returnDir.size()=" << returnDir.size() << std::endl;
	if (returnDir.size() < 3) {
		std::cerr << "Config file error: at least two arguments expected for return directive, found " << returnDir.size() - 1 << std::endl;
		exit (1);
	}
	if ((returnDir.size() - 1) % 2 != 0) {
		std::cerr << "Config file error: even-numbered arguments expected for return directive, found " << returnDir.size() - 1 << std::endl;
		exit (1);
	}

	std::map<int, std::string>::iterator	it;
	for (size_t i = 1; i < returnDir.size(); ++i) {
		if (i % 2 != 0) {
			// return_route.insert(std::pair<int, std::string>(atoi(returnDir[i].c_str())), returnDir.at(i + 1));
			try { return_route[ft_stoi(returnDir[i])] = returnDir.at(i + 1); }
			catch (std::exception& e) { std::cerr << "Config file error: " << e.what() << std::endl; exit (1); }
		}

	}

	// std::map<int, std::string>	new_return_route;
	// new_return_route;


}

void	LocationConfig::uploadlocDirCheckAndLoad(const std::vector<std::string> upload_locDir) {
	if (upload_locDir.size() != 2) {
		std::cerr << "Config file error: single mandatory argument expected for upload_loc directive, found " << upload_locDir.size() - 1 << std::endl;
		exit (1);
	}
	this->upload_loc = upload_locDir.at(1);

}

void	LocationConfig::uploadenbableDirCheckAndLoad(const std::vector<std::string> upload_enableDir) {
	if (upload_enableDir.size() != 2) {
		std::cerr << "Config file error: single mandatory argument expected for upload_enable directive, found " << upload_enableDir.size() - 1 << std::endl;
		exit (1);
	}

	if		(upload_enableDir.at(1) == "on")	{ this->upload_enable = true; }
	else if	(upload_enableDir.at(1) == "off")	{ this->upload_enable = false; }
	else {
		std::cerr << "Config file error: argument `" << upload_enableDir.at(1) << "` found in upload_enable directive is unsupported" << std::endl;
		exit (1);
	}
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
	// std::cout << "embed_block.name=" << embed_block.name << std::endl;
	// std::cout << "embed_block.parameters.size()=" << embed_block.parameters.size() << std::endl;
	// for (size_t i = 0; i < embed_block.parameters.size(); ++i) {
	// 	std::cout << "embed_block.parameters[" << i << "]=" << embed_block.parameters[i] << std::endl;
	// }
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
		else if (embed_block.directives[i][0] == "upload_enable") { uploadenbableDirCheckAndLoad(embed_block.directives[i]); }
		else if (embed_block.directives[i][0] == "upload_loc") { uploadlocDirCheckAndLoad(embed_block.directives[i]); }
		else if (embed_block.directives[i][0] == "return") { returnDirCheckAndLoad(embed_block.directives[i]); }
		else if (embed_block.directives[i][0] == "error_page") { errorpageDirCheckAndLoad(embed_block.directives[i]); }

		else {
			std::cerr << "Config file error: directive `" << embed_block.directives[i][0] << "` found in location context directive is unsupported" << std::endl;
			exit (1);
		}
	}

}
