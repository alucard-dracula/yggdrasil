//dtl_assign_t.hpp

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

#ifndef DTL_ASSIGN_T_H
#define DTL_ASSIGN_T_H

#include "dtl_config.h"
#include "variant_row.h"

BEGIN_DTL_NAMESPACE

template<class DataObj>
struct dtl_assign_t
{
	void operator()(const DataObj& src, DataObj& target) const
	{
		target = src;
	}
};

template<>
struct dtl_assign_t<variant_row>
{
	void operator()(const variant_row& src, variant_row& target) const
	{
		target.replace(src);
	}
};

END_DTL_NAMESPACE

#endif // DTL_ASSIGN_T_H