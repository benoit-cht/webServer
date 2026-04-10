/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandlerHttp.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchallat <bchallat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 13:48:28 by bchallat          #+#    #+#             */
/*   Updated: 2026/04/07 13:56:25 by bchallat         ###   ########.fr       */
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
  }
  else
  {
    std::cout << COLOR_GREEN << "Success : request is valid " << COLOR_RESET << std::endl;
  }
	
  return ( request );
}

/*t_httpResponse HttpHandler::setHttpResponse(int status, std::string body)
{
	t_httpResponse response;
	
	response.status = status;
	response.headers = res_create_headers();
	if (response.headers.size() == 0)
	{
	    std::cerr << " Impossible response " << std::endl;
  }
    
  response.body = body;
	return ( response );	
}*/

/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */


