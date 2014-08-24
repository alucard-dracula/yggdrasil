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
// Edited: 12/19/2000 - MG - added namespaces

#ifndef DTL_STAR_IT_
#define DTL_STAR_IT_

#include "dtl_config.h"

#include "std_warn_off.h"
#include <iterator>

#ifdef __SGI_STL_PORT
#include <hash_set>
#endif

#include "std_warn_on.h"

BEGIN_DTL_NAMESPACE

// TEMPLATE CLASS star_bidirectional_iterator
template<typename ItType,
	     typename ValType,
         typename RefType  = ValType&,
         typename PtrType  = ValType *,
         typename DistType = std::ptrdiff_t>
class star_bidirectional_iterator :
#ifndef __SGI_STL_PORT
	public STD_::iterator<STD_::bidirectional_iterator_tag,  ValType, DistType>
#elif defined (__SUNPRO_CC)
	public STD_::iterator<STD_::bidirectional_iterator_tag,  ValType, DistType>
#else
	 // not sure why STLport not recognizing the standard iterator tag
	 // when used in algorithms like
     //   copy(_InputIter __first,  _InputIter __last, _OutputIter __result)
	 // use STLport specific alternative
	 public STD_::bidirectional_iterator<ValType, DistType>
#endif
{
protected:
	ItType current;

public:
	typedef star_bidirectional_iterator<ItType,
		ValType, RefType, PtrType, DistType> ThisType;
	typedef ItType iter_type;
	typedef RefType reference_type;
	typedef PtrType pointer_type;
	star_bidirectional_iterator<ItType,ValType, RefType, PtrType, DistType>() {};

	explicit star_bidirectional_iterator<ItType,ValType, RefType, PtrType, DistType>(ItType X)
		: current(X) {};

	// converting constructor ... from one star_it type to another
	template<class OtherBaseIt, class OtherValType, class OtherRefType,
		class OtherPtrType, class OtherDistType>
		star_bidirectional_iterator<ItType,ValType, RefType, PtrType, DistType>(
			const star_bidirectional_iterator<OtherBaseIt, OtherValType, OtherRefType,
			OtherPtrType, OtherDistType> &other) :
			current(other.base()) { }

	// copy constructor
	star_bidirectional_iterator<ItType,ValType, RefType, PtrType, DistType>(
		const star_bidirectional_iterator<ItType,ValType, RefType, PtrType, DistType> &other) :
		current(other.base()) { }

	ItType base() const
		{return (current); }
	RefType operator*() const
		{return **current;}
	PtrType operator->() const
		{return (&**this); }
	ThisType& operator++()
		{++current;
		return (*this); }
	const ThisType operator++(int) // const here to prevent iterator++++ type operations
		{ThisType oldValue(*this); // standard is to return old value
		++current;
		return oldValue;}
	ThisType& operator--()
		{--current;
		return (*this); }
	const ThisType operator--(int)
		{ThisType oldValue(*this);
	    --current;
		return oldValue;}

  bool operator==(const ThisType &other) const
  {
    return base() == other.base();
  }

  bool operator!=(const ThisType &other) const
  {
    return !(*this == other);
  }
};

template<class ItType1, class ValType1, class RefType1, class PtrType1,
	class DistType1,
	class ItType2, class ValType2, class RefType2, class PtrType2,
	class DistType2> inline
	bool operator==(const star_bidirectional_iterator<ItType1,
			ValType1, RefType1, PtrType1, DistType1>& X,
		const star_bidirectional_iterator<ItType2,
			ValType2, RefType2, PtrType2, DistType2>& Y)
	{return (X.base() == Y.base()); }

template<class ItType1, class ValType1, class RefType1, class PtrType1,
	class DistType1,
	class ItType2, class ValType2, class RefType2, class PtrType2,
	class DistType2> inline
	bool operator!=(const star_bidirectional_iterator<ItType1,
			ValType1, RefType1, PtrType1, DistType1>& X,
		const star_bidirectional_iterator<ItType2,
			ValType2, RefType2, PtrType2, DistType2>& Y)
	{return (!(X == Y)); }

// TEMPLATE CLASS star_forward_iterator
// needed for hashed containers as they do not support iterator::operator--()

template<typename ItType,
	typename ValType,
	typename RefType = ValType&,
	typename PtrType = ValType *,
	typename DistType = std::ptrdiff_t>
	class star_forward_iterator :
#ifndef __SGI_STL_PORT
	public STD_::iterator<STD_::forward_iterator_tag,  ValType, DistType>
#elif defined (__SUNPRO_CC)
	public STD_::iterator<STD_::forward_iterator_tag,  ValType, DistType>
#else
	 // not sure why STLport not recognizing the standard iterator tag
	 // when used in algorithms like
     //   copy(_InputIter __first, _InputIter __last, _OutputIter __result)
	 // use STLport specific alternative
	 public STD_::forward_iterator<ValType, DistType>
#endif
{
protected:
	ItType current;

public:
	typedef star_forward_iterator<ItType,
		ValType, RefType, PtrType, DistType> ThisType;
	typedef ItType iter_type;
	typedef RefType reference_type;
	typedef PtrType pointer_type;
	star_forward_iterator<ItType,ValType, RefType, PtrType, DistType>() {};

	explicit star_forward_iterator<ItType,ValType, RefType, PtrType, DistType>(ItType X)
		: current(X) {};

	// converting constructor ... from one star_it type to another
	template<class OtherBaseIt, class OtherValType, class OtherRefType,
		class OtherPtrType, class OtherDistType>
		star_forward_iterator<ItType,ValType, RefType, PtrType, DistType>(
			const star_forward_iterator<OtherBaseIt, OtherValType, OtherRefType,
			OtherPtrType, OtherDistType> &other) :
			current(other.base()) { }

	// copy constructor
	star_forward_iterator<ItType,ValType, RefType, PtrType, DistType>(
		const star_forward_iterator<ItType,ValType, RefType, PtrType, DistType> &other) :
		current(other.base()) { }

	ItType base() const
		{return (current); }
	RefType operator*() const
		{return **current;}
	PtrType operator->() const
		{return (&**this); }
	ThisType& operator++()
		{++current;
		return (*this); }
	const ThisType operator++(int) // const here to prevent iterator++++ type operations
		{ThisType oldValue(*this); // standard is to return old value
		++current;
		return oldValue;}

    bool operator==(const ThisType &other) const
	{
      return base() == other.base();
	}

    bool operator!=(const ThisType &other) const
	{
      return !(*this == other);
	}
};

template<class ItType1, class ValType1, class RefType1, class PtrType1,
	class DistType1,
	class ItType2, class ValType2, class RefType2, class PtrType2,
	class DistType2> inline
	bool operator==(const star_forward_iterator<ItType1,
			ValType1, RefType1, PtrType1, DistType1>& X,
		const star_forward_iterator<ItType2,
			ValType2, RefType2, PtrType2, DistType2>& Y)
	{return (X.base() == Y.base()); }

template<class ItType1, class ValType1, class RefType1, class PtrType1,
	class DistType1,
	class ItType2, class ValType2, class RefType2, class PtrType2,
	class DistType2> inline
	bool operator!=(const star_forward_iterator<ItType1,
			ValType1, RefType1, PtrType1, DistType1>& X,
		const star_forward_iterator<ItType2,
			ValType2, RefType2, PtrType2, DistType2>& Y)
	{return (!(X == Y)); }

END_DTL_NAMESPACE
#endif
