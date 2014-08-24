//task_real_data_getter.hpp

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

#ifndef __YGGR_TASK_CENTER_SUPPORT_TASK_REAL_DATA_GETTER_HPP__
#define __YGGR_TASK_CENTER_SUPPORT_TASK_REAL_DATA_GETTER_HPP__

#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>

namespace yggr
{
namespace task_center
{
namespace support
{

template<typename Task>
struct task_real_data_getter
{
	typedef Task task_type;

    // this foo is mark don't use it
    template<typename Real_Data>
	Real_Data* operator()(task_type&) const;

     // this foo is mark don't use it
	template<typename Real_Data>
	const Real_Data* operator()(const task_type&) const;

//	template<typename Real_Data>
//	Real_Data* operator()(task_type&) const
//	{
//		BOOST_MPL_ASSERT((boost::mpl::false_));
//		return 0;
//	}
//
//	template<typename Real_Data>
//	const Real_Data* operator()(const task_type&) const
//	{
//		BOOST_MPL_ASSERT((boost::mpl::false_));
//		return 0;
//	}
};

} // namespace support
} // namespace task_center
} // namespace yggr

#endif //__YGGR_TASK_CENTER_SUPPORT_TASK_REAL_DATA_GETTER_HPP__
