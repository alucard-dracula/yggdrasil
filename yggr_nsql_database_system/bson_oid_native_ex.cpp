//bson_oid_native_ex.cpp

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

#include <yggr/nsql_database_system/bson_oid_native_ex.hpp>
#include <yggr/math/sign.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

/*static*/ 
bool bson_oid_native_ex::s_bson_oid_swap(bson_oid_t* pl, bson_oid_t* pr)
{
	assert(pl && pr);
	if(!(pl && pr))
	{
		return false;
	}

	if(pl != pr)
	{
		bson_oid_t tmp = {0};
		memcpy(boost::addressof(tmp), pl, this_type::E_bson_oid_byte_size);
		memcpy(pl, pr, this_type::E_bson_oid_byte_size);
		memcpy(pr, boost::addressof(tmp), this_type::E_bson_oid_byte_size);
	}
		
	return true;
}

/*static*/ 
bool bson_oid_native_ex::s_bson_oid_compare_eq(const bson_oid_t* poid, const char* str_oid)
{
	assert(poid);
	bson_oid_t tmp = {0};

	if(str_oid)
	{
		bson_oid_init_from_string(boost::addressof(tmp), str_oid);
	}
	return (poid) && (0 == memcmp(poid, boost::addressof(tmp), this_type::E_bson_oid_byte_size));
}

/*static*/ 
bool bson_oid_native_ex::s_bson_oid_compare_eq(const bson_oid_t* poid, const char* str_oid, std::size_t n)
{
	assert(poid);
	bson_oid_t tmp = {0};

	if(str_oid)
	{
		if(n == this_type::E_bson_oid_string_size)
		{
			bson_oid_init_from_string(boost::addressof(tmp), str_oid);
			return (poid) && (0 == memcmp(poid, boost::addressof(tmp), this_type::E_bson_oid_byte_size));
		}
		else if(n == this_type::E_bson_oid_byte_size)
		{
			return (poid) && (0 == memcmp(poid->bytes, str_oid, this_type::E_bson_oid_byte_size));
		}
		else
		{
			return false;
		}
	}
	else
	{
		return (poid) && (0 == memcmp(poid, boost::addressof(tmp), this_type::E_bson_oid_byte_size));
	}
}

/*static*/ 
s32 bson_oid_native_ex::s_bson_oid_compare(const bson_oid_t* poid, const char* str_oid)
{
	assert(poid);
	bson_oid_t tmp = {0};

	if(str_oid)
	{
		bson_oid_init_from_string(boost::addressof(tmp), str_oid);
		// in linux gcc memcmp return value is not formated, so using math sign foramt it
		return poid? math::sign(memcmp(poid, boost::addressof(tmp), this_type::E_bson_oid_byte_size)) : -1;
	}
	else
	{
	    // in linux gcc memcmp return value is not formated, so using math sign fora
		return (poid)? math::sign(memcmp(poid, boost::addressof(tmp), this_type::E_bson_oid_byte_size)) : 1;
	}
}

/*static*/ 
s32 bson_oid_native_ex::s_bson_oid_compare(const bson_oid_t* poid, const char* str_oid, std::size_t n)
{
	assert(poid);
	bson_oid_t tmp = {0};

	if(str_oid)
	{
		if(n == this_type::E_bson_oid_byte_size)
		{
		    // in linux gcc memcmp return value is not formated, so using math sign fora
			return (poid)? math::sign(memcmp(poid->bytes, str_oid, this_type::E_bson_oid_byte_size)) : -1;
		}
		else
		{
			bson_oid_init_from_string(boost::addressof(tmp), str_oid);
			// in linux gcc memcmp return value is not formated, so using math sign fora
			return (poid)? math::sign(memcmp(poid, boost::addressof(tmp), this_type::E_bson_oid_byte_size)) : -1;
		}
	}
	else
	{
	    // in linux gcc memcmp return value is not formated, so using math sign fora
		return (poid)? math::sign(memcmp(poid, boost::addressof(tmp), this_type::E_bson_oid_byte_size)) : 1;
	}
}

// !!!wchar_t c16 c32 using template version !!!

#ifndef YGGR_NO_CHAR8_T

/*static*/ 
bool bson_oid_native_ex::s_bson_oid_compare_eq(const bson_oid_t* poid, const c8* str_oid)
{
	return this_type::s_bson_oid_compare_eq(poid, reinterpret_cast<const char*>(str_oid));
}

/*static*/ 
bool bson_oid_native_ex::s_bson_oid_compare_eq(const bson_oid_t* poid, const c8* str_oid, std::size_t n)
{
	return this_type::s_bson_oid_compare_eq(poid, reinterpret_cast<const char*>(str_oid), n);
}

/*static*/ 
s32 bson_oid_native_ex::s_bson_oid_compare(const bson_oid_t* poid, const c8* str_oid)
{
	return this_type::s_bson_oid_compare(poid, reinterpret_cast<const char*>(str_oid));
}

/*static*/ 
s32 bson_oid_native_ex::s_bson_oid_compare(const bson_oid_t* poid, const c8* str_oid, std::size_t n)
{
	return this_type::s_bson_oid_compare(poid, reinterpret_cast<const char*>(str_oid), n);
}


#endif // YGGR_NO_CHAR8_T

/*static*/ 
bool bson_oid_native_ex::s_bson_oid_compare_eq(const bson_oid_t* pl, const bson_oid_t* pr)
{
	assert(pl && pr);
	return pl && pr && ((pl == pr) || (0 == memcmp(pl, pr, this_type::E_bson_oid_byte_size)));
}


/*static*/ 
s32 bson_oid_native_ex::s_bson_oid_compare(const bson_oid_t* pl, const bson_oid_t* pr)
{
	assert(pl && pr);
	// in linux gcc memcmp return value is not formated, so using math sign fora
	return 
		pl && pr? 
			pl == pr? 
			0
			: math::sign(memcmp(pl, pr, this_type::E_bson_oid_byte_size))
		: pl < pr? -1 : 1;
}

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{

void swap(bson_oid_t& l, bson_oid_t& r)
{
	bson_oid_native_ex::s_bson_oid_swap(boost::addressof(l), boost::addressof(r));
}

} // namespace swap_support
} // namespace nsql_database_system
} // namespace yggr


