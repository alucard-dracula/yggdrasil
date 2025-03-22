//wrap_vector.hpp

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

#ifndef __YGGR_LUA_WRAP_WRAP_VECTOR_HPP__
#define __YGGR_LUA_WRAP_WRAP_VECTOR_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/algorithm/shuffle.hpp>

#include <yggr/mplex/tag_sfinae.hpp>

#include <yggr/ppex/foo_params.hpp>
#include <yggr/ppex/symbol_if.hpp>
#include <yggr/ppex/symbols.hpp>

#include <yggr/iterator_ex/is_iterator.hpp>

#include <yggr/container/vector.hpp>
#include <yggr/container_ex/is_allocator.hpp>

#include <yggr/lua_wrap/wrap_base_t.hpp>
#include <yggr/lua_wrap/wrap_allocator.hpp>
#include <yggr/lua_wrap/wrap_random_access_iterator.hpp>
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
			template<typename _Ty, typename _Ax> class Vector >
class wrap_vector;

} // namespace lua_wrap
} // namespace yggr

namespace yggr
{
namespace lua_wrap
{
namespace detail
{

template<typename T>
struct is_wrap_vector
	: public boost::mpl::false_
{
};

template<typename Val, typename Ax,
			template<typename _Val, typename _Ax> class Vector>
struct is_wrap_vector< wrap_vector<Val, Ax, Vector> >
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

template <typename Val,
			typename Ax = typename mplex::typename_expand_get<container::vector<Val>, 1>::type,
			template<typename _Ty, typename _Ax> class Vector = container::vector
		>
class wrap_vector
{
public:
	typedef Vector<Val, Ax> base_type;

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
	typedef wrap_vector this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	typedef typename wrap_base_t_maker<size_type>::type wrap_size_type;
	typedef wrap_allocator<allocator_type> wrap_allocator_type;
	typedef wrap_random_access_iterator<iterator> wrap_iterator_type;
	typedef wrap_random_access_const_iterator<const_iterator> wrap_const_iterator_type;
	typedef wrap_reverse_iterator<reverse_iterator> wrap_reverse_iterator_type;
	typedef wrap_reverse_iterator<const_reverse_iterator> wrap_const_reverse_iterator_type;

public:
	YGGR_CONSTEXPR wrap_vector(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
	}

	template<typename Alloc> YGGR_CONSTEXPR
	explicit wrap_vector(const Alloc& alloc,
							typename
								boost::enable_if
								<
									container_ex::is_allocator<Alloc>,
									mplex::sfinae_type
								>::type sfinae = 0) YGGR_NOEXCEPT_OR_NOTHROW
		: _base(detail::unwrap_object(alloc))
	{
	}

	template<typename TSize> YGGR_CONSTEXPR
	wrap_vector(TSize count, const value_type& val,
					typename
						boost::enable_if
						<
							boost::is_arithmetic<TSize>,
							mplex::sfinae_type
						>::type sfinae = 0)
		: _base(static_cast<size_type>(count), val)
	{
	}

	template<typename TSize> YGGR_CONSTEXPR
	wrap_vector(const TSize& count, const value_type& val,
					typename
						boost::enable_if
						<
							detail::is_wrap_base_t<TSize>,
							mplex::sfinae_type
						>::type sfinae = 0)
		: _base(static_cast<size_type>(detail::unwrap_object(count)), val)
	{
	}

	template<typename TSize, typename Alloc> YGGR_CONSTEXPR
	wrap_vector(TSize count, const value_type& val, const Alloc& alloc,
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

	template<typename TSize, typename Alloc> YGGR_CONSTEXPR
	wrap_vector(const TSize& count, const value_type& val, const Alloc& alloc,
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

	template<typename TSize> YGGR_CONSTEXPR
	explicit wrap_vector(TSize count,
							typename
								boost::enable_if
								<
									boost::is_arithmetic<TSize>,
									mplex::sfinae_type
								>::type sfinae = 0)
		: _base(static_cast<size_type>(count), value_type())
	{
	}

	template<typename TSize> YGGR_CONSTEXPR
	explicit wrap_vector(const TSize& count,
							typename
								boost::enable_if
								<
									detail::is_wrap_base_t<TSize>,
									mplex::sfinae_type
								>::type sfinae = 0)
		: _base(static_cast<size_type>(detail::unwrap_object(count)), value_type())
	{
	}

	template<typename TSize, typename Alloc> YGGR_CONSTEXPR
	explicit wrap_vector(TSize count, const Alloc& alloc,
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

	template<typename TSize, typename Alloc> YGGR_CONSTEXPR
	explicit wrap_vector(const TSize& count, const Alloc& alloc,
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

	template<typename Iter> YGGR_CONSTEXPR
	wrap_vector(Iter s, Iter e,
					typename
						boost::enable_if
						<
							iterator_ex::is_iterator<Iter>,
							mplex::sfinae_type
						>::type sfinae = 0)
		: _base(detail::unwrap_object(s), detail::unwrap_object(e))
	{
	}

	template<typename Iter, typename Alloc> YGGR_CONSTEXPR
	wrap_vector(Iter s, Iter e, const Alloc& alloc,
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


	YGGR_CONSTEXPR wrap_vector(BOOST_RV_REF(base_type) base)
		: _base(boost::move(base))
	{
	}

	YGGR_CONSTEXPR wrap_vector(const base_type& base)
		: _base(base)
	{
	}

	template<typename Alloc> YGGR_CONSTEXPR
	wrap_vector(BOOST_RV_REF(base_type) right, const Alloc& alloc,
					typename
						boost::enable_if
						<
							container_ex::is_allocator<Alloc>,
							mplex::sfinae_type
						>::type sfinae = 0)
		: _base(boost::move(right), detail::unwrap_object(alloc))
	{
	}

	template<typename Alloc> YGGR_CONSTEXPR
	wrap_vector(const base_type& right, const Alloc& alloc,
					typename
						boost::enable_if
						<
							container_ex::is_allocator<Alloc>,
							mplex::sfinae_type
						>::type sfinae = 0)
		: _base(right, detail::unwrap_object(alloc))
	{
	}

	YGGR_CONSTEXPR wrap_vector(BOOST_RV_REF(this_type) right)
		: _base(boost::move(right._base))
	{
	}

	YGGR_CONSTEXPR wrap_vector(const this_type& right)
		: _base(right._base)
	{
	}

	template<typename Alloc> YGGR_CONSTEXPR
	wrap_vector(BOOST_RV_REF(this_type) right, const Alloc& alloc,
					typename
						boost::enable_if
						<
							container_ex::is_allocator<Alloc>,
							mplex::sfinae_type
						>::type sfinae = 0)
		: _base(boost::move(right._base), detail::unwrap_object(alloc))
	{
	}

	template<typename Alloc> YGGR_CONSTEXPR
	wrap_vector(const this_type& right, const Alloc& alloc,
					typename
						boost::enable_if
						<
							container_ex::is_allocator<Alloc>,
							mplex::sfinae_type
						>::type sfinae = 0)
		: _base(right._base, detail::unwrap_object(alloc))
	{
	}

	template<typename Ax2> YGGR_CONSTEXPR
	wrap_vector(BOOST_RV_REF_BEG Vector<value_type, Ax2> BOOST_RV_REF_END base,
					typename
						boost::enable_if
						<
							container::is_isomeric_same_vector
							<
								base_type,
								Vector<value_type, Ax2>
							>,
							mplex::sfinae_type
						>::type sfinae = 0)
		: _base(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<Vector<value_type, Ax2>&>(base))))
	{
	}

	template<typename Ax2> YGGR_CONSTEXPR
	wrap_vector(const Vector<value_type, Ax2>& base,
				typename
					boost::enable_if
					<
						container::is_isomeric_same_vector
						<
							base_type,
							Vector<value_type, Ax2>
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: _base(reinterpret_cast<const base_type&>(base))
	{
	}

	template<typename Ax2, typename Alloc> YGGR_CONSTEXPR
	wrap_vector(BOOST_RV_REF_BEG
					Vector<value_type, Ax2>
				BOOST_RV_REF_END right,
				const Alloc& alloc,
				typename
					boost::enable_if
					<
						boost::mpl::and_
						<
							container_ex::is_allocator<Alloc>,
							container::is_isomeric_same_vector
							<
								base_type,
								Vector<value_type, Ax2>
							>
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: _base(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<Vector<value_type, Ax2>&>(right))),
			detail::unwrap_object(alloc))
	{
	}

	template<typename Ax2, typename Alloc> YGGR_CONSTEXPR
	wrap_vector(const Vector<value_type, Ax2>& right,
				const Alloc& alloc,
				typename
					boost::enable_if
					<
						boost::mpl::and_
						<
							container_ex::is_allocator<Alloc>,
							container::is_isomeric_same_vector
							<
								base_type,
								Vector<value_type, Ax2>
							>
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: _base(
			reinterpret_cast<const base_type&>(right),
			detail::unwrap_object(alloc))
	{
	}

	template<typename Ax2> YGGR_CONSTEXPR
	wrap_vector(BOOST_RV_REF_BEG
					wrap_vector<value_type, Ax2>
				BOOST_RV_REF_END right,
				typename
					boost::enable_if
					<
						container::is_isomeric_same_vector
						<
							base_type,
							typename wrap_vector<value_type, Ax2>::base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: _base(boost::move(reinterpret_cast<base_type&>(right.unwrap())))
	{
	}

	template<typename Ax2> YGGR_CONSTEXPR
	wrap_vector(const wrap_vector<value_type, Ax2>& right,
				typename
					boost::enable_if
					<
						container::is_isomeric_same_vector
						<
							base_type,
							typename wrap_vector<value_type, Ax2>::base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: _base(reinterpret_cast<const base_type&>(right.unwrap()))
	{
	}

	template<typename Ax2, typename Alloc> YGGR_CONSTEXPR
	wrap_vector(BOOST_RV_REF_BEG
					wrap_vector<value_type, Ax2>
				BOOST_RV_REF_END right,
				const Alloc& alloc,
				typename
					boost::enable_if
					<
						boost::mpl::and_
						<
							container_ex::is_allocator<Alloc>,
							container::is_isomeric_same_vector
							<
								base_type,
								typename wrap_vector<value_type, Ax2>::base_type
							>
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: _base(
			boost::move(reinterpret_cast<base_type&>(right.unwrap())),
			detail::unwrap_object(alloc))
	{
	}

	template<typename Ax2, typename Alloc> YGGR_CONSTEXPR
	wrap_vector(const wrap_vector<value_type, Ax2>& right,
				const Alloc& alloc,
				typename
					boost::enable_if
					<
						boost::mpl::and_
						<
							container_ex::is_allocator<Alloc>,
							container::is_isomeric_same_vector
							<
								base_type,
								typename wrap_vector<value_type, Ax2>::base_type
							>
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: _base(
			reinterpret_cast<const base_type&>(right.unwrap()),
			detail::unwrap_object(alloc))
	{
	}

#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	YGGR_CONSTEXPR wrap_vector( std::initializer_list<value_type> il)
		: _base(boost::begin(il), boost::end(il))
	{
	}

	template<typename Alloc> YGGR_CONSTEXPR
	wrap_vector( std::initializer_list<value_type> il, const Alloc& alloc,
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

	YGGR_CXX20_CONSTEXPR ~wrap_vector(void)
	{
	}

public:
	YGGR_CXX14_CONSTEXPR_OR_INLINE this_type& operator=(BOOST_RV_REF(base_type) right) YGGR_NOEXCEPT_OR_NOTHROW
	{
		base_type& right_ref = right;
		copy_or_move_or_swap(_base, boost::move(right_ref));
		return *this;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE this_type& operator=(const base_type& right)
	{
		_base = right;
		return *this;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE this_type& operator=(BOOST_RV_REF(this_type) right) YGGR_NOEXCEPT_OR_NOTHROW
	{
		this_type& right_ref = right;
		if(this == boost::addressof(right_ref))
		{
			return *this;
		}

		copy_or_move_or_swap(_base, boost::move(right_ref._base));

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

	template<typename Ax2> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			container::is_isomeric_same_vector
			<
				base_type,
				Vector<value_type, Ax2>
			>,
			this_type&
		>::type
		operator=(BOOST_RV_REF_BEG
					Vector<value_type, Ax>
					BOOST_RV_REF_END right) YGGR_NOEXCEPT_OR_NOTHROW
	{
		typedef Vector<value_type, Ax> now_vt_type;
		now_vt_type& right_ref = right;
		copy_or_move_or_swap(_base, boost::move(reinterpret_cast<base_type&>(right_ref)));
		return *this;
	}

	template<typename Ax2> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			container::is_isomeric_same_vector
			<
				base_type,
				Vector<value_type, Ax2>
			>,
			this_type&
		>::type
		operator=(const Vector<value_type, Ax2>& right)
	{
		_base = reinterpret_cast<const base_type&>(right);
		return *this;
	}

	template<typename Ax2> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			container::is_isomeric_same_vector
			<
				base_type,
				typename wrap_vector<value_type, Ax2>::base_type
			>,
			this_type&
		>::type
		operator=(BOOST_RV_REF_BEG
					wrap_vector<value_type, Ax2>
					BOOST_RV_REF_END right) YGGR_NOEXCEPT_OR_NOTHROW
	{
		typedef wrap_vector<value_type, Ax2> now_vt_type;
		now_vt_type& right_ref = right;
		copy_or_move_or_swap(_base, boost::move(reinterpret_cast<base_type&>(right_ref.unwrap())));

		return *this;
	}

	template<typename Ax2> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			container::is_isomeric_same_vector
			<
				base_type,
				typename wrap_vector<value_type, Ax2>::base_type
			>,
			this_type&
		>::type
		operator=(const this_type& right)
	{
		typedef wrap_vector<value_type, Ax2> now_vt_type;

		_base = reinterpret_cast<const base_type&>(right.unwrap());
		return *this;
	}

#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	YGGR_CXX14_CONSTEXPR_OR_INLINE this_type& operator=( std::initializer_list<value_type> il )
	{
		_base.assign(boost::begin(il), boost::end(il));
		return *this;
	}

#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

public:
	YGGR_CXX14_CONSTEXPR_OR_INLINE void swap(BOOST_RV_REF(base_type) right) YGGR_NOEXCEPT_OR_NOTHROW
	{
		base_type& right_ref = right;
		this_type::swap(right_ref);
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE void swap(base_type& right) YGGR_NOEXCEPT_OR_NOTHROW
	{
		_base.swap(right);
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE void swap(BOOST_RV_REF(this_type) right) YGGR_NOEXCEPT_OR_NOTHROW
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE void swap(this_type& right) YGGR_NOEXCEPT_OR_NOTHROW
	{
		if(this == boost::addressof(right))
		{
			return;
		}
		_base.swap(right._base);
	}

	template<typename Ax2> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			container::is_isomeric_same_vector
			<
				base_type,
				Vector<value_type, Ax2>
			>,
			void
		>::type
		swap(BOOST_RV_REF_BEG
				Vector<value_type, Ax2>
				BOOST_RV_REF_END right) YGGR_NOEXCEPT_OR_NOTHROW
	{
		typedef Vector<value_type, Ax2> now_vt_type;
		now_vt_type& right_ref = right;

		base_type::swap(reinterpret_cast<base_type&>(right_ref));
	}

	template<typename Ax2> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			container::is_isomeric_same_vector
			<
				base_type,
				Vector<value_type, Ax2>
			>,
			void
		>::type
		swap(Vector<value_type, Ax2>& right) YGGR_NOEXCEPT_OR_NOTHROW
	{
		base_type::swap(reinterpret_cast<base_type&>(right));
	}

	template<typename Ax2> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			container::is_isomeric_same_vector
			<
				base_type,
				typename wrap_vector<value_type, Ax2>::base_type
			>,
			void
		>::type
		swap(BOOST_RV_REF_BEG
				wrap_vector<value_type, Ax2>
				BOOST_RV_REF_END right) YGGR_NOEXCEPT_OR_NOTHROW
	{
		typedef wrap_vector<value_type, Ax2> now_vt_type;
		now_vt_type& right_ref = right;
		base_type::swap(reinterpret_cast<base_type&>(right_ref.unwrap()));
	}

	template<typename Ax2> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			container::is_isomeric_same_vector
			<
				base_type,
				typename wrap_vector<value_type, Ax2>::base_type
			>,
			void
		>::type
		swap(wrap_vector<value_type, Ax2>& right) YGGR_NOEXCEPT_OR_NOTHROW
	{
		base_type::swap(reinterpret_cast<base_type&>(right.unwrap()));
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

	template<typename Ax2> YGGR_CXX14_CONSTEXPR_OR_INLINE
		operator Vector<value_type, Ax2>&(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		BOOST_MPL_ASSERT((is_isomeric_same_vector<base_type, Vector<value_type, Ax2> >));
		typedef Vector<value_type, Ax2> ret_type;

		return reinterpret_cast<ret_type&>(_base);
	}

	template<typename Ax2> YGGR_CXX14_CONSTEXPR_OR_INLINE
		operator const Vector<value_type, Ax2>&(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		BOOST_MPL_ASSERT((is_isomeric_same_vector<base_type, Vector<value_type, Ax2> >));
		typedef Vector<value_type, Ax2> ret_type;

		return reinterpret_cast<const ret_type&>(_base);
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

	YGGR_CXX14_CONSTEXPR_OR_INLINE this_type copy(void) const
	{
		return *this;
	}

public:
	template<typename TSize> YGGR_CXX14_CONSTEXPR_OR_INLINE
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

	template<typename TSize> YGGR_CXX14_CONSTEXPR_OR_INLINE
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

	template<typename InputIter> YGGR_CXX14_CONSTEXPR_OR_INLINE
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

	YGGR_CXX14_CONSTEXPR_OR_INLINE void assign( std::initializer_list<value_type> il )
	{
		_base.assign(boost::begin(il), boost::end(il));
	}

#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	YGGR_CXX14_CONSTEXPR_OR_INLINE allocator_type get_allocator(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _base.get_allocator();
	}

	// export get_allocator to lua
	YGGR_CXX14_CONSTEXPR_OR_INLINE wrap_allocator_type
		get_wraped_allocator(void) const YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return wrap_allocator_type(this_type::get_allocator());
	}

public:
	template<typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			reference
		>::type
		at(T n)
	{
		return _base.at(static_cast<size_type>(n));
	}

	template<typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<T>,
			reference
		>::type
		at(const T& n)
	{
		return _base.at(static_cast<size_type>(n.value()));
	}

	template<typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			const_reference
		>::type
		at(T n) const
	{
		return _base.at(static_cast<size_type>(n));
	}

	template<typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<T>,
			const_reference
		>::type
		at(const T& n) const
	{
		return _base.at(static_cast<size_type>(n.value()));
	}

	template<typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			reference
		>::type
		operator[](T n)
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
		operator[](const T& n)
	{
		return this_type::at(n);
	}

	template<typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			const_reference
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
			const_reference
		>::type
		operator[](const T& n) const
	{
		return this_type::at(n);
	}

	template<typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			typename boost::mpl::if_<boost::is_class<value_type>, const_reference, value_type >::type
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
			typename boost::mpl::if_<boost::is_class<value_type>, const_reference, value_type >::type
		>::type
		get_value(const T& n) const
	{
		return this_type::at(n);
	}

	template<typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			this_type&
		>::type
		set_value(T n, const value_type& val)
	{
		this_type::at(n) = val;
		return *this;
	}

	template<typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<T>,
			this_type&
		>::type
		set_value(const T& n, const value_type& val)
	{
		this_type::at(n) = val;
		return *this;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE reference front(void)
	{
		return _base.front();
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename boost::mpl::if_<boost::is_class<value_type>, reference, value_type >::type
		wraped_front(void)
	{
		return _base.front();
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename boost::mpl::if_<boost::is_class<value_type>, const_reference, value_type >::type
		front(void) const
	{
		return _base.front();
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE reference
		back(void)
	{
		return _base.back();
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename boost::mpl::if_<boost::is_class<value_type>, reference, value_type >::type
		wraped_back(void)
	{
		return _base.back();
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename boost::mpl::if_<boost::is_class<value_type>, const_reference, value_type >::type
		back(void) const
	{
		return _base.back();
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE this_type& set_front(const value_type& val)
	{
		_base.front() = val;
		return *this;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE this_type& set_back(const value_type& val)
	{
		_base.back() = val;
		return *this;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE value_type* data(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _base.data();
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE const value_type* data(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _base.data();
	}

public:
	// begin
	YGGR_CXX14_CONSTEXPR_OR_INLINE iterator begin(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _base.begin();
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE const_iterator begin(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _base.begin();
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE const_iterator cbegin(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::begin();
	}

	// exprot begin to lua
	YGGR_CXX14_CONSTEXPR_OR_INLINE wrap_iterator_type wraped_begin(void) YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return wrap_iterator_type(this_type::begin());
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE wrap_const_iterator_type wraped_begin(void) const YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return wrap_const_iterator_type(this_type::begin());
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE wrap_const_iterator_type wraped_cbegin(void) const YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return wrap_const_iterator_type(this_type::cbegin());
	}

	// end
	YGGR_CXX14_CONSTEXPR_OR_INLINE iterator end(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _base.end();
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE const_iterator end(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _base.end();
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE const_iterator cend(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::end();
	}

	// exprot end to lua
	YGGR_CXX14_CONSTEXPR_OR_INLINE wrap_iterator_type wraped_end(void) YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return wrap_iterator_type(this_type::end());
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE wrap_const_iterator_type wraped_end(void) const YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return wrap_const_iterator_type(this_type::end());
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE wrap_const_iterator_type wraped_cend(void) const YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return wrap_const_iterator_type(this_type::cend());
	}

	// rbegin
	YGGR_CXX14_CONSTEXPR_OR_INLINE reverse_iterator rbegin(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _base.rbegin();
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE const_reverse_iterator rbegin(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _base.rbegin();
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE const_reverse_iterator crbegin(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::rbegin();
	}

	// exprot rbegin to lua
	YGGR_CXX14_CONSTEXPR_OR_INLINE wrap_reverse_iterator_type wraped_rbegin(void) YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return wrap_reverse_iterator_type(this_type::rbegin());
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE wrap_const_reverse_iterator_type wraped_rbegin(void) const YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return wrap_const_reverse_iterator_type(this_type::rbegin());
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE wrap_const_reverse_iterator_type wraped_crbegin(void) const YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return wrap_const_reverse_iterator_type(this_type::crbegin());
	}

	// rend
	YGGR_CXX14_CONSTEXPR_OR_INLINE reverse_iterator rend(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _base.rend();
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE const_reverse_iterator rend(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _base.rend();
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE const_reverse_iterator crend(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::rend();
	}

	// exprot rend to lua
	YGGR_CXX14_CONSTEXPR_OR_INLINE wrap_reverse_iterator_type wraped_rend(void) YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return wrap_reverse_iterator_type(this_type::rend());
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE wrap_const_reverse_iterator_type wraped_rend(void) const YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return wrap_const_reverse_iterator_type(this_type::rend());
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE wrap_const_reverse_iterator_type wraped_crend(void) const YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return wrap_const_reverse_iterator_type(this_type::crend());
	}

public:
	YGGR_CXX14_CONSTEXPR_OR_INLINE bool empty(void) const YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return _base.empty();
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE size_type size(void) const YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return _base.size();
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE wrap_size_type wraped_size(void) const YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		typedef typename wrap_base_t_maker<size_type>::type wrap_base_size_type;
		return wrap_base_size_type(this_type::size());
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE size_type max_size(void) const YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return _base.max_size();
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE wrap_size_type wraped_max_size(void) const YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		typedef typename wrap_base_t_maker<size_type>::type wrap_base_size_type;
		return wrap_base_size_type(this_type::max_size());
	}

	template<typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			void
		>::type
		reserve(T n)
	{
		_base.reserve(static_cast<size_type>(n));
	}

	template<typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<T>,
			void
		>::type
		reserve(const T& n)
	{
		_base.reserve(static_cast<size_type>(n.value()));
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE size_type capacity(void) const YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return _base.capacity();
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE wrap_size_type wraped_capacity(void) const YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return wrap_size_type(this_type::capacity());
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE void shrink_to_fit(void)
	{
		yggr::container::shrink_to_fit(_base);
	}

public:
	YGGR_CXX14_CONSTEXPR_OR_INLINE void clear(void) YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		_base.clear();
	}

	// insert
	YGGR_CXX14_CONSTEXPR_OR_INLINE iterator insert(const_iterator pos, BOOST_RV_REF(value_type) val)
	{
		return _base.insert(pos, boost::move(val));
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE iterator insert(const_iterator pos, const value_type& val)
	{
		return _base.insert(pos, val);
	}

	template<typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
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

	template<typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
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

	template<typename InputIter> YGGR_CXX14_CONSTEXPR_OR_INLINE
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

	YGGR_CXX14_CONSTEXPR_OR_INLINE iterator insert(const_iterator pos, std::initializer_list<value_type> il)
	{
		return _base.insert(pos, boost::begin(il), boost::end(il));
	}

#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	// export insert to lua
	template<typename WrapPosIter> YGGR_CXX14_CONSTEXPR_OR_INLINE
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

	template<typename WrapPosIter> YGGR_CXX14_CONSTEXPR_OR_INLINE
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

	template<typename WrapPosIter, typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
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

	template<typename WrapPosIter, typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
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

	template<typename WrapPosIter, typename InputIter> YGGR_CXX14_CONSTEXPR_OR_INLINE
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

	template<typename ...Args> YGGR_CXX14_CONSTEXPR_OR_INLINE
	iterator emplace(const_iterator pos, BOOST_FWD_REF(Args) ...args)
	{
		return container::emplace(_base, pos, boost::forward<Args>(args)...);
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) YGGR_CXX14_CONSTEXPR_OR_INLINE \
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

	YGGR_CXX14_CONSTEXPR_OR_INLINE iterator erase( const_iterator pos )
	{
		return _base.erase(pos);
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE iterator erase( const_iterator s, const_iterator e )
	{
		return _base.erase(s, e);
	}

	// export erase to lua
	template<typename WrapPosIter> YGGR_CXX14_CONSTEXPR_OR_INLINE
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

	template<typename WrapPosIter1, typename WrapPosIter2> YGGR_CXX14_CONSTEXPR_OR_INLINE
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

	YGGR_CXX14_CONSTEXPR_OR_INLINE void push_back(BOOST_RV_REF(value_type) val)
	{
		_base.push_back(boost::move(val));
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE void push_back(const value_type& val)
	{
		_base.push_back(val);
	}

#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

	template<typename... Args > YGGR_CXX14_CONSTEXPR_OR_INLINE
	reference emplace_back(BOOST_FWD_REF(Args) ...args)
	{
		return container::emplace_back(_base, boost::forward<Args>(args)...);
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) YGGR_CXX14_CONSTEXPR_OR_INLINE \
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

	YGGR_CXX14_CONSTEXPR_OR_INLINE void pop_back(void)
	{
		_base.pop_back();
	}

	template<typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
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

	template<typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
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

	template<typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
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

	template<typename T> YGGR_CXX14_CONSTEXPR_OR_INLINE
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

protected:
	base_type _base;
};

// non-member foo

// ==
template<template<typename _T, typename _Ax> class Vector,
			typename T, typename Ax> YGGR_CXX14_CONSTEXPR_OR_INLINE
bool operator==(const typename wrap_vector<T, Ax, Vector>::base_type& l,
				const wrap_vector<T, Ax, Vector>& r)
{
	return l == r.unwrap();
}

template<template<typename _T, typename _Ax> class Vector,
			typename T, typename Ax> YGGR_CXX14_CONSTEXPR_OR_INLINE
bool operator==(const wrap_vector<T, Ax, Vector>& l,
				const typename wrap_vector<T, Ax, Vector>::base_type& r)
{
	return l.unwrap() == r;
}

template<template<typename _T, typename _Ax> class Vector,
			typename T, typename Ax> YGGR_CXX14_CONSTEXPR_OR_INLINE
bool operator==(const wrap_vector<T, Ax, Vector>& l,
				const wrap_vector<T, Ax, Vector>& r)
{
	return l.unwrap() == r.unwrap();
}

template<template<typename _T, typename _Ax> class Vector,
			typename T, typename Ax1, typename Ax2> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		container::is_isomeric_same_vector
		<
			Vector<T, Ax1>,
			typename wrap_vector<T, Ax2, Vector>::base_type
		>,
		bool
	>::type
	operator==(const Vector<T, Ax1>& l,
				const wrap_vector<T, Ax2, Vector>& r)
{
	typedef typename wrap_vector<T, Ax2, Vector>::base_type base_type;
	return reinterpret_cast<const base_type&>(l) == r.unwrap();
}

template<template<typename _T, typename _Ax> class Vector,
			typename T, typename Ax1, typename Ax2> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		container::is_isomeric_same_vector
		<
			typename wrap_vector<T, Ax1, Vector>::base_type,
			Vector<T, Ax2>
		>,
		bool
	>::type
	operator==(const wrap_vector<T, Ax1, Vector>& l,
				const Vector<T, Ax2>& r)
{
	typedef typename wrap_vector<T, Ax1, Vector>::base_type base_type;
	return l.unwrap() == reinterpret_cast<const base_type&>(r);
}

template<template<typename _T, typename _Ax> class Vector,
			typename T, typename Ax1, typename Ax2> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		container::is_isomeric_same_vector
		<
			typename wrap_vector<T, Ax1, Vector>::base_type,
			typename wrap_vector<T, Ax2, Vector>::base_type
		>,
		bool
	>::type
	operator==(const wrap_vector<T, Ax1, Vector>& l,
				const wrap_vector<T, Ax2, Vector>& r)
{
	typedef typename wrap_vector<T, Ax1, Vector>::base_type base_type;
	return reinterpret_cast<const base_type&>(l.unwrap()) == r.unwrap();
}

// !=
template<template<typename _T, typename _Ax> class Vector,
			typename T, typename Ax> YGGR_CXX14_CONSTEXPR_OR_INLINE
bool operator!=(const typename wrap_vector<T, Ax, Vector>::base_type& l,
				const wrap_vector<T, Ax, Vector>& r)
{
	return l != r.unwrap();
}

template<template<typename _T, typename _Ax> class Vector,
			typename T, typename Ax> YGGR_CXX14_CONSTEXPR_OR_INLINE
bool operator!=(const wrap_vector<T, Ax, Vector>& l,
				const typename wrap_vector<T, Ax, Vector>::base_type& r)
{
	return l.unwrap() != r;
}

template<template<typename _T, typename _Ax> class Vector,
			typename T, typename Ax> YGGR_CXX14_CONSTEXPR_OR_INLINE
bool operator!=(const wrap_vector<T, Ax, Vector>& l,
				const wrap_vector<T, Ax, Vector>& r)
{
	return l.unwrap() != r.unwrap();
}

template<template<typename _T, typename _Ax> class Vector,
			typename T, typename Ax1, typename Ax2> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		container::is_isomeric_same_vector
		<
			Vector<T, Ax1>,
			typename wrap_vector<T, Ax2, Vector>::base_type
		>,
		bool
	>::type
	operator!=(const Vector<T, Ax1>& l,
				const wrap_vector<T, Ax2, Vector>& r)
{
	typedef typename wrap_vector<T, Ax2, Vector>::base_type base_type;
	return reinterpret_cast<const base_type&>(l) != r.unwrap();
}

template<template<typename _T, typename _Ax> class Vector,
			typename T, typename Ax1, typename Ax2> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		container::is_isomeric_same_vector
		<
			typename wrap_vector<T, Ax1, Vector>::base_type,
			Vector<T, Ax2>
		>,
		bool
	>::type
	operator!=(const wrap_vector<T, Ax1, Vector>& l,
				const Vector<T, Ax2>& r)
{
	typedef typename wrap_vector<T, Ax1, Vector>::base_type base_type;
	return l.unwrap() != reinterpret_cast<const base_type&>(r);
}

template<template<typename _T, typename _Ax> class Vector,
			typename T, typename Ax1, typename Ax2> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		container::is_isomeric_same_vector
		<
			typename wrap_vector<T, Ax1, Vector>::base_type,
			typename wrap_vector<T, Ax2, Vector>::base_type
		>,
		bool
	>::type
	operator!=(const wrap_vector<T, Ax1, Vector>& l,
				const wrap_vector<T, Ax2, Vector>& r)
{
	typedef typename wrap_vector<T, Ax1, Vector>::base_type base_type;
	return reinterpret_cast<const base_type&>(l.unwrap()) != r.unwrap();
}

// <
template<template<typename _T, typename _Ax> class Vector,
			typename T, typename Ax> YGGR_CXX14_CONSTEXPR_OR_INLINE
bool operator<(const typename wrap_vector<T, Ax, Vector>::base_type& l,
				const wrap_vector<T, Ax, Vector>& r)
{
	return l < r.unwrap();
}

template<template<typename _T, typename _Ax> class Vector,
			typename T, typename Ax> YGGR_CXX14_CONSTEXPR_OR_INLINE
bool operator<(const wrap_vector<T, Ax, Vector>& l,
				const typename wrap_vector<T, Ax, Vector>::base_type& r)
{
	return l.unwrap() < r;
}

template<template<typename _T, typename _Ax> class Vector,
			typename T, typename Ax> YGGR_CXX14_CONSTEXPR_OR_INLINE
bool operator<(const wrap_vector<T, Ax, Vector>& l,
				const wrap_vector<T, Ax, Vector>& r)
{
	return l.unwrap() < r.unwrap();
}

template<template<typename _T, typename _Ax> class Vector,
			typename T, typename Ax1, typename Ax2> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		container::is_isomeric_same_vector
		<
			Vector<T, Ax1>,
			typename wrap_vector<T, Ax2, Vector>::base_type
		>,
		bool
	>::type
	operator<(const Vector<T, Ax1>& l,
				const wrap_vector<T, Ax2, Vector>& r)
{
	typedef typename wrap_vector<T, Ax2, Vector>::base_type base_type;
	return reinterpret_cast<const base_type&>(l) < r.unwrap();
}

template<template<typename _T, typename _Ax> class Vector,
			typename T, typename Ax1, typename Ax2> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		container::is_isomeric_same_vector
		<
			typename wrap_vector<T, Ax1, Vector>::base_type,
			Vector<T, Ax2>
		>,
		bool
	>::type
	operator<(const wrap_vector<T, Ax1, Vector>& l,
				const Vector<T, Ax2>& r)
{
	typedef typename wrap_vector<T, Ax1, Vector>::base_type base_type;
	return l.unwrap() < reinterpret_cast<const base_type&>(r);
}

template<template<typename _T, typename _Ax> class Vector,
			typename T, typename Ax1, typename Ax2> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		container::is_isomeric_same_vector
		<
			typename wrap_vector<T, Ax1, Vector>::base_type,
			typename wrap_vector<T, Ax2, Vector>::base_type
		>,
		bool
	>::type
	operator<(const wrap_vector<T, Ax1, Vector>& l,
				const wrap_vector<T, Ax2, Vector>& r)
{
	typedef typename wrap_vector<T, Ax1, Vector>::base_type base_type;
	return reinterpret_cast<const base_type&>(l.unwrap()) < r.unwrap();
}

// <=
template<template<typename _T, typename _Ax> class Vector,
			typename T, typename Ax> YGGR_CXX14_CONSTEXPR_OR_INLINE
bool operator<=(const typename wrap_vector<T, Ax, Vector>::base_type& l,
				const wrap_vector<T, Ax, Vector>& r)
{
	return l <= r.unwrap();
}

template<template<typename _T, typename _Ax> class Vector,
			typename T, typename Ax> YGGR_CXX14_CONSTEXPR_OR_INLINE
bool operator<=(const wrap_vector<T, Ax, Vector>& l,
				const typename wrap_vector<T, Ax, Vector>::base_type& r)
{
	return l.unwrap() <= r;
}

template<template<typename _T, typename _Ax> class Vector,
			typename T, typename Ax> YGGR_CXX14_CONSTEXPR_OR_INLINE
bool operator<=(const wrap_vector<T, Ax, Vector>& l,
				const wrap_vector<T, Ax, Vector>& r)
{
	return l.unwrap() <= r.unwrap();
}

template<template<typename _T, typename _Ax> class Vector,
			typename T, typename Ax1, typename Ax2> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		container::is_isomeric_same_vector
		<
			Vector<T, Ax1>,
			typename wrap_vector<T, Ax2, Vector>::base_type
		>,
		bool
	>::type
	operator<=(const Vector<T, Ax1>& l,
				const wrap_vector<T, Ax2, Vector>& r)
{
	typedef typename wrap_vector<T, Ax2, Vector>::base_type base_type;
	return reinterpret_cast<const base_type&>(l) <= r.unwrap();
}

template<template<typename _T, typename _Ax> class Vector,
			typename T, typename Ax1, typename Ax2> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		container::is_isomeric_same_vector
		<
			typename wrap_vector<T, Ax1, Vector>::base_type,
			Vector<T, Ax2>
		>,
		bool
	>::type
	operator<=(const wrap_vector<T, Ax1, Vector>& l,
				const Vector<T, Ax2>& r)
{
	typedef typename wrap_vector<T, Ax1, Vector>::base_type base_type;
	return l.unwrap() <= reinterpret_cast<const base_type&>(r);
}

template<template<typename _T, typename _Ax> class Vector,
			typename T, typename Ax1, typename Ax2> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		container::is_isomeric_same_vector
		<
			typename wrap_vector<T, Ax1, Vector>::base_type,
			typename wrap_vector<T, Ax2, Vector>::base_type
		>,
		bool
	>::type
	operator<=(const wrap_vector<T, Ax1, Vector>& l,
				const wrap_vector<T, Ax2, Vector>& r)
{
	typedef typename wrap_vector<T, Ax1, Vector>::base_type base_type;
	return reinterpret_cast<const base_type&>(l.unwrap()) <= r.unwrap();
}

// >
template<template<typename _T, typename _Ax> class Vector,
			typename T, typename Ax> YGGR_CXX14_CONSTEXPR_OR_INLINE
bool operator>(const typename wrap_vector<T, Ax, Vector>::base_type& l,
				const wrap_vector<T, Ax, Vector>& r)
{
	return l > r.unwrap();
}

template<template<typename _T, typename _Ax> class Vector,
			typename T, typename Ax> YGGR_CXX14_CONSTEXPR_OR_INLINE
bool operator>(const wrap_vector<T, Ax, Vector>& l,
				const typename wrap_vector<T, Ax, Vector>::base_type& r)
{
	return l.unwrap() > r;
}

template<template<typename _T, typename _Ax> class Vector,
			typename T, typename Ax> YGGR_CXX14_CONSTEXPR_OR_INLINE
bool operator>(const wrap_vector<T, Ax, Vector>& l,
				const wrap_vector<T, Ax, Vector>& r)
{
	return l.unwrap() > r.unwrap();
}

template<template<typename _T, typename _Ax> class Vector,
			typename T, typename Ax1, typename Ax2> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		container::is_isomeric_same_vector
		<
			Vector<T, Ax1>,
			typename wrap_vector<T, Ax2, Vector>::base_type
		>,
		bool
	>::type
	operator>(const Vector<T, Ax1>& l,
				const wrap_vector<T, Ax2, Vector>& r)
{
	typedef typename wrap_vector<T, Ax2, Vector>::base_type base_type;
	return reinterpret_cast<const base_type&>(l) > r.unwrap();
}

template<template<typename _T, typename _Ax> class Vector,
			typename T, typename Ax1, typename Ax2> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		container::is_isomeric_same_vector
		<
			typename wrap_vector<T, Ax1, Vector>::base_type,
			Vector<T, Ax2>
		>,
		bool
	>::type
	operator>(const wrap_vector<T, Ax1, Vector>& l,
				const Vector<T, Ax2>& r)
{
	typedef typename wrap_vector<T, Ax1, Vector>::base_type base_type;
	return l.unwrap() > reinterpret_cast<const base_type&>(r);
}

template<template<typename _T, typename _Ax> class Vector,
			typename T, typename Ax1, typename Ax2> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		container::is_isomeric_same_vector
		<
			typename wrap_vector<T, Ax1, Vector>::base_type,
			typename wrap_vector<T, Ax2, Vector>::base_type
		>,
		bool
	>::type
	operator>(const wrap_vector<T, Ax1, Vector>& l,
				const wrap_vector<T, Ax2, Vector>& r)
{
	typedef typename wrap_vector<T, Ax1, Vector>::base_type base_type;
	return reinterpret_cast<const base_type&>(l.unwrap()) > r.unwrap();
}

// >=
template<template<typename _T, typename _Ax> class Vector,
			typename T, typename Ax> YGGR_CXX14_CONSTEXPR_OR_INLINE
bool operator>=(const typename wrap_vector<T, Ax, Vector>::base_type& l,
				const wrap_vector<T, Ax, Vector>& r)
{
	return l >= r.unwrap();
}

template<template<typename _T, typename _Ax> class Vector,
			typename T, typename Ax> YGGR_CXX14_CONSTEXPR_OR_INLINE
bool operator>=(const wrap_vector<T, Ax, Vector>& l,
				const typename wrap_vector<T, Ax, Vector>::base_type& r)
{
	return l.unwrap() >= r;
}

template<template<typename _T, typename _Ax> class Vector,
			typename T, typename Ax> YGGR_CXX14_CONSTEXPR_OR_INLINE
bool operator>=(const wrap_vector<T, Ax, Vector>& l,
				const wrap_vector<T, Ax, Vector>& r)
{
	return l.unwrap() >= r.unwrap();
}

template<template<typename _T, typename _Ax> class Vector,
			typename T, typename Ax1, typename Ax2> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		container::is_isomeric_same_vector
		<
			Vector<T, Ax1>,
			typename wrap_vector<T, Ax2, Vector>::base_type
		>,
		bool
	>::type
	operator>=(const Vector<T, Ax1>& l,
				const wrap_vector<T, Ax2, Vector>& r)
{
	typedef typename wrap_vector<T, Ax2, Vector>::base_type base_type;
	return reinterpret_cast<const base_type&>(l) >= r.unwrap();
}

template<template<typename _T, typename _Ax> class Vector,
			typename T, typename Ax1, typename Ax2> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		container::is_isomeric_same_vector
		<
			typename wrap_vector<T, Ax1, Vector>::base_type,
			Vector<T, Ax2>
		>,
		bool
	>::type
	operator>=(const wrap_vector<T, Ax1, Vector>& l,
				const Vector<T, Ax2>& r)
{
	typedef typename wrap_vector<T, Ax1, Vector>::base_type base_type;
	return l.unwrap() >= reinterpret_cast<const base_type&>(r);
}

template<template<typename _T, typename _Ax> class Vector,
			typename T, typename Ax1, typename Ax2> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		container::is_isomeric_same_vector
		<
			typename wrap_vector<T, Ax1, Vector>::base_type,
			typename wrap_vector<T, Ax2, Vector>::base_type
		>,
		bool
	>::type
	operator>=(const wrap_vector<T, Ax1, Vector>& l,
				const wrap_vector<T, Ax2, Vector>& r)
{
	typedef typename wrap_vector<T, Ax1, Vector>::base_type base_type;
	return reinterpret_cast<const base_type&>(l.unwrap()) >= r.unwrap();
}

// output
template<template<typename _T, typename _Ax> class Vector,
			typename Char, typename Traits, typename T, typename Ax>
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os,
				const wrap_vector<T, Ax, Vector>& right)
{
	typedef T val_type;
	typedef Ax alloc_type;
	typedef wrap_vector<val_type, alloc_type, Vector> wrap_vector_type;
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

} // namespace lua_wrap
} // namespace yggr

namespace yggr
{
namespace lua_wrap
{

YGGR_PP_LUA_WRAP_CONTAINER_RANGE_SIZE_SUPPORT(2, wrap_vector)

} // namespace lua_wrap
} // namespace yggr

// swap
namespace yggr
{
namespace lua_wrap
{
namespace swap_support
{

template<template<typename _Val, typename _Ax> class Vector,
			typename Val, typename Ax > YGGR_CXX14_CONSTEXPR_OR_INLINE
void swap(typename wrap_vector<Val, Ax, Vector>::base_type& l, wrap_vector<Val, Ax, Vector>& r)
{
	r.swap(l);
}

template<template<typename _Val, typename _Ax> class Vector,
			typename Val, typename Ax > YGGR_CXX14_CONSTEXPR_OR_INLINE
void swap(wrap_vector<Val, Ax, Vector>& l, typename wrap_vector<Val, Ax, Vector>::base_type& r)
{
	l.swap(r);
}

template<template<typename _Val, typename _Ax> class Vector,
			typename Val, typename Ax > YGGR_CXX14_CONSTEXPR_OR_INLINE
void swap(wrap_vector<Val, Ax, Vector>& l, wrap_vector<Val, Ax, Vector>& r)
{
	l.swap(r);
}

} // namwspace swap_support

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

#endif // __YGGR_LUA_WRAP_WRAP_VECTOR_HPP__
