//ref_count_info.h

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

#ifndef __YGGR_REF_COUNT_INFO_H__
#define __YGGR_REF_COUNT_INFO_H__

#include <boost/atomic.hpp>
#include <yggr/base/yggrdef.h>


namespace yggr
{

template<typename Val = yggr::u64>
class ref_count_info
{
private:
	typedef Val val_type;
	typedef boost::atomic<val_type> count_type;

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

	virtual ~ref_count_info(void)
	{
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) {return *this;}
		_count = right.use_count();
		return *this;
	}

	val_type grab(void)
	{
		return _count++;
	}

	val_type drop(void)
	{
		val_type count = _count.load();
		--_count;
		return count;
	}

	void reset(void)
	{
		_count = val_type();
	}

	val_type use_count(void) const
	{
		return _count.load();
	}

	bool empty(void) const
	{
		return _count == val_type();
	}

	bool operator<(const this_type& right) const
	{
		return _count.load() < right.use_count();
	}

	bool operator<=(const this_type& right) const
	{
		return _count.load() <= right.use_count();
	}

	bool operator>(const this_type& right) const
	{
		return _count.load() > right.use_count();
	}

	bool operator>=(const this_type& right) const
	{
		return _count.load() >= right.use_count();
	}

	bool operator==(const this_type& right) const
	{
		return _count.load() == right.use_count();
	}

	bool operator!=(const this_type& right) const
	{
		return _count.load() != right.use_count();
	}

private:
	count_type _count;
};

template<typename T>
struct ref_count_info_lock
{
public:
	typedef T ref_count_type;
private:
	typedef ref_count_info_lock this_type;
public:
	ref_count_info_lock(ref_count_type& rc)
		: _rc(rc)
	{
		_rc.grab();
	}

	~ref_count_info_lock(void)
	{
		_rc.drop();
	}

private:
	ref_count_type& _rc;
};

} // namespace yggr

#endif //__YGGR_REF_COUNT_INFO_H__
