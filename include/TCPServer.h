#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "Server.h"
#include "Admin.h"

#include <list>
#include <memory>
#include "TCPConn.h"

/******************************************************************************************
 *  TCPServer - Child of Server, Class hosts the TCPServer that TCPClients interact with
 *
 *  	   TCPServer(): Public Constructor
 *       ~TCPServer(Dest): Public Destructor
 *
 *  	   bindSvr - binds the server socketFD to a supplied IP address and port
 *       listenSvr - main function of server, manage and add/remove connections, accept whitelisted IP addresses
 *       shutdown - shut down the server
 * 
 *       Exceptions: throws socket exceptions for recoverable errors and runtime errors for non-recoverable errors 	   
 *
 *****************************************************************************************/

const int max_connections = 3;

class TCPServer : public Server 
{
public:
   TCPServer();
   ~TCPServer();

   void bindSvr(const char *ip_addr, unsigned short port);
   void listenSvr();
   void shutdown();

private:
   SocketFD _sockFD;
   std::list<std::unique_ptr<TCPConn>> _connlist;
   Admin _server_admin;
};

#endif
