//c_mongo_file_stream.cpp

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

#include <yggr/nsql_database_system/c_mongo_file_stream.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

c_mongo_file_stream::c_mongo_file_stream(void)
{
}

c_mongo_file_stream::c_mongo_file_stream(base_org_type* pstream)
{
	if(pstream && pstream->type == base_type::E_STREAM_T_GRIDFS)
	{
		base_type::pro_init(pstream, true);
	}
}

c_mongo_file_stream::c_mongo_file_stream(base_org_type* pstream, bool need_del)
{
	if(pstream && pstream->type == base_type::E_STREAM_T_GRIDFS)
	{
		base_type::pro_init(pstream, need_del);
	}
}

c_mongo_file_stream::c_mongo_file_stream(int fd)
	: base_type(mongoc_stream_file_new(fd))
{
}

c_mongo_file_stream::c_mongo_file_stream(const char* fpath, int flags, int mode)
	: base_type(mongoc_stream_file_new_for_path(fpath, flags, mode))
{
}

c_mongo_file_stream::~c_mongo_file_stream(void)
{
}

void c_mongo_file_stream::swap(this_type& right)
{
	if(this == boost::addressof(right))
	{
		return;
	}

	base_type::pro_swap(right);
}

} // namespace nsql_database_system
} // namespace yggr
