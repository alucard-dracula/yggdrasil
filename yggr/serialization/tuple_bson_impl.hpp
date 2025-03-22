//tuple_bson_impl.hpp

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

#ifndef __YGGR_SERIALIZATION_TUPLE_BSON_IMPL_HPP__
//#define __YGGR_SERIALIZATION_TUPLE_BSON_IMPL_HPP__

#if defined(YGGR_HAS_BSON_OARCHIVE) || defined(YGGR_HAS_BSON_IARCHIVE)
#	define __YGGR_SERIALIZATION_TUPLE_BSON_IMPL_HPP__

#include <yggr/nsql_database_system/bson_type_traits.hpp>
#include <yggr/serialization/nvp.hpp>

//out_side_type don't use E_BSON_TYPE_DOCUMENT, because this is c_bson used!!!!!!!!
//#define BOOST_PP_LOCAL_MACRO( __n__ ) \
//	YGGR_PP_CONTAINER_BSON_TYPEID_IMPL(__n__, boost::tuple, bson_typeid_def::E_BSON_TYPE_SPLICE_OBJECT)
//#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
//#include BOOST_PP_LOCAL_ITERATE( )

YGGR_PP_CONTAINER_BSON_TYPEID_IMPL(BOOST_TUPLE_LIMIT_LENGTH(), boost::tuples::tuple, bson_typeid_def::E_BSON_TYPE_SPLICE_OBJECT)
YGGR_PP_CONTAINER_BSON_TYPEID_IMPL(YGGR_TUPLE_LIMIT_LENGTH(), yggr::tuples::tuple, bson_typeid_def::E_BSON_TYPE_SPLICE_OBJECT)

#if !defined(YGGR_NO_CXX11_HDR_TUPLE)

YGGR_PP_VARIADIC_CONTAINER_BSON_TYPEID_IMPL(YGGR_TUPLE_LIMIT_LENGTH(), std::tuple, bson_typeid_def::E_BSON_TYPE_SPLICE_OBJECT)

#endif // YGGR_NO_CXX11_HDR_TUPLE

#	ifdef YGGR_SERIALIZATION_TUPLE_BSON_IMPL_HPP_ARCHIVE
#		undef YGGR_SERIALIZATION_TUPLE_BSON_IMPL_HPP_ARCHIVE
#	endif // YGGR_SERIALIZATION_TUPLE_BSON_IMPL_HPP_ARCHIVE

#else
#	define YGGR_SERIALIZATION_TUPLE_BSON_IMPL_HPP_ARCHIVE
#endif // defined(YGGR_HAS_BSON_OARCHIVE) || defined(YGGR_HAS_BSON_IARCHIVE)

#endif // __YGGR_SERIALIZATION_TUPLE_BSON_IMPL_HPP__
