// -*- C++ -*-
// iterator used for random access SELECT update
// nested class of DBView
// Initial: 4/14/03
/* Copyright ?2003
Michael Gradman and Corwin Joy

Permission to use, copy, modify, distribute and sell this software and
its documentation for any purpose is hereby granted without fee, provided
that the above copyright notice appears in all copies and that both that
copyright notice and this permission notice appear in supporting documentation.
Corwin Joy and Michael Gradman make no representations about the suitability
of this software for any purpose.
It is provided "as is" without express or implied warranty.
*/


#ifndef DTL_RANDOM_SELECT_UPDATE_ITERATOR_H
#define DTL_RANDOM_SELECT_UPDATE_ITERATOR_H
#include "random_select_iterator.h"
#include "select_update_iterator.h"

BEGIN_DTL_NAMESPACE

// random access SELECT UPDATE iterator
template<class DataObj, class ParamObj = DefaultParamObj<DataObj> >
  class DB_random_select_update_iterator :
#ifdef __SGI_STL_PORT
  // STLport cannot properly recognize the standard iterator tag when the compiler does not support partial specilization
  public STD_::random_access_iterator<DataObj, std::ptrdiff_t>
#else
  #ifdef _MSC_VER
    // MSVC version 6 library not standard compliant
    public STD_::iterator<STD_::random_access_iterator_tag, DataObj>
  #else
    // use iterator definition in C++ standard
    public STD_::iterator<STD_::random_access_iterator_tag, DataObj, std::ptrdiff_t, const DataObj *, const DataObj &>
  #endif
#endif
{
private:
	typedef DB_select_iterator<DataObj, ParamObj> SelectIterator;
	typedef DB_random_select_update_iterator<DataObj, ParamObj> this_iterator;
	typedef DB_random_select_iterator_buffer<DataObj, ParamObj> this_buffer;
	// buffer declared mutable since read operations that do not change the location
	// of the iterator may result in changes to the buffer due to row repositioning
	mutable CountedPtr<this_buffer> pselect_buffer;
	SQLINTEGER row;
	CountedPtr<DB_replacer<DataObj, ParamObj> > preplacer;

public:

#if defined(__SGI_STL_PORT) || defined(_MSC_VER)
	// override the reference type to show it is const
	typedef const DataObj &   reference;
#endif


#if defined (__hpux) || defined(_MSC_VER) || defined (__BORLANDC__)
	// MSVC version 6 library not standard compliant
	typedef std::ptrdiff_t difference_type;
	typedef std::ptrdiff_t this_difference;
#else
	// piece of junk gcc requires that we label the inherited difference type with a
	// typname label to avoid "difference_type is implicitly a typename" warning
	typedef typename DB_random_select_update_iterator<DataObj, ParamObj>::difference_type this_difference;
#endif

	DB_random_select_update_iterator(const DB_random_select_update_iterator<DataObj, ParamObj> &other) :
	    pselect_buffer(other.pselect_buffer),
		row(other.row),
		preplacer(other.preplacer)
	{}


	DB_random_select_update_iterator(const SelectIterator &sel_it, const CountedPtr<this_buffer> &buffer) :
	  pselect_buffer(buffer),
	  row(1),
	  preplacer(NULL)
	{
	  // if the select iterator points to end(),
	  // then point the random access iterator to the end of the resultset
	  if (sel_it.at_end())
		  row = pselect_buffer->get_lastrow();
	  // else
	  //	default is for iterator to point to beginning of recordset

	}

	 DB_random_select_update_iterator(const DBView<DataObj, ParamObj> &view, const CountedPtr<this_buffer> &buffer) :
	  pselect_buffer(buffer),
	  row(1),
	  preplacer(NULL)
	{
		  //default is for iterator to point to beginning of recordset
	}

	DB_random_select_update_iterator(void) :
	  pselect_buffer(NULL),
	  row(1),
	  preplacer(NULL)
	{
		  //default is for iterator to point to beginning of recordset
	}

	~DB_random_select_update_iterator()
	{}

	// exception-safe swap()
	void swap(DB_random_select_update_iterator<DataObj, ParamObj> &other)
	{
		STD_::swap(row, other.row);
		STD_::swap(pselect_buffer, other.pselect_buffer);
		preplacer.swap(other.preplacer);
	}

	// exception-safe assignment
	DB_random_select_update_iterator<DataObj, ParamObj> &
		operator=(const DB_random_select_update_iterator<DataObj, ParamObj> &other)
	{
		if (this != &other)
		{
			DB_random_select_update_iterator<DataObj, ParamObj> temp(other);
			swap(temp);
		}

		return *this;
	}

	// Hand out select iterator used by buffer in case users need to set or
	// get the properties
	SelectIterator &GetSelectIterator() {
		return pselect_buffer->GetSelectIterator();
	}

	// postincrement
	this_iterator &operator++()
	{
		++row;
		return *this;
	}

	// advance to next record (postincrement)
   inline const this_iterator operator++(int)
   {
	this_iterator tmp(*this);
	++(*this);
	return tmp;
   }

   this_iterator &operator--()
   {
	  --row;
	 return *this;
   }

   inline const this_iterator operator--(int)
   {
	 this_iterator tmp(*this);
	 --(*this);
	 return tmp;
   }


   this_iterator& operator+=(this_difference n) {
        row += n;
        return *this;
   }

   this_iterator& operator-=(this_difference n) {
        row -= n;
        return *this;
   }

   // Moved from global scope to avoid ambiguity with the operator-() which
   // subtracts iterators from one another.
   this_iterator operator-(this_difference x) const
   { this_iterator result(*this); return result -= x; }

private:
   const DataObj &get_reference() const
   {
	   if (row < 1)
		   DTL_THROW DBException(_TEXT("DB_random_select_update_iterator::get_reference()"),
				_TEXT("Attempt to access element past end of iterator range!"), NULL, NULL);

	   bool at_end;
	   const DataObj *pd = pselect_buffer->get_row(row, at_end);
	   if (at_end)
		   DTL_THROW DBException(_TEXT("DB_random_select_update_iterator::get_reference()"),
				_TEXT("Attempt to access element past end of iterator range!"), NULL, NULL);

	   return *pd;

   }

public:

	DataObj GetDataObj() const
	{
		return get_reference();
	}

	// When converting iterator to value, return actual reference to underlying buffer
	// to avoid problems with lifetime of temporaries.
	operator const DataObj &() const
	{
		return get_reference();
	}

	operator DB_random_select_iterator<DataObj, ParamObj>() const
	{
		return DB_random_select_iterator<DataObj, ParamObj>(pselect_buffer, row);
	}


    this_iterator &operator*()
    {
	   return *this;
    }

	const this_iterator &operator*() const
    {
	   return *this;
    }

	this_iterator &
		operator=(const DataObj &data)
	{
		if (preplacer.get() == NULL)
			preplacer = new DB_replacer<DataObj, ParamObj>(pselect_buffer->GetView());
		preplacer->replace(**this, data);
		return *this;
	}

	void SetKey(const tstring &KeyFields) {
		if (preplacer.get() == NULL)
			preplacer = new DB_replacer<DataObj, ParamObj>(pselect_buffer->GetView());

		preplacer->SetKey(KeyFields);
	}

   // NOTE!  This returns only a const DataObj *
   // Why? Because if you attempt a modification of
   // the data via the operator the iterator would not
   // detect it.
   const DataObj *operator->() const
   {
	   bool at_end;
	   return (pselect_buffer->get_row(row, at_end));
   }


   const DataObj & operator[](SQLINTEGER i) const {
	 this_iterator tmp(*this);
	 tmp += i;
	 return tmp.get_reference();
   }

   this_difference distance(const this_iterator &other) const
   {
		return other.row - row;
   }


   bool operator==(const this_iterator &other) const {
	   return row==other.row && pselect_buffer == other.pselect_buffer;
   }

   bool operator!=(const this_iterator &other) const {
	   return !(*this == other);
   }

   // comparison versus view.end()
   bool operator==(const SelectIterator &other) const {
	   if (other.GetView() != pselect_buffer->GetView() || other.at_end() != true) {
		   return false;
	   }

	   // only way to find out if we are at the end is attempt to grab data
	   bool at_end;
	   pselect_buffer->get_row(row, at_end);
	   return at_end;
   }

   bool operator!=(const SelectIterator &other) const {
	   return !(*this == other);
   }

};


template <class DataObj, class ParamObj> DB_random_select_update_iterator<DataObj, ParamObj>
operator+
(const DB_random_select_update_iterator<DataObj, ParamObj> &it, typename DB_random_select_update_iterator<DataObj, ParamObj>::difference_type d) {
	DB_random_select_update_iterator<DataObj, ParamObj> tmp(it);
	return tmp += d;
}


template <class DataObj, class ParamObj> DB_random_select_update_iterator<DataObj, ParamObj>
operator+
(typename DB_random_select_update_iterator<DataObj, ParamObj>::difference_type d, const DB_random_select_update_iterator<DataObj, ParamObj> &it) {
	DB_random_select_update_iterator<DataObj, ParamObj> tmp(it);
	return tmp += d;
}


template <class DataObj, class ParamObj> DB_random_select_update_iterator<DataObj, ParamObj>
operator-
(const DB_random_select_update_iterator<DataObj, ParamObj> &it, typename DB_random_select_update_iterator<DataObj, ParamObj>::difference_type d) {
	DB_random_select_update_iterator<DataObj, ParamObj> tmp(it);
	return tmp -= d;
}

template <class DataObj, class ParamObj> typename DB_random_select_update_iterator<DataObj, ParamObj>::difference_type
operator-
(const DB_random_select_update_iterator<DataObj, ParamObj> &lhs, const DB_random_select_update_iterator<DataObj, ParamObj> &rhs) {
	return rhs.distance(lhs);
}

template <class DataObj, class ParamObj>
inline bool
operator<(const DB_random_select_update_iterator<DataObj, ParamObj> &lhs, const DB_random_select_update_iterator<DataObj, ParamObj> &rhs) {
	return lhs - rhs < 0;
}

template <class DataObj, class ParamObj>
inline bool
operator<=(const DB_random_select_update_iterator<DataObj, ParamObj> &lhs, const DB_random_select_update_iterator<DataObj, ParamObj> &rhs) {
	return lhs - rhs <= 0;
}

template <class DataObj, class ParamObj>
inline bool
operator>(const DB_random_select_update_iterator<DataObj, ParamObj> &lhs, const DB_random_select_update_iterator<DataObj, ParamObj> &rhs) {
	return lhs - rhs > 0;
}

template <class DataObj, class ParamObj>
inline bool
operator>=(const DB_random_select_update_iterator<DataObj, ParamObj> &lhs, const DB_random_select_update_iterator<DataObj, ParamObj> &rhs) {
	return lhs - rhs >= 0;
}


END_DTL_NAMESPACE

#endif //DTL_RANDOM_SELECT_UPDATE_ITERATOR_H
