//time_cast.hpp

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

#ifndef __YGGR_TIME_TIME_CAST_HPP__
#define __YGGR_TIME_TIME_CAST_HPP__

#include <yggr/base/yggrdef.h>

#include <boost/thread/xtime.hpp>

namespace yggr
{
namespace time
{

template<typename Dst, typename Src>
struct time_cast_helper
{
	typedef Src src_type;
	typedef Dst dst_type;
	typedef dst_type result_type;

	inline dst_type operator()(const src_type& src) const
	{
		typedef boost::xtime bridge_type;
		bridge_type xtm = src;
		return dst_type(xtm);
	}
};

template<typename T>
struct time_cast_helper<T, T>
{
	typedef T src_type;
	typedef T dst_type;
	typedef const dst_type& result_type;

	inline result_type operator()(const src_type& src) const
	{
		return src;
	}
};

template<typename Dst, typename Src> inline
typename time_cast_helper<Dst, Src>::result_type time_cast(const Src& src)
{
	typedef time_cast_helper<Dst, Src> helper_type;
	helper_type helper;
	return helper(src);
}

} // namespace time
} // namespace yggr

#endif // __YGGR_TIME_TIME_CAST_HPP__