// c_bson_error_error_code_maker.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_ERROR_ERROR_CODE_MAKER_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_ERROR_ERROR_CODE_MAKER_HPP__

#include <yggr/exception/error_code_maker.hpp>
#include <sstream>

namespace yggr
{
namespace exception
{

template<>
struct error_code_maker<bson_error_t>
{
private:
	typedef yggr::exception::error_code error_code_type;

public:
	typedef bson_error_t type;

	inline error_code_type operator()(const type& err) const
	{
		std::stringstream ss;
		ss << "domain = " << err.domain << " " << err.message;
		return error_code_type(err.code, ss.str());
	}
};

template<>
struct error_code_maker<yggr::nsql_database_system::c_bson_error>
{
private:
	typedef yggr::exception::error_code error_code_type;

public:
	typedef nsql_database_system::c_bson_error type;

	inline error_code_type operator()(const type& err) const
	{
		std::stringstream ss;
		ss << "domain = " << err.domain() << " " << err.what();
		return error_code_type(err.code(), ss.str());
	}
};

} // namespace exception
} // namespace yggr

#endif // __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_ERROR_ERROR_CODE_MAKER_HPP__
