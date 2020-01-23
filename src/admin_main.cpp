#include <iostream>
#include <string>
#include "string.h"
#include "Admin.h"
#include "exceptions.h"

using namespace std;

void displayHelp(const char *execname) { //Five possible examples
    cout << "Improper syntax. All possible examples shown below." << endl;
    cout << execname << " user add <username>" << endl;
    cout << execname << " user remove <username>" << endl;
    cout << execname << " user passwd <username>" << endl;
    cout << execname << " ip add <IP Address>" << endl;
    cout << execname << " ip remove <IP Address>" << endl;
}

int main(int argc, char *argv[])
{
    string option = "", username = "", password = "", ip_addr = "";

    //Improper syntax
    if( argc != 4 ) 
    {
        displayHelp(argv[0]);
        exit(-1);
    }

    //Manipulating users
    if( !strncmp( argv[1], "user", 5 ) )
    {
        //Read in parameters from command line
        option = argv[2];
        username = argv[3];

        Admin server_admin; //Password file = "passwd", Whitelist file = "whitelist", Log file = "server.log"

        if( option.compare("add") == 0 )
        {
            try //Add user
            {
                if( !server_admin.checkUser( username ) )
                {
                    server_admin.addUser( username );
                }
                else
                    cout << username << " already exists." << endl;
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
                if( server_admin.removeUser( username ) )
                    cout << "Removed " << username << endl;
            } catch (runtime_error &e)
            {
                cerr << "Unable to remove user: " << e.what() << endl;
                return -1;
            }
        }
        else if( option.compare("passwd") == 0)
        {
            try //Change user password
            {
                server_admin.changePassword( username );
                cout << "Password changed for " << username << endl;
            } catch (runtime_error &e)
            {
                cerr << "Unable to change user password: " << e.what() << endl;
                return -1;
            }
        }
        else
            displayHelp( argv[0] ); //Invalid args
    }

    //Manipulate Whitelisted IP addresses
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

        Admin server_admin; //Password file = "passwd", Whitelist file = "whitelist", Log file = "server.log"

        if( option.compare("add") == 0 )
        {
            try //Add IP to Whitelist
            {
                if( !server_admin.checkWhitelistIP( ip_addr ) )
                    server_admin.addWhitelistIP( ip_addr );
                else
                    cout << ip_addr << " is already whitelisted." << endl;
            } catch (runtime_error &e)
            {
                cerr << "Unable to add IP address: " << e.what() << endl;
                return -1;
            }
        }
        else if( option.compare("remove") == 0 )
        {
            try //Remove IP from Whitelist
            {
                server_admin.removeWhitelistIP( ip_addr );
            } catch (runtime_error &e)
            {
                cerr << "Unable to remove IP address: " << e.what() << endl;
                return -1;
            }
        }
        else
            displayHelp( argv[0] ); //Invalid args
    }
    else
        displayHelp( argv[0] ); //Invalid args
    
    exit(0);
}