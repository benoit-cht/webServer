/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rep_lexing.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchallat <bchallat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 14:25:13 by bchallat          #+#    #+#             */
/*   Updated: 2026/04/07 15:37:12 by bchallat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/request/utils_HandlerHttp.hpp"

static std::string  lx_value(const std::string raw_request, int& index)
{
  std::string str;

  while (raw_request[index] != 32 && raw_request[index] != '\0')
  {
    str = str + raw_request[index];
    index++;
  }
  return ( str );
}

static bool  lx_write_value_map(const std::string raw_request, std::map<t_token, std::string>& lexer)
{
  int   index = 0;
  
  while (raw_request[index] != '\0')
  {
    if(raw_request[index] == 32)
    {
      lexer.insert(std::pair<t_token, std::string>(WHITESPACE, raw_request[index]);
    }
    else
    {
      lexer.insert(std::pair<t_token, std::string>(UNKNOWN, lx_value(raw_request, index));
    }
    index++;
  }
  return ( true );
}

std::map<t_token, std::string> req_lexing(const std::string raw_request)
{
  std::map<t_token, std::string> lexer;

  if (!lx_write_value_map(raw_request, lexer))
  {
    return( lexer.clear() );
  }
  
  return( lexer );

}

/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */


