#include "TCPServer.h"

#include <iostream>
#include "exceptions.h"
#include <stdexcept>

/*
 * TCPServer::listenSvr() uses the modified algorithm from the website given below to
 * select which connection to handle at a given time and when to create a new connection.
 * https://www.geeksforgeeks.org/socket-programming-in-cc-handling-multiple-clients-on-server-without-multi-threading/
 */

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
    SocketFD sockFD;
    this->_sockFD = sockFD;
    this->_sockFD.setNonBlocking();
    this->_sockFD.bindFD( ip_addr, port );
}

/**********************************************************************************************
 * listenSvr - Performs a loop to look for connections and create TCPConn objects to handle
 *             them. Also loops through the list of connections and handles data received and
 *             sending of data. 
 *
 *    Throws: socket_error for recoverable errors, runtime_error for unrecoverable types
 **********************************************************************************************/

void TCPServer::listenSvr() {
    fd_set readfd;
    int activity, max_fd, fd;

    this->_sockFD.startListen();
    std::cout << "Maximum Connections Allowed: " << max_connections << std::endl; //Notify server admin

    while(true)
    {
        FD_ZERO( &readfd ); //Clear socket list
        FD_SET( this->_sockFD.getSockFD(), &readfd ); //Add server socket
        max_fd = this->_sockFD.getSockFD();

        //Add current connections to socket list
        for( auto const& connection : this->_connlist )
        {
            fd = connection->getConnFD();
            if( fd > 0 )
                FD_SET( fd , &readfd );
            if( fd > max_fd )
                max_fd = fd;
        }

        //Select connection to observe
        activity = select( max_fd + 1 , &readfd , NULL , NULL , NULL );

        if( ( activity < 0 ) && ( errno != EINTR ) )   
            throw std::runtime_error("ERROR! Could not select socket.");  

        if( FD_ISSET(this->_sockFD.getSockFD(), &readfd) )   
        {   
            if( this->_connlist.size() < max_connections ) //Allow connection to establish
            {
                std::unique_ptr<TCPConn> new_connection = TCPConn::New();
                if( !new_connection->acceptConn( this->_sockFD ) )
                    std::cout << "Blocked Nonwhitelisted IP" << std::endl;
                else
                {
                    new_connection->startAuthentication();
                    this->_connlist.push_back( std::move( new_connection ) );
                }
            }
            else //Refuse connection
            {
                std::unique_ptr<TCPConn> new_connection = TCPConn::New();
                new_connection->acceptConn( this->_sockFD );
                std::cout << "Maximum Connections Reached. New Connection Denied.\n";
                new_connection->disconnect();
            }
        }   

        for( auto connection = this->_connlist.begin(); connection != this->_connlist.end(); connection++ )
        {
            fd = (*connection)->getConnFD();
            if( FD_ISSET( fd, &readfd ) )
            {
                if( (*connection)->isConnected() ) //Performs a read and places input into buffer
                    (*connection)->handleConnection(); //Uses the buffer from last line
                else //Remove connection from _connlist
                {
                    auto new_it = connection;
                    ++new_it;
                    this->_connlist.remove( (*connection) );
                    connection = new_it; //Perform this operation to avoid undefined behavior
                }
            }
        }
    }
}

/**********************************************************************************************
 * shutdown - Cleanly closes the socket FD.
 *
 *    Throws: socket_error for recoverable errors, runtime_error for unrecoverable types
 **********************************************************************************************/

void TCPServer::shutdown() {
    this->_sockFD.shutdown();
}
