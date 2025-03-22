//bson_inner_data_access.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_BSON_INNER_DATA_ACCESSER_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_BSON_INNER_DATA_ACCESSER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/charset/assert_char.hpp>
#include <yggr/charset/charset_type_traits.hpp>
#include <yggr/charset/string_charset_helper.hpp>
#include <yggr/nsql_database_system/bson_inner_data_accesser_decl.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_const.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/utility/enable_if.hpp>

namespace yggr
{
namespace nsql_database_system
{

template<>
class bson_inner_data_accesser<false>
{
public:
	// now proxy
	//BOOST_MPL_ASSERT(( boost::mpl::bool_<false> ));
};

template<>
class bson_inner_data_accesser<true>
{
public:
	bson_inner_data_accesser(void) {}
	~bson_inner_data_accesser(void){}

public:
	// save
	//template<typename Bson, typename T, typename Char> inline
	//typename 
	//	boost::enable_if
	//	<
	//		boost::mpl::and_
	//		<
	//			boost::is_base_of<bson_t, Bson>,
	//			is_native_char<Char>
	//		>, 
	//		bool 
	//	>::type
	//	save(Bson& bs, const Char* name, const T& val) const
	//{
	//	typedef bson_typeid<typename boost::remove_const<T>::type> bson_typeid_type;
	//	typedef typename bson_typeid_type::wrap_type wrap_type;

	//	const wrap_type& wrap_val = reinterpret_cast<const wrap_type&>(val);
	//	return wrap_val.save(bs, name);
	//}

	//template<typename Bson, typename T, 
	//			typename Char, typename Traits, typename Alloc,
	//			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	//typename 
	//	boost::enable_if
	//	<
	//		boost::mpl::and_
	//		<
	//			boost::is_base_of<bson_t, Bson>,
	//			charset::is_string_t< Basic_String<Char, Traits, Alloc> >
	//		>, 
	//		bool 
	//	>::type
	//	save(Bson& bs, const Basic_String<Char, Traits, Alloc>& name, const T& val) const
	//{
	//	typedef bson_typeid<typename boost::remove_const<T>::type> bson_typeid_type;
	//	typedef typename bson_typeid_type::wrap_type wrap_type;

	//	const wrap_type& wrap_val = reinterpret_cast<const wrap_type&>(val);
	//	return wrap_val.save(bs, name);
	//}

	template<typename Bson, typename T, typename String > inline
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_base_of<bson_t, Bson>,
				charset::utf8_string_constructable<String>
			>, 
			bool 
		>::type
		save(Bson& bs, const String& name, const T& val) const
	{
		typedef bson_typeid<typename boost::remove_const<T>::type> bson_typeid_type;
		typedef typename bson_typeid_type::wrap_type wrap_type;

		const wrap_type& wrap_val = reinterpret_cast<const wrap_type&>(val);
		return wrap_val.save(bs, name);
	}

	// save_value
	template<typename Bson, typename T> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, bool>::type
		save_value(Bson& bs, const T& val) const
	{
		typedef bson_typeid<typename boost::remove_const<T>::type> bson_typeid_type;
		typedef typename bson_typeid_type::wrap_type wrap_type;

		const wrap_type& wrap_val = reinterpret_cast<const wrap_type&>(val);
		return wrap_val.save_value(bs);
	}

	// load

	//template<typename BsonIter, typename T, typename Char> inline
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
	//	load(const BsonIter& iter, const Char* name, T& val) const
	//{
	//	typedef bson_typeid<typename boost::remove_const<T>::type> bson_typeid_type;
	//	typedef typename bson_typeid_type::wrap_type wrap_type;

	//	wrap_type& wrap_val = reinterpret_cast<wrap_type&>(val);
	//	wrap_val.load(iter, name);
	//}

	//template<typename BsonIter, typename T, 
	//			typename Char, typename Traits, typename Alloc,
	//			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
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
	//	load(const BsonIter& iter, const Basic_String<Char, Traits, Alloc>& name, T& val) const
	//{
	//	typedef bson_typeid<typename boost::remove_const<T>::type> bson_typeid_type;
	//	typedef typename bson_typeid_type::wrap_type wrap_type;

	//	wrap_type& wrap_val = reinterpret_cast<wrap_type&>(val);
	//	wrap_val.load(iter, name);
	//}

	template<typename BsonIter, typename T, typename String > inline
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_base_of<bson_iter_t, BsonIter>,
				charset::utf8_string_constructable<String>
			>, 
			void 
		>::type 
		load(const BsonIter& iter, const String& name, T& val) const
	{
		typedef bson_typeid<typename boost::remove_const<T>::type> bson_typeid_type;
		typedef typename bson_typeid_type::wrap_type wrap_type;

		wrap_type& wrap_val = reinterpret_cast<wrap_type&>(val);
		wrap_val.load(iter, name);
	}

	// load_value
	template<typename BsonIter, typename T> inline
	typename boost::enable_if<boost::is_base_of<bson_iter_t, BsonIter>, void>::type
		load_value(const BsonIter& iter, T& val) const
	{
		typedef bson_typeid<typename boost::remove_const<T>::type> bson_typeid_type;
		typedef typename bson_typeid_type::wrap_type wrap_type;

		wrap_type& wrap_val = reinterpret_cast<wrap_type&>(val);
		wrap_val.load_value(iter);
	}
};

} // namespace nsql_database_system
} // namespace yggr


#endif // __YGGR_NSQL_DATABASE_SYSTEM_BSON_INNER_DATA_ACCESSER_HPP__
