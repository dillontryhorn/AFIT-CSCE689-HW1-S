#ifndef ADMIN_H
#define ADMIN_H

#include <string>
#include <fstream>

/******************************************************************************************
 * Admin - Administrative class to manipulate the server and provide authentication help
 *
 *  	   Admin(): Public Constructor, Overloaded constructors allow for custom filenames and salt/hash length
 *  	   ~Admin(Dest): Public Destructor
 *
 *  	   checkWhitelistIP - verify if supplied IP address string is in the whitelist file, return true if found
 *         addWhitelistIP - add an IP address string to the whitelist file, no return
 *         removeWhitelistIP - remove an IP address string from the whitelist file, no return
 * 
 *  	   checkUser - verify if supplied username string is in the password file, return true if found
 *         addUser - adds user to password file with or without supplied password, prompts for password 
 *             if none supplied, return true if added
 *         removeUser - remove user from password file, return true if removed
 *  	   
 *         checkPassword - verifies if supplied password and username match the combination in the password file
 *         changePassword - takes username or username and password, prompts for password if no password supplied
 *             return a verification string
 *         hashPassword - place supplied string through the Argon2 hashing algorithm, return hashed password
 *          
 *         logger - Logs a supplied message to defined log file with timestamp 	   
 *
 *****************************************************************************************/

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