#include <sys/socket.h>  // Pour les sockets
#include <netinet/in.h>  // Pour sockaddr_in
#include <unistd.h>      // Pour close(), read(), write()
#include <poll.h>        // Pour poll()
#include <fcntl.h>       // Pour fcntl() (optionnel, pour non-blocking)
#include <cstring>       // Pour memset, strerror
#include <iostream>      // Pour les logs (optionnel)
#include <arpa/inet.h>   // Pour inet_ntoa (affichage des IPs)

#define PORT 8080        // Port d'écoute du serveur
#define MAX_CLIENTS 10   // Nombre max de clients simultanés
#define BUFFER_SIZE 4096  // Taille du buffer pour lire les requêtes

int createServerSocket(int port) {
    // 1. Créer le socket TCP
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket() failed");
        exit(EXIT_FAILURE);
    }

    // 2. Configurer l'adresse du serveur
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;  // Écouter sur toutes les interfaces
    server_addr.sin_port = htons(port);        // Convertir le port en network byte order

    // 3. Associer le socket à l'adresse et au port
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind() failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 4. Mettre le socket en mode écoute
    if (listen(server_fd, MAX_CLIENTS) == -1) {
        perror("listen() failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "Serveur démarré sur le port " << port << std::endl;
    return server_fd;
}

void runServer(int server_fd) {
    // 1. Initialiser la structure pollfd pour surveiller les sockets
    struct pollfd fds[MAX_CLIENTS + 1];  // +1 pour le socket serveur
    fds[0].fd = server_fd;
    fds[0].events = POLLIN;  // Surveiller les nouvelles connexions

    // Initialiser les autres slots à -1 (inutilisés)
    for (int i = 1; i <= MAX_CLIENTS; i++) {
        fds[i].fd = -1;
    }

    while (true) {
        // 2. Attendre une activité sur les sockets (timeout infini)
        int ret = poll(fds, MAX_CLIENTS + 1, -1);
        if (ret == -1) {
            perror("poll() failed");
            break;
        }

        // 3. Vérifier si une nouvelle connexion arrive
        if (fds[0].revents & POLLIN) {
            // Accepter la nouvelle connexion
            struct sockaddr_in client_addr;
            socklen_t client_addr_len = sizeof(client_addr);
            int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
            if (client_fd == -1) {
                perror("accept() failed");
            } else {
                std::cout << "Nouvelle connexion depuis " << inet_ntoa(client_addr.sin_addr) << std::endl;

                // Trouver un slot libre dans fds
                int i;
                for (i = 1; i <= MAX_CLIENTS; i++) {
                    if (fds[i].fd == -1) {
                        fds[i].fd = client_fd;
                        fds[i].events = POLLIN;  // Surveiller les données entrantes
                        break;
                    }
                }

                if (i == MAX_CLIENTS + 1) {
                    std::cerr << "Trop de clients, connexion refusée." << std::endl;
                    close(client_fd);
                }
            }
        }

        // 4. Traiter les requêtes des clients existants
        for (int i = 1; i <= MAX_CLIENTS; i++) {
            if (fds[i].fd != -1 && fds[i].revents & POLLIN) {
                // Lire la requête du client
                char buffer[BUFFER_SIZE];
                memset(buffer, 0, BUFFER_SIZE);
                ssize_t bytes_read = read(fds[i].fd, buffer, BUFFER_SIZE - 1);

                if (bytes_read <= 0) {
                    // Le client a fermé la connexion ou une erreur s'est produite
                    std::cout << "Client déconnecté." << std::endl;
                    close(fds[i].fd);
                    fds[i].fd = -1;
                } else {
                    // Afficher la requête brute (pour débogage)
                    std::cout << "Requête reçue (" << bytes_read << " octets) :\n" << buffer << std::endl;

                    // Ici, tu pourrais appeler une fonction pour parser la requête HTTP.
                    // Mais pour l'instant, on s'arrête avant le parsing.
                }
            }
        }
    }

    // 5. Fermer le socket serveur
    close(server_fd);
}

int main() {
    int server_fd = createServerSocket(PORT);
    runServer(server_fd);
    return 0;
}

