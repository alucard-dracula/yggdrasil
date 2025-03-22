//bson_regex_native_ex.cpp

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

#include <yggr/nsql_database_system/bson_regex_native_ex.hpp>

#include <cstring>
#include <algorithm>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{



/*static*/
std::size_t bson_regex_native_ex::s_bson_regex_options_sorted(options_buffer_ref_type buffer, const char* options)
{
	if(!options)
	{
		buffer[0] = 0;
		return 0;
	}

	const char* c = boost::mpl::c_str<this_type::bson_regex_options_sorted_type>::value;
	std::size_t i = 0;

	for(; 
		(i != this_type::E_BSON_REGEX_OPTIONS_STRING_BUFFER_SIZE) && (*c);
		++c) 
	{
		if(std::strchr(options, *c)) 
		{
			buffer[i] = *c;
			++i;
		}
	}

	buffer[i] = 0;
	return i;
}

/*static*/
std::size_t bson_regex_native_ex::s_bson_regex_options_sorted(options_buffer_ref_type buffer, const char* options, std::size_t count)
{
	if(!options)
	{
		buffer[0] = 0;
		return 0;
	}

	const char* c = boost::mpl::c_str<this_type::bson_regex_options_sorted_type>::value;
	std::size_t i = 0;
	const char* opt_begin = options;
	const char* opt_end = options + count;

	for(; 
		(i != this_type::E_BSON_REGEX_OPTIONS_STRING_BUFFER_SIZE) && (*c);
		++c) 
	{
		if(std::find(opt_begin, opt_end, *c) != opt_end) 
		{
			buffer[i] = *c;
			++i;
		}
	}

	buffer[i] = 0;
	return i;
}

} // namespace nsql_database_system
} // namespace yggr

