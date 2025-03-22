//wrap_random_access_iterator.hpp

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

#ifndef __YGGR_LUA_WRAP_WRAP_RANDOM_ACCESS_ITERATOR_HPP__
#define __YGGR_LUA_WRAP_WRAP_RANDOM_ACCESS_ITERATOR_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/utility/copy_or_move_or_swap.hpp>

#include <yggr/mplex/static_assert.hpp>
#include <yggr/mplex/tag_sfinae.hpp>

#include <yggr/iterator_ex/iterator_traits.hpp>
#include <yggr/iterator_ex/iterator.hpp>
#include <yggr/iterator_ex/iterator_category_check.hpp>

#include <yggr/lua_wrap/wrap_base_t.hpp>
#include <yggr/lua_wrap/detail/luabind_function_noexcept.hpp>
#include <yggr/lua_wrap/detail/unwrap_t.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_class.hpp>

#include <ostream>

namespace yggr
{
namespace lua_wrap
{

template<typename TIter>
class wrap_random_access_iterator;

template<typename TIter>
class wrap_random_access_const_iterator;

} // namespace lua_wrap
} // namespace yggr

namespace yggr
{
namespace lua_wrap
{
namespace detail
{

template<typename TIter>
struct is_wrap_random_access_iterator
	: public boost::mpl::false_
{
};

template<typename TIter>
struct is_wrap_random_access_iterator< wrap_random_access_iterator<TIter> >
	: public boost::mpl::true_
{
};

template<typename TIter>
struct is_wrap_random_access_const_iterator
	: public boost::mpl::false_
{
};

template<typename TIter>
struct is_wrap_random_access_const_iterator< wrap_random_access_const_iterator<TIter> >
	: public boost::mpl::true_
{
};

} // namespace detail
} // namespace lua_wrap
} // namespace yggr


namespace yggr
{
namespace lua_wrap
{

//----------------------wrap_random_access_iterator-------------------------
template<typename TIter>
class wrap_random_access_iterator
{
private:
	BOOST_MPL_ASSERT((iterator_ex::is_random_access_iterator<TIter>));

public:
	typedef TIter base_type;
	typedef typename iterator_ex::iterator_value_ex<base_type>::type value_type;
	typedef typename iterator_ex::iterator_category_ex<base_type>::type iterator_category;
	typedef typename iterator_ex::iterator_pointer_ex<base_type>::type pointer;
	typedef typename iterator_ex::iterator_difference_ex<base_type>::type difference_type;
	typedef typename iterator_ex::iterator_reference_ex<base_type>::type reference;

private:
	typedef wrap_random_access_iterator this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	YGGR_CONSTEXPR wrap_random_access_iterator(void)
	{
	}

	YGGR_CONSTEXPR wrap_random_access_iterator(BOOST_RV_REF(base_type) right)
		: _base(boost::move(right))
	{
	}

	YGGR_CONSTEXPR wrap_random_access_iterator(const base_type& right)
		: _base(right)
	{
	}

	YGGR_CONSTEXPR wrap_random_access_iterator(BOOST_RV_REF(this_type) right)
		: _base(boost::move(right._base))
	{
	}

	YGGR_CONSTEXPR wrap_random_access_iterator(const this_type& right)
		: _base(right._base)
	{
	}

	YGGR_CXX20_CONSTEXPR ~wrap_random_access_iterator(void)
	{
	}

public:
	YGGR_CXX14_CONSTEXPR_OR_INLINE this_type& operator=(BOOST_RV_REF(base_type) right)
	{
		base_type& right_ref = right;
		if(boost::addressof(_base) == boost::addressof(right_ref))
		{
			return *this;
		}

		copy_or_move_or_swap(_base, boost::move(right));
		return *this;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE this_type& operator=(const base_type& right)
	{
		if(boost::addressof(_base) == boost::addressof(right))
		{
			return *this;
		}

		_base = right;
		return *this;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		if(this == boost::addressof(right_ref))
		{
			return *this;
		}

		copy_or_move_or_swap(_base, boost::move(right._base));
		return *this;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE this_type& operator=(const this_type& right)
	{
		if(this == boost::addressof(right))
		{
			return *this;
		}

		_base = right._base;
		return *this;
	}

public:
	YGGR_CXX14_CONSTEXPR_OR_INLINE void swap(BOOST_RV_REF(base_type) right)
	{
		base_type& right_ref = right;
		this_type::swap(right_ref);
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE void swap(base_type& right)
	{
		yggr::swap(_base, right);
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE void swap(this_type& right)
	{
		if(this == boost::addressof(right))
		{
			return;
		}
		yggr::swap(_base, right._base);
	}

public:
	YGGR_CXX14_CONSTEXPR_OR_INLINE operator base_type&(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _base;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE operator const base_type&(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _base;
	}

public:
	YGGR_CXX14_CONSTEXPR_OR_INLINE base_type& unwrap(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _base;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE const base_type& unwrap(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _base;
	}

public:
	YGGR_CXX14_CONSTEXPR_OR_INLINE this_type& set_iterator(const base_type& right)
	{
		_base = right;
		return *this;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE this_type& set_iterator(const this_type& right)
	{
		if(this == boost::addressof(right))
		{
			return *this;
		}

		_base = right._base;
		return *this;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE this_type copy(void) const YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return *this;
	}

public:
	// don't exprot at, export get_value set_value
	template<typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			reference
		>::type
		at(T n) const
	{
		return _base[static_cast<difference_type>(n)];
	}

	// don't exprot at, export get_value set_value
	template<typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<T>,
			reference
		>::type
		at(const T& n) const
	{
		return _base[static_cast<difference_type>(n.value())];
	}

public:
	YGGR_CONSTEXPR_OR_INLINE typename boost::mpl::if_<boost::is_class<value_type>, reference, value_type >::type
		get_value(void) const
	{
		return *_base;
	}

	template<typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			typename boost::mpl::if_<boost::is_class<value_type>, reference, value_type >::type
		>::type
		get_value(T n) const
	{
		return this_type::at(n);
	}

	template<typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<T>,
			typename boost::mpl::if_<boost::is_class<value_type>, reference, value_type >::type
		>::type
		get_value(const T& n) const
	{
		return this_type::at(n);
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE this_type& set_value(const value_type& val) const
	{
		*_base = val;
		return const_cast<this_type&>(*this);
	}

	template<typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			this_type&
		>::type
		set_value(T n, const value_type& val) const
	{
		this_type::at(n) = val;
		return const_cast<this_type&>(*this);
	}

	template<typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<T>,
			this_type&
		>::type
		set_value(const T& n, const value_type& val) const
	{
		this_type::at(n) = val;
		return const_cast<this_type&>(*this);
	}

public:
	YGGR_CXX14_CONSTEXPR_OR_INLINE this_type& head_increment(void)
	{
		iterator_ex::advance(_base, static_cast<difference_type>(1));
		return *this;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE this_type tail_increment(void)
	{
		this_type ret(_base);
		iterator_ex::advance(_base, static_cast<difference_type>(1));
		return ret;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE this_type& head_decrement(void)
	{
		iterator_ex::advance(_base, -static_cast<difference_type>(1));
		return *this;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE this_type tail_decrement(void)
	{
		this_type ret(_base);
		iterator_ex::advance(_base, -static_cast<difference_type>(1));
		return ret;
	}

	template<typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			this_type
		>::type
		plus(T n) const
	{
		return this_type(iterator_ex::advance_copy(_base, static_cast<difference_type>(n)));
	}

	template<typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<T>,
			this_type
		>::type
		plus(const T& n) const
	{
		return this_type(iterator_ex::advance_copy(_base, static_cast<difference_type>(n.value())));
	}

	template<typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			this_type&
		>::type
		plus_set(T n)
	{
		iterator_ex::advance(_base, static_cast<difference_type>(n));
		return *this;
	}

	template<typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<T>,
			this_type&
		>::type
		plus_set(const T& n)
	{
		iterator_ex::advance(_base, static_cast<difference_type>(n.value()));
		return *this;
	}

	template<typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			this_type
		>::type
		minus(T n) const
	{
		return this_type(iterator_ex::advance_copy(_base, -static_cast<difference_type>(n)));
	}

	template<typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<T>,
			this_type
		>::type
		minus(const T& n) const
	{
		return this_type(iterator_ex::advance_copy(_base, -static_cast<difference_type>(n.value())));
	}

	template<typename Iter> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			iterator_ex::is_random_access_iterator<Iter>,
			difference_type
		>::type
		minus(const Iter& right) const
	{
		return
			-iterator_ex::distance<typename detail::unwrap_t<Iter>::type>(
				_base, detail::unwrap_object(right));
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE difference_type minus(const base_type& right) const
	{
		return -iterator_ex::distance(_base, right);
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE difference_type minus(const this_type& right) const
	{
		return -iterator_ex::distance(_base, right._base);
	}

	template<typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			this_type&
		>::type
		minus_set(T n)
	{
		iterator_ex::advance(_base, -static_cast<difference_type>(n));
		return *this;
	}

	template<typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<T>,
			this_type&
		>::type
		minus_set(const T& n)
	{
		iterator_ex::advance(_base, -static_cast<difference_type>(n.value()));
		return *this;
	}

	template<typename Iter> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			iterator_ex::is_random_access_iterator<Iter>,
			typename wrap_base_t_maker<difference_type>::type
		>::type
		wraped_minus(const Iter& right) const
	{
		typedef typename wrap_base_t_maker<difference_type>::type wrap_diff_type;

		return wrap_diff_type(this_type::minus(right));
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE typename wrap_base_t_maker<difference_type>::type
		wraped_minus(const base_type& right) const
	{
		typedef typename wrap_base_t_maker<difference_type>::type wrap_diff_type;

		return wrap_diff_type(this_type::minus(right));
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE typename wrap_base_t_maker<difference_type>::type
		wraped_minus(const this_type& right) const
	{
		typedef typename wrap_base_t_maker<difference_type>::type wrap_diff_type;

		return wrap_diff_type(this_type::minus(right));
	}

public:
	template<typename Iter> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			iterator_ex::is_random_access_iterator<Iter>,
			bool
		>::type
		compare_eq(const Iter& right) const
	{
		return _base == detail::unwrap_object(right);
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE bool compare_eq(const base_type& right) const
	{
		return _base == right;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE bool compare_eq(const this_type& right) const
	{
		return _base == right.unwrap();
	}

	template<typename Iter> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			iterator_ex::is_random_access_iterator<Iter>,
			s32
		>::type
		compare(const Iter& right) const
	{
		difference_type diff = this_type::minus(right);
		return diff == 0? 0 : diff < 0? -1 : 1;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE s32 compare(const base_type& right) const
	{
		difference_type diff = this_type::minus(right);
		return diff == 0? 0 : diff < 0? -1 : 1;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE s32 compare(const this_type& right) const
	{
		difference_type diff = this_type::minus(right.unwrap());
		return diff == 0? 0 : diff < 0? -1 : 1;
	}

public:
	YGGR_CXX14_CONSTEXPR_OR_INLINE this_type& next(void)
	{
		return this_type::head_increment();
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE this_type& prev(void)
	{
		return this_type::head_decrement();
	}

public:
	template<typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			reference
		>::type
		operator[](T n) const
	{
		return this_type::at(n);
	}

	template<typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<T>,
			reference
		>::type
		operator[](const T& n) const
	{
		return this_type::at(n);
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE pointer operator->(void) const
	{
		return _base.operator->();
	}

protected:
	base_type _base;
};

// non-member foo
// *
template<typename L> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename wrap_random_access_iterator<L>::reference
	operator*(const wrap_random_access_iterator<L>& l)
{
	return *(l.unwrap());
}

// ++
template<typename L> YGGR_CXX14_CONSTEXPR_OR_INLINE
wrap_random_access_iterator<L>&
	operator++(wrap_random_access_iterator<L>& l)
{
	return l.head_increment();
}

template<typename L> YGGR_CXX14_CONSTEXPR_OR_INLINE
wrap_random_access_iterator<L>
	operator++(wrap_random_access_iterator<L>& l, int)
{
	return l.tail_increment();
}

//--
template<typename L> YGGR_CXX14_CONSTEXPR_OR_INLINE
wrap_random_access_iterator<L>&
	operator--(wrap_random_access_iterator<L>& l)
{
	return l.head_decrement();
}

template<typename L> YGGR_CXX14_CONSTEXPR_OR_INLINE
wrap_random_access_iterator<L>
	operator--(wrap_random_access_iterator<L>& l, int)
{
	return l.tail_decrement();
}

// +
template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		boost::is_arithmetic<L>,
		wrap_random_access_iterator<R>
	>::type
	operator+(L l, const wrap_random_access_iterator<R>& r)
{
	return r.plus(l);
}

template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		detail::is_wrap_base_t<L>,
		wrap_random_access_iterator<R>
	>::type
	operator+(const L& l, const wrap_random_access_iterator<R>& r)
{
	return r.plus(l);
}

template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		boost::is_arithmetic<R>,
		wrap_random_access_iterator<L>
	>::type
	operator+(const wrap_random_access_iterator<L>& l, R r)
{
	return l.plus(r);
}

template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		detail::is_wrap_base_t<R>,
		wrap_random_access_iterator<L>
	>::type
	operator+(const wrap_random_access_iterator<L>& l, const R& r)
{
	return l.plus(r);
}

// +=
template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		boost::is_arithmetic<R>,
		wrap_random_access_iterator<L>&
	>::type
	operator+=(wrap_random_access_iterator<L>& l, R r)
{
	return l.plus_set(r);
}

template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		detail::is_wrap_base_t<R>,
		wrap_random_access_iterator<L>&
	>::type
	operator+=(wrap_random_access_iterator<L>& l, const R& r)
{
	return l.plus_set(r);
}

// - diff
template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		boost::is_arithmetic<R>,
		wrap_random_access_iterator<L>
	>::type
	operator-(const wrap_random_access_iterator<L>& l, R r)
{
	return l.minus(r);
}

template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		detail::is_wrap_base_t<R>,
		wrap_random_access_iterator<L>
	>::type
	operator-(const wrap_random_access_iterator<L>& l, const R& r)
{
	return l.minus(r);
}

// -iter
template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		iterator_ex::is_random_access_iterator<L>,
		typename wrap_random_access_iterator<R>::difference_type
	>::type
	operator-(const L& l, const wrap_random_access_iterator<R>& r)
{
	return -r.minus(l);
}

template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		iterator_ex::is_random_access_iterator<R>,
		typename wrap_random_access_iterator<L>::difference_type
	>::type
	operator-(const wrap_random_access_iterator<L>& l, const R& r)
{
	return l.minus(r);
}

template<typename TIter> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename wrap_random_access_iterator<TIter>::difference_type
	operator-(const wrap_random_access_iterator<TIter>& l,
				const wrap_random_access_iterator<TIter>& r)
{
	return l.minus(r);
}

template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename wrap_random_access_iterator<L>::difference_type
	operator-(const wrap_random_access_iterator<L>& l,
				const wrap_random_access_const_iterator<R>& r)
{
	return l.minus(r);
}

// -=
template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		boost::is_arithmetic<R>,
		wrap_random_access_iterator<L>&
	>::type
	operator-=(wrap_random_access_iterator<L>& l, R r)
{
	return l.minus_set(r);
}

template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		detail::is_wrap_base_t<R>,
		wrap_random_access_iterator<L>&
	>::type
	operator-=(wrap_random_access_iterator<L>& l, const R& r)
{
	return l.minus_set(r);
}

// operator==
template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		iterator_ex::is_random_access_iterator<L>,
		bool
	>::type
	operator==(const L& l, const wrap_random_access_iterator<R>& r)
{
	return r.compare_eq(l);
}

template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		iterator_ex::is_random_access_iterator<R>,
		bool
	>::type
	operator==(const wrap_random_access_iterator<L>& l, const R& r)
{
	return l.compare_eq(r);
}

template<typename L> YGGR_CXX14_CONSTEXPR_OR_INLINE
bool operator==(const wrap_random_access_iterator<L>& l,
				const wrap_random_access_iterator<L>& r)
{
	return l.compare_eq(r);
}

template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
bool operator==(const wrap_random_access_iterator<L>& l,
				const wrap_random_access_const_iterator<R>& r)
{
	return l.compare_eq(r);
}

template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		iterator_ex::is_random_access_iterator<L>,
		bool
	>::type
	operator!=(const L& l, const wrap_random_access_iterator<R>& r)
{
	return !r.compare_eq(l);
}

template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		iterator_ex::is_random_access_iterator<R>,
		bool
	>::type
	operator!=(const wrap_random_access_iterator<L>& l, const R& r)
{
	return !l.compare_eq(r);
}

template<typename L> YGGR_CXX14_CONSTEXPR_OR_INLINE
bool operator!=(const wrap_random_access_iterator<L>& l,
				const wrap_random_access_iterator<L>& r)
{
	return !l.compare_eq(r);
}

template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
bool operator!=(const wrap_random_access_iterator<L>& l,
				const wrap_random_access_const_iterator<R>& r)
{
	return !l.compare_eq(r);
}

// operator<
template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		iterator_ex::is_random_access_iterator<L>,
		bool
	>::type
	operator<(const L& l, const wrap_random_access_iterator<R>& r)
{
	return 0 < r.compare(l);
}

template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		iterator_ex::is_random_access_iterator<R>,
		bool
	>::type
	operator<(const wrap_random_access_iterator<L>& l, const R& r)
{
	return l.compare(r) < 0;
}

template<typename L> YGGR_CXX14_CONSTEXPR_OR_INLINE
bool operator<(const wrap_random_access_iterator<L>& l,
				const wrap_random_access_iterator<L>& r)
{
	return l.compare(r) < 0;
}

template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
bool operator<(const wrap_random_access_iterator<L>& l,
				const wrap_random_access_const_iterator<R>& r)
{
	return l.compare(r) < 0;
}

// operator<=
template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		iterator_ex::is_random_access_iterator<L>,
		bool
	>::type
	operator<=(const L& l, const wrap_random_access_iterator<R>& r)
{
	return 0 <= r.compare(l) ;
}

template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		iterator_ex::is_random_access_iterator<R>,
		bool
	>::type
	operator<=(const wrap_random_access_iterator<L>& l, const R& r)
{
	return l.compare(r) <= 0;
}

template<typename L> YGGR_CXX14_CONSTEXPR_OR_INLINE
bool operator<=(const wrap_random_access_iterator<L>& l,
				const wrap_random_access_iterator<L>& r)
{
	return l.compare(r) <= 0;
}

template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
bool operator<=(const wrap_random_access_iterator<L>& l,
				const wrap_random_access_const_iterator<R>& r)
{
	return l.compare(r) <= 0;
}

// operator>
template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		iterator_ex::is_random_access_iterator<L>,
		bool
	>::type
	operator>(const L& l, const wrap_random_access_iterator<R>& r)
{
	return 0 > r.compare(l) ;
}

template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		iterator_ex::is_random_access_iterator<R>,
		bool
	>::type
	operator>(const wrap_random_access_iterator<L>& l, const R& r)
{
	return l.compare(r) > 0;
}

template<typename L> YGGR_CXX14_CONSTEXPR_OR_INLINE
bool operator>(const wrap_random_access_iterator<L>& l,
				const wrap_random_access_iterator<L>& r)
{
	return l.compare(r) > 0;
}

template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
bool operator>(const wrap_random_access_iterator<L>& l,
				const wrap_random_access_const_iterator<R>& r)
{
	return l.compare(r) > 0;
}

// operator>=
template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		iterator_ex::is_random_access_iterator<L>,
		bool
	>::type
	operator>=(const L& l, const wrap_random_access_iterator<R>& r)
{
	return 0 >= r.compare(l) ;
}

template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		iterator_ex::is_random_access_iterator<R>,
		bool
	>::type
	operator>=(const wrap_random_access_iterator<L>& l, const R& r)
{
	return l.compare(r) >= 0;
}

template<typename L> YGGR_CXX14_CONSTEXPR_OR_INLINE
bool operator>=(const wrap_random_access_iterator<L>& l,
				const wrap_random_access_iterator<L>& r)
{
	return l.compare(r) >= 0;
}

template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
bool operator>=(const wrap_random_access_iterator<L>& l,
				const wrap_random_access_const_iterator<R>& r)
{
	return l.compare(r) >= 0;
}

// output

template<typename Char, typename Traits, typename TIter> YGGR_CXX14_CONSTEXPR_OR_INLINE
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os,
				const wrap_random_access_iterator<TIter>& r)
{
	os << r.operator->();
	return os;
}

} // namespace lua_wrap
} // namespace yggr

namespace yggr
{
namespace lua_wrap
{
namespace swap_support
{

template<typename TIter> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		iterator_ex::is_random_access_iterator<TIter>,
		void
	>::type
	swap(TIter& l, yggr::lua_wrap::wrap_random_access_iterator<TIter>& r)
{
	r.swap(l);
}

template<typename TIter> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		iterator_ex::is_random_access_iterator<TIter>,
		void
	>::type
	swap(yggr::lua_wrap::wrap_random_access_iterator<TIter>& l, TIter& r)
{
	l.swap(r);
}

template<typename TIter> YGGR_CXX14_CONSTEXPR_OR_INLINE
void swap(yggr::lua_wrap::wrap_random_access_iterator<TIter>& l,
			yggr::lua_wrap::wrap_random_access_iterator<TIter>& r)
{
	l.swap(r);
}

} // namespace swap_support
} // namespace lua_wrap
} // namespace yggr

//-------------------wrap_random_access_const_iterator-----------------------

namespace yggr
{
namespace lua_wrap
{

template<typename TIter>
class wrap_random_access_const_iterator
{
public:
	typedef TIter base_type;
	typedef typename iterator_ex::iterator_value_ex<base_type>::type value_type;
	typedef typename iterator_ex::iterator_category_ex<base_type>::type iterator_category;
	typedef typename iterator_ex::iterator_pointer_ex<base_type>::type pointer;
	typedef typename iterator_ex::iterator_difference_ex<base_type>::type difference_type;
	typedef const value_type& reference;

private:
	typedef wrap_random_access_const_iterator this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	YGGR_CONSTEXPR wrap_random_access_const_iterator(void)
	{
	}

	template<typename Iter>
	YGGR_CONSTEXPR wrap_random_access_const_iterator(const Iter& right,
														typename
															boost::enable_if
															<
																iterator_ex::is_random_access_iterator<Iter>,
																mplex::sfinae_type
															>::type mark = 0)
		: _base(detail::unwrap_object(right))
	{
	}

	template<typename Iter>
	YGGR_CONSTEXPR wrap_random_access_const_iterator(const wrap_random_access_iterator<Iter>& right)
		: _base(detail::unwrap_object(right))
	{
	}

	YGGR_CONSTEXPR wrap_random_access_const_iterator(BOOST_RV_REF(base_type) right)
		: _base(boost::move(right))
	{
	}

	YGGR_CONSTEXPR wrap_random_access_const_iterator(const base_type& right)
		: _base(right)
	{
	}

	YGGR_CONSTEXPR wrap_random_access_const_iterator(BOOST_RV_REF(this_type) right)
		: _base(right._base)
	{
	}

	YGGR_CONSTEXPR wrap_random_access_const_iterator(const this_type& right)
		: _base(right._base)
	{
	}

	YGGR_CXX20_CONSTEXPR ~wrap_random_access_const_iterator(void)
	{
	}

public:
	template<typename Iter> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			iterator_ex::is_random_access_iterator<Iter>,
			this_type&
		>::type
		operator=(const Iter& right)
	{
		_base = detail::unwrap_object(right);
		return *this;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE this_type& operator=(BOOST_RV_REF(base_type) right)
	{
		base_type& right_ref = right;
		if(boost::addressof(_base) == boost::addressof(right_ref))
		{
			return *this;
		}

		copy_or_move_or_swap(_base, boost::move(right));
		return *this;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE this_type& operator=(const base_type& right)
	{
		if(boost::addressof(_base) == boost::addressof(right))
		{
			return *this;
		}

		_base = right;
		return *this;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		if(this == boost::addressof(right_ref))
		{
			return *this;
		}

		copy_or_move_or_swap(_base, boost::move(right._base));
		return *this;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE this_type& operator=(const this_type& right)
	{
		if(this == boost::addressof(right))
		{
			return *this;
		}

		_base = right._base;
		return *this;
	}

public:
	YGGR_CXX14_CONSTEXPR_OR_INLINE void swap(BOOST_RV_REF(base_type) right)
	{
		base_type& right_ref = right;
		this_type::swap(right_ref);
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE void swap(base_type& right)
	{
		yggr::swap(_base, right);
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE void swap(this_type& right)
	{
		if(this == boost::addressof(right))
		{
			return;
		}
		yggr::swap(_base, right._base);
	}

public:
	YGGR_CXX14_CONSTEXPR_OR_INLINE operator base_type&(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _base;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE operator const base_type&(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _base;
	}

public:
	YGGR_CXX14_CONSTEXPR_OR_INLINE base_type& unwrap(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _base;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE const base_type& unwrap(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _base;
	}

public:
	template<typename Iter> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			iterator_ex::is_random_access_iterator<Iter>,
			this_type&
		>::type
		set_iterator(const Iter& right)
	{
		_base = detail::unwrap_object(right);
		return *this;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE this_type& set_iterator(const base_type& right)
	{
		_base = right;
		return *this;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE this_type& set_iterator(const this_type& right)
	{
		if(this == boost::addressof(right))
		{
			return *this;
		}

		_base = right._base;
		return *this;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE this_type copy(void) const YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return *this;
	}

public:
	template<typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			reference
		>::type
		at(T n) const
	{
		return _base[static_cast<difference_type>(n)];
	}

	template<typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<T>,
			reference
		>::type
		at(const T& n) const
	{
		return _base[static_cast<difference_type>(n.value())];
	}

public:
	YGGR_CXX14_CONSTEXPR_OR_INLINE typename boost::mpl::if_<boost::is_class<value_type>, reference, value_type >::type
		get_value(void) const
	{
		return *_base;
	}

	template<typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			typename boost::mpl::if_<boost::is_class<value_type>, reference, value_type >::type
		>::type
		get_value(T n) const
	{
		return this_type::at(n);
	}

	template<typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<T>,
			typename boost::mpl::if_<boost::is_class<value_type>, reference, value_type >::type
		>::type
		get_value(const T& n) const
	{
		return this_type::at(n);
	}

public:
	YGGR_CXX14_CONSTEXPR_OR_INLINE this_type& head_increment(void)
	{
		iterator_ex::advance(_base, static_cast<difference_type>(1));
		return *this;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE this_type tail_increment(void)
	{
		this_type ret(_base);
		iterator_ex::advance(_base, static_cast<difference_type>(1));
		return ret;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE this_type& head_decrement(void)
	{
		iterator_ex::advance(_base, -static_cast<difference_type>(1));
		return *this;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE this_type tail_decrement(void)
	{
		this_type ret(_base);
		iterator_ex::advance(_base, -static_cast<difference_type>(1));
		return ret;
	}

	template<typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			this_type
		>::type
		plus(T n) const
	{
		return this_type(iterator_ex::advance_copy(_base, static_cast<difference_type>(n)));
	}

	template<typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<T>,
			this_type
		>::type
		plus(const T& n) const
	{
		return this_type(iterator_ex::advance_copy(_base, static_cast<difference_type>(n.value())));
	}

	template<typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			this_type&
		>::type
		plus_set(T n)
	{
		iterator_ex::advance(_base, static_cast<difference_type>(n));
		return *this;
	}

	template<typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<T>,
			this_type&
		>::type
		plus_set(const T& n)
	{
		iterator_ex::advance(_base, static_cast<difference_type>(n.value()));
		return *this;
	}

	template<typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			this_type
		>::type
		minus(T n) const
	{
		return this_type(iterator_ex::advance_copy(_base, -static_cast<difference_type>(n)));
	}

	template<typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<T>,
			this_type
		>::type
		minus(const T& n) const
	{
		return this_type(iterator_ex::advance_copy(_base, -static_cast<difference_type>(n.value())));
	}

	template<typename Iter> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			iterator_ex::is_random_access_iterator<Iter>,
			difference_type
		>::type
		minus(const Iter& right) const
	{
		return -iterator_ex::distance<base_type>(_base, detail::unwrap_object(right));
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE difference_type minus(const base_type& right) const
	{
		return -iterator_ex::distance(_base, right);
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE difference_type minus(const this_type& right) const
	{
		return -iterator_ex::distance(_base, right._base);
	}

	template<typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			this_type&
		>::type
		minus_set(T n)
	{
		iterator_ex::advance(_base, -static_cast<difference_type>(n));
		return *this;
	}

	template<typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<T>,
			this_type&
		>::type
		minus_set(const T& n)
	{
		iterator_ex::advance(_base, -static_cast<difference_type>(n.value()));
		return *this;
	}

	template<typename Iter> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			iterator_ex::is_random_access_iterator<Iter>,
			typename wrap_base_t_maker<difference_type>::type
		>::type
		wraped_minus(const Iter& right) const
	{
		typedef typename wrap_base_t_maker<difference_type>::type wrap_diff_type;

		return wrap_diff_type(this_type::minus(right));
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE typename wrap_base_t_maker<difference_type>::type
		wraped_minus(const base_type& right) const
	{
		typedef typename wrap_base_t_maker<difference_type>::type wrap_diff_type;

		return wrap_diff_type(this_type::minus(right));
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE typename wrap_base_t_maker<difference_type>::type
		wraped_minus(const this_type& right) const
	{
		typedef typename wrap_base_t_maker<difference_type>::type wrap_diff_type;

		return wrap_diff_type(this_type::minus(right));
	}

public:
	template<typename Iter> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			iterator_ex::is_random_access_iterator<Iter>,
			bool
		>::type
		compare_eq(const Iter& right) const
	{
		return _base == detail::unwrap_object(right);
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE bool compare_eq(const base_type& right) const
	{
		return _base == right;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE bool compare_eq(const this_type& right) const
	{
		return _base == right.unwrap();
	}

	template<typename Iter> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			iterator_ex::is_random_access_iterator<Iter>,
			s32
		>::type
		compare(const Iter& right) const
	{
		difference_type diff = this_type::minus(right);
		return diff == 0? 0 : diff < 0? -1 : 1;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE s32 compare(const base_type& right) const
	{
		difference_type diff = this_type::minus(right);
		return diff == 0? 0 : diff < 0? -1 : 1;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE s32 compare(const this_type& right) const
	{
		difference_type diff = this_type::minus(right.unwrap());
		return diff == 0? 0 : diff < 0? -1 : 1;
	}

public:
	YGGR_CXX14_CONSTEXPR_OR_INLINE this_type& next(void)
	{
		return this_type::head_increment();
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE this_type& prev(void)
	{
		return this_type::head_decrement();
	}

public:
	template<typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			reference
		>::type
		operator[](T n) const
	{
		return this_type::at(n);
	}

	template<typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<T>,
			reference
		>::type
		operator[](const T& n) const
	{
		return this_type::at(n);
	}

	pointer operator->(void) const
	{
		return _base.operator->();
	}

protected:
	base_type _base;
};

// non-member foo
// *
template<typename L> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename wrap_random_access_const_iterator<L>::reference
	operator*(const wrap_random_access_const_iterator<L>& l)
{
	return *(l.unwrap());
}

// ++
template<typename L> YGGR_CXX14_CONSTEXPR_OR_INLINE
wrap_random_access_const_iterator<L>&
	operator++(wrap_random_access_const_iterator<L>& l)
{
	return l.head_increment();
}

template<typename L> YGGR_CXX14_CONSTEXPR_OR_INLINE
wrap_random_access_const_iterator<L>
	operator++(wrap_random_access_const_iterator<L>& l, int)
{
	return l.tail_increment();
}

//--
template<typename L> YGGR_CXX14_CONSTEXPR_OR_INLINE
wrap_random_access_const_iterator<L>&
	operator--(wrap_random_access_const_iterator<L>& l)
{
	return l.head_decrement();
}

template<typename L> YGGR_CXX14_CONSTEXPR_OR_INLINE
wrap_random_access_const_iterator<L>
	operator--(wrap_random_access_const_iterator<L>& l, int)
{
	return l.tail_decrement();
}

// +
template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		boost::is_arithmetic<L>,
		wrap_random_access_const_iterator<R>
	>::type
	operator+(L l, const wrap_random_access_const_iterator<R>& r)
{
	return r.plus(l);
}

template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		detail::is_wrap_base_t<L>,
		wrap_random_access_const_iterator<R>
	>::type
	operator+(const L& l, const wrap_random_access_const_iterator<R>& r)
{
	return r.plus(l);
}

template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		boost::is_arithmetic<R>,
		wrap_random_access_const_iterator<L>
	>::type
	operator+(const wrap_random_access_const_iterator<L>& l, R r)
{
	return l.plus(r);
}

template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		detail::is_wrap_base_t<R>,
		wrap_random_access_const_iterator<L>
	>::type
	operator+(const wrap_random_access_const_iterator<L>& l, const R& r)
{
	return l.plus(r);
}

// +=
template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		boost::is_arithmetic<R>,
		wrap_random_access_const_iterator<L>&
	>::type
	operator+=(wrap_random_access_const_iterator<L>& l, R r)
{
	return l.plus_set(r);
}

template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		detail::is_wrap_base_t<R>,
		wrap_random_access_const_iterator<L>&
	>::type
	operator+=(wrap_random_access_const_iterator<L>& l, const R& r)
{
	return l.plus_set(r);
}

// - diff
template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		boost::is_arithmetic<R>,
		wrap_random_access_const_iterator<L>
	>::type
	operator-(const wrap_random_access_const_iterator<L>& l, R r)
{
	return l.minus(r);
}

template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		detail::is_wrap_base_t<R>,
		wrap_random_access_const_iterator<L>
	>::type
	operator-(const wrap_random_access_const_iterator<L>& l, const R& r)
{
	return l.minus(r);
}

// -iter
template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		iterator_ex::is_random_access_iterator<L>,
		typename wrap_random_access_const_iterator<R>::difference_type
	>::type
	operator-(const L& l, const wrap_random_access_const_iterator<R>& r)
{
	return -r.minus(l);
}

template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		iterator_ex::is_random_access_iterator<R>,
		typename wrap_random_access_const_iterator<L>::difference_type
	>::type
	operator-(const wrap_random_access_const_iterator<L>& l, const R& r)
{
	return l.minus(r);
}

template<typename L> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename wrap_random_access_const_iterator<L>::difference_type
	operator-(const wrap_random_access_const_iterator<L>& l,
				const wrap_random_access_const_iterator<L>& r)
{
	return l.minus(r);
}

template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename wrap_random_access_const_iterator<L>::difference_type
	operator-(const wrap_random_access_const_iterator<L>& l,
				const wrap_random_access_iterator<R>& r)
{
	return l.minus(r);
}

// -=
template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		boost::is_arithmetic<R>,
		wrap_random_access_const_iterator<L>&
	>::type
	operator-=(wrap_random_access_const_iterator<L>& l, R r)
{
	return l.minus_set(r);
}

template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		detail::is_wrap_base_t<R>,
		wrap_random_access_const_iterator<L>&
	>::type
	operator-=(wrap_random_access_const_iterator<L>& l, const R& r)
{
	return l.minus_set(r);
}

// operator==
template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		iterator_ex::is_random_access_iterator<L>,
		bool
	>::type
	operator==(const L& l, const wrap_random_access_const_iterator<R>& r)
{
	return r.compare_eq(l);
}

template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		iterator_ex::is_random_access_iterator<R>,
		bool
	>::type
	operator==(const wrap_random_access_const_iterator<L>& l, const R& r)
{
	return l.compare_eq(r);
}

template<typename L> YGGR_CXX14_CONSTEXPR_OR_INLINE
bool operator==(const wrap_random_access_const_iterator<L>& l,
				const wrap_random_access_const_iterator<L>& r)
{
	return l.compare_eq(r);
}

template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
bool operator==(const wrap_random_access_const_iterator<L>& l,
				const wrap_random_access_iterator<R>& r)
{
	return l.compare_eq(r);
}


template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		iterator_ex::is_random_access_iterator<L>,
		bool
	>::type
	operator!=(const L& l, const wrap_random_access_const_iterator<R>& r)
{
	return !r.compare_eq(l);
}

template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		iterator_ex::is_random_access_iterator<R>,
		bool
	>::type
	operator!=(const wrap_random_access_const_iterator<L>& l, const R& r)
{
	return !l.compare_eq(r);
}

template<typename L> YGGR_CXX14_CONSTEXPR_OR_INLINE
bool operator!=(const wrap_random_access_const_iterator<L>& l,
				const wrap_random_access_const_iterator<L>& r)
{
	return !l.compare_eq(r);
}

template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
bool operator!=(const wrap_random_access_const_iterator<L>& l,
				const wrap_random_access_iterator<R>& r)
{
	return !l.compare_eq(r);
}

// operator<
template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		iterator_ex::is_random_access_iterator<L>,
		bool
	>::type
	operator<(const L& l, const wrap_random_access_const_iterator<R>& r)
{
	return 0 < r.compare(l);
}

template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		iterator_ex::is_random_access_iterator<R>,
		bool
	>::type
	operator<(const wrap_random_access_const_iterator<L>& l, const R& r)
{
	return l.compare(r) < 0;
}

template<typename L> YGGR_CXX14_CONSTEXPR_OR_INLINE
bool operator<(const wrap_random_access_const_iterator<L>& l,
				const wrap_random_access_const_iterator<L>& r)
{
	return l.compare(r) < 0;
}

template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
bool operator<(const wrap_random_access_const_iterator<L>& l,
				const wrap_random_access_iterator<R>& r)
{
	return l.compare(r) < 0;
}

// operator<=
template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		iterator_ex::is_random_access_iterator<L>,
		bool
	>::type
	operator<=(const L& l, const wrap_random_access_const_iterator<R>& r)
{
	return 0 <= r.compare(l) ;
}

template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		iterator_ex::is_random_access_iterator<R>,
		bool
	>::type
	operator<=(const wrap_random_access_const_iterator<L>& l, const R& r)
{
	return l.compare(r) <= 0;
}

template<typename L> YGGR_CXX14_CONSTEXPR_OR_INLINE
bool operator<=(const wrap_random_access_const_iterator<L>& l,
				const wrap_random_access_const_iterator<L>& r)
{
	return l.compare(r) <= 0;
}

template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
bool operator<=(const wrap_random_access_const_iterator<L>& l,
				const wrap_random_access_iterator<R>& r)
{
	return l.compare(r) <= 0;
}

// operator>
template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		iterator_ex::is_random_access_iterator<L>,
		bool
	>::type
	operator>(const L& l, const wrap_random_access_const_iterator<R>& r)
{
	return 0 > r.compare(l) ;
}

template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		iterator_ex::is_random_access_iterator<R>,
		bool
	>::type
	operator>(const wrap_random_access_const_iterator<L>& l, const R& r)
{
	return l.compare(r) > 0;
}

template<typename L> YGGR_CXX14_CONSTEXPR_OR_INLINE
bool operator>(const wrap_random_access_const_iterator<L>& l,
				const wrap_random_access_const_iterator<L>& r)
{
	return l.compare(r) > 0;
}

template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
bool operator>(const wrap_random_access_const_iterator<L>& l,
				const wrap_random_access_iterator<R>& r)
{
	return l.compare(r) > 0;
}

// operator>=
template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		iterator_ex::is_random_access_iterator<L>,
		bool
	>::type
	operator>=(const L& l, const wrap_random_access_const_iterator<R>& r)
{
	return 0 >= r.compare(l) ;
}

template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		iterator_ex::is_random_access_iterator<R>,
		bool
	>::type
	operator>=(const wrap_random_access_const_iterator<L>& l, const R& r)
{
	return l.compare(r) >= 0;
}

template<typename L> YGGR_CXX14_CONSTEXPR_OR_INLINE
bool operator>=(const wrap_random_access_const_iterator<L>& l,
				const wrap_random_access_const_iterator<L>& r)
{
	return l.compare(r) >= 0;
}

template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
bool operator>=(const wrap_random_access_const_iterator<L>& l,
				const wrap_random_access_iterator<R>& r)
{
	return l.compare(r) >= 0;
}

// output

template<typename Char, typename Traits, typename TIter> YGGR_CXX14_CONSTEXPR_OR_INLINE
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os,
				const wrap_random_access_const_iterator<TIter>& r)
{
	os << r.operator->();
	return os;
}

} // namespace lua_wrap
} // namespace yggr

namespace yggr
{
namespace lua_wrap
{
namespace swap_support
{

template<typename TIter> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		iterator_ex::is_random_access_iterator<TIter>,
		void
	>::type
	swap(TIter& l, yggr::lua_wrap::wrap_random_access_const_iterator<TIter>& r)
{
	r.swap(l);
}

template<typename TIter> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		iterator_ex::is_random_access_iterator<TIter>,
		void
	>::type
	swap(yggr::lua_wrap::wrap_random_access_const_iterator<TIter>& l, TIter& r)
{
	l.swap(r);
}

template<typename TIter> YGGR_CXX14_CONSTEXPR_OR_INLINE
void swap(yggr::lua_wrap::wrap_random_access_const_iterator<TIter>& l,
			yggr::lua_wrap::wrap_random_access_const_iterator<TIter>& r)
{
	l.swap(r);
}

} // namespace swap_support

using swap_support::swap;

} // namespace lua_wrap
} // namespace yggr

namespace std
{
	using ::yggr::lua_wrap::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::lua_wrap::swap_support::swap;
} // namespace boost

#endif //__YGGR_LUA_WRAP_WRAP_RANDOM_ACCESS_ITERATOR_HPP__
