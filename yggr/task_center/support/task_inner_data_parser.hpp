//task_inner_data_parser.hpp

/****************************************************************************
Copyright (c) 2010-2022 yggdrasil

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

#ifndef __YGGR_TASK_CENTER_SUPPORT_TASK_INNER_DATA_PARSE_HPP__
#define __YGGR_TASK_CENTER_SUPPORT_TASK_INNER_DATA_PARSE_HPP__

#include <cassert>
#include <boost/ref.hpp>

#include <yggr/any_val/any.hpp>

namespace yggr
{
namespace task_center
{
namespace support
{

template<typename T>
struct task_inner_data_parser
{
	typedef T data_container_type;

    template<typename Real_Data>
	inline static Real_Data* inner_data(data_container_type& cont)
	{
		return cont.template parse_data<Real_Data>();
	}

	template<typename Real_Data>
	inline static const Real_Data* inner_data(const data_container_type& cont)
	{
		return cont.template parse_data<Real_Data>();
	}
};

} // namespace support
} // namespace task_center
} // namespace yggr

namespace yggr
{
namespace task_center
{
namespace support
{

template<>
struct task_inner_data_parser< ::yggr::any>
{
	typedef ::yggr::any data_container_type;

    template<typename Real_Data>
	inline static Real_Data* inner_data(data_container_type& cont)
	{
		assert(typeid(Real_Data) == cont.type());
		return ::yggr::any_cast<Real_Data>(boost::addressof(cont));
	}

	template<typename Real_Data>
	inline static const Real_Data* inner_data(const data_container_type& cont)
	{
		assert(typeid(Real_Data) == cont.type());
		return ::yggr::any_cast<Real_Data>(boost::addressof(cont));
	}
};

template<>
struct task_inner_data_parser< ::boost::any>
{
	typedef ::boost::any data_container_type;

    template<typename Real_Data>
	inline static Real_Data* inner_data(data_container_type& cont)
	{
		assert(typeid(Real_Data) == cont.type());
		return ::yggr::any_cast<Real_Data>(boost::addressof(cont));
	}

	template<typename Real_Data>
	inline static const Real_Data* inner_data(const data_container_type& cont)
	{
		assert(typeid(Real_Data) == cont.type());
		return ::yggr::any_cast<Real_Data>(boost::addressof(cont));
	}
};

} // namespace support
} // namespace task_center
} // namespace yggr

#endif //__YGGR_TASK_CENTER_SUPPORT_TASK_INNER_DATA_PARSE_HPP__
