//ref_count_info.h

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

#ifndef __YGGR_REF_COUNT_INFO_H__
#define __YGGR_REF_COUNT_INFO_H__

#include <yggr/base/yggrdef.h>
#include <boost/atomic.hpp>

namespace yggr
{

template<typename Val = yggr::u64>
class ref_count_info
{
public:
	typedef Val value_type;

private:
	typedef boost::atomic<value_type> count_type;

private:
	typedef ref_count_info this_type;

public:

	ref_count_info(void)
		: _count(0)
	{
	}

	ref_count_info(const this_type& right)
		: _count(right.use_count())
	{
	}

	~ref_count_info(void)
	{
	}

public:
	inline this_type& operator=(const this_type& right)
	{
		if(this == &right) {return *this;}
		_count = right.use_count();
		return *this;
	}

	inline value_type grab(void)
	{
		return _count++;
	}

	inline value_type drop(void)
	{
		value_type count = _count.load();
		--_count;
		return count;
	}

	inline void reset(void)
	{
		_count = value_type();
	}

	inline value_type use_count(void) const
	{
		return _count.load();
	}

	inline bool empty(void) const
	{
		return _count == value_type();
	}

private:
	count_type _count;
};

// non-member function
// ==
template<typename T1, typename T2> inline
bool operator==(const ref_count_info<T1>& l, const T2& r)
{
	return l.use_count() == r;
}

template<typename T1, typename T2> inline
bool operator==(const T1& l, const ref_count_info<T2>& r)
{
	return l == r.use_count();
}

template<typename T1, typename T2> inline
bool operator==(const ref_count_info<T1>& l, const ref_count_info<T2>& r)
{
	return l.use_count() == r.use_count();
}

// !=
template<typename T1, typename T2> inline
bool operator!=(const ref_count_info<T1>& l, const T2& r)
{
	return !(l.use_count() == r);
}

template<typename T1, typename T2> inline
bool operator!=(const T1& l, const ref_count_info<T2>& r)
{
	return !(l == r.use_count());
}

template<typename T1, typename T2> inline
bool operator!=(const ref_count_info<T1>& l, const ref_count_info<T2>& r)
{
	return !(l.use_count() == r.use_count());
}

// <
template<typename T1, typename T2> inline
bool operator<(const ref_count_info<T1>& l, const T2& r)
{
	return l.use_count() < r;
}

template<typename T1, typename T2> inline
bool operator<(const T1& l, const ref_count_info<T2>& r)
{
	return l < r.use_count();
}

template<typename T1, typename T2> inline
bool operator<(const ref_count_info<T1>& l, const ref_count_info<T2>& r)
{
	return l.use_count() < r.use_count();
}

// <=
template<typename T1, typename T2> inline
bool operator<=(const ref_count_info<T1>& l, const T2& r)
{
	return !(r < l.use_count());
}

template<typename T1, typename T2> inline
bool operator<=(const T1& l, const ref_count_info<T2>& r)
{
	return !(r.use_count() < l);
}

template<typename T1, typename T2> inline
bool operator<=(const ref_count_info<T1>& l, const ref_count_info<T2>& r)
{
	return !(r.use_count() < l.use_count());
}

// >
template<typename T1, typename T2> inline
bool operator>(const ref_count_info<T1>& l, const T2& r)
{
	return r < l.use_count();
}

template<typename T1, typename T2> inline
bool operator>(const T1& l, const ref_count_info<T2>& r)
{
	return r.use_count() < l;
}

template<typename T1, typename T2> inline
bool operator>(const ref_count_info<T1>& l, const ref_count_info<T2>& r)
{
	return r.use_count() < l.use_count();
}

// >=
template<typename T1, typename T2> inline
bool operator>=(const ref_count_info<T1>& l, const T2& r)
{
	return !(l.use_count() < r);
}

template<typename T1, typename T2> inline
bool operator>=(const T1& l, const ref_count_info<T2>& r)
{
	return !(l < r.use_count());
}

template<typename T1, typename T2> inline
bool operator>=(const ref_count_info<T1>& l, const ref_count_info<T2>& r)
{
	return !(l.use_count() < r.use_count());
}


// ref_count_info_locker

template<typename T>
struct ref_count_info_locker
{
public:
	typedef T ref_count_type;
private:
	typedef ref_count_info_locker this_type;
public:
	ref_count_info_locker(ref_count_type& rc)
		: _rc(rc)
	{
		_rc.grab();
	}

	~ref_count_info_locker(void)
	{
		_rc.drop();
	}

private:
	ref_count_type& _rc;
};

} // namespace yggr

#endif //__YGGR_REF_COUNT_INFO_H__
