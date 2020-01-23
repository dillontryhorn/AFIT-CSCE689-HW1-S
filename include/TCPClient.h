#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include "Client.h"

#include "TCPConn.h"

/******************************************************************************************
 * TCPClient - Child class of Client, program that clients use to connect to the TCPServer
 *
 *  	   TCPClient(): Public Constructor
 *  	   ~TCPClient(Dest): Public Destructor
 *
 *       connectTo - connects to a socket using a supplied IP address and port
 *       handleConnection - communicates with the server socket to send and receive data
 *       closeConn() - disconnect from the server socket 	   
 *    
 *       Exceptions - throws socket exceptions for recoverable errors and runtime errors for non-recoverable errors 	   
 *
 *****************************************************************************************/

const unsigned int socket_bufsize = 100;

class TCPClient : public Client
{
public:
   TCPClient();
   ~TCPClient();

   virtual void connectTo(const char *ip_addr, unsigned short port);
   virtual void handleConnection();

   virtual void closeConn();

private:
   SocketFD _stdinFD;
   char _buffer[socket_bufsize];
};


#endif
