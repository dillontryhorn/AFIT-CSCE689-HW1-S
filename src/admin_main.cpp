#include <iostream>
#include <string>
#include "string.h"
#include "Admin.h"
#include "exceptions.h"

using namespace std;

void displayHelp(const char *execname) {
    cout << "Improper syntax. All possible examples shown below." << endl;
    cout << execname << " user add <username> <password>" << endl;
    cout << execname << " user remove <username>" << endl;
    cout << execname << " ip add <IP Address>" << endl;
    cout << execname << " ip remove <IP Address>" << endl;
}

int main(int argc, char *argv[])
{
    string option = "", username = "", password = "", ip_addr = "";

    //Improper syntax
    if( argc < 4 || argc > 5 ) 
    {
        displayHelp(argv[0]);
        exit(0);
    }

    if( !strncmp( argv[1], "user", 5 ) )
    {
        //Read in parameters from command line
        if( argc == 4 && !strncmp(argv[2], "remove", 7) )
        {
            option = argv[2];
            username = argv[3];
        }
        else if( argc == 5 && !strncmp(argv[2], "add", 4) )
        {
            option = argv[2];
            username = argv[3];
            password = argv[4];
        }

        Admin server_admin;

        if( option.compare("add") == 0 )
        {
            try //Add user
            {
                server_admin.addUser( username, password );
            } catch (runtime_error &e)
            {
                cerr << "Unable to add user: " << e.what() << endl;
                return -1;
            }
        }
        else if( option.compare("remove") == 0 )
        {
            try //Remove user
            {
                server_admin.removeUser( username );
            } catch (runtime_error &e)
            {
                cerr << "Unable to remove user: " << e.what() << endl;
                return -1;
            }
        }
        else
            displayHelp( argv[0] );
    }
    else if( !strncmp( argv[1], "ip", 3 ) )
    {
        //Read in parameters from command line
        if( argc == 4 && !strncmp(argv[2], "remove", 7) )
        {
            option = argv[2];
            ip_addr = argv[3];
        }
        else if( argc == 4 && !strncmp(argv[2], "add", 4) )
        {
            option = argv[2];
            ip_addr = argv[3];
        }

        Admin server_admin;

        if( option.compare("add") == 0 )
        {
            try //Add IP to Whitelist
            {
                server_admin.addWhitelistIP( ip_addr );
            } catch (runtime_error &e)
            {
                cerr << "Unable to add IP address: " << e.what() << endl;
                return -1;
            }
        }
        else if( option.compare("remove") == 0 )
        {
            try //Add IP to Whitelist
            {
                server_admin.removeWhitelistIP( ip_addr );
            } catch (runtime_error &e)
            {
                cerr << "Unable to remove IP address: " << e.what() << endl;
                return -1;
            }
        }
        else
            displayHelp( argv[0] );
    }
    else
        displayHelp( argv[0] );
    
    exit(0);
}