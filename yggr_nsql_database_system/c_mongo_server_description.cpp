//c_mongo_server_description.cpp

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

#include <yggr/nsql_database_system/c_mongo_server_description.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

c_mongo_server_description::c_mongo_server_description(void)
{
}

c_mongo_server_description::c_mongo_server_description(mongoc_server_description_t* pdesc)
	: _pdesc(pdesc, (desc_deleter_type()))
{
}

c_mongo_server_description::c_mongo_server_description(const this_type& right)
	: _pdesc((mongoc_server_description_new_copy(right._pdesc.get())), (desc_deleter_type()))
{
}

c_mongo_server_description::~c_mongo_server_description(void)
{
}

//public:

c_mongo_server_description::this_type& 
	c_mongo_server_description::operator=(const this_type& right)
{
	if(this == boost::addressof(right))
	{
		return *this;
	}

	desc_ptr_type tmp((mongoc_server_description_new_copy(right._pdesc.get())), 
						(desc_deleter_type()));
	tmp.swap(_pdesc);

	return*this;
}

//public:

void c_mongo_server_description::swap(this_type& right)
{
	if(this == boost::addressof(right))
	{
		return;
	}

	_pdesc.swap(right._pdesc);
}

} // namespace nsql_database_system
} // namespace yggr
