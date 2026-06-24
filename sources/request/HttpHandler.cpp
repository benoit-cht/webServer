/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpHandler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 13:48:28 by bchallat          #+#    #+#             */
/*   Updated: 2026/06/24 16:13:57 by tjacquel         ###   ########.fr       */
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

	  // Replace Transfer-Encoding with Content-Length
	  size_t transEnc_pos = headers.find("Transfer-Encoding: chunked\r\n");
	  if (transEnc_pos != std::string::npos) {
		std::stringstream ss;
		ss << "Content-Length: " << body.size() << "\r\n";
		headers.replace(transEnc_pos, 28, ss.str()); // 28 == "Transfer-Encoding: chunked\r\n".size()
	  }

      processed_request = headers + body; // Reassemble the clean request
    }
  }

	lexer = HttpHandler::lexing(processed_request);
	if (HttpHandler::descending_deriv(lexer))
  {
	  request = HttpHandler::struct_http_request(lexer);
    std::cout << COLOR_GREEN << "Success : descending deriv " << COLOR_RESET << std::endl;
  }
  else
  {
    std::cout << COLOR_YELLOW << "Warging : bad request " << COLOR_RESET << std::endl;
  }

	std::map<std::string, std::string>::iterator it = request.headers.find("Content-Length");
		if (it != request.headers.end()) {
			size_t expected_len = static_cast<size_t>(std::atoi(it->second.c_str()));
			if (request.body.length() > expected_len) {
				request.body = request.body.substr(0, expected_len);
			}
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

  t_httpResponse final_response;
  std::map<std::string, std::string> cgi_map = directiv.config.getCgiPass();

  if (!var.extension.empty() && cgi_map.find(var.extension) != cgi_map.end()) {
      final_response = cgiConditionForExecute(config, request, var.actual_path, cgi_map, var.extension, directiv.config);
  }
  else if (request.method == "GET")  {
      final_response = HttpHandler::handler_methode_get(request, config);
  }
  else if (request.method == "POST") {
      final_response = HttpHandler::handler_methode_post(request, config);
  }
  else if (request.method == "DELETE")  {
      final_response = HttpHandler::handler_methode_delete(request, config);
  }
  else  {
      final_response = HandlerErrorHttp(500, request, config);
  }
  handleSession(request, final_response);
  return (final_response);
}

/* ========================================================================== */
/*                          -- HEADLER RESPONSSE --                           */
/* ========================================================================== */

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
/*                          -- COOKIE & SESSION --                            */
/* ========================================================================== */


std::map<std::string, int> HttpHandler::_sessions;

std::string HttpHandler::getCookieValue(const std::string& cookieHeader, const std::string& cookieName)
{
    size_t pos = cookieHeader.find(cookieName + "=");
    if (pos == std::string::npos) return "";

    pos += cookieName.length() + 1;
    size_t end = cookieHeader.find(";", pos);
    if (end == std::string::npos) end = cookieHeader.length();

    return cookieHeader.substr(pos, end - pos);
}

std::string HttpHandler::generateSessionId()
{
    // Generate a session ID
    std::stringstream ss;
    ss << "sess_" << std::rand() % 1000000;
    return ss.str();
}

void HttpHandler::handleSession(const t_httpRequest& request, t_httpResponse& response)
{
    std::string cookieHeader = "";

    // Check if the client sent a Cookie header
    std::map<std::string, std::string>::const_iterator it = request.headers.find("Cookie");
    if (it != request.headers.end()) {
        cookieHeader = it->second;
    }

    std::string sessionId = getCookieValue(cookieHeader, "session_id");

    if (sessionId.empty() || _sessions.find(sessionId) == _sessions.end()) {
        // --- NEW CLIENT ---
        sessionId = generateSessionId();

        // Browser to store the cookie
        response.headers["Set-Cookie"] = "session_id=" + sessionId + "; Max-Age=3600; Path=/";

        // Initialize session data
        _sessions[sessionId] = 1;
        std::cout << COLOR_GREEN << "[SESSION] New user! Assigned ID: " << sessionId << COLOR_RESET << std::endl;
    } else {
        // --- RETURNING CLIENT ---
        _sessions[sessionId]++;
        std::cout << COLOR_GREEN << "[SESSION] Welcome back " << sessionId
                  << "! Visit count: " << _sessions[sessionId] << COLOR_RESET << std::endl;
    }
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
