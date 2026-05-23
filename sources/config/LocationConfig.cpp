/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 17:23:36 by tjacquel          #+#    #+#             */
/*   Updated: 2026/05/23 18:05:00 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/LocationConfig.hpp>

LocationConfig::LocationConfig() { }

LocationConfig::~LocationConfig() { }

void	LocationConfig::printLocationConfig() const {
	std::cout << "    root=" << this->root << std::endl;
	for (size_t i = 0; i < this->allow_methods.size(); i++) {
		std::cout << "    allow_methods[" << i << "]=" << this->allow_methods[i] << std::endl;
	}
}

void	LocationConfig::loadFromBlock(const t_block& curr_block) {
	for (size_t i = 0; i < curr_block.directives.size(); i++) {
		if (curr_block.directives[i][0] == "root") {
			
		}
	}
}