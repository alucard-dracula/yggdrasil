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

#ifndef __YGGR_DATABASE_SYSTEM_DATABASE_OPERATOR_ZWEI_HPP__
#define __YGGR_DATABASE_SYSTEM_DATABASE_OPERATOR_ZWEI_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/database_system/basic_database_accesser.hpp>
#include <yggr/database_system/database_string_helper.hpp>

#include <boost/utility/enable_if.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>

namespace yggr
{
namespace database_system
{

class database_accesser
	: public basic_database_accesser
{
public:
	typedef basic_database_accesser base_type;

	typedef base_type::org_string_type org_string_type;
	typedef base_type::inner_string_type inner_string_type;
	typedef base_type::org_root_err_type org_root_err_type;
	typedef base_type::org_err_type org_err_type;

	typedef base_type::error_type error_type;

	typedef base_type::error_code_def_type error_code_def_type;

private:
	typedef database_accesser this_type;

public:
	// keep_connect
	template<typename Connection_T> inline
	static bool s_keep_connect(Connection_T& conn, error_type& err)
	{
		return base_type::s_org_keep_connect(conn, err);
	}

	// select
	template<typename Connection_T, typename Container,
				typename String1, typename String2> inline
	static 
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_select_records_of_view(Connection_T& conn,
									const String1& tb_names,
									Container& rows,
									const String2& postfix_str,
									error_type& err)
	{
		return 
			base_type::s_org_select_records_of_view(
				conn, 
				database_string_helper::s_conv(
					charset::string_charset_helper_first(tb_names), 
					charset::string_charset_helper_second(tb_names)), 
				rows, 
				database_string_helper::s_conv(
					charset::string_charset_helper_first(postfix_str), 
					charset::string_charset_helper_second(postfix_str)), 
				err);
	}

	template<typename DBViewData, 
				typename Connection_T, typename Container,
				typename String1, typename String2> inline
	static 
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_select_records_of_view(Connection_T& conn,
									const String1& tb_names,
									Container& rows,
									const String2& postfix_str,
									error_type& err)
	{
		typedef DBViewData db_view_data_type;

		return
			base_type::s_org_select_records_of_view<db_view_data_type>(
				conn, 
				database_string_helper::s_conv(
					charset::string_charset_helper_first(tb_names), 
					charset::string_charset_helper_second(tb_names)), 
				rows, 
				database_string_helper::s_conv(
					charset::string_charset_helper_first(postfix_str), 
					charset::string_charset_helper_second(postfix_str)), 
				err);
	}

	template<typename Connection_T, typename Container, typename Handler_Value_Maker,
				typename String1, typename String2> inline
	static 
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_select_records_of_view(Connection_T& conn,
									const String1& tb_names,
									Container& rows,
									const String2& postfix_str,
									const Handler_Value_Maker& handler_val_maker,
									error_type& err)
	{
		return 
			base_type::s_org_select_records_of_view(
				conn, 
				database_string_helper::s_conv(
					charset::string_charset_helper_first(tb_names), 
					charset::string_charset_helper_second(tb_names)), 
				rows, 
				database_string_helper::s_conv(
					charset::string_charset_helper_first(postfix_str), 
					charset::string_charset_helper_second(postfix_str)), 
				handler_val_maker, 
				err);
	}

	template<typename DBViewData, 
				typename Connection_T, typename Container, typename Handler_Value_Maker,
				typename String1, typename String2> inline
	static 
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_select_records_of_view(Connection_T& conn,
									const String1& tb_names,
									Container& rows,
									const String2& postfix_str,
									const Handler_Value_Maker& handler_val_maker,
									error_type& err)
	{
		typedef DBViewData db_view_data_type;
		return 
			base_type::s_org_select_records_of_view<db_view_data_type>(
				conn, 
				database_string_helper::s_conv(
					charset::string_charset_helper_first(tb_names), 
					charset::string_charset_helper_second(tb_names)), 
				rows, 
				database_string_helper::s_conv(
					charset::string_charset_helper_first(postfix_str), 
					charset::string_charset_helper_second(postfix_str)), 
				handler_val_maker, 
				err);
	}

	// insert
	template<typename Connection_T, typename Container, 
				typename String1, typename String2> inline
	static 
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_insert_records_of_table(Connection_T& conn,
									const String1& tb_names,
									const Container& rows,
									const String2& postfix_str,
									error_type& err,
									bool bcommit = false,
									bool brollback = true)
	{
		return 
			base_type::s_org_insert_records_of_table(
				conn, 
				database_string_helper::s_conv(
					charset::string_charset_helper_first(tb_names), 
					charset::string_charset_helper_second(tb_names)), 
				rows, 
				database_string_helper::s_conv(
					charset::string_charset_helper_first(postfix_str), 
					charset::string_charset_helper_second(postfix_str)), 
				err, 
				bcommit, 
				brollback);
	}

	template<typename DBViewData, 
				typename Connection_T, typename Container, 
				typename String1, typename String2> inline
	static 
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_insert_records_of_table(Connection_T& conn,
									const String1& tb_names,
									const Container& rows,
									const String2& postfix_str,
									error_type& err,
									bool bcommit = false,
									bool brollback = true)
	{
		typedef DBViewData db_view_data_type;
		return
			base_type::s_org_insert_records_of_table<db_view_data_type>(
				conn, 
				database_string_helper::s_conv(
					charset::string_charset_helper_first(tb_names), 
					charset::string_charset_helper_second(tb_names)), 
				rows, 
				database_string_helper::s_conv(
					charset::string_charset_helper_first(postfix_str), 
					charset::string_charset_helper_second(postfix_str)), 
				err, 
				bcommit, 
				brollback);
	}

	template<typename Connection_T, typename Container, 
				typename Handler_Value_Maker,
				typename String1, typename String2> inline
	static 
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_insert_records_of_table(Connection_T& conn,
									const String1& tb_names,
									const Container& rows,
									const String2& postfix_str,
									const Handler_Value_Maker& handler_val_maker,
									error_type& err,
									bool bcommit = false,
									bool brollback = true)
	{
		return
			base_type::s_org_insert_records_of_table(
				conn, 
				database_string_helper::s_conv(
					charset::string_charset_helper_first(tb_names), 
					charset::string_charset_helper_second(tb_names)), 
				rows, 
				database_string_helper::s_conv(
					charset::string_charset_helper_first(postfix_str), 
					charset::string_charset_helper_second(postfix_str)), 
				handler_val_maker, 
				err, 
				bcommit, 
				brollback);
	}

	template<typename DBViewData, 
				typename Connection_T, typename Container, 
				typename Handler_Value_Maker,
				typename String1, typename String2> inline
	static 
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_insert_records_of_table(Connection_T& conn,
									const String1& tb_names,
									const Container& rows,
									const String2& postfix_str,
									const Handler_Value_Maker& handler_val_maker,
									error_type& err,
									bool bcommit = false,
									bool brollback = true)
	{
		typedef DBViewData db_view_data_type;
		return 
			base_type::s_org_insert_records_of_table<db_view_data_type>(
				conn, 
				database_string_helper::s_conv(
					charset::string_charset_helper_first(tb_names), 
					charset::string_charset_helper_second(tb_names)), 
				rows, 
				database_string_helper::s_conv(
					charset::string_charset_helper_first(postfix_str), 
					charset::string_charset_helper_second(postfix_str)), 
				handler_val_maker, 
				err, 
				bcommit, 
				brollback);
	}

	// update
	template<typename Connection_T, typename Container, 
				typename String1, typename String2 > inline
	static 
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_update_records_of_table(Connection_T& conn,
									const String1& tb_names,
									const Container& rows,
									const String2& postfix_str,
									error_type& err,
									bool bcommit = false,
									bool brollback = true)
	{
		return 
			base_type::s_org_update_records_of_table(
				conn, 
				database_string_helper::s_conv(
					charset::string_charset_helper_first(tb_names), 
					charset::string_charset_helper_second(tb_names)), 
				rows, 
				database_string_helper::s_conv(
					charset::string_charset_helper_first(postfix_str), 
					charset::string_charset_helper_second(postfix_str)), 
				err, 
				bcommit, 
				brollback);
	}

	template<typename DBViewData,
				typename Connection_T, typename Container, 
				typename String1, typename String2 > inline
	static 
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_update_records_of_table(Connection_T& conn,
									const String1& tb_names,
									const Container& rows,
									const String2& postfix_str,
									error_type& err,
									bool bcommit = false,
									bool brollback = true)
	{
		typedef DBViewData db_view_data_type;
		return 
			base_type::s_org_update_records_of_table<db_view_data_type>(
				conn, 
				database_string_helper::s_conv(
					charset::string_charset_helper_first(tb_names), 
					charset::string_charset_helper_second(tb_names)), 
				rows, 
				database_string_helper::s_conv(
					charset::string_charset_helper_first(postfix_str), 
					charset::string_charset_helper_second(postfix_str)), 
				err, 
				bcommit, 
				brollback);
	}

	template<typename Connection_T, typename Container, 
				typename Handler_Value_Maker,
				typename String1, typename String2 > inline
	static 
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_update_records_of_table(Connection_T& conn,
									const String1& tb_names,
									const Container& rows,
									const String2& postfix_str,
									const Handler_Value_Maker& handler_val_maker,
									error_type& err,
									bool bcommit = false,
									bool brollback = true)
	{
		return 
			base_type::s_org_update_records_of_table(
				conn, 
				database_string_helper::s_conv(
					charset::string_charset_helper_first(tb_names), 
					charset::string_charset_helper_second(tb_names)), 
				rows, 
				database_string_helper::s_conv(
					charset::string_charset_helper_first(postfix_str), 
					charset::string_charset_helper_second(postfix_str)), 
				handler_val_maker, 
				err, 
				bcommit, 
				brollback);
	}

	template<typename DBViewData,
				typename Connection_T, typename Container, 
				typename Handler_Value_Maker,
				typename String1, typename String2 > inline
	static 
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_update_records_of_table(Connection_T& conn,
									const String1& tb_names,
									const Container& rows,
									const String2& postfix_str,
									const Handler_Value_Maker& handler_val_maker,
									error_type& err,
									bool bcommit = false,
									bool brollback = true)
	{
		typedef DBViewData db_view_data_type;
		return 
			base_type::s_org_update_records_of_table<db_view_data_type>(
				conn, 
				database_string_helper::s_conv(
					charset::string_charset_helper_first(tb_names), 
					charset::string_charset_helper_second(tb_names)), 
				rows, 
				database_string_helper::s_conv(
					charset::string_charset_helper_first(postfix_str), 
					charset::string_charset_helper_second(postfix_str)), 
				handler_val_maker, 
				err, 
				bcommit, 
				brollback);
	}

	// delete
	template<typename Connection_T, typename Container, 
				typename String1, typename String2 > inline
	static 
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_delete_records_of_table(Connection_T& conn,
									const String1& tb_names,
									const Container& rows,
									const String2& postfix_str,
									error_type& err,
									bool bcommit = false,
									bool brollback = true)
	{
		return 
			base_type::s_org_delete_records_of_table(
				conn, 
				database_string_helper::s_conv(
					charset::string_charset_helper_first(tb_names), 
					charset::string_charset_helper_second(tb_names)), 
				rows, 
				database_string_helper::s_conv(
					charset::string_charset_helper_first(postfix_str), 
					charset::string_charset_helper_second(postfix_str)), 
				err, 
				bcommit, 
				brollback);
	}

	template<typename DBViewData,
				typename Connection_T, typename Container, 
				typename String1, typename String2 > inline
	static 
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_delete_records_of_table(Connection_T& conn,
									const String1& tb_names,
									const Container& rows,
									const String2& postfix_str,
									error_type& err,
									bool bcommit = false,
									bool brollback = true)
	{
		typedef DBViewData db_view_data_type;
		return 
			base_type::s_org_delete_records_of_table<db_view_data_type>(
				conn, 
				database_string_helper::s_conv(
					charset::string_charset_helper_first(tb_names), 
					charset::string_charset_helper_second(tb_names)), 
				rows, 
				database_string_helper::s_conv(
					charset::string_charset_helper_first(postfix_str), 
					charset::string_charset_helper_second(postfix_str)), 
				err, 
				bcommit, 
				brollback);
	}

	template<typename Connection_T, typename Container, 
				typename Handler_Value_Maker,
				typename String1, typename String2> inline
	static 
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_delete_records_of_table(Connection_T& conn,
									const String1& tb_names,
									const Container& rows,
									const String2& postfix_str,
									const Handler_Value_Maker& handler_val_maker,
									error_type& err,
									bool bcommit = false,
									bool brollback = true)
	{
		//org_string_type org_tb_names = database_string_helper::s_conv(tb_names, charset1);
		//org_string_type org_postfix_str = database_string_helper::s_conv(postfix_str, charset2);

		return
			base_type::s_org_delete_records_of_table(
				conn, 
				database_string_helper::s_conv(
					charset::string_charset_helper_first(tb_names), 
					charset::string_charset_helper_second(tb_names)), 
				rows, 
				database_string_helper::s_conv(
					charset::string_charset_helper_first(postfix_str), 
					charset::string_charset_helper_second(postfix_str)), 
				handler_val_maker, 
				err, 
				bcommit, 
				brollback);
	}

	template<typename DBViewData,
				typename Connection_T, typename Container, 
				typename Handler_Value_Maker,
				typename String1, typename String2> inline
	static 
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_delete_records_of_table(Connection_T& conn,
									const String1& tb_names,
									const Container& rows,
									const String2& postfix_str,
									const Handler_Value_Maker& handler_val_maker,
									error_type& err,
									bool bcommit = false,
									bool brollback = true)
	{
		typedef DBViewData db_view_data_type;
		//org_string_type org_tb_names = database_string_helper::s_conv(tb_names, charset1);
		//org_string_type org_postfix_str = database_string_helper::s_conv(postfix_str, charset2);

		return 
			base_type::s_org_delete_records_of_table<db_view_data_type>(
				conn, 
				database_string_helper::s_conv(
					charset::string_charset_helper_first(tb_names), 
					charset::string_charset_helper_second(tb_names)), 
				rows, 
				database_string_helper::s_conv(
					charset::string_charset_helper_first(postfix_str), 
					charset::string_charset_helper_second(postfix_str)), 
				handler_val_maker, 
				err, 
				bcommit, 
				brollback);
	}

	// execute_sql
	template<typename Connection_T, typename String1> inline
	static typename boost::enable_if<charset::utf8_string_constructable<String1>, bool>::type
		s_execute_sql(Connection_T& conn,
						const String1& str_sql,
						error_type& err,
						bool bcommit = false,
						bool brollback = true)
	{
		return 
			base_type::s_org_execute_sql(
				conn, 
				database_string_helper::s_conv(
					charset::string_charset_helper_first(str_sql), 
					charset::string_charset_helper_second(str_sql)), 
				err, 
				bcommit, 
				brollback);
	}

	template<typename Connection_T> inline
	static bool s_execute_sql_non_conv(Connection_T& conn,
										const org_string_type& str_sql,
										error_type& err,
										bool bcommit = false,
										bool brollback = true)
	{
		return base_type::s_org_execute_sql(conn, str_sql, err, bcommit, brollback);
	}

	// connection_using_handler
	template<typename Connection_T, typename Handler> inline
	static typename boost::remove_reference<typename func::foo_t_info<Handler>::result_type>::type
		s_connection_using_handler(Connection_T& conn, const Handler& handler, error_type& err)
	{
		return base_type::s_org_connection_using_handler(conn, handler, err);
	}

};

} // namespace database_system 
} // namespace yggr

#endif //__YGGR_DATABASE_SYSTEM_DATABASE_OPERATOR_ZWEI_HPP__
