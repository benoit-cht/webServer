
~~~cpp
/* ************************************************************************** */

static struct sockaddr_in  set_socket_addr(int port)
{
  struct sockaddr_in socket_addr;
  
  memset(&socket_addr, 0, sizeof(socket_addr));
  socket_addr.sin_family = AF_INET;
  socket_addr.sin_addr.s_addr = INADDR_ANY;
  socket_addr.sin_port = htons(port);
  
  return ( socket_addr );
}

static int setnonblocking(int socketFd)
{
    int flags = fcntl(socketFd, F_GETFL, 0); /*get the current flags*/
    if (flags == -1)
    {
        std::cerr << "fcntl failed to put the socket in non blocking mode" << std::endl;
        return (EXIT_FAILURE);
    }
    return (fcntl(socketFd, F_SETFL, flags | O_NONBLOCK)); /*add non blocking mode to the current flags*/
}

int SocketHandler::createServerSocket(int port) 
{
  int opt = 1;
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in socket_addr = set_socket_addr(port);
    
  if (socket_fd == -1)
    throw std::runtime_error("socket() " + std::string(strerror(errno)));
  if (setnonblocking(socket_fd)
  {
    std::cerr << COLOR_RED << "..." << COLOR_RESET << std::endl;
  }
  if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
  {
        close(socket_fd);
        throw std::runtime_error("setsockopt(SO_REUSEADDR) " + std::string(strerror(errno)));
  }
  if (bind(socket_fd, (struct sockaddr *)&socket_addr, sizeof(socket_addr)) == -1)
  {
    close(socket_fd);
    throw std::runtime_error("bind() " + std::string(strerror(errno)));
  }

  if (listen(socket_fd, SOMAXCONN) == -1) 
  {
    close(socket_fd);
    throw std::runtime_error("listen() " + std::string(strerror(errno)));
  }

  std::cout << COLOR_GREEN << "Success : open socket to a port: " << port << COLOR_RESET << std::endl;
  return( socket_fd );
}

/* ************************************************************************** */

~~~
