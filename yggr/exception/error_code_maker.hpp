//error_code_maker.hpp

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

#ifndef __YGGR_EXCEPTION_ERROR_CODE_MAKER_HPP__
#define __YGGR_EXCEPTION_ERROR_CODE_MAKER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>
#include <yggr/compatibility/stl_exception.hpp>

#include <yggr/exception/error_code.hpp>
#include <yggr/exception/exception_sort.hpp>

#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/archive/archive_exception.hpp>
#include <boost/regex/pattern_except.hpp>

#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_base_of.hpp>

#include <sstream>

namespace yggr
{
namespace exception
{

struct yggr_inner_exception_code_def
{
	YGGR_STATIC_CONSTANT(u32, E_STL_ERR_CODE = exception_sort::E_code_stl_error);
};

namespace detail
{

template<typename Err>
struct root_exception_maker
{
private:
	typedef yggr::exception::error_code error_code_type;

public:
	typedef Err type;

	inline error_code_type operator()(const type& err) const
	{
		return error_code_type(err.code(), err.what());
	}
};

template<>
struct root_exception_maker<std::exception>
{
private:
	typedef yggr::exception::error_code error_code_type;
	typedef exception::yggr_inner_exception_code_def code_def_type;

public:
	typedef std::exception type;

	inline error_code_type operator()(const type& err) const
	{
		return error_code_type(code_def_type::E_STL_ERR_CODE, err.what());
	}
};

template<>
struct root_exception_maker<boost::system::error_code>
{
private:
	typedef yggr::exception::error_code error_code_type;

public:
	typedef boost::system::error_code type;

	inline error_code_type operator()(const type& err) const
	{
		return error_code_type(err.value(), err.message());
	}
};

} // namespace detail


template<typename Err>
struct error_code_maker
{
private:
	typedef yggr::exception::error_code error_code_type;

public:
	typedef Err type;

private:
	typedef typename 
		boost::mpl::if_
		<
			typename boost::is_base_of<std::exception, type>::type,
			detail::root_exception_maker<std::exception>,
			typename 
				boost::mpl::if_
				<
					typename boost::is_base_of<boost::system::error_code, type>::type,
					detail::root_exception_maker<boost::system::error_code>,
					detail::root_exception_maker<type>
				>::type
		>::type h_type;

public:
	inline error_code_type operator()(const type& err) const
	{
		h_type h;
		return h(err);
	}
};

} // namespace exception
} // namespace yggr

// special error_code_maker
namespace yggr
{
namespace exception
{

template<>
struct error_code_maker<boost::system::system_error>
{
private:
	typedef yggr::exception::error_code error_code_type;

public:
	typedef boost::system::system_error type;

	inline error_code_type operator()(const type& err) const
	{
		return error_code_type(err.code().value(), err.what());
	}
};

template<>
struct error_code_maker<boost::regex_error>
{
private:
	typedef yggr::exception::error_code error_code_type;

public:
	typedef boost::regex_error type;

	inline error_code_type operator()(const type& err) const
	{
		std::stringstream ss;
		ss << "regex error at " << err.position() << " msg is " << err.what();
		return error_code_type(err.code(), ss.str());
	}
};


} // namespace exception
} // namespace yggr


#endif // __YGGR_EXCEPTION_ERROR_CODE_MAKER_HPP__