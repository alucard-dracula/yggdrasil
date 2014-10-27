//inc_ver.h

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

#ifndef __YGGR_INC_VER_H__
#define __YGGR_INC_VER_H__

#include <yggr/base/yggrdef.h>
#include <boost/atomic.hpp>

#ifdef _MSC_VER
#	pragma warning( push )
#	pragma warning (disable : 4800)
#endif //_MSC_VER

namespace yggr
{

class inc_ver
{
private:
	typedef boost::atomic<u32> ver_type;	
	typedef inc_ver this_type;

public:

	inc_ver(void)
		: _ver(0)
	{
	};

	virtual ~inc_ver(void)
	{
	}

	inline u32 grab(void)
	{
		return _ver++;
	}

	inline u32 now_ver(void) const
	{
		return _ver.load();
	}

	inline operator bool(void) const
	{
		return now_ver();
	}

	operator u32(void) const
	{
		return _ver;
	}

private:
	ver_type _ver;
};

} // namespace yggr

#ifdef _MSC_VER
#	pragma warning( pop )
#endif //_MSC_VER

#endif //__YGGR_INC_VER_H__
