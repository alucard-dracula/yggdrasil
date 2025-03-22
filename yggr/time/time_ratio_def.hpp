// time_ratio_def.hpp

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

#ifndef __YGGR_TIME_TIME_RATIO_DEF_HPP__
#define __YGGR_TIME_TIME_RATIO_DEF_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>

#include <boost/mpl/integral_c.hpp>

namespace yggr
{
namespace time
{

struct time_ratio_def
{
public:
	typedef yggr::s32 value_type;

	// 'x >> 1' <=> 'x / 2'
	typedef boost::mpl::integral_c<value_type, 1000> s_and_ms_ratio_type;
	typedef boost::mpl::integral_c<value_type, (s_and_ms_ratio_type::value >> 1) > s_and_ms_half_ratio_type;

	typedef boost::mpl::integral_c<value_type, 1000000> s_and_mcs_ratio_type;
	typedef boost::mpl::integral_c<value_type, (s_and_mcs_ratio_type::value >> 1) > s_and_mcs_half_ratio_type;

	typedef boost::mpl::integral_c<value_type, 1000000000> s_and_ns_ratio_type;
	typedef boost::mpl::integral_c<value_type, (s_and_ns_ratio_type::value >> 1) > s_and_ns_half_ratio_type;

	typedef boost::mpl::integral_c<value_type, 1000> ms_and_mcs_ratio_type;
	typedef boost::mpl::integral_c<value_type, (ms_and_mcs_ratio_type::value >> 1) > ms_and_mcs_half_ratio_type;

	typedef boost::mpl::integral_c<value_type, 1000000> ms_and_ns_ratio_type;
	typedef boost::mpl::integral_c<value_type, (ms_and_ns_ratio_type::value >> 1) > ms_and_ns_half_ratio_type;

	typedef boost::mpl::integral_c<value_type, 1000> mcs_and_ns_ratio_type;
	typedef boost::mpl::integral_c<value_type, (mcs_and_ns_ratio_type::value >> 1) > mcs_and_ns_half_ratio_type;

	YGGR_STATIC_CONSTANT(value_type, E_S_AND_MS_RATIO = s_and_ms_ratio_type::value);
	YGGR_STATIC_CONSTANT(value_type, E_S_AND_MS_HALF_RATIO = s_and_ms_half_ratio_type::value);

	YGGR_STATIC_CONSTANT(value_type, E_S_AND_MCS_RATIO = s_and_mcs_ratio_type::value);
	YGGR_STATIC_CONSTANT(value_type, E_S_AND_MCS_HALF_RATIO = s_and_mcs_half_ratio_type::value);

	YGGR_STATIC_CONSTANT(value_type, E_S_AND_NS_RATIO = s_and_ns_ratio_type::value);
	YGGR_STATIC_CONSTANT(value_type, E_S_AND_NS_HALF_RATIO = s_and_ns_half_ratio_type::value);

	YGGR_STATIC_CONSTANT(value_type, E_MS_AND_MCS_RATIO = ms_and_mcs_ratio_type::value);
	YGGR_STATIC_CONSTANT(value_type, E_MS_AND_MCS_HALF_RATIO = ms_and_mcs_half_ratio_type::value);

	YGGR_STATIC_CONSTANT(value_type, E_MS_AND_NS_RATIO = ms_and_ns_ratio_type::value);
	YGGR_STATIC_CONSTANT(value_type, E_MS_AND_NS_HALF_RATIO = ms_and_ns_half_ratio_type::value);

	YGGR_STATIC_CONSTANT(value_type, E_MCS_AND_NS_RATIO = mcs_and_ns_ratio_type::value);
	YGGR_STATIC_CONSTANT(value_type, E_MCS_AND_NS_HALF_RATIO = mcs_and_ns_half_ratio_type::value);
};

} // namespace time
} // namespace yggr

#endif // __YGGR_TIME_TIME_RATIO_DEF_HPP__
