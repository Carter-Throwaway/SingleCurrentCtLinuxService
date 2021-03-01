#ifndef TCPSERVER_HPP_
#define TCPSERVER_HPP_

#include <iostream>

#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 

namespace linuxservice {

class TCPServer {
public:
	TCPServer() = delete;
	~TCPServer() = default;
	TCPServer(const TCPServer&) = delete;
	TCPServer& operator=(const TCPServer&) = delete;

    TCPServer(int port);

    int getServerSocketDescriptor();
    struct sockaddr_in getServerAddress();

private:
    int m_domain;
    int m_commType;
    int m_protocolVal;
    int m_serverSocketDescriptor;
    struct sockaddr_in m_address; //custom struct defined in netinet/in.h
    int m_port; //supplied by custom constructor
    int m_maxSocketsWaitingToConnect;

    //Socket Option Values:
    int m_reuseAddrSocketOption;
};

}

#endif /* TCPSERVER_HPP_ */