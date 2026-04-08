#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#include "../../include/request/utils_HandlerHttp.hpp"

std::string readFile(const std::string& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);  // Ouvre en mode binaire et se place à la fin pour connaître la taille
    if (!file) {
        std::cerr << "Erreur: Impossible d'ouvrir le fichier " << path << std::endl;
        return "HTTP/1.1 404 Not Found\r\n"
               "Content-Type: text/plain\r\n"
               "Connection: close\r\n"
               "\r\n"
               "404 File Not Found";
    }

    // Retourne au début du fichier
    file.seekg(0, std::ios::beg);

    // Lit tout le contenu
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
