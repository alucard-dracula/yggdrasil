//database_operator_zwei.hpp

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

#ifndef __YGGR_DATABASE_SYSTEM_DATABASE_OPERATOR_ZWEI_HPP__
#define __YGGR_DATABASE_SYSTEM_DATABASE_OPERATOR_ZWEI_HPP__

#ifndef YGGR_DATABASE_OPERATOR_INCLUDE
#	error "include error database_operator_zwei.hpp please include database_operator.hpp"
#endif // YGGR_DATABASE_OPERATOR_INCLUDE

namespace yggr
{
namespace database_system
{
template<>
class database_operator<dtl::DBConnection>
{
public:
	typedef dtl::DBConnection conn_type;
	typedef database_operator<conn_type> this_type;

public:
	template<typename Tuple>
	static bool select_records_of_view(const std::string& tb_names,
										std::vector<Tuple>& rows,
										const std::string& postfix_str = std::string(""),
										conn_type& conn = conn_type::GetDefaultConnection())
	{
		typedef Tuple tuple_type;
		typedef std::vector<tuple_type> rows_type;
		typedef dtl::DBView<tuple_type> dbview_type;
		typedef typename dbview_type::select_iterator select_iter_type;


		if(tb_names.empty())
		{
			return false;
		}

		rows.clear();

		try
		{
			dbview_type dbview(typename dbview_type::Args().conn(conn).tables(tb_names).postfix(postfix_str));
			select_iter_type isize = dbview.end();

			for(select_iter_type i = dbview.begin(); i != isize; ++i)
			{
				rows.push_back(*i);
			}
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

		return true;
	}


	template<typename Tuple>
	static bool insert_records_of_table(const std::string& tb_name,
											const std::vector<Tuple>& rows,
											const std::string& postfix_str = std::string(""),
											conn_type& conn = conn_type::GetDefaultConnection())
	{
		typedef Tuple tuple_type;
		typedef std::vector<tuple_type> rows_type;
		typedef dtl::DBView<tuple_type> dbview_type;
		typedef typename dbview_type::insert_iterator insert_iter_type;

		if(tb_name.empty() || rows.empty())
		{
			return false;
		}

		try
		{
			dbview_type dbview(typename dbview_type::Args().conn(conn).tables(tb_name).postfix(postfix_str));
			insert_iter_type iter = dbview;
			std::copy(rows.begin(), rows.end(), iter);
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
		return true;
	}


	template<typename Tuple>
	static bool update_records_of_table(const std::string& tb_name,
											const std::vector<Tuple>& rows,
											const std::string& postfix_str = std::string(""),
											conn_type& conn = conn_type::GetDefaultConnection())
	{
		typedef Tuple tuple_type;
		typedef std::vector<tuple_type> rows_type;
		typedef dtl::DBView<tuple_type, tuple_type> dbview_type;
		typedef typename dbview_type::update_iterator update_iter_type;

		if(tb_name.empty() || rows.empty())
		{
			return false;
		}

		try
		{
			dbview_type dbview(typename dbview_type::Args().conn(conn).tables(tb_name).postfix(postfix_str));
			update_iter_type iter = dbview;
			typename rows_type::const_iterator isize = rows.end();

			for(typename rows_type::const_iterator i = rows.begin(); i != isize; ++i)
			{
				iter.Params() = (*i);
				*iter = (*i);
			}
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
		return true;
	}

	template<typename Tuple>
	static bool delete_records_of_table(const std::string& tb_name,
											const std::vector<Tuple>& rows,
											const std::string& postfix_str = std::string(""),
											conn_type& conn = conn_type::GetDefaultConnection())
	{
		typedef Tuple tuple_type;
		typedef std::vector<tuple_type> rows_type;
		typedef dtl::DBView<tuple_type> dbview_type;
		typedef typename dbview_type::delete_iterator delete_iter_type;

		try
		{
			dbview_type dbview(typename dbview_type::Args().conn(conn).tables(tb_name).postfix(postfix_str));
			delete_iter_type del_iter = dbview;


			typename rows_type::const_iterator isize = rows.end();
			for(typename rows_type::const_iterator i = rows.begin(); i != isize; ++i)
			{
				*del_iter = *i;
			}
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

		return true;
	}

	static bool execute_sql(const std::string& str_sql, conn_type& conn = conn_type::GetDefaultConnection())
	{
		try
		{
			dtl::DBStmt stmt(str_sql, conn);
			stmt.Execute();
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

		return true;
	}

};

} // namespace database_system 
} // namespace yggr

#endif //__YGGR_DATABASE_SYSTEM_DATABASE_OPERATOR_ZWEI_HPP__
