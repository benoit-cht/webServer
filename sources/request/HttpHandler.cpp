/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpHandler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcluzan <lcluzan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 13:48:28 by bchallat          #+#    #+#             */
/*   Updated: 2026/05/20 11:25:47 by lcluzan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/request/HttpHandler.hpp"
#include "../../include/cgi/CgiHandler.hpp"
#include "cstdio"

static bool isStaticFile(const std::string& path);


t_httpRequest  HttpHandler::setHttpRequest(const std::string& raw_request)
{
	t_httpRequest       request;
	std::vector<t_token>   lexer;

	lexer = req_lexing(raw_request);
	if (req_descending_deriv(lexer))
  {
	  request = req_struct_http_request(lexer);
    std::cout << COLOR_GREEN << "Success : request is valid " << COLOR_RESET << std::endl;
  }
  else
  {
    std::cout << COLOR_YELLOW << "Warging : bad request " << COLOR_RESET << std::endl;
  }

  return ( request );
}

/*t_httpResponse HttpHandler::setHttpResponse(t_httpRequest request)
{
	t_httpResponse response;

  try {

    }
    catch (const std::exception&) {

    }

  return ( response );
}*/

t_httpResponse HttpHandler::setHttpResponse(t_httpRequest request)
{
	t_httpResponse                      response;
  std::map<std::string, std::string>  headers;

	 // 1. Vérifier la méthode
	if (request.method != "GET" && request.method != "POST" && request.method != "HEAD")
	{
    headers["Content-Type"] = "text/html";
    return t_httpResponse(400, headers, "<html><body>400 Bad Request</body></html>");
  }

	// 2. Analyser le chemin
  if (request.path == "/"){
    return serveStaticFile("index.html");
  }if (request.method == "DELETE") {
        return handleDelete(request.path);
  }else if (request.path.find("/cgi-bin/") == 0){
    return CgiHandler::executeCgi(request.path, request);
    std::cout << "Warging: cgi detect not implemente methode " << std::endl;
  }else if (isStaticFile(request.path)){
    return serveStaticFile(request.path);
  }else{
    headers["Content-Type"] = "text/html";
    return t_httpResponse(404, headers, "<html>404 Not Found</html>");
  }
  return ( response );
}

/*
static t_httpResponse HandlerErrorHttp(int status)
{
	std::string body = "<html><body><h1>"
					+ std::to_string(status)
					+ " " + getStatusMessage(status)
					+ "</h1></body></html>";

    return t_httpResponse{status, {{"Content-Type", "text/html"}}, body};
}
*/

/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */

static bool isStaticFile(const std::string& path)
{
    if (path.find("/static/") == 0 ||
        path.substr(path.find_last_of(".") + 1) == "html" ||
        path.substr(path.find_last_of(".") + 1) == "css" ||
        path.substr(path.find_last_of(".") + 1) == "js") {
        return true;
    }
    return false;
}

t_httpResponse HttpHandler::handleDelete(const std::string& path) {
    t_httpResponse response;

    std::string full_path = "./sources/www" + path;

    if (std::remove(full_path.c_str()) == 0) {
      response.status = 204;
      response.body = ""; 
    } else {
      response.status = 404; // Not Found
      response.body = "<html><body><h1>404 Not Found</h1><p>Le fichier n'existe pas ou ne peut etre supprime.</p></body></html>";
    }
    return response;
}