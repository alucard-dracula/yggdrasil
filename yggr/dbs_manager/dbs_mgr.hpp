//dbs_mgr.hpp

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

#ifndef __YGGR_DBS_MANAGER_DBS_MGR_HPP__
#define __YGGR_DBS_MANAGER_DBS_MGR_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>

#include <yggr/nonable/noncopyable.hpp>
#include <yggr/nonable/nonmoveable.hpp>

#include <yggr/any_val/any.hpp>
#include <yggr/ppex/foo_params.hpp>

#include <yggr/safe_container/safe_unordered_map.hpp>
#include <yggr/safe_container/safe_wrap.hpp>

#include <yggr/dbs_manager/detail/db_cmd_object_check.hpp>

#include <boost/utility/enable_if.hpp>

namespace yggr
{
namespace dbs_manager
{
namespace detail
{

template<typename Conn, typename InUserData, typename OutUserData>
class dbs_mgr_basic_command
{
public:
	typedef Conn conn_type;
	typedef InUserData in_user_data_type;
	typedef OutUserData out_user_data_type;

private:
	typedef dbs_mgr_basic_command this_type;

public:
	virtual ~dbs_mgr_basic_command(void) 
	{
	}

public:
	virtual bool execute(conn_type& conn,
							const in_user_data_type& in_data,
							out_user_data_type& out_data) const = 0;
};

template<typename Conn, typename InUserData, typename OutUserData, typename Handler>
class dbs_mgr_command
	: public dbs_mgr_basic_command<Conn, InUserData, OutUserData>
{
public:
	typedef Conn conn_type;
	typedef InUserData in_user_data_type;
	typedef OutUserData out_user_data_type;
	typedef Handler handler_type;
	typedef dbs_mgr_basic_command<conn_type, in_user_data_type, out_user_data_type> base_type;

private:
	typedef dbs_mgr_command this_type;

public:
	dbs_mgr_command(const handler_type& handler)
		: _handler(handler)
	{
	}

	~dbs_mgr_command(void)
	{
	}

public:
	virtual bool execute(conn_type& conn,
						const in_user_data_type& in_data,
						out_user_data_type& out_data ) const
	{
		return (_handler)(conn, in_data, out_data);
	}

private:
	handler_type _handler;
};

} // namespace detail
} // namespace dbs_manager
} // namespace dbs_manager

namespace yggr
{
namespace dbs_manager
{

template<typename Key, 
			typename Conn, 
			typename Accesser,
			typename InUserData = yggr::any,
			typename OutUserData = yggr::any>
class dbs_mgr
	: private nonable::noncopyable,
		private nonable::nonmoveable
{
public:
	typedef Key key_type;
	typedef Conn conn_type;
	typedef typename conn_type::init_type init_type;
	typedef Accesser accesser_type;

	typedef InUserData in_user_data_type;
	typedef OutUserData out_user_data_type;

private:
	YGGR_ANY_ASSERT(in_user_data_type);
	YGGR_ANY_ASSERT(out_user_data_type);

private:
	typedef safe_container::safe_wrap<conn_type> wrap_conn_type;
	typedef typename wrap_conn_type::value_ptr_type conn_ptr_type;

private:
	typedef dbs_mgr this_type;

protected:
	typedef 
		detail::dbs_mgr_basic_command
		<
			conn_type, 
			in_user_data_type, 
			out_user_data_type
		> basic_command_type;

	typedef shared_ptr<basic_command_type> icmd_type;
	typedef safe_container::safe_unordered_map<key_type, icmd_type> cmd_map_type;

public:
	dbs_mgr(void)
	{
	}

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename EArg, typename ...Args>
	dbs_mgr(BOOST_FWD_REF(EArg) earg, BOOST_FWD_REF(Args)... args)
		: _conn_wrap(yggr_nothrow_new conn_type(earg, boost::forward<Args>(args)...))
	{
	}

#else
	
#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) \
		dbs_mgr( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) \
			: _conn_wrap( \
				yggr_nothrow_new conn_type( \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))){ }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES


	dbs_mgr(const init_type& init)
		: _conn_wrap(yggr_nothrow_new conn_type(init))
	{
	}

	~dbs_mgr(void)
	{
		this_type::clear();
		assert(_conn_wrap.empty());
		assert(_cmd_map.empty());
	}

public:

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename ...Args> inline
	bool init(BOOST_FWD_REF(Args)... args)
	{
		typedef typename wrap_conn_type::element_ptr_type now_element_ptr_type;

		if(!_conn_wrap.empty())
		{
			return false;
		}
		else
		{
			now_element_ptr_type old_ptr;
			now_element_ptr_type new_ptr(yggr_nothrow_new conn_type(boost::forward<Args>(args)...));
			return _conn_wrap.compare_exchange_strong(old_ptr, new_ptr);
		}
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		bool init( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			typedef typename wrap_conn_type::element_ptr_type now_element_ptr_type; \
			if(!_conn_wrap.empty()){ return false; } \
			else { \
				now_element_ptr_type old_ptr; \
				now_element_ptr_type new_ptr( \
					yggr_nothrow_new conn_type( \
						YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); \
				return _conn_wrap.compare_exchange_strong(old_ptr, new_ptr); } }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename Handler> inline
	bool register_command(const key_type& key, const Handler& handler)

	{
		typedef Handler handler_type;
		//typedef command<handler_type> now_command_type;
		typedef 
			detail::dbs_mgr_command
			<
				conn_type,
				in_user_data_type,
				out_user_data_type,
				handler_type
			> now_command_type;

		icmd_type pcmd = 
			construct_shared<basic_command_type>(
				yggr_nothrow_new now_command_type(handler) );
		return pcmd && _cmd_map.insert(key, pcmd);
	}

	template<typename T> inline
	typename 
		boost::enable_if
		<
			detail::has_any_foo_register_db_command<T>,
			bool
		>::type
		register_command(T& obj)
	{
		return obj.template register_db_command<this_type>(*this);
	}

	inline bool unregister_command(const key_type& key)
	{
		return !!_cmd_map.erase(key);
	}

	template<typename T> inline
	typename 
		boost::enable_if
		<
			detail::has_any_foo_unregister_db_command<T>,
			bool
		>::type
		unregister_command(T& obj)
	{
		return obj.template unregister_db_command<this_type>(*this);
	}

	inline bool execute(const key_type& key) const
	{
		conn_ptr_type pconn;
		icmd_type pcmd;
		in_user_data_type indata;
		out_user_data_type outdata;

		return 
			(pconn = _conn_wrap.get_shared_ptr())
			&& _cmd_map.get_value(key, pcmd)
			&& pcmd 
			&& pcmd->execute(*pconn, indata, outdata);
	}

	inline bool execute(const key_type& key, const in_user_data_type& indata) const
	{
		conn_ptr_type pconn;
		icmd_type pcmd;
		out_user_data_type outdata;

		return 
			(pconn = _conn_wrap.get_shared_ptr())
			&& _cmd_map.get_value(key, pcmd)
			&& pcmd 
			&& pcmd->execute(*pconn, indata, outdata);
	}

	inline bool execute(const key_type& key, const in_user_data_type& indata, out_user_data_type& outdata) const
	{
		conn_ptr_type pconn;
		icmd_type pcmd;

		return 
			(pconn = _conn_wrap.get_shared_ptr())
			&& _cmd_map.get_value(key, pcmd)
			&& pcmd
			&& pcmd->execute(*pconn, indata, outdata);
	}

	template<typename Handler> inline
	bool run_command(const Handler& handler) const
	{
		conn_ptr_type pconn;
		in_user_data_type indata;
		out_user_data_type outdata;

		return 
			(pconn = _conn_wrap.get_shared_ptr())
			&& handler(*pconn, indata, outdata);
	}

	template<typename Handler> inline
	bool run_command(const Handler& handler, const in_user_data_type& indata) const
	{
		conn_ptr_type pconn;
		out_user_data_type outdata;

		return 
			(pconn = _conn_wrap.get_shared_ptr())
			&& handler(*pconn, indata, outdata);
	}

	template<typename Handler> inline
	bool run_command(const Handler& handler,
						const in_user_data_type& indata,
						out_user_data_type& outdata) const
	{
		conn_ptr_type pconn;

		return 
			(pconn = _conn_wrap.get_shared_ptr())
			&& handler(*pconn, indata, outdata);
	}

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename ...Args> inline
	bool reconnect(BOOST_FWD_REF(Args)... args) 
	{ 
		{ 
			conn_ptr_type pconn = _conn_wrap.get_shared_ptr(); 
			if(pconn)
			{ 
				pconn->clear(); 
			}
		} 

		conn_ptr_type pconn = 
			construct_shared<conn_type>(
				yggr_nothrow_new conn_type(boost::forward<Args>(args)...)); 
		return 
			_conn_wrap.locked_unsafe_using_handler( 
				boost::bind(&this_type::pro_s_reset_connection, _1, boost::ref(pconn))); 
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		bool reconnect( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			{ conn_ptr_type pconn = _conn_wrap.get_shared_ptr(); \
			  if(pconn){ pconn->clear(); } } \
			conn_ptr_type pconn = \
				construct_shared<conn_type>( \
					yggr_nothrow_new conn_type( \
						YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); \
			return _conn_wrap.locked_unsafe_using_handler( \
						boost::bind(&this_type::pro_s_reset_connection, _1, boost::ref(pconn))); }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	inline bool reconnect(const init_type& init)
	{
		{
			conn_ptr_type pconn = _conn_wrap.get_shared_ptr();
			if(pconn)
			{
				pconn->clear();
			}
		}

		conn_ptr_type pconn = construct_shared<conn_type>(yggr_nothrow_new conn_type(init));
		return 
			_conn_wrap.locked_unsafe_using_handler(
				boost::bind(&this_type::pro_s_reset_connection, _1, boost::ref(pconn)));
	}

	inline void clear(void)
	{
		{
			conn_ptr_type pconn = _conn_wrap.get_shared_ptr();
			if(pconn)
			{
				pconn->clear();
			}
		}
		_conn_wrap.clear();
		_cmd_map.clear();
	}

protected:
	inline static bool pro_s_reset_connection(typename wrap_conn_type::base_type& base,
												conn_ptr_type& pconn)
	{
		return (pconn) && (base.swap(pconn), true);
	}

protected:
	wrap_conn_type _conn_wrap;
	cmd_map_type _cmd_map;
};

} // namespace dbs_manager
} // namespace yggr

#endif // __YGGR_DBS_MANAGER_DBS_MGR_HPP__
