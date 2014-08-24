//c_bosn_err.cpp

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

#if defined(_MSC_VER) && defined(_DEBUG)
#	include <vld.h>
#endif // _MSC_VER

#include <yggr/nsql_database_system/c_bson_err.hpp>
#include <utility>
#include <sstream>

namespace yggr
{
namespace nsql_database_system
{

c_bson_err::c_bson_err(void)
	: c_bson()
{
}

c_bson_err::c_bson_err(const bson& right)
	: c_bson(right)
{
}

c_bson_err::c_bson_err(const base_type& right)
	: c_bson(right)
{
}

c_bson_err::c_bson_err(const this_type& right)
	: c_bson(right)
{
}

c_bson_err::~c_bson_err(void)
{
}

c_bson_err::this_type& c_bson_err::operator=(const bson& right)
{
	base_type::operator=(right);
	return *this;
}

c_bson_err::this_type& c_bson_err::operator=(const base_type& right)
{
	base_type::operator=(right);
	return *this;
}

c_bson_err::this_type& c_bson_err::operator=(const this_type& right)
{
	const base_type& right_ref = right;
	base_type::operator=(right_ref);
	return *this;
}

void c_bson_err::swap(bson& right)
{
	base_type::swap(right);
}

void c_bson_err::swap(base_type& right)
{
	base_type::swap(right);
}

void c_bson_err::swap(this_type& right)
{
	base_type& right_ref = right;
	base_type::swap(right_ref);
}

std::pair<u32, const std::string> c_bson_err::err_info(void) const
{
	typedef std::pair<const u32, const std::string> ret_type;

	u32 code = 0;
	std::string msg;

	c_bson::const_iterator iter = c_bson::begin();

	if(iter != std::string("err") || iter != bson_value_type::E_BSON_TYPE_STRING)
	{
		return ret_type(0, std::string(""));
	}

	iter.load("err", msg);

	++iter;

	if(iter != std::string("code") || iter != bson_value_type::E_BSON_TYPE_INT)
	{
		return ret_type(0, std::string(""));
	}

	iter.load("code", code);


	return ret_type(code, msg);
}

std::pair<const std::string, bool> c_bson_err::what(void) const
{
	typedef std::pair<const std::string, bool> ret_type;

	std::pair<u32, const std::string> info(err_info());

	if(!info.first)
	{
		return ret_type(std::string(""), false);
	}

	std::stringstream ss;
	ss << "code = " << info.first << " err = " << info.second;

	return ret_type(ss.str(), true);
}

} // namespace nsql_database_system
} // namespace yggr

#define _YGGR_NSQL_DATABASE_SYSTEM_BSON_ERROR_SWAP_IMPL() \
	void swap(yggr::nsql_database_system::c_bson_err& l, bson& r) { \
		l.swap(r); } \
	\
	void swap(bson& l, yggr::nsql_database_system::c_bson_err& r) { \
		r.swap(l); } \
	\
	void swap(yggr::nsql_database_system::c_bson_err& l, \
				yggr::nsql_database_system::c_bson_err::base_type& r) { \
		l.swap(r); } \
	\
	void swap(yggr::nsql_database_system::c_bson_err::base_type& l, \
				yggr::nsql_database_system::c_bson_err& r) { \
		r.swap(l); } \
	\
	void swap(yggr::nsql_database_system::c_bson_err& l, \
				yggr::nsql_database_system::c_bson_err& r) { \
		l.swap(r); }

namespace std
{
	_YGGR_NSQL_DATABASE_SYSTEM_BSON_ERROR_SWAP_IMPL()
} // namespace std

namespace boost
{
	_YGGR_NSQL_DATABASE_SYSTEM_BSON_ERROR_SWAP_IMPL()
} // namespace boost

#undef _YGGR_NSQL_DATABASE_SYSTEM_BSON_ERROR_SWAP_IMPL
