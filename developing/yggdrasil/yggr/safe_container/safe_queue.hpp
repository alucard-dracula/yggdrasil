//safe_queue.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_SAFE_QUEUE_HPP__
#define __YGGR_SAFE_CONTAINER_SAFE_QUEUE_HPP__

#include <algorithm>

#include <boost/container/deque.hpp>
#include <boost/thread/mutex.hpp>
#include <yggr/move/move.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/nonable/noncopyable.hpp>
#include <yggr/nonable/nonmoveable.hpp>
#include <yggr/helper/mutex_def_helper.hpp>
#include <yggr/container/queue.hpp>
#include <yggr/safe_container/detail/swap_def.hpp>

namespace yggr
{
namespace safe_container
{

template<	typename Val,
			typename Mutex = boost::mutex,
			typename Alloc = std::allocator<Val>,
			typename Container = boost::container::deque<Val, Alloc>,
			template <typename _Val, typename _Alloc, typename _Container> class Queue = yggr::container::queue
		>
class safe_queue
	: protected Queue<Val, Alloc, Container>,
		private nonable::noncopyable,
		private nonable::nonmoveable
{
public:
	typedef Val val_type;
	typedef Alloc alloc_type;
	typedef Container container_type;

public:
	typedef Queue<val_type, alloc_type, container_type> base_type;

	//typedef typename base_type container_type container_type;
	typedef typename base_type::value_type value_type;
	typedef typename base_type::size_type size_type;
	typedef typename base_type::reference reference;
	typedef typename base_type::const_reference const_reference;

private:
	typedef Mutex mutex_type;
	typedef helper::mutex_def_helper<mutex_type> mutex_def_helper_type;
	typedef typename mutex_def_helper_type::read_lock_type read_lock_type;
	typedef typename mutex_def_helper_type::write_lock_type write_lock_type;

	typedef safe_queue this_type;

public:
	safe_queue(void)
	{
	}

	explicit safe_queue(const alloc_type& alloc)
		: base_type(alloc)
	{
	}

	explicit safe_queue(BOOST_RV_REF(container_type) cont)
		: base_type(boost::forward<container_type>(cont))
	{
	}

	explicit safe_queue(const container_type& cont)
		: base_type(cont)
	{
	}

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	explicit safe_queue(BOOST_RV_REF(base_type) right)
		: base_type(boost::forward<base_type>(right))
	{
	}
#else
	explicit safe_queue(BOOST_RV_REF(base_type) right)
	{
		base_type& right_ref = right;
		base_type::swap(right_ref);
	}
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

	explicit safe_queue(const base_type& right)
		: base_type(right)
	{
	}

	~safe_queue(void)
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

	void push(BOOST_RV_REF(val_type) val)
	{
		write_lock_type lk(_mutex);
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		base_type::push(boost::forward<val_type>(val));
#else
		const val_type& val_cref = val;
		base_type::push(val_cref);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
	}

	void push(const val_type& val)
	{
		write_lock_type lk(_mutex);
		base_type::push(val);
	}

	bool pop(void)
	{
		write_lock_type lk(_mutex);
		if(base_type::empty())
		{
			return false;
		}

		base_type::pop();
		return true;
	}

	bool pop(val_type& val)
	{
		write_lock_type lk(_mutex);
		if(base_type::empty())
		{
			return false;
		}

		val = yggr::move::move_helper::forced_move(base_type::front());
		//boost::swap(val, base_type::front());
		//val = base_type::front();
		base_type::pop();
		return true;
	}

//	template<typename Value>
//	void emplace(BOOST_RV_REF(Value) val)
//	{
//		typedef Value now_val_type;
//		write_lock_type lk(_mutex);
//#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
//		base_type::emplace(boost::forward<now_val_type>(val));
//#else
//		const now_val_type& val_cref = val;
//		base_type::emplace(val_cref);
//#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
//	}

	void clear(void)
	{
		write_lock_type lk(_mutex);
		base_type tmp(base_type::get_allocator());
		base_type::swap(tmp);
	}

	void space_clear(void)
	{
		base_type tmp(base_type::get_allocator());
		write_lock_type lk(_mutex);
		if(!base_type::empty())
		{
			return;
		}

		base_type::swap(tmp);
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
#endif //YGGR_NO_CXX11_RVALUE_REFERENCES
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

	alloc_type get_allocator(void) const
	{
		read_lock_type lk(_mutex);
		return base_type::get_allocator();
	}

	// use handler:
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

private:
	mutable mutex_type _mutex;
};

} // namespace safe_container
} // namespace yggr

//--------------------------------------------------support std::swap-------------------------------------
namespace std
{
	YGGR_PP_SAFE_CONTAINER_SWAP(4, 3, yggr::safe_container::safe_queue)
} // namespace std

//-------------------------------------------------support boost::swap-----------------------------------
namespace boost
{
	YGGR_PP_SAFE_CONTAINER_SWAP(4, 3, yggr::safe_container::safe_queue)
} // namespace boost

//-------------------------safe_priority_queue--------------------------
namespace yggr
{
namespace safe_container
{
template<	typename Val,
			typename Mutex = boost::mutex,
			typename Alloc = std::allocator<Val>,
			typename Container = boost::container::vector<Val, Alloc>,
			typename Cmp = std::less<Val>,
			template <typename _Val, typename _Alloc, typename _Container, typename _Cmp> class Priority_Queue = yggr::container::priority_queue
		>
class safe_priority_queue
	: public Priority_Queue<Val, Alloc, Container, Cmp>,
		private nonable::noncopyable,
		private nonable::nonmoveable
{
public:
	typedef Val val_type;
	typedef Alloc alloc_type;
	typedef Container container_type;
	typedef Cmp cmp_type;

public:
	typedef Priority_Queue<val_type, alloc_type, container_type, cmp_type> base_type;

	//typedef typename base_tye::container_type container_type;
	typedef typename base_type::value_type value_type;
	typedef typename base_type::size_type size_type;
	typedef typename base_type::reference reference;
	typedef typename base_type::const_reference const_reference;

private:
	typedef Mutex mutex_type;
	typedef helper::mutex_def_helper<mutex_type> mutex_def_helper_type;
	typedef typename mutex_def_helper_type::read_lock_type read_lock_type;
	typedef typename mutex_def_helper_type::write_lock_type write_lock_type;

	typedef safe_priority_queue this_type;

public:
	safe_priority_queue(void)
	{
	}

	explicit safe_priority_queue(const cmp_type& cmp)
		: base_type(cmp)
	{
	}

	explicit safe_priority_queue(const alloc_type& alloc)
		: base_type(alloc)
	{
	}

	safe_priority_queue(const cmp_type& cmp, const alloc_type& alloc)
		: base_type(cmp, alloc)
	{
	}

	explicit safe_priority_queue(BOOST_RV_REF(container_type) cont)
		: base_type(boost::forward<container_type>(cont))
	{
	}

	explicit safe_priority_queue(const container_type& cont)
		: base_type(cont)
	{
	}

	safe_priority_queue(const cmp_type& cmp, BOOST_RV_REF(container_type) cont)
		: base_type(cmp, boost::forward<container_type>(cont))
	{
	}

	safe_priority_queue(const cmp_type& cmp, const container_type& cont)
		: base_type(cmp, cont)
	{
	}

	template<typename InIter>
	safe_priority_queue(InIter start, InIter last)
		: base_type(start, last)
	{
	}

	template<typename InIter>
	safe_priority_queue(InIter start, InIter last, const alloc_type& alloc)
		: base_type(start, last, cmp_type(), alloc)
	{
	}

	template<typename InIter>
	safe_priority_queue(InIter start, InIter last, const cmp_type& cmp)
		: base_type(start, last, cmp)
	{
	}

	template<typename InIter>
	safe_priority_queue(InIter start, InIter last, const cmp_type& cmp, const alloc_type& alloc)
		: base_type(start, last, cmp, alloc)
	{
	}

	template<typename InIter>
	safe_priority_queue(InIter start, InIter last, const cmp_type& cmp, BOOST_RV_REF(container_type) cont)
		: base_type(start, last, cmp, boost::forward<container_type>(cont))
	{
	}

	template<typename InIter>
	safe_priority_queue(InIter start, InIter last, const cmp_type& cmp, const container_type& cont)
		: base_type(start, last, cmp, cont)
	{
	}

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	explicit safe_priority_queue(BOOST_RV_REF(base_type) right)
		: base_type(boost::forward<base_type>(right))
	{
	}
#else
	explicit safe_priority_queue(BOOST_RV_REF(base_type) right)
	{
		base_type& right_ref = right;
		base_type::swap(right_ref);
	}
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

	explicit safe_priority_queue(const base_type& right)
		: base_type(right)
	{
	}

	~safe_priority_queue(void)
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

	bool top(val_type& val) const
	{
		read_lock_type lk(_mutex);
		if(base_type::empty())
		{
			return false;
		}

		val = base_type::top();
		return true;
	}

	void push(BOOST_RV_REF(val_type) val)
	{
		write_lock_type lk(_mutex);
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		base_type::push(boost::forward<val_type>(val));
#else
		const val_type& val_cref= val;
		base_type::push(val_cref);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
	}

	void push(const val_type& val)
	{
		write_lock_type lk(_mutex);
		base_type::push(val);
	}

	bool pop(void)
	{
		write_lock_type lk(_mutex);
		if(base_type::empty())
		{
			return false;
		}

		base_type::pop();
		return true;
	}

	bool pop(val_type& val)
	{
		write_lock_type lk(_mutex);
		if(base_type::empty())
		{
			return false;
		}

		//val = boost::move(base_type::top());
		val = base_type::top();
		base_type::pop();
		return true;
	}

//	template<typename Value>
//	void emplace(BOOST_RV_REF(Value) val)
//	{
//		typedef Value now_val_type;
//		write_lock_type lk(_mutex);
//#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
//		base_type::emplace(boost::forward<now_val_type>(val));
//#else
//		const now_val_type& val_cref = val;
//		base_type::emplace(val_cref);
//#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
//	}

	void clear(void)
	{
		base_type tmp(base_type::get_allocator());
		write_lock_type lk(_mutex);
		base_type::swap(tmp);
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

	alloc_type get_allocator(void) const
	{
		read_lock_type lk(_mutex);
		return base_type::get_allocator();
	}

	cmp_type value_comp(void) const
	{
		read_lock_type lk(_mutex);
		return base_type::value_comp();
	}

	// use handler:
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

private:
	mutable mutex_type _mutex;
};

} //namespace safe_container
} //namespace yggr

//--------------------------------------------------support std::swap-------------------------------------
namespace std
{
	YGGR_PP_SAFE_CONTAINER_SWAP(5, 4, yggr::safe_container::safe_priority_queue)
} // namespace std

//-------------------------------------------------support boost::swap-----------------------------------
namespace boost
{
	YGGR_PP_SAFE_CONTAINER_SWAP(5, 4, yggr::safe_container::safe_priority_queue)
} // namespace boost

#endif //__YGGR_SAFE_CONTAINER_SAFE_QUEUE_HPP__
