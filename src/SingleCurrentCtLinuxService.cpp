/*
 * SingleCurrentCtLinuxService.cpp
 * 
 * Establishes a TCP server on the passed port and accepts 'count' commands 
 * defined by a third party specification.
 */

#include "utils/TCPServer.hpp"
#include "utils/CountAPI.hpp"
#include "utils/ConnectionManager.hpp"

#include <iostream>

bool noSIGTERM = true;

void signalHandler(int signal) {
	std::cout << "Received signal: " << signal << std::endl;
	noSIGTERM = false;
}

int main(int argc, char const *argv[]) { 
	//Installs my custom SIGTERM signal handling
    signal(SIGTERM, signalHandler);

	int port;

	//Get port value from argv
	if(argc != 2){
		std::cerr << "No 'port' argument provided." << std::endl;
		return 0;
	} else {
		try { port = std::stoi(argv[1]); } 
		catch (const std::exception& e) {
			std::cerr << "Port value is an integer. " << e.what() << std::endl;
			return 0;
		}
	}

	std::shared_ptr<linuxservice::TCPServer> pServerSocket(new linuxservice::TCPServer(port));
	std::shared_ptr<linuxservice::API> pCountApi(new linuxservice::CountAPI());
	linuxservice::ConnectionManager connectionManager(pServerSocket, pCountApi);

	//Begins server loop for accepting new connections and handling active connections
	while(noSIGTERM){
		connectionManager.handleConnections();
	}

	std::cout << "Shutting down all connections..." << std::endl;
	connectionManager.shutdownAllConnections();
    
	std::cout << "Exit main()" << std::endl; //TO REMOVE: here to help me keep track of my SIGTERM handling for now
    return 0; 
} 