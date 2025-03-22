//copy_or_move_or_swap.hpp

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

#ifndef __YGGR_UTILITY_COPY_OR_MOVE_OR_SWAP_HPP__
#define __YGGR_UTILITY_COPY_OR_MOVE_OR_SWAP_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/utility/swap.hpp>

#include <boost/type_traits/is_pod.hpp>
#include <boost/mpl/size_t.hpp>


namespace yggr
{
namespace utility
{
namespace detail
{

template<typename L, typename R>
struct has_swap_detail
{
private:
	typedef has_swap_detail this_type;
	typedef L l_type;
	typedef R r_type;

	typedef u8 non_type;
	typedef u32 has_type;

	template<typename T, void (T::*)(r_type&)> struct check_lr_t;
	template<typename T, void (T::*)(l_type&)> struct check_rl_t;

	template<typename T>
	static has_type chk_foo_lr(const check_lr_t<T, &T::swap>* );

	template<typename T>
	static non_type chk_foo_lr(...);

	template<typename T>
	static has_type chk_foo_rl(const check_rl_t<T, &T::swap>* );

	template<typename T>
	static non_type chk_foo_rl(...);

public:
	typedef boost::mpl::size_t<static_cast<std::size_t>(sizeof(has_type) == sizeof(chk_foo_lr<l_type>(0)))> lr_type;
	typedef boost::mpl::size_t<static_cast<std::size_t>(sizeof(has_type) == sizeof(chk_foo_rl<l_type>(0)))> rl_type;

	typedef typename
		boost::mpl::size_t
		<
			static_cast<std::size_t>(lr_type::value? lr_type::value << 1 : rl_type::value)
		> type;
};

template<typename L, typename R>
struct has_swap
	: public has_swap_detail<L, R>::type
{
};

template<typename L, typename R,
			std::size_t swap_t = has_swap<L, R>::value >
struct copy_or_move_or_swap_helper;

template<typename L, typename R>
struct copy_or_move_or_swap_helper<L, R, 2>
{
	typedef L l_type;
	typedef R r_type;

	inline l_type& operator()(l_type& l, r_type& r) const
	{
		l.swap(r);
		return l;
	}
};

template<typename L, typename R>
struct copy_or_move_or_swap_helper<L, R, 1>
{
	typedef L l_type;
	typedef R r_type;

	inline l_type& operator()(l_type& l, r_type& r) const
	{
		r.swap(l);
		return l;
	}
};

template<typename L, typename R>
struct copy_or_move_or_swap_helper<L, R, 0>
{
	typedef L l_type;
	typedef R r_type;

	inline l_type& operator()(l_type& l, r_type& r) const
	{
		return (
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		l = boost::move(r)
#else
		l = r
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		);
	}
};

} // namespace utility
} // namespace detail

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
template<typename L, typename R> inline
L& copy_or_move_or_swap(L& l, BOOST_RV_REF(R) r)
{
	typedef utility::detail::copy_or_move_or_swap_helper<L, R> h_type;
	h_type h;
	R& ref = r;
	return h(l, ref);
}

#else

template<typename L, typename R> inline
L& copy_or_move_or_swap(L& l, R& r)
{
	typedef utility::detail::copy_or_move_or_swap_helper<L, R> h_type;
	h_type h;
	return h(l, r);
}

#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

template<typename L, typename R> inline
L& copy_or_move_or_swap(L& l, const R& r)
{
	return l = r;
}

} // namespace yggr

#endif // __YGGR_UTILITY_COPY_OR_MOVE_OR_SWAP_HPP__
