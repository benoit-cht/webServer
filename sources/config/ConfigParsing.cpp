/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParsing.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 18:37:03 by tjacquel          #+#    #+#             */
/*   Updated: 2026/04/23 18:53:54 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ServerConfig.hpp"

t_block		parseTokens(const std::vector<std::string>& tokens, size_t& i) {
	t_block	block;

	block.name = tokens[i];
	i++;

	while (i < tokens.size() && tokens[i] != "{") {
		block.parameters.push_back(tokens[i]);
		i++;
	}
	if (i < tokens.size() && tokens[i] == "{") {
		i++;
	}

	while (i < tokens.size() && tokens[i] != "}") {
		if (tokens[i] == "server" || tokens[i] == "location") {
			block.embed_block.push_back(parseTokens(tokens, i));
		}
		else {
			std::vector<std::string> directive;
			while (i < tokens.size() && tokens[i] != ";") {
				directive.push_back(tokens[i]);
				i++;
			}
			block.directives.push_back(directive);
			if (i < tokens.size() && tokens[i] == ";") {
				i++;
			}
		}
	}

	if (i < tokens.size() && tokens[i] == "}") {
		i++;
	}

	return (block);
}
