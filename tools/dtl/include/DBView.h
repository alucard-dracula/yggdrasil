// -*- C++ -*-
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
// abstraction for views -- main class only
// the nested classes (the DB cursor abstractions)are each defined
// in their own separate headers
// Initial: 9/7/2000 - MG
// Edited: 12/19/2000 - MG - added namespaces

#ifndef DTL_DBVIEW_H
#define DTL_DBVIEW_H


#include "BoundIO.h"
#include "DB_Base.h"
#include "DBConnection.h"
#include "DBDefaults.h"
#include "string_util.h"

#include "std_warn_off.h"
#include <algorithm>
#include <utility>
#include <string>
#include <set>
#include <vector>
#include "std_warn_on.h"

BEGIN_DTL_NAMESPACE

template<class DataObj, class ParamObj> class DB_iterator;

template<class DataObj, class ParamObj> class DB_iterator_with_cols;

template<class DataObj, class ParamObj> class DB_select_insert_iterator;

template<class DataObj, class ParamObj> class DB_select_iterator;

template<class DataObj, class ParamObj> class DB_select_update_iterator;

template<class DataObj, class ParamObj> class DB_insert_iterator;

template<class DataObj, class ParamObj> class DB_update_iterator;

template<class DataObj, class ParamObj> class DB_delete_iterator;

template<class DataObj, class ParamObj> class DB_sql_iterator;

template<class ParamObj> class DynamicDBView;

template<class DataObj, class BCA> class DynamicBCA;

template<class DataObj, class ParamObj> class DB_random_select_iterator_buffer;

// default functors for BCA and BPA -- bind nothing
// wrap function pointers with these objects to use

template<class DataObj> class DefaultBCA 
	// : public STD_::binary_function<BoundIOs &, DataObj &, void>
{
public:
	typedef BoundIOs& first_argument_type;
	typedef DataObj& second_argument_type;
	typedef void result_type;

public:
	 inline void operator()(BoundIOs & DTL_DUMMY_PARAM_1, DataObj & DTL_DUMMY_PARAM_2) const
     {
		DTL_THROW DBException(_TEXT("DefaultBCA::operator()()"), _TEXT("Must define a BCA!"), NULL, NULL);
	 }

};

template<class ParamObj> class DefaultBPA 
	//: public STD_::binary_function<BoundIOs &, ParamObj &, void>
{
public:
	typedef BoundIOs& first_argument_type;
	typedef ParamObj& second_argument_type;
	typedef void result_type;

  public:
	  inline void operator()(BoundIOs & DTL_DUMMY_PARAM_1, ParamObj & DTL_DUMMY_PARAM_2) const
	  { 
	  }
};

template<class ParamObj> class EmptyBPA 
	// : public STD_::binary_function<BoundIOs &, ParamObj &, void>
{
public:
	typedef BoundIOs& first_argument_type;
	typedef ParamObj& second_argument_type;
	typedef void result_type;

  public:
	  inline void operator()(BoundIOs & DTL_DUMMY_PARAM_1, ParamObj & DTL_DUMMY_PARAM_2) const
	  { 
	  }
};

// generic IO handler that expects a signature of
// dtl_ios_base::MeansOfRecovery handler()

template<class DataObj, class ParamObj>
class ImplBaseIO
{
public:

  ImplBaseIO() {}

  virtual ~ImplBaseIO () {}

  virtual ImplBaseIO* clone() const = 0 ;

  virtual dtl_ios_base::MeansOfRecovery invoke(RootException &ex,
											   dtl_ios_base  &base,
											   DataObj       &row,
											   ParamObj      &params) = 0;

  virtual const char * typenm() const = 0;

  // forwarding functions needed by bulk fetch handler
  virtual void RecordFailure(size_t index) = 0;
  virtual void ClearFailures() = 0;
  virtual STD_::vector<size_t> GetFailures() const = 0;
};

// The clone() is implemented here.
template<class DataObj, class ParamObj, typename T>
class ImplIO : public ImplBaseIO<DataObj, ParamObj>
{
public:

  ImplIO (const T &v) : data (v) {}

  virtual ~ImplIO () {}

  virtual ImplBaseIO<DataObj, ParamObj>* clone() const
  {
	ImplBaseIO<DataObj, ParamObj>* copy
	  = new ImplIO<DataObj, ParamObj, T>(data);

	return copy;
  }

  virtual dtl_ios_base::MeansOfRecovery invoke(RootException &ex,
											   dtl_ios_base  &base,
											   DataObj       &row,
											   ParamObj      &params)
  {
	return data(ex, base, row, params);
  }

  virtual const char * typenm() const
  {
	return typeid(data).name();
  }

  // forwarding functions needed by bulk fetch handler
  virtual void RecordFailure(size_t index)
  {
	data.RecordFailure(index);
  }

  virtual void ClearFailures()
  {
	data.ClearFailures();
  }

  virtual STD_::vector<size_t> GetFailures() const
  {
	return data.GetFailures();
  }

  T   data ;

};

template<class DataObj, class ParamObj>
class IOHandler
{
private:

  ImplBaseIO<DataObj, ParamObj>* data; // polymorphically stores the method object

  IOHandler()  { } // Don't allow default constructor -- too dangerous

  template<typename T>
  static ImplIO<DataObj, ParamObj, T>* CastFromBase (ImplBaseIO<DataObj, ParamObj>* v , T const* =NULL )
  {
	ImplIO<DataObj, ParamObj, T>* p;
	p = dynamic_cast<ImplIO<DataObj, ParamObj, T>*> ( v ) ;
	if ( p == NULL )
	{
	  STD_::string errmsg;
	  errmsg.reserve(512);
	  errmsg += typeid(T).name();
	  errmsg += " is not a valid type";

	  DTL_THROW VariantException(_TEXT("IOHandler::CastFromBase()"),
							 tstring_cast((tstring *)NULL, errmsg));
	}
	return p ;

  }

public:

	// act as the error functor ... in reality forward the call
	// to the actual handler itself

	// signature of handler must be:
	// dtl_ios_base::MeansOfRecovery(RootException &ex, dtl_ios_base &base,
	//	   DataObj &data, ParamObj &params;
	// forward call
    dtl_ios_base::MeansOfRecovery operator()(RootException &ex,
											 dtl_ios_base  &base,
											 DataObj       &row,
											 ParamObj      &params)
    {
	  return data->invoke(ex, base, row, params);
	}

	// these forwarding calls needed for bulk fetch handler logic ...
	// shouldn't be necessary, but Sun Workshop compiler barfs on
	// unwrapping the handler

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
	}

    template<typename T> IOHandler ( T const& v )
      : data ( new ImplIO<DataObj, ParamObj, T>(v) )
      {}

	  // * Copy the internal data instead of sharing it.
      IOHandler( const IOHandler & rhs )
      : data ( rhs.data != NULL ? rhs.data->clone()
                                : NULL
             )
      {}

	  // No need to test for concurrent users.
	  ~IOHandler() { delete data ; }


	  // exception-safe swap
	  void swap(IOHandler &h)
	  {
			std::swap(h.data, data);
	  }

	  // assignment operator
	  IOHandler &operator=(const IOHandler &method)
	  {
		   if (this != &method)
		   {
			  IOHandler tmp(method);
			  swap(tmp);
		   }

		   return *this;
	  }

	  template<typename T> const T & get( T const* dmy =NULL ) const
      {
#if defined (__BORLANDC__) // || defined (SOLARIS)
     	ImplIO<DataObj, ParamObj, T>* p;

		if (std_strcmp(data->typenm(), typeid(T).name()) != 0)
			p = NULL;
		else
			p = static_cast<ImplIO<DataObj, ParamObj, T>*> ( data ) ;

		if ( p == NULL )
		{
			STD_::string errmsg;
			errmsg.reserve(512);
			errmsg += typeid(T).name();
			errmsg += " is not a valid type";

			DTL_THROW VariantException(_TEXT("IOHandler::CastFromBase()"),
				tstring_cast((tstring *)NULL, errmsg));
        }
		return p->data ;
#else
		  return DTL_TEMPLATE_FUNC(CastFromBase, T)( data , dmy )->data ;
#endif
      }
};

// translation class from IOHandler<DataObj, ParamObj> to IOHandler<DataObj, DataObj>
// (this actually is a monomorphic IOHandler that contains the dimorphic one we 'converted)
template<class DataObj, class ParamObj> class DimorphToMonomorph
{
private:
	IOHandler<DataObj, ParamObj> io_handler;
public:
	DimorphToMonomorph(IOHandler<DataObj, ParamObj> h) : io_handler(h) { }

	dtl_ios_base::MeansOfRecovery operator()(RootException &ex, dtl_ios_base &base,
		DataObj &data, DataObj & DTL_DUMMY_PARAM_1)
	{
		ParamObj temp;
		return io_handler(ex, base, data, temp);
	}

	// forwarding calls for bulk fetch handler logic ...
	// call sent to appropriate failure recorder
	void RecordFailure(size_t index)
	{
		io_handler.RecordFailure(index);
	}

	void ClearFailures()
	{
		io_handler.ClearFailures();
	}

	STD_::vector<size_t> GetFailures() const
	{
		return io_handler.GetFailures();
	}
};

// template function to make an IOHandler<DataObj, DataObj> out of a
// IOHandler<DataObj, ParamObj> by using a DimorphToMonomorph translator
template<class DataObj, class ParamObj>
	IOHandler<DataObj, DataObj> monomorphize(IOHandler<DataObj, ParamObj> h)
{
	return IOHandler<DataObj, DataObj>(DimorphToMonomorph<DataObj, ParamObj>(h));
}

// DTL has three error handlers that provide predefined behavior

// mixins for recording failed objects from bulk fetch
// all handlers need to inherit from one of these recorders!

// record nothing
class FailureRecorderEmpty
{
public:
	FailureRecorderEmpty() { }

	// do nothings
	virtual void RecordFailure(size_t DTL_DUMMY_PARAM_1) { }
	virtual void ClearFailures() { }
	virtual STD_::vector<size_t> GetFailures() const
    {
		return STD_::vector<size_t>();
	}

	virtual ~FailureRecorderEmpty() { }
};

// record indexes of failed objects
class FailureRecorder
{
private:
	CountedPtr<STD_::vector<size_t> > pFailureIndexes;
public:
	FailureRecorder() : pFailureIndexes(new STD_::vector<size_t>) { }

	// records index of failed DataObj in the handler
	virtual void RecordFailure(size_t index)
	{
		pFailureIndexes->push_back(index);
	}

	// clear out failure list for new call to bulk_copy()
	virtual void ClearFailures()
	{
		pFailureIndexes->clear();
	}

	virtual STD_::vector<size_t> GetFailures() const
	{
		return *pFailureIndexes;
	}

	virtual ~FailureRecorder() { }
};


// this one directs the handling code to always throw

template<class DataObj, class ParamObj = DefaultParamObj<DataObj> > class AlwaysThrowsHandler :
// template<class DataObj, class ParamObj> class AlwaysThrowsHandler :
	public FailureRecorderEmpty
{
public:
	dtl_ios_base::MeansOfRecovery
		operator()(RootException & DTL_DUMMY_PARAM_1,
            dtl_ios_base & DTL_DUMMY_PARAM_2,
            DataObj & DTL_DUMMY_PARAM_3,
            ParamObj & DTL_DUMMY_PARAM_4)
	{
		return dtl_ios_base::THROW_EXCEPTION;
	}

	virtual ~AlwaysThrowsHandler() {}

};

// this handler logs exceptions to a STD_::vector
// then tells caller to reperform the action
template<class DataObj, class ParamObj = DefaultParamObj<DataObj> > class LoggingHandler :
	public FailureRecorderEmpty
{
public:
	struct LoggedTriple
	{
		tstring errmsg;
		DataObj dataObj;
		ParamObj paramObj;

		LoggedTriple() : errmsg(_TEXT("")), dataObj(), paramObj() { }

		LoggedTriple(const tstring &msg, const DataObj &data, const ParamObj &param) :
		  errmsg(msg), dataObj(data), paramObj(param) { }
	};

private:
	CountedPtr<STD_::vector<LoggedTriple> > pErrorLog;

public:

	LoggingHandler() : FailureRecorderEmpty(),
		  pErrorLog(new STD_::vector<LoggedTriple>) { }

	dtl_ios_base::MeansOfRecovery
		operator()(RootException &ex, dtl_ios_base & DTL_DUMMY_PARAM_1 /*base*/,
			DataObj &data, ParamObj &params)
	{
		pErrorLog->push_back(LoggedTriple(ex.twhat(), data, params));

		// throw if the iterator is bad
		// if (base.bad())
		//	 return dtl_ios_base::THROW_EXCEPTION;

		// otherwise take the action that the user passed in at construction time
		return dtl_ios_base::SUPPRESS_ERROR;
	}

	// returns a *copy* of the error log
	STD_::vector<LoggedTriple> GetLog() const
	{
		return *pErrorLog;
	}

	virtual ~LoggingHandler() { }
};

// error handling for bulk fetch ... records the data objects which
// N.B. !! This is DELIBERATELY not a full IO handler
// The idea is to INHERIT from this class and whatever IOHandler you want
// to use.  The purpose of this is just to provide row tracking for bulk
// fetch to use to exclude failed records
// were barfed up on in SelValidate() to a vector
// fatal errors will still propagate through

template<class DataObj, class ParamObj = DefaultParamObj<DataObj> >
class BulkFetchHandler : public LoggingHandler<DataObj, ParamObj>,
public FailureRecorder
{
public:
	virtual void RecordFailure(size_t index)
	{
		FailureRecorder::RecordFailure(index);
	}
	virtual void ClearFailures()
	{
		FailureRecorder::ClearFailures();
	}

	virtual STD_::vector<size_t> GetFailures() const
	{
		return FailureRecorder::GetFailures();
	}
};

template<class DataObj>
class ImplBase0
{
public:

  ImplBase0() {}

  virtual ~ImplBase0 () {}

  virtual  ImplBase0* clone() const = 0 ;

  virtual void invoke(BoundIOs &boundIOs, DataObj &rowbuf) = 0;

  virtual const char * typenm() const = 0;
};

// The clone() is implemented here.
template<class DataObj, typename T>
class Impl0 : public ImplBase0<DataObj>
{
public:

  Impl0 (const T &v) : data (v)  {}

  virtual ~Impl0 () {}

  virtual ImplBase0<DataObj>* clone() const
  {
	ImplBase0<DataObj>* copy = new Impl0<DataObj, T>(data);
	return copy;
  }

  virtual void invoke(BoundIOs &boundIOs, DataObj &rowbuf)
  {
	data(boundIOs, rowbuf);
  }

  virtual const char* typenm() const
  {
	return typeid(data).name();
  }

public:

  T data ;

};


// BCAWrap and BPAWrap are the same, couldn't find way to share code,
// simple inheritance does not work
template<class DataObj> class BCAWrap
{
public:

	  // signature of BCAWrap must be:
	  // void bca(BoundIOs &boundIOs, DataObj &rowbuf)
	  // forward call
  void operator()(BoundIOs &boundIOs, DataObj &rowbuf)
  {
	data->invoke(boundIOs, rowbuf);
  }

private:

  ImplBase0<DataObj>* data; // polymorphically stores the method object

  BCAWrap()  { } // Don't allow default constructor -- too dangerous

  template<typename T>
  static Impl0<DataObj, T>* CastFromBase (ImplBase0<DataObj>* v,
										  T const* = NULL)
   {
      	Impl0<DataObj, T>* p;
		p = dynamic_cast<Impl0<DataObj, T>*> (v) ;
		if ( p == NULL )
		{
			STD_::string errmsg;
			errmsg.reserve(512);
			errmsg += typeid(T).name();
			errmsg += " is not a valid type";

			DTL_THROW VariantException(_TEXT("BCAWrap::CastFromBase()"),
				tstring_cast((tstring *)NULL, errmsg));
		}
		return p ;

    }



public:

     friend class DynamicBCA<DataObj, BCAWrap<DataObj> >;

     template<typename T> BCAWrap ( T const& v )
      : data ( new Impl0<DataObj, T>(v) )
      {}

	  // * Copy the internal data instead of sharing it.
      BCAWrap( const BCAWrap & rhs )
      : data ( rhs.data != NULL ? rhs.data->clone()
                                : NULL
             )
      {}

	  // No need to test for concurrent users.
	  ~BCAWrap() { delete data ; }


	  // exception-safe swap
	  void swap(BCAWrap &h)
	  {
			std::swap(h.data, data);
	  }

	  // assignment operator
	  BCAWrap &operator=(const BCAWrap &method)
	  {
		   if (this != &method)
		   {
			  BCAWrap tmp(method);
			  swap(tmp);
		   }

		   return *this;
	  }

	  template<typename T> const T & get( T const* dmy =NULL ) const
      {
#ifndef __BORLANDC__
		return DTL_TEMPLATE_FUNC(CastFromBase, T)( data , dmy )->data ;
#else
     	Impl0<DataObj, T>* p;

		if (std_strcmp(data->typenm(), typeid(T).name()) != 0)
			p = NULL;
		else
			p = static_cast<Impl0<DataObj, T>*> ( data ) ;

		if ( p == NULL )
		{
			STD_::string errmsg;
			errmsg.reserve(512);
			errmsg += typeid(T).name();
			errmsg += " is not a valid type";

			DTL_THROW VariantException(_TEXT("BCAWrap::CastFromBase()"),
				tstring_cast((tstring *)NULL, errmsg));
		}

		return p->data ;
#endif
	  }

};

template<class DataObj> class BPAWrap
{
public:
  // signature of BPAWrap must be:
  // void bca(BoundIOs &boundIOs, DataObj &rowbuf)
  // forward call
  void operator()(BoundIOs &boundIOs, DataObj &rowbuf)
  {
	data->invoke(boundIOs, rowbuf);
  }


private:

  ImplBase0<DataObj>* data; // polymorphically stores the method object

  BPAWrap()  { } // Don't allow default constructor -- too dangerous

  template<typename T>
  static Impl0<DataObj, T>* CastFromBase (ImplBase0<DataObj>* v,
										  T const* =NULL )
   {
      Impl0<DataObj, T>* p = dynamic_cast<Impl0<DataObj, T>*> ( v ) ;
      if ( p == NULL )
	  {
		  STD_::string errmsg;
		  errmsg.reserve(512);
		  errmsg += typeid(T).name();
		  errmsg += " is not a valid type";

		  DTL_THROW VariantException(_TEXT("BPAWrap::CastFromBase()"),
			tstring_cast((tstring *)NULL, errmsg));
	  }
      return p ;
    }

public:

     template<typename T> BPAWrap ( T const& v )
      : data ( new Impl0<DataObj, T>(v) )
      {}

	  // * Copy the internal data instead of sharing it.
      BPAWrap( const BPAWrap & rhs )
      : data ( rhs.data != NULL ? rhs.data->clone()
                                : NULL
             )
      {}

	  // No need to test for concurrent users.
	  ~BPAWrap() { delete data ; }


	  // exception-safe swap
	  void swap(BPAWrap &h)
	  {
			std::swap(h.data, data);
	  }

	  // assignment operator
	  BPAWrap &operator=(const BPAWrap &method)
	  {
		   if (this != &method)
		   {
			  BPAWrap tmp(method);
			  swap(tmp);
		   }

		   return *this;
	  }

	  template<typename T> const T & get( T const* dmy =NULL ) const
      {
#ifndef __BORLANDC__
		return CastFromBase( data , dmy )->data ;
#else
     	Impl0<DataObj, T>* p;

		if (std_strcmp(data->typenm(), typeid(T).name()) != 0)
			p = NULL;
		else
			p = static_cast<Impl0<DataObj, T>*> (data);

		if ( p == NULL )
		{
			STD_::string errmsg;
			errmsg.reserve(512);
			errmsg += typeid(T).name();
			errmsg += " is not a valid type";

			DTL_THROW VariantException(_TEXT("BPAWrap::CastFromBase()"),
				tstring_cast((tstring *)NULL, errmsg));
        }
		return p->data ;
#endif
	  }


};

template<class DataObj>
class ImplBase1
{
public:

  ImplBase1() {}

  virtual ~ImplBase1 () {}

  virtual  ImplBase1* clone() const = 0 ;

  virtual bool invoke(BoundIOs &boundIOs, DataObj &rowbuf) = 0;

  virtual const char * typenm() const = 0;
};

// The clone() is implemented here.
template<class DataObj, typename T>
struct Impl1 : public ImplBase1<DataObj>
{
public:

  Impl1 (const T &v) : data (v) {}

  virtual ~Impl1 () {}

  virtual ImplBase1<DataObj>* clone() const
  {
	ImplBase1<DataObj>* copy = new Impl1<DataObj, T>(data);
	return copy;
  }

  virtual bool invoke(BoundIOs &boundIOs, DataObj &rowbuf)
  {
	return data(boundIOs, rowbuf);
  }

  virtual const char * typenm() const
  {
	return typeid(data).name();
  }

  T data ;
};

// SelValWrap and InsValWrap are the same - couldn't find a clean way
// to share the template, code - simple inheritance does not work

template<class DataObj> class SelValWrap
{
public:
  // signature of SelValWrap must be:
  // void bca(BoundIOs &boundIOs, DataObj &rowbuf)
  // forward call
  bool operator()(BoundIOs &boundIOs, DataObj &rowbuf)
  {
	return data->invoke(boundIOs, rowbuf);
  }


private:

  ImplBase1<DataObj>* data; // polymorphically stores the method object

  SelValWrap()  { } // Don't allow default constructor -- too dangerous

  template<typename T>
  static Impl1<DataObj, T>* CastFromBase (ImplBase1<DataObj>* v ,
										  T const* =NULL )
   {
      Impl1<DataObj, T>* p = dynamic_cast<Impl1<DataObj, T>*> ( v ) ;
      if ( p == NULL )
	  {
		  STD_::string errmsg;
		  errmsg.reserve(512);
		  errmsg += typeid(T).name();
		  errmsg += " is not a valid type";

		  DTL_THROW VariantException(_TEXT("SelValWrap::CastFromBase()"),
				tstring_cast((tstring *)NULL, errmsg));
	  }
      return p ;
    }


public:

     template<typename T> SelValWrap ( T const& v)
      : data ( new Impl1<DataObj, T>(v) )
     { }

	  // * Copy the internal data instead of sharing it.
      SelValWrap( const SelValWrap & rhs )
      : data ( rhs.data != NULL ? rhs.data->clone(): NULL)
      {}

	  // No need to test for concurrent users.
	  ~SelValWrap() { delete data ; }


	  // exception-safe swap
	  void swap(SelValWrap &h)
	  {
			std::swap(h.data, data);
	  }

	  // assignment operator
	  SelValWrap &operator=(const SelValWrap &method)
	  {
		   if (this != &method)
		   {
			  SelValWrap tmp(method);
			  swap(tmp);
		   }

		   return *this;
	  }

	  template<typename T> const T & get( T const* dmy =NULL ) const
      {
#ifndef __BORLANDC__
		return CastFromBase( data , dmy )->data ;
#else
     	Impl1<DataObj, T>* p;

		if (std_strcmp(data->typenm(), typeid(T).name()) != 0)
			p = NULL;
		else
			p = static_cast<Impl1<DataObj, T>*> ( data ) ;

		if ( p == NULL )
		{
			STD_::string errmsg;
			errmsg.reserve(512);
			errmsg += typeid(T).name();
			errmsg += " is not a valid type";

			DTL_THROW VariantException(_TEXT("SelValWrap::CastFromBase()"),
				tstring_cast((tstring *)NULL, errmsg));
        }

		return p->data ;
#endif
	  }

};


template<class DataObj> class InsValWrap
{
	public:
	  // signature of InsValWrap must be:
	  // void bca(BoundIOs &boundIOs, DataObj &rowbuf)
	  // forward call
	  bool operator()(BoundIOs &boundIOs, DataObj &rowbuf) {
			return data->invoke(boundIOs, rowbuf);
		}


    private:

	ImplBase1<DataObj>* data; // polymorphically stores the method object

    InsValWrap()  { } // Don't allow default constructor -- too dangerous

   template<typename T>
   static Impl1<DataObj, T>* CastFromBase (ImplBase1<DataObj>* v,
											T const* = NULL)
   {
      Impl1<DataObj, T>* p = dynamic_cast<Impl1<DataObj, T>*> (v) ;
      if ( p == NULL )
	  {
		  STD_::string errmsg;
		  errmsg.reserve(512);
		  errmsg += typeid(T).name();
		  errmsg += " is not a valid type";

		  DTL_THROW VariantException(_TEXT("InsValWrap::CastFromBase()"),
				tstring_cast((tstring *)NULL, errmsg));
	  }

      return p ;
    }

public:

     template<typename T> InsValWrap ( T const& v )
      : data ( new Impl1<DataObj, T>(v) )
      {}

	  // * Copy the internal data instead of sharing it.
      InsValWrap( const InsValWrap & rhs )
      : data ( rhs.data != NULL ? rhs.data->clone()
                                : NULL
             )
      {}

	  // No need to test for concurrent users.
	  ~InsValWrap() { delete data ; }


	  // exception-safe swap
	  void swap(InsValWrap &h)
	  {
			std::swap(h.data, data);
	  }

	  // assignment operator
	  InsValWrap &operator=(const InsValWrap &method)
	  {
		   if (this != &method)
		   {
			  InsValWrap tmp(method);
			  swap(tmp);
		   }

		   return *this;
	  }

	  template<typename T> const T & get( T const* dmy =NULL ) const
      {

#ifndef __BORLANDC__
		  return CastFromBase( data , dmy )->data ;
#else
     	Impl1<DataObj, T>* p;

		if (std_strcmp(data->typenm(), typeid(T).name()) != 0)
			p = NULL;
		else
			p = static_cast<Impl1<DataObj, T>*> ( data ) ;

		if ( p == NULL )
		{
			STD_::string errmsg;
			errmsg.reserve(512);
			errmsg += typeid(T).name();
			errmsg += " is not a valid type";

			DTL_THROW VariantException(_TEXT("InsValWrap::CastFromBase()"),
				tstring_cast((tstring *)NULL, errmsg));
        }

		return p->data ;
#endif
	  }


};


// **** stuff needed for building custom queries for DBView's ****
// (more follows DBView definition)

// forward declaration needed for DBView
template<class DataObj, class ParamObj> class DBView;

// forward declaration needed for BuildDefaultQry
template<class DataObj, class ParamObj> tstring
	BuildDefaultQry(const DBView<DataObj, ParamObj> &view, SQLQueryType qryType, const tstring &QuoteChar);

// generic version of BuildSpecialQry just calls the default BuildQry() mechanism
// (users: specialize this functor in your code to create your own
// mechanism for building a query for your DBView type)
template<class DataObj, class ParamObj = DefaultParamObj<DataObj> >
class BuildSpecialQry
{
public:
	tstring operator()(const DBView<DataObj, ParamObj> &view, SQLQueryType qryType, const tstring &QuoteChar) const
	{
		return BuildDefaultQry(view, qryType, QuoteChar);
	}

};

// template to enforce that only variant_row can use autokey mode
//
template<typename T> void CheckKeyMode(KeyMode &keyMode, T* DTL_DUMMY_PARAM_1) {
		keyMode = USE_ALL_FIELDS;
}

// template to enforce that only variant_row can use autokey mode
//
inline void CheckKeyMode(KeyMode & DTL_DUMMY_PARAM_1, variant_row * DTL_DUMMY_PARAM_1) {
}

// **** DBView definition ****

// default parameter object for a data class can either use a general template which will
// contain zero parameters to bind to or specialize the DefaultParamObj template to get
// the desired default behavior you want
template<class DataObj, class ParamObj = DefaultParamObj<DataObj> >
class DBView
{
public:
	typedef BCAWrap<DataObj> BCA;
	typedef BPAWrap<ParamObj> BPA;
	typedef InsValWrap<DataObj> InsVal;
	typedef SelValWrap<DataObj> SelVal;
    typedef IOHandler<DataObj, ParamObj> IOH;
    // typedef void (*BCA) (BoundIOs &boundIOs, DataObj &rowbuf);
    // typedef void (*BPA) (BoundIOs &boundIOs, ParamObj &paramBuf);

	// standard STL typedefs
	typedef DataObj value_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;

	typedef DB_select_iterator<DataObj, ParamObj> const_iterator;
    typedef const_iterator iterator;
	typedef std::ptrdiff_t difference_type;
	typedef size_t size_type;

	// typedefs for cursor abstractions
	// should be accessible as DBView<DataObj, ParamObj>::name
	typedef DB_iterator<DataObj, ParamObj> base_iterator;
	typedef DB_iterator_with_cols<DataObj, ParamObj> iterator_with_cols;
	typedef DB_select_insert_iterator<DataObj, ParamObj> select_insert_iterator;
	typedef DB_select_iterator<DataObj, ParamObj> select_iterator;
	typedef DB_insert_iterator<DataObj, ParamObj> insert_iterator;
	typedef DB_update_iterator<DataObj, ParamObj> update_iterator;
	typedef DB_delete_iterator<DataObj, ParamObj> delete_iterator;
	typedef DB_sql_iterator<DataObj, ParamObj> sql_iterator;

	typedef DB_select_update_iterator<DataObj, ParamObj> select_update_iterator;

	// typedefs exposed for binding
	typedef DataObj DataObject;
	typedef ParamObj ParamObject;

protected:
	tstring		rawSQL;		// raw SQL -- used for custom queries
    STD_::set<tstring> tableNames;
    STD_::vector<tstring> colNames;
	STD_::vector<tstring> keyNames;
    tstring postfixClause;
    BCA bca; // bind column addr. fn. - used for all column abstractions
	         // select, insert, update
    BPA bpa; // bind posfix addr. fn. - used for postfix clauses in all contexts
    unsigned int numParams;
    unsigned int numColumns;
    InsVal InsValidate;  // insert validate fn. - used both for insert and update
    SelVal SelValidate; // select validate fn. - used for select

	mutable IOHandler<DataObj, ParamObj> io_handler; // the default error handler for iterators over this view

    DBConnection *pConn;  // pointer to connection used by this view
	mutable iterator *pEnd;

	KeyMode keyMode;

public:

   	// only allow key mode to be set by subclasses
    DBView(const tstring tableList, const BCA bca_fn,
		   const tstring postfix, const BPA bpa_fn,
		   const SelVal sel_val,
		   const InsVal ins_val,
		   const IOH io_hand,
		   DBConnection &connection,
		   const tstring keyList,
		   KeyMode km) :
		   rawSQL(tableList),
		   tableNames(ParseTableList(tableList)),
		   colNames(),
		   keyNames(ParseCommaDelimitedList(keyList)),
		   postfixClause(postfix), bca(bca_fn), bpa(bpa_fn),
		   numParams(0), numColumns(0),
		   InsValidate(ins_val),
		   SelValidate(sel_val),
		   io_handler(io_hand), 		   pConn(&connection),
		   pEnd(NULL),
		   keyMode(km)
    {
		// logic to coerce keyMode to USE_ALL_FIELDS in the event we don't
		// have a DynamicDBView ... we must not allow a static DBView to be set to use autokeys
		CheckKeyMode(keyMode, (DataObj *)NULL);

		DataObj dummy_obj;
	    ParamObj param_obj;
	    // BoundIOs dummy_bc(BoundIO::BIND_ADDRESSES);
		BoundIOs dummy_bc;

		dummy_bc.BindAsBase(dummy_obj);

		bca(dummy_bc, dummy_obj);

		// BoundIOs dummy_bp(BoundIO::BIND_ADDRESSES);
		BoundIOs dummy_bp;

		dummy_bp.BindAsBase(param_obj);

		bpa(dummy_bp, param_obj);

		colNames = dummy_bc.GetColumnNames();

        numParams = dummy_bp.NumParams() + dummy_bc.NumParams();
        numColumns = colNames.size();
    }
public:
  // yes, as a member of DBView ...
  class Args
  {
  private: // one member for each possible argument
	tstring tableList;
	tstring fieldList; // used for DynamicDBView only
	tstring keyList;
	KeyMode keyMode; // USE_PK_ONLY, USE_ALL_FIELDS, USE_AUTO_KEY (autokey for DynamicDBView only)
	STD_::vector<tstring> appendedFieldNames; // used for DynamicDBView only
	BCA bca_fn;
	tstring postfixClause;
    BPA bpa_fn;
    SelVal sel_val;
    InsVal ins_val;
	IOHandler<DataObj, ParamObj> io_hand;
	DBConnection *pDBconnection;

  public:

	friend class DBView<DataObj, ParamObj>;
	friend class DynamicDBView<ParamObj>;

	// main constructor initializes the arguments to their defaults
	// as per the current 7 argument DBView constructor
	// must specify tablenames ... force this constraint by having
	// the DBView constructor check to make sure that string is nonempty

	// we get sneaky here by defaulting keyMode to USE_AUTO_KEY
	// it will get coerced anyway to USE_ALL_FIELDS if the view is static
	// (that is, it doesn't use DynamicRowBCA)
	Args() : tableList(_TEXT("")), fieldList(_TEXT("")), keyList(_TEXT("")), keyMode(USE_AUTO_KEY),
		 appendedFieldNames(),
		bca_fn(DefaultBCA<DataObj>()), postfixClause(_TEXT("")),
		bpa_fn(DefaultBPA<ParamObj>()), sel_val(DefaultSelValidate<DataObj>()),
		ins_val(DefaultInsValidate<DataObj>()),
		io_hand(DEFAULT_IO_HANDLER<DataObj, ParamObj>()),
		pDBconnection(&DBConnection::GetDefaultConnection())
	{ }

	// override defaults by calling these accessors
	// (note: none for tables as that must be specified w/ no default)

	Args &tables(const tstring &tablenames)
	{
		tableList = tablenames;
		return *this;
	}

	Args &fields(const tstring &f)
	{
		fieldList = f;
		return *this;
	}

	Args &keys(const tstring &k)
	{
		keyList = k;
		return *this;
	}

	Args &key_mode(KeyMode mode)
	{
		keyMode = mode;
		return *this;
	}

	Args &bca(const BCA user_bca)
	{
		bca_fn = user_bca;
		return *this;
	}

	Args &postfix(tstring user_postfix)
	{
		postfixClause = user_postfix;
		return *this;
	}

	Args &bpa(const BPA user_bpa)
	{
		bpa_fn = user_bpa;
		return *this;
	}

	Args &SelValidate(const SelVal user_sel_val)
	{
		sel_val = user_sel_val;
		return *this;
	}

	Args &InsValidate(const InsVal user_ins_val)
	{
		ins_val = user_ins_val;
		return *this;
	}

	Args &handler(const IOHandler<DataObj, ParamObj> user_io_hand)
	{
		io_hand = user_io_hand;
		return *this;
	}

	Args &conn(DBConnection &puser_conn)
      {
		pDBconnection = &puser_conn;
		return *this;
      }

	}; // end class Args

    // constructor accepting an Args object
    DBView(const Args &args) :
		   rawSQL(args.tableList),
		   tableNames(ParseTableList(args.tableList)),
		   colNames(), keyNames(ParseCommaDelimitedList(args.keyList)),
		   postfixClause(args.postfixClause), bca(args.bca_fn), bpa(args.bpa_fn),
		   numParams(0), numColumns(0), InsValidate(args.ins_val),
		   SelValidate(args.sel_val),
		   io_handler(args.io_hand), pConn(args.pDBconnection),
		   pEnd(NULL),
		   keyMode(args.keyMode)
    {
		// logic to coerce keyMode to USE_ALL_FIELDS in the event we don't
		// have a DynamicDBView ... we must not allow a static DBView to be set to use autokeys
		CheckKeyMode(keyMode, (DataObj *)NULL);

		DataObj dummy_obj;
	    ParamObj param_obj;
	    // BoundIOs dummy_bc(BoundIO::BIND_ADDRESSES);

		BoundIOs dummy_bc;

		dummy_bc.BindAsBase(dummy_obj);

		bca(dummy_bc, dummy_obj);

		// BoundIOs dummy_bp(BoundIO::BIND_ADDRESSES);

		BoundIOs dummy_bp;

		dummy_bp.BindAsBase(param_obj);

		bpa(dummy_bp, param_obj);

		colNames = dummy_bc.GetColumnNames();

        numParams = dummy_bc.NumParams() + dummy_bp.NumParams();
        numColumns = colNames.size();
    }

	// needed only for default constructor of DBIndex .. never use
	DBView() : rawSQL(), tableNames(), colNames(), keyNames(), postfixClause(_TEXT("")), bca(DefaultBCA<DataObj>()), bpa(DefaultBPA<ParamObj>()),
				numParams(0), numColumns(0),
				InsValidate(DefaultInsValidate<DataObj>()),
				SelValidate(DefaultSelValidate<DataObj>()),
				io_handler(DEFAULT_IO_HANDLER<DataObj, ParamObj>()), pConn(&(DBConnection::GetDefaultConnection())),
				pEnd(NULL), keyMode(USE_ALL_FIELDS) { }


    // constructor ... build view representation with table names
    // only actually initializes table names
    // _TEXT("tables") is a comma separated list of table names
    // NULL for any of the function pointers means no action is performed in that context
    // for that view

	typedef  DEFAULT_IO_HANDLER<DataObj, ParamObj> DIOH;

    DBView(const tstring tableList, const BCA bca_fn = DefaultBCA<DataObj>(),
		   const tstring postfix = _TEXT(""), const BPA bpa_fn = DefaultBPA<ParamObj>(),
		   const SelVal sel_val = DefaultSelValidate<DataObj>(),
		   const InsVal ins_val = DefaultInsValidate<DataObj>(),
		   const IOH io_hand =  DIOH(),
		   DBConnection &connection = DBConnection::GetDefaultConnection(),
		   const tstring keyList = _TEXT("")) :
		   rawSQL(tableList),
		   tableNames(ParseTableList(tableList)), colNames(),
		   keyNames(ParseCommaDelimitedList(keyList)),
		   postfixClause(postfix), bca(bca_fn), bpa(bpa_fn),
		   numParams(0), numColumns(0),
		   InsValidate(ins_val),
		   SelValidate(sel_val),
		   io_handler(io_hand), 		   pConn(&connection),
		   pEnd(NULL),
		   keyMode(USE_ALL_FIELDS)
    {
		DataObj dummy_obj;
	    ParamObj param_obj;
	    // BoundIOs dummy_bc(BoundIO::BIND_ADDRESSES);
		BoundIOs dummy_bc;

		dummy_bc.BindAsBase(dummy_obj);

		bca(dummy_bc, dummy_obj);

		// BoundIOs dummy_bp(BoundIO::BIND_ADDRESSES);
		BoundIOs dummy_bp;

		dummy_bp.BindAsBase(param_obj);

		bpa(dummy_bp, param_obj);

		colNames = dummy_bc.GetColumnNames();

        numParams = dummy_bc.NumParams() + dummy_bp.NumParams();
        numColumns = colNames.size();
    }

	// copy constructor and assignment operator required for Assignable and swap()
	// note that as the list of active rowbufs and the ref count map refer to DataObj's
	// in use by iterators of the particular view and no iterators are held by a
    // freshly constructed DBView, the list and map are made empty in the copy built
	DBView(const DBView<DataObj, ParamObj> &view) : rawSQL(view.rawSQL), tableNames(view.tableNames), colNames(view.colNames), keyNames(view.keyNames), postfixClause(view.postfixClause), bca(view.bca), bpa(view.bpa),
		numParams(view.numParams), numColumns(view.numColumns), InsValidate(view.InsValidate), SelValidate(view.SelValidate),
		io_handler(view.io_handler), pConn(view.pConn), pEnd(NULL), keyMode(view.keyMode)
	{ }

	// note that all existing iterators into the destination view become invalid
	// upon assignment of a new view!!!!
	DBView<DataObj, ParamObj> &operator=(const DBView<DataObj, ParamObj> &view)
	{
		if (this != &view)
		{
			DBView<DataObj, ParamObj> temp(view);
			swap(temp);
		}

		return *this;

	}

	// thanks to our custom copy constructor and assignment operator, swap()
	// should behave as desired!
	// Note that all iterators pointing into *this and other are invalidated
	// upon calling this function!!!!

	// rewritten to make exception-safe - MG - 1/10/2001
	void swap(DBView<DataObj, ParamObj> &other)
	{
		bca.swap(other.bca);
		bpa.swap(other.bpa);
		colNames.swap(other.colNames);
		InsValidate.swap(other.InsValidate);
		STD_::swap(numColumns, other.numColumns);
		STD_::swap(numParams, other.numParams);
		postfixClause.swap(other.postfixClause);
		SelValidate.swap(other.SelValidate);
		rawSQL.swap(other.rawSQL);
		tableNames.swap(other.tableNames);
		keyNames.swap(other.keyNames);
		STD_::swap(keyMode, other.keyMode);
		io_handler.swap(other.io_handler);
		STD_::swap(pEnd, other.pEnd);
		STD_::swap(pConn, other.pConn);
	}

    // delete any cached iterators
    ~DBView() {
		delete pEnd;
	}

    // return number of parameters expected in query
    unsigned int GetBoundParamCount() const
    {
	  return numParams;
    }

    // get number of columns in query
    unsigned int GetColumnCount() const
    {
	  return numColumns;
    }

	// set and get error handler functions
	// similar to set_new_handler() and get_new_handler() in the standard library

	// Note: no outstanding iterators over a view are affected when the handler is
	// changed, but any iterators created after the handler is reset will
	// use the new handler by default

	template<class UserHandler> const UserHandler &
		get_io_handler(const UserHandler *dummy) const
	{
		return io_handler.get(dummy);
	}

	// returns raw IOHandler object

	IOHandler<DataObj, ParamObj> &get_io_handler() const
	{
		return io_handler;
	}

	// pass by value instead of const reference as otherwise const_cast occurs
	// and h would change
	void set_io_handler(IOHandler<DataObj, ParamObj> h)
	{
		io_handler.swap(h);
	}

    // parse postfix clause to figure out the number of parameters expected by query
    unsigned int ParsePostfixForParams() const
    {
	  return numOfOccurrences('?', postfixClause);
    }

    // are the 2 views the same?
	// really we want the two views to actually be the same reference
	// given the design of the rest of DBObjects
    friend bool operator==(const DBView &v1, const DBView &v2)
    {
		return &v1 == &v2;
    }

    // are the 2 views different?
    friend bool operator!=(const DBView &v1, const DBView &v2)
    {
	  return !(v1 == v2);
    }


	// parse a comma-separated list of table names, packaging the result in a sorted set
	// of the strings
	static STD_::set<tstring> ParseTableList(tstring tableList)
	{
	  STD_::vector<tstring> parsedVec = ParseCommaDelimitedList(tableList);

	  STD_::set<tstring> parsedSet;

	  // copy list into a set, which is properly sorted for our needs

	  for (STD_::vector<tstring>::iterator it = parsedVec.begin(); it != parsedVec.end(); it++)
		  parsedSet.insert(*it);

	  return parsedSet;
	}


    // return a SQL query based on the query type
    // uses the tableNames, colNames, and postfixClause structures
    tstring BuildQry(SQLQueryType qryType) const
	{
	  return BuildSpecialQry<DataObj, ParamObj>()(*this, qryType, pConn->GetQuoteChar());
	}

	// remove column with given name from the view
	// only applies for autokey case
	void RemoveColName(const tstring &colNm)
	{
            STD_::vector<tstring> tmp;
            for (STD_::vector<tstring>::iterator it =  colNames.begin(); it != colNames.end(); ++it) {
                   if (*it != colNm)
                        tmp.push_back(*it);
            }

            colNames.swap(tmp);
#if 0
            // proper way to remove colNm from a vector (see Item 8 in Meyers' "Effective STL"
            // Borland rejects.
            typedef STD_::vector<tstring>::iterator vit;
            STD_::vector<tstring>::iterator remove_it =
                STD_::remove<tstring *, tstring>(colNames.begin(), colNames.end(), colNm);
	    colNames.erase(remove_it, colNames.end());
#endif

	}

	// accessors

	// return a variant_row initialized by the BCA as a prototype
	DataObj GetDataObj() const {
		DataObj dummy_obj;
		// BoundIOs dummy_bc(BoundIO::BIND_ADDRESSES);
		BoundIOs dummy_bc;
		dummy_bc.BindAsBase(dummy_obj);
		DBView *mutable_this = const_cast<DBView *>(this);
		//conceptually const - tho in practice bca may be initialized here
		mutable_this->bca(dummy_bc, dummy_obj);
		return dummy_obj;
	}

	// return a variant_row initialized by the BPA as a prototype
	ParamObj GetParamObj() const {
		ParamObj dummy_obj;
		// BoundIOs dummy_bc(BoundIO::BIND_ADDRESSES);
		BoundIOs dummy_bc;
		dummy_bc.BindAsBase(dummy_obj);
		DBView *mutable_this = const_cast<DBView *>(this);
		//conceptually const - tho in practice bpa may be initialized here
		mutable_this->bpa(dummy_bc, dummy_obj);
		return dummy_obj;
	}


    tstring GetPostfixClause() const    { return postfixClause; }

	tstring GetRawSQL() const		   { return rawSQL;			  }

    const BCA & GetBCA() const				   { return bca;			  }
	const BCA & RefreshBCA(HSTMT * DTL_DUMMY_PARAM_1) { return bca;			  }

    const BPA & GetBPA() const				   { return bpa;			  }

	void SetBPA(const BPA &new_bpa)
	{
		bpa = new_bpa;

		// fix the count for the number of parameters since we have only now set the bpa
		ParamObj param_obj;
		BoundIOs dummy_bp;
		dummy_bp.BindAsBase(param_obj);
		bpa(dummy_bp, param_obj);
		numParams =  dummy_bp.NumParams();
	}

    InsVal GetInsVal() const		   { return InsValidate;   }

    SelVal GetSelVal() const		   { return SelValidate;   }

    DBConnection &GetConnection() const{ return *pConn;          }

    const STD_::set<tstring> & GetTableNames() const  { return tableNames;    }

	const STD_::vector<tstring> & GetColNames() const
	{
		return colNames;
	}

	// set and get fields of primary key
	const STD_::vector<tstring> &GetKey() const
	{
		return keyNames;
	}

	void SetKey(const tstring &keys)
	{
		keyNames = ParseCommaDelimitedList(keys);
	}

	// set keys for autokey mode
	// if empty, that means the used database doesn't support autokeys
	// we then default to USE_ALL_FIELDS if that happens
	void SetAutoKey(const tstring &keys)
	{
		if (keyMode == USE_AUTO_KEY)
		{
			STD_::vector<tstring> tmp_keyNames = ParseCommaDelimitedList(keys);

			if (tmp_keyNames.empty())
				keyMode = USE_ALL_FIELDS;
			else
				keyNames = tmp_keyNames;
		}
	}

	KeyMode GetKeyMode() const
	{
		return keyMode;
	}

    // return _TEXT("dummy") iterators for beginning and end of view
	// final definitions for begin() , end() moved to select_iterator.h
	const_iterator begin() const;
	const const_iterator & end() const;
	iterator begin();

	friend class DB_iterator<DataObj, ParamObj>;
};

tstring MakeQryString( const tstring &postfixClause ,
	  const tstring &rawSQL ,
	  const STD_::set<tstring> & tableNames ,
	  const STD_::vector<tstring> & colNames ,
	  SQLQueryType qryType, const tstring &QuoteChar, const tstring &DBQuoteChar);

// general case does not quote the field names or table names
// for everything but variant_rows, user must explicitly use quotemarks around
// field names or table names that need them
template<class DataObj> tstring BuildQryString(const tstring &postfixClause,
	  const tstring &rawSQL,
	  const STD_::set<tstring> & tableNames,
	  const STD_::vector<tstring> & colNames,
	  SQLQueryType qryType, const tstring &QuoteChar, const DataObj *DTL_DUMMY_PARAM_1)
{
	return MakeQryString(postfixClause, rawSQL, tableNames, colNames, qryType,  _TEXT(""), QuoteChar);
}

// we must quote the field names and table names for variant_rows to properly
// protect the innocent
template<> inline tstring BuildQryString<variant_row>(const tstring &postfixClause,
	  const tstring &rawSQL,
	  const STD_::set<tstring> & tableNames,
	  const STD_::vector<tstring> & colNames,
	  SQLQueryType qryType, const tstring &QuoteChar, const variant_row *DTL_DUMMY_PARAM_1)
{
	return MakeQryString(postfixClause, rawSQL, tableNames, colNames, qryType, QuoteChar, QuoteChar);
}

// default mechanism used to build a query based on a DBView and the SQLQueryType
template<class DataObj, class ParamObj> tstring
BuildDefaultQry(const DBView<DataObj, ParamObj> &view,
				SQLQueryType                     qryType,
				const tstring                   &QuoteChar)
{
	  // get the necessary info. we need from the view
	  tstring postfixClause = view.GetPostfixClause();
	  tstring rawSQL = view.GetRawSQL();
	  const STD_::set<tstring> & tableNames = view.GetTableNames();
	  const STD_::vector<tstring> & colNames = view.GetColNames();

	  return BuildQryString(postfixClause, rawSQL, tableNames, colNames, qryType, QuoteChar,
		  (const DataObj *) NULL);
}


END_DTL_NAMESPACE


// We need to include select_iterator.h here since DBView returns a select_iterator with begin() and end()
// so this header is needed for proper destruction.
#include "select_iterator.h"

#endif
