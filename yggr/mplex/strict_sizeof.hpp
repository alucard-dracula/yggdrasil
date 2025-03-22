//strict_sizeof.hpp

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

#ifndef __YGGR_MPLEX_STRICT_SIZEOF_HPP__
#define __YGGR_MPLEX_STRICT_SIZEOF_HPP__

#include <yggr/base/yggrdef.h>

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_class.hpp>

#include <boost/mpl/size_t.hpp>

#define YGGR_PP_REG_STRICT_SIZEOF( __type__, __size__ ) \
	namespace yggr { namespace mplex { namespace detail { \
		template<> \
		struct strict_sizeof_helper< __type__ > \
			: public boost::mpl::size_t< static_cast<std::size_t>( __size__ ) >::type {}; }}}

namespace yggr
{
namespace mplex
{

namespace detail
{

template<typename T>
struct strict_sizeof_helper
	: public 
		boost::mpl::size_t
		<
			static_cast<std::size_t>(boost::is_class<T>::value? 0 : sizeof(T))
		>::type
{
};

} // naemspace detail

template<typename T>
struct strict_sizeof
	: public 
		boost::mpl::size_t
		<
			detail::strict_sizeof_helper<T>::value
		>::type
{
private:
	typedef typename
		boost::mpl::size_t
		<
			detail::strict_sizeof_helper<T>::value
		>::type now_size_type;

	BOOST_STATIC_ASSERT((now_size_type::value));
};

template<typename T>
struct compatible_strict_sizeof
	: public 
		boost::mpl::size_t
		<
			static_cast<std::size_t>(
				detail::strict_sizeof_helper<T>::value?
					detail::strict_sizeof_helper<T>::value
					: sizeof(T) )
		>::type
{
private:
	typedef typename
		boost::mpl::size_t
		<
			static_cast<std::size_t>(
				detail::strict_sizeof_helper<T>::value?
					detail::strict_sizeof_helper<T>::value
					: sizeof(T) )
		>::type now_size_type;

	BOOST_STATIC_ASSERT((now_size_type::value));
};

template<typename T>
struct strict_sizeof_bit
	: public
		boost::mpl::size_t
		<
			strict_sizeof<T>::value * 8
		>::type
{
private:
	typedef typename 
		boost::mpl::size_t
		<
			strict_sizeof<T>::value * 8
		>::type now_size_type;

	BOOST_STATIC_ASSERT((now_size_type::value));
};

template<typename T>
struct compatible_strict_sizeof_bit
	: public
		boost::mpl::size_t
		<
			compatible_strict_sizeof<T>::value * 8
		>::type
{
private:
	typedef typename 
		boost::mpl::size_t
		<
			compatible_strict_sizeof<T>::value * 8
		>::type now_size_type;

	BOOST_STATIC_ASSERT((now_size_type::value));
};

} // namespace mplex
} // namespace yggr

#endif //__YGGR_MPLEX_STRICT_SIZEOF_HPP__
