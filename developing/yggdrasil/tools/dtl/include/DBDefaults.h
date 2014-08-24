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
// default templates
// Initial: 9/6/2000 - MG
// Reviewed: 11/12/2000 - CJ
// Edited: 12/19/2000 - MG - added namespaces

#ifndef DTL_DBDEFAULTS_H
#define DTL_DBDEFAULTS_H

#include "BoundIO.h"

BEGIN_DTL_NAMESPACE

// default parameter object
// parameter objects contain objects (normally primitives or strings) whose addresses you
// want bound to SQL parameters using the == operator
template <class DataObj> class DefaultParamObj
{

};

// defaults for InsValidate and SelValidate functions

// insertion is by default, always valid
// DB will return errors when the actual
// insert is performed, if any RI constraints are violated,
// which occurs after InsValidate returns
// if there are certain business rule constraints you wish to enforce, put them in
// your own InsValidate function (of type DBView<DataObj, ParamObj>::InsVal)
// you can also specialize this template if you wish to have different default behavior
// for your data class
template<class DataObj> class DefaultInsValidate 
{
public:

	bool operator()(BoundIOs & DTL_DUMMY_PARAM_1, DataObj & DTL_DUMMY_PARAM_2 ){	
		return true;
	}

	~DefaultInsValidate() {};
};

// select default behavior ... data is valid if and only if
// there are no columns which are null
// if there are other checks you wish to make, put them in
// your own SelValidate function (of type DBView<DataObj, ParamObj>::SelVal)
// you can also specialize this template if you wish to have different default behavior
// for your data class
template<class DataObj> class DefaultSelValidate {

public:
	
	bool operator()(BoundIOs &boundIOs, DataObj & DTL_DUMMY_PARAM_1 ) 
	{
		for (BoundIOs::iterator b_it = boundIOs.begin();
					b_it != boundIOs.end(); b_it++)
		{
			BoundIO &boundIO = (*b_it).second;
			if (boundIO.IsColumn() && !boundIO.IsNullable() && boundIO.IsNull())
				return false;  // found null column ... data is invalid
		}

		return true;	// no nulls found ... data is OK
	}

	~DefaultSelValidate() {};
};

END_DTL_NAMESPACE

#endif
