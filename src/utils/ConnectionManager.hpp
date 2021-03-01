#ifndef CONNECTIONMANAGER_HPP_
#define CONNECTIONMANAGER_HPP_

#include "API.hpp"
#include "TCPServer.hpp"
#include <iostream>
#include <vector>
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h>

namespace linuxservice {

class ConnectionManager {
public:
	ConnectionManager() = delete;
	~ConnectionManager() = default;
	ConnectionManager(const ConnectionManager&) = delete;
	ConnectionManager& operator=(const ConnectionManager&) = delete;

    ConnectionManager(std::shared_ptr<TCPServer> serverSocket, std::shared_ptr<API> api);

    void handleConnections();
    void shutdownAllConnections();

private:
    std::shared_ptr<TCPServer> m_pServerSocket;
    std::shared_ptr<API> m_pApi;
    int m_maxConnections;
    std::vector<int> m_connections;

    int pollServerForRead();
    bool pollClientForRead(int clientSocketDescriptor);
    bool readClientInput(int socketDescriptor);
    int acceptConnections();
    void sendToAllConnections(std::string sendToAll);
    bool handleInputForCountApi(int& socketDescriptor, std::string readBuffer);
};

}

#endif /* CONNECTIONMANAGER_HPP_ */