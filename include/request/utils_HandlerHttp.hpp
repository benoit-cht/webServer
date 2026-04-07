/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_HandlerHttp.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchallat <bchallat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 13:37:08 by bchallat          #+#    #+#             */
/*   Updated: 2026/04/07 13:51:57 by bchallat         ###   ########.fr       */
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

typedef enum e_token
{
	METHOD,
	PATH,
	VERSION,
	HEADERS,
	BODY,
	WHITESPACE,
	UNKNOWN

}	t_token;

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
  
  // Convertit en chaîne HTTP brute
  std::string                        toString() const;

} t_httpResponse;

/*------------------------------------*/
/*    prototype fonction parsing      */
/*------------------------------------*/

std::map<t_token, std::string>      req_lexing(const std::string raw_request);
bool                                req_descending_deriv(std::map<t_token, std::string>& lexer);
t_httpRequest                       req_struct(std::map<t_token, std::string>& lexer); 

/*------------------------------------*/
/*    prototype fonction response     */
/*------------------------------------*/

std::map<std::string, std::string>  res_create_headers();

#endif // !UTILS_HANDLERHTTP_HPP


/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */

