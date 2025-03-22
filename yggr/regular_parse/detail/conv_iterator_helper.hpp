//conv_iterator_helper.hpp

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

#ifndef __YGGR_REGULAR_PARSE_DETAIL_CONV_ITERATOR_HELPER_HPP__
#define __YGGR_REGULAR_PARSE_DETAIL_CONV_ITERATOR_HELPER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/regular_parse/detail/unicode_iterator_ex.hpp>
#include <yggr/regular_parse/detail/iterator_helper.hpp>

#include <utility>

namespace yggr
{
namespace regular_parse
{
namespace detail
{

// conv_iterator_helper
template<typename SrcIter,
			typename Dst, typename Src, 
			std::size_t dst_size = sizeof(Dst), 
			std::size_t src_size = sizeof(Src)>
struct conv_iterator_helper_detail
{
	typedef mplex::null_type type;
	typedef std::pair<type, type> result_type;
};

template<typename SrcIter, typename Dst, typename Src, std::size_t byte_size>
struct conv_iterator_helper_detail<SrcIter, Dst, Src, byte_size, byte_size>
{
	typedef SrcIter type;
	typedef std::pair<type, type> result_type;

	inline result_type operator()(type i, type j) const
	{
		typedef std::pair<type, type> ret_type;
		return ret_type(i, j);
	}
};

template<typename SrcIter, typename Dst, typename Src>
struct conv_iterator_helper_detail<SrcIter, Dst, Src, 2, 1>
{
private:
	typedef SrcIter src_iter_type;
	typedef detail::iterator_helper<src_iter_type> src_iter_h_type;
	typedef typename src_iter_h_type::value_type chk_value_type;
	BOOST_MPL_ASSERT((boost::mpl::bool_<(sizeof(chk_value_type) == sizeof(Src))>));

public:
	typedef detail::u8_to_u16_iterator<src_iter_type> type;
	typedef std::pair<type, type> result_type;

	inline result_type operator()(src_iter_type i, src_iter_type j) const
	{
		typedef std::pair<type, type> ret_type;
		return ret_type(type(i, i, j), type(j, i, j));
	}
};

template<typename SrcIter, typename Dst, typename Src>
struct conv_iterator_helper_detail<SrcIter, Dst, Src, 1, 2>
{
private:
	typedef SrcIter src_iter_type;
	typedef detail::iterator_helper<src_iter_type> src_iter_h_type;
	typedef typename src_iter_h_type::value_type chk_value_type;
	BOOST_MPL_ASSERT((boost::mpl::bool_<(sizeof(chk_value_type) == sizeof(Src))>));

public:
	typedef detail::u16_to_u8_iterator<src_iter_type> type;
	typedef std::pair<type, type> result_type;

	inline result_type operator()(src_iter_type i, src_iter_type j) const
	{
		typedef std::pair<type, type> ret_type;
		return ret_type(type(i, i, j), type(j, i, j));
	}
};

template<typename SrcIter, typename Dst, typename Src>
struct conv_iterator_helper_detail<SrcIter, Dst, Src, 4, 1>
{
private:
	typedef SrcIter src_iter_type;
	typedef detail::iterator_helper<src_iter_type> src_iter_h_type;
	typedef typename src_iter_h_type::value_type chk_value_type;
	BOOST_MPL_ASSERT((boost::mpl::bool_<(sizeof(chk_value_type) == sizeof(Src))>));

public:
	typedef boost::u8_to_u32_iterator<src_iter_type> type;
	typedef std::pair<type, type> result_type;

	inline result_type operator()(src_iter_type i, src_iter_type j) const
	{
		typedef std::pair<type, type> ret_type;
		return ret_type(type(i, i, j), type(j, i, j));
	}
};

template<typename SrcIter, typename Dst, typename Src>
struct conv_iterator_helper_detail<SrcIter, Dst, Src, 1, 4>
{
private:
	typedef SrcIter src_iter_type;
	typedef detail::iterator_helper<src_iter_type> src_iter_h_type;
	typedef typename src_iter_h_type::value_type chk_value_type;
	BOOST_MPL_ASSERT((boost::mpl::bool_<(sizeof(chk_value_type) == sizeof(Src))>));

public:
	typedef boost::u32_to_u8_iterator<src_iter_type> type;
	typedef std::pair<type, type> result_type;

	inline result_type operator()(src_iter_type i, src_iter_type j) const
	{
		typedef std::pair<type, type> ret_type;
		return ret_type(type(i), type(j));
	}
};

template<typename SrcIter, typename Dst, typename Src>
struct conv_iterator_helper_detail<SrcIter, Dst, Src, 4, 2>
{
private:
	typedef SrcIter src_iter_type;
	typedef detail::iterator_helper<src_iter_type> src_iter_h_type;
	typedef typename src_iter_h_type::value_type chk_value_type;
	BOOST_MPL_ASSERT((boost::mpl::bool_<(sizeof(chk_value_type) == sizeof(Src))>));

public:
	typedef boost::u16_to_u32_iterator<src_iter_type> type;
	typedef std::pair<type, type> result_type;

	inline result_type operator()(src_iter_type i, src_iter_type j) const
	{
		typedef std::pair<type, type> ret_type;
		return ret_type(type(i, i, j), type(j, i, j));
	}
};

template<typename SrcIter, typename Dst, typename Src>
struct conv_iterator_helper_detail<SrcIter, Dst, Src, 2, 4>
{
private:
	typedef SrcIter src_iter_type;
	typedef detail::iterator_helper<src_iter_type> src_iter_h_type;
	typedef typename src_iter_h_type::value_type chk_value_type;
	BOOST_MPL_ASSERT((boost::mpl::bool_<(sizeof(chk_value_type) == sizeof(Src))>));

public:
	typedef boost::u32_to_u16_iterator<src_iter_type> type;
	typedef std::pair<type, type> result_type;

	inline result_type operator()(src_iter_type i, src_iter_type j) const
	{
		typedef std::pair<type, type> ret_type;
		return ret_type(type(i), type(j));
	}
};

template<typename SrcIter, typename Dst, typename Src>
struct conv_iterator_helper
	: public conv_iterator_helper_detail<SrcIter, Dst, Src>
{
};

} // namespace detail

using detail::conv_iterator_helper;

} // namespace regular_parse
} // namespace yggr

#endif // __YGGR_REGULAR_PARSE_DETAIL_CONV_ITERATOR_HELPER_HPP__
