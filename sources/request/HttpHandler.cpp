/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpHandler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchallat <bchallat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 13:48:28 by bchallat          #+#    #+#             */
/*   Updated: 2026/04/20 17:35:32 by bchallat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/request/HttpHandler.hpp"

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

static t_httpResponse setHttpResponse(t_httpRequest request)
{
	t_httpResponse response;
	
	 // 1. Vérifier la méthode
	if (request.method != "GET" && 
		request.method != "POST" && 
		request.method != "HEAD") 
	{return t_httpResponse{501, {{"Content-Type", "text/html"}}, "<html>501 Not Implemented</html>"};}
	
	// 2. Analyser le chemin
  if (request.path == "/") 
  {
    return serveStaticFile("/var/www/html/index.html");
  } 
  else if (request.path.find("/cgi-bin/") == 0) 
  {
    //To do 
    //return executeCgi(request.path, request);
    std::cout << "Warging: cgi detect not implemente methode " << std::endl;
  } 
  else if (isStaticFile(request.path)) 
  {
    return serveStaticFile(request.path);
  } 
  else 
  {
    return t_httpResponse{404, {{"Content-Type", "text/html"}}, "<html>404 Not Found</html>"};
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
