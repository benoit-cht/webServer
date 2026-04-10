/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   req_lexing.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchallat <bchallat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 14:25:13 by bchallat          #+#    #+#             */
/*   Updated: 2026/04/08 13:09:52 by bchallat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/request/utils_HandlerHttp.hpp"

/*
static std::string typeToString(t_type type) 
{
    switch (type) {
        case METHOD: return "METHOD";
        case PATH: return "PATH";
        case VERSION: return "VERSION";
        case HEADER_KEY: return "HEADER_KEY";
        case HEADER_VALUE: return "HEADER_VALUE";
        case COLON: return "COLON";
        case CRLF: return "CRLF";
        case BODY: return "BODY";
        case WHITESPACE: return "WHITESPACE";
        case UNKNOWN: return "UNKNOWN";
        default: return "UNKNOWN";
    }
}

static void printTokens(const std::vector<t_token>& lexer)
{
    for (std::vector<t_token>::const_iterator it = lexer.begin(); it != lexer.end(); ++it) 
    {
        const t_token& token = *it;
        std::cout << "Token type: " << typeToString(token.type) << ", value: \"" << token.value << "\"" << std::endl;
    }
} */

static void lx_tokenizer(std::vector<t_token>& lexer)
{
  int curr = lexer.size() - 1;
  int i = lexer[curr].value.size() - 1;
  if (curr == 0 && (lexer[curr].value == "GET" || lexer[curr].value == "POST" || lexer[curr].value == "DELETE "))
  {
    lexer[curr].type = METHOD;
  }
  else if (curr == 2 && lexer[curr].value[0] == '/')
  {
    lexer[curr].type = PATH;
  }
  else if (curr == 4 && lexer[curr].value == "HTTP/1.1")
  {
    lexer[curr].type = VERSION;
  }
  else if (curr >= 5 && lexer[curr].value[i] == ':')
  {
    lexer[curr].type = HEADER_KEY;
  }
  else if (curr >= 7 && lexer[curr - 2].type == HEADER_KEY)
  {
    lexer[curr].type = HEADER_VALUE;
  }
  else if (curr >= 8 && lexer[curr - 1].type == HEADER_VALUE)
  {
    lexer[curr].type = CRLF;
  }
  else if (lexer[curr - 1].type == CRLF || lexer[curr - 1].type == BODY)
  {
    lexer[curr].type = BODY;
  }
}

static std::string  lx_value(const std::string raw_request, size_t& index)
{
  std::string str;

  while (index < raw_request.size() && raw_request[index] != ' ' )
  {
    if (raw_request[index] == '\r' && raw_request[index + 1] == '\n')
    {
      index += 2;
      break ;
    }
    str = str + raw_request[index];
      index++;
  }
  return ( str );
}

static std::string lx_header_value(const std::string raw_request, size_t& index)
{
  std::string str;

  while (index < raw_request.size() && raw_request[index] != '\r' && raw_request[index + 1] != '\n')
  {
    str = str + raw_request[index];
    index++;
  }
  return ( str );
}

static void  lx_write_value(const std::string raw_request, size_t& index, std::vector<t_token>& lexer)
{
  std::string str;

  if(raw_request[index] == 32)
  {
    s_token temp(WHITESPACE, " ");
    lexer.push_back(temp);
    index++;
  }
  else if (index > 2 && raw_request[index - 2] == ':')
  {
    str = lx_header_value(raw_request, index);
    s_token temp(UNKNOWN, str);
    lexer.push_back(temp);
  
  }
  else
  {
    str = lx_value(raw_request, index); 
    s_token temp(UNKNOWN, str);
    lexer.push_back(temp);
  }
}

std::vector<t_token> req_lexing(const std::string raw_request)
{
  std::vector<t_token> lexer;
  size_t index = 0;

  while (index + 1 < raw_request.size())
  {
    lx_write_value(raw_request, index, lexer);
    lx_tokenizer(lexer);
  }

  return ( lexer );
}


/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */


