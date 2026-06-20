/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpHandler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 13:48:28 by bchallat          #+#    #+#             */
/*   Updated: 2026/06/20 15:17:46 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <request/HttpHandler.hpp>

/* ========================================================================== */
/*                          -- PARSING REQUESTE --                            */
/* ========================================================================== */

t_httpRequest  HttpHandler::setHttpRequest(const std::string& raw_request)
{
	t_httpRequest          request;
	std::vector<t_token>   lexer;
  std::string             processed_request = raw_request;

  if (processed_request.find("Transfer-Encoding: chunked") != std::string::npos) {
    size_t header_end = processed_request.find("\r\n\r\n");
    if (header_end != std::string::npos) {
      std::string headers = processed_request.substr(0, header_end + 4);
      std::string body = processed_request.substr(header_end + 4);

      body = unchunkBody(body); // Strip the hex numbers and zeros

      processed_request = headers + body; // Reassemble the clean request
    }
  }

	lexer = HttpHandler::lexing(raw_request);
	if (HttpHandler::descending_deriv(lexer))
  {
	  request = HttpHandler::struct_http_request(lexer);
    std::cout << COLOR_GREEN << "Success : descending deriv " << COLOR_RESET << std::endl;
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

t_httpResponse HttpHandler::setHttpResponse(t_httpRequest request, const ServerConfig& config)
{
  t_config            directiv;
  t_httpResponse      response;
  t_VarOfsetResponse  var;

  directiv.location = config.getLocationConfig();
  response = checkRequestIsLegit(var, request, directiv, config);
  if (!response.status == 0) {

    return (response);
  }

  // check if there is a redirection
  std::pair<int, std::string> redir = directiv.config.getRedirect();
  if (redir.first != 0) {
	t_httpResponse redirResp = HttpHandler::HandlerErrorHttp(redir.first, request, config);
	if (redir.second.find("http://") != std::string::npos || redir.second.find("https://") != std::string::npos) {
		redirResp.headers["Location"] = redir.second;
	}
	return (redirResp);
  }

  std::map<std::string, std::string> cgi_map = directiv.config.getCgiPass();
  if (!var.extension.empty() && cgi_map.find(var.extension) != cgi_map.end()) {

    return (cgiConditionForExecute(config, request, var.actual_path, cgi_map, var.extension, directiv.config));

  } else if (request.method == "GET")  {

      return (HttpHandler::handler_methode_get(request, config));

  } else if (request.method == "POST") {

      return (HttpHandler::handler_methode_post(request, config));

  } else if (request.method == "DELETE")  {

      return (HttpHandler::handler_methode_delete(request, config));

  } else  {

      return (HandlerErrorHttp(500, request, config));
  }
	return ( var.response );
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

  if (file.is_open() && !page[status].empty())
  {
    std::stringstream buffer;
    buffer << file.rdbuf();
    body = buffer.str();

  }

  return (body);
}

/*

void unitt-HandlerErrorHttp(int status, t_httpRequest request, const ServerConfig& config)
*/

void HttpHandler::setStructConfig(t_config directiv, const ServerConfig& serv, std::string path)
{
  directiv.location = serv.getLocationConfig();
  directiv.prefix = find_location(directiv.location, path);
  directiv.config = directiv.location[directiv.prefix];
}
/* ================================ ~CODE ====================================== */

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
      return ( HttpHandler::status_301(status, request, stError.response, directiv.config.getRedirect().second) );

    case 302:
      return ( HttpHandler::status_302(status, request, stError.response, directiv.config.getRedirect().second) );

    case 304:
      return ( HttpHandler::status_304(status, request, stError.response, directiv.config.getRedirect().second) );

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

// Decoding Function

std::string HttpHandler::unchunkBody(const std::string& chuncked_body) {
  std::string unchunked_body;
  size_t pos = 0;

  while (pos < chuncked_body.size()) {
    size_t crtlf_pos = chuncked_body.find("\r\n", pos);
    if (crtlf_pos == std::string::npos) break;

    std::string hex_str = chuncked_body.substr(pos, crtlf_pos - pos);
    long chunk_size = 0;
    std::stringstream ss;
    ss << std::hex << hex_str;
    ss >> chunk_size;

    if (chunk_size == 0) break; // Final chunk reached

    pos = crtlf_pos + 2;
    unchunked_body.append(chuncked_body, pos, chunk_size);
    pos += chunk_size + 2;
  }
  return unchunked_body;
}
