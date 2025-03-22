//log_sql_db_op_caller.hpp

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

#ifndef __YGGR_LOG_LOG_SQL_DB_OP_CALLER_HPP__
#define __YGGR_LOG_LOG_SQL_DB_OP_CALLER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/utility/to_ref_helper.hpp>
#include <yggr/charset/utf8_string.hpp>
#include <yggr/type_traits/native_t.hpp>

#include <yggr/ppex/foo_params.hpp>
#include <yggr/ppex/symbol_if.hpp>
#include <yggr/ppex/symbols.hpp>
#include <yggr/ppex/swap_this_def.hpp>

#include <yggr/mplex/tag_sfinae.hpp>

#include <yggr/container/array.hpp>

#include <yggr/safe_container/safe_wrap.hpp>
#include <yggr/database_system/database_accesser.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/thread/shared_mutex.hpp>

namespace yggr
{
namespace log
{

class log_sql_db_op_caller
{
public:
	typedef database_system::database_string_helper::org_string_type org_string_type;
	typedef database_system::database_string_helper::inner_string_type inner_string_type;

	typedef org_string_type id_type;
	typedef database_system::database_accesser::error_type error_type;

private:
	typedef database_system::database_string_helper string_helper_type;
	typedef safe_container::safe_wrap<org_string_type, boost::shared_mutex> org_string_wrap_type;
	typedef safe_container::safe_wrap<bool, boost::shared_mutex> bool_wrap_type;
	typedef safe_container::safe_container_error::error_type safe_cont_err_type;

private:
	typedef log_sql_db_op_caller this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

private:
	typedef mplex::sfinae_type sfinae_type;

public:
	log_sql_db_op_caller(void);

	template<typename String>
	log_sql_db_op_caller(const String& db_names,
							bool bcommit, 
							bool brollback,
							typename
								boost::enable_if
								<
									charset::utf8_string_constructable<String>,
									sfinae_type
								>::type sfinae = 0 )
		: _db_names(string_helper_type::s_conv(
						charset::string_charset_helper_first(db_names),
						charset::string_charset_helper_second(db_names))),
			_bcommit(bcommit), 
			_brollback(brollback)
	{
	}

	log_sql_db_op_caller(const org_string_type& db_names,
							bool bcommit, bool brollback);

	log_sql_db_op_caller(BOOST_RV_REF(this_type) right)
		: _db_names(boost::move(right._db_names)),
			_bcommit(boost::move(right._bcommit)),
			_brollback(boost::move(right._brollback))
	{
	}

	log_sql_db_op_caller(const this_type& right);

	~log_sql_db_op_caller(void);

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		copy_or_move_or_swap(_db_names, boost::move(right._db_names));
		copy_or_move_or_swap(_bcommit, boost::move(right._bcommit));
		copy_or_move_or_swap(_brollback, boost::move(right._brollback));

		return *this;
	}

	this_type& operator=(const this_type& right);

public:
	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right);

public:
	inline id_type id(void) const
	{
		try
		{
			return _db_names.load();
		}
		catch(const org_string_wrap_type::error_type&)
		{
			return id_type();
		}
	}

	inline bool empty(void) const
	{
		return _db_names.empty();
	}

	// call_append
	template<typename Op, typename LogData, typename String> inline
	typename boost::enable_if<charset::utf8_string_constructable<String>, bool>::type
		call_append(Op& op,
					const LogData& log_data,
					const String& postfix,
					error_type& err) const
	{
		org_string_type db_names;
		bool bcommit = false;
		bool brollback = false;

		try
		{
			db_names = _db_names.load();
			bcommit = _bcommit.load();
			brollback = _brollback.load();
		}
		catch(const safe_cont_err_type&)
		{
			return false;
		}

		return
			op.append(
				db_names, 
				log_data,
				postfix,
				err,
				bcommit,
				brollback);
	}

	template<typename Op, typename LogData> inline
	bool call_append(Op& op, const LogData& log_data) const
	{
		org_string_type postfix("");
		error_type err;

		return this_type::call_append(op, log_data, postfix, err);
	}

	template<typename Op, typename LogData, typename String, typename Handler_Value_Maker> inline
	typename boost::enable_if<charset::utf8_string_constructable<String>, bool>::type
		call_append(Op& op,
					const LogData& log_data,
					const String& postfix,
					const Handler_Value_Maker& handler_val_maker,
					error_type& err) const
	{
		org_string_type db_names;
		bool bcommit = false;
		bool brollback = false;

		try
		{
			db_names = _db_names.load();
			bcommit = _bcommit.load();
			brollback = _brollback.load();
		}
		catch(const safe_cont_err_type&)
		{
			return false;
		}

		return
			op.append(
				db_names, 
				log_data,
				postfix,
				handler_val_maker,
				err,
				bcommit,
				brollback);
	}

	template<typename Op, typename LogData, typename Handler_Value_Maker> inline
	bool call_append(Op& op, const LogData& log_data,
						const Handler_Value_Maker& handler_val_maker) const
	{
		org_string_type postfix("");
		error_type err;

		return 
			this_type::call_append(
				op, log_data, postfix, handler_val_maker, 
				err, YGGR_STR_DATABASE_CHARSET_NAME());
	}

	// call_append_any
	template<typename Op, typename LogData, typename String> inline
	typename boost::enable_if<charset::utf8_string_constructable<String>, bool>::type
		call_append_any(Op& op,
						const LogData& log_data,
						const String& postfix,
						error_type& err) const
	{
		return this_type::call_append(op, log_data, postfix, err);
	}

	template<typename Op, typename LogData> inline
	bool call_append_any(Op& op, const LogData& log_data) const
	{
		return this_type::call_append(op, log_data);
	}

	template<typename Op, typename LogData, typename String, typename Handler_Value_Maker> inline
	typename boost::enable_if<charset::utf8_string_constructable<String>, bool>::type
		call_append_any(Op& op,
						const LogData& log_data,
						const String& postfix,
						const Handler_Value_Maker& handler_val_maker,
						error_type& err) const
	{
		return this_type::call_append(op, log_data, postfix, handler_val_maker, err);
	}

	template<typename Op, typename LogData, typename Handler_Value_Maker> inline
	bool call_append_any(Op& op, const LogData& log_data,
						const Handler_Value_Maker& handler_val_maker) const
	{
		return this_type::call_append(op, log_data, handler_val_maker);
	}

	// call_clear
	template<typename Op, typename Cmd_Data, typename String> inline
	typename boost::enable_if<charset::utf8_string_constructable<String>, bool>::type
		call_clear(Op& op,
					const Cmd_Data& cmd_data,
					const String& postfix,
					error_type& err) const
	{
		org_string_type db_names;
		bool bcommit = false;
		bool brollback = false;

		try
		{
			db_names = _db_names.load();
			bcommit = _bcommit.load();
			brollback = _brollback.load();
		}
		catch(const safe_cont_err_type&)
		{
			return false;
		}

		return 
			op.clear(
				db_names, 
				cmd_data,
				postfix,
				err,
				bcommit,
				brollback);
	}

	template<typename Op, typename Cmd_Data> inline
	bool call_clear(Op& op, const Cmd_Data& cmd_data) const
	{
		org_string_type postfix("");
		error_type err;

		return this_type::call_clear(op, cmd_data, postfix, err);
	}

	template<typename Op, typename Cmd_Data,
				typename String, typename Handler_Value_Maker> inline
	typename boost::enable_if<charset::utf8_string_constructable<String>, bool>::type
		call_clear(Op& op,
					const Cmd_Data& cmd_data,
					const String& postfix,
					const Handler_Value_Maker& handler_val_maker,
					error_type& err) const
	{
		org_string_type db_names;
		bool bcommit = false;
		bool brollback = false;

		try
		{
			db_names = _db_names.load();
			bcommit = _bcommit.load();
			brollback = _brollback.load();
		}
		catch(const safe_cont_err_type&)
		{
			return false;
		}

		return
			op.clear(
				db_names, 
				cmd_data,
				postfix,
				handler_val_maker,
				err,
				bcommit,
				brollback);
	}

	template<typename Op, typename Cmd_Data, typename Handler_Value_Maker> inline
	bool call_clear(Op& op, const Cmd_Data& cmd_data,
					const Handler_Value_Maker& handler_val_maker) const
	{
		org_string_type postfix("");
		error_type err;

		return this_type::call_clear(op, cmd_data, postfix, handler_val_maker, err);
	}

	template<typename Op> inline
	bool call_clear(Op& op, error_type& err) const
	{
		org_string_type db_names;
		bool bcommit = false;
		bool brollback = false;

		try
		{
			db_names = _db_names.load();
			bcommit = _bcommit.load();
			brollback = _brollback.load();
		}
		catch(const safe_cont_err_type&)
		{
			return false;
		}

		return op.clear(db_names, err, bcommit, brollback);
	}

	template<typename Op> inline
	bool call_clear(Op& op) const
	{
		error_type err;
		return this_type::call_clear(op, err);
	}

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template< typename Op, typename ...Args> inline 
	bool call_clear_any( Op& op, BOOST_FWD_REF(Args)... args) const
	{
		return op.clear_any(boost::forward<Args>(args)...); 
	}

#else

	// call_clear_any
#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template< typename Op \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
	bool call_clear_any( Op& op \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const { \
		return op.clear_any( YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); }

#define YGGR_PP_FOO_ARG_NAME() init_arg
#define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE(  )
#undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	// call_search
	template<typename Op, typename Container, typename String> inline
	typename boost::enable_if<charset::utf8_string_constructable<String>, bool>::type
		call_search(Op& op, 
					Container& rows,
					const String& postfix,
					error_type& err) const
	{

		org_string_type db_names;

		try
		{
			db_names = _db_names.load();
		}
		catch(const safe_cont_err_type&)
		{
			return false;
		}

		return op.search(db_names, to_ref_helper::to_ref(rows), postfix, err);
	}

	template<typename Op, typename Container> inline
	bool call_search(Op& op, Container& rows) const
	{
		org_string_type postfix("");
		error_type err;
		return this_type::call_search(op, rows, postfix, err);
	}

	template<typename Op, typename Container,
				typename String, typename Handler_Value_Maker> inline
	typename boost::enable_if<charset::utf8_string_constructable<String>, bool>::type
		call_search(Op& op, 
						Container& rows,
						const String& postfix,
						const Handler_Value_Maker& handler_val_maker,
						error_type& err ) const
	{

		org_string_type db_names;

		try
		{
			db_names = _db_names.load();
		}
		catch(const safe_cont_err_type&)
		{
			return false;
		}

		return 
			op.search(
				db_names, 
				to_ref_helper::to_ref(rows),
				postfix,
				handler_val_maker,
				err);
	}

	template<typename Op, typename Container, typename Handler_Value_Maker> inline
	bool call_search(Op& op, 
						Container& rows,
						const Handler_Value_Maker& handler_val_maker) const
	{
		org_string_type postfix("");
		error_type err;

		return this_type::call_search(op, rows, postfix, handler_val_maker, err);
	}

	// call_search_any
	template<typename Op, typename Container, typename String> inline
	typename boost::enable_if<charset::utf8_string_constructable<String>, bool>::type
		call_search_any(Op& op, 
						Container& rows,
						const String& postfix,
						error_type& err) const
	{

		org_string_type db_names;

		try
		{
			db_names = _db_names.load();
		}
		catch(const safe_cont_err_type&)
		{
			return false;
		}

		return op.search_any(db_names, to_ref_helper::to_ref(rows), postfix, err);
	}

	template<typename Op, typename Container> inline
	bool call_search_any(Op& op, Container& rows) const
	{
		org_string_type postfix("");
		error_type err;
		return this_type::call_search_any(op, rows, postfix, err, YGGR_STR_DATABASE_CHARSET_NAME());
	}

	template<typename Op, typename Container,
				typename String, typename Handler_Value_Maker> inline
	typename boost::enable_if<charset::utf8_string_constructable<String>, bool>::type
		call_search_any(Op& op, 
						Container& rows,
						const String& postfix,
						const Handler_Value_Maker& handler_val_maker,
						error_type& err) const
	{

		org_string_type db_names;

		try
		{
			db_names = _db_names.load();
		}
		catch(const safe_cont_err_type&)
		{
			return false;
		}

		return 
			op.search_any(
				db_names, 
				to_ref_helper::to_ref(rows),
				postfix,
				handler_val_maker,
				err);
	}

	template<typename Op, typename Container, typename Handler_Value_Maker> inline
	bool call_search_any(Op& op, Container& rows,
							const Handler_Value_Maker& handler_val_maker) const
	{
		org_string_type postfix("");
		error_type err;

		return this_type::call_search_any(op, rows, postfix, handler_val_maker, err);
	}

private:
	inline static const char* unknown_log_name(void)
	{
		return "unknown_log";
	}

private:
	org_string_wrap_type _db_names;
	bool_wrap_type _bcommit;
	bool_wrap_type _brollback;
};

// non_member_foo

// operator==
inline bool operator==(const log_sql_db_op_caller::id_type& l, 
						const log_sql_db_op_caller& r)
{
	return l == r.id();
}

inline bool operator==(const log_sql_db_op_caller& l,
						const log_sql_db_op_caller::id_type& r)
{
	return l.id() == r;
}

inline bool operator==(const log_sql_db_op_caller& l,
						const log_sql_db_op_caller& r)
{
	return (boost::addressof(l) == boost::addressof(r)) || (l.id() == r.id());
}

// operator!=
inline bool operator!=(const log_sql_db_op_caller::id_type& l, 
						const log_sql_db_op_caller& r)
{
	return !(l == r.id());
}

inline bool operator!=(const log_sql_db_op_caller& l,
						const log_sql_db_op_caller::id_type& r)
{
	return !(l.id() == r);
}

inline bool operator!=(const log_sql_db_op_caller& l,
						const log_sql_db_op_caller& r)
{
	return !((boost::addressof(l) == boost::addressof(r)) || (l.id() == r.id()));
}

} // namespace log
} // namespace yggr

namespace yggr
{
namespace log
{
namespace swap_support
{
	YGGR_PP_INLINE_SWAP_THIS_REF_DEF_IMPL(log_sql_db_op_caller)
} // namespace swap_support

using swap_support::swap;

} // namespace log
} // namespace yggr

namespace std
{
	using ::yggr::log::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::log::swap_support::swap;
} // namespace boost

#endif // __YGGR_LOG_LOG_SQL_DB_OP_CALLER_HPP__
