//c_bson_oid.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_OID_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_OID_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/charset/utf8_string.hpp>
#include <yggr/move/move.hpp>

#include <yggr/ppex/friend.hpp>
#include <yggr/type_traits/is_native_char.hpp>
#include <yggr/mplex/tag_sfinae.hpp>

#include <yggr/nsql_database_system/bson_typeid.hpp>
#include <yggr/nsql_database_system/bson_inner_data_accesser_decl.hpp>

//#include <yggr/nsql_database_system/bson_oid_native_ex.hpp>
//#include <yggr/nsql_database_system/bson_native_ex.hpp>
#include <yggr/nsql_database_system/bson_value_native_ex.hpp>

#include <yggr/nsql_database_system/bson_visit_t.hpp>

#include <yggr/nsql_database_system/detail/string_member_var_get.hpp>

#include <boost/ref.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/not.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/is_base_of.hpp>

#include <boost/iterator/iterator_traits.hpp>

#include <cassert>
#include <memory>

/*
typedef struct {
   uint8_t bytes[12];
} bson_oid_t;
*/

namespace yggr
{
namespace nsql_database_system
{

class c_bson_oid;

} // namespace nsql_database_system
} // namespace yggr

YGGR_PP_BSON_WRAP_TYPEID_IMPL(bson_oid_t, c_bson_oid, bson_typeid_def::E_BSON_TYPE_OID)
YGGR_PP_BSON_TYPEID_IMPL(c_bson_oid, bson_typeid_def::E_BSON_TYPE_OID)

namespace yggr
{
namespace nsql_database_system
{

class c_bson_oid
	: public bson_oid_t
{
public:
	YGGR_STATIC_CONSTANT(u32, E_bson_oid_byte_size = bson_oid_native_ex::E_bson_oid_byte_size);
	YGGR_STATIC_CONSTANT(u32, E_bson_oid_string_size = bson_oid_native_ex::E_bson_oid_string_size); // E_bson_oid_byte_size * 2
	YGGR_STATIC_CONSTANT(u32, E_bson_oid_char_buffer_size = bson_oid_native_ex::E_bson_oid_char_buffer_size); // E_bson_oid_string_size + 1

	typedef bson_oid_t id_type;
	typedef id_type base_type;
	typedef base_type org_type;

private:
	typedef c_bson_oid this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

private:
	BOOST_MPL_ASSERT((boost::mpl::bool_<(sizeof(base_type) == this_type::E_bson_oid_byte_size)>));

private:
	typedef mplex::sfinae_type sfinae_type;

public:
	explicit c_bson_oid(bson_context_t* pcontext = 0);

	template<typename Char>
	explicit c_bson_oid(const Char* str_oid,
						typename
							boost::enable_if
							<
								is_native_char<Char>,
								sfinae_type
							>::type sfinae = 0)
	{
		this_type::assign(str_oid);
	}

	explicit c_bson_oid(const char* str_oid, std::size_t len);

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String>
	explicit c_bson_oid(const Basic_String<Char, Traits, Alloc>& str_oid,
							typename
								boost::enable_if
								<
									charset::is_string_t< Basic_String<Char, Traits, Alloc> >,
									sfinae_type
								>::type sfinae = 0)
	{
		this_type::assign(str_oid);
	}

	template<typename Char, typename Traits,
				template<typename _Char, typename _Traits> class BasicStringView>
	explicit c_bson_oid(const BasicStringView<Char, Traits>& str_oid,
							typename
								boost::enable_if
								<
									charset::is_string_view_t< BasicStringView<Char, Traits> >,
									sfinae_type
								>::type sfinae = 0)
	{
		this_type::assign(str_oid);
	}


	template<typename T1, typename T2>
	explicit c_bson_oid(const charset::string_charset_helper<T1, T2>& str_oid)
	{
		this_type::assign(str_oid);
	}

	template<typename T>
	explicit c_bson_oid(const T* data,
						typename
							boost::disable_if
							<
								is_native_char<T>,
								sfinae_type
							>::type sfinae = 0)
	{
		this_type::assign(data);
	}

	template<typename Cont>
	explicit c_bson_oid(const Cont& data,
						typename
							boost::enable_if
							<
								container_ex::is_container<Cont>,
								sfinae_type
							>::type sfinae = 0)
	{
		this_type::assign(data);
	}

	template<typename Iter>
	c_bson_oid(Iter s, Iter e)
	{
		this_type::assign(s, e);
	}

	c_bson_oid(const bson_value_t& val);

	c_bson_oid(BOOST_RV_REF(base_type) oid)
	{
		base_type& oid_ref = oid;
		this_type::assign(oid_ref);
	}

	c_bson_oid(const base_type& oid);

	c_bson_oid(BOOST_RV_REF(this_type) right)
	{
		const this_type& right_ref = right;
		this_type::assign(right_ref);
	}

	c_bson_oid(const this_type& right);
	~c_bson_oid(void);

public:
	template<typename String> inline
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String>,
			this_type&
		>::type
		operator=(const String& str_oid)
	{
		this_type::assign(str_oid);
		return *this;
	}

	template<typename T> inline
	typename boost::disable_if<is_native_char<T>, this_type&>::type
		operator=(const T* data)
	{
		this_type::assign(data);
		return *this;
	}

	template<typename Cont> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				container_ex::is_container<Cont>,
				boost::mpl::not_< charset::utf8_string_constructable<Cont> >
			>,
			this_type&
		>::type
		operator=(const Cont& data)
	{
		this_type::assign(data);
		return *this;
	}

	template<typename BsonValue> inline
	typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, this_type&>::type
		operator=(const BsonValue& val)
	{
		this_type::assign(val);
		return *this;
	}

	template<typename BsonOid> inline
	typename boost::enable_if<boost::is_base_of<base_type, BsonOid>, this_type&>::type
		operator=(BOOST_RV_REF(BsonOid) right)
	{
		BsonOid& right_ref = right;
		this_type::assign(right_ref);
		return *this;
	}

	template<typename BsonOid> inline
	typename boost::enable_if<boost::is_base_of<base_type, BsonOid>, this_type&>::type
		operator=(const BsonOid& right)
	{
		this_type::assign(right);
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::assign(right_ref);
		return *this;
	}

	this_type& operator=(const this_type& right);

protected:
	void pro_swap(base_type& right);

public:
	template<typename BsonOid> inline
	typename boost::enable_if<boost::is_base_of<base_type, BsonOid>, void>::type
		swap(BOOST_RV_REF(BsonOid) right)
	{
		BsonOid& right_ref = right;
		this_type::pro_swap( static_cast<base_type&>(right_ref));
	}

	template<typename BsonOid> inline
	typename boost::enable_if<boost::is_base_of<base_type, BsonOid>, void>::type
		swap(BsonOid& right)
	{
		this_type::pro_swap(static_cast<base_type&>(right));
	}

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::pro_swap( static_cast<base_type&>(right_ref));
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
	YGGR_CONSTEXPR_OR_INLINE static u32 value_typeid(void)
	{
		return bson_typeid_def::E_BSON_TYPE_OID;
	}

	YGGR_CONSTEXPR_OR_INLINE static u32 s_value_typeid(void)
	{
		return bson_typeid_def::E_BSON_TYPE_OID;
	}

public:
	inline utf8_string to_string(const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const
	{
		typedef yggr::utf8_string inner_string_type;

		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());

		inner_string_type str;
		this_type::prv_get_string(str);
		return str;
	}

	//template<typename Alloc> inline
	//utf8_string<char, traits, Alloc>
	//	to_string(const Alloc& alloc, const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const;

	//template<typename String> inline
	//String to_string(const string& ret_charset_name = charset::charset_name_t<String>()) const;

	//template<typename String, typename Alloc> inline
	//typename boost::enable_if<container_ex::is_allocator<Alloc>, String>::type
	//	to_string(const Alloc& alloc, const string& ret_charset_name = charset::charset_name_t<String>()) const;

	YGGR_PP_C_MONGO_STRING_MEMBER_GET(to_string)

public:
	template<typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String > inline
	operator Basic_String<char, Traits, Alloc>(void) const
	{
		typedef Basic_String<char, Traits, Alloc> now_string_type;

		now_string_type str;
		this_type::prv_get_string(str);
		return str;
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String > inline
	operator Basic_String<Char, Traits, Alloc>(void) const
	{
		typedef Basic_String<Char, Traits, Alloc> now_string_type;
		typedef yggr::utf8_string inner_string_type;

		BOOST_MPL_ASSERT((charset::is_string_t<now_string_type>));

		inner_string_type str;
		this_type::prv_get_string(str);
		return str.str<now_string_type>(YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_string_type));
	}

	inline YGGR_OP_BOOL_EXPLICIT() operator bool(void) const
	{
		return !this_type::empty();
	}

public:
	inline bool empty(void) const
	{
		return this_type::compare_eq(this_type::s_gen_empty_oid());
	}

protected:
	void pro_assign_bson_value_t(const bson_value_t& val);

public:
	void assign(const char* str_oid, std::size_t);
	void assign(const char* str_oid);

	template<typename Char> inline
	typename boost::enable_if<is_native_char<Char>, void>::type
		assign(const Char* str_oid)
	{
		typedef yggr::utf8_string inner_string_type;
		inner_string_type tmp(str_oid);
		this_type::assign(tmp);
	}

	template<typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
	typename boost::enable_if< charset::is_string_t< Basic_String<char, Traits, Alloc> >, void>::type
		assign(const Basic_String<char, Traits, Alloc>& str_oid)
	{
		base_type& base = *this;

		size_type len = charset::string_byte_size(str_oid);

		if(len == this_type::E_bson_oid_string_size)
		{
			bson_oid_init_from_string(boost::addressof(base), str_oid.data());
		}
		else if(len == this_type::E_bson_oid_byte_size)
		{
			bson_oid_init_from_data(boost::addressof(base), reinterpret_cast<const uint8_t*>(str_oid.data()));
		}
		else
		{
			assert(!len);
			bson_oid_init(boost::addressof(base), 0);
		}
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
	typename boost::enable_if< charset::is_string_t< Basic_String<Char, Traits, Alloc> >, void>::type
		assign(const Basic_String<Char, Traits, Alloc>& str_oid)
	{
		typedef yggr::utf8_string inner_string_type;
		inner_string_type tmp(str_oid);
		this_type::assign(tmp);
	}

	template<typename Traits,
				template<typename _Char, typename _Traits> class BasicStringView> inline
	typename boost::enable_if< charset::is_string_view_t< BasicStringView<char, Traits> >, void>::type
		assign(const BasicStringView<char, Traits>& str_oid)
	{
		base_type& base = *this;

		size_type len = charset::string_byte_size(str_oid);

		if(len == this_type::E_bson_oid_string_size)
		{
			bson_oid_init_from_string(boost::addressof(base), str_oid.data());
		}
		else if(len == this_type::E_bson_oid_byte_size)
		{
			bson_oid_init_from_data(boost::addressof(base), reinterpret_cast<const uint8_t*>(str_oid.data()));
		}
		else
		{
			assert(!len);
			bson_oid_init(boost::addressof(base), 0);
		}
	}

	template<typename Char, typename Traits,
				template<typename _Char, typename _Traits> class BasicStringView> inline
	typename boost::enable_if< charset::is_string_view_t< BasicStringView<Char, Traits> >, void>::type
		assign(const BasicStringView<Char, Traits>& str_oid)
	{
		typedef yggr::utf8_string inner_string_type;
		inner_string_type tmp(str_oid);
		this_type::assign(tmp);
	}

	template<typename T1, typename T2> inline
	void assign(const charset::string_charset_helper<T1, T2>& str_oid)
	{
		typedef yggr::utf8_string inner_string_type;
		inner_string_type tmp(str_oid);
		this_type::assign(tmp);
	}

	template<typename T> inline
	typename boost::disable_if<is_native_char<T>, void>::type
		assign(const T* data)
	{
		if(reinterpret_cast<void* const>(this) == reinterpret_cast<const void*>(data))
		{
			return;
		}

		base_type& base = *this;
		if(!data)
		{
			bson_oid_init(boost::addressof(base), 0);
			return;
		}

		memcpy(boost::addressof(base), data, sizeof(base_type));
	}

	template<typename Cont> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				container_ex::is_container<Cont>,
				boost::mpl::not_< container_ex::is_random_access_container<Cont> >
			>,
			void
		>::type
		assign(const Cont& data)
	{
		typedef typename range_ex::range_value_ex<Cont>::type value_type;

		base_type& base = *this;

		if(bytes::byte_size(data) != this_type::E_bson_oid_byte_size)
		{
			bson_oid_init(boost::addressof(base), 0);
			return;
		}

		std::copy(data.begin(), data.end(), reinterpret_cast<value_type*>(boost::addressof(base)));
	}

	template<typename Cont> inline
	typename
		boost::enable_if
		<
			container_ex::is_random_access_container<Cont>,
			void
		>::type
		assign(const Cont& data)
	{
		typedef typename range_ex::range_value_ex<Cont>::type value_type;

		base_type& base = *this;

		if(bytes::byte_size(data) != this_type::E_bson_oid_byte_size)
		{
			bson_oid_init(boost::addressof(base), 0);
			return;
		}

		memcpy(boost::addressof(base), boost::addressof(data[0]), sizeof(base_type));
	}

	template<typename T, std::size_t N,
				template<typename _T, std::size_t _N> class Array > inline
	void assign(const Array<T, N>& data)
	{
		typedef T value_type;
		BOOST_MPL_ASSERT((boost::mpl::bool_<(N * sizeof(value_type) == this_type::E_bson_oid_byte_size)>));
		base_type& base = *this;
		memcpy(boost::addressof(base), boost::addressof(data[0]), sizeof(base_type));
	}

	template<typename InputIter> inline
	void assign(InputIter s, InputIter e)
	{
		typedef InputIter iter_type;
		typedef typename boost::iterator_value<iter_type>::type iter_val_type;

		base_type& base = *this;
		if(std::distance(s, e) * sizeof(iter_val_type) != this_type::E_bson_oid_byte_size)
		{
			assert(false);
			bson_oid_init(boost::addressof(base), 0);
		}

		std::copy(s, e, reinterpret_cast<iter_val_type*>(boost::addressof(base)));
	}

	template<typename BsonValue> inline
	typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, void>::type
		assign(const BsonValue& val)
	{
		this_type::pro_assign_bson_value_t(val);
	}

	template<typename BsonOid> inline
	typename boost::enable_if<boost::is_base_of<base_type, BsonOid>, void>::type
		assign(const BsonOid& oid)
	{
		if(this == boost::addressof(oid))
		{
			return;
		}

		base_type& l = static_cast<base_type&>(*this);
		const base_type& r = static_cast<const base_type&>(oid);
		memcpy(boost::addressof(l), boost::addressof(r), sizeof(base_type));
	}

public:
	// compare_eq
	template<typename Char> inline
	typename boost::enable_if<is_native_char<Char>, bool>::type
		compare_eq(const Char* str_oid) const
	{
		const base_type& base = *this;
		return bson_oid_native_ex::s_bson_oid_compare_eq(boost::addressof(base), str_oid);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename boost::enable_if< charset::is_string_t< Basic_String<Char, Traits, Alloc> >, bool>::type
		compare_eq(const Basic_String<Char, Traits, Alloc>& str_oid) const
	{
		const base_type& base = *this;
		return bson_oid_native_ex::s_bson_oid_compare_eq(boost::addressof(base), str_oid);
	}

	template<typename Char, typename Traits,
				template<typename _Char, typename _Traits> class BasicStringView > inline
	typename boost::enable_if< charset::is_string_view_t< BasicStringView<Char, Traits> >, bool>::type
		compare_eq(const BasicStringView<Char, Traits>& str_oid) const
	{
		const base_type& base = *this;
		return bson_oid_native_ex::s_bson_oid_compare_eq(boost::addressof(base), str_oid);
	}

	template<typename T> inline
	typename boost::disable_if<is_native_char<T>, bool>::type
		compare_eq(const T* data) const
	{
		const base_type& base = *this;
		return bson_oid_native_ex::s_bson_oid_compare_eq(boost::addressof(base), data);
	}

	template<typename Cont> inline
	typename
		boost::enable_if
		<
			container_ex::is_container<Cont>,
			bool
		>::type
		compare_eq(const Cont& data) const
	{
		const base_type& base = *this;
		return bson_oid_native_ex::s_bson_oid_compare_eq(boost::addressof(base), data);
	}

	template<typename InputIter> inline
	bool compare_eq(InputIter s, InputIter e) const
	{
		const base_type& base = *this;
		return bson_oid_native_ex::s_bson_oid_compare_eq<InputIter>(boost::addressof(base), s, e);
	}

	template<typename BsonValue> inline
	typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
		compare_eq(const BsonValue& bs_val) const
	{
		const base_type& base = *this;
		return bson_value_native_ex::s_value_cmp_eq_oid(boost::addressof(bs_val), boost::addressof(base));
	}

	template<typename BsonOid> inline
	typename boost::enable_if<boost::is_base_of<base_type, BsonOid>, bool>::type
		compare_eq(const BsonOid& right) const
	{
		return bson_oid_native_ex::s_bson_oid_compare_eq(this, boost::addressof(right));
	}

public:
	// compare
	template<typename Char> inline
	typename boost::enable_if<is_native_char<Char>, s32>::type
		compare(const Char* str_oid) const
	{
		const base_type& base = *this;
		return bson_oid_native_ex::s_bson_oid_compare(boost::addressof(base), str_oid);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename boost::enable_if< charset::is_string_t< Basic_String<Char, Traits, Alloc> >, s32>::type
		compare(const Basic_String<Char, Traits, Alloc>& str_oid) const
	{
		const base_type& base = *this;
		return bson_oid_native_ex::s_bson_oid_compare(boost::addressof(base), str_oid);
	}

	template<typename Char, typename Traits,
				template<typename _Char, typename _Traits> class BasicStringView > inline
	typename boost::enable_if< charset::is_string_view_t< BasicStringView<Char, Traits> >, s32>::type
		compare(const BasicStringView<Char, Traits>& str_oid) const
	{
		const base_type& base = *this;
		return bson_oid_native_ex::s_bson_oid_compare(boost::addressof(base), str_oid);
	}

	template<typename T> inline
	typename boost::disable_if<is_native_char<T>, s32>::type
		compare(const T* data) const
	{
		const base_type& base = *this;
		return bson_oid_native_ex::s_bson_oid_compare(boost::addressof(base), data);
	}

	template<typename Cont> inline
	typename
		boost::enable_if
		<
			container_ex::is_container<Cont>,
			s32
		>::type
		compare(const Cont& data) const
	{
		const base_type& base = *this;
		return bson_oid_native_ex::s_bson_oid_compare(boost::addressof(base), data);
	}

	template<typename InputIter> inline
	s32 compare(InputIter s, InputIter e) const
	{
		const base_type& base = *this;
		return bson_oid_native_ex::s_bson_oid_compare<InputIter>(boost::addressof(base), s, e);
	}

	template<typename BsonValue> inline
	typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, s32>::type
		compare(const BsonValue& bs_val) const
	{
		const base_type& base = *this;
		return -bson_value_native_ex::s_value_cmp_oid(boost::addressof(bs_val), boost::addressof(base));
	}

	template<typename BsonOid> inline
	typename boost::enable_if<boost::is_base_of<base_type, BsonOid>, s32>::type
		compare(const BsonOid& right) const
	{
		return bson_oid_native_ex::s_bson_oid_compare(this, boost::addressof(right));
	}

public:
	inline static const this_type& s_gen_empty_oid(void)
	{
		static u8 arr[12] = {0};
		static const this_type tmp(arr);
		return tmp;
	}

	inline static this_type s_gen_oid(void)
	{
		this_type tmp;
		return tmp;
	}

private:
	template<typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
	Basic_String<char, Traits, Alloc>&
		prv_get_string(Basic_String<char, Traits, Alloc>& str) const
	{
		typedef Basic_String<char, Traits, Alloc> now_string_type;

		const base_type& base = *this;
		now_string_type tmp_str(this_type::E_bson_oid_string_size, 0);
		bson_oid_to_string(boost::addressof(base), const_cast<char*>(tmp_str.data()));

		assert((charset::string_byte_size(tmp_str)
					== this_type::E_bson_oid_string_size));

		str.swap(tmp_str);
		return str;
	}

private:
	YGGR_PP_FRIEND_CLASS( bson_inner_data_accesser<true> );

	//save and load def
	template<typename Bson, typename String > inline
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
		save(Bson& bs, const String& name) const
	{
		typedef String now_string_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, now_inner_string_type);

		if(bson_native_ex::s_bson_is_nil(boost::addressof(bs)))
		{
			bson_init(boost::addressof(bs));
		}

		now_inner_string_type str_name((charset::string_charset_helper_data(name)));
		const base_type& base = *this;
		return
			bson_append_oid(
				boost::addressof(bs),
				str_name.data(),
				static_cast<int>(str_name.size()),
				boost::addressof(base));
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, bool>::type
		save_value(Bson& bs) const
	{
		const base_type& base = *this;
		return
			bson_native_ex::s_bson_save_value_of_oid(
				boost::addressof(bs), boost::addressof(base));
	}

	template<typename BsonIter, typename String > inline
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
		load(const BsonIter& iter, const String& name)
	{
		assert(iter.load_check(bson_typeid< boost::remove_const<this_type>::type >::value, name));
		if(!iter.load_check(bson_typeid< boost::remove_const<this_type>::type >::value, name))
		{
			return;
		}
		const base_type *pid = bson_iter_oid(boost::addressof(iter));

		if(!pid)
		{
			return;
		}

		base_type& base = *this;
		memcpy(boost::addressof(base), pid, sizeof(id_type));
	}

	template<typename BsonIter> inline
	typename boost::enable_if<boost::is_base_of<bson_iter_t, BsonIter>, void>::type
		load_value(const BsonIter& iter)
	{
		const id_type *pid = bson_iter_oid(boost::addressof(iter));

		if(!pid)
		{
			return;
		}

		base_type& base = *this;
		memcpy(boost::addressof(base), pid, sizeof(id_type));
	}
};

// non member operator foo

// c_bson_oid

// operator ==

template<typename Char> inline
typename boost::enable_if<is_native_char<Char>, bool>::type
	operator==(const c_bson_oid& l, const Char* r)
{
	return l.compare_eq(r);
}

template<typename Char> inline
typename boost::enable_if<is_native_char<Char>, bool>::type
	operator==(const Char* l, const c_bson_oid& r)
{
	return r.compare_eq(l);
}

template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
typename boost::enable_if< charset::is_string_t< Basic_String<Char, Traits, Alloc> >, bool>::type
	operator==(const c_bson_oid& l, const Basic_String<Char, Traits, Alloc>& r)
{
	return l.compare_eq(r);
}

template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
typename boost::enable_if< charset::is_string_t< Basic_String<Char, Traits, Alloc> >, bool>::type
	operator==(const Basic_String<Char, Traits, Alloc>& l,  const c_bson_oid& r)
{
	return r.compare_eq(l);
}

template<typename T> inline
typename boost::disable_if<is_native_char<T>, bool>::type
	operator==(const c_bson_oid& l, const T* r)
{
	return l.compare_eq(r);
}

template<typename T> inline
typename boost::disable_if<is_native_char<T>, bool>::type
	operator==(const T* l, const c_bson_oid& r)
{
	return r.compare_eq(l);
}

template<typename Cont> inline
typename boost::enable_if<container_ex::is_container<Cont>, bool>::type
	operator==(const c_bson_oid& l, const Cont& r)
{
	return l.compare_eq(r);
}

template<typename Cont> inline
typename boost::enable_if<container_ex::is_container<Cont>, bool>::type
	operator==(const Cont& l, const c_bson_oid& r)
{
	return r.compare_eq(l);
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator==(const BsonValue& l, const c_bson_oid& r)
{
	return r.compare_eq(l);
}

template<typename BsonOid> inline
typename boost::enable_if<boost::is_base_of<bson_oid_t, BsonOid>, bool>::type
	operator==(const c_bson_oid& l, const BsonOid& r)
{
	return l.compare_eq(r);
}

// operator !=

template<typename Char> inline
typename boost::enable_if<is_native_char<Char>, bool>::type
	operator!=(const c_bson_oid& l, const Char* r)
{
	return !l.compare_eq(r);
}

template<typename Char> inline
typename boost::enable_if<is_native_char<Char>, bool>::type
	operator!=(const Char* l, const c_bson_oid& r)
{
	return !r.compare_eq(l);
}

template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
typename boost::enable_if< charset::is_string_t< Basic_String<Char, Traits, Alloc> >, bool>::type
	operator!=(const c_bson_oid& l, const Basic_String<Char, Traits, Alloc>& r)
{
	return !l.compare_eq(r);
}

template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
typename boost::enable_if< charset::is_string_t< Basic_String<Char, Traits, Alloc> >, bool>::type
	operator!=(const Basic_String<Char, Traits, Alloc>& l,  const c_bson_oid& r)
{
	return !r.compare_eq(l);
}

template<typename T> inline
typename boost::disable_if<is_native_char<T>, bool>::type
	operator!=(const c_bson_oid& l, const T* r)
{
	return !l.compare_eq(r);
}

template<typename T> inline
typename boost::disable_if<is_native_char<T>, bool>::type
	operator!=(const T* l, const c_bson_oid& r)
{
	return !r.compare_eq(l);
}

template<typename Cont> inline
typename boost::enable_if<container_ex::is_container<Cont>, bool>::type
	operator!=(const c_bson_oid& l, const Cont& r)
{
	return !l.compare_eq(r);
}

template<typename Cont> inline
typename boost::enable_if<container_ex::is_container<Cont>, bool>::type
	operator!=(const Cont& l, const c_bson_oid& r)
{
	return !r.compare_eq(l);
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator!=(const BsonValue& l, const c_bson_oid& r)
{
	return !r.compare_eq(l);
}


template<typename BsonOid> inline
typename boost::enable_if<boost::is_base_of<bson_oid_t, BsonOid>, bool>::type
	operator!=(const c_bson_oid& l, const BsonOid& r)
{
	return !l.compare_eq(r);
}

// operator <
template<typename Char> inline
typename boost::enable_if<is_native_char<Char>, bool>::type
	operator<(const c_bson_oid& l, const Char* r)
{
	return l.compare(r) < 0;
}

template<typename Char> inline
typename boost::enable_if<is_native_char<Char>, bool>::type
	operator<(const Char* l, const c_bson_oid& r)
{
	return 0 < r.compare(l);
}

template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
typename boost::enable_if< charset::is_string_t< Basic_String<Char, Traits, Alloc> >, bool>::type
	operator<(const c_bson_oid& l, const Basic_String<Char, Traits, Alloc>& r)
{
	return l.compare(r) < 0;
}

template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
typename boost::enable_if< charset::is_string_t< Basic_String<Char, Traits, Alloc> >, bool>::type
	operator<(const Basic_String<Char, Traits, Alloc>& l,  const c_bson_oid& r)
{
	return 0 < r.compare(l);
}

template<typename T> inline
typename boost::disable_if<is_native_char<T>, bool>::type
	operator<(const c_bson_oid& l, const T* r)
{
	return l.compare(r) < 0;
}

template<typename T> inline
typename boost::disable_if<is_native_char<T>, bool>::type
	operator<(const T* l, const c_bson_oid& r)
{
	return 0 < r.compare(l);
}

template<typename Cont> inline
typename boost::enable_if<container_ex::is_container<Cont>, bool>::type
	operator<(const c_bson_oid& l, const Cont& r)
{
	return l.compare(r) < 0;
}

template<typename Cont> inline
typename boost::enable_if<container_ex::is_container<Cont>, bool>::type
	operator<(const Cont& l, const c_bson_oid& r)
{
	return 0 < r.compare(l);
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator<(const BsonValue& l, const c_bson_oid& r)
{
	return 0 < r.compare(l);
}

template<typename BsonOid> inline
typename boost::enable_if<boost::is_base_of<bson_oid_t, BsonOid>, bool>::type
	operator<(const c_bson_oid& l, const BsonOid& r)
{
	return l.compare(r) < 0;
}

// operator <=
template<typename Char> inline
typename boost::enable_if<is_native_char<Char>, bool>::type
	operator<=(const c_bson_oid& l, const Char* r)
{
	return l.compare(r) <= 0;
}

template<typename Char> inline
typename boost::enable_if<is_native_char<Char>, bool>::type
	operator<=(const Char* l, const c_bson_oid& r)
{
	return 0 <= r.compare(l);
}

template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
typename boost::enable_if< charset::is_string_t< Basic_String<Char, Traits, Alloc> >, bool>::type
	operator<=(const c_bson_oid& l, const Basic_String<Char, Traits, Alloc>& r)
{
	return l.compare(r) <= 0;
}

template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
typename boost::enable_if< charset::is_string_t< Basic_String<Char, Traits, Alloc> >, bool>::type
	operator<=(const Basic_String<Char, Traits, Alloc>& l,  const c_bson_oid& r)
{
	return 0 <= r.compare(l);
}

template<typename T> inline
typename boost::disable_if<is_native_char<T>, bool>::type
	operator<=(const c_bson_oid& l, const T* r)
{
	return l.compare(r) <= 0;
}

template<typename T> inline
typename boost::disable_if<is_native_char<T>, bool>::type
	operator<=(const T* l, const c_bson_oid& r)
{
	return 0 <= r.compare(l);
}

template<typename Cont> inline
typename boost::enable_if<container_ex::is_container<Cont>, bool>::type
	operator<=(const c_bson_oid& l, const Cont& r)
{
	return l.compare(r) <= 0;
}

template<typename Cont> inline
typename boost::enable_if<container_ex::is_container<Cont>, bool>::type
	operator<=(const Cont& l, const c_bson_oid& r)
{
	return 0 <= r.compare(l);
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator<=(const BsonValue& l, const c_bson_oid& r)
{
	return 0 <= r.compare(l);
}

template<typename BsonOid> inline
typename boost::enable_if<boost::is_base_of<bson_oid_t, BsonOid>, bool>::type
	operator<=(const c_bson_oid& l, const BsonOid& r)
{
	return l.compare(r) <= 0;
}

// operator >
template<typename Char> inline
typename boost::enable_if<is_native_char<Char>, bool>::type
	operator>(const c_bson_oid& l, const Char* r)
{
	return l.compare(r) > 0;
}

template<typename Char> inline
typename boost::enable_if<is_native_char<Char>, bool>::type
	operator>(const Char* l, const c_bson_oid& r)
{
	return 0 > r.compare(l);
}

template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
typename boost::enable_if< charset::is_string_t< Basic_String<Char, Traits, Alloc> >, bool>::type
	operator>(const c_bson_oid& l, const Basic_String<Char, Traits, Alloc>& r)
{
	return l.compare(r) > 0;
}

template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
typename boost::enable_if< charset::is_string_t< Basic_String<Char, Traits, Alloc> >, bool>::type
	operator>(const Basic_String<Char, Traits, Alloc>& l,  const c_bson_oid& r)
{
	return 0 > r.compare(l);
}

template<typename T> inline
typename boost::disable_if<is_native_char<T>, bool>::type
	operator>(const c_bson_oid& l, const T* r)
{
	return l.compare(r) > 0;
}

template<typename T> inline
typename boost::disable_if<is_native_char<T>, bool>::type
	operator>(const T* l, const c_bson_oid& r)
{
	return 0 > r.compare(l);
}

template<typename Cont> inline
typename boost::enable_if<container_ex::is_container<Cont>, bool>::type
	operator>(const c_bson_oid& l, const Cont& r)
{
	return l.compare(r) > 0;
}

template<typename Cont> inline
typename boost::enable_if<container_ex::is_container<Cont>, bool>::type
	operator>(const Cont& l, const c_bson_oid& r)
{
	return 0 > r.compare(l);
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator>(const BsonValue& l, const c_bson_oid& r)
{
	return 0 > r.compare(l);
}

template<typename BsonOid> inline
typename boost::enable_if<boost::is_base_of<bson_oid_t, BsonOid>, bool>::type
	operator>(const c_bson_oid& l, const BsonOid& r)
{
	return l.compare(r) > 0;
}


// operator >=
template<typename Char> inline
typename boost::enable_if<is_native_char<Char>, bool>::type
	operator>=(const c_bson_oid& l, const Char* r)
{
	return l.compare(r) >= 0;
}

template<typename Char> inline
typename boost::enable_if<is_native_char<Char>, bool>::type
	operator>=(const Char* l, const c_bson_oid& r)
{
	return 0 >= r.compare(l);
}

template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
typename boost::enable_if< charset::is_string_t< Basic_String<Char, Traits, Alloc> >, bool>::type
	operator>=(const c_bson_oid& l, const Basic_String<Char, Traits, Alloc>& r)
{
	return l.compare(r) >= 0;
}

template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
typename boost::enable_if< charset::is_string_t< Basic_String<Char, Traits, Alloc> >, bool>::type
	operator>=(const Basic_String<Char, Traits, Alloc>& l,  const c_bson_oid& r)
{
	return 0 >= r.compare(l);
}

template<typename T> inline
typename boost::disable_if<is_native_char<T>, bool>::type
	operator>=(const c_bson_oid& l, const T* r)
{
	return l.compare(r) >= 0;
}

template<typename T> inline
typename boost::disable_if<is_native_char<T>, bool>::type
	operator>=(const T* l, const c_bson_oid& r)
{
	return 0 >= r.compare(l);
}

template<typename Cont> inline
typename boost::enable_if<container_ex::is_container<Cont>, bool>::type
	operator>=(const c_bson_oid& l, const Cont& r)
{
	return l.compare(r) >= 0;
}

template<typename Cont> inline
typename boost::enable_if<container_ex::is_container<Cont>, bool>::type
	operator>=(const Cont& l, const c_bson_oid& r)
{
	return 0 >= r.compare(l);
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator>=(const BsonValue& l, const c_bson_oid& r)
{
	return 0 >= r.compare(l);
}

template<typename BsonOid> inline
typename boost::enable_if<boost::is_base_of<bson_oid_t, BsonOid>, bool>::type
	operator>=(const c_bson_oid& l, const BsonOid& r)
{
	return l.compare(r) >= 0;
}

namespace detail
{

template<>
struct bson_operator_outter<c_bson_oid>
{
	template<typename Char, typename Traits> inline
	std::basic_ostream<Char, Traits>&
		operator()(std::basic_ostream<Char, Traits>& os,
					const c_bson_oid& val,
					bson_json_mode_def::mode_type mode) const
	{
		typedef c_bson_oid::org_type now_org_type;
		typedef bson_operator_outter<now_org_type> outter_type;

		outter_type outter;
		return outter(os, static_cast<const now_org_type&>(val), mode);
	}
};

} // namespace detail

// os << val
template<typename Char, typename Traits> inline
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os, const c_bson_oid& val)
{
	typedef detail::bson_operator_outter<c_bson_oid> outter_type;

	outter_type outter;
	return outter(os, val, 0);
}

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{

#if defined(__GNUC__) && (YGGR_CPP_VERSION < YGGR_CPP_VER_11)

inline void swap(c_bson_oid& l, bson_oid_t& r)
{
	l.swap(r);
}

#else

template<typename BsonOid> inline
typename boost::enable_if<boost::is_base_of<bson_oid_t, BsonOid>, void>::type
	swap(c_bson_oid& l, BsonOid& r)
{
	l.swap(r);
}

#endif // #if defined(__GNUC__) && (YGGR_CPP_VERSION < YGGR_CPP_VER_11)

inline void swap(c_bson_oid& l, c_bson_oid& r)
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

#endif // __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_OID_HPP__
