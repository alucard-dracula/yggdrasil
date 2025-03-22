//inc_ver.h

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

#ifndef __YGGR_INC_VER_H__
#define __YGGR_INC_VER_H__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/mplex/limits.hpp>
#include <yggr/support/operator_t.hpp>

#include <boost/ref.hpp>
#include <boost/atomic.hpp>

#ifdef _MSC_VER
#	pragma warning( push )
#	pragma warning (disable : 4800)
#endif //_MSC_VER

namespace yggr
{

class inc_ver
{

public:
	typedef u64 value_type;

private:
	typedef boost::atomic<value_type> ver_type;

private:
	typedef mplex::numeric_limits<value_type> numeric_limits_type;

private:
	typedef inc_ver this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type);

public:

	inc_ver(void)
		: _ver(0)
	{
	}

	inc_ver(BOOST_RV_REF(this_type) right)
		: _ver(right._ver.load())
	{
	}

	inc_ver(const this_type& right)
		: _ver(right._ver.load())
	{
	}

	~inc_ver(void)
	{
	}

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;

		if(this == boost::addressof(right_ref))
		{
			return *this;
		}

		u64 old_ver = 0;
		u64 new_ver = 0;

		do
		{
			old_ver = _ver.load();
			new_ver = right_ref.now_ver();
		} while(!_ver.compare_exchange_weak(old_ver, new_ver));

		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == boost::addressof(right))
		{
			return *this;
		}

		u64 old_ver = 0;
		u64 new_ver = 0;

		do
		{
			old_ver = _ver.load();
			new_ver = right.now_ver();
		} while(!_ver.compare_exchange_weak(old_ver, new_ver));
	}

public:
	u64 grab(void) const
	{
		u64 old_ver = 0;
		u64 new_ver = 0;

		do
		{
			old_ver = _ver.load();
			new_ver = old_ver < numeric_limits_type::max_type::value? old_ver + 1 : 0;
		} while(!_ver.compare_exchange_weak(old_ver, new_ver));

		return old_ver;
	}

	inline u64 now_ver(void) const
	{
		return _ver.load();
	}

	inline YGGR_OP_BOOL_EXPLICIT() operator bool(void) const
	{
		return !!_ver.load();
	}

	inline operator u64(void) const
	{
		return _ver.load();
	}

private:
	mutable ver_type _ver;
};

} // namespace yggr

#ifdef _MSC_VER
#	pragma warning( pop )
#endif //_MSC_VER

#endif //__YGGR_INC_VER_H__
