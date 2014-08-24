//wrap_list.hpp

/****************************************************************************
Copyright (c) 2014-2018 yggdrasil

author: yang xu

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef __YGGR_LUA_WRAP_WRAP_LIST_HPP__
#define __YGGR_LUA_WRAP_WRAP_LIST_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/lua_wrap/wrap_base_t_helper.hpp>
#include <yggr/lua_wrap/wrap_iterator.hpp>
#include <boost/container/list.hpp>
#include <algorithm>
#include <iostream>

namespace yggr
{
namespace lua_wrap
{

template<typename Val,
			typename Ax = std::allocator<Val>,
			template<typename _Val, typename _Ax> class List = boost::container::list
		>
class wrap_list : public List<Val, Ax>
{
public:
	typedef Val val_type;
	typedef Ax alloc_type;

public:
	typedef List<val_type, alloc_type> base_type;

	typedef typename base_type::allocator_type allocator_type;
	typedef typename base_type::value_type value_type;
	typedef typename base_type::size_type size_type;
	typedef typename base_type::difference_type difference;

	typedef typename base_type::reference reference;
	typedef typename base_type::const_reference const_reference;

	typedef typename base_type::pointer pointer;
	typedef typename base_type::const_pointer const_pointer;

	typedef typename base_type::iterator iterator;
	typedef typename base_type::const_iterator const_iterator;

	typedef typename base_type::reverse_iterator reverse_iterator;
	typedef typename base_type::const_reverse_iterator const_reverse_iterator;

private:
	typedef wrap_list this_type;
	typedef wrap_base_t_helper<val_type> val_wrap_helper_type;
	typedef wrap_base_t_helper<size_type> size_wrap_helper_type;

public:
	typedef typename val_wrap_helper_type::wrap_val_type wrap_val_type;
	typedef typename size_wrap_helper_type::wrap_val_type wrap_size_type;

	typedef wrap_allocator<this_type> wrap_allocator_type;
	typedef value_iterator<this_type> wrap_iterator_type;
	typedef value_const_iterator<this_type> wrap_const_iterator_type;
	typedef value_reverse_iterator<this_type> wrap_reverse_iterator_type;
	typedef value_const_reverse_iterator<this_type> wrap_const_reverse_iterator_type;

private:
	enum
	{
		E_sort_mode = 0,
		E_sort_less,
		E_sort_greater,
		E_compile_u32 = 0xffffffff
	};

public:
	wrap_list(void)
	{
	}

	explicit wrap_list(const wrap_allocator_type& alloc)
		: base_type(alloc)
	{
	}

	explicit wrap_list(const wrap_size_type& count)
		: base_type(count)
	{
	}

	wrap_list(const wrap_size_type& count, const wrap_val_type& val)
		: base_type(count, val)
	{
	}

	wrap_list(const wrap_size_type& count, const wrap_val_type& val, const wrap_allocator_type& alloc)
		: base_type(count, val, alloc)
	{
	}

	wrap_list(const wrap_iterator_type& first, const wrap_iterator_type& last)
		: base_type(first, last)
	{
	}

	wrap_list(const wrap_const_iterator_type& first, const wrap_const_iterator_type& last)
		: base_type(first, last)
	{
	}

	wrap_list(const wrap_reverse_iterator_type& first, const wrap_reverse_iterator_type& last)
		: base_type(first, last)
	{
	}

	wrap_list(const wrap_const_reverse_iterator_type& first, const wrap_const_reverse_iterator_type& last)
		: base_type(first, last)
	{
	}

	template<class InputIterator>
	wrap_list(InputIterator first, InputIterator last)
		: base_type(first, last)
	{
	}

	wrap_list(const wrap_iterator_type& first, const wrap_iterator_type& last, const wrap_allocator_type& alloc)
		: base_type(first, last, alloc)
	{
	}

	wrap_list(const wrap_const_iterator_type& first, const wrap_const_iterator_type& last, const wrap_allocator_type& alloc)
		: base_type(first, last, alloc)
	{
	}

	wrap_list(const wrap_reverse_iterator_type& first, const wrap_reverse_iterator_type& last, const wrap_allocator_type& alloc)
		: base_type(first, last, alloc)
	{
	}

	wrap_list(const wrap_const_reverse_iterator_type& first, const wrap_const_reverse_iterator_type& last, const wrap_allocator_type& alloc)
		: base_type(first, last, alloc)
	{
	}

	template<class InputIterator>
	wrap_list(InputIterator first, InputIterator last, const wrap_allocator_type& alloc)
		: base_type(first, last, alloc)
	{
	}

	explicit wrap_list(const base_type& right)
		: base_type(right)
	{
	}

	wrap_list(const base_type& right, const wrap_allocator_type& alloc)
		: base_type(right, alloc)
	{
	}

	wrap_list(const this_type& right)
		: base_type(right)
	{
	}

	~wrap_list(void)
	{
	}

	operator base_type&(void)
	{
		base_type& base = *this;
		return base;
	}

	operator const base_type&(void) const
	{
		const base_type& base = *this;
		return base;
	}

	// iterators:

	wrap_iterator_type begin(void)
	{
		return base_type::begin();
	}

	wrap_const_iterator_type begin(void) const
	{
		return base_type::begin();
	}

	wrap_iterator_type end(void)
	{
		return base_type::end();
	}

	wrap_const_iterator_type end(void) const
	{
		return base_type::end();
	}

	wrap_reverse_iterator_type rbegin(void)
	{
		return base_type::rbegin();
	}

	wrap_const_reverse_iterator_type rbegin(void) const
	{
		return base_type::rbegin();
	}

	wrap_reverse_iterator_type rend(void)
	{
		return base_type::rend();
	}

	wrap_const_reverse_iterator_type rend(void) const
	{
		return base_type::rend();
	}

	// capacity:
	wrap_size_type size(void) const
	{
		return base_type::size();
	}

	wrap_size_type max_size(void) const
	{
		return base_type::max_size();
	}

	bool empty(void) const
	{
		return base_type::empty();
	}

	//element access:

	wrap_val_type& front(void)
	{
		return base_type::front();
	}

	const wrap_val_type& front(void) const
	{
		return base_type::front();
	}

	wrap_val_type& back(void)
	{
		return base_type::back();
	}

	const wrap_val_type& back(void) const
	{
		return base_type::back();
	}

	// modifiers:
	void assign(const wrap_size_type& n, const wrap_val_type& val)
	{
		base_type::assign(n, val);
	}

	void assign(const wrap_iterator_type& start, const wrap_iterator_type& last)
	{
		base_type::assign(start, last);
	}

	void assign(const wrap_const_iterator_type& start, const wrap_const_iterator_type& last)
	{
		base_type::assign(start, last);
	}

	void assign(const wrap_reverse_iterator_type& start, const wrap_reverse_iterator_type& last)
	{
		base_type::assign(start, last);
	}

	void assign(const wrap_const_reverse_iterator_type& start, const wrap_const_reverse_iterator_type& last)
	{
		base_type::assign(start, last);
	}

	void push_front(const wrap_val_type& val)
	{
		base_type::push_front(val);
	}

	void push_back(const wrap_val_type& val)
	{
		base_type::push_back(val);
	}

	void pop_front(void)
	{
		base_type::pop_front();
	}

	void pop_back(void)
	{
		base_type::pop_back();
	}

	wrap_iterator_type insert(const wrap_iterator_type& pos, const wrap_val_type& val)
	{
		return base_type::insert(pos, val);
	}

	void insert(const wrap_iterator_type& pos, const wrap_size_type& n, const wrap_val_type& val)
	{
		base_type::insert(pos, n, val);
	}

	void insert(const wrap_iterator_type& pos, const wrap_iterator_type& first, const wrap_iterator_type& last)
	{
		base_type::insert(pos, first, last);
	}

	void insert(const wrap_iterator_type& pos, const wrap_const_iterator_type& first, const wrap_const_iterator_type& last)
	{
		base_type::insert(pos, first, last);
	}

	void insert(const wrap_iterator_type& pos, const wrap_reverse_iterator_type& first, const wrap_reverse_iterator_type& last)
	{
		base_type::insert(pos, first, last);
	}

	void insert(const wrap_iterator_type& pos, const wrap_const_reverse_iterator_type& first, const wrap_const_reverse_iterator_type& last)
	{
		base_type::insert(pos, first, last);
	}

	wrap_iterator_type erase(const wrap_iterator_type& iter)
	{
		return base_type::erase(iter);
	}

	wrap_iterator_type erase(const wrap_iterator_type& iters, const wrap_iterator_type& itere)
	{
		return base_type::erase(iters, itere);
	}

	void swap(this_type& right)
	{
		base_type::swap(right);
	}

	void resize(const size_type& size)
	{
		base_type::resize(size);
	}

	void clear(void)
	{
		base_type::clear();
	}

	// operations:

	void splice(const wrap_iterator_type& iter, this_type& right)
	{
		base_type::splice(iter, right);
	}

	void splice(const wrap_iterator_type& iter, this_type& right, const wrap_iterator_type& first)
	{
		base_type::splice(iter, right, first);
	}

	void splice(const wrap_iterator_type& iter, this_type& right, const wrap_iterator_type& first, const wrap_iterator_type& last)
	{
		base_type::splice(iter, right, first, last);
	}

	void remove(const wrap_val_type& val)
	{
		base_type::remove(val);
	}

	void unique(void)
	{
		base_type::unique();
	}

	void merge(this_type& right)
	{
		base_type::merge(right);
	}

	void sort(u32 mode) // true upper fase lower
	{
		switch(mode)
		{
		case E_sort_less:
			base_type::sort(std::less<val_type>());
			break;
		case E_sort_greater:
			base_type::sort(std::greater<val_type>());
			break;
		}
	}

	void reverse(void)
	{
		base_type::reverse();
	}

	// observers:

	wrap_allocator_type get_allocator(void) const
	{
		return base_type::get_allocator();
	}
};

} // namespce lua_wrap
} // namespace yggr


template<typename T, typename Ax>
std::ostream& operator<<(std::ostream& os, const yggr::lua_wrap::wrap_list<T, Ax>& vt)
{
	typedef T val_type;
	typedef Ax alloc_type;
	typedef yggr::lua_wrap::wrap_list<val_type, alloc_type> wrap_vector_type;
	typedef typename wrap_vector_type::base_type vector_type;
	typedef typename vector_type::const_iterator citer_type;

	for(citer_type begin = vt.begin(), i = vt.begin(), isize = vt.end(); i != isize; ++i)
	{
		if(i != begin)
		{
			os << ", ";
		}

		os << *i;
	}

	return os;
}

#endif //__YGGR_LUA_WRAP_WRAP_LIST_HPP__
