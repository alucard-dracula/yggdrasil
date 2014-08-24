/* Copyright © 2000 
Michael Gradman and Corwin Joy 

Permission to use, copy, modify, distribute and sell this software and 
its documentation for any purpose is hereby granted without fee, provided 
that the above copyright notice appears in all copies and that both that 
copyright notice and this permission notice appear in supporting documentation. 
Corwin Joy and Michael Gradman make no representations about the suitability 
of this software for any purpose. 
It is provided "as is" without express or implied warranty.
*/ 

#include "validate.h"
#include "clib_fwd.h"

#include "std_warn_off.h"
#include <sstream>
#include <typeinfo>
#include <algorithm>
#include "std_warn_on.h"

BEGIN_DTL_NAMESPACE

// ********** Implementation for ValidityException ***********
ValidityException::ValidityException(const tstring &meth, const tstring &obType,
		  Revalidation reval) : 
		  RootException(meth, _TEXT("Cannot operate on invalidated object of type ") + obType + _TEXT("!  ") +
		  (reval == REVALIDATE_FAILED ? _TEXT("(irreconcilable revalidate())") : _TEXT("")),
		  _TEXT("ValidityException")),
		  objType(obType)
{
	// write to error log???
}

const TCHAR *ValidityException::twhat() const throw()
{	
	    tstring rootWhat = RootException::twhat();

		tostringstream o;
	
		o << rootWhat << _TEXT("objType: ") << objType << STD_::endl << STD_::ends;
	    
		// this gymnastics is needed so result isn't destroyed
		// paste these two lines into all what() code
		whatbuf = o.str();

		return whatbuf.c_str();
}


const char *ValidityException::what() const throw()
{	
#ifndef DTL_NO_UNICODE
	   whatbuf_narrow = tstring_cast((STD_::string *)NULL, twhat());
	   return whatbuf_narrow.c_str();
#else
	   return whatbuf.c_str();
#endif
}


// ************ Implementation for ValidatedObject ***************
ValidatedObject::ValidatedObject() : validity(VALID)
{ }

ValidatedObject::~ValidatedObject() {invalidate();}

void ValidatedObject::swap(ValidatedObject &other) {
	  STD_::swap(validity, other.validity);
}


// mark object as invalid!
void ValidatedObject::invalidate()
{
	 validity = INVALID;
}

// throws an exception if the object is invalid
void ValidatedObject::validate() const
{
		 if (!valid())
			 DTL_THROW ValidityException(_TEXT("ValidatedObject::validate()"), 
			 tstring_cast((tstring *)NULL, STD_::string(typeid(*this).name())));
}

// make the object valid again by trying to get it back
// in a valid state, if not reconcilable, throw an exception
void ValidatedObject::revalidate()
{
		 // nothing to do if already valid
		 if (valid())
			 return;

		 			 DTL_THROW ValidityException(_TEXT("ValidatedObject::revalidate()"), 
			 tstring_cast((tstring *)NULL, STD_::string(typeid(*this).name())),
			 ValidityException::REVALIDATE_FAILED);
}

ValidatedObject::Validity ValidatedObject::GetValidity() const
{ 
		  valid(); // coerce validity flag to be updated
		  return validity;
}

// overrides of this function should invalidate() the object if they discover that
// the object is invalid
bool ValidatedObject::valid() const { return validity == VALID; }

END_DTL_NAMESPACE
