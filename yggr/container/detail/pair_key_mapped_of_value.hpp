//pair_key_mapped_of_value.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_PAIR_KEY_MAPPED_OF_VALUE_HPP__
#define __YGGR_CONTAINER_DETAIL_PAIR_KEY_MAPPED_OF_VALUE_HPP__

namespace yggr 
{
namespace container 
{
namespace detail
{

template<typename Key, typename Mapped>
struct pair_key_mapped_of_value
{
	typedef Key key_type;
	typedef Mapped mapped_type;

	template<typename Pair> inline
	const key_type& key_of_value(const Pair& p) const
	{  
		return p.first;
	}

	template<typename Pair> inline
	const mapped_type& mapped_of_value(const Pair& p) const
	{  
		return p.second;
	}

	template<typename Pair> inline
	key_type& key_of_value(Pair& p) const
	{ 
		return const_cast<key_type&>(p.first);
	}

	template<typename Pair> inline
	mapped_type& mapped_of_value(Pair& p) const
	{  
		return p.second;
	}

};

} // namespace detail
} // namespace container
} // namespace yggr

#endif   // __YGGR_CONTAINER_DETAIL_PAIR_KEY_MAPPED_OF_VALUE_HPP__

