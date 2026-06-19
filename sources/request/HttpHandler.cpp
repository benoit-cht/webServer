/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpHandler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 13:48:28 by bchallat          #+#    #+#             */
/*   Updated: 2026/06/19 16:30:14 by bchallat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <request/HttpHandler.hpp>

/* ========================================================================== */
/*                          -- PARSING REQUESTE --                            */
/* ========================================================================== */

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
/*                          -- SET RESPONSSE --                               */
/* ========================================================================== */

t_httpResponse HttpHandler::cgiConditionForExecute(const ServerConfig& config, t_httpRequest request, std::string actual_path , std::map<std::string, std::string> cgi_map, std::string extension, LocationConfig current_loc ) 
{
  std::string interpreter = cgi_map[extension]; // e.g., "/usr/bin/php-cgi"
  if (!interpreter.empty()) {

    std::string full_script_path = current_loc.getRoot() + actual_path;

    // F_OK checks if the file exists.
    if (access(full_script_path.c_str(), F_OK) !=0) {

          return HandlerErrorHttp(404, request, config);
    }
    // R_OK checks if we have permission to read the file.
    if (access(full_script_path.c_str(), R_OK) != 0) {

        return HandlerErrorHttp(403, request, config);
    }
  }
  // If it exists and is readable, we safely execute the CGI
  return HttpHandler::executeCgi(request.path, request, config, interpreter, current_loc);
}

t_httpResponse HttpHandler::setHttpResponse(t_httpRequest request, const ServerConfig& config)
{
	t_httpResponse                        response;
  std::map<std::string, std::string>    headers;
  std::map<std::string, LocationConfig> location = config.getLocationConfig();


  // si ya un query ? on a quand meme besoin d'extraire le path sans le ? et ce quil y a apres
  std::string actual_path = request.path;
  size_t q_pos = actual_path.find('?');
  if (q_pos != std::string::npos) {
    actual_path = actual_path.substr(0, q_pos);
  }cgi-bin/

  std::string prefix = find_location(location, actual_path);
  LocationConfig current_loc = location[prefix];

  if (request.headers.empty())  {

    return (HandlerErrorHttp(400, request, config));
  
  }
	if (!location[prefix].checkMethod(request.method))  {

    return (HandlerErrorHttp(405, request, config));
  
  }
  if (request.body.size() > config.getClientMaxBodySize()) { // Check client max body size

	    return (HandlerErrorHttp(413, request, config));
  
  }

  // New dynamic CGI location check
  std::string extension = "";
  size_t dot_pos = actual_path.find_last_of(".");
  if (dot_pos != std::string::npos) {

    extension = actual_path.substr(dot_pos);
  }
  std::map<std::string, std::string> cgi_map = current_loc.getCgiPass();

  if (!extension.empty() && cgi_map.find(extension) != cgi_map.end()) {

    return (cgiConditionForExecute(config, request, actual_path, cgi_map, extension, current_loc));
  
  } else if (request.method == "GET")  {

      return (HttpHandler::handler_methode_get(request, config));
  
  } else if (request.method == "POST") {
   
      return (HttpHandler::handler_methode_post(request, config));
  
  } else if (request.method == "DELETE")  {

      return (HttpHandler::handler_methode_delete(request, config));
  
  } else  {
    
      return (HandlerErrorHttp(500, request, config));
  }
	return ( response );
}

/* ========================================================================== */
/*                          -- HEADLER RESPONSSE --                           */
/* ========================================================================== */


std::string   HttpHandler::findAndOpenBody(int status, t_httpRequest request, const ServerConfig& config)
{
  std::string                           body;
  std::map<std::string, LocationConfig>	location = config.getLocationConfig();
  std::string                           prefix = find_location(location, request.path);
  std::map<int, std::string>            page = location[prefix].getErrorPage();


  std::string open = location[prefix].getRoot() + "/" + page[status];
  std::ifstream file(open.c_str(), std::ios::binary);
  std::ostringstream oss;

  if (file.is_open() && !page[status].empty())  {

    std::stringstream buffer;
    buffer << file.rdbuf();
    body = buffer.str();

  }
  return (body);
}

void HttpHandler::setStructConfig(t_config directiv, const ServerConfig& serv, std::string path)
{
  directiv.location = serv.getLocationConfig();
  directiv.prefix = find_location(directiv.location, path);
  directiv.config = directiv.location[directiv.prefix]; 
}

t_httpResponse HttpHandler::HandlerErrorHttp(int status, t_httpRequest request, const ServerConfig& serv)
{
  t_ErrorHttp   stError;
  t_config      directiv;

  setStructConfig(directiv, serv, request.path);
  stError.response.body  = HttpHandler::findAndOpenBody(status, request, serv);

  switch ( status ) {

    case 201:
      return ( HttpHandler::status_201(status, stError.response) );

    case 204:
      return ( HttpHandler::status_204(status, stError.response) );

    case 301:
      return ( HttpHandler::status_301(status, request, stError.response) );

    case 302:
      return ( HttpHandler::status_302(status, request, stError.response) );
    
    case 304:
      return ( HttpHandler::status_304(status, request, stError.response) );

    case 400:
      return ( HttpHandler::status_400(status, stError.response) );
    
    case 403:
      return ( HttpHandler::status_403(status, stError.response) );

    case 404:
      return ( HttpHandler::status_404(status, stError.response) );
    
    case 405:
      return ( HttpHandler::status_405(status, stError.response, directiv) );

    case 413:
      return ( HttpHandler::status_413(status, stError.response) );

    case 501:
      return ( HttpHandler::status_501(status, stError.response) );

    case 504:
      return ( HttpHandler::status_504(status, stError.response) );
  }
  
  return ( HttpHandler::status_500(status, stError.response) );

}

/* ========================================================================== */
/*                                                                            */
/* ========================================================================== */
