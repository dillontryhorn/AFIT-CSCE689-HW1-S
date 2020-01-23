#ifndef FILEDESC_H
#define FILEDESC_H

#include <netinet/in.h>

/******************************************************************************************
 *  SocketFD - helper class to provide socket file descriptor functionality for the TCP Server
 *
 *  	   SocketFD(): Public Constructor
 *         ~SocketFD(Dest): Public Destructor
 *
 *  	   setNonBlocking - modifies the socket to allow for non-blocking behavior
 *         bindFD - binds the socket file descriptor to a specified port and IP address
 *         connectTo - used for client to connect to a server
 *         startListen - starts listening for the server
 *         shutdown - Shuts down the server
 * 
 *         setSockFD - sets the file descriptor ID number for the global variable _sockFD
 *         getSockFD - returns the int value of the socket file descriptor
 *         setIPAddr - set the IP address of the FD
 *         getIPAddr - return the IP address struct of the FD
 * 
 *         Exceptions: throws socket exceptions for recoverable errors and runtime errors for non-recoverable errors 	   
 *
 *****************************************************************************************/

class SocketFD
{
public:
    SocketFD();
    ~SocketFD();

    void setNonBlocking();
    void bindFD(const char *ip_addr, short unsigned int port);
    void connectTo(const char *ip_addr, unsigned short port);
    void startListen();
    void shutdown();

    void setSockFD(int val);
    int getSockFD() { return _sockFD; };
    void setIPAddr(sockaddr_in ip_addr);
    sockaddr_in getIPAddr() { return _serv_addr; };
private:
    int _sockFD = -1;
    struct sockaddr_in _serv_addr;
};

#endif