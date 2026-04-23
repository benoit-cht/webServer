#include "../../include/config/Config.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>

ConfigParser::ConfigParser() {}
ConfigParser::~ConfigParser() {}

// Supprime le ';' à la fin d'un mot s'il existe
static void cleanSemicolon(std::string& word) {
    if (!word.empty() && word[word.size() - 1] == ';') {
        word.erase(word.size() - 1);
    }
}

const std::vector<ServerConfig>& ConfigParser::getServers() const {
    return _servers;
}

bool ConfigParser::parseFile(const std::string& filename) {
    std::ifstream file(filename.c_str());
    if (!file.is_open()) {
        std::cerr << "Erreur: Impossible d'ouvrir le fichier " << filename << std::endl;
        return false;
    }

    // 1. Lire tout le fichier d'un coup
    std::ostringstream buffer;
    buffer << file.rdbuf();
    
    // 2. Transformer en flux de mots (Tokeniseur)
    std::istringstream iss(buffer.str());
    std::string word;

    // 3. Boucle principale
    while (iss >> word) {
        if (word == "server") {
            iss >> word; // On s'attend à lire '{'
            if (word == "{") {
                // TODO: Appeler une sous-fonction parseServerBlock(iss) qui lira tout le bloc jusqu'à la fermeture '}' et remplira un ServerConfig
                std::cout << "Un bloc serveur a été trouvé !" << std::endl;
            } else {
                return false; // Erreur de syntaxe
            }
        }
    }
    return true;
}

// Parse l'intérieur d'un bloc location { ... }
LocationConfig ConfigParser::parseLocationBlock(std::istringstream& iss) {
    LocationConfig loc;
    loc.autoindex = false; // Valeur par défaut
    std::string word;

    // On lit mot par mot jusqu'à trouver l'accolade fermante du bloc location
    while (iss >> word && word != "}") {
        if (word == "root") {
            iss >> loc.root;
            cleanSemicolon(loc.root);
        } 
        else if (word == "index") {
            iss >> loc.index;
            cleanSemicolon(loc.index);
        } 
        else if (word == "autoindex") {
            iss >> word;
            cleanSemicolon(word);
            loc.autoindex = (word == "on");
        }
        // ajouter allow_methods (GET, POST...)
    }
    return loc;
}