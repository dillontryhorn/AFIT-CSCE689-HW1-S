#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "Server.h"
#include "Admin.h"

#include <list>
#include <memory>
#include "TCPConn.h"

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
