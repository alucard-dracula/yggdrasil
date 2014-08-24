// -*- C++ -*-
// iterator used for SELECTS
// nested class of DBView
// Initial: 9/8/2000 - MG
/* Copyright ? 2000
Michael Gradman and Corwin Joy

Permission to use, copy, modify, distribute and sell this software and
its documentation for any purpose is hereby granted without fee, provided
that the above copyright notice appears in all copies and that both that
copyright notice and this permission notice appear in supporting documentation.
Corwin Joy and Michael Gradman make no representations about the suitability
of this software for any purpose.
It is provided "as is" without express or implied warranty.
*/
// Edited: 12/19/2000 - MG - added namespaces

#ifndef DTL_SELECT_ITERATOR_H
#define DTL_SELECT_ITERATOR_H

#include <climits>

#include "select_insert_iterator.h"
#include "RootException.h"
#include "DBStmt.h"


BEGIN_DTL_NAMESPACE


template <class DataObj> inline void SetWorkingRowAddr(DataObj * DTL_DUMMY_PARAM_1, BoundIOs &b, BYTE *addr, TIMESTAMP_STRUCT *pts) {
	b.SetWorkingAddr(addr, pts);
}

template<> inline void SetWorkingRowAddr<variant_row>(variant_row * DTL_DUMMY_PARAM_1, BoundIOs &b, BYTE *addr, TIMESTAMP_STRUCT *pts) {
	b.SetWorkingAddr(addr + sizeof(variant_row), pts);
}

template <class DataObj> inline void SetComplexRowAddr(DataObj * DTL_DUMMY_PARAM_1, BoundIOs &b, BYTE *addr, TIMESTAMP_STRUCT *pts) {
	b.SetComplexAddr(addr, pts);
}

template<> inline void SetComplexRowAddr<variant_row>(variant_row * DTL_DUMMY_PARAM_1, BoundIOs &b, BYTE *addr, TIMESTAMP_STRUCT *pts) {
	b.SetComplexAddr(addr + sizeof(variant_row), pts);
}


// iterator used for SELECTS
template<class DataObj, class ParamObj = DefaultParamObj<DataObj> >
class DB_select_iterator :
     public DB_select_insert_iterator<DataObj, ParamObj>,
#ifndef __SGI_STL_PORT
     public STD_::iterator<STD_::input_iterator_tag, DataObj, std::ptrdiff_t>
#else
	 // not sure why STLport not recognizing the standard iterator tag
	 // when used in algorithms like
     // copy(_InputIter __first, _InputIter __last, _OutputIter __result)
	 // use STLport specific alternative
	 public STD_::input_iterator<DataObj, std::ptrdiff_t>
#endif
{
#ifdef __GNUC__ // need to declare members used from base template
    using DB_iterator<DataObj, ParamObj>::boundIOs;
    using DB_iterator<DataObj, ParamObj>::bpa;
    using DB_iterator<DataObj, ParamObj>::count;
    using DB_iterator<DataObj, ParamObj>::io_handler;
    using DB_iterator<DataObj, ParamObj>::lastCount;
    using DB_iterator<DataObj, ParamObj>::pDBview;
    using DB_iterator<DataObj, ParamObj>::pParambuf;
    using DB_iterator<DataObj, ParamObj>::pRowbuf;
    using DB_iterator<DataObj, ParamObj>::sqlQryType;
    using DB_iterator<DataObj, ParamObj>::stmt;

    using DB_iterator_with_cols<DataObj, ParamObj>::MakeColBindings;
#endif
public:
	// is this a dummy iterator representing view.begin() or view.end()???
    enum DummyFlag { NOT_DUMMY = 0, DUMMY = 1 };

    // action specifying how to initialize dummy values for view.begin() and view.end()
    enum DummyAction { DUMMY_BEGIN = 0, DUMMY_END = 1 };

	enum BulkCopyPosition { BULK_DEFAULT_POSITION = -1, BULK_END_POSITION = -2 };


	// are we at the beginning or the end of the recordset?
	bool at_beginning() const { return begin; }
	bool at_end() const	      { return end || !(this->stmt.valid());    }

private:
	bool begin; // are we at the beginning of the recordset?
	bool end;   // are we at the end of the recordset?
	bool empty; // is recordset empty?

	DummyFlag isDummy; // was this iterator set as a dummy for view.begin() and view.end()?

	SelValWrap<DataObj> SelValidate;


	// called from catch blocks to fetch rows until errors are handled
	// returns the action that should be taken by the caller
	dtl_ios_base::MeansOfRecovery FetchNextRowUntilHandled(RootException &ex)
	{
		  dtl_ios_base::MeansOfRecovery error_action = io_handler(ex, *this, *(this->pRowbuf.get()), *this->pParambuf);

		  // if we wish to reperform the operation, keep trying to fetch rows
		  // until successful or otherwise directed by the handler
		  while (error_action == dtl_ios_base::SUPPRESS_ERROR && !this->bad())
		  {
			// think of this as a 3rd condition &&'ed with the other two
			// we want to break out of the loop if at the end of the view
			// must perform it here otherwise operator!=() might
			// throw with an uncaught exception
			if (*this == this->pDBview->end())
				break;

	        try
			{
				FetchNextRow();
			}
			catch (RootException &ex1)
			{
				error_action = io_handler(ex1, *this, *(this->pRowbuf.get()), *this->pParambuf);

				// if the handler tells us to throw on a retry,
				// then we will rethrow the original exception???
				if (error_action == dtl_ios_base::THROW_EXCEPTION)
					throw;
			}

			// need to break out of loop if iterator read element successfully
			if (this->good())
				break;
		  }

		  // the only action we need to take here is to throw
		  // if the handler said to
		  return error_action;
	}

	// version of FetchFirstRow() that will grab the first row of the recordset
	// but with proper error handling using IOHandlers
    void FetchFirstRowUntilHandled()
	{
	    // try to fetch first row ... if fail, apply the handler and
		// keep trying rows until succeed (if applicable)

		// try to fetch first row
		// if failed, then try to read more rows if the handler says to
		try
		{
			if (this->bad())
				DTL_THROW DBException(_TEXT("DBView::select_iterator::FetchFirstRowUntilHandled()"),
					_TEXT("iterator tested bad!"), NULL, NULL);

			FetchFirstRow();
		}
		catch (RootException &ex)
		{
		  // fetch next rows until the exception (and any others triggered are cleared)
 		  dtl_ios_base::MeansOfRecovery handled = FetchNextRowUntilHandled(ex);

		  // rethrow original exception if it wasn't handled above
		  if (handled == dtl_ios_base::THROW_EXCEPTION)
			  throw;
		}
	}

	// fetch first row of recordset
	// exception-safety questionable
	void FetchFirstRow()
	{
		if (this->bad())
		{
			end = true; // set iterator to end of view

		    // error handler call in caller will determine how to handle the exception
			DTL_THROW DBException(_TEXT("DBView::select_iterator::FetchFirstRow()"),
				_TEXT("iterator tested bad!"), NULL, NULL);
		}

		// propagate postfix parameters to their strbufs
		// before we execute!

		try
		{
          // Not needed? this->boundIOs.InsUserValidate();
			this->boundIOs.PropagateToSQL(this->sqlQryType, this->stmt);
		}
		catch (...)
		{
			if (this->stmt.valid())
				this->setstate(this->failbit);
			else
			{
				end = true;
				this->setstate(this->badbit);
			}

			throw;
		}

		try
		{
			this->stmt.Execute();
			this->boundIOs.PropagateToSQLAfterExec(this->sqlQryType, this->stmt);
		}
		catch (...)
		{
			this->setstate(this->badbit);
			end = true;
			throw;
		}

		begin = true;
		empty = false;
		this->lastCount = 0;

		DBStmt::FetchStatus fetch_rc = this->stmt.Fetch();  // should fetch into rowbuf

		if (fetch_rc == DBStmt::NO_DATA_FETCHED)
		{
			// at end of recordset
			end = true;
			empty = true;
			this->clear();
		}
		else if (fetch_rc == DBStmt::ROW_FETCHED)
		{
			// first record fetched

			// must propagate results back
			// to the bound objects

			// iterate through BoundIOs and make any necessary propagations
			try
			{
				this->boundIOs.PropagateFromResults(this->stmt);
			}
			catch (...)
			{
			    if (this->stmt.valid())
				   this->setstate(this->failbit);
			    else
				{
				   end = true;
				   this->setstate(this->badbit);
				}

				throw;
			}

			this->count++;
			this->lastCount = 1;
			this->clear();

			// if user specified a SelVal, apply it
            if (!SelValidate(this->boundIOs, *this->pRowbuf))
			{
				if (this->stmt.valid())
					this->setstate(this->failbit);
				else
				{
					end = true;
					this->setstate(this->badbit);
				}

			 DTL_THROW DBException(_TEXT("DBView::select_iterator::FetchFirstRow()"),
						  _TEXT("SelValidate() failed on statement \"") +
						  this->stmt.GetQuery() + _TEXT("\"!"), NULL, NULL);
			}
		}
		else // stmt.Fetch() failed -- shouldn't get here as exception is thrown
			 // in DBStmt::Fetch()
		{
			end = true; // set iterator to view.end() to break infinite loops

			if (this->stmt.valid())
				this->setstate(this->failbit);
			else
				this->setstate(this->badbit);


			DTL_THROW DBException(_TEXT("DBView::select_iterator::FetchFirstRow()"),
					_TEXT("Fetch failed on statement \"") + this->stmt.GetQuery() +
					_TEXT("\"!"),
					&(this->stmt.GetConnection()), &(this->stmt));
		}
	}

	// initialize the iterator
	// exception-safety depends on FetchFirstRow() and DB_iterator::open()
	virtual void open()
	{
		try
		{
			DB_select_insert_iterator<DataObj, ParamObj>::open();

		    // try to fetch first row ... if fail, apply the handler and
		    // keep trying rows until succeed (if applicable)
            FetchFirstRowUntilHandled();

		    isDummy = NOT_DUMMY;
		}

		catch (...)
		{
			this->setstate(this->badbit);
			end = true;
			throw;
		}

	}


	// is recordset empty?
	bool is_empty() const     { return empty; }



	// fetches next row from database
	// exception-safety questionable
	void FetchNextRow()
	{
		if (this->bad())
		{
			end = true; // set iterator to end of view
			DTL_THROW DBException(_TEXT("DBView::select_iterator::FetchNextRow()"),
				_TEXT("iterator tested bad!"), NULL, NULL);
		}

		if (!this->IsReady())
		{
	        this->open(); // calls FetchFirstRow() internally
			return;
		}


		if (!at_end())  // don't try to advance the iterator if we know we're at the end of
						// the recordset
		{
		   // If a copy has been made of our rowbuf, we need to rebind so
		   // as not to trashed shared copy
		   //
		   if (this->pRowbuf.GetRefCount() > 1) // if shared RowBuf release & rebind
		   {
			    // we must be careful in the case FetchNextRow() is called from an
			    // error handler as otherwise SQL functions get called improperly
			    if (this->good())
				{
					this->stmt.ReInitialize();
					this->ReBindRowbuf();
					FetchFirstRow();
					return;
				}
		   }

		   // propagate STL strings to their strbufs for proper binding
          this->boundIOs.InsUserValidate();
		   this->boundIOs.PropagateToSQL(this->sqlQryType, this->stmt);

		   this->lastCount = 0;  // no record retrieved

		   DBStmt::FetchStatus fetch_rc = this->stmt.Fetch();  // should fill rowbuf

		   // update begin, end flags
		   if (fetch_rc == DBStmt::NO_DATA_FETCHED) // end of recordset
		   {
	          begin = false;  // no longer pointing to beginning of recordset
			  end = true;	  // now pointing at end
		   }
		   else if (fetch_rc == DBStmt::ROW_FETCHED) // row fetched
		   {
			  // propagate results back to bound STL strings
			  this->boundIOs.PropagateFromResults(this->stmt);

			  this->count++;
			  this->lastCount = 1;  // one record retrieved

			  begin = false;

			  // if user specified a SelVal, apply it
              if (!SelValidate(this->boundIOs, *this->pRowbuf))
			  {
				if (this->stmt.valid())
					this->setstate(this->failbit);
				else
				{
					end = true;
					this->setstate(this->badbit);
				}

			    DTL_THROW DBException(_TEXT("DBView::select_iterator::operator++()"),
							  _TEXT("SelValidate() failed on statement \"") +
							  this->stmt.GetQuery() + _TEXT("\"!"), NULL, NULL);
			  }
		   }
		   else // stmt.Fetch() failed - shouldn't happen
				// as exception should be thrown in DBStmt::Fetch()
		   {
			 end = true; // set to view.end() to break infinite loops
			 DTL_THROW DBException(_TEXT("DBView::select_iterator::FetchNextRow()"),
							  _TEXT("Fetch failed on statement \"") + this->stmt.GetQuery() +
							  _TEXT("\"!"), &(this->stmt.GetConnection()), &(this->stmt));
		   }
		}

		this->clear();
	}


	static bool EqualCompare(const DB_select_iterator<DataObj, ParamObj> &i1,
		const DB_select_iterator<DataObj, ParamObj> &i2)
	{
		bool same_view = (i1.pDBview == i2.pDBview);
		bool at_same_place = ( (i1.at_beginning() && i2.at_beginning()) ||
				(i1.at_end() && i2.at_end()) );

		if (same_view && at_same_place)
			return true;

		// need to bypass our logical constness to open() iterators if necessary
		DB_select_iterator<DataObj, ParamObj> &j1 =
			const_cast<DB_select_iterator<DataObj, ParamObj> &>(i1);

		DB_select_iterator<DataObj, ParamObj> &j2 =
			const_cast<DB_select_iterator<DataObj, ParamObj> &>(i2);

		// must catch exceptions here or comparisons will crash
		try
		{
		  if (i1.GetCount() < 1 && !(i1.isDummy == DUMMY && i1.at_end()))
		     j1.open();
		}
		catch (RootException &ex)
		{
			dtl_ios_base::MeansOfRecovery error_action =
				j1.io_handler(ex, j1, *(j1.pRowbuf.get()), *(j1.pParambuf));

			if (error_action == dtl_ios_base::THROW_EXCEPTION)
				throw;
		}

		try
		{
		  if (i2.GetCount() < 1 && !(i2.isDummy == DUMMY && i2.at_end()))
		     j2.open();
		}

		catch (RootException &ex)
		{
			dtl_ios_base::MeansOfRecovery error_action =
				j2.io_handler(ex, j2, *(j2.pRowbuf.get()), *(j2.pParambuf));

			if (error_action == dtl_ios_base::THROW_EXCEPTION)
				throw;
		}

		// test again for begin() == end() after open
		at_same_place = ( (i1.at_beginning() && i2.at_beginning()) ||
				(i1.at_end() && i2.at_end()) );

		bool same_ref = (i1.pRowbuf.get() == i2.pRowbuf.get());

		// if we are testing against end(), we need to add a special case if both
		// rowbuf pointers are NULL
		// otherwise, we'll terminate out of an iterator's loop improperly
		if (!i1.pRowbuf && !i2.pRowbuf)
		{
			if (!at_same_place)
				same_ref = false;
		}

		return (same_view && (at_same_place || same_ref));
	}


	// common code used by operator*() and operator->() to get pRowbuf properly
	// uses IOHandlers for proper error handling
	void OperatorArrowImp() {

		// handle errors that occur in try block using our IOHandler
		try
		{
			if (this->bad())
			{
			  end = true;

			  DTL_THROW DBException(_TEXT("DBView::select_iterator::OperatorArrow()"),
				_TEXT("iterator tested bad!"), NULL, NULL);
			}
			this->GetRowbufPtr(); // create rowbuf & fetch if not already done

		}

	    catch (RootException &ex)
		{


		  // fetch next rows until the exception (and any others triggered are cleared)

		  // fetch next rows until the exception (and any others triggered are cleared)
		  dtl_ios_base::MeansOfRecovery handled = FetchNextRowUntilHandled(ex);
		  // rethrow original exception if it wasn't handled above
		  if (handled == dtl_ios_base::THROW_EXCEPTION)
			  throw;
		}
	}

	CountedPtr<DataObj> OperatorArrow() const
	{
		DB_select_iterator<DataObj, ParamObj> *this_ptr =
			const_cast<DB_select_iterator<DataObj, ParamObj> *>(this);

		this_ptr->OperatorArrowImp();

		return this->pRowbuf; // in the face of an exception:
						// we can get a bad object if we try to use pData as the
						// assignment doesn't complete
						// here we are guaranteed a default constructed DataObj
	}


public:
	DB_select_iterator() : DB_select_insert_iterator<DataObj, ParamObj>(),
		begin(false), end(false),
		empty(false), isDummy(NOT_DUMMY), SelValidate(DefaultSelValidate<DataObj>()) { }

    DB_select_iterator(const DBView<DataObj, ParamObj> &view, bool bPrepare = false) :
           DB_select_insert_iterator<DataObj, ParamObj>(view, SELECT, bPrepare),
	   begin(false), end(false), empty(false), isDummy(NOT_DUMMY), SelValidate(view.GetSelVal())
       { }

	// copy constructor and assignment operator required for Assignable property
	DB_select_iterator(const DB_select_iterator &read_it) :
	   DB_select_insert_iterator<DataObj, ParamObj>(read_it),
	   begin(read_it.begin), end(read_it.end), empty(read_it.empty), isDummy(read_it.isDummy),
	   SelValidate(read_it.SelValidate)
       { }

	// exception-safe swap()
	void swap(DB_select_iterator<DataObj, ParamObj> &other)
	{
		DB_select_insert_iterator<DataObj, ParamObj>::swap(other);
		STD_::swap(begin, other.begin);
		STD_::swap(end, other.end);
		STD_::swap(empty, other.empty);
		STD_::swap(isDummy, other.isDummy);
		STD_::swap(SelValidate, other.SelValidate);
	}

	// exception-safe assignment
	DB_select_iterator<DataObj, ParamObj> &
		operator=(const DB_select_iterator<DataObj, ParamObj> &other)
	{
		if (this != &other)
		{
			// Special performance case here for assignment of begin() to open iterator
			if (this->pDBview == other.pDBview && this->IsReady() && other.at_beginning()) {
				Reset();
			}
			else {
				DB_select_iterator<DataObj, ParamObj> temp(other);
				swap(temp);
			}

		}
		return *this;
	}


	// Reset iterator to point to beginning of recordset.
	void Reset()
	{
		DB_select_insert_iterator<DataObj, ParamObj>::Reset(); // no-throw
		begin = true;
		end = false;
		empty = false;
	}

	// forms of constructor that only should be used for begin and end iterators

	// dummy form that uses another select iterator to build this one
	DB_select_iterator(const DB_select_iterator &read_it, DummyAction dumAction) :
	   DB_select_insert_iterator<DataObj, ParamObj>(read_it),
	   begin(dumAction == DUMMY_BEGIN),
	   end(dumAction == DUMMY_END), empty(false),
	   isDummy(DUMMY),
	   SelValidate(read_it.SelValidate)
       { }

	// dummy form that uses a view to build this iterator
	DB_select_iterator(const DBView<DataObj, ParamObj> &view, DummyAction dumAction) :
       DB_select_insert_iterator<DataObj, ParamObj>(view, SELECT, false),
	   begin(dumAction == DUMMY_BEGIN),
       end(dumAction == DUMMY_END), empty(false), isDummy(DUMMY),
       SelValidate(view.GetSelVal())
	   { }



	// return current record
	// DB_iterator::GetRowbufPtr() provides the necessary support to maintain
	// the following invariants as specified in the C++ standard for input iterators
	// exception-safety dependent on GetRowbufPtr(), SelValidate(),
	// and DataObj copy constructor
	const DataObj &operator*() const
	{
		return *OperatorArrow();
	}

	// overriding operator->() behavior to provide SelValidate() checks
	// exception-safety depends on GetRowbufPtr() and SelValidate()
	CountedPtr<DataObj> operator->() const
	{
		return OperatorArrow();
	}

	// advance to next record (preincrement)
	DB_select_iterator<DataObj, ParamObj> &operator++()
	{

      try
	  {
		if (!this->IsReady())
		{
			this->open(); // open the iterator if not done already
		}
		FetchNextRow();
	  }
	  catch (RootException &ex)
	  {
		  // fetch next rows until the exception (and any others triggered are cleared)
		  dtl_ios_base::MeansOfRecovery handled = FetchNextRowUntilHandled(ex);

		  // rethrow original exception if it wasn't handled above
		  if (handled == dtl_ios_base::THROW_EXCEPTION)
			  throw;
	  }

      return *this;
	}

// #ifndef __BORLANDC__
#if __hpux
  // non-proxy solution (disabled)... originally all compilers avoided
  // proxies except Borland, but now we'll use proxies for all compilers

  // advance to next record (postincrement)
  // standard allows us to return (void) rather than copy of iterator!
  // we really want to do this as users may foolishly use iterator++ thereby
  // creating invisible copies and killing performance
  inline void operator++(int)
  {
	++(*this);
  }

#else
  // proxy solution ... now used for all compilers, not just BCC

  // BCC 5.5 Rogue Wave library is non-standard compliant in its use of input
  // iterators.
  // Wants to use old value despite what ISO standard says this punishes all
  // those who use their library - recommend you either switch to
  // STLPort like BC 6 has done, or use your own copy algorithm, or change the
  // RW library, e.g.

#  if 0
	// For the example project:
	// algorith.cc line 422 should be changed to:
	// -----
	//
	template <class InputIterator, class OutputIterator>
	OutputIterator copy (InputIterator first, InputIterator last,
                       OutputIterator result)
	  {
		while (first != last) {
			*result = *first;
			++result; ++first;
		}

	// For the tests project:
	// line 62 changed to
	 template <class InputIterator, class Function>
	  Function for_each (InputIterator first, InputIterator last, Function f)
	  {
		while (first != last) {
			f(*first);
			++first;
		}
		return f;
	  }

	 // etc.
#  endif

	 // In the meantime we provide an ugly workaround which doesn't give true
	 // postincrement semantics, but rather returns a "pointer" to the old
	 // value so it can still be retrieved
	 // after the increment has taken place. This isn't the best thing in the
	 // world, but is about as efficient as we can do in the situation.

	 // This workaround makes the situation better for select_iterator.
	 // For insert_iterator and any of the output iterators the situation
	 // remains bad with Rogue wave as these will be forced to make a copy of
	 // the iterator on every step of the loop.

	template <class DataObjType> class DataObjPtr {
		private:
			DataObjType d;
		public:
			DataObjPtr (const DataObjType &dob)  : d(dob) {}
			const DataObjType &operator*() const {return d;}

	};

	inline DataObjPtr<DataObj> operator++(int)
	{
	   DataObjPtr<DataObj> d(**this);
       ++(*this);
	   return d;
	}




#endif



	// are the iterators pointing to the same place
	// only applies at begin and end of
	// recordset
	bool operator==(const DB_select_iterator<DataObj, ParamObj> &i2) const
	{
		return EqualCompare(*this, i2);
	}

	// are the iterators pointing to different records?
	// only meaningful if both iterators are at the start or end of the same recordset
	bool operator!=(const DB_select_iterator<DataObj, ParamObj> &i2) const
	{
		return !(*this == i2);
	}


	virtual ~DB_select_iterator<DataObj, ParamObj>()
	{
		try
		{
		  if (this->IsReady())
			this->stmt.CloseCursor();
		}
		catch (...)
		{

		}
	}


	SelValWrap<DataObj> &GetSelValidate() {return SelValidate;}

	friend class DB_sql_iterator<DataObj, ParamObj>;


#ifdef DTL_NO_BULK_OPERATIONS
	// Solaris doesn't support bulk fetch properly, so we have to use regular copy here
	void bulk_copy(DataObj *pbegin, DataObj *pend, bool bValidate = true,
				   size_t row_size = sizeof(DataObj), SQLINTEGER position = BULK_DEFAULT_POSITION)
	{

		    BYTE *pdata_begin = reinterpret_cast<BYTE *>(pbegin);
		    BYTE *pdata_end = reinterpret_cast<BYTE *>(pend);
			int n = 0;
			for (; *this != this->GetView().end()  && pdata_begin < pdata_end; ++*this, pdata_begin += row_size, ++n) {
				pbegin = reinterpret_cast<DataObj *>(pdata_begin);
				*pbegin = **this;
			}

			lastCount = n;
	}

#else

	// copy the contents from the DBView pointed to by this iterator to the output range
	// using SQLBulkOperations()
	void bulk_copy(DataObj *pbegin, DataObj *pend, bool bValidate = true,
				   size_t row_size = sizeof(DataObj), SQLINTEGER position = BULK_DEFAULT_POSITION)
	{
#ifndef ORACLE_11g_BUG
        typedef SQLUSMALLINT STATUS;  // This is what the size should be
#else
		typedef SQLINTEGER STATUS; // Temporary workaround for Oracle 11g ODBC bug.
#endif
		STATUS *pRowStatusArray = NULL;
		SQLLEN *pBytesFetchedArray = NULL;
		TIMESTAMP_STRUCT *pDatesFetchedArray = NULL;

		BYTE *pdata_begin = reinterpret_cast<BYTE *>(pbegin);
		BYTE *pdata_end = reinterpret_cast<BYTE *>(pend);

		// start with no failures recorded in vector
		io_handler.ClearFailures();

		if (pRowbuf.get() != pbegin) {
			if (!stmt.IsReady()) {
				// CountedPtr designed to accept only raw pointers from the heap
				// but we need to set the pRowbuf to the beginning of our output range

				pRowbuf = pbegin;

				pRowbuf.DontDelete(); // forces change to CountedPtr ... not to delete when
									  // reference count reaches 0, otherwise we'll get a crash!

				if (!(row_size >= sizeof(SQLLEN) * boundIOs.NumColumns()))
				{
					this->setstate(this->badbit);
					tostringstream errStream;
					errStream << _TEXT("Error.  For bulk_copy to work we require that row_size for ") << DTL_TYPEID_NAME (*pbegin)
						<< _TEXT(") > ") << sizeof(SQLLEN) * boundIOs.NumColumns() <<
						_TEXT(" (this is sizeof(SQLLEN) * # of bound columns).  To use bulk_copy you will ")
						_TEXT("need to pad your row object with extra bytes at the end to bring it up to this minimum size.");
					DTL_THROW DBException(_TEXT("DBView::select_iterator::bulk_copy()"), errStream.str(), NULL, NULL);
				}


				SQLINTEGER bufferRows;
				bufferRows = (pdata_end - pdata_begin) / row_size;

				if (bufferRows < 1)
					DTL_THROW DBException(_TEXT("DBView::select_iterator::bulk_copy()"),
						_TEXT("Error. Bulk fetch must be supplied with a buffer that can hold at least 1 row of data."),
						NULL, NULL);

				// Cheat! Make bytes fetched array same size as DataObj
				// Why? Now arithmetic for both data and length indicators is done by
				// address = BoundAddress + (Row Number - 1) * RowSize
				boundIOs.pBytesFetchedArray.reset(bufferRows * row_size);
				pBytesFetchedArray = (SQLLEN *)boundIOs.pBytesFetchedArray.get() ;
				int NumDates = boundIOs.NumJtimes();
				if (NumDates > 0) {
					if (!(row_size >= sizeof(TIMESTAMP_STRUCT) * NumDates))
					{
						this->setstate(this->badbit);
						tostringstream errStream;
						errStream << _TEXT("Error.  For bulk_copy to work we require that row_size for ") << DTL_TYPEID_NAME (*pbegin)
						<< _TEXT(") > ") << sizeof(TIMESTAMP_STRUCT) * NumDates <<
						_TEXT("  (this is sizeof(TIMESTAMP_STRUCT) * # of bound columns).  To use bulk_copy you will ")
						_TEXT("need to pad your row object with extra bytes at the end to bring it up to this minimum size.");
						DTL_THROW DBException(_TEXT("DBView::select_iterator::bulk_copy()"), errStream.str(), NULL, NULL);
					}
					boundIOs.pDatesFetchedArray.reset(bufferRows * row_size);
					pDatesFetchedArray = (TIMESTAMP_STRUCT *)boundIOs.pDatesFetchedArray.get() ;
				}

				// set BoundIOs to point to pBytesFetchedArray
				for (BoundIOs::iterator b_it = boundIOs.begin(); b_it != boundIOs.end(); ++b_it)
				{
					BoundIO &boundIO = b_it->second;
					if (boundIO.IsColumn())
						boundIO.SetBytesFetchedPtr(&pBytesFetchedArray[boundIO.GetColNo()]);
				}



				// For SQL Server, default is to use server side cursors s.t. more than

				// one iterator can be open at a time.  For bulk fetches we don't want this

				if (stmt.GetConnection().GetDBMSEnum() == DBConnection::DB_SQL_SERVER) {

					stmt.ClearStmtAttr(SQL_ATTR_CONCURRENCY);

				}




				// stmt.ClearStmtAttrs();

				// SetStmtAttr calls for row wise binding
				stmt.SetStmtAttr(SQL_ATTR_ROW_ARRAY_SIZE, (SQLPOINTER) bufferRows, 0);
				stmt.SetStmtAttr(SQL_ATTR_ROW_BIND_TYPE, (SQLPOINTER) row_size, 0);

				if (boundIOs.HasStrings()  || position != BULK_DEFAULT_POSITION) // if position != -1 require scrollable cursor
				{
				   // We require a STATIC type cursor for random access iterators
				   if (position != BULK_DEFAULT_POSITION)
				   {
					 stmt.SetStmtAttr(SQL_ATTR_CURSOR_TYPE, (SQLPOINTER)SQL_CURSOR_STATIC, 0);
				   }
				   else
				   {
					   // Need to call SQLSetPos to retrieve strings via GetData
					   if (stmt.GetConnection().GetDBMSEnum() == DBConnection::DB_SQL_SERVER)
					   {
						 // SQL Server requires the use of keyset driven cursors for bulk operations
						 stmt.SetStmtAttr(SQL_ATTR_CURSOR_TYPE, (SQLPOINTER)SQL_CURSOR_KEYSET_DRIVEN, 0);
					   }
					   else
					   {
						 stmt.SetStmtAttr(SQL_ATTR_CURSOR_TYPE, (SQLPOINTER)SQL_CURSOR_STATIC, 0);
					   }
				   }
				}
				else
				{
					// No need for get data, use the faster Forward only cursor
					stmt.SetStmtAttr(SQL_ATTR_CURSOR_TYPE, SQL_CURSOR_FORWARD_ONLY, 0);
				}


				//STATUS RowStatusArray[bufferRows];
				//SQLUINTEGER NumRowsFetched = 0;

				boundIOs.pRowStatusArray.reset(bufferRows * sizeof(STATUS));
				boundIOs.pNumRowsFetched = new SQLULEN(0);

				pRowStatusArray = (STATUS*)boundIOs.pRowStatusArray.get();

				// SetStmtAttr calls for row wise binding status flags
				stmt.SetStmtAttr(SQL_ATTR_ROW_STATUS_PTR, pRowStatusArray, 0);
				stmt.SetStmtAttr(SQL_ATTR_ROWS_FETCHED_PTR, boundIOs.pNumRowsFetched.get(), 0);

				// Make bindings using pBytesFetchedArray
				// Make bindings must come after SetStmtAttr calls for attributes to be set
				SetWorkingRowAddr((DataObj *)(NULL), boundIOs, (BYTE *)(pRowbuf.get()), pDatesFetchedArray);

				this->count = 0;
				this->lastCount = 0;

				try
				{
					DB_select_insert_iterator<DataObj, ParamObj>::open();

					isDummy = NOT_DUMMY;

					// propagate STL parameter strings to their strbufs for proper binding
               boundIOs.InsUserValidate();
					boundIOs.PropagateToSQL(sqlQryType, stmt);

					stmt.Execute();

					// need to call PutData for string parameters!
					this->boundIOs.PropagateToSQLAfterExec(this->sqlQryType, this->stmt);
				}
				catch (...)
				{
					this->setstate(this->badbit);
					throw;
				}

			}
			else {
				const SQLINTEGER bufferRows = boundIOs.pBytesFetchedArray.size() / row_size;

				// Assume we have same number of rows, just different section of memory for data
				// We require this since the buffer row size is set at statement execute time
				if (static_cast<size_t>(bufferRows) != ((pdata_end - pdata_begin) / row_size)) {
					this->setstate(this->badbit);
					DTL_THROW DBException(_TEXT("DBView::select_iterator::bulk_copy()"), _TEXT("Error. After the ")
						_TEXT("first call to bulk_copy, all subsequent calls must allocate the same number of ")
						_TEXT("available rows for the bulk fetch."), NULL, NULL);

				}

				// Rebind column addresses
				pRowbuf = pbegin;
				pRowbuf.DontDelete(); // forces change to CountedPtr ... not to delete when
									  // reference count reaches 0, otherwise we'll get a crash!

				// Make bindings using previously allocated pBytesFetchedArray
				pDatesFetchedArray = (TIMESTAMP_STRUCT *)boundIOs.pDatesFetchedArray.get() ;
				SetWorkingRowAddr((DataObj *)(NULL), boundIOs, (BYTE *)(pRowbuf.get()), pDatesFetchedArray);
				MakeColBindings();
				pRowStatusArray = (STATUS*)boundIOs.pRowStatusArray.get();
				pBytesFetchedArray = (SQLLEN *)boundIOs.pBytesFetchedArray.get() ;

				// need to call PutData for string parameters??????
				// this->boundIOs.PropagateToSQLAfterExec(this->sqlQryType, this->stmt);

			}
		}
		else
		{
			pRowStatusArray = (STATUS*)boundIOs.pRowStatusArray.get();
			pBytesFetchedArray = (SQLLEN *)boundIOs.pBytesFetchedArray.get() ;
			pDatesFetchedArray = (TIMESTAMP_STRUCT *)boundIOs.pDatesFetchedArray.get() ;

		}


		DBStmt::FetchStatus status = DBStmt::NO_DATA_FETCHED;

		if (position == BULK_DEFAULT_POSITION)
			// fetch next set of records
			status = stmt.FetchScroll(SQL_FETCH_NEXT, 0);
		else if (position == BULK_END_POSITION)
		{
			// fetch last set of records

			// To support MySQL do not use SQL_FETCH_LAST, instead use negative offset
			if (stmt.GetConnection().GetDBMSEnum() == DBConnection::DB_MYSQL)
				status = stmt.FetchScroll(SQL_FETCH_ABSOLUTE, -static_cast<SQLINTEGER>((pdata_end - pdata_begin) / row_size));
			else
				status = stmt.FetchScroll(SQL_FETCH_LAST, 0);

			if (status == DBStmt::NO_DATA_FETCHED)
			{
				this->count = 0;  // N.B.  NO_DATA_FETCHED status passed to next part of routine
			}
			else
			{

				// Set count of number of records to total up to beginning of rowset buffer
				// When we add pNumRowsFetched to this we should get total number of rows in recordset
				SQLULEN endrow=0;
				SQLINTEGER end_bytes;
				try {
					stmt.GetStmtAttr(SQL_ATTR_ROW_NUMBER, &endrow, SQL_IS_UINTEGER, &end_bytes);
				}
				catch (STD_::exception) {
					endrow = 0;
				}

			    if (static_cast<SQLINTEGER>(endrow) == -1)  // Treat BOF condition (empty table)
                {
                    this->count = 0;
                    end = true;
                    lastCount = 0;
                    return;
				}

				if (endrow == 0) // function could not get base row count
				{
					// if driver does not tell us how many rows, assume this
					// is of LONG_MAX size and simply throw an exception later
					// if we try to read past the end of the rows (since we
					// won't know how many rows we can read until we try).
					endrow = LONG_MAX - *(boundIOs.pNumRowsFetched);
	#if 0
						this->setstate(this->badbit);
						tostringstream errStream;
						errStream << _TEXT("Error.  SQLGetStmtAttr(SQL_ATTR_ROW_NUMBER) returned failure. ") <<
							 _TEXT("This means that the driver cannot tell us how many rows are in this snapshot so we can't represent it as a RandomDBView.");
						DTL_THROW DBException(_TEXT("DBView::select_iterator::bulk_copy()"), errStream.str(), NULL, NULL);
	#endif
				}
				this->count = endrow;
			}
		}
		else
			// perform a positioned fetch
			status = stmt.FetchScroll(SQL_FETCH_ABSOLUTE, position);


		if (status == DBStmt::NO_DATA_FETCHED) {
				end = true;
				lastCount = 0;
				return;
		}

		begin = false;
		this->count+=*(boundIOs.pNumRowsFetched);
		this->lastCount = *(boundIOs.pNumRowsFetched);

		// backup first bytes fetched row
		// inside both hasstrings loop and pdatesfetchedarray loop,
		// put in the memcpy() logic to set the bytes fetched
		// just before exit before selval, restore the values of the first row

		// Call SQLGetData as needed to fetch strings
		//
		BYTE *pBytesFetchedRow = NULL;
      BYTE *pBytesFetchedBackup = new BYTE[row_size];


		if (boundIOs.HasStrings()) {
			TIMESTAMP_STRUCT *pDatesFetchedRow;
			for (size_t i = 0; i < *(boundIOs.pNumRowsFetched); ++i)
			{
				if (pRowStatusArray[i] != SQL_ROW_ERROR) {
					 // propagate results back to bound STL strings
					pDatesFetchedRow = (TIMESTAMP_STRUCT *) (((BYTE *)pDatesFetchedArray) + i * row_size);

					// populate values of BytesFetched pointers in BoundIOs with result codes from row status and pBytesFetchedArray
					// note that we don't change the address of the pointers in the boundIOs here just
					// copy row contents as needed - this is faster and avoids counted pointer thrashing
					// - plus if we later need to fetch another row the pointers are already set up
					// - when we call MakeBindings() in the rebind column adresses case
					if (i > 0) {
						pBytesFetchedRow = (BYTE *) (((BYTE *)pBytesFetchedArray) + i * row_size);
						memcpy(pBytesFetchedArray, pBytesFetchedRow, row_size);
					}

					SetComplexRowAddr((DataObj *)(NULL), boundIOs, pdata_begin + i * row_size, pDatesFetchedRow);

					try {
						stmt.SetPos(i+1, SQL_POSITION, SQL_LOCK_NO_CHANGE);
					}
					catch (RootException &)
					{
						this->setstate(this->badbit);
						DTL_THROW DBException(_TEXT("DBView::select_iterator::bulk_copy()"),
								_TEXT("SQLSetPos failed. Driver may not support binding of type std::string with bulk fetches. ")
								_TEXT("Try using character arrays for the required fields instead."), NULL, NULL);
					}

					try
					{
						boundIOs.PropagateFromResults(stmt);
						if (i > 0 && bValidate) {
							// Have to copy any string lengths from SQLGetData() back to the status array
							memcpy(pBytesFetchedRow, pBytesFetchedArray, row_size);
						}
						else if (i == 0) {
							// backup first bytes fetched row
							memcpy(pBytesFetchedBackup, pBytesFetchedArray, row_size);
						}

					}
					catch (...)
					{
						this->setstate(this->badbit);
						throw;
					}
				}
			}
		}
		else if (pDatesFetchedArray != NULL) {
			// copy dates only
			TIMESTAMP_STRUCT *pDatesFetchedRow;
			for (size_t i = 0; i < *(boundIOs.pNumRowsFetched); ++i)
			{
				if (pRowStatusArray[i] != SQL_ROW_ERROR) {
					 // propagate results back to bound STL strings
					pDatesFetchedRow = (TIMESTAMP_STRUCT *) (((BYTE *)pDatesFetchedArray) + i * row_size);

					// populate values of BytesFetched pointers in BoundIOs with result codes from row status and pBytesFetchedArray
					// note that we don't change the address of the pointers in the boundIOs here just
					// copy row contents as needed - this is faster and avoids counted pointer thrashing
					// - plus if we later need to fetch another row the pointers are already set up
					// - when we call MakeBindings() in the rebind column adresses case
					if (i > 0) {
						pBytesFetchedRow = (BYTE *) (((BYTE *)pBytesFetchedArray) + i * row_size);
						memcpy(pBytesFetchedArray, pBytesFetchedRow, row_size);
					}

					SetComplexRowAddr((DataObj *)(NULL), boundIOs, pdata_begin + i * row_size, pDatesFetchedRow);
					try
					{
						boundIOs.PropagateFromResults(stmt);
						if (i > 0 && bValidate) {
							// Have to copy any string lengths from SQLGetData() back to the status array
							memcpy(pBytesFetchedRow, pBytesFetchedArray, row_size);
						}
						else if (i == 0) {
							// backup first bytes fetched row
							memcpy(pBytesFetchedBackup, pBytesFetchedArray, row_size);
						}
					}
					catch (...)
					{
						this->setstate(this->badbit);
						throw;
					}
				}
			}


		}

		if (boundIOs.HasStrings() || pDatesFetchedArray != NULL)
			memcpy(pBytesFetchedArray, pBytesFetchedBackup, row_size);

		delete[] pBytesFetchedBackup;

		// Exit if SelValidate calls are not to be made
		if (!bValidate)
			return;

		for (size_t i = 0; i < *(boundIOs.pNumRowsFetched); ++i)
		{
			// try to fetch first row
			// if failed, then try to read more rows if the handler says to
			try
			{
				if (pRowStatusArray[i] == SQL_ROW_ERROR)
				{
					tostringstream errStream;
					errStream << _TEXT("Fetch error in row #") << i;

					if (stmt.valid())
						this->setstate(this->failbit);
					else
						this->setstate(this->badbit);

					DTL_THROW DBException(_TEXT("DBView::select_iterator::bulk_copy()"), errStream.str(), NULL, NULL);
				}

				boundIOs.SelUserValidateAddr(pdata_begin + i * row_size);

				// populate values of BytesFetched pointers in BoundIOs with result codes from row status and pBytesFetchedArray
				// note that we don't change the address of the pointers in the boundIOs here just
				// copy row contents as needed - this is faster and avoids counted pointer thrashing
				// - plus if we later need to fetch another row the pointers are already set up
				// - when we call MakeBindings() in the rebind column adresses case
				if (i > 0) {
					pBytesFetchedRow = (BYTE *) (((BYTE *)pBytesFetchedArray) + i * row_size);
					memcpy(pBytesFetchedArray, pBytesFetchedRow, row_size);
				}


				// call SelValidate on row begin[i]
				if (!SelValidate(boundIOs,
					   *(reinterpret_cast<DataObj *>(pdata_begin + i * row_size))))
				{
					// handle error due to failed validation
					if (stmt.valid())
						this->setstate(this->failbit);
					else
						this->setstate(this->badbit);

					tostringstream errStream;
					errStream << _TEXT("SelValidate failed for row #") << i;
					DTL_THROW DBException(_TEXT("DBView::select_iterator::bulk_copy()"), errStream.str(), NULL, NULL);

				}

			}
			catch (RootException &ex)
			{
				dtl_ios_base::MeansOfRecovery error_action;
				error_action = io_handler(ex, *this,
					*(reinterpret_cast<DataObj *>(pdata_begin + i * row_size)),
					*pParambuf);

				// if we wish to reperform the operation, keep trying to fetch rows
				// until successful or otherwise directed by the handler
				if (error_action == dtl_ios_base::SUPPRESS_ERROR && !dtl_ios_base::bad())
				{

					io_handler.RecordFailure(i); // record index of failed row

					continue;
				}
				else
					throw;
			}



		}

		return;
	}
#endif
};


////////////////////////////////
// DBView begin() and end()
// iterators for begin and end of view
template<class DataObj, class ParamObj> DB_select_iterator<DataObj, ParamObj> DBView<DataObj, ParamObj>::begin() const
	{
		return DB_select_iterator<DataObj, ParamObj>(*this, DB_select_iterator<DataObj, ParamObj>::DUMMY_BEGIN);
	}

	// cache value of end() for speed
template<class DataObj, class ParamObj>	const DB_select_iterator<DataObj, ParamObj> & DBView<DataObj, ParamObj>::end() const {
	if (pEnd == NULL)
			pEnd = new DB_select_iterator<DataObj, ParamObj>(*this, DB_select_iterator<DataObj, ParamObj>::DUMMY_END);
		return *pEnd;
	}

template<class DataObj, class ParamObj>	DB_select_iterator<DataObj, ParamObj> DBView<DataObj, ParamObj>::begin()
	{
		return DB_select_iterator<DataObj, ParamObj>(*this, DB_select_iterator<DataObj, ParamObj>::DUMMY_BEGIN);
	}

// DBView begin() and end() definition complete
////////////////////////////////


END_DTL_NAMESPACE

#endif
