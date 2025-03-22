//log_mongodb_op.hpp

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

#ifndef __YGGR_LOG_LOGOP_LOG_MONGODB_OP_HPP__
#define __YGGR_LOG_LOGOP_LOG_MONGODB_OP_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>

#include <yggr/ppex/foo_params.hpp>
#include <yggr/ppex/symbol_if.hpp>
#include <yggr/ppex/symbols.hpp>

#include <yggr/type_traits/native_t.hpp>

#include <yggr/func/foo_t_info.hpp>
#include <yggr/container_ex/is_container.hpp>

#include <yggr/nonable/noncopyable.hpp>
#include <yggr/nonable/nonmoveable.hpp>

#include <yggr/tuple/tuple.hpp>

#include <yggr/nsql_database_system/c_mongo_connection.hpp>
#include <yggr/nsql_database_system/mongo_accesser.hpp>

#include <yggr/archive/bson_archive_partner.hpp>
#include <yggr/packet/packet.hpp>

#include <boost/utility/enable_if.hpp>

#include <boost/range/value_type.hpp>

#include <boost/preprocessor/comparison/equal.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>

#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_pointer.hpp>

namespace yggr
{
namespace log
{
namespace detail
{

template<typename Container, bool is_org_param>
struct bson_data_conv_helper;

template<typename Container>
struct bson_data_conv_helper<Container, true>
{
	typedef const Container& params_type;
	typedef const Container& result_type;

	inline result_type operator()(params_type t) const
	{
		return t;
	}
};

template<typename Container>
struct bson_data_conv_helper<Container, false>
{
	typedef const Container& params_type;
	typedef nsql_database_system::mongo_accesser::bson_type result_type;
	typedef packet::packet<archive::archive_partner::bson_oarchive_partner> opak_type;

	inline result_type operator()(params_type t) const
	{
		opak_type opak;
		opak.save(t);

		return opak.org_buf();
	}
};

//bson_cmd_conv_helper
template<typename Data, bool is_org_param>
struct bson_cmd_conv_helper;

template<typename Data>
struct bson_cmd_conv_helper<Data, true>
{
	typedef const Data& params_type;
	typedef const Data& result_type;

	inline result_type operator()(params_type t) const
	{
		return t;
	}
};

template<typename Data>
struct bson_cmd_conv_helper<Data, false>
{
	typedef const Data& params_type;
	typedef nsql_database_system::mongo_accesser::bson_type result_type;
	typedef packet::packet<archive::archive_partner::bson_nld_oarchive_partner> cmd_pak_type;

	inline result_type operator()(params_type t) const
	{
		cmd_pak_type opak;
		opak.save(t);

		return opak.org_buf();
	}
};

template<typename Container,
			bool is_cont = container_ex::is_container<Container>::value>
struct append_check_helper;

template<typename Container>
struct append_check_helper<Container, true>
{
	typedef nsql_database_system::mongo_accesser::org_bson_type org_bson_type;

	typedef Container cont_type;
	typedef typename boost::range_value<cont_type>::type data_type;
	typedef typename boost::remove_cv<typename boost::remove_reference<data_type>::type >::type chk_data_type;
	typedef typename boost::is_base_of<org_bson_type, chk_data_type>::type chk_type;

	typedef bson_data_conv_helper<cont_type, chk_type::value> h_type;
	typedef typename h_type::params_type params_type;
	typedef typename h_type::result_type result_type;

	inline result_type operator()(params_type t) const
	{
		h_type h;
		return h(t);
	}
};

template<typename Container>
struct append_check_helper<Container, false>
{
	typedef nsql_database_system::mongo_accesser::org_bson_type org_bson_type;

	typedef Container data_type;
	typedef typename boost::remove_cv<typename boost::remove_reference<data_type>::type >::type chk_data_type;
	typedef typename boost::is_base_of<org_bson_type, chk_data_type>::type chk_type;

	typedef bson_data_conv_helper<data_type, chk_type::value> h_type;
	typedef typename h_type::params_type params_type;
	typedef typename h_type::result_type result_type;

	inline result_type operator()(params_type t) const
	{
		h_type h;
		return h(t);
	}
};

//private:
template<typename Helper, std::size_t n>
struct append_call_helper;

template<typename Helper>
struct append_call_helper<Helper, static_cast<std::size_t>(6) > 
{
	typedef Helper helper_type;

	template<typename Conn, typename Tuple> inline
	bool operator()(Conn& conn, Tuple& tuple) const 
	{
		helper_type h;
		return 
			nsql_database_system::mongo_accesser::s_collection_insert(
				conn,
				yggr::get<0>(tuple),
				yggr::get<1>(tuple),
				h(yggr::get<2>(tuple)),
				yggr::get<3>(tuple),
				yggr::get<4>(tuple),
				yggr::get<5>(tuple) ); 
	}
};

template<typename Helper, std::size_t n>
struct clear_call_helper;

template<typename Helper>
struct clear_call_helper<Helper, std::size_t(7)> 
{
	typedef Helper helper_type;
		
	template<typename Conn, typename Tuple> inline
	bool operator()(Conn& conn, Tuple& tuple) const 
	{
		helper_type h;
		return 
			nsql_database_system::mongo_accesser::s_collection_remove(
				conn,
				yggr::get<0>(tuple),
				yggr::get<1>(tuple),
				yggr::get<2>(tuple),
				h(yggr::get<3>(tuple)),
				yggr::get<4>(tuple),
				yggr::get<5>(tuple),
				yggr::get<6>(tuple) ); 
	}
};

template<typename Helper, std::size_t n>
struct search_call_helper;

template<typename Helper>
struct search_call_helper<Helper, std::size_t(7)> 
{
	typedef Helper helper_type;

	template<typename Conn, typename Tuple> inline
	bool operator()(Conn& conn, Tuple& tuple) const 
	{
		helper_type h;
		return 
			nsql_database_system::mongo_accesser::s_collection_find(
				conn,
				yggr::get<0>(tuple),
				yggr::get<1>(tuple),
				h(yggr::get<2>(tuple)),
				yggr::get<3>(tuple),
				yggr::get<4>(tuple),
				yggr::get<5>(tuple),
				yggr::get<6>(tuple) ); 
	}
};

} // namespace detail
} // namespace log
} // namespace yggr

namespace yggr
{
namespace log
{

template<typename Def, typename Connection>
class log_mongodb_op 
	: private nonable::noncopyable,
		private nonable::nonmoveable
{
public:
	typedef Def def_type;
	YGGR_STATIC_CONSTANT(u32, E_LOG_OP_TYPE = def_type::E_LOG_OP_NSQL_DB);

	typedef typename def_type::id_type id_type;

	typedef Connection conn_type;

	typedef typename conn_type::init_type init_type;

	typedef nsql_database_system::mongo_accesser real_op_type;

	typedef packet::packet<archive::archive_partner::bson_oarchive_partner> opak_type;
	typedef packet::packet<archive::archive_partner::bson_iarchive_partner> ipak_type;

	typedef packet::packet<archive::archive_partner::bson_nld_oarchive_partner> cmd_pak_type;

private:
	typedef real_op_type::bson_type bson_type;
	typedef real_op_type::org_bson_type org_bson_type;
	
	typedef real_op_type::bson_error_type bson_error_type;
	typedef real_op_type::org_bson_error_type org_bson_error_type;

	typedef real_op_type::inner_string_type inner_string_type;
	typedef real_op_type::inner_string_view_type inner_string_view_type;
	
private:
	typedef log_mongodb_op this_type;

public:

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template< typename ...Args >
	log_mongodb_op(BOOST_FWD_REF(Args)... args) 
		: _conn(boost::forward<Args>(args)...)
	{ 
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		log_mongodb_op( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) \
			: _conn(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )){ }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	~log_mongodb_op(void)
	{
		_conn.clear();
	}

public:
#	include <yggr/log/detail/log_op_general_foo.ipp>

	// append
protected:
	template<typename String1, typename String2,
				typename LogData, typename BsonOpts> inline
	bool append_wrap(const String1& str_db, 
						const String2& str_coln,
						const LogData& log_data, 
						const BsonOpts& opts,
						org_bson_type* preply,
						org_bson_error_type* perr)
	{
		typedef LogData log_data_type;
		typedef BsonOpts bson_opts_type;

		typedef String1 string1_type;
		typedef String2 string2_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);

		typedef 
			yggr::tuple
			<
				const inner_string_type&, 
				const inner_string_type&,
				const log_data_type&,
				const bson_opts_type&,
				org_bson_type*,
				org_bson_error_type*
			> tuple_type;

		typedef typename native_t<log_data_type>::type data_type;
		typedef detail::append_check_helper<log_data_type> ah_type;

		typedef
			detail::append_call_helper
			<
				ah_type,
				boost::tuples::length<tuple_type>::value
			> ach_type;

		utf8_string1_type utf8_str_db(charset::string_charset_helper_data(str_db));
		utf8_string2_type utf8_str_coln(charset::string_charset_helper_data(str_coln));

		tuple_type tuple(yggr::make_tuple(
							boost::cref(utf8_str_db), boost::cref(utf8_str_coln), 
							boost::cref(log_data), boost::cref(opts), preply, perr));
		ach_type ach;

		return ach(_conn, tuple);
	}
public:
	// 6 args 
	template<typename String1, typename String2,
				typename LogData, typename BsonOpts> inline
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
		append(const String1& str_db, 
				const String2& str_coln,
				const LogData& log_data, 
				const BsonOpts& opts,
				org_bson_type* preply,
				org_bson_error_type* perr = 0)
	{
		return this_type::append_wrap(str_db, str_coln, log_data, opts, preply, perr);
	}

	template<typename String1, typename String2,
				typename LogData, typename BsonOpts> inline
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
		append(const String1& str_db, 
				const String2& str_coln,
				const LogData& log_data, 
				const BsonOpts& opts,
				org_bson_type& reply,
				org_bson_error_type* perr = 0)
	{
		return this_type::append_wrap(str_db, str_coln, log_data, opts, boost::addressof(reply), perr);
	}

	template<typename String1, typename String2,
				typename LogData, typename BsonOpts> inline
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
		append(const String1& str_db, 
				const String2& str_coln,
				const LogData& log_data, 
				const BsonOpts& opts,
				org_bson_type* preply,
				org_bson_error_type& err)
	{
		return this_type::append_wrap(str_db, str_coln, log_data, opts, preply, boost::addressof(err));
	}

	template<typename String1, typename String2,
				typename LogData, typename BsonOpts> inline
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
		append(const String1& str_db, 
				const String2& str_coln,
				const LogData& log_data, 
				const BsonOpts& opts,
				org_bson_type& reply,
				org_bson_error_type& err)
	{
		return this_type::append_wrap(str_db, str_coln, log_data, opts, boost::addressof(reply), boost::addressof(err));
	}

public:
	// 7 args
	template<typename String1, typename String2,
				typename Bson_Container, typename Handler_BsonPtr_Getter_Or_Size,
				typename BsonOpts> inline
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
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
				>
			>,
			bool
		>::type
		append(const String1& str_db, 
				const String2& str_coln,
				const Bson_Container& bson_container, 
				const Handler_BsonPtr_Getter_Or_Size& handler_bson_ptr_getter_or_size,
				const BsonOpts& opts,
				org_bson_type* preply,
				org_bson_error_type* perr = 0)
	{
		return 
			real_op_type::s_collection_insert(
				_conn, 
				str_db, str_coln, 
				bson_container, 
				handler_bson_ptr_getter_or_size, 
				opts,
				preply,
				perr);
	}

	template<typename String1, typename String2,
				typename Bson_Container, typename Handler_BsonPtr_Getter_Or_Size,
				typename BsonOpts> inline
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
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
				>
			>,
			bool
		>::type
		append(const String1& str_db, 
				const String2& str_coln,
				const Bson_Container& bson_container, 
				const Handler_BsonPtr_Getter_Or_Size& handler_bson_ptr_getter_or_size,
				const BsonOpts& opts,
				org_bson_type& reply,
				org_bson_error_type* perr = 0)
	{
		return 
			real_op_type::s_collection_insert(
				_conn, 
				str_db, str_coln, 
				bson_container, 
				handler_bson_ptr_getter_or_size, 
				opts,
				reply,
				perr);
	}

	template<typename String1, typename String2,
				typename Bson_Container, typename Handler_BsonPtr_Getter_Or_Size,
				typename BsonOpts> inline
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
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
				>
			>,
			bool
		>::type
		append(const String1& str_db, 
				const String2& str_coln,
				const Bson_Container& bson_container, 
				const Handler_BsonPtr_Getter_Or_Size& handler_bson_ptr_getter_or_size,
				const BsonOpts& opts,
				org_bson_type* preply,
				org_bson_error_type& err)
	{
		return 
			real_op_type::s_collection_insert(
				_conn, 
				str_db, str_coln, 
				bson_container, 
				handler_bson_ptr_getter_or_size, 
				opts,
				preply,
				err);
	}

	template<typename String1, typename String2,
				typename Bson_Container, typename Handler_BsonPtr_Getter_Or_Size,
				typename BsonOpts> inline
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
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
				>
			>,
			bool
		>::type
		append(const String1& str_db, 
				const String2& str_coln,
				const Bson_Container& bson_container, 
				const Handler_BsonPtr_Getter_Or_Size& handler_bson_ptr_getter_or_size,
				const BsonOpts& opts,
				org_bson_type& reply,
				org_bson_error_type& err)
	{
		return 
			real_op_type::s_collection_insert(
				_conn, 
				str_db, str_coln, 
				bson_container, 
				handler_bson_ptr_getter_or_size, 
				opts,
				reply,
				err);
	}

	// append_any
	// 6 args
	template<typename Arg1, typename Arg2, typename Arg3, typename Arg4> inline
	typename
		boost::enable_if
		<
			boost::mpl::or_
			<
				boost::is_integral<Arg4>,
				boost::is_base_of<bson_t, typename native_t<Arg4>::type >
			>,
			bool
		>::type
		append_any(const Arg1& arg1, const Arg2& arg2,
					const Arg3& arg3, const Arg4& arg4,
					org_bson_type* preply,
					org_bson_error_type* perr = 0)
	{
		return this_type::append(arg1, arg2, arg3, arg4, preply, perr);
	}

	template<typename Arg1, typename Arg2, typename Arg3, typename Arg4> inline
	typename
		boost::enable_if
		<
			boost::mpl::or_
			<
				boost::is_integral<Arg4>,
				boost::is_base_of<bson_t, typename native_t<Arg4>::type >
			>,
			bool
		>::type
		append_any(const Arg1& arg1, const Arg2& arg2,
					const Arg3& arg3, const Arg4& arg4,
					org_bson_type& reply,
					org_bson_error_type* perr = 0)
	{
		return this_type::append(arg1, arg2, arg3, arg4, reply, perr);
	}

	template<typename Arg1, typename Arg2, typename Arg3, typename Arg4> inline
	typename
		boost::enable_if
		<
			boost::mpl::or_
			<
				boost::is_integral<Arg4>,
				boost::is_base_of<bson_t, typename native_t<Arg4>::type >
			>,
			bool
		>::type
		append_any(const Arg1& arg1, const Arg2& arg2,
					const Arg3& arg3, const Arg4& arg4,
					org_bson_type* preply,
					org_bson_error_type& err)
	{
		return this_type::append(arg1, arg2, arg3, arg4, preply, err);
	}

	template<typename Arg1, typename Arg2, typename Arg3, typename Arg4> inline
	typename
		boost::enable_if
		<
			boost::mpl::or_
			<
				boost::is_integral<Arg4>,
				boost::is_base_of<bson_t, typename native_t<Arg4>::type >
			>,
			bool
		>::type
		append_any(const Arg1& arg1, const Arg2& arg2,
					const Arg3& arg3, const Arg4& arg4,
					org_bson_type& reply,
					org_bson_error_type& err)
	{
		return this_type::append(arg1, arg2, arg3, arg4, reply, err);
	}

	// 7 args
	template<typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::or_
				<
					boost::is_pointer<Arg3>,
					container_ex::is_container<Arg3>
				>,
				boost::mpl::or_
				<
					typename func::foo_t_info<Arg4>::is_callable_type,
					boost::is_integral<Arg4>
				>
			>,
			bool
		>::type
		append_any(const Arg1& arg1, const Arg2& arg2,
					const Arg3& arg3, const Arg4& arg4,
					const Arg5& arg5,
					org_bson_type* preply,
					org_bson_error_type* perr = 0)
	{
		return this_type::append(arg1, arg2, arg3, arg4, arg5, preply, perr);
	}

	template<typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::or_
				<
					boost::is_pointer<Arg3>,
					container_ex::is_container<Arg3>
				>,
				boost::mpl::or_
				<
					typename func::foo_t_info<Arg4>::is_callable_type,
					boost::is_integral<Arg4>
				>
			>,
			bool
		>::type
		append_any(const Arg1& arg1, const Arg2& arg2,
					const Arg3& arg3, const Arg4& arg4,
					const Arg5& arg5, 
					org_bson_type& reply,
					org_bson_error_type* perr = 0)
	{
		return this_type::append(arg1, arg2, arg3, arg4, arg5, reply, perr);
	}

	template<typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::or_
				<
					boost::is_pointer<Arg3>,
					container_ex::is_container<Arg3>
				>,
				boost::mpl::or_
				<
					typename func::foo_t_info<Arg4>::is_callable_type,
					boost::is_integral<Arg4>
				>
			>,
			bool
		>::type
		append_any(const Arg1& arg1, const Arg2& arg2,
					const Arg3& arg3, const Arg4& arg4,
					const Arg5& arg5,
					org_bson_type* preply,
					org_bson_error_type& err)
	{
		return this_type::append(arg1, arg2, arg3, arg4, arg5, preply, err);
	}

	template<typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::or_
				<
					boost::is_pointer<Arg3>,
					container_ex::is_container<Arg3>
				>,
				boost::mpl::or_
				<
					typename func::foo_t_info<Arg4>::is_callable_type,
					boost::is_integral<Arg4>
				>
			>,
			bool
		>::type
		append_any(const Arg1& arg1, const Arg2& arg2,
					const Arg3& arg3, const Arg4& arg4,
					const Arg5& arg5,
					org_bson_type& reply,
					org_bson_error_type& err)
	{
		return this_type::append(arg1, arg2, arg3, arg4, arg5, reply, err);
	}

	// clear
protected:
	template<typename String1, typename String2,
				typename Cmd, typename BsonOpts> inline
	bool clear_wrap(const String1& str_db, 
					const String2& str_coln, 
					u32 flags,
					const Cmd& cmd, 
					const BsonOpts& opts,
					org_bson_type* preply,
					org_bson_error_type* perr)
	{
		typedef Cmd cmd_type;
		typedef BsonOpts bson_opts_type;

		typedef String1 string1_type;
		typedef String2 string2_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);

		typedef 
			yggr::tuple
			<
				const inner_string_type&, 
				const inner_string_type&,
				u32,
				const cmd_type&,
				const bson_opts_type&,
				org_bson_type*,
				org_bson_error_type*
			> tuple_type;

		typedef typename native_t<cmd_type>::type data_type;

		typedef boost::is_base_of<org_bson_type, data_type> chk_type;
		typedef detail::bson_cmd_conv_helper<cmd_type, chk_type::value> bch_type;

		typedef 
			detail::clear_call_helper
			<
				bch_type,
				boost::tuples::length<tuple_type>::value
			>cch_type;

		utf8_string1_type utf8_str_db(charset::string_charset_helper_data(str_db));
		utf8_string2_type utf8_str_coln(charset::string_charset_helper_data(str_coln));

		tuple_type tuple(yggr::make_tuple(
							boost::cref(utf8_str_db), boost::cref(utf8_str_coln), 
							flags, boost::cref(cmd), boost::cref(opts), preply, perr));

		cch_type cch;

		return cch(_conn, tuple);
	}

public:
	template<typename String1, typename String2,
				typename Cmd, typename BsonOpts> inline
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
		clear(const String1& str_db, 
				const String2& str_coln, 
				u32 flags,
				const Cmd& cmd, 
				const BsonOpts& opts,
				org_bson_type* preply,
				org_bson_error_type* perr = 0)
	{
		return 
			this_type::clear_wrap(
				str_db, str_coln, flags, cmd, opts, 
				preply, perr);
	}

	template<typename String1, typename String2,
				typename Cmd, typename BsonOpts> inline
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
		clear(const String1& str_db, 
				const String2& str_coln, 
				u32 flags,
				const Cmd& cmd, 
				const BsonOpts& opts,
				org_bson_type& reply,
				org_bson_error_type* perr = 0)
	{
		return 
			this_type::clear_wrap(
				str_db, str_coln, flags, cmd, opts,
				boost::addressof(reply), perr);
	}

	template<typename String1, typename String2,
				typename Cmd, typename BsonOpts> inline
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
		clear(const String1& str_db, 
				const String2& str_coln, 
				u32 flags,
				const Cmd& cmd, 
				const BsonOpts& opts,
				org_bson_type* preply,
				org_bson_error_type& err)
	{
		return 
			this_type::clear_wrap(
				str_db, str_coln, flags, cmd, opts, 
				preply, boost::addressof(err));
	}

	template<typename String1, typename String2,
				typename Cmd, typename BsonOpts> inline
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
		clear(const String1& str_db, 
				const String2& str_coln, 
				u32 flags,
				const Cmd& cmd, 
				const BsonOpts& opts,
				org_bson_type& reply,
				org_bson_error_type& err)
	{
		return 
			this_type::clear_wrap(
				str_db, str_coln, flags, cmd, opts, 
				boost::addressof(reply), boost::addressof(err));
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
protected:
	template<typename String1, typename String2,
				typename Cmd,
				typename BsonOpts, typename ReadPrefs,
				typename HandlerCollector> inline
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
		search_wrap(const String1& str_db, 
					const String2& str_coln, 
					const Cmd& cmd, 
					const BsonOpts& opts,
					const ReadPrefs& read_prefs,
					org_bson_error_type* perr, 
					const HandlerCollector& handler_collector) const
	{
		typedef Cmd cmd_type;
		typedef BsonOpts bson_opts_type;
		typedef ReadPrefs read_prefs_type;
		typedef HandlerCollector handler_collector_type;

		typedef String1 string1_type;
		typedef String2 string2_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);

		typedef 
			yggr::tuple
			<
				const inner_string_type&, 
				const inner_string_type&,
				const cmd_type&,
				const bson_opts_type&, 
				const read_prefs_type&,
				org_bson_error_type*,
				const handler_collector_type&
			> tuple_type;

		typedef typename native_t<cmd_type>::type data_type;

		typedef boost::is_base_of<org_bson_type, data_type> chk_type;
		typedef detail::bson_cmd_conv_helper<cmd_type, chk_type::value> bch_type;

		typedef 
			detail::search_call_helper
			<
				bch_type,
				boost::tuples::length<tuple_type>::value
			> sch_type;

		utf8_string1_type utf8_str_db(charset::string_charset_helper_data(str_db));
		utf8_string2_type utf8_str_coln(charset::string_charset_helper_data(str_coln));

		tuple_type tuple(yggr::make_tuple(
							boost::cref(utf8_str_db), boost::cref(utf8_str_coln), 
							boost::cref(cmd), boost::cref(opts), boost::cref(read_prefs),
							perr, boost::cref(handler_collector) ) );

		sch_type sch;
		return sch(_conn, tuple);
	}
public:
	template<typename String1, typename String2,
				typename Cmd,
				typename BsonOpts, typename ReadPrefs,
				typename HandlerCollector> inline
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
		search(const String1& str_db, 
				const String2& str_coln, 
				const Cmd& cmd, 
				const BsonOpts& opts,
				const ReadPrefs& read_prefs,
				org_bson_error_type* perr, 
				const HandlerCollector& handler_collector) const
	{
		return 
			this_type::search_wrap(
				str_db, str_coln,
				cmd, opts, read_prefs,
				perr,
				handler_collector);
	}

	template<typename String1, typename String2,
				typename Cmd,
				typename BsonOpts, typename ReadPrefs,
				typename HandlerCollector> inline
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
		search(const String1& str_db, 
				const String2& str_coln, 
				const Cmd& cmd, 
				const BsonOpts& opts,
				const ReadPrefs& read_prefs,
				org_bson_error_type& err, 
				const HandlerCollector& handler_collector) const
	{
		return 
			this_type::search_wrap(
				str_db, str_coln,
				cmd, opts, read_prefs,
				boost::addressof(err),
				handler_collector);
	}

	// search_any
public:
	template<typename String1, typename String2,
				typename Cmd,
				typename BsonOpts, typename ReadPrefs,
				typename HandlerCollector> inline
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
		search_any(const String1& str_db, 
					const String2& str_coln, 
					const Cmd& cmd, 
					const BsonOpts& opts,
					const ReadPrefs& read_prefs,
					org_bson_error_type* perr, 
					const HandlerCollector& handler_collector) const
	{
		return 
			this_type::search_wrap(
				str_db, str_coln,
				cmd, opts, read_prefs, 
				perr, handler_collector);
	}

	template<typename String1, typename String2,
				typename Cmd,
				typename BsonOpts, typename ReadPrefs,
				typename HandlerCollector> inline
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
		search_any(const String1& str_db, 
					const String2& str_coln, 
					const Cmd& cmd, 
					const BsonOpts& opts,
					const ReadPrefs& read_prefs,
					org_bson_error_type& err, 
					const HandlerCollector& handler_collector) const
	{
		return 
			this_type::search_wrap(
				str_db, str_coln,
				cmd, opts, read_prefs,
				boost::addressof(err),
				handler_collector);
	}

private:
	mutable conn_type _conn;
};

} // namespace log
} // namespace yggr

#endif //__YGGR_LOG_LOGOP_LOG_MONGODB_OP_HPP__
