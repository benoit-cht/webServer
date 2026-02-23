>[!important] _note _

# 1 Structure d’une requête HTTP

>[!info] Format suivie d’une requête HTTP
>~~~
>[Ligne de requête]
>[Headers]
>[Corps] (Optionnel, séparé par une ligne vide)
>~~~ 

_Exemple d’une requête HPPP:_
~~~ http
GET /index.html HTTP/1.1
Host: localhost:8080
User-Agent: curl/7.68.0
Accept: */*

[Ligne vide]
~~~
## Ligne de requête 

1. La premiere ligne contient la **Méthode** ( GET, POST, DELETE ).
2. La méthode est suivie du **Chemin** ( index.html ).
3. Et enfin elle ce termine avec la **Version** du protocole ( HTTP/1.1 ).

>[!info] voici un exemple`<Méthode><Chemin><Version HTTP>` 
>~~~ http
>GET /index.html http/1.1
>~~~

### Méthode HTTP supportées:

- GET : Récupérer une ressource (fichier statique ou CGI).
- POST : Envoyer des données (ex : formulaires).
- DELETE : Supprimer une ressource.
## Les ligne du header

| Header         | Description                                                                      |
| -------------- | -------------------------------------------------------------------------------- |
| Host           | Nom de domaine et port du serveur (ex : localhost:8080).                         |
| User-Agent     | Navigateur ou outil utilisé par le client (ex : curl/7.68.0).                    |
| Content-Type   | Type de contenu pour les requêtes POST (ex : application/x-www-form-urlencoded). |
| Content-Length | Taille du corps de la requête (pour POST)                                        |
| Content-Length | Taille du corps de la requête (pour POST).                                       |
| Connection     | Indique si la connexion doit être fermée après la réponse (ex : keep-alive).     |


>[!important] _ lien pour retourner a indexObs:_ [**index**](indexObs_WebServe)
