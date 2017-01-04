// Author: Sean Davis
#include <iostream>
#include <cstring>
#include <cstdlib>
#include "directory.h"
#include "Time.h"
using namespace std;

Directory::Directory( const char *nam, short umask,
                      Directory *paren): subDirectories(NULL),
                       subDirectoryCount(0), parent(paren)
{
    name = new char(strlen(nam) + 1);
    strcpy(name, nam);
    permissions.set(0777, umask);
    
}  // Directory())

Directory::~Directory()
{
  
    delete  [] name;

    for (int i = 0; i < subDirectoryCount; i++)
    {
        delete subDirectories[i];
    } // for loop the deconstract

    delete [] subDirectories;
    
}  // ~Directory

Directory* Directory::cd( int argCount, const char *arguments[])
{
    
  if (argCount != 2)
  {
    cout << "usage: cd directoryName" << endl;
    return this;
  }  // if two many arguments

  if (strcmp(arguments[1], "..") == 0)
  {
    if (parent)
      return parent;
    else  // this is root
      return this;
  }  // if cd ..

  for (int i = 0; i < subDirectoryCount; i++)
  {
    if ( *subDirectories[i] == arguments[1] )
    {
      if ( ! subDirectories[i]->permissions.isPermitted( 0100 ))
      {
          cout << arguments[1] << ": Permission denied." << endl;
          return this;
      } // if not permitted

      return subDirectories[i];
    } // if found
  }  // for each subdirectory
 
  cout << arguments[1] << ": No such file or directory\n";
  return this;
//}
}  // cd()



void  Directory::ls( int argCount, const char *arguments[])
{
  if (argCount > 2 || (argCount == 2 && strcmp(arguments[1], "-l") != 0))
    cout << "usage: ls [-l]\n";
  else  // correct number of arguments
  {
    if ( ! permissions.isPermitted( 0400 ))
    {
      cout << "ls: cannot open directory .: Permission denied" << endl;
      return ;
    } // if not permitted

    if (argCount == 1)  // simple ls
    {
      for (int i = 0; i < subDirectoryCount; i++)
        cout << subDirectories[i]->name << " ";
      
      cout << "\n";
    }  // if simple ls
    else  // must be ls -l
    {
      for (int i = 0; i < subDirectoryCount; i++)
      {
        subDirectories[i]->permissions.print();
          cout << " ";
          subDirectories[i]->ttime.print();
          cout << " " << subDirectories[i]->name << endl;
      }  // for each subdirectory
    }  // else is ls -l
  }  // else correct arguments
}  // ls()


void Directory::mkdir(int argCount, const char *arguments[], short umask)
{
  if (argCount == 1)
  {
     cout << "mkdir: missing operand" << endl;
     cout << "Try 'mkdir --help' for more information." << endl;
     return;
  }  // if too few arguments


  for (int i = 0; i < subDirectoryCount; i++)
  {
    for (int j = 1; j < argCount; j++)
     { if (*subDirectories[i] == arguments[j])
       {
        cout << "mkdir: cannot create directory ‘" << arguments[j] << 
         "’: File exists" << endl;
        return;
       }  // if subdirectory already exists.
    }  // for each subdirectory
  } // end of subdir
  
  if ( ! permissions.isPermitted(0200) )
  {
   
     for (int j = 1; j < argCount; j++)
         cout << "mkdir: cannot create directory ‘" << arguments[j] << 
       "’: Permission denied" << endl;
     return;
  } // if no permission

  Directory* *subDirectoriesTemp = 
  new Directory* [subDirectoryCount + argCount - 1];

  for (int i = 0; i < subDirectoryCount; i++)
    subDirectoriesTemp[i] = subDirectories[i];

    delete [] subDirectories;
 
  for (int j = 1; j < argCount; j++)
  {
    subDirectories = subDirectoriesTemp;
    //cout<<"umask = "<< oct << umask << oct <<endl;
    subDirectories[subDirectoryCount] = 
    new Directory(arguments[j], umask,  this);
    subDirectoryCount++;
  } //end for 

  ttime.update();
}  // mkdir()


void Directory::showPath()
{
  if (parent == NULL)
  {
    cout << "/";
    return;
  }  // at root
   
    parent->showPath();
    cout << name << "/";
}  // showPath())

ostream& operator << (ostream &os, const Directory &dir)
{
  os << dir.name << " " << dir.ttime.modificationTime.tm_mon
      << " " << dir.ttime.modificationTime.tm_mday << " " 
          << dir.ttime.modificationTime.tm_hour << " " 
          << dir.ttime.modificationTime.tm_min << " "
          << dir.permissions << " "
          << dir.subDirectoryCount << endl;
  int i = 0;
 
   while ( i < dir.subDirectoryCount )
  {
    os << *(dir.subDirectories[i]);
    i++;
  } // end

  return os;
} //os

istream& operator >> (istream &is, Directory &dir)
{
  char name[80];
    //char ch;
  is >> name;
  delete [] dir.name;
  dir.name = new char [strlen(name) + 1];
  strcpy(dir.name, name);
  is >> dir.ttime.modificationTime.tm_mon
          >> dir.ttime.modificationTime.tm_mday
          >> dir.ttime.modificationTime.tm_hour 
          >> dir.ttime.modificationTime.tm_min
    >> dir.permissions
    >> dir.subDirectoryCount;
    is.ignore();
  
  int i = 0;
  dir.subDirectories = new Directory* [dir.subDirectoryCount];

  while (i < dir.subDirectoryCount)
  {
    dir.subDirectories[i] = new Directory ("readin", 0, &dir);
    is >> *dir.subDirectories[i];
      i++;
  } //end while
    
  return is;
} //overload >>



bool Directory::operator == (const char* &rhs) const
{
  return !strcmp (name, rhs);

} //==

int compare (const char * str1, const char * str2)
{
    return strcmp(str1, str2);
} // compare

void Directory::chmod(int argCount, const char *arguments[])
{
  int i, j, value = 0;

  if (argCount == 1)
  {
    cout << "chmod: missing operand" << endl;
      cout << "Try 'chmod --help' for more information." << endl;
    return;
  } // if incorrect argument count.
    
    if (argCount == 2)
    {
        cout << "chmod: missing operand after ‘" << arguments[1] 
        << "’" << endl;
        cout << "Try 'chmod --help' for more information." << endl;
        return;
    } // argnum=2

  for (i = 0; arguments[1][i]; i++)
    if (! isdigit(arguments[1][i]) || arguments[1][i] == '9'
        || arguments[1][i] == '8')
    {
      cout << "chmod: invalid mode: ‘" << arguments[1] << endl;
        cout << "Try 'chmod --help' for more information." << endl;
      return;
    } // out of 7  
    else //else
      value = 8 * value + arguments[1][i] - '0';

  if (value < 0 || value > 0777)
  {
    cout << "chmod: invalid mode: ‘" << arguments[1] << "’\n";
      cout << "Try 'chmod --help' for more information." << endl;
    return;
  } // if invalid value

  for (j = 2; j < argCount; j++)
  {
    for (i = 0; i < subDirectoryCount; i++)
       if (compare(subDirectories[i]->name, arguments[j]) == 0)
      {
        //cout<<"match found"<<endl;
        subDirectories[i]->permissions.set(value, 0);
        break;
      } // if found matching directory name

    if ( i == subDirectoryCount)
      cout << "chmod: cannot access ‘" << arguments[j]
        << "’: No such file or directory" << endl;
  } //forloop end
} // chmod()

