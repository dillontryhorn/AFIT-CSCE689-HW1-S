#include "TCPClient.h"

#include <iostream>
#include <stdexcept>
#include "exceptions.h"

/**********************************************************************************************
 * TCPClient (constructor) - Creates a Stdin file descriptor to simplify handling of user input. 
 *
 **********************************************************************************************/

TCPClient::TCPClient() {
    SocketFD stdin_fd;
    if( stdin_fd.getSockFD() < 0 )
        throw socket_error("ERROR! Client socket could not be opened.");
    this->_stdinFD = stdin_fd;
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
    this->_stdinFD.connectTo( ip_addr, port );
}

/**********************************************************************************************
 * handleConnection - Performs a loop that checks if the connection is still open, then 
 *                    looks for user input and sends it if available. Finally, looks for data
 *                    on the socket and sends it.
 * 
 *    Throws: socket_error for recoverable errors, runtime_error for unrecoverable types
 **********************************************************************************************/

void TCPClient::handleConnection() {
    int msg = 0;
    while(1)
    {
        if( (msg = read( this->_stdinFD.getSockFD(), this->_buffer, socket_bufsize-1 ) ) == 0 ) //Always read first
        {   
            std::cout << "Connection refused.\n";
            break;
        }
        if( msg < 0 ) 
            throw socket_error("ERROR! Server message could not be read.");
        std::cout << this->_buffer << '\n';

        std::cout << ">> ";
        memset( &this->_buffer[0], 0, sizeof(this->_buffer) ); //Make sure buffer is empty
        std::cin >> this->_buffer;
        if( !strcmp(this->_buffer, "exit") ) //Disconnecting
            break;
        msg = write( this->_stdinFD.getSockFD(), this->_buffer, strlen(this->_buffer) ); //Always write after read
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
    this->_stdinFD.shutdown();
}
