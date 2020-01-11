#include "FileDesc.h"

#include <stdexcept>
#include "exceptions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netdb.h> 

SocketFD::SocketFD() {
    this->_sockFD = socket( AF_INET, SOCK_STREAM, 0 );
    if( this->_sockFD < 0 )
        throw std::runtime_error("ERROR! SocketFD could not be opened.");
}

SocketFD::~SocketFD() {

}

void SocketFD::setNonBlocking() {
    int option = 1;
    if( setsockopt( this->_sockFD, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option,  sizeof(option) ) < 0 )   
        throw socket_error("ERROR! Unable to set SocketFD to nonblocking.");
}

void SocketFD::bindFD(const char *ip_addr, short unsigned int port) {
    bzero( (char *) &this->_serv_addr, sizeof(this->_serv_addr) ); //clean junk data
    this->_serv_addr.sin_family = AF_INET; //ipv4
    inet_aton( ip_addr, &this->_serv_addr.sin_addr ); //ip string to network bytes
    this->_serv_addr.sin_port = htons( port ); //port int to network bytes

    if( bind(this->_sockFD, (struct sockaddr *) &this->_serv_addr, sizeof(_serv_addr)) < 0)
        throw std::runtime_error("ERROR! SocketFD could not bind.");
}

void SocketFD::connectTo(const char *ip_addr, unsigned short port) {
    struct sockaddr_in server_address;
    struct hostent *server = nullptr;
    
    server = gethostbyname( ip_addr );
    if( server == nullptr )
        throw socket_error("ERROR! Invalid server name.");

    bzero( (char *) &server_address, sizeof(server_address) ); //clean junk data
    server_address.sin_family = AF_INET; //ipv4
    bcopy( (char *)server->h_addr, (char *)&server_address.sin_addr.s_addr, server->h_length ); //copy server address pointer data to sockaddr_in
    server_address.sin_port = htons( port ); //port int to network bytes
    
    if( connect( this->_sockFD, (struct sockaddr *) &server_address, sizeof(server_address) ) < 0 ) 
        throw socket_error("ERROR! Connection unable to establish.");
}

void SocketFD::startListen() {
    if( listen( this->_sockFD, 3 ) < 0 )
        throw std::runtime_error("ERROR! SocketFD is unable to listen.");
}

void SocketFD::shutdown() {
    if( close( this->_sockFD ) < 0 )
        throw std::runtime_error("ERROR! Unable to close SocketFD.");
}

void SocketFD::setSockFD(int val) {
    this->_sockFD = val;
}

void SocketFD::setIPAddr(sockaddr_in ip_addr) {
    this->_serv_addr = ip_addr;
}