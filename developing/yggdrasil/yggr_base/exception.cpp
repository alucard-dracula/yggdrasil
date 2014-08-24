//exception.cpp

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

#include <iostream>

#include <yggr/base/exception.hpp>
#include <yggr/log/yggr_exception_log_accesser.hpp>

#if defined(_MSC_VER)
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace exception
{

//-----------------error_code---------------------------------------
exception::error_code::error_code(void)
	: _msg("invlid code")
{
}

exception::error_code::error_code(u32 code, const err_string_type& msg)
	: _tm(std::time(0)), _trd_id(this_thread_type::id()), _code(code), _msg(msg)
{
}

exception::error_code::error_code(u32 code, const exception::error_code::trd_id_type& tid, const err_string_type& msg)
	: _tm(std::time(0)), _trd_id(tid), _code(code), _msg(msg)
{
}

exception::error_code::error_code(const error_code& ec)
	: _tm(ec._tm), _trd_id(ec._trd_id), _code(ec._code), _msg(ec._msg)
{
}

exception::error_code::~error_code(void)
{
}

const std::time_t& exception::error_code::time(void) const
{
	return _tm;
}

const exception::error_code::trd_id_type& exception::error_code::thread_id(void) const
{
	return _trd_id;
}

const exception::err_string_type exception::error_code::str_time(void) const
{
	err_string_type::value_type str[64] = {0};
	std::strftime(str, sizeof(str) - sizeof(err_string_type::value_type), "%Y-%m-%d %X", std::localtime(&_tm));

	ss_type ss;
	ss << str;
	return ss.str();
}

const u32& exception::error_code::code(void) const
{
	return _code;
}

const exception::err_string_type& exception::error_code::message(void) const
{
	return _msg;
}

const exception::err_string_type exception::error_code::to_string(void) const
{
	ss_type ss;
	ss << "tm = " << str_time() << ", thread id = " << _trd_id << ", code = "<< code() << ", msg = " << message() << std::endl;
	return ss.str();
}

//--------------------exception----------------------------------

exception::exception(void)
	: _brun(false), _tm_step(0, 33000000)
{
	init_obj();
}

exception::exception(const exception::time_type& tm_step)
	: _brun(false), _tm_step(tm_step)
{
	init_obj();
}

exception::~exception(void)
{
	stop();
}

void exception::init_obj(void)
{
	if(_brun)
	{
		return;
	}

	_brun = true;

	if(!(_trd_group.create_thread(boost::bind(&exception::run, this))))
	{
		_brun = false;
	}
}

void exception::run(void)
{
	for(;_brun;)
	{
		if(message())
		{
			continue;
		}
		thread_type::yield();
		time_type tm;
		tm += _tm_step;
		thread_type::sleep(tm);
	}
}

void exception::post(const error_code& e)
{
	_err_queue.push(e);
}


void exception::post_and_fix(const error_code& e, const u32& class_code)
{
	ptr_single<system_controller::ctrl_center>::obj_ptr_type pctrl = ptr_single<system_controller::ctrl_center>::get_ins();
	if(pctrl)
	{
		pctrl->ctrl(e.code(), class_code);
	}

	_err_queue.push(e);
}


bool exception::message(void)
{
	if(_err_queue.empty())
	{
		return false;
	}

	for(;!_err_queue.empty();)
	{
		error_code e;
		if(_err_queue.pop(e))
		{
			//std::cerr << e.to_string() << std::endl;

			ptr_single<log::yggr_exception_log_accesser_type>::obj_ptr_type plog = ptr_single<log::yggr_exception_log_accesser_type>::get_ins();
			if(plog)
			{
				plog->append(e);
				continue;
			}

			std::cerr << e.to_string() << std::endl;
		}
	}

	return true;
}

void exception::join(void)
{
	_trd_group.join_all();
}

void exception::stop(void)
{
	if(!_brun)
	{
		return;
	}
	_brun = false;
	join();
	for(;message(););
}


void exception::throw_error(u32 code, const exception::err_string_type& msg)
{
	ptr_single<this_type>::obj_ptr_type pe = ptr_single<this_type>::get_ins();
	if(pe)
	{
		pe->post(this_type::make_error(code, msg));
		return;
	}

	std::cerr << "code = " << code << " msg = " << msg << std::endl;
}

void exception::throw_error(u32 code, const exception::err_string_type& msg, u32 class_code)
{
	ptr_single<this_type>::obj_ptr_type pe = ptr_single<this_type>::get_ins();
	if(pe)
	{
		pe->post_and_fix(this_type::make_error(code, msg), class_code);
		return;
	}

	std::cerr << "code = " << code << " msg = " << msg << " class code = " << class_code << std::endl;
}

exception::error_code exception::make_error(const boost::system::system_error& e)
{
	return error_code(e.code().value(), e.what());
}

exception::error_code exception::make_error(const boost::system::error_code& e)
{
	return error_code(e.value(), e.message());
}

exception::error_code exception::make_error(const boost::archive::archive_exception& e)
{
	return error_code(e.code, e.what());
}

exception::error_code exception::make_error(const compatibility::stl_exception& e)
{
	return error_code(exception::E_STL_ERR_CODE, e.what());
}

//exception::error_code exception::make_error(const luabind::error& e)
//{
//	return error_code(exception::E_LUABIND_ERR_CODE, e.what());
//}
//
//exception::error_code exception::make_error(const luabind::cast_failed& e)
//{
//	return error_code(exception::E_LUABIND_ERR_CODE, e.what());
//}

exception::error_code exception::make_error(const boost::regex_error& e)
{
	std::stringstream ss;
	ss << "regex error at " << e.position() << " msg is " << e.what();
	return error_code(e.code(), ss.str());
}

exception::error_code exception::make_error(const std::runtime_error& e)
{
	return error_code(exception::E_STL_ERR_CODE, e.what());
}

exception::error_code exception::make_error(u32 code, const err_string_type& msg)
{
	return error_code(code, msg);
}

std::ostream& operator<<(std::ostream& os, const exception::error_code& e)
{
	os << e.to_string();
	return os;
}

} // namespace exception
} // namespace yggr

