/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requestHttp.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ton_utilisateur_42 <ton_email@student.42.  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 17:27:34 by ton_utilisate     #+#    #+#             */
/*   Updated: 2026/02/23 17:41:23 by ton_utilisate    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

class RequestHttp {

  public :
    
    RequestHttp ( void );
    virtual ~RequestHttp ( void );
    RequestHttp (const RequestHttp& other);
    RequestHttp operator=(const RequestHttp& other);

  private :
  /* first ligne in request */
    int         _methode ;
    std::string _link;
    double      _version;
  /* header lingne */
    std::string _host;
    std::string _userAgent;
    std::string _contentType;
    ssize_t     _contentLength;
    std::string _connection;
}








/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */

