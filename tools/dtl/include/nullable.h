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
 *
 * Most of the code was derived from boost.org's boost/optional.hpp
 * I wish I could just #include <boost/optional.hpp>, maybe I can
 * figure out how to adapt optional<> in the future.
 *
 * Here is its License:
 *
 * Copyright (C) 2003, Fernando Luis Cacciola Carballal.
 *
 * Use, modification, and distribution is subject to the Boost Software
 * License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/lib/optional for documentation.
 *
 * You are welcome to contact the author at:
 *  fernando_cacciola@hotmail.com
 */


// nullable<> is used to provide a generic, DTL-compatible NULL type.
// You can bind it using the standard <<, >>, == DTL binders.
//
// Initial: 18/11/2004 - PH
// Rewritten to use DTL user validate feature: 26/11/2004 - CJ

#ifndef DTL_NULLABLE_H
#define DTL_NULLABLE_H

#include "dtl_config.h"
#include "BoundIO.h"

#include "std_warn_off.h"
#include <utility>
#include "std_warn_on.h"

// There are some neat things that can be done with boost.
// If you have it, uncomment this:

// #define DTL_HAVE_BOOST

#ifdef DTL_HAVE_BOOST
#include <boost/call_traits.hpp>
#include <boost/optional.hpp>

#include <boost/version.hpp>
#if BOOST_VERSION == 103000
   // There is no 'none'.
#elif BOOST_VERSION < 103200
   #include <boost/detail/none.hpp>
#else
   #include <boost/none.hpp>
#endif

#endif

BEGIN_DTL_NAMESPACE

/* Usage:
 * Just wrap your basic type in a nullable template.
 * This template will then hold values that can optionally be NULL.
 * So, e.g. nullable<double> holds a double value that may be NULL.
 *
 * To test if a given value is NULL use the isNull() method.
 * See tests/test_nullable.cpp for an example.
 */


/* Running Example in Notes
 * ========================
 *
 * struct nulltest
 * {
 *    nullable<double> value;
 * };
 *
 * struct DefaultBCA<nulltest>
 * {
 *    void operator()(dtl::BoundIOs & boundIOs, nulltest & rowbuf)
 *    {
 *       boundIOs["value"] == rowbuf.value;
 *    }
 * };
 *
 * Valitator examples are below.
 */


/* Design Notes
 * ============
 *
 * - nullable<> was born from my attempts to use boost::optional<T>
 *   with the DTL.  It is a heavily decorated pair<bool,T> which has
 *   two states:
 *   NULL:    (false,T())
 *   NOTNULL: (true,value)
 *
 * - nullable<> is basically the same as boost::optional<>
 *   A use-example:
 *   nullable<double> val;
 *   // Is val null?
 *   if (val) cout << "Yes" << endl;
 *   // Set to a value
 *   val = 23.0;
 *   // print if not null
 *   if (!val) cout << *val << endl;
 *
 * - To load nullable<>, I make the DTL request 2 columns:
 *   SELECT NOT ISNULL(column), column ...
 *   The first column is written to nullable<>::first, the
 *   second to second.  This initialises nullable properly.
 *
 * - To write a nullable<>, the Insert Validator must call
 *   nullable_insval() for each nullable bound value.
 *   Then the DTL is ONLY BOUND to the second value (ie not the
 *   first 'null flag' value).
 *
 * - The first and second members are private, the only
 *   way to externally update the members directly is via
 *   the operator<<,>>,== friends that write to BoundIO.
 *
 * - I have made T reset to its default-constructed value because:
 *    1. It would allow users to release any funky resources.
 *    2. I can't (yet) destroy the instance like optional<>, so
 *       it has to be something.
 *    3. DTL bounded types must be default-constructable now anyway.
 *
 * - It is assumed types are either PODs or very simple types.
 *   This is a valid assumption if this is used for binding ODBC types.
 */

template <typename T>
struct nullable : private std::pair<bool,T>
{
private:
   typedef std::pair<bool,T> _Base;

public:
   // types
   typedef nullable<T>  this_type;

   typedef T          value_type;
   typedef T &        reference_type;
   typedef T const &  reference_const_type;
   typedef T *        pointer_type;
   typedef T const *  pointer_const_type;

#ifdef DTL_HAVE_BOOST
   typedef typename boost::call_traits<T>::param_type argument_type;
#else
   typedef T const &  argument_type;
#endif

   // constructors
   nullable () : _Base(false,T()) {}
   nullable ( argument_type val ) : _Base(true,val) {}
   nullable ( nullable const& val ) : _Base(val.first,val.second) {}

#if defined(DTL_HAVE_BOOST) && (BOOST_VERSION > 103000)
   nullable ( boost::none_t const& ) : _Base(false,T()) {}

   nullable& operator= ( boost::none_t const& )
   {
      this->first = false;
      this->second = T();
      return *this;
   }
#endif

   // assignments
   nullable& operator= ( argument_type val )
   {
      this->first = true;
      this->second = val;
      return *this;
   }

   nullable& operator= ( nullable const& rhs )
   {
      this->first = rhs.first;
      this->second = rhs.second;
      return *this;
   }

   // accessors
   //
   // NOTE: these will always return a pointer, when they
   // should not.  Ideally, if this is NULL, it should
   // return a NULL pointer.
   //
   // BE CAREFUL.

   reference_const_type get()        const { return this->second;  }
   reference_type       get()              { return this->second;  }
   pointer_const_type   operator->() const { return &this->second; }
   pointer_type         operator->()       { return &this->second; }
   reference_const_type operator*()  const { return this->second;  }
   reference_type       operator*()        { return this->second;  }
   pointer_const_type   get_ptr()    const { return &this->second; }
   pointer_type         get_ptr()          { return &this->second; }

   // a more stl/boost-like accessor (they follow standard containers
   // and boost::optional)
   bool empty() const { return this->first == false; }
   void reset() { this->first = false; }
   void reset( argument_type val ) { *this = val; }

   // test & set NULL
   bool IsNull() const { return empty(); }

   // Note: this is mostly for the validations
   void SetNull() { reset(); }
   void ClearNull() { this->first = true; }

   // for boost::equal_pointees()
   bool operator!() const { return empty(); }
};

template <class T> inline tostream &operator<<(tostream &o, const nullable<T> &t)
{
	if (t.IsNull())
		o << _TEXT("NULL");
	else
		o << t.get();
	return o;
}

// Non-intrusive functions to allow binding of this class in DTL

template <class T> struct nullable_validate
{

	static bool ins_validate(BoundIOs &bios, BoundIOs::UserValidate *val)
	{
      BYTE *offset = val->src_class;
      offset += reinterpret_cast<size_t>(bios.GetWorkingAddr());
      nullable<T> *px = reinterpret_cast<nullable<T> *>(offset);

	   // N.B.!! For insert we must not assume this column will be found.
	   // In many cases the parameters used on e.g. an update will
	   // only be a subset of the full set of columns.
	   //
	   // Also, this routine must be robust enough to allow it to be called
	   // multiple times for the same class.
	   BoundIOs::iterator it = bios.find(val->bios[0]);
	   if (it != bios.end())
	   {
		   BoundIO &b = it->second;

		   if (px->IsNull())
		   {
			   b.SetNull();
		   }
		   else
		   {
			   b.ClearNull();
		   }
	   }

	   return true;
	}

	static bool sel_validate(BoundIOs &bios, BoundIOs::UserValidate *val)
	{
      BYTE *offset = val->src_class;
      offset += reinterpret_cast<size_t>(bios.GetWorkingAddr());
      nullable<T> *px = reinterpret_cast<nullable<T> *>(offset);

	   BoundIO &b = bios[val->bios[0]];

	   if (b.IsNull())
	   {
		   px->SetNull();
	   }
	   else
	   {
		   px->ClearNull();
	   }

	   return true;
	}
};


template <class T> BoundIO &nullable_bind(BoundIO & b, nullable<T> & x)
{
   STD_::vector<tstring> cols;
   cols.push_back(b.GetName());
   std::ptrdiff_t offset = reinterpret_cast<BYTE*>(&x) - b.GetBoundIOsPtr()->GetWorkingAddr();
   BoundIOs::UserValidate uv(reinterpret_cast<BYTE*>(offset), nullable_validate<T>::ins_validate, nullable_validate<T>::sel_validate, cols);
   b.GetBoundIOsPtr()->AddUserValidate(uv);
   b.SetNullable(true); // set the nullable flag to fix DefaultSelValidate
   return b;
}

template <class T> BoundIO operator == (BoundIO & b, nullable<T> & x)
{
  return nullable_bind(b,x) == *(x.get_ptr());
}

template <class T> BoundIO operator >> (BoundIO & b, nullable<T> & x)
{
  return nullable_bind(b,x) >> *(x.get_ptr());
}

template <class T> BoundIO operator << (BoundIO & b, nullable<T> & x)
{
  return nullable_bind(b,x) << *(x.get_ptr());
}



// copied near-exactly from optional (just changed typenames)
//
// nullable's relational operators ( ==, !=, <, >, <=, >= )
// have deep-semantics (compare values).
//
// WARNING: This is UNLIKE pointers.
// Use equal_pointees()/less_pointess() in generic code instead.

#ifdef DTL_HAVE_BOOST
template<class T>
inline
bool operator == ( nullable<T> const& x, nullable<T> const& y )
{ return boost::equal_pointees(x,y); }

template<class T>
inline
bool operator < ( nullable<T> const& x, nullable<T> const& y )
{ return boost::less_pointees(x,y); }
#else
template<class T>
inline
bool operator == ( nullable<T> const& x, nullable<T> const& y )
{
   // copied from boost::equal_pointees()
   return (!x) != (!y) ? false : ( !x ? true : (*x) == (*y) ) ;
}

template<class T>
inline
bool operator < ( nullable<T> const& x, nullable<T> const& y )
{
   // copied from boost::less_pointees()
  return !y ? false : ( !x ? true : (*x) < (*y) ) ;
}
#endif

template<class T>
inline
bool operator != ( nullable<T> const& x, nullable<T> const& y )
{ return !( x == y ); }

template<class T>
inline
bool operator > ( nullable<T> const& x, nullable<T> const& y )
{ return y < x; }

template<class T>
inline
bool operator <= ( nullable<T> const& x, nullable<T> const& y )
{ return !( y < x ); }

template<class T>
inline
bool operator >= ( nullable<T> const& x, nullable<T> const& y )
{ return !( x < y ); }

#if defined(DTL_HAVE_BOOST) && (BOOST_VERSION > 103000)
template<class T>
inline
bool operator == ( nullable<T> const& x, boost::none_t const& )
{ return boost::equal_pointees(x, nullable<T>() ); }

template<class T>
inline
bool operator < ( nullable<T> const& x, boost::none_t const& )
{ return boost::less_pointees(x,nullable<T>() ); }

template<class T>
inline
bool operator != ( nullable<T> const& x, boost::none_t const& y )
{ return !( x == y ); }

template<class T>
inline
bool operator > ( nullable<T> const& x, boost::none_t const& y )
{ return y < x; }

template<class T>
inline
bool operator <= ( nullable<T> const& x, boost::none_t const& y )
{ return !( y < x ); }

template<class T>
inline
bool operator >= ( nullable<T> const& x, boost::none_t const& y )
{ return !( x < y ); }

template<class T>
inline
bool operator == ( boost::none_t const& x, nullable<T> const& y )
{ return boost::equal_pointees(nullable<T>() ,y); }

template<class T>
inline
bool operator < ( boost::none_t const& x, nullable<T> const& y )
{ return boost::less_pointees(nullable<T>() ,y); }

template<class T>
inline
bool operator != ( boost::none_t const& x, nullable<T> const& y )
{ return !( x == y ); }

template<class T>
inline
bool operator > ( boost::none_t const& x, nullable<T> const& y )
{ return y < x; }

template<class T>
inline
bool operator <= ( boost::none_t const& x, nullable<T> const& y )
{ return !( y < x ); }

template<class T>
inline
bool operator >= ( boost::none_t const& x, nullable<T> const& y )
{ return !( x < y ); }

#endif   // end of #define DTL_HAVE_BOOST


template<class T>
   inline typename nullable<T>::reference_const_type
   get ( nullable<T> const& nbl ) { return nbl.get(); }

template<class T>
   inline typename nullable<T>::reference_type
   get ( nullable<T> & nbl ) { return nbl.get(); }

template<class T>
   inline typename nullable<T>::pointer_const_type
   get ( nullable<T> const* nbl ) { return nbl->get_ptr(); }

template<class T>
   inline typename nullable<T>::pointer_type
   get ( nullable<T>* nbl ) { return nbl->get_ptr(); }

template<class T>
   inline typename nullable<T>::pointer_const_type
   get_pointer ( nullable<T> const& nbl ) { nbl.get_ptr(); }

template<class T>
   inline typename nullable<T>::pointer_type
   get_pointer ( nullable<T> & nbl ) { return nbl.get_ptr(); }

template<class T> inline void swap( nullable<T>& x, nullable<T>& y )
{
   if ( !x && !!y )
   {
      x = y;
#if defined(DTL_HAVE_BOOST) && (BOOST_VERSION > 103000)
      y = boost::none;
#else
      y = nullable<T>();
#endif
   }
   else if ( !!x && !y )
   {
      y = x;
#if defined(DTL_HAVE_BOOST) && (BOOST_VERSION > 103000)
      x = boost::none;
#else
      x = nullable<T>();
#endif
   }
   else if ( !!x and !!y )
   {
      using std::swap;
      swap(*x,*y);
   }
}



END_DTL_NAMESPACE

#endif
