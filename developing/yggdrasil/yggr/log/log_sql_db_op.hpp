//log_sql_db_op.hpp

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

#ifndef __YGGR_LOG_LOGOP_LOG_SQL_DB_OP_HPP__
#define __YGGR_LOG_LOGOP_LOG_SQL_DB_OP_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/nonable/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>

#include <yggr/database_system/database_mgr.hpp>
#include <yggr/charset/string.hpp>
#include <vector>

namespace yggr
{
namespace log
{
namespace log_op
{

template<typename Def, typename DB_Operator, typename Packet>
class log_sql_db_op : private nonable::noncopyable
{
public:

	typedef Def def_type;
	enum
	{
		E_LOG_OP_TYPE = def_type::E_LOG_OP_SQL_DB,
		E_id_compile_u32 = 0xffffffff
	};


	typedef typename def_type::id_type id_type;

	typedef DB_Operator db_operator_type;
	typedef typename db_operator_type::conn_type conn_type;
	//typedef Conn conn_type;
	typedef boost::shared_ptr<conn_type> conn_ptr_type;
	typedef conn_ptr_type init_type;

	typedef db_operator_type real_op_type;

	typedef Packet pak_type;
	typedef pak_type opak_type;
	typedef pak_type ipak_type;

private:
	typedef log_sql_db_op<def_type, conn_type, pak_type> this_type;
	typedef std::vector<pak_type> pak_vt_type;

public:
	log_sql_db_op(const init_type& init)
		: _pconn(init)
	{
	}

	~log_sql_db_op(void)
	{
	}

#	include <yggr/support/log_op_general_foo.ipp>

	template<typename Tuple_Handler>
	void append(const Tuple_Handler& handler)
	{
		if(!keep_connect())
		{
			return;
		}
		pak_vt_type pak_vt(1);
		pak_vt[0] = boost::get<1>(handler);
		real_op_type::insert_records_of_table(boost::get<0>(handler), pak_vt, boost::get<2>(handler), *_pconn);
	}

	template<typename Tuple_Handler>
	void clear(const Tuple_Handler& handler)
	{
		if(!keep_connect())
		{
			return;
		}

		std::stringstream ss;
		ss << "DELETE FROM " << boost::get<0>(handler);
		real_op_type::execute_sql(ss.str(), *_pconn);
	}

	template<typename Tuple_Handler>
	void search(const Tuple_Handler& handler)
	{
		if(!keep_connect())
		{
			return;
		}
		pak_vt_type pak_vt;
		real_op_type::select_records_of_view(boost::get<0>(handler), pak_vt, boost::get<1>(handler), *_pconn);
		(boost::get<2>(handler))(pak_vt);
	}

private:
	bool keep_connect(void)
	{
		if(!_pconn->IsConnected())
		{
			try
			{
				_pconn->Connect();
			}
			catch(const dtl::DBException& e)
			{
				exception::exception::throw_error(exception::exception::E_DTL_DB_ERR_CODE, e.what());
				return false;
			}
			catch(const dtl::RootException& e)
			{
				exception::exception::throw_error(exception::exception::E_DTL_ROOT_ERR_CODE, e.what());
				return false;
			}
		}

		return true;
	}

private:
	conn_ptr_type _pconn;
};

} // namespce log_op
} // namspace log
} // namespace yggr

#endif //__YGGR_LOG_LOGOP_LOG_SQL_DB_OP_HPP__
