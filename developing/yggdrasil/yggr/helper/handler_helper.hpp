//handler_helper.hpp

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

#ifndef __YGGR_HELPER_HANDLER_HELPER_HPP__
#define __YGGR_HELPER_HANDLER_HELPER_HPP__

#include <yggr/base/interface_ptr.hpp>

namespace yggr
{
namespace helper
{

class base_handler_helper
{
public:
	virtual ~base_handler_helper(void) {}
	virtual void run(void) const = 0;
};

template<typename Handler>
class handler_helper : public base_handler_helper
{
private:
	typedef Handler handler_type;
public:
	handler_helper(const handler_type& handler)
		: _handler(handler)
	{
	}

	virtual ~handler_helper(void)
	{
	}

	virtual void run(void) const
	{
		_handler();
	}
private:
	handler_type _handler;
};

typedef interface_ptr<base_handler_helper> handler_helper_interface_type;

} // namespace helper
} // namespace yggr

#endif //__YGGR_HELPER_HANDLER_HELPER_HPP__