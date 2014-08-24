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


// dtl_enum<> is used to provide a generic, DTL-compatible ENUM type.
// You can bind it using the standard <<, >>, == DTL binders, but
// it requires some extra work in SelVal and InsVal
//
// Initial: 19/11/2004 - PH

#ifndef DTL_DTL_ENUM_H
#define DTL_DTL_ENUM_H

#include "dtl_config.h"
#include "BoundIO.h"
#include "array_string.h"
#include <assert.h>

BEGIN_DTL_NAMESPACE


/* USAGE
 * ====
 *

in a header file:

namespace my_ns {
    enum E_TypeSource { User, Detected, Imported };
    DEFINE_DTL_ENUM(TypeSource,3,User,8);

    struct MyType
    {
       size_t id;
       TypeSource typeSource;
    };
}

in the .cpp file:
namespace my_ns {
    INIT_DTL_ENUM(TypeSource) = { "USER", "DETECTED", "IMPORTED" };
}


Note that the '3' is # of enums.
Note that 'User' between 3,8 is the default value.
Note that the '8' is the maximum length of any enum in string-form.
Note: You should be able to wrap this with nullable<> and get a nullable
enum type.


Now you can just do stuff like this:
my_ns::MyType xyz;
xyz.typeSource = my_ns::Imported;
... and write it back to the db and it'll work as expected.
switch (xyz.typeSource)
{
    case my_ns::User: ...
    case my_ns::Detected: ...
    case my_ns::Imported: ...
}


*/


// DEFINE_DTL_ENUM
// ===============
//
// Name: Name of class to define. Must have an enum defined as E_Name
//       Enum must be numbered from 0 to n, sequentially.
// Num:  Number of enumerated values
// Default: Default enumerated value
// SqlLen: Maximum length of any enumerated value when in SQL form.
//
// You must use it like so:
// DEFINE_DTL_ENUM(...) = { "value1", "value2", ... };
// Where there are exactly Num values, in the order of the enum.



template <class T> struct dtl_enum_validate;

template <typename Def>
struct dtl_enum : Def
{
   typedef typename Def::Value ENum;

   dtl_enum( ENum t = Def::default_value ) : value(t) {}

   // direct access to the value
   operator ENum & () { return value; }
   operator const ENum & () const { return value; }

   // switch() has problems with the above operators, use switch(x.get()) instead
   const ENum get() const { return value; }
   ENum& get() { return value; }

   typename const Def::sql_type & str() const
   {
      return Def::sql_values[value];
   }

   bool operator==( ENum t ) const { return value == t; }

   friend bool operator== (dtl_enum const& a, dtl_enum const& b)
   {
      return a.value == b.value;
   }

   // Design notes:
   // I had the choice of:
   // - creating a public get_ptr() function
   // - making these non-member friends
   //
   // Since these are the ONLY functions I want to access
   // the sql_value buffer in ANY way, I have made them friends,
   // which removed the need for get_ptr().
   //
   // Then, there was no point putting them outside the class,
   // as it would bloat the header file with extra useless template<> bits and pieces.
   friend BoundIO operator== (BoundIO & b, dtl_enum & x) { return dtl_enum_bind(b,x) == x.sql_value; }
   friend BoundIO operator>> (BoundIO & b, dtl_enum & x) { return dtl_enum_bind(b,x) >> x.sql_value; }
   friend BoundIO operator<< (BoundIO & b, dtl_enum & x) { return dtl_enum_bind(b,x) << x.sql_value; }

private:
   // to keep things private, we need to make some friends.
   // this is intrusive.
   friend struct dtl_enum_validate<Def>;

   // storage for the DTL binding, it must be updated pre-insert.
   typename Def::sql_type sql_value;
   ENum value;
};

// Non-intrusive functions to allow binding of this class in DTL
// These are kinda of intrusive because they are friends.  They may
// as well be members.
//
// I want these to be friends because I don't want to expose the
// DTL buffer via the public interface.  These are the only functions
// that should have access to the buffer.

template <class T> struct dtl_enum_validate
{

	static bool ins_validate(BoundIOs & b, BoundIOs::UserValidate *val)
	{
      BYTE *offset = val->src_class;
      offset += reinterpret_cast<size_t>(b.GetWorkingAddr());
      dtl_enum<T> *px = reinterpret_cast<dtl_enum<T> *>(offset);
      if ( px->value < 0 and px->value >= dtl_enum<T>::num ) return false;
      px->sql_value = dtl_enum<T>::sql_values[px->value];
      return true;
	}

	static bool sel_validate(BoundIOs & b, BoundIOs::UserValidate *val)
	{
      BYTE *offset = val->src_class;
      offset += reinterpret_cast<size_t>(b.GetWorkingAddr());
      dtl_enum<T> *px = reinterpret_cast<dtl_enum<T> *>(offset);

      // converts the sql-string into the bitset
      // linear search through the (hopefully short) list
      // Future: cache it during dtl::SelVal and operator=()
      typedef typename dtl_enum<T>::ENum ENum;
      ENum v = static_cast<ENum>(
            std::distance(&dtl_enum<T>::sql_values[0],
               std::find(&dtl_enum<T>::sql_values[0],&dtl_enum<T>::sql_values[dtl_enum<T>::num], px->sql_value)));
      // invalid?
      if (v == dtl_enum<T>::num) return false;
      px->value = v;
	   return true;
	}
};


template <class T> BoundIO &dtl_enum_bind(BoundIO & b, dtl_enum<T> & x)
{
   STD_::vector<tstring> cols;
   cols.push_back(b.GetName());
   std::ptrdiff_t offset = reinterpret_cast<BYTE*>(&x) - b.GetBoundIOsPtr()->GetWorkingAddr();
   BoundIOs::UserValidate uv(reinterpret_cast<BYTE*>(offset), dtl_enum_validate<T>::ins_validate, dtl_enum_validate<T>::sel_validate, cols);
   b.GetBoundIOsPtr()->AddUserValidate(uv);
   return b;
}



#define DEFINE_DTL_ENUM(Name,Num,Default,SqlLen) \
   struct Name ## _Def \
   { \
      typedef Name ## _E Value; \
      static const int num = Num; \
      static const Value default_value = Default; \
      static const size_t sql_len = SqlLen; \
      typedef dtl::tcstring<sql_len> sql_type; \
      static const sql_type sql_values[num]; \
   }; \
   typedef dtl::dtl_enum<Name ## _Def> Name

#define INIT_DTL_ENUM(Name) const Name ## _Def::sql_type Name ## _Def::sql_values[Name ## _Def::num]

END_DTL_NAMESPACE

#endif
