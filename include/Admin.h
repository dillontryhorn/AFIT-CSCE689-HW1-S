#ifndef ADMIN_H
#define ADMIN_H

#include <string>
#include <fstream>

class Admin 
{
    public:
        Admin();
        Admin( const char *password_filename, const char *whitelist_filename );
        Admin( const char *password_filename, const char *whitelist_filename, int hash_length, int salt_length );
        ~Admin();

        bool checkWhitelistIP( std::string ip_addr );
        void addWhitelistIP( std::string ip_addr );
        void removeWhitelistIP( std::string ip_addr );

        bool checkUser( std::string username );
        bool addUser( std::string username );
        bool addUser( std::string username, std::string password );
        bool removeUser( std::string username );

        bool checkPassword( std::string username, std::string password );
        std::string changePassword( std::string username );
        std::string hashPassword( std::string password );
    private:
        std::ofstream _outFile;
        std::ifstream _inFile;

        const char *PASSWORD_FILENAME = "passwd";
        const char *WHITELIST_FILENAME = "whitelist";
        uint32_t HASHLEN = 32;
        uint32_t SALTLEN = 16;
};

#endif