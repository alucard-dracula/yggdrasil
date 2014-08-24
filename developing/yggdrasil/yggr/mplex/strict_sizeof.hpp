//strict_sizeof.hpp

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

#ifndef __YGGR_MPLEX_STRICT_SIZEOF_HPP__
#define __YGGR_MPLEX_STRICT_SIZEOF_HPP__

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_class.hpp>

#include <yggr/base/yggrdef.h>

#define YGGR_PP_REG_STRICT_SIZEOF(__type__, __size__) \
	namespace yggr { namespace mplex { \
		template<> \
		struct strict_sizeof<__type__> { \
			enum { \
				value = ( __size__ ), \
				E_compile_u32 = 0xffffffff }; }; } }

namespace yggr
{
namespace mplex
{

template<typename T>
struct strict_sizeof
{
private:
	typedef boost::is_class<T> is_class_type;

public:
	enum
	{
		value = (is_class_type::value? 0 : sizeof(T)),
		E_compile_u32 = 0xffffffff
	};

	BOOST_STATIC_ASSERT((value != 0));
};

} // namespace mplex
} // namespace yggr


#endif //__YGGR_MPLEX_STRICT_SIZEOF_HPP__