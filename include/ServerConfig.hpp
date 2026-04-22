/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 19:26:05 by lcluzan           #+#    #+#             */
/*   Updated: 2026/04/21 15:25:29 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include <map>
# include <fstream>
# include <iostream>
# include <string>

class ServerConfig
{

public:
	ServerConfig();
	~ServerConfig();

    int port;
    std::string server_name;
} ;

#endif
