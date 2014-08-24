//yggr serialization collection_size_type_impl.hpp

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

#ifndef __YGGR_SERIALIZATION_COLLECTION_SIZE_TYPE_BSON_IMPL_HPP__
//#define __YGGR_SERIALIZATION_COLLECTION_SIZE_TYPE_BSON_IMPL_HPP__

#if ( defined (YGGR_HAS_BSON_OARCHIVE) || defined (YGGR_HAS_BSON_IARCHIVE) )
#	define __YGGR_SERIALIZATION_COLLECTION_SIZE_TYPE_BSON_IMPL_HPP__

#include <yggr/nsql_database_system/bson_type_traits.hpp>

namespace yggr 
{
namespace nsql_database_system 
{

template<> 
struct bson_value_typeid< boost::serialization::collection_size_type > 
{ 
	enum 
	{ 
		value = (sizeof(ser_size_type) == 4
						? bson_value_type::E_BSON_TYPE_INT
						: sizeof(ser_size_type) == 8
							? bson_value_type::E_BSON_TYPE_LONG
							: bson_value_type::E_BSON_TYPE_EOO)
	};
};

template<> 
struct bson_value_typeid< yggr::serialization::collection_size_type > 
{ 
	enum 
	{ 
		value = (sizeof(yggr::serialization::collection_size_type) == 4
						? bson_value_type::E_BSON_TYPE_INT
						: sizeof(yggr::serialization::collection_size_type) == 8
							? bson_value_type::E_BSON_TYPE_LONG
							: bson_value_type::E_BSON_TYPE_EOO)
	};
};

} // namespace nsql_database_system
} // namespace yggr

#	ifdef YGGR_SERIALIZATION_COLLECTION_SIZE_TYPE_BSON_IMPL_HPP_BSON_ARCHIVE
#		undef YGGR_SERIALIZATION_COLLECTION_SIZE_TYPE_BSON_IMPL_HPP_BSON_ARCHIVE
#	endif // YGGR_SERIALIZATION_COLLECTION_SIZE_TYPE_BSON_IMPL_HPP_BSON_ARCHIVE

#else
#	define YGGR_SERIALIZATION_COLLECTION_SIZE_TYPE_BSON_IMPL_HPP_BSON_ARCHIVE
#endif // defined (YGGR_HAS_BSON_OARCHIVE) || defined (YGGR_HAS_BSON_IARCHIVE)

#endif //__YGGR_SERIALIZATION_COLLECTION_SIZE_TYPE_BSON_IMPL_HPP__


