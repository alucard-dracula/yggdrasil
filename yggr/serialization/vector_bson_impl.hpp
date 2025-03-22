// vector_bson_impl.hpp

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

#ifndef __YGGR_SERIALIZATION_VECTOR_BSON_IMPL_HPP__
//#define __YGGR_SERIALIZATION_VECTOR_BSON_IMPL_HPP__

#if ( defined(YGGR_HAS_BSON_OARCHIVE) || defined(YGGR_HAS_BSON_IARCHIVE) )
#   define __YGGR_SERIALIZATION_VECTOR_BSON_IMPL_HPP__

#include <yggr/nsql_database_system/bson_type_traits.hpp>

YGGR_PP_CONTAINER_BSON_TYPEID_IMPL(2, std::vector, bson_typeid_def::E_BSON_TYPE_ARRAY)

#if (BOOST_VERSION < 106700)
	YGGR_PP_CONTAINER_BSON_TYPEID_IMPL(2, boost::container::vector, bson_typeid_def::E_BSON_TYPE_ARRAY)
#else
	YGGR_PP_CONTAINER_BSON_TYPEID_IMPL(3, boost::container::vector, bson_typeid_def::E_BSON_TYPE_ARRAY)
	YGGR_PP_CONTAINER_BSON_TYPEID_IMPL(2, yggr::container::detail::vector, bson_typeid_def::E_BSON_TYPE_ARRAY)
#endif // (BOOST_VERSION < 106700)

#	ifdef YGGR_SERIALIZATION_VECTOR_BSON_IMPL_HPP_ARCHIVE
#		undef YGGR_SERIALIZATION_VECTOR_BSON_IMPL_HPP_ARCHIVE
#	endif // YGGR_SERIALIZATION_VECTOR_BSON_IMPL_HPP_ARCHIVE
#else
#	define YGGR_SERIALIZATION_VECTOR_BSON_IMPL_HPP_ARCHIVE
#endif // ( defined(YGGR_HAS_BSON_OARCHIVE) || defined(YGGR_HAS_BSON_IARCHIVE) )

#endif // __YGGR_SERIALIZATION_VECTOR_BSON_IMPL_HPP__

#include <yggr/serialization/liner_container_bson_impl.hpp>
