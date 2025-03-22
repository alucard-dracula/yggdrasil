//database_connection.cpp

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

#include <yggr/database_system/database_connection.hpp>
#include <yggr/bind/bind.hpp>
#include <yggr/utility/swap.hpp>

#include <boost/ref.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace database_system
{
namespace detail
{

//------------connection_container-------------------
database_connection_container::database_connection_container(void)
	: _bpoped(false)
{
}

database_connection_container::database_connection_container(const init_type& init)
	: _bpoped(false)
{
	org_conn_ptr_type pconn = ::yggr::make_shared<conn_type>();

	if(!pconn)
	{
		return;
	}

	try
	{
		pconn->Connect(init.dsn_string(), init.prompt(), init.auto_commit());
	}
	catch(const org_err_type& e)
	{
		std::cerr << e << std::endl;
		return;
	}
	catch(const org_root_err_type& e)
	{
		std::cerr << e << std::endl;
		return;
	}
	pconn->SetAutoCommit(init.auto_commit());

	_pconn.swap(pconn);
}

database_connection_container::database_connection_container(const this_type& right)
	: _pconn(right._pconn), _bpoped(right._bpoped)
{
}

database_connection_container::~database_connection_container(void)
{
}


database_connection_container::this_type& 
	database_connection_container::operator=(const this_type& right)
{
	if(this == &right)
	{
		return *this;
	}

	_pconn = right._pconn;
	_bpoped = right._bpoped;

	return *this;
}

void database_connection_container::swap(this_type& right)
{
	if(this == &right)
	{
		return;
	}

	_pconn.swap(right._pconn);
	yggr::swap(_bpoped, right._bpoped);
}

} // namespace detail
} // namespace database_system
} // namespace yggr


namespace yggr
{
namespace database_system
{

//-----------------database_connection---------------------

database_connection::database_connection(void)
	: _pconn_cont_wrap(connection_container_type())
{
}

database_connection::database_connection(const init_type& init)
	: _pconn_cont_wrap(connection_container_type(init))
{
}

database_connection::~database_connection(void)
{
}

void database_connection::swap(this_type& right)
{
	if(this == boost::addressof(right))
	{
		return;
	}

	_pconn_cont_wrap.swap(right._pconn_cont_wrap);
}

} // namespace database_system
} // namespace yggr
