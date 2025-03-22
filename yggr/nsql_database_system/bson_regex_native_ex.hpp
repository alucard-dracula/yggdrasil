//bson_regex_native_ex.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_BSON_REGEX_NATIVE_EX_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_BSON_REGEX_NATIVE_EX_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>

#include <yggr/nsql_database_system/bson_config.hpp>
#include <yggr/nsql_database_system/native_ex_decl_bson.hpp>

#include <boost/mpl/string.hpp>
#include <boost/mpl/size_t.hpp>

namespace yggr
{
namespace nsql_database_system
{

class bson_regex_native_ex
{
public:
	typedef boost::mpl::string<'ilms', 'ux'> bson_regex_options_sorted_type; //"ilmsux"
	typedef boost::mpl::size<bson_regex_options_sorted_type>::type bson_regex_options_buffer_size_type;
	YGGR_STATIC_CONSTANT(std::size_t, E_BSON_REGEX_OPTIONS_BUFFER_SIZE = bson_regex_options_buffer_size_type::value); // 6
	YGGR_STATIC_CONSTANT(std::size_t, E_BSON_REGEX_OPTIONS_STRING_BUFFER_SIZE = bson_regex_options_buffer_size_type::value + 1); // 7

public:
	typedef char (&options_buffer_ref_type)[E_BSON_REGEX_OPTIONS_STRING_BUFFER_SIZE];

private:
	typedef bson_regex_native_ex this_type;

public:
	static std::size_t s_bson_regex_options_sorted(options_buffer_ref_type buffer, const char* options);
	static std::size_t s_bson_regex_options_sorted(options_buffer_ref_type buffer, const char* options, std::size_t count);
};

} // namespace nsql_database_system
} // namespace yggr

#endif // __YGGR_NSQL_DATABASE_SYSTEM_BSON_REGEX_NATIVE_EX_HPP__
