//balance_io_service.hpp

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

#ifndef __YGGR_NETWORK_BALANCE_IO_SERVICE_HPP__
#define __YGGR_NETWORK_BALANCE_IO_SERVICE_HPP__

#include <yggr/base/yggrdef.h>
#include <boost/atomic.hpp>
#include <limits>

namespace yggr
{
namespace network
{

template<typename Service>
class balance_io_service : public Service
{
public:
	typedef Service base_service_type;
	typedef balance_io_service<base_service_type> this_type;

private:
	typedef std::size_t size_type;
	typedef boost::atomic<size_type> atomic_size_type;
public:
	balance_io_service(void)
        : _count(1)
	{
	}

	explicit balance_io_service(std::size_t concurrency_hint)
		: base_service_type(concurrency_hint), _count(1)
	{
	}

	~balance_io_service(void)
	{
	}

	operator base_service_type&(void)
	{
		base_service_type& base = *this;
		return base;
	}

	base_service_type& org_service(void)
	{
		base_service_type& base = *this;
		return base;
	}

	bool operator< (const this_type& right) const
	{
		return _count.load() < right._count.load();
	}

	bool operator<= (const this_type& right) const
	{
		return _count.load() <= right._count.load();
	}

	bool operator > (const this_type& right) const
	{
		return _count.load() > right._count.load();
	}

	bool operator >=(const this_type& right) const
	{
		return _count.load() >= right._count.load();
	}

	void ex(void)
	{
		//assert(_count.load() < std::numeric_limits<size_type>::max());
		++_count;

	}

	void unex(void)
	{
		//assert(_count.load() > 0);
		--_count;
	}

private:
	atomic_size_type _count;
};

} // namespace network
} // namespace yggr


#endif // __YGGR_NETWORK_BALANCE_IO_SERVICE_HPP__
