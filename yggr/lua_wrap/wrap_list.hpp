//wrap_list.hpp

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

#ifndef __YGGR_LUA_WRAP_WRAP_LIST_HPP__
#define __YGGR_LUA_WRAP_WRAP_LIST_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/algorithm/shuffle.hpp>

#include <yggr/mplex/tag_sfinae.hpp>

#include <yggr/ppex/foo_params.hpp>
#include <yggr/ppex/symbol_if.hpp>
#include <yggr/ppex/symbols.hpp>

#include <yggr/iterator_ex/is_iterator.hpp>

#include <yggr/container/list.hpp>
#include <yggr/container_ex/is_allocator.hpp>

#include <yggr/lua_wrap/wrap_base_t.hpp>
#include <yggr/lua_wrap/wrap_allocator.hpp>
#include <yggr/lua_wrap/wrap_bidirectional_iterator.hpp>

#include <yggr/lua_wrap/wrap_reverse_iterator.hpp>

#include <yggr/lua_wrap/detail/luabind_function_noexcept.hpp>
#include <yggr/lua_wrap/detail/range_size_def.hpp>
#include <yggr/lua_wrap/detail/unwrap_t.hpp>

#include <boost/mpl/or.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/bool.hpp>

#include <ostream>

namespace yggr
{
namespace lua_wrap
{

template <typename Val, typename Ax,
			template<typename _Ty, typename _Ax> class List >
class wrap_list;

} // namespace lua_wrap
} // namespace yggr

namespace yggr
{
namespace lua_wrap
{
namespace detail
{

template<typename T>
struct is_wrap_list
	: public boost::mpl::false_
{
};

template<typename Val, typename Ax,
			template<typename _Val, typename _Ax> class List>
struct is_wrap_list< wrap_list<Val, Ax, List> >
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

template<typename Val,
			typename Ax = typename container::list<Val>::allocator_type,
			template<typename _Val, typename _Ax> class List = container::list
		>
class wrap_list
	//: public List<Val, Ax>
{

public:
	typedef List<Val, Ax> base_type;

	typedef typename base_type::allocator_type allocator_type;
	typedef typename base_type::value_type value_type;
	typedef typename base_type::size_type size_type;
	typedef typename base_type::difference_type difference;

	typedef typename base_type::reference reference;
	typedef typename base_type::const_reference const_reference;

	typedef typename base_type::pointer pointer;
	typedef typename base_type::const_pointer const_pointer;

	typedef typename base_type::iterator iterator;
	typedef typename base_type::const_iterator const_iterator;

	typedef typename base_type::reverse_iterator reverse_iterator;
	typedef typename base_type::const_reverse_iterator const_reverse_iterator;

private:
	typedef wrap_list this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	typedef typename wrap_base_t_maker<size_type>::type wrap_size_type;
	typedef wrap_allocator<allocator_type> wrap_allocator_type;
	typedef wrap_bidirectional_iterator<iterator> wrap_iterator_type;
	typedef wrap_bidirectional_const_iterator<const_iterator> wrap_const_iterator_type;
	typedef wrap_reverse_iterator<reverse_iterator> wrap_reverse_iterator_type;
	typedef wrap_reverse_iterator<const_reverse_iterator> wrap_const_reverse_iterator_type;

public:
	wrap_list(void)
	{
	}

	template<typename Alloc>
	explicit wrap_list(const Alloc& alloc,
							typename
								boost::enable_if
								<
									container_ex::is_allocator<Alloc>,
									mplex::sfinae_type
								>::type sfinae = 0)
		: _base(detail::unwrap_object(alloc))
	{
	}

	template<typename TSize>
	wrap_list(TSize count, const value_type& val,
					typename
						boost::enable_if
						<
							boost::is_arithmetic<TSize>,
							mplex::sfinae_type
						>::type sfinae = 0)
		: _base(static_cast<size_type>(count), val)
	{
	}

	template<typename TSize>
	wrap_list(const TSize& count, const value_type& val,
					typename
						boost::enable_if
						<
							detail::is_wrap_base_t<TSize>,
							mplex::sfinae_type
						>::type sfinae = 0)
		: _base(static_cast<size_type>(detail::unwrap_object(count)), val)
	{
	}

	template<typename TSize, typename Alloc>
	wrap_list(TSize count, const value_type& val, const Alloc& alloc,
					typename
						boost::enable_if
						<
							boost::mpl::and_
							<
								boost::is_arithmetic<TSize>,
								container_ex::is_allocator<Alloc>
							>,
							mplex::sfinae_type
						>::type sfinae = 0)
		: _base(
			static_cast<size_type>(count),
			val,
			detail::unwrap_object(alloc))
	{
	}

	template<typename TSize, typename Alloc>
	wrap_list(const TSize& count, const value_type& val, const Alloc& alloc,
					typename
						boost::enable_if
						<
							boost::mpl::and_
							<
								detail::is_wrap_base_t<TSize>,
								container_ex::is_allocator<Alloc>
							>,
							mplex::sfinae_type
						>::type sfinae = 0)
		: _base(
			static_cast<size_type>(detail::unwrap_object(count)),
			val,
			detail::unwrap_object(alloc))
	{
	}

	template<typename TSize>
	explicit wrap_list(TSize count,
						typename
							boost::enable_if
							<
								boost::is_arithmetic<TSize>,
								mplex::sfinae_type
							>::type sfinae = 0)
		: _base(static_cast<size_type>(count), value_type())
	{
	}

	template<typename TSize>
	explicit wrap_list(const TSize& count,
						typename
							boost::enable_if
							<
								detail::is_wrap_base_t<TSize>,
								mplex::sfinae_type
							>::type sfinae = 0)
		: _base(static_cast<size_type>(detail::unwrap_object(count)), value_type())
	{
	}

	template<typename TSize, typename Alloc>
	explicit wrap_list(TSize count, const Alloc& alloc,
						typename
							boost::enable_if
							<
								boost::mpl::and_
								<
									boost::is_arithmetic<TSize>,
									container_ex::is_allocator<Alloc>
								>,
								mplex::sfinae_type
							>::type sfinae = 0)
		: _base(
			static_cast<size_type>(count),
			value_type(),
			detail::unwrap_object(alloc))
	{
	}

	template<typename TSize, typename Alloc>
	explicit wrap_list(const TSize& count, const Alloc& alloc,
						typename
							boost::enable_if
							<
								boost::mpl::and_
								<
									detail::is_wrap_base_t<TSize>,
									container_ex::is_allocator<Alloc>
								>,
								mplex::sfinae_type
							>::type sfinae = 0)
		: _base(
			static_cast<size_type>(detail::unwrap_object(count)),
			value_type(),
			detail::unwrap_object(alloc))
	{
	}

	template<typename Iter>
	wrap_list(Iter s, Iter e,
				typename
					boost::enable_if
					<
						iterator_ex::is_iterator<Iter>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: _base(detail::unwrap_object(s), detail::unwrap_object(e))
	{
	}

	template<typename Iter, typename Alloc>
	wrap_list(Iter s, Iter e, const Alloc& alloc,
					typename
						boost::enable_if
						<
							boost::mpl::and_
							<
								iterator_ex::is_iterator<Iter>,
								container_ex::is_allocator<Alloc>
							>,
							mplex::sfinae_type
						>::type sfinae = 0)
		: _base(
			detail::unwrap_object(s),
			detail::unwrap_object(e),
			detail::unwrap_object(alloc))
	{
	}


	wrap_list(BOOST_RV_REF(base_type) base)
		: _base(boost::move(base))
	{
	}

	wrap_list(const base_type& base)
		: _base(base)
	{
	}

	template<typename Alloc>
	wrap_list(BOOST_RV_REF(base_type) right, const Alloc& alloc,
				typename
					boost::enable_if
					<
						container_ex::is_allocator<Alloc>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: _base(boost::move(right), detail::unwrap_object(alloc))
	{
	}

	template<typename Alloc>
	wrap_list(const base_type& right, const Alloc& alloc,
				typename
					boost::enable_if
					<
						container_ex::is_allocator<Alloc>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: _base(right, detail::unwrap_object(alloc))
	{
	}

	wrap_list(BOOST_RV_REF(this_type) right)
		: _base(boost::move(right._base))
	{
	}

	wrap_list(const this_type& right)
		: _base(right._base)
	{
	}

	template<typename Alloc>
	wrap_list(BOOST_RV_REF(this_type) right, const Alloc& alloc,
							typename
								boost::enable_if
								<
									container_ex::is_allocator<Alloc>,
									mplex::sfinae_type
								>::type sfinae = 0)
		: _base(boost::move(right._base), detail::unwrap_object(alloc))
	{
	}

	template<typename Alloc>
	wrap_list(const this_type& right, const Alloc& alloc,
				typename
					boost::enable_if
					<
						container_ex::is_allocator<Alloc>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: _base(right._base, detail::unwrap_object(alloc))
	{
	}

#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	wrap_list( std::initializer_list<value_type> il)
		: _base(boost::begin(il), boost::end(il))
	{
	}

	template<typename Alloc>
	wrap_list( std::initializer_list<value_type> il, const Alloc& alloc,
					typename
						boost::enable_if
						<
							container_ex::is_allocator<Alloc>,
							mplex::sfinae_type
						>::type sfinae = 0)
		: _base(boost::begin(il), boost::end(il), alloc)
	{
	}

#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	~wrap_list(void)
	{
	}

public:
	inline this_type& operator=(BOOST_RV_REF(base_type) right) YGGR_NOEXCEPT_OR_NOTHROW
	{
		base_type& right_ref = right;
		copy_or_move_or_swap(_base, boost::move(right_ref));
		return *this;
	}

	inline this_type& operator=(const base_type& right)
	{
		_base = right;
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right) YGGR_NOEXCEPT_OR_NOTHROW
	{
		this_type& right_ref = right;
		if(this == boost::addressof(right_ref))
		{
			return *this;
		}

		copy_or_move_or_swap(_base, boost::move(right_ref._base));

		return *this;
	}

	inline this_type& operator=(const this_type& right)
	{
		if(this == boost::addressof(right))
		{
			return *this;
		}

		_base = right._base;
		return *this;
	}

#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	inline this_type& operator=( std::initializer_list<value_type> il )
	{
		_base.assign(boost::begin(il), boost::end(il));
		return *this;
	}

#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

public:
	inline void swap(BOOST_RV_REF(base_type) right)
	{
		base_type& right_ref = right;
		this_type::swap(right_ref);
	}

	inline void swap(base_type& right)
	{
		_base.swap(right);
	}

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	inline void swap(this_type& right)
	{
		if(this == boost::addressof(right))
		{
			return;
		}
		_base.swap(right._base);
	}

public:
	inline operator base_type&(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _base;
	}

	inline operator const base_type&(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _base;
	}

public:
	inline base_type& unwrap(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _base;
	}

	inline const base_type& unwrap(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _base;
	}

	inline this_type copy(void) const
	{
		return *this;
	}

public:
	template<typename TSize> inline
	typename
		boost::enable_if
		<
			boost::is_arithmetic<TSize>,
			void
		>::type
		assign(TSize count, const value_type& val)
	{
		_base.assign(static_cast<size_type>(count), val);
	}

	template<typename TSize> inline
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<TSize>,
			void
		>::type
		assign(const TSize& count, const value_type& val)
	{
		_base.assign(static_cast<size_type>(count.value()), val);
	}

	template<typename InputIter> inline
	typename
		boost::enable_if
		<
			iterator_ex::is_iterator<InputIter>,
			void
		>::type
		assign(InputIter s, InputIter e)
	{
		_base.assign(detail::unwrap_object(s), detail::unwrap_object(e));
	}

#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	inline void assign( std::initializer_list<value_type> il )
	{
		_base.assign(boost::begin(il), boost::end(il));
	}

#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	inline allocator_type get_allocator(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _base.get_allocator();
	}

	// export get_allocator to lua
	inline wrap_allocator_type
		get_wraped_allocator(void) const YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return wrap_allocator_type(this_type::get_allocator());
	}

public:
	inline reference front(void)
	{
		return _base.front();
	}

	inline typename boost::mpl::if_<boost::is_class<value_type>, reference, value_type >::type
		wraped_front(void)
	{
		return _base.front();
	}

	inline typename boost::mpl::if_<boost::is_class<value_type>, const_reference, value_type >::type
		front(void) const
	{
		return _base.front();
	}

	inline reference back(void)
	{
		return _base.back();
	}

	inline typename boost::mpl::if_<boost::is_class<value_type>, reference, value_type >::type
		wraped_back(void)
	{
		return _base.back();
	}

	inline typename boost::mpl::if_<boost::is_class<value_type>, const_reference, value_type >::type
		back(void) const
	{
		return _base.back();
	}

	inline this_type& set_front(const value_type& val)
	{
		_base.front() = val;
		return *this;
	}

	inline this_type& set_back(const value_type& val)
	{
		_base.back() = val;
		return *this;
	}

public:
	// begin
	inline iterator begin(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _base.begin();
	}

	inline const_iterator begin(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _base.begin();
	}

	inline const_iterator cbegin(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::begin();
	}

	// exprot begin to lua
	inline wrap_iterator_type wraped_begin(void) YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return wrap_iterator_type(this_type::begin());
	}

	inline wrap_const_iterator_type wraped_begin(void) const YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return wrap_const_iterator_type(this_type::begin());
	}

	inline wrap_const_iterator_type wraped_cbegin(void) const YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return wrap_const_iterator_type(this_type::cbegin());
	}

	// end
	inline iterator end(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _base.end();
	}

	inline const_iterator end(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _base.end();
	}

	inline const_iterator cend(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::end();
	}

	// exprot end to lua
	inline wrap_iterator_type wraped_end(void) YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return wrap_iterator_type(this_type::end());
	}

	inline wrap_const_iterator_type wraped_end(void) const YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return wrap_const_iterator_type(this_type::end());
	}

	inline wrap_const_iterator_type wraped_cend(void) const YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return wrap_const_iterator_type(this_type::cend());
	}

	// rbegin
	inline reverse_iterator rbegin(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _base.rbegin();
	}

	inline const_reverse_iterator rbegin(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _base.rbegin();
	}

	inline const_reverse_iterator crbegin(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::rbegin();
	}

	// exprot rbegin to lua
	inline wrap_reverse_iterator_type wraped_rbegin(void) YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return wrap_reverse_iterator_type(this_type::rbegin());
	}

	inline wrap_const_reverse_iterator_type wraped_rbegin(void) const YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return wrap_const_reverse_iterator_type(this_type::rbegin());
	}

	inline wrap_const_reverse_iterator_type wraped_crbegin(void) const YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return wrap_const_reverse_iterator_type(this_type::crbegin());
	}

	// rend
	inline reverse_iterator rend(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _base.rend();
	}

	inline const_reverse_iterator rend(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _base.rend();
	}

	inline const_reverse_iterator crend(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::rend();
	}

	// exprot rend to lua
	inline wrap_reverse_iterator_type wraped_rend(void) YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return wrap_reverse_iterator_type(this_type::rend());
	}

	inline wrap_const_reverse_iterator_type wraped_rend(void) const YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return wrap_const_reverse_iterator_type(this_type::rend());
	}

	inline wrap_const_reverse_iterator_type wraped_crend(void) const YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return wrap_const_reverse_iterator_type(this_type::crend());
	}

public:
	inline bool empty(void) const YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return _base.empty();
	}

	inline size_type size(void) const YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return _base.size();
	}

	inline wrap_size_type wraped_size(void) const YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		typedef typename wrap_base_t_maker<size_type>::type wrap_base_size_type;
		return wrap_base_size_type(this_type::size());
	}

	inline size_type max_size(void) const YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return _base.max_size();
	}

	inline wrap_size_type wraped_max_size(void) const YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		typedef typename wrap_base_t_maker<size_type>::type wrap_base_size_type;
		return wrap_base_size_type(this_type::max_size());
	}

public:
	inline void clear(void) YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		_base.clear();
	}

	// insert
	inline iterator insert(const_iterator pos, BOOST_RV_REF(value_type) val)
	{
		return _base.insert(pos, boost::move(val));
	}

	inline iterator insert(const_iterator pos, const value_type& val)
	{
		return _base.insert(pos, val);
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			iterator
		>::type
		insert(const_iterator pos, T count, const value_type& val)
	{
		return _base.insert(pos, static_cast<size_type>(count), val);
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<T>,
			iterator
		>::type
		insert(const_iterator pos, const T& count, const value_type& val)
	{
		return _base.insert(pos, static_cast<size_type>(count.value()), val);
	}

	template<typename InputIter> inline
	typename
		boost::enable_if
		<
			iterator_ex::is_iterator<InputIter>,
			iterator
		>::type
		insert(const_iterator pos, InputIter s, InputIter e)
	{
		return _base.insert(pos, detail::unwrap_object(s), detail::unwrap_object(e));
	}

#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	inline iterator insert(const_iterator pos, std::initializer_list<value_type> il)
	{
		return _base.insert(pos, boost::begin(il), boost::end(il));
	}

#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	// export insert to lua
	template<typename WrapPosIter> inline
	typename
		boost::enable_if
		<
			iterator_ex::is_iterator<WrapPosIter>,
			wrap_iterator_type
		>::type
		wraped_insert(WrapPosIter pos, BOOST_RV_REF(value_type) val)
	{
		return wrap_iterator_type(this_type::insert(detail::unwrap_object(pos), boost::move(val)));
	}

	template<typename WrapPosIter> inline
	typename
		boost::enable_if
		<
			iterator_ex::is_iterator<WrapPosIter>,
			wrap_iterator_type
		>::type
		wraped_insert(WrapPosIter pos, const value_type& val)
	{
		return wrap_iterator_type(this_type::insert(detail::unwrap_object(pos), val));
	}

	template<typename WrapPosIter, typename T> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				iterator_ex::is_iterator<WrapPosIter>,
				boost::is_arithmetic<T>
			>,
			wrap_iterator_type
		>::type
		wraped_insert(WrapPosIter pos, T count, const value_type& val)
	{
		return wrap_iterator_type(this_type::insert(detail::unwrap_object(pos), count, val));
	}

	template<typename WrapPosIter, typename T> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				iterator_ex::is_iterator<WrapPosIter>,
				detail::is_wrap_base_t<T>
			>,
			wrap_iterator_type
		>::type
		wraped_insert(WrapPosIter pos, const T& count, const value_type& val)
	{
		return wrap_iterator_type(this_type::insert(detail::unwrap_object(pos), count.value(), val));
	}

	template<typename WrapPosIter, typename InputIter> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				iterator_ex::is_iterator<WrapPosIter>,
				iterator_ex::is_iterator<InputIter>
			>,
			wrap_iterator_type
		>::type
		wraped_insert(WrapPosIter pos, InputIter s, InputIter e)
	{
		return
			wrap_iterator_type(
				_base.insert(
					detail::unwrap_object(pos),
					detail::unwrap_object(s),
					detail::unwrap_object(e)));
	}

#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

	template<typename ...Args> inline
	iterator emplace(const_iterator pos, BOOST_FWD_REF(Args) ...args)
	{
		return container::emplace(_base, pos, boost::forward<Args>(args)...);
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		iterator emplace(const_iterator pos \
							YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			return \
				container::emplace( \
					_base, pos \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE()
#	undef YGGR_PP_FOO_ARG_NAME

#endif // #if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

	inline iterator erase( const_iterator pos )
	{
		return _base.erase(pos);
	}

	inline iterator erase( const_iterator s, const_iterator e )
	{
		return _base.erase(s, e);
	}

	// export erase to lua
	template<typename WrapPosIter> inline
	typename
		boost::enable_if
		<
			iterator_ex::is_iterator<WrapPosIter>,
			wrap_iterator_type
		>::type
		wraped_erase(WrapPosIter pos)
	{
		return _base.erase(detail::unwrap_object(pos));
	}

	template<typename WrapPosIter1, typename WrapPosIter2> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				iterator_ex::is_iterator<WrapPosIter1>,
				iterator_ex::is_iterator<WrapPosIter2>
			>,
			wrap_iterator_type
		>::type
		wraped_erase(WrapPosIter1 s, WrapPosIter2 e)
	{
		return
			_base.erase(
				detail::unwrap_object(s),
				detail::unwrap_object(e) );
	}

	inline void push_back(BOOST_RV_REF(value_type) val)
	{
		_base.push_back(boost::move(val));
	}

	inline void push_back(const value_type& val)
	{
		_base.push_back(val);
	}

#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

	template<typename... Args > inline
	reference emplace_back(BOOST_FWD_REF(Args) ...args)
	{
		return container::emplace_back(_base, boost::forward<Args>(args)...);
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		reference emplace_back( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			return \
				container::emplace_back( \
					_base \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE()
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	inline void pop_back(void)
	{
		_base.pop_back();
	}

	inline void push_front(BOOST_RV_REF(value_type) val)
	{
		_base.push_front(boost::move(val));
	}

	inline void push_front(const value_type& val)
	{
		_base.push_front(val);
	}

#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

	template<typename... Args > inline
	reference emplace_front(BOOST_FWD_REF(Args) ...args)
	{
		return container::emplace_front(_base, boost::forward<Args>(args)...);
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		reference emplace_front( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			return \
				container::emplace_front( \
					_base \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE()
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	inline void pop_front(void)
	{
		_base.pop_front();
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			void
		>::type
		resize(T count)
	{
		_base.resize(static_cast<size_type>(count));
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<T>,
			void
		>::type
		resize(const T& count)
	{
		_base.resize(static_cast<size_type>(count.value()));
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			void
		>::type
		resize(T count, const value_type& val)
	{
		_base.resize(static_cast<size_type>(count), val);
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<T>,
			void
		>::type
		resize(const T& count, const value_type& val)
	{
		_base.resize(static_cast<size_type>(count.value()), val);
	}

// operations
public:
	// merge
	inline void merge(BOOST_RV_REF(base_type) right)
	{
		_base.merge(boost::move(right));
	}

	inline void merge(base_type& right)
	{
		_base.merge(right);
	}

	template<typename Pred> inline
	void merge(BOOST_RV_REF(base_type) right, Pred pred)
	{
		_base.merge(boost::move(right), pred);
	}

	template<typename Pred> inline
	void merge(base_type& right, Pred pred)
	{
		_base.merge(right, pred);
	}

	inline void merge(BOOST_RV_REF(this_type) right)
	{
		_base.merge(boost::move(right._base));
	}

	inline void merge(this_type& right)
	{
		_base.merge(right._base);
	}

	template<typename Pred> inline
	void merge(BOOST_RV_REF(this_type) right, Pred pred)
	{
		_base.merge(boost::move(right._base), pred);
	}

	template<typename Pred> inline
	void merge(this_type& right, Pred pred)
	{
		_base.merge(right._base, pred);
	}

	inline void merge_equal_to(this_type& right)
	{
		_base.merge(right._base, std::equal_to<value_type>());
	}

	inline void merge_not_equal_to(this_type& right)
	{
		_base.merge(right._base, std::not_equal_to<value_type>());
	}

	inline void merge_less(this_type& right)
	{
		_base.merge(right._base, std::less<value_type>());
	}

	inline void merge_less_equal(this_type& right)
	{
		_base.merge(right._base, std::less_equal<value_type>());
	}

	inline void merge_greater(this_type& right)
	{
		_base.merge(right._base, std::greater<value_type>());
	}

	inline void merge_greater_equal(this_type& right)
	{
		_base.merge(right._base, std::greater_equal<value_type>());
	}

	// splice
	template<typename PosIter> inline
	typename
		boost::enable_if
		<
			iterator_ex::is_iterator<PosIter>,
			void
		>::type
		splice(PosIter pos, BOOST_RV_REF(base_type) right)
	{
		_base.splice(detail::unwrap_object(pos), boost::move(right));
	}

	template<typename PosIter> inline
	typename
		boost::enable_if
		<
			iterator_ex::is_iterator<PosIter>,
			void
		>::type
		splice(PosIter pos, base_type& right)
	{
		_base.splice(detail::unwrap_object(pos), right);
	}

	template<typename PosIter, typename Iter> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				iterator_ex::is_iterator<PosIter>,
				iterator_ex::is_iterator<Iter>
			>,
			void
		>::type
		splice(PosIter pos, BOOST_RV_REF(base_type) right, Iter it)
	{
		_base.splice(
			detail::unwrap_object(pos),
			boost::move(right),
			detail::unwrap_object(it));
	}


	template<typename PosIter, typename Iter> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				iterator_ex::is_iterator<PosIter>,
				iterator_ex::is_iterator<Iter>
			>,
			void
		>::type
		splice(PosIter pos, base_type& right, Iter it)
	{
		_base.splice(
			detail::unwrap_object(pos),
			right,
			detail::unwrap_object(it));
	}

	template<typename PosIter, typename Iter1, typename Iter2> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				iterator_ex::is_iterator<PosIter>,
				iterator_ex::is_iterator<Iter1>,
				iterator_ex::is_iterator<Iter2>
			>,
			void
		>::type
		splice(PosIter pos, BOOST_RV_REF(base_type) right, Iter1 s, Iter2 e)
	{
		_base.splice(
			const_iterator(detail::unwrap_object(pos)),
			boost::move(right),
			const_iterator(detail::unwrap_object(s)),
			const_iterator(detail::unwrap_object(e)) );
	}

	template<typename PosIter, typename Iter1, typename Iter2> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				iterator_ex::is_iterator<PosIter>,
				iterator_ex::is_iterator<Iter1>,
				iterator_ex::is_iterator<Iter2>
			>,
			void
		>::type
		splice(PosIter pos, base_type& right, Iter1 s, Iter2 e)
	{
		_base.splice(
			const_iterator(detail::unwrap_object(pos)),
			right,
			const_iterator(detail::unwrap_object(s)),
			const_iterator(detail::unwrap_object(e)) );
	}

	template<typename PosIter> inline
	typename
		boost::enable_if
		<
			iterator_ex::is_iterator<PosIter>,
			void
		>::type
		splice(PosIter pos, BOOST_RV_REF(this_type) right)
	{
		this_type::splice(const_iterator(detail::unwrap_object(pos)), boost::move(right._base));
	}

	template<typename PosIter> inline
	typename
		boost::enable_if
		<
			iterator_ex::is_iterator<PosIter>,
			void
		>::type
		splice(PosIter pos, this_type& right)
	{
		this_type::splice(const_iterator(detail::unwrap_object(pos)), right._base);
	}

	template<typename PosIter, typename Iter> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				iterator_ex::is_iterator<PosIter>,
				iterator_ex::is_iterator<Iter>
			>,
			void
		>::type
		splice(PosIter pos, BOOST_RV_REF(this_type) right, Iter it)
	{
		this_type::splice(
			const_iterator(detail::unwrap_object(pos)),
			boost::move(right._base),
			const_iterator(detail::unwrap_object(it)));
	}

	template<typename PosIter, typename Iter> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				iterator_ex::is_iterator<PosIter>,
				iterator_ex::is_iterator<Iter>
			>,
			void
		>::type
		splice(PosIter pos, this_type& right, Iter it)
	{
		this_type::splice(
			const_iterator(detail::unwrap_object(pos)),
			boost::move(right._base),
			const_iterator(detail::unwrap_object(it)));
	}

	template<typename PosIter, typename Iter1, typename Iter2> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				iterator_ex::is_iterator<PosIter>,
				iterator_ex::is_iterator<Iter1>,
				iterator_ex::is_iterator<Iter2>
			>,
			void
		>::type
		splice(PosIter pos, BOOST_RV_REF(this_type) right, Iter1 s, Iter2 e)
	{
		this_type::splice(
			const_iterator(detail::unwrap_object(pos)),
			boost::move(right._base),
			const_iterator(detail::unwrap_object(s)),
			const_iterator(detail::unwrap_object(e)) );
	}

	template<typename PosIter, typename Iter1, typename Iter2> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				iterator_ex::is_iterator<PosIter>,
				iterator_ex::is_iterator<Iter1>,
				iterator_ex::is_iterator<Iter2>
			>,
			void
		>::type
		splice(PosIter pos, this_type& right, Iter1 s, Iter2 e)
	{
		this_type::splice(
			const_iterator(detail::unwrap_object(pos)),
			right._base,
			const_iterator(detail::unwrap_object(s)),
			const_iterator(detail::unwrap_object(e)) );
	}

	// remove
	inline size_type remove(const value_type& value)
	{
		size_type old_size = _base.size();
		_base.remove(value);
		return old_size - _base.size();
	}

	inline wrap_size_type wraped_remove(const value_type& value)
	{
		return wrap_size_type(this_type::remove(value));
	}

	// remove_if
	template<typename Pred> inline
	size_type remove_if(Pred pred)
	{
		size_type old_size = _base.size();
		_base.remove_if(pred);
		return old_size - _base.size();
	}

	template<typename Pred> inline
	wrap_size_type wraped_remove_if(Pred pred)
	{
		return wrap_size_type(this_type::wraped_remove_if(pred));
	}

	// reverse
	inline void reverse(void) YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		_base.reverse();
	}

	// unique
	inline size_type unique(void)
	{
		size_type old_size = _base.size();
		_base.unique();
		return old_size - _base.size();
	}

	template<typename Pred> inline
	size_type unique(Pred pred)
	{
		size_type old_size = _base.size();
		_base.unique(pred);
		return old_size - _base.size();
	}

	inline size_type unique_equal_to(void)
	{
		return this_type::unique(std::equal_to<value_type>());
	}

	inline size_type unique_not_equal_to(void)
	{
		return this_type::unique(std::not_equal_to<value_type>());
	}

	inline size_type unique_less(void)
	{
		return this_type::unique(std::less<value_type>());
	}

	inline size_type unique_less_equal(void)
	{
		return this_type::unique(std::less_equal<value_type>());
	}

	inline size_type unique_greater(void)
	{
		return this_type::unique(std::greater<value_type>());
	}

	inline size_type unique_greater_equal(void)
	{
		return this_type::unique(std::greater_equal<value_type>());
	}

	inline wrap_size_type wraped_unique(void)
	{
		return wrap_size_type(this_type::unique());
	}

	template<typename Pred> inline
	wrap_size_type wraped_unique(Pred pred)
	{
		return wrap_size_type(this_type::unique(pred));
	}

	inline wrap_size_type wraped_unique_equal_to(void)
	{
		return wrap_size_type(this_type::unique_equal_to());
	}

	inline wrap_size_type wraped_unique_not_equal_to(void)
	{
		return wrap_size_type(this_type::unique_not_equal_to());
	}

	inline wrap_size_type wraped_unique_less(void)
	{
		return wrap_size_type(this_type::unique_less());
	}

	inline wrap_size_type wraped_unique_less_equal(void)
	{
		return wrap_size_type(this_type::unique_less_equal());
	}

	inline wrap_size_type wraped_unique_greater(void)
	{
		return wrap_size_type(this_type::unique_greater());
	}

	inline wrap_size_type wraped_unique_greater_equal(void)
	{
		return wrap_size_type(this_type::unique_greater_equal());
	}

	// sort
	inline void sort(void)
	{
		_base.sort();
	}

	template<typename Pred> inline
	void sort(Pred pred)
	{
		_base.sort(pred);
	}

	inline void sort_aesc(void)
	{
		_base.sort(std::less<value_type>());
	}

	inline void sort_desc(void)
	{
		_base.sort(std::greater<value_type>());
	}

protected:
	base_type _base;
};

// non-member foo

// ==
template<template<typename _T, typename _Ax> class Deque,
			typename T, typename Ax> inline
bool operator==(const typename wrap_list<T, Ax, Deque>::base_type& l,
				const wrap_list<T, Ax, Deque>& r)
{
	return l == r.unwrap();
}

template<template<typename _T, typename _Ax> class Deque,
			typename T, typename Ax> inline
bool operator==(const wrap_list<T, Ax, Deque>& l,
				const typename wrap_list<T, Ax, Deque>::base_type& r)
{
	return l.unwrap() == r;
}

template<template<typename _T, typename _Ax> class Deque,
			typename T, typename Ax> inline
bool operator==(const wrap_list<T, Ax, Deque>& l,
				const wrap_list<T, Ax, Deque>& r)
{
	return l.unwrap() == r.unwrap();
}

// !=
template<template<typename _T, typename _Ax> class Deque,
			typename T, typename Ax> inline
bool operator!=(const typename wrap_list<T, Ax, Deque>::base_type& l,
				const wrap_list<T, Ax, Deque>& r)
{
	return l != r.unwrap();
}

template<template<typename _T, typename _Ax> class Deque,
			typename T, typename Ax> inline
bool operator!=(const wrap_list<T, Ax, Deque>& l,
				const typename wrap_list<T, Ax, Deque>::base_type& r)
{
	return l.unwrap() != r;
}

template<template<typename _T, typename _Ax> class Deque,
			typename T, typename Ax> inline
bool operator!=(const wrap_list<T, Ax, Deque>& l,
				const wrap_list<T, Ax, Deque>& r)
{
	return l.unwrap() != r.unwrap();
}

// <
template<template<typename _T, typename _Ax> class Deque,
			typename T, typename Ax> inline
bool operator<(const typename wrap_list<T, Ax, Deque>::base_type& l,
				const wrap_list<T, Ax, Deque>& r)
{
	return l < r.unwrap();
}

template<template<typename _T, typename _Ax> class Deque,
			typename T, typename Ax> inline
bool operator<(const wrap_list<T, Ax, Deque>& l,
				const typename wrap_list<T, Ax, Deque>::base_type& r)
{
	return l.unwrap() < r;
}

template<template<typename _T, typename _Ax> class Deque,
			typename T, typename Ax> inline
bool operator<(const wrap_list<T, Ax, Deque>& l,
				const wrap_list<T, Ax, Deque>& r)
{
	return l.unwrap() < r.unwrap();
}

// <=
template<template<typename _T, typename _Ax> class Deque,
			typename T, typename Ax> inline
bool operator<=(const typename wrap_list<T, Ax, Deque>::base_type& l,
				const wrap_list<T, Ax, Deque>& r)
{
	return l <= r.unwrap();
}

template<template<typename _T, typename _Ax> class Deque,
			typename T, typename Ax> inline
bool operator<=(const wrap_list<T, Ax, Deque>& l,
				const typename wrap_list<T, Ax, Deque>::base_type& r)
{
	return l.unwrap() <= r;
}

template<template<typename _T, typename _Ax> class Deque,
			typename T, typename Ax> inline
bool operator<=(const wrap_list<T, Ax, Deque>& l,
				const wrap_list<T, Ax, Deque>& r)
{
	return l.unwrap() <= r.unwrap();
}

// >
template<template<typename _T, typename _Ax> class Deque,
			typename T, typename Ax> inline
bool operator>(const typename wrap_list<T, Ax, Deque>::base_type& l,
				const wrap_list<T, Ax, Deque>& r)
{
	return l > r.unwrap();
}

template<template<typename _T, typename _Ax> class Deque,
			typename T, typename Ax> inline
bool operator>(const wrap_list<T, Ax, Deque>& l,
				const typename wrap_list<T, Ax, Deque>::base_type& r)
{
	return l.unwrap() > r;
}

template<template<typename _T, typename _Ax> class Deque,
			typename T, typename Ax> inline
bool operator>(const wrap_list<T, Ax, Deque>& l,
				const wrap_list<T, Ax, Deque>& r)
{
	return l.unwrap() > r.unwrap();
}

// >=
template<template<typename _T, typename _Ax> class Deque,
			typename T, typename Ax> inline
bool operator>=(const typename wrap_list<T, Ax, Deque>::base_type& l,
				const wrap_list<T, Ax, Deque>& r)
{
	return l >= r.unwrap();
}

template<template<typename _T, typename _Ax> class Deque,
			typename T, typename Ax> inline
bool operator>=(const wrap_list<T, Ax, Deque>& l,
				const typename wrap_list<T, Ax, Deque>::base_type& r)
{
	return l.unwrap() >= r;
}

template<template<typename _T, typename _Ax> class Deque,
			typename T, typename Ax> inline
bool operator>=(const wrap_list<T, Ax, Deque>& l,
				const wrap_list<T, Ax, Deque>& r)
{
	return l.unwrap() >= r.unwrap();
}

template<template<typename _T, typename _Ax> class Deque,
			typename Char, typename Traits, typename T, typename Ax>
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os,
				const wrap_list<T, Ax, Deque>& right)
{
	typedef T val_type;
	typedef Ax alloc_type;
	typedef wrap_list<val_type, alloc_type, Deque> wrap_vector_type;
	typedef typename wrap_vector_type::base_type vector_type;
	typedef typename vector_type::const_iterator citer_type;

	{
		citer_type i = right.begin(), isize = right.end();

		if(i != isize)
		{
			os << *i;
			++i;
		}

		for(; i != isize; ++i)
		{
			os << ", " << *i;
		}
	}

	return os;
}

} // namespce lua_wrap
} // namespace yggr

namespace yggr
{
namespace lua_wrap
{

YGGR_PP_LUA_WRAP_CONTAINER_RANGE_SIZE_SUPPORT(2, wrap_list)

} // namespce lua_wrap
} // namespace yggr


// swap
namespace yggr
{
namespace lua_wrap
{
namespace swap_support
{

template<typename Val, typename Ax,
			template<typename _Val, typename _Ax> class List> inline
void swap(wrap_list<Val, Ax, List>& l, typename wrap_list<Val, Ax, List>::base_type& r)
{
	l.swap(r);
}

template<typename Val, typename Ax,
			template<typename _Val, typename _Ax> class List> inline
void swap(typename wrap_list<Val, Ax, List>::base_type& l, wrap_list<Val, Ax, List>& r)
{
	r.swap(l);
}

template<typename Val, typename Ax,
			template<typename _Val, typename _Ax> class List> inline
void swap(wrap_list<Val, Ax, List>& l, wrap_list<Val, Ax, List>& r)
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

#endif //__YGGR_LUA_WRAP_WRAP_LIST_HPP__
