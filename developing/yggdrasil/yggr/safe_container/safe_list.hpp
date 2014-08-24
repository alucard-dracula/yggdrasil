//safe_list.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_SAFE_LIST_HPP__
#define __YGGR_SAFE_CONTAINER_SAFE_LIST_HPP__

#include <boost/container/list.hpp>
#include <boost/thread/mutex.hpp>
#include <yggr/move/move.hpp>
#include <boost/bind.hpp>
#include <boost/tuple/tuple.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/nonable/noncopyable.hpp>
#include <yggr/nonable/nonmoveable.hpp>
#include <yggr/helper/mutex_def_helper.hpp>
#include <yggr/safe_container/detail/swap_def.hpp>


namespace yggr
{
namespace safe_container
{


template<typename Val, typename Mutex = boost::mutex,
			typename Ax = std::allocator<Val>,
			template<typename _Val, typename _Ax> class List = boost::container::list
		>
class safe_list
	: protected List<Val, Ax>,
		private nonable::noncopyable,
		private nonable::nonmoveable
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

	typedef Mutex mutex_type;
	typedef helper::mutex_def_helper<mutex_type> mutex_def_helper_type;
	typedef typename mutex_def_helper_type::read_lock_type read_lock_type;
	typedef typename mutex_def_helper_type::write_lock_type write_lock_type;

	typedef safe_list this_type;

public:
	safe_list(void)
	{
	}

	explicit safe_list(const alloc_type& alloc)
		: base_type(alloc)
	{
	}

	explicit safe_list(size_type count)
		: base_type(count)
	{
	}

	safe_list(size_type count, const val_type& val)
		: base_type(count, val)
	{
	}

	safe_list(size_type count, const val_type& val, const alloc_type& alloc)
		: base_type(count, val, alloc)
	{
	}

	template<class InputIterator>
	safe_list(InputIterator first, InputIterator last)
		: base_type(first, last)
	{
	}

	template<class InputIterator>
	safe_list(InputIterator first, InputIterator last, const alloc_type& alloc)
		: base_type(first, last, alloc)
	{
	}

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	explicit safe_list(BOOST_RV_REF(base_type) right)
		: base_type(boost::forward<base_type>(right))
	{
	}
#else
	explicit safe_list(BOOST_RV_REF(base_type) right)
	{
		base_type& right_ref = right;
		base_type::swap(right_ref);
	}
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

	explicit safe_list(const base_type& right)
		: base_type(right)
	{
	}

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	safe_list(BOOST_RV_REF(base_type) right, const alloc_type& alloc)
		: base_type(alloc)
	{
		base_type::operator=(boost::forward<base_type>(right));
	}
#else
	safe_list(BOOST_RV_REF(base_type) right, const alloc_type& alloc)
		: base_type(alloc)
	{
		base_type& right_ref = right;
		base_type::swap(right_ref);
	}
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

	safe_list(const base_type& right, const alloc_type& alloc)
		: base_type(alloc)
	{
	    base_type::assign(right.begin(), right.end());
	}

	~safe_list(void)
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

	void resize(size_type size)
	{
		write_lock_type lk(_mutex);
		base_type::resize(size);
	}

	// element access:
	bool front(val_type& val) const
	{
		read_lock_type lk(_mutex);
		if(base_type::empty())
		{
			return false;
		}

		val = base_type::front();
		return true;
	}

	bool back(val_type& val) const
	{
		read_lock_type lk(_mutex);

		if(base_type::empty())
		{
			return false;
		}

		val = base_type::back();
		return true;
	}

	// modifiers:
	template<typename InputIterator>
	void assign(InputIterator first, InputIterator last)
	{
		write_lock_type lk(_mutex);
		base_type::assign(first, last);
	}

	void assign(size_type size, const val_type& val)
	{
		write_lock_type lk(_mutex);
		base_type::assign(size, val);
	}

	void push_front(BOOST_RV_REF(val_type) val)
	{
		write_lock_type lk(_mutex);
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		base_type::push_front(boost::forward<val_type>(val));
#else
		const val_type& val_cref = val;
		base_type::push_front(val_cref);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
	}

	void push_front(const val_type& val)
	{
		write_lock_type lk(_mutex);
		base_type::push_front(val);
	}

	template<typename Handler>
	void push_front(BOOST_RV_REF(val_type) val, const Handler& handler)
	{
		write_lock_type lk(_mutex);
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		base_type::push_front(boost::forward<val_type>(val));
#else
		const val_type& val_cref = val;
		base_type::push_front(val_cref);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		handler(base_type::front());
	}

	template<typename Handler>
	void push_front(const val_type& val, const Handler& handler)
	{
		write_lock_type lk(_mutex);
		base_type::push_front(val);
		handler(base_type::front());
	}

	bool pop_front(void)
	{
		write_lock_type lk(_mutex);
		if(base_type::empty())
		{
			return false;
		}

		base_type::pop_front();
		return true;
	}

	bool pop_front(val_type& val)
	{
		write_lock_type lk(_mutex);
		if(base_type::empty())
		{
			return false;
		}

		val = yggr::move::move_helper::forced_move(base_type::front());
		//boost::swap(val, base_type::front());
		//val = base_type::front();
		base_type::pop_front();
		return true;
	}

	void push_back(BOOST_RV_REF(val_type) val)
	{
		write_lock_type lk(_mutex);
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		base_type::push_back(boost::forward<val_type>(val));
#else
		const val_type& val_cref = val;
		base_type::push_back(val_cref);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
	}

	void push_back(const val_type& val)
	{
		write_lock_type lk(_mutex);
		base_type::push_back(val);
	}

	template<typename Handler>
	void push_back(BOOST_RV_REF(val_type) val, const Handler& handler)
	{
		write_lock_type lk(_mutex);
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		base_type::push_back(boost::forward<val_type>(val));
#else
		const val_type& val_cref = val;
		base_type::push_back(val_cref);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		handler(base_type::back());
	}

	template<typename Handler>
	void push_back(const val_type& val, const Handler& handler)
	{
		write_lock_type lk(_mutex);
		base_type::push_back(val);
		handler(base_type::back());
	}

	bool pop_back(void)
	{
		write_lock_type lk(_mutex);
		if(base_type::empty())
		{
			return false;
		}
		base_type::pop_back();
		return true;
	}

	bool pop_back(val_type& val)
	{
		write_lock_type lk(_mutex);
		if(base_type::empty())
		{
			return false;
		}

		val = yggr::move::move_helper::forced_move(base_type::back());
		//boost::swap(val, base_type::back());
		//val = base_type::back();
		base_type::pop_back();
		return true;
	}

	void swap(BOOST_RV_REF(base_type) right)
	{
		write_lock_type lk(_mutex);
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		//base_type::swap(boost::forward<base_type>(right));
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
		base_type tmp(base_type::get_allocator());
		write_lock_type lk(_mutex);
		base_type::swap(tmp);
	}

	// operations:
	void remove(const val_type& val)
	{
		write_lock_type lk(_mutex);
		base_type::remove(val);
	}

	template<typename Predicate>
	void remove_if(Predicate pred)
	{
		write_lock_type lk(_mutex);
		base_type::remove_if(pred);
	}

	void unique(void)
	{
		write_lock_type lk(_mutex);
		base_type::unique();
	}

	template<typename Cmp>
	void unique(Cmp cmp)
	{
		write_lock_type lk(_mutex);
		base_type::unique(cmp);
	}

	void merge(BOOST_RV_REF(base_type) right)
	{
		write_lock_type lk(_mutex);
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		base_type::merge(boost::forward<base_type>(right));
#else
		base_type& right_ref = right;
		base_type::merge(right_ref);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
	}

	void merge(base_type& right)
	{
		write_lock_type lk(_mutex);
		base_type::merge(right);
	}

	template<typename Traits>
	void merge(BOOST_RV_REF(base_type) right, Traits comp)
	{
		write_lock_type lk(_mutex);

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		base_type::merge(boost::forward<base_type>(right), comp);
#else
		base_type& right_ref = right;
		base_type::merge(right_ref, comp);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
	}

	template<typename Traits>
	void merge(base_type& right, Traits comp)
	{
		write_lock_type lk(_mutex);
		base_type::merge(right, comp);
	}

	void merge(this_type& right)
	{
		write_lock_type lk(_mutex);
		base_type& base = *this;
		right.use_handler(boost::bind(&this_type::handler_merge_eins, this, boost::ref(base), _1));
	}

	template<typename Traits>
	void merge(this_type& right, Traits comp)
	{
		typedef Traits cmp_type;
		typedef boost::tuple<cmp_type> cmp_tuple_type;
		write_lock_type lk(_mutex);
		base_type& base = *this;
		right.use_handler(boost::bind(&this_type::handler_merge_zwei<cmp_tuple_type>, this,
										boost::ref(base), _1, boost::make_tuple(comp)));
	}

	void sort(void)
	{
		write_lock_type lk(_mutex);
		base_type::sort();
	}

	template<typename Cmp>
	void sort(Cmp cmp)
	{
		write_lock_type lk(_mutex);
		base_type::sort(cmp);
	}

	void reverse(void)
	{
		write_lock_type lk(_mutex);
		base_type::reverse();
	}

	// allocator:
	alloc_type get_allocator(void) const
	{
		read_lock_type lk(_mutex);
		return base_type::get_allocator();
	}

	// safe other

	bool is_exists(const val_type& val) const
	{
		read_lock_type lk(_mutex);
		return std::find(base_type::begin(), base_type::end(), val) != base_type::end();
	}

	// use handler
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
	void handler_merge_eins(base_type& l, base_type& r)
	{
		l.merge(r);
	}

	template<typename Traits>
	void handler_merge_zwei(base_type& l, base_type& r, Traits cmp)
	{
		l.merge(r, boost::get<0>(cmp));
	}

private:
	mutable mutex_type _mutex;
};

} // namespace safe_container
} // namespace yggr

//--------------------------------------------------support std::swap-------------------------------------
namespace std
{
	YGGR_PP_SAFE_CONTAINER_SWAP(3, 2, yggr::safe_container::safe_list)
} // namespace std

//-------------------------------------------------support boost::swap-----------------------------------
namespace boost
{
	YGGR_PP_SAFE_CONTAINER_SWAP(3, 2, yggr::safe_container::safe_list)
} // namespace boost

#endif //__YGGR_SAFE_CONTAINER_SAFE_LIST_HPP__
