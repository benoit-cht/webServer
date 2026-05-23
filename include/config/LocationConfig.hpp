/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 17:24:59 by tjacquel          #+#    #+#             */
/*   Updated: 2026/05/23 18:11:46 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONCONFIG_HPP
# define LOCATIONCONFIG_HPP

# include <string>
# include <vector>
# include <iostream>
// # include "ServerConfig.hpp"
# include "ConfigParsing.hpp"

class	LocationConfig {

public:
	LocationConfig();
	~LocationConfig();

	void	loadFromBlock(const t_block& curr_block);

	void	printLocationConfig() const;

private:
	std::string					root;
	std::vector<std::string>	allow_methods;
} ;

#endif