//safe_map.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_SAFE_MAP_HPP__
#define __YGGR_SAFE_CONTAINER_SAFE_MAP_HPP__

#include <cassert>

#include <yggr/utility/swap.hpp>
#include <boost/container/map.hpp>
#include <boost/thread/mutex.hpp>
#include <yggr/move/move.hpp>
#include <boost/bind.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/nonable/noncopyable.hpp>
#include <yggr/nonable/nonmoveable.hpp>
#include <yggr/helper/mutex_def_helper.hpp>
#include <yggr/safe_container/detail/swap_def.hpp>

namespace yggr
{
namespace safe_container
{

template<typename Key, typename Val,
			typename Mutex = boost::mutex,
			typename Cmp = std::less<Key>,
			typename Alloc = std::allocator<std::pair<const Key, Val> >,
			template<typename _Key, typename _Val, typename _Cmp, typename _Alloc> class Map = boost::container::map
		>
class safe_map
	: protected Map<Key, Val, Cmp, Alloc>,
		private nonable::noncopyable,
		private nonable::nonmoveable
{
public:
	typedef Key key_type;
	typedef Val val_type;
	typedef Cmp cmp_type;
	typedef Alloc alloc_type;

public:
	typedef Map<key_type, val_type, cmp_type, alloc_type> base_type;

	typedef typename base_type::allocator_type allocator_type;

	//typedef typename base_type::key_type key_type;
	typedef typename base_type::mapped_type mapped_type;

	typedef typename base_type::value_type value_type;
	typedef typename base_type::size_type size_type;
	typedef typename base_type::difference_type difference;

	typedef typename base_type::key_compare key_compare;
	typedef typename base_type::value_compare value_compare;

	typedef typename base_type::reference reference;
	typedef typename base_type::const_reference const_reference;

	typedef typename base_type::pointer pointer;
	typedef typename base_type::const_pointer const_pointer;

	typedef typename base_type::iterator iterator;
	typedef typename base_type::const_iterator const_iterator;

	typedef typename base_type::reverse_iterator reverse_iterator;
	typedef typename base_type::const_reverse_iterator const_reverse_iterator;

	typedef std::pair<iterator, bool> insert_result_type;

private:
	typedef Mutex mutex_type;
	typedef helper::mutex_def_helper<mutex_type> mutex_def_helper_type;
	typedef typename mutex_def_helper_type::read_lock_type read_lock_type;
	typedef typename mutex_def_helper_type::write_lock_type write_lock_type;

	typedef safe_map this_type;

public:
	safe_map(void)
	{
	}

	explicit safe_map(const cmp_type& cmp)
		: base_type(cmp)
	{
	}

	explicit safe_map(const alloc_type& alloc)
		: base_type(cmp_type(), alloc)
	{
	}

	safe_map(const cmp_type& cmp, const alloc_type& alloc)
		: base_type(cmp, alloc)
	{
	}

	template<class InputIterator>
	safe_map(InputIterator first, InputIterator last)
		: base_type(first, last)
	{
	}

	template<class InputIterator>
	safe_map(InputIterator first, InputIterator last, const cmp_type& cmp)
		: base_type(first, last, cmp)
	{
	}

	template<class InputIterator>
	safe_map(InputIterator first, InputIterator last, const alloc_type& alloc)
		: base_type(first, last, cmp_type(), alloc)
	{
	}

	template<class InputIterator>
	safe_map(InputIterator first, InputIterator last, const cmp_type& cmp, const alloc_type& alloc)
		: base_type(first, last, cmp, alloc)
	{
	}

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	explicit safe_map(BOOST_RV_REF(base_type) right)
		: base_type(boost::forward<base_type>(right))
	{
	}
#else
	explicit safe_map(BOOST_RV_REF(base_type) right)
	{
		base_type& right_ref = right;
		base_type::swap(right_ref);
	}
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

	explicit safe_map(const base_type& right)
		: base_type(right)
	{
	}

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	safe_map(BOOST_RV_REF(base_type) right, const alloc_type& alloc)
		: base_type(cmp_type(), alloc)
	{
		//base_type::swap(boost::forward<base_type>(right));
		base_type::swap(right);
	}
#else
	safe_map(BOOST_RV_REF(base_type) right, const alloc_type& alloc)
		: base_type(cmp_type(), alloc)
	{
		base_type& right_ref = right;
		base_type::swap(right_ref);
	}
#endif //YGGR_NO_CXX11_RVALUE_REFERENCES

	safe_map(const base_type& right, const alloc_type& alloc)
		: base_type(cmp_type(), alloc)
	{
		base_type tmp(right);
	    base_type::swap(tmp);
	}

	~safe_map(void)
	{
	}

	this_type& operator=(BOOST_RV_REF(base_type) right)
	{
		write_lock_type lk(_mutex);
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		base_type::operator=(boost::forward<base_type>(right));
#else
		base_type& right_ref = right;
		base_type::swap(right_ref);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		return *this;
	}

	this_type& operator=(const base_type& right)
	{
		write_lock_type lk(_mutex);
		base_type::operator=(right);
		return *this;
	}

	// capacity:
	bool empty(void) const
	{
		read_lock_type lk(_mutex);
		return base_type::empty();
	}

	size_type size(void) const
	{
		read_lock_type lk(_mutex);
		return base_type::size();
	}

	size_type max_size(void) const
	{
		read_lock_type lk(_mutex);
		return base_type::max_size();
	}

	// element access:
	bool get_value(const key_type& key, val_type& val) const
	{
		const_iterator iter;
		read_lock_type lk(_mutex);
		iter = base_type::find(key);

		if(iter == base_type::end())
		{
			return false;
		}

		val = iter->second;
		return true;
	}

	// modifiers:
	bool insert(const key_type& key, BOOST_RV_REF(val_type) val)
	{
		value_type tmp(key, boost::forward<val_type>(val));
		return this_type::insert(yggr::move::move_helper::forced_move(tmp));
	}

	bool insert(const key_type& key, const val_type& val)
	{
	    value_type tmp(key, val);
		return this_type::insert(yggr::move::move_helper::forced_move(tmp));
	}

	//!!! this foo ambiguous bool insert(const key_type& key, const val_type& val, const Handler& handler) !!!
	//template<typename Handler>
	//bool insert(const key_type& key, BOOST_RV_REF(val_type) val, const Handler& handler)
	//{
	//	return this_type::insert(boost::move(value_type(key, boost::forward<val_type>(val))), handler);
	//}

	template<typename Handler>
	bool insert(const key_type& key, const val_type& val, const Handler& handler)
	{
	    value_type tmp(key, val);
		return this_type::insert(yggr::move::move_helper::forced_move(tmp), handler);
	}

	template<typename Handler, typename Err_Fix_Handler>
	bool insert(const key_type& key, BOOST_RV_REF(val_type) val, const Handler& handler, const Err_Fix_Handler& ef_handler)
	{
	    value_type tmp(key,  boost::forward<val_type>(val));
		return this_type::insert(yggr::move::move_helper::forced_move(tmp), handler, ef_handler);
	}

	template<typename Handler, typename Err_Fix_Handler>
	bool insert(const key_type& key, const val_type& val, const Handler& handler, const Err_Fix_Handler& ef_handler)
	{
	    value_type tmp(key, val);
		return this_type::insert(yggr::move::move_helper::forced_move(tmp), handler, ef_handler);
	}

	bool insert(BOOST_RV_REF(value_type) val)
	{
		write_lock_type lk(_mutex);
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		return base_type::insert(boost::forward<value_type>(val)).second;
#else
		const value_type& val_cref = val;
		return base_type::insert(val).second;
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
	}

	bool insert(const value_type& val)
	{
		write_lock_type lk(_mutex);
		return base_type::insert(val).second;
	}

	template<typename Handler>
	bool insert(BOOST_RV_REF(value_type) val, const Handler& handler)
	{
		write_lock_type lk(_mutex);
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		insert_result_type rst = base_type::insert(boost::forward<value_type>(val));
#else
		const value_type& val_cref = val;
		insert_result_type rst = base_type::insert(val_cref);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		if(rst.second)
		{
			handler(rst.first);
		}

		return rst.second;
	}

	template<typename Handler>
	bool insert(const value_type& val, const Handler& handler)
	{
		write_lock_type lk(_mutex);
		insert_result_type rst = base_type::insert(val);

		if(rst.second)
		{
			handler(rst.first);
		}

		return rst.second;
	}

	template<typename Handler, typename Err_Fix_Handler>
	bool insert(BOOST_RV_REF(value_type) val, const Handler& handler, const Err_Fix_Handler& ef_handler)
	{
		write_lock_type lk(_mutex);

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		insert_result_type rst = base_type::insert(boost::forward<value_type>(val));
#else
		const value_type& val_cref = val;
		insert_result_type rst = base_type::insert(val_cref);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		if(rst.second)
		{
			handler(rst.first);
			return rst.second;
		}

		return ef_handler(*this, boost::forward<value_type>(val));
	}

	template<typename Handler, typename Err_Fix_Handler>
	bool insert(const value_type& val, const Handler& handler, const Err_Fix_Handler& ef_handler)
	{
		write_lock_type lk(_mutex);
		insert_result_type rst = base_type::insert(val);
		if(rst.second)
		{
			handler(rst.first);
			return rst.second;
		}

		return ef_handler(*this, val);
	}

	template<typename InputIter>
	bool insert(InputIter start, InputIter last)
	{
		write_lock_type lk(_mutex);
		size_type old_size = base_type::size();
		base_type::insert(start, last);
		return old_size < base_type::size();
	}

	template<typename InputIter, typename Handler>
	bool insert(InputIter start, InputIter last, const Handler& handler)
	{
		write_lock_type lk(_mutex);
		size_type old_size = base_type::size();
		base_type::insert(start, last);
		if(old_size < base_type::size())
		{
			base_type& base = *this;
			handler(base);
			return true;
		}

		return false;
	}

	template<typename InputIter, typename Handler, typename Err_Fix_Handler>
	bool insert(InputIter start, InputIter last, const Handler& handler, const Err_Fix_Handler& ef_handler)
	{
		write_lock_type lk(_mutex);
		size_type old_size = base_type::size();
		base_type::insert(start, last);
		base_type& base = *this;
		if(old_size < base_type::size())
		{
			handler(base);
			return true;
		}

		return ef_handler(base, start, last);
	}

	size_type erase(const key_type& key)
	{
		write_lock_type lk(_mutex);
		return base_type::erase(key);
	}

	void swap(BOOST_RV_REF(base_type) right)
	{
		write_lock_type lk(_mutex);
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		base_type::swap(right);
#else
		base_type& right_ref = right;
		base_type::swap(right_ref);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
	}

	void swap(base_type& right)
	{
		write_lock_type lk(_mutex);
		base_type::swap(right);
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}
		write_lock_type lk(_mutex);
		base_type& base = *this;
		right.swap(base);
	}

	void clear(void)
	{
		base_type tmp(base_type::key_comp(), base_type::get_allocator());
		write_lock_type lk(_mutex);
		base_type::swap(tmp);
	}

	bool set_value(const key_type& key, BOOST_RV_REF(val_type) val)
	{
		value_type tmp(key, boost::forward<val_type>(val));
		return this_type::set_value(yggr::move::move_helper::forced_move(tmp));
	}

	bool set_value(const key_type& key, const val_type& val)
	{
		value_type tmp(key, val);
		return this_type::set_value(yggr::move::move_helper::forced_move(tmp));
	}

	bool set_value(BOOST_RV_REF(value_type) val)
	{
		write_lock_type lk(_mutex);
		base_type& base = *this;
		iterator iter = base_type::find(val.first);
		if(iter == base_type::end())
		{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
			return base_type::insert(boost::forward<value_type>(val)).second;
#else
			const value_type& val_cref = val;
			return base_type::insert(val_cref).second;
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		}

		iter->second = val.second;
		return true;
	}

	bool set_value(const value_type& val)
	{
		write_lock_type lk(_mutex);
		iterator iter = base_type::find(val.first);
		if(iter == base_type::end())
		{
			return base_type::insert(val).second;
		}

		iter->second = val.second;
		return true;
	}

	bool replace(const key_type& key, BOOST_RV_REF(val_type) val)
	{
		value_type tmp(key, boost::forward<val_type>(val));
		return this_type::replace(yggr::move::move_helper::forced_move(tmp));
	}

	bool replace(const key_type& key, const val_type& val)
	{
		value_type tmp(key, val);
		return this_type::replace(yggr::move::move_helper::forced_move(tmp));
	}

	bool replace(BOOST_RV_REF(value_type) val)
	{
		write_lock_type lk(_mutex);
		iterator iter = base_type::find(val.first);
		if(iter == base_type::end())
		{
			return false;
		}

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		iter->second = boost::forward<val_type>(val.second);
#else
		value_type& val_ref = val;
		boost::swap(iter->second, val_ref.second);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		return true;
	}

	bool replace(const value_type& val)
	{
		write_lock_type lk(_mutex);
		iterator iter = base_type::find(val.first);
		if(iter == base_type::end())
		{
			return false;
		}

		iter->second = val.second;
		return true;
	}

	// observers:
	key_compare key_comp(void) const
	{
		read_lock_type lk(_mutex);
		return base_type::key_comp();
	}

	value_compare value_comp(void) const
	{
		read_lock_type lk(_mutex);
		return base_type::value_comp();
	}

	// operations:
	size_type count(const key_type& key) const
	{
		read_lock_type lk(_mutex);
		return base_type::count(key);
	}

	// allocator:
	alloc_type get_allocator(void) const
	{
		read_lock_type lk(_mutex);
		return base_type::get_allocator();
	}

	// safe other
	bool is_exists(const key_type& key) const
	{
		read_lock_type lk(_mutex);
		return find(key) != base_type::end();
	}

	void copy_to_base(base_type& out) const
	{
		read_lock_type lk(_mutex);
		const base_type& base = *this;
		out = base;
	}

	// use handler
	template<typename Handler>
	bool use_handler(const key_type& key, const Handler& handler)
	{
		write_lock_type lk(_mutex);
		iterator iter = base_type::find(key);
		if(iter == base_type::end())
		{
			return false;
		}

		handler(iter);
		return true;
	}

	template<typename Handler>
	bool use_handler(const key_type& key, const Handler& handler) const
	{
		read_lock_type lk(_mutex);
		const_iterator iter = base_type::find(key);
		if(iter == base_type::end())
		{
			return false;
		}

		handler(iter);
		return true;
	}

	template<typename Handler, typename Return_Handler>
	bool use_handler(const key_type& key, const Handler& handler, const Return_Handler& ret)
	{
		write_lock_type lk(_mutex);
		iterator iter = base_type::find(key);
		if(iter == base_type::end())
		{
			return false;
		}

		handler(iter, ret);
		return true;
	}

	template<typename Handler, typename Return_Handler>
	bool use_handler(const key_type& key, const Handler& handler, const Return_Handler& ret) const
	{
		read_lock_type lk(_mutex);
		const_iterator iter = base_type::find(key);
		if(iter == base_type::end())
		{
			return false;
		}

		handler(iter, ret);
		return true;
	}

	template<typename Handler>
	typename Handler::result_type use_handler(const Handler& handler)
	{
		write_lock_type lk(_mutex);

		base_type& base = *this;
		return handler(base);
	}

	template<typename Handler>
	typename Handler::result_type use_handler(const Handler& handler) const
	{
		read_lock_type lk(_mutex);

		const base_type& base = *this;
		return handler(base);
	}

	template<typename Handler, typename Return_Handler>
	typename Handler::result_type use_handler(const Handler& handler, const Return_Handler& ret)
	{
		write_lock_type lk(_mutex);

		base_type& base = *this;
		return handler(base, ret);
	}

	template<typename Handler, typename Return_Handler>
	typename Handler::result_type use_handler(const Handler& handler, const Return_Handler& ret) const
	{
		read_lock_type lk(_mutex);

		const base_type& base = *this;
		return handler(base, ret);
	}

	template<typename Handler>
	void use_handler_of_all(const Handler& handler)
	{
		write_lock_type lk(_mutex);
		for(iterator i = base_type::begin(), isize = base_type::end(); i != isize; ++i)
		{
			handler(i);
		}
	}

	template<typename Handler>
	void use_handler_of_all(const Handler& handler) const
	{
		read_lock_type lk(_mutex);
		for(const_iterator i = base_type::begin(), isize = base_type::end(); i != isize; ++i)
		{
			handler(i);
		}
	}

	template<typename Handler, typename Condition_Handler>
	void use_handler_of_all(const Handler& handler, const Condition_Handler& cdt_handler)
	{
		write_lock_type lk(_mutex);
		for(iterator i = base_type::begin(), isize = base_type::end(); i != isize; ++i)
		{
			if(cdt_handler(i))
			{
				handler(i);
			}
		}
	}

	template<typename Handler, typename Condition_Handler>
	void use_handler_of_all(const Handler& handler, const Condition_Handler& cdt_handler) const
	{
		read_lock_type lk(_mutex);
		for(const_iterator i = base_type::begin(), isize = base_type::end(); i != isize; ++i)
		{
			if(cdt_handler(i))
			{
				handler(i);
			}
		}
	}

	template<typename Handler>
	void use_handler_of_reverse_all(const Handler& handler)
	{
		write_lock_type lk(_mutex);
		for(reverse_iterator i = base_type::rbegin(), isize = base_type::rend(); i != isize; ++i)
		{
			handler(i);
		}
	}

	template<typename Handler>
	void use_handler_of_reverse_all(const Handler& handler) const
	{
		read_lock_type lk(_mutex);
		for(const_reverse_iterator i = base_type::rbegin(), isize = base_type::rend(); i != isize; ++i)
		{
			handler(i);
		}
	}

	template<typename Handler, typename Condition_Handler>
	void use_handler_of_reverse_all(const Handler& handler, const Condition_Handler& cdt_handler)
	{
		write_lock_type lk(_mutex);
		for(reverse_iterator i = base_type::rbegin(), isize = base_type::rend(); i != isize; ++i)
		{
			if(cdt_handler(i))
			{
				handler(i);
			}
		}
	}

	template<typename Handler, typename Condition_Handler>
	void use_handler_of_reverse_all(const Handler& handler, const Condition_Handler& cdt_handler) const
	{
		read_lock_type lk(_mutex);
		for(const_reverse_iterator i = base_type::rbegin(), isize = base_type::rend(); i != isize; ++i)
		{
			if(cdt_handler(i))
			{
				handler(i);
			}
		}
	}

private:
	mutable mutex_type _mutex;
};

} // namespace safe_container
} // namespace yggr

//--------------------------------------------------support std::swap-------------------------------------
namespace std
{
	YGGR_PP_SAFE_CONTAINER_SWAP(5, 4, yggr::safe_container::safe_map)
} // namespace std

//-------------------------------------------------support boost::swap-----------------------------------
namespace boost
{
	YGGR_PP_SAFE_CONTAINER_SWAP(5, 4, yggr::safe_container::safe_map)
} // namespace boost

namespace yggr
{
namespace safe_container
{
//------------------------multimap-------------------------------------------------------
template<typename Key, typename Val,
			typename Mutex = boost::mutex,
			typename Cmp = std::less<Key>,
			typename Alloc = std::allocator< std::pair<const Key, Val> >,
			template<typename _Key, typename _Val, typename _Cmp, typename _Alloc> class MultiMap = boost::container::multimap
		>
class safe_multimap
	: protected MultiMap<Key, Val, Cmp, Alloc>,
		private nonable::noncopyable,
		private nonable::nonmoveable
{
public:
	typedef Key key_type;
	typedef Val val_type;
	typedef Cmp cmp_type;
	typedef Alloc alloc_type;

public:
	typedef MultiMap<Key, Val, Cmp, Alloc> base_type;

	typedef typename base_type::allocator_type allocator_type;

	//typedef typename base_type::key_type key_type;
	typedef typename base_type::mapped_type mapped_type;

	typedef typename base_type::value_type value_type;
	typedef typename base_type::size_type size_type;
	typedef typename base_type::difference_type difference;

	typedef typename base_type::key_compare key_compare;
	typedef typename base_type::value_compare value_compare;

	typedef typename base_type::reference reference;
	typedef typename base_type::const_reference const_reference;

	typedef typename base_type::pointer pointer;
	typedef typename base_type::const_pointer const_pointer;

	typedef typename base_type::iterator iterator;
	typedef typename base_type::const_iterator const_iterator;

	typedef typename base_type::reverse_iterator reverse_iterator;
	typedef typename base_type::const_reverse_iterator const_reverse_iterator;

	typedef iterator insert_result_type;

private:
	typedef Mutex mutex_type;
	typedef helper::mutex_def_helper<mutex_type> mutex_def_helper_type;
	typedef typename mutex_def_helper_type::read_lock_type read_lock_type;
	typedef typename mutex_def_helper_type::write_lock_type write_lock_type;

	typedef safe_multimap this_type;

public:
	safe_multimap(void)
	{
	}

	explicit safe_multimap(const cmp_type& cmp)
		: base_type(cmp)
	{
	}

	explicit safe_multimap(const alloc_type& alloc)
		: base_type(cmp_type(), alloc)
	{
	}

	safe_multimap(const cmp_type& cmp, const alloc_type& alloc)
		: base_type(cmp, alloc)
	{
	}

	template<class InputIterator>
	safe_multimap(InputIterator first, InputIterator last)
		: base_type(first, last)
	{
	}

	template<class InputIterator>
	safe_multimap(InputIterator first, InputIterator last, const cmp_type& cmp)
		: base_type(first, last, cmp)
	{
	}

	template<class InputIterator>
	safe_multimap(InputIterator first, InputIterator last, const alloc_type& alloc)
		: base_type(first, last, cmp_type(), alloc)
	{
	}

	template<class InputIterator>
	safe_multimap(InputIterator first, InputIterator last, const cmp_type& cmp, const alloc_type& alloc)
		: base_type(first, last, cmp, alloc)
	{
	}

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	explicit safe_multimap(BOOST_RV_REF(base_type) right)
		: base_type(boost::forward<base_type>(right))
	{
	}
#else
	explicit safe_multimap(BOOST_RV_REF(base_type) right)
	{
		base_type& right_ref = right;
		base_type::swap(right_ref);
	}
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

	explicit safe_multimap(const base_type& right)
		: base_type(right)
	{
	}

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	safe_multimap(BOOST_RV_REF(base_type) right, const alloc_type& alloc)
		: base_type(cmp_type(), alloc)
	{
		//base_type::swap(boost::forward<base_type>(right));
		base_type::swap(right);
	}
#else
	safe_multimap(BOOST_RV_REF(base_type) right, const alloc_type& alloc)
		: base_type(cmp_type(), alloc)
	{
		base_type& right_ref = right;
		base_type::swap(right_ref);
	}
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

	safe_multimap(const base_type& right, const alloc_type& alloc)
		: base_type(cmp_type(), alloc)
	{
	    base_type tmp(right);
	    base_type::swap(tmp);
	}

	~safe_multimap(void)
	{
	}

	this_type& operator=(BOOST_RV_REF(base_type) right)
	{
		write_lock_type lk(_mutex);
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		base_type::operator=(boost::forward<base_type>(right));
#else
		base_type& right_ref = right;
		base_type::swap(right_ref);
#endif //YGGR_NO_CXX11_RVALUE_REFERENCES
		return *this;
	}

	this_type& operator=(const base_type& right)
	{
		write_lock_type lk(_mutex);
		base_type::operator=(right);
		return *this;
	}

	// capacity:
	bool empty(void) const
	{
		read_lock_type lk(_mutex);
		return base_type::empty();
	}

	size_type size(void) const
	{
		read_lock_type lk(_mutex);
		return base_type::size();
	}

	size_type max_size(void) const
	{
		read_lock_type lk(_mutex);
		return base_type::max_size();
	}

	// modifiers:
	bool insert(const key_type& key, BOOST_RV_REF(val_type) val)
	{
		value_type tmp(key, boost::forward<val_type>(val));
		return this_type::insert(yggr::move::move_helper::forced_move(tmp));
	}

	bool insert(const key_type& key, const val_type& val)
	{
		value_type tmp(key, val);
		return this_type::insert(yggr::move::move_helper::forced_move(tmp));
	}

	//!!!this foo ambiguous bool insert(const key_type& key, const val_type& val, const Handler& handler)!!!
	//template<typename Handler>
	//bool insert(const key_type& key, BOOST_RV_REF(val_type) val, const Handler& handler)
	//{
	//	return this_type::insert(boost::move(value_type(key, boost::forward<val_type>(val))), handler);
	//}

	template<typename Handler>
	bool insert(const key_type& key, const val_type& val, const Handler& handler)
	{
		value_type tmp(key, val);
		return this_type::insert(yggr::move::move_helper::forced_move(tmp), handler);
	}

	template<typename Handler, typename Err_Fix_Handler>
	bool insert(const key_type& key, BOOST_RV_REF(val_type) val, const Handler& handler, const Err_Fix_Handler& ef_handler)
	{
		value_type tmp(key, boost::forward<val_type>(val));
		return this_type::insert(yggr::move::move_helper::forced_move(tmp), handler, ef_handler);
	}

	template<typename Handler, typename Err_Fix_Handler>
	bool insert(const key_type& key, const val_type& val, const Handler& handler, const Err_Fix_Handler& ef_handler)
	{
		value_type tmp(key, val);
		return this_type::insert(yggr::move::move_helper::forced_move(tmp), handler, ef_handler);
	}

	bool insert(BOOST_RV_REF(value_type) val)
	{
		write_lock_type lk(_mutex);
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		return base_type::insert(boost::forward<value_type>(val)) != base_type::end();
#else
		const value_type& val_cref = val;
		return base_type::insert(val_cref) != base_type::end();
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
	}

	bool insert(const value_type& val)
	{
		write_lock_type lk(_mutex);
		return base_type::insert(val) != base_type::end();
	}

	template<typename Handler>
	bool insert(BOOST_RV_REF(value_type) val, const Handler& handler)
	{
		write_lock_type lk(_mutex);
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		insert_result_type rst = base_type::insert(boost::forward<value_type>(val));
#else
		const value_type& val_cref = val;
		insert_result_type rst = base_type::insert(val_cref);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		if(rst == base_type::end())
		{
			return false;
		}

		handler(rst);
		return true;
	}

	template<typename Handler>
	bool insert(const value_type& val, const Handler& handler)
	{
		write_lock_type lk(_mutex);
		insert_result_type rst = base_type::insert(val);
		if(rst == base_type::end())
		{
			return false;
		}

		handler(rst);
		return true;
	}

	template<typename Handler, typename Err_Fix_Handler>
	bool insert(BOOST_RV_REF(value_type) val, const Handler& handler, const Err_Fix_Handler& ef_handler)
	{
		write_lock_type lk(_mutex);
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		insert_result_type rst = base_type::insert(boost::forward<value_type>(val));
#else
		const value_type& val_cref = val;
		insert_result_type rst = base_type::insert(val_cref);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		if(rst != base_type::end())
		{
			handler(rst);
			return true;
		}

		return ef_handler(*this, val);
	}

	template<typename Handler, typename Err_Fix_Handler>
	bool insert(const value_type& val, const Handler& handler, const Err_Fix_Handler& ef_handler)
	{
		write_lock_type lk(_mutex);
		insert_result_type rst = base_type::insert(val);
		if(rst != base_type::end())
		{
			handler(rst);
			return true;
		}

		return ef_handler(*this, val);
	}

	template<typename InputIter>
	bool insert(InputIter start, InputIter last)
	{
		write_lock_type lk(_mutex);
		size_type old_size = base_type::size();
		base_type::insert(start, last);
		return old_size < base_type::size();
	}

	template<typename InputIter, typename Handler>
	bool insert(InputIter start, InputIter last, const Handler& handler)
	{
		write_lock_type lk(_mutex);
		size_type old_size = base_type::size();
		base_type::insert(start, last);
		if(old_size < base_type::size())
		{
			base_type& base = *this;
			handler(base);
			return true;
		}

		return false;
	}

	template<typename InputIter, typename Handler, typename Err_Fix_Handler>
	bool insert(InputIter start, InputIter last, const Handler& handler, const Err_Fix_Handler& ef_handler)
	{
		write_lock_type lk(_mutex);
		size_type old_size = base_type::size();
		base_type::insert(start, last);
		base_type& base = *this;
		if(old_size < base_type::size())
		{
			handler(base);
			return true;
		}

		return ef_handler(base, start, last);
	}

	size_type erase(const key_type& key)
	{
		write_lock_type lk(_mutex);
		return base_type::erase(key);
	}

	void swap(BOOST_RV_REF(base_type) right)
	{
		write_lock_type lk(_mutex);
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		base_type::swap(right);
#else
		base_type& right_ref = right;
		base_type::swap(right_ref);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
	}

	void swap(base_type& right)
	{
		write_lock_type lk(_mutex);
		base_type::swap(right);
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}
		write_lock_type lk(_mutex);
		base_type& base = *this;
		right.swap(base);
	}

	void clear(void)
	{
		base_type tmp(base_type::key_comp(), base_type::get_allocator());
		write_lock_type lk(_mutex);
		base_type::swap(tmp);
	}

	// observers:
	key_compare key_comp(void) const
	{
		read_lock_type lk(_mutex);
		return base_type::key_comp();
	}

	value_compare value_comp(void) const
	{
		read_lock_type lk(_mutex);
		return base_type::value_comp();
	}

	// operations:
	size_type count(const key_type& key) const
	{
		read_lock_type lk(_mutex);
		return base_type::count(key);
	}

	// allocator:
	alloc_type get_allocator(void) const
	{
		read_lock_type lk(_mutex);
		return base_type::get_allocator();
	}

	// safe other
	bool is_exists(const key_type& key) const
	{
		read_lock_type lk(_mutex);
		return find(key) != base_type::end();
	}

	void copy_to_base(base_type& out) const
	{
		read_lock_type lk(_mutex);
		const base_type& base = *this;
		out = base;
	}

	// use handler
	template<typename Handler>
	bool use_handler(const key_type& key, const Handler& handler)
	{
		write_lock_type lk(_mutex);
		iterator iter = base_type::find(key);
		if(iter == base_type::end())
		{
			return false;
		}

		handler(iter);
		return true;
	}

	template<typename Handler>
	bool use_handler(const key_type& key, const Handler& handler) const
	{
		read_lock_type lk(_mutex);
		const_iterator iter = base_type::find(key);
		if(iter == base_type::end())
		{
			return false;
		}

		handler(iter);
		return true;
	}

	template<typename Handler, typename Return_Handler>
	bool use_handler(const key_type& key, const Handler& handler, const Return_Handler& ret)
	{
		write_lock_type lk(_mutex);
		iterator iter = base_type::find(key);
		if(iter == base_type::end())
		{
			return false;
		}

		handler(iter, ret);
		return true;
	}

	template<typename Handler, typename Return_Handler>
	bool use_handler(const key_type& key, const Handler& handler, const Return_Handler& ret) const
	{
		read_lock_type lk(_mutex);
		const_iterator iter = base_type::find(key);
		if(iter == base_type::end())
		{
			return false;
		}

		handler(iter, ret);
		return true;
	}

	template<typename Handler>
	typename Handler::result_type use_handler(const Handler& handler)
	{
		write_lock_type lk(_mutex);

		base_type& base = *this;
		return handler(base);
	}

	template<typename Handler>
	typename Handler::result_type use_handler(const Handler& handler) const
	{
		read_lock_type lk(_mutex);

		const base_type& base = *this;
		return handler(base);
	}

	template<typename Handler, typename Return_Handler>
	typename Handler::result_type use_handler(const Handler& handler, const Return_Handler& ret)
	{
		write_lock_type lk(_mutex);

		base_type& base = *this;
		return handler(base, ret);
	}

	template<typename Handler, typename Return_Handler>
	typename Handler::result_type use_handler(const Handler& handler, const Return_Handler& ret) const
	{
		read_lock_type lk(_mutex);

		const base_type& base = *this;
		return handler(base, ret);
	}

	template<typename Handler>
	void use_handler_of_all(const Handler& handler)
	{
		write_lock_type lk(_mutex);
		for(iterator i = base_type::begin(), isize = base_type::end(); i != isize; ++i)
		{
			handler(i);
		}
	}

	template<typename Handler>
	void use_handler_of_all(const Handler& handler) const
	{
		read_lock_type lk(_mutex);
		for(const_iterator i = base_type::begin(), isize = base_type::end(); i != isize; ++i)
		{
			handler(i);
		}
	}

	template<typename Handler, typename Condition_Handler>
	void use_handler_of_all(const Handler& handler, const Condition_Handler& cdt_handler)
	{
		write_lock_type lk(_mutex);
		for(iterator i = base_type::begin(), isize = base_type::end(); i != isize; ++i)
		{
			if(cdt_handler(i))
			{
				handler(i);
			}
		}
	}

	template<typename Handler, typename Condition_Handler>
	void use_handler_of_all(const Handler& handler, const Condition_Handler& cdt_handler) const
	{
		read_lock_type lk(_mutex);
		for(const_iterator i = base_type::begin(), isize = base_type::end(); i != isize; ++i)
		{
			if(cdt_handler(i))
			{
				handler(i);
			}
		}
	}

	template<typename Handler>
	void use_handler_of_reverse_all(const Handler& handler)
	{
		write_lock_type lk(_mutex);
		for(reverse_iterator i = base_type::rbegin(), isize = base_type::rend(); i != isize; ++i)
		{
			handler(i);
		}
	}

	template<typename Handler>
	void use_handler_of_reverse_all(const Handler& handler) const
	{
		read_lock_type lk(_mutex);
		for(const_reverse_iterator i = base_type::rbegin(), isize = base_type::rend(); i != isize; ++i)
		{
			handler(i);
		}
	}

	template<typename Handler, typename Condition_Handler>
	void use_handler_of_reverse_all(const Handler& handler, const Condition_Handler& cdt_handler)
	{
		write_lock_type lk(_mutex);
		for(reverse_iterator i = base_type::rbegin(), isize = base_type::rend(); i != isize; ++i)
		{
			if(cdt_handler(i))
			{
				handler(i);
			}
		}
	}

	template<typename Handler, typename Condition_Handler>
	void use_handler_of_reverse_all(const Handler& handler, const Condition_Handler& cdt_handler) const
	{
		read_lock_type lk(_mutex);
		for(const_reverse_iterator i = base_type::rbegin(), isize = base_type::rend(); i != isize; ++i)
		{
			if(cdt_handler(i))
			{
				handler(i);
			}
		}
	}

private:
	mutable mutex_type _mutex;
};

} // namespace safe_container
} // namespace yggr

//--------------------------------------------------support std::swap-------------------------------------
namespace std
{
	YGGR_PP_SAFE_CONTAINER_SWAP(5, 4, yggr::safe_container::safe_multimap)
} // namespace std

//-------------------------------------------------support boost::swap-----------------------------------
namespace boost
{
	YGGR_PP_SAFE_CONTAINER_SWAP(5, 4, yggr::safe_container::safe_multimap)
} // namespace boost

#endif // __YGGR_SAFE_CONTAINER_SAFE_MAP_HPP__
