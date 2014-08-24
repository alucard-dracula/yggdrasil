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
// enumerations, constants, and other basics needed by DB Abstractions
// initial: 9/5/2000 - MG
// revised: 10/6/2000 - MG - added BoundColMode BIND_AS_INDICES and BoundType INDEX
// reviewed: 11/12/2000 - CJ
// revised: 12/19/2000 - MG - adding namespaces!

#ifndef DTL_DB_BASE_H
#define DTL_DB_BASE_H

#include "dtl_base_types.h"

BEGIN_DTL_NAMESPACE

// uncomment if your DataObj's are printable, that is, they define operator<<()
// #define DATAOBJ_PRINTABLE

// enumerations

// What type of query?
enum SQLQueryType { SELECT, INSERT, UPDATE, DEL, SQL, USER_DEFINED };

// is the indexed view operating in bound or unbound mode?
// bound = changes propagate through to DB
// unbound = changes don't propagate
enum BoundMode { UNBOUND, BOUND };

// is the indexed view inserting or deleting based on primary key fields only,
// all fields, or use the auto key mechanism
enum KeyMode { USE_ALL_FIELDS, USE_PK_FIELDS_ONLY, USE_AUTO_KEY };

// fetch mode to use for IndexedDBView
enum FetchMode { BULK_FETCH, SINGLE_FETCH };


// needed for implicit conversion between ints and dtl_state_enums
// exception-safe as only working with enums
class dtl_iostate
{
private:

   enum dtl_state_enum
   {
     goodbit = 0x00,
     failbit = 0x01,
     badbit = 0x02,
     bad_and_fail_bit = 0x03 // never use this option
   };

   dtl_state_enum my_state;   
public:
   dtl_iostate(dtl_state_enum state = goodbit);

   // conversion from int to dtl_iostate
   dtl_iostate(int state);

   // conversion from dtl_iostate to int
   operator int() const;

   friend class dtl_ios_base;
};

class dtl_ios_base
{
public:
   
protected:
   
   dtl_iostate my_io_state; // bits indicating what dtl_iostate I am in
public:
   // flags used by DTL iterators to determine their state
   // goodbit = iterator is OK
   // badbit = iterator is in a unrecoverable state
   // failbit = iterator failed to process last item properly, but 
   //           the iterator itself is OK
   static const dtl_iostate goodbit;
   static const dtl_iostate failbit;
   static const dtl_iostate badbit;

   // how do we wish to recover from an error if one occurs?
   enum MeansOfRecovery {
	   THROW_EXCEPTION, SUPPRESS_ERROR
   };

   // default constructor
   dtl_ios_base();

   // exception-safe swap
   void swap(dtl_ios_base &other);

   // exception-safe assignment
   dtl_ios_base &operator=(const dtl_ios_base &other);

   // return the state of the object
   dtl_iostate rdstate() const;
   
   // *** functions that modify the state of the object
   // also call the object's handler unless rdstate() == goodbit ***

   // sets the state of the IO object, but doesn't clear any already set bits
   void setstate(dtl_iostate state);

   // clears the state and resets it to the state passed in
   void clear(dtl_iostate state = goodbit);

   // tests to indicate whether I am good, bad, or have failed
   bool good() const;

   bool bad() const;

   bool fail() const;

};

END_DTL_NAMESPACE

#endif
