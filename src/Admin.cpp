#include "Admin.h"
#include <iostream>

static const char* PASSWORD_FILENAME = "server_passwords.txt";
static const char* WHITELIST_FILENAME = "server_whitelist.txt";


void Admin::addWhitelistIP( std::string ip_addr )
{
    this->_outFile.open( WHITELIST_FILENAME, std::ios_base::app );
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
    this->_inFile.open( WHITELIST_FILENAME );
    if( this->_inFile )
    {
        std::string line;
        while( getline( this->_inFile, line ) )
        {
            if( line.find( ip_addr ) == std::string::npos )
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
    remove( WHITELIST_FILENAME );
    rename( "temp.txt", WHITELIST_FILENAME );
    if( !exists )
        std::cout << ip_addr << " was not found in the whitelist file." << std::endl;
}

void Admin::addUser( std::string username, std::string password )
{
    this->_outFile.open( PASSWORD_FILENAME, std::ios_base::app );
    if( this->_outFile )
    {
        this->_outFile << username << ":" << password << std::endl;
        std::cout << "Added " << username << " to the password file." << std::endl;
    }
    this->_outFile.close();
}

void Admin::removeUser( std::string username )
{
    bool exists = false;
    std::ofstream temp;
    temp.open( "temp.txt" );
    this->_inFile.open( PASSWORD_FILENAME );
    if( this->_inFile )
    {
        std::string line;
        while( getline( this->_inFile, line ) )
        {
            if( line.find( username ) == std::string::npos )
                temp << line << std::endl;
            else
            {
                std::cout << "Removed " << username << " from the password file." << std::endl;
                exists = true;
            }
        }
    }
    this->_inFile.close();
    temp.close();
    remove( PASSWORD_FILENAME );
    rename( "temp.txt", PASSWORD_FILENAME );
    if( !exists )
        std::cout << username << " was not found in the password file." << std::endl;
}

