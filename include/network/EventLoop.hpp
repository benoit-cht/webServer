/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 06:29:31 by bchallat          #+#    #+#             */
/*   Updated: 2026/06/12 17:04:43 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENT_LOOP_HPP
#define EVENT_LOOP_HPP

#include <iostream>
#include <cstring>
#include <vector>
#include <poll.h>

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
    short             getPollEvent(int i) const;

  private:
    SocketHandler     _socket_handler;
    ClientManager     _client_manager;
    std::vector<int>  _server_fds;
    std::vector<int>  _ports;
    std::vector<struct pollfd> _poll_fds;
  
/* ************************************************************************** */
  public:
    size_t getPollFdsSize() const;
    const struct pollfd& getPollFd(size_t index) const;
    void queueResponse(int fd, const std::string& response);
    void handlePendingWrites(int fd); 
  private:
    std::map<int, std::string> _pendingResponses;  // Buffer des réponses
  
/* ************************************************************************** */
  private:
    // ...
    std::map<int, std::string> _clientBuffers;  // fd → données accumulées
    std::map<int, size_t> _clientContentLength; // fd → Content-Length attendu
    std::map<int, bool> _clientHeadersEnded;    // fd → headers terminés ?
public:
    // ...
    void appendToClientBuffer(int fd, const std::string& data);
    bool isRequestComplete(int fd);
    std::string getFullRequest(int fd);                                     //
};

#endif // !EVENT_LOOP_HPP

/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */
