//runtime_performance_tester.hpp

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

#ifndef __YGGR_TIME_RUNTIME_PERFORMANCE_TESTER_HPP__
#define __YGGR_TIME_RUNTIME_PERFORMANCE_TESTER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/nonable/noncopyable.hpp>

#include <ostream>

namespace yggr
{
namespace time
{

template<typename Time, typename Out = f64, typename OStream = std::ostream>
class runtime_performance_tester
    : private nonable::noncopyable
{
private:
	typedef Time time_type;
	typedef Out out_type;
	typedef OStream ostream_type;
	typedef runtime_performance_tester this_type;

public:

	runtime_performance_tester(out_type count = 1.0, ostream_type& os = std::cout)
		: _os(os), _count(count)
	{
	}

	~runtime_performance_tester(void)
	{
		time_type e_tm;
		_os << ((e_tm - _s_tm).template ex_to_second<out_type>() / _count) << "\n";
	}

private:
	ostream_type& _os;
	time_type _s_tm;
	out_type _count;
};

} // namespace time

using time::runtime_performance_tester;

} // namespace yggr

#endif // __YGGR_TIME_RUNTIME_PERFORMANCE_TESTER_HPP__
