# Utilise une image Ubuntu officielle
FROM ubuntu:22.04

# Met à jour les paquets et installe les dépendances
RUN apt-get update && apt-get install -y \
    g++ \
    make \
    gdb \
    valgrind \
    # net-tools \
    # && rm -rf /var/lib/apt/lists/*

# Crée un répertoire de travail
# WORKDIR /webserv

# Copie les fichiers du projet dans le conteneur
# COPY . .

# Compile le projet (adapte la commande si nécessaire)
# RUN make

# Expose le port 8080
# EXPOSE 8080

# Commande par défaut pour lancer le serveur
# CMD ["./webserv"]

