//regex_iterator_t_maker.hpp

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

#ifndef __YGGR_REGULAR_PARSE_REGEX_ITERATOR_T_MAKER_HPP__
#define __YGGR_REGULAR_PARSE_REGEX_ITERATOR_T_MAKER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/regular_parse/detail/conv_iterator_helper.hpp>
#include <yggr/regular_parse/regex_traits.hpp>

#ifdef BOOST_HAS_ICU
#	include <boost/regex/icu.hpp>
#endif // BOOST_HAS_ICU

namespace yggr
{
namespace regular_parse
{

template<typename InputIter, typename Regex>
struct regex_iterator_t_maker
{
public:
	typedef InputIter input_iterator_type;
	typedef typename detail::iterator_helper<input_iterator_type>::org_iter_type base_iterator_type;
	typedef typename detail::iterator_helper<input_iterator_type>::value_type value_type;
	
	typedef Regex regex_type;
	typedef typename regex_traits<regex_type>::value_type re_value_type;
	typedef typename regex_traits<regex_type>::traits_type re_traits_type;

	typedef typename 
		detail::conv_iterator_helper
		<
			base_iterator_type, 
			re_value_type, 
			value_type
		>::type iterator_type;
	
	typedef boost::regex_iterator<iterator_type, re_value_type, re_traits_type> regex_iterator_type;
	typedef regex_iterator_type type;
};

#ifdef BOOST_HAS_ICU

template<typename InputIter>
struct regex_iterator_t_maker<InputIter, boost::u32regex>
{
public:
	typedef InputIter input_iterator_type;
	typedef typename detail::iterator_helper<input_iterator_type>::org_iter_type base_iterator_type;
	typedef typename detail::iterator_helper<input_iterator_type>::value_type value_type;
	
	typedef boost::u32regex regex_type;
	typedef regex_traits<regex_type>::value_type re_value_type;
	typedef regex_traits<regex_type>::traits_type re_traits_type;

	typedef base_iterator_type iterator_type;
	typedef boost::u32regex_iterator<iterator_type> regex_iterator_type;
	typedef regex_iterator_type type;
};

#endif // BOOST_HAS_ICU

template<typename InputIter, typename Regex>
struct regex_token_iterator_t_maker
{
public:
	typedef InputIter input_iterator_type;
	typedef typename detail::iterator_helper<input_iterator_type>::org_iter_type base_iterator_type;
	typedef typename detail::iterator_helper<input_iterator_type>::value_type value_type;
	
	typedef Regex regex_type;
	typedef typename regex_traits<regex_type>::value_type re_value_type;
	typedef typename regex_traits<regex_type>::traits_type re_traits_type;

	typedef typename 
		detail::conv_iterator_helper
		<
			base_iterator_type, 
			re_value_type, 
			value_type
		>::type iterator_type;
	
	typedef boost::regex_token_iterator<iterator_type, re_value_type, re_traits_type> regex_token_iterator_type;
	typedef regex_token_iterator_type type;
};

#ifdef BOOST_HAS_ICU

template<typename InputIter>
struct regex_token_iterator_t_maker<InputIter, boost::u32regex>
{
public:
	typedef InputIter input_iterator_type;
	typedef typename detail::iterator_helper<input_iterator_type>::org_iter_type base_iterator_type;
	typedef typename detail::iterator_helper<input_iterator_type>::value_type value_type;
	
	typedef boost::u32regex regex_type;
	typedef regex_traits<regex_type>::value_type re_value_type;
	typedef regex_traits<regex_type>::traits_type re_traits_type;

	typedef base_iterator_type iterator_type;
	typedef boost::u32regex_token_iterator<iterator_type> regex_token_iterator_type;
	typedef regex_token_iterator_type type;
};

#endif // BOOST_HAS_ICU

} // namespace regular_parse
} // namespace yggr

#endif // __YGGR_REGULAR_PARSE_REGEX_ITERATOR_T_MAKER_HPP__
