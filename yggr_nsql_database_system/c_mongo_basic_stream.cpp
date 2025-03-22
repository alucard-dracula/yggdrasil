//c_mongo_basic_stream.cpp

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

#include <yggr/nsql_database_system/c_mongo_basic_stream.hpp>
#include <yggr/utility/swap.hpp>

#include <cassert>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

//----------------c_mongo_basic_stream----------------
c_mongo_basic_stream::c_mongo_basic_stream(void)
	: _need_del(true)
{
}

c_mongo_basic_stream::c_mongo_basic_stream(org_type* pstream)
	: _need_del(true)
{
	this_type::pro_init(pstream, true);
}

c_mongo_basic_stream::c_mongo_basic_stream(org_type* pstream, bool need_del)
	: _need_del(true)
{
	this_type::pro_init(pstream, need_del);
}

c_mongo_basic_stream::~c_mongo_basic_stream(void)
{
}

void c_mongo_basic_stream::swap(this_type& right)
{
	this_type::pro_swap(right);
}

//protected:

void c_mongo_basic_stream::pro_init(org_type* pstream, bool need_del)
{
	if(!pstream)
	{
		{
			stream_ptr_type tmp;
			_pstream.swap(tmp);
		}
		_need_del = true;
	}
	else
	{
		// using {} release old stream_ptr
		{
			stream_ptr_type tmp(pstream, shared_ptr_deleter_type(*this));
			_pstream.swap(tmp);
		}
		_need_del = need_del;
	}
}

void c_mongo_basic_stream::pro_move(this_type& right)
{
	if(this == boost::addressof(right))
	{
		return;
	}

	org_type* pstream = right.org_pointer();
	bool need_del = right.need_delete();

	this_type::pro_init(pstream, need_del);
	right.release();
}

void c_mongo_basic_stream::pro_swap(this_type& right)
{
	if(this == boost::addressof(right))
	{
		return;
	}

	org_type* pstream_l = this_type::org_pointer();
	bool need_del_l = right.need_delete();

	this_type::pro_move(right);
	right.pro_init(pstream_l, need_del_l);
}

} // namespace nsql_database_system
} // namespace yggr
