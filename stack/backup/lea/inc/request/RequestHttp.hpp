/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHttp.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ton_utilisateur_42 <ton_email@student.42.  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 17:27:34 by ton_utilisate     #+#    #+#             */
/*   Updated: 2026/02/25 18:37:49 by bchallat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef REQUESTHTTP_HPP
# define REQUESTHTTP_HPP

#include <map>
#include <string>
#include <cstdlib>

class RequestHttp {

  public :

  RequestHttp ( void );
  virtual ~RequestHttp ( void );
  RequestHttp (const RequestHttp& other);
  RequestHttp operator=(const RequestHttp& other);
  // Une énumération pour rendre le code lisible (GET=0, POST=1, DELETE=2)
  enum MethodType { GET, POST, DELETE, UNKNOWN };
  // ... (vos constructeurs/destructeurs existants) ...
  // La fonction principale de parsing
  bool parse(const std::string& raw_request);

  private :

  // Une sous-fonction pour gérer uniquement la première ligne
  bool parseFirstLine(const std::string& line);
  // La fonction pour analyser le bloc des headers
  bool parseHeaders(const std::string& headers_chunk);
    // Map pour stocker TOUS les headers facilement
  std::map<std::string, std::string> _headers;
  /* first ligne in request */
    int         _methode ;
    std::string _link;
    double      _version;
    std::string _body;
  /* header lingne */
    std::string _host;
    std::string _userAgent;
    std::string _contentType;
    ssize_t     _contentLength;
    std::string _connection;
};

# endif

/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */

