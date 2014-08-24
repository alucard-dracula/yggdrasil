// handy utilities for tests ... making output easier to delimit
// Initial: 12/27/2000 - grabbed from example project

#ifndef _PRINT_UTIL_H
#define _PRINT_UTIL_H

#include <iostream>
#include <string>
#include "dtl_base_types.h"

using namespace dtl;
using namespace std;

// prints out asterisks between each example function
void PrintSeparator(tostream &o);

// prints a header with title for a particular example
void PrintHeader(tostream &o, const tstring &title);

#endif
