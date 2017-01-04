// Author: Sean Davis

#ifndef PERMISSIONS_H
  #define PERMISSIONS_H

#include <iostream>
using namespace std;
class Permissions
{
  short permissions;
public:
    
  void set( short originalPermissions, short umask);
  void print()const;
  friend ostream& operator<< (ostream & os, const Permissions &p);
  friend istream& operator>> (istream & is, Permissions &p);
  bool isPermitted(short request_permissions);
}; // class Permissions
#endif  // PERMISSIONS_H

