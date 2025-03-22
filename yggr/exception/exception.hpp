//exception.hpp

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

#ifndef __YGGR_EXCPTION_EXCEPTION_HPP__
#define __YGGR_EXCPTION_EXCEPTION_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/thread/boost_thread_config.hpp>
#include <yggr/compatibility/stl_exception.hpp>

#include <yggr/time/time.hpp>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/string_converter.hpp>
#include <yggr/safe_container/safe_deque.hpp>

#include <yggr/exception/error_code_maker.hpp>
#include <yggr/system_controller/ctrl_center.hpp>

#include <boost/atomic.hpp>

#include <iostream>

namespace yggr
{
namespace exception
{
namespace detail
{

class exception_basic_log_helper
{
public:
	typedef error_code error_code_type;

private:
	typedef exception_basic_log_helper this_type;

public:
	virtual ~exception_basic_log_helper(void)
	{
	}

public:
	virtual bool append(const error_code_type& e) = 0;
};

template<typename Accesser, typename StoreConverter = void>
class exception_log_helper
	: public exception_basic_log_helper
{
public:
	typedef exception_basic_log_helper base_type;

public:
	typedef Accesser accesser_type;
	typedef StoreConverter store_conv_type;
	typedef ::yggr::shared_ptr<accesser_type> accesser_ptr_type;

private:
	typedef exception_log_helper this_type;

public:
	exception_log_helper(accesser_ptr_type pacc)
		: _pacc(pacc)
	{
	}

	exception_log_helper(accesser_ptr_type pacc, const store_conv_type& conv)
		: _pacc(pacc), _conv(conv)
	{
	}

	~exception_log_helper(void)
	{
	}

public:
	virtual bool append(const error_code_type& e)
	{
		typedef typename store_conv_type::result_type rst_type;

		rst_type rst = _conv(e);
		return _pacc && _pacc->append(e.code(), rst);
	}

private:
	store_conv_type _conv;
	accesser_ptr_type _pacc;
};

template<typename Accesser>
class exception_log_helper<Accesser, void>
	: public exception_basic_log_helper
{
public:
	typedef exception_basic_log_helper base_type;

public:
	typedef Accesser accesser_type;
	typedef void store_conv_type;
	typedef ::yggr::shared_ptr<accesser_type> accesser_ptr_type;

private:
	typedef exception_log_helper this_type;

public:
	exception_log_helper(accesser_ptr_type pacc)
		: _pacc(pacc)
	{
	}

	template<typename SC_Nil>
	exception_log_helper(accesser_ptr_type pacc, const SC_Nil&)
		: _pacc(pacc)
	{
	}

	~exception_log_helper(void)
	{
	}

public:
	virtual bool append(const error_code_type& e)
	{
		return _pacc && _pacc->append(e.code(), e);
	}

private:
	accesser_ptr_type _pacc;
};

} // namespace detail
} // namespace exception
} // namespace yggr

namespace yggr
{
namespace exception
{

class exception
	: private nonable::noncopyable,
		private nonable::nonmoveable
{

private:
	typedef error_code error_code_type;
	typedef std::stringstream ss_type;

public:
	typedef std::string err_string_type;

private:
	typedef thread::boost_thread_config_type thread_config_type;
	typedef thread_config_type::thread_type thread_type;
	typedef thread_config_type::thread_group_type thread_group_type;
	typedef thread_config_type::time_type time_type;
	typedef boost::atomic<bool> run_state_type;

private:
	typedef exception this_type;

private:
	typedef safe_container::safe_deque<error_code_type> err_queue_type;

private:
	typedef detail::exception_basic_log_helper basic_log_helper_type;

	typedef shared_ptr<basic_log_helper_type> interface_log_helper_type;
	typedef safe_container::safe_wrap<interface_log_helper_type> log_helper_ptr_wrap_type;

public:
	exception(void);
	exception(const time_type& tm_step);
	~exception(void);

public:
	inline void post(const error_code_type& e)
	{
		_err_queue.push_back(e);
	}

	inline void join(void)
	{
		_trd_group.join_all();
	}

	void stop(void);

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename ...Args> inline
	void post_and_fix(const error_code_type& e, const u32& class_code, const Args&... args)
	{
		ptr_single<system_controller::ctrl_center>::obj_ptr_type pctrl =
			ptr_single<system_controller::ctrl_center>::get_ins();

		if(pctrl)
		{
			pctrl->ctrl(e.code(), class_code, boost::cref(args)...);
		}

		_err_queue.push_back(e);
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		void post_and_fix(const error_code_type& e, const u32& class_code \
							YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CREF_PARAMS ) ) { \
			ptr_single<system_controller::ctrl_center>::obj_ptr_type pctrl = \
				ptr_single<system_controller::ctrl_center>::get_ins(); \
			if(pctrl) { \
				pctrl->ctrl( \
					e.code(), class_code \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_CREF( __n__, YGGR_PP_SYMBOL_COMMA) ); } \
			_err_queue.push_back(e); }

#	define YGGR_PP_FOO_ARG_NAME() arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE()
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

public:
	template<typename Accesser> inline
	bool bind_log_accesser(::yggr::shared_ptr<Accesser> pacc)
	{
		typedef Accesser acc_type;
		typedef detail::exception_log_helper<acc_type> log_helper_type;

		interface_log_helper_type ptr
			= construct_shared<basic_log_helper_type>(yggr_nothrow_new log_helper_type(pacc));
		return (ptr) && (_log_helper_wrap.store(ptr), true);
	}

	template<typename Accesser, typename Stone_Conv> inline
	bool bind_log_accesser(::yggr::shared_ptr<Accesser> pacc, const Stone_Conv& conv)
	{
		typedef Accesser acc_type;
		typedef Stone_Conv conv_type;
		typedef detail::exception_log_helper<acc_type, conv_type> log_helper_type;

		interface_log_helper_type ptr
			= construct_shared<basic_log_helper_type>(yggr_nothrow_new log_helper_type(pacc, conv));
		return (ptr) && (_log_helper_wrap.store(ptr), true);
	}

	inline void clear_log_access(void)
	{
		_log_helper_wrap.clear();
	}

public:

	// !!!!this place don't support charset!!!!
	inline static void throw_error(u32 code, const err_string_type& msg)
	{
		ptr_single<this_type>::obj_ptr_type pe = ptr_single<this_type>::get_ins();
		if(pe)
		{
			pe->post(this_type::make_error(code, msg));
			return;
		}

		std::cerr << this_type::make_error(code, msg) << std::endl;
	}

	template<typename Err> inline
	static void throw_error(const Err& e)
	{
		ptr_single<this_type>::obj_ptr_type pe = ptr_single<this_type>::get_ins();
		if(pe)
		{
			pe->post(this_type::make_error(e));
		}
#ifndef YGGR_EXCEPTION_DEBUG
		else
#endif // YGGR_EXCEPTION_DEBUG
			std::cerr << this_type::make_error(e) << std::endl;
	}


#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename Char, typename ...Args> inline
	static void throw_error(u32 code, const Char* msg, u32 class_code, const Args& ...args)
	{
		this_type::throw_error(
			code,
			charset::string_converter::s_conv<err_string_type>(msg),
			class_code,
			args...);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String,
				typename ...Args > inline
	static void throw_error(u32 code, const Basic_String<Char, Traits, Alloc>& msg, u32 class_code, const Args& ...args)
	{
		this_type::throw_error(
			code,
			charset::string_converter::s_conv<err_string_type>(msg),
			class_code,
			args...);
	}

	template<typename ...Args> inline
	static void throw_error(u32 code, const err_string_type& msg, u32 class_code, const Args& ...args)
	{
		ptr_single<this_type>::obj_ptr_type pe = ptr_single<this_type>::get_ins();
		if(pe)
		{
			pe->post_and_fix(this_type::make_error(code, msg), class_code, args...);
		}
#ifndef YGGR_EXCEPTION_DEBUG
		else
#endif // YGGR_EXCEPTION_DEBUG
			std::cerr << this_type::make_error(code, msg) << " class code = " << class_code << " has param true"<< std::endl;
	}

	template<typename Err, typename ...Args> inline
	static void throw_error(const Err& e, u32 class_code, const Args& ...args)
	{
		ptr_single<this_type>::obj_ptr_type pe = ptr_single<this_type>::get_ins();
		if(pe)
		{
			pe->post_and_fix(this_type::make_error(e), class_code, args...);
		}
#ifndef YGGR_EXCEPTION_DEBUG
		else
#endif // YGGR_EXCEPTION_DEBUG
			std::cerr << this_type::make_error(e) << " class code = " << class_code << " has param true "<< std::endl;
	}

#else

#	if !defined(YGGR_EXCEPTION_DEBUG)
#		define ED_ELSE() else
#	else
#		define ED_ELSE()
#	endif // YGGR_EXCEPTION_DEBUG

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template<typename Char \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
	static void throw_error(u32 code, const Char* msg, u32 class_code \
							YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CREF_PARAMS ) ) { \
		this_type::throw_error( \
			code, charset::string_converter::s_conv<err_string_type>(msg), class_code \
			YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
			YGGR_PP_FOO_PARAMS_OP( __n__, YGGR_PP_SYMBOL_COMMA ) ); } \
	\
	template<typename Char, typename Traits, typename Alloc, \
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
	static void throw_error(u32 code, const Basic_String<Char, Traits, Alloc>& msg, u32 class_code \
								YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
								YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CREF_PARAMS ) ) { \
		this_type::throw_error( \
			code, charset::string_converter::s_conv<err_string_type>(msg), class_code \
			YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
			YGGR_PP_FOO_PARAMS_OP( __n__, YGGR_PP_SYMBOL_COMMA ) ); } \
	\
	BOOST_PP_EXPR_IF( __n__, template< ) \
		YGGR_PP_FOO_TYPES_DEF( __n__ ) \
	BOOST_PP_EXPR_IF( __n__, > ) inline \
	static void throw_error(u32 code, const err_string_type& msg, u32 class_code \
							YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CREF_PARAMS ) ) { \
		ptr_single<this_type>::obj_ptr_type pe = ptr_single<this_type>::get_ins(); \
		if(pe) { \
			pe->post_and_fix( \
				this_type::make_error(code, msg), class_code \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_FOO_PARAMS_OP( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		ED_ELSE() \
			std::cerr \
				<< this_type::make_error(code, msg) \
				<< " class code = " << class_code \
				<< " has param true"<< std::endl; } \
	\
	template<typename Err \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
	static void throw_error(const Err& e, u32 class_code \
							YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CREF_PARAMS ) ) { \
		ptr_single<this_type>::obj_ptr_type pe = ptr_single<this_type>::get_ins(); \
		if(pe) { \
			pe->post_and_fix( \
				this_type::make_error(e), class_code \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_FOO_PARAMS_OP( __n__, YGGR_PP_SYMBOL_COMMA ) ); } \
		ED_ELSE() \
			std::cerr \
				<< this_type::make_error(e) \
				<< " class code = " << class_code \
				<< " has param true "<< std::endl; }


#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE()
#	undef YGGR_PP_FOO_ARG_NAME
#	undef ED_ELSE

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

private:
	inline static error_code_type make_error(u32 code, const err_string_type& msg)
	{
		return error_code_type(code, msg);
	}

	template<typename Err> inline
	static error_code_type make_error(const Err& oe)
	{
		typedef error_code_maker<Err> maker_type;

		maker_type make;
		return make(oe);
	}

private:
	bool message(void);
	void run(void);
	void init_obj(void);

private:
    run_state_type _brun;
    time_type _tm_step;
	log_helper_ptr_wrap_type _log_helper_wrap;
	err_queue_type _err_queue;
	thread_group_type _trd_group;
};

} // namespace exception
} // namespace yggr

#endif // __YGGR_EXCPTION_EXCEPTION_HPP__
