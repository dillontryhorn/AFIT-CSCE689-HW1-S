#include "TCPConn.h"

#include <iostream>
#include "exceptions.h"
#include <stdexcept>
#include <fstream>
#include <thread>
#include <chrono>
#include <sstream>

static const char* PASSWORD_FILENAME = "server_passwords.txt";

TCPConn::TCPConn() {
    std::ofstream password_file( PASSWORD_FILENAME, std::ios_base::app );
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
    TCPConn::sleep();
    if ( ( new_conn = accept( server.getSockFD(), (struct sockaddr *)&address, (socklen_t*)&addrlen ) ) < 0 )   
        return false; //Connection failed
    this->_connfd.setSockFD( new_conn ); //Set and print info to server admin
    TCPConn::sleep();
    this->_ip_client = inet_ntoa(address.sin_addr);
    if( this->_server_admin.checkWhitelistIP( inet_ntoa(address.sin_addr) ) )
    {
        std::cout << "NEW CONNECTION SUCCESS, SocketFD: " << getConnFD();
        std::cout << ", IP: " << inet_ntoa(address.sin_addr);
        std::cout << ", Port: " << ntohs(address.sin_port) << std::endl;
        std::stringstream ss;
        ss << "New Whitelisted Connection Success. Client IP address: " << TCPConn::getIPAddrStr() << " and Port: " << ntohs(address.sin_port) << std::endl;
        this->_server_admin.logger( ss.str() );
        return true;
    }
    else
    {
        std::cout << "Client is not whitelisted!" << std::endl;
        std::stringstream ss;
        ss << "Non-whitelisted Client Denied. Client IP address: " << TCPConn::getIPAddrStr() << " and Port: " << ntohs(address.sin_port) << std::endl;
        this->_server_admin.logger( ss.str() );
        TCPConn::disconnect();
    }
    return false;
}

int TCPConn::sendText(const char *msg) { //Simpler than method below
    TCPConn::sleep();
    int success = write( getConnFD(), msg, strlen(msg) );
    if( success < 0 )
        throw socket_error("ERROR! Message could not be written.");
    TCPConn::sleep();
}

int TCPConn::sendText(const char *msg, int size) { //More precise
    TCPConn::sleep();
    int success = write( getConnFD(), msg, size );
    if( success < 0 )
        throw socket_error("ERROR! Message could not be written.");
    TCPConn::sleep();
}

void TCPConn::handleConnection() {
    if( this->_status == s_username )
        TCPConn::getUsername();
    else if( this->_status == s_passwd )
        TCPConn::getPasswd();
    else if( this->_status == s_changepwd || this->_status == s_confirmpwd )
        TCPConn::changePassword();
    else if( this->_status == s_menu )
        TCPConn::getMenuChoice(); //Sends message back  
}

void TCPConn::startAuthentication() {
    TCPConn::sendText( "Enter your username", 20 );
}

void TCPConn::getUsername() {
    this->_username.assign(this->_buffer);
    this->_server_admin.checkUser( this->_username );
    if( this->_server_admin.checkUser( this->_username ) )
    {
        this->_status = s_passwd;
        TCPConn::sendText( "Enter your password", 20 );
    }
    else
    {
        std::stringstream ss;
        ss << this->_username << " is not recognized. IP: " << TCPConn::getIPAddrStr() << std::endl;
        this->_server_admin.logger( ss.str() );
        TCPConn::sendText( "User does not exist. Disconnecting.", 36 );
    }
}

void TCPConn::getPasswd() {
    std::string passwd(this->_buffer);
    bool passwd_found = false;
    if( this->_pwd_attempts <= max_attempts )
    {
        if( !this->_server_admin.checkPassword( this->_username, passwd ) )
            this->_pwd_attempts++;
        else
        {
            passwd_found = true;
            this->_status = s_menu;
            std::stringstream ss;
            ss << this->_username << " successfully logged in. IP: " << TCPConn::getIPAddrStr() << std::endl;
            this->_server_admin.logger( ss.str() );
        }
    }
    
    if( this->_pwd_attempts >= max_attempts )
    {
        std::stringstream ss;
        ss << this->_username << " failed to input correct password twice. IP: " << TCPConn::getIPAddrStr() << std::endl;
        this->_server_admin.logger( ss.str() );
        TCPConn::sendText( "Maximum password attempts reached. Disconnecting.", 50 );
    }
    else if( !passwd_found )
        TCPConn::sendText( "Invalid password. Try again.", 29 );

    if( passwd_found )
        TCPConn::sendMenu();
}

void TCPConn::sendMenu() {
    TCPConn::sendText( "Welcome to Dillon Tryhorn's Server.\nList of commands:\nhello\nmenu\n1\n2\n3\n4\n5\npasswd\nexit", 87 );
}

void TCPConn::getMenuChoice() { //All standard menu choices
    if( !strncmp(this->_buffer, "hello", 6) )
        TCPConn::sendText( "Hello! I hope you are doing well.", 34 );
    else if( !strncmp(this->_buffer, "1", 2) )
        TCPConn::sendText( "Did you know? Many species of birds can fly.", 45 );
    else if( !strncmp(this->_buffer, "2", 2) )
        TCPConn::sendText( "My grandmother's name was Maureen.", 35 );
    else if( !strncmp(this->_buffer, "3", 2) )
        TCPConn::sendText( "My social security number is 347-MY-1337", 41 );
    else if( !strncmp(this->_buffer, "4", 2) )
        TCPConn::sendText( "I wish to live in New York City one day and eat a big apple!", 61 );
    else if( !strncmp(this->_buffer, "5", 2) )
        TCPConn::sendText( "At a given time, I have $0 in my wallet.", 41 );
    else if( !strncmp(this->_buffer, "passwd", 7) )
        TCPConn::changePassword();
    else if( !strncmp(this->_buffer, "menu", 5) )
        TCPConn::sendText( "List of commands:\nhello\nmenu\n1\n2\n3\n4\n5\npasswd\nexit", 51 ); 
    else
        TCPConn::sendText( "INVALID COMMAND", 16 );
}

void TCPConn::changePassword() {
    if( this->_status == s_menu)
    {
        TCPConn::sendText( "Enter your new password: ", 26 );
        this->_status = s_changepwd;
    }
    else if( this->_status == s_changepwd )
    {
        this->_newpwd.assign( this->_buffer );
        TCPConn::sendText( "Confirm your new password: ", 28 );
        this->_status = s_confirmpwd;
    }
    else if( this->_status == s_confirmpwd )
    {
        if( this->_server_admin.hashPassword( this->_newpwd ).compare( this->_server_admin.hashPassword( this->_buffer ) ) == 0 )
        {
            this->_server_admin.changePassword( this->_username, this->_newpwd );
            TCPConn::sendText( "Password succesfully changed.", 30 );
        }
        else
        {
            TCPConn::sendText( "Passwords do not match. Nothing was changed.", 45 );
        }
        this->_status = s_menu;
    }
}

void TCPConn::disconnect() {
    std::stringstream ss;
    ss << "Disconnected " << this->_username << " at IP: " << TCPConn::getIPAddrStr() << std::endl;
    this->_server_admin.logger( ss.str() );
    if( close( getConnFD() ) < 0 )
        throw std::runtime_error("ERROR! Unable to disconnect.");
}

bool TCPConn::isConnected() {
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int msg;
    memset(&this->_buffer[0], 0, sizeof(this->_buffer)); //Make sure buffer is clear
    TCPConn::sleep();
    if( (msg = read( getConnFD() , this->_buffer, 99) ) == 0 ) //Read in buffer to see if connection is still open  
    {   
        getpeername( getConnFD(), (struct sockaddr*)&address, (socklen_t*)&addrlen ); //Display client info
        std::cout << "Host disconnected. IP: " << inet_ntoa(address.sin_addr);
        std::cout << " Port: " << ntohs(address.sin_port) << std::endl;
        std::stringstream ss;
        ss << this->_username << " disconnected. IP: " << inet_ntoa(address.sin_addr);
        ss << " Port: " << ntohs(address.sin_port) << std::endl;
        this->_server_admin.logger( ss.str() );
        return false;
    }
    TCPConn::sleep();
    return true;
}

void TCPConn::sleep() {
    std::this_thread::sleep_for( std::chrono::milliseconds(10) );
}
