/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpHandler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcluzan <lcluzan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 11:31:58 by bchallat          #+#    #+#             */
/*   Updated: 2026/05/20 11:34:16 by lcluzan          ###   ########.fr       */
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
    static t_httpResponse handleDelete(const std::string& path);
    static bool           isDirectory(const std::string& path);
    static t_httpResponse handleAutoIndex(const std::string& path);
};

# endif // !HTTPHANDLER_HPP 

/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */

