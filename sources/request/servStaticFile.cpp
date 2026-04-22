/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   servStaticFile.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchallat <bchallat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 17:31:55 by bchallat          #+#    #+#             */
/*   Updated: 2026/04/20 17:32:41 by bchallat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/request/utils_HandlerHttp.hpp"

t_httpResponse serveStaticFile(const std::string& path) 
{
    if (!isSafePath(path)) 
    {
        return t_httpResponse{403, {{"Content-Type", "text/html"}}, "<html>403 Forbidden</html>"};
    }

    std::string full_path = "/var/www/html" + path;
    std::ifstream file(full_path, std::ios::binary);
    if (!file) 
    {
        return t_httpResponse{404, {{"Content-Type", "text/html"}}, "<html>404 Not Found</html>"};
    }

	std::string content((std::istreambuf_iterator<char>(file)),
						std::istreambuf_iterator<char>());
    
    std::string mime_type = getMimeType(full_path);

    return t_httpResponse{
                           200,
                           {{"Content-Type", mime_type}, 
                            {"Content-Length", std::to_string(content.size())}},
                            content
    };
}

/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */



