#ifndef ADMIN_H
#define ADMIN_H

#include <string>
#include <fstream>

class Admin 
{
    public:
        Admin() = default;
        ~Admin() = default;

        void addWhitelistIP( std::string ip_addr );
        void removeWhitelistIP( std::string ip_addr );

        void addUser( std::string username, std::string password );
        void removeUser( std::string username );

    private:
        std::ofstream _outFile;
        std::ifstream _inFile;
};

#endif