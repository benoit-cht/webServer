/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   servStaticFile.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchallat <bchallat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 17:31:55 by bchallat          #+#    #+#             */
/*   Updated: 2026/06/01 11:12:19 by ton_utilisate    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "../../include/request/utils_HandlerHttp.hpp"
#include <request/HttpHandler.hpp>
#include <fstream>   // Pour std::ifstream
#include <sstream>   // Pour std::stringstream
                     //
bool isSafePath(const std::string& path) {
    // Empêche les chemins relatifs (comme "../")
    return path.find("../") == std::string::npos;
}

std::string getMimeType(const std::string& path) {
    // Implémentez la logique pour déterminer le type MIME
    if (path.find(".html") != std::string::npos) return "text/html";
    if (path.find(".css") != std::string::npos) return "text/css";
    // Ajoutez d'autres types MIME selon les besoins
    return "text/plain";
}

t_httpResponse HttpHandler::serveStaticFile(const std::string& path) 
{
    if (!isSafePath(path))
    {
      return (HandlerErrorHttp(403));
    }

    std::string full_path = "./sources/www/" + path; // Assurez-vous que le chemin est correct
    std::ifstream file(full_path.c_str(), std::ios::binary);

    if (!file.is_open())
    {
      return (HandlerErrorHttp(404));
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    t_httpResponse response;
    response.status = 200;
    response.headers["Content-Type"] = getMimeType(full_path);
    response.body = content;
    
    return (response);
}

/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */



