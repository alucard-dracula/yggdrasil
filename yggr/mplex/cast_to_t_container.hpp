//bind_list_to_vector.hpp

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

#ifndef __YGGR_MPLEX_CAST_TO_CONTAINER_HPP__
#define __YGGR_MPLEX_CAST_TO_CONTAINER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/ppex/typedef.hpp>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/set.hpp>
#include <boost/mpl/deque.hpp>
#include <boost/mpl/map.hpp>

namespace yggr
{
namespace mplex
{

template<typename T = void>
struct cast_to_vector
	: public boost::mpl::vector<T>
{
};

template<>
struct cast_to_vector<void>
	: public boost::mpl::vector<>
{
};

template<typename T = void>
struct cast_to_list
	: public boost::mpl::list<T>
{
};

template<>
struct cast_to_list<void>
	: public boost::mpl::list<>
{
};

template<typename T = void>
struct cast_to_deque
	: public boost::mpl::deque<T>
{
};

template<>
struct cast_to_deque<void>
	: public boost::mpl::deque<>
{
};

template<typename T = void>
struct cast_to_set
	: public boost::mpl::set<T>
{
};

template<>
struct cast_to_set<void>
	: public boost::mpl::set<>
{
};

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

#	define YGGR_PP_CAST_TO_CONTAINER_EINS( __cont_name__ ) \
		template< template<typename ..._T > class Other_Cont, typename ...T> \
		struct BOOST_PP_CAT( cast_to_, __cont_name__ )< Other_Cont< T... > > \
			: public boost::mpl::__cont_name__< T... >{};

	YGGR_PP_CAST_TO_CONTAINER_EINS(vector);
	YGGR_PP_CAST_TO_CONTAINER_EINS(list);
	YGGR_PP_CAST_TO_CONTAINER_EINS(deque);
	YGGR_PP_CAST_TO_CONTAINER_EINS(set);

#	undef YGGR_PP_CAST_TO_CONTAINER_EINS

#else

// vector list deque set
#	define YGGR_PP_CAST_TO_CONTAINER_EINS( __n__ ) \
		template<YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, typename T), \
					template<YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, typename _T) > \
					class Other_Cont > \
		struct BOOST_PP_CAT(cast_to_, YGGR_PP_CONTAINER_NAME())< \
				Other_Cont< YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, T) > > \
			: public boost::mpl::YGGR_PP_CONTAINER_NAME()< \
				YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, T) >  {};

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
				YGGR_PP_CAST_TO_CONTAINER_EINS( __n__ )

#	define YGGR_PP_CONTAINER_NAME() vector
#	define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_TEMPLATE_PARAMS_LEN() )
#	include BOOST_PP_LOCAL_ITERATE( )
#	undef YGGR_PP_CONTAINER_NAME


#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
				YGGR_PP_CAST_TO_CONTAINER_EINS( __n__ )

#	define YGGR_PP_CONTAINER_NAME() list
#	define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_TEMPLATE_PARAMS_LEN() )
#	include BOOST_PP_LOCAL_ITERATE( )
#	undef YGGR_PP_CONTAINER_NAME


#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
				YGGR_PP_CAST_TO_CONTAINER_EINS( __n__ )

#	define YGGR_PP_CONTAINER_NAME() deque
#	define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_TEMPLATE_PARAMS_LEN() )
#	include BOOST_PP_LOCAL_ITERATE( )
#	undef YGGR_PP_CONTAINER_NAME


#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
				YGGR_PP_CAST_TO_CONTAINER_EINS( __n__ )

#	define YGGR_PP_CONTAINER_NAME() set
#	define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_TEMPLATE_PARAMS_LEN() )
#	include BOOST_PP_LOCAL_ITERATE( )
#	undef YGGR_PP_CONTAINER_NAME

#	undef YGGR_PP_CAST_TO_CONTAINER_EINS

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES


} // namespace mplex
} // namespace yggr

#endif // __YGGR_MPLEX_CAST_TO_CONTAINER_HPP__
