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
// base class for DB accessors ... nested classes inside of DBView template
// initial: 9/7/2000 - MG
// Reviewed: 11/12/2000 - CJ
// Edited: 12/19/2000 - MG - added namespaces

#ifndef DTL_DB_ITERATOR_H
#define DTL_DB_ITERATOR_H

#include "dtl_assign_t.h"

#include "DBView.h"

#include "DynaDBView.h"
#include "DBStmt.h"

BEGIN_DTL_NAMESPACE

// creating the BoundIOs object is common for both kinds of
// contains facilities common to all database accessors (SELECT, INSERT, UPDATE,
// DELETE)
template<class DataObj, class ParamObj = DefaultParamObj<DataObj> > class DB_iterator :
	public dtl_ios_base
{
public:
	// What type of query?
    // enum SQLQueryType { SELECT, INSERT, UPDATE, DEL, USER_DEFINED };

	// get query type of the iterator
	SQLQueryType GetQueryType() const { return sqlQryType; }

	typedef ParamObj param_type;

protected:
	DBView<DataObj, ParamObj> *pDBview; // view this accessor refers to
	BoundIOs boundIOs;		// map of bindings from memory structures to DB columns
							// and parameters
	CountedPtr<DataObj>  pRowbuf;		// pointer to the buffer for a DataObj
	CountedPtr<ParamObj> pParambuf;		// current parameter object for accessor
	DBStmt stmt;			// statement for this accessor

	SQLQueryType sqlQryType; // what type of SQL query is this?

	typedef BCAWrap<DataObj> BCA;
	typedef BPAWrap<ParamObj> BPA;

	BCA bca; // bind column address function
	BPA bpa; // bind parameter address function

	// signature of error handler for the iterator
	// couldn't get a typedef in DBView with a forwarding typedef here to work
	// due to template instantiation problem
	// typedef typename DBView<DataObj, ParamObj>::IOHandler IOHandler;

	mutable IOHandler<DataObj, ParamObj> io_handler; // error handler for this iterator

	long count;		// # of records accessed so far since iterator was opened
	long lastCount; // # of records accessed in last operation
					// insert and select iterators, means since last
					// iterator assignment or copy
					// update and delete, means since last call
					// to operator++()

    // is accessor ready for use ... that is, is stmt. ready to be executed
	bool IsReady() const { return stmt.IsExecuted(); }

	// used to manually allocate DataObj, ParamObj and bind them
	// needed for proper behavior with variant_rows
	void BuildAndBindEarly()
	{
		if (!pRowbuf) {
		   // allocate DataObj and ParamObj manually
		   pRowbuf = new DataObj();
		}

		if (!pParambuf) {
		   // allocate DataObj and ParamObj manually
		   pParambuf = new ParamObj();
		}

		// manually invoke BCA and BPA
		boundIOs.BindAsBase(*pParambuf);
		bpa(boundIOs, *pParambuf);

		boundIOs.BindAsBase(*pRowbuf);
		bca(boundIOs, *pRowbuf);

	}

	// Point the boundIOs to a new rowbuf
	void ReBindRowbuf() {
		if (!pRowbuf)
			pRowbuf = new DataObj();
		else
			pRowbuf = new DataObj(*pRowbuf);
		boundIOs.EraseColumns();
		boundIOs.BindAsBase(*pRowbuf);
		bca(boundIOs, *pRowbuf);
		if (IsReady())
		  MakeBindings();
	}

	// functions to open and destroy the iterators - mainly for bindings and DB resources
    virtual void open()
	{
	    try
		{
		  if (stmt.IsReady())
			return;

		  if (pRowbuf.GetRefCount() > 1) // if shared RowBuf release & rebind
			ReBindRowbuf();

		  stmt.Initialize();

		  MakeBindings(); // virtual call will invoke appropriate version of MakeBindings()

		  count = 0;
		  lastCount = 0;
		}
		catch (...)
		{
			setstate(badbit);   // iterator is bad if open is unsuccessful
			throw;
		}
	}


	// no throw destroy()
	virtual void destroy()
	{
		// clean out stmt
	   	if (stmt.IsAllocated())
	   	{
		   try
		   {
			   stmt.Destroy();
		   }
		   catch (...)
		   {

		   }
		}

		count = 0;
		lastCount = 0;

		// note that we don't touch the rowbuf reference in here
		// if the iterator gets reopened, it will get a fresh rowbuf reference to work
		// with
	}

	virtual void ClearBindings()
	{
		stmt.UnbindParams();
		stmt.UnbindCols();
	}

	virtual void ClearColBindings()
	{
		stmt.UnbindCols();
	}

	// when we have to change a query for the underlying statement,
	// which me must when we RemoveAppendedFields()
	// create a temporary DBView that uses the new colNames
	// to gives us our query
	virtual void FixQuery(const STD_::vector<tstring> &removedFieldNames)
	{
		if (removedFieldNames.empty())
			return;

		if (pDBview == NULL)
			DTL_THROW DBException(_TEXT("DB_iterator::FixQuery()"),
				_TEXT("Unable to fix query through NULL view ptr."), NULL, NULL);

		DBView<DataObj, ParamObj> tmp_view(*pDBview);

		// remove each column from the view that was removed from the iterator's BoundIOs
		for (STD_::vector<tstring>::const_iterator col_it = removedFieldNames.begin();
				col_it != removedFieldNames.end(); ++col_it)
		{
			tmp_view.RemoveColName(*col_it);
		}

		stmt.ResetWithNewQuery(tmp_view.BuildQry(sqlQryType));
	}


	// utility function to aid with making the necessary calls
	// to stmt.BindCol() and stmt.BindParam() for the BoundIOs

	// exception safety impossible if BPA isn't exception safe
	// (may not be possible anyway?)
	virtual void MakeBindings()
	{
	  try
	  {
	    ClearBindings();

		// apply BPA in BuildAndBindEarly

		// *** For DynamicRowBCA case only for BCA ***
	    // for inserts and updates must remove appended fields as reported by GetFieldInfo()
	    // from table fields list
		STD_::vector<tstring>
			removed = RemoveAppendedFieldsFromBCA(bca, boundIOs, sqlQryType);
										// if query type not INSERT or UPDATE, does nothing

		// must fix DBStmt's query to reflect the dropped columns
		if (!removed.empty())
			FixQuery(removed);

		// fix column names for correct binding order ... needed for GetData()
		// in the case of sql_iterator, trust the programmer
		if (sqlQryType != SQL)
			boundIOs.GetColumnNames();


		ValidateParams();

		if (sqlQryType == INSERT)
			return;

		// need to know the number of parameters already bound so
		// we can add the proper offset to calls to BindParam()
		int numParamsAlreadyBound;

		// number of parameters already bound is:
		// 0 in the case of a select (they've been bound using BindCol())
		// boundIOs.NumColumns() in the case of an insert, update, or delete

		switch (sqlQryType)
		{
		case SELECT: case SQL: default: numParamsAlreadyBound = 0; break;
		case INSERT: case UPDATE: case DEL:
			numParamsAlreadyBound = boundIOs.NumColumns();
			break;
		}

		// now bind the parameters using stmt.BindParam()
		for (BoundIOs::iterator b_it = boundIOs.begin();
				b_it != boundIOs.end(); b_it++)
		{
			BoundIO &param = (*b_it).second;

			if (param.IsParam())
			{
			   // bind the actual column using BindParam()

			   // apply offset to get parameter # we need to bind to in the SQL stmt.
			   int paramNum = numParamsAlreadyBound + std_tatoi(param.GetName().c_str());

			   // if parameter is a variable string we are going to use PutData()
			   // otherwise use standard binding
			   if (param.IsVariString()) // variable string case
			   {
				 // calculate magic # that will be used later by ParamData()
				 // which is simply the distance of iterator from boundIOs.begin()
			     // NOTE Solaris has a non-standard interface to distance
				 BoundIOs::difference_type   magic_num;
                 magic_num = std_distance(boundIOs.begin(), b_it);

				 // cast magic # to type required by BindParam()
				 void *magic_ptr = reinterpret_cast<void *>(magic_num);

				 SQLLEN *bytes_fetched_ptr =
							param.GetLenAtExecPtr();

				 switch (sqlQryType)
				 {
				   case INSERT: break;
				   case SQL: // not sure how to handle output parameters
					 stmt.BindParam(paramNum, param.GetParamType(),
							param.GetCType(), param.GetSQLType(),
							param.GetColumnSize(pDBview->GetConnection()), param.GetPrecision(),
							magic_ptr, 0, bytes_fetched_ptr);
						break;
				   default:
					 stmt.BindParam(paramNum, SQL_PARAM_INPUT,
							param.GetCType(), param.GetSQLType(),
							param.GetColumnSize(pDBview->GetConnection()), param.GetPrecision(),
							magic_ptr, 0, bytes_fetched_ptr);
						break;
				 }

			   }

			   else // standard binding
			   {
			     // now actually bind the parameter
			     switch (sqlQryType)
				 {
				    case INSERT: break;
				    case SQL:
					  stmt.BindParam(paramNum, param.GetParamType(),
						param.GetCType(), param.GetSQLType(),
						param.GetColumnSize(pDBview->GetConnection()), param.GetPrecision(), param.GetRawPtr(),
						param.GetBufferLength(), param.GetBytesFetchedPtr());
					  break;
				    default:
					  stmt.BindParam(paramNum, SQL_PARAM_INPUT,
						param.GetCType(), param.GetSQLType(),
						param.GetColumnSize(pDBview->GetConnection()), param.GetPrecision(), param.GetRawPtr(),
						param.GetBufferLength(), param.GetBytesFetchedPtr());
					break;
				 }

			   }
			}

		}
	  }
	  catch (...)
	  {
		 setstate(badbit);
		 throw;
	  }

	}

	// returns a valid rowbuf reference in order to preserve the invariants for input iterators
	CountedPtr<DataObj> GetRowbufPtr()
	{
		if (pRowbuf && count > 0) // have valid existing row
			return pRowbuf;

		open(); // exception safe if open() is exception safe

		return pRowbuf;
	}

public:
	DB_iterator() : dtl_ios_base(),
				pDBview(NULL), boundIOs(), pRowbuf(NULL), pParambuf(NULL), stmt(), sqlQryType(USER_DEFINED),
				bca(DefaultBCA<DataObj>()),
				bpa(DefaultBPA<ParamObj>()),
				io_handler(DEFAULT_IO_HANDLER<DataObj, ParamObj>()),
				count(0), lastCount(0)
	{ }

	DB_iterator(const DBView<DataObj, ParamObj> &view, SQLQueryType qryType,
				bool bPrepare) :
				dtl_ios_base(),
				pDBview(NULL), //boundIOs(BoundIO::BIND_ADDRESSES),
				boundIOs(), pRowbuf(NULL), pParambuf(NULL), stmt(view.BuildQry(qryType), view.GetConnection(), bPrepare, false), sqlQryType(qryType),
				bca(view.GetBCA()), bpa(view.GetBPA()),
				io_handler(view.io_handler),
				count(0), lastCount(0)
	{
	  DBView<DataObj, ParamObj> &view_ref =
		  const_cast<DBView<DataObj, ParamObj> &>(view);
	  pDBview = &view_ref;

	  // needed for proper variant_row behavior
	  BuildAndBindEarly();
	}

    // copy constructor and assignment operator required for Assignable property
	DB_iterator(const DB_iterator<DataObj, ParamObj> &db_it) :
	    dtl_ios_base(),
		pDBview(db_it.pDBview), //boundIOs(BoundIO::BIND_ADDRESSES),
		boundIOs(), pRowbuf(db_it.pRowbuf), pParambuf(new ParamObj(*(db_it.pParambuf))),
		stmt(db_it.stmt),
		sqlQryType(db_it.sqlQryType), bca(db_it.bca), bpa(db_it.bpa), io_handler(db_it.io_handler), count(db_it.count),
		lastCount(db_it.lastCount)
		{
		// needed for proper variant_row behavior
		BuildAndBindEarly();

		}

	// exception-safe swap()
	void swap(DB_iterator<DataObj, ParamObj> &other) {
		// put the two user constructs at the beginning to minimize damage if they throw
		// -- which they should not as part of the user requirements
		dtl_ios_base::swap(other);
		bca.swap(other.bca);
		bpa.swap(other.bpa);
		STD_::swap(pDBview, other.pDBview);
		boundIOs.swap(other.boundIOs);
		pParambuf.swap(other.pParambuf);
		pRowbuf.swap(other.pRowbuf);
		stmt.swap(other.stmt);
		STD_::swap(sqlQryType, other.sqlQryType);
		STD_::swap(count, other.count);
		STD_::swap(lastCount, other.lastCount);
		io_handler.swap(other.io_handler);
	}

	// exception-safe assignment
	DB_iterator<DataObj, ParamObj> &
		operator=(const DB_iterator<DataObj, ParamObj> &db_it)
	{
	  if (this != &db_it)
	  {
		DB_iterator<DataObj, ParamObj> temp(db_it);
		swap(temp);
	  }

	  return *this;
	}

#if 0
	// return a pointer to the rowbuf
	DataObj *operator->()
	{
	   return GetRowbufPtr();
	}
#endif

	// operator*() must return a DataObj rather than
	// a DataObj & for select iterators
	// all other iterator types return a DataObj &

	// needed so users can set parameter values
	ParamObj &Params()
	{
		if (!pParambuf)
			BuildAndBindEarly();

		return *pParambuf;
    }

	// exception-safety - atomicity and consistency guaranteed
	// if ParamObj::operator=() is atomic and consistent
	void Params(const ParamObj &params)
	{
		if (!pParambuf)
			BuildAndBindEarly();

		*pParambuf = params;
	}


	unsigned int GetBoundParamCount()
	{
		return (unsigned int) (boundIOs.NumParams());
	}

	// returns a *copy* of the DataObj currently held by the iterator
	// note that we only use are careful not to open the iterator here
	DataObj GetDataObj() const
	{
		DB_iterator<DataObj, ParamObj> *this_ptr =
			const_cast<DB_iterator<DataObj, ParamObj> *>(this);
		if (!pRowbuf)
			this_ptr->BuildAndBindEarly(); // allocate rowbuf if we don't already have one
		return *(this_ptr->pRowbuf);
	}

	// return view referred to by this iterator
	// made to return as const so the caller can not modify the view
	const DBView<DataObj, ParamObj> &GetView() const
	{
	  return *pDBview;
	}

	// return # of records accessed so far since iterator was opened
	long GetCount() const
	{
	  return count;
	}

	// return # of records accessed since last operation
	// (see description of lastCount data member for more details)
	long GetLastCount() const
	{
      return lastCount;
	}

	// return query tstring associated with this iterator
	tstring GetQuery() const
	{
	  return stmt.GetQuery();
	}

	// return a reference to the accessor's stmt.
	DBStmt &GetStmt() { return stmt; }
	const DBStmt &GetStmt() const { return stmt; }

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

	void set_io_handler(IOHandler<DataObj, ParamObj> h)
	{
		io_handler.swap(h) ;
	}

	virtual ~DB_iterator()
	{
		try
		{
		    destroy();
		}
		catch (...)
		{

		}
	}

	// accessor so we can print out BoundIOs
	BoundIOs GetBoundIOs() const
	{
		return boundIOs;
	}

	// validate the integrity of the params ... boundIOs[0] ... boundIOs[NumParams() - 1]
	// must exist
	void ValidateParams() const
	{
		size_t cnt = numOfOccurrences(_TEXT('?'), pDBview->GetPostfixClause());

		if (cnt == 0)
			return;

		STD_::set<size_t> paramNums;

		for (BoundIOs::const_iterator b_it = boundIOs.begin();
					b_it != boundIOs.end(); ++b_it)
		{
			if (b_it->second.IsParam())
				paramNums.insert(std_tatoi(b_it->first.c_str()));
		}

		size_t checker = 0;

		for (STD_::set<size_t>::iterator set_it = paramNums.begin();
				set_it != paramNums.end(); ++set_it, ++checker)
		{
			size_t param = *set_it;
			if ((param != checker) || (param >= cnt))
				DTL_THROW DBException(_TEXT("DB_iterator::ValidateParams()"),
					_TEXT("Invalid parameter numbering in BPA!"), NULL, NULL);

		}

	}

	// set flag to use SQLPrepare when executing iterator
	void SetPrepare(bool flag) {stmt.SetPrepare(flag);}

	void Reset()
	{
	    stmt.Reset();
	    count = 0;
	    lastCount = 0;
		dtl_ios_base * base = static_cast<dtl_ios_base *>(this);
		*base = dtl_ios_base(); // reset any error flags
	}

	friend class DB_select_update_iterator<DataObj, ParamObj>;
};

// cursor base with added abstractions for columns ... needed for SELECT, INSERT,
// UPDATE
template<class DataObj, class ParamObj = DefaultParamObj<DataObj> >
	class DB_iterator_with_cols :
       public DB_iterator<DataObj, ParamObj>
{
  protected:

	// utility function to aid with making the necessary calls
	// to stmt.BindCol() and stmt.BindParam() for the BoundIOs

	// can't be made exception safe???
	virtual void MakeBindings()
	{
	 try
	 {
	   // apply BCA (generate binding addr. mapping)
	   // boundIOs.BindAsBase(*pRowbuf);
       // bca(boundIOs, *pRowbuf);

	   // superclass method does first part of work
	   // binding the params ... needed to be done first as mySQL requires
	   // all parameters to be bound before any columns may be bound
	   DB_iterator<DataObj, ParamObj>::MakeBindings();

	   // bind columns appropriately
	   MakeColBindings();

	 }
	 catch (...)
	 {
		this->setstate(this->badbit);
		throw;
	 }
	}

	// make column bindings only
	virtual void MakeColBindings(bool need_to_clear = false)
	{
	  try
	  {
	   if (need_to_clear)
		   this->ClearColBindings();

	   // note that columns will appear in the map in the same order that they are
	   // in the view's colNames set as both conainers are ordered
	   for (BoundIOs::iterator bc_it = this->boundIOs.begin(); bc_it != this->boundIOs.end(); bc_it++)
	   {
			BoundIO &column = (*bc_it).second;

			if (column.IsColumn())
			{
#ifdef DTL_NO_SQL_GETDATA
			  if (true) // Use SQLBindCol instead of SQLGetData
#else
			  if (!column.IsVariString()) // standard binding
#endif
			  {
				// get target buffer we need to bind to
				void *addr_to_bind = column.GetRawPtr();
				SQLLEN size_to_bind = column.GetRawSize();
				SQLLEN buffer_len = column.GetBufferLength();

				// must call stmt.BindCol() on a SELECT
				// and BindParam() for INSERT and UPDATE
				switch (this->sqlQryType)
				{
				   case SQL:
					  this->stmt.BindCol(column.GetColNo(),
						    column.GetCType(), addr_to_bind,
					        size_to_bind, column.GetBytesFetchedPtr());
					  break;
				   case SELECT:
					  // bind the actual column using BindCol()
					  this->stmt.BindCol(column.GetColNo(),
						    column.GetCType(), addr_to_bind,
					        size_to_bind, column.GetBytesFetchedPtr());
					  break;
				   case INSERT: case UPDATE: case DEL:
					  // bind the actual column using BindParam()
					  this->stmt.BindParam(column.GetColNo(), SQL_PARAM_INPUT,
						column.GetCType(), column.GetSQLType(),
						column.GetColumnSize(this->pDBview->GetConnection()), column.GetPrecision(), addr_to_bind,
						buffer_len, column.GetBytesFetchedPtr());
					  break;
				   case USER_DEFINED:
				   	DTL_THROW RootException(_TEXT("MakeColBindings"),_TEXT("USER_DEFINED not supported"));
				}
			  }
			  // Do not bind columns we intend to retrieve with SQLGetData
			  // In the case of INSERT, UPDATE or DELETE we still need to convert columns
			  // to parameters here.
			  //
			  else if (column.IsVariString() && this->sqlQryType != SELECT &&
							this->sqlQryType != SQL)
			  {
				 // calculate magic # that will be used later by ParamData()
				 // which is simply the distance of iterator from boundIOs.begin()
				 // NOTE Solaris has a non-standard interface to distance
				 BoundIOs::difference_type   magic_num;

                 magic_num = std_distance(this->boundIOs.begin(), bc_it);

				 // cast magic # to type required by BindParam()
				 void *magic_ptr = reinterpret_cast<void *>(magic_num);

				 SQLLEN *bytes_fetched_ptr =
							column.GetLenAtExecPtr();

				 // INSERT, UPDATE, DEL
				 this->stmt.BindParam(column.GetColNo(), SQL_PARAM_INPUT,
							column.GetCType(), column.GetSQLType(),
							column.GetColumnSize(this->pDBview->GetConnection()), column.GetPrecision(),
							magic_ptr, 0, bytes_fetched_ptr);


			  }
			}
	   }
	  }
	  catch (...)
	  {
		this->setstate(this->badbit);
		throw;
	  }
	}

  public:
	DB_iterator_with_cols() : DB_iterator<DataObj, ParamObj>()
	{ }

	DB_iterator_with_cols(const DBView<DataObj, ParamObj> &view, SQLQueryType qryType,
			bool bPrepare) :
			DB_iterator<DataObj, ParamObj>(view, qryType, bPrepare)
	{ }

	// copy constructor and assignment operator required for Assignable property
	// the superclass versions should do all the work
	DB_iterator_with_cols(const DB_iterator_with_cols<DataObj, ParamObj> &db_it) :
			DB_iterator<DataObj, ParamObj>(db_it)
	{ }

	// exception safe swap function
	void swap(DB_iterator_with_cols<DataObj, ParamObj> &other) {
		DB_iterator<DataObj, ParamObj>::swap(other);
	}

	// exception safe assignment
	DB_iterator_with_cols<DataObj, ParamObj> &
		operator=(const DB_iterator_with_cols<DataObj, ParamObj> &db_it)
	{
		if (this != &db_it)
		{
			DB_iterator_with_cols<DataObj, ParamObj> temp(db_it);
			swap(temp);
		}

		return *this;
	}

	virtual ~DB_iterator_with_cols() { }

};

//template<class DataObj> void dtl_assign(const DataObj &src, DataObj &target) {
//	target = src;
//}
//
//inline void dtl_assign(const variant_row &src, variant_row &target) {
//	target.replace(src);
//}

//template<class DataObj>
//struct dtl_assign_t
//{
//	void operator()(const DataObj& src, DataObj& target) const
//	{
//		target = src;
//	}
//};
//
//template<>
//struct dtl_assign_t<variant_row>
//{
//	void operator()(const variant_row& src, variant_row& target) const
//	{
//		target.replace(src);
//	}
//};

END_DTL_NAMESPACE

#endif
