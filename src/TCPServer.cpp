#include "TCPServer.h"

#include <iostream>

#include <stdexcept>
#include "exceptions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h> 
#include <sys/socket.h>

//http://www.linuxhowtos.org/C_C++/socket.htm
//https://www.geeksforgeeks.org/socket-programming-in-cc-handling-multiple-clients-on-server-without-multi-threading/


TCPServer::TCPServer() {

}


TCPServer::~TCPServer() {

}

/**********************************************************************************************
 * bindSvr - Creates a network socket and sets it nonblocking so we can loop through looking for
 *           data. Then binds it to the ip address and port
 *
 *    Throws: socket_error for recoverable errors, runtime_error for unrecoverable types
 **********************************************************************************************/

void TCPServer::bindSvr(const char *ip_addr, short unsigned int port) {
    this->server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if( this->server_socket < 0 )
        throw std::runtime_error("ERROR! Server socket could not be opened.");

    //Allow multiple connections to server_socket
    int option = 1; //True
    if( setsockopt(this->server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option,  sizeof(option)) < 0 )   
        throw socket_error("ERROR! Server socket unable to change options.");   

    bzero((char *) &this->server_address, sizeof(this->server_address)); //clean junk data
    this->server_address.sin_family = AF_INET; //ipv4
    inet_aton(ip_addr, &this->server_address.sin_addr); //ip string to network bytes
    this->server_address.sin_port = htons(port); //port int to network bytes
    
    if( bind(this->server_socket, (struct sockaddr *) &this->server_address, sizeof(server_address)) < 0)
        throw std::runtime_error("ERROR! Server socket could not bind.");
}

/**********************************************************************************************
 * listenSvr - Performs a loop to look for connections and create TCPConn objects to handle
 *             them. Also loops through the list of connections and handles data received and
 *             sending of data. 
 *
 *    Throws: socket_error for recoverable errors, runtime_error for unrecoverable types
 **********************************************************************************************/

void TCPServer::listenSvr() {
    bzero(this->buffer, socket_bufsize);
    int msg = 0, max_sd;
    if( listen(this->server_socket, 3) < 0 )
        throw std::runtime_error("ERROR! Server socket is unable to listen.");
    this->client_length = sizeof(this->client_address);

    this->accept_socket = accept(this->server_socket, (struct sockaddr *) &this->client_address, &this->client_length);
    if( this->accept_socket < 0 )
        throw std::runtime_error("ERROR! Client failed to connect.");

    msg = write(this->accept_socket, "Welcome to Dillon Tryhorn's Server.\nList of commands:\nhello\nmenu\n1\n2\n3\n4\n5\npasswd\nexit", 87);
    if( msg < 0 )
        throw socket_error("ERROR! Message failed to send.");

    while(1)
    {
        bzero(this->buffer, socket_bufsize);
        msg = read(this->accept_socket, this->buffer, socket_bufsize-1);
        if( msg < 0 )
            throw socket_error("ERROR! Could not receive message.");
        TCPServer::runCommand( this->buffer );
    }
}

/**********************************************************************************************
 * shutdown - Cleanly closes the socket FD.
 *
 *    Throws: socket_error for recoverable errors, runtime_error for unrecoverable types
 **********************************************************************************************/

void TCPServer::shutdown() {
    if( close(this->accept_socket) < 0 || close(this->server_socket) < 0 )
        throw std::runtime_error("ERROR! Unable to close sockets.");
}

/**********************************************************************************************
 * runCommand - Takes a command char string and prints the appropriate output.
 *
 *    Throws: socket_error for recoverable errors, runtime_error for unrecoverable types
 **********************************************************************************************/

void TCPServer::runCommand(const char *command) {
    int msg = -1;

    if( !strcmp(command, "hello") )
        msg = write(this->accept_socket, "Hello! I hope you are doing well.", 34);

    else if( !strcmp(command, "1") )
        msg = write(this->accept_socket, "Did you know? Many species of birds can fly.", 45);

    else if( !strcmp(command, "2") )
        msg = write(this->accept_socket, "My grandmother's name was Maureen.", 35);

    else if( !strcmp(command, "3") )
        msg = write(this->accept_socket, "My social security number is 347-83-1337", 41);

    else if( !strcmp(command, "4") )
        msg = write(this->accept_socket, "I wish to live in New York City one day and eat a big apple!", 61);

    else if( !strcmp(command, "5") )
        msg = write(this->accept_socket, "At a given time, I have $0 in my wallet.", 41);

    else if( !strcmp(command, "passwd") )
        msg = write(this->accept_socket, "Not implemented yet. Cannot change password.", 45);

    else if( !strcmp(command, "menu") )
        msg = write(this->accept_socket, "List of commands:\nhello\nmenu\n1\n2\n3\n4\n5\npasswd\nexit", 51);

    else
        msg = write(this->accept_socket, "INVALID COMMAND", 16);

    if( msg < 0 )
        throw socket_error("ERROR! Message failed to send.");
}