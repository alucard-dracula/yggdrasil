//c_mongo_write_concern.cpp

/****************************************************************************
Copyright (c) 2014-2018 yggdrasil

author: yang xu

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

#include <algorithm>
#include <yggr/nsql_database_system/c_mongo_write_concern.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER


namespace yggr
{
namespace nsql_database_system
{


c_mongo_write_concern::c_mongo_write_concern(void)
{
	mongo_write_concern_init(this);
	mongo_write_concern_finish(this);
}

c_mongo_write_concern::c_mongo_write_concern(int w_count)
{
	mongo_write_concern_init(this);
	base_type::w = w_count;
	mongo_write_concern_finish(this);
}

c_mongo_write_concern::c_mongo_write_concern(const this_type& right)
{
	this_type::operator=(right);
}

c_mongo_write_concern::~c_mongo_write_concern(void)
{
	mongo_write_concern_destroy(this);
}

c_mongo_write_concern& c_mongo_write_concern::operator=(const this_type& right)
{
	if(this == &right) {return *this;}
	base_type::w = right.w;
	base_type::wtimeout = right.wtimeout;
	base_type::j = right.j;
	base_type::fsync = right.fsync;
	base_type::mode = right.mode;

	if(!right.cmd)
	{
		base_type::cmd = 0;
	}
	else
	{
		base_type::cmd = new bson;
		bson_copy(base_type::cmd, right.cmd);
	}

	return *this;
}

void c_mongo_write_concern::swap(this_type& right)
{
	if(this == &right)
	{
		return;
	}
	std::swap(base_type::w, right.w);
	std::swap(base_type::wtimeout, right.wtimeout);
	std::swap(base_type::j, right.j);
	std::swap(base_type::fsync, right.fsync);
	std::swap(base_type::mode, right.mode);

	if(!right.cmd)
	{
		base_type::cmd = 0;
	}
	else
	{
		std::swap(base_type::cmd, right.cmd);
	}
}

} // namespace nsql_database_system
} // namespace yggr

#define _YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_WRITE_CONCERN_SWAP_IMPL() \
	void swap(yggr::nsql_database_system::c_mongo_write_concern& l, \
				yggr::nsql_database_system::c_mongo_write_concern& r) { \
		l.swap(r);}

namespace std
{
	_YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_WRITE_CONCERN_SWAP_IMPL()
} // namespace std

namespace boost
{
	_YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_WRITE_CONCERN_SWAP_IMPL()
} // naemsapce boost

#undef _YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_WRITE_CONCERN_SWAP_DEF