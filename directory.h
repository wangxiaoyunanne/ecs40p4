// Author: Sean Davis

#ifndef DIRECTORY_H
  #define DIRECTORY_H

#include <fstream>
#include "permissions.h"
#include "Time.h"


class Directory
{
  char *name;
  Time ttime;
  Directory** subDirectories;
  int subDirectoryCount;
  Directory *parent;
  Permissions permissions;
    
public:
  Directory(const char *nam, short umask,  Directory *paren);
    ~Directory();
  Directory* cd( int argCount, const char *arguments[]);
   
  void ls(int argCount, const char *arguments[]);
  void mkdir(  int argCount, const char *arguments[],
               short umask);
  void chmod(int argCount, const char *arguments[]);
 
 void showPath();
  friend ostream& operator << (ostream &os, const Directory &directory);
  friend istream& operator >> (istream &is, Directory &directory); 
  bool operator== (const Directory &rhs) const;
  bool operator== (const char* &rhs) const;
}; //class directory
#endif  // DIRECTORY_H

