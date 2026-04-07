/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpHandler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ton_utilisateur_42 <ton_email@student.42.  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 17:27:34 by ton_utilisate     #+#    #+#             */
/*   Updated: 2026/04/07 13:46:03 by bchallat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef HANDLERHTTP_HPP
# define HANDLERHTTP_HPP

#include "./utils_HandlerHttp.hpp"

class HttpHandler {
	
	public:
		HttpHandler( void ) = delete;
		virtual ~HttpHandler( void ) = delete;
		
	public:
		static t_httpRequest  setHttpRequest(const std::string& raw_request);
		static t_httpResponse setHttpResponse(int status, std::string body);
	
};

# endif // !HTTPHANDLER_HPP 

/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */

