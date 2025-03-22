//bson_value_native_ex.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_BSON_VALUE_NATIVE_EX_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_BSON_VALUE_NATIVE_EX_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/static_assert.hpp>

#include <yggr/charset/const_strings.hpp>
#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/utf8_string_view.hpp>

#include <yggr/container/resize.hpp>
#include <yggr/container/get_allocator.hpp>

#include <yggr/range_ex/range_value_ex.hpp>
#include <yggr/range_ex/range_iterator_ex.hpp>

#include <yggr/iterator_ex/iterator_category_check.hpp>

#include <yggr/nsql_database_system/bson_config.hpp>
#include <yggr/nsql_database_system/bson_typeid_def.hpp>

#include <yggr/nsql_database_system/native_ex_decl_bson.hpp>

#include <yggr/nsql_database_system/bson_oid_native_ex.hpp>
#include <yggr/nsql_database_system/bson_regex_native_ex.hpp>
#include <yggr/nsql_database_system/bson_date_time_native_ex.hpp>
#include <yggr/nsql_database_system/bson_error_native_ex.hpp>
#include <yggr/nsql_database_system/bson_native_ex.hpp>

#include <yggr/nsql_database_system/json_escape_string.hpp>
#include <yggr/nsql_database_system/bson_visit_t.hpp>

#include <yggr/nsql_database_system/detail/bson_value_conflict_fixer.hpp>

#include <boost/ref.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/size_t.hpp>

#include <boost/type_traits/is_const.hpp>
#include <boost/preprocessor/cat.hpp>

#include <iomanip>
#include <base64.h>

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

class bson_value_native_ex
{
private:
	typedef yggr::utf8_string inner_string_type;
	typedef yggr::utf8_string_view inner_string_view_type;
	typedef bson_value_native_ex this_type;

public:
	static void s_bson_value_clear(bson_value_t* pval);
	static bool s_bson_value_swap(bson_value_t* l, bson_value_t* r);
	static bool s_bson_value_copy(bson_value_t* pdst, const bson_value_t* psrc);

public:
	// to_value
	static bool s_int32_to_value(bson_value_t* pdst, s32 val);
	static bool s_int64_to_value(bson_value_t* pdst, s64 val);
	static bool s_bool_to_value(bson_value_t* pdst, bool val);
	static bool s_double_to_value(bson_value_t* pdst, f64 val);

	template<typename String> inline
	static typename boost::enable_if< charset::utf8_string_constructable<String>, bool >::type
		s_string_to_value(bson_value_t* pdst, const String& val)
	{
		typedef String now_string_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, conv_string_type);

		assert(pdst);
		if(!(pdst))
		{
			return false;
		}

		this_type::s_bson_value_clear(pdst);

		pdst->value_type = static_cast<bson_type_t>(bson_typeid_def::E_BSON_TYPE_UTF8);

		conv_string_type str((charset::string_charset_helper_data(val)));
		uint32_t len = str.size();
		pdst->value.v_utf8.str = reinterpret_cast<char*>(bson_malloc(len + 1));
		if(len) memcpy(pdst->value.v_utf8.str, str.data(), static_cast<std::size_t>(len));
		pdst->value.v_utf8.str[len] = 0;
		pdst->value.v_utf8.len = len;

		return true;
	}

	static bool s_null_to_value(bson_value_t* pdst);
	static bool s_undefined_to_value(bson_value_t* pdst);

	static bool s_maxkey_to_value(bson_value_t* pdst);
	static bool s_minkey_to_value(bson_value_t* pdst);

	static bool s_date_to_value(bson_value_t* pdst, s64 val);
	static bool s_time_to_value(bson_value_t* pdst, s64 val);
	static bool s_timeval_to_value(bson_value_t* pdst, const timeval* val);
	static bool s_timestamp_to_value(bson_value_t* pdst, u32 tm, u32 inc);

	static bool s_oid_to_value(bson_value_t* pdst, const bson_oid_t* val);

	static bool s_binary_buffer_to_value(bson_value_t* pdst, u32 sub_t, const u8* val, u64 size);

	template<typename String1, typename String2> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_regex_to_value(bson_value_t* pdst, const String1& val_regex, const String2& val_opts)
	{
		typedef String1 regex_string_type;
		typedef String2 opts_string_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(regex_string_type, conv_regex_string_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(opts_string_type, conv_opts_string_type);

		assert(pdst);
		if(!pdst)
		{
			return false;
		}

		//if(pdst->value_type)
		//{
		//	bson_value_destroy(pdst);
		//}
		//memset(pdst, 0, sizeof(bson_value_t));

		this_type::s_bson_value_clear(pdst);

		pdst->value_type = static_cast<bson_type_t>(bson_typeid_def::E_BSON_TYPE_REGEX);
		{
			conv_regex_string_type str((charset::string_charset_helper_data(val_regex)));
			u64 len = str.size();
			pdst->value.v_regex.regex = reinterpret_cast<char*>(bson_malloc(static_cast<std::size_t>(len + 1)));
			if(len) memcpy(pdst->value.v_regex.regex, str.data(), static_cast<std::size_t>(len));
			pdst->value.v_regex.regex[len] = 0;
		}

		{
			char options_buffer[bson_regex_native_ex::E_BSON_REGEX_OPTIONS_STRING_BUFFER_SIZE] = {0};
			conv_opts_string_type str((charset::string_charset_helper_data(val_opts)));
			uint32_t len = static_cast<uint32_t>(bson_regex_native_ex::s_bson_regex_options_sorted(options_buffer, str.data(), str.size()));
			pdst->value.v_regex.options = reinterpret_cast<char*>(bson_malloc(len + 1));
			if(len) memcpy(pdst->value.v_regex.options, options_buffer, static_cast<std::size_t>(len));
			pdst->value.v_regex.options[len] = 0;
		}

		return true;
	}

	template<typename String> inline
	static typename boost::enable_if< charset::utf8_string_constructable<String>, bool >::type
		s_code_to_value(bson_value_t* pdst, const String& val)
	{
		typedef String now_string_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, conv_string_type);

		assert(pdst);
		if(!(pdst))
		{
			return false;
		}

		this_type::s_bson_value_clear(pdst);

		pdst->value_type = static_cast<bson_type_t>(bson_typeid_def::E_BSON_TYPE_CODE);

		conv_string_type str((charset::string_charset_helper_data(val)));
		uint32_t len = static_cast<uint32_t>(str.size());
		pdst->value.v_code.code = reinterpret_cast<char*>(bson_malloc(len + 1));
		if(len) memcpy(pdst->value.v_code.code, str.data(), static_cast<std::size_t>(len));
		pdst->value.v_code.code[len] = 0;
		pdst->value.v_code.code_len = len;

		return true;
	}

	template<typename String> inline
	static typename boost::enable_if< charset::utf8_string_constructable<String>, bool >::type
		s_symbol_to_value(bson_value_t* pdst, const String& val)
	{
		typedef String now_string_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, conv_string_type);

		assert(pdst);
		if(!(pdst))
		{
			return false;
		}

		this_type::s_bson_value_clear(pdst);

		pdst->value_type = static_cast<bson_type_t>(bson_typeid_def::E_BSON_TYPE_SYMBOL);

		conv_string_type str((charset::string_charset_helper_data(val)));
		uint32_t len = static_cast<uint32_t>(str.size());
		pdst->value.v_symbol.symbol = reinterpret_cast<char*>(bson_malloc(len + 1));
		if(len) memcpy(pdst->value.v_symbol.symbol, str.data(), static_cast<std::size_t>(len));
		pdst->value.v_symbol.symbol[len] = 0;
		pdst->value.v_symbol.len = len;

		return true;
	}

	template<typename String> inline
	static typename boost::enable_if< charset::utf8_string_constructable<String>, bool >::type
		s_code_w_scope_to_value(bson_value_t* pdst, const String& val, const bson_t* scope)
	{
		typedef String now_string_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, conv_string_type);

		assert(pdst && scope && scope->len);
		if(!(pdst && scope && scope->len))
		{
			return false;
		}

		this_type::s_bson_value_clear(pdst);

		pdst->value_type = static_cast<bson_type_t>(bson_typeid_def::E_BSON_TYPE_CODEWSCOPE);

		conv_string_type str((charset::string_charset_helper_data(val)));
		uint32_t len = str.size();
		pdst->value.v_codewscope.code = reinterpret_cast<char*>(bson_malloc(len + 1));
		if(len) memcpy(pdst->value.v_codewscope.code, str.data(), static_cast<std::size_t>(len));
		pdst->value.v_codewscope.code[len] = 0;
		pdst->value.v_codewscope.code_len = len;

		pdst->value.v_codewscope.scope_data = reinterpret_cast<uint8_t*>(bson_malloc(scope->len));
		memcpy(pdst->value.v_codewscope.scope_data, bson_native_ex::s_bson_data(scope), static_cast<std::size_t>(scope->len));
		pdst->value.v_codewscope.scope_len = scope->len;

		return true;
	}

	template<typename String> inline
	static typename boost::enable_if< charset::utf8_string_constructable<String>, bool >::type
		s_dbpointer_to_value(bson_value_t* pdst, const String& val, const bson_oid_t* oid)
	{
		typedef String now_string_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, conv_string_type);

		assert(pdst && oid);
		if(!(pdst && oid))
		{
			return false;
		}

		this_type::s_bson_value_clear(pdst);

		pdst->value_type = static_cast<bson_type_t>(bson_typeid_def::E_BSON_TYPE_DBPOINTER);

		conv_string_type str((charset::string_charset_helper_data(val)));
		uint32_t len = str.size();
		pdst->value.v_dbpointer.collection = reinterpret_cast<char*>(bson_malloc(len + 1));
		if(len) memcpy(pdst->value.v_dbpointer.collection, str.data(), static_cast<std::size_t>(len));
		pdst->value.v_dbpointer.collection[len] = 0;
		pdst->value.v_dbpointer.collection_len = len;

		memcpy(&(pdst->value.v_dbpointer.oid), oid, bson_oid_native_ex::E_bson_oid_byte_size);

		return true;
	}

	static bool s_bson_to_value(bson_value_t* pdst, const bson_t* pval);
	static bool s_array_to_value(bson_value_t* pdst, const bson_t* pval);

	static bool s_decimal128_to_value(bson_value_t* pdst, const bson_decimal128_t* pval);

	//---------------------------------------
	// value_to

	static bool s_value_to_int32(s32& val, const bson_value_t* psrc);
	static bool s_value_to_int64(s64& val, const bson_value_t* psrc);
	static bool s_value_to_bool(bool& val, const bson_value_t* psrc);
	static bool s_value_to_double(f64& val, const bson_value_t* psrc);

//	template<typename String> inline
//	static typename boost::enable_if< charset::utf8_string_loadable<String>, bool >::type
//		s_value_to_string(String& val, const bson_value_t* psrc)
//	{
//		typedef typename charset::string_charset_helper_first_t<String>::type out_string_type;
//		YGGR_TYPEDEF_UTF8_STRING_TYPE_TPL(out_string_type, mid_string_type);
//
//		assert(psrc);
//		if(!psrc)
//		{
//			return false;
//		}
//
//		if(static_cast<u32>(psrc->value_type) != bson_typeid_def::E_BSON_TYPE_UTF8)
//		{
//			return false;
//		}
//
//		/*
//		!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//		!!! why using YGGR_AUTO,																		!!!
//		!!! because:																					!!!
//		!!! in msvc if using "mid_string_type mid_str(charset::string_charset_helper_data(val));",		!!!
//		!!! msvc has a bug can't deduce the "charset::string_charset_helper_data(val)" 's result type!	!!!
//		!!! [fight bug with bug] -_-!!! (in msvc BOOST_AUTO is a bug skill)								!!!
//		!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//		*/
//
//#ifdef _MSC_VER
//		YGGR_AUTO_TPL(mid_data, charset::string_charset_helper_data(val));
//		mid_string_type mid_str(mid_data);
//#else
//		mid_string_type mid_str(charset::string_charset_helper_data(val));
//#endif // _MSC_VER
//
//		mid_str.assign(psrc->value.v_utf8.str,
//						psrc->value.v_utf8.str + psrc->value.v_utf8.len,
//						YGGR_STR_UTF8_STRING_CHARSET_NAME());
//
//		charset::string_charset_helper_first(val)
//			= mid_str.template str<out_string_type>(
//				container::get_allocator(charset::string_charset_helper_first(val)),
//				charset::string_charset_helper_second(val));
//
//		return true;
//	}

	template<typename String> inline
	static typename boost::enable_if< charset::utf8_string_loadable<String>, bool >::type
		s_value_to_string(String& val, const bson_value_t* psrc)
	{
		typedef typename charset::string_charset_helper_first_t<String>::type out_string_type;

		assert(psrc);
		if(!((psrc) && (static_cast<u32>(psrc->value_type) == bson_typeid_def::E_BSON_TYPE_UTF8)))
		{
			return false;
		}

		inner_string_view_type mid_str(psrc->value.v_utf8.str, psrc->value.v_utf8.len);

		charset::string_charset_helper_first(val)
			= mid_str.str<out_string_type>(
				container::get_allocator(charset::string_charset_helper_first(val)),
				charset::string_charset_helper_second(val));

		return true;
	}

	template<typename String> inline
	static typename boost::enable_if< charset::utf8_string_view_loadable<String>, bool >::type
		s_value_to_string(String& val, const bson_value_t* psrc)
	{
		typedef typename charset::string_charset_helper_first_t<String>::type out_string_view_type;

		assert(psrc);
		if(!((psrc)
			&& (static_cast<u32>(psrc->value_type) == bson_typeid_def::E_BSON_TYPE_UTF8)
			&& (charset::string_charset_helper_second(val) == YGGR_STR_UTF8_STRING_CHARSET_NAME()) ) )
		{
			return false;
		}

		val = out_string_view_type(psrc->value.v_utf8.str, psrc->value.v_utf8.len);
		return true;
	}

	//template<typename Utf8StringView> inline
	//static typename boost::enable_if< charset::utf8_string_view_loadable<Utf8StringView>, bool >::type
	//	s_value_to_utf8_string_view(Utf8StringView& val, const bson_value_t* psrc)
	//{
	//	typedef typename charset::string_charset_helper_first_t<Utf8StringView>::type out_string_view_type;

	//	assert(psrc);
	//	if((!psrc)
	//		|| (static_cast<u32>(psrc->value_type) != bson_typeid_def::E_BSON_TYPE_UTF8)
	//		|| (charset::string_charset_helper_second(val) != YGGR_STR_UTF8_STRING_CHARSET_NAME()) )
	//	{
	//		return false;
	//	}

	//	val = out_string_view_type(psrc->value.v_utf8.str, psrc->value.v_utf8.len);
	//	return true;
	//}

	static bool s_value_to_null(const bson_value_t* psrc);
	static bool s_value_to_undefined(const bson_value_t* psrc);

	static bool s_value_to_maxkey(const bson_value_t* psrc);
	static bool s_value_to_minkey(const bson_value_t* psrc);

	static bool s_value_to_date(s64& val, const bson_value_t* psrc);
	static bool s_value_to_time(s64& val, const bson_value_t* psrc);

	static bool s_value_to_timeval(timeval* pval, const bson_value_t* psrc);
	static bool s_value_to_oid(bson_oid_t* pval, const bson_value_t* psrc);

	template<typename Buffer> inline
	static bool s_value_to_binary_buffer(u32& sub_t, Buffer& buf, const bson_value_t* psrc)
	{
		typedef Buffer buf_type;
		typedef typename range_ex::range_value_ex<buf_type>::type buf_val_type;
		typedef typename range_ex::range_iterator_ex<buf_type>::type iter_type;
		BOOST_MPL_ASSERT((is_random_access_iterator<iter_type>));
		BOOST_MPL_ASSERT((boost::mpl::bool_<sizeof(buf_val_type) == 1>));

		assert(psrc);

		if(!((psrc && static_cast<u32>(psrc->value_type) == bson_typeid_def::E_BSON_TYPE_BINARY)
				&& (psrc->value.v_binary.data && psrc->value.v_binary.data_len)))
		{
			return false;
		}

		sub_t = 0;
		yggr::resize(buf, psrc->value.v_binary.data_len);
		memcpy(&(*boost::begin(buf)), psrc->value.v_binary.data, static_cast<std::size_t>(psrc->value.v_binary.data_len));
		sub_t = psrc->value.v_binary.subtype;
		return true;
	}

	template<typename Buffer> inline
	static bool s_value_to_binary_buffer(Buffer& buf, const bson_value_t* psrc, u32 chk_sub_t)
	{
		typedef Buffer buf_type;
		typedef typename range_ex::range_value_ex<buf_type>::type buf_val_type;
		typedef typename range_ex::range_iterator_ex<buf_type>::type iter_type;
		BOOST_MPL_ASSERT((is_random_access_iterator<iter_type>));
		BOOST_MPL_ASSERT((boost::mpl::bool_<sizeof(buf_val_type) == 1>));

		assert(psrc);

		if(!((psrc && static_cast<u32>(psrc->value_type) == bson_typeid_def::E_BSON_TYPE_BINARY)
				&& (chk_sub_t == psrc->value.v_binary.subtype)
				&& (psrc->value.v_binary.data && psrc->value.v_binary.data_len)))
		{
			return false;
		}

		yggr::resize(buf, psrc->value.v_binary.data_len);
		memcpy(&(*boost::begin(buf)), psrc->value.v_binary.data, static_cast<std::size_t>(psrc->value.v_binary.data_len));
		return true;
	}

	template<typename String1, typename String2> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_loadable< String1 >,
				charset::utf8_string_loadable< String2 >
			>,
			bool
		>::type
		s_value_to_regex(String1& val_regex, String2& val_opts, const bson_value_t* psrc)
	{
		typedef typename charset::string_charset_helper_first_t<String1>::type regex_string_type;
		typedef typename charset::string_charset_helper_first_t<String2>::type opts_string_type;

		assert(psrc);
		if(!(psrc && static_cast<u32>(psrc->value_type) == bson_typeid_def::E_BSON_TYPE_REGEX))
		{
			return false;
		}

		// regex
		inner_string_view_type mid_regex_str(psrc->value.v_regex.regex);

		charset::string_charset_helper_first(val_regex)
			= mid_regex_str.str<regex_string_type>(
				container::get_allocator(charset::string_charset_helper_first(val_regex)),
				charset::string_charset_helper_second(val_regex));

		// opts
		char options_buffer[bson_regex_native_ex::E_BSON_REGEX_OPTIONS_STRING_BUFFER_SIZE] = {0};
		std::size_t opts_size = bson_regex_native_ex::s_bson_regex_options_sorted(options_buffer, psrc->value.v_regex.options);

		inner_string_view_type mid_opts_str(options_buffer, opts_size);

		charset::string_charset_helper_first(val_opts)
			= mid_opts_str.str<opts_string_type>(
				container::get_allocator(charset::string_charset_helper_first(val_opts)),
				charset::string_charset_helper_second(val_opts));

		return true;
	}

	template<typename String1, typename String2> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_view_loadable< String1 >,
				charset::utf8_string_loadable< String2 >
			>,
			bool
		>::type
		s_value_to_regex(String1& val_regex, String2& val_opts, const bson_value_t* psrc)
	{
		typedef typename charset::string_charset_helper_first_t<String1>::type regex_string_type;
		typedef typename charset::string_charset_helper_first_t<String2>::type opts_string_type;

		assert(psrc);
		if(!(psrc && static_cast<u32>(psrc->value_type) == bson_typeid_def::E_BSON_TYPE_REGEX))
		{
			return false;
		}

		// regex
		charset::string_charset_helper_first(val_regex) = regex_string_type(psrc->value.v_regex.regex);

		// opts
		char options_buffer[bson_regex_native_ex::E_BSON_REGEX_OPTIONS_STRING_BUFFER_SIZE] = {0};
		std::size_t opts_size = bson_regex_native_ex::s_bson_regex_options_sorted(options_buffer, psrc->value.v_regex.options);

		inner_string_view_type mid_opts_str(options_buffer, opts_size);

		charset::string_charset_helper_first(val_opts)
			= mid_opts_str.str<opts_string_type>(
				container::get_allocator(charset::string_charset_helper_first(val_opts)),
				charset::string_charset_helper_second(val_opts));

		return true;
	}

	// code
//	template<typename String> inline
//	static typename boost::enable_if< charset::utf8_string_loadable<String>, bool >::type
//		s_value_to_code(String& val, const bson_value_t* psrc)
//	{
//		typedef typename charset::string_charset_helper_first_t<String>::type out_string_type;
//		YGGR_TYPEDEF_UTF8_STRING_TYPE_TPL(out_string_type, mid_string_type);
//
//		assert(psrc);
//		if(!(psrc && static_cast<u32>(psrc->value_type) == bson_typeid_def::E_BSON_TYPE_CODE))
//		{
//			return false;
//		}
//
//#ifdef _MSC_VER
//		YGGR_AUTO_TPL(mid_data, charset::string_charset_helper_data(val));
//		mid_string_type mid_str(mid_data);
//#else
//		mid_string_type mid_str(charset::string_charset_helper_data(val));
//#endif // _MSC_VER
//		mid_str.assign(psrc->value.v_code.code,
//						psrc->value.v_code.code + psrc->value.v_code.code_len,
//						YGGR_STR_UTF8_STRING_CHARSET_NAME());
//
//		charset::string_charset_helper_first(val)
//			= mid_str.template str<out_string_type>(
//				container::get_allocator(charset::string_charset_helper_first(val)),
//				charset::string_charset_helper_second(val));
//
//		return true;
//	}

	template<typename String> inline
	static typename boost::enable_if< charset::utf8_string_loadable<String>, bool >::type
		s_value_to_code(String& val, const bson_value_t* psrc)
	{
		typedef typename charset::string_charset_helper_first_t<String>::type out_string_type;

		assert(psrc);
		if(!(psrc && static_cast<u32>(psrc->value_type) == bson_typeid_def::E_BSON_TYPE_CODE))
		{
			return false;
		}

		inner_string_view_type mid_str(psrc->value.v_code.code, psrc->value.v_code.code_len);

		charset::string_charset_helper_first(val)
			= mid_str.str<out_string_type>(
				container::get_allocator(charset::string_charset_helper_first(val)),
				charset::string_charset_helper_second(val));

		return true;
	}

	template<typename String> inline
	static typename boost::enable_if< charset::utf8_string_view_loadable<String>, bool >::type
		s_value_to_code(String& val, const bson_value_t* psrc)
	{
		typedef typename charset::string_charset_helper_first_t<String>::type out_string_view_type;

		assert(psrc);
		if(!(psrc && static_cast<u32>(psrc->value_type) == bson_typeid_def::E_BSON_TYPE_CODE))
		{
			return false;
		}

		charset::string_charset_helper_first(val) =
			out_string_view_type(psrc->value.v_code.code, psrc->value.v_code.code_len);

		return true;
	}

//	template<typename String> inline
//	static typename boost::enable_if< charset::utf8_string_loadable<String>, bool >::type
//		s_value_to_symbol(String& val, const bson_value_t* psrc)
//	{
//		typedef typename charset::string_charset_helper_first_t<String>::type out_string_type;
//		YGGR_TYPEDEF_UTF8_STRING_TYPE_TPL(out_string_type, mid_string_type);
//
//		assert(psrc);
//		if(!(psrc && static_cast<u32>(psrc->value_type) == bson_typeid_def::E_BSON_TYPE_SYMBOL))
//		{
//			return false;
//		}
//
//#ifdef _MSC_VER
//		YGGR_AUTO_TPL(mid_data, charset::string_charset_helper_data(val));
//		mid_string_type mid_str(mid_data);
//#else
//		mid_string_type mid_str(charset::string_charset_helper_data(val));
//#endif // _MSC_VER
//		mid_str.assign(psrc->value.v_symbol.symbol,
//						psrc->value.v_symbol.symbol + psrc->value.v_symbol.len,
//						YGGR_STR_UTF8_STRING_CHARSET_NAME());
//
//		charset::string_charset_helper_first(val)
//			= mid_str.template str<out_string_type>(
//				container::get_allocator(charset::string_charset_helper_first(val)),
//				charset::string_charset_helper_second(val));
//
//		return true;
//	}

	template<typename String> inline
	static typename boost::enable_if< charset::utf8_string_loadable<String>, bool >::type
		s_value_to_symbol(String& val, const bson_value_t* psrc)
	{
		typedef typename charset::string_charset_helper_first_t<String>::type out_string_type;

		assert(psrc);
		if(!(psrc && static_cast<u32>(psrc->value_type) == bson_typeid_def::E_BSON_TYPE_SYMBOL))
		{
			return false;
		}

		inner_string_view_type mid_str(psrc->value.v_symbol.symbol, psrc->value.v_symbol.len);

		charset::string_charset_helper_first(val)
			= mid_str.str<out_string_type>(
				container::get_allocator(charset::string_charset_helper_first(val)),
				charset::string_charset_helper_second(val));

		return true;
	}

	template<typename String> inline
	static typename boost::enable_if< charset::utf8_string_view_loadable<String>, bool >::type
		s_value_to_symbol(String& val, const bson_value_t* psrc)
	{
		typedef typename charset::string_charset_helper_first_t<String>::type out_string_view_type;

		assert(psrc);
		if(!(psrc && static_cast<u32>(psrc->value_type) == bson_typeid_def::E_BSON_TYPE_SYMBOL))
		{
			return false;
		}

		charset::string_charset_helper_first(val) =
			out_string_view_type(psrc->value.v_symbol.symbol, psrc->value.v_symbol.len);

		return true;
	}

	template<typename String> inline
	static typename boost::enable_if< charset::utf8_string_loadable<String>, bool >::type
		s_value_to_code_w_scope(String& val_code, bson_t* pval_scope, const bson_value_t* psrc)
	{
		typedef typename charset::string_charset_helper_first_t<String>::type code_string_type;

		assert(psrc && pval_scope);

		if(!(psrc && pval_scope
				&& static_cast<u32>(psrc->value_type) == bson_typeid_def::E_BSON_TYPE_CODEWSCOPE))
		{
			return false;
		}

		inner_string_view_type mid_str(psrc->value.v_codewscope.code, psrc->value.v_codewscope.code_len);

		charset::string_charset_helper_first(val_code)
			= mid_str.template str<code_string_type>(
				container::get_allocator(charset::string_charset_helper_first(val_code)),
				charset::string_charset_helper_second(val_code));

		bson_native_ex::s_bson_init_from_data(
			pval_scope,
			psrc->value.v_codewscope.scope_data,
			psrc->value.v_codewscope.scope_len);
		return true;
	}

	template<typename String> inline
	static typename boost::enable_if< charset::utf8_string_view_loadable<String>, bool >::type
		s_value_to_code_w_scope(String& val_code, bson_t* pval_scope, const bson_value_t* psrc)
	{
		typedef typename charset::string_charset_helper_first_t<String>::type code_string_view_type;

		assert(psrc && pval_scope);

		if(!(psrc && pval_scope
				&& static_cast<u32>(psrc->value_type) == bson_typeid_def::E_BSON_TYPE_CODEWSCOPE))
		{
			return false;
		}


		charset::string_charset_helper_first(val_code)
			= code_string_view_type(psrc->value.v_codewscope.code, psrc->value.v_codewscope.code_len);

		bson_native_ex::s_bson_init_from_data(
			pval_scope,
			psrc->value.v_codewscope.scope_data,
			psrc->value.v_codewscope.scope_len);
		return true;
	}

	static bool s_value_to_timestamp(u32& time, u32& increment, const bson_value_t* psrc);

	template<typename String> inline
	static typename boost::enable_if< charset::utf8_string_loadable<String>, bool >::type
		s_value_to_dbpointer(String& val_col, bson_oid_t* pval_oid, const bson_value_t* psrc)
	{
		typedef typename charset::string_charset_helper_first_t<String>::type col_string_type;

		assert(psrc && pval_oid);
		if(!(psrc && pval_oid
				&& static_cast<u32>(psrc->value_type) == bson_typeid_def::E_BSON_TYPE_DBPOINTER))
		{
			return false;
		}

		inner_string_view_type mid_str(psrc->value.v_dbpointer.collection,
										psrc->value.v_dbpointer.collection_len);

		charset::string_charset_helper_first(val_col)
			= mid_str.template str<col_string_type>(
				container::get_allocator(charset::string_charset_helper_first(val_col)),
				charset::string_charset_helper_second(val_col));

		memcpy(pval_oid, &(psrc->value.v_dbpointer.oid), bson_oid_native_ex::E_bson_oid_byte_size);
		return true;
	}

	template<typename String> inline
	static typename boost::enable_if< charset::utf8_string_view_loadable<String>, bool >::type
		s_value_to_dbpointer(String& val_col, bson_oid_t* pval_oid, const bson_value_t* psrc)
	{
		typedef typename charset::string_charset_helper_first_t<String>::type col_string_view_type;

		assert(psrc && pval_oid);
		if(!(psrc && pval_oid
				&& static_cast<u32>(psrc->value_type) == bson_typeid_def::E_BSON_TYPE_DBPOINTER))
		{
			return false;
		}

		charset::string_charset_helper_first(val_col) =
			col_string_view_type(
				psrc->value.v_dbpointer.collection,
				psrc->value.v_dbpointer.collection_len);

		memcpy(pval_oid, &(psrc->value.v_dbpointer.oid), bson_oid_native_ex::E_bson_oid_byte_size);
		return true;
	}

	static bool s_value_to_bson(bson_t* pval, const bson_value_t* psrc);
	static bool s_value_to_array(bson_t* pval, const bson_value_t* psrc);
	static bool s_value_to_bson_or_array(bson_t* pval, const bson_value_t* psrc);

	static bool s_value_to_decimal128(bson_decimal128_t* pval, const bson_value_t* psrc);

	//-----------------------------
	// comparer

	static bool s_value_cmp_eq_bool(const bson_value_t* l, bool r);
	static s32 s_value_cmp_bool(const bson_value_t* l, bool r);

	static bool s_value_cmp_eq_int32(const bson_value_t* l, s32 r);
	static s32 s_value_cmp_int32(const bson_value_t* l, s32 r);

	static bool s_value_cmp_eq_int64(const bson_value_t* l, s64 r);
	static s32 s_value_cmp_int64(const bson_value_t* l, s64 r);

	static bool s_value_cmp_eq_double(const bson_value_t* l, f64 r);
	static s32 s_value_cmp_double(const bson_value_t* l, f64 r);

	template<typename String> inline
	static typename boost::enable_if< charset::utf8_string_constructable<String>, bool >::type
		s_value_cmp_eq_string(const bson_value_t* l, const String& r)
	{
		typedef String now_string_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, conv_string_type);

		assert(l);
		conv_string_type str((charset::string_charset_helper_data(r)));

		return
			(l)
			&& (static_cast<u32>(l->value_type) == bson_typeid_def::E_BSON_TYPE_UTF8)
			&& (l->value.v_utf8.str)
			&& (l->value.v_utf8.len == str.size())
			&& (0 == memcmp(l->value.v_utf8.str, str.data(), l->value.v_utf8.len));
	}

	template<typename String> inline
	static typename boost::enable_if< charset::utf8_string_constructable<String>, s32 >::type
		s_value_cmp_string(const bson_value_t* l, const String& r)
	{
		typedef String now_string_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, conv_string_type);

		assert(
			l
			&& (static_cast<u32>(l->value_type) == bson_typeid_def::E_BSON_TYPE_UTF8)
			&& (l->value.v_utf8.str));

		conv_string_type str((charset::string_charset_helper_data(r)));

		if(!(static_cast<u32>(l->value_type) == bson_typeid_def::E_BSON_TYPE_UTF8))
		{
			bson_error_t bs_err;
			this_type::pro_s_make_bson_validate_diff_t_err(bs_err, l->value_type, bson_typeid_def::E_BSON_TYPE_UTF8);
			throw bs_err;
		}

		return charset::utf8_foo::strcmp(l->value.v_utf8.str, str.data(), l->value.v_utf8.len, str.size());
	}

	static bool s_value_cmp_eq_null(const bson_value_t* l);
	static bool s_value_cmp_eq_undefined(const bson_value_t* l);
	static bool s_value_cmp_eq_maxkey(const bson_value_t* l);
	static bool s_value_cmp_eq_minkey(const bson_value_t* l);

	static bool s_value_cmp_eq_date(const bson_value_t* l, s64 r);
	static s32 s_value_cmp_date(const bson_value_t* l, s64 r);

	static bool s_value_cmp_eq_time(const bson_value_t* l, s64 r);
	static s32 s_value_cmp_time(const bson_value_t* l, s64 r);

	static bool s_value_cmp_eq_timeval(const bson_value_t* l, const timeval* r);
	static s32 s_value_cmp_timeval(const bson_value_t* l, const timeval* r);

	inline static bool s_value_cmp_eq_oid(const bson_value_t* l, const bson_oid_t* r)
	{
		return detail::bson_value_conflict_fixer::s_value_cmp_eq_oid(l, r);
	}

	inline static s32 s_value_cmp_oid(const bson_value_t* l, const bson_oid_t* r)
	{
		return detail::bson_value_conflict_fixer::s_value_cmp_oid(l, r);
	}

	static bool s_value_cmp_eq_binary_buffer(const bson_value_t* l, u32 sub_t, const u8* r, u64 size);

	template<typename String1, typename String2> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable< String1 >,
				charset::utf8_string_constructable< String2 >
			>,
			bool
		>::type
		s_value_cmp_eq_regex(const bson_value_t* l, const String1& r_regex, const String2& r_opts)
	{
		typedef String1 regex_string_type;
		typedef String2 opts_string_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(regex_string_type, conv_regex_string_type); // strcmp not support string_view size;
		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(opts_string_type, conv_opts_string_type);

		assert(l);

		conv_regex_string_type str_regex((charset::string_charset_helper_data(r_regex)));
		conv_opts_string_type str_opts((charset::string_charset_helper_data(r_opts)));

		char options_buffer[bson_regex_native_ex::E_BSON_REGEX_OPTIONS_STRING_BUFFER_SIZE] = {0};
		//std::size_t opts_size = bson_regex_native_ex::s_bson_regex_options_sorted(options_buffer, str_opts.data(), str_opts.size());
		bson_regex_native_ex::s_bson_regex_options_sorted(options_buffer, str_opts.data(), str_opts.size());

		return
			(l)
			&& (static_cast<u32>(l->value_type) == bson_typeid_def::E_BSON_TYPE_REGEX)
			&& (0 == strcmp(l->value.v_regex.regex, str_regex.data()))
			&& (0 == strcmp(l->value.v_regex.options, options_buffer));
	}

	template<typename String> inline
	static typename boost::enable_if< charset::utf8_string_constructable<String>, bool >::type
		s_value_cmp_eq_code(const bson_value_t* l, const String& r)
	{
		typedef String now_string_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, conv_string_type);

		assert(l);

		conv_string_type str((charset::string_charset_helper_data(r)));

		return
			(l)
			&& (static_cast<u32>(l->value_type) == bson_typeid_def::E_BSON_TYPE_CODE)
			&& (l->value.v_code.code_len == str.size())
			&& (0 == memcmp(l->value.v_code.code, str.data(), l->value.v_code.code_len));
	}

	template<typename String> inline
	static typename boost::enable_if< charset::utf8_string_constructable<String>, bool >::type
		s_value_cmp_eq_symbol(const bson_value_t* l, const String& r)
	{
		typedef String now_string_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, conv_string_type);

		assert(l);

		conv_string_type str((charset::string_charset_helper_data(r)));

		return
			(l)
			&& (static_cast<u32>(l->value_type) == bson_typeid_def::E_BSON_TYPE_SYMBOL)
			&& (l->value.v_symbol.len == str.size())
			&& (0 == memcmp(l->value.v_symbol.symbol, str.data(), l->value.v_symbol.len));
	}

	template<typename String> inline
	static typename boost::enable_if< charset::utf8_string_constructable<String>, bool >::type
		s_value_cmp_eq_code_w_scope(const bson_value_t* l, const String& r_code, const bson_t* r_scope)
	{
		typedef String now_string_type;
		//YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_string_type, conv_string_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, conv_string_type);

		assert(l);
		assert((r_scope && r_scope->len));

		conv_string_type str((charset::string_charset_helper_data(r_code)));
		bson_t scope = {0};
		bool binit = bson_init_static(&scope, l->value.v_codewscope.scope_data, l->value.v_codewscope.scope_len);
		bool ret = (l) && (r_scope) && (r_scope->len) && (binit)
					&& (static_cast<u32>(l->value_type) == bson_typeid_def::E_BSON_TYPE_CODEWSCOPE)
					&& (l->value.v_codewscope.code_len == str.size())
					&& (0 == memcmp(l->value.v_codewscope.code, str.data(), l->value.v_codewscope.code_len))
					&& (bson_equal(r_scope, &scope));

		bson_destroy(&scope);
		return ret;
	}

	static bool s_value_cmp_eq_timestamp(const bson_value_t* l, u32 timestamp, u32 increment);

	template<typename String> inline
	static typename boost::enable_if< charset::utf8_string_constructable<String>, bool >::type
		s_value_cmp_eq_dbpointer(const bson_value_t* l, const String& r_col, const bson_oid_t* r_oid)
	{
		typedef String now_string_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, conv_string_type);

		assert(l);
		assert(r_oid);

		conv_string_type str((charset::string_charset_helper_data(r_col)));

		return
			(l) && (r_oid)
			&& (static_cast<u32>(l->value_type) == bson_typeid_def::E_BSON_TYPE_DBPOINTER)
			&& (l->value.v_dbpointer.collection_len == str.size())
			&& (0 == memcmp(l->value.v_dbpointer.collection, str.data(), l->value.v_dbpointer.collection_len))
			&& (0 == memcmp(boost::addressof(l->value.v_dbpointer.oid), r_oid, bson_oid_native_ex::E_bson_oid_byte_size));
	}

	static bool s_value_cmp_eq_decimal128(const bson_value_t* l, const bson_decimal128_t* r);
	static s32 s_value_cmp_decimal128(const bson_value_t* l, const bson_decimal128_t* r);

	static bool s_value_cmp_eq_bson(const bson_value_t* l, const bson_t* r);
	static bool s_value_cmp_eq_array(const bson_value_t* l, const bson_t* r);

	static bool s_value_cmp_eq_value(const bson_value_t* l, const bson_value_t* r);
	static s32 s_value_cmp_value(const bson_value_t* l, const bson_value_t* r);

protected:
	static bson_error_t& pro_s_make_bson_validate_diff_t_err(bson_error_t& err, u32 lt, u32 rt);
	static bson_error_t& pro_s_make_bson_validate_only_support_cmp_eq_t_err(bson_error_t& err, u32 t);
};

} // namespace nsql_database_sytem
} // namespace yggr

// non member foo
inline bool operator==(const bson_value_t& l, const bson_value_t& r)
{
	return yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_value(&l, &r);
}

inline bool operator!=(const bson_value_t& l, const bson_value_t& r)
{
	return !yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_value(&l, &r);
}

//----------------------
#define YGGR_BSON_VALUE_TAG_DEF(__tag_name__, __tag_typeid__) \
	namespace yggr { namespace nsql_database_system { \
		struct BOOST_PP_CAT(bson_value_tag_, __tag_name__) \
			: public boost::mpl::integral_c<u32, __tag_typeid__> {}; }}

YGGR_BSON_VALUE_TAG_DEF(eod, BSON_TYPE_EOD)
YGGR_BSON_VALUE_TAG_DEF(double, BSON_TYPE_DOUBLE)
YGGR_BSON_VALUE_TAG_DEF(utf8, BSON_TYPE_UTF8)
YGGR_BSON_VALUE_TAG_DEF(document, BSON_TYPE_DOCUMENT)
YGGR_BSON_VALUE_TAG_DEF(array, BSON_TYPE_ARRAY)
YGGR_BSON_VALUE_TAG_DEF(binary, BSON_TYPE_BINARY)
YGGR_BSON_VALUE_TAG_DEF(undefined, BSON_TYPE_UNDEFINED)
YGGR_BSON_VALUE_TAG_DEF(oid, BSON_TYPE_OID)
YGGR_BSON_VALUE_TAG_DEF(bool, BSON_TYPE_BOOL)
YGGR_BSON_VALUE_TAG_DEF(date_time, BSON_TYPE_DATE_TIME)
YGGR_BSON_VALUE_TAG_DEF(time, BSON_TYPE_DATE_TIME)
YGGR_BSON_VALUE_TAG_DEF(timeval, BSON_TYPE_DATE_TIME)
YGGR_BSON_VALUE_TAG_DEF(null, BSON_TYPE_NULL)
YGGR_BSON_VALUE_TAG_DEF(regex, BSON_TYPE_REGEX)
YGGR_BSON_VALUE_TAG_DEF(dbpointer, BSON_TYPE_DBPOINTER)
YGGR_BSON_VALUE_TAG_DEF(code, BSON_TYPE_CODE)
YGGR_BSON_VALUE_TAG_DEF(symbol, BSON_TYPE_SYMBOL)
YGGR_BSON_VALUE_TAG_DEF(codewscope, BSON_TYPE_CODEWSCOPE)
YGGR_BSON_VALUE_TAG_DEF(int32, BSON_TYPE_INT32)
YGGR_BSON_VALUE_TAG_DEF(timestamp, BSON_TYPE_TIMESTAMP)
YGGR_BSON_VALUE_TAG_DEF(int64, BSON_TYPE_INT64)
YGGR_BSON_VALUE_TAG_DEF(decimal128, BSON_TYPE_DECIMAL128)
YGGR_BSON_VALUE_TAG_DEF(maxkey, BSON_TYPE_MAXKEY)
YGGR_BSON_VALUE_TAG_DEF(minkey, BSON_TYPE_MINKEY)

YGGR_BSON_VALUE_TAG_DEF(value, bson_typeid_def::E_BSON_TYPE_ANY_VALUE)

#undef YGGR_BSON_VALUE_TAG_DEF

#define YGGR_BSON_VALUE_TAG_TYPE( __tag_name__ ) \
	yggr::nsql_database_system::BOOST_PP_CAT( bson_value_tag_, __tag_name__ )

#define YGGR_BSON_VALUE_TAG_OBJECT( __tag_name__ ) \
	yggr::nsql_database_system::BOOST_PP_CAT( bson_value_tag_, __tag_name__ )()

#define YGGR_BSON_VALUE_TAG_TYPEID( __tag_name__ ) \
	yggr::nsql_database_system::BOOST_PP_CAT( bson_value_tag_, __tag_name__ )::value

namespace yggr
{
namespace nsql_database_system
{
namespace detail
{

template<>
struct bson_operator_outter<bson_value_t>
{
	template<typename Char, typename Traits> inline
	std::basic_ostream<Char, Traits>&
		operator()(std::basic_ostream<Char, Traits>& os,
					const bson_value_t& val,
					bson_json_mode_def::mode_type mode) const
	{
		typedef boost::mpl::size_t<256> str_buf_max_size_type;

		char str_buf[str_buf_max_size_type::value] = {0};
		s32 str_buf_len = 0;

		switch(val.value_type)
		{
		case yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_DOUBLE:
			{
				bool legacy =
					(mode == bson_json_mode_def::E_BSON_JSON_MODE_LEGACY
						|| (mode == bson_json_mode_def::E_BSON_JSON_MODE_RELAXED
						&& !(val.value.v_double != val.value.v_double || val.value.v_double * 0 != 0)));

				if(!legacy)
				{
					os << "{ \"$numberDouble\" : \"";
				}

				if(!legacy && val.value.v_double != val.value.v_double)
				{
					os << "NaN";
				}
				else if(!legacy && val.value.v_double * 0 != 0)
				{
					if(val.value.v_double > 0)
					{
						os << "Infinity";
					}
					else
					{
						os << "-Infinity";
					}
				}
				else
				{
					str_buf_len = std::sprintf(str_buf, "%.20g", val.value.v_double);
					assert(0 <= str_buf_len && str_buf_len < str_buf_max_size_type::value - 2);

					/* ensure trailing ".0" to distinguish "3" from "3.0" */
					if(strspn(str_buf, "0123456789-") == str_buf_len)
					{
						str_buf[str_buf_len] = '.';
						str_buf[str_buf_len + 1] = '0';
					}

					os << str_buf;
				}

				if(!legacy)
				{
					os << "\" }";
				}
			}
			break;

		case yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_UTF8:
			{
				json_escape_string escaped(val.value.v_utf8.str, val.value.v_utf8.len);
				os << escaped;
			}
			break;

		case yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_DOCUMENT:
		case yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_ARRAY:
			{
				bson_t local = {0};
				if(bson_init_static(&local,
										val.value.v_doc.data,
										val.value.v_doc.data_len))
				{
					os << local;
					bson_destroy (&local);
				}
				else
				{
					os << "{ }";
				}
			}
			break;

		case yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_BINARY:
			{
				typedef yggr::string out_string_type;

				YGGR_PP_CONST_CHAR_LOCAL_DEF(char_zero, '0')

				std::ios_base::fmtflags org_flags = os.flags();
				Char org_fill = os.fill();

				std::ios_base::fmtflags hex_flags = (org_flags & ~std::ios::basefield) | std::ios::hex;
				Char hex_fill = YGGR_PP_CONST_CHAR_LOCAL_GET(char_zero, Char);

				out_string_type ostr;

				if(val.value.v_binary.data)
				{
					yggr::u64 buf_byte_size = val.value.v_binary.data_len;
					yggr::u64 b64_len = b64_ntop_byte_size(buf_byte_size);
					out_string_type b64_buf(b64_len, 0);
					b64_ntop(val.value.v_binary.data, buf_byte_size, &b64_buf[0], b64_len);
					ostr.swap(b64_buf);
				}

				switch(mode)
				{
				case bson_json_mode_def::E_BSON_JSON_MODE_CANONICAL:
				case bson_json_mode_def::E_BSON_JSON_MODE_RELAXED:
					os << "{ \"$binary\" : { \"base64\" : \""
						<< ostr
						<< "\", \"subType\" : \"";
					os.flags(hex_flags);
					os.fill(hex_fill);
					os << std::setw(2) << val.value.v_binary.subtype;
					os.flags(org_flags);
					os.fill(org_fill);
					os << "\" } }";
					break;
				default:
					os << "{ \"$binary\" : \""
						<< ostr
						<< "\", \"$type\" : \"";
					os.flags(hex_flags);
					os.fill(hex_fill);
					os << std::setw(2) << val.value.v_binary.subtype;
					os.flags(org_flags);
					os.fill(org_fill);
					os << "\" }";
				}
			}
			break;

		case yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_NULL:
			{
				os << "null";
			}
			break;

		case yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_UNDEFINED:
			{
				os << "{ \"$undefined\" : true }";
			}
			break;

		case yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_OID:
			{
				os << val.value.v_oid;
			}
			break;

		case yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_BOOL:
			{
				os << (val.value.v_bool? "true" : "false");
			}
			break;

		case yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_DATE_TIME:
			{
				switch(mode)
				{
				case bson_json_mode_def::E_BSON_JSON_MODE_RELAXED:
					if(!(val.value.v_datetime < 0))
					{
						os << "{ \"$date\" : \""
							<< bson_date_time_native_ex::s_bson_date_format_iso8601(
								val.value.v_datetime, str_buf, str_buf_max_size_type::value)
							<< "\" }";
						break;
					}
				case bson_json_mode_def::E_BSON_JSON_MODE_CANONICAL:
					str_buf_len = std::sprintf(str_buf, "%" PRId64, val.value.v_datetime);
					assert(0 <= str_buf_len && str_buf_len < str_buf_max_size_type::value);

					os << "{ \"$date\" : { \"$numberLong\" : \"" << str_buf << "\" } }";
					break;
				default:
					str_buf_len = std::sprintf(str_buf, "%" PRId64, val.value.v_datetime);
					assert(0 <= str_buf_len && str_buf_len < str_buf_max_size_type::value);
					os << "{ \"$date\" : " << str_buf << " }";
				}
			}
			break;

		case yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_REGEX:
			{
				typedef yggr::nsql_database_system::bson_regex_native_ex bson_regex_native_ex_type;
				json_escape_string escaped_v_regex(val.value.v_regex.regex);

				char options_buffer[bson_regex_native_ex_type::E_BSON_REGEX_OPTIONS_STRING_BUFFER_SIZE] = {0};
				bson_regex_native_ex_type::s_bson_regex_options_sorted(options_buffer, val.value.v_regex.options);

				switch(mode)
				{
				case bson_json_mode_def::E_BSON_JSON_MODE_CANONICAL:
				case bson_json_mode_def::E_BSON_JSON_MODE_RELAXED:
					os << "{ \"$regularExpression\" : { \"pattern\" : "
						<< escaped_v_regex
						<< ", \"options\" : \""
						<< options_buffer
						<< "\" } }";
					break;
				default:
					os << "{ \"$regex\" : " << escaped_v_regex
						<< ", \"$options\" : \"" << options_buffer << "\" }";
				}
			}
			break;

		case yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_CODE:
			{
				/*
				old_version:
					os << "\"" << val.value.v_code.code<< "\""; // old_version
				new_version:
					bson_string_append(state->str, "{ \"$code\" : \"");
					bson_string_append(state->str, escaped);
					bson_string_append(state->str, "\" }");
				*/
				json_escape_string escaped(val.value.v_code.code, val.value.v_code.code_len);
				os << "{ \"$code\" : " << escaped << " }";

			}
			break;

		case yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_SYMBOL:
			{
				json_escape_string escaped(val.value.v_symbol.symbol, val.value.v_symbol.len);

				switch(mode)
				{
				case bson_json_mode_def::E_BSON_JSON_MODE_CANONICAL:
				case bson_json_mode_def::E_BSON_JSON_MODE_RELAXED:
					os << "{ \"$symbol\" : " << escaped << " }";
					break;
				default:
					os << escaped;
				}
			}
			break;

		case yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_CODEWSCOPE:
			{
				json_escape_string code_escaped(val.value.v_codewscope.code, val.value.v_codewscope.code_len);
				bson_t bs_scope = {0};

				os << "{ \"$code\" : "
					<<  code_escaped
					<< ", \"$scope\" : ";

				if(bson_init_static(
					&bs_scope,
					val.value.v_codewscope.scope_data,
					val.value.v_codewscope.scope_len))
				{
					os << bs_scope << " }";
					bson_destroy(&bs_scope);
				}
				else
				{
					os << "{ } }";
				}
			}
			break;

		case yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_DBPOINTER:
			{
				json_escape_string collection_escaped(val.value.v_dbpointer.collection,
															val.value.v_dbpointer.collection_len);
				yggr::string oid_str(bson_oid_native_ex::E_bson_oid_string_size, 0);
				bson_oid_to_string(&(val.value.v_dbpointer.oid), &(oid_str[0]));

				switch(mode)
				{
				case bson_json_mode_def::E_BSON_JSON_MODE_CANONICAL:
				case bson_json_mode_def::E_BSON_JSON_MODE_RELAXED:
					os << "{ \"$dbPointer\" : { \"$ref\" : " << collection_escaped
						<< ", \"$id\" : \"" << oid_str
						<< "\" } }";
					break;

				default:
					os << "{ \"$ref\" : " << collection_escaped
						<< ", \"$id\" : \"" << oid_str
						<< "\" }";
				}
			}
			break;

		case yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_INT32:
			{
				switch(mode)
				{
				case bson_json_mode_def::E_BSON_JSON_MODE_CANONICAL:
					os << "{ \"$numberInt\" : \"" << val.value.v_int32 << "\" }";
					break;
				default:
					os << val.value.v_int32;
				}
			}
			break;

		case yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_INT64:
			{
				switch(mode)
				{
				case bson_json_mode_def::E_BSON_JSON_MODE_CANONICAL:
					os << "{ \"$numberLong\" : \"" << val.value.v_int64 << "\" }";
					break;
				default:
					os << val.value.v_int64;
				}
			}
			break;

		case yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_TIMESTAMP:
			{
				os << "{ \"$timestamp\" : { \"t\" : "
					<< val.value.v_timestamp.timestamp
					<< ", \"i\" : " << val.value.v_timestamp.increment
					<< " } }";
			}
			break;

		case yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_DECIMAL128:
			{
				char decimal128_string[BSON_DECIMAL128_STRING] = {0};
				bson_decimal128_to_string(&(val.value.v_decimal128), decimal128_string);
				os << "{ \"$numberDecimal\" : \""
					<< decimal128_string
					<< "\" }";
			}
			break;

		case yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_MAXKEY:
			{
				os << "{ \"$maxKey\" : 1 }";
			}
			break;

		case yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_MINKEY:
			{
				os << "{ \"$minKey\" : 1 }";
			}
			break;

		case yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_EOD:
		default:
			break;
		};

		return os;
	}
};

} // namespace detail
} // namespace nsql_database_system
} // namespace yggr

template<typename Char, typename Traits> inline
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os, const bson_value_t& val)
{
	typedef yggr::nsql_database_system::bson_json_mode_def bson_json_mode_def_type;
	typedef yggr::nsql_database_system::detail::bson_operator_outter<bson_value_t> outter_type;

	outter_type outter;
	return outter(os, val, bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY);
}

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{

void swap(bson_value_t& l, bson_value_t& r);

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, void>::type
	swap(bson_value_t& l, BsonValue& r)
{
	bson_value_native_ex::s_bson_value_swap(boost::addressof(l), boost::addressof(r));
}

#if defined(__GNUC__) && (YGGR_CPP_VERSION < YGGR_CPP_VER_11)

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, void>::type
	swap(bson_value_t& l, BOOST_RV_REF(BsonValue) r)
{
    BsonValue& right_ref = r;
	bson_value_native_ex::s_bson_value_swap(boost::addressof(l), boost::addressof(right_ref));
}

#endif // #if defined(__GNUC__) && (YGGR_CPP_VERSION < YGGR_CPP_VER_11)

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

#endif // __YGGR_NSQL_DATABASE_SYSTEM_BSON_VALUE_NATIVE_EX_HPP__

