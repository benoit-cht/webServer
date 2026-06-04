#include "../../include/request/RequestHttp.hpp"
#include <sstream>
#include <iostream>

// Supprime les espaces et \r\n au début et à la fin d'une chaîne
static std::string trimSpaces(const std::string& str) {
    size_t first = str.find_first_not_of(" \t");
    if (first == std::string::npos)
        return ""; // La chaîne est vide ou ne contient que des espaces

    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

// Découpe la première ligne : "GET /index.html HTTP/1.1"
bool RequestHttp::parseFirstLine(const std::string& line) {
    std::istringstream iss(line);
    std::string methodStr, versionStr;

    // Extraction automatique basée sur les espaces
    if (!(iss >> methodStr >> _link >> versionStr))
        return false; // Erreur de formatage

    // Conversion de la méthode (std::string vers int/enum)
    if (methodStr == "GET") _methode = GET;
    else if (methodStr == "POST") _methode = POST;
    else if (methodStr == "DELETE") _methode = DELETE;
    else _methode = UNKNOWN;

    // Conversion de la version (std::string vers double)
    if (versionStr == "HTTP/1.1") _version = 1.1;
    else if (versionStr == "HTTP/1.0") _version = 1.0;
    else _version = 0.0; // Version non supportée

    return true;
}

// Fonction d'entrée pour analyser toute la requête brute
bool RequestHttp::parse(const std::string& raw_request) {
    // Trouver la fin de la première ligne
    size_t endOfFirstLine = raw_request.find("\r\n");
    if (endOfFirstLine == std::string::npos) return false;

    // Analyser la première ligne
    std::string firstLine = raw_request.substr(0, endOfFirstLine);
    if (!parseFirstLine(firstLine)) return false;

    // Trouver la fin des headers (\r\n\r\n)
    size_t endOfHeaders = raw_request.find("\r\n\r\n");
    if (endOfHeaders == std::string::npos) return false; // Incomplet

    // Extraire le bloc de texte des headers et l'analyser
    // On commence après le premier \r\n (+2) jusqu'au double \r\n\r\n
    std::string headers_chunk = raw_request.substr(
        endOfFirstLine + 2,
        endOfHeaders - endOfFirstLine - 2
    );
	// Gestion du Body
    if (_contentLength > 0) {
        size_t bodyStart = endOfHeaders + 4; // On saute le \r\n\r\n

        // Vérifie si le buffer contient l'intégralité du Body
        if (raw_request.size() - bodyStart < static_cast<size_t>(_contentLength)) {
            return false; // Body incomplet, TCP fragmentation !
        }

        // Body complet, on l'extrait et on le sauvegarde
        _body = raw_request.substr(bodyStart, _contentLength);
    }

    return true;
}
}

// Analyse le bloc des headers et remplit la map et les attributs
bool RequestHttp::parseHeaders(const std::string& headers_chunk) {
    std::istringstream iss(headers_chunk);
    std::string line;

    // Lire ligne par ligne jusqu'à la fin du bloc
    while (std::getline(iss, line)) {
        size_t colon_pos = line.find(':');

        if (colon_pos != std::string::npos) {
            // Extraire la Clé et la Valeur
            std::string key = line.substr(0, colon_pos);
            std::string value = line.substr(colon_pos + 1);

            // Nettoyer la valeur et l'ajouter à la map
            _headers[key] = trimSpaces(value);

            // 4. Remplir vos attributs spécifiques pour un accès rapide
            if (key == "Host") _host = _headers[key];
            else if (key == "Content-Length") _contentLength = std::atoi(_headers[key].c_str());
            else if (key == "Content-Type") _contentType = _headers[key];
            else if (key == "Connection") _connection = _headers[key];
        }
    }
    return true;
}
