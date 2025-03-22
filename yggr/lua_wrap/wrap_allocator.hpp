//wrap_allocator.hpp

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

#ifndef __YGGR_LUA_WRAP_WRAP_ALLOCATOR_HPP__
#define __YGGR_LUA_WRAP_WRAP_ALLOCATOR_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/container_ex/allocator_traits.hpp>
#include <yggr/container_ex/is_allocator.hpp>

#include <yggr/ppex/foo_params.hpp>
#include <yggr/mplex/static_assert.hpp>

#include <yggr/lua_wrap/detail/unwrap_t.hpp>
#include <yggr/lua_wrap/detail/luabind_function_noexcept.hpp>
#include <yggr/lua_wrap/wrap_base_t.hpp>

#include <boost/ref.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/bool.hpp>

#include <ostream>

namespace yggr
{
namespace lua_wrap
{

template<typename TAlloc>
class wrap_allocator;

} // namepsace lua_wrap
} // namespace yggr

namespace yggr
{
namespace lua_wrap
{
namespace detail
{

template<typename T>
struct is_wrap_allocator
	: public boost::mpl::false_
{
};

template<typename T>
struct is_wrap_allocator< wrap_allocator<T> >
	: public boost::mpl::true_
{
};

} // namespace detail
} // namepsace lua_wrap
} // namespace yggr

namespace yggr
{
namespace lua_wrap
{

template<typename TAlloc>
class wrap_allocator
{
private:
	BOOST_MPL_ASSERT((container_ex::is_allocator<TAlloc>));

public:
	typedef TAlloc base_type;
	typedef typename container_ex::allocator_traits<base_type>::value_type value_type;
	typedef typename container_ex::allocator_traits<base_type>::pointer pointer;
	typedef typename container_ex::allocator_traits<base_type>::const_pointer const_pointer;
	typedef typename container_ex::allocator_traits<base_type>::reference reference;
	typedef typename container_ex::allocator_traits<base_type>::const_reference const_reference;
	typedef typename container_ex::allocator_traits<base_type>::size_type size_type;
	typedef typename container_ex::allocator_traits<base_type>::difference_type difference_type;
	typedef typename
		container_ex::allocator_traits<base_type>::
			propagate_on_container_move_assignment propagate_on_container_move_assignment;

private:
	template<typename Alloc>
	friend class wrap_allocator;

private:
	typedef wrap_allocator this_type;

public:
	YGGR_CONSTEXPR wrap_allocator(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
	}

	template<typename Alloc> YGGR_CONSTEXPR
	wrap_allocator(const Alloc& right) YGGR_NOEXCEPT_OR_NOTHROW
		: _base(detail::unwrap_object(right))
	{
	}

	YGGR_CONSTEXPR wrap_allocator(const base_type& right) YGGR_NOEXCEPT_OR_NOTHROW
		: _base(right)
	{
	}

	template<typename Alloc> YGGR_CONSTEXPR
	wrap_allocator(const wrap_allocator<Alloc>& right) YGGR_NOEXCEPT_OR_NOTHROW
		: _base(right._base)
	{
	}

	YGGR_CONSTEXPR wrap_allocator(const this_type& right) YGGR_NOEXCEPT_OR_NOTHROW
		: _base(right._base)
	{
	}

	YGGR_CXX20_CONSTEXPR ~wrap_allocator(void)
	{
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
	YGGR_CXX14_CONSTEXPR_OR_INLINE const this_type& get_allocator(void) const YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return *this;
	}

public:
	YGGR_CXX14_CONSTEXPR_OR_INLINE this_type copy(void) const YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return *this;
	}

public:
	YGGR_CXX14_CONSTEXPR_OR_INLINE pointer address(reference ref) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return boost::addressof(ref);
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE const_pointer address(const_reference cref) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return boost::addressof(cref);
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE pointer allocate(std::size_t n)
	{
		return _base.allocate(n);
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE pointer allocate(std::size_t n, const void* /*hint*/)
	{
		return _base.allocate(n);
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE void deallocate(pointer p, std::size_t n )
	{
		_base.deallocate(p, n);
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE size_type max_size(void) const YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return _base.max_size();
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE typename wrap_base_t_maker<size_type>::type wraped_max_size(void) const YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		typedef typename wrap_base_t_maker<size_type>::type wrap_type;
		return wrap_type(_base.max_size());
	}

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename U, typename ...Args> YGGR_CXX14_CONSTEXPR_OR_INLINE
	void construct(U* p, BOOST_FWD_REF(Args)... args)
	{
		new (static_cast<void*>(p)) U(boost::forward<Args>(args)...);
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		template<typename U \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > YGGR_CXX14_CONSTEXPR_OR_INLINE \
		void construct(U* p \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			new (static_cast<void*>(p)) U(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); }

#	define YGGR_PP_FOO_ARG_NAME() arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE()
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename U> YGGR_CXX14_CONSTEXPR_OR_INLINE
	void destroy(U* p)
	{
		if(p)
		{
			p->~U();
		}
	}

public:
	template<typename Alloc> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			container_ex::is_allocator<Alloc>,
			bool
		>::type
		compare_eq(const Alloc& right) const
	{
		return _base == right;
	}

	template<typename Alloc> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			detail::is_wrap_allocator<Alloc>,
			bool
		>::type
		compare_eq(const Alloc& right) const
	{
		return _base == right._base;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE bool compare_eq(const this_type& right) const // luabind using this foo
	{
		return (this == boost::addressof(right)) || (_base == right._base);
	}

protected:
	base_type _base;
};

// non-member foo

// ==
template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		container_ex::is_allocator<L>,
		bool
	>::type
	operator==(const L& l, const wrap_allocator< R >& r)
{
	return r.compare_eq(l);
}

template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		container_ex::is_allocator<R>,
		bool
	>::type
	operator==(const wrap_allocator< L >& l, const R& r)
{
	return l.compare_eq(r);
}

template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
bool operator==(const wrap_allocator< L >& l, const wrap_allocator< R >& r)
{
	return l.compare_eq(r);
}

// !=
template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		container_ex::is_allocator<L>,
		bool
	>::type
	operator!=(const L& l, const wrap_allocator< R >& r)
{
	return !r.compare_eq(l);
}

template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		container_ex::is_allocator<R>,
		bool
	>::type
	operator!=(const wrap_allocator< L >& l, const R& r)
{
	return !l.compare_eq(r);
}

template<typename L, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
bool operator!=(const wrap_allocator< L >& l, const wrap_allocator< R >& r)
{
	return !l.compare_eq(r);
}

// output
template<typename Char, typename Traits, typename R> YGGR_CXX14_CONSTEXPR_OR_INLINE
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os, const wrap_allocator<R>& val)
{
	os << boost::addressof(val);
	return os;
}

} // namespace lua_wrap
} // namespace yggr

#endif // __YGGR_LUA_WRAP_WRAP_ALLOCATOR_HPP__
