//queue.hpp

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

#ifndef __YGGR_CONTAINER_QUEUE_HPP__
#define __YGGR_CONTAINER_QUEUE_HPP__

#include <queue>
#include <compatibility/stl_switch.hpp>
#include <boost/utility/swap.hpp>
#include <boost/container/deque.hpp>
#include <boost/container/vector.hpp>
#include <yggr/utility/swap.hpp>
#include <yggr/move/move.hpp>
#include <yggr/container/detail/swap_def.hpp>

namespace yggr
{
namespace container
{

template<	typename Val,
			typename Alloc = std::allocator<Val>,
			typename Container = boost::container::deque<Val, Alloc>
		>
class queue
    : public std::queue<Val, Container>
{
public:
	typedef Val val_type;
	typedef Alloc alloc_type;
	typedef Container container_type;

public:
	typedef std::queue<val_type, container_type> base_type;

	//typedef typename base_type container_type container_type;
	typedef typename base_type::value_type value_type;
	typedef typename base_type::size_type size_type;
	typedef typename base_type::reference reference;
	typedef typename base_type::const_reference const_reference;

private:
	typedef queue this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	queue(void)
	{
	}

	explicit queue(const alloc_type& alloc)
		: base_type(container_type(alloc))
	{
	}

	explicit queue(BOOST_RV_REF(container_type) cont)
		: base_type(boost::forward<container_type>(cont))
	{
	}

	explicit queue(const container_type& cont)
		: base_type(cont)
	{
	}

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	explicit queue(BOOST_RV_REF(base_type) right)
		: base_type(boost::forward<base_type>(right))
	{
	}
#else
	explicit queue(BOOST_RV_REF(base_type) right)
	{
		base_type& right_ref = right;
		using boost::swap;
		swap(base_type::c, right_ref.c);
		//base_type::swap(right_ref);
	}
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

	explicit queue(const base_type& right)
		: base_type(right)
	{
	}

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	queue(BOOST_RV_REF(this_type) right)
		: base_type(boost::forward<this_type>(right))
	{
	}
#else
	queue(BOOST_RV_REF(this_type) right)
	{
		base_type& right_ref = right;
		using boost::swap;
		swap(base_type::c, right_ref.c);
		//base_type::swap(right_ref);
	}
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

	queue(const this_type& right)
		: base_type(right)
	{
	}

	~queue(void)
	{
	}

	this_type& operator=(BOOST_RV_REF(base_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		base_type::operator=(boost::forward<base_type>(right));
#else
		base_type& right_ref = right;
		using boost::swap;
		swap(base_type::c, right_ref.c);
		//base_type::swap(right_ref);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		return *this;
	}

	this_type& operator=(const base_type& right)
	{
		base_type::operator=(right);
		return *this;
	}

	this_type& operator=(BOOST_RV_REF(this_type) right)
	{

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		base_type::operator=(boost::forward<this_type>(right));
#else
		this_type& right_ref = right;
		using boost::swap;
		swap(base_type::c, right_ref.c);
		//base_type::swap(right_ref);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) {return *this;}
		base_type& base = *this;
		base_type rbase(right);
		boost::swap(base, rbase);
		return *this;
	}

	void swap(BOOST_RV_REF(base_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		//base_type::swap(boost::forward<base_type>(right));
		base_type::swap(right);
#else
		base_type& right_ref = right;
		using boost::swap;
		swap(base_type::c, right_ref.c);
		//base_type::swap(right_ref);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
	}

	void swap(base_type& right)
	{
		base_type::swap(right);
	}

	void swap(BOOST_RV_REF(this_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		//base_type::swap(boost::forward<base_type>(right));
		base_type::swap(right);
#else
		this_type& right_ref = right;
		using boost::swap;
		swap(base_type::c, right_ref.c);
		//base_type::swap(right_ref);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}
		//base_type::swap(right);
		base_type& base = *this;
		base_type& rbase = right;
		boost::swap(base, rbase);
	}

	alloc_type get_allocator(void) const
	{
#	if YGGR_USE_STL_VER == YGGR_USE_MICROSOFT_STL
		return base_type::_Get_container().get_allocator();
#	elif YGGR_USE_STL_VER == YGGR_USE_GNUC_STL
		return base_type::c.get_allocator();
#	else
#		error "please add now stl ver code"
#	endif //  YGGR_USE_STL_VER
	}
};

// queue TEMPLATE FUNCTIONS
template< YGGR_PP_TEMPLATE_PARAMS_TYPES(3, typename T) >
inline
bool operator==(const YGGR_PP_TEMPLATE_TYPE(queue, YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T) )& l,
					const YGGR_PP_TEMPLATE_TYPE(queue, YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T) )& r)
{	// test for queue equality
#	if YGGR_USE_STL_VER == YGGR_USE_MICROSOFT_STL
	return (l._Get_container() == r._Get_container());
#	elif YGGR_USE_STL_VER == YGGR_USE_GNUC_STL
    return l == r;
#	else
#		error "please add now stl ver code"
#	endif //  YGGR_USE_STL_VER
}

template< YGGR_PP_TEMPLATE_PARAMS_TYPES(3, typename T) >
inline
bool operator==(const typename YGGR_PP_TEMPLATE_TYPE(queue, YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T) )::base_type& l,
				const YGGR_PP_TEMPLATE_TYPE(queue, YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T) )& r)
{	// test for queue equality
#	if YGGR_USE_STL_VER == YGGR_USE_MICROSOFT_STL
	return (l._Get_container() == r._Get_container());
#	elif YGGR_USE_STL_VER == YGGR_USE_GNUC_STL
    return l == r;
#	else
#		error "please add now stl ver code"
#	endif //  YGGR_USE_STL_VER
}

template< YGGR_PP_TEMPLATE_PARAMS_TYPES(3, typename T) >
inline
bool operator!=(const YGGR_PP_TEMPLATE_TYPE(queue, YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T) )& l,
				const YGGR_PP_TEMPLATE_TYPE(queue, YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T) )& r)
{	// test for queue inequality
	return (!(l == r));
}

template< YGGR_PP_TEMPLATE_PARAMS_TYPES(3, typename T) >
inline
bool operator!=(const typename YGGR_PP_TEMPLATE_TYPE(queue, YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T) )::base_type& l,
				const YGGR_PP_TEMPLATE_TYPE(queue, YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T) )& r)
{	// test for queue inequality
	return (!(l == r));
}

template< YGGR_PP_TEMPLATE_PARAMS_TYPES(3, typename T) >
inline
bool operator<(const YGGR_PP_TEMPLATE_TYPE(queue, YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T) )& l,
				const YGGR_PP_TEMPLATE_TYPE(queue, YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T) )& r)
{	// test if l < r for queues
#	if YGGR_USE_STL_VER == YGGR_USE_MICROSOFT_STL
	return (l._Get_container() < r._Get_container());
#	elif YGGR_USE_STL_VER == YGGR_USE_GNUC_STL
    return l < r;
#	else
#		error "please add now stl ver code"
#	endif //  YGGR_USE_STL_VER
}

template< YGGR_PP_TEMPLATE_PARAMS_TYPES(3, typename T) >
inline
bool operator<(const typename YGGR_PP_TEMPLATE_TYPE(queue, YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T) )::base_type& l,
				const YGGR_PP_TEMPLATE_TYPE(queue, YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T) )& r)
{	// test if l < r for queues
#	if YGGR_USE_STL_VER == YGGR_USE_MICROSOFT_STL
	return (l._Get_container() < r._Get_container());
#	elif YGGR_USE_STL_VER == YGGR_USE_GNUC_STL
    return l < r;
#	else
#		error "please add now stl ver code"
#	endif //  YGGR_USE_STL_VER
}

template< YGGR_PP_TEMPLATE_PARAMS_TYPES(3, typename T) >
inline
bool operator>(const YGGR_PP_TEMPLATE_TYPE(queue, YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T) )& l,
				const YGGR_PP_TEMPLATE_TYPE(queue, YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T) )& r)
{	// test if l > r for queues
	return (r < l);
}

template< YGGR_PP_TEMPLATE_PARAMS_TYPES(3, typename T) >
inline
bool operator>(const typename YGGR_PP_TEMPLATE_TYPE(queue, YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T) )::base_type& l,
				const YGGR_PP_TEMPLATE_TYPE(queue, YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T) )& r)
{	// test if l > r for queues
	return (r < l);
}

template< YGGR_PP_TEMPLATE_PARAMS_TYPES(3, typename T) >
inline
bool operator<=(const YGGR_PP_TEMPLATE_TYPE(queue, YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T) )& l,
					const YGGR_PP_TEMPLATE_TYPE(queue, YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T) )& r)
{	// test if l <= r for queues
	return (!(r < l));
}

template< YGGR_PP_TEMPLATE_PARAMS_TYPES(3, typename T) >
inline
bool operator<=(const typename YGGR_PP_TEMPLATE_TYPE(queue, YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T) )::base_type& l,
					const YGGR_PP_TEMPLATE_TYPE(queue, YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T) )& r)
{	// test if l <= r for queues
	return (!(r < l));
}

template< YGGR_PP_TEMPLATE_PARAMS_TYPES(3, typename T) >
inline
bool operator>=(const YGGR_PP_TEMPLATE_TYPE(queue, YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T) )& l,
				const YGGR_PP_TEMPLATE_TYPE(queue, YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T) )& r)
{	// test if l >= r for queues
	return (!(l < r));
}

template< YGGR_PP_TEMPLATE_PARAMS_TYPES(3, typename T) >
inline
bool operator>=(const typename YGGR_PP_TEMPLATE_TYPE(queue, YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T) )::base_type& l,
				const YGGR_PP_TEMPLATE_TYPE(queue, YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T) )& r)
{	// test if l >= r for queues
	return (!(l < r));
}

} // namespace container
} // namespace yggr

//----------------------------------support std::swap------------------------------
namespace std
{
	YGGR_PP_CONTAINER_SWAP(3, yggr::container::queue)
} // namespace std

//---------------------------------support boost::swap-----------------------------
namespace boost
{
	YGGR_PP_CONTAINER_SWAP(3, yggr::container::queue)
} // namespace boost


//----------------------priority_queue----------------------------------

namespace yggr
{
namespace container
{
template<	typename Val,
			typename Alloc = std::allocator<Val>,
			typename Container = boost::container::vector<Val, Alloc>,
			typename Cmp = std::less<Val>
		>
class priority_queue
    : public std::priority_queue<Val, Container, Cmp>
{
public:
	typedef Val val_type;
	typedef Alloc alloc_type;
	typedef Container container_type;
	typedef Cmp cmp_type;

public:
	typedef  std::priority_queue<val_type, container_type, cmp_type> base_type;

	//typedef typename base_tye::container_type container_type;
	typedef typename base_type::value_type value_type;
	typedef typename base_type::size_type size_type;
	typedef typename base_type::reference reference;
	typedef typename base_type::const_reference const_reference;

private:
	typedef priority_queue this_type;

public:
	priority_queue(void)
	{
	}

	explicit priority_queue(const cmp_type& cmp)
		: base_type(cmp)
	{
	}

	explicit priority_queue(const alloc_type& alloc)
		: base_type(cmp_type(), container_type(alloc))
	{
	}

	explicit priority_queue(const container_type& cont)
		: base_type(cmp_type(), cont)
	{
	}

	explicit priority_queue(const cmp_type& cmp, const alloc_type& alloc)
		: base_type(cmp, alloc)
	{
	}


	explicit priority_queue(const cmp_type& cmp, BOOST_RV_REF(container_type) cont)
		: base_type(cmp, boost::forward<container_type>(cont))
	{
	}

	explicit priority_queue(const cmp_type& cmp, const container_type& cont)
		: base_type(cmp, cont)
	{
	}

	template<typename InIter>
	explicit priority_queue(InIter start, InIter last)
		: base_type(start, last)
	{
	}

	template<typename InIter>
	explicit priority_queue(InIter start, InIter last, const alloc_type& alloc)
		: base_type(start, last, cmp_type(), container_type(alloc))
	{
	}

	template<typename InIter>
	explicit priority_queue(InIter start, InIter last, const cmp_type& cmp)
		: base_type(start, last, cmp)
	{
	}

	template<typename InIter>
	explicit priority_queue(InIter start, InIter last, const cmp_type& cmp, const alloc_type& alloc)
		: base_type(start, last, cmp, container_type(alloc))
	{
	}

	template<typename InIter>
	explicit priority_queue(InIter start, InIter last, const cmp_type& cmp, BOOST_RV_REF(container_type) cont)
		: base_type(start, last, cmp, boost::forward<container_type>(cont))
	{
	}

	template<typename InIter>
	explicit priority_queue(InIter start, InIter last, const cmp_type& cmp, const container_type& cont)
		: base_type(start, last, cmp, cont)
	{
	}

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	priority_queue(BOOST_RV_REF(base_type) right)
		: base_type(boost::forward<base_type>(right))
	{
	}
#else
	priority_queue(BOOST_RV_REF(base_type) right)
	{
		base_type& right_ref = right;
		base_type::swap(right_ref);
	}
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

	priority_queue(const base_type& right)
		: base_type(right)
	{
	}

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	priority_queue(BOOST_RV_REF(this_type) right)
		: base_type(boost::forward<this_type>(right))
	{
	}
#else
	priority_queue(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		base_type::swap(right_ref);
	}
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

	priority_queue(const this_type& right)
		: base_type(right)
	{
	}

	~priority_queue(void)
	{
	}

	this_type& operator=(const base_type& right)
	{
		base_type::operator=(right);
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) {return *this;}
		base_type rbase(right);
		base_type& base = *this;
		boost::swap(base, rbase);
		return *this;
	}

	this_type& operator=(BOOST_RV_REF(base_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		base_type::operator=(boost::forward<base_type>(right));
#else
		base_type& right_ref = right;
		base_type::swap(right_ref);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		return *this;
	}

	this_type& operator=(BOOST_RV_REF(this_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		base_type::operator=(boost::forward<base_type>(right));
#else
		this_type& right_ref = right;
		base_type::swap(right_ref);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		return *this;
	}

	void swap(BOOST_RV_REF(base_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		//base_type::swap(boost::forward<base_type>(right));
		base_type::swap(right);
#else
		base_type& right_ref = right;
		base_type::swap(right_ref);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
	}

	void swap(BOOST_RV_REF(this_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		base_type::swap(right);
#else
		this_type& right_ref = right;
		base_type::swap(right);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
	}

	void swap(base_type& right)
	{
		base_type::swap(right);
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}
	    base_type& base = *this;
		base_type& rbase = right;
		//base_type::swap(rbase);
		boost::swap(base, rbase);
	}

	alloc_type get_allocator(void) const
	{
#	if YGGR_USE_STL_VER == YGGR_USE_MICROSOFT_STL
		return base_type::c.get_allocator();
#	elif YGGR_USE_STL_VER == YGGR_USE_GNUC_STL
		return base_type::c.get_allocator();
#	else
#		error "please add now stl ver code"
#	endif //  YGGR_USE_STL_VER
	}

	cmp_type value_comp(void) const
	{
#	if YGGR_USE_STL_VER == YGGR_USE_MICROSOFT_STL
		return base_type::comp;
#	elif YGGR_USE_STL_VER == YGGR_USE_GNUC_STL
		return base_type::comp;
#	else
#		error "please add now stl ver code"
#	endif //  YGGR_USE_STL_VER
	}
};

} // container
} // namespace yggr

//----------------------------------support std::swap------------------------------
namespace std
{
	YGGR_PP_CONTAINER_SWAP(4, yggr::container::priority_queue)
} // namespace std

//---------------------------------support boost::swap-----------------------------
namespace boost
{
	YGGR_PP_CONTAINER_SWAP(4, yggr::container::priority_queue)
} // namespace boost

#endif //__YGGR_CONTAINER_QUEUE_HPP__
