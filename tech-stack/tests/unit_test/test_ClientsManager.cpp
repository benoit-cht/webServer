#include "../include/catch.hpp"
#include <network/ClientManager.hpp>
#include <network/ClientInfo.hpp>

TEST_CASE("ClientManager: Constructor and Destructor", "[ClientManager]") {
    SECTION("Constructor initializes empty structures") {
        ClientManager manager;
        REQUIRE(manager.getClients().empty());
    }

    SECTION("Destructor cleans up all clients") {
        ClientManager manager;
        manager.addClient(1, "192.168.1.1", 8080);
        manager.addClient(2, "192.168.1.2", 8081);
        REQUIRE_FALSE(manager.getClients().empty());
    } // Le destructeur est appelé ici et doit libérer la mémoire
}

TEST_CASE("ClientManager: addClient", "[ClientManager]") {
    ClientManager manager;

    SECTION("Adds a client to both vector and map") {
        manager.addClient(1, "192.168.1.1", 8080);

        REQUIRE_FALSE(manager.getClients().empty());
        REQUIRE(manager.getClient(1) != NULL);
        REQUIRE(manager.getClient(1)->getFileDescriptor() == 1);
        REQUIRE(manager.getClient(1)->getAdressIp() == "192.168.1.1");
        REQUIRE(manager.getClient(1)->getPortNumber() == 8080);
    }

    SECTION("Handles multiple clients") {
        manager.addClient(1, "192.168.1.1", 8080);
        manager.addClient(2, "192.168.1.2", 8081);

        REQUIRE(manager.getClients().size() == 2);
        REQUIRE(manager.getClient(1) != NULL);
        REQUIRE(manager.getClient(2) != NULL);
    }
}

TEST_CASE("ClientManager: removeClient", "[ClientManager]") {
    ClientManager manager;
    manager.addClient(1, "192.168.1.1", 8080);
    manager.addClient(2, "192.168.1.2", 8081);

    SECTION("Removes client from both vector and map") {
        manager.removeClient(1);

        REQUIRE(manager.getClient(1) == NULL);
        REQUIRE(manager.getClients().size() == 1);
    }

    SECTION("Does nothing if client does not exist") {
        manager.removeClient(99); // Client inexistant
        REQUIRE(manager.getClients().size() == 2);
    }
}

TEST_CASE("ClientManager: getClient", "[ClientManager]") {
    ClientManager manager;
    manager.addClient(1, "192.168.1.1", 8080);

    SECTION("Returns correct client") {
        ClientInfo* client = manager.getClient(1);
        REQUIRE(client != NULL);
        REQUIRE(client->getFileDescriptor() == 1);
    }

    SECTION("Returns NULL for non-existent client") {
        REQUIRE(manager.getClient(99) == NULL);
    }
}

TEST_CASE("ClientManager: getClients", "[ClientManager]") {
    ClientManager manager;
    manager.addClient(1, "192.168.1.1", 8080);
    manager.addClient(2, "192.168.1.2", 8081);

    SECTION("Returns reference to clients vector") {
        const std::vector<ClientInfo*>& clients = manager.getClients();
        REQUIRE(clients.size() == 2);
        REQUIRE(clients[0]->getFileDescriptor() == 1);
        REQUIRE(clients[1]->getFileDescriptor() == 2);
    }

    SECTION("Vector is const-correct") {
        const ClientManager const_manager;
        const std::vector<ClientInfo*>& clients = const_manager.getClients();
        REQUIRE(clients.empty()); // const_manager n'a pas de clients
    }
}
