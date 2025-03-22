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
// root exception type for DBObjects
// Initial: 11/14/2000 - MG
// Edited: 12/19/2000 - MG - added namespaces

// derives from standard STL exception class
#ifndef DTL_ROOT_EXCEPTION_H
#define DTL_ROOT_EXCEPTION_H

#include "dtl_config.h"
#include "DB_Base.h"
#include "CountedPtr.h"
#include "RootException.h"

#include "std_warn_off.h"
#include <iosfwd>
#include <string>
#include <exception>
#include "std_warn_on.h"

#ifdef DTL_MINGW64
	#include <typeinfo>
#endif

#include "clib_fwd.h"
BEGIN_DTL_NAMESPACE

class RootException : public STD_::exception
{
public:
  const tstring method;
  const tstring errmsg;
  const tstring exType;

  mutable tstring whatbuf;	// buffer used to store result of what() operation
							// so it doesn't get destroyed on us

#ifndef DTL_NOT_UNICODE
  mutable STD_::string whatbuf_narrow;
#endif

  RootException();

  RootException(const tstring &meth, const tstring &err,
	  const tstring &excType = _TEXT("RootException"));

  friend tostream &operator<<(tostream &o, const RootException &ex);

  // override if you have additional members to print
  virtual const char *what() const throw();
  virtual const TCHAR *twhat() const throw();
  virtual ~RootException() throw() {};

};

// exception class when something goes wrong in Variants
class VariantException : public RootException
{
public:
        VariantException(const tstring &meth, const tstring &msg);
        virtual ~VariantException() throw(){};
                                                                                                 
        // superclass behavior ok for what()
};


// global error handler functionality

class ErrorSeverity
{
public:
	enum SeverityCode {ERR_FAILURE = 0, ERR_WARNING = 1};
};

// generic IO handler that expects a signature of
// dtl_ios_base::MeansOfRecovery handler(const RootException *,
//										 ErrorSeverity::SeverityCode)

class ImplBaseErr
{
  public:
     ImplBaseErr() {}

     virtual ~ImplBaseErr () {}

	 virtual ImplBaseErr* clone() const = 0 ;

	 virtual dtl_ios_base::MeansOfRecovery invoke(const RootException *pEx, 
												ErrorSeverity::SeverityCode severity) = 0;

	 virtual const char * typenm() const = 0;

     // put any forwarding functions here needed for interface
};

// The clone() is implemented here.
template<typename T> class ImplErr : public ImplBaseErr
{
public:

  ImplErr (const T &v) : data (v) {}

  virtual ~ImplErr () {}

  virtual ImplBaseErr* clone() const
  { 
	ImplBaseErr* copy = new ImplErr<T>(data);
	return copy; 
  }

  virtual dtl_ios_base::MeansOfRecovery invoke(const RootException *pEx, 
											   ErrorSeverity::SeverityCode severity)
  {
	return data(pEx, severity);
  }

  virtual const char * typenm() const 
  {
	return typeid(data).name();
  }

  // put any forwarding functions here needed for interface

  T   data ;

};
 
class ErrorHandler_t
{
private: 
    
  ImplBaseErr* data; // polymorphically stores the error handler method object

  ErrorHandler_t()  { } // Don't allow default constructor -- too dangerous

  template<typename T>
  static ImplErr<T>* CastFromBase (ImplBaseErr* v , T const* =NULL )
  {
	ImplErr<T>* p;
	p = dynamic_cast<ImplErr<T>*> ( v ) ;
	if ( p == NULL )
	{
	  STD_::string errmsg;
	  errmsg.reserve(512);
	  errmsg += typeid(T).name();
	  errmsg += " is not a valid type";
	  
	  throw VariantException(_TEXT("ErrorHandler_t::CastFromBase()"),
							 tstring_cast((tstring *)NULL, errmsg));
	}
	return p ;
	
  }

public:

	virtual ErrorHandler_t* clone() const
	{
		ErrorHandler_t* copy = new ErrorHandler_t(*this);
		return copy;
	}

	// act as the error functor ... in reality forward the call
	// to the actual handler itself

	// signature of handler must be:
	// dtl_ios_base::MeansOfRecovery(const RootException *pEx,
	//								 ErrorSeverity::SeverityCode severity);
	// forward call
    dtl_ios_base::MeansOfRecovery operator()(const RootException *pEx,
											 ErrorSeverity::SeverityCode severity)
    {
	  return data->invoke(pEx, severity);
	}

	// forwarding calls needed for any fetch handler logic ...
	// shouldn't be necessary, but Sun Workshop compiler barfs on
	// unwrapping the handler

	/* examples:

	void RecordFailure(size_t index)
	{
	  data->RecordFailure(index);
	}

	void ClearFailures()
	{
	  data->ClearFailures();
	}

	STD_::vector<size_t> GetFailures() const
	{
	  return data->GetFailures();
	} */

    template<typename T> ErrorHandler_t ( T const& v )
      : data ( new ImplErr<T>(v) )
      {}

	// * Copy the internal data instead of sharing it.
    ErrorHandler_t( const ErrorHandler_t & rhs )
      : data ( rhs.data != NULL ? rhs.data->clone()
                                : NULL
             )
      {}

	// No need to test for concurrent users.
	virtual ~ErrorHandler_t() { delete data ; }

	   
	// exception-safe swap
	void swap(ErrorHandler_t &h)
	{
		STD_::swap(h.data, data);
	}

	// assignment operator
	ErrorHandler_t &operator=(const ErrorHandler_t &method)
	{
		if (this != &method)
		{
			ErrorHandler_t tmp(method);
			swap(tmp);
		}

		return *this;
	}

	template<typename T> const T & get( T const* dmy =NULL ) const
    {
#if defined (__BORLANDC__) // || defined (SOLARIS)
     	ImplErr<T>* p;

		if (std_strcmp(data->typenm(), typeid(T).name()) != 0) 
			p = NULL;
		else
			p = static_cast<ImplErr<T>*> ( data ) ;

		if ( p == NULL )
		{
			STD_::string errmsg;
			errmsg.reserve(512);
			errmsg += typeid(T).name();
			errmsg += " is not a valid type";

			throw VariantException(_TEXT("ErrorHandler_t::CastFromBase()"),
				tstring_cast((tstring *)NULL, errmsg));
        } 
		return p->data ;
#else
		return DTL_TEMPLATE_FUNC(CastFromBase, T)( data , dmy )->data ;
#endif
     }
};

class ErrorHandler
{
private:
    // typedef dtl_ios_base::MeansOfRecovery (*ErrorHandler_t)(const RootException *, ErrorSeverity severity);
 
	CountedPtr<ErrorHandler_t> curHandler;

public:
	ErrorHandler() : curHandler(NULL) {}
	
	// Use this function to clear the error handler, i.e. set it to NULL
	void ClearHandler() 
	{
		curHandler = NULL;
	}

	// this will allow the passing in of a raw function ptr. or functor
	// N. B.: do not try to pass NULL to this function
	template<class Func> void SetHandler(const Func &raw_h)
	{
		curHandler = new ErrorHandler_t(raw_h);
	}

	// -- not sure about cloning handler --
	// to be consistent with the way IO handlers work, we would want to
	// return a pointer to the handler
	// (if the handler could not be NULL, we'd return a reference)
	// but the other version of GetHandler() is only safe if we return a clone to
	// the encapsulated functor ... so it's best to be consistent within
	// this class and return a *copy* of the functor
	CountedPtr<ErrorHandler_t> GetHandler() const
	{
		if (!curHandler)
		{
			return NULL;
		}

		return curHandler->clone();
	}

	// if the user knows the data type of the handler, he can use this version
	// of GetHandler
	template<class Func> void GetHandler(CountedPtr<Func> &pFunc) const
	{
		if (!curHandler)
		{
			pFunc = NULL;
		}
		else
		{
			pFunc = new Func(curHandler->get((Func *) NULL));
		}
	}

	template<class errorType> void handle_err(const errorType &err,
		ErrorSeverity::SeverityCode severity) 
	{
		const RootException * rerr = &err;
		if (curHandler)
		{
			if ((*curHandler)(rerr, severity) == dtl_ios_base::THROW_EXCEPTION)
			   throw err;
		}
		else
		{
			if (severity == ErrorSeverity::ERR_FAILURE)
				throw err;
		}
	}
};

template<class errorType> void operator-(ErrorHandler &h, const errorType &err) 
{
	h.handle_err(err, ErrorSeverity::ERR_FAILURE);
}

template<class errorType> void operator+(ErrorHandler &h, const errorType &err) 
{
	h.handle_err(err, ErrorSeverity::ERR_WARNING);
}

ErrorHandler &GetErrorHandler();

#define DTL_THROW  dtl::GetErrorHandler() - 
#define DTL_WARN  dtl::GetErrorHandler() + 

END_DTL_NAMESPACE



#endif
