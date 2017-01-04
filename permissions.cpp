// Author: Sean Davis

#include <iostream>
#include <cstring>
#include "permissions.h"
using namespace std;

void Permissions::set( short originalPermissions, short umask)
{
    
  permissions = originalPermissions & ~umask;
}  // createPermissions()


void Permissions::print()const
{
    //cout << "permissions = " << oct << permissions << dec <<endl;
    if (permissions & 256) cout << "r";
    else   cout << "-"; // no read permissions
    
    if (permissions & 128) cout << "w";
    else  cout << "-"; // no write permissions
        
    if (permissions & 64)cout << "x";
    else  cout << "-"; // no execute permissions
        
    
    if (permissions & 32) cout << "r";
    else  cout << "-"; // no read permissions
        
    
    if (permissions & 16) cout << "w";
    else  cout << "-"; // no write permissions
        
    
    if (permissions & 8) cout << "x";
    else  cout << "-"; // no execute permissions
        
    
  if (permissions & 4) cout << "r";
  else  cout << "-"; // no read permissions
  
  if (permissions & 2) cout << "w";
  else  cout << "-"; // no write permissions
  
  if (permissions & 1) cout << "x";
  else cout << "-"; //sth
}  // printPermissions()


 ostream& operator<< (ostream & os, const Permissions &p)
{
    os << dec << p.permissions;
    return os;
} //<<

 istream& operator>> (istream & is, Permissions &p)
{
    is >> dec >> p.permissions;
    return is;
} // >>

bool Permissions::isPermitted (short request_permission)
{
    
    if ( ( permissions & request_permission ) == 0 )
        return false ;
    else // esle 
        return true ;
} //isper
