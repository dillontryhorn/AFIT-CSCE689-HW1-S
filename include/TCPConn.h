#ifndef TCPCONN_H
#define TCPCONN_H

#include "FileDesc.h"
#include "Admin.h"

#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h> 
#include <sys/socket.h>

const int max_attempts = 2;
const int bufsize = 100;

class TCPConn 
{
public:
   TCPConn();
   ~TCPConn();
   static std::unique_ptr<TCPConn> New();

   bool acceptConn(SocketFD &server);

   int sendText(const char *msg);
   int sendText(const char *msg, int size);

   void handleConnection();
   void startAuthentication();
   void getUsername();
   void getPasswd();
   void sendMenu();
   void getMenuChoice();
   void setPassword();
   void changePassword();
   void sleep();

   void disconnect();
   bool isConnected();

   sockaddr_in getIPAddr() { return _connfd.getIPAddr(); };
   int getConnFD() { return _connfd.getSockFD(); };

private:


   enum statustype { s_username, s_changepwd, s_confirmpwd, s_passwd, s_menu };

   statustype _status = s_username;

   SocketFD _connfd;
 
   std::string _username; // The username this connection is associated with

   std::string _newpwd; // Used to store user input for changing passwords

   char _buffer[bufsize];

   int _pwd_attempts = 0;

   Admin _server_admin;
};


#endif
