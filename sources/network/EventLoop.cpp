/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcluzan <lcluzan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 06:01:32 by bchallat          #+#    #+#             */
/*   Updated: 2026/06/15 23:23:50 by lcluzan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <network/EventLoop.hpp>

EventLoop::EventLoop(const std::vector<int>& ports) : _ports(ports) {}

EventLoop::~EventLoop() { cleanup(); }

/* ************************************************************************** */

void EventLoop::setupServerSockets() 
{
  size_t index = 0;
  _server_fds = _socket_handler.createSocketWithPort(_ports);

  while ( index < _server_fds.size()) 
  {
    struct pollfd pfd;
    pfd.fd = _server_fds[index];
    pfd.events = POLLIN;
    _poll_fds.push_back(pfd);
    index++;
  }
}

/* ************************************************************************** */

int EventLoop::waitForActivity() 
{
  return (poll(&_poll_fds[0], _poll_fds.size(), -1));
}

std::vector<int> EventLoop::getActiveFds() const 
{
  size_t            index = 0;
  std::vector<int>  active;
  
  while( index < _poll_fds.size()) 
  {
    if (_poll_fds[index].revents & POLLIN) 
    {
      active.push_back(_poll_fds[index].fd);
    }
    index++;
  }
  
  return ( active );
}

bool EventLoop::isServerFd(int fd) const 
{
  for (size_t index = 0; index < _server_fds.size(); index++) 
  {
    if (_server_fds[index] == fd) 
    {
      return( true );
    }
  }

  return( false );
}

/* ************************************************************************** */

ClientInfo* EventLoop::acceptNewConnection(int server_fd) 
{
    std::string client_ip;
    int client_port;
    int client_fd = _socket_handler.acceptConnection(server_fd, client_ip, client_port);
    
    if (client_fd == -1) {

        return( NULL );
    }

    _client_manager.addClient(client_fd, client_ip, client_port);

    struct pollfd client_pollfd;
    client_pollfd.fd = client_fd;
    client_pollfd.events = POLLIN;
    _poll_fds.push_back(client_pollfd);

    std::cout << COLOR_CYAN << "(Log Client) : new connection accepted (fd=" << client_fd
              << ", IP=" << client_ip << ", Port client=" << client_port << ")" << COLOR_RESET << std::endl;

    return ( _client_manager.getClient(client_fd) );
}

/* ************************************************************************** */

ssize_t EventLoop::readFromClient(int fd, char* buffer, size_t size)
{
  ssize_t bytes_read = 0;
  ClientInfo* client = _client_manager.getClient(fd);
  if (!client) 
  {
    std::cerr << "Client non trouvé (fd=" << fd << ")." << std::endl;
    _socket_handler.closeSocket(fd);
    return( -1 );
  }

  memset(buffer, 0, size);
  bytes_read = _socket_handler.readFromSocket(fd, buffer, size - 1);
  
  if (bytes_read > 0) 
  {
    std::cout << COLOR_BLUE << "Données reçues de " << client->getAdressIp() << ":" 
              << client->getPortNumber() << " (" << bytes_read << " octets)" << COLOR_RESET << std::endl;
  }
  
  return bytes_read;
}

/* ************************************************************************** */

void EventLoop::sendResponse(int fd, const std::string& response)
{
  _socket_handler.writeToSocket(fd, response.c_str(), response.size());
}

void EventLoop::removeClient(int fd) 
{
  _client_manager.removeClient(fd);
  for (size_t i = 0; i < _poll_fds.size(); i++) 
  {
    if (_poll_fds[i].fd == fd) 
    {
      _poll_fds.erase(_poll_fds.begin() + i);
      break;
    }
  }
}

void EventLoop::cleanup() 
{
  const std::vector<ClientInfo*>& clients = _client_manager.getClients();
  for (size_t i = 0; i < clients.size(); i++) 
  {
    _client_manager.removeClient(clients[i]->getFileDescriptor());
  }
  for (size_t i = 0; i < _server_fds.size(); i++) 
  {
    _socket_handler.closeSocket(_server_fds[i]);
  }
  
}

// 1. Register the pipe so poll() starts watching it for POLLIN
void EventLoop::registerCgi(int client_fd, int cgi_read_fd, pid_t cgi_pid) {
    struct pollfd pfd;
    pfd.fd = cgi_read_fd;
    pfd.events = POLLIN; // Wait for the CGI script to write something
    pfd.revents = 0;
    _poll_fds.push_back(pfd);

    CgiState state;
    state.client_fd = client_fd;
    state.cgi_pid = cgi_pid;
    state.output_buffer = "";
    
    _cgi_read_fds[cgi_read_fd] = state;
}

// 2. Check if a triggered FD is a CGI pipe
bool EventLoop::isCgiReadFd(int fd) {
    return _cgi_read_fds.find(fd) != _cgi_read_fds.end();
}

// 3. Read the output and finish the response when the script ends
void EventLoop::handleCgiRead(int cgi_read_fd) {
    char buffer[4096];
    ssize_t bytes = read(cgi_read_fd, buffer, sizeof(buffer) - 1);

    if (bytes > 0) {
        // The script printed something! Save it in the buffer.
        buffer[bytes] = '\0';
        _cgi_read_fds[cgi_read_fd].output_buffer.append(buffer, bytes);
    } 
    else if (bytes == 0) {
        // EOF: The CGI script has finished executing and closed its stdout!
        CgiState state = _cgi_read_fds[cgi_read_fd];

        // 1. Kill the zombie process!
        int wstatus;
        waitpid(state.cgi_pid, &wstatus, 0); 

        // 2. Format the final HTTP Response
        std::stringstream ss;
        ss << "HTTP/1.1 200 OK\r\n";
        ss << "Content-Length: " << state.output_buffer.size() << "\r\n";
        ss << "\r\n"; // End of headers
        ss << state.output_buffer; // Append the CGI output

        // 3. Queue the full string for the client to download
        queueResponse(state.client_fd, ss.str());

        // 4. Cleanup the pipe and remove from poll
        close(cgi_read_fd);
        _cgi_read_fds.erase(cgi_read_fd);

        for (size_t i = 0; i < _poll_fds.size(); i++) {
            if (_poll_fds[i].fd == cgi_read_fd) {
                _poll_fds.erase(_poll_fds.begin() + i);
                break;
            }
        }
    } else {
        // Error reading from pipe. Cleanup and send 500 error.
        close(cgi_read_fd);
        queueResponse(_cgi_read_fds[cgi_read_fd].client_fd, "HTTP/1.1 500 Internal Server Error\r\n\r\n");
        _cgi_read_fds.erase(cgi_read_fd);
    }
}