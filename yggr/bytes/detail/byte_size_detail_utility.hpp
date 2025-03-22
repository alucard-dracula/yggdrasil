//byte_size_detail_utility.hpp

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

#ifndef __YGGR_BYTES_DETAIL_BYTE_SIZE_DETAIL_UILITY_HPP__
#define __YGGR_BYTES_DETAIL_BYTE_SIZE_DETAIL_UILITY_HPP__

//#include <yggr/bytes/detail/byte_size_detail_container.hpp>
#include <yggr/bytes/detail/byte_size_detail_tpl.hpp>
#include <utility>

namespace yggr
{
namespace bytes
{
namespace detail
{

// std::pair
template<typename T1, typename T2>
struct byte_size_tpl_helper< std::pair<T1, T2> >
	: public boost::mpl::bool_<(byte_size_helper< typename boost::remove_cv<T1>::type >::value
								|| byte_size_helper< typename boost::remove_cv<T2>::type >::value)>
{
public:
	typedef std::pair<T1, T2> value_type;

	inline u64 operator()(const value_type& val) const
	{
		typedef typename value_type::first_type cv_first_type;
		typedef typename boost::remove_cv<cv_first_type>::type first_type;
		typedef typename value_type::second_type cv_second_type;
		typedef typename boost::remove_cv<cv_second_type>::type second_type;

		typedef byte_size_helper<first_type> h1_type;
		typedef byte_size_helper<second_type> h2_type;

		h1_type h1;
		h2_type h2;
		return h1(val.first) + h2(val.second);
	}

	inline u64 operator()(void) const
	{
		typedef typename value_type::first_type cv_first_type;
		typedef typename boost::remove_cv<cv_first_type>::type first_type;
		typedef typename value_type::second_type cv_second_type;
		typedef typename boost::remove_cv<cv_second_type>::type second_type;

		typedef byte_size_helper<first_type> h1_type;
		typedef byte_size_helper<second_type> h2_type;

		h1_type h1;
		h2_type h2;

		return h1() + h2();
	}
};

} // namespace detail
} // namespace bytes
} // namespace yggr

#endif // __YGGR_BYTES_DETAIL_BYTE_SIZE_DETAIL_UILITY_HPP__
