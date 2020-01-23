#ifndef ADMIN_H
#define ADMIN_H

#include <string>
#include <fstream>

class Admin 
{
    public:
        Admin();
        Admin( const char *password_filename, const char *whitelist_filename, const char *log_filename );
        Admin( const char *password_filename, const char *whitelist_filename, const char *log_filename, int hash_length, int salt_length );
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
        std::string changePassword( std::string username, std::string new_password );
        std::string hashPassword( std::string password );

        void logger( std::string msg );
    private:
        std::ofstream _outFile;
        std::ifstream _inFile;

        const char *PASSWORD_FILENAME = "passwd";
        const char *WHITELIST_FILENAME = "whitelist";
        const char *LOG_FILENAME = "server.log";
        uint32_t HASHLEN = 32;
        uint32_t SALTLEN = 16;
};

#endif