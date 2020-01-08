#include "TCPClient.h"

#include <iostream>
#include <stdexcept>
#include "exceptions.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h> 

/**********************************************************************************************
 * TCPClient (constructor) - Creates a Stdin file descriptor to simplify handling of user input. 
 *
 **********************************************************************************************/

TCPClient::TCPClient() {
    this->client_socket = socket(AF_INET, SOCK_STREAM, 0);
}

/**********************************************************************************************
 * TCPClient (destructor) - No cleanup right now
 *
 **********************************************************************************************/

TCPClient::~TCPClient() {

}

/**********************************************************************************************
 * connectTo - Opens a File Descriptor socket to the IP address and port given in the
 *             parameters using a TCP connection.
 *
 *    Throws: socket_error exception if failed. socket_error is a child class of runtime_error
 **********************************************************************************************/

void TCPClient::connectTo(const char *ip_addr, unsigned short port) {
    this->connected = 0;
    
    if( this->client_socket < 0 )
        throw socket_error("ERROR! Client socket could not be opened.");
    
    this->server = gethostbyname( ip_addr );
    if( this->server == nullptr )
        throw socket_error("ERROR! Invalid server name.");
    
    bzero((char *) &this->server_address, sizeof(this->server_address)); //clean junk data
    this->server_address.sin_family = AF_INET; //ipv4
    bcopy((char *)this->server->h_addr, (char *)&this->server_address.sin_addr.s_addr, this->server->h_length); //copy server address pointer data to sockaddr_in
    this->server_address.sin_port = htons(port); //port int to network bytes
    
    if( connect(this->client_socket, (struct sockaddr *) &server_address, sizeof(server_address)) < 0 ) 
        throw socket_error("ERROR! Connection unable to establish.");
    this->connected = 1;
}

/**********************************************************************************************
 * handleConnection - Performs a loop that checks if the connection is still open, then 
 *                    looks for user input and sends it if available. Finally, looks for data
 *                    on the socket and sends it.
 * 
 *    Throws: socket_error for recoverable errors, runtime_error for unrecoverable types
 **********************************************************************************************/

void TCPClient::handleConnection() {
    if( this->connected == 0 )
        throw std::runtime_error("ERROR! Connected has not yet been established.");
    
    int msg = 0;
    char buffer[socket_bufsize];

    while(1)
    {
        msg = read(this->client_socket, buffer, socket_bufsize-1);
        if( msg < 0 ) 
            throw socket_error("ERROR! Server message could not be read.");
        std::cout << buffer << '\n';

        std::cout << ">> ";
        bzero(buffer, stdin_bufsize);
        std::cin >> buffer;
        if( !strcmp(buffer, "exit") )
            break;
        msg = write(this->client_socket, buffer, strlen(buffer));
        if( msg < 0 )
            throw socket_error("ERROR! Message could not be written.");
    }
}

/**********************************************************************************************
 * closeConnection - Calls the close command for the client_socket
 *
 *    Throws: socket_error for recoverable errors, runtime_error for unrecoverable types
 **********************************************************************************************/

void TCPClient::closeConn() {
    if( close(this->client_socket) < 0 )
        throw std::runtime_error("ERROR! Unable to close socket.");
}
