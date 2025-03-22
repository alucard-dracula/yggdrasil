// -*- C++ -*-
// iterator used for SELECTS
// nested class of DBView
// Initial: 4/10/03 - CJ
/* Copyright � 2003
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

#ifndef DTL_RANDOM_SELECT_ITERATOR_H
#define DTL_RANDOM_SELECT_ITERATOR_H

#include "select_iterator.h"
#include "dtl_algo.h"
#include <assert.h>

BEGIN_DTL_NAMESPACE

// implementation for random access SELECT
template<class DataObj, class ParamObj>
  class DB_random_select_iterator_buffer
{
private:

	typedef DB_select_iterator<DataObj, ParamObj> SelectIterator;
	SelectIterator read_it;
	size_t NumColumns;
	// we must pad the buffer which will hold the fetched DataObj's
	// with extra bytes if necessary
	size_t row_size;
	enum {DEFAULT_BUFFER_SIZE = 64};
	size_t buffer_size;  // how many rows to fetch at a time
	STD_::vector<BYTE> buffer;
	SQLINTEGER base_row, rows_in_buffer, last_row;
	bool constructed;



	void construct_buffer(DataObj &tmp) {
		assert(!constructed);
		// do in place construction of the DataObj's to be held
		// in the buffer
		for (size_t i = 0; i < buffer_size; ++i)
		{
			DataObj *ptr = reinterpret_cast<DataObj *>(&*buffer.begin() + i * row_size);
			init_row_ptr<DataObj>(tmp, ptr);
		}
		constructed = true;
	}

	void destroy_buffer() {
		assert(constructed);
		// do in place destruction of all DataObj's
		// held in the buffer
		for (size_t j = 0; j < buffer_size; ++j)
		{
			DataObj *ptr = reinterpret_cast<DataObj *>(&*buffer.begin() + j * row_size);
			ptr->~DataObj();
		}

		constructed = false;
	}

	void fetch(SQLINTEGER row) {
		DataObj tmp(read_it.GetDataObj());

		if (base_row > 0 || constructed)
			destroy_buffer();

		construct_buffer(tmp);
		base_row = ((int)((row-1)/buffer_size)) * buffer_size + 1;

		DataObj *buf_begin = reinterpret_cast<DataObj *>(&*buffer.begin());
		// DataObj *buf_end = reinterpret_cast<DataObj *>(&*buffer.end());
		// Have to be careful, compilers like MSVC 2005 will freak if they think we are accessing *end()
		DataObj *buf_end = reinterpret_cast<DataObj *>(&*buffer.begin() + buffer.size() + 1);

		rows_in_buffer = 0;

		// bulk read rows, perform SelValidate()
		read_it.bulk_copy(buf_begin, buf_end, true, row_size, base_row);
		rows_in_buffer = read_it.GetLastCount();
	}

public:
	DB_random_select_iterator_buffer(const SelectIterator &sel_it) :
		read_it(sel_it),
		NumColumns(read_it.GetBoundIOs().NumColumns()),
		row_size(DTL_MAX(get_row_size(read_it.GetDataObj()), DTL_MAX(sizeof(SQLINTEGER) * NumColumns, sizeof(TIMESTAMP_STRUCT) * NumColumns))),
		buffer_size(DEFAULT_BUFFER_SIZE),
		buffer(),
		base_row(0),
		rows_in_buffer(0),
		last_row(0),
		constructed(false)
	{
		buffer.resize(row_size * buffer_size);
	}

	~DB_random_select_iterator_buffer() {
		if (constructed)
			destroy_buffer();
	}

	SelectIterator &GetSelectIterator() {
		return read_it;
	}

	SQLINTEGER get_lastrow() {
		DataObj tmp(read_it.GetDataObj());

		if (last_row > 0)
			return last_row;

		if (base_row > 0 || constructed)
			destroy_buffer();

		construct_buffer(tmp);

		DataObj *buf_begin = reinterpret_cast<DataObj *>(&*buffer.begin());
		// DataObj *buf_end = reinterpret_cast<DataObj *>(&*buffer.end());
		// Have to be careful, compilers like MSVC 2005 will freak if they think we are accessing *end()
		DataObj *buf_end = reinterpret_cast<DataObj *>(&*buffer.begin() + buffer.size() + 1);


		rows_in_buffer = 0;

		// Read the last set of rows
		read_it.bulk_copy(buf_begin, buf_end, true, row_size, SelectIterator::BULK_END_POSITION);
		rows_in_buffer = read_it.GetLastCount();
		base_row = read_it.GetCount() - read_it.GetLastCount() + 1;
		last_row = DTL_MAX<SQLINTEGER>(read_it.GetCount(), base_row);

		return last_row;
	}


	const DataObj *get_row(SQLINTEGER row, bool &at_end)  {
		if (base_row > 0 && row >= base_row && row < base_row + (SQLINTEGER)buffer_size) {
			if (row >= base_row + rows_in_buffer) { //past end of data in buffer
				at_end = true;
				// just return first element from buffer
				// (which may only have default constructed data)
				const DataObj *copy_me =
							(reinterpret_cast<const DataObj *>(
							  &*buffer.begin()
							));
				return copy_me;
			}

			at_end = false;

			SQLINTEGER tmp_idx = row - base_row;
			DataObj *copy_me =
							(reinterpret_cast<DataObj *>(
							  &*buffer.begin() + tmp_idx * row_size
							));
			return copy_me;
		}


		fetch(row);
		return get_row(row, at_end);
	}

	const DBView<DataObj, ParamObj> &GetView() const
	{
	  return read_it.GetView();
	}

	// Empty buffer, close existing resultset.
	// Force requery of database to retrieve new results
	void Reset() {
		read_it.GetStmt().Reset();
		DataObj tmp(read_it.GetDataObj());
		if (constructed)
			destroy_buffer();
		construct_buffer(tmp);
		base_row = 0;
		rows_in_buffer = 0;
		last_row = 0;
	}

};

// random access SELECT iterator
template<class DataObj, class ParamObj = DefaultParamObj<DataObj> >
  class DB_random_select_iterator :
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
	typedef typename DB_random_select_iterator<DataObj, ParamObj>::difference_type this_difference;
#endif

private:

	typedef DB_select_iterator<DataObj, ParamObj> SelectIterator;
	typedef DB_random_select_iterator<DataObj, ParamObj> this_iterator;
	typedef DB_random_select_iterator_buffer<DataObj, ParamObj> this_buffer;
	// buffer declared mutable since read operations that do not change the location
	// of the iterator may result in changes to the buffer due to row repositioning
	mutable CountedPtr<this_buffer> pselect_buffer;
	SQLINTEGER row;

public:
	DB_random_select_iterator(const DB_random_select_iterator<DataObj, ParamObj> &other) :
		pselect_buffer(other.pselect_buffer),
		row(other.row)
	{}


	DB_random_select_iterator(const SelectIterator &sel_it, const CountedPtr<this_buffer> &buffer) :
	  pselect_buffer(buffer),
	  row(1)
	{
	  // if the select iterator points to end(),
	  // then point the random access iterator to the end of the resultset
	  if (sel_it.at_end())
		  row = pselect_buffer->get_lastrow();
	  // else
	  //	default is for iterator to point to beginning of recordset

	}

	DB_random_select_iterator(const DBView<DataObj, ParamObj> &view, const CountedPtr<this_buffer> &buffer) :
	  pselect_buffer(buffer),
	  row(1)
	{
		  //default is for iterator to point to beginning of recordset
	}

	DB_random_select_iterator(void) :
	  pselect_buffer(NULL),
	  row(1)
	{
		  //default is for iterator to point to beginning of recordset
	}

	DB_random_select_iterator(CountedPtr<this_buffer> &psb,
		SQLINTEGER row_no) : pselect_buffer(psb), row(row_no)
	{}

	~DB_random_select_iterator()
	{}

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
   const DataObj &get_value() const
   {
	   if (row < 1)
		   DTL_THROW DBException(_TEXT("DB_random_select_iterator::get_value()"),
				_TEXT("Attempt to access element past end of iterator range!"), NULL, NULL);

	   bool at_end;
	   const DataObj *pd = pselect_buffer->get_row(row, at_end);
	   if (at_end)
		   DTL_THROW DBException(_TEXT("DB_random_select_iterator::get_value()"),
				_TEXT("Attempt to access element past end of iterator range!"), NULL, NULL);

	   return *pd;

   }

public:

   const DataObj &operator*() const
   {
		return get_value();
   }

   const DataObj *operator->() const
   {
	   bool at_end;
	   return (pselect_buffer->get_row(row, at_end));
   }

   const DataObj &operator[](SQLINTEGER i) const {
	 this_iterator tmp(*this);
	 tmp += i;
	 return tmp.get_value();
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


template <class DataObj, class ParamObj> DB_random_select_iterator<DataObj, ParamObj>
operator+
(const DB_random_select_iterator<DataObj, ParamObj> &it, typename DB_random_select_iterator<DataObj, ParamObj>::difference_type d) {
	DB_random_select_iterator<DataObj, ParamObj> tmp(it);
	return tmp += d;
}


template <class DataObj, class ParamObj> DB_random_select_iterator<DataObj, ParamObj>
operator+
(typename DB_random_select_iterator<DataObj, ParamObj>::difference_type d, const DB_random_select_iterator<DataObj, ParamObj> &it) {
	DB_random_select_iterator<DataObj, ParamObj> tmp(it);
	return tmp += d;
}


template <class DataObj, class ParamObj> DB_random_select_iterator<DataObj, ParamObj>
operator-
(const DB_random_select_iterator<DataObj, ParamObj> &it, typename DB_random_select_iterator<DataObj, ParamObj>::difference_type d) {
	DB_random_select_iterator<DataObj, ParamObj> tmp(it);
	return tmp -= d;
}

template <class DataObj, class ParamObj> typename DB_random_select_iterator<DataObj, ParamObj>::difference_type
operator-
(const DB_random_select_iterator<DataObj, ParamObj> &lhs, const DB_random_select_iterator<DataObj, ParamObj> &rhs) {
	return rhs.distance(lhs);
}

template <class DataObj, class ParamObj>
inline bool
operator<(const DB_random_select_iterator<DataObj, ParamObj> &lhs, const DB_random_select_iterator<DataObj, ParamObj> &rhs) {
	return lhs - rhs < 0;
}

template <class DataObj, class ParamObj>
inline bool
operator<=(const DB_random_select_iterator<DataObj, ParamObj> &lhs, const DB_random_select_iterator<DataObj, ParamObj> &rhs) {
	return lhs - rhs <= 0;
}

template <class DataObj, class ParamObj>
inline bool
operator>(const DB_random_select_iterator<DataObj, ParamObj> &lhs, const DB_random_select_iterator<DataObj, ParamObj> &rhs) {
	return lhs - rhs > 0;
}

template <class DataObj, class ParamObj>
inline bool
operator>=(const DB_random_select_iterator<DataObj, ParamObj> &lhs, const DB_random_select_iterator<DataObj, ParamObj> &rhs) {
	return lhs - rhs >= 0;
}


END_DTL_NAMESPACE


#endif //DTL_SELECT_RANDOM_ACCESS_ITERATOR
