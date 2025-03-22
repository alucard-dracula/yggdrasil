//uuid.cpp

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

#include <yggr/ids/uuid.hpp>

#include <boost/functional/hash/hash.hpp>

#include <memory>
#include <iomanip>
#include <iterator>
#include <algorithm>
#include <sstream>

#if defined(_MSC_VER)
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace ids
{

uuid::uuid(void)
{
}

uuid::uuid(const this_type& right)
	: base_type(right)
{
}

uuid::~uuid(void)
{
}

uuid::this_type& uuid::operator=(const this_type& right)
{
	base_type::operator=(static_cast<const base_type&>(right));
	return *this;
}

void uuid::swap(this_type& right)
{
	base_type::swap(static_cast<base_type&>(right));
}

std::size_t uuid::hash(void) const
{
	std::size_t seed = 0;
	for(const_iterator i = base_type::begin(), isize = base_type::end(); i != isize; ++i)
	{
		seed ^= static_cast<std::size_t>(*i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}

	return seed;
}

} // namespace ids
} // namespace yggr

