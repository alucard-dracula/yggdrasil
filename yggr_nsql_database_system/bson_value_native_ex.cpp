//bson_value_native_ex.cpp

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

#include <yggr/nsql_database_system/bson_value_native_ex.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

/*static*/
void bson_value_native_ex::s_bson_value_clear(bson_value_t* pval)
{
	if(!(pval && pval->value_type))
	{
		return;
	}

	bson_value_destroy(pval);
	memset(pval, 0, sizeof(bson_value_t));
}

/*static*/
bool bson_value_native_ex::s_bson_value_swap(bson_value_t* l, bson_value_t* r)
{
	assert(l && r);
	if(!(l && r))
	{
		return false;
	}

	if(l != r)
	{
		bson_value_t tmp = {static_cast<bson_type_t>(0)};
		memcpy(&tmp, l, sizeof(bson_value_t));
		memcpy(l, r, sizeof(bson_value_t));
		memcpy(r, &tmp, sizeof(bson_value_t));
	}

	return true;
}

/*static*/
bool bson_value_native_ex::s_bson_value_copy(bson_value_t* pdst, const bson_value_t* psrc)
{
	assert(pdst && psrc);
	if(!(pdst && psrc))
	{
		return false;
	}

	if(pdst != psrc)
	{
		this_type::s_bson_value_clear(pdst);
		if(psrc->value_type)
		{
			bson_value_copy(psrc, pdst);
		}
	}

	return true;
}

// to_value
/*static*/
bool bson_value_native_ex::s_int32_to_value(bson_value_t* pdst, s32 val)
{
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

	pdst->value_type = static_cast<bson_type_t>(bson_typeid_def::E_BSON_TYPE_INT32);
	pdst->value.v_int32 = val;
	return true;
}

/*static*/
bool bson_value_native_ex::s_int64_to_value(bson_value_t* pdst, s64 val)
{
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

	pdst->value_type = static_cast<bson_type_t>(bson_typeid_def::E_BSON_TYPE_INT64);
	pdst->value.v_int64 = val;
	return true;
}

/*static*/
bool bson_value_native_ex::s_bool_to_value(bson_value_t* pdst, bool val)
{
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

	pdst->value_type = static_cast<bson_type_t>(bson_typeid_def::E_BSON_TYPE_BOOL);
	pdst->value.v_bool = val;
	return true;
}

/*static*/
bool bson_value_native_ex::s_double_to_value(bson_value_t* pdst, f64 val)
{
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

	pdst->value_type = static_cast<bson_type_t>(bson_typeid_def::E_BSON_TYPE_DOUBLE);
	pdst->value.v_double = val;
	return true;
}

/*static*/
bool bson_value_native_ex::s_null_to_value(bson_value_t* pdst)
{
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

	pdst->value_type = static_cast<bson_type_t>(bson_typeid_def::E_BSON_TYPE_NULL);
	return true;
}

/*static*/
bool bson_value_native_ex::s_undefined_to_value(bson_value_t* pdst)
{
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

	pdst->value_type = static_cast<bson_type_t>(bson_typeid_def::E_BSON_TYPE_UNDEFINED);
	return true;
}

/*static*/
bool bson_value_native_ex::s_maxkey_to_value(bson_value_t* pdst)
{
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

	pdst->value_type = static_cast<bson_type_t>(bson_typeid_def::E_BSON_TYPE_MAXKEY);
	return true;
}

/*static*/
bool bson_value_native_ex::s_minkey_to_value(bson_value_t* pdst)
{
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

	pdst->value_type = static_cast<bson_type_t>(bson_typeid_def::E_BSON_TYPE_MINKEY);
	return true;
}

/*static*/
bool bson_value_native_ex::s_date_to_value(bson_value_t* pdst, s64 val)
{
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

	pdst->value_type = static_cast<bson_type_t>(bson_typeid_def::E_BSON_TYPE_DATE_TIME);
	pdst->value.v_datetime = val;
	return true;
}

/*static*/
bool bson_value_native_ex::s_time_to_value(bson_value_t* pdst, s64 val)
{
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

	pdst->value_type = static_cast<bson_type_t>(bson_typeid_def::E_BSON_TYPE_DATE_TIME);
	pdst->value.v_datetime = val * time::time_ratio_def::E_S_AND_MS_RATIO;
	return true;
}

/*static*/
bool bson_value_native_ex::s_timeval_to_value(bson_value_t* pdst, const timeval* val)
{
	assert(pdst && val);
	if(!(pdst && val))
	{
		return false;
	}

	//if(pdst->value_type)
	//{
	//	bson_value_destroy(pdst);
	//}
	//memset(pdst, 0, sizeof(bson_value_t));

	this_type::s_bson_value_clear(pdst);

	pdst->value_type = static_cast<bson_type_t>(bson_typeid_def::E_BSON_TYPE_DATE_TIME);
	pdst->value.v_datetime =
		static_cast<s64>(val->tv_sec) * time::time_ratio_def::E_S_AND_MS_RATIO
		+ static_cast<s64>(val->tv_usec) / time::time_ratio_def::E_MS_AND_MCS_RATIO ;
	return true;
}

/*static*/
bool bson_value_native_ex::s_oid_to_value(bson_value_t* pdst, const bson_oid_t* val)
{
	assert(pdst && val);
	if(!(pdst && val))
	{
		return false;
	}

	//if(pdst->value_type)
	//{
	//	bson_value_destroy(pdst);
	//}
	//memset(pdst, 0, sizeof(bson_value_t));

	this_type::s_bson_value_clear(pdst);

	pdst->value_type = static_cast<bson_type_t>(bson_typeid_def::E_BSON_TYPE_OID);
	memcpy(&(pdst->value.v_oid), val, bson_oid_native_ex::E_bson_oid_byte_size);

	return true;
}

/*static*/
bool bson_value_native_ex::s_binary_buffer_to_value(bson_value_t* pdst, u32 sub_t, const u8* val, u64 size)
{
	assert(pdst && val);
	if(!(pdst && val))
	{
		return false;
	}

	this_type::s_bson_value_clear(pdst);

	pdst->value_type = static_cast<bson_type_t>(bson_typeid_def::E_BSON_TYPE_BINARY);
	pdst->value.v_binary.subtype = static_cast<bson_subtype_t>(sub_t);
	pdst->value.v_binary.data_len = static_cast<uint32_t>(size);
	if(size)
	{
		pdst->value.v_binary.data = reinterpret_cast<uint8_t*>(bson_malloc(size));
		memcpy(pdst->value.v_binary.data, val, size);
	}

	return true;
}

/*static*/
bool bson_value_native_ex::s_timestamp_to_value(bson_value_t* pdst, u32 timestamp, u32 increment)
{
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

	pdst->value_type = static_cast<bson_type_t>(bson_typeid_def::E_BSON_TYPE_TIMESTAMP);
	pdst->value.v_timestamp.timestamp = timestamp;
	pdst->value.v_timestamp.increment = increment;
	return true;
}

/*static*/
bool bson_value_native_ex::s_bson_to_value(bson_value_t* pdst, const bson_t* pval)
{
	assert(pdst && pval && pval->len);
	if(!(pdst && pval && pval->len))
	{
		return false;
	}

	this_type::s_bson_value_clear(pdst);

	pdst->value_type = static_cast<bson_type_t>(bson_typeid_def::E_BSON_TYPE_DOCUMENT);

	pdst->value.v_doc.data = reinterpret_cast<uint8_t*>(bson_malloc(pval->len));
	memcpy(pdst->value.v_doc.data, bson_native_ex::s_bson_data(pval), pval->len);
	pdst->value.v_doc.data_len = pval->len;
	return true;
}

/*static*/
bool bson_value_native_ex::s_array_to_value(bson_value_t* pdst, const bson_t* pval)
{
	assert(pdst && pval && pval->len);
	if(!(pdst && pval && pval->len))
	{
		return false;
	}

	this_type::s_bson_value_clear(pdst);

	pdst->value_type = static_cast<bson_type_t>(bson_typeid_def::E_BSON_TYPE_ARRAY);

	pdst->value.v_doc.data = reinterpret_cast<uint8_t*>(bson_malloc(pval->len));
	memcpy(pdst->value.v_doc.data, bson_native_ex::s_bson_data(pval), pval->len);
	pdst->value.v_doc.data_len = pval->len;
	return true;
}

/*static*/
bool bson_value_native_ex::s_decimal128_to_value(bson_value_t* pdst, const bson_decimal128_t* pval)
{
	assert(pdst && pval);
	if(!(pdst && pval))
	{
		return false;
	}

	//if(pdst->value_type)
	//{
	//	bson_value_destroy(pdst);
	//}
	//memset(pdst, 0, sizeof(bson_value_t));

	this_type::s_bson_value_clear(pdst);

	pdst->value_type = static_cast<bson_type_t>(bson_typeid_def::E_BSON_TYPE_DECIMAL128);
	pdst->value.v_decimal128.low = pval->low;
	pdst->value.v_decimal128.high = pval->high;
	return true;
}

//---------------------------------------
// value_to
/*static*/
bool bson_value_native_ex::s_value_to_int32(s32& val, const bson_value_t* psrc)
{
	assert(psrc);
	if(!psrc)
	{
		return false;
	}

	if(static_cast<u32>(psrc->value_type) != bson_typeid_def::E_BSON_TYPE_INT32)
	{
		return false;
	}

	val = psrc->value.v_int32;
	return true;
}

/*static*/
bool bson_value_native_ex::s_value_to_int64(s64& val, const bson_value_t* psrc)
{
	assert(psrc);
	if(!psrc)
	{
		return false;
	}

	if(static_cast<u32>(psrc->value_type) != bson_typeid_def::E_BSON_TYPE_INT64)
	{
		return false;
	}

	val = psrc->value.v_int64;
	return true;
}

/*static*/
bool bson_value_native_ex::s_value_to_bool(bool& val, const bson_value_t* psrc)
{
	assert(psrc);
	if(!psrc)
	{
		return false;
	}

	if(static_cast<u32>(psrc->value_type) != bson_typeid_def::E_BSON_TYPE_BOOL)
	{
		return false;
	}

	val = psrc->value.v_bool;
	return true;
}

/*static*/
bool bson_value_native_ex::s_value_to_double(f64& val, const bson_value_t* psrc)
{
	assert(psrc);
	if(!psrc)
	{
		return false;
	}

	if(static_cast<u32>(psrc->value_type) != bson_typeid_def::E_BSON_TYPE_DOUBLE)
	{
		return false;
	}

	val = psrc->value.v_double;
	return true;
}

/*static*/
bool bson_value_native_ex::s_value_to_null(const bson_value_t* psrc)
{
	assert(psrc);
	if(!psrc)
	{
		return false;
	}

	if(static_cast<u32>(psrc->value_type) != bson_typeid_def::E_BSON_TYPE_NULL)
	{
		return false;
	}

	return true;
}

/*static*/
bool bson_value_native_ex::s_value_to_undefined(const bson_value_t* psrc)
{
	assert(psrc);
	if(!psrc)
	{
		return false;
	}

	if(static_cast<u32>(psrc->value_type) != bson_typeid_def::E_BSON_TYPE_UNDEFINED)
	{
		return false;
	}

	return true;
}

/*static*/
bool bson_value_native_ex::s_value_to_maxkey(const bson_value_t* psrc)
{
	assert(psrc);
	if(!psrc)
	{
		return false;
	}

	if(static_cast<u32>(psrc->value_type) != bson_typeid_def::E_BSON_TYPE_MAXKEY)
	{
		return false;
	}

	return true;
}

/*static*/
bool bson_value_native_ex::s_value_to_minkey(const bson_value_t* psrc)
{
	assert(psrc);
	if(!psrc)
	{
		return false;
	}

	if(static_cast<u32>(psrc->value_type) != bson_typeid_def::E_BSON_TYPE_MINKEY)
	{
		return false;
	}

	return true;
}

/*static*/
bool bson_value_native_ex::s_value_to_date(s64& val, const bson_value_t* psrc)
{
	assert(psrc);
	if(!psrc)
	{
		return false;
	}

	if(static_cast<u32>(psrc->value_type) != bson_typeid_def::E_BSON_TYPE_DATE_TIME)
	{
		return false;
	}

	val = psrc->value.v_datetime;
	return true;
}

/*static*/
bool bson_value_native_ex::s_value_to_time(s64& val, const bson_value_t* psrc)
{
	assert(psrc);
	if(!psrc)
	{
		return false;
	}

	if(static_cast<u32>(psrc->value_type) != bson_typeid_def::E_BSON_TYPE_DATE_TIME)
	{
		return false;
	}

	val = psrc->value.v_datetime / time::time_ratio_def::E_S_AND_MS_RATIO;
	return true;
}

/*static*/
bool bson_value_native_ex::s_value_to_timeval(timeval* pval, const bson_value_t* psrc)
{
#ifdef BSON_OS_WIN32
	typedef long timeval_member_type;
#else
	typedef std::time_t timeval_member_type;
#endif // BSON_OS_WIN32
	assert(psrc && pval);
	if(!(psrc && pval))
	{
		return false;
	}

	if(static_cast<u32>(psrc->value_type) != bson_typeid_def::E_BSON_TYPE_DATE_TIME)
	{
		return false;
	}

	pval->tv_sec =
		static_cast<timeval_member_type>(
			psrc->value.v_datetime / time::time_ratio_def::E_S_AND_MS_RATIO);
	pval->tv_usec =
		static_cast<timeval_member_type>(
			(psrc->value.v_datetime % time::time_ratio_def::E_S_AND_MS_RATIO) * time::time_ratio_def::E_MS_AND_MCS_RATIO);
	return true;
}

/*static*/
bool bson_value_native_ex::s_value_to_oid(bson_oid_t* pval, const bson_value_t* psrc)
{
	assert(psrc && pval);
	if(!(psrc && pval))
	{
		return false;
	}

	if(static_cast<u32>(psrc->value_type) != bson_typeid_def::E_BSON_TYPE_OID)
	{
		return false;
	}

	memcpy(pval, boost::addressof(psrc->value.v_oid), bson_oid_native_ex::E_bson_oid_byte_size);
	return true;
}

/*static*/
bool bson_value_native_ex::s_value_to_timestamp(u32& timestamp, u32& increment, const bson_value_t* psrc)
{
	assert(psrc);
	if(!psrc)
	{
		return false;
	}

	if(static_cast<u32>(psrc->value_type) != bson_typeid_def::E_BSON_TYPE_TIMESTAMP)
	{
		return false;
	}

	timestamp = psrc->value.v_timestamp.timestamp;
	increment = psrc->value.v_timestamp.increment;

	return true;
}

/*static*/
bool bson_value_native_ex::s_value_to_bson(bson_t* pval, const bson_value_t* psrc)
{
	assert(psrc);
	if(!psrc)
	{
		return false;
	}

	if(static_cast<u32>(psrc->value_type) != bson_typeid_def::E_BSON_TYPE_DOCUMENT)
	{
		return false;
	}

	bson_native_ex::s_bson_init_from_data(pval, psrc->value.v_doc.data, psrc->value.v_doc.data_len);
	return true;
}

/*static*/
bool bson_value_native_ex::s_value_to_array(bson_t* pval, const bson_value_t* psrc)
{
	assert(psrc);
	if(!psrc)
	{
		return false;
	}

	if(static_cast<u32>(psrc->value_type) != bson_typeid_def::E_BSON_TYPE_ARRAY)
	{
		return false;
	}

	bson_native_ex::s_bson_init_from_data(pval, psrc->value.v_doc.data, psrc->value.v_doc.data_len);
	return true;
}

/*static*/
bool bson_value_native_ex::s_value_to_bson_or_array(bson_t* pval, const bson_value_t* psrc)
{
	assert(psrc);
	if(!psrc)
	{
		return false;
	}

	if(!(psrc->value_type == bson_typeid_def::E_BSON_TYPE_DOCUMENT
			|| psrc->value_type == bson_typeid_def::E_BSON_TYPE_ARRAY))
	{
		return false;
	}

	bson_native_ex::s_bson_init_from_data(pval, psrc->value.v_doc.data, psrc->value.v_doc.data_len);
	return true;
}

/*static*/
bool bson_value_native_ex::s_value_to_decimal128(bson_decimal128_t* pval, const bson_value_t* psrc)
{
	assert(pval && psrc);
	if(!(pval && psrc))
	{
		return false;
	}

	if(static_cast<u32>(psrc->value_type) != bson_typeid_def::E_BSON_TYPE_DECIMAL128)
	{
		return false;
	}

	pval->low = psrc->value.v_decimal128.low;
	pval->high = psrc->value.v_decimal128.high;
	return true;
}

//-------------------
// comparer

/*static*/
bool bson_value_native_ex::s_value_cmp_eq_bool(const bson_value_t* l, bool r)
{
	assert(l);
	return (l)
			&& (static_cast<u32>(l->value_type) == bson_typeid_def::E_BSON_TYPE_BOOL)
			&& (l->value.v_bool == r);
}

/*static*/
s32 bson_value_native_ex::s_value_cmp_bool(const bson_value_t* l, bool r)
{
	assert(l && (static_cast<u32>(l->value_type) == bson_typeid_def::E_BSON_TYPE_BOOL));

	if(!(static_cast<u32>(l->value_type) == bson_typeid_def::E_BSON_TYPE_BOOL))
	{
		bson_error_t bs_err;
		this_type::pro_s_make_bson_validate_diff_t_err(bs_err, l->value_type, bson_typeid_def::E_BSON_TYPE_BOOL);
		throw bs_err;
	}

	return (l->value.v_bool == r)? 0 : (l->value.v_bool < r)? -1 : 1;
}

/*static*/
bool bson_value_native_ex::s_value_cmp_eq_int32(const bson_value_t* l, s32 r)
{
	assert(l);
	return (l)
			&& (static_cast<u32>(l->value_type) == bson_typeid_def::E_BSON_TYPE_INT32)
			&& (l->value.v_int32 == r);
}

/*static*/
s32 bson_value_native_ex::s_value_cmp_int32(const bson_value_t* l, s32 r)
{
	assert(l && (static_cast<u32>(l->value_type) == bson_typeid_def::E_BSON_TYPE_INT32));

	if(!(static_cast<u32>(l->value_type) == bson_typeid_def::E_BSON_TYPE_INT32))
	{
		bson_error_t bs_err;
		this_type::pro_s_make_bson_validate_diff_t_err(bs_err, l->value_type, bson_typeid_def::E_BSON_TYPE_INT32);
		throw bs_err;
	}

	return (l->value.v_int32 == r)? 0 : (l->value.v_int32 < r)? -1 : 1;
}

/*static*/
bool bson_value_native_ex::s_value_cmp_eq_int64(const bson_value_t* l, s64 r)
{
	assert(l);
	return (l)
			&& (static_cast<u32>(l->value_type) == bson_typeid_def::E_BSON_TYPE_INT64)
			&& (l->value.v_int64 == r);
}

/*static*/
s32 bson_value_native_ex::s_value_cmp_int64(const bson_value_t* l, s64 r)
{
	assert(l && (static_cast<u32>(l->value_type) == bson_typeid_def::E_BSON_TYPE_INT64));

	if(!(static_cast<u32>(l->value_type) == bson_typeid_def::E_BSON_TYPE_INT64))
	{
		bson_error_t bs_err;
		this_type::pro_s_make_bson_validate_diff_t_err(bs_err, l->value_type, bson_typeid_def::E_BSON_TYPE_INT64);
		throw bs_err;
	}

	return (l->value.v_int64 == r)? 0 : (l->value.v_int64 < r)? -1 : 1;
}

/*static*/
bool bson_value_native_ex::s_value_cmp_eq_double(const bson_value_t* l, f64 r)
{
	assert(l);
	return (l)
			&& (static_cast<u32>(l->value_type) == bson_typeid_def::E_BSON_TYPE_DOUBLE)
			&& (l->value.v_double == r);
}

/*static*/
s32 bson_value_native_ex::s_value_cmp_double(const bson_value_t* l, f64 r)
{
	assert(l && (static_cast<u32>(l->value_type) == bson_typeid_def::E_BSON_TYPE_DOUBLE));

	if(!(static_cast<u32>(l->value_type) == bson_typeid_def::E_BSON_TYPE_DOUBLE))
	{
		bson_error_t bs_err;
		this_type::pro_s_make_bson_validate_diff_t_err(bs_err, l->value_type, bson_typeid_def::E_BSON_TYPE_DOUBLE);
		throw bs_err;
	}

	return (l->value.v_double == r)? 0 : (l->value.v_double < r)? -1 : 1;
}

/*static*/
bool bson_value_native_ex::s_value_cmp_eq_null(const bson_value_t* l)
{
	assert(l);
	return (l)
			&& (static_cast<u32>(l->value_type) == bson_typeid_def::E_BSON_TYPE_NULL);
}

/*static*/
bool bson_value_native_ex::s_value_cmp_eq_undefined(const bson_value_t* l)
{
	assert(l);
	return (l)
			&& (static_cast<u32>(l->value_type) == bson_typeid_def::E_BSON_TYPE_UNDEFINED);
}

/*static*/
bool bson_value_native_ex::s_value_cmp_eq_maxkey(const bson_value_t* l)
{
	assert(l);
	return (l)
			&& (static_cast<u32>(l->value_type) == bson_typeid_def::E_BSON_TYPE_MAXKEY);
}

/*static*/
bool bson_value_native_ex::s_value_cmp_eq_minkey(const bson_value_t* l)
{
	assert(l);
	return (l)
			&& (static_cast<u32>(l->value_type) == bson_typeid_def::E_BSON_TYPE_MINKEY);
}

/*static*/
bool bson_value_native_ex::s_value_cmp_eq_date(const bson_value_t* l, s64 r)
{
	assert(l);
	return (l)
			&& (static_cast<u32>(l->value_type) == bson_typeid_def::E_BSON_TYPE_DATE_TIME)
			&& (l->value.v_datetime == r);
}

/*static*/
s32 bson_value_native_ex::s_value_cmp_date(const bson_value_t* l, s64 r)
{
	assert(l && (static_cast<u32>(l->value_type) == bson_typeid_def::E_BSON_TYPE_DATE_TIME));

	if(!(static_cast<u32>(l->value_type) == bson_typeid_def::E_BSON_TYPE_DATE_TIME))
	{
		bson_error_t bs_err;
		this_type::pro_s_make_bson_validate_diff_t_err(bs_err, l->value_type, bson_typeid_def::E_BSON_TYPE_DATE_TIME);
		throw bs_err;
	}

	return (l->value.v_datetime == r)? 0 : (l->value.v_datetime < r)? -1 : 1;
}

/*static*/
bool bson_value_native_ex::s_value_cmp_eq_time(const bson_value_t* l, s64 r)
{
	return this_type::s_value_cmp_eq_date(l, r * time::time_ratio_def::E_S_AND_MS_RATIO);
}

/*static*/
s32 bson_value_native_ex::s_value_cmp_time(const bson_value_t* l, s64 r)
{
	return this_type::s_value_cmp_date(l, r * time::time_ratio_def::E_S_AND_MS_RATIO);
}

/*static*/
bool bson_value_native_ex::s_value_cmp_eq_timeval(const bson_value_t* l, const timeval* r)
{
	return
		this_type::s_value_cmp_eq_date(
			l,
			static_cast<s64>(r->tv_sec) * time::time_ratio_def::E_S_AND_MS_RATIO
			+ static_cast<s64>(r->tv_usec) / time::time_ratio_def::E_MS_AND_MCS_RATIO);
}

/*static*/
s32 bson_value_native_ex::s_value_cmp_timeval(const bson_value_t* l, const timeval* r)
{
	return
		this_type::s_value_cmp_date(
			l,
			static_cast<s64>(r->tv_sec) * time::time_ratio_def::E_S_AND_MS_RATIO
			+ static_cast<s64>(r->tv_usec) / time::time_ratio_def::E_MS_AND_MCS_RATIO);
}

///*static*/
//bool bson_value_native_ex::s_value_cmp_eq_oid(const bson_value_t* l, const bson_oid_t* r)
//{
//	assert(l && r);
//	return (l) && (r)
//			&& (static_cast<u32>(l->value_type) == bson_typeid_def::E_BSON_TYPE_OID)
//			&& (0 == memcmp(boost::addressof(l->value.v_oid), r, bson_oid_native_ex::E_bson_oid_byte_size));
//}

///*static*/
//s32 bson_value_native_ex::s_value_cmp_oid(const bson_value_t* l, const bson_oid_t* r)
//{
//	assert(l && r && (static_cast<u32>(l->value_type) == bson_typeid_def::E_BSON_TYPE_OID));
//
//	if(!(static_cast<u32>(l->value_type) == bson_typeid_def::E_BSON_TYPE_OID))
//	{
//		bson_error_t bs_err;
//		this_type::pro_s_make_bson_validate_diff_t_err(bs_err, l->value_type, bson_typeid_def::E_BSON_TYPE_OID);
//		throw bs_err;
//	}
//	return memcmp(boost::addressof(l->value.v_oid), r, bson_oid_native_ex::E_bson_oid_byte_size);
//}

/*static*/
bool bson_value_native_ex::s_value_cmp_eq_binary_buffer(const bson_value_t* l, u32 sub_t, const u8* r, u64 size)
{
	assert(l);
	assert(r);
	return (l) && (r)
			&& (static_cast<u32>(l->value_type) == bson_typeid_def::E_BSON_TYPE_BINARY)
			&& (l->value.v_binary.subtype == sub_t)
			&& (l->value.v_binary.data_len == size)
			&& (0 == memcmp(l->value.v_binary.data, r, size));
}

/*static*/
bool bson_value_native_ex::s_value_cmp_eq_timestamp(const bson_value_t* l, u32 timestamp, u32 increment)
{
	assert(l);
	return (l)
			&& (static_cast<u32>(l->value_type) == bson_typeid_def::E_BSON_TYPE_TIMESTAMP)
			&& (l->value.v_timestamp.timestamp == timestamp)
			&& (l->value.v_timestamp.increment == increment);
}

/*static*/
bool bson_value_native_ex::s_value_cmp_eq_decimal128(const bson_value_t* l, const bson_decimal128_t* r)
{
	assert(l && r);
	return
		(l && r)
		&& (static_cast<u32>(l->value_type) == bson_typeid_def::E_BSON_TYPE_DECIMAL128)
		&& (l->value.v_decimal128.low == r->low)
		&& (l->value.v_decimal128.high == r->high);
}

/*static*/
s32 bson_value_native_ex::s_value_cmp_decimal128(const bson_value_t* l, const bson_decimal128_t* r)
{
	assert((l) && (r) && (static_cast<u32>(l->value_type) == bson_typeid_def::E_BSON_TYPE_DECIMAL128) );

	if(!(static_cast<u32>(l->value_type) == bson_typeid_def::E_BSON_TYPE_DECIMAL128))
	{
		bson_error_t bs_err;
		this_type::pro_s_make_bson_validate_diff_t_err(bs_err, l->value_type, bson_typeid_def::E_BSON_TYPE_DECIMAL128);
		throw bs_err;
	}

	return
		(l->value.v_decimal128.high == r->high)?
			(l->value.v_decimal128.low == r->low)?
				0
				: (l->value.v_decimal128.low < r->low)? -1 : 1
			: (l->value.v_decimal128.high < r->high)? -1 : 1;
}

/*static*/
bool bson_value_native_ex::s_value_cmp_eq_bson(const bson_value_t* l, const bson_t* r)
{
	assert(l);
	assert(r);

	bson_t tmp = {0};
	/*bool binit = */bson_init_static(&tmp, l->value.v_doc.data, l->value.v_doc.data_len);

	bool ret = (l) && (r)
				&& (static_cast<u32>(l->value_type) == bson_typeid_def::E_BSON_TYPE_DOCUMENT)
				&& (bson_equal(&tmp, r));

	bson_destroy(&tmp);

	return ret;
}

/*static*/
bool bson_value_native_ex::s_value_cmp_eq_array(const bson_value_t* l, const bson_t* r)
{
	assert(l);
	assert(r);

	bson_t tmp = {0};
	/*bool binit = */bson_init_static(&tmp, l->value.v_doc.data, l->value.v_doc.data_len);

	bool ret = (l) && (r)
				&& (static_cast<u32>(l->value_type) == bson_typeid_def::E_BSON_TYPE_ARRAY)
				&& (bson_equal(&tmp, r));

	bson_destroy(&tmp);

	return ret;
}

/*static*/
bool bson_value_native_ex::s_value_cmp_eq_value(const bson_value_t* l, const bson_value_t* r)
{
	assert((l && r));
	if(l == r)
	{
		return true;
	}

	if(l->value_type != r->value_type)
	{
		return false;
	}

	switch(l->value_type)
	{
	case bson_typeid_def::E_BSON_TYPE_EOD:
	case bson_typeid_def::E_BSON_TYPE_NULL:
	case bson_typeid_def::E_BSON_TYPE_UNDEFINED:
	case bson_typeid_def::E_BSON_TYPE_MAXKEY:
	case bson_typeid_def::E_BSON_TYPE_MINKEY:
		return true;

	case bson_typeid_def::E_BSON_TYPE_INT32:
		return l->value.v_int32 == r->value.v_int32;

	case bson_typeid_def::E_BSON_TYPE_INT64:
		return l->value.v_int64 == r->value.v_int64;

	case bson_typeid_def::E_BSON_TYPE_BOOL:
		return l->value.v_bool == r->value.v_bool;

	case bson_typeid_def::E_BSON_TYPE_DOUBLE:
		return l->value.v_double == r->value.v_double;

	case bson_typeid_def::E_BSON_TYPE_UTF8:
		return (l->value.v_utf8.str == r->value.v_utf8.str)
				|| ((l->value.v_utf8.len == r->value.v_utf8.len)
					&& (0 == memcmp(l->value.v_utf8.str,
									r->value.v_utf8.str,
									l->value.v_utf8.len)));

	case bson_typeid_def::E_BSON_TYPE_DOCUMENT:
	case bson_typeid_def::E_BSON_TYPE_ARRAY:
		return (l->value.v_doc.data == r->value.v_doc.data)
				|| ((l->value.v_doc.data_len == r->value.v_doc.data_len)
					&& (0 == memcmp(l->value.v_doc.data,
									r->value.v_doc.data,
									l->value.v_doc.data_len)));

	case bson_typeid_def::E_BSON_TYPE_BINARY:
		return (l->value.v_binary.subtype == r->value.v_binary.subtype)
				&& ((l->value.v_binary.data == r->value.v_binary.data)
					|| ((l->value.v_binary.data_len == r->value.v_binary.data_len)
						&& (0 == memcmp(l->value.v_binary.data,
										r->value.v_binary.data,
										l->value.v_binary.data_len))));

	case bson_typeid_def::E_BSON_TYPE_OID:
		return
			0 == memcmp(
					boost::addressof(l->value.v_oid),
					boost::addressof(r->value.v_oid),
					bson_oid_native_ex::E_bson_oid_byte_size);

	case bson_typeid_def::E_BSON_TYPE_DATE_TIME:
		return l->value.v_datetime == r->value.v_datetime;

	case bson_typeid_def::E_BSON_TYPE_REGEX:
		return (0 == strcmp(l->value.v_regex.regex, r->value.v_regex.regex))
				&& (0 == strcmp(l->value.v_regex.options, r->value.v_regex.options));

	case bson_typeid_def::E_BSON_TYPE_DBPOINTER:
		return ((l->value.v_dbpointer.collection == r->value.v_dbpointer.collection)
					|| ((l->value.v_dbpointer.collection_len == r->value.v_dbpointer.collection_len)
						&& (0 == memcmp(l->value.v_dbpointer.collection,
										r->value.v_dbpointer.collection,
										l->value.v_dbpointer.collection_len))))
				&& (0 == memcmp(boost::addressof(l->value.v_dbpointer.oid),
								boost::addressof(r->value.v_dbpointer.oid),
								bson_oid_native_ex::E_bson_oid_byte_size));

	case bson_typeid_def::E_BSON_TYPE_CODE:
		return (l->value.v_code.code == r->value.v_code.code)
				|| ((l->value.v_code.code_len == r->value.v_code.code_len)
					&& (0 == memcmp(l->value.v_code.code, r->value.v_code.code, l->value.v_code.code_len)));

	case bson_typeid_def::E_BSON_TYPE_SYMBOL:
		return (l->value.v_symbol.symbol == r->value.v_symbol.symbol)
				|| ((l->value.v_symbol.len == r->value.v_symbol.len)
					&& (0 == memcmp(l->value.v_symbol.symbol, r->value.v_symbol.symbol, l->value.v_symbol.len)));

	case bson_typeid_def::E_BSON_TYPE_CODEWSCOPE:
		return ((l->value.v_codewscope.code == r->value.v_codewscope.code)
					|| ((l->value.v_codewscope.code_len == r->value.v_codewscope.code_len)
						&& (0 == memcmp(l->value.v_codewscope.code,
										r->value.v_codewscope.code,
										l->value.v_codewscope.code_len))))
				&& ((l->value.v_codewscope.scope_data == r->value.v_codewscope.scope_data)
					|| ((l->value.v_codewscope.scope_len == r->value.v_codewscope.scope_len)
						&& (0 == memcmp(l->value.v_codewscope.scope_data,
										r->value.v_codewscope.scope_data,
										l->value.v_codewscope.scope_len))));

	case bson_typeid_def::E_BSON_TYPE_TIMESTAMP:
		return (l->value.v_timestamp.timestamp == r->value.v_timestamp.timestamp)
				&& (l->value.v_timestamp.increment == r->value.v_timestamp.increment);

	case bson_typeid_def::E_BSON_TYPE_DECIMAL128:
		return
			(l->value.v_decimal128.high == r->value.v_decimal128.high)
			&& (l->value.v_decimal128.low == r->value.v_decimal128.low);

	default:
		assert(false);
	}

	return false;
}

/*static*/
s32 bson_value_native_ex::s_value_cmp_value(const bson_value_t* l, const bson_value_t* r)
{
	assert((l && r));
	if(l == r)
	{
		return 0;
	}

	assert(l->value_type == r->value_type);
	if(l->value_type != r->value_type)
	{
		bson_error_t bs_err;
		this_type::pro_s_make_bson_validate_diff_t_err(bs_err, l->value_type, r->value_type);
		throw bs_err;
	}

	switch(l->value_type)
	{
	case bson_typeid_def::E_BSON_TYPE_EOD:
	case bson_typeid_def::E_BSON_TYPE_NULL:
	case bson_typeid_def::E_BSON_TYPE_UNDEFINED:
	case bson_typeid_def::E_BSON_TYPE_MAXKEY:
	case bson_typeid_def::E_BSON_TYPE_MINKEY:
		return 0;

	case bson_typeid_def::E_BSON_TYPE_INT32:
		return l->value.v_int32 == r->value.v_int32? 0 : l->value.v_int32 < r->value.v_int32? -1 : 1;

	case bson_typeid_def::E_BSON_TYPE_INT64:
		return l->value.v_int64 == r->value.v_int64? 0 : l->value.v_int64 < r->value.v_int64? -1 : 1;

	case bson_typeid_def::E_BSON_TYPE_BOOL:
		return l->value.v_bool == r->value.v_bool? 0 : l->value.v_bool < r->value.v_bool? -1 : 1;

	case bson_typeid_def::E_BSON_TYPE_DOUBLE:
		return l->value.v_double == r->value.v_double? 0 : l->value.v_double < r->value.v_double? -1 : 1;

	case bson_typeid_def::E_BSON_TYPE_UTF8:
		return
			(l->value.v_utf8.str == r->value.v_utf8.str)?
			0
			: charset::utf8_foo::strcmp(l->value.v_utf8.str, r->value.v_utf8.str);

	case bson_typeid_def::E_BSON_TYPE_DECIMAL128:
		return
			(l->value.v_decimal128.high == r->value.v_decimal128.high)?
				(l->value.v_decimal128.low == r->value.v_decimal128.low)?
					0
					: (l->value.v_decimal128.low < r->value.v_decimal128.low)? -1 : 1
				: (l->value.v_decimal128.high < r->value.v_decimal128.high)? -1 : 1;

	case bson_typeid_def::E_BSON_TYPE_OID:
	    // in linux gcc memcmp return value is not formated, so using math sign fora
		return
            math::sign(
                memcmp(
                    boost::addressof(l->value.v_oid),
                    boost::addressof(r->value.v_oid),
                    bson_oid_native_ex::E_bson_oid_byte_size));

	case bson_typeid_def::E_BSON_TYPE_DATE_TIME:
		return l->value.v_datetime == r->value.v_datetime? 0 : l->value.v_datetime < r->value.v_datetime? -1 : 1;

	//case bson_typeid_def::E_BSON_TYPE_DOCUMENT:
	//case bson_typeid_def::E_BSON_TYPE_ARRAY:
	//case bson_typeid_def::E_BSON_TYPE_BINARY:
	//case bson_typeid_def::E_BSON_TYPE_REGEX:
	//case bson_typeid_def::E_BSON_TYPE_DBPOINTER:
	//case bson_typeid_def::E_BSON_TYPE_CODE:
	//case bson_typeid_def::E_BSON_TYPE_SYMBOL:
	//case bson_typeid_def::E_BSON_TYPE_CODEWSCOPE:
	//case bson_typeid_def::E_BSON_TYPE_TIMESTAMP:
	default:
		{
			bson_error_t bs_err;
			this_type::pro_s_make_bson_validate_only_support_cmp_eq_t_err(bs_err, l->value_type);
			throw bs_err;
		}
	}

	assert(false);
	return -1;
}

//protected:
/*static*/
bson_error_t& bson_value_native_ex::pro_s_make_bson_validate_diff_t_err(bson_error_t& err, u32 lt, u32 rt)
{
	bson_error_native_ex::s_bson_error_init(
		boost::addressof(err),
		BSON_ERROR_INVALID,
		bson_error_code_ex_def::BSON_VALIDATE_DIFFERENT_TYPE,
		bson_error_code_ex_def::msg_bson_validate_different_type(),
		lt, rt);

	return err;
}

/*static*/
bson_error_t& bson_value_native_ex::pro_s_make_bson_validate_only_support_cmp_eq_t_err(bson_error_t& err, u32 t)
{
	bson_error_native_ex::s_bson_error_init(
		boost::addressof(err),
		BSON_ERROR_INVALID,
		bson_error_code_ex_def::BSON_VALIDATE_DIFFERENT_TYPE,
		bson_error_code_ex_def::msg_bson_validate_only_support_compare_equal_type(),
		t);

	return err;
}

} // namespace nsql_database_sytem
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{

void swap(bson_value_t& l, bson_value_t& r)
{
	bson_value_native_ex::s_bson_value_swap(boost::addressof(l), boost::addressof(r));
}

} // namespace swap_support
} // namespace nsql_database_system
} // namespace yggr


