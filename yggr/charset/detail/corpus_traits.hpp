// corpus_traits.hpp

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

#ifndef __YGGR_CHARSET_DETAIL_CORPUS_TRAITS_HPP__
#define __YGGR_CHARSET_DETAIL_CORPUS_TRAITS_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/iterator_ex/is_reverse_iterator.hpp>

#include <yggr/algorithm/detail/corpus_traits.hpp>
#include <yggr/charset/utf8_string_iterator.hpp>

namespace yggr
{
namespace algorithm
{
namespace detail
{

// corpus_iterator_value

template<typename Iter>
struct corpus_utf8_string_iterator_value
{
private:
	typedef Iter iter_type;
	typedef typename iter_type::char_type char_type;
public:
	typedef typename char_type::char_view_type type;
};

template<typename Iter, bool is_riter>
struct corpus_utf8_string_iterator_value_helper;


template<typename Iter>
struct corpus_utf8_string_iterator_value_helper<Iter, false>
	: public corpus_utf8_string_iterator_value<Iter>
{
};

template<typename Iter>
struct corpus_utf8_string_iterator_value_helper<Iter, true>
	: public corpus_utf8_string_iterator_value<typename Iter::iterator_type>
{
};

//-------------

template<typename Char, typename Basic_Iterator>
struct corpus_iterator_value< charset::utf8_string_iterator<Char, Basic_Iterator> >
	: public 
		corpus_utf8_string_iterator_value_helper
		<
			charset::utf8_string_iterator<Char, Basic_Iterator>, false
		>
{
};

template<typename Char, typename Basic_Iterator>
struct corpus_iterator_value< charset::utf8_string_modify_iterator<Char, Basic_Iterator> >
	: public 
		corpus_utf8_string_iterator_value_helper
		<
			charset::utf8_string_iterator<Char, Basic_Iterator>, false
		>
{
};

template<typename Char, typename Basic_Iterator>
struct corpus_iterator_value< charset::utf8_string_const_iterator<Char, Basic_Iterator> >
	: public 
		corpus_utf8_string_iterator_value_helper
		<
			charset::utf8_string_iterator<Char, Basic_Iterator>, false
		>
{
};


template<typename Char, typename Basic_Iterator,
			template<typename _Iter> class RIter>
struct corpus_iterator_value< RIter< charset::utf8_string_iterator<Char, Basic_Iterator> > >
	: public
		corpus_utf8_string_iterator_value_helper
		<
			RIter< charset::utf8_string_iterator<Char, Basic_Iterator> >,
			is_reverse_iterator
			< 
				RIter< charset::utf8_string_iterator<Char, Basic_Iterator> > 
			>::value
		>
{
};

template<typename Char, typename Basic_Iterator,
			template<typename _Iter> class RIter>
struct corpus_iterator_value< RIter< charset::utf8_string_modify_iterator<Char, Basic_Iterator> > >
	: public
		corpus_utf8_string_iterator_value_helper
		<
			RIter< charset::utf8_string_modify_iterator<Char, Basic_Iterator> >,
			is_reverse_iterator
			< 
				RIter< charset::utf8_string_modify_iterator<Char, Basic_Iterator> > 
			>::value
		>
{
};

template<typename Char, typename Basic_Iterator,
			template<typename _Iter> class RIter>
struct corpus_iterator_value< RIter< charset::utf8_string_const_iterator<Char, Basic_Iterator> > >
	: public
		corpus_utf8_string_iterator_value_helper
		<
			RIter< charset::utf8_string_const_iterator<Char, Basic_Iterator> >,
			is_reverse_iterator
			< 
				RIter< charset::utf8_string_const_iterator<Char, Basic_Iterator> > 
			>::value
		>
{
};

} // namespace detail
} // namespace algorithm
} // namespace yggr

namespace yggr
{
namespace charset
{
namespace detail
{
} // namespace detail
} // namespace charset
} // namespace yggr

#endif // __YGGR_CHARSET_DETAIL_CORPUS_TRAITS_HPP__