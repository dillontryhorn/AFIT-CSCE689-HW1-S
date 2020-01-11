#include "TCPConn.h"

#include <iostream>
#include "exceptions.h"
#include <stdexcept>

TCPConn::TCPConn() {

}

TCPConn::~TCPConn() {

}

std::unique_ptr<TCPConn> TCPConn::New() {
    std::unique_ptr<TCPConn> connection = std::make_unique<TCPConn>();
    return std::move( connection );
}

bool TCPConn::acceptConn(SocketFD &server) {
    struct sockaddr_in address = server.getIPAddr();
    int addrlen = sizeof( address );
    int new_conn;
    if ( ( new_conn = accept( server.getSockFD(), (struct sockaddr *)&address, (socklen_t*)&addrlen ) ) < 0 )   
        return false;
    this->_connfd.setSockFD( new_conn );
    std::cout << "NEW CONNECTION SUCCESS, SocketFD: " << getConnFD();
    std::cout << ", IP: " << inet_ntoa(address.sin_addr);
    std::cout << ", Port: " << ntohs(address.sin_port) << std::endl;
    return true;
}

int TCPConn::sendText(const char *msg) {
    return 0;
}

int TCPConn::sendText(const char *msg, int size) {
    return 0;
}

void TCPConn::handleConnection() {
    //TCPConn::getUserInput( msg );
}

void TCPConn::startAuthentication() {
    //TO BE IMPLEMENTED IN HW2
}

void TCPConn::getUsername() {
    //TO BE IMPLEMENTED IN HW2
}

void TCPConn::getPasswd() {
    //TO BE IMPLEMENTED IN HW2
}

void TCPConn::sendMenu() {
    int msg = write(getConnFD(), "Welcome to Dillon Tryhorn's Server.\nList of commands:\nhello\nmenu\n1\n2\n3\n4\n5\npasswd\nexit", 87);
    if( msg < 0 )
        throw socket_error("ERROR! Message failed to send.");
}

void TCPConn::getMenuChoice() {
    /* int msg = -1;

    if( !strcmp(command, "hello") )
        msg = write(this->accept_socket, "Hello! I hope you are doing well.", 34);

    else if( !strcmp(command, "1") )
        msg = write(this->accept_socket, "Did you know? Many species of birds can fly.", 45);

    else if( !strcmp(command, "2") )
        msg = write(this->accept_socket, "My grandmother's name was Maureen.", 35);

    else if( !strcmp(command, "3") )
        msg = write(this->accept_socket, "My social security number is 347-MY-1337", 41);

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
 */
}

void TCPConn::setPassword() {
    //TO BE IMPLEMENTED IN HW2
}

void TCPConn::changePassword() {
    //TO BE IMPLEMENTED IN HW2
}

bool TCPConn::getUserInput(std::string &cmd) {
    return false;
}

void TCPConn::disconnect() {
    if( close( getConnFD() ) < 0 )
        throw std::runtime_error("ERROR! Unable to disconnect.");
}

bool TCPConn::isConnected() {
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int msg;
    if( (msg = read( getConnFD() , this->_buffer, 99) ) == 0 )   
    {   
        getpeername( getConnFD(), (struct sockaddr*)&address, (socklen_t*)&addrlen ); 
        std::cout << "Host disconnected. IP: " << inet_ntoa(address.sin_addr);
        std::cout << " Port: " << ntohs(address.sin_port) << std::endl;
        return false;
    }
    return true;
}
