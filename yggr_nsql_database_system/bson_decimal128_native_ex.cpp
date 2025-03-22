// bson_decimal128_native_ex.cpp

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

#include <yggr/nsql_database_system/bson_decimal128_native_ex.hpp>

#include <cassert>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

/*static*/ 
char* bson_decimal128_native_ex::s_bson_decimal128_to_string(const bson_decimal128_t* dec, char* out)
{
	assert(dec && out);
	return dec && out? (bson_decimal128_to_string(dec, out), out) : 0;
}

/*static*/ 
bool bson_decimal128_native_ex::s_bson_decimal128_from_string(const char* str, bson_decimal128_t* out)
{
	assert(str && out);
	return str && out && bson_decimal128_from_string(str, out);
}

/*static*/ 
bool bson_decimal128_native_ex::s_bson_decimal128_from_string(const char* str, std::size_t len, bson_decimal128_t* out)
{
	assert(str && out);
	return str && out && bson_decimal128_from_string_w_len(str, static_cast<int>(len), out);
}

// compare
//s_bson_decimal128_compare_eq
/*static*/ 
bool bson_decimal128_native_ex::s_bson_decimal128_compare_eq(const bson_decimal128_t* pdec, u64 nhigh, u64 nlow)
{
	assert(pdec);
	return pdec && pdec->high == nhigh && pdec->low == nlow;
}

/*static*/ 
bool bson_decimal128_native_ex::s_bson_decimal128_compare_eq(const bson_decimal128_t* l, const bson_decimal128_t* r)
{
	assert(l && r);
	return (l == r) || (l->high == r->high && l->low == r->low); 
}

//s_bson_decimal128_compare
//static
s32 bson_decimal128_native_ex::s_bson_decimal128_compare(const bson_decimal128_t* pdec, u64 nhigh, u64 nlow)
{
	assert(pdec);
	return
		pdec->high == nhigh?
			pdec->low == nlow? 
				0
				: pdec->low < nlow? -1 :1
			: pdec->high < nhigh? -1 : 1;
}

/*static*/ 
s32 bson_decimal128_native_ex::s_bson_decimal128_compare(const bson_decimal128_t* l, const bson_decimal128_t* r)
{
	assert(l && r);
	return
		l->high == r->high?
			l->low == r->low? 
				0
				: l->low < r->low? -1 :1
			: l->high < r->high? -1 : 1;
}

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{


void swap(bson_decimal128_t& l, bson_decimal128_t& r)
{
	if(boost::addressof(l) == boost::addressof(r))
	{
		return;
	}

	yggr::swap(l.low, r.low);
	yggr::swap(l.high, r.high);
}

} // namespace swap_support
} // namespace nsql_database_system
} // namespace yggr
