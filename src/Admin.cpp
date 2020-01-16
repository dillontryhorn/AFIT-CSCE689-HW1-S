#include "Admin.h"
#include <iostream>

static const char* PASSWORD_FILENAME = "server_passwords.txt";

void Admin::addUser( std::string username, std::string password )
{
    this->_outFile.open( PASSWORD_FILENAME, std::ios_base::app );
    if( this->_outFile )
    {
        this->_outFile << username << ":" << password << "\n";
        std::cout << "Added " << username << " to the password file.\n";
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
                std::cout << "Removed " << username << " from the password file.\n";
                exists = true;
            }
        }
    }
    this->_inFile.close();
    temp.close();
    remove( PASSWORD_FILENAME );
    rename( "temp.txt", PASSWORD_FILENAME );
    if( !exists )
        std::cout << username << " was not found in the password file.\n";
}

