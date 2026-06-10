/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response_HttpHandler.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ton_utilisateur_42 <ton_email@student.42.  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 12:20:23 by ton_utilisate     #+#    #+#             */
/*   Updated: 2026/06/10 11:41:33 by bchallat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>   // Pour std::ifstream
#include <sstream>   // Pour std::stringstream

#include <request/HttpHandler.hpp>

/* ========================================================================== */
/*         -- CONSTRUCTOR/DESTRUCTOR && METHODE OF STRUCT RESPONSE --         */
/* ========================================================================== */

// Constructeur par défaut
t_httpResponse::t_httpResponse()
    : status(200), body("") {
    headers.insert(std::make_pair("Content-Type", "text/html"));
}
// Constructeur paramétré
t_httpResponse::t_httpResponse(int status, const std::map<std::string, std::string>& headers, const std::string& body)
    : status(status), headers(headers), body(body) {}

/* ========================================================================== */

// Méthode pour convertir la réponse en chaîne HTTP brute
std::string t_httpResponse::toString() const {
    // std::string response;
	std::stringstream	ss;

    // Ligne de statut (ex: "HTTP/1.1 200 OK")
	// std::stringstream	ss;
	// ss << "HTTP/1.1 " << status << " ";
	// response += ss.str();
	ss << "HTTP/1.1 " << status << " ";
    // response += "HTTP/1.1 " + to_string(status) + " ";
    switch (status) {
        // case 200: response += "OK"; break;
        // case 400: response += "Bad Request"; break;
        // case 404: response += "Not Found"; break;
        // case 501: response += "Not Implemented"; break;
        // default: response += "Unknown Status"; break;
		case 200: ss <<  "OK"; break;
        case 400: ss <<  "Bad Request"; break;
        case 404: ss <<  "Not Found"; break;
        case 501: ss <<  "Not Implemented"; break;
        default:  ss << "Unknown Status"; break;
    }
	ss << "\r\n";
    // response += "\r\n";

    // En-têtes
    for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it) {
    // response += it->first + ": " + it->second + "\r\n";
	ss << it->first << ": " << it->second << "\r\n";
}

    // Body (si présent)
    if (!body.empty()) {
		// ss << "Content-Length: " << body.size() << "\r\n";
        // response += "Content-Length: " + std::to_string(body.size()) + "\r\n";
		// response += ss.str();
		ss << "Content-Length: " << body.size() << "\r\n";
    }
    // response += "\r\n";
    // response += body;
	ss << "\r\n" << body;

    // return response;
	return ss.str();
}

/* ========================================================================== */
/*                 -- SERV STATIC FILE IMPLEMENT FONCTION  --                 */
/* ========================================================================== */

bool isSafePath(const std::string& path) 
{
  return (path.find("../") == std::string::npos); // Empêche les chemins relatifs (comme "../")
}

std::string getMimeType(const std::string& path) 
{
    // Implémentez la logique pour déterminer le type MIME
    if (path.find(".html") != std::string::npos) return "text/html";
    if (path.find(".css") != std::string::npos) return "text/css";
    // Ajoutez d'autres types MIME selon les besoins
    return ("text/plain");
}

t_httpResponse HttpHandler::serveStaticFile(const std::string& path) 
{
    if (!isSafePath(path))
    {
      return (HandlerErrorHttp(403));
    }
    
    std::ifstream file(path.c_str(), std::ios::binary);
    if (!file.is_open())
    {
      std::cout << COLOR_MAGENTA << path << ":" << COLOR_RESET << std::endl;
      return (HandlerErrorHttp(404));
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    t_httpResponse response;
    response.status = 200;
    response.headers["Content-Type"] = getMimeType(path);
    response.body = content;
    
    return (response);
}

/* ========================================================================== */
/*                              -- TOOLS --                                   */
/* ========================================================================== */

std::string HttpHandler::readFile(const std::string& filepath) 
{
    std::ifstream       file(filepath.c_str());
    std::ostringstream  ss;

    if (!file.is_open()) 
    {
        return ""; // Le fichier n'existe pas ou n'a pas les droits de lecture
    }
    ss << file.rdbuf();
    
    return (ss.str());
}

/* ========================================================================== */

bool HttpHandler::isStaticFile(const std::string& path)
{
    if (path.find("/static/") == 0 ||
        path.substr(path.find_last_of(".") + 1) == "html" ||
        path.substr(path.find_last_of(".") + 1) == "css" ||
        path.substr(path.find_last_of(".") + 1) == "js") {
        return true;
    }
    return false;
}

/* ========================================================================== */
/*                          -- tools of POST METHODE  --                      */
/* ========================================================================== */

/* ========================================================================== */
/*                     -- Clean Multipart Body Function --                   */
/* ========================================================================== */

static void cleanMultipartBody(t_post_methode* postData) {
    
    std::cout << COLOR_CYAN << postData->body << COLOR_RESET << std::endl;
    // Vérifier si le boundary est présent
    if (postData->boundary.empty()) {
        // Si ce n'est pas une requête multipart, le body est déjà propre
        std::cout << COLOR_CYAN << "-return 0" << COLOR_RESET << std::endl;
        return;
    }

    // Construire les délimiteurs de boundary
    std::string boundaryStart = "--" + postData->boundary;
    std::string boundaryEnd = boundaryStart + "--";

    // Trouver la première occurrence du boundary
    size_t start = postData->body.find(boundaryStart);
    if (start == std::string::npos) {
      std::cout << COLOR_CYAN << "- return 1" + boundaryStart << start << COLOR_RESET << std::endl;
        return; // Boundary non trouvé
    }

    // Trouver le début du contenu du fichier (après les headers de la partie)
    start = postData->body.find("\r\n\r\n", start);
    if (start == std::string::npos) {
        return; // Headers de la partie non trouvés
    }
    start += 4; // Sauter "\r\n\r\n"

    // Trouver la fin du contenu du fichier (prochain boundary ou boundary de fin)
    size_t end = postData->body.find(boundaryStart, start);
    if (end == std::string::npos) {
        end = postData->body.find(boundaryEnd, start);
        if (end == std::string::npos) {
    std::cout << COLOR_CYAN << "-return 2 "<< COLOR_RESET << std::endl;
            return; // Boundary de fin non trouvé
        }
    }

    // Extraire le contenu du fichier
    std::string fileContent = postData->body.substr(start, end - start);

    // Nettoyer les espaces ou sauts de ligne en trop au début et à la fin
    size_t contentStart = fileContent.find_first_not_of("\r\n");
    if (contentStart != std::string::npos) {
        fileContent = fileContent.substr(contentStart);
    }
    size_t contentEnd = fileContent.find_last_not_of("\r\n");
    if (contentEnd != std::string::npos) {
        fileContent = fileContent.substr(0, contentEnd + 1);
    }

    // Mettre à jour le body avec le contenu nettoyé
    postData->body = fileContent;
    std::cout << COLOR_CYAN << "->>" + fileContent << COLOR_RESET << std::endl;
}

t_post_methode HttpHandler::post_parse_header_request(t_httpRequest request)
{
  size_t          pos_boundary = 0;
  t_post_methode  parsing;
  
  // find alength of body 
  if (!request.headers["Content-Length"].empty()) {

    parsing.bodyLength = request.headers["Content-Length"];
    std::cout << COLOR_MAGENTA << "bodyLength : " << parsing.bodyLength  << COLOR_RESET << std::endl; 
  }

  //find a boundary wtchi headers Content-Type 
  pos_boundary = request.headers["Content-Type"].find("boundary=");
  if (pos_boundary != 0) {
    
    parsing.boundary = request.headers["Content-Type"].substr(pos_boundary + 9);
    std::cout << COLOR_MAGENTA << "boundary : " << parsing.boundary << COLOR_RESET <<std::endl; 
  }

  // create a map of hedaer 
  for (size_t start = 0, end = 0; start + end < request.body.size(); start += end) {

    size_t  key_pos = request.body.find("Content-", start);
    size_t  ope_pos = request.body.find(":", key_pos);
    end = request.body.find("\n", key_pos);

    std::string key = request.body.substr(key_pos, ope_pos - key_pos);
    std::string value = request.body.substr(ope_pos, end - ope_pos);

    parsing.headers[key] = value;
    std::cout << COLOR_MAGENTA << key << parsing.headers[key] << COLOR_RESET << std::endl;
    
  }
    // a name file 
    std::string str = parsing.headers["Content-Disposition"];
    size_t filename_pos = str.find("filename=\"");
    if (filename_pos != std::string::npos) {

        size_t start_quote = filename_pos + 10; // 9 = longueur de "filename=\""
        size_t end_quote = str.find("\"", start_quote);

        if (end_quote != std::string::npos) {

            parsing.nameFile = str.substr(start_quote, end_quote - start_quote);
            std::cout << COLOR_MAGENTA << "Nom du fichier : " << parsing.nameFile << parsing.nameFile.length() << COLOR_RESET << std::endl;

        } else {
          
          std::cerr << "Erreur : Guillemet fermant manquant." << std::endl;
        }
    } else {
        
      std::cerr << "Erreur : 'filename=\"' non trouvé dans Content-Type." << std::endl;
    }

 /*
  // clear body 
  parsing.body = request.body;
  parsing.body.erase(parsing.body.find(parsing.boundary), parsing.boundary.length());
  parsing.body.erase(parsing.body.find(parsing.boundary), parsing.boundary.length());
  for (std::map<std::string, std::string>::iterator it = parsing.headers.begin(); it != parsing.headers.end(); ++it) {

    parsing.body.erase(parsing.body.find(it->first), it->first.length());
    parsing.body.erase(parsing.body.find(it->second), it->second.length());

  }
  */
    std::cout << COLOR_CYAN << request.body << COLOR_RESET << std::endl;
  parsing.body = request.body;
  cleanMultipartBody(&parsing);
  return (parsing);
}

/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */

