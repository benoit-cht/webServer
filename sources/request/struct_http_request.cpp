/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   req_struct_http_request.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchallat <bchallat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 12:24:05 by bchallat          #+#    #+#             */
/*   Updated: 2026/04/08 12:58:06 by bchallat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "../../include/request/utils_HandlerHttp.hpp"
#include <request/HttpHandler.hpp>

static void print_http_request(const t_httpRequest& request) {
    // Affichage de la méthode, du chemin et de la version
    std::cout << COLOR_CYAN << "\n=== HTTP Request ===" << COLOR_RESET << std::endl;
    std::cout << "Method: " << COLOR_YELLOW << request.method << COLOR_RESET << std::endl;
    std::cout << "Path: " << COLOR_YELLOW << request.path << COLOR_RESET << std::endl;
    std::cout << "Version: " << COLOR_YELLOW << request.version << COLOR_RESET << std::endl;

    // Affichage des en-têtes
    std::cout << COLOR_CYAN << "\n--- Headers ---" << COLOR_RESET << std::endl;
    if (request.headers.empty()) {
        std::cout << COLOR_RED << "No headers." << COLOR_RESET << std::endl;
    } else {
        for (std::map<std::string, std::string>::const_iterator it = request.headers.begin();
             it != request.headers.end(); ++it) {
            std::cout << it->first << ": " << COLOR_GREEN << it->second << COLOR_RESET << std::endl;
        }
    }

    // Affichage du body (si présent)
    std::cout << COLOR_CYAN << "\n--- Body ---" << COLOR_RESET << std::endl;
    if (request.body.empty()) {
        std::cout << COLOR_RED << "No body." << COLOR_RESET << std::endl;
    } else {
        std::cout << COLOR_BLUE << request.body << COLOR_RESET << std::endl;
    }

    std::cout << COLOR_CYAN << "===================\n" << COLOR_RESET << std::endl;
}

static void   init_hash_table_header(t_httpRequest& request, std::vector<t_token>& lexer)
{
  size_t      index = 0;
  std::string h_key;
  std::string h_value;

  while (index + 1 < lexer.size())
  {
    if (lexer[index].type == HEADER_KEY)
    {
      h_key = lexer[index].value;
    }
    else if (lexer[index].type == HEADER_VALUE && !h_key.empty())
    {
      h_value = lexer[index].value;
      request.headers[h_key] = h_value;
    
      h_key.clear();
      h_value.clear();
    }
    index++;
  }
}

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
  else if (token.type == BODY)
    request.body += token.value;
}

t_httpRequest HttpHandler::struct_http_request(std::vector<t_token>& lexer)
{
  size_t        indx = 0;
  t_httpRequest request;

  while (indx < lexer.size()) 
  {
    st_write_in_struct(request, lexer[indx]);
    indx++;
  }
  
  init_hash_table_header(request, lexer);
  print_http_request(request);
  return (request);
}

/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */

