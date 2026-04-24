#include "include/catch.hpp"
#include <network/ClientInfo.hpp>

TEST_CASE("ClientInfo: Constructor and Getters", "[ClientInfo]") {
    // Données de test
    int fd = 42;
    std::string ip = "192.168.1.1";
    int port = 8080;

    SECTION("Constructor initializes members correctly") {
        ClientInfo client(fd, ip, port);

        // Vérifie que les getters retournent les valeurs attendues
        REQUIRE(client.getFileDescriptor() == fd);
        REQUIRE(client.getPortNumber() == port);
        REQUIRE(client.getAdressIp() == ip);
    }

    SECTION("Getters return correct values after construction") {
        ClientInfo client(fd, ip, port);

        // Vérifie que les valeurs ne changent pas entre les appels
        REQUIRE(client.getFileDescriptor() == fd);
        REQUIRE(client.getFileDescriptor() == fd); // Appel répété pour vérifier la cohérence
        REQUIRE(client.getPortNumber() == port);
        REQUIRE(client.getAdressIp() == ip);
    }

    SECTION("Const-correctness of getters") {
        const ClientInfo client(fd, ip, port);

        // Vérifie que les getters fonctionnent sur un objet const
        REQUIRE(client.getFileDescriptor() == fd);
        REQUIRE(client.getPortNumber() == port);
        REQUIRE(client.getAdressIp() == ip);
    }
}

TEST_CASE("ClientInfo: Destructor", "[ClientInfo]") {
    // Ce test vérifie que le destructeur ne lance pas d'exception
    // et que la destruction se passe bien.
    int fd = 42;
    std::string ip = "192.168.1.1";
    int port = 8080;

    SECTION("Destructor does not throw") {
        ClientInfo* client = new ClientInfo(fd, ip, port);
        REQUIRE_NOTHROW(delete client);
    }
}
