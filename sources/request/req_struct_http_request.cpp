/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   req_struct_http_request.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchallat <bchallat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 12:24:05 by bchallat          #+#    #+#             */
/*   Updated: 2026/04/08 12:47:37 by bchallat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/request/utils_HandlerHttp.hpp"

static void   st_write_in_struct(t_httpRequest& request, t_token token)
{
  if (token.type == WHITESPACE || token.type == CRLF)
    return ;
  else if (token.type == METHOD)
    request.method = token.value;
  else if (token.type == PATH)
    request.path = token.value;
  else if (token.type == VERSION)
    request.version = token.value;
  else if (token.type == HEADER_KEY)
    //to do 
  else if (token.type == HEADER_VALUE)
    //to do
  else if (token.type == BODY)
    request.body = request.body + token.value;

}

t_httpRequest req_struct_http_request(std::vector<t_token>& lexer)
{
  size_t        indx = 0;
  t_httpRequest request;

  while (indx + 1 < lexer.size()) 
  {
    st_write_in_struct(request, lexer[indx]);
    indx++;
  }

  return (request);
}

/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */

