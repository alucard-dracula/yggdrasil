//charset_base_foo.cpp

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

#include <yggr/charset/charset_base_foo.hpp>
#include <yggr/base/static_constant.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

namespace yggr
{
namespace charset
{

/*static*/
s_size_type charset_base_foo::strncmp_case(const char* l, const char* r, size_type len)
{
	YGGR_STATIC_CONSTANT(char, E_delta = 'a' - 'A');

	BOOST_MPL_ASSERT((boost::mpl::bool_<(E_delta > 0)>));

	assert((l && r));

	if(l == r)
	{
		return 0;
	}

	size_type i = 0;
	bool ret = false;
	for(; (i != len)
			&& (l[i]) && (r[i]) 
			&& (ret = 
				((l[i] == r[i])
				|| ((l[i] >= 'a' && l[i] <= 'z') && l[i] - E_delta == r[i])
				|| ((l[i] >= 'A' && l[i] <= 'Z') && l[i] + E_delta == r[i]) )); ++i);
	
	assert(i <= len);

	return (ret) && (i == len || l[i] == r[i])? 0 : l[i] < r[i]? -1 : 1;
}

} // namespace charset
} // namespace yggr
