#include <iostream>
#include <string>
#include "string.h"
#include "Admin.h"
#include "exceptions.h"

using namespace std;

void displayHelp(const char *execname) {
    cout << "Improper syntax. Two possible examples shown below.\n";
    cout << execname << " <add> <username> <password>\n";
    cout << execname << " <remove> <username>\n";
}

int main(int argc, char *argv[])
{
    string option = "", username = "", password = "";
    //Improper syntax
    if( argc < 3 || argc > 4 ) 
    {
        displayHelp(argv[0]);
        exit(0);
    }

    //Read in parameters from command line
    if( argc == 3 && !strncmp(argv[1], "remove", 7) )
    {
        option = argv[1];
        username = argv[2];
    }
    else if( argc == 4 && !strncmp(argv[1], "add", 4) )
    {
        option = argv[1];
        username = argv[2];
        password = argv[3];
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
        displayHelp(argv[0]);
    
    return 0;
}