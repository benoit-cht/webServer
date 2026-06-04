#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <vector>
#include <map>

// Représente un bloc "location" (ex: location /kapouet { ... })
struct LocationConfig {
    std::string                 root;       // Dossier racine (ex: sources/www)
    std::string                 index;      // Fichier par défaut (ex: index.html)
    bool                        autoindex;  // Directory listing (true/false)
    std::vector<std::string>    methods;    // Méthodes autorisées (GET, POST...)
};

// Représente un bloc "server" entier
struct ServerConfig {
    int                                     port;
    std::string                             host;
    std::string                             serverName;
    std::map<int, std::string>              errorPages; // ex: 404 -> error404.html
    long                                    maxBodySize;
    std::map<std::string, LocationConfig>   locations;  // Clé = l'URL (ex: "/")
};

class ConfigParser {
  public:
    ConfigParser();
    ~ConfigParser();

    bool parseFile(const std::string& filename);
    const std::vector<ServerConfig>& getServers() const;
    LocationConfig parseLocationBlock(std::istringstream& iss);

  private:
    std::vector<ServerConfig> _servers; // Un fichier conf peut avoir plusieurs "server {}"
};

#endif