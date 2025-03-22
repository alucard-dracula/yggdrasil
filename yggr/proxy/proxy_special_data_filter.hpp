//proxy_special_data_filter.hpp

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

#ifndef __YGGR_PROXY_PROXY_SPECIAL_DATA_FILTER_HPP__
#define __YGGR_PROXY_PROXY_SPECIAL_DATA_FILTER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/nonable/nonable.hpp>

#include <yggr/safe_container/safe_set.hpp>

namespace yggr
{
namespace proxy
{

template<typename ProxyData, typename Mutex = void>
class proxy_special_data_filter
	: private nonable::noncopyable,
		private nonable::nonmoveable
{
public:
	typedef ProxyData proxy_data_type;
	typedef typename proxy_data_type::data_info_type data_info_type;

private:
	typedef typename proxy_data_type::data_info_parser_type data_info_parser_type;

private:
	typedef Mutex tpl_arg_mutex_type;
	typedef safe_container::safe_set<data_info_type, tpl_arg_mutex_type> safe_data_info_set_type;

private:
	typedef proxy_special_data_filter this_type;

public:
	proxy_special_data_filter(void)
	{
	}

	~proxy_special_data_filter(void)
	{
	}

public:
	template<typename T> inline
	bool append(void)
	{
		return _filter_table.insert(data_info_parser_type::template data_info<T>());
	}

	inline bool append(const data_info_type& info)
	{
		return _filter_table.insert(info);
	}

	template<typename T> inline
	bool need_filter(void) const
	{
		return _filter_table.find(data_info_parser_type::template data_info<T>());
	}

	inline bool need_filter(const data_info_type& info) const
	{
		return _filter_table.find(info);
	}

	template<typename T> inline
	bool remove(void)
	{
		return !!_filter_table.erase(data_info_parser_type::template data_info<T>());
	}

	inline bool remove(const data_info_type& info)
	{
		return !!_filter_table.erase(info);
	}

	inline void clear(void)
	{
		_filter_table.clear();
	}

private:
	safe_data_info_set_type _filter_table;
};

} // namespace proxy
} // namespace yggr

#endif // __YGGR_PROXY_PROXY_SPECIAL_DATA_FILTER_HPP__
