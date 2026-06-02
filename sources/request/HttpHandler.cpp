/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpHandler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcluzan <lcluzan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 13:48:28 by bchallat          #+#    #+#             */
/*   Updated: 2026/06/02 10:11:02 by bchallat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <request/HttpHandler.hpp>

t_httpRequest  HttpHandler::setHttpRequest(const std::string& raw_request)
{
	t_httpRequest       request;
	std::vector<t_token>   lexer;

	lexer = HttpHandler::lexing(raw_request);
	if (HttpHandler::descending_deriv(lexer))
  {
	  request = HttpHandler::struct_http_request(lexer);
    std::cout << COLOR_GREEN << "Success : request is valid " << COLOR_RESET << std::endl;
  }
  else
  {
    std::cout << COLOR_YELLOW << "Warging : bad request " << COLOR_RESET << std::endl;
  }

  return ( request );
}

/* ========================================================================== */

t_httpResponse HttpHandler::setHttpResponse(t_httpRequest request)
{
	t_httpResponse                      response;
  std::map<std::string, std::string>  headers;

	if (request.method != "GET" && request.method != "POST" && request.method != "HEAD")
	{
    return (HandlerErrorHttp(400));
  }
    //else if (HttpHandler::isStaticFile(request.path))
  else if (request.method == "GET")
  {
    return (HttpHandler::handler_methode_get(request));
  }
  else if (request.method == "POST")
  {
    return (HttpHandler::handler_methode_post(request));
  }
  else if (request.method == "DELETE")
  {
    std::cout << COLOR_YELLOW << "Warnig : a methode delete not implement " << COLOR_RESET << std::endl;
    //return (HttpHandler::handler_methode_delete(request));
  }
  else
  {
    return (HandlerErrorHttp(404));
  }
	return ( response );
}

/* ========================================================================== */

t_httpResponse HttpHandler::HandlerErrorHttp(int status)
{
	t_httpResponse                      response;
  std::map<std::string, std::string>  headers;
  std::string body;
  
  headers["Connection"] = "close";
  headers["Content-Type"] = "text/html";
  
  std::ostringstream oss;

  if (status == 400)
    body ="<html><body>400 Bad Request</body></html>";

  else if (status == 403)
    body ="<html><body>403 Forbidden</body></html>";
    
  else if (status == 404)
    body ="<html><body>404 Not Found</body></html>";

  else if (status == 405)
    body ="<html><body>405 Method Not Allowed</body></html>";
  
  else if (status == 500)
    body ="<html><body>500 Internal Server Error</body></html>";
  
  else if (status == 501)
    body ="<html><body>501 Not Implemented</body></html>";


  oss << body.size();
  headers["Content-Length"] = oss.str();

  return (t_httpResponse(status, headers, body));
}

/* ========================================================================== */
/*                          -- HTTP METHODE --                                */
/* ========================================================================== */

t_httpResponse HttpHandler::handler_methode_get(t_httpRequest& request)
{
  if (request.path == "/")
  {
    return HttpHandler::serveStaticFile("index.html");
  }
  else if (HttpHandler::isStaticFile(request.path))
  {
    return (HttpHandler::serveStaticFile(request.path));
  }
  else if (request.path.find("/cgi-bin/") == 0 || request.path.find(".php") != std::string::npos || request.path.find(".py") != std::string::npos)
  {
    return HttpHandler::executeCgi(request.path, request);
  }
  else
    return (HandlerErrorHttp(500));
}

t_httpResponse HttpHandler::handler_methode_post(t_httpRequest& request)
{
  std::cout << COLOR_YELLOW << "Warnig : a methode post not implement " << COLOR_RESET << std::endl;
  
  if (request.path.find("/cgi-bin/") == 0 || request.path.find(".php") != std::string::npos || request.path.find(".py") != std::string::npos)
  {
    return HttpHandler::executeCgi(request.path, request);
  }
  else
    return (HandlerErrorHttp(500));

}

/*t_httpResponse HttpHandler::handler_methode_delete(t_httpRequest& request)
{
  
  return (HandlerErrorHttp(500));
}*/

/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */


