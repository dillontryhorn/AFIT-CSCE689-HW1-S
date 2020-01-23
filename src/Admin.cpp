#include "Admin.h"
#include <argon2.h>
#include <chrono>
#include <ctime>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

Admin::Admin()
{
}

Admin::Admin( const char *password_filename, const char *whitelist_filename, const char *log_filename ) : 
    PASSWORD_FILENAME( password_filename ), WHITELIST_FILENAME( whitelist_filename ), LOG_FILENAME( log_filename )
{
}

Admin::Admin( const char *password_filename, const char *whitelist_filename, const char *log_filename, int hash_length, int salt_length ) :
    PASSWORD_FILENAME( password_filename ), WHITELIST_FILENAME( whitelist_filename ), LOG_FILENAME( log_filename ),
    HASHLEN( hash_length ), SALTLEN( salt_length )
{
}

Admin::~Admin()
{
}

bool Admin::checkWhitelistIP( std::string ip_addr )
{
    this->_inFile.open( this->WHITELIST_FILENAME );
    if( this->_inFile )
    {
        std::string line;
        while( getline( this->_inFile, line ) )
        {
            if( line.compare( ip_addr ) == 0 )
                return true;
        }
    }
    this->_inFile.close();
    return false;
}

void Admin::addWhitelistIP( std::string ip_addr )
{
    this->_outFile.open( this->WHITELIST_FILENAME, std::ios_base::app );
    if( this->_outFile )
    {
        this->_outFile << ip_addr << std::endl;
        std::cout << "Added " << ip_addr << " to the whitelist file." << std::endl;
    }
    this->_outFile.close();
}

void Admin::removeWhitelistIP( std::string ip_addr )
{
    bool exists = false;
    std::ofstream temp;
    temp.open( "temp.txt" );
    this->_inFile.open( this->WHITELIST_FILENAME );
    if( this->_inFile )
    {
        std::string line;
        while( getline( this->_inFile, line ) )
        {
            if( line.compare( ip_addr ) != 0 )
                temp << line << std::endl;
            else
            {
                std::cout << "Removed " << ip_addr << " from the whitelist file." << std::endl;
                exists = true;
            }
        }
    }
    this->_inFile.close();
    temp.close();
    remove( this->WHITELIST_FILENAME );
    rename( "temp.txt", this->WHITELIST_FILENAME );
    if( !exists )
        std::cout << ip_addr << " was not found in the whitelist file." << std::endl;
}

bool Admin::checkUser( std::string username )
{
    bool doesExist = false;
    std::vector<std::string> strings;
    this->_inFile.open( this->PASSWORD_FILENAME );
    if( this->_inFile )
    {
        std::string line;
        while( getline( this->_inFile, line ) )
        {
            if( line.find( username ) != std::string::npos )
            {
                strings.clear();
                std::istringstream user_line( line );
                std::string token;    
                while( getline(user_line, token, ':') )
                    strings.push_back( token );
                if( strings.at(0).compare( username ) == 0 )
                    doesExist = true;
            }
        }
    }
    this->_inFile.close();
    return doesExist;
}

bool Admin::addUser( std::string username )
{
    bool is_added = false;
    std::string hashed_password = Admin::hashPassword( Admin::changePassword( username ) );
    this->_outFile.open( this->PASSWORD_FILENAME, std::ios_base::app );
    if( this->_outFile )
    {
        this->_outFile << username << ":" << hashed_password << std::endl;
        is_added = true;
    }
    this->_outFile.close();
    return is_added;
}

bool Admin::addUser( std::string username, std::string password )
{
    bool is_added = false;
    std::string hashed_password = Admin::hashPassword( password );
    this->_outFile.open( this->PASSWORD_FILENAME, std::ios_base::app );
    if( this->_outFile )
    {
        this->_outFile << username << ":" << hashed_password << std::endl;
        is_added = true;
    }
    this->_outFile.close();
    return is_added;
}

bool Admin::removeUser( std::string username )
{
    bool deleted = false;
    std::ofstream temp;
    std::vector<std::string> strings;
    temp.open( "temp.txt" );
    this->_inFile.open( this->PASSWORD_FILENAME );
    if( this->_inFile )
    {
        std::string line;
        while( getline( this->_inFile, line ) )
        {
            if( line.find( username ) == std::string::npos )
                temp << line << std::endl;
            else
            {
                strings.clear();
                std::istringstream user_line( line );
                std::string token;    
                while( getline(user_line, token, ':') )
                    strings.push_back( token );
                if( strings.at(0).compare( username ) == 0 )
                {
                    deleted = true;
                }
                else
                    temp << line << std::endl;
            }
        }
    }
    this->_inFile.close();
    temp.close();
    remove( this->PASSWORD_FILENAME );
    rename( "temp.txt", this->PASSWORD_FILENAME );
    if( !deleted )
        std::cout << username << " was not found in the password file." << std::endl;
    return deleted;
}

bool Admin::checkPassword( std::string username, std::string password )
{
    bool isSamePasswd = false;
    this->_inFile.open( this->PASSWORD_FILENAME );
    if( this->_inFile )
    {
        std::string line;
        while( getline( this->_inFile, line ) )
        {
            if( line.find( username ) != std::string::npos )
                break;
        }
        std::vector<std::string> strings;
        std::istringstream user_line(line);
        std::string token;    
        while( getline(user_line, token, ':') )
            strings.push_back( token );
        if( strings.at(1).compare( Admin::hashPassword( password ) ) == 0 )
            isSamePasswd = true;
    }
    this->_inFile.close();
    return isSamePasswd;
}

std::string Admin::changePassword( std::string username )
{
    std::string password, confirm_password;
    do
    {
        std::cout << "Enter your new password: ";
        std::cin >> password;
        std::cout << "Confirm your password: ";
        std::cin >> confirm_password;
        if( Admin::hashPassword( password ).compare( Admin::hashPassword( confirm_password ) ) != 0 )
            std::cout << "Passwords do not match." << std::endl;
    } while( Admin::hashPassword( password ).compare( Admin::hashPassword( confirm_password ) ) != 0 );
    if( !Admin::checkUser( username ) )
        return password;
    else
    {
        Admin::removeUser( username );
        Admin::addUser( username, password );
        return "Added User";
    }
}

std::string Admin::changePassword( std::string username, std::string new_password )
{
    if( !Admin::checkUser( username ) )
        return "User does not exist";
    else
    {
        Admin::removeUser( username );
        Admin::addUser( username, new_password );
        return "Added User";
    }
}

std::string Admin::hashPassword( std::string password )
{
    /* MOST OF CODE TAKEN STRAIGHT FROM 
     * https://github.com/P-H-C/phc-winner-argon2
     * Some code repurposed to integrate iostream and stringstream
     */
    char *PWD = const_cast<char*>( password.c_str() );
    uint8_t hash1[this->HASHLEN];
    uint8_t hash2[this->HASHLEN];

    uint8_t salt[this->SALTLEN];
    memset( salt, 0x00, this->SALTLEN );

    uint8_t *pwd = (uint8_t *)strdup( PWD );
    uint32_t pwdlen = strlen( (char *)PWD );

    uint32_t t_cost = 2;            // 1-pass computation
    uint32_t m_cost = (1<<16);      // 64 mebibytes memory usage
    uint32_t parallelism = 1;       // number of threads and lanes

    argon2i_hash_raw(t_cost, m_cost, parallelism, pwd, pwdlen, salt, this->SALTLEN, hash1, this->HASHLEN);

    argon2_context context = {
        hash2,  /* output array, at least HASHLEN in size */
        this->HASHLEN, /* digest length */
        pwd, /* password array */
        pwdlen, /* password length */
        salt,  /* salt array */
        this->SALTLEN, /* salt length */
        NULL, 0, /* optional secret data */
        NULL, 0, /* optional associated data */
        t_cost, m_cost, parallelism, parallelism,
        ARGON2_VERSION_13, /* algorithm version */
        NULL, NULL, /* custom memory allocation / deallocation functions */
        /* by default only internal memory is cleared (pwd is not wiped) */
        ARGON2_DEFAULT_FLAGS
    };

    int rc = argon2i_ctx( &context );
    if(ARGON2_OK != rc) 
    {
        std::cout << "ERROR! " << argon2_error_message(rc);
        exit(1);
    }
    free( pwd );

    if( memcmp(hash1, hash2, this->HASHLEN) )
    { 
        std::cerr << "ERROR! Hashes do not match." << std::endl;
        exit(1);
    }

    std::stringstream hashed_password;
    for (int i = 0; i < this->HASHLEN; ++i)
        hashed_password << std::hex << static_cast<int>(hash2[i]);

    return hashed_password.str();
}

void Admin::logger( std::string msg )
{
    this->_outFile.open( this->LOG_FILENAME, std::ios_base::app );
    if( this->_outFile )
    {
        std::time_t current_time = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now() );
        this->_outFile << std::ctime( &current_time ) << msg << std::endl;
    }
    this->_outFile.close();
}