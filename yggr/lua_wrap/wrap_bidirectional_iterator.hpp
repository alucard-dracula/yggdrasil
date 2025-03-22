// wrap_bidirectional_iterator.hpp

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

#ifndef __YGGR_LUA_WRAP_WRAP_BIDIRECTIONAL_ITERATOR_HPP__
#define __YGGR_LUA_WRAP_WRAP_BIDIRECTIONAL_ITERATOR_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/utility/copy_or_move_or_swap.hpp>

#include <yggr/mplex/static_assert.hpp>
#include <yggr/mplex/tag_sfinae.hpp>

#include <yggr/iterator_ex/iterator_traits.hpp>
#include <yggr/iterator_ex/iterator.hpp>
#include <yggr/iterator_ex/iterator_category_check.hpp>

#include <yggr/lua_wrap/wrap_base_t.hpp>
#include <yggr/lua_wrap/detail/unwrap_t.hpp>
#include <yggr/lua_wrap/detail/luabind_function_noexcept.hpp>

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
class wrap_bidirectional_iterator;

template<typename TIter>
class wrap_bidirectional_const_iterator;

} // namespace lua_wrap
} // namespace yggr

namespace yggr
{
namespace lua_wrap
{
namespace detail
{

template<typename TIter>
struct is_wrap_bidirectional_iterator
	: public boost::mpl::false_
{
};

template<typename TIter>
struct is_wrap_bidirectional_iterator< wrap_bidirectional_iterator<TIter> >
	: public boost::mpl::true_
{
};

template<typename TIter>
struct is_wrap_bidirectional_const_iterator
	: public boost::mpl::false_
{
};

template<typename TIter>
struct is_wrap_bidirectional_const_iterator< wrap_bidirectional_const_iterator<TIter> >
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

//----------------------wrap_bidirectional_iterator-------------------------
template<typename TIter>
class wrap_bidirectional_iterator
{
private:
	BOOST_MPL_ASSERT((iterator_ex::is_bidirectional_iterator<TIter>));

public:
	typedef TIter base_type;
	typedef typename iterator_ex::iterator_value_ex<base_type>::type value_type;
	typedef typename iterator_ex::iterator_category_ex<base_type>::type iterator_category;
	typedef typename iterator_ex::iterator_pointer_ex<base_type>::type pointer;
	typedef typename iterator_ex::iterator_difference_ex<base_type>::type difference_type;
	typedef typename iterator_ex::iterator_reference_ex<base_type>::type reference;

private:
	typedef wrap_bidirectional_iterator this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	wrap_bidirectional_iterator(void)
	{
	}

	wrap_bidirectional_iterator(BOOST_RV_REF(base_type) right)
		: _base(boost::move(right))
	{
	}

	wrap_bidirectional_iterator(const base_type& right)
		: _base(right)
	{
	}

	wrap_bidirectional_iterator(BOOST_RV_REF(this_type) right)
		: _base(boost::move(right._base))
	{
	}

	wrap_bidirectional_iterator(const this_type& right)
		: _base(right._base)
	{
	}

	~wrap_bidirectional_iterator(void)
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

	YGGR_CONSTEXPR_OR_INLINE operator const base_type&(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _base;
	}

public:
	YGGR_CXX14_CONSTEXPR_OR_INLINE base_type& unwrap(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _base;
	}

	YGGR_CONSTEXPR_OR_INLINE const base_type& unwrap(void) const YGGR_NOEXCEPT_OR_NOTHROW
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

	YGGR_CONSTEXPR_OR_INLINE this_type copy(void) const YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return *this;
	}

public:
	YGGR_CONSTEXPR_OR_INLINE typename boost::mpl::if_<boost::is_class<value_type>, reference, value_type >::type
		get_value(void) const
	{
		return *_base;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE this_type& set_value(const value_type& val) const
	{
		*_base = val;
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

public:
	template<typename Iter> YGGR_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			iterator_ex::is_bidirectional_iterator<Iter>,
			bool
		>::type
		compare_eq(const Iter& right) const
	{
		return _base == detail::unwrap_object(right);
	}

	YGGR_CONSTEXPR_OR_INLINE bool compare_eq(const base_type& right) const
	{
		return _base == right;
	}

	YGGR_CONSTEXPR_OR_INLINE bool compare_eq(const this_type& right) const
	{
		return _base == right.unwrap();
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
	YGGR_CONSTEXPR_OR_INLINE pointer operator->(void) const
	{
		return _base.operator->();
	}

protected:
	base_type _base;
};

// non-member foo
// *
template<typename L> YGGR_CONSTEXPR_OR_INLINE
typename wrap_bidirectional_iterator<L>::reference
	operator*(const wrap_bidirectional_iterator<L>& l)
{
	return *(l.unwrap());
}

// ++
template<typename L> YGGR_CONSTEXPR_OR_INLINE
wrap_bidirectional_iterator<L>&
	operator++(wrap_bidirectional_iterator<L>& l)
{
	return l.head_increment();
}

template<typename L> YGGR_CONSTEXPR_OR_INLINE
wrap_bidirectional_iterator<L>
	operator++(wrap_bidirectional_iterator<L>& l, int)
{
	return l.tail_increment();
}

//--
template<typename L> YGGR_CONSTEXPR_OR_INLINE
wrap_bidirectional_iterator<L>&
	operator--(wrap_bidirectional_iterator<L>& l)
{
	return l.head_decrement();
}

template<typename L> YGGR_CONSTEXPR_OR_INLINE
wrap_bidirectional_iterator<L>
	operator--(wrap_bidirectional_iterator<L>& l, int)
{
	return l.tail_decrement();
}

// operator==
template<typename L, typename R> YGGR_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		iterator_ex::is_bidirectional_iterator<L>,
		bool
	>::type
	operator==(const L& l, const wrap_bidirectional_iterator<R>& r)
{
	return r.compare_eq(l);
}

template<typename L, typename R> YGGR_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		iterator_ex::is_bidirectional_iterator<R>,
		bool
	>::type
	operator==(const wrap_bidirectional_iterator<L>& l, const R& r)
{
	return l.compare_eq(r);
}

template<typename L> YGGR_CONSTEXPR_OR_INLINE
bool operator==(const wrap_bidirectional_iterator<L>& l,
				const wrap_bidirectional_iterator<L>& r)
{
	return l.compare_eq(r);
}

template<typename L, typename R> YGGR_CONSTEXPR_OR_INLINE
bool operator==(const wrap_bidirectional_iterator<L>& l,
				const wrap_bidirectional_const_iterator<R>& r)
{
	return l.compare_eq(r);
}

template<typename L, typename R> YGGR_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		iterator_ex::is_bidirectional_iterator<L>,
		bool
	>::type
	operator!=(const L& l, const wrap_bidirectional_iterator<R>& r)
{
	return !r.compare_eq(l);
}

template<typename L, typename R> YGGR_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		iterator_ex::is_bidirectional_iterator<R>,
		bool
	>::type
	operator!=(const wrap_bidirectional_iterator<L>& l, const R& r)
{
	return !l.compare_eq(r);
}

template<typename L> YGGR_CONSTEXPR_OR_INLINE
bool operator!=(const wrap_bidirectional_iterator<L>& l,
				const wrap_bidirectional_iterator<L>& r)
{
	return !l.compare_eq(r);
}

template<typename L, typename R> YGGR_CONSTEXPR_OR_INLINE
bool operator!=(const wrap_bidirectional_iterator<L>& l,
				const wrap_bidirectional_const_iterator<R>& r)
{
	return !l.compare_eq(r);
}

// output

template<typename Char, typename Traits, typename TIter> YGGR_CXX14_CONSTEXPR_OR_INLINE
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os,
				const wrap_bidirectional_iterator<TIter>& r)
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
		iterator_ex::is_bidirectional_iterator<TIter>,
		void
	>::type
	swap(TIter& l, yggr::lua_wrap::wrap_bidirectional_iterator<TIter>& r)
{
	r.swap(l);
}

template<typename TIter> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		iterator_ex::is_bidirectional_iterator<TIter>,
		void
	>::type
	swap(yggr::lua_wrap::wrap_bidirectional_iterator<TIter>& l, TIter& r)
{
	l.swap(r);
}

template<typename TIter> YGGR_CXX14_CONSTEXPR_OR_INLINE
void swap(yggr::lua_wrap::wrap_bidirectional_iterator<TIter>& l,
			yggr::lua_wrap::wrap_bidirectional_iterator<TIter>& r)
{
	l.swap(r);
}

} // namespace swap_support
} // namespace lua_wrap
} // namespace yggr

//-------------------wrap_bidirectional_const_iterator-----------------------

namespace yggr
{
namespace lua_wrap
{

template<typename TIter>
class wrap_bidirectional_const_iterator
{
public:
	typedef TIter base_type;
	typedef typename iterator_ex::iterator_value_ex<base_type>::type value_type;
	typedef typename iterator_ex::iterator_category_ex<base_type>::type iterator_category;
	typedef typename iterator_ex::iterator_pointer_ex<base_type>::type pointer;
	typedef typename iterator_ex::iterator_difference_ex<base_type>::type difference_type;
	typedef const value_type& reference;

private:
	typedef wrap_bidirectional_const_iterator this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	wrap_bidirectional_const_iterator(void)
	{
	}

	template<typename Iter>
	wrap_bidirectional_const_iterator(const Iter& right,
										typename
											boost::enable_if
											<
												iterator_ex::is_bidirectional_iterator<Iter>,
												mplex::sfinae_type
											>::type mark = 0)
		: _base(detail::unwrap_object(right))
	{
	}

	template<typename Iter>
	wrap_bidirectional_const_iterator(const wrap_bidirectional_iterator<Iter>& right)
		: _base(detail::unwrap_object(right))
	{
	}

	wrap_bidirectional_const_iterator(BOOST_RV_REF(base_type) right)
		: _base(boost::move(right))
	{
	}

	wrap_bidirectional_const_iterator(const base_type& right)
		: _base(right)
	{
	}

	wrap_bidirectional_const_iterator(BOOST_RV_REF(this_type) right)
		: _base(right._base)
	{
	}

	wrap_bidirectional_const_iterator(const this_type& right)
		: _base(right._base)
	{
	}

	~wrap_bidirectional_const_iterator(void)
	{
	}

public:
	template<typename Iter> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			iterator_ex::is_bidirectional_iterator<Iter>,
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

	YGGR_CONSTEXPR_OR_INLINE operator const base_type&(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _base;
	}

public:
	YGGR_CXX14_CONSTEXPR_OR_INLINE base_type& unwrap(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _base;
	}

	YGGR_CONSTEXPR_OR_INLINE const base_type& unwrap(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _base;
	}

public:
	template<typename Iter> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			iterator_ex::is_bidirectional_iterator<Iter>,
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

	YGGR_CONSTEXPR_OR_INLINE this_type copy(void) const YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return *this;
	}

public:
	YGGR_CONSTEXPR_OR_INLINE typename boost::mpl::if_<boost::is_class<value_type>, reference, value_type >::type
		get_value(void) const
	{
		return *_base;
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

public:
	template<typename Iter> YGGR_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			iterator_ex::is_bidirectional_iterator<Iter>,
			bool
		>::type
		compare_eq(const Iter& right) const
	{
		return _base == detail::unwrap_object(right);
	}

	YGGR_CONSTEXPR_OR_INLINE bool compare_eq(const base_type& right) const
	{
		return _base == right;
	}

	YGGR_CONSTEXPR_OR_INLINE bool compare_eq(const this_type& right) const
	{
		return _base == right.unwrap();
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
	YGGR_CONSTEXPR_OR_INLINE pointer operator->(void) const
	{
		return _base.operator->();
	}

protected:
	base_type _base;
};

// non-member foo
// *
template<typename L> YGGR_CONSTEXPR_OR_INLINE
typename wrap_bidirectional_const_iterator<L>::reference
	operator*(const wrap_bidirectional_const_iterator<L>& l)
{
	return *(l.unwrap());
}

// ++
template<typename L> YGGR_CONSTEXPR_OR_INLINE
wrap_bidirectional_const_iterator<L>&
	operator++(wrap_bidirectional_const_iterator<L>& l)
{
	return l.head_increment();
}

template<typename L> YGGR_CONSTEXPR_OR_INLINE
wrap_bidirectional_const_iterator<L>
	operator++(wrap_bidirectional_const_iterator<L>& l, int)
{
	return l.tail_increment();
}

//--
template<typename L> YGGR_CONSTEXPR_OR_INLINE
wrap_bidirectional_const_iterator<L>&
	operator--(wrap_bidirectional_const_iterator<L>& l)
{
	return l.head_decrement();
}

template<typename L> YGGR_CONSTEXPR_OR_INLINE
wrap_bidirectional_const_iterator<L>
	operator--(wrap_bidirectional_const_iterator<L>& l, int)
{
	return l.tail_decrement();
}

// operator==
template<typename L, typename R> YGGR_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		iterator_ex::is_bidirectional_iterator<L>,
		bool
	>::type
	operator==(const L& l, const wrap_bidirectional_const_iterator<R>& r)
{
	return r.compare_eq(l);
}

template<typename L, typename R> YGGR_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		iterator_ex::is_bidirectional_iterator<R>,
		bool
	>::type
	operator==(const wrap_bidirectional_const_iterator<L>& l, const R& r)
{
	return l.compare_eq(r);
}

template<typename L> YGGR_CONSTEXPR_OR_INLINE
bool operator==(const wrap_bidirectional_const_iterator<L>& l,
				const wrap_bidirectional_const_iterator<L>& r)
{
	return l.compare_eq(r);
}

template<typename L, typename R> YGGR_CONSTEXPR_OR_INLINE
bool operator==(const wrap_bidirectional_const_iterator<L>& l,
				const wrap_bidirectional_iterator<R>& r)
{
	return l.compare_eq(r);
}


template<typename L, typename R> YGGR_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		iterator_ex::is_bidirectional_iterator<L>,
		bool
	>::type
	operator!=(const L& l, const wrap_bidirectional_const_iterator<R>& r)
{
	return !r.compare_eq(l);
}

template<typename L, typename R> YGGR_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		iterator_ex::is_bidirectional_iterator<R>,
		bool
	>::type
	operator!=(const wrap_bidirectional_const_iterator<L>& l, const R& r)
{
	return !l.compare_eq(r);
}

template<typename L> YGGR_CONSTEXPR_OR_INLINE
bool operator!=(const wrap_bidirectional_const_iterator<L>& l,
				const wrap_bidirectional_const_iterator<L>& r)
{
	return !l.compare_eq(r);
}

template<typename L, typename R> YGGR_CONSTEXPR_OR_INLINE
bool operator!=(const wrap_bidirectional_const_iterator<L>& l,
				const wrap_bidirectional_iterator<R>& r)
{
	return !l.compare_eq(r);
}

// output

template<typename Char, typename Traits, typename TIter> YGGR_CXX14_CONSTEXPR_OR_INLINE
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os,
				const wrap_bidirectional_const_iterator<TIter>& r)
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
		iterator_ex::is_bidirectional_iterator<TIter>,
		void
	>::type
	swap(TIter& l, yggr::lua_wrap::wrap_bidirectional_const_iterator<TIter>& r)
{
	r.swap(l);
}

template<typename TIter> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		iterator_ex::is_bidirectional_iterator<TIter>,
		void
	>::type
	swap(yggr::lua_wrap::wrap_bidirectional_const_iterator<TIter>& l, TIter& r)
{
	l.swap(r);
}

template<typename TIter> YGGR_CXX14_CONSTEXPR_OR_INLINE
void swap(yggr::lua_wrap::wrap_bidirectional_const_iterator<TIter>& l,
			yggr::lua_wrap::wrap_bidirectional_const_iterator<TIter>& r)
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

#endif //__YGGR_LUA_WRAP_WRAP_BIDIRECTIONAL_ITERATOR_HPP__
