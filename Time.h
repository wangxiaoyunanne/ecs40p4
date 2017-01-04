
#ifndef TIME_H
  #define TIME_H

#include <ctime>
#include <iostream>

using namespace std;

class Time
{
public:
    Time();
    struct tm modificationTime;

    void update();
    void print() const;
    friend ostream& operator << (ostream& os, const Time &t);
}; //Time


#endif
