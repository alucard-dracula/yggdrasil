//wrap_foo_of_iterators.hpp

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

#ifndef __YGGR_LUA_WRAP_WRAP_FOO_OF_ITERATORS_HPP__
#define __YGGR_LUA_WRAP_WRAP_FOO_OF_ITERATORS_HPP__

#include <yggr/iterator_ex/is_const_iterator.hpp>
#include <yggr/iterator_ex/iterator_traits.hpp>
#include <yggr/iterator_ex/iterator.hpp>

#include <yggr/lua_wrap/wrap_base_t.hpp>
#include <yggr/lua_wrap/detail/unwrap_t.hpp>

#include <boost/type_traits/is_class.hpp>
#include <boost/mpl/if.hpp>

namespace yggr
{
namespace lua_wrap
{

namespace detail
{

// Compatible android clang std::advance

/*

template < class _InputIter,
           class _Distance,
           class _IntegralDistance = decltype(std::__convert_to_integral(std::declval<_Distance>())),
           __enable_if_t<is_integral<_IntegralDistance>::value, int> = 0>
_LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX17 void advance(_InputIter& __i, _Distance __orig_n) {
  typedef typename iterator_traits<_InputIter>::difference_type _Difference;
  _Difference __n = static_cast<_Difference>(std::__convert_to_integral(__orig_n));
  // Calling `advance` with a negative value on a non-bidirectional iterator is a no-op in the current implementation.
  _LIBCPP_ASSERT_PEDANTIC(__n >= 0 || __has_bidirectional_iterator_category<_InputIter>::value,
                          "Attempt to advance(it, n) with negative n on a non-bidirectional iterator");
  std::__advance(__i, __n, typename iterator_traits<_InputIter>::iterator_category());
}

*/

template<typename T>
struct advance_distance_unpacker
{
	typedef typename
		boost::mpl::if_
		<
			boost::is_class<T>,
			const T&,
			T
		>::type arg_type;
	typedef arg_type ret_type;

	YGGR_CONSTEXPR_OR_INLINE ret_type operator()(arg_type in) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return in;
	}
};

template<typename Val>
struct advance_distance_unpacker< wrap_base_t<Val> >
{
	typedef const wrap_base_t<Val>& arg_type;
	typedef Val ret_type;

	YGGR_CONSTEXPR_OR_INLINE ret_type operator()(arg_type in) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return in.unwrap();
	}
};


} // namespace detail

template<typename InputIt, typename Dis> YGGR_CXX17_CONSTEXPR_OR_INLINE
void wrap_foo_advance(InputIt& iter,
							typename 
								boost::mpl::if_
								<
									boost::is_class<Dis>,
									const Dis&,
									Dis
								>::type n)
{
	typedef Dis dis_type;
	typedef detail::advance_distance_unpacker<dis_type> unpacker_type;

	unpacker_type unpacker;

	iterator_ex::advance(iter, unpacker(n));
}

template<typename InputIt, typename Dis> YGGR_CXX17_CONSTEXPR_OR_INLINE
InputIt wrap_foo_advance_copy(InputIt iter,
								typename 
									boost::mpl::if_
									<
										boost::is_class<Dis>,
										const Dis&,
										Dis
									>::type n)
{
	typedef Dis dis_type;
	typedef detail::advance_distance_unpacker<dis_type> unpacker_type;

	unpacker_type unpacker;

	return iterator_ex::advance_copy(iter, unpacker(n));
}

template<typename InputIt1, typename InputIt2> YGGR_CXX17_CONSTEXPR_OR_INLINE
typename iterator_ex::iterator_difference_ex<InputIt1>::type 
	wrap_foo_distance(InputIt1 s, InputIt2 e)
{
	typedef typename
		boost::mpl::if_
		<
			is_const_iterator<InputIt1>,
			InputIt1,
			InputIt2
		>::type dis_iter_type;

	return iterator_ex::distance<dis_iter_type>(s, e);
}

template<typename InputIt1, typename InputIt2> YGGR_CXX17_CONSTEXPR_OR_INLINE
typename 
	wrap_base_t_maker
	<
		typename iterator_ex::iterator_difference_ex<InputIt1>::type 
	>::type
	wrap_foo_wraped_distance(InputIt1 s, InputIt2 e)
{
	typedef typename
		boost::mpl::if_
		<
			is_const_iterator<InputIt1>,
			InputIt1,
			InputIt2
		>::type dis_iter_type;
	
	typedef typename 
		wrap_base_t_maker
		<
			typename iterator_ex::iterator_difference_ex<InputIt1>::type 
		>::type ret_type;

	return ret_type(iterator_ex::distance<dis_iter_type>(s, e));
}

// next

template<typename Iter> YGGR_CXX17_CONSTEXPR_OR_INLINE
Iter wrap_foo_next(Iter it)
{
	return iterator_ex::next(it);
}

template<typename Iter, typename Dis> YGGR_CXX17_CONSTEXPR_OR_INLINE
Iter wrap_foo_next(Iter it,
					typename 
						boost::mpl::if_
						<
							boost::is_class<Dis>,
							const Dis&,
							Dis
						>::type n)
{
	typedef Dis dis_type;
	typedef detail::advance_distance_unpacker<dis_type> unpacker_type;

	unpacker_type unpacker;
	return iterator_ex::next(it, unpacker(n));
}

// prev

template<typename Iter> YGGR_CXX17_CONSTEXPR_OR_INLINE
Iter wrap_foo_prev(Iter it)
{
	return iterator_ex::prev(it);
}

template<typename Iter, typename Dis> YGGR_CXX17_CONSTEXPR_OR_INLINE
Iter wrap_foo_prev(Iter it,
					typename 
						boost::mpl::if_
						<
							boost::is_class<Dis>,
							const Dis&,
							Dis
						>::type n)
{
	typedef Dis dis_type;
	typedef detail::advance_distance_unpacker<dis_type> unpacker_type;

	unpacker_type unpacker;
	return iterator_ex::prev(it, unpacker(n));
}

} // namespace lua_wrap
} // namespace yggr

#endif // __YGGR_LUA_WRAP_WRAP_FOO_OF_ITERATORS_HPP__
