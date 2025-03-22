// stream_deleter.hpp

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

#ifndef __YGGR_LOG_LOGOP_DETAIL_STREAM_DELETER_HPP__
#define __YGGR_LOG_LOGOP_DETAIL_STREAM_DELETER_HPP__

namespace yggr
{
namespace log
{
namespace log_op
{
namespace detail
{

template<typename T>
struct stream_deleter
{
	typedef void result_type;
	typedef T* argument_type;

	inline result_type operator()(argument_type x) const
	{
		if(x) 
		{
			if(x->is_open())
			{
				x->close();
			}
			delete x;
		}
	}
};

} // namespace detal
} // namespace log_op
} // namespace log
} // namespace yggr

#endif // __YGGR_LOG_LOGOP_DETAIL_STREAM_DELETER_HPP__
