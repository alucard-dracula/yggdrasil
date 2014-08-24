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
// simple class which simply keeps track whether an object is in a valid state
// Initial: 9/5/2000 - MG
// Edited: 11/14/2000 - MG - Inherited ValidityException from RootException class
// Edited: 12/19/2000 - MG - added namespaces

#ifndef DTL_VALIDATE_H
#define DTL_VALIDATE_H

#include "RootException.h"

#include "std_warn_off.h"
#include <string>
#include "std_warn_on.h"

BEGIN_DTL_NAMESPACE

// enum Validity { INVALID = 0, VALID = 1 };
// enum Revalidation { NONE_TRIED = 0, REVALIDATE_FAILED = 1 };

// thrown on a ValidatedObject if object's state is invalid when validate() called
class ValidityException : public RootException
{
   public:
      enum Revalidation { NONE_TRIED = 0, REVALIDATE_FAILED = 1 };

	  const tstring objType; // stringified type of the object
	  // const tstring errmsg;  // message inherited from RootException

      ValidityException(const tstring &meth, const tstring &obType,
		  Revalidation reval = NONE_TRIED);

	  virtual const char *what() const throw();
	  virtual const TCHAR *twhat() const throw();
	  virtual ~ValidityException() throw() {};
};


class ValidatedObject
{
  public:
      enum Validity { INVALID = 0, VALID = 1 };
  private:
 	  Validity validity;	// is the object's state valid?
  public:
	  ValidatedObject();

	  virtual ~ValidatedObject();

	  virtual void swap(ValidatedObject &other);

	  // mark object as invalid!
	  virtual void invalidate();

	  // throws an exception if the object is invalid
	  virtual void validate() const;

	  // make the object valid again by trying to get it back
	  // in a valid state, if not reconcilable, throw an exception
	  virtual void revalidate();

	  Validity GetValidity() const;

	  // overrides of this function should invalidate() the object if they discover that
	  // the object is invalid
	  virtual bool valid() const;
};

END_DTL_NAMESPACE

#endif

