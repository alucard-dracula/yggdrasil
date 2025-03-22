//args.hpp

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

#ifndef __YGGR_MPLEX_ARGS_HPP__
#define __YGGR_MPLEX_ARGS_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/null_t.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/at.hpp>

namespace yggr
{
namespace mplex
{
namespace detail
{

template<typename Failed, typename T, std::size_t N,
			bool is_validate = (N < boost::mpl::size<T>::value) >
struct wrap_at_c
	: public boost::mpl::at_c<T, N>
{
};

template<typename Failed, typename T, std::size_t N>
struct wrap_at_c<Failed, T, N, false>
{
	typedef Failed type;
};

template<typename Params, size_type N, typename Failed = null_type>
struct get_arg_detail
	: public
		boost::mpl::if_c
		<
			!!(boost::mpl::size<Params>::value),
			typename wrap_at_c<Failed, Params, N>::type,
			Failed
		>
{
};

template<size_type N, typename Failed>
struct get_arg_detail<null_type, N, Failed>
{
	typedef Failed type;
};

} // namespace detail

template<typename Params, size_type N, typename Failed = null_type>
struct get_arg
	: public detail::get_arg_detail<Params, N, Failed>
{
};

} // namespace mplex
} // namespace yggr

#endif // __YGGR_MPLEX_ARGS_HPP__
