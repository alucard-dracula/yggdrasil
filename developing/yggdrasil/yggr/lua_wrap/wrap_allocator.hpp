//wrap_allocator.hpp

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

#ifndef __YGGR_LUA_WRAP_WRAP_ALLOCATOR_HPP__
#define __YGGR_LUA_WRAP_WRAP_ALLOCATOR_HPP__

#include <yggr/lua_wrap/wrap_base_t_helper.hpp>

namespace yggr
{
namespace lua_wrap
{

template<typename Container>
class wrap_allocator : public Container::alloc_type
{
private:
	typedef Container container_type;

public:
	typedef typename container_type::alloc_type base_type;
	typedef typename base_type::value_type 	value_type; 
	typedef typename base_type::pointer pointer;
	typedef typename base_type::const_pointer const_pointer;
	typedef typename base_type::reference reference;
	typedef typename base_type::const_reference const_reference;
	typedef typename base_type::size_type size_type;
	typedef typename base_type::difference_type difference_type;

private:
	typedef wrap_allocator this_type;
	typedef wrap_base_t_helper<size_type> size_wrap_helper_type;
	
public:
	typedef typename size_wrap_helper_type::wrap_val_type wrap_size_type;

public:
	wrap_allocator(void)
	{
	}

	wrap_allocator(const base_type& right)
		: base_type(right)
	{
	}

	wrap_allocator(const this_type& right)
		: base_type(right)
	{
	}

	~wrap_allocator(void)
	{
	}

	operator base_type&(void)
	{
		return *this;
	}

	operator const base_type&(void) const
	{
		return *this;
	}

	wrap_size_type max_size(void) const
	{
		return base_type::max_size();
	}

	bool operator==(const base_type& right) const
	{
		const base_type& base = *this;
		return base == right;
	}

	bool operator==(const this_type& right) const
	{
		const base_type& l = *this;
		const base_type& r = right;

		return l == r;
	}

	bool operator!=(const base_type& right) const
	{
		return !this_type::operator==(right);
	}

	bool operator!=(const this_type& right) const
	{
		return !this_type::operator==(right);
	}
};

} // namespace lua_wrap
} // namespace yggr


template<typename Container>
bool operator==(const yggr::lua_wrap::wrap_allocator< Container >& l,
				const yggr::lua_wrap::wrap_allocator< Container >& r)
{
	return l == r;
}

template<typename Container>
bool operator!=(const yggr::lua_wrap::wrap_allocator< Container >& l,
				const yggr::lua_wrap::wrap_allocator< Container >& r)
{
	return l != r;
}

#endif // __YGGR_LUA_WRAP_WRAP_ALLOCATOR_HPP__