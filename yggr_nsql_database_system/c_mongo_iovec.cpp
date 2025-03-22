//c_mongo_iovec.cpp

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

#include <yggr/nsql_database_system/c_mongo_iovec.hpp>
#include <yggr/utility/swap.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

c_mongo_iovec::c_mongo_iovec(void)
{
	base_type& base = *this;
	memset(&base, 0, sizeof(base_type));
}

c_mongo_iovec::c_mongo_iovec(u32 byte_size, char* buf_ptr)
{
	base_type::iov_len = byte_size;
	base_type::iov_base = buf_ptr;
}

c_mongo_iovec::c_mongo_iovec(const base_type& right)
{
	base_type::iov_len = right.iov_len;
	base_type::iov_base = right.iov_base;
}

c_mongo_iovec::c_mongo_iovec(const this_type& right)
{
	base_type::iov_len = right.iov_len;
	base_type::iov_base = right.iov_base;
}

c_mongo_iovec::~c_mongo_iovec(void)
{
}

c_mongo_iovec::this_type& c_mongo_iovec::operator=(const base_type& right)
{
	if(this == &right)
	{
		return *this;
	}

	base_type::iov_len = right.iov_len;
	base_type::iov_base = right.iov_base;
	return *this;
}

c_mongo_iovec::this_type& c_mongo_iovec::operator=(const this_type& right)
{
	if(this == &right)
	{
		return *this;
	}

	base_type::iov_len = right.iov_len;
	base_type::iov_base = right.iov_base;
	return *this;
}

void c_mongo_iovec::swap(base_type& right)
{
	if(this == &right)
	{
		return;
	}

	yggr::swap(base_type::iov_len, right.iov_len);
	yggr::swap(base_type::iov_base, right.iov_base);
}

void c_mongo_iovec::swap(this_type& right)
{
	if(this == &right)
	{
		return;
	}

	yggr::swap(base_type::iov_len, right.iov_len);
	yggr::swap(base_type::iov_base, right.iov_base);
}

} // namespace nsql_database_system
} // namespace yggr
