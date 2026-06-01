# Introduction 

~~~
[Requête HTTP Brute]
       │
       ▼
[Parsing] ──────────────────────► t_httpRequest (méthode, chemin, headers, corps)
       │
       ▼
[Routing] ──────────────────────► Décide du traitement (fichier/CGI/erreur)
       │
       ▼
[Traitement] ─┬─────► Fichier statique → Lire le fichier
       │      ├─────► CGI → Exécuter le script
       │      └─────► Erreur → Retourner un code 4xx/5xx
       │
       ▼
[Réponse HTTP] ←──────────────────────── t_httpResponse (status, headers, corps)
~~~
#
---
~~~ mermaid
sequenceDiagram
    participant Client
    participant Serveur
    participant CGI

    Client->>Serveur: POST /submit HTTP/1.1
    Client->>Serveur: Content-Type: application/x-www-form-urlencoded
    Client->>Serveur: Body: name=Benoit&age=25

    Serveur->>Serveur: Parse la requête (méthode, headers, body)
    Serveur->>Serveur: Vérifie si l'URL correspond à une route CGI
    alt Route CGI
        Serveur->>CGI: Exécute le script (ex: /cgi-bin/process_form.py)
        CGI->>CGI: Traite les données (name, age)
        CGI-->>Serveur: Retourne une réponse (ex: 200 OK + HTML)
    else Route statique
        Serveur-->>Client: 405 Method Not Allowed (POST non autorisé)
    end
    Serveur-->>Client: Réponse HTTP (statut + contenu)
~~~
# 
---
>[!important] _ lien pour retourner a index:_ [**00-index-wiki**](00-index-wiki.md)