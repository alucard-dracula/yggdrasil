//c_mongo_connection.cpp

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

#include <yggr/nsql_database_system/c_mongo_connection.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER


namespace yggr
{
namespace nsql_database_system
{

c_mongo_connection::_init_t::_init_t(void)
{
	_si_set.insert(socket_info_type("127.0.0.1", 27017));
}

c_mongo_connection::_init_t::_init_t(const std::string& set_name)
	: _set_name(set_name)
{
}

c_mongo_connection::_init_t::_init_t(const std::string& set_name, const std::string& str_host, port_type nport)
	: _set_name(set_name)
{
	_si_set.insert(socket_info_type(str_host, nport));
}

c_mongo_connection::_init_t::_init_t(const std::string& set_name, const std::string& str_host, const std::string& str_port)
	: _set_name(set_name)
{
	port_type port = 0;

	std::stringstream ss;
	ss << str_port;
	ss >> port;

	_si_set.insert(socket_info_type(str_host, port));
}

c_mongo_connection::_init_t::_init_t(const std::string& set_name, const std::string& str) //str: ("ip:nport")
	: _set_name(set_name)
{
	parse_string(str);
}

//c_mongo_connection::_init_t::_init_t(BOOST_RV_REF(c_mongo_connection::_init_t) right)
//{
//	_init_t::swap(right);
//}

c_mongo_connection::_init_t::_init_t(const _init_t& right)
	: _set_name(right._set_name), _si_set(right._si_set)
{
}

c_mongo_connection::_init_t::~_init_t(void)
{
	socket_info_set_type tset;
	tset.swap(_si_set);
	tset.clear();
}

c_mongo_connection::_init_t& c_mongo_connection::_init_t::add_seed(const std::string& str_host, port_type& nport)
{
	_si_set.insert(socket_info_type(str_host, nport));
	return *this;
}

c_mongo_connection::_init_t& c_mongo_connection::_init_t::operator<<(const std::string& addr)
{
	parse_string(addr);
	return *this;
}


//c_mongo_connection::_init_t& c_mongo_connection::_init_t::operator=(BOOST_RV_REF(c_mongo_connection::_init_t) right)
//{
//	_init_t::swap(right);
//	return *this;
//}

c_mongo_connection::_init_t& c_mongo_connection::_init_t::operator=(const c_mongo_connection::_init_t& right)
{
	_set_name = right._set_name;
	_si_set = right._si_set;
	return *this;
}

bool c_mongo_connection::_init_t::empty(void) const
{
	return _si_set.empty();
}

size_type c_mongo_connection::_init_t::size(void) const
{
	return _si_set.size();
}

const std::string& c_mongo_connection::_init_t::name(void) const
{
	return _set_name;
}

void c_mongo_connection::_init_t::swap(_init_t& right)
{
	_set_name.swap(right._set_name);
	_si_set.swap(right._si_set);
}

void c_mongo_connection::_init_t::parse_string(const std::string& str)
{
	std::vector<std::string> str_vt;
	std::string str_re(":");

	regular_parse::regex_parse::split(str_vt, str, str_re);

	if(str_vt.empty() || str_vt.size() < 2)
	{
		return;
	}

	_si_set.insert(socket_info_type(str_vt[0], atoi(&str_vt[1][0])));

	str_vt.clear();
}

//-------------------------------------c_mongo_connection------------------------------

c_mongo_connection::c_mongo_connection(void)
{
	base_type& conn = *this;
	memset(&conn, 0, sizeof(base_type));
	mongodb_init::init_network_context();//try catch
}

c_mongo_connection::c_mongo_connection(const std::string& str)
	: _init("", str)
{
	base_type& conn = *this;
	memset(&conn, 0, sizeof(base_type));
	mongodb_init::init_network_context();
}

c_mongo_connection::c_mongo_connection(const std::string& str_host, u16 port)
	: _init("", str_host, port)
{
	base_type& conn = *this;
	memset(&conn, 0, sizeof(base_type));
	mongodb_init::init_network_context();
}

//c_mongo_connection::c_mongo_connection(BOOST_RV_REF(init_type) init)
//	: _init(boost::forward<init_type>(init))
//{
//	base_type& conn = *this;
//	memset(&conn, 0, sizeof(base_type));
//	mongodb_init::init_network_context();//try catch
//}

c_mongo_connection::c_mongo_connection(const init_type& init)
	: _init(init)
{
	base_type& conn = *this;
	memset(&conn, 0, sizeof(base_type));
	mongodb_init::init_network_context();//try catch
}

c_mongo_connection::~c_mongo_connection(void)
{
	close();
	if(now_ver())
	{
		mongo_destroy(this);
	}
}

bool c_mongo_connection::connect(void)
{
	if(connected)
	{
		return true;
	}

	if(_init.empty())
	{
		return false;
	}

	u32 error = 0;
	u32 ver = grab();

	if(ver)
	{
		error = mongo_reconnect(this);
	}
	else
	{
		error = _init.use_handler(boost::bind(&this_type::prv_connect, this, _1, _2));
	}

	if(error)
	{
		exception::exception::throw_error(error_maker_type::make_error(err));
		if(base_type::errcode)
		{
			exception::exception::throw_error(base_type::errcode, base_type::errstr);
		}
		return false;
	}

	return true;
}

bool c_mongo_connection::is_connected(void) const
{
	return connected;
}

bool c_mongo_connection::reset_connect(void)
{
	connected = 0;
	return true;
}

bool c_mongo_connection::reset_connect_of_ver(u32 ver)
{
	if(now_ver() != ver)
	{
		return false;
	}

	connected = 0;
	grab();
	return true;
}

bool c_mongo_connection::reconnect(void)
{
	if(!now_ver())
	{
		return connect();
	}

	grab();
	u32 error = mongo_reconnect(this);
	if(error)
	{
		exception::exception::throw_error(error_maker_type::make_error(err));

		if(base_type::errcode)
		{
			exception::exception::throw_error(base_type::errcode, base_type::errstr);
		}
		return false;
	}

	return true;
}

bool c_mongo_connection::reconnect_of_ver(u32 ver)
{
	if(now_ver() != ver)
	{
		return false;
	}

	return reconnect();
}

u64 c_mongo_connection::id(void) const
{
	return (u64)this;
}

bool c_mongo_connection::close(void)
{
	if(!connected)
	{
		return true;
	}

	mongo_disconnect(this);
	return true;
}

u32 c_mongo_connection::prv_connect(const std::string& set_name, const init_type::socket_info_set_type& set)
{
	typedef init_type::socket_info_set_citer_type iter_type;

	assert(!_init.empty());
	if(_init.size() == 1)
	{
		iter_type i = set.begin();
		return mongo_client(this, i->host().c_str(), i->port()); 
	}
		
	mongo_replica_set_init(this, set_name.c_str());
	for(iter_type i = set.begin(), isize = set.end(); i != isize; ++i)
	{
		mongo_replica_set_add_seed(this, i->host().c_str(), i->port());
	}
			
	return mongo_replica_set_client(this);
		
}

} // namespace nsql_database_system
} // namespace yggr
