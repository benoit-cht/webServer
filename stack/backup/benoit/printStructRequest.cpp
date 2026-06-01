static void print_http_request(const t_httpRequest& request) {
    // Affichage de la méthode, du chemin et de la version
    std::cout << COLOR_CYAN << "\n=== HTTP Request ===" << COLOR_RESET << std::endl;
    std::cout << "Method: " << COLOR_YELLOW << request.method << COLOR_RESET << std::endl;
    std::cout << "Path: " << COLOR_YELLOW << request.path << COLOR_RESET << std::endl;
    std::cout << "Version: " << COLOR_YELLOW << request.version << COLOR_RESET << std::endl;

    // Affichage des en-têtes
    std::cout << COLOR_CYAN << "\n--- Headers ---" << COLOR_RESET << std::endl;
    if (request.headers.empty()) {
        std::cout << COLOR_RED << "No headers." << COLOR_RESET << std::endl;
    } else {
        for (std::map<std::string, std::string>::const_iterator it = request.headers.begin();
             it != request.headers.end(); ++it) {
            std::cout << it->first << ": " << COLOR_GREEN << it->second << COLOR_RESET << std::endl;
        }
    }

    // Affichage du body (si présent)
    std::cout << COLOR_CYAN << "\n--- Body ---" << COLOR_RESET << std::endl;
    if (request.body.empty()) {
        std::cout << COLOR_RED << "No body." << COLOR_RESET << std::endl;
    } else {
        std::cout << COLOR_BLUE << request.body << COLOR_RESET << std::endl;
    }

    std::cout << COLOR_CYAN << "===================\n" << COLOR_RESET << std::endl;
}
