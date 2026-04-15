/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpHandler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchallat <bchallat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 13:48:28 by bchallat          #+#    #+#             */
/*   Updated: 2026/04/10 20:24:22 by bchallat         ###   ########.fr       */
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

/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */
