//log_nsql_db_op.hpp

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

#ifndef __YGGR_LOG_LOGOP_LOG_NSQL_DB_OP_HPP__
#define __YGGR_LOG_LOGOP_LOG_NSQL_DB_OP_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/nonable/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>

#include <yggr/nsql_database_system/c_mongo_connection.hpp>
#include <yggr/nsql_database_system/mongo_accesser.hpp>

#include <yggr/archive/bson_archive_partner.hpp>
#include <yggr/packet/packet.hpp>



namespace yggr
{
namespace log
{
namespace log_op
{

template<typename Def>
class log_nsql_db_op : private nonable::noncopyable
{
public:

	typedef Def def_type;
	enum
	{
		E_LOG_OP_TYPE = def_type::E_LOG_OP_NSQL_DB,
		E_id_compile_u32 = 0xffffffff
	};


	typedef typename def_type::id_type id_type;

	//typedef nsql_database_system::c_mongo_connection<Len> conn_type;
	typedef nsql_database_system::c_mongo_connection conn_type;
	typedef nsql_database_system::mongo_accesser<conn_type> real_op_type;

	typedef packet::packet<archive::archive_partner::bson_oarchive_partner> opak_type;
	typedef packet::packet<archive::archive_partner::bson_iarchive_partner> ipak_type;;

	//typedef typename conn_type::c_mongo_connection_ptr_type conn_ptr_type;
	typedef typename conn_type::this_ptr_type conn_ptr_type;
	typedef conn_ptr_type init_type;

private:
	typedef log_nsql_db_op this_type;

public:
	log_nsql_db_op(const init_type& init)
		: _pconn(init), _real_op(init)
	{
	}

	~log_nsql_db_op(void)
	{
	}

#	include <yggr/support/log_op_general_foo.ipp>

	template<typename Tuple_Handler>
	bool append(const Tuple_Handler& handler)
	{
		if(!keep_connect())
		{
			return false;
		}
		opak_type opak;
		opak.save(boost::get<2>(handler));
		return 0 == _real_op.insert(boost::get<0>(handler), boost::get<1>(handler), opak);
	}

	template<typename Tuple_Handler>
	bool clear(const Tuple_Handler& handler)
	{
		if(!keep_connect())
		{
			return false;
		}
		return 0 == _real_op.remove_all(boost::get<0>(handler), boost::get<1>(handler));
	}

	inline bool clear(void)
	{
		assert(false);
		return false;
	}

	template<typename Tuple_Handler>
	bool search(const Tuple_Handler& handler)
	{
		Tuple_Handler& ref_handler = const_cast<Tuple_Handler&>(handler);
		if(!keep_connect())
		{
			return false;
		}
		return 0 == _real_op.find(boost::get<0>(ref_handler), boost::get<1>(ref_handler),
									boost::get<2>(ref_handler), boost::get<3>(ref_handler),
									boost::get<4>(ref_handler), boost::get<5>(ref_handler),
									boost::get<6>(ref_handler), boost::get<7>(ref_handler));
	}

private:
	bool keep_connect(void)
	{
		if(!_pconn)
		{
			return false ;
		}

		if(_pconn->is_connected())
		{
			return true;
		}

		return _pconn->connect();
	}

private:
	conn_ptr_type _pconn;
	real_op_type _real_op;
};

} // namespace log_op
} // namespace log
} // namespace yggr

#endif //__YGGR_LOG_LOGOP_LOG_NSQL_DB_OP_HPP__
