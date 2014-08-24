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
#ifndef DTL_MINIMAX_H
#define DTL_MINIMAX_H

#include "dtl_config.h"

BEGIN_DTL_NAMESPACE

// template functions for min and max ... due to ambiguities in stlport
template<class T> inline const T &DTL_MIN(const T& a, const T& b)
{
	return (a < b ? a : b);	
}

template<class T> inline const T &DTL_MAX(const T& a, const T& b)
{
	return (a > b ? a : b);
}

END_DTL_NAMESPACE

#endif
