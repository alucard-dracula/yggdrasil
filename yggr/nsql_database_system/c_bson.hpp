//c_bson.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/nonable/noncopyable.hpp>
#include <yggr/utility/swap.hpp>
#include <yggr/move/move.hpp>

#include <yggr/type_traits/native_t.hpp>

#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/utf8_string_view.hpp>
#include <yggr/charset/string_converter.hpp>
#include <yggr/type_traits/is_native_char.hpp>

#include <yggr/container/get_allocator.hpp>
#include <yggr/container_ex/is_allocator.hpp>

#include <yggr/ppex/symbols.hpp>
#include <yggr/ppex/concat.hpp>

#include <yggr/mplex/static_assert.hpp>
#include <yggr/mplex/tag_sfinae.hpp>

#include <yggr/nsql_database_system/bson_typeid.hpp>
#include <yggr/nsql_database_system/bson_inner_data.hpp>
#include <yggr/nsql_database_system/native_ex_decl_bson.hpp>

#include <yggr/nsql_database_system/bson_native_ex.hpp>
#include <yggr/nsql_database_system/bson_value_native_ex.hpp>
#include <yggr/nsql_database_system/bson_inner_data_access_proxy.hpp>
#include <yggr/nsql_database_system/bson_visit_t.hpp>

#include <boost/ref.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/or.hpp>

#include <cstddef>
#include <utility>
#include <iterator>

namespace yggr
{
namespace nsql_database_system
{

class c_bson_access_proxy;
class c_bson;
class c_bson_const_iterator;

} // namespace nsql_database_system
} // namespace yggr

YGGR_PP_BSON_TYPEID_IMPL(bson_t, bson_typeid_def::E_BSON_TYPE_DOCUMENT)
YGGR_PP_BSON_TYPEID_IMPL(c_bson, bson_typeid_def::E_BSON_TYPE_DOCUMENT)

// remark:
// !!!!! nil bson not participate storage !!!!

namespace yggr
{
namespace nsql_database_system
{

// the c_bson_access_proxy don't using sfinae check string

class c_bson_access_proxy
{
public:
	template<typename Bson, typename String, typename Val> inline
	static
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
		save(Bson& bs, const String& name, const Val& val)
	{
		return bs.save(name, val);
	}

	template<typename Val, typename Bson, typename String, typename Mark> inline
	static
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
		save_begin(Bson& bs, const String& name, const Mark& mark)
	{
		return bs.template save_begin<Val>(name, mark);
	}

	template<typename Bson, typename String, typename Mark> inline
	static
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
		save_begin(Bson& bs, u8 t, const String& name, const Mark& mark)
	{
		return bs.save_begin(t, name, mark);
	}

	template<typename Val, typename Bson, typename Mark> inline
	static typename boost::enable_if<boost::is_base_of<bson_t, Bson>, bool>::type
		save_end(Bson& bs, const Mark& mark)
	{
		return bs.template save_end<Val>(mark);
	}

	template<typename Bson, typename Val> inline
	static typename boost::enable_if<boost::is_base_of<bson_t, Bson>, bool>::type
		save_value(Bson& bs, const Val& val)
	{
		return bs.save_value(val);
	}

	template<typename BsonIter, typename String, typename Val> inline
	static
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
		load(const BsonIter& iter, const String& name, Val& val)
	{
		iter.load(name, val);
	}

	template<typename Val, typename BsonIter, typename String> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_base_of<bson_iter_t, BsonIter>,
				charset::utf8_string_constructable<String>
			>,
			bool
		>::type
		load_begin(BsonIter& tmp_iter, BsonIter& iter, const String& name)
	{
		return iter.template load_begin<Val>(tmp_iter, name);
	}

	template<typename Val, typename BsonIter, typename String> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_base_of<bson_iter_t, BsonIter>,
				charset::utf8_string_constructable<String>
			>,
			bool
		>::type
		load_begin(BsonIter& tmp_iter, BsonIter& iter, u8 t, const String& name)
	{
		return iter.template load_begin<Val>(tmp_iter, t, name);
	}

	template<typename Val, typename BsonIter> inline
	static typename boost::enable_if<boost::is_base_of<bson_iter_t, BsonIter>, bool>::type
		load_end(const BsonIter& tmp_iter, BsonIter& iter)
	{
		return iter.template load_end<Val>(tmp_iter);
	}

	template<typename BsonIter, typename Val> inline
	static typename boost::enable_if<boost::is_base_of<bson_iter_t, BsonIter>, void>::type
		load_value(const BsonIter& iter, Val& val)
	{
		iter.load_value(val);
	}
};

} // namespace nsql_database_system
} // namespace yggr

#define YGGR_BSON_ITER_FIND_CASE_TAG_TYPE( __name__ ) \
	yggr::nsql_database_system::c_bson_const_iterator::BOOST_PP_CAT( tag_, __name__ )

#define YGGR_BSON_ITER_FIND_CASE_TAG_OBJECT( __name__ ) \
	yggr::nsql_database_system::c_bson_const_iterator::BOOST_PP_CAT( tag_, __name__ )()

#define YGGR_BSON_ITER_INIT_CASE_TAG_TYPE( __name__ ) \
	yggr::nsql_database_system::c_bson_const_iterator::BOOST_PP_CAT( tag_, __name__ )

#define YGGR_BSON_ITER_INIT_CASE_TAG_OBJECT( __name__ ) \
	yggr::nsql_database_system::c_bson_const_iterator::BOOST_PP_CAT( tag_, __name__ )()

namespace yggr
{
namespace nsql_database_system
{

// c_bson_const_iterator

//BSON_ALIGNED_BEGIN (128)
//typedef struct
//{
//   const uint8_t *raw;      /* The raw buffer being iterated. */
//   uint32_t       len;      /* The length of raw. */
//   uint32_t       off;      /* The offset within the buffer. */
//   uint32_t       type;     /* The offset of the type byte. */
//   uint32_t       key;      /* The offset of the key byte. */
//   uint32_t       d1;       /* The offset of the first data byte. */
//   uint32_t       d2;       /* The offset of the second data byte. */
//   uint32_t       d3;       /* The offset of the third data byte. */
//   uint32_t       d4;       /* The offset of the fourth data byte. */
//   uint32_t       next_off; /* The offset of the next field. */
//   uint32_t       err_off;  /* The offset of the error. */
//   bson_value_t   value;    /* Internal value for various state. */
//} bson_iter_t
//BSON_ALIGNED_END (128);

struct bson_iterator_helper
{
public:
	inline static u32 bson_iterator_bin_type(const bson_iter_t* piter)
	{
		return
			piter?
				static_cast<bson_subtype_t>(*(piter->raw + piter->d2))
				: bson_binary_typeid_def::E_BSON_BIN_TYPE_BINARY;
	}
};

class c_bson_const_iterator
	: public bson_iter_t
{
private:
	friend class c_bson_access_proxy;
public:
	typedef bson_iter_t base_type;
	typedef base_type org_type;

	struct tag_case {};
	struct tag_noncase {};

public:
	typedef utf8_string inner_string_type;
	typedef utf8_string_view inner_string_view_type;

public:
	typedef std::ptrdiff_t difference_type;
	typedef std::forward_iterator_tag iterator_category;

private:
	typedef c_bson_const_iterator this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

private:
	typedef mplex::sfinae_type sfinae_type;

public:
	c_bson_const_iterator(void);
	explicit c_bson_const_iterator(const bson_t& right);
	explicit c_bson_const_iterator(const c_bson& right);

	template<typename Bson, typename String>
	c_bson_const_iterator(const Bson& bs, const String& key,
							typename
								boost::enable_if
								<
									boost::mpl::and_
									<
										boost::is_base_of<bson_t, Bson>,
										charset::utf8_string_constructable<String>
									>,
									sfinae_type
								>::type sfinae = 0)
	{
		typedef String now_string_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, now_inner_string_type);

		now_inner_string_type str_key((charset::string_charset_helper_data(key)));
		const bson_t& now_bs = bs;
		this_type::init_find(now_bs, str_key.data(), str_key.size(), tag_noncase());
	}

	template<typename Case_Tag, typename Bson, typename String>
	c_bson_const_iterator(const Case_Tag& tag,
							const Bson& bs,
							const String& key,
							typename
								boost::enable_if
								<
									boost::mpl::and_
									<
										boost::mpl::or_
										<
											boost::is_same<Case_Tag, tag_case>,
											boost::is_same<Case_Tag, tag_noncase>
										>,
										boost::is_base_of<bson_t, Bson>,
										charset::utf8_string_constructable<String>
									>,
									sfinae_type
								>::type sfinae = 0)
	{
		typedef String now_string_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, now_inner_string_type);

		now_inner_string_type str_key((charset::string_charset_helper_data(key)));
		this_type::init_find(bs, str_key.data(), str_key.size(), tag);
	}

	c_bson_const_iterator(BOOST_RV_REF(base_type) right)
	{
		base_type& right_ref = right;
		this_type::pro_operator_set(right_ref);
	}

	c_bson_const_iterator(const base_type& right);

	explicit c_bson_const_iterator(const base_type* right);

	c_bson_const_iterator(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::pro_operator_set(right_ref);
	}

	c_bson_const_iterator(const this_type& right);
	~c_bson_const_iterator(void);


protected:
	this_type& pro_operator_set(const base_type& right);

public:
	template<typename BsonIter> inline
	typename boost::enable_if<boost::is_base_of<base_type, BsonIter>, this_type&>::type
		operator=(BOOST_RV_REF(BsonIter) right)
	{
		BsonIter& right_ref = right;
		base_type& right_base_ref = right_ref;
		return this_type::pro_operator_set(right_base_ref);
	}

	template<typename BsonIter> inline
	typename boost::enable_if<boost::is_base_of<base_type, BsonIter>, this_type&>::type
		operator=(const BsonIter& right)
	{
		const BsonIter& right_cref = right;
		const base_type& right_base_cref = right_cref;
		return this_type::pro_operator_set(right_base_cref);
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		base_type& right_base_ref = right_ref;
		return this_type::pro_operator_set(right_base_ref);
	}

	inline this_type& operator=(const this_type& right)
	{
		const this_type& right_cref = right;
		const base_type& right_base_cref = right_cref;
		return this_type::pro_operator_set(right_base_cref);
	}

protected:
	void pro_swap(base_type& right);

public:
	template<typename BsonIter> inline
	typename boost::enable_if<boost::is_base_of<base_type, BsonIter>, void>::type
		swap(BOOST_RV_REF(BsonIter) right)
	{
		typedef BsonIter right_bson_iter_type;

		right_bson_iter_type& right_ref = right;
		base_type& base_right_ref = right_ref;
		this_type::pro_swap(base_right_ref);
	}

	template<typename BsonIter> inline
	typename boost::enable_if<boost::is_base_of<base_type, BsonIter>, void>::type
		swap(BsonIter& right)
	{
		typedef BsonIter right_bson_iter_type;

		right_bson_iter_type& right_ref = right;
		base_type& base_right_ref = right_ref;
		this_type::pro_swap(base_right_ref);
	}

public:
	inline base_type* org_pointer(void)
	{
		return static_cast<base_type*>(this);
	}

	inline const base_type* org_pointer(void) const
	{
		return static_cast<const base_type*>(this);
	}

public:
	template<typename String, typename T> inline
	operator std::pair<String, T>(void) const
	{
		typedef String now_string_type;
		BOOST_MPL_ASSERT((charset::is_string_t<now_string_type>));
		typedef std::pair<now_string_type, T> ret_type;
		typedef typename ret_type::first_type first_type;
		typedef typename ret_type::second_type second_type;

		inner_string_type str_key(this_type::org_get_key(), YGGR_STR_UTF8_STRING_CHARSET_NAME());
		ret_type ret;
		this_type::load(str_key, ret.second);
		ret.first = boost::move(str_key.str<first_type>());
		return ret;
	}

public:
	this_type& operator++(void); // must write to cpp
	this_type operator++(int); // must write to cpp

public:
	inline bool empty(void) const
	{
		return base_type::raw == 0 || base_type::len == 0;
	}

	inline inner_string_view_type get_key(void) const
	{
		return inner_string_view_type(this_type::org_get_key(), YGGR_STR_UTF8_STRING_CHARSET_NAME());
	}

	template<typename String> inline
	typename boost::enable_if<charset::utf8_string_loadable<String>, String&>::type
		get_key(String& val) const
	{
		typedef typename charset::string_charset_helper_first_t<String>::type ret_type;

		charset::string_converter::s_conv(
			charset::string_charset_helper_first(val),
			this_type::org_get_key(),
			YGGR_STR_UTF8_STRING_CHARSET_NAME(),
			charset::string_charset_helper_second(val));

		return val;
	}

	template<typename Alloc> inline
	typename
		boost::enable_if
		<
			container_ex::is_allocator<Alloc>,
			typename detail::make_utf8_string_t_from_allocator<Alloc>::type
		>::type
		get_key(const Alloc& alloc) const
	{
		typedef typename detail::make_utf8_string_t_from_allocator<Alloc>::type now_inner_string_type;
		return now_inner_string_type(this_type::org_get_key(), alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			boost::is_same<typename native_t<String>::type, String>,
			String
		>::type
		get_key(const string& charset_name
					= YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String)) const
	{
		typedef String ret_type;
		return
			charset::string_converter::s_conv<ret_type>(
				this_type::org_get_key(),
				YGGR_STR_UTF8_STRING_CHARSET_NAME(),
				charset_name);
	}

	template<typename String, typename Alloc> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_same<typename native_t<String>::type, String>,
				container_ex::is_allocator<Alloc>
			>,
			String
		>::type
		get_key(const Alloc& alloc,
				const yggr::string& charset_name
					= YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String)) const
	{
		typedef String ret_type;

		return
			charset::string_converter::s_conv<ret_type>(
				this_type::org_get_key(), alloc,
				YGGR_STR_UTF8_STRING_CHARSET_NAME(),
				charset_name);
	}

	// the foo use in serialization optimization comparer
	inline const char* get_key_pointer(void) const
	{
		return this_type::org_get_key();
	}

	u32 typeid_of_value(void) const; // must write to cpp

	inline this_type& skip_auto_id(void)
	{
		if( (this_type::typeid_of_value() == bson_typeid_def::E_BSON_TYPE_OID)
			&& (0 == std::strcmp(this_type::org_get_key(), "_id")) )
		{
			return this_type::operator++();
		}

		return *this;
	}

	template<typename T> inline
	T& get_value(T& val) const
	{
		assert(this_type::load_check_of_utf8(bson_typeid<T>::value, this_type::get_key_pointer()));
		if(this_type::load_check_of_utf8(bson_typeid<T>::value, this_type::get_key_pointer()))
		{
			this_type::load_value(val);
		}

		return val;
	}

	template<typename T> inline
	T get_value(void) const
	{
		T t;
		assert(this_type::load_check_of_utf8(bson_typeid<T>::value, this_type::get_key_pointer()));
		if(this_type::load_check_of_utf8(bson_typeid<T>::value, this_type::get_key_pointer()))
		{
			this_type::load_value(t);
		}
		return t;
	}

	inline c_bson_value get_value(void) const
	{
		c_bson_value t;

		assert(this_type::load_check_of_utf8(bson_typeid_def::E_BSON_TYPE_ANY_VALUE, this_type::get_key_pointer()));
		if(this_type::load_check_of_utf8(bson_typeid_def::E_BSON_TYPE_ANY_VALUE, this_type::get_key_pointer()))
		{
			this_type::load_value(t);
		}

		return t;
	}

	// find
	template<typename String> inline
	typename boost::enable_if< charset::utf8_string_constructable<String>, this_type >::type
		find(const String& name) const
	{
		typedef String now_string_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, now_inner_string_type);

		now_inner_string_type str_name((charset::string_charset_helper_data(name)));
		return this_type::org_find(str_name.data(), static_cast<u32>(str_name.size()), tag_noncase());
	}

	template<typename Case_Tag, typename String> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::or_
				<
					boost::is_same<Case_Tag, tag_case>,
					boost::is_same<Case_Tag, tag_noncase>
				>,
				charset::utf8_string_constructable<String>
			>,
			this_type
		>::type
		find(const String& name) const
	{
		typedef Case_Tag tag_type;
		typedef String now_string_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, now_inner_string_type);

		now_inner_string_type str_name((charset::string_charset_helper_data(name)));
		return this_type::org_find(str_name.data(), str_name.size(), tag_type());
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String>,
			std::pair<this_type, this_type>
		>::type
		find_descendant(const String& name) const
	{
		typedef String now_string_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, now_inner_string_type);

		now_inner_string_type str_name((charset::string_charset_helper_data(name)));
		return this_type::org_find_descendant(str_name.data(), tag_noncase());
	}

	template<typename Case_Tag, typename String> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::or_
				<
					boost::is_same<Case_Tag, tag_case>,
					boost::is_same<Case_Tag, tag_noncase>
				>,
				charset::utf8_string_constructable<String>
			>,
			std::pair<this_type, this_type>
		>::type
		find_descendant(const String& name) const
	{
		typedef Case_Tag tag_type;
		typedef String now_string_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, now_inner_string_type);

		now_inner_string_type str_name((charset::string_charset_helper_data(name)));
		return this_type::org_find_descendant(str_name.data(), tag_type());
	}

public:
	// the foo use in self optimization comparer
	inline bool load_check_of_utf8(u32 type_id, const char* name) const
	{
		assert(name);
		assert(charset::utf8_foo::is_utf8_string(name));

		return (type_id == bson_typeid_def::E_BSON_TYPE_ANY_VALUE
					|| this_type::typeid_of_value() == type_id)
				&& (0 == charset::utf8_foo::strcmp(this_type::org_get_key(), name));
	}

	// load_check
	template<typename String> inline
	typename boost::enable_if<charset::utf8_string_constructable<String>, bool>::type
		load_check(u32 type_id, const String& name) const
	{
		typedef String now_string_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, now_inner_string_type);

		now_inner_string_type str_name((charset::string_charset_helper_data(name)));

		return (type_id == bson_typeid_def::E_BSON_TYPE_ANY_VALUE || this_type::typeid_of_value() == type_id)
					&& (0 == str_name.compare(this_type::org_get_key(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
	}

	template<typename String> inline
	typename boost::enable_if<charset::utf8_string_constructable<String>, bool>::type
		load_check(u32 type_id, const String& name, u32 sub_type_id) const
	{
		typedef String now_string_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, now_inner_string_type);

		now_inner_string_type str_name((charset::string_charset_helper_data(name)));

		return (type_id == bson_typeid_def::E_BSON_TYPE_ANY_VALUE || this_type::typeid_of_value() == type_id)
					&& (0 == str_name.compare(this_type::org_get_key(), YGGR_STR_UTF8_STRING_CHARSET_NAME()))
					&& (bson_iterator_helper::bson_iterator_bin_type(this) == sub_type_id);
	}

	// load s8
	template<typename String> inline
	typename boost::enable_if<charset::utf8_string_constructable<String>, void>::type
		load(const String& name, s8& val) const
	{
		assert(this_type::load_check(bson_typeid<s8>::value, name));
		if(!this_type::load_check(bson_typeid<s8>::value, name))
		{
			return;
		}

		this_type::load_value(val);
	}

	// load char
	template<typename String, typename Char> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String>,
				is_native_char<Char>
			>,
			void
		>::type
		load(const String& name, Char& val) const
	{
		assert(this_type::load_check(bson_typeid<Char>::value, name));
		if(!this_type::load_check(bson_typeid<Char>::value, name))
		{
			return;
		}

		this_type::load_value(val);
	}

	// load u8
	template<typename String> inline
	typename boost::enable_if<charset::utf8_string_constructable<String>, void>::type
		load(const String& name, u8& val) const
	{
		assert(this_type::load_check(bson_typeid<u8>::value, name));
		if(!this_type::load_check(bson_typeid<u8>::value, name))
		{
			return;
		}

		this_type::load_value(val);
	}

	// load s16
	template<typename String> inline
	typename boost::enable_if<charset::utf8_string_constructable<String>, void>::type
		load(const String& name, s16& val) const
	{
		assert(this_type::load_check(bson_typeid<s16>::value, name));
		if(!this_type::load_check(bson_typeid<s16>::value, name))
		{
			return;
		}

		this_type::load_value(val);
	}

	// load u16
	template<typename String> inline
	typename boost::enable_if<charset::utf8_string_constructable<String>, void>::type
		load(const String& name, u16& val) const
	{
		assert(this_type::load_check(bson_typeid<u16>::value, name));
		if(!this_type::load_check(bson_typeid<u16>::value, name))
		{
			return;
		}

		this_type::load_value(val);
	}

	// load s32
	template<typename String> inline
	typename boost::enable_if<charset::utf8_string_constructable<String>, void>::type
		load(const String& name, s32& val) const
	{
		assert(this_type::load_check(bson_typeid<s32>::value, name));
		if(!this_type::load_check(bson_typeid<s32>::value, name))
		{
			return;
		}

		this_type::load_value(val);
	}

	// load u32
	template<typename String> inline
	typename boost::enable_if<charset::utf8_string_constructable<String>, void>::type
		load(const String& name, u32& val) const
	{
		assert(this_type::load_check(bson_typeid<u32>::value, name));
		if(!this_type::load_check(bson_typeid<u32>::value, name))
		{
			return;
		}

		this_type::load_value(val);
	}

	// load s64
	template<typename String> inline
	typename boost::enable_if<charset::utf8_string_constructable<String>, void>::type
		load(const String& name, s64& val) const
	{
		assert(this_type::load_check(bson_typeid<s64>::value, name));
		if(!this_type::load_check(bson_typeid<s64>::value, name))
		{
			return;
		}

		this_type::load_value(val);
	}

	// load u64
	template<typename String> inline
	typename boost::enable_if<charset::utf8_string_constructable<String>, void>::type
		load(const String& name, u64& val) const
	{
		assert(this_type::load_check(bson_typeid<u64>::value, name));
		if(!this_type::load_check(bson_typeid<u64>::value, name))
		{
			return;
		}

		this_type::load_value(val);
	}

	// load bool
	template<typename String> inline
	typename boost::enable_if<charset::utf8_string_constructable<String>, void>::type
		load(const String& name, bool& val) const
	{
		assert(this_type::load_check(bson_typeid<bool>::value, name));
		if(!this_type::load_check(bson_typeid<bool>::value, name))
		{
			return;
		}

		this_type::load_value(val);
	}

	// load f32
	template<typename String> inline
	typename boost::enable_if<charset::utf8_string_constructable<String>, void>::type
		load(const String& name, f32& val) const
	{
		assert(this_type::load_check(bson_typeid<f32>::value, name));
		if(!this_type::load_check(bson_typeid<f32>::value, name))
		{
			return;
		}

		this_type::load_value(val);
	}

	// load f64
	template<typename String> inline
	typename boost::enable_if<charset::utf8_string_constructable<String>, void>::type
		load(const String& name, f64& val) const
	{
		assert(this_type::load_check(bson_typeid<f64>::value, name));
		if(!this_type::load_check(bson_typeid<f64>::value, name))
		{
			return;
		}

		this_type::load_value(val);
	}

	// load string
	template<typename String_Name, typename String_Val> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String_Name>,
				boost::mpl::or_
				<
					charset::utf8_string_loadable<String_Val>,
					charset::utf8_string_view_loadable<String_Val>
				>
			>,
			void
		>::type
		load(const String_Name& name, String_Val& val) const
	{
		typedef String_Val now_string_type;
		assert(this_type::load_check(bson_typeid<now_string_type>::value, name));
		if(!this_type::load_check(bson_typeid<now_string_type>::value, name))
		{
			return;
		}

		this_type::load_value(val);
	}

	// load bson
	template<typename String, typename Bson> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_base_of<bson_t, Bson>,
				charset::utf8_string_constructable<String>
			>,
			void
		>::type
		load(const String& name, Bson& val) const
	{
		assert(this_type::load_check(bson_typeid<c_bson>::value, name));
		if(!this_type::load_check(bson_typeid<c_bson>::value, name))
		{
			return;
		}

		this_type::load_value(val);
	}

	// sub_array_iterator
	template<typename String>
	typename boost::enable_if<charset::utf8_string_constructable<String>, this_type>::type
		sub_array_iterator(const String& name) const
	{
		assert(this_type::load_check(bson_typeid_def::E_BSON_TYPE_ARRAY, name));
		if(!this_type::load_check(bson_typeid_def::E_BSON_TYPE_ARRAY, name))
		{
			return this_type();
		}

		this_type iter;
		bool bright = bson_iter_recurse(this, &iter);
		if(bright)
		{
			++iter;
		}
		return iter;
	}

	template<typename String>
	typename boost::enable_if<charset::utf8_string_constructable<String>, this_type>::type
		sub_document_iterator(const String& name) const
	{
		assert(this_type::load_check(bson_typeid_def::E_BSON_TYPE_DOCUMENT, name));
		if(!this_type::load_check(bson_typeid_def::E_BSON_TYPE_DOCUMENT, name))
		{
			return this_type();
		}

		this_type iter;
		bool bright = bson_iter_recurse(this, &iter);
		if(bright)
		{
			++iter;
		}
		return iter;
	}

	template<typename String, typename Obj> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String>,
				boost::mpl::and_
				<
					boost::mpl::not_< charset::utf8_string_loadable<Obj> >,
					boost::mpl::not_< charset::utf8_string_view_loadable<Obj> >
				>,
				boost::mpl::and_
				<
					boost::mpl::not_< is_native_char<Obj> >,
					boost::mpl::not_<boost::is_base_of<bson_t, Obj> >
				>
			>,
			void
		>::type
		load(const String& name, Obj& obj) const
	{
		typedef String now_string_type;
		bson_inner_data_access_proxy::load(*this, name, obj);
	}

	//c_bson_date
	//c_bson_time
	//c_bson_oid
	//c_bson_binary_buffer
	//c_bson_null
	//c_bson_undefined
	//c_bson_regex
	//these types provided by the member function load


protected:
	inline bool load_mark_start(this_type& tmp_iter, const bson_mark_splice&)
	{
		tmp_iter = *this;

		bool bright = bson_iter_recurse(&tmp_iter, this);
		if(bright)
		{
			this_type::operator++();
		}
		return bright;
	}

	inline bool load_mark_end(const this_type& tmp_iter, const bson_mark_splice&)
	{
		*this = tmp_iter;
		this_type::operator++();
		return true;
	}

	template<typename Mark> inline
	bool load_mark_start(this_type& tmp_iter, const Mark&)
	{
		return true;
	}

	template<typename Mark> inline
	bool load_mark_end(const this_type& tmp_iter, const Mark&)
	{
		return true;
	}

	template<typename Obj, typename String> inline
	typename boost::enable_if< charset::utf8_string_constructable<String>, bool>::type
		load_begin(this_type& tmp_iter, const String& name)
	{
		typedef typename bson_value_mark<typename boost::remove_const<Obj>::type>::type mark_type;
		return this_type::load_check(overlap_bson_typeid<Obj>(), name)
					&& this_type::load_mark_start(tmp_iter, mark_type());
	}

	template<typename Obj, typename String> inline
	typename boost::enable_if< charset::utf8_string_constructable<String>, bool>::type
		load_begin(this_type& tmp_iter, u8 t, const String& name)
	{
		typedef typename bson_value_mark<typename boost::remove_const<Obj>::type>::type mark_type;

		u8 obj_t = overlap_bson_typeid<Obj>();

		assert((obj_t == t
				|| obj_t == static_cast<u8>(bson_typeid_def::E_BSON_TYPE_ANY_VALUE)));
		return this_type::load_check(t, name)
					&& this_type::load_mark_start(tmp_iter, mark_type());
	}

	template<typename Obj> inline
	bool load_end(const this_type& tmp_iter)
	{
		typedef typename bson_value_mark<typename boost::remove_const<Obj>::type>::type mark_type;
		return load_mark_end(tmp_iter, mark_type());
	}

public:
	inline void load_value(s8& val) const
	{
		val = static_cast<s8>(bson_iter_int32(this));
	}

	template<typename Char> inline
	typename boost::enable_if<is_native_char<Char>, void>::type
		load_value(Char& val) const
	{
		val = static_cast<char>(bson_iter_int32(this));
	}

	inline void load_value(u8& val) const
	{
		val = static_cast<u8>(bson_iter_int32(this));
	}

	inline void load_value(s16& val) const
	{
		val = static_cast<s16>(bson_iter_int32(this));
	}

	inline void load_value(u16& val) const
	{
		val = static_cast<u16>(bson_iter_int32(this));
	}

	inline void load_value(s32& val) const
	{
		val = bson_iter_int32(this);
	}

	inline void load_value(u32& val) const
	{
		s32 nval = bson_iter_int32(this);
		val = *(reinterpret_cast<u32*>(&nval));
	}

	inline void load_value(s64& val) const
	{
		val = bson_iter_int64(this);
	}

	inline void load_value(u64& val) const
	{
		s64 nval = bson_iter_int64(this);
		val = *(reinterpret_cast<u64*>(&nval));
	}

	inline void load_value(bool& val) const
	{
		val = bson_iter_bool(this);
	}

	inline void load_value(f32& val) const
	{
		val = static_cast<f32>(bson_iter_double(this));
	}

	inline void load_value(f64& val) const
	{
		val = bson_iter_double(this);
	}

	template<typename String>
	typename boost::enable_if< charset::utf8_string_loadable<String>, void>::type
		load_value(String& val) const
	{
		typedef typename charset::string_charset_helper_first_t<String>::type ret_type;

		u32 len = 0;
		const char* pstr = 0;
		pstr = bson_iter_utf8(this, &len);

		if(!(pstr && len))
		{
			return;
		}

		inner_string_view_type inner_str_view(pstr, pstr + len);

//		charset::string_charset_helper_first(val) =
//			boost::move(
//				inner_str_view.str<ret_type>(
//					::yggr::get_allocator(val),
//					charset::string_charset_helper_second(val)));

        charset::string_charset_helper_first(val) =
				inner_str_view.str<ret_type>(
					::yggr::get_allocator(val),
					charset::string_charset_helper_second(val));
	}

	template<typename StringView>
	typename boost::enable_if< charset::utf8_string_view_loadable<StringView>, void>::type
		load_value(StringView& val) const
	{
		typedef typename charset::string_charset_helper_first_t<StringView>::type ret_type;

		u32 len = 0;
		const char* pstr = 0;
		pstr = bson_iter_utf8(this, &len);

		if(!(pstr && len))
		{
			return;
		}

		assert(charset::string_charset_helper_second(val) == YGGR_STR_UTF8_STRING_CHARSET_NAME());

		ret_type inner_str_view(pstr, pstr + len);
		charset::string_charset_helper_first(val) = boost::move(inner_str_view);
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, void>::type
		load_value(Bson& val) const
	{
		const u8* pdoc = 0;
		u32 doc_len = 0;
		bson_iter_document(this, &doc_len, &pdoc);

		if(!(pdoc && doc_len))
		{
			return;
		}

		bson_native_ex::s_bson_init_from_data(boost::addressof(val), pdoc, doc_len);
	}

	template<typename Obj> inline
	typename
		boost::disable_if
		<
			boost::mpl::or_
			<
				boost::mpl::or_
				<
					charset::utf8_string_loadable<Obj>,
					charset::utf8_string_view_loadable<Obj>
				>,
				boost::mpl::or_
				<
					is_native_char<Obj>,
					boost::is_base_of<bson_t, Obj>
				>
			>,
			void
		>::type
		load_value(Obj& obj) const
	{
		bson_inner_data_access_proxy::load_value(*this, obj);
	}

protected:
	bool pro_compare_eq(const base_type& right) const; // must write to cpp

public:
	template<typename BsonIter> inline
	typename boost::enable_if<boost::is_base_of<base_type, BsonIter>, bool>::type
		compare_eq(const BsonIter& right) const
	{
		const base_type& base_right_cref = right;
		return this_type::pro_compare_eq(base_right_cref);
	}

private:
	void init(const bson_t& right);
	void init_end(const bson_t& right);

	const char* org_get_key(void) const; // must write to cpp

	bool org_key_cmp(const char* key, u32 key_len) const; // must write to cpp
	bool org_key_cmp_case(const char* key, u32 key_len) const; // must write to cpp

	void init_find(const bson_t& right, const char* key, u32 key_len, const tag_noncase&);
	void init_find(const bson_t& right, const char* key, u32 key_len, const tag_case&);

	this_type org_find(const char* key, u32 key_len, const tag_noncase&) const;
	this_type org_find(const char* key, u32 key_len, const tag_case&) const;

	std::pair<this_type, this_type> org_find_descendant(const char* dotkey, const tag_noncase&) const;
	std::pair<this_type, this_type> org_find_descendant(const char* dotkey, const tag_case&) const;
};

} // namespace nsql_database_system
} // namespace yggr

// nonmember foo

template<typename BsonIter> inline
typename boost::enable_if<boost::is_base_of<bson_iter_t, BsonIter>, bool>::type
	operator==(const bson_iter_t& l, const BsonIter& r)
{
	typedef yggr::nsql_database_system::c_bson_const_iterator child_iter_type;
	return reinterpret_cast<const child_iter_type&>(l).compare_eq(r);
}

template<typename BsonIter> inline
typename boost::enable_if<boost::is_base_of<bson_iter_t, BsonIter>, bool>::type
	operator!=(const bson_iter_t& l, const BsonIter& r)
{
	typedef yggr::nsql_database_system::c_bson_const_iterator child_iter_type;
	return !reinterpret_cast<const child_iter_type&>(l).compare_eq(r);
}

namespace yggr
{
namespace nsql_database_system
{

// ==
template<typename BsonIter> inline
typename boost::enable_if<boost::is_base_of<bson_iter_t, BsonIter>, bool>::type
	operator==(const c_bson_const_iterator& l, const BsonIter& r)
{
	return l.compare_eq(r);
}

template<typename BsonIter> inline
typename boost::enable_if<boost::is_base_of<bson_iter_t, BsonIter>, bool>::type
	operator!=(const c_bson_const_iterator& l, const BsonIter& r)
{
	return !l.compare_eq(r);
}

} // namesapce nsql_database_system
} // namespace yggr

// swap

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{

template<typename BsonIter> inline
typename boost::enable_if<boost::is_base_of<bson_iter_t, BsonIter>, void>::type
	swap(c_bson_const_iterator& l, BsonIter& r)
{
	l.swap(r);
}

inline void swap(c_bson_const_iterator& l, c_bson_const_iterator& r)
{
	l.swap(r);
}

} // namespace swap_support

using swap_support::swap;

} // namespace nsql_database_system
} // namespace yggr

namespace std
{
	using ::yggr::nsql_database_system::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::nsql_database_system::swap_support::swap;
} // namespace boost

// c_bson
namespace yggr
{
namespace nsql_database_system
{

class c_bson
	: public bson_t // must use public extends
{
private:
	friend class c_bson_access_proxy;
public:
	typedef bson_t base_type;
	typedef base_type org_type;

	typedef c_bson_const_iterator const_iterator;
	typedef const_iterator iterator;

	typedef bson_mark_splice mark_type;

public:
	typedef utf8_string inner_string_type;
	typedef utf8_string_view inner_string_view_type;

	// don't append this code, because bson is multimap not map !!!
//private:
//	class save_helper
//	{
//	public:
//		typedef c_bson outside_type;
//
//	private:
//		typedef save_helper this_type;
//
//	public:
//		template<typename Char>
//		save_helper(outside_type& outside,
//					const Char* name )
//			: _outside(outside),
//				_name(name)
//
//		{
//		}
//
//		template<typename Char, typename Traits, typename Alloc,
//					template<typename _Char, typename _Traits, typename _Alloc> class Basic_String>
//		save_helper(outside_type& outside,
//					const Basic_String<Char, Traits, Alloc>& name )
//			: _outside(outside),
//				_name(name)
//
//		{
//		}
//
//		save_helper(const this_type& right);
//		~save_helper(void);
//
//		operator c_bson_value(void) const;
//
//		template<typename Mark>
//		outside_type& operator=(const bson_mark_begin<Mark>& mark)
//		{
//			return _outside;
//		}
//
//		template<typename Mark>
//		outside_type& operator=(const bson_mark_end<Mark>& mark)
//		{
//			return _outside;
//		}
//
//		outside_type& operator=(const bson_mark_begin<bson_mark_splice>& mark);
//		outside_type& operator=(const bson_mark_end<bson_mark_splice>& mark);
//
//		template<typename T>
//		outside_type& operator=(const T& t)
//		{
//			_outside.save(_name, t);
//			return _outside;
//		}
//
//	private:
//		outside_type& _outside;
//		inner_string_type _name;
//	};

private:
	typedef c_bson this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

private:
	typedef mplex::sfinae_type sfinae_type;

public:
	c_bson(void);

	template<typename Char>
	c_bson(const Char* str_json)
	{
		YGGR_ASSERT_CHAR_TYPE(Char);
		memset(static_cast<base_type*>(this), 0, sizeof(base_type));
		::bson_init(this);
		this_type::assign(str_json);
	}

	// string, utf8_string,
	template<typename T1, typename T2, typename T3,
				template<typename _T1, typename _T2, typename _T3> class TPL3 >
	c_bson(const TPL3<T1, T2, T3>& str_json)
	{
		memset(static_cast<base_type*>(this), 0, sizeof(base_type));
		::bson_init(this);
		this_type::assign(str_json);
	}

	// string_view, utf8_string_view, string_charset_helper
	template<typename T1, typename T2,
				template<typename _T1, typename _T2> class TPL2 >
	c_bson(const TPL2<T1, T2>& str_json)
	{
		memset(static_cast<base_type*>(this), 0, sizeof(base_type));
		::bson_init(this);
		this_type::assign(str_json);
	}

	// bson can not support iterator assign, because bson is a unordered-tree like filesystem !!!
	//template<typename InputIter>
	//c_bson(InputIter s, InputIter e);

	c_bson(const bson_value_t& val);

	c_bson(BOOST_RV_REF(base_type) right)
	{
		memset(static_cast<base_type*>(this), 0, sizeof(base_type));
		::bson_init(this);
		base_type& right_ref = right;
		bson_native_ex::s_bson_swap(this, &right_ref);
	}

	c_bson(const base_type& base);
	explicit c_bson(const base_type* right);

	c_bson(BOOST_RV_REF(this_type) right)
	{
		memset(static_cast<base_type*>(this), 0, sizeof(base_type));
		::bson_init(this);
		this_type& right_ref = right;
		bson_native_ex::s_bson_swap(this, &right_ref);
	}

	c_bson(const this_type& right);
	~c_bson(void);

public:
	template<typename String> inline
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String>,
			this_type&
		>::type
		operator=(const String& str_json)
	{
		this_type::assign(str_json);
		return *this;
	}

	template<typename BsonValue> inline
	typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, this_type&>::type
		operator=(const BsonValue& tval)
	{
		this_type::assign(tval);
		return *this;
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, this_type&>::type
		operator=(BOOST_RV_REF(Bson) right)
	{
		this_type::assign(boost::move(right));
		return *this;
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, this_type&>::type
		operator=(const Bson& right)
	{
		this_type::assign(right);
		return *this;
	}

	// don't delete operator= this_type, because cpp using default bson_t operator= first
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type::assign(boost::move(right));
		return *this;
	}

	this_type& operator=(const this_type& right);

protected:
	void pro_swap(base_type& right);

public:
	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, void>::type
		swap(BOOST_RV_REF(Bson) right)
	{
		Bson& right_ref = right;
		this_type::pro_swap(static_cast<base_type&>(right_ref));
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, void>::type
		swap(Bson& right)
	{
		base_type& right_ref = right;
		this_type::pro_swap(right_ref);
	}

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::pro_swap(static_cast<base_type&>(right_ref));
	}

	void swap(this_type& right);

public:
	inline org_type* org_pointer(void)
	{
		return static_cast<org_type*>(this);
	}

	inline const org_type* org_pointer(void) const
	{
		return static_cast<const org_type*>(this);
	}

public:
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	operator Basic_String<Char, Traits, Alloc>(void) const
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		BOOST_MPL_ASSERT((charset::is_string_t<string_type>));

		return this_type::to_json<string_type>();
	}

public:
	void init(void);

protected:
	void pro_assign_bson_value_t(const bson_value_t& val);

public:
	template<typename String> inline
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String>,
			void
		>::type
		assign(const String& str_json)
	{
		typedef String now_string_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, now_inner_string_type);

		now_inner_string_type utf8_json((charset::string_charset_helper_data(str_json)));
		if(utf8_json.empty())
		{
			this_type::init();
		}
		else
		{
			bson_native_ex::s_bson_init_from_json(
				this, utf8_json.data(), utf8_json.size(), 0);
		}
	}

	template<typename BsonValue> inline
	typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, void>::type
		assign(const BsonValue& tval)
	{
		const bson_value_t& val(tval);
		this_type::pro_assign_bson_value_t(val);
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, void>::type
		assign(BOOST_RV_REF(Bson) right)
	{
		Bson& right_ref = right;
		base_type& base_right_ref = right_ref;
		bson_native_ex::s_bson_swap(static_cast<bson_t*>(this), boost::addressof(base_right_ref));
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, void>::type
		assign(const Bson& right)
	{
		const base_type& right_ref = right;
		bson_native_ex::s_bson_copy(static_cast<bson_t*>(this), boost::addressof(right_ref));
	}

public:
	inline void clear(void)
	{
		this_type::init();
	}

	inline void clear_to_nil(void)
	{
		base_type& base = *this;
		if(!this_type::is_nil())
		{
			bson_destroy(this);
			memset(&base, 0, sizeof(base_type));
		}
	}

public:
	inline s32 finish(void) const
	{
		return this_type::is_nil()? 0 : 1;
	}

	inline bool is_finished(void) const
	{
		return !this_type::is_nil();
	}

	inline bool is_operator_enable(void) const
	{
		return !this_type::is_nil();
	}

	inline bool is_nil(void) const
	{
		return !base_type::len;
	}

	inline bool empty(void) const
	{
		return this_type::is_nil() || bson_empty(this);
	}

	inline bool is_array(void) const
	{
		return bson_native_ex::s_bson_is_array(this);
	}

	template<typename String> inline
	typename boost::enable_if<charset::utf8_string_constructable<String>, bool>::type
		has_field(const String& name) const
	{
		typedef String now_string_type;
		// don't change to YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL, because bson_has_field not support (const char*, size)
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_string_type, now_inner_string_type);

		now_inner_string_type str_name(charset::string_charset_helper_data(name));
		return (!str_name.empty()) && bson_has_field(str_name.data());
	}

	inline const u8* data(void) const
	{
		return bson_native_ex::s_bson_data(this);
	}

	inline u8* data(void)
	{
		return bson_native_ex::s_bson_data(this);
	}

	inline size_type size(void) const
	{
		return base_type::len;
	}

	inline size_type count_key(void) const
	{
		return this_type::empty()? 0 : bson_count_keys(this);
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, bool>::type
		concat(const Bson& right)
	{
		return
			this == boost::addressof(right)?
				bson_native_ex::s_bson_concat_self(this)
				: ::bson_concat(this, boost::addressof(right));
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, s32>::type
		compare(const Bson& right) const
	{
		return ::bson_compare(this, boost::addressof(right));
	}

	inline inner_string_type to_json(void) const
	{
		return bson_native_ex::s_bson_to_json<inner_string_type>(this);
	}

	template<typename String> inline
	String to_json(const utf8_string::base_type& dst_charset_name
							= charset::charset_name_t<typename String::value_type>()) const
	{
		typedef String now_string_type;
		return bson_native_ex::s_bson_to_json<now_string_type>(this);
	}

	inline inner_string_type to_json_canonical(void) const
	{
		return bson_native_ex::s_bson_to_json_canonical<inner_string_type>(this);
	}

	template<typename String> inline
	String to_json_canonical(const utf8_string::base_type& dst_charset_name
								= charset::charset_name_t<typename String::value_type>()) const
	{
		typedef String now_string_type;
		return bson_native_ex::s_bson_to_json_canonical<now_string_type>(this);
	}

	inline inner_string_type to_json_relaxed(void) const
	{
		return bson_native_ex::s_bson_to_json_relaxed<inner_string_type>(this);
	}

	template<typename String> inline
	String to_json_relaxed(const utf8_string::base_type& dst_charset_name
								= charset::charset_name_t<typename String::value_type>()) const
	{
		typedef String now_string_type;
		return bson_native_ex::s_bson_to_json_relaxed<now_string_type>(this);
	}

	template<typename Handler> inline
	this_type select(const Handler& handler) const
	{
		this_type tmp;
		bson_native_ex::s_bson_select_copy(boost::addressof(tmp), this, handler);
		return tmp;
	}

	template<typename Bson, typename Handler> inline
	Bson& select(Bson& out, const Handler& handler) const
	{
		bson_native_ex::s_bson_select_copy(boost::addressof(out), this, handler);
		return out;
	}

public:
	inline const_iterator begin(void) const
	{
		const_iterator iter(*this);
		return ++iter;
	}

	inline const_iterator cbegin(void) const
	{
		return this_type::begin();
	}

	inline const_iterator end(void) const
	{
		const_iterator iter;
		iter.key = static_cast<uint32_t>(this_type::size());
		iter.type = iter.off = iter.key - 1;
		return iter;
	}

	inline const_iterator cend(void) const
	{
		return this_type::end();
	}

public:

	// don't append operator[]. because bson or json like a std::multimap not std::map
	//template<typename Char>
	//save_helper operator[](const Char* key)
	//{
	//	YGGR_ASSERT_CHAR_TYPE(Char);

	//	assert(key);
	//	inner_string_type utf8_key(key);
	//	return save_helper(*this, key);
	//}

	//template<typename Char, typename Traits, typename Alloc,
	//			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String>
	//save_helper operator[](const Basic_String<Char, Traits, Alloc>& key)
	//{
	//	YGGR_ASSERT_CHAR_TYPE(Char);
	//	typedef Basic_String<Char, Traits, Alloc> now_string_type;

	//	YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_string_type, utf8_string_type);

	//	utf8_string_type utf8_key = key;

	//	return save_helper(*this, key);
	//}

	//template<typename Char>
	//c_bson_value operator[](const Char* key) const
	//{

	//	YGGR_ASSERT_CHAR_TYPE(Char);

	//	assert(key);
	//	inner_string_type utf8_key(key);

	//	iterator iter(*this);
	//	iterator ret_iter = iter.find(key);
	//	return ret_iter == this_type::end()?
	//			c_bson_value() : ret_iter.get_value();
	//}

	//template<typename Char, typename Traits, typename Alloc,
	//			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String>
	//c_bson_value operator[](const Basic_String<Char, Traits, Alloc>& key) const
	//{
	//	YGGR_ASSERT_CHAR_TYPE(Char);
	//	typedef Basic_String<Char, Traits, Alloc> now_string_type;

	//	YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_string_type, utf8_string_type);

	//	inner_string_type utf8_key = key;

	//	iterator iter(*this);
	//	iterator ret_iter = iter.find(key);
	//	return ret_iter == this_type::end()?
	//			c_bson_value() : ret_iter.get_value();
	//}

	template<typename String> inline
	typename boost::enable_if<charset::utf8_string_constructable<String>, iterator>::type
		find(const String& key) const
	{
		iterator iter(*this);
		return iter.find(key);
	}

	template<typename String> inline
	typename boost::enable_if<charset::utf8_string_constructable<String>, c_bson_value>::type
		get_value(const String& key) const
	{
		iterator iter(*this);
		iterator ret_iter = iter.find(key);
		return
			ret_iter == this_type::end()?
				c_bson_value() : ret_iter.get_value();
	}

	template<typename T, typename String> inline
	typename boost::enable_if<charset::utf8_string_constructable<String>, T>::type
		get_value(const String& key) const
	{
		iterator iter(*this);
		iterator ret_iter = iter.find(key);
		return
			ret_iter == this_type::end()?
				T() : ret_iter.get_value<T>();
	}

public:
	//save
	// save s8
	template<typename String> inline
	typename boost::enable_if<charset::utf8_string_constructable<String>, bool>::type
		save(const String& name, s8 val)
	{
		typedef String now_string_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, now_inner_string_type);

		if(this_type::is_nil())
		{
			::bson_init(this);
		}

		now_inner_string_type str_name(charset::string_charset_helper_data(name));
		return bson_append_int32(this, str_name.data(), str_name.size(), s32(val));
	}

	// save char
	template<typename String, typename Char> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String>,
				is_native_char<Char>
			>,
			bool
		>::type
		save(const String& name, Char val)
	{
		typedef String now_string_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, now_inner_string_type);

		if(this_type::is_nil())
		{
			::bson_init(this);
		}

		now_inner_string_type str_name(charset::string_charset_helper_data(name));
		return bson_append_int32(this, str_name.data(), str_name.size(), s32(val));
	}

	// save u8
	template<typename String> inline
	typename boost::enable_if<charset::utf8_string_constructable<String>, bool>::type
		save(const String& name, u8 val)
	{
		typedef String now_string_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, now_inner_string_type);

		if(this_type::is_nil())
		{
			::bson_init(this);
		}

		now_inner_string_type str_name(charset::string_charset_helper_data(name));
		return bson_append_int32(this, str_name.data(), str_name.size(), s32(val));
	}

	// save s16
	template<typename String> inline
	typename boost::enable_if<charset::utf8_string_constructable<String>, bool>::type
		save(const String& name, s16 val)
	{
		typedef String now_string_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, now_inner_string_type);

		if(this_type::is_nil())
		{
			::bson_init(this);
		}

		now_inner_string_type str_name(charset::string_charset_helper_data(name));
		return bson_append_int32(this, str_name.data(), str_name.size(), s32(val));
	}

	// save u16
	template<typename String> inline
	typename boost::enable_if<charset::utf8_string_constructable<String>, bool>::type
		save(const String& name, u16 val)
	{
		typedef String now_string_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, now_inner_string_type);

		if(this_type::is_nil())
		{
			::bson_init(this);
		}

		now_inner_string_type str_name(charset::string_charset_helper_data(name));
		return bson_append_int32(this, str_name.data(), str_name.size(), s32(val));
	}

	// save s32
	template<typename String> inline
	typename boost::enable_if<charset::utf8_string_constructable<String>, bool>::type
		save(const String& name, s32 val)
	{
		typedef String now_string_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, now_inner_string_type);

		if(this_type::is_nil())
		{
			::bson_init(this);
		}

		now_inner_string_type str_name(charset::string_charset_helper_data(name));
		return bson_append_int32(this, str_name.data(), static_cast<s32>(str_name.size()), val);
	}

	// save u32
	template<typename String> inline
	typename boost::enable_if<charset::utf8_string_constructable<String>, bool>::type
		save(const String& name, u32 val)
	{
		typedef String now_string_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, now_inner_string_type);

		if(this_type::is_nil())
		{
			::bson_init(this);
		}

		now_inner_string_type str_name(charset::string_charset_helper_data(name));
		return bson_append_int32(this, str_name.data(), static_cast<s32>(str_name.size()),
									*(reinterpret_cast<s32*>(&val)));
	}

	// save s64
	template<typename String> inline
	typename boost::enable_if<charset::utf8_string_constructable<String>, bool>::type
		save(const String& name, s64 val)
	{
		typedef String now_string_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, now_inner_string_type);

		if(this_type::is_nil())
		{
			::bson_init(this);
		}

		now_inner_string_type str_name(charset::string_charset_helper_data(name));
		return bson_append_int64(this, str_name.data(), str_name.size(), val);
	}

	// save u64
	template<typename String > inline
	typename boost::enable_if<charset::utf8_string_constructable<String>, bool>::type
		save(const String& name, u64 val)
	{
		typedef String now_string_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, now_inner_string_type);

		if(this_type::is_nil())
		{
			::bson_init(this);
		}

		now_inner_string_type str_name(charset::string_charset_helper_data(name));
		return bson_append_int64(this, str_name.data(), str_name.size(),
									*(reinterpret_cast<s64*>(&val)));
	}

	// save bool
	template<typename String> inline
	typename boost::enable_if<charset::utf8_string_constructable<String>, bool>::type
		save(const String& name, bool val)
	{
		typedef String now_string_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, now_inner_string_type);

		if(this_type::is_nil())
		{
			::bson_init(this);
		}

		now_inner_string_type str_name(charset::string_charset_helper_data(name));
		return bson_append_bool(this, str_name.data(), static_cast<int>(str_name.size()), val);
	}

	// save f32
	template<typename String> inline
	typename boost::enable_if<charset::utf8_string_constructable<String>, bool>::type
		save(const String& name, f32 val)
	{
		typedef String now_string_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, now_inner_string_type);

		if(this_type::is_nil())
		{
			::bson_init(this);
		}

		now_inner_string_type str_name(charset::string_charset_helper_data(name));
		return bson_append_double(this, str_name.data(), str_name.size(), f64(val));
	}

	// save f64
	template<typename String > inline
	typename boost::enable_if<charset::utf8_string_constructable<String>, bool>::type
		save(const String& name, f64 val)
	{
		typedef String now_string_type;
		//YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_string_type, now_inner_string_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, now_inner_string_type);

		if(this_type::is_nil())
		{
			::bson_init(this);
		}

		now_inner_string_type str_name(charset::string_charset_helper_data(name));
		return bson_append_double(this, str_name.data(), str_name.size(), val);
	}

	// save string
	template<typename String_Name, typename String_Val> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String_Name>,
				charset::utf8_string_constructable<String_Val>,
				boost::mpl::not_< is_native_char<String_Val> >
			>,
			bool
		>::type
		save(const String_Name& name, const String_Val& val)
	{
		typedef String_Name now_string_name_type;
		typedef String_Val now_string_val_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_name_type, now_inner_string_name_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_val_type, now_inner_string_val_type);

		if(this_type::is_nil())
		{
			::bson_init(this);
		}

		now_inner_string_name_type str_name(charset::string_charset_helper_data(name));
		now_inner_string_val_type str_val(charset::string_charset_helper_data(val));
		return
			bson_append_utf8(
				this,
				str_name.data(), static_cast<s32>(str_name.size()),
				str_val.data(), static_cast<s32>(str_val.size()));
	}

	// save this_type
	template<typename String, typename Bson> inline
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
		save(const String& name, const Bson& val)
	{
		typedef String now_string_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, now_inner_string_type);

		assert(val.len);
		if(this_type::is_nil())
		{
			::bson_init(this);
		}

		now_inner_string_type str_name(charset::string_charset_helper_data(name));

		return
			this == boost::addressof(val)?
				bson_native_ex::s_bson_save_self(this, str_name.data(), static_cast<s32>(str_name.size()))
				: bson_append_document(this, str_name.data(), static_cast<int>(str_name.size()), boost::addressof(val));
	}

	// save array start and end
	template<typename String> inline
	typename boost::enable_if<charset::utf8_string_constructable<String>, this_type&>::type
		save_array_start(const String& name, const bson_mark_splice& mark)
	{
		if(this_type::save_name(bson_typeid_def::E_BSON_TYPE_ARRAY, name))
		{
			this_type::save_mark_start(mark);
		}

		return *this;
	}

	inline this_type& save_array_end(const bson_mark_splice& mark)
	{
		this_type::save_mark_end(mark);
		return *this;
	}

	// save object start and end
	template<typename String> inline
	typename boost::enable_if<charset::utf8_string_constructable<String>, this_type&>::type
		save_document_start(const String& name, const bson_mark_splice& mark)
	{
		if(this_type::save_name(bson_typeid_def::E_BSON_TYPE_DOCUMENT, name))
		{
			this_type::save_mark_start(mark);
		}

		return *this;
	}

	inline this_type& save_document_end(const bson_mark_splice& mark)
	{
		this_type::save_mark_end(mark);
		return *this;
	}

	// save iterator don't use to serialization
	template<typename String, typename BsonIter> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_base_of<bson_iter_t, BsonIter>,
				charset::utf8_string_constructable<String>
			>,
			bool
		>::type
		save(const String& name, const BsonIter& val)
	{
		typedef String now_string_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, now_inner_string_type);

		if(this_type::is_nil())
		{
			::bson_init(this);
		}

		now_inner_string_type str_name(charset::string_charset_helper_data(name));
		return bson_append_iter(this, str_name.data(), str_name.size(), boost::addressof(val));
	}

	template<typename String, typename Obj> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String>,
				boost::mpl::and_
				<
					boost::mpl::not_< boost::is_base_of<bson_t, Obj> >,
					boost::mpl::not_< boost::is_base_of<bson_iter_t, Obj> >,
					boost::mpl::not_< charset::utf8_string_constructable<Obj> >
				>
			>,
			bool
		>::type
		save(const String& name, const Obj& obj)
	{
		if(this_type::is_nil())
		{
			::bson_init(this);
		}

		return bson_inner_data_access_proxy::save(*this, name, obj);
	}

	//c_bson_date
	//c_bson_time
	//c_bson_oid
	//c_bson_binary_buffer
	//c_bson_null
	//c_bson_undefined
	//c_bson_regex
	//these types provided by the member function save

protected:
	// save_name
	template<typename String> inline
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String>,
			bool
		>::type
		save_name(u8 type, const String& name)
	{
		typedef String now_string_type;
		//YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_string_type, now_inner_string_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, now_inner_string_type);

		now_inner_string_type utf8_name((charset::string_charset_helper_data(name)));
		return bson_native_ex::s_bson_save_name(this, type, utf8_name.data(), static_cast<s32>(utf8_name.size()));
	}

	template<typename Obj, typename String> inline
	typename boost::enable_if<charset::utf8_string_constructable<String>, bool>::type
		save_name(const String& name)
	{
		typedef String now_string_type;
		//YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_string_type, now_inner_string_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, now_inner_string_type);

		now_inner_string_type utf8_name((charset::string_charset_helper_data(name)));
		return
			bson_native_ex::s_bson_save_name(
				this,
				bson_overlap_typeid
				<
					typename boost::remove_const<Obj>::type
				>::value,
				utf8_name.data(), utf8_name.size());
	}

	bool save_mark_start(const bson_mark_splice& mark);
	bool save_mark_end(const bson_mark_splice& mark);

	template<typename Mark> inline
	bool save_mark_start(const Mark&)
	{
		return true;
	}

	template<typename Mark> inline
	bool save_mark_end(const Mark&)
	{
		return true;
	}

	template<typename Obj, typename String, typename Mark> inline
	typename boost::enable_if<charset::utf8_string_constructable<String>, bool>::type
		save_begin(const String& name, const Mark& mark)
	{
		return
			this_type::save_name<Obj>(name)
			&& this_type::save_mark_start(mark);
	}

	template<typename String, typename Mark> inline
	typename boost::enable_if<charset::utf8_string_constructable<String>, bool>::type
		save_begin(u8 t, const String& name, const Mark& mark)
	{
		return
			this_type::save_name(t, name)
			&& this_type::save_mark_start(mark);
	}

	template<typename Obj, typename Mark> inline
	bool save_end(const Mark& mark)
	{
		return this_type::save_mark_end(mark);
	}

public:
	//save_value
	inline bool save_value(s8 val)
	{
		return bson_native_ex::s_bson_save_value_of_s32(this, static_cast<s32>(val));
	}

	template<typename Char> inline
	typename boost::enable_if<is_native_char<Char>, bool>::type
		save_value(Char val)
	{
		return bson_native_ex::s_bson_save_value_of_s32(this, static_cast<s32>(val));
	}

	inline bool save_value(u8 val)
	{
		return bson_native_ex::s_bson_save_value_of_s32(this, static_cast<s32>(val));
	}

	inline bool save_value(s16 val)
	{
		return bson_native_ex::s_bson_save_value_of_s32(this, static_cast<s32>(val));
	}

	inline bool save_value(u16 val)
	{
		return bson_native_ex::s_bson_save_value_of_s32(this, static_cast<s32>(val));
	}

	inline bool save_value(s32 val)
	{
		return bson_native_ex::s_bson_save_value_of_s32(this, val);
	}

	inline bool save_value(u32 val)
	{
		return bson_native_ex::s_bson_save_value_of_s32(this, *(reinterpret_cast<s32*>(&val)));
	}

	inline bool save_value(s64 val)
	{
		return bson_native_ex::s_bson_save_value_of_s64(this, val);
	}

	inline bool save_value(u64 val)
	{
		return bson_native_ex::s_bson_save_value_of_s64(this, *(reinterpret_cast<s64*>(&val)));
	}

	inline bool save_value(bool val)
	{
		return bson_native_ex::s_bson_save_value_of_bool(this, val);
	}

	inline bool save_value(f32 val)
	{
		return bson_native_ex::s_bson_save_value_of_f64(this, static_cast<f64>(val));
	}

	inline bool save_value(f64 val)
	{
		return bson_native_ex::s_bson_save_value_of_f64(this, val);
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_< charset::is_can_be_conv_into_char<String> >, // fix conflict save_value(Char val)
				charset::utf8_string_constructable<String>
			>,
			bool
		>::type
		save_value(const String& val)
	{
		typedef String now_string_type;
		//YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_string_type, now_inner_string_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, now_inner_string_type);

		now_inner_string_type now_val((charset::string_charset_helper_data(val)));
		return bson_native_ex::s_bson_save_value_of_utf8(this, now_val.data(), now_val.size());
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, bool>::type
		save_value(const Bson& val)
	{
		const base_type& base = val;
		return bson_native_ex::s_bson_save_value_of_bson(this, boost::addressof(base));
	}

	template<typename BsonIter> inline
	typename boost::enable_if<boost::is_base_of<bson_iter_t, BsonIter>, bool>::type
		save_value(const BsonIter& val)
	{
		return bson_native_ex::s_bson_save_value_of_iter(this, boost::addressof(val));
	}

	template<typename Obj> inline
	typename
		boost::disable_if
		<
			boost::mpl::or_
			<
				boost::is_base_of<bson_t, Obj>,
				boost::is_base_of<bson_iter_t, Obj>,
				charset::utf8_string_constructable<Obj>
			>,
			bool
		>::type
		save_value(const Obj& obj)
	{
		return bson_inner_data_access_proxy::save_value(*this, obj);
	}

protected:
	bool pro_compare_eq(const base_type& right) const;

public:
	template<typename BsonValue> inline
	typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
		compare_eq(const BsonValue& right) const
	{
		return
			bson_native_ex::s_bson_is_array(this)?
				bson_value_native_ex::s_value_cmp_eq_array(boost::addressof(right), this)
				: bson_value_native_ex::s_value_cmp_eq_bson(boost::addressof(right), this);
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<base_type, Bson>, bool>::type
		compare_eq(const Bson& right) const
	{
		const base_type& right_ref = right;
		return this_type::pro_compare_eq(right_ref);
	}

};

namespace detail
{

template<>
struct bson_operator_outter<c_bson>
{
	template<typename Char, typename Traits> inline
	std::basic_ostream<Char, Traits>&
		operator()(std::basic_ostream<Char, Traits>& os,
					const c_bson& cb,
					bson_json_mode_def::mode_type mode) const
	{
		typedef c_bson::org_type org_bson_type;
		typedef bson_operator_outter<org_bson_type> outter_type;

		outter_type outter;
		return outter(os, static_cast<const org_bson_type&>(cb), mode);
	}
};

} // namespace detail

template<typename Char, typename Traits> inline
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os, const c_bson& cb)
{
	typedef detail::bson_operator_outter<c_bson> outter_type;

	outter_type outter;
	return outter(os, cb, bson_json_mode_def::E_BSON_JSON_MODE_LEGACY);
}

// need test, if ok, remove it
//template<typename Char, typename Traits>
//std::basic_ostream<Char, Traits>&
//	operator<<(std::basic_ostream<Char, Traits>& os,
//				BOOST_RV_REF(yggr::nsql_database_system::c_bson) cb)
//{
//	const yggr::nsql_database_system::c_bson& right_cref = cb;
//	return yggr::nsql_database_system::operator<<(os, right_cref);
//}

// now don't use this foo style, but don't delete it
//template<typename Char, typename Traits,
//			template<typename _Char, typename _Traits> class Basic_OStream >
//Basic_OStream<Char, Traits>&
//	operator<<(Basic_OStream<Char, Traits>& os,
//				const yggr::nsql_database_system::c_bson& cb);

} // namespace nsql_database_system
} // namespace yggr

// nonmember foo

// + // return c_bson
template<typename Bson> inline
typename boost::enable_if<boost::is_base_of<bson_t, Bson>, yggr::nsql_database_system::c_bson>::type
	operator+(const bson_t& l, const Bson& r)
{
	typedef yggr::nsql_database_system::c_bson ret_bson_type;
	ret_bson_type tmp(l);
	tmp.concat(r);
	return tmp;
}

// +=
template<typename Bson> inline
typename boost::enable_if<boost::is_base_of<bson_t, Bson>, bson_t&>::type
	operator+=(bson_t& l, const Bson& r)
{
	return
		(boost::addressof(l) == boost::addressof(r)?
			::yggr::nsql_database_system::bson_native_ex::s_bson_concat_self(boost::addressof(l))
			: ::bson_concat(boost::addressof(l), boost::addressof(r)),
		l);
}

// ==
template<typename BsonValue> inline
typename boost::enable_if< boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator==(const bson_t& l, const BsonValue& r)
{
	return
		yggr::nsql_database_system::bson_native_ex::s_bson_is_array(boost::addressof(l))?
			yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_array(
				boost::addressof(r), boost::addressof(l))
			: yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_bson(
				boost::addressof(r), boost::addressof(l));
}


template<typename Bson> inline
typename boost::enable_if<boost::is_base_of<bson_t, Bson>, bool>::type
	operator==(const bson_t& l, const Bson& r)
{
	return
		(l.len == 0 || r.len == 0)?
			l.len == r.len
			: bson_equal(boost::addressof(l), boost::addressof(r));
}

// !=
template<typename BsonValue> inline
typename boost::enable_if< boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator!=(const bson_t& l, const BsonValue& r)
{
	return
		!(yggr::nsql_database_system::bson_native_ex::s_bson_is_array(boost::addressof(l))?
			yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_array(
				boost::addressof(r), boost::addressof(l))
			: yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_bson(
				boost::addressof(r), boost::addressof(l)) );
}


template<typename Bson> inline
typename boost::enable_if<boost::is_base_of<bson_t, Bson>, bool>::type
	operator!=(const bson_t& l, const Bson& r)
{
	return
		!((l.len == 0 || r.len == 0)?
			l.len == r.len
			: bson_equal(boost::addressof(l), boost::addressof(r)));
}

namespace yggr
{
namespace nsql_database_system
{

// + // return c_bson
template<typename Bson> inline
typename boost::enable_if<boost::is_base_of<bson_t, Bson>, yggr::nsql_database_system::c_bson>::type
	operator+(const c_bson& l, const Bson& r)
{
	c_bson tmp(l);
	::bson_concat(boost::addressof(tmp), boost::addressof(r));
	return tmp;
}

// +=
template<typename Bson> inline
typename boost::enable_if<boost::is_base_of<bson_t, Bson>, c_bson&>::type
	operator+=(c_bson& l, const Bson& r)
{
	return (l.concat(r), l);
}

// ==
template<typename BsonValue> inline
typename boost::enable_if< boost::is_base_of< bson_value_t, BsonValue>, bool>::type
	operator==(const c_bson& l, const BsonValue& r)
{
	return l.compare_eq(r);
}

template<typename Bson> inline
typename boost::enable_if<boost::is_base_of<bson_t, Bson>, bool>::type
	operator==(const c_bson& l, const Bson& r)
{
	return l.compare_eq(r);
}

// !=
template<typename BsonValue> inline
typename boost::enable_if< boost::is_base_of< bson_value_t, BsonValue>, bool>::type
	operator!=(const c_bson& l, const BsonValue& r)
{
	return !l.compare_eq(r);
}

template<typename Bson> inline
typename boost::enable_if<boost::is_base_of<bson_t, Bson>, bool>::type
	operator!=(const c_bson& l, const Bson& r)
{
	return !l.compare_eq(r);
}

} // namespace nsql_database_system
} // namespace yggr


namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{

template<typename Bson> inline
typename boost::enable_if<boost::is_base_of<bson_t, Bson>, void>::type
	swap(c_bson& l, Bson& r)
{
	l.swap(r);
}

inline void swap(c_bson& l, c_bson& r)
{
	l.swap(r);
}

} // namespace swap_support

using swap_support::swap;

} // namespace nsql_database_system
} // namespace yggr

namespace std
{
	using ::yggr::nsql_database_system::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::nsql_database_system::swap_support::swap;
} // namespace boost

#if defined(BOOST_MOVE_TT_CXX11_IS_COPY_CONSTRUCTIBLE)
namespace boost
{
namespace move_detail
{

template<>
struct is_copy_constructible<yggr::nsql_database_system::c_bson>
{
	static const bool value = true;
};

} // namespace modetail
} // namespace boost
#endif // BOOST_MOVE_TT_CXX11_IS_COPY_CONSTRUCTIBLE

#endif //__YGGR_NSQL_DATABASE_SYSTEM_C_BSON_HPP__

