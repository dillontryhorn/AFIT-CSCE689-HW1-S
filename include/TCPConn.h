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

/******************************************************************************************
 *  TCPConn - Helper class for TCPServer to help manage connections
 *    handles and receives/sends data, performs authentication, checks for alive connections
 *
 *  	   TCPConn(): Public Constructor
 *       ~TCPConn(Dest): Public Destructor
 *       New - creates a unique pointer of a connection for handling
 * 
 *       acceptConn - binds a client to the server socket
 *       
 *       sendText - sends text data to the user with size of data
 * 
 *       handleConnection - based on state of connection, directs client to proper stage of authentication
 *       startAuthentication - on startup, prompts for username, state -> s_username
 *       getUsername - takes username from buffer and prompts for password, state -> s_passwd
 *       getPassword - takes password from buffer and checks if the hash is correct, if not try again,
 *          disconnect after max attempts, state -> s_menu
 *       sendMenu - sends default command list to the client
 *       getMenuChoice - sends appropriate response to client based on input, invalid input prompts for "INVALID COMMAND"
 *       changePassword - allows user to change password with two matching inputs
 *       sleep - allows the server to sleep for a few milliseconds to lighten the load on the CPU
 *       
 *       disconnect - Disconnects the client from the server
 *       isConnected - verifies if a client is connected, takes incoming data into the buffer
 *       
 *       getIPAddr - get the IP address, either sockaddr_in or string
 *       getConnFD - gets the file descriptor integer ID
 *
 *       Exceptions: throws socket exceptions for recoverable errors and runtime errors for non-recoverable errors 	   
 *
 *****************************************************************************************/

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
   void changePassword();
   void sleep();

   void disconnect();
   bool isConnected();

   sockaddr_in getIPAddr() { return _connfd.getIPAddr(); };
   std::string getIPAddrStr() { return _ip_client; };
   int getConnFD() { return _connfd.getSockFD(); };

private:


   enum statustype { s_username, s_changepwd, s_confirmpwd, s_passwd, s_menu };

   statustype _status = s_username;

   SocketFD _connfd;
 
   std::string _username = "NO USER"; // The username this connection is associated with, Default is "NO USER"

   std::string _newpwd; // Used to store user input for changing passwords

   char _buffer[bufsize];

   int _pwd_attempts = 0;

   Admin _server_admin;

   std::string _ip_client;
};


#endif
