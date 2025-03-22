// c_bson_value.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_VALUE_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_VALUE_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>

#include <yggr/ppex/friend.hpp>
#include <yggr/ppex/foo_params.hpp>

#include <yggr/mplex/static_assert.hpp>
#include <yggr/mplex/tag_sfinae.hpp>

#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/utf8_string_view.hpp>

#include <yggr/nsql_database_system/bson_typeid.hpp>
#include <yggr/nsql_database_system/bson_inner_data_accesser_decl.hpp>
#include <yggr/nsql_database_system/bson_native_ex.hpp>
#include <yggr/nsql_database_system/bson_value_native_ex.hpp>

#include <yggr/nsql_database_system/c_bson_null.hpp>
#include <yggr/nsql_database_system/c_bson_undefined.hpp>

#include <yggr/nsql_database_system/c_bson_minkey.hpp>
#include <yggr/nsql_database_system/c_bson_maxkey.hpp>

#include <yggr/nsql_database_system/c_bson_date.hpp>
#include <yggr/nsql_database_system/c_bson_time.hpp>
#include <yggr/nsql_database_system/c_bson_timeval.hpp>

#include <yggr/nsql_database_system/c_bson_oid.hpp>

#include <yggr/nsql_database_system/c_bson_binary_buffer.hpp>
#include <yggr/nsql_database_system/c_bson_binary_buffer_ref.hpp>

#include <yggr/nsql_database_system/c_bson_regex.hpp>
#include <yggr/nsql_database_system/c_bson_code.hpp>
#include <yggr/nsql_database_system/c_bson_symbol.hpp>
#include <yggr/nsql_database_system/c_bson_code_w_scope.hpp>

#include <yggr/nsql_database_system/c_bson_timestamp.hpp>

#include <yggr/nsql_database_system/c_bson_dbpointer.hpp>
#include <yggr/nsql_database_system/c_bson_decimal128.hpp>

#include <yggr/nsql_database_system/bson_visit_t.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/type_traits/is_base_of.hpp>

#include <cassert>
#include <utility>

namespace yggr
{
namespace nsql_database_system
{

class c_bson_value;

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{

template<>
struct bson_typeid< bson_value_t >
{
	typedef bson_value_t type;
	typedef c_bson_value wrap_type;

	YGGR_STATIC_CONSTANT(
		bson_typeid_def::bson_typeid_type,
		value = bson_typeid_def::E_BSON_TYPE_ANY_VALUE);

	YGGR_CONSTEXPR_OR_INLINE u8 operator()(void) const
	{
		return static_cast<u8>(value);
	}

	inline u8 operator()(const type& obj) const
	{
		return obj.value_type;
	}
};

// if write the code to file end, error C2908 occurs with cpp20
template<>
struct bson_typeid< c_bson_value >
{
	typedef c_bson_value type;
	typedef c_bson_value wrap_type;

	YGGR_STATIC_CONSTANT(
		bson_typeid_def::bson_typeid_type,
		value = bson_typeid_def::E_BSON_TYPE_ANY_VALUE);

	YGGR_CONSTEXPR_OR_INLINE u8 operator()(void) const
	{
		return static_cast<u8>(value);
	}

	inline u8 operator()(const type& obj) const
	{
		return static_cast<u8>(reinterpret_cast<const bson_value_t&>(obj).value_type);
	}
};

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{

//BSON_ALIGNED_BEGIN (8)
//typedef struct _bson_value_t {
//   bson_type_t value_type;
//   int32_t padding;
//   union {
//      bson_oid_t v_oid;
//      int64_t v_int64;
//      int32_t v_int32;
//      int8_t v_int8;
//      double v_double;
//      bool v_bool;
//      int64_t v_datetime;
//      struct {
//         uint32_t timestamp;
//         uint32_t increment;
//      } v_timestamp;
//      struct {
//         char *str;
//         uint32_t len;
//      } v_utf8;
//      struct {
//         uint8_t *data;
//         uint32_t data_len;
//      } v_doc;
//      struct {
//         uint8_t *data;
//         uint32_t data_len;
//         bson_subtype_t subtype;
//      } v_binary;
//      struct {
//         char *regex;
//         char *options;
//      } v_regex;
//      struct {
//         char *collection;
//         uint32_t collection_len;
//         bson_oid_t oid;
//      } v_dbpointer;
//      struct {
//         char *code;
//         uint32_t code_len;
//      } v_code;
//      struct {
//         char *code;
//         uint8_t *scope_data;
//         uint32_t code_len;
//         uint32_t scope_len;
//      } v_codewscope;
//      struct {
//         char *symbol;
//         uint32_t len;
//      } v_symbol;
//      bson_decimal128_t v_decimal128;
//   } value;
//} bson_value_t BSON_ALIGNED_END (8);

class c_bson_value
	: public bson_value_t
{
public:
	typedef bson_value_t base_type;
	typedef base_type org_type;

private:
	typedef c_bson_value this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

private:
	typedef mplex::sfinae_type sfinae_type;

public:
	c_bson_value(void);

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename Tag, typename ...Args>
	explicit c_bson_value(const Tag& tag, BOOST_FWD_REF(Args)... args)
	{
		this_type::pro_init();
		this_type::assign(tag, boost::forward<Args>(args)...);
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template< typename Tag \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
	explicit c_bson_value(const Tag& tag YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
		this_type::pro_init(); \
		this_type::assign(tag YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	c_bson_value(s32 val);
	c_bson_value(s64 val);
	c_bson_value(bool val);
	c_bson_value(f32 val);
	c_bson_value(f64 val);

	c_bson_value(const char* val);

#ifndef YGGR_NO_CWCHAR
	c_bson_value(const wchar_t* val);
#endif // YGGR_NO_CWCHAR

#ifndef YGGR_NO_CHAR8_T
	c_bson_value(const c8* val);
#endif // YGGR_NO_CHAR8_T

#ifndef YGGR_NO_CHAR16_T
	c_bson_value(const c16* val);
#endif // YGGR_NO_CHAR16_T

#ifndef YGGR_NO_CHAR32_T
	c_bson_value(const c32* val);
#endif // YGGR_NO_CHAR32_T

	template<typename Char>
	c_bson_value(const Char* val,
					typename
						boost::enable_if
						<
							is_native_char< Char >,
							sfinae_type
						>::type sfinae = 0)
	{
		this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(utf8), val);
	}

	template<typename Char, typename Traits,
			template<typename _Char, typename _Traits> class Basic_StringView>
	c_bson_value(const Basic_StringView<Char, Traits>& val)
	{
		this_type::pro_init();
		this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(utf8), val);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String>
	c_bson_value(const Basic_String<Char, Traits, Alloc>& val)
	{
		this_type::pro_init();
		this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(utf8), val);
	}

	template<typename T1, typename T2>
	c_bson_value(const charset::string_charset_helper<T1, T2>& val)
	{
		this_type::pro_init();
		this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(utf8), val);
	}

	c_bson_value(const c_bson_null&);
	c_bson_value(const c_bson_undefined&);
	c_bson_value(const c_bson_maxkey&);
	c_bson_value(const c_bson_minkey&);

	c_bson_value(const c_bson_date& val);
	c_bson_value(const c_bson_time& val);
	c_bson_value(const timeval& val);
	c_bson_value(const c_bson_timeval& val);

	c_bson_value(const bson_oid_t& val);
	c_bson_value(const c_bson_oid& val);

	c_bson_value(const bson_decimal128_t& val);
	c_bson_value(const c_bson_decimal128& val);

	template<u32 Binary_ID>
	c_bson_value(const c_bson_binary_buffer<Binary_ID>& val)
	{
		this_type::pro_init();
		this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(binary), val);
	}

	template<u32 Binary_ID, typename Buffer>
	c_bson_value(const c_bson_binary_buffer_ref<Binary_ID, Buffer>& val)
	{
		this_type::pro_init();
		this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(binary), val);
	}

	c_bson_value(const c_bson_regex& val);
	c_bson_value(const c_bson_code& val);
	c_bson_value(const c_bson_symbol& val);

	template<typename Scope>
	c_bson_value(const c_bson_code_w_scope<Scope>& val)
	{
		this_type::pro_init();
		this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(codewscope), val);
	}

	c_bson_value(const bson_timestamp_t& val);
	c_bson_value(const c_bson_timestamp& val);

	template<typename Oid>
	c_bson_value(const c_bson_dbpointer<Oid>& val)
	{
		this_type::pro_init();
		this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(dbpointer), val);
	}

	c_bson_value(const bson_t& val);

	c_bson_value(BOOST_RV_REF(base_type) right)
	{
		this_type::pro_init();

		base_type& right_ref = right;
		base_type tmp = {static_cast<bson_type_t>(0)};
		memcpy(&tmp, this, sizeof(base_type));
		memcpy(this, &right_ref, sizeof(base_type));
		memcpy(&right_ref, &tmp, sizeof(base_type));
	}

	c_bson_value(const base_type& right);

	c_bson_value(BOOST_RV_REF(this_type) right)
	{
		this_type::pro_init();

		this_type& right_ref = right;
		base_type tmp = {static_cast<bson_type_t>(0)};
		memcpy(&tmp, this, sizeof(base_type));
		memcpy(this, &right_ref, sizeof(base_type));
		memcpy(&right_ref, &tmp, sizeof(base_type));
	}

	c_bson_value(const this_type& right);
	~c_bson_value(void);

public:
	this_type& operator=(s32 val);
	this_type& operator=(s64 val);
	this_type& operator=(bool val);
	this_type& operator=(f32 val);
	this_type& operator=(f64 val);

	template<typename String> inline
	typename boost::enable_if<charset::utf8_string_constructable<String>, this_type&>::type
		operator=(const String& val)
	{
		this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(utf8), val);
		return *this;
	}

	this_type& operator=(const c_bson_null&);
	this_type& operator=(const c_bson_undefined&);
	this_type& operator=(const c_bson_maxkey&);
	this_type& operator=(const c_bson_minkey&);

	this_type& operator=(const c_bson_date& val);
	this_type& operator=(const c_bson_time& val);

	template<typename BsonTimeval> inline
	typename boost::enable_if<boost::is_base_of<timeval, BsonTimeval>, this_type&>::type
		operator=(const BsonTimeval& val)
	{
		this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(timeval), val);
		return *this;
	}

	template<typename BsonOid> inline
	typename boost::enable_if<boost::is_base_of<bson_oid_t, BsonOid>, this_type&>::type
		operator=(const BsonOid& val)
	{
		this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(oid), val);
		return *this;
	}

	template<u32 Binary_ID> inline
	this_type& operator=(const c_bson_binary_buffer<Binary_ID>& val)
	{
		this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(binary), val);
		return *this;
	}

	template<u32 Binary_ID, typename Buffer> inline
	this_type& operator=(const c_bson_binary_buffer_ref<Binary_ID, Buffer>& val)
	{
		this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(binary), val);
		return *this;
	}

	this_type& operator=(const c_bson_regex& val);
	this_type& operator=(const c_bson_code& val);
	this_type& operator=(const c_bson_symbol& val);

	template<typename Scope> inline
	this_type& operator=(const c_bson_code_w_scope<Scope>& val)
	{
		this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(codewscope), val);
		return *this;
	}

	template<typename BsonTimestamp> inline
	typename boost::enable_if<boost::is_base_of<bson_timestamp_t, BsonTimestamp>, this_type&>::type
		operator=(const BsonTimestamp& val)
	{
		this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(timestamp), val);
		return *this;
	}

	template<typename Oid>
	this_type& operator=(const c_bson_dbpointer<Oid>& val)
	{
		this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(dbpointer), val);
		return *this;
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, this_type&>::type
		operator=(const Bson& val)
	{
		bson_native_ex::s_bson_is_array(&val)?
			this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(array), val)
			: this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(document), val);
		return *this;
	}

protected:
	this_type& pro_operator_set(const base_type& right);

public:
	template<typename BsonValue> inline
	typename boost::enable_if<boost::is_base_of<base_type, BsonValue>, this_type&>::type
		operator=(BOOST_RV_REF(BsonValue) right)
	{
		BsonValue& right_ref = right;
		this_type::pro_swap(static_cast<base_type&>(right_ref));
		return *this;
	}

	template<typename BsonValue> inline
	typename boost::enable_if<boost::is_base_of<base_type, BsonValue>, this_type&>::type
		operator=(const BsonValue& right)
	{
		const base_type& right_cref = right;
		return this_type::pro_operator_set(right_cref);
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::pro_swap(static_cast<base_type&>(right_ref));
		return *this;
	}

	this_type& operator=(const this_type& right);

protected:
	void pro_swap(base_type& right);

public:
	template<typename BsonValue> inline
	typename boost::enable_if<boost::is_base_of<base_type, BsonValue>, void>::type
		swap(BOOST_RV_REF(BsonValue) right)
	{
		BsonValue& right_ref = right;
		this_type::pro_swap(static_cast<base_type&>(right_ref));
	}

	template<typename BsonValue> inline
	typename boost::enable_if<boost::is_base_of<base_type, BsonValue>, void>::type
		swap(BsonValue& right)
	{
		this_type::pro_swap(static_cast<base_type&>(right));
	}

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::pro_swap(static_cast<base_type&>(right_ref));
	}

	void swap(this_type& right);

public:
	inline u32 value_typeid(void) const
	{
		return base_type::value_type;
	}

	YGGR_CONSTEXPR_OR_INLINE static u32 s_value_typeid(void)
	{
		return bson_typeid_def::E_BSON_TYPE_ANY_VALUE;
	}

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
	inline operator std::pair<s32, bool>(void) const
	{
		typedef std::pair<s32, bool> ret_type;

		ret_type ret;
		ret.second = this_type::get_value(YGGR_BSON_VALUE_TAG_OBJECT(int32), ret.first);
		return ret;
	}

	inline operator std::pair<s64, bool>(void) const
	{
		typedef std::pair<s64, bool> ret_type;

		ret_type ret;
		ret.second = this_type::get_value(YGGR_BSON_VALUE_TAG_OBJECT(int64), ret.first);
		return ret;
	}

	inline operator std::pair<bool, bool>(void) const
	{
		typedef std::pair<bool, bool> ret_type;

		ret_type ret;
		ret.second = this_type::get_value(YGGR_BSON_VALUE_TAG_OBJECT(bool), ret.first);
		return ret;
	}

	inline operator std::pair<f32, bool>(void) const
	{
		typedef std::pair<f32, bool> ret_type;

		ret_type ret;
		ret.second = this_type::get_value(YGGR_BSON_VALUE_TAG_OBJECT(double), ret.first);
		return ret;
	}

	inline operator std::pair<f64, bool>(void) const
	{
		typedef std::pair<f64, bool> ret_type;

		ret_type ret;
		ret.second = this_type::get_value(YGGR_BSON_VALUE_TAG_OBJECT(double), ret.first);
		return ret;
	}

	inline operator std::pair<bson_decimal128_t, bool>(void) const
	{
		typedef std::pair<bson_decimal128_t, bool> ret_type;

		ret_type ret;
		ret.second = this_type::get_value(YGGR_BSON_VALUE_TAG_OBJECT(decimal128), ret.first);
		return ret;
	}

	inline operator std::pair<c_bson_decimal128, bool>(void) const
	{
		typedef std::pair<c_bson_decimal128, bool> ret_type;

		ret_type ret;
		ret.second = this_type::get_value(YGGR_BSON_VALUE_TAG_OBJECT(decimal128), ret.first);
		return ret;
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	operator std::pair<Basic_String<Char, Traits, Alloc>, bool>(void) const
	{
		typedef std::pair<Basic_String<Char, Traits, Alloc>, bool> ret_type;

		ret_type ret;
		ret.second = this_type::get_value(YGGR_BSON_VALUE_TAG_OBJECT(utf8), ret.first);
		return ret;
	}

	template<typename Char, typename Traits> inline
	operator std::pair<charset::basic_utf8_string_view<Char, Traits>, bool>(void) const
	{
		typedef std::pair<charset::basic_utf8_string_view<Char, Traits>, bool> ret_type;

		ret_type ret;
		ret.second = this_type::get_value(YGGR_BSON_VALUE_TAG_OBJECT(utf8), ret.first);
		return ret;
	}

	inline operator std::pair<c_bson_null, bool>(void) const
	{
		typedef std::pair<c_bson_null, bool> ret_type;

		ret_type ret;
		ret.second = this_type::get_value(YGGR_BSON_VALUE_TAG_OBJECT(null), ret.first);
		return ret;
	}

	inline operator std::pair<c_bson_undefined, bool>(void) const
	{
		typedef std::pair<c_bson_undefined, bool> ret_type;

		ret_type ret;
		ret.second = this_type::get_value(YGGR_BSON_VALUE_TAG_OBJECT(undefined), ret.first);
		return ret;
	}

	inline operator std::pair<c_bson_maxkey, bool>(void) const
	{
		typedef std::pair<c_bson_maxkey, bool> ret_type;

		ret_type ret;
		ret.second = this_type::get_value(YGGR_BSON_VALUE_TAG_OBJECT(maxkey), ret.first);
		return ret;
	}

	inline operator std::pair<c_bson_minkey, bool>(void) const
	{
		typedef std::pair<c_bson_minkey, bool> ret_type;

		ret_type ret;
		ret.second = this_type::get_value(YGGR_BSON_VALUE_TAG_OBJECT(minkey), ret.first);
		return ret;
	}

	inline operator std::pair<c_bson_date, bool>(void) const
	{
		typedef std::pair<c_bson_date, bool> ret_type;

		ret_type ret;
		ret.second = this_type::get_value(YGGR_BSON_VALUE_TAG_OBJECT(date_time), ret.first);
		return ret;
	}

	inline operator std::pair<c_bson_time, bool>(void) const
	{
		typedef std::pair<c_bson_time, bool> ret_type;

		ret_type ret;
		ret.second = this_type::get_value(YGGR_BSON_VALUE_TAG_OBJECT(time), ret.first);
		return ret;
	}

	inline operator std::pair<timeval, bool>(void) const
	{
		typedef std::pair<timeval, bool> ret_type;

		ret_type ret;
		ret.second = this_type::get_value(YGGR_BSON_VALUE_TAG_OBJECT(timeval), ret.first);
		return ret;
	}

	inline operator std::pair<c_bson_timeval, bool>(void) const
	{
		typedef std::pair<c_bson_timeval, bool> ret_type;

		ret_type ret;
		ret.second = this_type::get_value(YGGR_BSON_VALUE_TAG_OBJECT(timeval), ret.first);
		return ret;
	}

	inline operator std::pair<bson_oid_t, bool>(void) const
	{
		typedef std::pair<bson_oid_t, bool> ret_type;

		ret_type ret;
		ret.second = this_type::get_value(YGGR_BSON_VALUE_TAG_OBJECT(oid), ret.first);
		return ret;
	}

	inline operator std::pair<c_bson_oid, bool>(void) const
	{
		typedef std::pair<c_bson_oid, bool> ret_type;

		ret_type ret;
		ret.second = this_type::get_value(YGGR_BSON_VALUE_TAG_OBJECT(oid), ret.first);
		return ret;
	}

	template<u32 BINARY_ID> inline
	operator std::pair<c_bson_binary_buffer<BINARY_ID>, bool>(void) const
	{
		typedef std::pair<c_bson_binary_buffer<BINARY_ID>, bool> ret_type;

		ret_type ret;
		ret.second = this_type::get_value(YGGR_BSON_VALUE_TAG_OBJECT(binary), ret.first);
		return ret;
	}

	inline operator std::pair<c_bson_regex, bool>(void) const
	{
		typedef std::pair<c_bson_regex, bool> ret_type;

		ret_type ret;
		ret.second = this_type::get_value(YGGR_BSON_VALUE_TAG_OBJECT(regex), ret.first);
		return ret;
	}

	inline operator std::pair<c_bson_code, bool>(void) const
	{
		typedef std::pair<c_bson_code, bool> ret_type;

		ret_type ret;
		ret.second = this_type::get_value(YGGR_BSON_VALUE_TAG_OBJECT(code), ret.first);
		return ret;
	}

	inline operator std::pair<c_bson_symbol, bool>(void) const
	{
		typedef std::pair<c_bson_symbol, bool> ret_type;

		ret_type ret;
		ret.second = this_type::get_value(YGGR_BSON_VALUE_TAG_OBJECT(symbol), ret.first);
		return ret;
	}

	template<typename Scope> inline
	operator std::pair<c_bson_code_w_scope<Scope>, bool>(void) const
	{
		typedef std::pair<c_bson_code_w_scope<Scope>, bool> ret_type;

		ret_type ret;
		ret.second = this_type::get_value(YGGR_BSON_VALUE_TAG_OBJECT(codewscope), ret.first);
		return ret;
	}

	inline operator std::pair<bson_timestamp_t, bool>(void) const
	{
		typedef std::pair<bson_timestamp_t, bool> ret_type;

		ret_type ret;
		ret.second = this_type::get_value(YGGR_BSON_VALUE_TAG_OBJECT(timestamp), ret.first);
		return ret;
	}

	inline operator std::pair<c_bson_timestamp, bool>(void) const
	{
		typedef std::pair<c_bson_timestamp, bool> ret_type;

		ret_type ret;
		ret.second = this_type::get_value(YGGR_BSON_VALUE_TAG_OBJECT(timestamp), ret.first);
		return ret;
	}

	template<typename BsonOid> inline
	operator std::pair<c_bson_dbpointer<BsonOid>, bool>(void) const
	{
		typedef std::pair<c_bson_dbpointer<BsonOid>, bool> ret_type;

		ret_type ret;
		ret.second = this_type::get_value(YGGR_BSON_VALUE_TAG_OBJECT(dbpointer), ret.first);
		return ret;
	}

	template<typename Bson> inline
	operator std::pair<Bson, bool>(void) const
	{
		YGGR_ASSERT_BSON_TYPE(Bson);
		typedef std::pair<Bson, bool> ret_type;

		ret_type ret;
		memset(&(ret.first), 0, sizeof(bson_t));
		ret.second = bson_value_native_ex::s_value_to_bson_or_array(&ret.first, this);
		return ret;
	}

public:
	// assign
#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template< typename Tag, typename ...Args> inline
	void assign( BOOST_FWD_REF(Args)... args)
	{
		this_type::assign(Tag(), boost::forward<Args>(args)...);
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template< typename Tag \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
	void assign( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
		this_type::assign(Tag() YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }

#	define YGGR_PP_FOO_ARG_NAME() arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	inline void assign(const YGGR_BSON_VALUE_TAG_TYPE(int32)&, s32 val)
	{
		bson_value_native_ex::s_int32_to_value(this, val);
	}

	inline void assign(const YGGR_BSON_VALUE_TAG_TYPE(int64)&, s64 val)
	{
		bson_value_native_ex::s_int64_to_value(this, val);
	}

	inline void assign(const YGGR_BSON_VALUE_TAG_TYPE(bool)&, bool val)
	{
		bson_value_native_ex::s_bool_to_value(this, val);
	}

	inline void assign(const YGGR_BSON_VALUE_TAG_TYPE(double)&, f64 val)
	{
		bson_value_native_ex::s_double_to_value(this, val);
	}

	template<typename String> inline
	typename boost::enable_if< charset::utf8_string_constructable<String>, void>::type
		assign(const YGGR_BSON_VALUE_TAG_TYPE(utf8)&, const String& val)
	{
		bson_value_native_ex::s_string_to_value(this, val);
	}

	inline void assign(const YGGR_BSON_VALUE_TAG_TYPE(null)&)
	{
		bson_value_native_ex::s_null_to_value(this);
	}

	inline void assign(const YGGR_BSON_VALUE_TAG_TYPE(null)&, const c_bson_null&)
	{
		bson_value_native_ex::s_null_to_value(this);
	}

	inline void assign(const YGGR_BSON_VALUE_TAG_TYPE(undefined)&)
	{
		bson_value_native_ex::s_undefined_to_value(this);
	}

	inline void assign(const YGGR_BSON_VALUE_TAG_TYPE(undefined)&, const c_bson_undefined&)
	{
		bson_value_native_ex::s_undefined_to_value(this);
	}

	inline void assign(const YGGR_BSON_VALUE_TAG_TYPE(maxkey)&)
	{
		bson_value_native_ex::s_maxkey_to_value(this);
	}

	inline void assign(const YGGR_BSON_VALUE_TAG_TYPE(maxkey)&, const c_bson_maxkey&)
	{
		bson_value_native_ex::s_maxkey_to_value(this);
	}

	inline void assign(const YGGR_BSON_VALUE_TAG_TYPE(minkey)&)
	{
		bson_value_native_ex::s_minkey_to_value(this);
	}

	inline void assign(const YGGR_BSON_VALUE_TAG_TYPE(minkey)&, const c_bson_minkey&)
	{
		bson_value_native_ex::s_minkey_to_value(this);
	}

	inline void assign(const YGGR_BSON_VALUE_TAG_TYPE(date_time)&, s64 val)
	{
		bson_value_native_ex::s_date_to_value(this, val);
	}

	inline void assign(const YGGR_BSON_VALUE_TAG_TYPE(date_time)&, const c_bson_date& val)
	{
		bson_value_native_ex::s_date_to_value(this, val.date());
	}

	inline void assign(const YGGR_BSON_VALUE_TAG_TYPE(time)&, s64 val)
	{
		bson_value_native_ex::s_time_to_value(this, val);
	}

	inline void assign(const YGGR_BSON_VALUE_TAG_TYPE(time)&, const c_bson_time& val)
	{
		bson_value_native_ex::s_time_to_value(this, val.time());
	}

	template<typename BsonTimeval> inline
	typename boost::enable_if<boost::is_base_of<timeval, BsonTimeval>, void>::type
		assign(const YGGR_BSON_VALUE_TAG_TYPE(timeval)&, const BsonTimeval& val)
	{
		bson_value_native_ex::s_timeval_to_value(this, boost::addressof(val));
	}

	template<typename BsonOid> inline
	typename boost::enable_if<boost::is_base_of<bson_oid_t, BsonOid>, void>::type
		assign(const YGGR_BSON_VALUE_TAG_TYPE(oid)&, const BsonOid& val)
	{
		bson_value_native_ex::s_oid_to_value(this, boost::addressof(val));
	}

	inline void assign(const YGGR_BSON_VALUE_TAG_TYPE(binary)&, u32 sub_t, const u8* val, u64 size)
	{
		bson_value_native_ex::s_binary_buffer_to_value(this, sub_t, val, size);
	}

	template<u32 BINARY_ID> inline
	void assign(const YGGR_BSON_VALUE_TAG_TYPE(binary)&, const c_bson_binary_buffer<BINARY_ID>& val)
	{
		bson_value_native_ex::s_binary_buffer_to_value(this, val.buffer_typeid(), val.data(), val.size());
	}

	template<u32 BINARY_ID, typename Buffer> inline
	void assign(const YGGR_BSON_VALUE_TAG_TYPE(binary)&, const c_bson_binary_buffer_ref<BINARY_ID, Buffer>& val)
	{
		c_bson_binary_buffer<BINARY_ID> buf(val.conv_to_binary_buffer());
		bson_value_native_ex::s_binary_buffer_to_value(this, buf.buffer_typeid(), buf.data(), buf.size());
	}

	template<typename String1, typename String2> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			void
		>::type
		assign(const YGGR_BSON_VALUE_TAG_TYPE(regex)&, const String1& val_regex, const String2& val_opts)
	{
		bson_value_native_ex::s_regex_to_value(this, val_regex, val_opts);
	}

	inline void assign(const YGGR_BSON_VALUE_TAG_TYPE(regex)&, const c_bson_regex& val)
	{
		bson_value_native_ex::s_regex_to_value(
			this,
			val.regex<c_bson_regex::string_type>(),
			val.options<c_bson_regex::string_type>());
	}

	template<typename String> inline
	typename boost::enable_if< charset::utf8_string_constructable<String>, void>::type
		assign(const YGGR_BSON_VALUE_TAG_TYPE(code)&, const String& val)
	{
		bson_value_native_ex::s_code_to_value(this, val);
	}

	inline void assign(const YGGR_BSON_VALUE_TAG_TYPE(code)&, const c_bson_code& val)
	{
		bson_value_native_ex::s_code_to_value(this, val.code<c_bson_code::string_type>());
	}

	template<typename String> inline
	typename boost::enable_if< charset::utf8_string_constructable<String>, void>::type
		assign(const YGGR_BSON_VALUE_TAG_TYPE(symbol)&, const String& val)
	{
		bson_value_native_ex::s_symbol_to_value(this, val);
	}

	inline void assign(const YGGR_BSON_VALUE_TAG_TYPE(symbol)&, const c_bson_symbol& val)
	{
		bson_value_native_ex::s_symbol_to_value(this, val.symbol<c_bson_symbol::string_type>());
	}

	template<typename String, typename Bson> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String>,
				boost::is_base_of<bson_t, Bson>
			>,
			void
		>::type
		assign(const YGGR_BSON_VALUE_TAG_TYPE(codewscope)&, const String& val, const Bson& scope)
	{
		bson_value_native_ex::s_code_w_scope_to_value(this, val, boost::addressof(scope));
	}

	template<typename Scope> inline
	void assign(const YGGR_BSON_VALUE_TAG_TYPE(codewscope)&, const c_bson_code_w_scope<Scope>& val)
	{
		typedef c_bson_code_w_scope<Scope> code_w_scope_type;

		bson_value_native_ex::s_code_w_scope_to_value(
            this, val.template code<typename code_w_scope_type::string_type>(), &(val.scope()));
	}

	inline void assign(const YGGR_BSON_VALUE_TAG_TYPE(timestamp)&, u32 timestamp, u32 increment)
	{
		bson_value_native_ex::s_timestamp_to_value(this, timestamp, increment);
	}

	template<typename TimeStamp> inline
	typename boost::enable_if<boost::is_base_of<bson_timestamp_t, TimeStamp>, void>::type
		assign(const YGGR_BSON_VALUE_TAG_TYPE(timestamp)&, const TimeStamp& val)
	{
		bson_value_native_ex::s_timestamp_to_value(this, val.t, val.i);
	}

	template<typename BsonDecimal128> inline
	typename boost::enable_if<boost::is_base_of<bson_decimal128_t, BsonDecimal128>, void>::type
		assign(const YGGR_BSON_VALUE_TAG_TYPE(decimal128)&, const BsonDecimal128& val)
	{
		bson_value_native_ex::s_decimal128_to_value(this, boost::addressof(val));
	}

	template<typename String> inline
	typename boost::enable_if< charset::utf8_string_constructable<String>, void>::type
		assign(const YGGR_BSON_VALUE_TAG_TYPE(dbpointer)&, const String& val, const bson_oid_t& oid)
	{
		bson_value_native_ex::s_dbpointer_to_value(this, val, &oid);
	}

	template<typename BsonOid> inline
	void assign(const YGGR_BSON_VALUE_TAG_TYPE(dbpointer)&, const c_bson_dbpointer<BsonOid>& val)
	{
		typedef c_bson_dbpointer<BsonOid> dbpointer_type;
		bson_value_native_ex::s_dbpointer_to_value(
            this, val.template collection<typename dbpointer_type::string_type>(), &(val.oid()));
	}

	template<typename Bson> inline
	typename boost::enable_if< boost::is_base_of<bson_t, Bson>, void>::type
		assign(const YGGR_BSON_VALUE_TAG_TYPE(document)&, const Bson& val)
	{
		bson_value_native_ex::s_bson_to_value(this, boost::addressof(val));
	}

	template<typename Bson> inline
	typename boost::enable_if< boost::is_base_of< bson_t, Bson>, void>::type
		assign(const YGGR_BSON_VALUE_TAG_TYPE(array)&, const Bson& val)
	{
		bson_value_native_ex::s_array_to_value(this, boost::addressof(val));
	}

	// used it can't delete
	template<typename Bson> inline
	typename boost::enable_if< boost::is_base_of< bson_t, Bson>, void>::type
		assign(const Bson& val)
	{
		bson_native_ex::s_bson_is_array(boost::addressof(val))?
			this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(array), val)
			: this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(document), val);
	}

	// get_value

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template< typename Tag, typename ...Args> inline
	bool get_value(Args&... args)
	{
		return this_type::get_value(Tag(), args...);
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template< typename Tag \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
	bool get_value( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_REF_PARAMS ) ) const { \
		return this_type::get_value(Tag() YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
										YGGR_PP_FOO_PARAMS_OP( __n__, YGGR_PP_SYMBOL_COMMA )); }

#	define YGGR_PP_FOO_ARG_NAME() arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	inline bool get_value(const YGGR_BSON_VALUE_TAG_TYPE(int32)&, s32& val) const
	{
		return bson_value_native_ex::s_value_to_int32(val, this);
	}

	inline bool get_value(const YGGR_BSON_VALUE_TAG_TYPE(int64)&, s64& val) const
	{
		return bson_value_native_ex::s_value_to_int64(val, this);
	}

	inline bool get_value(const YGGR_BSON_VALUE_TAG_TYPE(bool)&, bool& val) const
	{
		return bson_value_native_ex::s_value_to_bool(val, this);
	}

	inline bool get_value(const YGGR_BSON_VALUE_TAG_TYPE(double)&, f32& val) const
	{
		f64 tmp_val = 0.0;
		return
			bson_value_native_ex::s_value_to_double(tmp_val, this)?
			(val = static_cast<f32>(tmp_val), true)
			: false;
	}

	inline bool get_value(const YGGR_BSON_VALUE_TAG_TYPE(double)&, f64& val) const
	{
		return bson_value_native_ex::s_value_to_double(val, this);
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			boost::mpl::or_
			<
				charset::utf8_string_loadable<String>,
				charset::utf8_string_view_loadable<String>
			>,
			bool
		>::type
		get_value(const YGGR_BSON_VALUE_TAG_TYPE(utf8)&, String& val) const
	{
		return bson_value_native_ex::s_value_to_string(val, this);
	}

	//template<typename Char, typename Traits> inline
	//bool get_value(const YGGR_BSON_VALUE_TAG_TYPE(utf8)&, charset::basic_utf8_string_view<Char, Traits>& val) const
	//{
	//	//return bson_value_native_ex::s_value_to_utf8_string_view(val, this);
	//	return bson_value_native_ex::s_value_to_string(val, this);
	//}

	inline bool get_value(const YGGR_BSON_VALUE_TAG_TYPE(null)&) const
	{
		return bson_value_native_ex::s_value_to_null(this);
	}

	inline bool get_value(const YGGR_BSON_VALUE_TAG_TYPE(null)&, c_bson_null&) const
	{
		return bson_value_native_ex::s_value_to_null(this);
	}

	inline bool get_value(const YGGR_BSON_VALUE_TAG_TYPE(undefined)&) const
	{
		return bson_value_native_ex::s_value_to_undefined(this);
	}

	inline bool get_value(const YGGR_BSON_VALUE_TAG_TYPE(undefined)&, c_bson_undefined&) const
	{
		return bson_value_native_ex::s_value_to_undefined(this);
	}

	inline bool get_value(const YGGR_BSON_VALUE_TAG_TYPE(maxkey)&) const
	{
		return bson_value_native_ex::s_value_to_maxkey(this);
	}

	inline bool get_value(const YGGR_BSON_VALUE_TAG_TYPE(maxkey)&, c_bson_maxkey&) const
	{
		return bson_value_native_ex::s_value_to_maxkey(this);
	}

	inline bool get_value(const YGGR_BSON_VALUE_TAG_TYPE(minkey)&) const
	{
		return bson_value_native_ex::s_value_to_minkey(this);
	}

	inline bool get_value(const YGGR_BSON_VALUE_TAG_TYPE(minkey)&, c_bson_minkey&) const
	{
		return bson_value_native_ex::s_value_to_minkey(this);
	}

	inline bool get_value(const YGGR_BSON_VALUE_TAG_TYPE(date_time)&, s64& val) const
	{
		return bson_value_native_ex::s_value_to_date(val, this);
	}

	inline bool get_value(const YGGR_BSON_VALUE_TAG_TYPE(date_time)&, c_bson_date& val) const
	{
		return bson_value_native_ex::s_value_to_date(val.date(), this);
	}

	inline bool get_value(const YGGR_BSON_VALUE_TAG_TYPE(time)&, s64& val) const
	{
		return bson_value_native_ex::s_value_to_time(val, this);
	}

	inline bool get_value(const YGGR_BSON_VALUE_TAG_TYPE(time)&, c_bson_time& val) const
	{
		return bson_value_native_ex::s_value_to_time(val.time(), this);
	}

	template<typename BsonTimeval> inline
	typename boost::enable_if<boost::is_base_of<timeval, BsonTimeval>, bool>::type
		get_value(const YGGR_BSON_VALUE_TAG_TYPE(timeval)&, BsonTimeval& val) const
	{
		return bson_value_native_ex::s_value_to_timeval(boost::addressof(val), this);
	}

	template<typename BsonOid> inline
	typename boost::enable_if<boost::is_base_of<bson_oid_t, BsonOid>, bool>::type
		get_value(const YGGR_BSON_VALUE_TAG_TYPE(oid)&, BsonOid& val) const
	{
		return bson_value_native_ex::s_value_to_oid(boost::addressof(val), this);
	}

	template<typename Buffer> inline
	bool get_value(const YGGR_BSON_VALUE_TAG_TYPE(binary)&, u32& sub_t, Buffer& val) const
	{
		return bson_value_native_ex::s_value_to_binary_buffer(sub_t, val, this);
	}

	template<u32 BINARY_ID> inline
	bool get_value(const YGGR_BSON_VALUE_TAG_TYPE(binary)&, c_bson_binary_buffer<BINARY_ID>& val) const
	{
		typedef c_bson_binary_buffer<BINARY_ID> binary_buf_type;
		u32 sub_t = 0;

		return
			bson_value_native_ex::s_value_to_binary_buffer(sub_t, val.buffer(), this)
				&& ((sub_t == binary_buf_type::buffer_typeid()) || (val.clear(), false));
	}

	template<typename String1, typename String2 > inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::or_
				<
					charset::utf8_string_loadable<String1>,
					charset::utf8_string_view_loadable<String1>
				>,
				charset::utf8_string_loadable<String2>
			>,
			bool
		>::type
		get_value(const YGGR_BSON_VALUE_TAG_TYPE(regex)&,
					String1& val_regex, String2& val_opts) const
	{
		return bson_value_native_ex::s_value_to_regex(val_regex, val_opts, this);
	}

	inline bool get_value(const YGGR_BSON_VALUE_TAG_TYPE(regex)&, c_bson_regex& val) const
	{
		typedef c_bson_regex::inner_string_type now_string_type;

		now_string_type regex, opts;
		bool bchk = bson_value_native_ex::s_value_to_regex(regex, opts, this);

		if(bchk)
		{
			c_bson_regex tmp(boost::move(regex), boost::move(opts));
			val.swap(tmp);
		}

		return bchk;
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			boost::mpl::or_
			<
				charset::utf8_string_loadable<String>,
				charset::utf8_string_view_loadable<String>
			>,
			bool
		>::type
		get_value(const YGGR_BSON_VALUE_TAG_TYPE(code)&, String& val) const
	{
		return bson_value_native_ex::s_value_to_code(val, this);
	}

	inline bool get_value(const YGGR_BSON_VALUE_TAG_TYPE(code)&, c_bson_code& val) const
	{
		return bson_value_native_ex::s_value_to_code(val.code(), this);
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			boost::mpl::or_
			<
				charset::utf8_string_loadable<String>,
				charset::utf8_string_view_loadable<String>
			>,
			bool
		>::type
		get_value(const YGGR_BSON_VALUE_TAG_TYPE(symbol)&, String& val) const
	{
		return bson_value_native_ex::s_value_to_symbol(val, this);
	}

	inline bool get_value(const YGGR_BSON_VALUE_TAG_TYPE(symbol)&, c_bson_symbol& val) const
	{
		return bson_value_native_ex::s_value_to_symbol(val.symbol(), this);
	}

	template<typename String, typename Bson> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::or_
				<
					charset::utf8_string_loadable<String>,
					charset::utf8_string_view_loadable<String>
				>,
				boost::is_base_of<bson_t, Bson>
			>,
			bool
		>::type
		get_value(const YGGR_BSON_VALUE_TAG_TYPE(codewscope)&, String& val, Bson& scope) const
	{
		return bson_value_native_ex::s_value_to_code_w_scope(val, &scope, this);
	}

	template<typename Scope> inline
	bool get_value(const YGGR_BSON_VALUE_TAG_TYPE(codewscope)&, c_bson_code_w_scope<Scope>& val) const
	{
		return bson_value_native_ex::s_value_to_code_w_scope(val.code(), &(val.scope()), this);
	}

	inline bool get_value(const YGGR_BSON_VALUE_TAG_TYPE(timestamp)&, u32& timestamp, u32& increment) const
	{
		return bson_value_native_ex::s_value_to_timestamp(timestamp, increment, this);
	}

	template<typename BsonTimestamp> inline
	typename boost::enable_if<boost::is_base_of<bson_timestamp_t, BsonTimestamp>, bool>::type
		get_value(const YGGR_BSON_VALUE_TAG_TYPE(timestamp)&, BsonTimestamp& val) const
	{
		return bson_value_native_ex::s_value_to_timestamp(val.t, val.i, this);
	}

	template<typename String, typename BsonOid> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::or_
				<
					charset::utf8_string_loadable<String>,
					charset::utf8_string_view_loadable<String>
				>,
				boost::is_base_of<bson_oid_t, BsonOid>
			>,
			bool
		>::type
		get_value(const YGGR_BSON_VALUE_TAG_TYPE(dbpointer)&, String& val, BsonOid& oid) const
	{
		return bson_value_native_ex::s_value_to_dbpointer(val, boost::addressof(oid), this);
	}

	template<typename BsonOid> inline
	bool get_value(const YGGR_BSON_VALUE_TAG_TYPE(dbpointer)&, c_bson_dbpointer<BsonOid>& val) const
	{
		return bson_value_native_ex::s_value_to_dbpointer(val.collection(), boost::addressof(val.oid()), this);
	}

	template<typename BsonDecimal128> inline
	typename boost::enable_if<boost::is_base_of<bson_decimal128_t, BsonDecimal128>, bool>::type
		get_value(const YGGR_BSON_VALUE_TAG_TYPE(decimal128)&, BsonDecimal128& val) const
	{
		return bson_value_native_ex::s_value_to_decimal128(boost::addressof(val), this);
	}

	template<typename Bson> inline
	typename boost::enable_if< boost::is_base_of< bson_t, Bson>, bool>::type
		get_value(const YGGR_BSON_VALUE_TAG_TYPE(document)&, Bson& val) const
	{
		return bson_value_native_ex::s_value_to_bson(&val, this);
	}

	template<typename Bson> inline
	typename boost::enable_if< boost::is_base_of< bson_t, Bson>, bool>::type
		get_value(const YGGR_BSON_VALUE_TAG_TYPE(array)&, Bson& val) const
	{
		return bson_value_native_ex::s_value_to_array(&val, this);
	}

	//------------------------------
	// comparer

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename Tag, typename ...Args> inline
	bool compare_eq(BOOST_FWD_REF(Args)... args) const
	{
		return this_type::compare_eq(Tag(), boost::forward<Args>(args)...);
	}

	template<typename Tag, typename ...Args> inline
	bool compare(BOOST_FWD_REF(Args)... args) const
	{
		return this_type::compare_eq(Tag(), boost::forward<Args>(args)...);
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template< typename Tag \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
	bool compare_eq( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const { \
		return \
			this_type::compare_eq( \
				Tag() YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
	\
	template< typename Tag \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
	bool compare( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const { \
		return \
			this_type::compare_eq( \
				Tag() YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }

#	define YGGR_PP_FOO_ARG_NAME() arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

#define YGGR_TMP_PP_C_BSON_VALUE_COMPARE_GENERAL_FOOS( __foo_name__ ) \
	inline bool __foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(null)&) const { \
		return bson_value_native_ex::s_value_cmp_eq_null(this); } \
	\
	inline bool __foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(null)&, const c_bson_null&) const { \
		return bson_value_native_ex::s_value_cmp_eq_null(this); } \
	\
	inline bool __foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(undefined)&) const { \
		return bson_value_native_ex::s_value_cmp_eq_undefined(this); } \
	\
	inline bool __foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(undefined)&, const c_bson_undefined&) const { \
		return bson_value_native_ex::s_value_cmp_eq_undefined(this); } \
	\
	inline bool __foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(maxkey)&) const { \
		return bson_value_native_ex::s_value_cmp_eq_maxkey(this); } \
	\
	inline bool __foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(maxkey)&, const c_bson_maxkey&) const { \
		return bson_value_native_ex::s_value_cmp_eq_maxkey(this); } \
	\
	inline bool __foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(minkey)&) const { \
		return bson_value_native_ex::s_value_cmp_eq_minkey(this); } \
	\
	inline bool __foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(minkey)&, const c_bson_minkey&) const { \
		return bson_value_native_ex::s_value_cmp_eq_minkey(this); } \
	\
	inline bool __foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(binary)&, u32 sub_t, const u8* val, u64 size) const { \
		return bson_value_native_ex::s_value_cmp_eq_binary_buffer(this, sub_t, val, size); } \
	\
	template<u32 BINARY_ID> inline \
	bool __foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(binary)&, const c_bson_binary_buffer<BINARY_ID>& val) const { \
		return bson_value_native_ex::s_value_cmp_eq_binary_buffer(this, val.buffer_typeid(), val.data(), val.size()); } \
	\
	template<u32 BINARY_ID, typename Buffer> inline \
	bool __foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(binary)&, const c_bson_binary_buffer_ref<BINARY_ID, Buffer>& val) const { \
		c_bson_binary_buffer<BINARY_ID> buf(val.conv_to_binary_buffer()); \
		return bson_value_native_ex::s_value_cmp_eq_binary_buffer(this, buf.buffer_typeid(), buf.data(), buf.size()); } \
	\
	template<typename String1, typename String2> inline \
	typename \
		boost::enable_if< \
			boost::mpl::and_< \
				charset::utf8_string_constructable<String1>, \
				charset::utf8_string_constructable<String2> >, \
			bool >::type \
		__foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(regex)&, \
					const String1& val_regex, const String2& val_opts) const { \
		return bson_value_native_ex::s_value_cmp_eq_regex(this, val_regex, val_opts); } \
	\
	inline bool __foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(regex)&, const c_bson_regex& val) const { \
		return \
			bson_value_native_ex::s_value_cmp_eq_regex( \
				this, \
				val.regex<c_bson_regex::string_type>(), \
				val.options<c_bson_regex::string_type>()); } \
	\
	template<typename String> inline \
	typename boost::enable_if< charset::utf8_string_constructable<String>, bool>::type \
		__foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(code)&, const String& val) const { \
		return bson_value_native_ex::s_value_cmp_eq_code(this, val); } \
	\
	inline bool __foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(code)&, const c_bson_code& val) const { \
		return bson_value_native_ex::s_value_cmp_eq_code(this, val.code<c_bson_code::string_type>()); } \
	\
	template<typename String> inline \
	typename boost::enable_if< charset::utf8_string_constructable<String>, bool>::type \
		__foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(symbol)&, const String& val) const { \
		return bson_value_native_ex::s_value_cmp_eq_symbol(this, val); } \
	\
	inline bool __foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(symbol)&, const c_bson_symbol& val) const { \
		return bson_value_native_ex::s_value_cmp_eq_symbol(this, val.symbol<c_bson_symbol::string_type>()); } \
	\
	template<typename String> inline \
	typename boost::enable_if< charset::utf8_string_constructable<String>, bool>::type \
		__foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(codewscope)&, const String& val, const bson_t& scope) const { \
		return bson_value_native_ex::s_value_cmp_eq_code_w_scope(this, val, boost::addressof(scope)); } \
	\
	template<typename Scope> inline \
	bool __foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(codewscope)&, const c_bson_code_w_scope<Scope>& val) const { \
		typedef c_bson_code_w_scope<Scope> code_w_scope_type; \
		return \
			bson_value_native_ex::s_value_cmp_eq_code_w_scope( \
                this, val.template code<typename code_w_scope_type::string_type>(), boost::addressof(val.scope())); } \
	\
	inline bool __foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(timestamp)&, u32 timestamp, u32 increment) const { \
		return bson_value_native_ex::s_value_cmp_eq_timestamp(this, timestamp, increment); } \
	\
	template<typename BsonTimestamp> inline \
	typename boost::enable_if<boost::is_base_of<bson_timestamp_t, BsonTimestamp>, bool>::type \
		__foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(timestamp)&, const BsonTimestamp& val) const { \
		return bson_value_native_ex::s_value_cmp_eq_timestamp(this, val.t, val.i); } \
	\
	template<typename String> inline \
	typename boost::enable_if< charset::utf8_string_constructable<String>, bool>::type \
		__foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(dbpointer)&, \
						const String& val, const bson_oid_t& oid) const { \
		return bson_value_native_ex::s_value_cmp_eq_dbpointer(this, val, boost::addressof(oid)); } \
	\
	template<typename BsonOid> inline \
	bool __foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(dbpointer)&, const c_bson_dbpointer<BsonOid>& val) const { \
		typedef c_bson_dbpointer<BsonOid> dbpointer_type; \
		return \
			bson_value_native_ex::s_value_cmp_eq_dbpointer( \
                this, val.template collection<typename dbpointer_type::string_type>(), boost::addressof(val.oid())); } \
	\
	template<typename Bson> inline \
	typename boost::enable_if< boost::is_base_of< bson_t, Bson>, bool>::type \
		__foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(document)&, const Bson& val) const { \
		return bson_value_native_ex::s_value_cmp_eq_bson(this, boost::addressof(val)); } \
	\
	template<typename Bson> inline \
	typename boost::enable_if< boost::is_base_of< bson_t, Bson>, bool>::type \
		__foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(array)&, const Bson& val) const { \
		return bson_value_native_ex::s_value_cmp_eq_array(this, boost::addressof(val)); }

#define YGGR_TMP_PP_C_BSON_VALUE_COMPARE_EQUAL_FOOS( __foo_name__ ) \
	YGGR_TMP_PP_C_BSON_VALUE_COMPARE_GENERAL_FOOS( __foo_name__ ) \
	\
	inline bool __foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(int32)&, s32 val) const { \
		return bson_value_native_ex::s_value_cmp_eq_int32(this, val); } \
	\
	inline bool __foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(int64)&, s64 val) const { \
		return bson_value_native_ex::s_value_cmp_eq_int64(this, val); } \
	\
	inline bool __foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(bool)&, bool val) const { \
		return bson_value_native_ex::s_value_cmp_eq_bool(this, val); } \
	\
	inline bool __foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(double)&, f64 val) const { \
		return bson_value_native_ex::s_value_cmp_eq_double(this, val); } \
	\
	template<typename String > inline \
	typename boost::enable_if< charset::utf8_string_constructable<String>, bool>::type \
		__foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(utf8)&, const String& val) { \
		return bson_value_native_ex::s_value_cmp_eq_string(this, val); } \
	\
	inline bool __foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(date_time)&, s64 val) const { \
		return bson_value_native_ex::s_value_cmp_eq_date(this, val); } \
	\
	inline bool __foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(date_time)&, const c_bson_date& val) const { \
		return bson_value_native_ex::s_value_cmp_eq_date(this, val.date()); } \
	\
	inline bool __foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(time)&, s64 val) const { \
		return bson_value_native_ex::s_value_cmp_eq_time(this, val); } \
	\
	inline bool __foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(time)&, const c_bson_time& val) const { \
		return bson_value_native_ex::s_value_cmp_eq_time(this, val.time()); } \
	\
	template<typename BsonTimeval> inline \
	typename boost::enable_if<boost::is_base_of<timeval, BsonTimeval>, bool>::type \
		__foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(timeval)&, const BsonTimeval& val) const { \
		return bson_value_native_ex::s_value_cmp_eq_timeval(this, boost::addressof(val)); } \
	\
	template<typename BsonOid> inline \
	typename boost::enable_if<boost::is_base_of<bson_oid_t, BsonOid>, bool>::type \
		__foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(oid)&, const BsonOid& val) const { \
		return bson_value_native_ex::s_value_cmp_eq_oid(this, boost::addressof(val)); } \
	\
	template<typename BsonDecimal128> inline \
	typename boost::enable_if<boost::is_base_of<bson_decimal128_t, BsonDecimal128>, bool>::type \
		__foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(decimal128)&, const BsonDecimal128& val) const { \
		return bson_value_native_ex::s_value_cmp_eq_decimal128(this, boost::addressof(val)); } \
	\
	template<typename BsonValue> inline \
	typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type \
		__foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(value)&, const BsonValue& val) const { \
		return bson_value_native_ex::s_value_cmp_eq_value(this, boost::addressof(val)); } \
	\
	template<typename BsonValue> inline \
	typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type \
		__foo_name__(const BsonValue& val) const { \
		return bson_value_native_ex::s_value_cmp_eq_value(this, boost::addressof(val)); }

#define YGGR_TMP_PP_C_BSON_VALUE_COMPARE_FOOS( __foo_name__ ) \
	YGGR_TMP_PP_C_BSON_VALUE_COMPARE_GENERAL_FOOS( __foo_name__ ) \
	\
	inline s32 __foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(int32)&, s32 val) const { \
		return bson_value_native_ex::s_value_cmp_int32(this, val); } \
	\
	inline s32 __foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(int64)&, s64 val) const { \
		return bson_value_native_ex::s_value_cmp_int64(this, val); } \
	\
	inline s32 __foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(bool)&, bool val) const { \
		return bson_value_native_ex::s_value_cmp_bool(this, val); } \
	\
	inline s32 __foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(double)&, f64 val) const { \
		return bson_value_native_ex::s_value_cmp_double(this, val); } \
	\
	template<typename String > inline \
	typename boost::enable_if< charset::utf8_string_constructable<String>, s32>::type \
		__foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(utf8)&, const String& val) { \
		return bson_value_native_ex::s_value_cmp_string(this, val); } \
	\
	inline s32 __foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(date_time)&, s64 val) const { \
		return bson_value_native_ex::s_value_cmp_date(this, val); } \
	\
	inline s32 __foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(date_time)&, const c_bson_date& val) const { \
		return bson_value_native_ex::s_value_cmp_date(this, val.date()); } \
	\
	inline s32 __foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(time)&, s64 val) const { \
		return bson_value_native_ex::s_value_cmp_time(this, val); } \
	\
	inline s32 __foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(time)&, const c_bson_time& val) const { \
		return bson_value_native_ex::s_value_cmp_time(this, val.time()); } \
	\
	template<typename BsonTimeval> inline \
	typename boost::enable_if<boost::is_base_of<timeval, BsonTimeval>, s32>::type \
		__foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(timeval)&, const BsonTimeval& val) const { \
		return bson_value_native_ex::s_value_cmp_timeval(this, boost::addressof(val)); } \
	\
	template<typename BsonOid> inline \
	typename boost::enable_if<boost::is_base_of<bson_oid_t, BsonOid>, s32>::type \
		__foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(oid)&, const BsonOid& val) const { \
		return bson_value_native_ex::s_value_cmp_oid(this, boost::addressof(val)); } \
	\
	template<typename BsonDecimal128> inline \
	typename boost::enable_if<boost::is_base_of<bson_decimal128_t, BsonDecimal128>, s32>::type \
		__foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(decimal128)&, const BsonDecimal128& val) const { \
		return bson_value_native_ex::s_value_cmp_decimal128(this, boost::addressof(val)); } \
	\
	template<typename BsonValue> inline \
	typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, s32>::type \
		__foo_name__(const YGGR_BSON_VALUE_TAG_TYPE(value)&, const BsonValue& val) const { \
		return bson_value_native_ex::s_value_cmp_value(this, boost::addressof(val)); } \
	\
	template<typename BsonValue> inline \
	typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, s32>::type \
		__foo_name__(const BsonValue& val) const { \
		return bson_value_native_ex::s_value_cmp_value(this, boost::addressof(val)); }


	YGGR_TMP_PP_C_BSON_VALUE_COMPARE_EQUAL_FOOS(compare_eq)
	YGGR_TMP_PP_C_BSON_VALUE_COMPARE_FOOS(compare)

#undef YGGR_TMP_PP_C_BSON_VALUE_COMPARE_FOOS

protected:
	void pro_init(void);

private:
	YGGR_PP_FRIEND_CLASS( bson_inner_data_accesser<true> );

	//save and load def
	template<typename Bson, typename String> inline
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
		save(Bson& bs, const String& name) const
	{
		typedef String now_string_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, now_inner_string_type);

		if(bson_native_ex::s_bson_is_nil(&bs))
		{
			bson_init(&bs);
		}

		const base_type& base = *this;
		now_inner_string_type str_name((charset::string_charset_helper_data(name)));

		return bson_append_value(&bs, str_name.data(), static_cast<int>(str_name.size()), &base);
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, bool>::type
		save_value(Bson& bs) const
	{
		const base_type& base = *this;
		return bson_native_ex::s_bson_save_value_of_value(&bs, &base);
	}

	template<typename BsonIter, typename String> inline
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
		load(const BsonIter& iter, const String& name)
	{
		assert(iter.load_check(bson_typeid< boost::remove_const<this_type>::type >::value, name));
		if(!iter.load_check(bson_typeid< boost::remove_const<this_type>::type >::value, name))
		{
			return;
		}

		base_type& base = *this;
		const bson_iter_t* piter = boost::addressof(iter);
		const org_type* pval = bson_iter_value(const_cast<bson_iter_t*>(piter));
		if(pval)
		{
			bson_value_native_ex::s_bson_value_copy(&base, pval);
		}
	}

	template<typename BsonIter> inline
	typename boost::enable_if<boost::is_base_of<bson_iter_t, BsonIter>, void>::type
		load_value(const BsonIter& iter)
	{
		base_type& base = *this;
		const bson_iter_t* piter = boost::addressof(iter);
		const org_type* pval = bson_iter_value(const_cast<bson_iter_t*>(piter));
		if(pval)
		{
			bson_value_native_ex::s_bson_value_copy(&base, pval);
		}
	}
};

// non member foo

// s32
// ==
inline bool operator==(const c_bson_value& l, s32 r)
{
	return l.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(int32), r);
}

inline bool operator==(s32 l, const c_bson_value& r)
{
	return r.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(int32), l);
}

// !=
inline bool operator!=(const c_bson_value& l, s32 r)
{
	return !l.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(int32), r);
}

inline bool operator!=(s32 l, const c_bson_value& r)
{
	return !r.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(int32), l);
}

// <
inline bool operator<(const c_bson_value& l, s32 r)
{
	return l.compare(YGGR_BSON_VALUE_TAG_OBJECT(int32), r) < 0;
}

inline bool operator<(s32 l, const c_bson_value& r)
{
	return r.compare(YGGR_BSON_VALUE_TAG_OBJECT(int32), l) > 0;
}

// <=
inline bool operator<=(const c_bson_value& l, s32 r)
{
	return l.compare(YGGR_BSON_VALUE_TAG_OBJECT(int32), r) <= 0;
}

inline bool operator<=(s32 l, const c_bson_value& r)
{
	return r.compare(YGGR_BSON_VALUE_TAG_OBJECT(int32), l) >= 0;
}

// >
inline bool operator>(const c_bson_value& l, s32 r)
{
	return l.compare(YGGR_BSON_VALUE_TAG_OBJECT(int32), r) > 0;
}

inline bool operator>(s32 l, const c_bson_value& r)
{
	return r.compare(YGGR_BSON_VALUE_TAG_OBJECT(int32), l) < 0;
}

// >=
inline bool operator>=(const c_bson_value& l, s32 r)
{
	return l.compare(YGGR_BSON_VALUE_TAG_OBJECT(int32), r) >= 0;
}

inline bool operator>=(s32 l, const c_bson_value& r)
{
	return r.compare(YGGR_BSON_VALUE_TAG_OBJECT(int32), l) <= 0;
}


// s64
// ==
inline bool operator==(const c_bson_value& l, s64 r)
{
	return l.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(int64), r);
}

inline bool operator==(s64 l, const c_bson_value& r)
{
	return r.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(int64), l);
}

// !=
inline bool operator!=(const c_bson_value& l, s64 r)
{
	return !l.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(int64), r);
}

inline bool operator!=(s64 l, const c_bson_value& r)
{
	return !r.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(int64), l);
}

// <
inline bool operator<(const c_bson_value& l, s64 r)
{
	return l.compare(YGGR_BSON_VALUE_TAG_OBJECT(int64), r) < 0;
}

inline bool operator<(s64 l, const c_bson_value& r)
{
	return r.compare(YGGR_BSON_VALUE_TAG_OBJECT(int64), l) > 0;
}

// <=
inline bool operator<=(const c_bson_value& l, s64 r)
{
	return l.compare(YGGR_BSON_VALUE_TAG_OBJECT(int64), r) <= 0;
}

inline bool operator<=(s64 l, const c_bson_value& r)
{
	return r.compare(YGGR_BSON_VALUE_TAG_OBJECT(int64), l) >= 0;
}

// >
inline bool operator>(const c_bson_value& l, s64 r)
{
	return l.compare(YGGR_BSON_VALUE_TAG_OBJECT(int64), r) > 0;
}

inline bool operator>(s64 l, const c_bson_value& r)
{
	return r.compare(YGGR_BSON_VALUE_TAG_OBJECT(int64), l) < 0;
}

// >=
inline bool operator>=(const c_bson_value& l, s64 r)
{
	return l.compare(YGGR_BSON_VALUE_TAG_OBJECT(int64), r) >= 0;
}

inline bool operator>=(s64 l, const c_bson_value& r)
{
	return r.compare(YGGR_BSON_VALUE_TAG_OBJECT(int64), l) <= 0;
}

// bool
// ==
inline bool operator==(const c_bson_value& l, bool r)
{
	return bson_value_native_ex::s_value_cmp_eq_bool(&l, r);
}

inline bool operator==(bool l, const c_bson_value& r)
{
	return bson_value_native_ex::s_value_cmp_eq_bool(&r, l);
}

// !=
inline bool operator!=(const c_bson_value& l, bool r)
{
	return !bson_value_native_ex::s_value_cmp_eq_bool(&l, r);
}

inline bool operator!=(bool l, const c_bson_value& r)
{
	return !bson_value_native_ex::s_value_cmp_eq_bool(&r, l);
}

// <
inline bool operator<(const c_bson_value& l, bool r)
{
	return l.compare(YGGR_BSON_VALUE_TAG_OBJECT(bool), r) < 0;
}

inline bool operator<(bool l, const c_bson_value& r)
{
	return r.compare(YGGR_BSON_VALUE_TAG_OBJECT(bool), l) > 0;
}

// <=
inline bool operator<=(const c_bson_value& l, bool r)
{
	return l.compare(YGGR_BSON_VALUE_TAG_OBJECT(bool), r) <= 0;
}

inline bool operator<=(bool l, const c_bson_value& r)
{
	return r.compare(YGGR_BSON_VALUE_TAG_OBJECT(bool), l) >= 0;
}

// >
inline bool operator>(const c_bson_value& l, bool r)
{
	return l.compare(YGGR_BSON_VALUE_TAG_OBJECT(bool), r) > 0;
}

inline bool operator>(bool l, const c_bson_value& r)
{
	return r.compare(YGGR_BSON_VALUE_TAG_OBJECT(bool), l) < 0;
}

// >=
inline bool operator>=(const c_bson_value& l, bool r)
{
	return l.compare(YGGR_BSON_VALUE_TAG_OBJECT(bool), r) >= 0;
}

inline bool operator>=(bool l, const c_bson_value& r)
{
	return r.compare(YGGR_BSON_VALUE_TAG_OBJECT(bool), l) <= 0;
}

// f32
// ==
inline bool operator==(const c_bson_value& l, f32 r)
{
	return bson_value_native_ex::s_value_cmp_eq_double(&l, static_cast<f64>(r));
}

inline bool operator==(f32 l, const c_bson_value& r)
{
	return bson_value_native_ex::s_value_cmp_eq_double(&r, static_cast<f64>(l));
}

// !=
inline bool operator!=(const c_bson_value& l, f32 r)
{
	return !bson_value_native_ex::s_value_cmp_eq_double(&l, static_cast<f64>(r));
}

inline bool operator!=(f32 l, const c_bson_value& r)
{
	return !bson_value_native_ex::s_value_cmp_eq_double(&r, static_cast<f64>(l));
}

// <
inline bool operator<(const c_bson_value& l, f32 r)
{
	return bson_value_native_ex::s_value_cmp_double(&l, static_cast<f64>(r)) < 0;
}

inline bool operator<(f32 l, const c_bson_value& r)
{
	return bson_value_native_ex::s_value_cmp_double(&r, static_cast<f64>(l)) > 0;
}

// <=
inline bool operator<=(const c_bson_value& l, f32 r)
{
	return bson_value_native_ex::s_value_cmp_double(&l, static_cast<f64>(r)) <= 0;
}

inline bool operator<=(f32 l, const c_bson_value& r)
{
	return bson_value_native_ex::s_value_cmp_double(&r, static_cast<f64>(l)) >= 0;
}

// >
inline bool operator>(const c_bson_value& l, f32 r)
{
	return bson_value_native_ex::s_value_cmp_double(&l, static_cast<f64>(r)) > 0;
}

inline bool operator>(f32 l, const c_bson_value& r)
{
	return bson_value_native_ex::s_value_cmp_double(&r, static_cast<f64>(l)) < 0;
}

// >=
inline bool operator>=(const c_bson_value& l, f32 r)
{
	return bson_value_native_ex::s_value_cmp_double(&l, static_cast<f64>(r)) >= 0;
}

inline bool operator>=(f32 l, const c_bson_value& r)
{
	return bson_value_native_ex::s_value_cmp_double(&r, static_cast<f64>(l)) <= 0;
}

// f64
// ==
inline bool operator==(const c_bson_value& l, f64 r)
{
	return bson_value_native_ex::s_value_cmp_eq_double(&l, r);
}

inline bool operator==(f64 l, const c_bson_value& r)
{
	return bson_value_native_ex::s_value_cmp_eq_double(&r, l);
}

// !=
inline bool operator!=(const c_bson_value& l, f64 r)
{
	return !bson_value_native_ex::s_value_cmp_eq_double(&l, r);
}

inline bool operator!=(f64 l, const c_bson_value& r)
{
	return !bson_value_native_ex::s_value_cmp_eq_double(&r, l);
}

// <
inline bool operator<(const c_bson_value& l, f64 r)
{
	return bson_value_native_ex::s_value_cmp_double(&l, r) < 0;
}

inline bool operator<(f64 l, const c_bson_value& r)
{
	return bson_value_native_ex::s_value_cmp_double(&r, l) > 0;
}

// <=
inline bool operator<=(const c_bson_value& l, f64 r)
{
	return bson_value_native_ex::s_value_cmp_double(&l, r) <= 0;
}

inline bool operator<=(f64 l, const c_bson_value& r)
{
	return bson_value_native_ex::s_value_cmp_double(&r, l) >= 0;
}

// >
inline bool operator>(const c_bson_value& l, f64 r)
{
	return bson_value_native_ex::s_value_cmp_double(&l, r) > 0;
}

inline bool operator>(f64 l, const c_bson_value& r)
{
	return bson_value_native_ex::s_value_cmp_double(&r, l) < 0;
}

// >=
inline bool operator>=(const c_bson_value& l, f64 r)
{
	return bson_value_native_ex::s_value_cmp_double(&l, r) >= 0;
}

inline bool operator>=(f64 l, const c_bson_value& r)
{
	return bson_value_native_ex::s_value_cmp_double(&r, l) <= 0;
}

// string
// ==
template<typename String > inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			boost::mpl::not_
			<
				// fix boost 165 and later c2666 'yggr::nsql_database_system::operator ==' : 22 overloads have similar conversions
				boost::mpl::or_
				<
					boost::is_same<int, String>,
					::yggr::is_enum<String>
				>
			>,
			charset::utf8_string_constructable<String>
		>,
		bool
	>::type
	operator==(const c_bson_value& l, const String& r)
{
	return bson_value_native_ex::s_value_cmp_eq_string(&l, r);
}

template<typename String> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			boost::mpl::not_
			<
				boost::mpl::or_
				<
					boost::is_same<int, String>,
					::yggr::is_enum<String>
				>
			>,
			charset::utf8_string_constructable<String>
		>,
		bool
	>::type
	operator==(const String& l, const c_bson_value& r)
{
	return bson_value_native_ex::s_value_cmp_eq_string(&r, l);
}

// !=
template<typename String > inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			boost::mpl::not_
			<
				boost::mpl::or_
				<
					boost::is_same<int, String>,
					::yggr::is_enum<String>
				>
			>,
			charset::utf8_string_constructable<String>
		>,
		bool
	>::type
	operator!=(const c_bson_value& l, const String& r)
{
	return !bson_value_native_ex::s_value_cmp_eq_string(&l, r);
}

template<typename String> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			boost::mpl::not_
			<
				boost::mpl::or_
				<
					boost::is_same<int, String>,
					::yggr::is_enum<String>
				>
			>,
			charset::utf8_string_constructable<String>
		>,
		bool
	>::type
	operator!=(const String& l, const c_bson_value& r)
{
	return !bson_value_native_ex::s_value_cmp_eq_string(&r, l);
}

// <
template<typename String > inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			boost::mpl::not_
			<
				boost::mpl::or_
				<
					boost::is_same<int, String>,
					::yggr::is_enum<String>
				>
			>,
			charset::utf8_string_constructable<String>
		>,
		bool
	>::type
	operator<(const c_bson_value& l, const String& r)
{
	return bson_value_native_ex::s_value_cmp_string(&l, r) < 0;
}

template<typename String> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			boost::mpl::not_
			<
				boost::mpl::or_
				<
					boost::is_same<int, String>,
					::yggr::is_enum<String>
				>
			>,
			charset::utf8_string_constructable<String>
		>,
		bool
	>::type
	operator<(const String& l, const c_bson_value& r)
{
	return bson_value_native_ex::s_value_cmp_string(&r, l) > 0;
}

// <=
template<typename String > inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			boost::mpl::not_
			<
				boost::mpl::or_
				<
					boost::is_same<int, String>,
					::yggr::is_enum<String>
				>
			>,
			charset::utf8_string_constructable<String>
		>,
		bool
	>::type
	operator<=(const c_bson_value& l, const String& r)
{
	return bson_value_native_ex::s_value_cmp_string(&l, r) <= 0;
}

template<typename String> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			boost::mpl::not_
			<
				boost::mpl::or_
				<
					boost::is_same<int, String>,
					::yggr::is_enum<String>
				>
			>,
			charset::utf8_string_constructable<String>
		>,
		bool
	>::type
	operator<=(const String& l, const c_bson_value& r)
{
	return bson_value_native_ex::s_value_cmp_string(&r, l) >= 0;
}

// >
template<typename String > inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			boost::mpl::not_
			<
				boost::mpl::or_
				<
					boost::is_same<int, String>,
					::yggr::is_enum<String>
				>
			>,
			charset::utf8_string_constructable<String>
		>,
		bool
	>::type
	operator>(const c_bson_value& l, const String& r)
{
	return bson_value_native_ex::s_value_cmp_string(&l, r) > 0;
}

template<typename String> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			boost::mpl::not_
			<
				boost::mpl::or_
				<
					boost::is_same<int, String>,
					::yggr::is_enum<String>
				>
			>,
			charset::utf8_string_constructable<String>
		>,
		bool
	>::type
	operator>(const String& l, const c_bson_value& r)
{
	return bson_value_native_ex::s_value_cmp_string(&r, l) < 0;
}

// >=
template<typename String > inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			boost::mpl::not_
			<
				boost::mpl::or_
				<
					boost::is_same<int, String>,
					::yggr::is_enum<String>
				>
			>,
			charset::utf8_string_constructable<String>
		>,
		bool
	>::type
	operator>=(const c_bson_value& l, const String& r)
{
	return bson_value_native_ex::s_value_cmp_string(&l, r) >= 0;
}

template<typename String> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			boost::mpl::not_
			<
				boost::mpl::or_
				<
					boost::is_same<int, String>,
					::yggr::is_enum<String>
				>
			>,
			charset::utf8_string_constructable<String>
		>,
		bool
	>::type
	operator>=(const String& l, const c_bson_value& r)
{
	return bson_value_native_ex::s_value_cmp_string(&r, l) <= 0;
}

// null
// undefined
// maxkey
// minkey
// date
// time
// timeval
// oid_org_type
// oid
// binary_buffer
// binary_buffer_ref
// regex
// code
// symbol
// code_w_scope
// timestamp
// decimal128_org_type
// decimal128
// dbpointer
// bson and array
// ==
template<typename Bson> inline
typename boost::enable_if< boost::is_base_of< bson_t, Bson>, bool>::type
	operator==(const c_bson_value& l, const Bson& r)
{
	return bson_native_ex::s_bson_is_array(&r)?
			bson_value_native_ex::s_value_cmp_eq_array(&l, &r)
			: bson_value_native_ex::s_value_cmp_eq_bson(&l, &r);
}

// !=
template<typename Bson> inline
typename boost::enable_if< boost::is_base_of< bson_t, Bson>, bool>::type
	operator!=(const c_bson_value& l, const Bson& r)
{
	return !(bson_native_ex::s_bson_is_array(&r)?
				bson_value_native_ex::s_value_cmp_eq_array(&l, &r)
				: bson_value_native_ex::s_value_cmp_eq_bson(&l, &r));
}

// c_bson_value
// ==
template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator==(const c_bson_value& l, const BsonValue& r)
{
	return l.compare_eq(r);
}

// !=
template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator!=(const c_bson_value& l, const BsonValue& r)
{
	return !l.compare_eq(r);
}

// <
template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator<(const c_bson_value& l, const BsonValue& r)
{
	return l.compare(r) < 0;
}

// <=
template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator<=(const c_bson_value& l, const BsonValue& r)
{
	return l.compare(r) <= 0;
}

// >
template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator>(const c_bson_value& l, const BsonValue& r)
{
	return l.compare(r) > 0;
}

// >=
template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator>=(const c_bson_value& l, const BsonValue& r)
{
	return l.compare(r) >= 0;
}

namespace detail
{

template<>
struct bson_operator_outter<c_bson_value>
{
	template<typename Char, typename Traits> inline
	std::basic_ostream<Char, Traits>&
		operator()(std::basic_ostream<Char, Traits>& os,
					const c_bson_value& val,
					bson_json_mode_def::mode_type mode) const
	{
		typedef c_bson_value::org_type org_type;
		typedef bson_operator_outter<org_type> outter_type;

		outter_type outter;
		return outter(os, static_cast<const org_type&>(val), mode);
	}
};


} // namespace detail

// operator<<

template<typename Char, typename Traits> inline
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os, const c_bson_value& val)
{
	typedef detail::bson_operator_outter<c_bson_value> outter_type;

	outter_type outter;
	return outter(os, val, bson_json_mode_def::E_BSON_JSON_MODE_LEGACY);
}

// need test, if ok, remove it
//template<typename Char, typename Traits>
//std::basic_ostream<Char, Traits>&
//	operator<<(std::basic_ostream<Char, Traits>& os, BOOST_RV_REF(c_bson_value) val)
//{
//	const c_bson_value& right_cref = val;
//	return yggr::nsql_database_system::operator<<(os, right_cref);
//}

} // namespace nsql_database_system
} // namespace yggr

// non member foo
// s32
// ==
inline bool operator==(const bson_value_t& l, yggr::s32 r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_int32(
			boost::addressof(l), r);
}

inline bool operator==(yggr::s32 l, const bson_value_t& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_int32(
			boost::addressof(r), l);
}

// !=
inline bool operator!=(const bson_value_t& l, yggr::s32 r)
{
	return
		!(yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_int32(
			boost::addressof(l), r));
}

inline bool operator!=(yggr::s32 l, const bson_value_t& r)
{
	return
		!(yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_int32(
			boost::addressof(r), l));
}

// <
inline bool operator<(const bson_value_t& l, yggr::s32 r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_int32(
			boost::addressof(l), r) < 0;
}

inline bool operator<(yggr::s32 l, const bson_value_t& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_int32(
			boost::addressof(r), l) > 0;
}

// <=
inline bool operator<=(const bson_value_t& l, yggr::s32 r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_int32(
			boost::addressof(l), r) <= 0;
}

inline bool operator<=(yggr::s32 l, const bson_value_t& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_int32(
			boost::addressof(r), l) >= 0;
}

// >
inline bool operator>(const bson_value_t& l, yggr::s32 r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_int32(
			boost::addressof(l), r) > 0;
}

inline bool operator>(yggr::s32 l, const bson_value_t& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_int32(
			boost::addressof(r), l) < 0;
}

// >=
inline bool operator>=(const bson_value_t& l, yggr::s32 r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_int32(
			boost::addressof(l), r) >= 0;
}

inline bool operator>=(yggr::s32 l, const bson_value_t& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_int32(
			boost::addressof(r), l) <= 0;
}

// s64
// ==
inline bool operator==(const bson_value_t& l, yggr::s64 r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_int64(
			boost::addressof(l), r);
}

inline bool operator==(yggr::s64 l, const bson_value_t& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_int64(
			boost::addressof(r), l);
}

// !=
inline bool operator!=(const bson_value_t& l, yggr::s64 r)
{
	return
		!yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_int64(
			boost::addressof(l), r);
}

inline bool operator!=(yggr::s64 l, const bson_value_t& r)
{
	return
		!yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_int64(
			boost::addressof(r), l);
}

// <
inline bool operator<(const bson_value_t& l, yggr::s64 r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_int64(
			boost::addressof(l), r) < 0;
}

inline bool operator<(yggr::s64 l, const bson_value_t& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_int64(
			boost::addressof(r), l) > 0;
}

// <=
inline bool operator<=(const bson_value_t& l, yggr::s64 r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_int64(
			boost::addressof(l), r) <= 0;
}

inline bool operator<=(yggr::s64 l, const bson_value_t& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_int64(
			boost::addressof(r), l) >= 0;
}

// >
inline bool operator>(const bson_value_t& l, yggr::s64 r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_int64(
			boost::addressof(l), r) > 0;
}

inline bool operator>(yggr::s64 l, const bson_value_t& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_int64(
			boost::addressof(r), l) < 0;
}

// >=
inline bool operator>=(const bson_value_t& l, yggr::s64 r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_int64(
			boost::addressof(l), r) >= 0;
}

inline bool operator>=(yggr::s64 l, const bson_value_t& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_int64(
			boost::addressof(r), l) <= 0;
}

// bool
// ==
inline bool operator==(const bson_value_t& l, bool r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_bool(
			boost::addressof(l), r);
}

inline bool operator==(bool l, const bson_value_t& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_bool(
			boost::addressof(r), l);
}

// !=
inline bool operator!=(const bson_value_t& l, bool r)
{
	return
		!yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_bool(
			boost::addressof(l), r);
}

inline bool operator!=(bool l, const bson_value_t& r)
{
	return
		!yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_bool(
			boost::addressof(r), l);
}

// <
inline bool operator<(const bson_value_t& l, bool r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_bool(
			boost::addressof(l), r) < 0;
}

inline bool operator<(bool l, const bson_value_t& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_bool(
			boost::addressof(r), l) > 0;
}

// <=
inline bool operator<=(const bson_value_t& l, bool r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_bool(
			boost::addressof(l), r) <= 0;
}

inline bool operator<=(bool l, const bson_value_t& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_bool(
			boost::addressof(r), l) >= 0;
}

// >
inline bool operator>(const bson_value_t& l, bool r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_bool(
			boost::addressof(l), r) > 0;
}

inline bool operator>(bool l, const bson_value_t& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_bool(
			boost::addressof(r), l) < 0;
}

// >=
inline bool operator>=(const bson_value_t& l, bool r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_bool(
			boost::addressof(l), r) >= 0;
}

inline bool operator>=(bool l, const bson_value_t& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_bool(
			boost::addressof(r), l) <= 0;
}


// f32
// ==
inline bool operator==(const bson_value_t& l, yggr::f32 r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_double(
			boost::addressof(l), static_cast<yggr::f64>(r));
}

inline bool operator==(yggr::f32 l, const bson_value_t& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_double(
			boost::addressof(r), static_cast<yggr::f64>(l));
}

// !=
inline bool operator!=(const bson_value_t& l, yggr::f32 r)
{
	return
		!yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_double(
			boost::addressof(l), static_cast<yggr::f64>(r));
}

inline bool operator!=(yggr::f32 l, const bson_value_t& r)
{
	return
		!yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_double(
			boost::addressof(r), static_cast<yggr::f64>(l));
}

// <
inline bool operator<(const bson_value_t& l, yggr::f32 r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_double(
			boost::addressof(l), static_cast<yggr::f64>(r)) < 0;
}

inline bool operator<(yggr::f32 l, const bson_value_t& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_double(
			boost::addressof(r), static_cast<yggr::f64>(l)) > 0;
}

// <=
inline bool operator<=(const bson_value_t& l, yggr::f32 r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_double(
			boost::addressof(l), static_cast<yggr::f64>(r)) <= 0;
}

inline bool operator<=(yggr::f32 l, const bson_value_t& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_double(
			boost::addressof(r), static_cast<yggr::f64>(l)) >= 0;
}

// >
inline bool operator>(const bson_value_t& l, yggr::f32 r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_double(
			boost::addressof(l), static_cast<yggr::f64>(r)) > 0;
}

inline bool operator>(yggr::f32 l, const bson_value_t& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_double(
			boost::addressof(r), static_cast<yggr::f64>(l)) < 0;
}

// >=
inline bool operator>=(const bson_value_t& l, yggr::f32 r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_double(
			boost::addressof(l), static_cast<yggr::f64>(r)) >= 0;
}

inline bool operator>=(yggr::f32 l, const bson_value_t& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_double(
			boost::addressof(r), static_cast<yggr::f64>(l)) <= 0;
}

// f64
// ==
inline bool operator==(const bson_value_t& l, yggr::f64 r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_double(
			boost::addressof(l), r);
}

inline bool operator==(yggr::f64 l, const bson_value_t& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_double(
			boost::addressof(r), l);
}

// !=
inline bool operator!=(const bson_value_t& l, yggr::f64 r)
{
	return
		!yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_double(
			boost::addressof(l), r);
}

inline bool operator!=(yggr::f64 l, const bson_value_t& r)
{
	return
		!yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_double(
			boost::addressof(r), l);
}

// <
inline bool operator<(const bson_value_t& l, yggr::f64 r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_double(
			boost::addressof(l), r) < 0;
}

inline bool operator<(yggr::f64 l, const bson_value_t& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_double(
			boost::addressof(r), l) > 0;
}

// <=
inline bool operator<=(const bson_value_t& l, yggr::f64 r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_double(
			boost::addressof(l), r) <= 0;
}

inline bool operator<=(yggr::f64 l, const bson_value_t& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_double(
			boost::addressof(r), l) >= 0;
}

// >
inline bool operator>(const bson_value_t& l, yggr::f64 r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_double(
			boost::addressof(l), r) > 0;
}

inline bool operator>(yggr::f64 l, const bson_value_t& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_double(
			boost::addressof(r), l) < 0;
}

// >=
inline bool operator>=(const bson_value_t& l, yggr::f64 r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_double(
			boost::addressof(l), r) >= 0;
}

inline bool operator>=(yggr::f64 l, const bson_value_t& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_double(
			boost::addressof(r), l) <= 0;
}

// string
// ==
template<typename String> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			boost::mpl::not_
			<
				// fix boost 165 and later c2666 'yggr::nsql_database_system::operator ==' : 22 overloads have similar conversions
				boost::mpl::or_
				<
					boost::is_same<int, String>,
					yggr::is_enum<String>
				>
			>,
			yggr::charset::utf8_string_constructable<String>
		>,
		bool
	>::type
	operator==(const bson_value_t& l, const String& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_string(
			boost::addressof(l), r);
}

template<typename String> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			boost::mpl::not_
			<
				boost::mpl::or_
				<
					boost::is_same<int, String>,
					::yggr::is_enum<String>
				>
			>,
			yggr::charset::utf8_string_constructable<String>
		>,
		bool
	>::type
	operator==(const String& l, const bson_value_t& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_string(
			boost::addressof(r), l);
}

// !=
template<typename String> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			boost::mpl::not_
			<
				boost::mpl::or_
				<
					boost::is_same<int, String>,
					yggr::is_enum<String>
				>
			>,
			yggr::charset::utf8_string_constructable<String>
		>,
		bool
	>::type
	operator!=(const bson_value_t& l, const String& r)
{
	return
		!yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_string(
			boost::addressof(l), r);
}

template<typename String> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			boost::mpl::not_
			<
				boost::mpl::or_
				<
					boost::is_same<int, String>,
					::yggr::is_enum<String>
				>
			>,
			yggr::charset::utf8_string_constructable<String>
		>,
		bool
	>::type
	operator!=(const String& l, const bson_value_t& r)
{
	return
		!yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_string(
			boost::addressof(r), l);
}

// <
template<typename String> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			boost::mpl::not_
			<
				boost::mpl::or_
				<
					boost::is_same<int, String>,
					yggr::is_enum<String>
				>
			>,
			yggr::charset::utf8_string_constructable<String>
		>,
		bool
	>::type
	operator<(const bson_value_t& l, const String& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_string(
			boost::addressof(l), r) < 0;
}

template<typename String> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			boost::mpl::not_
			<
				boost::mpl::or_
				<
					boost::is_same<int, String>,
					::yggr::is_enum<String>
				>
			>,
			yggr::charset::utf8_string_constructable<String>
		>,
		bool
	>::type
	operator<(const String& l, const bson_value_t& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_string(
			boost::addressof(r), l) > 0;
}

// <=
template<typename String> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			boost::mpl::not_
			<
				boost::mpl::or_
				<
					boost::is_same<int, String>,
					yggr::is_enum<String>
				>
			>,
			yggr::charset::utf8_string_constructable<String>
		>,
		bool
	>::type
	operator<=(const bson_value_t& l, const String& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_string(
			boost::addressof(l), r) <= 0;
}

template<typename String> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			boost::mpl::not_
			<
				boost::mpl::or_
				<
					boost::is_same<int, String>,
					::yggr::is_enum<String>
				>
			>,
			yggr::charset::utf8_string_constructable<String>
		>,
		bool
	>::type
	operator<=(const String& l, const bson_value_t& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_string(
			boost::addressof(r), l) >= 0;
}

// >
template<typename String> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			boost::mpl::not_
			<
				boost::mpl::or_
				<
					boost::is_same<int, String>,
					yggr::is_enum<String>
				>
			>,
			yggr::charset::utf8_string_constructable<String>
		>,
		bool
	>::type
	operator>(const bson_value_t& l, const String& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_string(
			boost::addressof(l), r) > 0;
}

template<typename String> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			boost::mpl::not_
			<
				boost::mpl::or_
				<
					boost::is_same<int, String>,
					::yggr::is_enum<String>
				>
			>,
			yggr::charset::utf8_string_constructable<String>
		>,
		bool
	>::type
	operator>(const String& l, const bson_value_t& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_string(
			boost::addressof(r), l) < 0;
}

// >=
template<typename String> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			boost::mpl::not_
			<
				boost::mpl::or_
				<
					boost::is_same<int, String>,
					yggr::is_enum<String>
				>
			>,
			yggr::charset::utf8_string_constructable<String>
		>,
		bool
	>::type
	operator>=(const bson_value_t& l, const String& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_string(
			boost::addressof(l), r) >= 0;
}

template<typename String> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			boost::mpl::not_
			<
				boost::mpl::or_
				<
					boost::is_same<int, String>,
					::yggr::is_enum<String>
				>
			>,
			yggr::charset::utf8_string_constructable<String>
		>,
		bool
	>::type
	operator>=(const String& l, const bson_value_t& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_string(
			boost::addressof(r), l) <= 0;
}

// null
// undefined
// maxkey
// minkey
// date
// time
// timeval
// oid_org_type
// oid
// binary_buffer
// binary_buffer_ref
// regex
// code
// symbol
// code_w_scope
// timestamp
// decimal128_org_type
// decimal128
// dbpointer

// bson and array
// ==
template<typename Bson> inline
typename boost::enable_if< boost::is_base_of< bson_t, Bson>, bool>::type
	operator==(const bson_value_t& l, const Bson& r)
{
	return yggr::nsql_database_system::bson_native_ex::s_bson_is_array(&r)?
			yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_array(&l, &r)
			: yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_bson(&l, &r);
}

// !=
template<typename Bson> inline
typename boost::enable_if< boost::is_base_of< bson_t, Bson>, bool>::type
	operator!=(const bson_value_t& l, const Bson& r)
{
	return !(yggr::nsql_database_system::bson_native_ex::s_bson_is_array(&r)?
				yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_array(&l, &r)
				: yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_bson(&l, &r));
}


// bson_value
// ==
template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator==(const bson_value_t& l, const BsonValue& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_value(
			boost::addressof(l), boost::addressof(r));
}

// !=
template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator!=(const bson_value_t& l, const BsonValue& r)
{
	return
		!yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_value(
			boost::addressof(l), boost::addressof(r));
}

// <
template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator<(const bson_value_t& l, const BsonValue& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_value(
			boost::addressof(l), boost::addressof(r)) < 0;
}

// <=
template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator<=(const bson_value_t& l, const BsonValue& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_value(
			boost::addressof(l), boost::addressof(r)) <= 0;
}

// >
template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator>(const bson_value_t& l, const BsonValue& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_value(
			boost::addressof(l), boost::addressof(r)) > 0;
}

// >=
template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator>=(const bson_value_t& l, const BsonValue& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_value(
			boost::addressof(l), boost::addressof(r)) >= 0;
}

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{

#if defined(__GNUC__) && (YGGR_CPP_VERSION < YGGR_CPP_VER_11)

inline void swap(c_bson_value& l, bson_value_t& r)
{
	l.swap(r);
}

#else

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, void>::type
	swap(c_bson_value& l, BsonValue& r)
{
	l.swap(r);
}

#endif // #if defined(__GNUC__) && (YGGR_CPP_VERSION < YGGR_CPP_VER_11)

inline void swap(c_bson_value& l, c_bson_value& r)
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


#endif // __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_VALUE_HPP__
