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
        return false; //Connection failed
    this->_connfd.setSockFD( new_conn ); //Set and print info to server admin
    std::cout << "NEW CONNECTION SUCCESS, SocketFD: " << getConnFD();
    std::cout << ", IP: " << inet_ntoa(address.sin_addr);
    std::cout << ", Port: " << ntohs(address.sin_port) << std::endl;
    return true;
}

int TCPConn::sendText(const char *msg) { //Simpler than method below
    int success = write( getConnFD(), msg, strlen(msg) );
    if( success < 0 )
        throw socket_error("ERROR! Message could not be written.");
}

int TCPConn::sendText(const char *msg, int size) { //More precise
    int success = write( getConnFD(), msg, size );
    if( success < 0 )
        throw socket_error("ERROR! Message could not be written.");
}

void TCPConn::handleConnection() {
    //Add authentication here later

    TCPConn::getMenuChoice(); //Sends message back  
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

void TCPConn::sendMenu() { //Banner
    int msg = write(getConnFD(), "Welcome to Dillon Tryhorn's Server.\nList of commands:\nhello\nmenu\n1\n2\n3\n4\n5\npasswd\nexit", 87);
    if( msg < 0 )
        throw socket_error("ERROR! Message failed to send.");
}

void TCPConn::getMenuChoice() { //All standard menu choices
    if( !strcmp(this->_buffer, "hello") )
        TCPConn::sendText("Hello! I hope you are doing well.", 34);

    else if( !strcmp(this->_buffer, "1") )
        TCPConn::sendText("Did you know? Many species of birds can fly.", 45);

    else if( !strcmp(this->_buffer, "2") )
        TCPConn::sendText("My grandmother's name was Maureen.", 35);

    else if( !strcmp(this->_buffer, "3") )
        TCPConn::sendText("My social security number is 347-MY-1337", 41);

    else if( !strcmp(this->_buffer, "4") )
        TCPConn::sendText("I wish to live in New York City one day and eat a big apple!", 61);

    else if( !strcmp(this->_buffer, "5") )
        TCPConn::sendText("At a given time, I have $0 in my wallet.", 41);

    else if( !strcmp(this->_buffer, "passwd") )
        TCPConn::sendText("Not implemented yet. Cannot change password.", 45);

    else if( !strcmp(this->_buffer, "menu") )
        TCPConn::sendText("List of commands:\nhello\nmenu\n1\n2\n3\n4\n5\npasswd\nexit", 51);
        
    else
        TCPConn::sendText("INVALID COMMAND", 16);
}

void TCPConn::setPassword() {
    //TO BE IMPLEMENTED IN HW2
}

void TCPConn::changePassword() {
    //TO BE IMPLEMENTED IN HW2
}

bool TCPConn::getUserInput(std::string &cmd) {
    return false; //Unused
}

void TCPConn::disconnect() {
    if( close( getConnFD() ) < 0 )
        throw std::runtime_error("ERROR! Unable to disconnect.");
}

bool TCPConn::isConnected() {
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int msg;
    memset(&this->_buffer[0], 0, sizeof(this->_buffer)); //Make sure buffer is clear
    if( (msg = read( getConnFD() , this->_buffer, 99) ) == 0 ) //Read in buffer to see if connection is still open  
    {   
        getpeername( getConnFD(), (struct sockaddr*)&address, (socklen_t*)&addrlen ); //Display client info
        std::cout << "Host disconnected. IP: " << inet_ntoa(address.sin_addr);
        std::cout << " Port: " << ntohs(address.sin_port) << std::endl;
        return false;
    }
    return true;
}
