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
>[!important] _ lien pour retourner a index:_ [**00-index-wiki**](00-index-wiki.md)