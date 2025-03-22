//log_mongodb_op_caller

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

#ifndef __YGGR_LOG_LOG_MONGODB_OP_CALLER_HPP__
#define __YGGR_LOG_LOG_MONGODB_OP_CALLER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>

#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/string_converter.hpp>

#include <yggr/ppex/foo_params.hpp>
#include <yggr/ppex/symbol_if.hpp>
#include <yggr/ppex/symbols.hpp>
#include <yggr/ppex/swap_this_def.hpp>

#include <yggr/mplex/tag_sfinae.hpp>

#include <yggr/type_traits/native_t.hpp>

#include <yggr/safe_container/safe_wrap.hpp>

#include <yggr/nsql_database_system/c_bson.hpp>
#include <yggr/nsql_database_system/c_bson_error.hpp>

#include <yggr/nsql_database_system/c_mongo_read_prefs.hpp>
#include <yggr/nsql_database_system/c_mongo_write_concern.hpp>

#include <boost/ref.hpp>
#include <boost/thread/shared_mutex.hpp>

namespace yggr
{
namespace log
{

class log_mongodb_op_caller
{
public:
	typedef utf8_string inner_string_type;

	typedef nsql_database_system::c_bson bson_type;
	typedef bson_type::org_type org_bson_type;

	typedef nsql_database_system::c_bson_error bson_error_type;
	typedef bson_error_type::org_type org_bson_error_type;

	typedef nsql_database_system::c_mongo_read_prefs read_prefs_type;
	typedef nsql_database_system::c_mongo_write_concern write_concern_type;

	typedef inner_string_type id_type;

private:
	typedef safe_container::safe_wrap<inner_string_type, boost::shared_mutex> inner_string_wrap_type;
	typedef safe_container::safe_wrap<bson_type> bson_wrap_type;
	typedef safe_container::safe_wrap<read_prefs_type, boost::shared_mutex> read_prefs_wrap_type;
	typedef safe_container::safe_wrap<write_concern_type, boost::shared_mutex> write_concern_wrap_type;
	typedef safe_container::safe_wrap<u32, boost::shared_mutex> flag_wrap_type;
	typedef safe_container::safe_container_error::error_type safe_cont_err_type;

private:
	typedef log_mongodb_op_caller this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

private:
	typedef mplex::sfinae_type sfinae_type;

public:
	log_mongodb_op_caller(void);

	template<typename String1, typename String2>
	log_mongodb_op_caller(const String1& str_db,
							const String2& str_coln,
							typename
								boost::enable_if
								<
									boost::mpl::and_
									<
										charset::utf8_string_constructable<String1>,
										charset::utf8_string_constructable<String2>
									>,
									sfinae_type
								>::type sfinae = 0)
		: _str_db((inner_string_type(str_db))),
			_str_coln((inner_string_type(str_coln))),
			_str_db_coln((
							inner_string_type(str_db)
							+ inner_string_type(".", YGGR_STR_UTF8_STRING_CHARSET_NAME())
							+ inner_string_type(str_coln)
						)),
			_append_opts(bson_type()),
			_search_opts(bson_type()),
			_search_read_prefs(read_prefs_type()),
			_clear_flags(static_cast<u32>(0)),
			_clear_opts(bson_type())
	{
	}

	template<typename String1, typename String2,
				typename BsonOptsAppend,
				typename BsonOptsSearch, typename SearchReadPrefs,
				typename BsonOptsClear>
	log_mongodb_op_caller(const String1& str_db,
							const String2& str_coln,
							const BsonOptsAppend& opts_append,
							const BsonOptsSearch& opts_search,
							const SearchReadPrefs& read_prefs_search,
							u32 flags_clear,
							const BsonOptsClear& opts_clear,
							typename
								boost::enable_if
								<
									boost::mpl::and_
									<
										charset::utf8_string_constructable<String1>,
										charset::utf8_string_constructable<String2>
									>,
									sfinae_type
								>::type sfinae = 0)
		: _str_db((inner_string_type(str_db))),
			_str_coln((inner_string_type(str_coln))),
			_str_db_coln((
							inner_string_type(str_db)
							+ inner_string_type(".", YGGR_STR_UTF8_STRING_CHARSET_NAME())
							+ inner_string_type(str_coln)
						)),
			_append_opts(opts_append),
			_search_opts(opts_search),
			_search_read_prefs((read_prefs_type())),
			_clear_flags(flags_clear),
			_clear_opts(opts_clear)
	{
	}

	log_mongodb_op_caller(BOOST_RV_REF(this_type) right)
		: _str_db(boost::move(right._str_db)),
			_str_coln(boost::move(right._str_coln)),
			_str_db_coln(boost::move(right._str_db_coln)),
			_append_opts(boost::move(right._append_opts)),
			_search_opts(boost::move(right._search_opts)),
			_search_read_prefs(boost::move(right._search_read_prefs)),
			_clear_flags(boost::move(right._clear_flags)),
			_clear_opts(boost::move(right._clear_opts))
	{
	}

	log_mongodb_op_caller(const this_type& right);
	~log_mongodb_op_caller(void);

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		copy_or_move_or_swap(_str_db, boost::move(right._str_db));
		copy_or_move_or_swap(_str_coln, boost::move(right._str_coln));
		copy_or_move_or_swap(_str_db_coln, boost::move(right._str_db_coln));

		copy_or_move_or_swap(_append_opts, boost::move(right._append_opts));

		copy_or_move_or_swap(_search_opts, boost::move(right._search_opts));
		copy_or_move_or_swap(_search_read_prefs, boost::move(right._search_read_prefs));

		copy_or_move_or_swap(_clear_flags, boost::move(right._clear_flags));
		copy_or_move_or_swap(_clear_opts, boost::move(right._clear_opts));

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
			return _str_db_coln.load();
		}
		catch(const inner_string_wrap_type::error_type&)
		{
			return id_type();
		}
	}

	inline bool empty(void) const
	{
		return _str_db_coln.empty();
	}

	// call_append
	// 6 args

protected:
	template<typename Op, typename LogData, typename BsonOpts> inline
	bool call_append_wrap(Op& op,
							const LogData& log_data, const BsonOpts& opts,
							org_bson_type* preply,
							org_bson_error_type* perr) const
	{
		inner_string_type str_db;
		inner_string_type str_coln;

		try
		{
			str_db = _str_db.load();
			str_coln = _str_coln.load();
		}
		catch(const safe_cont_err_type&)
		{
			return false;
		}

		return
			op.append(
				str_db, str_coln,
				log_data, opts,
				preply, perr);
	}

public:
	template<typename Op, typename LogData, typename BsonOpts> inline
	typename
		boost::enable_if
		<
			boost::mpl::or_
			<
				boost::is_integral<BsonOpts>,
				boost::is_base_of<bson_t, typename native_t<BsonOpts>::type >
			>,
			bool
		>::type
		call_append(Op& op,
					const LogData& log_data, const BsonOpts& opts,
					org_bson_type* preply,
					org_bson_error_type* perr = 0) const
	{
		return this_type::call_append_wrap(op, log_data, opts, preply, perr);
	}

	template<typename Op, typename LogData, typename BsonOpts> inline
	typename
		boost::enable_if
		<
			boost::mpl::or_
			<
				boost::is_integral<BsonOpts>,
				boost::is_base_of<bson_t, typename native_t<BsonOpts>::type >
			>,
			bool
		>::type
		call_append(Op& op,
					const LogData& log_data, const BsonOpts& opts,
					org_bson_type& reply,
					org_bson_error_type* perr = 0) const
	{
		return this_type::call_append_wrap(op, log_data, opts, boost::addressof(reply), perr);
	}

	template<typename Op, typename LogData, typename BsonOpts> inline
	typename
		boost::enable_if
		<
			boost::mpl::or_
			<
				boost::is_integral<BsonOpts>,
				boost::is_base_of<bson_t, typename native_t<BsonOpts>::type >
			>,
			bool
		>::type
		call_append(Op& op,
					const LogData& log_data, const BsonOpts& opts,
					org_bson_type* preply,
					org_bson_error_type& err) const
	{
		return this_type::call_append_wrap(op, log_data, opts, preply, boost::addressof(err));
	}

	template<typename Op, typename LogData, typename BsonOpts> inline
	typename
		boost::enable_if
		<
			boost::mpl::or_
			<
				boost::is_integral<BsonOpts>,
				boost::is_base_of<bson_t, typename native_t<BsonOpts>::type >
			>,
			bool
		>::type
		call_append(Op& op,
					const LogData& log_data, const BsonOpts& opts,
					org_bson_type& reply,
					org_bson_error_type& err) const
	{
		return this_type::call_append_wrap(op, log_data, opts, boost::addressof(reply), boost::addressof(err));
	}

protected:
	template<typename Op,
				typename Bson_Container, typename Handler_BsonPtr_Getter_Or_Size,
				typename BsonOpts> inline
	bool call_append_wrap(Op& op,
							const Bson_Container& bson_container,
							const Handler_BsonPtr_Getter_Or_Size& handler_bson_ptr_getter_or_size,
							const BsonOpts& opts,
							org_bson_type* preply,
							org_bson_error_type* perr) const
	{
		inner_string_type str_db;
		inner_string_type str_coln;

		try
		{
			str_db = _str_db.load();
			str_coln = _str_coln.load();
		}
		catch(const safe_cont_err_type&)
		{
			return false;
		}

		return
			op.append(
				str_db, str_coln,
				bson_container, handler_bson_ptr_getter_or_size, opts,
				preply, perr);
	}


public:
	// 7 args
	template<typename Op,
				typename Bson_Container, typename Handler_BsonPtr_Getter_Or_Size,
				typename BsonOpts> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::or_
				<
					boost::is_pointer<Bson_Container>,
					container_ex::is_container<Bson_Container>
				>,
				boost::mpl::or_
				<
					typename func::foo_t_info<Handler_BsonPtr_Getter_Or_Size>::is_callable_type,
					boost::is_integral<Handler_BsonPtr_Getter_Or_Size>
				>
			>,
			bool
		>::type
		call_append(Op& op,
					const Bson_Container& bson_container,
					const Handler_BsonPtr_Getter_Or_Size& handler_bson_ptr_getter_or_size,
					const BsonOpts& opts,
					org_bson_type* preply,
					org_bson_error_type* perr = 0) const
	{
		return
			this_type::call_append_wrap(
				op, bson_container, handler_bson_ptr_getter_or_size, opts,
				preply, perr);
	}

	template<typename Op,
				typename Bson_Container, typename Handler_BsonPtr_Getter_Or_Size,
				typename BsonOpts> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::or_
				<
					boost::is_pointer<Bson_Container>,
					container_ex::is_container<Bson_Container>
				>,
				boost::mpl::or_
				<
					typename func::foo_t_info<Handler_BsonPtr_Getter_Or_Size>::is_callable_type,
					boost::is_integral<Handler_BsonPtr_Getter_Or_Size>
				>
			>,
			bool
		>::type
		call_append(Op& op,
					const Bson_Container& bson_container,
					const Handler_BsonPtr_Getter_Or_Size& handler_bson_ptr_getter_or_size,
					const BsonOpts& opts,
					org_bson_type& reply,
					org_bson_error_type* perr = 0) const
	{
		return
			this_type::call_append_wrap(
				op, bson_container, handler_bson_ptr_getter_or_size, opts,
				boost::addressof(reply), perr);
	}

	template<typename Op,
				typename Bson_Container, typename Handler_BsonPtr_Getter_Or_Size,
				typename BsonOpts> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::or_
				<
					boost::is_pointer<Bson_Container>,
					container_ex::is_container<Bson_Container>
				>,
				boost::mpl::or_
				<
					typename func::foo_t_info<Handler_BsonPtr_Getter_Or_Size>::is_callable_type,
					boost::is_integral<Handler_BsonPtr_Getter_Or_Size>
				>
			>,
			bool
		>::type
		call_append(Op& op,
					const Bson_Container& bson_container,
					const Handler_BsonPtr_Getter_Or_Size& handler_bson_ptr_getter_or_size,
					const BsonOpts& opts,
					org_bson_type* preply,
					org_bson_error_type& err) const
	{
		return
			this_type::call_append_wrap(
				op, bson_container, handler_bson_ptr_getter_or_size, opts,
				preply, boost::addressof(err));
	}

	template<typename Op,
				typename Bson_Container, typename Handler_BsonPtr_Getter_Or_Size,
				typename BsonOpts> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::or_
				<
					boost::is_pointer<Bson_Container>,
					container_ex::is_container<Bson_Container>
				>,
				boost::mpl::or_
				<
					typename func::foo_t_info<Handler_BsonPtr_Getter_Or_Size>::is_callable_type,
					boost::is_integral<Handler_BsonPtr_Getter_Or_Size>
				>
			>,
			bool
		>::type
		call_append(Op& op,
					const Bson_Container& bson_container,
					const Handler_BsonPtr_Getter_Or_Size& handler_bson_ptr_getter_or_size,
					const BsonOpts& opts,
					org_bson_type& reply,
					org_bson_error_type& err) const
	{
		return
			this_type::call_append_wrap(
				op, bson_container, handler_bson_ptr_getter_or_size, opts,
				boost::addressof(reply), boost::addressof(err));
	}

public:
	template<typename Op, typename LogData> inline
	bool call_append(Op& op, const LogData& log_data) const
	{
		bson_type opts;

		try
		{
			opts = _append_opts.load();
		}
		catch(const safe_cont_err_type&)
		{
			return false;
		}

		return
			this_type::call_append_wrap(
				op, log_data, opts,
				0, 0);
	}

	template<typename Op, typename Bson_Container, typename Handler_BsonPtr_Getter_Or_Size> inline
	bool call_append(Op& op,
						const Bson_Container& bson_container,
						const Handler_BsonPtr_Getter_Or_Size& handler_bson_ptr_getter_or_size) const
	{
		bson_type opts;

		try
		{
			opts = _append_opts.load();
		}
		catch(const safe_cont_err_type&)
		{
			return false;
		}

		return
			this_type::call_append_wrap(
				op, bson_container, handler_bson_ptr_getter_or_size, opts,
				0, 0);
	}

	// call_append_any
	// 6 args
protected:
	template<typename Op, typename LogData, typename BsonOpts> inline
	bool call_append_any_wrap(Op& op,
								const LogData& log_data, const BsonOpts& opts,
								org_bson_type* preply,
								org_bson_error_type* perr) const
	{
		inner_string_type str_db;
		inner_string_type str_coln;

		try
		{
			str_db = _str_db.load();
			str_coln = _str_coln.load();
		}
		catch(const safe_cont_err_type&)
		{
			return false;
		}

		return
			op.append_any(
				str_db, str_coln,
				log_data, opts,
				preply, perr);
	}

public:
	template<typename Op, typename LogData, typename BsonOpts> inline
	typename
		boost::enable_if
		<
			boost::mpl::or_
			<
				boost::is_integral<BsonOpts>,
				boost::is_base_of<bson_t, typename native_t<BsonOpts>::type >
			>,
			bool
		>::type
		call_append_any(Op& op,
						const LogData& log_data, const BsonOpts& opts,
						org_bson_type* preply,
						org_bson_error_type* perr = 0) const
	{
		return this_type::call_append_any_wrap(op, log_data, opts, preply, perr);
	}

	template<typename Op, typename LogData, typename BsonOpts> inline
	typename
		boost::enable_if
		<
			boost::mpl::or_
			<
				boost::is_integral<BsonOpts>,
				boost::is_base_of<bson_t, typename native_t<BsonOpts>::type >
			>,
			bool
		>::type
		call_append_any(Op& op,
						const LogData& log_data, const BsonOpts& opts,
						org_bson_type& reply,
						org_bson_error_type* perr = 0) const
	{
		return this_type::call_append_any_wrap(op, log_data, opts, boost::addressof(reply), perr);
	}

	template<typename Op, typename LogData, typename BsonOpts> inline
	typename
		boost::enable_if
		<
			boost::mpl::or_
			<
				boost::is_integral<BsonOpts>,
				boost::is_base_of<bson_t, typename native_t<BsonOpts>::type >
			>,
			bool
		>::type
		call_append_any(Op& op,
						const LogData& log_data, const BsonOpts& opts,
						org_bson_type* preply,
						org_bson_error_type& err) const
	{
		return this_type::call_append_any_wrap(op, log_data, opts, preply, boost::addressof(err));
	}

	template<typename Op, typename LogData, typename BsonOpts> inline
	typename
		boost::enable_if
		<
			boost::mpl::or_
			<
				boost::is_integral<BsonOpts>,
				boost::is_base_of<bson_t, typename native_t<BsonOpts>::type >
			>,
			bool
		>::type
		call_append_any(Op& op,
						const LogData& log_data, const BsonOpts& opts,
						org_bson_type& reply,
						org_bson_error_type& err) const
	{
		return this_type::call_append_any_wrap(op, log_data, opts, boost::addressof(reply), boost::addressof(err));
	}

protected:
	template<typename Op,
				typename Bson_Container, typename Handler_BsonPtr_Getter_Or_Size,
				typename BsonOpts> inline
	bool call_append_any_wrap(Op& op,
								const Bson_Container& bson_container,
								const Handler_BsonPtr_Getter_Or_Size& handler_bson_ptr_getter_or_size,
								const BsonOpts& opts,
								org_bson_type* preply,
								org_bson_error_type* perr) const
	{
		inner_string_type str_db;
		inner_string_type str_coln;

		try
		{
			str_db = _str_db.load();
			str_coln = _str_coln.load();
		}
		catch(const safe_cont_err_type&)
		{
			return false;
		}

		return
			op.append_any(
				str_db, str_coln,
				bson_container, handler_bson_ptr_getter_or_size, opts,
				preply, perr);
	}


public:
	// 7 args
	template<typename Op,
				typename Bson_Container, typename Handler_BsonPtr_Getter_Or_Size,
				typename BsonOpts> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::or_
				<
					boost::is_pointer<Bson_Container>,
					container_ex::is_container<Bson_Container>
				>,
				boost::mpl::or_
				<
					typename func::foo_t_info<Handler_BsonPtr_Getter_Or_Size>::is_callable_type,
					boost::is_integral<Handler_BsonPtr_Getter_Or_Size>
				>
			>,
			bool
		>::type
		call_append_any(Op& op,
						const Bson_Container& bson_container,
						const Handler_BsonPtr_Getter_Or_Size& handler_bson_ptr_getter_or_size,
						const BsonOpts& opts,
						org_bson_type* preply,
						org_bson_error_type* perr = 0) const
	{
		return
			this_type::call_append_any_wrap(
				op, bson_container, handler_bson_ptr_getter_or_size, opts,
				preply, perr);
	}

	template<typename Op,
				typename Bson_Container, typename Handler_BsonPtr_Getter_Or_Size,
				typename BsonOpts> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::or_
				<
					boost::is_pointer<Bson_Container>,
					container_ex::is_container<Bson_Container>
				>,
				boost::mpl::or_
				<
					typename func::foo_t_info<Handler_BsonPtr_Getter_Or_Size>::is_callable_type,
					boost::is_integral<Handler_BsonPtr_Getter_Or_Size>
				>
			>,
			bool
		>::type
		call_append_any(Op& op,
						const Bson_Container& bson_container,
						const Handler_BsonPtr_Getter_Or_Size& handler_bson_ptr_getter_or_size,
						const BsonOpts& opts,
						org_bson_type& reply,
						org_bson_error_type* perr = 0) const
	{
		return
			this_type::call_append_any_wrap(
				op, bson_container, handler_bson_ptr_getter_or_size, opts,
				boost::addressof(reply), perr);
	}

	template<typename Op,
				typename Bson_Container, typename Handler_BsonPtr_Getter_Or_Size,
				typename BsonOpts> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::or_
				<
					boost::is_pointer<Bson_Container>,
					container_ex::is_container<Bson_Container>
				>,
				boost::mpl::or_
				<
					typename func::foo_t_info<Handler_BsonPtr_Getter_Or_Size>::is_callable_type,
					boost::is_integral<Handler_BsonPtr_Getter_Or_Size>
				>
			>,
			bool
		>::type
		call_append_any(Op& op,
						const Bson_Container& bson_container,
						const Handler_BsonPtr_Getter_Or_Size& handler_bson_ptr_getter_or_size,
						const BsonOpts& opts,
						org_bson_type* preply,
						org_bson_error_type& err) const
	{
		return
			this_type::call_append_any_wrap(
				op, bson_container, handler_bson_ptr_getter_or_size, opts,
				preply, boost::addressof(err));
	}

	template<typename Op,
				typename Bson_Container, typename Handler_BsonPtr_Getter_Or_Size,
				typename BsonOpts> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::or_
				<
					boost::is_pointer<Bson_Container>,
					container_ex::is_container<Bson_Container>
				>,
				boost::mpl::or_
				<
					typename func::foo_t_info<Handler_BsonPtr_Getter_Or_Size>::is_callable_type,
					boost::is_integral<Handler_BsonPtr_Getter_Or_Size>
				>
			>,
			bool
		>::type
		call_append_any(Op& op,
						const Bson_Container& bson_container,
						const Handler_BsonPtr_Getter_Or_Size& handler_bson_ptr_getter_or_size,
						const BsonOpts& opts,
						org_bson_type& reply,
						org_bson_error_type& err) const
	{
		return
			this_type::call_append_any_wrap(
				op, bson_container, handler_bson_ptr_getter_or_size, opts,
				boost::addressof(reply), boost::addressof(err));
	}
public:
	template<typename Op, typename LogData> inline
	bool call_append_any(Op& op, const LogData& log_data) const
	{
		bson_type opts;

		try
		{
			opts = _append_opts.load();
		}
		catch(const safe_cont_err_type&)
		{
			return false;
		}

		return
			this_type::call_append_any_wrap(
				op, log_data, opts,
				0, 0);
	}

	template<typename Op, typename Bson_Container, typename Handler_BsonPtr_Getter_Or_Size> inline
	bool call_append_any(Op& op,
							const Bson_Container& bson_container,
							const Handler_BsonPtr_Getter_Or_Size& handler_bson_ptr_getter_or_size) const
	{
		bson_type opts;

		try
		{
			opts = _append_opts.load();
		}
		catch(const safe_cont_err_type&)
		{
			return false;
		}

		return
			this_type::call_append_any_wrap(
				op, bson_container, handler_bson_ptr_getter_or_size, opts,
				0, 0);
	}


	// call_clear
protected:
	template<typename Op, typename Cmd, typename BsonOpts> inline
	bool call_clear_wrap(Op& op,
							u32 flags,
							const Cmd& cmd,
							const BsonOpts& opts,
							org_bson_type* preply,
							org_bson_error_type* perr) const
	{
		inner_string_type str_db;
		inner_string_type str_coln;

		try
		{
			str_db = _str_db.load();
			str_coln = _str_coln.load();
		}
		catch(const safe_cont_err_type&)
		{
			return false;
		}

		return
			op.clear(
				str_db, str_coln,
				flags, cmd, opts,
				preply, perr);
	}
public:
	template<typename Op, typename Cmd, typename BsonOpts> inline
	bool call_clear(Op& op,
					u32 flags,
					const Cmd& cmd,
					const BsonOpts& opts,
					org_bson_type* preply,
					org_bson_error_type* perr = 0) const
	{
		return
			this_type::call_clear_wrap(
				op,
				flags, cmd, opts,
				preply, perr);
	}

	template<typename Op, typename Cmd, typename BsonOpts> inline
	bool call_clear(Op& op,
					u32 flags,
					const Cmd& cmd,
					const BsonOpts& opts,
					org_bson_type& reply,
					org_bson_error_type* perr = 0) const
	{
		return
			this_type::call_clear_wrap(
				op,
				flags, cmd, opts,
				boost::addressof(reply), perr);
	}

	template<typename Op, typename Cmd, typename BsonOpts> inline
	bool call_clear(Op& op,
					u32 flags,
					const Cmd& cmd,
					const BsonOpts& opts,
					org_bson_type* preply,
					org_bson_error_type& err) const
	{
		return
			this_type::call_clear_wrap(
				op,
				flags, cmd, opts,
				preply, boost::addressof(err));
	}

	template<typename Op, typename Cmd, typename BsonOpts> inline
	bool call_clear(Op& op,
					u32 flags,
					const Cmd& cmd,
					const BsonOpts& opts,
					org_bson_type& reply,
					org_bson_error_type& err) const
	{
		return
			this_type::call_clear_wrap(
				op,
				flags, cmd, opts,
				boost::addressof(reply), boost::addressof(err));
	}

public:
	template<typename Op, typename Cmd> inline
	bool call_clear(Op& op, const Cmd& cmd) const
	{
		bson_type opts;
		u32 flags = 0;

		try
		{
			opts = _clear_opts.load();
			flags = _clear_flags.load();
		}
		catch(const safe_cont_err_type&)
		{
			return false;
		}

		return
			this_type::call_clear_wrap(
				op,
				flags, cmd, opts,
				0, 0);
	}

	// call_clear_any
#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES
	template< typename Op, typename ...Args > inline
	bool call_clear_any( Op& op, BOOST_FWD_REF(Args)... args) const
	{
		return op.clear_any(boost::forward<Args>(args)...);
	}
#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		template< typename Op \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		bool call_clear_any( Op& op \
								YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const { \
			return op.clear_any( YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	// call_search
protected:
	template<typename Op,
				typename Cmd,
				typename BsonOpts, typename ReadPrefs,
				typename HandlerCollector> inline
	bool call_search_wrap(Op& op,
							const Cmd& cmd,
							const BsonOpts& opts,
							const ReadPrefs& read_prefs,
							org_bson_error_type* perr,
							const HandlerCollector& handler_collector) const
	{
		inner_string_type str_db;
		inner_string_type str_coln;

		try
		{
			str_db = _str_db.load();
			str_coln = _str_coln.load();
		}
		catch(const safe_cont_err_type&)
		{
			return false;
		}

		return
			op.search(
				str_db, str_coln,
				cmd, opts, read_prefs,
				perr,
				handler_collector );
	}

public:
	template<typename Op,
				typename Cmd,
				typename BsonOpts, typename ReadPrefs,
				typename HandlerCollector> inline
	bool call_search(Op& op,
						const Cmd& cmd,
						const BsonOpts& opts,
						const ReadPrefs& read_prefs,
						org_bson_error_type* perr,
						const HandlerCollector& handler_collector) const
	{
		return this_type::call_search_wrap(op, cmd, opts, read_prefs, perr, handler_collector);
	}

	template<typename Op,
				typename Cmd,
				typename BsonOpts, typename ReadPrefs,
				typename HandlerCollector> inline
	bool call_search(Op& op,
						const Cmd& cmd,
						const BsonOpts& opts,
						const ReadPrefs& read_prefs,
						org_bson_error_type& err,
						const HandlerCollector& handler_collector) const
	{
		return this_type::call_search_wrap(op, cmd, opts, read_prefs, boost::addressof(err), handler_collector);
	}

	template<typename Op,
				typename Cmd,
				typename BsonOpts,
				typename HandlerCollector> inline
	bool call_search(Op& op,
						const Cmd& cmd,
						const BsonOpts& opts,
						const HandlerCollector& handler_collector) const
	{
		read_prefs_type read_prefs;

		try
		{
			read_prefs = _search_read_prefs.load();
		}
		catch(const safe_cont_err_type&)
		{
			return false;
		}

		return this_type::call_search_wrap(op, cmd, opts, read_prefs, 0, handler_collector);
	}

	template<typename Op, typename Cmd, typename HandlerCollector> inline
	bool call_search(Op& op,
						const Cmd& cmd,
						const HandlerCollector& handler_collector) const
	{
		read_prefs_type read_prefs;
		bson_type opts;

		try
		{
			read_prefs = _search_read_prefs.load();
			opts = _search_opts.load();
		}
		catch(const safe_cont_err_type&)
		{
			return false;
		}

		return this_type::call_search_wrap(op, cmd, opts, read_prefs, 0, handler_collector);
	}

	// call_search_any

protected:
	template<typename Op,
				typename Cmd,
				typename BsonOpts, typename ReadPrefs,
				typename HandlerCollector> inline
	bool call_search_any_wrap(Op& op,
								const Cmd& cmd,
								const BsonOpts& opts,
								const ReadPrefs& read_prefs,
								org_bson_error_type* perr,
								const HandlerCollector& handler_collector) const
	{
		inner_string_type str_db;
		inner_string_type str_coln;

		try
		{
			str_db = _str_db.load();
			str_coln = _str_coln.load();
		}
		catch(const safe_cont_err_type&)
		{
			return false;
		}

		return
			op.search_any(
				str_db, str_coln,
				cmd, opts, read_prefs,
				perr,
				handler_collector );
	}

public:
	template<typename Op,
				typename Cmd,
				typename BsonOpts, typename ReadPrefs,
				typename HandlerCollector> inline
	bool call_search_any(Op& op,
							const Cmd& cmd,
							const BsonOpts& opts,
							const ReadPrefs& read_prefs,
							org_bson_error_type* perr,
							const HandlerCollector& handler_collector) const
	{
		return this_type::call_search_any_wrap(op, cmd, opts, read_prefs, perr, handler_collector);
	}

	template<typename Op,
				typename Cmd,
				typename BsonOpts, typename ReadPrefs,
				typename HandlerCollector> inline
	bool call_search_any(Op& op,
							const Cmd& cmd,
							const BsonOpts& opts,
							const ReadPrefs& read_prefs,
							org_bson_error_type& err,
							const HandlerCollector& handler_collector) const
	{
		return this_type::call_search_any_wrap(op, cmd, opts, read_prefs, boost::addressof(err), handler_collector);
	}

	template<typename Op,
				typename Cmd,
				typename BsonOpts,
				typename HandlerCollector> inline
	bool call_search_any(Op& op,
							const Cmd& cmd,
							const BsonOpts& opts,
							const HandlerCollector& handler_collector) const
	{
		read_prefs_type read_prefs;

		try
		{
			read_prefs = _search_read_prefs.load();
		}
		catch(const safe_cont_err_type&)
		{
			return false;
		}

		return this_type::call_search_any_wrap(op, cmd, opts, read_prefs, 0, handler_collector);
	}

	template<typename Op, typename Cmd, typename HandlerCollector> inline
	bool call_search_any(Op& op,
							const Cmd& cmd,
							const HandlerCollector& handler_collector) const
	{
		read_prefs_type read_prefs;
		bson_type opts;

		try
		{
			read_prefs = _search_read_prefs.load();
			opts = _search_opts.load();
		}
		catch(const safe_cont_err_type&)
		{
			return false;
		}

		return this_type::call_search_any_wrap(op, cmd, opts, read_prefs, 0, handler_collector);
	}

private:
	inline static const char* unknown_log_name(void)
	{
		return "unknown_log.log_test";
	}

	inline static const char* unknown_log_db_name(void)
	{
		return "unknown_log";
	}

	inline static const char* unknown_log_coln_name(void)
	{
		return "log_test";
	}

private:
	inner_string_wrap_type _str_db;
	inner_string_wrap_type _str_coln;
	inner_string_wrap_type _str_db_coln;

	bson_wrap_type _append_opts;

	bson_wrap_type _search_opts;
	read_prefs_wrap_type _search_read_prefs;

	flag_wrap_type _clear_flags;
	bson_wrap_type _clear_opts;
};

// non_member_foo

// operator==
inline bool operator==(const log_mongodb_op_caller::id_type& l,
						const log_mongodb_op_caller& r)
{
	return l == r.id();
}

inline bool operator==(const log_mongodb_op_caller& l,
						const log_mongodb_op_caller::id_type& r)
{
	return l.id() == r;
}

inline bool operator==(const log_mongodb_op_caller& l,
						const log_mongodb_op_caller& r)
{
	return (boost::addressof(l) == boost::addressof(r)) || (l.id() == r.id());
}

// operator!=
inline bool operator!=(const log_mongodb_op_caller::id_type& l,
						const log_mongodb_op_caller& r)
{
	return !(l == r.id());
}

inline bool operator!=(const log_mongodb_op_caller& l,
						const log_mongodb_op_caller::id_type& r)
{
	return !(l.id() == r);
}

inline bool operator!=(const log_mongodb_op_caller& l,
						const log_mongodb_op_caller& r)
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
	YGGR_PP_INLINE_SWAP_THIS_REF_DEF_IMPL(log_mongodb_op_caller)
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

#endif // __YGGR_LOG_LOG_MONGODB_OP_CALLER_HPP__

