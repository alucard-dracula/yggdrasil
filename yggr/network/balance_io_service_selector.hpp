//balance_io_service_selector.hpp

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

#ifndef __YGGR_NETWORK_BALANCE_IO_SERVICE_SELECTOR_HPP__
#define __YGGR_NETWORK_BALANCE_IO_SERVICE_SELECTOR_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/ptr_val/ptr_val_operator.hpp>
#include <yggr/iterator_ex/iterator.hpp>

#include <cassert>
#include <algorithm>

namespace yggr
{
namespace network
{

template<typename Saver>
class balance_io_service_selector
{
private:
	typedef Saver saver_type;
	typedef typename saver_type::iterator saver_val_iter_type;
	typedef typename saver_type::const_iterator saver_val_c_iter_type;
	typedef typename saver_type::value_type saver_val_type;

private:
	typedef balance_io_service_selector this_type;

public:
	explicit balance_io_service_selector(saver_type& saver)
		: _saver(saver)
	{
		assert(!_saver.empty());
	}

	explicit balance_io_service_selector(const this_type& right)
		: _saver(right._saver)
	{
		assert(!_saver.empty());
	}

	~balance_io_service_selector(void)
	{
	}

public:
	inline saver_val_type& operator()(void)
	{
		assert(!_saver.empty());

		if(_saver.empty())
		{
			return this_type::failed_saver_val();
		}

		ptr_val::ptr_val_less<saver_val_type> cmp;
		saver_val_iter_type iter = std::min_element(boost::begin(_saver), boost::end(_saver), cmp);
		assert(iter != _saver.end());
		
		return 
			iter == _saver.end()?
				this_type::failed_saver_val()
				: *iter;
	}

private:
	inline static saver_val_type& failed_saver_val(void)
	{
		static saver_val_type failed_val;
		return failed_val;
	}

private:
	saver_type& _saver;
};

} // namespace network
} // namespace yggr

#endif // __YGGR_NETWORK_BALANCE_IO_SERVICE_SELECTOR_HPP__