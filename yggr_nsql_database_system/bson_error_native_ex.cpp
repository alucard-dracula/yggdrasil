//bson_error_native_ex.cpp

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

#include <yggr/nsql_database_system/bson_error_native_ex.hpp>
#include <cassert>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

/*static*/ 
bson_error_t* bson_error_native_ex::s_bson_error_init_non_check(bson_error_t* pberr)
{
	return pberr? (memset(pberr, 0, sizeof(bson_error_t)), pberr) : pberr;
}

/*static*/ 
int bson_error_native_ex::s_bson_error_init_va_list(bson_error_t* pberr, u32 domain, u32 code, const char* format, va_list ap)
{
	int r = -1;

	if(pberr) 
	{
		pberr->domain = domain;
		pberr->code = code;

		r = bson_vsnprintf(pberr->message, sizeof(pberr->message), format, ap);

		pberr->message[sizeof(pberr->message) - 1] = '\0';
	}

	return r;
}

/*static*/ 
bson_error_t* bson_error_native_ex::s_bson_error_init(bson_error_t* pberr, u32 domain, u32 code, const char* format, ...)
{
	assert(pberr);
	if(!pberr)
	{
		return 0;
	}

	va_list args;
	va_start(args, format);
	this_type::s_bson_error_init_va_list(pberr, domain, code, format, args);
    va_end(args);
	return pberr;
}

/*static*/ 
bson_error_t* bson_error_native_ex::s_bson_error_copy(bson_error_t* pdst, const bson_error_t* psrc)
{
	assert((psrc && pdst));
	if(!(psrc && pdst))
	{
		return 0;
	}

	if(pdst == psrc)
	{
		return pdst;
	}
	memcpy(pdst, psrc, sizeof(bson_error_t));
	return pdst;
}

/*static*/ 
void bson_error_native_ex::s_bson_error_swap(bson_error_t* pl, bson_error_t* pr)
{
	assert((pl && pr));
	if(!(pl && pr) || (pl == pr))
	{
		return;
	}

	bson_error_t tmp = {0};
	memcpy(&tmp, pl, sizeof(bson_error_t));
	memcpy(pl, pr, sizeof(bson_error_t));
	memcpy(pr, &tmp, sizeof(bson_error_t));
}

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{

void swap(bson_error_t& l, bson_error_t& r)
{
	bson_error_native_ex::s_bson_error_swap(&l, &r);
}

} // namespace swap_support
} // namespace nsql_database_system
} // namespace yggr
