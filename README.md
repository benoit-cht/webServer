*This project has been created as part of the curriculum by <bchallat> [. <tjacquel>, <lcluzan>].*

# Webserv

## Description
Webserv is a custom, non-blocking HTTP server from scratch in C++98. At its core, whenever a browser needs a file hosted on a web server, it requests the file vie HTTP. This software accepts the request via TCP sockets, finds or generates the requested document, and send it back to the client.

**Key Features:**
* Accepts connections from web browsers and CGI tools like `curl` or `telnet`.
* Fully understands and processes core HTTP methods (`GET`, `POST` and `DELETE`).
* Delivers accurate HTTP response status code (e.g., `200 OK`, `300 Multiple Choices`, `400 Bad Request`, `404 Not Found`, `500 Internal Server Error`).
* Serves static content including HTML, CSS and images (handling various media types).
* Manages complex behaviors via a custom `config.conf` file, supporting multiple virtual hosts (server delineated by IP or port).
* Implements CGI (Common Gateway Interface) to execute external scripts (like PHP or Python) by passing environments variables and managing `execve` forks.

---

## Instructions

* `make` : Compiles the `webserv` executable.
* `make clean` : Remove object files.
* `make fclean` : Removes object files and the executable.
* `make re` : Completely recompiles the project.

### Execution
Run the server with an explicit configuration file.
```bash
./webserv [path/to/config.conf]
```

### Testing the server
