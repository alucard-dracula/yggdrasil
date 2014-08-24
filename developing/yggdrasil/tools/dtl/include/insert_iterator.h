// -*- C++ -*-
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
// iterator used for INSERTS
// nested class of DBView
// Initial: 9/8/2000 - MG
// Edited: 12/19/2000 - MG - added namespaces

#ifndef DTL_INSERT_ITERATOR_H
#define DTL_INSERT_ITERATOR_H

#include "select_insert_iterator.h"
#include "RootException.h"

BEGIN_DTL_NAMESPACE

// iterator used for INSERTS
template<class DataObj, class ParamObj = DefaultParamObj<DataObj> >
  class DB_insert_iterator :
    public DB_select_insert_iterator<DataObj, ParamObj>,
#ifndef __SGI_STL_PORT
		public STD_::iterator<STD_::output_iterator_tag, DataObj, std::ptrdiff_t>
#else
	 // not sure why STLport not recognizing the standard iterator tag
	 // when used in algorithms like copy(_InputIter __first, _InputIter __last, _OutputIter __result)
	 // use STLport specific alternative
	public STD_::forward_iterator<DataObj, std::ptrdiff_t>
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

#endif

private:

	InsValWrap<DataObj> InsValidate;
	bool validRowbuf; // was previous object valid?

	// write the current row out to the database
	// exception-safety questionable
	void WriteCurrentRow()
	{
		try
		{
		  if (!this->IsReady())
		  {
	    	 this->open();
		  }

		  // must call InsValidate() here to fix bug with NULL's
		  // BoundIO columns get erased in this instance with open()
		  if (!InsValidate(this->boundIOs, *(this->pRowbuf)))
		  {
			DTL_THROW DBException(_TEXT("DBView::insert_iterator::WriteCurrentRow()"),
				_TEXT("InsValidate() failed on statement ") + this->stmt.GetQuery(), NULL, NULL);
		  }

		  validRowbuf = true;

		  this->lastCount = 0;

		  if (this->bad())
		  {
			DTL_THROW DBException(_TEXT("DBView::insert_iterator::WriteCurrentRow()"),
				_TEXT("iterator tested bad!"), NULL, NULL);
		  }

		  // propagate STL strings to their strbufs for proper binding
        this->boundIOs.InsUserValidate();
		  this->boundIOs.PropagateToSQL(this->sqlQryType, this->stmt);
		  RETCODE rc = this->stmt.Execute();
 		  if (rc == SQL_NEED_DATA)
			  this->boundIOs.PropagateToSQLAfterExec(this->sqlQryType, this->stmt);
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

public:
    DB_insert_iterator() : DB_select_insert_iterator<DataObj, ParamObj>(),
		InsValidate(DefaultInsValidate<DataObj>()), validRowbuf(false)
	{ }

	DB_insert_iterator(const DBView<DataObj, ParamObj> &view, bool bPrepare = true) :
		DB_select_insert_iterator<DataObj, ParamObj>(view, INSERT, bPrepare),
			InsValidate(view.GetInsVal()),
			validRowbuf(false)
            { }


	// copy constructor and assignment operator required for Assignable property
	DB_insert_iterator(const DB_insert_iterator<DataObj, ParamObj> &write_it) :
		   DB_select_insert_iterator<DataObj, ParamObj>(write_it),
		   InsValidate(write_it.InsValidate), validRowbuf(write_it.validRowbuf)
            { }


	// exception-safe swap()
	void swap(DB_insert_iterator<DataObj, ParamObj> &other)
	{
		DB_select_insert_iterator<DataObj, ParamObj>::swap(other);
		STD_::swap(InsValidate, other.InsValidate);
		STD_::swap(validRowbuf, other.validRowbuf);
	}

	// exception-safe assignment
	DB_insert_iterator<DataObj, ParamObj> &
		operator=(const DB_insert_iterator<DataObj, ParamObj> &other)
	{
		if (this != &other)
		{
			DB_insert_iterator<DataObj, ParamObj> temp(other);

			// Special performance case here for assignment to open iterator against same DBView
			// Surprisingly, the code below is exception safe in the following sense
			// As long as the hstmt is valid we should always be able to close an open cursor on
			// it.  If the iterator is not valid, then we do not lose anything by marking it as
			// invalid & trying to rebuild it since we won't be able to perform any further operations
			// against the invalid iterator anyway.
			if (this->pDBview == other.pDBview  && this->IsReady()) {
				this->stmt.swap(temp.stmt); // move stmt to destination
				swap(temp); // setup iterator
				this->stmt.Reset(); // no-throw
			}
			else
			swap(temp);
		}

		return *this;
	}

	// return a proxy
	// exception-safe
	DB_insert_iterator<DataObj, ParamObj> &
		operator*()
	{
		return *this;
	}

	// proxy paradigm used to be able to perform InsValidate()
	// on the rowbuf held by the iterator

	// exception-safety guaranteed if GetRowbufPtr(), InsValidate(),
	// and DataObj::operator=() are exception-safe
	DB_insert_iterator<DataObj, ParamObj> &
		operator=(const DataObj &data)
	{
		try
		{
			if (this->bad())
			{
			   DTL_THROW DBException(_TEXT("DBView::insert_iterator::operator=(const DataObj &)"),
				_TEXT("iterator tested bad!"), NULL, NULL);
			}

			CountedPtr<DataObj> pData = this->GetRowbufPtr();


			//dtl_assign(data, *pData);
			dtl_assign_t<DataObj> dtl_assign;
			dtl_assign(data, *pData);


			// if user specified a InsVal, apply it
			if (!InsValidate(this->boundIOs, *pData))
			{
				validRowbuf = false;

				if (this->stmt.valid())
					this->setstate(this->failbit);
				else
					this->setstate(this->badbit);

				throw DBException(_TEXT("DBView::insert_iterator::operator=(const DataObj &)"),
							  _TEXT("InsValidate() failed on statement \"") +
							  this->stmt.GetQuery() + _TEXT("\"!"), NULL, NULL);
			}

			validRowbuf = true;
		}
		catch (RootException &ex)
		{
			if (this->stmt.valid())
				this->setstate(this->failbit);
			else
				this->setstate(this->badbit);


			dtl_ios_base::MeansOfRecovery error_action =
				io_handler(ex, *this, *(this->pRowbuf.get()), *this->pParambuf);

			// what to do for REPERFORM_ACTION?
			switch (error_action)
			{
			  case dtl_ios_base::THROW_EXCEPTION: throw;
			  case dtl_ios_base::SUPPRESS_ERROR: return *this;
			}
		}

		this->clear();

        // commit on operator=()
		this->WriteCurrentRow();

		return *this;
	}



	// _TEXT("advance to next record") (preincrement) ... involves writing out latest record
	// while conceptually advancing the iterator, we really aren't changing the state
	// of the iterator at all.

	// IO handler logic is inside of WriteCurrentRow()
	DB_insert_iterator<DataObj, ParamObj> &operator++()
	{
        return *this;
	}

    // _TEXT("advance to next record") (postincrement) ... involves writing out latest record
	// while conceptually advancing the iterator, we really aren't changing the state
	// of the iterator at all.
    inline DB_insert_iterator<DataObj, ParamObj> operator++(int) //non-const return to allow output via old iterator
	{
	  DB_insert_iterator<DataObj, ParamObj> oldValue(*this);
	  ++(*this);
	  return oldValue;
	}

	// commit inserts from this iterator and clean up
	virtual ~DB_insert_iterator()
	{ }

#ifdef DTL_NO_BULK_OPERATIONS
	// Solaris doesn't support bulk insert properly, so we have to use regular copy loop
	void bulk_copy(DataObj *pbegin, DataObj *pend, bool bValidate = true,
				   size_t row_size = sizeof(DataObj))
	{
		    BYTE *pdata_begin = reinterpret_cast<BYTE *>(pbegin);
		    BYTE *pdata_end = reinterpret_cast<BYTE *>(pend);
			int n = 0;
			for (; pdata_begin < pdata_end; ++*this, pdata_begin += row_size, ++n) {
				pbegin = reinterpret_cast<DataObj *>(pdata_begin);
				**this = *pbegin;
			}

			lastCount = n;
	}

#else
	// copy the contents from the DBView pointed to by this iterator to the output range
	// using SQLBulkOperations()
	void bulk_copy(const DataObj *pbegin, const DataObj *pend, bool bValidate = true,
				   size_t row_size = sizeof(DataObj))
	{

		this->lastCount = 0;

		// nothing to do if no records to bulk copy
		if (pbegin == pend)
			return;

		SQLUSMALLINT *pRowStatusArray;
		SQLLEN *pBytesFetchedArray;
		TIMESTAMP_STRUCT *pDatesFetchedArray = NULL;
		SQLULEN bufferRows;

		const BYTE *pdata_begin = reinterpret_cast<const BYTE *>(pbegin);
		const BYTE *pdata_end = reinterpret_cast<const BYTE *>(pend);

		if (this->pRowbuf.get() != pbegin) {
			if (!this->stmt.IsReady()) {
				// CountedPtr designed to accept only raw pointers from the heap
				// but we need to set the this->pRowbuf to the beginning of our output range

				this->pRowbuf = const_cast<DataObj *>(pbegin);

				this->pRowbuf.DontDelete(); // forces change to CountedPtr ... not to delete when
									  // reference count reaches 0, otherwise we'll get a crash!

				if (!(row_size >= sizeof(SQLLEN) * this->boundIOs.NumColumns()))
				{
					this->setstate(this->badbit);
					tostringstream errStream;
					errStream << _TEXT("Error.  For bulk_copy to work we require that row_size for ") << DTL_TYPEID_NAME(*pbegin)
						<< _TEXT(" > ") << sizeof(SQLLEN) * this->boundIOs.NumColumns() <<
						_TEXT(" (this is sizeof(SQLINTEGER) * # of bound columns).  To use bulk_copy you will ")
						_TEXT("need to pad your row object with extra bytes at the end to bring it up to this minimum size.");
					DTL_THROW DBException(_TEXT("DBView::insert_iterator::bulk_copy()"), errStream.str(), NULL, NULL);
				}


				bufferRows = (pdata_end - pdata_begin) / row_size;

				// Cheat! Make bytes fetched array same size as DataObj
				// Why? Now arithmetic for both data and length indicators is done by
				// address = BoundAddress + (Row Number - 1) * RowSize
				this->boundIOs.pBytesFetchedArray.reset(bufferRows * row_size);
				pBytesFetchedArray = (SQLLEN *)this->boundIOs.pBytesFetchedArray.get() ;
				int NumDates = this->boundIOs.NumJtimes();
				if (NumDates > 0) {
					if (!(row_size >= sizeof(TIMESTAMP_STRUCT) * NumDates))
					{
						this->setstate(this->badbit);
						tostringstream errStream;
						errStream << _TEXT("Error.  For bulk_copy to work we require that row_size for ") << DTL_TYPEID_NAME(*pbegin)
						<< _TEXT(" > ") << sizeof(TIMESTAMP_STRUCT) * NumDates <<
						_TEXT("  (this is sizeof(TIMESTAMP_STRUCT) * # of bound columns).  To use bulk_copy you will ")
						_TEXT("need to pad your row object with extra bytes at the end to bring it up to this minimum size.");
						DTL_THROW DBException(_TEXT("DBView::insert_iterator::bulk_copy()"), errStream.str(), NULL, NULL);
					}
					this->boundIOs.pDatesFetchedArray.reset(bufferRows * row_size);
					pDatesFetchedArray = (TIMESTAMP_STRUCT *)this->boundIOs.pDatesFetchedArray.get() ;
				}

				// set BoundIOs to point to pBytesFetchedArray
				for (BoundIOs::iterator b_it = this->boundIOs.begin(); b_it != this->boundIOs.end(); ++b_it)
				{
					BoundIO &boundIO = b_it->second;
					if (boundIO.IsColumn())
					{
						boundIO.SetBytesFetchedPtr(&pBytesFetchedArray[boundIO.GetColNo()]);
					}
				}

				this->stmt.ClearStmtAttrs();

				// SetStmtAttr calls for row wise binding
				this->stmt.SetStmtAttr(SQL_ATTR_PARAM_BIND_TYPE, (SQLPOINTER)row_size, 0);
				this->stmt.SetStmtAttr(SQL_ATTR_PARAMSET_SIZE, (SQLPOINTER)bufferRows, 0);



				//SQLUSMALLINT RowStatusArray[bufferRows];
				//SQLUINTEGER NumRowsFetched = 0;

				this->boundIOs.pRowStatusArray.reset(bufferRows * sizeof(SQLUSMALLINT));
				this->boundIOs.pNumRowsFetched = new SQLULEN(0);

				pRowStatusArray = (SQLUSMALLINT*)this->boundIOs.pRowStatusArray.get();

				// SetStmtAttr calls for row wise binding status flags
				this->stmt.SetStmtAttr(SQL_ATTR_PARAM_STATUS_PTR,  pRowStatusArray, 0);
				this->stmt.SetStmtAttr(SQL_ATTR_PARAMS_PROCESSED_PTR, (SQLPOINTER) this->boundIOs.pNumRowsFetched.get(), 0);

				// Make bindings using pBytesFetchedArray
				// Make bindings must come after SetStmtAttr calls for attributes to be set
				this->boundIOs.SetWorkingAddr((BYTE *)(this->pRowbuf.get()), pDatesFetchedArray);

				// Most likely bulk_copy will be called only once
				this->stmt.SetPrepare(false);

				this->stmt.Initialize();

				this->MakeBindings();

				this->count = 0;
				this->lastCount = 0;
			}
			else {
				bufferRows = this->boundIOs.pBytesFetchedArray.size() / row_size;

				// number of rows may not be the same as last call
				// reset attribute to new row size

				this->stmt.OverrideStmtAttr(SQL_ATTR_PARAMSET_SIZE,
						(SQLPOINTER) ((pdata_end - pdata_begin) / row_size), 0);


				// Rebind column addresses
				this->pRowbuf = const_cast<DataObj *>(pbegin);
				this->pRowbuf.DontDelete(); // forces change to CountedPtr ... not to delete when
									  // reference count reaches 0, otherwise we'll get a crash!

				// Make bindings using previously allocated pBytesFetchedArray
				pDatesFetchedArray = (TIMESTAMP_STRUCT *)this->boundIOs.pDatesFetchedArray.get() ;
				this->boundIOs.SetWorkingAddr((BYTE *)(this->pRowbuf.get()), pDatesFetchedArray);
				this->MakeBindings();
				pRowStatusArray = (SQLUSMALLINT*)this->boundIOs.pRowStatusArray.get();
				pBytesFetchedArray = (SQLLEN *)this->boundIOs.pBytesFetchedArray.get() ;


			}
		}
		else
		{
			bufferRows = this->boundIOs.pBytesFetchedArray.size() / row_size;

			// number of rows may not be the same as last call
			// reset attribute to new row size

			this->stmt.OverrideStmtAttr(SQL_ATTR_PARAMSET_SIZE,
					(SQLPOINTER) ((pdata_end - pdata_begin) / row_size), 0);

			pRowStatusArray = (SQLUSMALLINT*)this->boundIOs.pRowStatusArray.get();
			pBytesFetchedArray = (SQLLEN *)this->boundIOs.pBytesFetchedArray.get() ;
			pDatesFetchedArray = (TIMESTAMP_STRUCT *)this->boundIOs.pDatesFetchedArray.get() ;

		}

		SQLINTEGER numRows = (pdata_end - pdata_begin) / row_size;

		// Only validate if so requested
		if (bValidate) {
			BYTE *pBytesFetchedRow;
			for (int i = numRows-1; i >=0 ; --i)
			{
				// try to validate next row
				// if failed, then try to validate more rows if the handler says to
				try
				{

					// WORK IN REVERSE!! Copy null flags from BoundIOs to pBytesFetchedArray!!!!
					boundIOs.InsUserValidateAddr(const_cast<BYTE *>(pdata_begin) + i * row_size);

					// call InsValidate on row begin[i]
					if (!InsValidate(this->boundIOs,
						*(reinterpret_cast<DataObj *>(const_cast<BYTE *>(pdata_begin) + i * row_size))))
					{
						// handle error due to failed validation
						if (this->stmt.valid())
							this->setstate(this->failbit);
						else
							this->setstate(this->badbit);

						tostringstream errStream;
						errStream << _TEXT("InsValidate failed for row #") << i;
						DTL_THROW DBException(_TEXT("DBView::insert_iterator::bulk_copy()"), errStream.str(), NULL, NULL);

					}

				}
				catch (RootException &ex)
				{
					dtl_ios_base::MeansOfRecovery error_action;
					error_action = this->io_handler(ex, *this,
						*(reinterpret_cast<DataObj *>(const_cast<BYTE *>(pdata_begin) + i * row_size)),
						*(this->pParambuf));

					// if we wish to reperform the operation, keep trying to insert rows
					// until successful or otherwise directed by the handler
					if (error_action == dtl_ios_base::SUPPRESS_ERROR && !this->bad())
						continue;
					else
						throw;
				}

				// WORK IN REVERSE!! Copy null flags from BoundIOs to pBytesFetchedArray row!!!!

				// InsValidate populates values of BytesFetched pointers in BoundIOs with result codes
				// This sets NULL flags in row 0 as specified in MakeBindings
				// Simply copy these values to the correct row
				// Note that we require InsValidate to set & clear null flags for all fields in each row
				// Default construction of pBytesFetchedRow will clear all the null flags for the first row,
				// after that if InsValidate wants to set null flags, it will then need to make
				// sure to reset those flags when the next row is processed
				if (i > 0) {
					pBytesFetchedRow = (BYTE *) (((BYTE *)pBytesFetchedArray) + i * row_size);
					memcpy(pBytesFetchedRow, pBytesFetchedArray, row_size);
				}




			}
		}
		else // bValidate == false
		{
			// must copy the bytes fetched flags over to the other rows
			for (size_t i = 1; i < static_cast<size_t>(numRows); ++i)
			{
				BYTE *pBytesFetchedRow = (BYTE *) (((BYTE *)pBytesFetchedArray) + i * row_size);
				memcpy(pBytesFetchedRow, pBytesFetchedArray, row_size);
			}
		}

		try
		{
			// use this->boundIOs.PropagateToSQL to copy
			// jtime_c values to date array
			if (pDatesFetchedArray != NULL) {
				std_memset(pDatesFetchedArray, 0, numRows * row_size);
				// copy dates only
				TIMESTAMP_STRUCT *pDatesFetchedRow;
				for (int i = 0; i < numRows; ++i)
				{
						pDatesFetchedRow = (TIMESTAMP_STRUCT *) (((BYTE *)pDatesFetchedArray) + i * row_size);
						this->boundIOs.SetComplexAddr(const_cast<BYTE *>(pdata_begin) + i * row_size, pDatesFetchedRow);
                 this->boundIOs.InsUserValidate();
						this->boundIOs.PropagateToSQL(INSERT, this->stmt);
				}
			}

			// Write array of parameters (perform bulk insert)
			this->stmt.Execute();
			SQLULEN *pRow = this->boundIOs.pNumRowsFetched.get();

			// Call SQLPutData as needed to put strings
			//
			if (this->boundIOs.HasStrings()) {
				BYTE *pBytesFetchedRow;
				SQLPOINTER pToken;
				SQLULEN i(0), last_i(0);

				this->boundIOs.SetComplexAddr(const_cast<BYTE *>(pdata_begin) + i * row_size,
					pDatesFetchedArray);

				while(this->stmt.ParamData(&pToken) == SQL_NEED_DATA)
				{
				  // Check row #
				  i = *pRow - 1;

				  // ODBC docs are confusing here.  See "ODBC Programmers Reference, Chapter 9, "Executing Statements", "Using Arrays of Parameters"
				  // On web: http://msdn.microsoft.com/library/en-us/odbc/htm/odbcusing_arrays_of_parameters.asp
				  // The ambiguity is the last sentence of the doc:
				  // "For the data-at-execution parameters, the value that SQLParamData provides to the application, which normally could be used to "
				  // "identify the data that the driver is requesting on this occasion, is always the address of the array."
				  // Driver may return magic # / token that was bound.
				  // Or it may return some kind of pointer into array.
				  // Drivers seem to be returning the token.  For now, throw if they do not
				  if (pToken > (void *)50) {
					    this->setstate(this->badbit);
						DTL_THROW DBException(_TEXT("DBView::insert_iterator::bulk_copy()"),
							_TEXT("Driver is not returning column # token when calling SQLParamData.  Unable to send long data for bulk insert."),
							NULL, NULL);
				  }

				  if (i != last_i) {
				  		this->boundIOs.SetComplexAddr(const_cast<BYTE *>(pdata_begin) + i * row_size,
							pDatesFetchedArray);
						last_i = i;
				  }

				  // Populate values of BytesFetched pointers in BoundIOs with NULL flag values
				  // from InsValidate above.
				  // MoveWriteAfterExec requires this in order to properly insert NULL values
				  if (i > 0) {
						pBytesFetchedRow = (BYTE *) (((BYTE *)pBytesFetchedArray) + i * row_size);
						memcpy(pBytesFetchedArray, pBytesFetchedRow, row_size);
				  }


    			  this->boundIOs.GetBoundIOforColumn(pToken).MoveWriteAfterExec(this->sqlQryType, this->stmt);

				}
			}
			this->lastCount = *pRow;

			this->count += *pRow;

			//count += (lastCount = this->stmt.RowCount()); // is this how we update count????
		}
		catch (...)
		{
			this->setstate(this->badbit);
			throw;
		}
	}
#endif

};

END_DTL_NAMESPACE

#endif
