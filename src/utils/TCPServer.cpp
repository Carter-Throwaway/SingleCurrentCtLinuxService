#include "TCPServer.hpp"

namespace linuxservice {

/**
 * Only constructor for TCPServer.
 * TCPServer spins up a TCP server on the port provided.
 * 
 * @param port int representing server's desired port
 */
TCPServer::TCPServer(int port) {
    m_domain = AF_INET; //AF_INET(IPv4) or AF_INET6(IPv6)
    m_commType = SOCK_STREAM; //SOCK_STREAM(TCP), UDP would be SOCK_DGRAM
    m_protocolVal = 0; //always "0" for IP
    m_port = port;
    m_maxSocketsWaitingToConnect = 1024; //from spec
    //Socket Options values:
    m_reuseAddrSocketOption = 1; //a zero value disables this socket option
    //Fill custom 'netinet/in.h' struct:
    m_address.sin_family = m_domain; //always the AF_INET domain family
    m_address.sin_addr.s_addr = INADDR_ANY; //localhost address, can accept both UDP and TCP
    m_address.sin_port = htons(m_port);

    //Step 1: create server socket
    if ((m_serverSocketDescriptor = socket(m_domain, m_commType, m_protocolVal)) == 0) { 
        std::cerr << "Server Socket Failure: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    //Step 2: establish my socket related options
    //SOL_SOCKET - specifies this option is for socket level, IPPROTO and IPPROTO6 for IPv4 and IPv6 level
    //SO_REUSEADDR - allows server to bind() to the same port multiple times as long as every invocation uses 
    //              a different local IP address and the wildcard address INADDR_ANY is used only one time per port
    if (setsockopt(m_serverSocketDescriptor, SOL_SOCKET, SO_REUSEADDR , &m_reuseAddrSocketOption, sizeof(m_reuseAddrSocketOption))) { 
        std::cerr << "Setting Server Socket Options Failure: " << strerror(errno) << std::endl; 
        exit(EXIT_FAILURE); 
    }

    //Step 3: binding the socket to the address & port
    if (bind(m_serverSocketDescriptor, (struct sockaddr *)&m_address, sizeof(m_address)) < 0) { 
        std::cerr << "Server Bind Failure: " << strerror(errno) << std::endl; 
        exit(EXIT_FAILURE); 
    }

    //Step 4: puts server socket in passive mode to wait ona  client connection
    if (listen(m_serverSocketDescriptor, m_maxSocketsWaitingToConnect) < 0) { 
        std::cerr << "Server Listen Failure: " << strerror(errno) << std::endl; 
        exit(EXIT_FAILURE); 
    }
}

/**
 * Getter for 'm_serverSocketDescriptor' member variable
 * 
 * @return int socket descriptor of server
 */
int TCPServer::getServerSocketDescriptor(){
    return m_serverSocketDescriptor;
}

/**
 * Getter for 'm_address' member variable
 * 
 * @return struct sockaddr_in representing server's address
 */
struct sockaddr_in TCPServer::getServerAddress(){
    return m_address;
}

}