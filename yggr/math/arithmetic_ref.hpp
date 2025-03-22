// marked_ref.hpp

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

#ifndef __YGGR_MATH_ARITHMETIC_REF_HPP__
#define __YGGR_MATH_ARITHMETIC_REF_HPP__

#include <yggr/base/yggrdef.h>

#include <boost/ref.hpp>
#include <boost/type_traits/remove_const.hpp>

namespace yggr
{
namespace math
{

template<typename M, typename R, typename T>
struct arith_reference_wrapper
	: public boost::reference_wrapper<T>
{
public:
	typedef M mark_type;
	typedef boost::reference_wrapper<T> base_type;

	typedef typename base_type::type type;

	typedef typename boost::remove_const<type>::type value_type;
	typedef R result_type;


private:
	typedef arith_reference_wrapper this_type;

public:
	arith_reference_wrapper(T& t)
		: base_type(t)
	{
	}

	~arith_reference_wrapper(void)
	{
	}
};

#if defined( __BORLANDC__ ) && BOOST_WORKAROUND( __BORLANDC__, BOOST_TESTED_AT(0x581) )
#	define YGGR_REF_CONST
#else
#	define YGGR_REF_CONST const
#endif // __BORLANDC__

template<typename M, typename T> inline 
arith_reference_wrapper<M, T, T> YGGR_REF_CONST 
	arith_ref(T& t)
{ 
    return arith_reference_wrapper<M, T, T>(t);
}

template<typename M, typename R, typename T> inline 
arith_reference_wrapper<M, R, T> YGGR_REF_CONST 
	arith_ref(T& t)
{ 
    return arith_reference_wrapper<M, R, T>(t);
}

template<typename M, typename T> inline 
arith_reference_wrapper<M, T, T const> YGGR_REF_CONST 
	arith_cref(T const& t)
{
    return arith_reference_wrapper<M, T, T const>(t);
}

template<typename M, typename R, typename T> inline 
arith_reference_wrapper<M, R, T const> YGGR_REF_CONST 
	arith_cref(T const& t)
{
    return arith_reference_wrapper<M, R, T const>(t);
}

#undef YGGR_REF_CONST

} // namespace math

using math::arith_reference_wrapper;
using math::arith_ref;
using math::arith_cref;

} // namesapce yggr

namespace boost
{

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

template<typename M, typename R, typename T>
class is_reference_wrapper< yggr::math::arith_reference_wrapper<M, R, T> > 
	: public mpl::true_ 
{
};

template<typename M, typename R, typename T>
class unwrap_reference< yggr::math::arith_reference_wrapper<M, R, T> > 
{
public: 
	typedef T type; 
};

#if !defined(BOOST_NO_CV_SPECIALIZATIONS)

template<typename M, typename R, typename T>
class is_reference_wrapper< yggr::math::arith_reference_wrapper<M, R, T> const > 
	: public mpl::true_ 
{
};

template<typename M, typename R, typename T>
class unwrap_reference< yggr::math::arith_reference_wrapper<M, R, T> const > 
{
public: 
	typedef T type; 
};

template<typename M, typename R, typename T>
class is_reference_wrapper< yggr::math::arith_reference_wrapper<M, R, T> volatile > 
	: public mpl::true_ 
{
};

template<typename M, typename R, typename T>
class unwrap_reference< yggr::math::arith_reference_wrapper<M, R, T> volatile > 
{
public: 
	typedef T type; 
};

template<typename M, typename R, typename T>
class is_reference_wrapper< yggr::math::arith_reference_wrapper<M, R, T> const volatile > 
	: public mpl::true_ 
{
};

template<typename M, typename R, typename T>
class unwrap_reference< yggr::math::arith_reference_wrapper<M, R, T> const volatile > 
{
public: 
	typedef T type; 
};

#endif // BOOST_NO_CV_SPECIALIZATIONS

# else // no partial specialization 

} // namespace boost

namespace boost
{

namespace detail
{

template<typename M, typename R, typename T>
yes_reference_wrapper_t 
	is_reference_wrapper_test(type< yggr::math::arith_reference_wrapper<M, R, T> >);


} // namespace detail;

# endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

template<typename M, typename R, typename T> inline 
T* get_pointer( yggr::math::arith_reference_wrapper<M, R, T> const & r)
{
    return r.get_pointer();
}

} // namespace boost

#endif // __YGGR_MATH_ARITHMETIC_REF_HPP__
