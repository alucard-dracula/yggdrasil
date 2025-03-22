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

///////////////////////////////////////////////////////////////////////////////
//                                 Version 3.7
///////////////////////////////////////////////////////////////////////////////

#ifndef DTL_DTL_H_
#define DTL_DTL_H_

// DTL.h includes everything in the library
// however, if you are only using part of the library,
// it is faster to only include the specific headers you are using.
// So, e.g., if you are only using DBView and a select_iterator in your code, 
// you need only #include "DBView.h" and "select_iterator.h."
// Also, be sure to explicity #include any standard library headers you need
#include "DBView.h"
#include "select_iterator.h"
#include "insert_iterator.h"
#include "update_iterator.h"
#include "delete_iterator.h"
#include "sql_iterator.h"
#include "select_update_iterator.h"
#include "IndexedDBView.h"
#include "DynaDBView.h"
#include "LocalBCA.h"
#include "dtl_algo.h"
#include "RandomDBView.h"

#endif
