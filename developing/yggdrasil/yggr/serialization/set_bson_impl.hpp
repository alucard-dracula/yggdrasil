// set_bson_impl.hpp

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

#ifndef __YGGR_SERIALIZATION_SET_BSON_IMPL_HPP__
//#define __YGGR_SERIALIZATION_SET_BSON_IMPL_HPP__

#if defined(YGGR_HAS_BSON_OARCHIVE) || defined(YGGR_HAS_BSON_IARCHIVE)
#	define __YGGR_SERIALIZATION_SET_BSON_IMPL_HPP__

#include <yggr/nsql_database_system/bson_type_traits.hpp>
//#include <yggr/serialization/detail/bson_archive_compile_def.hpp>

YGGR_PP_CONTAINER_BSON_VALUE_TYPE_ID(3, std::set, bson_value_type::E_BSON_TYPE_ARRAY)
YGGR_PP_CONTAINER_BSON_VALUE_TYPE_ID(3, std::multiset, bson_value_type::E_BSON_TYPE_ARRAY)
YGGR_PP_CONTAINER_BSON_VALUE_TYPE_ID(3, boost::container::set, bson_value_type::E_BSON_TYPE_ARRAY)
YGGR_PP_CONTAINER_BSON_VALUE_TYPE_ID(3, boost::container::multiset, bson_value_type::E_BSON_TYPE_ARRAY)

#	ifdef YGGR_SERIALIZATION_SET_BSON_IMPL_HPP_ARCHIVE
#		undef YGGR_SERIALIZATION_SET_BSON_IMPL_HPP_ARCHIVE
#	endif // YGGR_SERIALIZATION_SET_BSON_IMPL_HPP_ARCHIVE

#else
#	define YGGR_SERIALIZATION_SET_BSON_IMPL_HPP_ARCHIVE
#endif // defined(YGGR_HAS_BSON_OARCHIVE) || defined(YGGR_HAS_BSON_IARCHIVE)

#endif // __YGGR_SERIALIZATION_SET_BSON_IMPL_HPP__

#include <yggr/serialization/unliner_container_bson_impl.hpp>