// c_mongo_enc_connection.cpp

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

#include <yggr/nsql_database_system/c_mongo_enc_connection.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

//public:
c_mongo_enc_connection::c_mongo_enc_connection(void)
{
}

c_mongo_enc_connection::~c_mongo_enc_connection(void)
{
	assert(!_poped_keyvault_clt_wrap);
	if(_poped_keyvault_clt_wrap)
	{
		mongoc_client_ptr_type poped_keyvault_clt;
		_poped_keyvault_clt_wrap.swap(poped_keyvault_clt);

		if(_pconn_keyvalut)
		{
			_pconn_keyvalut->push(poped_keyvault_clt);
		}
	}
}

//public:
c_mongo_enc_connection::mongoc_client_ptr_type 
	c_mongo_enc_connection::pop(void)
{
	if(!_pconn || _poped_keyvault_clt_wrap)
	{
		return mongoc_client_ptr_type();
	}

	mongoc_client_ptr_type pclt_keyvault;
	if(_pconn_keyvalut)
	{
		pclt_keyvault = _pconn_keyvalut->pop();

		if(!pclt_keyvault)
		{
			return mongoc_client_ptr_type();
		}
	}

	mongoc_client_ptr_type pclt = _pconn->pop();
	if(!pclt)
	{
		if(pclt_keyvault)
		{
			assert(_pconn_keyvalut);
			bool bpush = _pconn_keyvalut->push(pclt_keyvault);
			assert(bpush);
		}

		return mongoc_client_ptr_type();
	}

	if(pclt_keyvault)
	{
		_poped_keyvault_clt_wrap.swap(pclt_keyvault);
	}
	return pclt;
}

bool c_mongo_enc_connection::push(const mongoc_client_ptr_type pclt)
{
	if(!(_pconn && pclt))
	{
		return false;
	}

	if(!_pconn->revocable(pclt.get()))
	{
		return false;
	}

	if(_pconn_keyvalut)
	{
		assert(_poped_keyvault_clt_wrap);
		mongoc_client_ptr_type pclt_keyvault = _poped_keyvault_clt_wrap.get_shared_ptr();

		assert(_pconn_keyvalut->revocable(pclt_keyvault.get()));
		if(!_pconn_keyvalut->revocable(pclt_keyvault.get()))
		{
			return false;
		}

		{
			mongoc_client_ptr_type tmp;
			_poped_keyvault_clt_wrap.swap(tmp);
		}

		bool bchk = _pconn_keyvalut->push(pclt_keyvault);
		assert(bchk);
	}

	bool bchk = _pconn->push(pclt);
	assert(bchk);

	return true;
}

//protected:
bool c_mongo_enc_connection::pro_format(void)
{
	assert(!_poped_keyvault_clt_wrap);

	if(!!_poped_keyvault_clt_wrap)
	{
		return false;
	}

	if(_pconn == _pconn_keyvalut)
	{
		mongo_connection_ptr_type tmp;
		_pconn_keyvalut.swap(tmp);
	}

	return true;
}

} // namespace nsql_database_system
} // namespace yggr
