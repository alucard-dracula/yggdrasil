//bson_inner_data_access_proxy.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_BSON_INNER_DATA_ACCESS_PROXY_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_BSON_INNER_DATA_ACCESS_PROXY_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/nonable/noncreateable.hpp>
#include <yggr/nsql_database_system/bson_type_traits.hpp>
#include <yggr/nsql_database_system/bson_inner_data_accesser.hpp>

#include <boost/utility/enable_if.hpp>

#ifdef _MSC_VER
#	pragma warning (push)
#	pragma warning (disable : 4624)
#endif //_MSC_VER

namespace yggr
{
namespace nsql_database_system
{

class bson_inner_data_access_proxy
    : private nonable::noncreateable
{

private:
	typedef bson_inner_data_access_proxy this_type;

public:
	// save
	template<typename Bson, typename T, typename String > inline
	static 
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_base_of<bson_t, Bson>,
				charset::utf8_string_constructable< String >
			>, 
			bool 
		>::type
		save(Bson& bs, const String& name, const T& val)
	{
		typedef bson_inner_data_accesser<is_bson_inside_type<T>::value> accesser_type;
		accesser_type ass;
		return ass.save(bs, name, val);
	}
	
	// save_value
	template<typename Bson, typename T> inline
	static typename boost::enable_if<boost::is_base_of<bson_t, Bson>, bool>::type
		save_value(Bson& bs, const T& val)
	{
		typedef bson_inner_data_accesser<is_bson_inside_type<T>::value> accesser_type;
		accesser_type ass;
		return ass.save_value(bs, val);
	}

	// load
	//template<typename BsonIter, typename T> inline
	//static typename boost::enable_if<boost::is_base_of<bson_iter_t, BsonIter>, void>::type
	//	load(const BsonIter& iter, const char* name, T& val)
	//{
	//	typedef bson_inner_data_accesser<is_bson_inside_type<T>::value> accesser_type;
	//	accesser_type ass;
	//	ass.load(iter, name, val);
	//}

	//template<typename BsonIter, typename T, typename Char> inline
	//static 
	//typename 
	//	boost::enable_if
	//	<
	//		boost::mpl::and_
	//		<
	//			boost::is_base_of<bson_iter_t, BsonIter>,
	//			is_native_char<Char>
	//		>, 
	//		void
	//	>::type 
	//	load(const BsonIter& iter, const Char* name, T& val)
	//{
	//	typedef bson_inner_data_accesser<is_bson_inside_type<T>::value> accesser_type;
	//	accesser_type ass;
	//	ass.load(iter, name, val);
	//}

	//template<typename BsonIter, typename T,
	//			typename Char, typename Traits, typename Alloc,
	//			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	//static 
	//typename 
	//	boost::enable_if
	//	<
	//		boost::mpl::and_
	//		<
	//			boost::is_base_of<bson_iter_t, BsonIter>,
	//			charset::is_string_t< Basic_String<Char, Traits, Alloc> >
	//		>, 
	//		void 
	//	>::type 
	//	load(const BsonIter& iter, const Basic_String<Char, Traits, Alloc>& name, T& val)
	//{
	//	typedef bson_inner_data_accesser<is_bson_inside_type<T>::value> accesser_type;
	//	accesser_type ass;
	//	ass.load(iter, name, val);
	//}

	template<typename BsonIter, typename T, typename String> inline
	static 
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_base_of<bson_iter_t, BsonIter>,
				charset::utf8_string_constructable< String >
			>, 
			void 
		>::type 
		load(const BsonIter& iter, const String& name, T& val)
	{
		typedef bson_inner_data_accesser<is_bson_inside_type<T>::value> accesser_type;
		accesser_type ass;
		ass.load(iter, name, val);
	}

	// load value
	template<typename BsonIter, typename T> inline
	static typename boost::enable_if<boost::is_base_of<bson_iter_t, BsonIter>, void>::type 
		load_value(const BsonIter& iter, T& val)
	{
		typedef bson_inner_data_accesser<is_bson_inside_type<T>::value> accesser_type;
		accesser_type ass;
		ass.load_value(iter, val);
	}
};

} // namespace nsql_database_system
} // namespace yggr

#ifdef _MSC_VER
#	pragma warning (pop)
#endif //_MSC_VER

#endif // __YGGR_NSQL_DATABASE_SYSTEM_BSON_INNER_DATA_ACCESS_PROXY_HPP__
