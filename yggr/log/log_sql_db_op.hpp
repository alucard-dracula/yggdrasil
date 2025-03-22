//log_sql_db_op.hpp

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

#ifndef __YGGR_LOG_LOGOP_LOG_SQL_DB_OP_HPP__
#define __YGGR_LOG_LOGOP_LOG_SQL_DB_OP_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>

#include <yggr/tuple/tuple.hpp>
#include <yggr/charset/utf8_string.hpp>

#include <yggr/container_ex/is_container.hpp>
#include <yggr/type_traits/native_t.hpp>

#include <yggr/ppex/foo_params.hpp>
#include <yggr/ppex/symbol_if.hpp>
#include <yggr/ppex/symbols.hpp>

#include <yggr/smart_ptr_ex/shared_ptr.hpp>

#include <yggr/container/array.hpp>
#include <yggr/container/list.hpp>

#include <yggr/nonable/noncopyable.hpp>
#include <yggr/nonable/nonmoveable.hpp>
#include <yggr/database_system/database_accesser.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/ref.hpp>

namespace yggr
{
namespace log
{
namespace log_op
{
namespace detail
{

template<bool is_container>
struct log_sql_db_op_container_make_helper;

template< >
struct log_sql_db_op_container_make_helper<true >
{
	template<typename T> inline 
	const T& operator()(const T& t) const
	{
		return t;
	}
};

template< >
struct log_sql_db_op_container_make_helper<false >
{
	template<typename T> inline
	yggr::array<T, 1> operator()(const T& t) const
	{
		typedef yggr::array<T, 1> ret_type;
		ret_type ret = {t};
		return ret;
	}
};

} // namespace detail
} // namespace log_op
} // namespace log
} // namespace yggr

namespace yggr
{
namespace log
{
namespace log_op
{

template<typename Def, typename Connect>
class log_sql_db_op
	: private nonable::noncopyable,
		private nonable::nonmoveable
{
public:
	typedef Def def_type;
	YGGR_STATIC_CONSTANT(u32, E_LOG_OP_TYPE = def_type::E_LOG_OP_SQL_DB);

	typedef typename def_type::id_type id_type;

	typedef Connect conn_type;

	typedef typename conn_type::init_type init_type;

	typedef database_system::database_accesser real_op_type;
	typedef real_op_type::error_type error_type;

private:
	typedef real_op_type::org_string_type org_string_type;

private:
	typedef log_sql_db_op this_type;

public:

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename ...Args> 
	log_sql_db_op(BOOST_FWD_REF(Args)... args) 
		: _conn(boost::forward<Args>(args)...)
	{
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		log_sql_db_op( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) \
			: _conn(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )){ }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	~log_sql_db_op(void)
	{
		_conn.clear();
	}

#	include <yggr/log/detail/log_op_general_foo.ipp>

public:

	// append
	template<typename Log_Data,
				typename String1, typename String2> inline
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
		append(const String1& tb_names,
				const Log_Data& log_data,
				const String2& postfix_str,
				error_type& err,
				bool bcommit = false,
				bool brollback = true)
	{
		typedef container_ex::is_container<Log_Data> chk_type;
		typedef detail::log_sql_db_op_container_make_helper<chk_type::value> h_type;

		h_type h;

		return real_op_type::s_insert_records_of_table(_conn, 
														tb_names,
														h(log_data),
														postfix_str,
														err,
														bcommit,
														brollback);
	}

	template<typename Log_Data, typename Handler_Value_Maker,
				typename String1, typename String2> inline
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
		append(const String1& tb_names,
				const Log_Data& log_data,
				const String2& postfix_str,
				const Handler_Value_Maker& handler_val_maker,
				error_type& err,
				bool bcommit = false,
				bool brollback = true)
	{
		typedef container_ex::is_container<Log_Data> chk_type;
		typedef detail::log_sql_db_op_container_make_helper<chk_type::value> h_type;

		h_type h;

		return real_op_type::s_insert_records_of_table(_conn, 
														tb_names,
														h(log_data),
														postfix_str,
														handler_val_maker,
														err,
														bcommit,
														brollback);
	}

	// append_any // same to append
	template<typename Log_Data,
				typename String1, typename String2> inline
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
		append_any(const String1& tb_names,
					const Log_Data& log_data,
					const String2& postfix_str,
					error_type& err,
					bool bcommit = false,
					bool brollback = true)
	{
		return this_type::append(tb_names, log_data, postfix_str, 
									err, bcommit, brollback);
	}

	template<typename Log_Data, typename Handler_Value_Maker,
				typename String1, typename String2> inline
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
		append_any(const String1& tb_names,
					const Log_Data& log_data,
					const String2& postfix_str,
					const Handler_Value_Maker& handler_val_maker,
					error_type& err,
					bool bcommit = false,
					bool brollback = true)
	{
		return this_type::append(tb_names, log_data, postfix_str, handler_val_maker, 
									err, bcommit, brollback);
	}

	// clear
	template<typename Cmd_Data,
				typename String1, typename String2> inline
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
		clear(const String1& tb_names,
				const Cmd_Data& cmd_data,
				const String2& postfix_str,
				error_type& err,
				bool bcommit = false,
				bool brollback = true)
	{
		typedef container_ex::is_container<Cmd_Data> chk_type;
		typedef detail::log_sql_db_op_container_make_helper<chk_type::value> h_type;

		h_type h;

		return real_op_type::s_delete_records_of_table(
					_conn, tb_names, h(cmd_data), postfix_str, err);
	}

	template<typename Cmd_Data, typename Handler_Value_Maker,
				typename String1, typename String2> inline
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
		clear(const String1& tb_names,
				const Cmd_Data& cmd_data,
				const String2& postfix_str,
				const Handler_Value_Maker& handler_val_maker,
				error_type& err,
				bool bcommit = false,
				bool brollback = true)
	{
		typedef container_ex::is_container<Cmd_Data> chk_type;
		typedef detail::log_sql_db_op_container_make_helper<chk_type::value> h_type;

		h_type h;

		return real_op_type::s_delete_records_of_table(
				_conn, tb_names, h(cmd_data), postfix_str,
				handler_val_maker, err);
	}

	template<typename String> inline
	typename boost::enable_if< charset::utf8_string_constructable<String>, bool>::type
		clear(const String& tb_name,
				error_type& err,
				bool bcommit = false,
				bool brollback = true)
	{
		org_string_type cmd("delete from ");
		org_string_type cmd_data(
			database_system::database_string_helper::s_conv(
				charset::string_charset_helper_first(tb_name), 
				charset::string_charset_helper_second(tb_name)) );

		return real_op_type::s_execute_sql_non_conv(
				_conn, cmd + cmd_data, err, bcommit, brollback);
	}

	// clear_any
#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template< typename ...Args > inline
	bool clear_any(BOOST_FWD_REF(Args)... args) 
	{
		assert(false);
		return false; 
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		bool clear_any( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			assert(false); \
			return false; }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	// search
	template<typename Container,
				typename String1, typename String2> inline
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
		search(const String1& tb_names,
					Container& rows,
					const String2& postfix_str,
					error_type& err) const
	{
		return 
			real_op_type::s_select_records_of_view(
				_conn, tb_names, rows, postfix_str, err);
	}

	template<typename Container, typename Handler_Value_Maker,
				typename String1, typename String2> inline
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
		search(const String1& tb_names,
					Container& rows,
					const String2& postfix_str,
					const Handler_Value_Maker& handler_val_maker,
					error_type& err) const
	{
		return 
			real_op_type::s_select_records_of_view(
				_conn, tb_names, rows, postfix_str, err);
	}

	// search_any
	template<typename Container,
				typename String1, typename String2> inline
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
		search_any(const String1& tb_names,
					Container& rows,
					const String2& postfix_str,
					error_type& err) const
	{
		return this_type::search(tb_names. rows, postfix_str, err);
	}

	template<typename Container, typename Handler_Value_Maker,
				typename String1, typename String2> inline
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
		search_any(const String1& tb_names,
					Container& rows,
					const String2& postfix_str,
					const Handler_Value_Maker& handler_val_maker,
					error_type& err) const
	{
		return this_type::search(tb_names. rows, postfix_str, handler_val_maker, err);
	}

private:
	mutable conn_type _conn;
};

} // namespce log_op
} // namspace log
} // namespace yggr

#endif //__YGGR_LOG_LOGOP_LOG_SQL_DB_OP_HPP__

