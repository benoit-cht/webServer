# Nom de l'image Docker
IMAGE_NAME = webserv

# Port utilisé par le serveur
PORT = 8080

# Commandes Docker
.PHONY: build run stop clean fclean re

# Construire l'image Docker
build:
	docker build -t $(IMAGE_NAME) .

# Lancer le conteneur
run:
	docker run -it -p $(PORT):$(PORT) -v $$(pwd):/webserv $(IMAGE_NAME)

# Arrêter le conteneur en cours d'exécution
stop:
	docker stop $$(docker ps -q --filter ancestor=$(IMAGE_NAME))

# Nettoyer les conteneurs arrêtés
clean:
	docker rm $$(docker ps -aq --filter ancestor=$(IMAGE_NAME))

# Nettoyer les conteneurs + l'image
fclean: clean
	docker rmi $(IMAGE_NAME)

# Reconstruire et relancer le conteneur
re: clean fclean build run
	
