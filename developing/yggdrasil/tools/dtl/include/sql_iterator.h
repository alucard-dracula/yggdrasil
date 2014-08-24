// -*- C++ -*-
// iterator used for SELECTS
// nested class of DBView
// Initial: 9/8/2000 - MG
/* Copyright � 2000
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

#ifndef DTL_SQL_ITERATOR_H
#define DTL_SQL_ITERATOR_H

#include "select_insert_iterator.h"

BEGIN_DTL_NAMESPACE

// iterator used for SELECTS
template<class DataObj, class ParamObj = DefaultParamObj<DataObj> >
class DB_sql_iterator :
	public DB_select_insert_iterator<DataObj, ParamObj>,
#ifndef __SGI_STL_PORT
	public STD_::iterator<STD_::input_iterator_tag, DataObj, std::ptrdiff_t>
#else
	 // not sure why STLport not recognizing the standard iterator tag
	 // when used in algorithms like copy(_InputIter __first, _InputIter __last, _OutputIter __result)
	 // use STLport specific alternative
	public STD_::input_iterator<DataObj, std::ptrdiff_t>
#endif
{
#ifdef __GNUC__ // need to declare members used from base template
    using DB_iterator<DataObj, ParamObj>::bca;
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
    using DB_iterator<DataObj, ParamObj>::ReBindRowbuf;

    using DB_iterator_with_cols<DataObj, ParamObj>::MakeColBindings;
#endif

public:
	// is this a dummy iterator representing view.begin() or view.end()???
    enum DummyFlag { NOT_DUMMY = 0, DUMMY = 1 };

    // action specifying how to initialize dummy values for view.begin() and view.end()
    enum DummyAction { DUMMY_BEGIN = 0, DUMMY_END = 1 };

private:
	typedef typename DBView<DataObj, ParamObj>::select_iterator select_iterator;

	bool begin; // are we at the beginning of the recordset?
	bool end;    // are we at the end of the recordset?
	bool empty; // is recordset empty?

	DummyFlag isDummy; // was this iterator set as a dummy for view.begin() and view.end()?

	SelValWrap<DataObj> SelValidate;
	InsValWrap<DataObj> InsValidate;
	bool validRowbuf; // was previous object valid?

	// called from catch blocks to fetch rows until errors are handled
	// returns the action that should be taken by the caller
	dtl_ios_base::MeansOfRecovery FetchNextRowUntilHandled(RootException &ex)
	{
		  dtl_ios_base::MeansOfRecovery error_action;

		  error_action = io_handler(ex, *this, *(this->pRowbuf.get()), *this->pParambuf);

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
	    if (this->boundIOs.NumColumns() < 1)  // No bound columns, do not fetch
		{
			// No columns to return, trivially at end of recordset
			end = true;
			empty = true;
			return;
		}

		// try to fetch first row ... if fail, apply the handler and
		// keep trying rows until succeed (if applicable)

		// try to fetch first row
		// if failed, then try to read more rows if the handler says to
		try
		{
			if (this->bad())
				DTL_THROW DBException(_TEXT("DBView::sql_iterator::FetchFirstRowUntilHandled()"),
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
		if (this->boundIOs.NumColumns() < 1)  // No bound columns, do not fetch
			return;

		if (this->bad())
		{
			end = true; // set iterator to end of view

		    // error handler call in caller will determine how to handle the exception
			DTL_THROW DBException(_TEXT("DBView::sql_iterator::FetchFirstRow()"),
				_TEXT("iterator tested bad!"), NULL, NULL);
		}

		// do not fetch next if at end
		if (end == true)
			return;

		if (!this->IsReady())
		{
			// do not execute if this is a call after MoreResults()

			// propagate postfix parameters to their strbufs
			// before we execute!
			try
			{
           this->boundIOs.InsUserValidate();
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

				// SQL server bug workaround.
				// See knowledge base article 124899  http://support.microsoft.com/default.aspx?scid=kb;en-us;124899
				// SQLNumResultCols, SQLDescribeCol may not work right unless SQLExecute is called first.
				// This bug seems to only apply for batch statements.
				// In general we don't want to do this since it is inefficient.
				// Rebuild the row binding if this is a batch
				if(stmt.GetConnection().GetDBMSEnum() == DBConnection::DB_SQL_SERVER)
				{
					if (stmt.GetQuery().find(_TEXT(";")) != tstring::npos) // test for ; to identify a batch
						RebuildBCA();
				}

			}
			catch (...)
			{
				this->setstate(this->badbit);
				end = true;
				throw;
			}
		}

		begin = true;
		empty = false;
		this->lastCount = 0; // no records fetched yet

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
			end = false;

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

			 DTL_THROW DBException(_TEXT("DBView::sql_iterator::FetchFirstRow()"),
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


			DTL_THROW DBException(_TEXT("DBView::sql_iterator::FetchFirstRow()"),
					_TEXT("Fetch failed on statement \"") + this->stmt.GetQuery() +
					_TEXT("\"!"),
					&(this->stmt.GetConnection()), &(this->stmt));
		}
	}

	// write the current row out to the database
	// exception-safety questionable
	void WriteCurrentRow()
	{
		try
		{
			this->lastCount = 0; // no records retrieved yet

			if (!this->IsReady())
			 DB_select_insert_iterator<DataObj, ParamObj>::open();

			if (this->bad())
			{
			DTL_THROW DBException(_TEXT("DBView::sql_iterator::WriteCurrentRow()"),
				_TEXT("iterator tested bad!"), NULL, NULL);
			}

			// if user specified a InsVal, apply it
			if (!InsValidate(this->boundIOs, *this->pRowbuf))
			{
			validRowbuf = false;

			if (this->stmt.valid())
					this->setstate(this->failbit);
			else
					this->setstate(this->badbit);

			DTL_THROW DBException(_TEXT("DBView::sql_iterator::operator=(const DataObj &)"),
							  _TEXT("InsValidate() failed on statement \"") +
							  this->stmt.GetQuery() + _TEXT("\"!"), NULL, NULL);
			}

			validRowbuf = true;

			// propagate STL strings to their strbufs for proper binding
			this->boundIOs.PropagateToSQL(this->sqlQryType, this->stmt);

			RETCODE rc = this->stmt.Execute();
 			if (rc == SQL_NEED_DATA)
			  this->boundIOs.PropagateToSQLAfterExec(this->sqlQryType, this->stmt);

			begin = true;
			end = false;


		    this->boundIOs.PropagateFromResults(this->stmt);

			// if user specified a SelVal, apply it
			if (!end && !SelValidate(this->boundIOs, *this->pRowbuf) && this->boundIOs.HasOutput())
			 {
				validRowbuf = false;

				if (this->stmt.valid())
						this->setstate(this->failbit);
				else
						this->setstate(this->badbit);

				DTL_THROW DBException(_TEXT("DBView::sql_iterator::operator=(const DataObj &)"),
								  _TEXT("SelValidate() failed on statement \"") +
								  this->stmt.GetQuery() + _TEXT("\"!"), NULL, NULL);
			 }

		}
		catch (RootException &ex)
		{
			if (this->stmt.valid())
				this->setstate(this->failbit);
			else
				this->setstate(this->badbit);

			dtl_ios_base::MeansOfRecovery error_action =
				io_handler(ex, *this, *(this->pRowbuf.get()), *this->pParambuf);

			// what to do for REPERFORM_OPERATION?
			switch (error_action)
			{
			  case dtl_ios_base::THROW_EXCEPTION: throw;
			  case dtl_ios_base::SUPPRESS_ERROR: return;
			}
		}

		this->count++;
		this->lastCount = 1;

		this->clear();
		validRowbuf = false;
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

	// are we at the beginning or the end of the recordset?
	bool at_beginning() const { return begin; }
	bool at_end() const	      { return end || !(this->stmt.valid());    }

	// is recordset empty?
	bool is_empty() const     { return empty; }



	// fetches next row from database
	// exception-safety questionable
	void FetchNextRow()
	{
		this->lastCount = 0; // no records retrieved yet

		if (this->boundIOs.NumColumns() < 1)  // No bound columns, do not fetch
			return;

		if (this->bad())
		{
			end = true; // set iterator to end of view
			DTL_THROW DBException(_TEXT("DBView::sql_iterator::FetchFirstRow()"),
				_TEXT("iterator tested bad!"), NULL, NULL);
		}

		if (!this->IsReady())
		{
	        this->open();  // internally calls FetchFirstRow()
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
			  this->lastCount = 1;

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

			    DTL_THROW DBException(_TEXT("DBView::sql_iterator::FetchNextRow()"),
						  _TEXT("SelValidate() failed on statement \"") +
						  this->stmt.GetQuery() + _TEXT("\"!"), NULL, NULL);
			  }
		   }
		   else // stmt.Fetch() failed - shouldn't happen
				// as exception should be thrown in DBStmt::Fetch()
		   {
			 end = true; // set to view.end() to break infinite loops
			 DTL_THROW DBException(_TEXT("DBView::sql_iterator::FetchNextRow()"),
							  _TEXT("Fetch failed on statement \"") + this->stmt.GetQuery()
							  +_TEXT("\"!"), &(this->stmt.GetConnection()), &(this->stmt));
		   }
		}

		this->clear();
	}


	static bool EqualCompare(const DB_sql_iterator<DataObj, ParamObj> &i1,
		const DB_sql_iterator<DataObj, ParamObj> &i2)
	{

		bool same_view = (*(i1.pDBview) == *(i2.pDBview));
		bool at_same_place = ( (i1.at_beginning() && i2.at_beginning()) ||
				(i1.at_end() && i2.at_end()) );

		if (same_view && at_same_place)
			return true;

		// need to bypass our logical constness to open() iterators if necessary
		DB_sql_iterator<DataObj, ParamObj> &j1 =
			const_cast<DB_sql_iterator<DataObj, ParamObj> &>(i1);

		DB_sql_iterator<DataObj, ParamObj> &j2 =
			const_cast<DB_sql_iterator<DataObj, ParamObj> &>(i2);

		// must catch exceptions here or comparisons will crash
		try
		{
		  // N.B. must use i1.empty() here to handle MoreResults()
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
	CountedPtr<DataObj> OperatorArrow() const
	{
		// variables used in try/catch
		CountedPtr<DataObj> pData = NULL;

		DB_sql_iterator<DataObj, ParamObj> *this_ptr =
			const_cast<DB_sql_iterator<DataObj, ParamObj> *>(this);


		// handle errors that occur in try block using our IOHandler
		try
		{
			if (this->bad())
			{
			  this_ptr->end = true;
			  DTL_THROW DBException(_TEXT("DBView::sql_iterator::OperatorArrow()"),
				_TEXT("iterator tested bad!"), NULL, NULL);
			}
			this_ptr->GetRowbufPtr(); // create rowbuf & fetch if not already done

		}
		catch (RootException &ex)
		{
		  // fetch next rows until the exception (and any others triggered are cleared)
		  dtl_ios_base::MeansOfRecovery handled = this_ptr->FetchNextRowUntilHandled(ex);

		  // rethrow original exception if it wasn't handled above
		  if (handled == dtl_ios_base::THROW_EXCEPTION)
			  throw;
		}

		return this->pRowbuf; // in the face of an exception:
						// we can get a bad object if we try to use pData as the
						// assignment doesn't complete
						// here we are guaranteed a default constructed DataObj
	}


public:
	DB_sql_iterator() : DB_select_insert_iterator<DataObj, ParamObj>(),
		begin(false), end(false),
		empty(false), isDummy(NOT_DUMMY), SelValidate(DefaultSelValidate<DataObj>()),
		InsValidate(DefaultInsValidate<DataObj>()),
		validRowbuf(false) { }

    DB_sql_iterator(const DBView<DataObj, ParamObj> &view, bool bPrepare = true) :
       DB_select_insert_iterator<DataObj, ParamObj>(view, SQL, bPrepare),
		   begin(false), end(false), empty(false), isDummy(NOT_DUMMY),
		   SelValidate(view.GetSelVal()),
		   InsValidate(view.GetInsVal()),
		   validRowbuf(false)
    {
	   // For SQL Server, default is to use server side cursors s.t. more than
	   // one iterator can be open at a time.  Unfortunately, for sql_iterators
	   // we need to use client side cursors since many stored procedures will not
	   // run otherwise
	   if (stmt.GetConnection().GetDBMSEnum() == DBConnection::DB_SQL_SERVER) {
			stmt.ClearStmtAttr(SQL_ATTR_CONCURRENCY);
	   }

	}

	// copy constructor and assignment operator required for Assignable property
	DB_sql_iterator(const DB_sql_iterator &read_it) :
	   DB_select_insert_iterator<DataObj, ParamObj>(read_it), begin(read_it.begin), end(read_it.end), empty(read_it.empty), isDummy(read_it.isDummy),
	   SelValidate(read_it.SelValidate), InsValidate(read_it.InsValidate),
	   validRowbuf(false)
       { }

	// copy constructor and assignment operator required for Assignable property
	DB_sql_iterator(const select_iterator &read_it) :
	   DB_select_insert_iterator<DataObj, ParamObj>(read_it.GetView(), SQL,
		  read_it.stmt.GetPrepare()),
	   begin(read_it.begin), end(read_it.end), empty(read_it.empty), SelValidate(read_it.SelValidate), InsValidate(read_it.GetView().GetInsVal())

       {
			int e = read_it.isDummy;
			isDummy = (DummyFlag) e;
			// For SQL Server, default is to use server side cursors s.t. more than
			// one iterator can be open at a time.  Unfortunately, for sql_iterators
			// we need to use client side cursors since many stored procedures will not
			// run otherwise
			if (stmt.GetConnection().GetDBMSEnum() == DBConnection::DB_SQL_SERVER) {
				stmt.ClearStmtAttr(SQL_ATTR_CONCURRENCY);
			}
	   }


	// exception-safe swap()
	void swap(DB_sql_iterator<DataObj, ParamObj> &other)
	{
		DB_select_insert_iterator<DataObj, ParamObj>::swap(other);
		STD_::swap(begin, other.begin);
		STD_::swap(end, other.end);
		STD_::swap(empty, other.empty);
		STD_::swap(isDummy, other.isDummy);
		STD_::swap(SelValidate, other.SelValidate);
		STD_::swap(InsValidate, other.InsValidate);
		STD_::swap(validRowbuf, other.validRowbuf);
	}

	// exception-safe assignment
	DB_sql_iterator<DataObj, ParamObj> &
		operator=(const DB_sql_iterator<DataObj, ParamObj> &other)
	{

		if (this != &other)
		{
			DB_sql_iterator<DataObj, ParamObj> temp(other);

			// Special performance case here for assignment of begin() to open iterator
			if (this->pDBview == other.pDBview && this->IsReady() && other.at_beginning()) {
				Reset();
			}
			else {
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
	DB_sql_iterator(const DB_sql_iterator &read_it, DummyAction dumAction) :
	   DB_select_insert_iterator<DataObj, ParamObj>(read_it),
	   begin(dumAction == DUMMY_BEGIN),
	   end(dumAction == DUMMY_END), empty(false),
	   isDummy(DUMMY), SelValidate(read_it.SelValidate)
       { }

	DB_sql_iterator(const select_iterator &read_it, DummyAction dumAction) :
	   DB_select_insert_iterator<DataObj, ParamObj>(read_it.GetView(), SQL),
	   begin(dumAction == DUMMY_BEGIN),
	   end(dumAction == DUMMY_END), empty(false),
	   isDummy(DUMMY), SelValidate(read_it.SelValidate)
       { }

	// dummy form that uses a view to build this iterator
	DB_sql_iterator(const DBView<DataObj, ParamObj> &view, DummyAction dumAction) :
       DB_select_insert_iterator<DataObj, ParamObj>(view, SQL),
	   begin(dumAction == DUMMY_BEGIN),
	   end(dumAction == DUMMY_END), empty(false), isDummy(DUMMY), SelValidate(view.GetSelVal())
	   { }

	// overriding operator->() behavior to provide SelValidate() checks
	// exception-safety depends on GetRowbufPtr() and SelValidate()
	CountedPtr<DataObj> operator->() const
	{
		return OperatorArrow();
	}

	// advance to next record (preincrement)
	DB_sql_iterator<DataObj, ParamObj> &operator++()
	{
	  // Need to call MakeBindings() before we know how many columns we have
	  if (!this->IsReady())
	        this->open();

	  if (this->boundIOs.NumColumns() > 0) { // treat as select
		  try
		  {
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
	  }
	  else { // parameters only, same as insert case
		  // WriteCurrentRow(); // disabled ... moved to commit on operator=() for output case
	  }

      return *this;
	}

	// proxy operator=()
	DB_sql_iterator<DataObj, ParamObj> &operator=(const DataObj &dataObj)
	{
		try
		{
			if (this->bad())
			{
			   DTL_THROW DBException(_TEXT("DBView::sql_iterator::operator=(const DataObj &)"),
				  _TEXT("iterator tested bad!"), NULL, NULL);
			}

			// prepare to execute statement if this has not already been done
			DB_select_insert_iterator<DataObj, ParamObj>::open();
			isDummy = NOT_DUMMY;

			// NOTE! DO NOT call GetRowbufPtr as this may execute a fetch before the
			// DataObj has been assigned and the DataObj might hold parameters
			CountedPtr<DataObj> pData = pRowbuf;

			// assign to the data row
			dtl_assign(dataObj, *pData);

			int nCols = this->boundIOs.NumColumns();
			if (nCols < 1)
			{
				// This is a pure output iterator, just execute the statement
				WriteCurrentRow();
			}
			else
			{
				// This iterator has bound columns.  Force an execute, then fetch the first row of results.
				// This means that if an iterator has input parameters and a result set then
				// every time we invoke the assignemet operation this will force another
				// execution of the iterator as specified in the docs.

				if (static_cast<size_t>(nCols) == this->boundIOs.size()) // no parameters, pure select type statement
					stmt.Reset(); // have to close cursor for selects
				FetchFirstRowUntilHandled();
			}

		}
		catch (RootException &ex)
		{

			dtl_ios_base::MeansOfRecovery error_action =
				io_handler(ex, *this, *(this->pRowbuf.get()), *this->pParambuf);

			// what to do for REPERFORM_OPERATION?
			switch (error_action)
			{
			  case dtl_ios_base::THROW_EXCEPTION: throw;
			  case dtl_ios_base::SUPPRESS_ERROR: return *this;
			}
		}

		return *this;
	}

// #ifndef __BORLANDC__
#if __hpux // non-proxy solution (disabled) for all compilers
	// advance to next record (postincrement)
    inline DB_sql_iterator<DataObj, ParamObj> operator++(int) //non-const return to allow output via old iterator
	{
	   DB_sql_iterator<DataObj, ParamObj> oldValue(*this);
       ++(*this);
	   return oldValue;
	}

#else  // proxy solution ... now used for all compilers, not just BCC

	// BCC 5.5 Rogue Wave library is non-standard compliant in its use of input iterators.
	// Wants to use old value despite what ISO standard says
	// this punishes all those who use their library - recommend you either switch to
	// STLPort like BC 6 has done, or use your own copy algorithm, or change the RW library, e.g.

#if 0
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
#endif


	// In the meantime we provide an ugly workaround which doesn't give true postincrement
	// semantics, but rather returns a "pointer" to the old value so it can still be retrieved
	// after the increment has taken place. This isn't the best thing in the world, but is about as
	// efficient as we can do in the situation.

	// This workaround makes the situation better for select_iterator.
	// For insert_iterator and any of the output iterators the situation remains
	// bad with Rougue wave as these will be forced to make a copy of the iterator on
	// every step of the loop.

	template <class DataObjType> class DataObjPtr {
		private:
			DataObjType d;
		public:
			DataObjPtr (const DataObjType &dob)  :  d(dob) {}

			const DataObjType &operator*() const {return d;}

	};

	inline DataObjPtr<DataObj> operator++(int)
	{
	   DataObjPtr<DataObj> d(**this);
       ++(*this);
	   return d;
	}


#endif


public:

	DB_sql_iterator<DataObj, ParamObj> &operator*()
	{
		return *this;
	}

	// notice this operator is non-const
	operator DataObj()
	{
		return *this->GetRowbufPtr();
	}

	// are the iterators pointing to the same place
	// only applies at begin and end of
	// recordset
	bool operator==(const DB_sql_iterator<DataObj, ParamObj> &i2) const
	{
		return EqualCompare(*this, i2);
	}

	// are the iterators pointing to different records?
	// only meaningful if both iterators are at the start or end of the same recordset
	bool operator!=(const DB_sql_iterator<DataObj, ParamObj> &i2) const
	{
		return !(*this == i2);
	}


	virtual ~DB_sql_iterator()
	{
		try
		{
		  if (this->IsReady() && this->boundIOs.NumColumns() != 0)
			this->stmt.CloseCursor();
		}
		catch (...)
		{

		}
	}


	// Get the next result set, if any
	bool MoreResults() {
		bool bMore;
		this->count = 0;
		this->lastCount = 0;
		begin = true;
		if (!this->stmt.IsExecuted()) {
			this->stmt.Execute();
		}
		bMore = this->stmt.MoreResults();
		if (bMore) {
			end = false;
			empty = false;

			// After call to MoreResults() columns may have changed
			RebuildBCA();
		}
		else {
			end = true;
			empty = true;
		}



		return bMore;
	}


	void RebuildBCA()
	{
			// Rebuild BCA and bindings
			HSTMT hstmt = GetHSTMT(stmt);
			bca = pDBview->RefreshBCA(&hstmt);
			pRowbuf = NULL;
			pParambuf = NULL;
			ReBindRowbuf();
	}
};

// needed in case select_iterator given first in comparison
template<class DataObj, class ParamObj>
   bool operator==(const DB_select_iterator<DataObj, ParamObj> &sel_it, const DB_sql_iterator<DataObj, ParamObj> &sql_it)
{
	return (sql_it == sel_it);
}

template<class DataObj, class ParamObj>
   bool operator!=(const DB_select_iterator<DataObj, ParamObj> &sel_it, const DB_sql_iterator<DataObj, ParamObj> &sql_it)
{
	return !(sel_it == sql_it);
}

END_DTL_NAMESPACE

#endif
