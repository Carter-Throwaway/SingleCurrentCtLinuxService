#include "ConnectionManager.hpp"
#include "CountAPI.hpp"

#include <sys/time.h>
#include <sys/types.h>

namespace linuxservice {

/**
 * Only constructor for ConnectionManager.
 * ConnectionManager adds connection control between the passed TCPServer 
 * and the TCPServer's ultimate clients. ConnectionManager also ensures the
 * clients adhere to the InputCommands and OutputCommands defined by
 * the passed API. 
 * 
 * Note: The current ConnectionManager only handles a CountAPI. If adding new 
 * API handling to ConnectionManager search for "newApi" on this class.
 * 
 * @param serverSocket shared ptr to a TCPServer instance
 * @param api shared ptr to an API instance
 */
ConnectionManager::ConnectionManager(std::shared_ptr<TCPServer> serverSocket, std::shared_ptr<API> api) {
    m_pServerSocket = serverSocket;
    m_pApi = api;
    m_maxConnections = 1024; //defined by spec
    std::vector<int> m_connections;
}

/**
 * Adds any waiting clients to list of connections; checks all connections for data 
 * to read and performs API functionality.
 * 
 * This function is intended to be placed in a server loop.
 */
void ConnectionManager::handleConnections() {
    //Part 1: Polls server to accept new connections
    if(m_connections.size() < m_maxConnections) {
        int possibleNewSocketClient = pollServerForRead();
        if(possibleNewSocketClient > -1) {
            m_connections.push_back(possibleNewSocketClient);
            std::cout << "New Connection added. Connections: " << m_connections.size() << std::endl;
        }
    } else {
        std::cout << "Max Connection Capacity: Server skipped polling for new connections." << std::endl;
    }

    //Part 2: Polls clients for commands to read and handle
    std::vector<int> stillActiveConnections;
    for(int i = 0; i < m_connections.size(); i++){
        int currentClientSocketDescriptor = m_connections.at(i);
        //only polled sockets with successful reads/writes stay in active connections:
        if(pollClientForRead(currentClientSocketDescriptor)){
            stillActiveConnections.push_back(currentClientSocketDescriptor);
        } else {
            std::cout << "Connection removed." << std::endl;
        }
    }
    m_connections = stillActiveConnections; //ensures only active connections remain in the 'connections' list 
}

/**
 * Helper for 'handleConnections'.
 * Uses select() to poll the fd_set of server socket descriptor for waiting 
 * clients then attempts to connect them.
 * 
 * @return int representing new client for successful accpetance, -1 for unsuccessful.
 */
int ConnectionManager::pollServerForRead(){
    struct timeval timer; 
    fd_set readEvents;
    int selectReturn;

    int serverDescriptor = m_pServerSocket->getServerSocketDescriptor();
    timer.tv_sec = 0;
    timer.tv_usec = 0;

    FD_ZERO(&readEvents);
    FD_SET(serverDescriptor, &readEvents);

    // don't care about writefds and exceptfds for this instance:
    selectReturn = select((serverDescriptor+1), &readEvents, NULL, NULL, &timer);

    if (selectReturn == -1) {
        std::cerr << "Error in server select(): " << strerror(errno) << std::endl;
        return selectReturn;
    } else if (selectReturn > 0) {
        return(acceptConnections());
    } else {
        //Timeout
    }
    return -1;
}

/**
 * Helper for 'pollServerForRead'.
 * Attempts to accept a waiting client.
 * 
 * Note: Though there is confirmation that a client is in fact waiting, 
 * the client could drop off at any time.
 * 
 * @return int representing descriptor of now accepted client socket or -1 for failure.
 */
int ConnectionManager::acceptConnections(){
    struct sockaddr_in address(m_pServerSocket->getServerAddress());
    int addrlen = sizeof(address);
    int socketDescriptor;

    if ((socketDescriptor = accept(m_pServerSocket->getServerSocketDescriptor(), 
        (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) { 
        std::cerr << "New Client Acceptance Failed: " << strerror(errno) << std::endl;
    } else {
        //notifies client socket of server acceptance
        const char* toSend = "---Connection Accepted---\r\n";
		if(send(socketDescriptor , toSend , strlen(toSend) , 0 ) < 0){
            std::cerr << "New Client Send Failed: " << strerror(errno) << std::endl;
            socketDescriptor = -1;
        }
    }
    return socketDescriptor;
}

/**
 * Helper for 'handleConnections'.
 * Uses select() to poll the fd_set of passed socket descriptor for data then attempts read.
 * 
 * @param clientSocketDescriptor int that identifies a specific accepted client socket.
 * @return bool representing true for successful read, false for unsuccessful.
 */
bool ConnectionManager::pollClientForRead(int clientSocketDescriptor) {
    struct timeval timer; 
    fd_set readEvents; //could add writeEvents & exceptionEvents handling in future versions
    int selectReturn;

    timer.tv_sec = 0;
    timer.tv_usec = 0;

    FD_ZERO(&readEvents);
    FD_SET(clientSocketDescriptor, &readEvents);

    //writeEvents and exceptionEvents would replace the NULLS accordingly
    selectReturn = select((clientSocketDescriptor+1), &readEvents, NULL, NULL, &timer);

    if (selectReturn == -1) {
        std::cerr << "Error in client select(): " << strerror(errno) << std::endl;
        return false;
    } else if (selectReturn > 0) {
        return(readClientInput(clientSocketDescriptor));
    } else {
        //Timeout
    }

    return true;
}

/**
 * Helper for 'pollClientForRead'.
 * Attempts to read input from a client which was identified in select() call.
 * 
 * Note: Though there is confirmation that data is in fact available, 
 * the client could drop off at any time.
 * 
 * @param socketDescriptor int that identifies a specific accepted client socket.
 * @return bool representing true for successful I/O handling, false for unsuccessful.
 */
bool ConnectionManager::readClientInput(int socketDescriptor){
    int readReturn;
    const int bufferSize = 1024;
    char readBuffer[bufferSize] = {0};
    if((readReturn = read(socketDescriptor , readBuffer, bufferSize)) < 0){
        std::cerr << "Socket Read Failed: " << strerror(errno) << std::endl;
        return false;
    }
    //Passes the read contents from the client to the APIs supported by ConnectionManager:
    std::string apiType = m_pApi->getApiType();
    if(apiType == "CountAPI") {
        return handleInputForCountApi(socketDescriptor, readBuffer);
    } else if(apiType == "newAPI") {
        //Add new API command handling here e.g. handleInputForNewApi(socketDescriptor, readBuffer);
    } else {
        std::cerr << "The passed API is not supported by ConnectionManager." << std::endl;
    }
    return true;
}

/**
 * Helper for 'readClientInput'.
 * Passes the successfully read input into the correct API for handling.
 * 
 * Note: Though there is confirmation that read() was successful, 
 * the client could drop off at any time and fail the send().
 * 
 * @param socketDescriptor address for the socket descriptor.
 * @param readBuffer string passed from a successful read() associated with socketDescriptor.
 * @return bool representing true for successful I/O handling, false for unsuccessful.
 */
bool ConnectionManager::handleInputForCountApi(int& socketDescriptor, std::string readBuffer){
    std::string handledToSend = "Not a command handled by the server.\r\n";
    //can confidently cast here bc of the API type string check:
    CountAPI::InputCommand command = static_cast<CountAPI&>(*m_pApi).handleInCommand(readBuffer, handledToSend);
    if(command == CountAPI::INCR || command == CountAPI::DECR){
        sendToAllConnections(handledToSend);
    } else {
        const char* toSend = handledToSend.c_str();
        if((send(socketDescriptor , toSend , strlen(toSend) , 0 )) < 0){
            std::cerr << "Socket Send Failed: " << strerror(errno) << std::endl;
            return false;
        } else {
            std::cout << "Server sent message '" << handledToSend << "' ."<< std::endl;
        }
    }
    return true;
}

/**
 * Shutsdown all active connections.
 * 
 * Note: Though m_connections is constantly being polled/evaluated for 
 * dropped connections, it is possible a connection could drop at any time.
 */
void ConnectionManager::shutdownAllConnections() {
    for(int i = 0; i < m_connections.size(); i++){
         //file descriptor at i no longer read/writable but still open
        if((shutdown(m_connections.at(i), SHUT_RDWR)) < 0){
            std::cerr << "Failure shutting down a connection.: " << strerror(errno) << std::endl;
        }
    }
}

/**
 * Sends passed string to all active connections.
 * 
 * Note: Though m_connections is constantly being polled/evaluated for 
 * dropped connections, it is possible a connection could drop at any time.
 * 
 * @param sendToAll string intended to send to all connections.
 */
void ConnectionManager::sendToAllConnections(std::string sendToAll) {
    for(int i = 0; i < m_connections.size(); i++){
        if(send(m_connections.at(i) , sendToAll.c_str() , sendToAll.length() , 0 ) < 0){
            std::cerr << "Failure sending message to a socket descriptor: " << strerror(errno) << std::endl;
        }
    }
    std::cout << "Server sent message '" << sendToAll << "' to all active connections."<< std::endl;
        
}

}