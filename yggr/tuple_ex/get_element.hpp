//get_element.hpp (tuple_ex)

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

#ifndef __YGGR_TUPLE_EX_GET_ELEMENT_HPP__
#define __YGGR_TUPLE_EX_GET_ELEMENT_HPP__

#include <yggr/ppex/typedef.hpp>

#include <yggr/utility/get_element_default.hpp>
#include <yggr/tuple_ex/tuple.hpp>


namespace yggr
{
namespace tuples
{

// -------------tuple-------------

//if T is tuple return first element
//------------------get_first_element-----------

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

// boost tuple
template<typename ...T> inline 
typename 
	boost::tuples::access_traits
	<
		typename 
			boost::tuples::element
			< 
				0, 
				boost::tuple< T... > 
			>::type
	>::non_const_type
	get_first_element(boost::tuple< T... >& c)
{
	return yggr::get<0>(c);
}

template<typename ...T> inline 
typename 
	boost::tuples::access_traits
	<
		typename 
			boost::tuples::element
			< 
				0, 
				boost::tuple< T... > 
			>::type
	>::const_type
	get_first_element(const boost::tuple< T... >& c)
{
	return yggr::get<0>(c);
}

// yggr tuple
template<typename ...T> inline 
typename 
	yggr::tuples::access_traits
	<
		typename 
			yggr::tuples::element
			< 
				0, 
				yggr::tuple< T... > 
			>::type
	>::non_const_type
	get_first_element(yggr::tuple< T... >& c)
{
	return yggr::get<0>(c);
}

template<typename ...T> inline 
typename 
	yggr::tuples::access_traits
	<
		typename 
			yggr::tuples::element
			< 
				0, 
				yggr::tuple< T... > 
			>::type
	>::const_type
	get_first_element(const yggr::tuple< T... >& c)
{
	return yggr::get<0>(c);
}

#else

// boost tuple
template<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T)> inline 
typename 
	boost::tuples::access_traits
	<
		typename 
			boost::tuples::element
			< 
				0, 
				boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T) > 
			>::type
	>::non_const_type
	get_first_element(boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T) >& c)
{
	return yggr::get<0>(c);
}

template<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T)> inline 
typename 
	boost::tuples::access_traits
	<
		typename 
			boost::tuples::element
			< 
				0, 
				boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T) > 
			>::type
	>::const_type
	get_first_element(const boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T) >& c)
{
	return yggr::get<0>(c);
}

// yggr tuple
template<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), typename T)> inline 
typename 
	yggr::tuples::access_traits
	<
		typename 
			yggr::tuples::element
			< 
				0, 
				yggr::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T) > 
			>::type
	>::non_const_type
	get_first_element(yggr::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T) >& c)
{
	return yggr::get<0>(c);
}

template<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), typename T)> inline 
typename 
	yggr::tuples::access_traits
	<
		typename 
			yggr::tuples::element
			< 
				0, 
				yggr::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T) > 
			>::type
	>::const_type
	get_first_element(const yggr::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T) >& c)
{
	return yggr::get<0>(c);
}

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

//if T is tuple return last element
// -----------------get_last_element------------

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

// boost tuple
template<typename ...T> inline 
typename 
	boost::tuples::access_traits
	<
		typename 
			boost::tuples::element
			< 
				boost::tuples::length
				< 
					boost::tuple< T... > 
				>::value - 1,
				boost::tuple< T... > 
			>::type
	>::non_const_type
	get_last_element(boost::tuple< T... >& c)
{
	typedef boost::tuple< T... > tuple_type;
	return yggr::get<boost::tuples::length<tuple_type>::value - 1>(c);
}

template<typename ...T> inline 
typename 
	boost::tuples::access_traits
	<
		typename 
			boost::tuples::element
			< 
				boost::tuples::length
				< 
					boost::tuple< T... > 
				>::value - 1,
				boost::tuple< T... > 
			>::type
	>::const_type
	get_last_element(const boost::tuple< T... >& c)
{
	typedef boost::tuple< T... > tuple_type;
	return yggr::get<boost::tuples::length<tuple_type>::value - 1>(c);
}

template< typename ...T > inline 
typename 
	yggr::tuples::access_traits
	<
		typename 
			yggr::tuples::element
			< 
				yggr::tuples::length
				< 
					yggr::tuple< T... > 
				>::value - 1,
				yggr::tuple< T... > 
			>::type
	>::non_const_type
	get_last_element(yggr::tuple< T... >& c)
{
	typedef yggr::tuple< T... > tuple_type;
	return yggr::get<yggr::tuples::length<tuple_type>::value - 1>(c);
}

template< typename ...T > inline 
typename 
	yggr::tuples::access_traits
	<
		typename 
			yggr::tuples::element
			< 
				yggr::tuples::length
				< 
					yggr::tuple< T... > 
				>::value - 1,
				yggr::tuple< T... > 
			>::type
	>::const_type
	get_last_element(const yggr::tuple< T... >& c)
{
	typedef yggr::tuple< T... > tuple_type;
	return yggr::get<yggr::tuples::length<tuple_type>::value - 1>(c);
}

#else

// boost tuple
template<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T)> inline 
typename 
	boost::tuples::access_traits
	<
		typename 
			boost::tuples::element
			< 
				boost::tuples::length
				< 
					boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T) > 
				>::value - 1,
				boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T) > 
			>::type
	>::non_const_type
	get_last_element(boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T) >& c)
{
	typedef boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T) > tuple_type;
	return yggr::get<boost::tuples::length<tuple_type>::value - 1>(c);
}

template<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T)> inline 
typename 
	boost::tuples::access_traits
	<
		typename 
			boost::tuples::element
			< 
				boost::tuples::length
				< 
					boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T) > 
				>::value - 1,
				boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T) > 
			>::type
	>::const_type
	get_last_element(const boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T) >& c)
{
	typedef boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T) > tuple_type;
	return yggr::get<boost::tuples::length<tuple_type>::value - 1>(c);
}

template<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), typename T)> inline 
typename 
	yggr::tuples::access_traits
	<
		typename 
			yggr::tuples::element
			< 
				yggr::tuples::length
				< 
					yggr::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T) > 
				>::value - 1,
				yggr::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T) > 
			>::type
	>::non_const_type
	get_last_element(yggr::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T) >& c)
{
	typedef yggr::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T) > tuple_type;
	return yggr::get<yggr::tuples::length<tuple_type>::value - 1>(c);
}

template<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), typename T)> inline 
typename 
	yggr::tuples::access_traits
	<
		typename 
			yggr::tuples::element
			< 
				yggr::tuples::length
				< 
					yggr::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T) > 
				>::value - 1,
				yggr::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T) > 
			>::type
	>::const_type
	get_last_element(const yggr::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T) >& c)
{
	typedef yggr::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T) > tuple_type;
	return yggr::get<yggr::tuples::length<tuple_type>::value - 1>(c);
}

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES


} // namespace tuples

using tuples::get_first_element;
using tuples::get_last_element;

} // namespace yggr

#endif // __YGGR_TUPLE_EX_GET_ELEMENT_HPP__
