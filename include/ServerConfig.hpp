/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcluzan <lcluzan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 19:26:05 by lcluzan           #+#    #+#             */
/*   Updated: 2026/04/22 17:58:08 by lcluzan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include <map>
# include <fstream>
# include <iostream>
# include <string>
# include <vector>

typedef struct	s_ServerParser {
	std::string					context;
	std::vector<std::string>	directives;
}				t_ServerParser;

class ServerConfig {

public:
	ServerConfig();
	~ServerConfig();

    int port;
    std::string server_name;
} ;

#endif
