#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "Server.h"

#include <netinet/in.h>

const unsigned int socket_bufsize = 100;

class TCPServer : public Server 
{
public:
   TCPServer();
   ~TCPServer();

   void bindSvr(const char *ip_addr, unsigned short port);
   void listenSvr();
   void shutdown();
   void runCommand(const char *command);

private:
   int server_socket, accept_socket;
   struct sockaddr_in server_address, client_address;
   socklen_t client_length;
   char buffer[socket_bufsize];
};


#endif
