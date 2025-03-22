//async_dbs_mgr.hpp

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

#ifndef __YGGR_DBS_MANAGER_ASYNC_DBS_MGR_HPP__
#define __YGGR_DBS_MANAGER_ASYNC_DBS_MGR_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/utility/swap.hpp>

#include <yggr/thread/boost_thread_config.hpp>
#include <yggr/thread/this_thread.hpp>

#include <yggr/dbs_manager/dbs_mgr.hpp>

#include <yggr/nonable/noncopyable.hpp>
#include <yggr/nonable/nonmoveable.hpp>

#include <yggr/smart_ptr_ex/shared_ptr.hpp>

#include <yggr/safe_container/safe_queue.hpp>

#include <boost/atomic.hpp>

namespace yggr
{
namespace dbs_manager
{

// ---------------- db_notify--------------------
template<typename Runner, 
			typename InUserData = yggr::any,
			typename OutUserData = yggr::any >
class basic_db_notify
	: private nonable::noncopyable,
		private nonable::nonmoveable
{
public:
	typedef Runner runner_type;

	typedef InUserData in_user_data_type;
	typedef OutUserData out_user_data_type;

private:
	YGGR_ANY_ASSERT(in_user_data_type);
	YGGR_ANY_ASSERT(out_user_data_type);

private:
	typedef basic_db_notify this_type;

public:
	basic_db_notify(runner_type& runner)
		: _runner(runner)
	{
	}

	basic_db_notify(runner_type& runner,
					const in_user_data_type& in_user_data)
		: _runner(runner),
			_in_user_data(in_user_data)
	{
	}

	virtual ~basic_db_notify(void)
	{
	}

public:
	virtual bool execute(void) = 0;

protected:
	runner_type& _runner;
	in_user_data_type _in_user_data;
	out_user_data_type _out_user_data;
};

// --------------db_notify----------------
template<typename Runner,
			typename Key, 
			typename Handler,
			typename InUserData = yggr::any,
			typename OutUserData = yggr::any >
class db_notify
	: public basic_db_notify<Runner, InUserData, OutUserData>
{
public:
	typedef basic_db_notify<Runner, InUserData, OutUserData> base_type;

	typedef Key key_type;
	typedef typename base_type::runner_type runner_type;
	typedef typename base_type::in_user_data_type in_user_data_type;
	typedef typename base_type::out_user_data_type out_user_data_type;

	typedef Handler handler_type;

private:
	YGGR_ANY_ASSERT(in_user_data_type);
	YGGR_ANY_ASSERT(out_user_data_type);

private:
	typedef db_notify this_type;

public:
	db_notify(runner_type& runner, 
				const key_type& key, 
				const handler_type& handler)
		: base_type(runner), 
			_key(key), 
			_handler(handler)
	{
	}

	db_notify(runner_type& runner, 
				const key_type& key, 
				const in_user_data_type& in_user_data,
				const handler_type& handler)
		: base_type(runner, in_user_data), 
			_key(key),
			_handler(handler)
	{
	}

	~db_notify(void)
	{
	}

public:
	virtual bool execute(void)
	{
		bool bret = base_type::_runner.execute(_key, base_type::_in_user_data, base_type::_out_user_data);
		// the _handler is owned currently db_notify, so don't copy it
		(_handler)(bret, base_type::_out_user_data); 
		return bret;
	}

protected:
	key_type _key;
	handler_type _handler;
};

template<typename Runner,
			typename Key, 
			typename InUserData,
			typename OutUserData>
class db_notify<Runner, Key, void, InUserData, OutUserData>
	: public basic_db_notify<Runner, InUserData, OutUserData>
{
public:
	typedef basic_db_notify<Runner, InUserData, OutUserData> base_type;

	typedef Key key_type;
	typedef typename base_type::runner_type runner_type;
	typedef typename base_type::in_user_data_type in_user_data_type;
	typedef typename base_type::out_user_data_type out_user_data_type;

private:
	YGGR_ANY_ASSERT(in_user_data_type);
	YGGR_ANY_ASSERT(out_user_data_type);

private:
	typedef db_notify this_type;

public:
	db_notify(runner_type& runner, const key_type& key)
		: base_type(runner), _key(key)
	{
	}

	db_notify(runner_type& runner, 
				const key_type& key, 
				const in_user_data_type& in_user_data)
		: base_type(runner, in_user_data), 
			_key(key)
	{
	}

	~db_notify(void)
	{
	}

public:
	virtual bool execute(void)
	{
		return base_type::_runner.execute(_key, base_type::_in_user_data, base_type::_out_user_data);
	}

protected:
	key_type _key;
};

//-------------db_cmd_notify----------------
template<typename Runner, 
			typename Handler1,
			typename Handler2,
			typename InUserData = yggr::any,
			typename OutUserData = yggr::any >
class db_cmd_notify
	: public basic_db_notify<Runner, InUserData, OutUserData>
{
public:
	typedef basic_db_notify<Runner, InUserData, OutUserData> base_type;
	
	typedef typename base_type::runner_type runner_type;
	typedef typename base_type::in_user_data_type in_user_data_type;
	typedef typename base_type::out_user_data_type out_user_data_type;

	typedef Handler1 cmd_handler_type;
	typedef Handler2 after_cmd_handler_type;

private:
	YGGR_ANY_ASSERT(in_user_data_type);
	YGGR_ANY_ASSERT(out_user_data_type);

private:
	typedef db_cmd_notify this_type;

public:
	db_cmd_notify(runner_type& runner,
					const cmd_handler_type& handler_cmd,
					const after_cmd_handler_type& handler_after_cmd)
		: base_type(runner),
			_handler_cmd(handler_cmd),
			_handler_after_cmd(handler_after_cmd)
	{
	}

	db_cmd_notify(runner_type& runner,
					const cmd_handler_type& handler_cmd,
					const in_user_data_type& in_user_data,
					const after_cmd_handler_type& handler_after_cmd )
		: base_type(runner, in_user_data),
			_handler_cmd(handler_cmd),
			_handler_after_cmd(handler_after_cmd)
	{
	}

	~db_cmd_notify(void)
	{
	}

public:
	virtual bool execute(void)
	{
		bool bret = base_type::_runner.run_command(_handler_cmd, base_type::_in_user_data, base_type::_out_user_data);
		(_handler_after_cmd)(bret, base_type::_out_user_data);
		return bret;
	}

protected:
	cmd_handler_type _handler_cmd;
	after_cmd_handler_type _handler_after_cmd;
};

template<typename Runner, 
			typename Handler1,
			typename InUserData,
			typename OutUserData >
class db_cmd_notify<Runner, Handler1, void, InUserData, OutUserData>
	: public basic_db_notify<Runner, InUserData, OutUserData>
{
public:
	typedef basic_db_notify<Runner, InUserData, OutUserData> base_type;
	
	typedef typename base_type::runner_type runner_type;
	typedef typename base_type::in_user_data_type in_user_data_type;
	typedef typename base_type::out_user_data_type out_user_data_type;

	typedef Handler1 cmd_handler_type;

private:
	YGGR_ANY_ASSERT(in_user_data_type);
	YGGR_ANY_ASSERT(out_user_data_type);

private:
	typedef db_cmd_notify this_type;

public:
	db_cmd_notify(runner_type& runner,
					const cmd_handler_type& handler_cmd)
		: base_type(runner), _handler_cmd(handler_cmd)
	{
	}

	db_cmd_notify(runner_type& runner,
					const in_user_data_type& in_user_data,
					const cmd_handler_type& handler_cmd)
		: base_type(runner, in_user_data),
			_handler_cmd(handler_cmd)
	{
	}

	~db_cmd_notify(void)
	{
	}

public:
	virtual bool execute(void)
	{
		return base_type::_runner.run_command(_handler_cmd, base_type::_in_user_data, base_type::_out_user_data);
	}

protected:
	cmd_handler_type _handler_cmd;
};

} // namespace dbs_manager
} // namespace yggr

//---------------async_dbs_mgr------------------

namespace yggr
{
namespace dbs_manager
{
template<typename Key, 
			typename Conn, 
			typename Accesser,
			typename InUserData = yggr::any,
			typename OutUserData = yggr::any,
			typename Thread_Config = thread::boost_thread_config_type >
class async_dbs_mgr
	: public dbs_mgr<Key, Conn, Accesser, InUserData, OutUserData>
{
public:
	typedef dbs_mgr<Key, Conn, Accesser, InUserData, OutUserData> base_type;
	
	typedef typename base_type::key_type key_type;
	typedef typename base_type::conn_type conn_type;
	typedef typename base_type::accesser_type accesser_type;
	typedef typename base_type::in_user_data_type in_user_data_type;
	typedef typename base_type::out_user_data_type out_user_data_type;

	typedef typename base_type::init_type init_type;

private:
	typedef Thread_Config thread_config_type;
	typedef typename thread_config_type::thread_type thread_type;
	typedef typename thread_config_type::thread_group_type thread_group_type;
	typedef typename thread_config_type::id_type thread_id_type;
	typedef typename thread_config_type::time_type time_type;

	typedef thread::this_thread<thread_type> this_thread_type;

	typedef boost::atomic<bool> run_state_type;

private:
	typedef 
		basic_db_notify
		<
			base_type, 
			in_user_data_type, 
			out_user_data_type
		> basic_notify_type;

	typedef shared_ptr<basic_notify_type> interface_notify_type;

	typedef safe_container::safe_queue<interface_notify_type> notify_queue_type;


private:
	YGGR_ANY_ASSERT(in_user_data_type);
	YGGR_ANY_ASSERT(out_user_data_type);

private:
	typedef async_dbs_mgr this_type;

public:

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename ...Args>
	async_dbs_mgr(BOOST_FWD_REF(Args)... args)
		: base_type(boost::forward<Args>(args)...),
			_run_state(false)
	{
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) \
		async_dbs_mgr( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) \
			: base_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )), \
				_run_state(false){ }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	async_dbs_mgr(const init_type& init)
		: base_type(init), _run_state(false)
	{
	}

	~async_dbs_mgr(void)
	{
		assert(!_run_state.load());
	}

public:

	using base_type::init;
	using base_type::register_command;
	using base_type::unregister_command;

public:
	inline bool async_execute(const key_type& key)
	{
		typedef 
			db_notify
			<
				base_type, 
				key_type, 
				void, 
				in_user_data_type, 
				out_user_data_type
			> now_notify_type;

		if(_run_state.load())
		{
			base_type& base = *this;
			interface_notify_type pnotify = 
				construct_shared<basic_notify_type>(
					yggr_nothrow_new now_notify_type(base, key) );
			return (pnotify) && (_notify_queue.push(pnotify), true);
		}
		else
		{
			return base_type::execute(key);
		}
	}

	template<typename Handler> inline
	bool async_execute(const key_type& key, const Handler& handler)
	{
		typedef Handler handler_type;
		typedef 
			db_notify
			< 
				base_type,
				key_type, 
				handler_type, 
				in_user_data_type, 
				out_user_data_type 
			> now_notify_type;

		if(_run_state.load())
		{
			base_type& base = *this;
			interface_notify_type pnotify = 
				construct_shared<basic_notify_type>(
					yggr_nothrow_new now_notify_type(base, key, handler) );
			return (pnotify) && (_notify_queue.push(pnotify), true);
		}
		else
		{
			return base_type::execute(key, handler);
		}
	}

	inline bool async_execute(const key_type& key, const in_user_data_type& in_user_data)
	{
		typedef 
			db_notify
			< 
				base_type,
				key_type, 
				void, 
				in_user_data_type, 
				out_user_data_type 
			> now_notify_type;

		if(_run_state.load())
		{
			base_type& base = *this;
			interface_notify_type pnotify = 
				construct_shared<basic_notify_type>(
					yggr_nothrow_new now_notify_type(base, key, in_user_data) );
			return (pnotify) && (_notify_queue.push(pnotify), true);
		}
		else
		{
			return base_type::execute(key, in_user_data);
		}
	}

	template<typename Handler> inline
	bool async_execute(const key_type& key,
						const in_user_data_type& in_user_data,
						const Handler& handler)
	{
		typedef Handler handler_type;

		typedef 
			db_notify
			< 
				base_type,
				key_type, 
				handler_type, 
				in_user_data_type, 
				out_user_data_type 
			> now_notify_type;

		if(_run_state.load())
		{
			base_type& base = *this;
			interface_notify_type pnotify = 
				construct_shared<basic_notify_type>(
					yggr_nothrow_new now_notify_type(base, key, in_user_data, handler) );
			return (pnotify) && (_notify_queue.push(pnotify), true);
		}
		else
		{
			out_user_data_type out_data;
			bool bsuccess = base_type::execute(key, in_user_data, out_data);
			return (handler(bsuccess, out_data), bsuccess);
		}
	}

	template<typename Handler> inline
	bool async_run_command(const Handler& handler)
	{
		typedef Handler handler_type;
		typedef 
			db_cmd_notify
			<
				base_type, 
				handler_type, 
				void, 
				in_user_data_type, 
				out_user_data_type
			> now_notify_type;

		if(_run_state.load())
		{
			base_type& base = *this;
			interface_notify_type pnotify = 
				construct_shared<basic_notify_type>(
					yggr_nothrow_new now_notify_type(base, handler) );
			return (pnotify) && (_notify_queue.push(pnotify), true);
		}
		else
		{
			return base_type::run_command(handler);
		}
	}

	template<typename Handler> inline
	bool async_run_command(const Handler& handler,
							const in_user_data_type& in_user_data)
	{
		typedef Handler handler_type;
		typedef 
			db_cmd_notify
			<
				base_type, 
				handler_type, 
				void, 
				in_user_data_type, 
				out_user_data_type
			> now_notify_type;

		if(_run_state.load())
		{
			base_type& base = *this;
			interface_notify_type pnotify = 
				construct_shared<basic_notify_type>(
					yggr_nothrow_new now_notify_type(base, in_user_data, handler) );
			return (pnotify) && (_notify_queue.push(pnotify), true);
		}
		else
		{
			return base_type::run_command(handler);
		}
	}

	template<typename Handler1, typename Handler2> inline
	bool async_run_command(const Handler1& handler1, const Handler2& handler2)
	{
		typedef Handler1 handler1_type;
		typedef Handler2 handler2_type;

		typedef 
			db_cmd_notify
			<
				base_type, 
				handler1_type, 
				handler2_type, 
				in_user_data_type, 
				out_user_data_type
			> now_notify_type;

		if(_run_state.load())
		{
			base_type& base = *this;
			interface_notify_type pnotify = 
				construct_shared<basic_notify_type>(
					yggr_nothrow_new now_notify_type(base, handler1, handler2) );
			return (pnotify) && (_notify_queue.push(pnotify), true);
		}
		else
		{
			in_user_data_type in_user_data;
			out_user_data_type out_data;
			bool bret = base_type::run_command(handler1, in_user_data, out_data);
			return (handler2(bret, out_data), bret);
		}
	}

	template<typename Handler1, typename Handler2> inline
	bool async_run_command(const Handler1& handler1,
							const in_user_data_type& in_user_data,
							const Handler2& handler2 )
	{
		typedef Handler1 handler1_type;
		typedef Handler2 handler2_type;

		typedef 
			db_cmd_notify
			<
				base_type, 
				handler1_type, 
				handler2_type, 
				in_user_data_type, 
				out_user_data_type
			> now_notify_type;

		if(_run_state.load())
		{
			base_type& base = *this;
			interface_notify_type pnotify = 
				construct_shared<basic_notify_type>(
					yggr_nothrow_new now_notify_type(base, handler1, in_user_data, handler2) );
			return (pnotify) && (_notify_queue.push(pnotify), true);
		}
		else
		{
			out_user_data_type out_data;
			bool bret = base_type::run_command(handler1, in_user_data, out_data);
			return (handler2(bret, out_data), bret);
		}
	}

	void start(u32 trd_count = 0, 
				const time_type& tm_step 
						= thread_config_type::default_sleep_time_step())
	{
		if(!trd_count)
		{
			return;
		}

		bool now_run_state = false;
		bool next_run_state = true;

		if(!_run_state.compare_exchange_strong(now_run_state, next_run_state))
		{
			return;
		}

		for(u32 i = 0, isize = trd_count; i != isize; ++i)
		{
			_trd_group.create_thread(boost::bind(&this_type::prv_handler_run, this, tm_step));
		}
	}

	inline bool is_running(void) const
	{
		return _run_state.load();
	}

	inline void stop(void)
	{
		bool now_run_state = true;
		bool next_run_state = false;

		_run_state.compare_exchange_strong(now_run_state, next_run_state);
	}

	inline void join(void)
	{
		_trd_group.join_all();
	}

	void clear_notify(void)
	{
		for(;this_type::prv_handler_execute(););
	}

	void clear(void)
	{
		for(;this_type::prv_handler_execute(););
		base_type::clear();
	}

protected:
	void prv_handler_run(const time_type& tm_step)
	{
		for(;_run_state.load();)
		{
			if(this_type::prv_handler_execute())
			{
				continue;
			}

			this_thread_type::yield();
			this_thread_type::sleep(tm_step);
		}

		accesser_type::s_thread_clear();
	}

	inline bool prv_handler_execute(void)
	{
		interface_notify_type pnotify;

		return 
			_notify_queue.pop(pnotify)
			&& pnotify
			&& pnotify->execute();
	}

protected:
	run_state_type _run_state;
	notify_queue_type _notify_queue;
	thread_group_type _trd_group;
};

} // namespace dbs_manager
} // namespace yggr

#endif // __YGGR_DBS_MANAGER_ASYNC_DBS_MGR_HPP__
