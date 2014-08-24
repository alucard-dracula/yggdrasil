//yggr serialization bson_data_type.hpp

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

#ifndef __YGGR_SERIALIZATION_BSON_DATA_TYPE_HPP__
#define __YGGR_SERIALIZATION_BSON_DATA_TYPE_HPP__

#include <yggr/nsql_database_system/c_bson.hpp>

#include <boost/serialization/string.hpp>
#include <boost/serialization/collections_save_imp.hpp>
#include <boost/serialization/collections_load_imp.hpp>
#include <boost/serialization/split_free.hpp>

#include <boost/serialization/collection_traits.hpp>

BOOST_CLASS_IMPLEMENTATION(yggr::nsql_database_system::c_bson_date, boost::serialization::primitive_type)
BOOST_CLASS_IMPLEMENTATION(yggr::nsql_database_system::c_bson_time, boost::serialization::primitive_type)
BOOST_CLASS_IMPLEMENTATION(yggr::nsql_database_system::c_bson_oid, boost::serialization::primitive_type)

BOOST_CLASS_IMPLEMENTATION(yggr::nsql_database_system::c_bson_null, boost::serialization::primitive_type)
BOOST_CLASS_IMPLEMENTATION(yggr::nsql_database_system::c_bson_undefined, boost::serialization::primitive_type)
BOOST_CLASS_IMPLEMENTATION(yggr::nsql_database_system::c_bson_regex, boost::serialization::primitive_type)
BOOST_CLASS_IMPLEMENTATION(yggr::nsql_database_system::c_bson_code, boost::serialization::primitive_type)
BOOST_CLASS_IMPLEMENTATION(yggr::nsql_database_system::c_bson_symbol, boost::serialization::primitive_type)

BOOST_CLASS_IMPLEMENTATION(yggr::nsql_database_system::c_bson_timestamp, boost::serialization::primitive_type)

BOOST_CLASS_IMPLEMENTATION(yggr::nsql_database_system::c_bson, boost::serialization::primitive_type)

namespace boost 
{                                    
namespace serialization 
{
	template<typename Scope>                                          
	struct implementation_level_impl< const yggr::nsql_database_system::c_bson_code_w_scope<Scope> >                    
	{                                                    
		typedef mpl::integral_c_tag tag;                 
		typedef mpl::int_< boost::serialization::primitive_type > type;                     
		BOOST_STATIC_CONSTANT(                           
			int,                                         
			value = implementation_level_impl::type::value    
		);                                               
	};

	template<int BINARY_ID>                                          
	struct implementation_level_impl< const yggr::nsql_database_system::c_bson_binary_buffer<BINARY_ID> >                    
	{                                                    
		typedef mpl::integral_c_tag tag;                 
		typedef mpl::int_< boost::serialization::primitive_type > type;                     
		BOOST_STATIC_CONSTANT(                           
			int,                                         
			value = implementation_level_impl::type::value    
		);                                               
	};                                                   
} // namespace serialization
} // namespace boost

#include <boost/serialization/version.hpp>

YGGR_PP_BSON_VALUE_TYPE_ID(boost::serialization::version_type, bson_value_type::E_BSON_TYPE_INT)
YGGR_PP_BSON_VALUE_TYPE_ID(boost::serialization::item_version_type, bson_value_type::E_BSON_TYPE_INT)

#endif // __YGGR_SERIALIZATION_STRING_HPP__
