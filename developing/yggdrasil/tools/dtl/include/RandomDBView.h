// -*- C++ -*-
// Version of DBView that allows random access iterators
// Initial: 4/14/03
/* Copyright © 2003 
Michael Gradman and Corwin Joy 

Permission to use, copy, modify, distribute and sell this software and 
its documentation for any purpose is hereby granted without fee, provided 
that the above copyright notice appears in all copies and that both that 
copyright notice and this permission notice appear in supporting documentation. 
Corwin Joy and Michael Gradman make no representations about the suitability 
of this software for any purpose. 
It is provided "as is" without express or implied warranty.
*/ 


#ifndef DTL_RANDOM_DBVIEW_H
#define DTL_RANDOM_DBVIEW_H
#include "random_select_update_iterator.h"

BEGIN_DTL_NAMESPACE
template<class DataObj, class ParamObj = DefaultParamObj<DataObj> >
  class RandomDBView
  
{
private:
	typedef DBView<DataObj, ParamObj> view_type;
	typedef RandomDBView<DataObj, ParamObj> my_type;
	typedef DB_random_select_iterator_buffer<DataObj, ParamObj> this_buffer;
	// buffer declared mutable since read operations that do not change the location
	// of the iterator may result in changes to the buffer due to row repositioning
	typedef CountedPtr<this_buffer> counted_buffer;

	CountedPtr<view_type> pview;
	counted_buffer pbuffer;
	KeyMode useWhichFields;	// do we USE_PK_ONLY or USE_ALL_FIELDS for writing changes to DB
	tstring KeyFields;
	   

public:
	typedef DB_random_select_update_iterator<DataObj, ParamObj> iterator;
	typedef DB_random_select_iterator<DataObj, ParamObj> const_iterator;

	typedef DataObj value_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef size_t size_type;
	typedef typename DB_random_select_iterator<DataObj, ParamObj>::difference_type difference_type;

#if defined(_MSC_VER) && ( _MSC_VER < 1300)
	// MSVC 6 non-standard on reverse iterator
	typedef STD_::reverse_iterator<iterator, value_type, const_reference> reverse_iterator;
	typedef STD_::reverse_iterator<const_iterator, value_type, const_reference> const_reverse_iterator;
#else
	typedef STD_::reverse_iterator<iterator> reverse_iterator;
	typedef STD_::reverse_iterator<const_iterator> const_reverse_iterator;
#endif

	RandomDBView(const view_type &vw) : pview(new view_type(vw)), pbuffer(new this_buffer(pview->begin())),
		useWhichFields(vw.GetKeyMode()),  KeyFields(MakeDelimitedList(vw.GetKey())) {}

	RandomDBView(const my_type &other) : pview(other.pview), pbuffer(other.pbuffer),
		useWhichFields(other.useWhichFields),  KeyFields(MakeDelimitedList(other.KeyFields)){}

	// exception-safe swap and assignment
	void swap(my_type &x)
	{
		pview.swap(x.pview);
		pbuffer.swap(x.pbuffer);
		STD_::swap(useWhichFields, x.useWhichFields);
		KeyFields.swap(x.KeyFields);
	}

	my_type &operator=(const my_type &other)
	{
		if (this != &other)
		{
			my_type tmp(other);
			swap(tmp);
		}

		return *this;
	}


	const_iterator begin() const
	{
		return const_iterator(*pview, pbuffer);
	}

	const_iterator end() const
	{
		return const_iterator(pview->end(), pbuffer);
	}

	iterator begin()
	{
		iterator it(*pview, pbuffer);
		if (useWhichFields == USE_PK_FIELDS_ONLY)
		   it.SetKey(KeyFields);
		return it;
	}

	iterator end()
	{
		iterator it(pview->end(), pbuffer);
		if (useWhichFields == USE_PK_FIELDS_ONLY)
		   it.SetKey(KeyFields);
		return it;
	}

	const_reverse_iterator rbegin() const
	{
		return const_reverse_iterator(end());
	}

    const_reverse_iterator rend() const
	{
		return const_reverse_iterator(begin());
	}

	reverse_iterator rbegin()
	{
		return reverse_iterator(end());
	}

	reverse_iterator rend()
	{
		return reverse_iterator(begin());
	}

    size_type size() const
	{
		return end()-begin();
	}

    size_type max_size() const
	{
		return end()-begin();
	}

    bool empty() const
	{
		return end()==begin();
	}

	void insert(const value_type& x)
	{
		typename view_type::insert_iterator in_it(*pview);
		*in_it = x;
		++in_it;
	}

    template<class InputIterator> 
    void insert(InputIterator first, InputIterator last)
	{
		typename view_type::insert_iterator in_it(*pview);
		STD_::copy(first, last, in_it);
	}

	// set key fields to use for update clause
	// ****** note this overrides DBView behavior!!! *****
	void SetKey(const tstring &KF) {
		useWhichFields = USE_PK_FIELDS_ONLY;
		KeyFields = KF;
	}

private:
	DBView<DataObj, ParamObj> make_delete_view() 
	{
	   DB_replacer<DataObj, ParamObj> replacer(*pview);
	   if (useWhichFields == USE_PK_FIELDS_ONLY)
		   replacer.SetKey(KeyFields);
	   else {
		   STD_::vector<tstring> tmp_fields;
           DataObj rowbuf;
		   replacer.Init_bpabios(tmp_fields,rowbuf);
	   }

	   BoundIOs tmpbios(replacer.Get_bpabios());

	   
	   // note: as the below view does not have a postfix clause,
	   // extra records might get deleted then what the user might expect
	   // (except in autokey case where only the requested records should get
	   //  wiped from the face of the earth!)
	   DBView<DataObj, ParamObj> del_view(MakeDelimitedList(pview->GetTableNames()),
		   DynamicBCA<DataObj, TYPENAME_IN_TEMPLATE_PARAMS DBView<DataObj, ParamObj>::BCA>(tmpbios, pview->GetBCA()), _TEXT(""),
		   EmptyBPA<ParamObj>(), pview->GetSelVal(), pview->GetInsVal(),
		   pview->get_io_handler(),
		   pview->GetConnection());

	   return del_view;
	}

public:
	void erase(iterator it)
	{

	   DBView<DataObj, ParamObj> del_view(make_delete_view());

	   // bPrepare = false as stmt. only executed once
	   typename DBView<DataObj, ParamObj>::delete_iterator del_it(del_view, false);

	   *del_it = *it;
   	   ++del_it;

	    // if iterator encountered a problem, that means deletion failed, so throw
	   if (del_it.bad() || del_it.fail()  || del_it.GetLastCount() == 0)
			  DTL_THROW DBException(_TEXT("RandomDBView::erase(iterator first, iterator last)"), 
			  _TEXT("Delete from DB failed! Record may have been changed by another user."),
					NULL, NULL);

	}

	void erase(iterator first, iterator last)
	{
		DBView<DataObj, ParamObj> del_view(make_delete_view);

	   typename DBView<DataObj, ParamObj>::delete_iterator del_it(del_view);

	   STD_::copy(first, last, del_it);
	   
	   // if iterator encountered a problem, that means deletion failed, so throw
	   if (del_it.bad() || del_it.fail()  || del_it.GetLastCount() == 0)
			  DTL_THROW DBException(_TEXT("RandomDBView::erase(iterator first, iterator last)"), 
			  _TEXT("Delete from DB failed! Record may have been changed by another user."),
					NULL, NULL);
	}

	// Close the cursor used by the random access iterators
	// Force re-query of the database to reflect updated/inserted/deleted
	// records by this and other users.
	void ReQuery() 
	{
		pbuffer->Reset();
	}


	bool operator==(const my_type &other) const 
	{
	   return pview==other.pview;
    }

	bool operator!=(const my_type &other) const 
	{
	   return !(*this == other);
    }

	// NOTE!  We only return a const DataObj &
    // Why? Because if you attempt a modification of
    // the data via the operator the container would not
    // detect it.
	const DataObj &operator[](SQLINTEGER i) const {
	 const_iterator it(begin());
	 it += i;
	 return *it;
    }



};

template <class DataObj, class ParamObj>
inline bool
operator<(const RandomDBView<DataObj, ParamObj> &lhs, const RandomDBView<DataObj, ParamObj> &rhs) {
	return STD_::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <class DataObj, class ParamObj>
inline bool
operator>(const RandomDBView<DataObj, ParamObj> &lhs, const RandomDBView<DataObj, ParamObj> &rhs) {
	return rhs < lhs;
}

template <class DataObj, class ParamObj>
inline bool
operator<=(const RandomDBView<DataObj, ParamObj> &lhs, const RandomDBView<DataObj, ParamObj> &rhs) {
	return !(lhs > rhs);
}

template <class DataObj, class ParamObj>
inline bool
operator>=(const RandomDBView<DataObj, ParamObj> &lhs, const RandomDBView<DataObj, ParamObj> &rhs) {
	return !(lhs < rhs);
}

END_DTL_NAMESPACE

#endif //DTL_RANDOM_DBVIEW_H
