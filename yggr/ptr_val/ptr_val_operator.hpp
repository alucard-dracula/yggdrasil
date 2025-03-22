//ptr_val_operator.hpp

/****************************************************************************
Copyright (c) 2010-2024 yggdrasil

author: xu yang

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

#ifndef __YGGR_PTR_VAL_PTR_VAL_OPERATOR_HPP__
#define __YGGR_PTR_VAL_PTR_VAL_OPERATOR_HPP__

#include <yggr/mplex/static_assert.hpp>
#include <yggr/mplex/pointer_to_value_t.hpp>

#include <yggr/type_traits/pointer_checker.hpp>

#include <functional>
#include <cassert>

namespace yggr
{
namespace ptr_val
{

template<typename T>
struct ptr_val_negate
#if YGGR_CPP_VERSION < YGGR_CPP_VER_17
	: public std::unary_function<T, typename mplex::pointer_to_value_t<T>::type >
#endif // #if YGGR_CPP_VERSION < YGGR_CPP_VER_17
{
	BOOST_MPL_ASSERT((yggr::is_pointer<T>));
	typedef typename mplex::pointer_to_value_t<T>::type result_type;
	typedef T argument_type;
	typedef result_type l_value_type;

	inline result_type operator()(T l) const
	{
		assert(l);
		return std::negate<result_type>()(*l);
	}
};

template<typename T1, typename T2 = T1>
struct ptr_val_plus
#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	: public std::binary_function<T1, T2, typename mplex::pointer_to_value_t<T1>::type >
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
{
	BOOST_MPL_ASSERT((yggr::is_pointer<T1>));
	BOOST_MPL_ASSERT((yggr::is_pointer<T2>));

	typedef T1 first_argument_type;
	typedef T2 second_argument_type;

	typedef typename mplex::pointer_to_value_t<T1>::type result_type;
	typedef result_type l_value_type;
	typedef typename mplex::pointer_to_value_t<T2>::type r_value_type;

	inline result_type operator()(T1 l, T2 r) const
	{
		assert(l && r);
		return (*l) + (*r);
	}
};

template<typename T1, typename T2 = T1>
struct ptr_val_minus
#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	: public std::binary_function<T1, T2, typename mplex::pointer_to_value_t<T1>::type >
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
{
	BOOST_MPL_ASSERT((yggr::is_pointer<T1>));
	BOOST_MPL_ASSERT((yggr::is_pointer<T2>));

	typedef T1 first_argument_type;
	typedef T2 second_argument_type;

	typedef typename mplex::pointer_to_value_t<T1>::type result_type;
	typedef result_type l_value_type;
	typedef typename mplex::pointer_to_value_t<T2>::type r_value_type;

	inline result_type operator()(T1 l, T2 r) const
	{
		assert(l && r);
		return (*l) - (*r);
	}
};

template<typename T1, typename T2 = T1>
struct ptr_val_multiplies
#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	: public std::binary_function<T1, T2, typename mplex::pointer_to_value_t<T1>::type >
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
{
	BOOST_MPL_ASSERT((yggr::is_pointer<T1>));
	BOOST_MPL_ASSERT((yggr::is_pointer<T2>));

	typedef T1 first_argument_type;
	typedef T2 second_argument_type;

	typedef typename mplex::pointer_to_value_t<T1>::type result_type;
	typedef result_type l_value_type;
	typedef typename mplex::pointer_to_value_t<T2>::type r_value_type;

	inline result_type operator()(T1 l, T2 r) const
	{
		assert(l && r);
		return (*l) * (*r);
	}
};

template<typename T1, typename T2 = T1>
struct ptr_val_divides
#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	: public std::binary_function<T1, T2, typename mplex::pointer_to_value_t<T1>::type >
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
{
	BOOST_MPL_ASSERT((yggr::is_pointer<T1>));
	BOOST_MPL_ASSERT((yggr::is_pointer<T2>));

	typedef T1 first_argument_type;
	typedef T2 second_argument_type;

	typedef typename mplex::pointer_to_value_t<T1>::type result_type;
	typedef result_type l_value_type;
	typedef typename mplex::pointer_to_value_t<T2>::type r_value_type;

	inline result_type operator()(T1 l, T2 r) const
	{
		assert(l && r);
		return (*l) / (*r);
	}
};

template<typename T>
struct ptr_val_modulus
#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	: public std::binary_function<T, T, typename mplex::pointer_to_value_t<T>::type >
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
{
	BOOST_MPL_ASSERT((yggr::is_pointer<T>));

	typedef T first_argument_type;
	typedef T second_argument_type;

	typedef typename mplex::pointer_to_value_t<T>::type result_type;
	typedef result_type l_value_type;
	typedef result_type r_value_type;

	inline result_type operator()(const T& l, const T& r) const
	{
		assert(l && r);
		return std::modulus<result_type>()(*l, *r);
	}
};

template<typename T1, typename T2 = T1>
struct ptr_val_less
#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	: public std::binary_function<T1, T2, bool>
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
{
	BOOST_MPL_ASSERT((yggr::is_pointer<T1>));
	BOOST_MPL_ASSERT((yggr::is_pointer<T2>));

	typedef T1 first_argument_type;
	typedef T2 second_argument_type;
	typedef bool result_type;

	typedef typename mplex::pointer_to_value_t<T1>::type l_value_type;
	typedef typename mplex::pointer_to_value_t<T2>::type r_value_type;

	inline bool operator()(T1 l, T2 r) const
	{
		assert(l && r);
		return (*l) < (*r);
	}
};

template<typename T1, typename T2 = T1>
struct ptr_val_less_equal
#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	: public std::binary_function<T1, T2, bool>
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
{
	BOOST_MPL_ASSERT((yggr::is_pointer<T1>));
	BOOST_MPL_ASSERT((yggr::is_pointer<T2>));

	typedef T1 first_argument_type;
	typedef T2 second_argument_type;
	typedef bool result_type;

	typedef typename mplex::pointer_to_value_t<T1>::type l_value_type;
	typedef typename mplex::pointer_to_value_t<T2>::type r_value_type;

	inline bool operator()(T1 l, T2 r) const
	{
		assert(l && r);
		return (*l) <= (*r);
	}
};

template<typename T1, typename T2 = T1>
struct ptr_val_greater
#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	: public std::binary_function<T1, T2, bool>
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
{
	BOOST_MPL_ASSERT((yggr::is_pointer<T1>));
	BOOST_MPL_ASSERT((yggr::is_pointer<T2>));

	typedef T1 first_argument_type;
	typedef T2 second_argument_type;
	typedef bool result_type;
	
	typedef typename mplex::pointer_to_value_t<T1>::type l_value_type;
	typedef typename mplex::pointer_to_value_t<T2>::type r_value_type;

	inline bool operator()(T1 l, T2 r) const
	{
		assert(l && r);
		return (*l) > (*r);
	}
};

template<typename T1, typename T2 = T1>
struct ptr_val_greater_equal
#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	: public std::binary_function<T1, T2, bool>
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
{
	BOOST_MPL_ASSERT((yggr::is_pointer<T1>));
	BOOST_MPL_ASSERT((yggr::is_pointer<T2>));

	typedef T1 first_argument_type;
	typedef T2 second_argument_type;
	typedef bool result_type;

	typedef typename mplex::pointer_to_value_t<T1>::type l_value_type;
	typedef typename mplex::pointer_to_value_t<T2>::type r_value_type;

	inline bool operator()(T1 l, T2 r) const
	{
		assert(l && r);
		return (*l) >= (*r);
	}
};

template<typename T1, typename T2 = T1>
struct ptr_val_equal_to
#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	: public std::binary_function<T1, T2, bool>
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
{
	BOOST_MPL_ASSERT((yggr::is_pointer<T1>));
	BOOST_MPL_ASSERT((yggr::is_pointer<T2>));

	typedef T1 first_argument_type;
	typedef T2 second_argument_type;
	typedef bool result_type;

	typedef typename mplex::pointer_to_value_t<T1>::type l_value_type;
	typedef typename mplex::pointer_to_value_t<T2>::type r_value_type;

	inline bool operator()(T1 l, T2 r) const
	{
		assert(l && r);
		return (*l) == (*r);
	}
};

template<typename T1, typename T2 = T1>
struct ptr_val_not_equal_to
#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	: public std::binary_function<T1, T2, bool>
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
{
	BOOST_MPL_ASSERT((yggr::is_pointer<T1>));
	BOOST_MPL_ASSERT((yggr::is_pointer<T2>));

	typedef T1 first_argument_type;
	typedef T2 second_argument_type;
	typedef bool result_type;

	typedef typename mplex::pointer_to_value_t<T1>::type l_value_type;
	typedef typename mplex::pointer_to_value_t<T2>::type r_value_type;

	inline bool operator()(T1 l, T2 r) const
	{
		assert(l && r);
		return (*l) != (*r);
	}
};

template<typename T1, typename T2 = T1>
struct ptr_val_logical_and
#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	: public std::binary_function<T1, T2, bool>
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
{
	BOOST_MPL_ASSERT((yggr::is_pointer<T1>));
	BOOST_MPL_ASSERT((yggr::is_pointer<T2>));

	typedef T1 first_argument_type;
	typedef T2 second_argument_type;
	typedef bool result_type;

	typedef typename mplex::pointer_to_value_t<T1>::type l_value_type;
	typedef typename mplex::pointer_to_value_t<T2>::type r_value_type;

	inline bool operator()(T1 l, T2 r) const
	{
		assert(l && r);
		return (*l) && (*r);
	}
};

template<typename T1, typename T2 = T1>
struct ptr_val_logical_or
#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	: public std::binary_function<T1, T2, bool>
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
{
	BOOST_MPL_ASSERT((yggr::is_pointer<T1>));
	BOOST_MPL_ASSERT((yggr::is_pointer<T2>));

	typedef T1 first_argument_type;
	typedef T2 second_argument_type;
	typedef bool result_type;

	typedef typename mplex::pointer_to_value_t<T1>::type l_value_type;
	typedef typename mplex::pointer_to_value_t<T2>::type r_value_type;

	inline bool operator()(T1 l, T2 r) const
	{
		assert(l && r);
		return (*l) || (*r);
	}
};

template<typename T>
struct ptr_val_logical_not
#if YGGR_CPP_VERSION < YGGR_CPP_VER_17
	: public std::unary_function<T, bool>
#endif // #if YGGR_CPP_VERSION < YGGR_CPP_VER_17
{
	BOOST_MPL_ASSERT((yggr::is_pointer<T>));
	typedef const T& argument_type;
	typedef bool result_type;
	typedef typename mplex::pointer_to_value_t<T>::type l_value_type;

	inline bool operator()(const T& l) const
	{
		assert(l);
		return !(*l);
	}
};

} // namespace ptr_val
} // namespace yggr

#endif //__YGGR_PTR_VAL_OPERATOR_PTR_VAL_OPERATOR_HPP__
