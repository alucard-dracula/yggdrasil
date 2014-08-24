// handy utilities for tests ... making output easier to delimit
// Initial: 12/27/2000 - grabbed from example project

#include "print_util.h"

using namespace std;

// prints out asterisks between each example function
void PrintSeparator(tostream &o)
{
   o << endl;
   o << _TEXT("********************************") << endl;
   o << endl;
}

// prints a header with title for a particular example
void PrintHeader(tostream &o, const tstring &title)
{
   o << _TEXT("!!!!!!!!!!!!!!!!!!!!! Begin Example ") << title << _TEXT(" !!!!!!!!!!!!!!!!!!!! ");
   o << endl;
}
