//hmacsha1_tool.cpp

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

#include <yggr/encryption_tool/hmacsha1_tool.hpp>
#include <yggr/algorithm/shuffle.hpp>
#include <yggr/utility/swap.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace encryption_tool
{

hmacsha1_tool::hmacsha1_tool(void)
	: _key_size()
{
	this_type::init();
}

hmacsha1_tool::hmacsha1_tool(const this_type& right)
	: _key_size(right._key_size)
{
	key_type key = construct_shared<value_type>(yggr_nothrow_new value_type[_key_size], key_deleter_type());
	_key.swap(key);

	if(_key_size) memcpy(&(*_key), &(*right._key), _key_size);
}

hmacsha1_tool::~hmacsha1_tool(void)
{
}

hmacsha1_tool::this_type& 
	hmacsha1_tool::operator=(const this_type& right)
{
	if(this == &right) 
	{
		return *this;
	}

	_key_size = right._key_size;
	key_type key = construct_shared<value_type>(yggr_nothrow_new value_type[_key_size], key_deleter_type());
	_key.swap(key);

	if(_key_size) memcpy(_key.get(), right._key.get(), _key_size);
	return *this;
}

void hmacsha1_tool::swap(this_type& right)
{
	if(this == &right)
	{
		return;
	}

	yggr::swap(_key_size, right._key_size);
	_key.swap(right._key);
}

void hmacsha1_tool::init(void)
{
	_key_size = E_default_key_size;
	key_type key = construct_shared<value_type>(yggr_nothrow_new value_type[_key_size], key_deleter_type());
	_key.swap(key);
	for(u32 i = 0; i != _key_size; ++i)
	{
		(_key.get())[i] = random::gen_random<u8>();
	}

	::yggr::shuffle(_key.get(), _key.get() + _key_size);
}

} // namespace encryption_tool
} // namespace yggr


