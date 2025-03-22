//options_setter.hpp

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

#ifndef __YGGR_NETWORK_SESSION_HELPER_OPTIONS_SETTER_HPP__
#define __YGGR_NETWORK_SESSION_HELPER_OPTIONS_SETTER_HPP__

#include <yggr/base/yggrdef.h>

namespace yggr
{
namespace network
{
namespace session_helper
{

struct empty_options_setter
{
public:
	template<typename T> inline
	bool operator()(const T&) const 
	{
		return true;
	}
};

typedef empty_options_setter default_options_setter_type;

} // namespace session_helper

using session_helper::default_options_setter_type;

} // namespace network
} // namespace yggr

#endif //__YGGR_NETWORK_SESSION_HELPER_OPTIONS_SETTER_HPP__