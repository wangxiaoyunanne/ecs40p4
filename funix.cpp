// Author: Sean Davis
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include "funix.h"
#include "directory.h"
#include <fstream>
  using namespace std;

Funix::Funix():  umask(022) // creates currentDirectory, and sets umask and time
{
    currentDirectory = new Directory ("/", umask, NULL);
    ifstream inf;
    inf.open("directories.txt");

    if (inf.is_open())
      inf >> *this;
     else // else
      umask = 022; 
   
}  // init()

Funix::~Funix()
{
    int argCount = 2;
    const char *arguments[2] = {"cd", ".."};
   
    while ((currentDirectory->cd(argCount, arguments)) != currentDirectory)
   {
       currentDirectory = currentDirectory->cd(argCount, arguments);
   } //while end

    delete currentDirectory;
} // ~funix


int Funix::eXit(int argCount, const char *arguments[])
{
  // checks "exit" command, returns 0 on proper exit
  
  if (argCount > 1)
  {
    cout << "usage: exit" << endl;
    return 1;
  }  // if more than one argument
  else // else
   {
    const char *arg[2] = {"cd", ".."};
     
       while ((currentDirectory->cd(2, arg)) != currentDirectory)
    {
       currentDirectory = currentDirectory->cd(2, arg);
    } //while end

     ofstream output;
    output.open("directories.txt");
    output << *this;
    output.close();
  } //sles end
  
  return 0;
}  // eXit()

void Funix::getCommand(char *command)  // writes prompt and reads command
{
  writePrompt();
  cin.getline(command, COMMAND_LENGTH);
  
}  // getCommand()

int Funix::processCommand(char *command)  // returns 0 on proper exit
{
  static const char *commands[] = 
   {"cd", "exit", "ls", "mkdir", "umask", "chmod"};
  const char *arguments[MAX_ARGUMENTS];
  char *ptr;
  int argCount = 0, commandNum;
  ptr = strtok(command, " ");
  
  while (ptr)
  {
    arguments[argCount++] = ptr;
    ptr = strtok(NULL, " ");
  }  // while more arguments in the command line
  
  if (argCount > 0)
  {
    for (commandNum = 0; commandNum < NUM_COMMANDS; commandNum++)
    {
      if (strcmp(arguments[0], commands[commandNum]) == 0)
        break;
    }  // for each possible command

    switch (commandNum)
    {
      case 0: currentDirectory = currentDirectory->cd( argCount, arguments);
              break;
      case 1: return eXit( argCount, arguments);
      case 2: currentDirectory->ls(argCount, arguments); break;
      case 3: currentDirectory->mkdir( argCount, arguments, umask); break;
      case 4: setUmask( argCount, arguments); break;
      case 5: currentDirectory->chmod( argCount, arguments); break;   
      default: cout << arguments[0] << ": Command not found." << endl;
    }  // switch on commandNum   
  }  // if at least one argument
  
  return 1;
}  // processCommand()


void Funix::run()
// reads and processes commands until proper exit
{
  char command[COMMAND_LENGTH];
  getCommand( command);
  
  while (processCommand( command))
    getCommand(command);
}  // run()




int atoi_oct(const char str[])
{
    int x = 0;

    for (unsigned int i = 0; i < strlen(str); i++)
        x = x*8 + (str[i]-'0');
    return x;
} //end of fn




void Funix::setUmask( int argCount, const char *arguments[])


{
  if (argCount == 1)
  {
    cout << oct << this -> umask << dec << endl;
    return;
  }  // if wrong number of arguments
    
    if (argCount > 2)
    {
        cout << "umask: Too many arguments." << endl;
        return;
    } //if end
    
  int length = strlen(arguments[1]);

  for (int i = 0; i < length; i++ )
     if ((arguments[1][i] < '0' || arguments[1][i] > '7')
         || length > 3)
  {
    cout << "umask: Improper mask." << endl;
    return;
  }  // if incorrect 

  umask = atoi_oct(arguments[1]);
} // end


void Funix::setUmask( int argCount)
  //overload umask()
{
  if (argCount == 1)
  {
     cout << oct << this -> umask << dec << endl;
     return;
  } //end if
}  //set umask


void Funix::writePrompt()  // shows path and '#'
{
  currentDirectory->showPath();
  cout << " # ";
}  // writePrompt()

ostream& operator << (ostream& os, Funix& fu)
{
  os << fu.umask << endl;
  os << *(fu.currentDirectory);
  return os;
  
} //<<

istream& operator >> (istream& is, Funix& fu)
{
  is  >> (fu.umask);
  is >> *(fu.currentDirectory);
  return is;
} //>>
