#ifndef FILEDESC_H
#define FILEDESC_H

#include <netinet/in.h>

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