/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcluzan <lcluzan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 06:29:31 by bchallat          #+#    #+#             */
/*   Updated: 2026/06/15 23:24:17 by lcluzan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENT_LOOP_HPP
#define EVENT_LOOP_HPP

#include <iostream>
#include <cstring> 
#include <vector>
#include <poll.h>
#include <sys/wait.h>
#include <map>
#include <sstream>

#include <network/SocketHandler.hpp>
#include <network/ClientManager.hpp>
#include <log/colorLog.hpp>

class EventLoop {

  public:
    EventLoop(const std::vector<int>& ports);
    ~EventLoop();

  public:
    void              setupServerSockets();
    void              cleanup();
    void              removeClient(int fd);
    void              sendResponse(int fd, const std::string& response);

  public:
    int               waitForActivity();
    bool              isServerFd(int fd) const;
    ssize_t           readFromClient(int fd, char* buffer, size_t size);
    ClientInfo*       acceptNewConnection(int server_fd);
    std::vector<int>  getActiveFds() const;

  public:
    void registerCgi(int client_fd, int cgi_read_fd, pid_t cgi_pid);
    bool isCgiReadFd(int fd);
    void handleCgiRead(int cgi_read_fd);

  private:
    SocketHandler     _socket_handler;
    ClientManager     _client_manager;
    std::vector<int>  _server_fds;
    std::vector<int>  _ports; 
    std::vector<struct pollfd> _poll_fds;

    private:
    struct CgiState {
        int         client_fd;
        pid_t       cgi_pid;
        std::string output_buffer;
    };

    std::map<int, CgiState> _cgi_read_fds;

};

#endif // !EVENT_LOOP_HPP

/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */
