/* Copyright ?2000 
Michael Gradman and Corwin Joy 

Permission to use, copy, modify, distribute and sell this software and 
its documentation for any purpose is hereby granted without fee, provided 
that the above copyright notice appears in all copies and that both that 
copyright notice and this permission notice appear in supporting documentation. 
Corwin Joy and Michael Gradman make no representations about the suitability 
of this software for any purpose. 
It is provided "as is" without express or implied warranty.
*/ 

#include "RootException.h"
#include "clib_fwd.h"

#include "std_warn_off.h"
#include <sstream>
#include <algorithm>
#include "std_warn_on.h"

BEGIN_DTL_NAMESPACE


// *********** Implementation code for RootException ***************
RootException::RootException() : STD_::exception(), method(_TEXT("")), errmsg(_TEXT("")), exType(_TEXT("RootException")), whatbuf() { }

RootException::RootException(const tstring &meth, const tstring &err,
	  const tstring &excType) :
	  STD_::exception(), method(meth), errmsg(err), exType(excType), whatbuf() { }

tostream &operator<<(tostream &o, const RootException &ex)
{
	 o << ex.twhat();
	 return o;
}


// override if you have additional members to print
const TCHAR *RootException::twhat() const throw()
{
	 tostringstream errstr;
	 errstr << _TEXT("Exception type: ") << exType << STD_::endl;
	 errstr << _TEXT("Method: ") << method << STD_::endl;
	 errstr << _TEXT("Error Message: ") << errmsg << STD_::endl;
	 errstr << STD_::ends;

	 // this gymnastics is needed so result isn't destroyed
	 // paste these two lines into all what() code
	 whatbuf = errstr.str();

   return whatbuf.c_str();
}

const char *RootException::what() const throw()
{
#ifndef DTL_NO_UNICODE
	   whatbuf_narrow = tstring_cast((STD_::string *)NULL, twhat());
	   return whatbuf_narrow.c_str();
#else
	   return twhat();
#endif
}


ErrorHandler &GetErrorHandler()
{
	static ErrorHandler h;

	return h;
}

END_DTL_NAMESPACE
