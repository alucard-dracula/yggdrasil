//wrap_iterator.hpp

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

#ifndef __YGGR_LUA_WRAP_WRAP_ITERATOR_HPP__
#define __YGGR_LUA_WRAP_WRAP_ITERATOR_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/lua_wrap/wrap_base_t_helper.hpp>

namespace yggr
{
namespace lua_wrap
{

//----------------------value_random_access_iterator-------------------------
template<typename Container>
class value_random_access_iterator : public Container::iterator
{
public:
	typedef Container container_type;
	typedef typename container_type::iterator base_type;
	typedef typename container_type::value_type value_type;
	typedef typename base_type::difference_type difference_type;
private:
	typedef wrap_base_t_helper<value_type> val_wrap_helper_type;
	typedef wrap_base_t_helper<difference_type> diff_wrap_helper_type;

public:
	typedef typename val_wrap_helper_type::wrap_val_type wrap_val_type;
	typedef typename diff_wrap_helper_type::wrap_val_type wrap_diff_type;

private:
	typedef value_random_access_iterator this_type;

public:
	value_random_access_iterator(void)
	{
	}

	value_random_access_iterator(const base_type& right)
		: base_type(right)
	{
	}

	value_random_access_iterator(const this_type& right)
		: base_type(right)
	{
	}

	~value_random_access_iterator(void)
	{
	}

	operator base_type&(void)
	{
		base_type& base = *this;
		return base;
	}

	operator const base_type&(void) const
	{
		base_type& base = *this;
		return base;
	}

	template<typename Iter>
	bool operator ==(const Iter& right) const
	{
		typedef Iter iter_type;
		typedef typename iter_type::base_type rbase_type;

		const base_type& base = *this;
		const rbase_type& rbase = right;

		return base == rbase;
	}

	bool operator==(const base_type& right) const
	{
		const base_type& base = *this;
		return base == right;
	}

	bool operator==(const this_type& right) const
	{
		const base_type& lbase = *this, &rbase = right;
		return lbase == rbase;
	}

	template<typename Iter>
	bool operator !=(const Iter& right) const
	{
		typedef Iter iter_type;
		typedef typename iter_type::base_type rbase_type;

		const base_type& base = *this;
		const rbase_type& rbase = right;

		return base != rbase;
	}

	bool operator!=(const base_type& right) const
	{
		const base_type& base = *this;
		return base != right;
	}

	bool operator!=(const this_type& right) const
	{
		const base_type& lbase = *this, &rbase = right;
		return lbase == rbase;
	}

	this_type operator+(const wrap_diff_type& n) const
	{
		return this_type(base_type::operator+(n));
	}

	this_type operator-(const wrap_diff_type& n) const
	{
		return this_type(base_type::operator-(n));
	}

	wrap_diff_type operator-(const base_type& right) const
	{
		const base_type& base = *this;
		return base - right;
	}

	wrap_diff_type operator-(const this_type& right) const
	{
		const base_type& l = *this;
		const base_type& r = right;
		return l - r;
	}

	this_type& next(void)
	{
		base_type& base = *this;
		++base;
		return *this;
	}

	this_type& before(void)
	{
		base_type& base = *this;
		--base;
		return *this;
	}

	const wrap_val_type& get_val(void) const
	{
		const base_type& base = *this;
		return *base;
	}

	void set_val(const wrap_val_type& right)
	{
		base_type& base = *this;
		*base = right;
	}

};

//-------------------value_random_access_const_iterator-----------------------

template<typename Container>
class value_random_access_const_iterator : public Container::const_iterator
{
public:
	typedef Container container_type;
	typedef typename container_type::const_iterator base_type;
	typedef typename container_type::value_type value_type;
	typedef typename base_type::difference_type difference_type;
private:
	typedef wrap_base_t_helper<value_type> val_wrap_helper_type;
	typedef wrap_base_t_helper<difference_type> diff_wrap_helper_type;

public:
	typedef typename val_wrap_helper_type::wrap_val_type wrap_val_type;
	typedef typename diff_wrap_helper_type::wrap_val_type wrap_diff_type;

private:
	typedef value_random_access_const_iterator this_type;

public:
	value_random_access_const_iterator(void)
	{
	}

	value_random_access_const_iterator(const base_type& right)
		: base_type(right)
	{
	}

	value_random_access_const_iterator(const this_type& right)
		: base_type(right)
	{
	}

	~value_random_access_const_iterator(void)
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

	template<typename Iter>
	bool operator ==(const Iter& right) const
	{
		typedef Iter iter_type;
		typedef typename iter_type::base_type rbase_type;

		const base_type& base = *this;
		const rbase_type& rbase = right;

		return base == rbase;
	}

	bool operator==(const base_type& right) const
	{
		const base_type& base = *this;
		return base == right;
	}

	bool operator==(const this_type& right) const
	{
		const base_type& lbase = *this, &rbase = right;
		return lbase == rbase;
	}

	template<typename Iter>
	bool operator !=(const Iter& right) const
	{
		typedef Iter iter_type;
		typedef typename iter_type::base_type rbase_type;

		const base_type& base = *this;
		const rbase_type& rbase = right;

		return base != rbase;
	}

	bool operator!=(const base_type& right) const
	{
		const base_type& base = *this;
		return base != right;
	}

	bool operator!=(const this_type& right) const
	{
		const base_type& lbase = *this, &rbase = right;
		return lbase == rbase;
	}

	this_type operator+(const wrap_diff_type& n) const
	{
		return this_type(base_type::operator+(n));
	}

	this_type operator-(const wrap_diff_type& n) const
	{
		return this_type(base_type::operator-(n));
	}

	wrap_diff_type operator-(const base_type& right) const
	{
		const base_type& base = *this;
		return base - right;
	}

	wrap_diff_type operator-(const this_type& right) const
	{
		const base_type& l = *this;
		const base_type& r = right;
		return l - r;
	}

	this_type& next(void)
	{
		base_type& base = *this;
		++base;
		return *this;
	}

	this_type& before(void)
	{
		base_type& base = *this;
		--base;
		return *this;
	}

	const wrap_val_type& get_val(void) const
	{
		const base_type& base = *this;
		return *base;
	}
};

//----------------------value_random_access_reverse_iterator-------------------------
template<typename Container>
class value_random_access_reverse_iterator : public Container::reverse_iterator
{
public:
	typedef Container container_type;
	typedef typename container_type::reverse_iterator base_type;
	typedef typename container_type::value_type value_type;
	typedef typename base_type::difference_type difference_type;
private:
	typedef wrap_base_t_helper<value_type> val_wrap_helper_type;
	typedef wrap_base_t_helper<difference_type> diff_wrap_helper_type;

public:
	typedef typename val_wrap_helper_type::wrap_val_type wrap_val_type;
	typedef typename diff_wrap_helper_type::wrap_val_type wrap_diff_type;

private:
	typedef value_random_access_reverse_iterator this_type;

public:
	value_random_access_reverse_iterator(void)
	{
	}

	value_random_access_reverse_iterator(const base_type& right)
		: base_type(right)
	{
	}

	value_random_access_reverse_iterator(const this_type& right)
		: base_type(right)
	{
	}

	~value_random_access_reverse_iterator(void)
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

	template<typename Iter>
	bool operator ==(const Iter& right) const
	{
		typedef Iter iter_type;
		typedef typename iter_type::base_type rbase_type;

		const base_type& base = *this;
		const rbase_type& rbase = right;

		return base == rbase;
	}

	bool operator==(const base_type& right) const
	{
		const base_type& base = *this;
		return base == right;
	}

	bool operator==(const this_type& right) const
	{
		const base_type& lbase = *this, &rbase = right;
		return lbase == rbase;
	}

	template<typename Iter>
	bool operator !=(const Iter& right) const
	{
		typedef Iter iter_type;
		typedef typename iter_type::base_type rbase_type;

		const base_type& base = *this;
		const rbase_type& rbase = right;

		return base != rbase;
	}

	bool operator!=(const base_type& right) const
	{
		const base_type& base = *this;
		return base != right;
	}

	bool operator!=(const this_type& right) const
	{
		const base_type& lbase = *this, &rbase = right;
		return lbase == rbase;
	}

	this_type operator+(const wrap_diff_type& n) const
	{
		return this_type(base_type::operator+(n));
	}

	this_type operator-(const wrap_diff_type& n) const
	{
		return this_type(base_type::operator-(n));
	}

	wrap_diff_type operator-(const base_type& right) const
	{
		const base_type& base = *this;
		return base - right;
	}

	wrap_diff_type operator-(const this_type& right) const
	{
		const base_type& l = *this;
		const base_type& r = right;
		return l - r;
	}

	this_type& next(void)
	{
		base_type& base = *this;
		++base;
		return *this;
	}

	this_type& before(void)
	{
		base_type& base = *this;
		--base;
		return *this;
	}

	const wrap_val_type& get_val(void) const
	{
		const base_type& base = *this;
		return *base;
	}

	void set_val(const wrap_val_type& right)
	{
		base_type& base = *this;
		*base = right;
	}

};

//-------------------value_random_access_const_reverse_iterator-----------------------

template<typename Container>
class value_random_access_const_reverse_iterator : public Container::const_reverse_iterator
{
public:
	typedef Container container_type;
	typedef typename container_type::const_reverse_iterator base_type;
	typedef typename container_type::value_type value_type;
	typedef typename base_type::difference_type difference_type;
private:
	typedef wrap_base_t_helper<value_type> val_wrap_helper_type;
	typedef wrap_base_t_helper<difference_type> diff_wrap_helper_type;

public:
	typedef typename val_wrap_helper_type::wrap_val_type wrap_val_type;
	typedef typename diff_wrap_helper_type::wrap_val_type wrap_diff_type;

private:
	typedef value_random_access_const_reverse_iterator this_type;

public:
	value_random_access_const_reverse_iterator(void)
	{
	}

	value_random_access_const_reverse_iterator(const base_type& right)
		: base_type(right)
	{
	}

	value_random_access_const_reverse_iterator(const this_type& right)
		: base_type(right)
	{
	}

	~value_random_access_const_reverse_iterator(void)
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

	template<typename Iter>
	bool operator ==(const Iter& right) const
	{
		typedef Iter iter_type;
		typedef typename iter_type::base_type rbase_type;

		const base_type& base = *this;
		const rbase_type& rbase = right;

		return base == rbase;
	}

	bool operator==(const base_type& right) const
	{
		const base_type& base = *this;
		return base == right;
	}

	bool operator==(const this_type& right) const
	{
		const base_type& lbase = *this, &rbase = right;
		return lbase == rbase;
	}

	template<typename Iter>
	bool operator !=(const Iter& right) const
	{
		typedef Iter iter_type;
		typedef typename iter_type::base_type rbase_type;

		const base_type& base = *this;
		const rbase_type& rbase = right;

		return base != rbase;
	}

	bool operator!=(const base_type& right) const
	{
		const base_type& base = *this;
		return base != right;
	}

	bool operator!=(const this_type& right) const
	{
		const base_type& lbase = *this, &rbase = right;
		return lbase == rbase;
	}

	this_type operator+(const wrap_diff_type& n) const
	{
		return this_type(base_type::operator+(n));
	}

	this_type operator-(const wrap_diff_type& n) const
	{
		return this_type(base_type::operator-(n));
	}

	wrap_diff_type operator-(const base_type& right) const
	{
		const base_type& base = *this;
		return base - right;
	}

	wrap_diff_type operator-(const this_type& right) const
	{
		const base_type& l = *this;
		const base_type& r = right;
		return l - r;
	}

	this_type& next(void)
	{
		base_type& base = *this;
		++base;
		return *this;
	}

	this_type& before(void)
	{
		base_type& base = *this;
		--base;
		return *this;
	}

	const wrap_val_type& get_val(void) const
	{
		const base_type& base = *this;
		return *base;
	}
};

//----------------------value_iterator-------------------------
template<typename Container>
class value_iterator : public Container::iterator
{
public:
	typedef Container container_type;
	typedef typename container_type::iterator base_type;
	typedef typename container_type::value_type value_type;
private:
	typedef wrap_base_t_helper<value_type> val_wrap_helper_type;

public:
	typedef typename val_wrap_helper_type::wrap_val_type wrap_val_type;

private:
	typedef value_iterator this_type;

public:
	value_iterator(void)
	{
	}

	value_iterator(const base_type& right)
		: base_type(right)
	{
	}

	value_iterator(const this_type& right)
		: base_type(right)
	{
	}

	~value_iterator(void)
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

	template<typename Iter>
	bool operator ==(const Iter& right) const
	{
		typedef Iter iter_type;
		typedef typename iter_type::base_type rbase_type;

		const base_type& base = *this;
		const rbase_type& rbase = right;

		return base == rbase;
	}

	bool operator==(const base_type& right) const
	{
		const base_type& base = *this;
		return base == right;
	}

	bool operator==(const this_type& right) const
	{
		const base_type& lbase = *this, &rbase = right;
		return lbase == rbase;
	}

	template<typename Iter>
	bool operator !=(const Iter& right) const
	{
		typedef Iter iter_type;
		typedef typename iter_type::base_type rbase_type;

		const base_type& base = *this;
		const rbase_type& rbase = right;

		return base != rbase;
	}

	bool operator!=(const base_type& right) const
	{
		const base_type& base = *this;
		return base != right;
	}

	bool operator!=(const this_type& right) const
	{
		const base_type& lbase = *this, &rbase = right;
		return lbase == rbase;
	}

	this_type& next(void)
	{
		base_type& base = *this;
		++base;
		return *this;
	}

	this_type& before(void)
	{
		base_type& base = *this;
		--base;
		return *this;
	}

	const wrap_val_type& get_val(void) const
	{
		const base_type& base = *this;
		return *base;
	}

	void set_val(const wrap_val_type& right)
	{
		base_type& base = *this;
		*base = right;
	}

};

//-------------------value_const_iterator-----------------------

template<typename Container>
class value_const_iterator : public Container::const_iterator
{
public:
	typedef Container container_type;
	typedef typename container_type::const_iterator base_type;
	typedef typename container_type::value_type value_type;
private:
	typedef wrap_base_t_helper<value_type> val_wrap_helper_type;

public:
	typedef typename val_wrap_helper_type::wrap_val_type wrap_val_type;

private:
	typedef value_const_iterator this_type;

public:
	value_const_iterator(void)
	{
	}

	value_const_iterator(const base_type& right)
		: base_type(right)
	{
	}

	value_const_iterator(const this_type& right)
		: base_type(right)
	{
	}

	~value_const_iterator(void)
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

	template<typename Iter>
	bool operator ==(const Iter& right) const
	{
		typedef Iter iter_type;
		typedef typename iter_type::base_type rbase_type;

		const base_type& base = *this;
		const rbase_type& rbase = right;

		return base == rbase;
	}

	bool operator==(const base_type& right) const
	{
		const base_type& base = *this;
		return base == right;
	}

	bool operator==(const this_type& right) const
	{
		const base_type& lbase = *this, &rbase = right;
		return lbase == rbase;
	}

	template<typename Iter>
	bool operator !=(const Iter& right) const
	{
		typedef Iter iter_type;
		typedef typename iter_type::base_type rbase_type;

		const base_type& base = *this;
		const rbase_type& rbase = right;

		return base != rbase;
	}

	bool operator!=(const base_type& right) const
	{
		const base_type& base = *this;
		return base != right;
	}

	bool operator!=(const this_type& right) const
	{
		const base_type& lbase = *this, &rbase = right;
		return lbase == rbase;
	}

	this_type& next(void)
	{
		base_type& base = *this;
		++base;
		return *this;
	}

	this_type& before(void)
	{
		base_type& base = *this;
		--base;
		return *this;
	}

	const wrap_val_type& get_val(void) const
	{
		const base_type& base = *this;
		return *base;
	}
};

//----------------value_reverse_iterator----------
template<typename Container>
class value_reverse_iterator : public Container::reverse_iterator
{
public:
	typedef Container container_type;
	typedef typename container_type::reverse_iterator base_type;
	typedef typename container_type::value_type value_type;
private:
	typedef wrap_base_t_helper<value_type> val_wrap_helper_type;

public:
	typedef typename val_wrap_helper_type::wrap_val_type wrap_val_type;

private:
	typedef value_reverse_iterator this_type;

public:
	value_reverse_iterator(void)
	{
	}

	value_reverse_iterator(const base_type& right)
		: base_type(right)
	{
	}

	value_reverse_iterator(const this_type& right)
		: base_type(right)
	{
	}

	~value_reverse_iterator(void)
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

	template<typename Iter>
	bool operator==(const Iter& right) const
	{
		typedef Iter iter_type;
		typedef typename iter_type::base_type rbase_type;

		const base_type& base = *this;
		const rbase_type& rbase = right;

		return base == rbase;
	}

	bool operator==(const base_type& right) const
	{
		const base_type& base = *this;
		return base == right;
	}

	bool operator==(const this_type& right) const
	{
		const base_type& lbase = *this, &rbase = right;
		return lbase == rbase;
	}

	template<typename Iter>
	bool operator !=(const Iter& right) const
	{
		typedef Iter iter_type;
		typedef typename iter_type::base_type rbase_type;

		const base_type& base = *this;
		const rbase_type& rbase = right;

		return base != rbase;
	}

	bool operator!=(const base_type& right) const
	{
		const base_type& base = *this;
		return base != right;
	}

	bool operator!=(const this_type& right) const
	{
		const base_type& lbase = *this, &rbase = right;
		return lbase == rbase;
	}

	this_type& next(void)
	{
		base_type& base = *this;
		++base;
		return *this;
	}

	this_type& before(void)
	{
		base_type& base = *this;
		--base;
		return *this;
	}

	const wrap_val_type& get_val(void) const
	{
		const base_type& base = *this;
		return *base;
	}

	void set_val(const wrap_val_type& right)
	{
		base_type& base = *this;
		*base = right;
	}
};

//----------------value_reverse_iterator----------
template<typename Container>
class value_const_reverse_iterator : public Container::const_reverse_iterator
{
public:
	typedef Container container_type;
	typedef typename container_type::const_reverse_iterator base_type;
	typedef typename container_type::value_type value_type;
private:
	typedef wrap_base_t_helper<value_type> val_wrap_helper_type;

public:
	typedef typename val_wrap_helper_type::wrap_val_type wrap_val_type;

private:
	typedef value_const_reverse_iterator this_type;

public:
	value_const_reverse_iterator(void)
	{
	}

	value_const_reverse_iterator(const base_type& right)
		: base_type(right)
	{
	}

	value_const_reverse_iterator(const this_type& right)
		: base_type(right)
	{
	}

	~value_const_reverse_iterator(void)
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

	template<typename Iter>
	bool operator==(const Iter& right) const
	{
		typedef Iter iter_type;
		typedef typename iter_type::base_type rbase_type;

		const base_type& base = *this;
		const rbase_type& rbase = right;

		return base == rbase;
	}

	bool operator==(const base_type& right) const
	{
		const base_type& base = *this;
		return base == right;
	}

	bool operator==(const this_type& right) const
	{
		const base_type& lbase = *this, &rbase = right;
		return lbase == rbase;
	}

	template<typename Iter>
	bool operator !=(const Iter& right) const
	{
		typedef Iter iter_type;
		typedef typename iter_type::base_type rbase_type;

		const base_type& base = *this;
		const rbase_type& rbase = right;

		return base != rbase;
	}

	bool operator!=(const base_type& right) const
	{
		const base_type& base = *this;
		return base != right;
	}

	bool operator!=(const this_type& right) const
	{
		const base_type& lbase = *this, &rbase = right;
		return lbase == rbase;
	}

	this_type& next(void)
	{
		base_type& base = *this;
		++base;
		return *this;
	}

	this_type& before(void)
	{
		base_type& base = *this;
		--base;
		return *this;
	}

	const wrap_val_type& get_val(void) const
	{
		const base_type& base = *this;
		return *base;
	}

	void set_val(const wrap_val_type& right)
	{
		base_type& base = *this;
		*base = right;
	}

};

//----------------key_value_iterator------------------

template<typename Container>
class key_value_iterator : public Container::iterator
{
public:
	typedef Container container_type;
	typedef typename container_type::iterator base_type;
	typedef typename container_type::key_type key_type;
	typedef typename container_type::value_type value_type;
	typedef typename value_type::second_type val_type;
private:
	typedef wrap_base_t_helper<key_type> key_wrap_helper_type;
	typedef wrap_base_t_helper<val_type> val_wrap_helper_type;
	typedef wrap_base_t_helper<value_type> value_wrap_helper_type;

public:
	typedef typename key_wrap_helper_type::wrap_val_type wrap_key_type;
	typedef typename val_wrap_helper_type::wrap_val_type wrap_val_type;
	typedef typename value_wrap_helper_type::wrap_val_type wrap_value_type;

private:
	typedef key_value_iterator this_type;

public:
	key_value_iterator(void)
	{
	}

	key_value_iterator(const base_type& right)
		: base_type(right)
	{
	}

	key_value_iterator(const this_type& right)
		: base_type(right)
	{
	}

	~key_value_iterator(void)
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

	template<typename Iter>
	bool operator ==(const Iter& right) const
	{
		typedef Iter iter_type;
		typedef typename iter_type::base_type rbase_type;

		const base_type& base = *this;
		const rbase_type& rbase = right;

		return base == rbase;
	}

	bool operator==(const base_type& right) const
	{
		const base_type& base = *this;
		return base == right;
	}

	bool operator==(const this_type& right) const
	{
		const base_type& lbase = *this, &rbase = right;
		return lbase == rbase;
	}

	template<typename Iter>
	bool operator !=(const Iter& right) const
	{
		typedef Iter iter_type;
		typedef typename iter_type::base_type rbase_type;

		const base_type& base = *this;
		const rbase_type& rbase = right;

		return base != rbase;
	}

	bool operator!=(const base_type& right) const
	{
		const base_type& base = *this;
		return base != right;
	}

	bool operator!=(const this_type& right) const
	{
		const base_type& lbase = *this, &rbase = right;
		return lbase == rbase;
	}

	this_type& next(void)
	{
		base_type& base = *this;
		++base;
		return *this;
	}

	this_type& before(void)
	{
		base_type& base = *this;
		--base;
		return *this;
	}

	const wrap_value_type& get(void) const
	{
		const base_type& base = *this;
		return *base;
	}

	const wrap_key_type& get_key(void) const
	{
		const base_type& base = *this;
		return base->first;
	}

	const wrap_val_type& get_val(void) const
	{
		const base_type& base = *this;
		return base->second;
	}

	void set_val(const wrap_val_type& right)
	{
		base_type& base = *this;
		base->second = right;
	}
};

//----------------------key_value_const_iterator------------

template<typename Container>
class key_value_const_iterator : public Container::const_iterator
{
public:
	typedef Container container_type;
	typedef typename container_type::const_iterator base_type;
	typedef typename container_type::key_type key_type;
	typedef typename container_type::value_type value_type;
	typedef typename value_type::second_type val_type;
private:
	typedef wrap_base_t_helper<key_type> key_wrap_helper_type;
	typedef wrap_base_t_helper<val_type> val_wrap_helper_type;
	typedef wrap_base_t_helper<value_type> value_wrap_helper_type;

public:
	typedef typename key_wrap_helper_type::wrap_val_type wrap_key_type;
	typedef typename val_wrap_helper_type::wrap_val_type wrap_val_type;
	typedef typename value_wrap_helper_type::wrap_val_type wrap_value_type;

private:
	typedef key_value_const_iterator this_type;

public:
	key_value_const_iterator(void)
	{
	}

	key_value_const_iterator(const base_type& right)
		: base_type(right)
	{
	}

	key_value_const_iterator(const this_type& right)
		: base_type(right)
	{
	}

	~key_value_const_iterator(void)
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

	template<typename Iter>
	bool operator ==(const Iter& right) const
	{
		typedef Iter iter_type;
		typedef typename iter_type::base_type rbase_type;

		const base_type& base = *this;
		const rbase_type& rbase = right;

		return base == rbase;
	}

	bool operator==(const base_type& right) const
	{
		const base_type& base = *this;
		return base == right;
	}

	bool operator==(const this_type& right) const
	{
		const base_type& lbase = *this, &rbase = right;
		return lbase == rbase;
	}

	template<typename Iter>
	bool operator !=(const Iter& right) const
	{
		typedef Iter iter_type;
		typedef typename iter_type::base_type rbase_type;

		const base_type& base = *this;
		const rbase_type& rbase = right;

		return base != rbase;
	}

	bool operator!=(const base_type& right) const
	{
		const base_type& base = *this;
		return base != right;
	}

	bool operator!=(const this_type& right) const
	{
		const base_type& lbase = *this, &rbase = right;
		return lbase == rbase;
	}

	this_type& next(void)
	{
		base_type& base = *this;
		++base;
		return *this;
	}

	this_type& before(void)
	{
		base_type& base = *this;
		--base;
		return *this;
	}

	const wrap_value_type& get(void) const
	{
		const base_type& base = *this;
		return *base;
	}

	const wrap_key_type& get_key(void) const
	{
		const base_type& base = *this;
		return base->first;
	}

	const wrap_val_type& get_val(void) const
	{
		const base_type& base = *this;
		return base->second;
	}

	void set_val(const wrap_val_type& right)
	{
		base_type& base = *this;
		base->second = right;
	}
};

//----------------key_value_reverse_iterator------------------

template<typename Container>
class key_value_reverse_iterator : public Container::reverse_iterator
{
public:
	typedef Container container_type;
	typedef typename container_type::reverse_iterator base_type;
	typedef typename container_type::key_type key_type;
	typedef typename container_type::value_type value_type;
	typedef typename value_type::second_type val_type;
private:
	typedef wrap_base_t_helper<key_type> key_wrap_helper_type;
	typedef wrap_base_t_helper<val_type> val_wrap_helper_type;
	typedef wrap_base_t_helper<value_type> value_wrap_helper_type;

public:
	typedef typename key_wrap_helper_type::wrap_val_type wrap_key_type;
	typedef typename val_wrap_helper_type::wrap_val_type wrap_val_type;
	typedef typename value_wrap_helper_type::wrap_val_type wrap_value_type;

private:
	typedef key_value_reverse_iterator this_type;

public:
	key_value_reverse_iterator(void)
	{
	}

	key_value_reverse_iterator(const base_type& right)
		: base_type(right)
	{
	}

	key_value_reverse_iterator(const this_type& right)
		: base_type(right)
	{
	}

	~key_value_reverse_iterator(void)
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

	template<typename Iter>
	bool operator ==(const Iter& right) const
	{
		typedef Iter iter_type;
		typedef typename iter_type::base_type rbase_type;

		const base_type& base = *this;
		const rbase_type& rbase = right;

		return base == rbase;
	}

	bool operator==(const base_type& right) const
	{
		const base_type& base = *this;
		return base == right;
	}

	bool operator==(const this_type& right) const
	{
		const base_type& lbase = *this, &rbase = right;
		return lbase == rbase;
	}

	template<typename Iter>
	bool operator !=(const Iter& right) const
	{
		typedef Iter iter_type;
		typedef typename iter_type::base_type rbase_type;

		const base_type& base = *this;
		const rbase_type& rbase = right;

		return base != rbase;
	}

	bool operator!=(const base_type& right) const
	{
		const base_type& base = *this;
		return base != right;
	}

	bool operator!=(const this_type& right) const
	{
		const base_type& lbase = *this, &rbase = right;
		return lbase == rbase;
	}

	this_type& next(void)
	{
		base_type& base = *this;
		++base;
		return *this;
	}

	this_type& before(void)
	{
		base_type& base = *this;
		--base;
		return *this;
	}

	const wrap_value_type& get(void) const
	{
		const base_type& base = *this;
		return *this;
	}

	const wrap_key_type& get_key(void) const
	{
		const base_type& base = *this;
		return base->first;
	}

	const wrap_val_type& get_val(void) const
	{
		const base_type& base = *this;
		return base->second;
	}

	void set_val(const wrap_val_type& right)
	{
		base_type& base = *this;
		base->second = right;
	}
};

//----------------------key_value_const_reverse_iterator------------

template<typename Container>
class key_value_const_reverse_iterator : public Container::const_reverse_iterator
{
public:
	typedef Container container_type;
	typedef typename container_type::const_reverse_iterator base_type;
	typedef typename container_type::key_type key_type;
	typedef typename container_type::value_type value_type;
	typedef typename value_type::second_type val_type;
private:
	typedef wrap_base_t_helper<key_type> key_wrap_helper_type;
	typedef wrap_base_t_helper<val_type> val_wrap_helper_type;
	typedef wrap_base_t_helper<value_type> value_wrap_helper_type;

public:
	typedef typename key_wrap_helper_type::wrap_val_type wrap_key_type;
	typedef typename val_wrap_helper_type::wrap_val_type wrap_val_type;
	typedef typename value_wrap_helper_type::wrap_val_type wrap_value_type;

private:
	typedef key_value_const_reverse_iterator this_type;

public:
	key_value_const_reverse_iterator(void)
	{
	}

	key_value_const_reverse_iterator(const base_type& right)
		: base_type(right)
	{
	}

	key_value_const_reverse_iterator(const this_type& right)
		: base_type(right)
	{
	}

	~key_value_const_reverse_iterator(void)
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

	template<typename Iter>
	bool operator ==(const Iter& right) const
	{
		typedef Iter iter_type;
		typedef typename iter_type::base_type rbase_type;

		const base_type& base = *this;
		const rbase_type& rbase = right;

		return base == rbase;
	}

	bool operator==(const base_type& right) const
	{
		const base_type& base = *this;
		return base == right;
	}

	bool operator==(const this_type& right) const
	{
		const base_type& lbase = *this, &rbase = right;
		return lbase == rbase;
	}

	template<typename Iter>
	bool operator !=(const Iter& right) const
	{
		typedef Iter iter_type;
		typedef typename iter_type::base_type rbase_type;

		const base_type& base = *this;
		const rbase_type& rbase = right;

		return base != rbase;
	}

	bool operator!=(const base_type& right) const
	{
		const base_type& base = *this;
		return base != right;
	}

	bool operator!=(const this_type& right) const
	{
		const base_type& lbase = *this, &rbase = right;
		return lbase == rbase;
	}

	this_type& next(void)
	{
		base_type& base = *this;
		++base;
		return *this;
	}

	this_type& before(void)
	{
		base_type& base = *this;
		--base;
		return *this;
	}

	const wrap_value_type& get(void) const
	{
		const base_type& base = *this;
		return *base;
	}

	const wrap_key_type& get_key(void) const
	{
		const base_type& base = *this;
		return base->first;
	}

	const wrap_val_type& get_val(void) const
	{
		const base_type& base = *this;
		return base->second;
	}

	void set_val(const wrap_val_type& right)
	{
		base_type& base = *this;
		base->second = right;
	}
};

} // namespace lua_wrap
} // namespace yggr

template<typename Container>
std::ostream& operator<<(std::ostream& os, const yggr::lua_wrap::value_random_access_iterator<Container>& right)
{
//	typedef Container container_type;
//	typedef yggr::lua_wrap::value_random_access_iterator<container_type> iter_type;
//	typedef typename iter_type::base_type iter_base_type;
//	const iter_base_type& base = right;

	os << &right;
	return os;
}

template<typename Container>
std::ostream& operator<<(std::ostream& os, const yggr::lua_wrap::value_random_access_const_iterator<Container>& right)
{
//	typedef Container container_type;
//	typedef yggr::lua_wrap::value_random_access_const_iterator<container_type> citer_type;
//	typedef typename citer_type::base_type citer_base_type;
//	const citer_base_type& base = right;

	os << &right;
	return os;
}

template<typename Container>
std::ostream& operator<<(std::ostream& os, const yggr::lua_wrap::value_random_access_reverse_iterator<Container>& right)
{
//	typedef Container container_type;
//	typedef yggr::lua_wrap::value_random_access_reverse_iterator<container_type> iter_type;
//	typedef typename iter_type::base_type iter_base_type;
//	const iter_base_type& base = right;

	os << &right;
	return os;
}

template<typename Container>
std::ostream& operator<<(std::ostream& os, const yggr::lua_wrap::value_random_access_const_reverse_iterator<Container>& right)
{
//	typedef Container container_type;
//	typedef yggr::lua_wrap::value_random_access_const_reverse_iterator<container_type> citer_type;
//	typedef typename citer_type::base_type citer_base_type;
//	const citer_base_type& base = right;

	os << &right;
	return os;
}

template<typename Container>
std::ostream& operator<<(std::ostream& os, const yggr::lua_wrap::value_iterator<Container>& right)
{
//	typedef Container container_type;
//	typedef yggr::lua_wrap::value_iterator<container_type> iter_type;
//	typedef typename iter_type::base_type iter_base_type;
//	const iter_base_type& base = right;

	os << &right;
	return os;
}

template<typename Container>
std::ostream& operator<<(std::ostream& os, const yggr::lua_wrap::value_const_iterator<Container>& right)
{
//	typedef Container container_type;
//	typedef yggr::lua_wrap::value_const_iterator<container_type> citer_type;
//	typedef typename citer_type::base_type citer_base_type;
//	const citer_base_type& base = right;
	os << &right;
	return os;
}

template<typename Container>
std::ostream& operator<<(std::ostream& os, const yggr::lua_wrap::value_reverse_iterator<Container>& right)
{
//	typedef Container container_type;
//	typedef yggr::lua_wrap::value_reverse_iterator<container_type> iter_type;
//	typedef typename iter_type::base_type iter_base_type;
//	const iter_base_type& base = right;

	os << &right;
	return os;
}

template<typename Container>
std::ostream& operator<<(std::ostream& os, const yggr::lua_wrap::value_const_reverse_iterator<Container>& right)
{
//	typedef Container container_type;
//	typedef yggr::lua_wrap::value_const_reverse_iterator<container_type> citer_type;
//	typedef typename citer_type::base_type citer_base_type;
//	const citer_base_type& base = right;
	os << &right;
	return os;
}

template<typename Container>
std::ostream& operator<<(std::ostream& os, const yggr::lua_wrap::key_value_iterator<Container>& right)
{
//	typedef Container container_type;
//	typedef yggr::lua_wrap::key_value_iterator<container_type> citer_type;
//	typedef typename citer_type::base_type citer_base_type;
//	const citer_base_type& base = right;
	os << &right;
	return os;
}

template<typename Container>
std::ostream& operator<<(std::ostream& os, const yggr::lua_wrap::key_value_const_iterator<Container>& right)
{
//	typedef Container container_type;
//	typedef yggr::lua_wrap::key_value_const_iterator<container_type> citer_type;
//	typedef typename citer_type::base_type citer_base_type;
//	const citer_base_type& base = right;
	os << &right;
	return os;
}

template<typename Container>
std::ostream& operator<<(std::ostream& os, const yggr::lua_wrap::key_value_reverse_iterator<Container>& right)
{
//	typedef Container container_type;
//	typedef yggr::lua_wrap::key_value_reverse_iterator<container_type> citer_type;
//	typedef typename citer_type::base_type citer_base_type;
//	const citer_base_type& base = right;
	os << &right;
	return os;
}

template<typename Container>
std::ostream& operator<<(std::ostream& os, const yggr::lua_wrap::key_value_const_reverse_iterator<Container>& right)
{
//	typedef Container container_type;
//	typedef yggr::lua_wrap::key_value_const_reverse_iterator<container_type> citer_type;
//	typedef typename citer_type::base_type citer_base_type;
//	const citer_base_type& base = right;
	os << &right;
	return os;
}

#endif //__YGGR_LUA_WRAP_WRAP_ITERATOR_HPP__
