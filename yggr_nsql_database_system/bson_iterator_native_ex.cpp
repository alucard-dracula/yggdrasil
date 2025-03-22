//bson_iterator_native_ex.cpp

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

#include <yggr/nsql_database_system/bson_iterator_native_ex.hpp>

#include <yggr/nsql_database_system/bson_native_ex.hpp>
#include <yggr/nsql_database_system/bson_date_time_native_ex.hpp>
#include <yggr/nsql_database_system/bson_regex_native_ex.hpp>

#include <yggr/charset/charset_base_foo.hpp>

#include <cassert>
#include <cstring>
#include <memory>


#if !defined(BSON_INSIDE)
#	define BSON_INSIDE
#	define YGGR_USING_BSON_INSIDE
#endif // BSON_INSIDE

#include <bson/bson-iter.h>

#if defined(YGGR_USING_BSON_INSIDE)
#	undef BSON_INSIDE
#	undef YGGR_USING_BSON_INSIDE
#endif // YGGR_USING_BSON_INSIDE

#include <base64.h>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

const bson_visitor_t bson_iterator_native_ex::_s_bson_as_json_visitors = 
{
	bson_iterator_native_ex::s_bson_as_json_visit_before,
	bson_iterator_native_ex::s_bson_as_json_visit_after, /* visit_after */
	bson_iterator_native_ex::s_bson_as_json_visit_corrupt, /* visit_corrupt */
	bson_iterator_native_ex::s_bson_as_json_visit_double,
	bson_iterator_native_ex::s_bson_as_json_visit_utf8,
	bson_iterator_native_ex::s_bson_as_json_visit_document,
	bson_iterator_native_ex::s_bson_as_json_visit_array,
	bson_iterator_native_ex::s_bson_as_json_visit_binary,
	bson_iterator_native_ex::s_bson_as_json_visit_undefined,
	bson_iterator_native_ex::s_bson_as_json_visit_oid,
	bson_iterator_native_ex::s_bson_as_json_visit_bool,
	bson_iterator_native_ex::s_bson_as_json_visit_date_time,
	bson_iterator_native_ex::s_bson_as_json_visit_null,
	bson_iterator_native_ex::s_bson_as_json_visit_regex,
	bson_iterator_native_ex::s_bson_as_json_visit_dbpointer,
	bson_iterator_native_ex::s_bson_as_json_visit_code,
	bson_iterator_native_ex::s_bson_as_json_visit_symbol,
	bson_iterator_native_ex::s_bson_as_json_visit_codewscope,
	bson_iterator_native_ex::s_bson_as_json_visit_int32,
	bson_iterator_native_ex::s_bson_as_json_visit_timestamp,
	bson_iterator_native_ex::s_bson_as_json_visit_int64,
	bson_iterator_native_ex::s_bson_as_json_visit_maxkey,
	bson_iterator_native_ex::s_bson_as_json_visit_minkey,
	0, /* visit_unsupported_type */
	bson_iterator_native_ex::s_bson_as_json_visit_decimal128
};

#define YGGR_BSON_ITER_TYPE(i) ((bson_type_t) *((i)->raw + (i)->type))

/*static*/ 
bool bson_iterator_native_ex::s_bson_iter_next(bson_iter_t* iter,    /* INOUT */
												uint32_t next_keylen, /* IN */
												const char** key,     /* OUT */
												uint32_t* bson_type,  /* OUT */
												bool* unsupported)    /* OUT */
{
	assert(iter);
	if(!iter)
	{
		*key = 0;
		*bson_type = BSON_TYPE_EOD;
		return false;
	}

	const uint8_t* data = 0;
	uint32_t o = 0;
	unsigned int len = 0;

	assert(unsupported);
	*unsupported = false;

	if(!iter->raw) 
	{
		*key = 0;
		*bson_type = BSON_TYPE_EOD;
		return false;
	}

	data = iter->raw;
	len = iter->len;

	iter->off = iter->next_off;
	iter->type = iter->off;
	iter->key = iter->off + 1;
	iter->d1 = 0;
	iter->d2 = 0;
	iter->d3 = 0;
	iter->d4 = 0;

	if(next_keylen == 0) 
	{
		/* iterate from start to end of NULL-terminated key string */
		for(o = iter->key; o < len; ++o) 
		{
			if(!data[o]) 
			{
				iter->d1 = ++o;
				goto fill_data_fields;
			}
		}
	} 
	else 
	{
		o = iter->key + next_keylen + 1;
		iter->d1 = o;
		goto fill_data_fields;
	}

	goto mark_invalid;

fill_data_fields:

	*key = bson_iter_key_unsafe(iter);
	*bson_type = YGGR_BSON_ITER_TYPE(iter);

	switch (*bson_type) 
	{
	case BSON_TYPE_DATE_TIME:
	case BSON_TYPE_DOUBLE:
	case BSON_TYPE_INT64:
	case BSON_TYPE_TIMESTAMP:
		{
			iter->next_off = o + 8;
		}
		break;
	case BSON_TYPE_CODE:
	case BSON_TYPE_SYMBOL:
	case BSON_TYPE_UTF8:
		{
			uint32_t l = 0;

			if((o + 4) >= len) 
			{
				iter->err_off = o;
				goto mark_invalid;
			}

			iter->d2 = o + 4;
			memcpy(&l, iter->raw + iter->d1, sizeof(l));
			l = BSON_UINT32_FROM_LE(l);

			if(l > (len - (o + 4))) 
			{
				iter->err_off = o;
				goto mark_invalid;
			}

			iter->next_off = o + 4 + l;

			/*
			* Make sure the string length includes the NUL byte.
			*/
			if(BSON_UNLIKELY ((l == 0) || (iter->next_off >= len))) 
			{
				iter->err_off = o;
				goto mark_invalid;
			}

			/*
			* Make sure the last byte is a NUL byte.
			*/
			if(BSON_UNLIKELY ((iter->raw + iter->d2)[l - 1] != '\0')) {
				iter->err_off = o + 4 + l - 1;
				goto mark_invalid;
			}
		} 
		break;
	case BSON_TYPE_BINARY:
		{
			uint8_t subtype = 0;
			uint32_t l = 0;

			if(o >= (len - 4)) 
			{
				iter->err_off = o;
				goto mark_invalid;
			}

			iter->d2 = o + 4;
			iter->d3 = o + 5;

			memcpy(&l, iter->raw + iter->d1, sizeof(l));
			l = BSON_UINT32_FROM_LE (l);

			if(l >= (len - o - 4)) 
			{
				iter->err_off = o;
				goto mark_invalid;
			}

			subtype = *(iter->raw + iter->d2);

			if(subtype == BSON_SUBTYPE_BINARY_DEPRECATED) 
			{
				if(l < 4) 
				{
					iter->err_off = o;
					goto mark_invalid;
				}

				/* subtype 2 has a redundant length header in the data */
				int32_t binary_len = 0;
				memcpy(&binary_len, (iter->raw + iter->d3), sizeof(binary_len));
				binary_len = BSON_UINT32_FROM_LE(binary_len);

				if(binary_len + 4 != l) 
				{
					iter->err_off = iter->d3;
					goto mark_invalid;
				}
			}

			iter->next_off = o + 5 + l;
		} 
		break;
	case BSON_TYPE_ARRAY:
	case BSON_TYPE_DOCUMENT: 
		{
			if(o >= (len - 4)) 
			{
				iter->err_off = o;
				goto mark_invalid;
			}

			uint32_t l = 0;
			memcpy(&l, iter->raw + iter->d1, sizeof(l));
			l = BSON_UINT32_FROM_LE(l);

			if((l > len) || (l > (len - o))) 
			{
				iter->err_off = o;
				goto mark_invalid;
			}

			iter->next_off = o + l;
		} 
		break;
	case BSON_TYPE_OID:
		{
			iter->next_off = o + 12;
		}
		break;
	case BSON_TYPE_BOOL: 
		{
			if(iter->d1 >= len) 
			{
				iter->err_off = o;
				goto mark_invalid;
			}

			char val = 0;
			memcpy(&val, iter->raw + iter->d1, 1);
			if(val != 0x00 && val != 0x01)
			{
				iter->err_off = o;
				goto mark_invalid;
			}

			iter->next_off = o + 1;
		} 
		break;
	case BSON_TYPE_REGEX: 
		{
			bool eor = false;
			bool eoo = false;

			for(; o < len; o++) 
			{
				if(!data[o]) 
				{
					iter->d2 = ++o;
					eor = true;
					break;
				}
			}

			if(!eor)
			{
				iter->err_off = iter->next_off;
				goto mark_invalid;
			}

			for(; o < len; o++) 
			{
				if(!data[o]) 
				{
					eoo = true;
					break;
				}
			}

			if(!eoo)
			{
				iter->err_off = iter->next_off;
				goto mark_invalid;
			}

			iter->next_off = o + 1;
		}
		break;
	case BSON_TYPE_DBPOINTER:
		{
			if(o >= (len - 4)) 
			{
				iter->err_off = o;
				goto mark_invalid;
			}

			iter->d2 = o + 4;
			uint32_t l = 0;
			memcpy(&l, iter->raw + iter->d1, sizeof(l));
			l = BSON_UINT32_FROM_LE(l);

			/* Check valid string length. l counts '\0' but not 4 bytes for itself. */
			if(l == 0 || l > (len - o - 4)) 
			{
				iter->err_off = o;
				goto mark_invalid;
			}

			if(*(iter->raw + o + l + 3)) 
			{
				/* not null terminated */
				iter->err_off = o + l + 3;
				goto mark_invalid;
			}

			iter->d3 = o + 4 + l;
			iter->next_off = o + 4 + l + 12;
		}
		break;
	case BSON_TYPE_CODEWSCOPE: 
		{
			if((len < 19) || (o >= (len - 14))) 
			{
				iter->err_off = o;
				goto mark_invalid;
			}

			iter->d2 = o + 4;
			iter->d3 = o + 8;

			uint32_t l = 0;
			memcpy(&l, iter->raw + iter->d1, sizeof(l));
			l = BSON_UINT32_FROM_LE(l);

			if((l < 14) || (l >= (len - o))) 
			{
				iter->err_off = o;
				goto mark_invalid;
			}

			iter->next_off = o + l;

			if(iter->next_off >= len) 
			{
				iter->err_off = o;
				goto mark_invalid;
			}

			memcpy(&l, iter->raw + iter->d2, sizeof(l));
			l = BSON_UINT32_FROM_LE(l);

			if(l == 0 || l >= (len - o - 4 - 4)) 
			{
				iter->err_off = o;
				goto mark_invalid;
			}

			if((o + 4 + 4 + l + 4) >= iter->next_off)
			{
				iter->err_off = o + 4;
				goto mark_invalid;
			}

			iter->d4 = o + 4 + 4 + l;
			uint32_t doclen = 0;
			memcpy(&doclen, iter->raw + iter->d4, sizeof(doclen));
			doclen = BSON_UINT32_FROM_LE(doclen);

			if((o + 4 + 4 + l + doclen) != iter->next_off) 
			{
				iter->err_off = o + 4 + 4 + l;
				goto mark_invalid;
			}
		}
		break;
	case BSON_TYPE_INT32:
		{
			iter->next_off = o + 4;
		}
		break;
	case BSON_TYPE_DECIMAL128:
		{
			iter->next_off = o + 16;
		}
		break;
	case BSON_TYPE_MAXKEY:
	case BSON_TYPE_MINKEY:
	case BSON_TYPE_NULL:
	case BSON_TYPE_UNDEFINED:
		{
			iter->next_off = o;
		}
		break;
	default:
		*unsupported = true;
	/* FALL THROUGH */
	case BSON_TYPE_EOD:
		{
			iter->err_off = o;
			goto mark_invalid;
		}
	}

	/*
	* Check to see if(any of the field locations would overflow the
	* current BSON buffer. If so, set the error location to the offset
	* of where the field starts.
	*/
	if(iter->next_off >= len) 
	{
		iter->err_off = o;
		goto mark_invalid;
	}

	iter->err_off = 0;

	return true;

mark_invalid:
	iter->raw = 0;
	iter->len = 0;
	iter->next_off = 0;

	return false;
}


#undef YGGR_BSON_ITER_TYPE

///*static*/ 
//bool bson_iterator_native_ex::s_bson_as_json_visit_utf8(const bson_iter_t* iter, const char* key, 
//															size_t v_utf8_len, const char* v_utf8, void* data)
//{
//	native_bson_json_state_t* state = reinterpret_cast<native_bson_json_state_t*>(data);
//	assert(state && state->str);
//	if(!(state && state->str))
//	{
//		return false;
//	}
//
//	char* escaped = bson_utf8_escape_for_json(v_utf8, v_utf8_len);
//
//	if(escaped) 
//	{
//		bson_string_append(state->str, "\"");
//		bson_string_append(state->str, escaped);
//		bson_string_append(state->str, "\"");
//		bson_free(escaped);
//		return false;
//	}
//
//	return true;
//}

/*static*/ 
bool bson_iterator_native_ex::s_bson_as_json_visit_utf8(const bson_iter_t* iter, const char* key, 
															size_t v_utf8_len, const char* v_utf8, void* data)
{
	assert(iter);
	if(!iter)
	{
		return true;
	}
	native_bson_json_state_t* state = reinterpret_cast<native_bson_json_state_t*>(data);
	assert(state && state->str);
	
	if(!(state && state->str))
	{
		return false;
	}

	char* escaped = bson_utf8_escape_for_json (v_utf8, v_utf8_len);

	if(!escaped)
	{
		return true;
	}

	bson_string_append(state->str, "\"");
	bson_string_append(state->str, escaped);
	bson_string_append(state->str, "\"");
	bson_free(escaped);
	return false;
}

/*static*/ 
bool bson_iterator_native_ex::s_bson_as_json_visit_int32(const bson_iter_t* iter, const char* key, 
															int32_t v_int32, void* data)
{
	assert(iter);
	if(!iter)
	{
		return true;
	}
	native_bson_json_state_t* state = reinterpret_cast<native_bson_json_state_t*>(data);
	assert(state && state->str);
	
	if(!(state && state->str))
	{
		return false;
	}

	switch(state->mode)
	{
	case BSON_JSON_MODE_CANONICAL:
		bson_string_append_printf(state->str, "{ \"$numberInt\" : \"%" PRId32 "\" }", v_int32);
		break;
	default:
		bson_string_append_printf(state->str, "%" PRId32, v_int32);
	}

	return false;
}

/*static*/ 
bool bson_iterator_native_ex::s_bson_as_json_visit_int64(const bson_iter_t* iter, const char* key, 
															int64_t v_int64, void* data)
{
	assert(iter);
	if(!iter)
	{
		return true;
	}
	native_bson_json_state_t* state = reinterpret_cast<native_bson_json_state_t*>(data);
	assert(state && state->str);
	
	if(!(state && state->str))
	{
		return false;
	}

	switch(state->mode)
	{
	case BSON_JSON_MODE_CANONICAL:
		bson_string_append_printf(state->str, "{ \"$numberLong\" : \"%" PRId64 "\" }", v_int64);
		break;
	default:
		bson_string_append_printf(state->str, "%" PRId64, v_int64);
	}

	return false;
}

/*static*/ 
bool bson_iterator_native_ex::s_bson_as_json_visit_decimal128(const bson_iter_t* iter, const char* key, 
																const bson_decimal128_t* v_dec128, void* data)
{
	assert(iter);
	if(!iter)
	{
		return true;
	}
	native_bson_json_state_t* state = reinterpret_cast<native_bson_json_state_t*>(data);
	assert(state && state->str);
	
	if(!(state && state->str))
	{
		return false;
	}

	char decimal128_string[BSON_DECIMAL128_STRING] = {0};

	bson_decimal128_to_string(v_dec128, decimal128_string);

	bson_string_append(state->str, "{ \"$numberDecimal\" : \"");
	bson_string_append(state->str, decimal128_string);
	bson_string_append(state->str, "\" }");

	return false;
}

///*static*/ 
//bool bson_iterator_native_ex::s_bson_as_json_visit_double(const bson_iter_t* iter, const char* key, 
//															double v_double, void* data)
//{
//	native_bson_json_state_t* state = reinterpret_cast<native_bson_json_state_t*>(data);
//	assert(state && state->str);
//	if(!(state && state->str))
//	{
//		return false;
//	}
//
//#ifdef _MSC_VER
//	unsigned int current_format = _set_output_format(_TWO_DIGIT_EXPONENT);
//#endif
//
//	bson_string_append_printf(state->str, "%.15g", v_double);
//
//#ifdef _MSC_VER
//	_set_output_format(current_format);
//#endif
//
//	return false;
//}

/*static*/ 
bool bson_iterator_native_ex::s_bson_as_json_visit_double(const bson_iter_t* iter, const char* key, 
															double v_double, void* data)
{
	assert(iter);
	if(!iter)
	{
		return true;
	}
	native_bson_json_state_t* state = reinterpret_cast<native_bson_json_state_t*>(data);
	assert(state && state->str);
	
	if(!(state && state->str))
	{
		return false;
	}

	//bson_json_state_t *state = data;
	//bson_string_t* str = state->str;
	//uint32_t start_len = 0;
	//bool legacy = false;

	//BSON_UNUSED (iter);
	//BSON_UNUSED (key);

	/* Determine if legacy (i.e. unwrapped) output should be used. Relaxed mode
	* will use this for nan and inf values, which we check manually since old
	* platforms may not have isinf or isnan. */
	bool legacy = 
		state->mode == BSON_JSON_MODE_LEGACY 
		|| (state->mode == BSON_JSON_MODE_RELAXED
			&& !(v_double != v_double || v_double * 0 != 0));

	if (!legacy)
	{
		bson_string_append(state->str, "{ \"$numberDouble\" : \"");
	}

	bson_string_t* str = state->str;

	if (!legacy && v_double != v_double) 
	{
		bson_string_append(str, "NaN");
	} 
	else if (!legacy && v_double * 0 != 0) 
	{
		if (v_double > 0) 
		{
			bson_string_append(str, "Infinity");
		} 
		else 
		{
			bson_string_append(str, "-Infinity");
		}
	} 
	else 
	{
		uint32_t start_len = str->len;
		bson_string_append_printf (str, "%.20g", v_double);

		/* ensure trailing ".0" to distinguish "3" from "3.0" */
		if (strspn (&str->str[start_len], "0123456789-") == str->len - start_len) 
		{
			bson_string_append(str, ".0");
		}
	}

	if (!legacy) 
	{
		bson_string_append(state->str, "\" }");
	}

	return false;
}

/*static*/ 
bool bson_iterator_native_ex::s_bson_as_json_visit_undefined(const bson_iter_t* iter, const char* key, void* data)
{
	assert(iter);
	if(!iter)
	{
		return true;
	}
	native_bson_json_state_t* state = reinterpret_cast<native_bson_json_state_t*>(data);
	assert(state && state->str);
	
	if(!(state && state->str))
	{
		return false;
	}

	bson_string_append(state->str, "{ \"$undefined\" : true }");
	return false;
}

/*static*/ 
bool bson_iterator_native_ex::s_bson_as_json_visit_null(const bson_iter_t* iter, const char* key, void* data)
{
	assert(iter);
	if(!iter)
	{
		return true;
	}
	native_bson_json_state_t* state = reinterpret_cast<native_bson_json_state_t*>(data);
	assert(state && state->str);
	
	if(!(state && state->str))
	{
		return false;
	}

	bson_string_append(state->str, "null");
	return false;
}

/*static*/ 
bool bson_iterator_native_ex::s_bson_as_json_visit_oid(const bson_iter_t* iter, const char* key, const bson_oid_t* oid, void* data)
{
	assert(iter);
	if(!iter)
	{
		return true;
	}
	native_bson_json_state_t* state = reinterpret_cast<native_bson_json_state_t*>(data);
	assert(state && state->str);
	
	if(!(state && state->str))
	{
		return false;
	}

	char str_oid[25] = {0};

	assert(oid);
	if(!oid)
	{
		return false;
	}

	bson_oid_to_string(oid, str_oid);
	bson_string_append(state->str, "{ \"$oid\" : \"");
	bson_string_append(state->str, str_oid);
	bson_string_append(state->str, "\" }");

	return false;
}

/*static*/ 
bool bson_iterator_native_ex::s_bson_as_json_visit_binary(const bson_iter_t* iter, const char* key, bson_subtype_t v_subtype,
															size_t v_binary_len, const uint8_t* v_binary, void* data)
{
	assert(iter);
	if(!iter)
	{
		return true;
	}
	native_bson_json_state_t* state = reinterpret_cast<native_bson_json_state_t*>(data);
	assert(state && state->str);
	
	if(!(state && state->str))
	{
		return false;
	}

	//size_t b64_len = 0;
	//char* b64 = 0;

	//b64_len = ((v_binary_len + 2) / 3) * 4 + 1;
	u64 b64_len = b64_ntop_byte_size(v_binary_len) + 1; // from base64
	char* b64 = reinterpret_cast<char*>(bson_malloc0(b64_len));
	memset(b64, 0, b64_len);
	if(-1 == b64_ntop(v_binary, v_binary_len, b64, b64_len))
	{
		bson_free(b64);
		return true;
	}

	switch(state->mode)
	{
	case BSON_JSON_MODE_CANONICAL: 
	case BSON_JSON_MODE_RELAXED:
		bson_string_append(state->str, "{ \"$binary\" : { \"base64\" : \"");
		bson_string_append(state->str, b64);
		bson_string_append(state->str, "\", \"subType\" : \"");
		bson_string_append_printf(state->str, "%02x", v_subtype);
		bson_string_append(state->str, "\" } }");
		break;
	default:
		bson_string_append(state->str, "{ \"$binary\" : \"");
		bson_string_append(state->str, b64);
		bson_string_append(state->str, "\", \"$type\" : \"");
		bson_string_append_printf(state->str, "%02x", v_subtype);
		bson_string_append(state->str, "\" }");
	}

	bson_free(b64);

	return false;
}

//static bool
//_bson_as_json_visit_binary (const bson_iter_t *iter,
//                            const char *key,
//                            bson_subtype_t v_subtype,
//                            size_t v_binary_len,
//                            const uint8_t *v_binary,
//                            void *data)
//{
//   bson_json_state_t *state = data;
//   size_t b64_len;
//   char *b64;
//
//   BSON_UNUSED (iter);
//   BSON_UNUSED (key);
//
//   b64_len = mcommon_b64_ntop_calculate_target_size (v_binary_len);
//   b64 = bson_malloc0 (b64_len);
//   BSON_ASSERT (mcommon_b64_ntop (v_binary, v_binary_len, b64, b64_len) != -1);
//
//   if (state->mode == BSON_JSON_MODE_CANONICAL ||
//       state->mode == BSON_JSON_MODE_RELAXED) {
//      bson_string_append(state->str, "{ \"$binary\" : { \"base64\" : \"");
//      bson_string_append(state->str, b64);
//      bson_string_append(state->str, "\", \"subType\" : \"");
//      bson_string_append_printf (state->str, "%02x", v_subtype);
//      bson_string_append(state->str, "\" } }");
//   } else {
//      bson_string_append(state->str, "{ \"$binary\" : \"");
//      bson_string_append(state->str, b64);
//      bson_string_append(state->str, "\", \"$type\" : \"");
//      bson_string_append_printf (state->str, "%02x", v_subtype);
//      bson_string_append(state->str, "\" }");
//   }
//
//   bson_free (b64);
//
//   return false;
//}

/*static*/
bool bson_iterator_native_ex::s_bson_as_json_visit_bool(const bson_iter_t* iter, const char* key,
															bool v_bool, void* data)
{
	assert(iter);
	if(!iter)
	{
		return true;
	}
	native_bson_json_state_t* state = reinterpret_cast<native_bson_json_state_t*>(data);
	assert(state && state->str);
	
	if(!(state && state->str))
	{
		return false;
	}

	bson_string_append(state->str, v_bool ? "true" : "false");
	return false;
}

/*static*/ 
bool bson_iterator_native_ex::s_bson_as_json_visit_date_time(const bson_iter_t* iter, const char* key, 
																			int64_t msec_since_epoch, void* data)
{
	assert(iter);
	if(!iter)
	{
		return true;
	}
	native_bson_json_state_t* state = reinterpret_cast<native_bson_json_state_t*>(data);
	assert(state && state->str);
	
	if(!(state && state->str))
	{
		return false;
	}

	switch(state->mode)
	{
	case BSON_JSON_MODE_RELAXED:
		if(!(msec_since_epoch < 0))
		{
			bson_string_append(state->str, "{ \"$date\" : \"");
			bson_date_time_native_ex::s_bson_date_format_iso8601(msec_since_epoch, state->str);
			bson_string_append(state->str, "\" }");
			break;
		}
	case BSON_JSON_MODE_CANONICAL:
		bson_string_append(state->str, "{ \"$date\" : { \"$numberLong\" : \"");
		bson_string_append_printf(state->str, "%" PRId64, msec_since_epoch);
		bson_string_append(state->str, "\" } }");
		break;
	default:
		bson_string_append(state->str, "{ \"$date\" : ");
		bson_string_append_printf(state->str, "%" PRId64, msec_since_epoch);
		bson_string_append(state->str, " }");
	}

	return false;
}

/*static*/ 
bool bson_iterator_native_ex::s_bson_as_json_visit_regex(const bson_iter_t* iter, const char* key, 
															const char* v_regex, const char* v_options, void* data)
{
	assert(iter);
	if(!iter)
	{
		return true;
	}
	native_bson_json_state_t* state = reinterpret_cast<native_bson_json_state_t*>(data);
	assert(state && state->str);
	
	if(!(state && state->str))
	{
		return false;
	}

	char* escaped_v_regex = bson_utf8_escape_for_json(v_regex, -1);
	if(!escaped_v_regex)
	{
		return true;
	}

	char options_buffer[bson_regex_native_ex::E_BSON_REGEX_OPTIONS_STRING_BUFFER_SIZE] = {0};
	bson_regex_native_ex::s_bson_regex_options_sorted(options_buffer, v_options);

	switch(state->mode)
	{
	case BSON_JSON_MODE_CANONICAL:
	case BSON_JSON_MODE_RELAXED:
		bson_string_append(state->str, "{ \"$regularExpression\" : { \"pattern\" : \"");
		bson_string_append(state->str, escaped_v_regex);
		bson_string_append(state->str, "\", \"options\" : \"");
		//bson_regex_native_ex::s_bson_append_regex_options_sorted(state->str, v_options); // org code
		bson_string_append(state->str, options_buffer); // opt code
		bson_string_append(state->str, "\" } }");
		break;
	default:
		bson_string_append(state->str, "{ \"$regex\" : \"");
		bson_string_append(state->str, escaped_v_regex);
		bson_string_append(state->str, "\", \"$options\" : \"");
		//bson_regex_native_ex::s_bson_append_regex_options_sorted(state->str, v_options); // org code
		bson_string_append(state->str, options_buffer); // opt code
		bson_string_append(state->str, "\" }");
	}

	bson_free(escaped_v_regex);
	return false;
}

/*static*/ 
bool bson_iterator_native_ex::s_bson_as_json_visit_timestamp(const bson_iter_t* iter, const char* key, 
																uint32_t v_timestamp, uint32_t v_increment, void* data)
{
	assert(iter);
	if(!iter)
	{
		return true;
	}
	native_bson_json_state_t* state = reinterpret_cast<native_bson_json_state_t*>(data);
	assert(state && state->str);
	
	if(!(state && state->str))
	{
		return false;
	}

	bson_string_append(state->str, "{ \"$timestamp\" : { \"t\" : ");
	bson_string_append_printf(state->str, "%u", v_timestamp);
	bson_string_append(state->str, ", \"i\" : ");
	bson_string_append_printf(state->str, "%u", v_increment);
	bson_string_append(state->str, " } }");

	return false;
}

/*static*/ 
bool bson_iterator_native_ex::s_bson_as_json_visit_dbpointer(const bson_iter_t* iter, const char* key, size_t v_collection_len,
																const char* v_collection, const bson_oid_t* v_oid, void* data)
{
	assert(iter);
	if(!iter)
	{
		return true;
	}
	native_bson_json_state_t* state = reinterpret_cast<native_bson_json_state_t*>(data);
	assert(state && state->str);
	
	if(!(state && state->str))
	{
		return false;
	}

	char str_oid[25] = {0};

	char* v_collection_escaped = bson_utf8_escape_for_json(v_collection, -1);
	if(!v_collection_escaped) 
	{
		return true;
	}

	switch(state->mode)
	{
	case BSON_JSON_MODE_CANONICAL:
	case BSON_JSON_MODE_RELAXED:
		bson_string_append(state->str, "{ \"$dbPointer\" : { \"$ref\" : \"");
		bson_string_append(state->str, v_collection_escaped);
		bson_string_append(state->str, "\"");

		if(v_oid) 
		{
			bson_oid_to_string(v_oid, str_oid);
			bson_string_append(state->str, ", \"$id\" : \"");
			bson_string_append(state->str, str_oid);
			bson_string_append(state->str, "\" }");
		}

		bson_string_append(state->str, " } }");
		break;

	default:
		bson_string_append(state->str, "{ \"$ref\" : \"");
		bson_string_append(state->str, v_collection_escaped);
		bson_string_append(state->str, "\"");

		if(v_oid) 
		{
			bson_oid_to_string(v_oid, str_oid);
			bson_string_append(state->str, ", \"$id\" : \"");
			bson_string_append(state->str, str_oid);
			bson_string_append(state->str, "\"");
		}

		bson_string_append(state->str, " }");
	}

	bson_free (v_collection_escaped);

	return false;
}

/*static*/ 
bool bson_iterator_native_ex::s_bson_as_json_visit_minkey(const bson_iter_t* iter, const char* key, void* data)
{
	assert(iter);
	if(!iter)
	{
		return true;
	}
	native_bson_json_state_t* state = reinterpret_cast<native_bson_json_state_t*>(data);
	assert(state && state->str);
	
	if(!(state && state->str))
	{
		return false;
	}

	bson_string_append(state->str, "{ \"$minKey\" : 1 }");
	return false;
}

/*static*/ 
bool bson_iterator_native_ex::s_bson_as_json_visit_maxkey(const bson_iter_t* iter, const char* key, void* data)
{
	assert(iter);
	if(!iter)
	{
		return true;
	}
	native_bson_json_state_t* state = reinterpret_cast<native_bson_json_state_t*>(data);
	assert(state && state->str);
	
	if(!(state && state->str))
	{
		return false;
	}

	bson_string_append(state->str, "{ \"$maxKey\" : 1 }");
	return false;
}

///*static*/ 
//bool bson_iterator_native_ex::s_bson_as_json_visit_before(const bson_iter_t* iter, const char* key, void* data)
//{
//	native_bson_json_state_t* state = reinterpret_cast<native_bson_json_state_t*>(data);
//	if(!(state && state->str))
//	{
//		return false;
//	}
//
//	if(state->count)
//	{
//		bson_string_append(state->str, ", ");
//	}
//
//	if(state->keys)
//	{
//		char* escaped_key = bson_utf8_escape_for_json(key, -1);
//		if(escaped_key) 
//		{
//			bson_string_append(state->str, "\"");
//			bson_string_append(state->str, escaped_key);
//			bson_string_append(state->str, "\" : ");
//			bson_free(escaped_key);
//		} 
//		else 
//		{
//			return true;
//		}
//	}
//
//	state->count++;
//
//	return false;
//}

/*static*/ 
bool bson_iterator_native_ex::s_bson_as_json_visit_before(const bson_iter_t* iter, const char* key, void* data)
{
	assert(iter && key);
	if(!(iter && key))
	{
		return true;
	}
	native_bson_json_state_t* state = reinterpret_cast<native_bson_json_state_t*>(data);
	assert(state && state->str);
	
	if(!(state && state->str))
	{
		return false;
	}

	if (state->max_len_reached) 
	{
		return true;
	}

	if (state->count) 
	{
		bson_string_append(state->str, ", ");
	}

	if (state->keys)
	{
		char* escaped = bson_utf8_escape_for_json (key, -1);
		if (escaped)
		{
			bson_string_append(state->str, "\"");
			bson_string_append(state->str, escaped);
			bson_string_append(state->str, "\" : ");
			bson_free (escaped);
		}
		else 
		{
			return true;
		}
	}

	state->count++;

	return false;
}

/*static*/ 
bool bson_iterator_native_ex::s_bson_as_json_visit_after(const bson_iter_t* iter, const char* key, void* data)
{
	assert(iter);
	if(!iter)
	{
		return true;
	}
	native_bson_json_state_t* state = reinterpret_cast<native_bson_json_state_t*>(data);
	assert(state && state->str);
	
	if(!(state && state->str))
	{
		return false;
	}

	if(state->max_len == BSON_MAX_LEN_UNLIMITED) 
	{
		return false;
	}

	if(state->str->len >= state->max_len) 
	{
		state->max_len_reached = true;

		if(state->str->len > state->max_len) 
		{
			/* Truncate string to maximum length */
			bson_string_truncate(state->str, state->max_len);
		}

		return true;
	}

	return false;
}

/*static*/ 
void bson_iterator_native_ex::s_bson_as_json_visit_corrupt(const bson_iter_t* iter, void* data)
{
	assert(iter);
	if(!iter)
	{
		return;
	}
	native_bson_json_state_t* state = reinterpret_cast<native_bson_json_state_t*>(data);
	assert(state);
	
	if(!(state))
	{
		return;
	}

	*(state->err_offset) = iter->off;
}

/*static*/ 
bool bson_iterator_native_ex::s_bson_as_json_visit_symbol(const bson_iter_t* iter, const char* key, 
															size_t v_symbol_len, const char* v_symbol, void* data)
{
	assert(iter);
	if(!iter)
	{
		return true;
	}
	native_bson_json_state_t* state = reinterpret_cast<native_bson_json_state_t*>(data);
	assert(state && state->str);
	
	if(!(state && state->str))
	{
		return false;
	}

	char* escaped_v_symbol = bson_utf8_escape_for_json(v_symbol, v_symbol_len);
	if(!escaped_v_symbol) 
	{
		return true;
	}

	switch(state->mode)
	{
	case BSON_JSON_MODE_CANONICAL:
	case BSON_JSON_MODE_RELAXED:
		bson_string_append(state->str, "{ \"$symbol\" : \"");
		bson_string_append(state->str, escaped_v_symbol);
		bson_string_append(state->str, "\" }");
		break;
	default:
		bson_string_append(state->str, "\"");
		bson_string_append(state->str, escaped_v_symbol);
		bson_string_append(state->str, "\"");
	}

	bson_free(escaped_v_symbol);

	return false;
}

///*static*/ 
//bool bson_iterator_native_ex::s_bson_as_json_visit_code(const bson_iter_t* iter, const char* key, 
//															size_t v_code_len, const char* v_code, void* data)
//{
//	assert(iter);
//	if(!iter)
//	{
//		return true;
//	}
//	native_bson_json_state_t* state = reinterpret_cast<native_bson_json_state_t*>(data);
//	assert(state && state->str);
//	
//	if(!(state && state->str))
//	{
//		return false;
//	}
//
//	char* escaped_v_code = bson_utf8_escape_for_json(v_code, v_code_len);
//
//	if(!escaped_v_code)
//	{
//		return true;
//	}
//
//	bson_string_append(state->str, "\"");
//	bson_string_append(state->str, escaped_v_code);
//	bson_string_append(state->str, "\"");
//	bson_free(escaped_v_code);
//	return false;
//}

/*static*/ 
bool bson_iterator_native_ex::s_bson_as_json_visit_code(const bson_iter_t* iter, const char* key, 
															size_t v_code_len, const char* v_code, void* data)
{
	assert(iter);
	if(!iter)
	{
		return true;
	}
	native_bson_json_state_t* state = reinterpret_cast<native_bson_json_state_t*>(data);
	assert(state && state->str);
	
	if(!(state && state->str))
	{
		return false;
	}

	char* escaped_v_code = bson_utf8_escape_for_json(v_code, v_code_len);

	if(!escaped_v_code)
	{
		return true;
	}

	bson_string_append(state->str, "{ \"$code\" : \"");
	bson_string_append(state->str, escaped_v_code);
	bson_string_append(state->str, "\" }");
	bson_free(escaped_v_code);
	return false;
}

///*static*/ 
//bool bson_iterator_native_ex::s_bson_as_json_visit_codewscope(const bson_iter_t* iter, const char* key, 
//																size_t v_code_len, const char* v_code,
//																const bson_t* v_scope, void* data)
//{
//	assert(iter);
//	if(!iter)
//	{
//		return true;
//	}
//	native_bson_json_state_t* state = reinterpret_cast<native_bson_json_state_t*>(data);
//	assert(state && state->str);
//	
//	if(!(state && state->str))
//	{
//		return false;
//	}
//
//	char* escaped_v_code = bson_utf8_escape_for_json(v_code, v_code_len);
//
//	if(escaped_v_code) 
//	{
//		bson_string_append(state->str, "\"");
//		bson_string_append(state->str, escaped_v_code);
//		bson_string_append(state->str, "\"");
//		bson_free(escaped_v_code);
//		return false;
//	}
//
//	return true;
//}

/*static*/ 
bool bson_iterator_native_ex::s_bson_as_json_visit_codewscope(const bson_iter_t* iter, const char* key, 
																size_t v_code_len, const char* v_code,
																const bson_t* v_scope, void* data)
{
	assert(iter);
	if(!iter)
	{
		return true;
	}
	native_bson_json_state_t* state = reinterpret_cast<native_bson_json_state_t*>(data);
	assert(state && state->str);
	
	if(!(state && state->str))
	{
		return false;
	}

	char* code_escaped = bson_utf8_escape_for_json (v_code, v_code_len);
	if (!code_escaped) 
	{
		return true;
	}

	bson_string_append(state->str, "{ \"$code\" : \"");
	bson_string_append(state->str, code_escaped);
	bson_string_append(state->str, "\", \"$scope\" : ");

	bson_free (code_escaped);

	int32_t max_scope_len = 
		state->max_len != BSON_MAX_LEN_UNLIMITED? 
			(std::max<int32_t>)(0, state->max_len - state->str->len)
			: BSON_MAX_LEN_UNLIMITED;

	char* scope = bson_native_ex::s_bson_as_json_visit_all(v_scope, 0, state->mode, max_scope_len);

	if (!scope) 
	{
		return true;
	}

	bson_string_append(state->str, scope);
	bson_string_append(state->str, " }");

	bson_free (scope);

	return false;
}


#if !defined(BSON_MAX_RECURSION)
#	define YGGR_BSON_MAX_RECURSION 200
#else
#	define YGGR_BSON_MAX_RECURSION BSON_MAX_RECURSION
#endif // BSON_MAX_RECURSION

///*static*/ 
//bool bson_iterator_native_ex::s_bson_as_json_visit_document(const bson_iter_t* iter, const char* key, 
//																const bson_t* v_document, void* data)
//{
//	native_bson_json_state_t* state = reinterpret_cast<native_bson_json_state_t*>(data);
//	if(!(state && state->str))
//	{
//		return false;
//	}
//
//	native_bson_json_state_t child_state = { 0, true };
//	bson_iter_t child = {0};
//
//	if(state->depth >= YGGR_BSON_MAX_RECURSION) 
//	{
//		bson_string_append(state->str, "{ ... }");
//		return false;
//	}
//
//	if(bson_iter_init(&child, v_document))
//	{
//		child_state.str = bson_string_new("{ ");
//		child_state.depth = state->depth + 1;
//		if(!bson_empty(v_document))
//		{
//			this_type::s_bson_iter_visit_all(&child, &this_type::_s_bson_as_json_visitors, &child_state);
//		}
//		bson_string_append(child_state.str, " }");
//		bson_string_append(state->str, child_state.str->str);
//		bson_string_free(child_state.str, true);
//	}
//
//	return false;
//}

/*static*/ 
bool bson_iterator_native_ex::s_bson_as_json_visit_document(const bson_iter_t* iter, const char* key, 
																const bson_t* v_document, void* data)
{
	assert(iter);
	if(!iter)
	{
		return true;
	}
	native_bson_json_state_t* state = reinterpret_cast<native_bson_json_state_t*>(data);
	assert(state && state->str);
	
	if(!(state && state->str))
	{
		return false;
	}

	native_bson_json_state_t child_state = {0, true, state->err_offset};
	bson_iter_t child = {0};

	//BSON_UNUSED (iter);
	//BSON_UNUSED (key);

	if(state->depth >= YGGR_BSON_MAX_RECURSION) 
	{
		bson_string_append(state->str, "{ ... }");
		return false;
	}

	if(bson_iter_init(&child, v_document)) 
	{
		child_state.str = bson_string_new("{ ");
		child_state.depth = state->depth + 1;
		child_state.mode = state->mode;
		child_state.max_len = BSON_MAX_LEN_UNLIMITED;
		if(state->max_len != BSON_MAX_LEN_UNLIMITED)
		{
			child_state.max_len = (std::max<int32_t>)(0, state->max_len - state->str->len);
		}

		child_state.max_len_reached = child_state.max_len == 0;

		if(this_type::s_bson_iter_visit_all(&child, &this_type::_s_bson_as_json_visitors, &child_state)) 
		{
			if(child_state.max_len_reached) 
			{
				bson_string_append(state->str, child_state.str->str);
			}

			bson_string_free(child_state.str, true);

			/* If max_len was reached, we return a success state to ensure that
			* VISIT_AFTER is still called
			*/
			return !child_state.max_len_reached;
		}

		bson_string_append(child_state.str, " }");
		bson_string_append(state->str, child_state.str->str);
		bson_string_free(child_state.str, true);
	}

	return false;
}


///*static*/ 
//bool bson_iterator_native_ex::s_bson_as_json_visit_array(const bson_iter_t* iter, const char* key, 
//															const bson_t* v_array, void* data)
//{
//	native_bson_json_state_t* state = reinterpret_cast<native_bson_json_state_t*>(data);
//	if(!(state && state->str))
//	{
//		return false;
//	}
//
//	native_bson_json_state_t child_state = { 0, false };
//	bson_iter_t child = { 0 };
//
//	if(state->depth >= YGGR_BSON_MAX_RECURSION)
//	{
//		bson_string_append(state->str, "{ ... }");
//		return false;
//	}
//
//	if(bson_iter_init(&child, v_array))
//	{
//		child_state.str = bson_string_new("[ ");
//		child_state.depth = state->depth + 1;
//		if(!bson_empty(v_array))
//		{
//			this_type::s_bson_iter_visit_all(&child, &this_type::_s_bson_as_json_visitors, &child_state);
//		}
//		bson_string_append(child_state.str, " ]");
//		bson_string_append(state->str, child_state.str->str);
//		bson_string_free(child_state.str, true);
//	}
//
//	return false;
//}

/*static*/ 
bool bson_iterator_native_ex::s_bson_as_json_visit_array(const bson_iter_t* iter, const char* key, 
															const bson_t* v_array, void* data)
{
	assert(iter);
	if(!iter)
	{
		return true;
	}
	native_bson_json_state_t* state = reinterpret_cast<native_bson_json_state_t*>(data);
	assert(state && state->str);
	
	if(!(state && state->str))
	{
		return false;
	}

	native_bson_json_state_t child_state = {0, false, state->err_offset};
	bson_iter_t child = {0};

	//BSON_UNUSED (iter);
	//BSON_UNUSED (key);

	if(state->depth >= YGGR_BSON_MAX_RECURSION) 
	{
		bson_string_append(state->str, "{ ... }");
		return false;
	}

	if(bson_iter_init(&child, v_array)) 
	{
		child_state.str = bson_string_new("[ ");
		child_state.depth = state->depth + 1;
		child_state.mode = state->mode;
		child_state.max_len = BSON_MAX_LEN_UNLIMITED;
		if(state->max_len != BSON_MAX_LEN_UNLIMITED) 
		{
			child_state.max_len = (std::max<int32_t>)(0, state->max_len - state->str->len);
		}

		child_state.max_len_reached = child_state.max_len == 0;

		if(this_type::s_bson_iter_visit_all(&child, &this_type::_s_bson_as_json_visitors, &child_state)) 
		{
			if(child_state.max_len_reached) 
			{
				bson_string_append(state->str, child_state.str->str);
			}

			bson_string_free(child_state.str, true);

			/* If max_len was reached, we return a success state to ensure that
			* VISIT_AFTER is still called
			*/
			return !child_state.max_len_reached;
		}

		bson_string_append(child_state.str, " ]");
		bson_string_append(state->str, child_state.str->str);
		bson_string_free(child_state.str, true);
	}

	return false;
}


#undef YGGR_BSON_MAX_RECURSION

#define YGGR_BSON_VISIT_FIELD(name) visitor->visit_##name && visitor->visit_##name
#define YGGR_BSON_VISIT_AFTER YGGR_BSON_VISIT_FIELD(after)
#define YGGR_BSON_VISIT_BEFORE YGGR_BSON_VISIT_FIELD(before)
#define YGGR_BSON_VISIT_CORRUPT if(visitor->visit_corrupt) visitor->visit_corrupt
#define YGGR_BSON_VISIT_DOUBLE YGGR_BSON_VISIT_FIELD(double)
#define YGGR_BSON_VISIT_UTF8 YGGR_BSON_VISIT_FIELD(utf8)
#define YGGR_BSON_VISIT_DOCUMENT YGGR_BSON_VISIT_FIELD(document)
#define YGGR_BSON_VISIT_ARRAY YGGR_BSON_VISIT_FIELD(array)
#define YGGR_BSON_VISIT_BINARY YGGR_BSON_VISIT_FIELD(binary)
#define YGGR_BSON_VISIT_UNDEFINED YGGR_BSON_VISIT_FIELD(undefined)
#define YGGR_BSON_VISIT_OID YGGR_BSON_VISIT_FIELD(oid)
#define YGGR_BSON_VISIT_BOOL YGGR_BSON_VISIT_FIELD(bool)
#define YGGR_BSON_VISIT_DATE_TIME YGGR_BSON_VISIT_FIELD(date_time)
#define YGGR_BSON_VISIT_NULL YGGR_BSON_VISIT_FIELD(null)
#define YGGR_BSON_VISIT_REGEX YGGR_BSON_VISIT_FIELD(regex)
#define YGGR_BSON_VISIT_DBPOINTER YGGR_BSON_VISIT_FIELD(dbpointer)
#define YGGR_BSON_VISIT_CODE YGGR_BSON_VISIT_FIELD(code)
#define YGGR_BSON_VISIT_SYMBOL YGGR_BSON_VISIT_FIELD(symbol)
#define YGGR_BSON_VISIT_CODEWSCOPE YGGR_BSON_VISIT_FIELD(codewscope)
#define YGGR_BSON_VISIT_INT32 YGGR_BSON_VISIT_FIELD(int32)
#define YGGR_BSON_VISIT_TIMESTAMP YGGR_BSON_VISIT_FIELD(timestamp)
#define YGGR_BSON_VISIT_INT64 YGGR_BSON_VISIT_FIELD(int64)
#define YGGR_BSON_VISIT_DECIMAL128 YGGR_BSON_VISIT_FIELD(decimal128)
#define YGGR_BSON_VISIT_MAXKEY YGGR_BSON_VISIT_FIELD(maxkey)
#define YGGR_BSON_VISIT_MINKEY YGGR_BSON_VISIT_FIELD(minkey)

//bool
//bson_iter_visit_all (bson_iter_t *iter,             /* INOUT */
//                     const bson_visitor_t *visitor, /* IN */
//                     void *data)                    /* IN */

/*static*/ 
bool bson_iterator_native_ex::s_bson_iter_visit_all(bson_iter_t* iter, const bson_visitor_t* visitor, void* data)
{
	assert(iter && visitor);
	if(!(iter && visitor))
	{
		return false;
	}

	uint32_t bson_type = 0;
	const char* key = 0;
	bool unsupported = false;
	std::size_t key_len = 0;

	while(this_type::s_bson_iter_next(iter, 0, &key, &bson_type, &unsupported)) 
	{
		if(*key && !bson_utf8_validate(key, (key_len = strlen(key)), false))
		{
			iter->err_off = iter->off;
			break;
		}

		if(YGGR_BSON_VISIT_BEFORE(iter, key, data))
		{
			return true;
		}

		switch(bson_type) 
		{
		case BSON_TYPE_DOUBLE:
			if(YGGR_BSON_VISIT_DOUBLE(iter, key, bson_iter_double(iter), data)) 
			{
				return true;
			}
			break;
		case BSON_TYPE_UTF8:
			{
				uint32_t utf8_len = 0;
				const char* utf8 = bson_iter_utf8(iter, &utf8_len);

				if(!bson_utf8_validate(utf8, utf8_len, true)) 
				{
					iter->err_off = iter->off;
					return true;
				}

				if(YGGR_BSON_VISIT_UTF8(iter, key, utf8_len, utf8, data))
				{
					return true;
				}
			} 
			break;
		case BSON_TYPE_DOCUMENT: 
			{
				const uint8_t* docbuf = 0;
				uint32_t doclen = 0;
				bson_t b = {0};

				bson_iter_document(iter, &doclen, &docbuf);

				if(bson_init_static(&b, docbuf, doclen) 
					&& YGGR_BSON_VISIT_DOCUMENT(iter, key, &b, data)) 
				{
					return true;
				}
			}
			break;
		case BSON_TYPE_ARRAY: 
			{
				const uint8_t* docbuf = 0;
				uint32_t doclen = 0;
				bson_t b = {0};

				bson_iter_array(iter, &doclen, &docbuf);

				if(bson_init_static(&b, docbuf, doclen) 
					&& YGGR_BSON_VISIT_ARRAY(iter, key, &b, data))
				{
					return true;
				}
			} 
			break;
		case BSON_TYPE_BINARY: 
			{
				const uint8_t* binary = 0;
				bson_subtype_t subtype = BSON_SUBTYPE_BINARY;
				uint32_t binary_len = 0;

				bson_iter_binary(iter, &subtype, &binary_len, &binary);

				if(YGGR_BSON_VISIT_BINARY(iter, key, subtype, binary_len, binary, data)) 
				{
					return true;
				}
			} 
			break;
		case BSON_TYPE_UNDEFINED:
			{
				if(YGGR_BSON_VISIT_UNDEFINED(iter, key, data)) 
				{
					return true;
				}
			}
			break;
		case BSON_TYPE_OID:
			{
				if(YGGR_BSON_VISIT_OID(iter, key, bson_iter_oid (iter), data))
				{
					return true;
				}
			}
			break;
		case BSON_TYPE_BOOL:
			{
				if(YGGR_BSON_VISIT_BOOL(iter, key, bson_iter_bool(iter), data)) 
				{
					return true;
				}
			}
			break;
		case BSON_TYPE_DATE_TIME:
			{
				if(YGGR_BSON_VISIT_DATE_TIME(iter, key, bson_iter_date_time(iter), data)) 
				{
					return true;
				}
			}
			break;
		case BSON_TYPE_NULL:
			{
				if(YGGR_BSON_VISIT_NULL (iter, key, data)) 
				{
					return true;
				}
			}
			break;
		case BSON_TYPE_REGEX: 
			{
				const char* options = 0;
				const char* regex = bson_iter_regex(iter, &options);

				if(!bson_utf8_validate(regex, strlen(regex), true)) 
				{
					iter->err_off = iter->off;
					return true;
				}

				if(YGGR_BSON_VISIT_REGEX(iter, key, regex, options, data)) 
				{
					return true;
				}
			} 
			break;
		case BSON_TYPE_DBPOINTER: 
			{
				uint32_t collection_len = 0;
				const char* collection = 0;
				const bson_oid_t* oid = 0;

				bson_iter_dbpointer(iter, &collection_len, &collection, &oid);

				if(!bson_utf8_validate(collection, collection_len, true)) 
				{
					iter->err_off = iter->off;
					return true;
				}

				if(YGGR_BSON_VISIT_DBPOINTER(
						iter, key, collection_len, collection, oid, data))
				{
					return true;
				}
			} 
			break;
		case BSON_TYPE_CODE: 
			{
				uint32_t code_len = 0;
				const char* code = bson_iter_code(iter, &code_len);

				if(!bson_utf8_validate(code, code_len, true)) 
				{
					iter->err_off = iter->off;
					return true;
				}

				if(YGGR_BSON_VISIT_CODE(iter, key, code_len, code, data)) 
				{
					return true;
				}
			} 
			break;
		case BSON_TYPE_SYMBOL: 
			{
				uint32_t symbol_len = 0;
				const char* symbol = bson_iter_symbol(iter, &symbol_len);

				if(!bson_utf8_validate(symbol, symbol_len, true)) 
				{
					iter->err_off = iter->off;
					return true;
				}

				if(YGGR_BSON_VISIT_SYMBOL(iter, key, symbol_len, symbol, data)) 
				{
					return true;
				}
			} 
			break;
		case BSON_TYPE_CODEWSCOPE: 
			{
				uint32_t length = 0;
				const uint8_t* docbuf = 0;
				uint32_t doclen = 0;
				bson_t b = {0};

				const char* code = bson_iter_codewscope(iter, &length, &doclen, &docbuf);

				if(!bson_utf8_validate(code, length, true)) 
				{
					iter->err_off = iter->off;
					return true;
				}

				if(bson_init_static(&b, docbuf, doclen)
					 && YGGR_BSON_VISIT_CODEWSCOPE (iter, key, length, code, &b, data)) 
				{
					return true;
				}
			} 
			break;
		case BSON_TYPE_INT32:
			{
				if(YGGR_BSON_VISIT_INT32(iter, key, bson_iter_int32 (iter), data))
				{
					return true;
				}
			}
			break;
		case BSON_TYPE_TIMESTAMP:
			{
				uint32_t timestamp = 0;
				uint32_t increment = 0;
				bson_iter_timestamp(iter, &timestamp, &increment);

				if(YGGR_BSON_VISIT_TIMESTAMP(iter, key, timestamp, increment, data)) 
				{
					return true;
				}
			} 
			break;
		case BSON_TYPE_INT64:
			{
				if(YGGR_BSON_VISIT_INT64(iter, key, bson_iter_int64(iter), data)) 
				{
					return true;
				}
			}
			break;
		case BSON_TYPE_DECIMAL128:
			{
				bson_decimal128_t dec = {0};
				bson_iter_decimal128(iter, &dec);

				if(YGGR_BSON_VISIT_DECIMAL128(iter, key, &dec, data)) 
				{
					return true;
				}
			} 
			break;
		case BSON_TYPE_MAXKEY:
			{
				if(YGGR_BSON_VISIT_MAXKEY(iter, bson_iter_key_unsafe(iter), data))
				{
					return true;
				}
			}
			break;
		case BSON_TYPE_MINKEY:
			{
				if(YGGR_BSON_VISIT_MINKEY(iter, bson_iter_key_unsafe(iter), data)) 
				{
					return true;
				}
			}
			break;
		case BSON_TYPE_EOD:
		default:
			break;
		}

		if(YGGR_BSON_VISIT_AFTER(iter, bson_iter_key_unsafe(iter), data)) 
		{
			return true;
		}
	}

	if(iter->err_off)
	{
		if(unsupported && visitor->visit_unsupported_type 
			&& bson_utf8_validate(key, key_len, false)) 
		{
			visitor->visit_unsupported_type(iter, key, bson_type, data);
			return false;
		}

		YGGR_BSON_VISIT_CORRUPT(iter, data);
	}

	return false;
}

#undef YGGR_BSON_VISIT_FIELD
#undef YGGR_BSON_VISIT_AFTER
#undef YGGR_BSON_VISIT_BEFORE
#undef YGGR_BSON_VISIT_CORRUPT
#undef YGGR_BSON_VISIT_DOUBLE
#undef YGGR_BSON_VISIT_UTF8
#undef YGGR_BSON_VISIT_DOCUMENT
#undef YGGR_BSON_VISIT_ARRAY
#undef YGGR_BSON_VISIT_BINARY
#undef YGGR_BSON_VISIT_UNDEFINED
#undef YGGR_BSON_VISIT_OID
#undef YGGR_BSON_VISIT_BOOL
#undef YGGR_BSON_VISIT_DATE_TIME
#undef YGGR_BSON_VISIT_NULL
#undef YGGR_BSON_VISIT_REGEX
#undef YGGR_BSON_VISIT_DBPOINTER
#undef YGGR_BSON_VISIT_CODE
#undef YGGR_BSON_VISIT_SYMBOL
#undef YGGR_BSON_VISIT_CODEWSCOPE
#undef YGGR_BSON_VISIT_INT32
#undef YGGR_BSON_VISIT_TIMESTAMP
#undef YGGR_BSON_VISIT_INT64
#undef YGGR_BSON_VISIT_DECIMAL128
#undef YGGR_BSON_VISIT_MAXKEY
#undef YGGR_BSON_VISIT_MINKEY

/*static*/ 
bool bson_iterator_native_ex::s_bson_iter_validate(const bson_iter_t* piter)
{
	return piter && piter->raw && piter->len && piter->next_off;
}

/*static*/ 
u8 bson_iterator_native_ex::s_bson_iter_type(const bson_iter_t* piter)
{
	return bson_iter_type(piter);
}

/*static*/ 
const char* bson_iterator_native_ex::s_bson_iter_key(const bson_iter_t* piter)
{
	return bson_iter_key(piter);
}

/*static*/ 
bool bson_iterator_native_ex::s_bson_iter_cmp_eq(const bson_iter_t* pl, const bson_iter_t* pr)
{
	std::size_t size = 
		reinterpret_cast<const u8*>(&(pl->err_off)) 
		- reinterpret_cast<const u8*>(pl) 
		+ sizeof(pl->err_off);
	return pl == pr || !memcmp(pl, pr, size);
}

/*static*/ 
bool bson_iterator_native_ex::s_bson_iter_is_root(const bson_iter_t* piter)
{
	assert(piter);
	return ( piter->raw) 
			&& ( piter->len)
			&& (!piter->off) //(piter->off == 0)
			&& (!piter->type) // (piter->type == 0)
			&& (!piter->key) // (piter->key == 0)
			&& (!piter->d1) // (piter->d1 == 0)
			&& (!piter->d2) // (piter->d2 == 0)
			&& (!piter->d3) // (piter->d3 == 0)
			&& (!piter->d4) // (piter->d4 == 0)
			&& ( piter->next_off == 4) 
			&& (!piter->err_off) // (iter->err_off == 0)
			;
}

/*static*/ 
const u8* bson_iterator_native_ex::s_bson_iter_value_t_pointer(const bson_iter_t* piter, s32& len)
{
	const u8* ret = 0;
	len = 0;
	if(this_type::s_bson_iter_validate(piter))
	{
		ret = piter->raw + piter->type;
		len = 1;
	}
		
	return ret;
}

/*static*/ 
const char* bson_iterator_native_ex::s_bson_iter_key_pointer(const bson_iter_t* piter, s32& len)
{
	const char* ret = 0;
	len = 0;

	if(this_type::s_bson_iter_validate(piter))
	{
		ret = reinterpret_cast<const char*>(piter->raw + piter->key);
		len = piter->d1 - piter->type - 1;
	}
		
	return ret;
}

/*static*/ 
const u8* bson_iterator_native_ex::s_bson_iter_value_pointer(const bson_iter_t* piter, s32& len)
{
	const u8* ret = 0;
	len = 0;

	if(this_type::s_bson_iter_validate(piter))
	{
		ret = piter->raw + piter->d1;
		len = piter->next_off - piter->d1;
	}

	return ret;
}

/*static*/ 
bool bson_iterator_native_ex::s_bson_iter_swap(bson_iter_t* pl, bson_iter_t* pr)
{
	assert(pl && pr);
	if(!(pl && pr))
	{
		return false;
	}

	if(pl != pr)
	{
		bson_iter_t tmp = {0};

		memcpy(&tmp, pr, sizeof(bson_iter_t));
		memcpy(pr, pl, sizeof(bson_iter_t));
		memcpy(pl, &tmp, sizeof(bson_iter_t));
	}

	return true;
}

///*static*/ 
//u32 bson_iterator_native_ex::s_bson_iter_key_size(const bson_iter_t* piter)
//{
//	assert(piter);
//
//	return ((piter) && (this_type::s_bson_iter_validate(piter)) && (piter->next_off))?
//				piter->d1 == static_cast<uint32_t>(-1)? 
//					piter->next_off - piter->key - 1 : piter->d1 - piter->key - 1
//			:	0;
//}
//
///*static*/ 
//u32 bson_iterator_native_ex::s_bson_iter_key_buffer_size(const bson_iter_t* piter)
//{
//	assert(piter);
//
//	return ((piter) && (this_type::s_bson_iter_validate(piter)) && (piter->next_off))?
//				piter->d1 == static_cast<uint32_t>(-1)? 
//					piter->next_off - piter->key : piter->d1 - piter->key
//			:	0;
//}

/*static*/ 
u32 bson_iterator_native_ex::s_bson_iter_key_size(const bson_iter_t* piter)
{
	assert(piter);
	assert(piter->d1 != static_cast<u32>(-1));

	return 
		((piter) && (this_type::s_bson_iter_validate(piter)) && (piter->next_off))?
			piter->d1 == 0? piter->next_off - piter->key - 1 : piter->d1 - piter->key - 1
		: 0;
}

/*static*/ 
u32 bson_iterator_native_ex::s_bson_iter_key_buffer_size(const bson_iter_t* piter)
{
	assert(piter);
	assert(piter->d1 != static_cast<u32>(-1));

	return 
		((piter) && (this_type::s_bson_iter_validate(piter)) && (piter->next_off))?
			piter->d1 == 0?  piter->next_off - piter->key : piter->d1 - piter->key 
			: 0;
}

///*static*/ 
//u32 bson_iterator_native_ex::s_bson_iter_d1_size(const bson_iter_t* piter)
//{
//	assert(piter);
//
//	return ((piter) && (this_type::s_bson_iter_validate(piter))
//			&& (piter->d1) && (piter->d1 != static_cast<u32>(-1)) && (piter->next_off)) ?
//				piter->d2 == 0? 
//					piter->next_off - piter->d1 : piter->d2 - piter->d1
//			:	0;
//}

/*static*/ 
u32 bson_iterator_native_ex::s_bson_iter_d1_size(const bson_iter_t* piter)
{
	assert(piter);
	assert(piter->d1 != static_cast<u32>(-1));

	return 
		((piter) && (this_type::s_bson_iter_validate(piter)) && (piter->d1) && (piter->next_off))?
			piter->d2 == 0? piter->next_off - piter->d1 : piter->d2 - piter->d1
			: 0;
}

/*static*/ 
u32 bson_iterator_native_ex::s_bson_iter_d2_size(const bson_iter_t* piter)
{
	assert(piter);

	return 
		((piter) && (this_type::s_bson_iter_validate(piter)) && (piter->d2) && (piter->next_off))?
			piter->d3 == 0? piter->next_off - piter->d2 : piter->d3 - piter->d2
			: 0;
}

/*static*/ 
u32 bson_iterator_native_ex::s_bson_iter_d3_size(const bson_iter_t* piter)
{
	assert(piter);

	return 
		((piter) && (this_type::s_bson_iter_validate(piter)) && (piter->d3) && (piter->next_off))?
				piter->d4 == 0? 
					piter->next_off - piter->d3 : piter->d4 - piter->d3
			:	0;
}

/*static*/ 
u32 bson_iterator_native_ex::s_bson_iter_d4_size(const bson_iter_t* piter)
{
	assert(piter);

	return 
		((piter) && (this_type::s_bson_iter_validate(piter)) && (piter->d4) && (piter->next_off))?
			piter->next_off - piter->d4 
			: 0;
}

/*static*/ 
bool bson_iterator_native_ex::s_bson_iter_make_end(bson_iter_t* pend, const bson_iter_t* pnow)
{
	assert((pend && pnow));

	if(!pnow->len)
	{
		return false;
	}

	uint32_t len = pnow->len;
	memset(pend, 0, sizeof(bson_iter_t));
	pend->key = len;
	pend->type = pend->off = pend->key - 1;

	return true;
}

/*static*/ 
bool bson_iterator_native_ex::s_bson_iter_find_descendant(bson_iter_t* iter, const char* dotkey, bson_iter_t* descendant)
{
	bson_iter_t tmp = {0};
	const char* dot = 0;
	size_t sublen = 0;

	BSON_ASSERT (iter);
	BSON_ASSERT (dotkey);
	BSON_ASSERT (descendant);

	if((dot = strchr(dotkey, '.')))
	{
		sublen = dot - dotkey;
	}
	else 
	{
		sublen = strlen (dotkey);
	}

	if(this_type::s_bson_iter_find_with_len(iter, dotkey, static_cast<int>(sublen))) 
	{
		if(!dot) 
		{
			*descendant = *iter;
			return true;
		}

		if(BSON_ITER_HOLDS_DOCUMENT(iter) || BSON_ITER_HOLDS_ARRAY(iter)) 
		{
			if(bson_iter_recurse (iter, &tmp)) 
			{
				return this_type::s_bson_iter_find_descendant(&tmp, dot + 1, descendant);
			}
		}
	}

	return false;
}

/*static*/ 
bool bson_iterator_native_ex::s_bson_iter_find_descendant_case(bson_iter_t* iter, const char * dotkey, bson_iter_t* descendant)
{
	bson_iter_t tmp = {0};
	const char* dot = 0;
	size_t sublen = 0;

	BSON_ASSERT (iter);
	BSON_ASSERT (dotkey);
	BSON_ASSERT (descendant);

	if((dot = strchr(dotkey, '.')))
	{
		sublen = dot - dotkey;
	} 
	else 
	{
		sublen = strlen (dotkey);
	}

	if(this_type::s_bson_iter_find_case_with_len(iter, dotkey, static_cast<int>(sublen))) 
	{
		if(!dot) 
		{
			*descendant = *iter;
			return true;
		}

		if(BSON_ITER_HOLDS_DOCUMENT(iter) || BSON_ITER_HOLDS_ARRAY(iter)) 
		{
			if(bson_iter_recurse (iter, &tmp)) 
			{
				return this_type::s_bson_iter_find_descendant_case(&tmp, dot + 1, descendant);
			}
		}
	}

	return false;
}

//private:
/*static*/ 
bool bson_iterator_native_ex::s_bson_iter_find_with_len(bson_iter_t* iter, const char* key, int keylen)
{
	const char* ikey = 0;

	if(keylen == 0) 
	{
		return false;
	}

	if(keylen < 0) 
	{
		keylen = static_cast<int>(strlen(key));
	}

	do
	{
		if((0 == iter->off && iter->next_off == 4))
		{
			continue;
		}

		ikey = bson_iter_key (iter);

		if((0 == std::strncmp(key, ikey, keylen)) && (ikey[keylen] == '\0')) 
		{
			return true;
		}

	} while(bson_iter_next(iter));

	return false;
}

/*static*/
bool bson_iterator_native_ex::s_bson_iter_find_case_with_len(bson_iter_t* iter, const char* key, int keylen) /* IN */
{
	const char* ikey = 0;

	if(keylen == 0) 
	{
		return false;
	}

	if(keylen < 0) 
	{
		keylen = static_cast<int>(strlen(key));
	}

	do
	{
		if((0 == iter->off && iter->next_off == 4))
		{
			continue;
		}

		ikey = bson_iter_key (iter);

		if((0 == charset::charset_base_foo::strncmp_case(key, ikey, keylen))
			&& (ikey[keylen] == '\0')) 
		{
			return true;
		}

	} while(bson_iter_next(iter));

	return false;
}

} // namesapce nsql_database_system
} // namespace yggr


namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{

void swap(bson_iter_t& l, bson_iter_t& r)
{
	bson_iterator_native_ex::s_bson_iter_swap(boost::addressof(l), boost::addressof(r));
}

} // namespace swap_support
} // namespace nsql_database_system
} // namespace yggr
