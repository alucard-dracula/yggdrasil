//database_mgr.hpp

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

#ifndef __YGGR_DATABASE_SYSTEM_DATABASE_MANAGER_HPP__
#define __YGGR_DATABASE_SYSTEM_DATABASE_MANAGER_HPP__

#include <yggr/database_system/dtl_include_helper.hpp>
#include <vector>
#include <yggr/charset/string.hpp>
#include <yggr/base/interface_ptr.hpp>
#include <yggr/base/ptr_single.hpp>
#include <yggr/base/exception.hpp>
#include <sstream>
#include <yggr/nonable/noncopyable.hpp>
#include <boost/unordered_map.hpp>
#include <yggr/database_system/database_check_operator.hpp>
//#include <boost/thread/mutex.hpp>

namespace yggr
{
namespace database_system
{

template<typename DB_OP>
class database_mgr : private nonable::noncopyable
{
public:
	typedef DB_OP db_op_type;
	typedef typename db_op_type::conn_type conn_type;

	typedef database_mgr<db_op_type> this_type;
//private:

	//typedef boost::mutex::scoped_lock lock_type;
public:
	//-----------------------------base_view_info S--------------------------------------

	class base_view_info : private nonable::noncopyable
	{

	public:
		typedef std::string id_type;
	public:

		base_view_info(const std::string& op_str)
			: _op_str(op_str)
		{
		}

		~base_view_info(void)
		{
		}

		virtual const id_type id(void) const = 0;
		virtual void execute(conn_type& conn, const std::vector<std::string>& tb_names, const std::string& postfix_str) = 0;

	protected:
		std::string _op_str;

	};

	typedef interface_ptr<base_view_info> view_info_container_type;

	typedef boost::unordered_map<std::string, view_info_container_type> view_info_hash_map_type;

	typedef typename view_info_hash_map_type::iterator view_info_hash_map_iter_type;
	typedef typename view_info_hash_map_type::const_iterator view_info_hash_map_c_iter_type;
	typedef typename view_info_hash_map_type::value_type view_info_hash_map_value_type;
	typedef typename std::pair<view_info_hash_map_iter_type, bool> view_info_hash_map_ins_rst_type;

	//-----------------------------base_view_info S--------------------------------------

	//-----------------------------view_info S-------------------------------------------

	template<typename Tuple, typename Trans_Handler, typename Return_Handler>
	class select_view_info : public base_view_info
	{
	private:
		typedef base_view_info base_type;
		typedef Tuple tuple_type;
		typedef std::vector<tuple_type> rows_type;
		typedef Trans_Handler trans_handler_type;
		typedef Return_Handler return_handler_type;

	public:
		select_view_info(const std::string& op_str, const trans_handler_type& trans_handler, const return_handler_type& return_handler)
			:  base_type(op_str), _trans_handler(trans_handler), _return_handler(return_handler)
		{
		}

		virtual const typename base_type::id_type id(void) const
		{
			return database_check_operator::make_database_command_id<tuple_type>(base_type::_op_str);
		}

		virtual void execute(conn_type& conn, const std::vector<std::string>& tb_names, const std::string& postfix_str)
		{
			rows_type rows;
			bool b_right = _trans_handler(database_check_operator::make_table_names(tb_names), rows, postfix_str, conn);
			_return_handler(b_right, rows);
		}

	private:
		trans_handler_type _trans_handler;
		return_handler_type _return_handler;
	};

//-------------------------------------------------------------------------------------------------------
	template<typename Tuple, typename Trans_Handler, typename Return_Handler>
	class modify_view_info : public base_view_info
	{
	private:
		typedef base_view_info base_type;
		typedef Tuple tuple_type;
		typedef std::vector<tuple_type> rows_type;
		typedef Trans_Handler trans_handler_type;
		typedef Return_Handler return_handler_type;

	public:
		modify_view_info(const std::string& op_str, const trans_handler_type& trans_handler, const return_handler_type& return_handler)
			:  base_type(op_str), _trans_handler(trans_handler), _return_handler(return_handler)
		{
		}

		virtual const typename base_type::id_type id(void) const
		{
			return database_check_operator::make_database_command_id<tuple_type>(base_type::_op_str);
		}

		virtual void execute(conn_type& conn, const std::vector<std::string>& tb_names, const std::string& postfix_str)
		{
			rows_type rows;
			bool b_right = _return_handler(rows);
			if(!b_right || rows.empty())
			{
				return;
			}

			_trans_handler(database_check_operator::make_table_names(tb_names), rows, postfix_str, conn);
		}

	private:
		trans_handler_type _trans_handler;
		return_handler_type _return_handler;
	};

	//-----------------------------view_info E-------------------------------------------

	//-----------------------------view_info_mgr S---------------------------------------

	class view_info_mgr : private nonable::noncopyable
	{
	public:
		view_info_mgr(void)
		{
		}

		~view_info_mgr(void)
		{
			_view_info_map.clear();
		}

		bool reg_info(const view_info_container_type& container)
		{
			return _view_info_map.insert(view_info_hash_map_value_type(container->id(), container)).second;
		}

		void execute(conn_type& conn, const std::string& id, const std::vector<std::string>& tb_names, const std::string& postfix_str)
		{
			view_info_hash_map_iter_type iter = _view_info_map.find(id);
			if(iter != _view_info_map.end())
			{
				iter->second->execute(conn, tb_names, postfix_str);
			}
		}

	private:
		//boost::mutex _mutex;
		view_info_hash_map_type _view_info_map;
	};

	//-----------------------------view_info_mgr E---------------------------------------

public:
	template<typename Tuple, typename Return_Handler>
	static bool register_select_view(const std::string& op_str, const Return_Handler& return_handler)
	{
		typedef Tuple this_tuple_type;
		return register_select_view_helper<this_tuple_type>(op_str,
																boost::bind(&db_op_type::template select_records_of_view<this_tuple_type>, _1, _2, _3, _4),
																return_handler);
	}

	template<typename Tuple, typename Return_Handler>
	static bool register_insert_view(const std::string& op_str, const Return_Handler& return_handler)
	{
		typedef Tuple this_tuple_type;
		return register_modify_view_helper<this_tuple_type>(op_str,
																boost::bind(&db_op_type::template insert_records_of_table<this_tuple_type>, _1, _2, _3, _4),
																return_handler);
	}

	template<typename Tuple, typename Return_Handler>
	static bool register_update_view(const std::string& op_str, const Return_Handler& return_handler)
	{
		typedef Tuple this_tuple_type;
		return register_modify_view_helper<this_tuple_type>(op_str,
																boost::bind(&db_op_type::template update_records_of_table<this_tuple_type>, _1, _2, _3, _4),
																return_handler);
	}

	template<typename Tuple, typename Return_Handler>
	static bool register_delete_view(const std::string& op_str, const Return_Handler& return_handler)
	{
		typedef Tuple this_tuple_type;
		return register_modify_view_helper<this_tuple_type>(op_str,
																boost::bind(&db_op_type::template delete_records_of_table<this_tuple_type>, _1, _2, _3, _4),
																return_handler);
	}

private:
	template<typename Tuple, typename Trans_Handler, typename Return_Handler>
	static bool register_select_view_helper(const std::string& op_str, const Trans_Handler& trans_handler, const Return_Handler& return_handler)
	{

		typedef Tuple this_tuple_type;
		typedef Trans_Handler this_trans_handler_type;
		typedef Return_Handler this_return_handler_type;

		view_info_container_type container(new select_view_info<this_tuple_type,
																	this_trans_handler_type,
																	this_return_handler_type
																>(op_str, trans_handler, return_handler)
											);
		return _view_info_mgr.reg_info(container);
	}

	template<typename Tuple, typename Trans_Handler, typename Return_Handler>
	static bool register_modify_view_helper(const std::string& op_str, const Trans_Handler& trans_handler, const Return_Handler& return_handler)
	{

		typedef Tuple this_tuple_type;
		typedef Trans_Handler this_trans_handler_type;
		typedef Return_Handler this_return_handler_type;

		view_info_container_type container(new modify_view_info<this_tuple_type,
																	this_trans_handler_type,
																	this_return_handler_type
																>(op_str, trans_handler, return_handler)
											);
		return _view_info_mgr.reg_info(container);
	}

public:

	explicit database_mgr(const std::string& link_str, bool b_prompt = false, bool b_auto_commit = false)
	{
		try
		{
			_conn.Connect(link_str, b_prompt, b_auto_commit);
		}
		catch(const dtl::DBException& e)
		{
			exception::exception::throw_error(exception::exception::E_DTL_DB_ERR_CODE, e.what());
		}
		catch(const dtl::RootException& e)
		{
			exception::exception::throw_error(exception::exception::E_DTL_ROOT_ERR_CODE, e.what());
		}
	}

	~database_mgr(void)
	{
	}

	void execute_view(const std::string& id, const std::vector<std::string>& tb_names, const std::string& postfix_str)
	{
		if(!keep_connect())
		{
			return;
		}
		this_type::_view_info_mgr.execute(_conn, id, tb_names, postfix_str);
	}

	void execute_sql(const std::string& str_sql)
	{
		if(!keep_connect())
		{
			return;
		}
		db_op_type::execute_sql(_conn, str_sql);
	}

private:

	bool keep_connect(void)
	{
		if(!_conn.IsConnected())
		{
			try
			{
				_conn.Connect();
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
	conn_type _conn;
private:
	static view_info_mgr _view_info_mgr;

};

template<typename Conn>
typename database_mgr<Conn>::view_info_mgr database_mgr<Conn>::_view_info_mgr;

} //namespace database_system
} //namespace yggr

#endif // __YGGR_DATABASE_SYSTEM_DATABASE_MANAGER_HPP__
