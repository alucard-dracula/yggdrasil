// c_mongo_connection_encryption.cpp

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

#include <yggr/nsql_database_system/c_mongo_connection_encryption.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

//public:
c_mongo_connection_encryption::c_mongo_connection_encryption(void)
{
}

c_mongo_connection_encryption::~c_mongo_connection_encryption(void)
{
	assert(!_poped_pclt_wrap);
	if(_poped_pclt_wrap)
	{
		mongoc_client_ptr_type poped_clt;
		_poped_pclt_wrap.swap(poped_clt);

		if(_pconn)
		{
			_pconn->push(poped_clt);
		}
	}
}

//public:
c_mongo_connection_encryption::mongoc_client_encryption_ptr_type 
	c_mongo_connection_encryption::pop(void)
{
	mongoc_client_ptr_type poped_clt;

	if(_pconn && (poped_clt = _pconn->pop()))
	{
		assert(!_poped_pclt_wrap);
		if(_poped_pclt_wrap)
		{
			bool bpush = _pconn->push(poped_clt);
			assert(bpush);
			return mongoc_client_encryption_ptr_type();
		}

		_poped_pclt_wrap.swap(poped_clt);
		return _penc;
	}
	else
	{
		return mongoc_client_encryption_ptr_type();
	}
}

bool c_mongo_connection_encryption::push(const mongoc_client_encryption_ptr_type& ptr)
{
	if(!(_pconn && _poped_pclt_wrap && ptr == _penc))
	{
		return false;
	}

	mongoc_client_ptr_type poped_clt;
	_poped_pclt_wrap.swap(poped_clt);

	return poped_clt && _pconn->push(poped_clt);
}

} // namespace nsql_database_system
} // namespace yggr

