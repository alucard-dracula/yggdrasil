//wrap_pair.hpp

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

#ifndef __YGGR_LUA_WRAP_WRAP_PAIR_HPP__
#define __YGGR_LUA_WRAP_WRAP_PAIR_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/lua_wrap/wrap_base_t_helper.hpp>
#include <utility>
#include <algorithm>
#include <iostream>

namespace yggr
{
namespace lua_wrap
{

template<typename First, typename Second, 
			template<typename _F, typename _S> class Pair = std::pair>
class wrap_pair : public Pair<First, Second>
{
private:
	typedef First base_f_type;
	typedef Second base_s_type;
public:
	typedef Pair<base_f_type, base_s_type> base_type;
	
private:
	typedef wrap_base_t_helper<base_f_type> f_wrap_helper_type;
	typedef wrap_base_t_helper<base_s_type> s_wrap_helper_type;

public:
	typedef typename f_wrap_helper_type::wrap_val_type first_type;
	typedef typename s_wrap_helper_type::wrap_val_type second_type;

private:
	typedef wrap_pair this_type;

public:
	wrap_pair(void)
	{
		base_type::first = first_type();
		base_type::second = second_type();
	}
	
	wrap_pair(const first_type& nf, const second_type& ns)
	{
		base_type::first = nf;
		base_type::second = ns;
	}

	wrap_pair(const base_type& right)
	{
		base_type::first = right.first;
		base_type::second = right.second;
	}

	wrap_pair(const this_type& right)
	{
		base_type::first = right.first;
		base_type::second = right.second;
	}

	~wrap_pair(void)
	{
	}

	operator base_type&(void)
	{
		base_type& base = *this;
		return base;
	}

	operator const base_type&(void) const
	{
		const base_type& base = *this;
		return base;
	}

	this_type& operator=(const base_type& right)
	{
		base_type::first = right.first;
		base_type::second = right.second;
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) {return *this;}
		first = right.first;
		second = right.second;
		return *this;
	}

	first_type get_first(void) const
	{
		return first_type(base_type::first);
	}

	void set_first(const first_type& val)
	{
		base_type::first = val;
	}

	second_type get_second(void) const
	{
		return second_type(base_type::second);
	}

	void set_second(const second_type& val)
	{
		base_type::second = val;
	}
};

} // namespace lua_wrap
} // namespace yggr

template<typename F, typename S>
std::ostream& operator<<(std::ostream& os, const yggr::lua_wrap::wrap_pair<F, S>& right)
{
	typedef yggr::lua_wrap::wrap_pair<F, S> pair_type;
	typedef typename pair_type::base_type base_type;

	const base_type& o = right;
	os << "[ " << o.first << ", " << o.second << " ]";
	return os;
}


#endif //__YGGR_LUA_WRAP_WRAP_PAIR_HPP__