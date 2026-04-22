/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpHandler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchallat <bchallat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 11:31:58 by bchallat          #+#    #+#             */
/*   Updated: 2026/04/22 11:33:11 by bchallat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef HANDLERHTTP_HPP
# define HANDLERHTTP_HPP

#include "./utils_HandlerHttp.hpp"

class HttpHandler {
	
  public:
		static t_httpRequest  setHttpRequest(const std::string& raw_request);
		static t_httpResponse setHttpResponse(t_httpRequest request);
    static t_httpResponse HandlerErrorHttp(int status);

	private:
		HttpHandler( void );
		virtual ~HttpHandler( void );
};

# endif // !HTTPHANDLER_HPP 

/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */

