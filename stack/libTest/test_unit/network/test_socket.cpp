// Dans EventLoop.cpp
void EventLoop::setupServerSockets(const std::vector<int>& ports) {
    _server_fds = _socket_handler.createServerSockets(ports);

    // Ajouter chaque socket serveur à _poll_fds
    for (size_t i = 0; i < _server_fds.size(); i++) {
        struct pollfd pfd;
        pfd.fd = _server_fds[i];
        pfd.events = POLLIN;
        _poll_fds.push_back(pfd);
    }
}

