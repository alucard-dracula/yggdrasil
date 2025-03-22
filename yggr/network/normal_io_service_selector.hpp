//normal_io_service_selector.hpp

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

#ifndef __YGGR_NETWORK_NORMAL_IO_SERVICE_SELECTOR_HPP__
#define __YGGR_NETWORK_NORMAL_IO_SERVICE_SELECTOR_HPP__

#include <yggr/base/yggrdef.h>

namespace yggr
{
namespace network
{

template<typename Saver>
class normal_io_service_selector
{
private:
	typedef Saver saver_type;
	typedef typename saver_type::iterator saver_val_iter_type;
	typedef typename saver_type::const_iterator saver_val_c_iter_type;
	typedef typename saver_type::value_type saver_val_type;

private:
	typedef normal_io_service_selector this_type;

public:
	explicit normal_io_service_selector(saver_type& saver)
		: _saver(saver), _next(_saver.begin())
	{
		assert(_saver.size());
	}

	explicit normal_io_service_selector(const normal_io_service_selector& right)
		: _saver(right._saver), _next(right._next)
	{
		assert(_saver.size());
	}

	~normal_io_service_selector(void)
	{
	}

public:
	inline saver_val_type& operator()(void)
	{
		assert(_saver.size());

		saver_val_type& val = *_next;
		if(++_next == _saver.end())
		{
			_next = _saver.begin();
		}

		return val;
	}

private:
	saver_type& _saver;
	saver_val_iter_type _next;
};

} // namespace network
} // namespace yggr

#endif // __YGGR_NETWORK_NORMAL_IO_SERVICE_SELECTOR_HPP__