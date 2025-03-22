// -*- C++ -*-
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
// abstraction for DELETES
// nested class of DBView
// Initial: 9/8/2000 - MG
// Edited: 12/19/2000 - MG - added namespaces

#ifndef DTL_DELETE_ITERATOR_H
#define DTL_DELETE_ITERATOR_H

#include "DB_iterator.h"
#include "DBStmt.h"


BEGIN_DTL_NAMESPACE


// hacked from select_update_iterator.h:ExtractKey()
inline BoundIOs ExtractNotNulls(BoundIOs const& bs)
{
   // we must do a copy so the user-validations are also copied
   BoundIOs boundIOs(bs);
   boundIOs.clear();
   for (BoundIOs::const_iterator b = bs.begin(); b != bs.end(); ++b)
      // if (!(*b).second.IsNull())
         boundIOs[(*b).first] = (*b).second;
   return boundIOs;
}

template<class DataObj, class ParamObj = DefaultParamObj<DataObj> >
  class DB_delete_iterator :
     public DB_iterator_with_cols<DataObj, ParamObj>,
#ifndef __SGI_STL_PORT
	 public STD_::iterator<STD_::output_iterator_tag, DataObj, std::ptrdiff_t>
#else
	 // STLport not recognizing the standard iterator tag
	 // since in STLPort this relies on partial specialization
	 // use STLport specific alternative
	 public STD_::forward_iterator<DataObj, std::ptrdiff_t>
#endif
{
private:


	// call this function to execute the update
	// returns number of rows updated
	// to perform another update, change the necessary parameter values
	// and then make another call to ExecDelete() to perform that Delete

	// exception-safety questionable
   // TODO test this by making user-validates throw some exceptions.
	int ExecDelete()
	{
		  int rowsDeleted = 0;

		  try
		  {
			if (!this->IsReady())
				this->open();

		    if (this->bad())
			{
			  DTL_THROW DBException(_TEXT("DBView::delete_iterator::ExecDelete()"),
				 _TEXT("iterator tested bad!"), NULL, NULL);
			}

			// propagate STL strings to their strbufs for proper binding
		    this->boundIOs.PropagateToSQL(this->sqlQryType, this->stmt);  // if this throws, no delete is executed
			this->stmt.Execute();
			this->boundIOs.PropagateToSQLAfterExec(this->sqlQryType, this->stmt);
		  }
		  catch (RootException &ex)
		  {
			  if (this->stmt.valid())
				  this->setstate(this->failbit);
			  else
				  this->setstate(this->badbit);

			  dtl_ios_base::MeansOfRecovery error_action =
				  this->io_handler(ex, *this, *(this->pRowbuf.get()), *this->pParambuf);

			  // what do we do on a REPERFORM_ACTION???
			  switch (error_action)
			  {
			    case dtl_ios_base::THROW_EXCEPTION: throw;
				case dtl_ios_base::SUPPRESS_ERROR: return (this->lastCount = 0);
			  }

		  }

		  // trying to stay as consistent as we can be here
		  // but no way we can be if this line throws ... in matter of fact, we'll have
		  // a corrupted iterator, but at least it's contained to here
		  // only the row count info will be messed up
		  // (the delete will have already been performed, but not the update
		  // of the state information of the iterator)
		  try
		  {
			  rowsDeleted = this->stmt.RowCount();
		  }
		  catch (RootException &ex)
		  {
			  if (this->stmt.valid())
				  this->setstate(this->failbit);
			  else
				  this->setstate(this->badbit);

			  dtl_ios_base::MeansOfRecovery error_action =
				  this->io_handler(ex, *this, *(this->pRowbuf.get()), *this->pParambuf);

			  switch (error_action)
			  {
			    case dtl_ios_base::THROW_EXCEPTION: throw;
				case dtl_ios_base::SUPPRESS_ERROR: return (this->lastCount = 0);
			  }

		  }

		  // count member keeps track of *cumulative* number of elements deleted
		  // through this iterator in order to be consistent with the other
		  // DB iterator classes
		  this->count += rowsDeleted;
		  this->clear();
	      return (this->lastCount = rowsDeleted);
	}

public:
	DB_delete_iterator() : DB_iterator_with_cols<DataObj, ParamObj>() { }

    DB_delete_iterator(const DBView<DataObj, ParamObj> &view, bool bPrepare = true) :
		DB_iterator_with_cols<DataObj, ParamObj>(view, DEL, bPrepare)
		{ }

	// copy constructor and assignment operator required for Assignable
	DB_delete_iterator(const DB_delete_iterator<DataObj, ParamObj> &del_it) :
	   DB_iterator_with_cols<DataObj, ParamObj>(del_it)
	{ }

	// exception-safe swap()
	void swap(DB_delete_iterator<DataObj, ParamObj> &other)
	{
		DB_iterator_with_cols<DataObj, ParamObj>::swap(other);
	}

	// exception-safe assignment
	DB_delete_iterator<DataObj, ParamObj> &
		operator=(const DB_delete_iterator<DataObj, ParamObj> &other)
	{
		if (this != &other)
		{
			DB_delete_iterator<DataObj, ParamObj> temp(other);

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

	// commit deletes from this deleter and clean up
	virtual ~DB_delete_iterator()
	{ }


	// return a proxy
	// exception-safe
	DB_delete_iterator<DataObj, ParamObj> &
		operator*()
	{
		return *this;
	}

	// proxy paradigm used to be able to perform InsValidate()
	// on the rowbuf held by the iterator

	// exception-safe if DataObj::operator=() is exception-safe
	// and GetRowbufPtr() is also
	DB_delete_iterator<DataObj, ParamObj> &
		operator=(const DataObj &data)
	{
		CountedPtr<DataObj> pData = NULL;

		try
		{
		    if (this->bad())
			{
			   DTL_THROW DBException(_TEXT("DBView::delete_iterator::ExecDelete()"),
				  _TEXT("iterator tested bad!"), NULL, NULL);
			}

			pData = this->GetRowbufPtr();

		    //dtl_assign(data, *pData);
			dtl_assign_t<DataObj> dtl_assign;
			dtl_assign(data, *pData);
		}
		catch (RootException &ex)
		{
			if (this->stmt.valid())
				this->setstate(this->failbit);
			else
				this->setstate(this->badbit);

			dtl_ios_base::MeansOfRecovery error_action =
				this->io_handler(ex, *this, *(this->pRowbuf.get()), *this->pParambuf);

			// on the proxy assignment, what's the context of REPERFORM_OPERATION???
			switch (error_action)
			{
			   case dtl_ios_base::THROW_EXCEPTION: throw;
			   case dtl_ios_base::SUPPRESS_ERROR: break;
			}
		}

		this->clear();

        // commit on operator=()
	    this->ExecDelete(); // will open() the iterator if necessary

		return *this;
	}


	// execute the delete and preincrement
	// handler logic taken care of in ExecDelete()
	DB_delete_iterator<DataObj, ParamObj> &operator++()
	{
		return *this;
	}

	// execute the delete and postincrement
	inline DB_delete_iterator<DataObj, ParamObj> operator++(int) //non-const return to allow output via old iterator
	{
		DB_delete_iterator<DataObj, ParamObj> oldValue(*this);
		++(*this);
		return oldValue;
	}
};

END_DTL_NAMESPACE

#endif
