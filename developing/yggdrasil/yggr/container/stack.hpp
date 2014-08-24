// stack.hpp

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

#ifndef __YGGR_CONTAINER_STACK_HPP__
#define __YGGR_CONTAINER_STACK_HPP__

#include <stack>
#include <compatibility/stl_switch.hpp>
#include <yggr/utility/swap.hpp>
#include <boost/container/deque.hpp>
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
class stack : public std::stack<Val, Container>
{
public:
	typedef Val val_type;
	typedef Alloc alloc_type;
	typedef Container container_type;

public:
	typedef std::stack<Val, Container> base_type;

	//typedef typename base_type::container_type container_type;
	typedef typename base_type::value_type value_type;
	typedef typename base_type::size_type size_type;
	typedef typename base_type::reference reference;
	typedef typename base_type::const_reference const_reference;

private:
	typedef stack this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	stack(void)
	{
	}

	explicit stack(const alloc_type& alloc)
		: base_type(container_type(alloc))
	{
	}

	explicit stack(const container_type& cont)
		: base_type(cont)
	{
	}

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	explicit stack(BOOST_RV_REF(base_type) right)
		: base_type(boost::forward<base_type>(right))
	{
	}
#else
	explicit stack(BOOST_RV_REF(base_type) right)
	{
		base_type& right_ref = right;
		using boost::swap;
		swap(base_type::c, right_ref.c);
		//base_type::swap(right_ref);
	}
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

	explicit stack(const base_type& right)
		: base_type(right)
	{
	}

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	stack(BOOST_RV_REF(this_type) right)
		: base_type(boost::forward<this_type>(right))
	{
	}
#else
	stack(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		using boost::swap;
		swap(base_type::c, right_ref.c);
		//base_type::swap(right_ref);
	}
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

	stack(const this_type& right)
		: base_type(right)
	{
	}

	~stack(void)
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
		base_type::operator=(boost::forward<base_type>(right));
#else
		this_type& right_ref = right;
		using boost::swap;
		swap(base_type::c, right_ref.c);
		//base_type::swap(right);
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
#endif // #ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
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

// stack TEMPLATE FUNCTIONS
template< YGGR_PP_TEMPLATE_PARAMS_TYPES(3, typename T) >
inline
bool operator==(const YGGR_PP_TEMPLATE_TYPE(stack, YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T) )& l,
					const YGGR_PP_TEMPLATE_TYPE(stack, YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T) )& r)
{	// test for stack equality
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
bool operator!=(const YGGR_PP_TEMPLATE_TYPE(stack, YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T) )& l,
					const YGGR_PP_TEMPLATE_TYPE(stack, YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T) )& r)
{	// test for stack inequality
	return (!(l == r));
}

template< YGGR_PP_TEMPLATE_PARAMS_TYPES(3, typename T) >
inline
bool operator<(const YGGR_PP_TEMPLATE_TYPE(stack, YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T) )& l,
				const YGGR_PP_TEMPLATE_TYPE(stack, YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T) )& r)
{	// test if l < r for stacks
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
bool operator>(const YGGR_PP_TEMPLATE_TYPE(stack, YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T) )& l,
				const YGGR_PP_TEMPLATE_TYPE(stack, YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T) )& r)
{	// test if l > r for stacks
	return (r < l);
}

template< YGGR_PP_TEMPLATE_PARAMS_TYPES(3, typename T) >
inline
bool operator<=(const YGGR_PP_TEMPLATE_TYPE(stack, YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T) )& l,
					const YGGR_PP_TEMPLATE_TYPE(stack, YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T) )& r)
{	// test if l <= r for stacks
	return (!(r < l));
}

template< YGGR_PP_TEMPLATE_PARAMS_TYPES(3, typename T) >
inline
bool operator>=(const YGGR_PP_TEMPLATE_TYPE(stack, YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T) )& l,
					const YGGR_PP_TEMPLATE_TYPE(stack, YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T) )& r)
{	// test if l >= r for stacks
	return (!(l < r));
}

} // namespace container
} // namespace yggr

//----------------------------------support std::swap------------------------------
namespace std
{
	YGGR_PP_CONTAINER_SWAP(3, yggr::container::stack)
} // namespace std

//---------------------------------support boost::swap-----------------------------
namespace boost
{
	YGGR_PP_CONTAINER_SWAP(3, yggr::container::stack)
} // namespace boost

#endif // __YGGR_CONTAINER_STACK_HPP__
