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
// abstraction for UPDATES
// nested class of DBView
// Initial: 9/8/2000 - MG
// Edited: 12/19/2000 - MG - added namespaces

#ifndef DTL_UPDATE_ITERATOR_H
#define DTL_UPDATE_ITERATOR_H

#include "DB_iterator.h"

#include "std_warn_off.h"
#include <string>
#include <vector>
#include "std_warn_on.h"

BEGIN_DTL_NAMESPACE

// abstraction for UPDATES
template<class DataObj, class ParamObj = DefaultParamObj<DataObj> >
  class DB_update_iterator :
     public DB_iterator_with_cols<DataObj, ParamObj>,
#ifndef __SGI_STL_PORT
	 public STD_::iterator<STD_::output_iterator_tag, DataObj, std::ptrdiff_t>
#else
	 // not sure why STLport not recognizing the standard iterator tag
	 // when used in algorithms like copy(_InputIter __first, _InputIter __last, _OutputIter __result)
	 // use STLport specific alternative
	 public STD_::forward_iterator<DataObj, std::ptrdiff_t>
#endif
{
//#ifdef __GNUC__ // need to declare members used from base template //org
//#if defined(__GNUC__) || defined(_MSC_VER) // need to declare members used from base template
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
//#endif
private:
    InsValWrap<DataObj> InsValidate;
    bool validRowbuf; // was previous object valid?

	// call this function to execute the update
	// returns number of rows updated
	// to perform another update, change the necessary data and parameter values
	// and then make another call to ExecUpdate() to perform that update

	// exception-safety questionable
	int ExecUpdate()
	{
			if (!this->IsReady())
			{
	    	    this->open();
			}

		    if (this->bad())
			{
			  DTL_THROW DBException(_TEXT("DBView::delete_iterator::ExecDelete()"),
				_TEXT("iterator tested bad!"), NULL, NULL);
			}

			this->lastCount = 0;
			int updatedRows = 0;
			try
			{
			   // must call InsValidate() here to fix bug with NULL's
			   // BoundIO columns get erased in this instance with open()

			   if (!InsValidate(this->boundIOs, *this->pRowbuf))
			   {
				 DTL_THROW DBException(_TEXT("DBView::update_iterator::ExecUpdate()"),
				   _TEXT("InsValidate() call failed on statement ") + this->stmt.GetQuery(), NULL, NULL);
			   }
			   validRowbuf = true;

			   // propagate STL strings to their strbufs for proper binding
            // DO NOT EXECUTE THIS HERE
            // It screws up previous validations.
            // this->boundIOs.InsUserValidate();
			   this->boundIOs.PropagateToSQL(this->sqlQryType, this->stmt);
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
					io_handler(ex, *this, *(this->pRowbuf.get()), *this->pParambuf);

				// what to do in case of REPERFORM_OPERATION???
				switch (error_action)
				{
				case dtl_ios_base::THROW_EXCEPTION: throw;
				case dtl_ios_base::SUPPRESS_ERROR: return (this->lastCount = 0);
				}
			}


			// object becomes inconsistent if this line fails
			try
			{
				updatedRows = this->stmt.RowCount();
			}
			catch (RootException &ex)
			{
				if (this->stmt.valid())
					this->setstate(this->failbit);
				else
					this->setstate(this->badbit);

				dtl_ios_base::MeansOfRecovery error_action =
					io_handler(ex, *this, *(this->pRowbuf.get()), *this->pParambuf);

				// what to do in case of REPERFORM_OPERATION???
				switch (error_action)
				{
				case dtl_ios_base::THROW_EXCEPTION: throw;
				case dtl_ios_base::SUPPRESS_ERROR: return (this->lastCount = 0);
				}
			}

			// count member is *cumulative* number of rows updated so far
			// do things this way in order to stay consistent with the other
			// DB iterator classes
			this->count += updatedRows;
			this->clear();
			return (this->lastCount = updatedRows);
	}

public:
    DB_update_iterator() : DB_iterator_with_cols<DataObj, ParamObj>(),
		InsValidate(DefaultInsValidate<DataObj>()), validRowbuf(false)
	{ }

    DB_update_iterator(const DBView<DataObj, ParamObj> &view, bool bPrepare = true) :
			DB_iterator_with_cols<DataObj, ParamObj>(view, UPDATE, bPrepare),
			InsValidate(view.GetInsVal()), validRowbuf(false)
			{ }

	// copy constructor and assignment operator required for Assignable property
	DB_update_iterator(const DB_update_iterator<DataObj, ParamObj> &up_it) :

		DB_iterator_with_cols<DataObj, ParamObj>(up_it),
			InsValidate(up_it.InsValidate), validRowbuf(up_it.validRowbuf)
	{ }

	// exception-safe swap()
	void swap(DB_update_iterator<DataObj, ParamObj> &other)
	{
		DB_iterator_with_cols<DataObj, ParamObj>::swap(other);
		STD_::swap(InsValidate, other.InsValidate);
		STD_::swap(validRowbuf, other.validRowbuf);
	}

	// exception-safe assignment
	DB_update_iterator<DataObj, ParamObj> &
		operator=(const DB_update_iterator<DataObj, ParamObj> &other)
	{
		if (this != &other)
		{
			DB_update_iterator<DataObj, ParamObj> temp(other);

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

	// commit updates from this updater and clean up
	virtual ~DB_update_iterator()
	{ }

	// return a proxy
	// exception-safe
	DB_update_iterator<DataObj, ParamObj> &
		operator*()
	{
		return *this;
	}

	// proxy paradigm used to be able to perform InsValidate()
	// on the rowbuf held by the iterator
	// exception-safety dependent on GetRowbufPtr(), InsValidate(),
	// and DataObj::operator=()
	DB_update_iterator<DataObj, ParamObj> &
		operator=(const DataObj &data)
	{

		CountedPtr<DataObj> pData = NULL;

		try
		{
		   pData = this->GetRowbufPtr();
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

				throw DBException(_TEXT("DBView::update_iterator::operator=(const DataObj &)"),
							  _TEXT("InsValidate() failed on statement \"") +
							  this->stmt.GetQuery() + _TEXT("\"!"), NULL, NULL);
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

		validRowbuf = true;
		this->clear();

        // commit on operator=()
	    ExecUpdate(); // ExecUpdate() will open() the iterator if necessary
		return *this;
	}



	// execute the update for the referenced DataObj and preincrement
	// IO handler logic is in ExecUpdate()
	DB_update_iterator<DataObj, ParamObj> &operator++()
	{
	   return *this;
	}

	// execute the update for the referenced DataObj and postincrement
	inline DB_update_iterator<DataObj, ParamObj> operator++(int) //non-const return to allow output via old iterator
	{
	   DB_update_iterator<DataObj, ParamObj> oldValue(*this);
	   ++(*this);
	   return oldValue;
	}
};

// builds the postfix clause for update of the fields listed in boundIOs
inline STD_::pair<tstring, STD_::vector<tstring> > PostfixForUpdate(const BoundIOs &boundIOs, tstring QuoteChar)
{
   tstring postfix;
   postfix.reserve(1024);
   postfix += _TEXT("WHERE (");
   STD_::vector<tstring> orderOfParams;

   BoundIOs::const_iterator it = boundIOs.begin();

   // First, check if its NULL
   // note: do NOT add it to orderOfParams if null - no variable.
   if ((*it).second.IsNull())
      postfix += QuoteChar + (*it).first + QuoteChar + _TEXT(" IS NULL");

   else
   {

      // Doubles cannot be compared exactly via equality, therefore, it's a bad
      // idea to have them in a WHERE clause. Some databases such as MySQL are strict
      // about this and will fail on SQL_DOUBLE or SQL_FLOAT comparisons.
      // Here we try to do something sensible for these cases.
      // Compare within an epsilon of accuracy.
      if ((*it).second.GetTypeID() == C_DOUBLE || (*it).second.GetTypeID() == C_FLOAT)
         postfix += _TEXT("ABS(") + QuoteChar + (*it).first + QuoteChar + _TEXT(" - (?)) <= ") +
         _TEXT("ABS(") + QuoteChar + (*it).first + QuoteChar + _TEXT(")/1000000 ");
      else
          postfix += QuoteChar + (*it).first + QuoteChar + _TEXT(" = (?) ");

      orderOfParams.push_back((*it).first);
   }

   it++;


   while (it != boundIOs.end())
   {
	   postfix += _TEXT(" AND ");

      // First, check if its NULL
      // note: do NOT add it to orderOfParams if null - no variable.
      if ((*it).second.IsNull())
         postfix += QuoteChar + (*it).first + QuoteChar + _TEXT(" IS NULL");

      else
      {
         if ((*it).second.GetTypeID() == C_DOUBLE || (*it).second.GetTypeID() == C_FLOAT)
            postfix += _TEXT("ABS(") + QuoteChar + (*it).first + QuoteChar + _TEXT(" - (?)) <= ") +
            _TEXT("ABS(") + QuoteChar + (*it).first + QuoteChar + _TEXT(")/1000000 ");

         else
            postfix += QuoteChar + (*it).first + QuoteChar + _TEXT(" = (?) ");

         orderOfParams.push_back((*it).first);
      }

	   it++;
   }

   postfix += _TEXT(")");

   return STD_::pair<tstring, STD_::vector<tstring> >(postfix, orderOfParams);
}

// builds the postfix clause for update of the fields listed in boundIOs
// (for static views, we must not attach quote chars as the field names may already be quoted in the view)
// (for dynamic views, we must attach quote chars to protect us from field names with spaces)
template<class DataObj> STD_::pair<tstring, STD_::vector<tstring> >
	MakePostfixForUpdate(const BoundIOs &boundIOs, tstring DTL_DUMMY_PARAM_1, const DataObj * DTL_DUMMY_PARAM_2)
{
	return PostfixForUpdate(boundIOs, _TEXT(""));
}

template<> inline STD_::pair<tstring, STD_::vector<tstring> >
	MakePostfixForUpdate(const BoundIOs &boundIOs, tstring QuoteChar, const variant_row * DTL_DUMMY_PARAM_1)
{
	return PostfixForUpdate(boundIOs, QuoteChar);
}

END_DTL_NAMESPACE

#endif
