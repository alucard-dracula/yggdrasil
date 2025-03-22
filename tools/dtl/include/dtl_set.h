/* Copyright ?2004
 * Paul Harris <paulharris@computer.org>
 *
 * Permission to use, copy, modify, distribute and sell this software and
 * its documentation for any purpose is hereby granted without fee, provided
 * that the above copyright notice appears in all copies and that both that
 * copyright notice and this permission notice appear in supporting documentation.
 * Paul Harris makes no representations about the suitability
 * of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 */


// dtl_set<> is used to provide a generic, DTL-compatible set type.
// You can bind it using the standard <<, >>, == DTL binders.
//
// Initial: 19/11/2004 - PH

#ifndef DTL_DTL_SET_H
#define DTL_DTL_SET_H

#include "dtl_config.h"
#include "BoundIO.h"
#include "array_string.h"
#include "clib_fwd.h"

#include <assert.h>
#include <bitset>

BEGIN_DTL_NAMESPACE

/* USAGE
 * ====
 *

in a header file:
namespace my_ns {

    enum SetMembers_E { Member1, Member2, Member3 };

    DEFINE_DTL_SET(SetMembers,3,23);

    struct Type2
    {
       SetMembers members;
    };
}

in a .cpp file:
namespace my_ns {
    INIT_DTL_SET(SetMembers) =
    { "members1", "members2", "members3" };
}

'3' is number of enums
'23' is the length of the string with ALL the members listed WITH
commas. eg: 'Member1,Member2,Member3' is 23 characters long.

Now you can use it just like an std::bitset, eg

SetMembers members;
if (members[my_ns::Members1])
    do_whatever();

 */

template <class T> struct dtl_set_validate;

template <class Def>
struct dtl_set : Def
{
   typedef typename Def::sql_type sql_type;
   typedef typename Def::SetVals SetVals;
   typedef std::bitset<Def::num> value_type;

   // direct access to the bitset
   operator value_type & () { return value; }
   operator const value_type & () const { return value; }

   bool test( SetVals n ) const
   {
      assert(n >= 0 and n < Def::num);
      return value.test(n);
   }

   void set( SetVals n, bool v )
   {
      assert(n >= 0 and n < Def::num);
      value.set(n,v);
   }

   const bool operator[]( SetVals n ) const { return test(n); }

   typename value_type::reference operator[]( SetVals n )
   {
      assert(n >= 0 and n < Def::num);
      return value[n];
   }

   // build the string from the bitset
   const sql_type str() const
   {
      tstring s;
      s.reserve(Def::sql_len);

      for ( int i = 0; i != Def::num; ++i )
         if (value.test(i))
         {
            if (not s.empty()) s += ",";
            s += Def::sql_values[i].c_str();
         }
      return s;
   }

   friend bool operator== (dtl_set const& a, dtl_set const& b)
   {
      return a.value == b.value;
   }

private:
   // to keep things private, we need to make some friends.
   // this is intrusive.
   friend struct dtl_set_validate<Def>;

   // required to be here for friendship, but can't implement here
   // because older compilers (gcc 3.0.4, for eg) can't find them.
   // Newer compilers can be friends, without the template!
   template <class D> friend BoundIO operator== (BoundIO & b, dtl_set<D> & x);
   template <class D> friend BoundIO operator>> (BoundIO & b, dtl_set<D> & x);
   template <class D> friend BoundIO operator<< (BoundIO & b, dtl_set<D> & x);

   // storage for the DTL binding, it must be updated pre-insert.
   sql_type sql_value;
   value_type value;
};

// Non-intrusive functions to allow binding of this class in DTL

template <class D>
BoundIO operator== (BoundIO & b, dtl_set<D> & x) { return dtl_set_bind(b,x) == x.sql_value; }
template <class D>
BoundIO operator>> (BoundIO & b, dtl_set<D> & x) { return dtl_set_bind(b,x) >> x.sql_value; }
template <class D>
BoundIO operator<< (BoundIO & b, dtl_set<D> & x) { return dtl_set_bind(b,x) << x.sql_value; }

template <class T> struct dtl_set_validate
{

	static bool ins_validate(BoundIOs & b, BoundIOs::UserValidate *val)
	{
      BYTE *offset = val->src_class;
      offset += reinterpret_cast<size_t>(b.GetWorkingAddr());
      dtl_set<T> *px = reinterpret_cast<dtl_set<T> *>(offset);
      // converts the bitset into an sql-string
      px->sql_value = px->str();
      // will always return true
	   return true;
	}

	static bool sel_validate(BoundIOs & b, BoundIOs::UserValidate *val)
	{
      BYTE *offset = val->src_class;
      offset += reinterpret_cast<size_t>(b.GetWorkingAddr());
      dtl_set<T> *px = reinterpret_cast<dtl_set<T> *>(offset);

      // converts the sql-string into the bitset
      // Scan through the tokens and set the appropriate bits.
      px->value.reset();
      TCHAR* savept = NULL;
      TCHAR* buffer = strdup(px->sql_value.c_str());
      for (TCHAR* tok = std_tstrtok(buffer,_TEXT(","),&savept);
            tok != NULL;
            tok = std_tstrtok(NULL,_TEXT(","),&savept))
      {
         typename const dtl_set<T>::sql_type* s = std::find(&dtl_set<T>::sql_values[0],&dtl_set<T>::sql_values[dtl_set<T>::num], tok);
         assert( s != &dtl_set<T>::sql_values[dtl_set<T>::num] ); // check for crazy SQL behaviour
         px->value.set( std::distance(&dtl_set<T>::sql_values[0],s), true);
      }

      // free memory
      free(buffer);

      // will always return true
	   return true;
	}
};


template <class T> BoundIO &dtl_set_bind(BoundIO & b, dtl_set<T> & x)
{
   STD_::vector<tstring> cols;
   cols.push_back(b.GetName());
   std::ptrdiff_t offset = reinterpret_cast<BYTE*>(&x) - b.GetBoundIOsPtr()->GetWorkingAddr();
   BoundIOs::UserValidate uv(reinterpret_cast<BYTE*>(offset), dtl_set_validate<T>::ins_validate, dtl_set_validate<T>::sel_validate, cols);
   b.GetBoundIOsPtr()->AddUserValidate(uv);
   return b;
}


#define DEFINE_DTL_SET(Name,Num,SqlLen) \
   /* Notes: \
    * num is int for the sake of comparisons \
    * max length: all with commas in between \
    */ \
   struct Name ## _Def \
   { \
      typedef Name ## _E SetVals; \
      static const int num = Num; \
      static const size_t sql_len = SqlLen; \
      typedef dtl::tcstring<sql_len> sql_type; \
      static const sql_type sql_values[num]; \
   }; \
   typedef dtl::dtl_set<Name ## _Def> Name

#define INIT_DTL_SET(Name) const Name ## _Def::sql_type Name ## _Def::sql_values[Name ## _Def::num]


END_DTL_NAMESPACE

#endif
