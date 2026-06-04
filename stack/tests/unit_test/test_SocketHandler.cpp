#define CATCH_CONFIG_MAIN
#include "../include/catch.hpp"
#include <network/SocketHandler.hpp>
#include <sys/socket.h>
#include <unistd.h>

TEST_CASE("SocketHandler: createServerSocket", "[SocketHandler]") {
    SocketHandler handler;
    int port = 8080;

    SECTION("Create a valid server socket") {
        int fd = handler.createServerSocket(port);
        REQUIRE(fd > 0);
        handler.closeSocket(fd);
    }

    SECTION("Throw exception if port is already in use") {
        int fd1 = handler.createServerSocket(port);
        REQUIRE_THROWS_AS(handler.createServerSocket(port), std::runtime_error);
        handler.closeSocket(fd1);
    }
}

TEST_CASE("SocketHandler: closeSocket", "[SocketHandler]") {
    SocketHandler handler;
    int fd = handler.createServerSocket(8081);
    REQUIRE_NOTHROW(handler.closeSocket(fd));
}

TEST_CASE("SocketHandler: readFromSocket and writeToSocket", "[SocketHandler]") {
    SocketHandler handler;
    int fds[2];
    REQUIRE(socketpair(AF_UNIX, SOCK_STREAM, 0, fds) != -1);

    const char* msg = "Hello, World!";
    ssize_t written = handler.writeToSocket(fds[1], msg, strlen(msg));
    REQUIRE(written > 0);

    char buffer[1024] = {0};
    ssize_t read_bytes = handler.readFromSocket(fds[0], buffer, sizeof(buffer));
    REQUIRE(read_bytes > 0);
    REQUIRE(strcmp(buffer, msg) == 0);

    close(fds[0]);
    close(fds[1]);
}
