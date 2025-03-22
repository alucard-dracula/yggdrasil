// c_mongo_mc_kms_credentials_callback.cpp

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

#include <yggr/nsql_database_system/c_mongo_mc_kms_credentials_callback.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

//public:

c_mc_kms_credentials_callback::c_mc_kms_credentials_callback(void)
{
	base_type::fn = 0;
	base_type::userdata = 0;
}

c_mc_kms_credentials_callback::c_mc_kms_credentials_callback(callback_fn_type pfoo, void* puserdata)
{
	base_type::fn = pfoo;
	base_type::userdata = puserdata;
}

c_mc_kms_credentials_callback::c_mc_kms_credentials_callback(const this_type& right)
{
	base_type::fn = right.fn;
	base_type::userdata = right.userdata;
}

c_mc_kms_credentials_callback::~c_mc_kms_credentials_callback(void)
{
}

//public:
	
c_mc_kms_credentials_callback::this_type& 
	c_mc_kms_credentials_callback::operator=(const this_type& right)
{
	if(this == boost::addressof(right))
	{
		return *this;
	}

	base_type::fn = right.fn;
	base_type::userdata = right.userdata;
		
	return *this;
}

//public:

void c_mc_kms_credentials_callback::swap(this_type& right)
{
	if(this == boost::addressof(right))
	{
		return;
	}

	::yggr::swap(base_type::fn, right.fn);
	::yggr::swap(base_type::userdata, right.userdata);
}


} // namespace nsql_database_system
} // namespace yggr

