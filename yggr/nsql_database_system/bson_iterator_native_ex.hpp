//bson_iterator_native_ex.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_BSON_ITER_NATIVE_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_BSON_ITER_NATIVE_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/nsql_database_system/bson_config.hpp>
#include <yggr/nsql_database_system/native_ex_decl_bson.hpp>

#include <boost/ref.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_base_of.hpp>

namespace yggr
{
namespace nsql_database_system
{

//typedef struct
//{
//	uint32_t       count;
//	bool           keys;
//	uint32_t       depth;
//	bson_string_t* str;
//} native_bson_json_state_t; // discard

/*
typedef struct {
   uint32_t count;
   bool keys;
   ssize_t *err_offset;
   uint32_t depth;
   bson_string_t *str;
   bson_json_mode_t mode;
   int32_t max_len;
   bool max_len_reached;
} bson_json_state_t;
*/

typedef struct 
{
	uint32_t count;
	bool keys;
	ssize_t *err_offset;
	uint32_t depth;
	bson_string_t *str;
	bson_json_mode_t mode;
	int32_t max_len;
	bool max_len_reached;
} native_bson_json_state_t;

class bson_iterator_native_ex
{
private:
	typedef bson_iterator_native_ex this_type;

public:
	//static const bson_visitor_t& default_bson_as_json_visitors(void);
	inline static const bson_visitor_t& default_bson_as_json_visitors(void)
	{
		return this_type::_s_bson_as_json_visitors;
	}

	static bool s_bson_iter_next(bson_iter_t* iter, uint32_t next_keylen,
									const char** key, uint32_t* bson_type, bool* unsupported);

	inline static bool s_bson_iter_next(bson_iter_t* iter)
	{
		uint32_t bson_type = 0;
		const char* key = 0;
		bool unsupported = false;

		return this_type::s_bson_iter_next(iter, 0, &key, &bson_type, &unsupported);
	}

	static bool s_bson_as_json_visit_utf8(const bson_iter_t* iter, const char* key, 
											size_t v_utf8_len, const char* v_utf8, void* data);
	static bool s_bson_as_json_visit_int32(const bson_iter_t* iter, const char* key, 
											int32_t v_int32, void* data);
	static bool s_bson_as_json_visit_int64(const bson_iter_t* iter, const char* key, 
											int64_t v_int64, void* data);
	static bool s_bson_as_json_visit_decimal128(const bson_iter_t* iter, const char* key, 
												const bson_decimal128_t* v_dec128, void* data);
	static bool s_bson_as_json_visit_double(const bson_iter_t* iter, const char* key, 
											double v_double, void* data);

	static bool s_bson_as_json_visit_undefined(const bson_iter_t* iter, const char* key, void* data);
	static bool s_bson_as_json_visit_null(const bson_iter_t* iter, const char* key, void* data);
	static bool s_bson_as_json_visit_oid(const bson_iter_t* iter, const char* key, const bson_oid_t* oid, void* data);
	static bool s_bson_as_json_visit_binary(const bson_iter_t* iter, const char* key, bson_subtype_t v_subtype,
											size_t v_binary_len, const uint8_t* v_binary, void* data);
	static bool s_bson_as_json_visit_bool(const bson_iter_t* iter, const char* key, bool v_bool, void* data);
	static bool s_bson_as_json_visit_date_time(const bson_iter_t* iter, const char* key, int64_t msec_since_epoch, void* data);
	static bool s_bson_as_json_visit_regex(const bson_iter_t* iter, const char* key, 
											const char* v_regex, const char* v_options, void* data);	
	static bool s_bson_as_json_visit_timestamp(const bson_iter_t* iter, const char* key, 
												uint32_t v_timestamp, uint32_t v_increment, void* data);
	static bool s_bson_as_json_visit_dbpointer(const bson_iter_t* iter, const char* key, size_t v_collection_len,
												const char* v_collection, const bson_oid_t* v_oid, void* data);
	static bool s_bson_as_json_visit_minkey(const bson_iter_t* iter, const char* key, void* data);
	static bool s_bson_as_json_visit_maxkey(const bson_iter_t* iter, const char* key, void* data);
	static bool s_bson_as_json_visit_before(const bson_iter_t* iter, const char* key, void* data);
	static bool s_bson_as_json_visit_after(const bson_iter_t* iter, const char* key, void* data);
	static void s_bson_as_json_visit_corrupt(const bson_iter_t* iter, void* data);
	
	static bool s_bson_as_json_visit_code(const bson_iter_t* iter, const char* key, size_t v_code_len, const char* v_code, void* data);
	static bool s_bson_as_json_visit_symbol(const bson_iter_t* iter, const char* key, size_t v_symbol_len, const char* v_symbol, void* data);
	static bool s_bson_as_json_visit_codewscope(const bson_iter_t* iter, const char* key, 
													size_t v_code_len, const char* v_code, const bson_t* v_scope, void* data);
	static bool s_bson_as_json_visit_document(const bson_iter_t* iter, const char* key, const bson_t* v_document, void* data);
	static bool s_bson_as_json_visit_array(const bson_iter_t* iter, const char* key, const bson_t* v_array, void* data);
	static bool s_bson_iter_visit_all(bson_iter_t* iter, const bson_visitor_t* visitor, void* data);

	static bool s_bson_iter_validate(const bson_iter_t* piter); 
	static u8 s_bson_iter_type(const bson_iter_t* piter);
	static const char* s_bson_iter_key(const bson_iter_t* piter);

	static bool s_bson_iter_cmp_eq(const bson_iter_t* pl, const bson_iter_t* pr);

	static bool s_bson_iter_is_root(const bson_iter_t* piter);

	static const u8* s_bson_iter_value_t_pointer(const bson_iter_t* piter, s32& len);
	static const char* s_bson_iter_key_pointer(const bson_iter_t* piter, s32& len);
	static const u8* s_bson_iter_value_pointer(const bson_iter_t* piter, s32& len);

	static bool s_bson_iter_swap(bson_iter_t* pl, bson_iter_t* pr);

	static u32 s_bson_iter_key_size(const bson_iter_t* piter);
	static u32 s_bson_iter_key_buffer_size(const bson_iter_t* piter);
	static u32 s_bson_iter_d1_size(const bson_iter_t* piter);
	static u32 s_bson_iter_d2_size(const bson_iter_t* piter);
	static u32 s_bson_iter_d3_size(const bson_iter_t* piter);
	static u32 s_bson_iter_d4_size(const bson_iter_t* piter);

	static bool s_bson_iter_make_end(bson_iter_t* pend, const bson_iter_t* pnow);

	static bool s_bson_iter_find_descendant(bson_iter_t* iter, const char* dotkey, bson_iter_t* descendant);
	static bool s_bson_iter_find_descendant_case(bson_iter_t* iter, const char * dotkey, bson_iter_t* descendant);

private:
	static bool s_bson_iter_find_with_len(bson_iter_t* iter, const char* key, int keylen);
	static bool s_bson_iter_find_case_with_len(bson_iter_t* iter, const char* key, int keylen);

private:
	static const bson_visitor_t _s_bson_as_json_visitors;
};

} // namesapce nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{


void swap(bson_iter_t& l, bson_iter_t& r);

template<typename BsonIter> inline
typename boost::enable_if<boost::is_base_of<bson_iter_t, BsonIter>, void>::type
	swap(bson_iter_t& l, BsonIter& r)
{
	bson_iterator_native_ex::s_bson_iter_swap(boost::addressof(l), boost::addressof(r));
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

#endif // __YGGR_NSQL_DATABASE_SYSTEM_BSON_ITER_NATIVE_HPP__
