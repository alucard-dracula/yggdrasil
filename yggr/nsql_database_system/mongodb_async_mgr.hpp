//mongodb_async_mgr.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_MONGODB_ASYNC_MGR_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_MONGODB_ASYNC_MGR_HPP__

#include <yggr/dbs_manager/async_dbs_mgr.hpp>
#include <yggr/nsql_database_system/mongo_accesser.hpp>


namespace yggr
{
namespace nsql_database_system
{

// ---------------- mongodb_notify--------------------

using dbs_manager::basic_db_notify;
using dbs_manager::db_notify;

//---------------mongodb_async_mgr------------------

template<typename Key, 
			typename Conn, 
			typename Accesser = mongo_accesser,
			typename InUserData = yggr::any,
			typename OutUserData = yggr::any,
			typename Thread_Config = thread::boost_thread_config_type>
class mongodb_async_mgr
	: public dbs_manager::async_dbs_mgr<
				Key, Conn, Accesser,InUserData, OutUserData, Thread_Config>
{
public:
	typedef dbs_manager::async_dbs_mgr<
				Key, Conn, Accesser,InUserData, OutUserData, Thread_Config> base_type;
	
	typedef typename base_type::key_type key_type;
	typedef typename base_type::conn_type conn_type;
	typedef typename base_type::accesser_type accesser_type;
	typedef typename base_type::in_user_data_type in_user_data_type;
	typedef typename base_type::out_user_data_type out_user_data_type;

	typedef typename base_type::init_type init_type;

private:
	typedef mongodb_async_mgr this_type;

public:

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename ...Args>
	mongodb_async_mgr(BOOST_FWD_REF(Args)... args)
		: base_type(boost::forward<Args>(args)...)
	{
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) \
		mongodb_async_mgr( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) \
			: base_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )){ }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES 

	mongodb_async_mgr(const init_type& init)
		: base_type(init)
	{
	}

	~mongodb_async_mgr(void)
	{
	}

public:
	using base_type::init;
	using base_type::async_execute;
	using base_type::async_run_command;
	using base_type::start;
	using base_type::is_running;
	using base_type::stop;
	using base_type::join;
	using base_type::clear_notify;
	using base_type::clear;
};

} // namespace nsql_database_system
} // namespace yggr

#endif // __YGGR_NSQL_DATABASE_SYSTEM_MONGODB_ASYNC_MGR_HPP__
