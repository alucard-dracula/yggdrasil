//security_protocol.hpp

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

#ifndef __YGGR_FLASH_SECURITY_SECURITY_PROTOCAL_HPP__
#define __YGGR_FLASH_SECURITY_SECURITY_PROTOCAL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/charset/string.hpp>
#include <yggr/file_system/local_file_operator.hpp>

#include <yggr/base/exception_sort.hpp>
#include <yggr/base/exception.hpp>

#include <boost/bind.hpp>
#include <algorithm>
#include <yggr/base/error_make.hpp>

namespace yggr
{
namespace flash_security
{

template<typename Def>
class security_protocol
{
private:
	//class error S----------------------------------------------------------

	ERROR_MAKER_BEGIN("security_protocol")
		ERROR_CODE_DEF_BEGIN(exception::exception_sort::E_code_start_7)
			ERROR_CODE_DEF(E_invalid_request_file)
			ERROR_CODE_DEF(E_invalid_post_file)
		ERROR_CODE_DEF_END(exception::exception_sort::E_code_end_7)

		ERROR_CODE_MSG_BEGIN()
			ERROR_CODE_MSG(E_invalid_request_file, "invalid request file")
			ERROR_CODE_MSG(E_invalid_post_file, "invalid post file")
		ERROR_CODE_MSG_END()
	ERROR_MAKER_END()

	//class error E----------------------------------------------------------

public:
	typedef Def def_type;
private:
	typedef security_protocol<def_type> this_type;
public:
	security_protocol(void)
	{
	}

	security_protocol(const std::string& request_str, const std::string& post_str)
		: _request_str(request_str), _post_str(post_str)
	{
	}

	security_protocol(const security_protocol& right)
		: _request_str(right._request_str), _post_str(right._post_str)
	{
	}

	~security_protocol(void)
	{
	}

	void load_of_file(const std::string& request_name, const std::string& post_name)
	{
		read_request_file(request_name);
		read_post_file(post_name);
	}

	void load_of_file(const std::string& request_name)
	{
		read_request_file(request_name);
	}


	bool is_availability(void) const
	{
		return _request_str.length() == def_type::request_length();
	}

	const std::string& get_request_str(void) const
	{
		return _request_str;
	}

	const std::string& get_post_str(void) const
	{
		return _post_str;
	}

private:
	void read_request_file(const std::string& file_name)
	{
		typedef yggr::file_system::local_file_operator_type::file_size_type file_size_type;
		file_size_type size = 0;
		std::string buf;
		yggr::file_system::local_file_operator_type::read_file_of_binary(file_name, buf, size);

		if(size < def_type::request_length() - 1)
		{
			exception::exception::throw_error(error_maker_type::make_error(error_maker_type::E_invalid_request_file));
			return;
		}
		else
		{
			_request_str.resize(def_type::request_length());
			memset(&_request_str[0], 0, def_type::request_length());
			//std::copy(buf.begin(), buf.begin() + def_type::request_length(), _request_str.begin());
			memcpy(&_request_str[0], &buf[0], def_type::request_length() - 1);
		}
	}

	void read_post_file(const std::string& file_name)
	{
		typedef yggr::file_system::local_file_operator_type::file_size_type file_size_type;
		typedef std::string::iterator str_iter_type;
		file_size_type size = 0;
		std::string buf;
		yggr::file_system::local_file_operator_type::read_file_of_binary(file_name, buf, size);

		if(!size)
		{
			exception::exception::throw_error(error_maker_type::make_error(error_maker_type::E_invalid_post_file));
			return;
		}
		else
		{
			str_iter_type new_end = std::remove_if(buf.begin(), buf.end(), boost::bind(&this_type::remove_fileter, this, _1));
			buf.erase(new_end, buf.end());
			size = buf.size() + 1;
			_post_str.resize(size);
			memset(&_post_str[0], 0, size);
			memcpy(&_post_str[0], &buf[0], size - 1);
			//std::copy(buf.begin(), buf.end(), _post_str.begin());

			//str_iter_type new_end = std::remove_if(_post_str.begin(), _post_str.end(), boost::bind(&this_type::remove_fileter, this, _1));
			
		}
	}

	bool remove_fileter(const char val) const
	{
		return val == '\r' || val == '\n' || val == '\t';
	}

private:
	std::string _request_str;
	std::string _post_str;
};

}
}

#endif //__YGGR_FLASH_SECURITY_SECURITY_PROTOCAL_HPP__