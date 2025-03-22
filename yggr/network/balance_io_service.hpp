//balance_io_service.hpp

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

#ifndef __YGGR_NETWORK_BALANCE_IO_SERVICE_HPP__
#define __YGGR_NETWORK_BALANCE_IO_SERVICE_HPP__

#include <yggr/base/yggrdef.h>
#include <boost/atomic.hpp>

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
	typedef u64 size_type;
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

public:
	inline operator base_service_type&(void)
	{
		return static_cast<base_service_type&>(*this);
	}

	inline operator const base_service_type&(void) const
	{
		return static_cast<const base_service_type&>(*this);
	}

	inline base_service_type& org_service(void)
	{
		return static_cast<base_service_type&>(*this);
	}

	inline const base_service_type& org_service(void) const
	{
		return static_cast<const base_service_type&>(*this);
	}

public:
	
	template<typename OService> inline
	s32 compare(const balance_io_service<OService>& right) const
	{
		size_type lcount = _count.load();
		size_type rcount = right.count();
		return 
			lcount == rcount?
				0
				: lcount < rcount? -1 : 1;
	}

	inline s32 compare(const this_type& right) const
	{
		if(this == boost::addressof(right))
		{
			return 0;
		}

		size_type lcount = _count.load();
		size_type rcount = right._count.load();
		return 
			lcount == rcount?
				0
				: lcount < rcount? -1 : 1;
	}

	template<typename OService> inline
	bool compare_eq(const balance_io_service<OService>& right) const
	{
		return _count.load() == right.count();
	}

	inline bool compare_eq(const this_type& right) const
	{
		return 
			(this == boost::addressof(right))
			|| (_count.load() == right._count.load());
	}

public:
	inline size_type count(void) const
	{
		return _count.load();
	}

public:
	inline void ex(void)
	{
		++_count;
	}

	inline void unex(void)
	{
		--_count;
	}

private:
	atomic_size_type _count;
};

// non-member function
template<typename S1, typename S2> inline
bool operator==(const balance_io_service<S1>& l, 
				const balance_io_service<S2>& r)
{
	return l.compare_eq(r);
}

template<typename S1, typename S2> inline
bool operator!=(const balance_io_service<S1>& l, 
				const balance_io_service<S2>& r)
{
	return !l.compare_eq(r);
}

template<typename S1, typename S2> inline
bool operator<(const balance_io_service<S1>& l, 
				const balance_io_service<S2>& r)
{
	return l.compare(r) < 0;
}

template<typename S1, typename S2> inline
bool operator<=(const balance_io_service<S1>& l, 
				const balance_io_service<S2>& r)
{
	return l.compare(r) <= 0;
}

template<typename S1, typename S2> inline
bool operator>(const balance_io_service<S1>& l, 
				const balance_io_service<S2>& r)
{
	return l.compare(r) > 0;
}

template<typename S1, typename S2> inline
bool operator>=(const balance_io_service<S1>& l, 
				const balance_io_service<S2>& r)
{
	return l.compare(r) >= 0;
}

} // namespace network
} // namespace yggr


#endif // __YGGR_NETWORK_BALANCE_IO_SERVICE_HPP__
