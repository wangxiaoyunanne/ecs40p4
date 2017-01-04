


#include <iomanip>
#include "Time.h"

using namespace std;

Time::Time()
{
    update();
} //time

void Time::update()
{
    time_t t = time(NULL);
    modificationTime = *localtime (&t);
} //update

void Time::print()const
{
    
    static const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
"Jul", "Agu", "Sep", "Nov", "Dec"};
    
    cout << months[modificationTime.tm_mon] << " " << setw(2) 
          << modificationTime.tm_mday <<
         " " << setw(2) << modificationTime.tm_hour;
    
    if ( modificationTime.tm_min > 9 )
        cout << ":" ;
    else cout << ":0" ; // if
    cout << modificationTime.tm_min;
    
  
}//istream& operator << ( Time

ostream& operator << (ostream& os, Time& ti)
{
    return os;
} // end of function

istream& operator >> (istream& is, Time& ti)
{
    return is;
} // end of function

