//bson_native_ex.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_BSON_NATIVE_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_BSON_NATIVE_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/container_ex/is_allocator.hpp>

#include <yggr/charset/utf8_string.hpp>

#include <yggr/nsql_database_system/bson_config.hpp>
#include <yggr/nsql_database_system/bson_json_opts.hpp>
#include <yggr/nsql_database_system/bson_typeid_def.hpp>
#include <yggr/nsql_database_system/native_ex_decl_bson.hpp>
#include <yggr/nsql_database_system/bson_visit_t.hpp>

#include <boost/type_traits/is_base_of.hpp>

#if !defined(BSON_INSIDE)
#	define BSON_INSIDE
#	define YGGR_USING_BSON_INSIDE
#endif // BSON_INSIDE

#include <bson/bson-private.h> // include the file is ok

#if defined(YGGR_USING_BSON_INSIDE)
#	undef BSON_INSIDE
#	undef YGGR_USING_BSON_INSIDE
#endif // YGGR_USING_BSON_INSIDE

namespace yggr
{
namespace nsql_database_system
{

class bson_native_ex
{
public:
	typedef bson_json_mode_def::mode_type bson_json_mode_type;

private:
	typedef bson_native_ex this_type;

public:
	// org bson_init not init infill data
	static void s_bson_init(bson_t *bson);

	// the static bson is a bson const_reference
	static bson_t* s_bson_init_static(bson_t* bson, const uint8_t* data, std::size_t length);

	static bson_t* s_bson_init_from_json(bson_t* bson, const char* str_json,
											ssize_t len, bson_error_t* error);

	static bson_t* s_bson_init_from_size(bson_t* bson, ssize_t size);
	static bson_t* s_bson_init_from_data(bson_t* bson, const u8* buffer, ssize_t len);

	static bson_t* s_bson_copy_to(const bson_t* psrc, bson_t* pdst);
	static bson_t* s_bson_copy(bson_t* pdst, const bson_t* psrc);
	static bson_t* s_bson_copy_uninitialized(bson_t* pdst, const bson_t* psrc);

	template<typename Handler> // handler() if return true remove, if return false copy
	static bson_t* s_bson_select_copy(bson_t* pdst, const bson_t* psrc, const Handler& handler)
	{
		assert(pdst && psrc);

		if(!(pdst && psrc))
		{
			return 0;
		}

		if(!this_type::s_bson_is_validate(psrc))
		{
			return 0;
		}

		//don't remove the code, debug memory used.
		//u8* buf = new u8[psrc->len];
		//memset(buf, 0, psrc->len);
		yggr::string buf(psrc->len, 0);


		bson_iter_t iter = {0};
		bson_iter_init(&iter, psrc);

		std::size_t pos = 0;
		std::size_t copy_size = 0;

		copy_size = iter.next_off - iter.off;
		if(copy_size) 
		{
			memcpy(&buf[pos], iter.raw + iter.off, copy_size);
			pos += copy_size;
		}

		for(;bson_iter_next(&iter);)
		{
			if(handler(bson_iter_key(&iter)))
			{
				copy_size = iter.next_off - iter.off;
				if(copy_size)
				{
					memcpy(&buf[pos], iter.raw +  iter.off, copy_size);
					pos += copy_size;
				}
			}
		}

		uint32_t byte_size = static_cast<uint32_t>(pos + 1);
#	if BSON_BYTE_ORDER == BSON_LITTLE_ENDIAN
		memcpy(&buf[0], &byte_size, sizeof(uint32_t));
#	else
		uint32_t length_le = BSON_UINT32_TO_LE (byte_size);
		memcpy(&buf[0], &length_le, sizeof(uint32_t));
#	endif // BSON_BYTE_ORDER == BSON_LITTLE_ENDIAN
		this_type::s_bson_init_from_data(pdst, reinterpret_cast<u8*>(&buf[0]), byte_size);

		//don't remove the code, debug memory used
		//delete []buf;
		return pdst;
	}

	static void s_bson_swap(bson_t* pl, bson_t* r);

	// c_bson save_start save_end need a swap chain at serialization,
	// but mongodb_c_driver not support parent->child, (^_^|||)
	// ****lucky****(~@^_^@~), plenty of space for the bson_t.
	// so ~~~~~~~~~~~~~~~~~~~
	static void s_bson_mark_set_child(bson_t* pnow, bson_t* pchild);
	static bson_t* s_bson_mark_get_child(bson_t* pnow);

private:
	static void s_bson_swap_of_inline(bson_t* pl, bson_t* pr);
	static void s_bson_swap_of_not_inline(bson_t* pl, bson_t* pr);
	static void s_bson_swap_of_mix(bson_t* pl, bson_t* pr);

public:
	static const u8* s_bson_data(const bson_t* pbs);
	static u8* s_bson_data(bson_t* pbs);
	static u32 s_bson_size(const bson_t* pbs);

	static void s_bson_encode_length(bson_t* pbs);
	static bool s_bson_impl_inline_grow(bson_impl_inline_t* pimpl, std::size_t size);
	static bool s_bson_impl_alloc_grow(bson_impl_alloc_t* pimpl, std::size_t size);
	static bool s_bson_grow(bson_t* pbs, uint32_t size);

	static char* s_bson_as_json_visit_all(const bson_t* bson, size_t* length,
											bson_json_mode_type mode, s32 max_len);

	inline static char* s_bson_as_json_with_opts(const bson_t* pbs,
													std::size_t* psize,
													const bson_json_opts_t* popts)
	{
		return this_type::s_bson_as_json_with_opts(pbs, psize, reinterpret_cast<const native_bson_json_opts_t*>(popts));
	}

	inline static char* s_bson_as_json_with_opts(const bson_t* pbs,
													std::size_t* psize,
													const native_bson_json_opts_t* popts)
	{
		assert(popts);
		return 
			popts? 
				this_type::s_bson_as_json_visit_all(
					pbs, psize, popts->mode, popts->max_len)
				: 0;
	}

	inline static char* s_bson_as_json(const bson_t* pbs, std::size_t* psize)
	{
		static const native_bson_json_opts_t opts 
			= { bson_json_mode_def::E_BSON_JSON_MODE_LEGACY,
				bson_json_mode_def::E_BSON_MAX_LEN_UNLIMITED };
		return this_type::s_bson_as_json_with_opts(pbs, psize, &opts);
	}

	inline static char* s_bson_as_json_canonical_extended(const bson_t* pbs, std::size_t* psize)
	{
		static const native_bson_json_opts_t opts 
			= { bson_json_mode_def::E_BSON_JSON_MODE_CANONICAL, 
				bson_json_mode_def::E_BSON_MAX_LEN_UNLIMITED };
		return this_type::s_bson_as_json_with_opts(pbs, psize, &opts);
	}

	inline static char* s_bson_as_json_relaxed_extended(const bson_t* pbs, std::size_t* psize)
	{
		static const native_bson_json_opts_t opts 
			= { bson_json_mode_def::E_BSON_JSON_MODE_RELAXED, 
				bson_json_mode_def::E_BSON_MAX_LEN_UNLIMITED };
		return this_type::s_bson_as_json_with_opts(pbs, psize, &opts);
	}

	static bool s_bson_is_nil(const bson_t* pbs);
	static bool s_bson_is_validate(const bson_t* pbs, u32 flags, size_t* psize = 0);
	static bool s_bson_is_validate(const bson_t* pbs);
	static bool s_bson_is_array(const bson_t* pbs);

	template<typename String> inline
	static String s_bson_to_json(const bson_t* pbs,
									const string& charset_name = charset::charset_name_t<String>())
	{
		typedef String ret_string_type;
		typedef utf8_string utf8_string_type;

		if(!pbs || !pbs->len)
		{
			return ret_string_type();
		}

		std::size_t byte_size = 0;
		char* pstr = bson_native_ex::s_bson_as_json(pbs, &byte_size);
		if(!(pstr && byte_size))
		{
			return ret_string_type();

		}
		utf8_string_type str(pstr, byte_size, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		free(pstr);
		return str.str<ret_string_type>(charset_name);
	}

	template<typename String, typename Alloc> inline
	static typename boost::enable_if<container_ex::is_allocator<Alloc>, String>::type
		s_bson_to_json(const bson_t* pbs, const Alloc& alloc,
						const string& charset_name = charset::charset_name_t<String>())
	{
		typedef String ret_string_type;
		typedef utf8_string utf8_string_type;

		if(!pbs || !pbs->len)
		{
			return ret_string_type();
		}

		std::size_t byte_size = 0;
		char* pstr = bson_native_ex::s_bson_as_json(pbs, &byte_size);
		if(!(pstr && byte_size))
		{
			return ret_string_type();

		}
		utf8_string_type str(pstr, byte_size, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		free(pstr);
		return str.str<ret_string_type>(alloc, charset_name);
	}

	template<typename String> inline
	static String s_bson_to_json_canonical(const bson_t* pbs,
											const string& charset_name = charset::charset_name_t<String>())
	{
		typedef String ret_string_type;
		typedef utf8_string utf8_string_type;

		if(!pbs || !pbs->len)
		{
			return ret_string_type();
		}

		std::size_t byte_size = 0;
		char* pstr = bson_native_ex::s_bson_as_json_canonical_extended(pbs, &byte_size);
		if(!(pstr && byte_size))
		{
			return ret_string_type();

		}
		utf8_string_type str(pstr, byte_size, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		free(pstr);
		return str.str<ret_string_type>(charset_name);
	}

	template<typename String, typename Alloc> inline
	static typename boost::enable_if<container_ex::is_allocator<Alloc>, String>::type
		s_bson_to_json_canonical(const bson_t* pbs, const Alloc& alloc,
									const string& charset_name = charset::charset_name_t<String>())
	{
		typedef String ret_string_type;
		typedef utf8_string utf8_string_type;

		if(!pbs || !pbs->len)
		{
			return ret_string_type();
		}

		std::size_t byte_size = 0;
		char* pstr = bson_native_ex::s_bson_as_json_canonical_extended(pbs, &byte_size);
		if(!(pstr && byte_size))
		{
			return ret_string_type();

		}
		utf8_string_type str(pstr, byte_size, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		free(pstr);
		return str.str<ret_string_type>(alloc, charset_name);
	}

	template<typename String> inline
	static String s_bson_to_json_relaxed(const bson_t* pbs,
											const string& charset_name = charset::charset_name_t<String>())
	{
		typedef String ret_string_type;
		typedef utf8_string utf8_string_type;

		if(!pbs || !pbs->len)
		{
			return ret_string_type();
		}

		std::size_t byte_size = 0;
		char* pstr = bson_native_ex::s_bson_as_json_relaxed_extended(pbs, &byte_size);
		if(!(pstr && byte_size))
		{
			return ret_string_type();

		}
		utf8_string_type str(pstr, byte_size, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		free(pstr);
		return str.str<ret_string_type>(charset_name);
	}

	template<typename String, typename Alloc> inline
	static typename boost::enable_if<container_ex::is_allocator<Alloc>, String>::type
		s_bson_to_json_relaxed(const bson_t* pbs, const Alloc& alloc,
									const string& charset_name = charset::charset_name_t<String>())
	{
		typedef String ret_string_type;
		typedef utf8_string utf8_string_type;

		if(!pbs || !pbs->len)
		{
			return ret_string_type();
		}

		std::size_t byte_size = 0;
		char* pstr = bson_native_ex::s_bson_as_json_relaxed_extended(pbs, &byte_size);
		if(!(pstr && byte_size))
		{
			return ret_string_type();

		}
		utf8_string_type str(pstr, byte_size, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		free(pstr);
		return str.str<ret_string_type>(alloc, charset_name);
	}

	static bool s_bson_concat_self(bson_t* pbs);

	static bool s_bson_save_self(bson_t* pbs, const char* name, s32 nlen);
	static bool s_bson_save_root_iter(bson_t* pbs, const char* name, s32 nlen, const bson_iter_t* val);
	static bool s_bson_save_iter(bson_t* pbs, const char* name, s32 nlen, const bson_iter_t* val);

	// warning don't call s_bson_save_name and s_bson_save_value_of_iter at same address bson_t and bson_iter_t
	static bool s_bson_save_name(bson_t* pbs, u32 ntype, const char* name, s32 nlen);

	inline static bool s_bson_save_value_of_null(bson_t* pbs)
	{
		assert(pbs);
		assert(!bson_native_ex::s_bson_is_nil(pbs));
		assert(bson_native_ex::s_check_save_name(pbs, bson_typeid_def::E_BSON_TYPE_NULL));
		// don't need save
		return true;
	}

	inline static bool s_bson_save_value_of_undefined(bson_t* pbs)
	{
		assert(pbs);
		assert(!bson_native_ex::s_bson_is_nil(pbs));
		assert(bson_native_ex::s_check_save_name(pbs, bson_typeid_def::E_BSON_TYPE_UNDEFINED));
		// don't need save
		return true;
	}

	inline static bool s_bson_save_value_of_minkey(bson_t* pbs)
	{
		assert(pbs);
		assert(!bson_native_ex::s_bson_is_nil(pbs));
		assert(bson_native_ex::s_check_save_name(pbs, bson_typeid_def::E_BSON_TYPE_MINKEY));
		// don't need save
		return true;
	}

	inline static bool s_bson_save_value_of_maxkey(bson_t* pbs)
	{
		assert(pbs);
		assert(!bson_native_ex::s_bson_is_nil(pbs));
		assert(bson_native_ex::s_check_save_name(pbs, bson_typeid_def::E_BSON_TYPE_MAXKEY));
		// don't need save
		return true;
	}

	static bool s_bson_save_value_of_s32(bson_t* pbs, s32 val);
	static bool s_bson_save_value_of_s64(bson_t* pbs, s64 val);
	static bool s_bson_save_value_of_bool(bson_t* pbs, bool val);
	static bool s_bson_save_value_of_f64(bson_t* pbs, f64 val);
	static bool s_bson_save_value_of_root_iter(bson_t* pbs, const bson_iter_t* val);
	static bool s_bson_save_value_of_iter(bson_t* pbs, const bson_iter_t* val);

	static bool s_bson_save_value_of_bson_self(bson_t* pbs);
	static bool s_bson_save_value_of_array(bson_t* pbs, const bson_t* val);
	static bool s_bson_save_value_of_bson(bson_t* pbs, const bson_t* val);
	static bool s_bson_save_value_of_bson_object(bson_t* pbs, const bson_t* val);

	// change semantics!!! if val is zero save empty string, not save null_type
	static bool s_bson_save_value_of_utf8(bson_t* pbs, const char* val, s32 nlen);
	// change semantics!!! if org_buf is zero save empty buffer, not assert false
	static bool s_bson_save_value_of_binary_buffer(bson_t* pbs, u32 sub_t, const u8* org_buf, s32 nlen);

	static bool s_bson_save_value_of_date(bson_t* pbs, u64 date);
	static bool s_bson_save_value_of_timestamp(bson_t* pbs, u32 tm, u32 inc);
	static bool s_bson_save_value_of_timestamp(bson_t* pbs, u64 timestamp);

	static bool s_bson_save_value_of_oid(bson_t* pbs, const bson_oid_t* poid);

	static bool s_bson_save_value_of_regex(bson_t* pbs,
											const char* regex,
											const char* opts);

	static bool s_bson_save_value_of_regex(bson_t* pbs,
											const char* regex, u32 regex_len,
											const char* opts, u32 opts_len);

	static bool s_bson_save_value_of_string_item(bson_t* pbs, u32 bs_t, const char* str);
	static bool s_bson_save_value_of_string_item(bson_t* pbs, u32 bs_t, const char* str, u32 len);

	static bool s_bson_save_value_of_code(bson_t* pbs, const char* code);
	static bool s_bson_save_value_of_code(bson_t* pbs, const char* code, u32 code_len);

	static bool s_bson_save_value_of_symbol(bson_t* pbs, const char* symbol);
	static bool s_bson_save_value_of_symbol(bson_t* pbs, const char* symbol, u32 symbol_len);

	static bool s_bson_save_value_of_code_w_scope(bson_t* pbs, const char* code, const bson_t* pscope);
	static bool s_bson_save_value_of_code_w_scope(bson_t* pbs, const char* code, u32 len, const bson_t* pscope);

	static bool s_bson_save_value_of_dbpointer(bson_t* pbs, const char* str_col, const bson_oid_t* poid);
	static bool s_bson_save_value_of_dbpointer(bson_t* pbs, const char* str_col, u32 len, const bson_oid_t* poid);

	static bool s_bson_save_value_of_decimal128(bson_t* pbs, const bson_decimal128_t* pdecimal128);

	static bool s_bson_save_value_of_value(bson_t* pbs, const bson_value_t* pval);

#ifdef _DEBUG
	static bool s_check_save_name(bson_t* pbs, u32 nchk_num);
#endif //_DEBUG

protected:
	static bool pro_s_bson_save_name_undo(bson_t* pbs);
	static uint8_t* pro_s__bson_data(const bson_t* pbs);
	
};

} // namesapce nsql_database_system
} // namespace yggr

#define YGGR_ASSERT_BSON_TYPE( __type__ ) \
	BOOST_MPL_ASSERT((boost::is_base_of< bson_t, __type__>))

#define YGGR_ASSERT_BSON_TYPE_NOT( __type__ ) \
	BOOST_MPL_ASSERT_NOT((boost::is_base_of< bson_t, __type__>))


namespace yggr
{
namespace nsql_database_system
{
namespace detail
{

template<>
struct bson_operator_outter<bson_t>
{
	template<typename Char, typename Traits> inline
	std::basic_ostream<Char, Traits>&
		operator()(std::basic_ostream<Char, Traits>& os, 
					const bson_t& cb, 
					bson_json_mode_def::mode_type mode) const
	{
		assert((bson_json_mode_def::E_BSON_JSON_MODE_LEGACY <= mode
				&& mode <= bson_json_mode_def::E_BSON_JSON_MODE_RELAXED));
		
		const native_bson_json_opts_t opts 
			= { (bson_json_mode_def::E_BSON_JSON_MODE_LEGACY <= mode
						&& mode <= bson_json_mode_def::E_BSON_JSON_MODE_RELAXED)? 
					mode : bson_json_mode_def::E_BSON_JSON_MODE_LEGACY,
				bson_json_mode_def::E_BSON_MAX_LEN_UNLIMITED };

		char* pstr = bson_native_ex::s_bson_as_json_with_opts(&cb, 0, &opts);
		os << (pstr? pstr : "nil_bson");
		if(pstr)
		{
			bson_free(pstr);
		}
		return os;
	}
};

} // namespace detail
} // namespace nsql_database_system
} // namespace yggr

template<typename Char, typename Traits> inline
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os, const bson_t& cb)
{
	typedef yggr::nsql_database_system::detail::bson_operator_outter<bson_t> outter_type;

	outter_type outter;
	return outter(os, cb, yggr::nsql_database_system::bson_json_mode_def::E_BSON_JSON_MODE_LEGACY);
}

// now don't use this foo style, but don't delete it
//template<typename Char, typename Traits,
//			template<typename _Char, typename _Traits> class Basic_OStream >
//Basic_OStream<Char, Traits>&
//	operator<<(Basic_OStream<Char, Traits>& os,
//				const yggr::nsql_database_system::c_bson::base_type& cb);

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{

void swap(bson_t& l, bson_t& r);

template<typename Bson> inline
typename boost::enable_if<boost::is_base_of<bson_t, Bson>, void>::type
	swap(bson_t& l, Bson& r)
{
	bson_native_ex::s_bson_swap(boost::addressof(l), boost::addressof(r));
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

#endif // __YGGR_NSQL_DATABASE_SYSTEM_BSON_NATIVE_HPP__
