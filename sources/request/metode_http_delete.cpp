/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   metode_http_delete.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchallat <bchallat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 16:34:45 by bchallat          #+#    #+#             */
/*   Updated: 2026/06/11 16:35:19 by bchallat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <request/HttpHandler.hpp>

/* ========================================================================== */
/*                          -- HTTP METHODE --                                */
/* ========================================================================== */

t_httpResponse HttpHandler::handler_methode_delete(t_httpRequest& request, const ServerConfig& config)
{
  int     status = 500;
  std::map<std::string, LocationConfig>	location = config.getLocationConfig();

  std::cout << COLOR_MAGENTA << "   ~~~~~~ METHODE DELETE ~~~~~~\n" << COLOR_RESET << std::endl;
  if(remove((location["/"].getRoot() + request.path).c_str()) == 0) {
    status = 204;
    std::cout << COLOR_GREEN << "Success: " << location["/"].getRoot() + request.path <<" is delete !" << COLOR_RESET << std::endl;
  
  }else {
    status = 404 ; 
    std::cout << COLOR_RED << "Le fichier n'a pas été supprimer a cause qu'il n'existe pas, un droit d'accès refusé,..." << COLOR_RESET << std::endl;
  }
  std::cout << COLOR_MAGENTA << "\n   ~~~~~~  METHODE END  ~~~~~~" << COLOR_RESET << std::endl;
  return (HandlerErrorHttp(status));
}

/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */

