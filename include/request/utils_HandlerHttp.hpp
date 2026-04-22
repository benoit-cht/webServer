/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_HandlerHttp.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchallat <bchallat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 13:37:08 by bchallat          #+#    #+#             */
/*   Updated: 2026/04/22 12:12:24 by bchallat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HANDLERHTTP_HPP 
#define UTILS_HANDLERHTTP_HPP

/*------------------------------------*/
/*             include                */
/*------------------------------------*/

#include <iostream>
#include <stdexcept>
#include <string>
#include <map>

#include "./../log/colorLog.hpp"

/*------------------------------------*/
/*          definition struct         */
/*------------------------------------*/

typedef enum e_type
{
	METHOD,
	PATH,
	VERSION,
  HEADER_KEY, 
	HEADER_VALUE, 
	COLON, 
	CRLF,
	BODY,
	WHITESPACE,
	UNKNOWN

} t_type;

typedef struct s_token
{ 
	t_type       type; 
	std::string  value;
  s_token(t_type t, const std::string& v) : type(t), value(v) {}
  ~s_token(void){};

} t_token;

typedef struct s_httpRequest 
{
	std::string                        method;
  std::string                        path;
  std::string                        version;
  std::map<std::string, std::string> headers;
  std::string                        body;
    
}   t_httpRequest;

typedef struct s_httpResponse 
{
	int                                status;
  std::map<std::string, std::string> headers;
	std::string                        body;
  
  s_httpResponse();
  s_httpResponse(int status, const std::map<std::string, std::string>& headers, const std::string& body);
  std::string toString() const;

} t_httpResponse;

/*------------------------------------*/
/*    prototype fonction parsing      */
/*------------------------------------*/

bool                  req_descending_deriv(std::vector<t_token>& lexer);
t_httpRequest         req_struct_http_request(std::vector<t_token>& lexer); 
std::vector<t_token>  req_lexing(const std::string raw_request);

/*------------------------------------*/
/*    prototype fonction response     */
/*------------------------------------*/

std::string readFile(const std::string& path);
t_httpResponse serveStaticFile(const std::string& path);
void print_http_response(const t_httpResponse& response);

#endif // !UTILS_HANDLERHTTP_HPP


/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */

