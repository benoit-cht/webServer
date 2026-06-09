/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 17:24:16 by bchallat          #+#    #+#             */
/*   Updated: 2026/06/08 16:40:02 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <csignal>
#include <cstdlib>
#include <iostream>

# include "../include/webserver.hpp"


/* ========================================================================== */
/*                    -- definition and prototype --                          */
/* ========================================================================== */
#if RUN_WITH_SERVER

volatile sig_atomic_t g_should_exit = 0;

static void           signal_handler(int signal);
static void           print_http_request(const t_httpRequest& request); 
static void           print_http_response(const t_httpResponse& response); 
std::string           readFullHttpRequest(int fd, EventLoop& loop) ;
static void           inf_loop(std::vector<int> ports, const std::vector<ServerConfig>& config);
static int            getPortServer(int fd);
static void           initVerctorOfPort(std::vector<int>& ports, const std::vector<ServerConfig>& virtual_servers);

#endif
/* ========================================================================== */
/*                           -- main focntion --                              */
/* ========================================================================== */

int main (int av, char *ag[]) 
{
	if (av != 2) {

		std::cout << "Usage: ./webserv [configuration file]" << std::endl;
		return (EXIT_FAILURE);
	}

#if RUN_WITH_SERVER
		std::signal(SIGINT, signal_handler);// Enregistrer le gestionnaire de signal
#endif

try {
	std::vector<ServerConfig>	virtual_servers;
	confParsingHandler(ag[1], virtual_servers);

#if RUN_WITH_SERVER
	std::vector<int> ports;
	initVerctorOfPort(ports, virtual_servers);
	inf_loop(ports, virtual_servers);
#endif

    } catch (const std::exception& e) {

      std::cerr << COLOR_RED << "Error: " << e.what() << COLOR_RESET << std::endl;
      return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}


/* ========================================================================== */
/*                           -- Loop of event --                              */
/* ========================================================================== */
#if RUN_WITH_SERVER


static void handler_conection(EventLoop &loop, const std::vector<ServerConfig>& vec_config)
{
  std::vector<int> activeFds = loop.getActiveFds();
  
  for (size_t i = 0; i < activeFds.size(); i++) 
  {
    int fd = activeFds[i];
    if (loop.isServerFd(fd)) {

      loop.acceptNewConnection(fd);
    } else {

      std::string string_request;
      string_request = readFullHttpRequest(fd, loop);
      
      ( void )     vec_config;
      ServerConfig config; 
      std::cout << "Server port for fd=" << fd << ": " << getPortServer(fd) << std::endl;

      t_httpRequest request = HttpHandler::setHttpRequest(string_request);
      t_httpResponse response = HttpHandler::setHttpResponse(request, config);

      std::string raw_response = response.toString();
      loop.sendResponse(fd, raw_response);

      if (response.status == 400)
      {
        loop.removeClient(fd);
      }

      print_http_request(request);
      print_http_response(response);

    }
  }
}

static void inf_loop(std::vector<int> ports, const std::vector<ServerConfig>& vec_config)
{
  int       ret = -1;
  EventLoop loop(ports);

  loop.setupServerSockets();
  while (!g_should_exit) {

    ret = loop.waitForActivity();
    if (ret == -1)
      std::cerr << COLOR_RED << "Error: poll() failed (interrupted system call?)" << COLOR_RESET << std::endl;
    else
      handler_conection(loop,vec_config);
  }
}

/* ========================================================================== */
/*                                                                            */
/* ========================================================================== */

static void signal_handler(int signal)
{
  if (signal == SIGINT)
  {
    std::cout << COLOR_YELLOW << " \nWarning : Reçu SIGINT (Ctrl+C), arrêt du serveur... " << COLOR_RESET << std::endl;
    g_should_exit = 1;
  }
}

	// std::cout << "virtual_servers.size()=" << virtual_servers.size() << std::endl;
		// std::cout << "virtual_servers[i].getPort()=" << virtual_servers[i].getPort() << std::endl;
static void initVerctorOfPort(std::vector<int>& ports, const std::vector<ServerConfig>& virtual_servers)
{
	for (size_t i = 0; i < virtual_servers.size(); ++i)
    ports.push_back(virtual_servers[i].getPort());
}

static int  getPortServer(int fd)
{
  struct sockaddr_in server_addr;
  socklen_t server_addr_len = sizeof(server_addr);
  
  if (getsockname(fd, (struct sockaddr *)&server_addr, &server_addr_len) == -1)
        std::cerr << COLOR_RED << "getsockname() failed: " << strerror(errno) << COLOR_RESET << std::endl;

  return ( ntohs(server_addr.sin_port) );
}

/* ========================================================================== */
/*                 -- print fonction for request/response --                  */
/* ========================================================================== */

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
#if DEBUG_FLAG
    // Affichage du body (si présent)
    std::cout << COLOR_CYAN << "\n--- Body ---" << COLOR_RESET << std::endl;
    if (request.body.empty()) {
        std::cout << COLOR_RED << "No body." << COLOR_RESET << std::endl;
    } else {
        std::cout << COLOR_BLUE << request.body << COLOR_RESET << std::endl;
    }
#endif 
    std::cout << COLOR_CYAN << "===================\n" << COLOR_RESET << std::endl;
}

static void print_http_response(const t_httpResponse& response) {
    // Affichage du statut
    std::cout << COLOR_CYAN << "\n=== HTTP Response ===" << COLOR_RESET << std::endl;
    std::cout << "Status: " << COLOR_YELLOW << response.status << COLOR_RESET;
    switch (response.status) {
        case 200: std::cout << " (OK)"; break;
        case 400: std::cout << " (Bad Request)"; break;
        case 403: std::cout << " (Forbidden)"; break;
        case 404: std::cout << " (Not Found)"; break;
        case 501: std::cout << " (Not Implemented)"; break;
        default: std::cout << " (Unknown Status)"; break;
    }
    std::cout << COLOR_RESET << std::endl;

    // Affichage des en-têtes
    std::cout << COLOR_CYAN << "\n--- Headers ---" << COLOR_RESET << std::endl;
    if (response.headers.empty()) {
        std::cout << COLOR_RED << "No headers." << COLOR_RESET << std::endl;
    } else {
        for (std::map<std::string, std::string>::const_iterator it = response.headers.begin();
             it != response.headers.end(); ++it) {
            std::cout << it->first << ": " << COLOR_GREEN << it->second << COLOR_RESET << std::endl;
        }
    }
#if DEBUG_FLAG
    // Affichage du body (si présent)
    std::cout << COLOR_CYAN << "\n--- Body ---" << COLOR_RESET << std::endl;
    if (response.body.empty()) {
        std::cout << COLOR_RED << "No body." << COLOR_RESET << std::endl;
    } else {
        std::cout << COLOR_BLUE << response.body << COLOR_RESET << std::endl;
    }
    // Affichage de la réponse complète (optionnel)
    /*std::cout << COLOR_CYAN << "\n--- Full Response ---" << COLOR_RESET << std::endl;
    std::cout << COLOR_MAGENTA << response.toString() << COLOR_RESET << std::endl;*/

#endif 
    std::cout << COLOR_CYAN << "====================\n" << COLOR_RESET << std::endl;
}

/* ========================================================================== */
/*                                                                            */
/* ========================================================================== */

std::string readFullHttpRequest(int fd, EventLoop& loop) {
    std::string request;
    char buffer[4096];
    ssize_t bytes_read;
    bool headers_ended = false;
    size_t content_length = 0;
    size_t body_bytes_read = 0;

    while (true) {
        // Lire un morceau de données
        bytes_read = loop.readFromClient(fd, buffer, sizeof(buffer));
        if (bytes_read <= 0) {
            if (bytes_read == 0) {
                // Client a fermé la connexion
                std::cout << COLOR_RED << "Client disconnected (fd=" << fd << ")" << COLOR_RESET << std::endl;
            } else {
                // Erreur de lecture
                std::cerr << "Error reading from client (fd=" << fd << ")" << std::endl;
            }
            return ""; // Retourner une chaîne vide en cas d'erreur
        }

        // Ajouter les données lues à la requête
        request.append(buffer, bytes_read);

        // Si on n'a pas encore trouvé la fin des headers
        if (!headers_ended) {
            // Chercher \r\n\r\n (fin des headers)
            size_t header_end_pos = request.find("\r\n\r\n");
            if (header_end_pos != std::string::npos) {
                headers_ended = true;
                // Extraire Content-Length
                size_t content_length_pos = request.find("Content-Length: ");
                if (content_length_pos != std::string::npos) {
                    size_t start = content_length_pos + 16; // "Content-Length: " = 16 chars
                    size_t end = request.find("\r\n", start);
                    if (end != std::string::npos) {
                        std::string length_str = request.substr(start, end - start);
                        content_length = atoi(length_str.c_str());
                    }
                }
                // Si pas de Content-Length, on suppose que c'est une requête sans body (ex: GET)
                else {
                    content_length = 0;
                }
            }
        }

        // Si les headers sont terminés et qu'on a un Content-Length
        if (headers_ended) {
            // Calculer la taille du body déjà reçu
            size_t header_end_pos = request.find("\r\n\r\n");
            if (header_end_pos != std::string::npos) {
                body_bytes_read = request.size() - (header_end_pos + 4); // +4 pour \r\n\r\n
            }

            // Si on a reçu tout le body, on arrête
            if (body_bytes_read >= content_length) {
                break;
            }
        }
    }

    return request;
}
#endif

