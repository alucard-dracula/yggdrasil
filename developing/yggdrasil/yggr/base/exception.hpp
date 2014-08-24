//exception.hpp

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

#ifndef __YGGR_EXCPTION_EXCEPTION_HPP__
#define __YGGR_EXCPTION_EXCEPTION_HPP__

#include <iostream>
#include <sstream>

#include <compatibility/stl_exception.hpp>
#include <yggr/base/yggrdef.h>

#include <yggr/charset/string.hpp>
#include <yggr/time/time.hpp>

#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/archive/archive_exception.hpp>
#include <boost/regex/pattern_except.hpp>
//#include <luabind/error.hpp>

#include <yggr/base/ctrl_center.hpp>

#include <yggr/base/ptr_single.hpp>
#include <yggr/safe_container/safe_queue.hpp>
#include <yggr/thread/boost_thread_config.hpp>
#include <yggr/base/exception_sort.hpp>



namespace yggr
{
namespace exception
{

class exception //: public ptr_single<exception>
{

public:
	enum
	{
		E_STL_ERR_CODE = exception_sort::E_code_stl_error,
		E_DTL_DB_ERR_CODE = exception_sort::E_code_dtl_db_error,
		E_DTL_ROOT_ERR_CODE = exception_sort::E_code_dtl_root_error,
		//E_LUABIND_ERR_CODE = exception_sort::E_code_luabind_error,
		E_RUNTIME_ERR_CODE = exception_sort::E_code_runtime_error,

		E_compile_u32 = 0xffffffff
	};

	typedef boost::any param_type;
public:

	typedef std::string err_string_type;
	typedef std::stringstream ss_type;

private:

	typedef thread::boost_thread_config_type thread_config_type;
	typedef thread_config_type::thread_type thread_type;
	typedef thread_config_type::thread_group_type thread_group_type;
	typedef thread_config_type::time_type time_type;
	typedef exception this_type;

public:
	struct error_code
	{
	private:
		typedef thread_config_type::id_type trd_id_type;
		typedef thread_config_type::this_thread_type this_thread_type;
	public:
		error_code(void);
		error_code(u32 code, const err_string_type& msg);
		error_code(u32 code, const trd_id_type& tid, const err_string_type& msg);
		error_code(const error_code& ec);
		~error_code(void);

		const std::time_t& time(void) const;
		const trd_id_type& thread_id(void) const;
		const err_string_type str_time(void) const;
		const u32& code(void) const;
		const err_string_type& message(void) const;
		const err_string_type to_string(void) const;

	private:
		std::time_t _tm;
		trd_id_type _trd_id;
		u32 _code;
		err_string_type _msg;
	};

public:
	typedef safe_container::safe_queue<error_code> err_queue_type;

public:
	exception(void);
	exception(const time_type& tm_step);
	~exception(void);

	void post(const error_code& e);
	void post_and_fix(const error_code& e, const u32& class_code);
	void join(void);
	void stop(void);

	template<typename Wrap_Param>
	void post_and_fix(const error_code& e, const u32& class_code, const Wrap_Param& wrap_param)
	{
		typedef system_controller::ctrl_center::param_type param_type;

		ptr_single<system_controller::ctrl_center>::obj_ptr_type pctrl = ptr_single<system_controller::ctrl_center>::get_ins();

		if(pctrl)
		{
			param_type param(wrap_param);
			pctrl->ctrl(e.code(), class_code, param);
		}

		_err_queue.push(e);
	}

public:

	static void throw_error(u32 code, const err_string_type& msg);
	static void throw_error(u32 code, const err_string_type& msg, u32 class_code);

	template<typename Param>
	static void throw_error(u32 code, const err_string_type& msg, u32 class_code, const Param param)
	{
		ptr_single<this_type>::obj_ptr_type pe = ptr_single<this_type>::get_ins();
		if(pe)
		{
			pe->post_and_fix(this_type::make_error(code, msg), class_code, param);
			return;
		}

		std::cerr << "code = " << code << " msg = " << msg << " class code = " << class_code << " has param"<< std::endl;
	}

	template<typename Err>
	static void throw_error(const Err& e)
	{
		ptr_single<this_type>::obj_ptr_type pe = ptr_single<this_type>::get_ins();
		if(pe)
		{
			pe->post(this_type::make_error(e));
			return;
		}

		error_code ec(this_type::make_error(e));
		std::cerr << "code = " << ec.code() << " msg = " << ec.message() << std::endl;
	}

	template<typename Err>
	static void throw_error(const Err& e, u32 class_code)
	{
		ptr_single<this_type>::obj_ptr_type pe = ptr_single<this_type>::get_ins();
		if(pe)
		{
			pe->post_and_fix(this_type::make_error(e), class_code);
			return;
		}

		error_code ec(this_type::make_error(e));
		std::cerr << "code = " << ec.code() << " msg = " << ec.message() << " class code = " << class_code << std::endl;
	}

	template<typename Err, typename Param>
	static void throw_error(const Err& e, u32 class_code, const Param param)
	{
		ptr_single<this_type>::obj_ptr_type pe = ptr_single<this_type>::get_ins();
		if(pe)
		{
			pe->post_and_fix(this_type::make_error(e), class_code, param);
			return;
		}

		error_code ec(this_type::make_error(e));
		std::cerr << "code = " << ec.code() << " msg = " << ec.message() << " class code = " << class_code << " has param"<< std::endl;
	}

private:
	static error_code make_error(const boost::system::system_error& e);
	static error_code make_error(const boost::system::error_code& e);
	static error_code make_error(const boost::archive::archive_exception& e);
	static error_code make_error(const compatibility::stl_exception& e);
	static error_code make_error(const std::runtime_error& e);
	static error_code make_error(const boost::regex_error& e);
	static error_code make_error(u32 code, const err_string_type& msg);

	template<typename Err>
	static error_code make_error(const Err& oe)
	{
		//return error_code(oe.code(), oe.message()); // old ver is the def
		return error_code(oe.code(), oe.what());
	}

private:

	bool message(void);
	void run(void);
	void init_obj(void);

private:
    bool _brun;
    time_type _tm_step;
	err_queue_type _err_queue;
	thread_group_type _trd_group;
};

std::ostream& operator<<(std::ostream& os, const exception::error_code& e);

} // namespace exception
} // namespace yggr

#endif // __YGGR_EXCPTION_EXCEPTION_HPP__
