//time.cpp

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

#include <yggr/time/time.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

namespace yggr
{
namespace time
{

time::time(void)
{
	base_type& base = *this;

#if BOOST_VERSION < 105300
	boost::xtime_get(&base, boost::TIME_UTC); 
#else
	boost::xtime_get(&base, boost::TIME_UTC_); 
#endif // BOOST_VERSION
}

time::time(xtime_sec_t tsec, xtime_nsec_t tnsec)
{
	base_type::sec = tsec;
	base_type::nsec = tnsec;
	this_type::rebuild();
}

time::time(const time::base_type& right)
{
	base_type::sec = right.sec;
	base_type::nsec = right.nsec;
	this_type::rebuild();
}

time::time(const this_type& right)
{
	assert(right.validate());

	base_type::sec = right.sec;
	base_type::nsec = right.nsec;
}

time::~time(void)
{
}

std::tm time::to_tm(void) const
{
	return boost::posix_time::to_tm(*this);
}

std::tm time::to_local_tm(void) const
{
	typedef boost::date_time::c_local_adjustor<boost::system_time> local_adj;

	boost::system_time sys_time = *this;
	boost::system_time local_sys_time = local_adj::utc_to_local(sys_time);

	return boost::posix_time::to_tm(local_sys_time);
}

} //namespace time
} //namespace yggr


