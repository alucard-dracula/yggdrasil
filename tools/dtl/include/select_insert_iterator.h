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
// iterators for reading (SELECT) and writing (INSERT) to/from database
// nested class of DBView
// Initial: 9/8/2000 - MG
// Edited: 12/19/2000 - MG - added namespaces

#ifndef DTL_SELECT_INSERT_ITERATOR_H
#define DTL_SELECT_INSERT_ITERATOR_H

#include "DB_iterator.h"

BEGIN_DTL_NAMESPACE

// iterators for reading (SELECT) and writing (INSERT) to/from database
template<class DataObj, class ParamObj = DefaultParamObj<DataObj> > 
  class DB_select_insert_iterator :
	public DB_iterator_with_cols<DataObj, ParamObj>
{
public:
	// each class must define a default constructor
	DB_select_insert_iterator() : DB_iterator_with_cols<DataObj, ParamObj>() { }

    DB_select_insert_iterator(const DBView<DataObj, ParamObj> &view,  SQLQueryType qryType,
		bool bPrepare) :   
		DB_iterator_with_cols<DataObj, ParamObj>(view, qryType, bPrepare)
	{ }

	// copy constructor and assignment operator required for Assignable property
    DB_select_insert_iterator(const DB_select_insert_iterator<DataObj, ParamObj> &iter) : 
		DB_iterator_with_cols<DataObj, ParamObj>(iter)
	{ }

	// exception-safe swap() function
	void swap(DB_select_insert_iterator<DataObj, ParamObj> &other)
	{
		DB_iterator_with_cols<DataObj, ParamObj>::swap(other);
	}

	// assign one iterator to another
	// exception-safe
	DB_select_insert_iterator<DataObj, ParamObj>
		&operator=(const DB_select_insert_iterator<DataObj, ParamObj> &other)
	{
		if (this != &other)
		{
			DB_select_insert_iterator<DataObj, ParamObj> temp(other);
			swap(temp);
		}
		
		return *this;
	
	}

	virtual ~DB_select_insert_iterator()
	{ }
};

END_DTL_NAMESPACE

#endif
