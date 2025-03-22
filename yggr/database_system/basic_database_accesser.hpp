//database_operator_zwei.hpp

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

#ifndef __YGGR_DATABASE_SYSTEM_BASIC_DATABASE_ACCESSER_HPP__
#define __YGGR_DATABASE_SYSTEM_BASIC_DATABASE_ACCESSER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/charset/utf8_string.hpp>

#include <yggr/mplex/static_assert.hpp>
#include <yggr/ppex/friend.hpp>

#include <yggr/func/foo_t_info.hpp>

#include <yggr/iterator_ex/iterator.hpp>

#include <yggr/container/clear.hpp>
#include <yggr/container/vector.hpp>

#include <yggr/container_ex/container_mapped_value_t.hpp>

#include <yggr/database_system/dtl_include_helper.hpp>
#include <yggr/database_system/dtl_error.hpp>

#include <boost/range/functions.hpp>
#include <boost/range/value_type.hpp>
#include <boost/range/iterator.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/remove_reference.hpp>

#include <cassert>
#include <algorithm>


namespace yggr
{
namespace database_system
{
namespace detail
{

// keep_connect_helper
struct keep_connect_helper
{
public:
	typedef dtl::DBException org_err_type;
	typedef dtl::RootException org_root_err_type;
	typedef dtl_error error_type;
	typedef dtl_error_code_def error_code_def_type;

private:
	typedef keep_connect_helper this_type;

public:
	template<typename Conn> inline
	bool operator()(Conn& conn, error_type& err) const
	{
		if(!conn.IsConnected())
		{
			try
			{
				conn.Connect();
			}
			catch(const org_err_type& e)
			{
				error_type tmp(error_code_def_type::E_DTL_DB_ERR_CODE, e.what());
				err.swap(tmp);
				return false;
			}
			catch(const org_root_err_type& e)
			{
				error_type tmp(error_code_def_type::E_DTL_ROOT_ERR_CODE, e.what());
				err.swap(tmp);
				return false;
			}
		}

		return true;
	}
};

// connection_using_handler_helper
template<typename Ret>
struct connection_using_handler_helper
{
	typedef Ret ret_type;
	typedef keep_connect_helper keep_connect_helper_type;

	typedef dtl::DBException org_err_type;
	typedef dtl::RootException org_root_err_type;
	typedef dtl_error error_type;
	typedef dtl_error_code_def error_code_def_type;

	BOOST_MPL_ASSERT_NOT((boost::is_reference<ret_type>));

	template<typename Connection_T, typename Handler>
	ret_type operator()(Connection_T& conn, const Handler& handler, error_type& err) const
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::org_conn_ptr_type org_conn_ptr_type;

		typedef Handler handler_type;
		typedef typename func::foo_t_info<handler_type>::result_type cv_ret_type;
		typedef typename boost::remove_reference<cv_ret_type>::type chk_ret_type;

		BOOST_MPL_ASSERT((boost::is_same<ret_type, chk_ret_type>));

		org_conn_ptr_type pconn = conn.pop();
		if(!pconn)
		{
			return ret_type();
		}

		keep_connect_helper_type kch;
		if(!kch(*pconn, err))
		{
			conn.push(pconn);
			return ret_type();
		}

		ret_type ret;
		try
		{
			ret = handler(pconn);
		}
		catch(const org_err_type& e)
		{
			error_type tmp(error_code_def_type::E_DTL_DB_ERR_CODE, e.what());
			err.swap(tmp);
			conn.push(pconn);
			return ret_type();
		}
		catch(const org_root_err_type& e)
		{
			error_type tmp(error_code_def_type::E_DTL_ROOT_ERR_CODE, e.what());
			err.swap(tmp);
			conn.push(pconn);
			return ret_type();
		}

		bool bpushed = conn.push(pconn);
		assert(bpushed);

		return ret;
	}
};

template<>
struct connection_using_handler_helper<void>
{
	typedef void ret_type;
	typedef keep_connect_helper keep_connect_helper_type;

	typedef dtl::DBException org_err_type;
	typedef dtl::RootException org_root_err_type;
	typedef dtl_error error_type;
	typedef dtl_error_code_def error_code_def_type;

	template<typename Connection_T, typename Handler>
	ret_type operator()(Connection_T& conn, const Handler& handler, error_type& err) const
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::org_conn_ptr_type org_conn_ptr_type;

		typedef Handler handler_type;
		typedef typename func::foo_t_info<handler_type>::result_type cv_ret_type;
		typedef typename boost::remove_reference<cv_ret_type>::type chk_ret_type;

		BOOST_MPL_ASSERT((boost::is_same<ret_type, chk_ret_type>));

		org_conn_ptr_type pconn = conn.pop();
		if(!pconn)
		{
			return;
		}

		keep_connect_helper_type kch;
		if(!kch(*pconn, err))
		{
			conn.push(pconn);
			return ret_type();
		}

		try
		{
			handler(pconn);
		}
		catch(const org_err_type& e)
		{
			error_type tmp(error_code_def_type::E_DTL_DB_ERR_CODE, e.what());
			err.swap(tmp);
			conn.push(pconn);
			return ret_type();
		}
		catch(const org_root_err_type& e)
		{
			error_type tmp(error_code_def_type::E_DTL_ROOT_ERR_CODE, e.what());
			err.swap(tmp);
			conn.push(pconn);
			return ret_type();
		}

		bool bpushed = conn.push(pconn);
		assert(bpushed);
	}

};

} // namespace detail
} // namespace database_system
} // namespace yggr

namespace yggr
{
namespace database_system
{

class basic_database_accesser
{
public:
	typedef dtl::tstring org_string_type;
	typedef charset::utf8_string inner_string_type;
	typedef dtl::RootException org_root_err_type;
	typedef dtl::DBException org_err_type;

	typedef dtl_error error_type;
	typedef dtl_error_code_def error_code_def_type;

private:
	typedef basic_database_accesser this_type;

protected:
	typedef detail::keep_connect_helper keep_connect_helper_type;
	
public:
	// keep_connect
	template<typename Connection_T> inline
	static bool s_org_keep_connect(Connection_T& conn, error_type& err)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::org_conn_ptr_type org_conn_ptr_type;

		org_conn_ptr_type pconn = conn.pop();
		if(!pconn)
		{
			return false;
		}

		keep_connect_helper_type kch;

		bool bsuccess = kch(*pconn, err);

		bool bpushed = conn.push(pconn);
		assert(bpushed);

		return bsuccess;
	}

	// select
	template<typename Connection_T, typename Container> inline
	static bool s_org_select_records_of_view(Connection_T& conn,
												const org_string_type& tb_names,
												Container& rows,
												const org_string_type& postfix_str,
												error_type& err)
	{

		typedef Container cont_type;
		typedef typename container_mapped_value_t<cont_type>::type cont_data_val_type;
		
		return 
			this_type::s_org_select_records_of_view<cont_data_val_type>(
				conn, tb_names, rows, postfix_str, err);
	}

	template<typename DBViewData, typename Connection_T, typename Container>
	static bool s_org_select_records_of_view(Connection_T& conn,
												const org_string_type& tb_names,
												Container& rows,
												const org_string_type& postfix_str,
												error_type& err)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::org_conn_ptr_type org_conn_ptr_type;

		typedef Container cont_type;
		typedef typename container_mapped_value_t<cont_type>::value_type cont_val_type;
		typedef typename container_mapped_value_t<cont_type>::type cont_data_val_type;
		typedef DBViewData  db_view_data_type;
		typedef dtl::DBView<db_view_data_type> dbview_type;


		if(tb_names.empty())
		{
			return false;
		}

		org_conn_ptr_type pconn = conn.pop();
		if(!pconn)
		{
			return false;
		}

		keep_connect_helper_type kch;
		if(!kch(*pconn, err))
		{
			conn.push(pconn);
			return false;
		}

		try
		{
			dbview_type dbview(typename dbview_type::Args().conn(*pconn).tables(tb_names).postfix(postfix_str));
			std::copy(dbview.begin(), dbview.end(), iterator_ex::inserter(rows, boost::end(rows)));
		}
		catch(const org_err_type& e)
		{
			error_type tmp(error_code_def_type::E_DTL_DB_ERR_CODE, e.what());
			err.swap(tmp);
			conn.push(pconn);
			return false;
		}
		catch(const org_root_err_type& e)
		{
			error_type tmp(error_code_def_type::E_DTL_ROOT_ERR_CODE, e.what());
			err.swap(tmp);
			conn.push(pconn);
			return false;
		}

		bool bpushed = conn.push(pconn);
		assert(bpushed);

		return true;
	}

	template<typename Connection_T, typename Container, typename Handler_Value_Maker> inline
	static bool s_org_select_records_of_view(Connection_T& conn,
												const org_string_type& tb_names,
												Container& rows,
												const org_string_type& postfix_str,
												const Handler_Value_Maker& handler_val_maker,
												error_type& err)
	{
		
		typedef Container cont_type;
		typedef typename container_mapped_value_t<cont_type>::type cont_data_val_type;
		
		return 
			this_type::s_org_select_records_of_view<cont_data_val_type>(
				conn, tb_names, rows, postfix_str, err);

	}


	template<typename DBViewData, typename Connection_T, typename Container, typename Handler_Value_Maker>
	static bool s_org_select_records_of_view(Connection_T& conn,
												const org_string_type& tb_names,
												Container& rows,
												const org_string_type& postfix_str,
												const Handler_Value_Maker& handler_val_maker,
												error_type& err)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::org_conn_ptr_type org_conn_ptr_type;

		typedef Container cont_type;
		typedef typename container_mapped_value_t<cont_type>::value_type cont_val_type;
		typedef typename container_mapped_value_t<cont_type>::type cont_data_val_type;
		typedef DBViewData db_view_data_type;
		typedef dtl::DBView<db_view_data_type> dbview_type;
		typedef typename dbview_type::select_iterator select_iter_type;


		if(tb_names.empty())
		{
			return false;
		}

		org_conn_ptr_type pconn = conn.pop();
		if(!pconn)
		{
			return false;
		}

		keep_connect_helper_type kch;
		if(!kch(*pconn, err))
		{
			conn.push(pconn);
			return false;
		}

		try
		{
			dbview_type dbview(typename dbview_type::Args().conn(*pconn).tables(tb_names).postfix(postfix_str));
			for(select_iter_type i = dbview.begin(), isize = dbview.end(); i != isize; ++i)
			{
				iterator_ex::inserter(rows, boost::end(rows)) = handler_val_maker(*i);
			}
		}
		catch(const org_err_type& e)
		{
			error_type tmp(error_code_def_type::E_DTL_DB_ERR_CODE, e.what());
			err.swap(tmp);
			conn.push(pconn);
			return false;
		}
		catch(const org_root_err_type& e)
		{
			error_type tmp(error_code_def_type::E_DTL_ROOT_ERR_CODE, e.what());
			err.swap(tmp);
			conn.push(pconn);
			return false;
		}

		bool bpushed = conn.push(pconn);
		assert(bpushed);

		return true;
	}

	// insert
	template<typename Connection_T, typename Container> inline
	static bool s_org_insert_records_of_table(Connection_T& conn,
												const org_string_type& tb_names,
												const Container& rows,
												const org_string_type& postfix_str,
												error_type& err,
												bool bcommit,
												bool brollback)
	{
		
		typedef Container cont_type;
		typedef typename container_mapped_value_t<cont_type>::type cont_data_val_type;

		return 
			this_type::s_org_insert_records_of_table<cont_data_val_type>(
				conn, tb_names, rows, postfix_str, err, bcommit, brollback);
	}

	template<typename DBViewData, typename Connection_T, typename Container>
	static bool s_org_insert_records_of_table(Connection_T& conn,
												const org_string_type& tb_names,
												const Container& rows,
												const org_string_type& postfix_str,
												error_type& err,
												bool bcommit,
												bool brollback)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::org_conn_ptr_type org_conn_ptr_type;

		typedef Container cont_type;
		typedef typename container_mapped_value_t<cont_type>::value_type cont_val_type;
		typedef typename container_mapped_value_t<cont_type>::type cont_data_val_type;
		typedef DBViewData db_view_data_type;
		typedef dtl::DBView<db_view_data_type> dbview_type;
		typedef typename dbview_type::insert_iterator insert_iter_type;

		if(tb_names.empty())
		{
			return false;
		}

		if(boost::empty(rows))
		{
			return true;
		}

		org_conn_ptr_type pconn = conn.pop();
		if(!pconn)
		{
			return false;
		}

		keep_connect_helper_type kch;
		if(!kch(*pconn, err))
		{
			conn.push(pconn);
			return false;
		}

		try
		{
			dbview_type dbview(typename dbview_type::Args().conn(*pconn).tables(tb_names).postfix(postfix_str));
			insert_iter_type iter = dbview;
			std::copy(rows.begin(), rows.end(), iter);
		}
		catch(const org_err_type& e)
		{
			error_type tmp(error_code_def_type::E_DTL_DB_ERR_CODE, e.what());
			err.swap(tmp);

			if(brollback)
			{
				this_type::s_prv_rollback_all(*pconn);
			}
			conn.push(pconn);
			return false;
		}
		catch(const org_root_err_type& e)
		{
			error_type tmp(error_code_def_type::E_DTL_ROOT_ERR_CODE, e.what());
			err.swap(tmp);
			if(brollback)
			{
				this_type::s_prv_rollback_all(*pconn);
			}
			conn.push(pconn);
			return false;
		}

		if(bcommit)
		{
			this_type::s_prv_commit_all(*pconn);
		}

		bool bpushed = conn.push(pconn);
		assert(bpushed);

		return true;
	}

	template<typename Connection_T, typename Container, typename Handler_Value_Maker > inline
	static bool s_org_insert_records_of_table(Connection_T& conn,
												const org_string_type& tb_names,
												const Container& rows,
												const org_string_type& postfix_str,
												const Handler_Value_Maker& handler_val_maker,
												error_type& err,
												bool bcommit,
												bool brollback)
	{
		
		typedef Container cont_type;
		typedef typename container_mapped_value_t<cont_type>::type cont_data_val_type;
		
		return 
			this_type::s_org_insert_records_of_table<cont_data_val_type>(
				conn, tb_names, rows, postfix_str, handler_val_maker, err, bcommit, brollback);
	}

	template<typename DBViewData, typename Connection_T, typename Container, typename Handler_Value_Maker >
	static bool s_org_insert_records_of_table(Connection_T& conn,
												const org_string_type& tb_names,
												const Container& rows,
												const org_string_type& postfix_str,
												const Handler_Value_Maker& handler_val_maker,
												error_type& err,
												bool bcommit,
												bool brollback)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::org_conn_ptr_type org_conn_ptr_type;

		typedef Container cont_type;
		typedef typename boost::range_const_iterator<cont_type>::type cont_iter_type;
		typedef typename container_mapped_value_t<cont_type>::value_type cont_val_type;
		typedef typename container_mapped_value_t<cont_type>::type cont_data_val_type;
		typedef DBViewData db_view_data_type;
		typedef dtl::DBView<db_view_data_type> dbview_type;
		typedef typename dbview_type::insert_iterator insert_iter_type;

		typedef yggr::vector<cont_data_val_type> data_val_vt_type;

		if(tb_names.empty())
		{
			return false;
		}

		if(boost::empty(rows))
		{
			return true;
		}

		org_conn_ptr_type pconn = conn.pop();
		if(!pconn)
		{
			return false;
		}

		keep_connect_helper_type kch;
		if(!kch(*pconn, err))
		{
			conn.push(pconn);
			return false;
		}

		data_val_vt_type vt_rows;
		for(cont_iter_type i = boost::begin(rows), isize = boost::end(rows); i != isize; ++i)
		{
			vt_rows.push_back(handler_val_maker(*i));
		}

		try
		{
			dbview_type dbview(typename dbview_type::Args().conn(*pconn).tables(tb_names).postfix(postfix_str));
			insert_iter_type iter = dbview;
			std::copy(vt_rows.begin(), vt_rows.end(), iter);
		}
		catch(const org_err_type& e)
		{
			error_type tmp(error_code_def_type::E_DTL_DB_ERR_CODE, e.what());
			err.swap(tmp);
			if(brollback)
			{
				this_type::s_prv_rollback_all(*pconn);
			}
			conn.push(pconn);
			return false;
		}
		catch(const org_root_err_type& e)
		{
			error_type tmp(error_code_def_type::E_DTL_ROOT_ERR_CODE, e.what());
			err.swap(tmp);
			if(brollback)
			{
				this_type::s_prv_rollback_all(*pconn);
			}
			conn.push(pconn);
			return false;
		}

		if(bcommit)
		{
			this_type::s_prv_commit_all(*pconn);
		}

		bool bpushed = conn.push(pconn);
		assert(bpushed);

		return true;
	}

	// update
	template<typename Connection_T, typename Container> inline
	static bool s_org_update_records_of_table(Connection_T& conn,
												const org_string_type& tb_names,
												const Container& rows,
												const org_string_type& postfix_str,
												error_type& err,
												bool bcommit,
												bool brollback)
	{
		typedef Container cont_type;
		typedef typename container_mapped_value_t<cont_type>::type cont_data_val_type;

		return 
			this_type::s_org_update_records_of_table<cont_data_val_type>(
				conn, tb_names, rows, postfix_str, err, bcommit, brollback);
	}

	template<typename DBViewData, typename Connection_T, typename Container>
	static bool s_org_update_records_of_table(Connection_T& conn,
												const org_string_type& tb_names,
												const Container& rows,
												const org_string_type& postfix_str,
												error_type& err,
												bool bcommit,
												bool brollback)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::org_conn_ptr_type org_conn_ptr_type;

		typedef Container cont_type;
		typedef typename boost::range_const_iterator<cont_type>::type cont_iter_type;
		typedef typename container_mapped_value_t<cont_type>::value_type cont_val_type;
		typedef typename container_mapped_value_t<cont_type>::type cont_data_val_type;
		typedef DBViewData db_view_data_type;
		typedef dtl::DBView<db_view_data_type, db_view_data_type> dbview_type;
		typedef typename dbview_type::update_iterator update_iter_type;

		if(tb_names.empty())
		{
			return false;
		}

		if(boost::empty(rows))
		{
			return true;
		}

		org_conn_ptr_type pconn = conn.pop();
		if(!pconn)
		{
			return false;
		}

		keep_connect_helper_type kch;
		if(!kch(*pconn, err))
		{
			conn.push(pconn);
			return false;
		}

		try
		{
			dbview_type dbview(typename dbview_type::Args().conn(*pconn).tables(tb_names).postfix(postfix_str));
			update_iter_type iter = dbview;

			for(cont_iter_type i = boost::begin(rows), isize = boost::end(rows); i != isize; ++i)
			{
				iter.Params() = (*i);
				*iter = (*i);
			}
		}
		catch(const org_err_type& e)
		{
			error_type tmp(error_code_def_type::E_DTL_DB_ERR_CODE, e.what());
			err.swap(tmp);
			if(brollback)
			{
				this_type::s_prv_rollback_all(*pconn);
			}
			conn.push(pconn);
			return false;
		}
		catch(const org_root_err_type& e)
		{
			error_type tmp(error_code_def_type::E_DTL_ROOT_ERR_CODE, e.what());
			err.swap(tmp);
			if(brollback)
			{
				this_type::s_prv_rollback_all(*pconn);
			}
			conn.push(pconn);
			return false;
		}

		if(bcommit)
		{
			this_type::s_prv_commit_all(*pconn);
		}

		bool bpushed = conn.push(pconn);
		assert(bpushed);

		return true;
	}

	template<typename Connection_T, typename Container, typename Handler_Value_Maker> inline
	static bool s_org_update_records_of_table(Connection_T& conn,
												const org_string_type& tb_names,
												const Container& rows,
												const org_string_type& postfix_str,
												const Handler_Value_Maker& handler_val_maker,
												error_type& err,
												bool bcommit,
												bool brollback)
	{
		typedef Container cont_type;
		typedef typename container_mapped_value_t<cont_type>::type cont_data_val_type;
		
		return 
			this_type::s_org_update_records_of_table<cont_data_val_type>(
				conn, tb_names, rows, postfix_str, handler_val_maker, err, bcommit, brollback);
	}

	template<typename DBViewData, typename Connection_T, typename Container, typename Handler_Value_Maker>
	static bool s_org_update_records_of_table(Connection_T& conn,
												const org_string_type& tb_names,
												const Container& rows,
												const org_string_type& postfix_str,
												const Handler_Value_Maker& handler_val_maker,
												error_type& err,
												bool bcommit,
												bool brollback)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::org_conn_ptr_type org_conn_ptr_type;

		typedef Container cont_type;
		typedef typename boost::range_const_iterator<cont_type>::type cont_iter_type;
		typedef typename container_mapped_value_t<cont_type>::value_type cont_val_type;
		typedef typename container_mapped_value_t<cont_type>::type cont_data_val_type;
		typedef DBViewData db_view_data_type;
		typedef dtl::DBView<db_view_data_type> dbview_type;
		typedef typename dbview_type::update_iterator update_iter_type;

		if(tb_names.empty())
		{
			return false;
		}

		if(boost::empty(rows))
		{
			return true;
		}

		org_conn_ptr_type pconn = conn.pop();
		if(!pconn)
		{
			return false;
		}

		keep_connect_helper_type kch;
		if(!kch(*pconn, err))
		{
			conn.push(pconn);
			return false;
		}

		try
		{
			dbview_type dbview(typename dbview_type::Args().conn(*pconn).tables(tb_names).postfix(postfix_str));
			update_iter_type iter = dbview;

			for(cont_iter_type i = boost::begin(rows), isize = boost::end(rows); i != isize; ++i)
			{
				const cont_data_val_type& data_val_cref = handler_val_maker(*i)
				//iter.Params() = data_val_cref;
				(&iter)->Params() = data_val_cref; // msvc cpp20 need this style
				*iter = data_val_cref;
			}
		}
		catch(const org_err_type& e)
		{
			error_type tmp(error_code_def_type::E_DTL_DB_ERR_CODE, e.what());
			err.swap(tmp);
			if(brollback)
			{
				this_type::s_prv_rollback_all(*pconn);
			}
			conn.push(pconn);
			return false;
		}
		catch(const org_root_err_type& e)
		{
			error_type tmp(error_code_def_type::E_DTL_ROOT_ERR_CODE, e.what());
			err.swap(tmp);
			if(brollback)
			{
				this_type::s_prv_rollback_all(*pconn);
			}
			conn.push(pconn);
			return false;
		}

		if(bcommit)
		{
			this_type::s_prv_commit_all(*pconn);
		}

		bool bpushed = conn.push(pconn);
		assert(bpushed);

		return true;
	}

	// delete
	template<typename Connection_T, typename Container> inline
	static bool s_org_delete_records_of_table(Connection_T& conn,
												const org_string_type& tb_names,
												const Container& rows,
												const org_string_type& postfix_str,
												error_type& err,
												bool bcommit,
												bool brollback)
	{

		typedef Container cont_type;
		typedef typename container_mapped_value_t<cont_type>::type cont_data_val_type;

		return
			this_type::s_org_delete_records_of_table<cont_data_val_type>(
				conn, tb_names, rows, postfix_str, err, bcommit, brollback);
	}

	template<typename DBViewData, typename Connection_T, typename Container>
	static bool s_org_delete_records_of_table(Connection_T& conn,
												const org_string_type& tb_names,
												const Container& rows,
												const org_string_type& postfix_str,
												error_type& err,
												bool bcommit,
												bool brollback)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::org_conn_ptr_type org_conn_ptr_type;

		typedef Container cont_type;
		typedef typename boost::range_const_iterator<cont_type>::type cont_iter_type;
		typedef typename container_mapped_value_t<cont_type>::value_type cont_val_type;
		typedef typename container_mapped_value_t<cont_type>::type cont_data_val_type;
		typedef DBViewData db_view_data_type;
		typedef dtl::DBView<db_view_data_type> dbview_type;
		typedef typename dbview_type::delete_iterator delete_iter_type;

		if(tb_names.empty())
		{
			return false;
		}

		if(boost::empty(rows))
		{
			return true;
		}

		org_conn_ptr_type pconn = conn.pop();
		if(!pconn)
		{
			return false;
		}

		keep_connect_helper_type kch;
		if(!kch(*pconn, err))
		{
			conn.push(pconn);
			return false;
		}

		try
		{
			dbview_type dbview(typename dbview_type::Args().conn(*pconn).tables(tb_names).postfix(postfix_str));
			delete_iter_type del_iter = dbview;
			for(cont_iter_type i = boost::begin(rows), isize = boost::end(rows); i != isize; ++i)
			{
				*del_iter = *i;
			}
		}
		catch(const org_err_type& e)
		{
			error_type tmp(error_code_def_type::E_DTL_DB_ERR_CODE, e.what());
			err.swap(tmp);
			if(brollback)
			{
				this_type::s_prv_rollback_all(*pconn);
			}
			conn.push(pconn);
			return false;
		}
		catch(const org_root_err_type& e)
		{
			error_type tmp(error_code_def_type::E_DTL_ROOT_ERR_CODE, e.what());
			err.swap(tmp);
			if(brollback)
			{
				this_type::s_prv_rollback_all(*pconn);
			}
			conn.push(pconn);
			return false;
		}

		if(bcommit)
		{
			this_type::s_prv_commit_all(*pconn);
		}

		bool bpushed = conn.push(pconn);
		assert(bpushed);

		return true;
	}

	template<typename Connection_T, typename Container, typename Handler_Value_Maker> inline
	static bool s_org_delete_records_of_table(Connection_T& conn,
												const org_string_type& tb_names,
												const Container& rows,
												const org_string_type& postfix_str,
												const Handler_Value_Maker& handler_val_maker,
												error_type& err,
												bool bcommit,
												bool brollback)
	{
		typedef Container cont_type;
		typedef typename container_mapped_value_t<cont_type>::type cont_data_val_type;
		
		return 
			this_type::s_org_delete_records_of_table<cont_data_val_type>(
				conn, tb_names, rows, postfix_str, handler_val_maker, err, bcommit, brollback);
	}

	template<typename DBViewData, typename Connection_T, typename Container, typename Handler_Value_Maker>
	static bool s_org_delete_records_of_table(Connection_T& conn,
												const org_string_type& tb_names,
												const Container& rows,
												const org_string_type& postfix_str,
												const Handler_Value_Maker& handler_val_maker,
												error_type& err,
												bool bcommit,
												bool brollback)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::org_conn_ptr_type org_conn_ptr_type;

		typedef Container cont_type;
		typedef typename boost::range_const_iterator<cont_type>::type cont_iter_type;
		typedef typename container_mapped_value_t<cont_type>::value_type cont_val_type;
		typedef typename container_mapped_value_t<cont_type>::type cont_data_val_type;
		typedef DBViewData db_view_data_type;
		typedef dtl::DBView<db_view_data_type> dbview_type;
		typedef typename dbview_type::delete_iterator delete_iter_type;

		if(tb_names.empty())
		{
			return false;
		}

		if(boost::empty(rows))
		{
			return true;
		}

		org_conn_ptr_type pconn = conn.pop();
		if(!pconn)
		{
			return false;
		}

		keep_connect_helper_type kch;
		if(!kch(*pconn, err))
		{
			conn.push(pconn);
			return false;
		}

		try
		{
			dbview_type dbview(typename dbview_type::Args().conn(conn).tables(tb_names).postfix(postfix_str));
			delete_iter_type del_iter = dbview;
			for(cont_iter_type i = boost::begin(rows), isize = boost::end(rows); i != isize; ++i)
			{
				*del_iter = handler_val_maker(*i);
			}
		}
		catch(const org_err_type& e)
		{
			error_type tmp(error_code_def_type::E_DTL_DB_ERR_CODE, e.what());
			err.swap(tmp);
			if(brollback)
			{
				this_type::s_prv_rollback_all(*pconn);
			}
			conn.push(pconn);
			return false;
		}
		catch(const org_root_err_type& e)
		{
			error_type tmp(error_code_def_type::E_DTL_ROOT_ERR_CODE, e.what());
			err.swap(tmp);
			if(brollback)
			{
				this_type::s_prv_rollback_all(*pconn);
			}
			conn.push(pconn);
			return false;
		}

		if(bcommit)
		{
			this_type::s_prv_commit_all(*pconn);
		}

		bool bpushed = conn.push(pconn);
		assert(bpushed);

		return true;
	}

	// execute_sql
	template<typename Connection_T>
	static bool s_org_execute_sql(Connection_T& conn, 
									const org_string_type& str_sql, 
									error_type& err,
									bool bcommit,
									bool brollback)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::org_conn_ptr_type org_conn_ptr_type;

		if(str_sql.empty())
		{
			return false;
		}

		org_conn_ptr_type pconn = conn.pop();
		if(!pconn)
		{
			return false;
		}

		keep_connect_helper_type kch;
		if(!kch(*pconn, err))
		{
			conn.push(pconn);
			return false;
		}

		bool bexecuted = false;
		try
		{
			dtl::DBStmt stmt(str_sql, *pconn);
			stmt.Execute();
			bexecuted = stmt.IsExecuted();
		}
		catch(const org_err_type& e)
		{
			error_type tmp(error_code_def_type::E_DTL_DB_ERR_CODE, e.what());
			err.swap(tmp);
			if(brollback)
			{
				this_type::s_prv_rollback_all(*pconn);
			}
			conn.push(pconn);
			return false;
		}
		catch(const org_root_err_type& e)
		{
			error_type tmp(error_code_def_type::E_DTL_ROOT_ERR_CODE, e.what());
			err.swap(tmp);
			if(brollback)
			{
				this_type::s_prv_rollback_all(*pconn);
			}
			conn.push(pconn);
			return false;
		}

		if(bcommit)
		{
			this_type::s_prv_commit_all(*pconn);
		}

		bool bpushed = conn.push(pconn);
		assert(bpushed);

		return bexecuted;
	}

	// connection_using_handler
public:
	template<typename Connection_T, typename Handler> inline
	static typename boost::remove_reference<typename func::foo_t_info<Handler>::result_type>::type
		s_org_connection_using_handler(Connection_T& conn, const Handler& handler, error_type& err)
	{
		typedef Handler handler_type;
		typedef typename func::foo_t_info<handler_type>::result_type cv_ret_type;
		typedef typename boost::remove_reference<cv_ret_type>::type ret_type;

		typedef detail::connection_using_handler_helper<ret_type> h_type;

		h_type h;
		return h(conn, handler, err);
	}

	inline static void s_thread_clear(void)
	{
		// don't clear thread, so its empty;
	}

private:
	//commit
	template<typename Conn> inline
	static void s_prv_commit_all(Conn& conn)
	{
		if(conn.GetAutoCommit())
		{
			return;
		}

		conn.CommitAll();
	}

	template<typename Conn> inline
	static void s_prv_rollback_all(Conn& conn)
	{
		conn.RollbackAll();
	}
};

} // namespace database_system 
} // namespace yggr

#endif //__YGGR_DATABASE_SYSTEM_BASIC_DATABASE_ACCESSER_HPP__
