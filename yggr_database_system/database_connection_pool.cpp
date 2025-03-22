//database_connection_pool.cpp

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

#include <yggr/database_system/database_connection_pool.hpp>
#include <yggr/bind/bind.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

namespace yggr
{
namespace database_system
{

database_connection_pool::database_connection_pool(void)
{
}

database_connection_pool::database_connection_pool(const init_type& init)
	: _init_wrap(init)
{
}

database_connection_pool::~database_connection_pool(void)
{
}

// don't use swap
//void swap(this_type& right);

database_connection_pool::org_conn_ptr_type 
	database_connection_pool::pop(void)
{
	init_type init;

	try
	{
		init = _init_wrap.load();
	}
	catch(const init_wrap_type::error_type&)
	{
		return org_conn_ptr_type();
	}

	org_conn_ptr_type ptr;
	bool bsuccess = _conn_queue.pop(ptr);

	if(!bsuccess)
	{
		org_conn_ptr_type tmp = ::yggr::make_shared<conn_type>();

		if(!tmp)
		{
			return ptr;
		}
		
		try
		{
			tmp->Connect(init.dsn_string(), init.prompt(), init.auto_commit());
		}
		catch(const org_err_type& e)
		{
			std::cerr << e << std::endl;
			return org_conn_ptr_type();
		}
		catch(const org_root_err_type& e)
		{
			std::cerr << e << std::endl;
			return org_conn_ptr_type();
		}

		ptr.swap(tmp);

		if(init.dsn_string() != ptr->GetDSN())
		{
			init.dsn_string() = ptr->GetDSN();
			_init_wrap.store(init);
		}
		ptr->SetAutoCommit(init.auto_commit());
	}

	return ptr;
}

} // namespace database_system
} // namespace yggr

