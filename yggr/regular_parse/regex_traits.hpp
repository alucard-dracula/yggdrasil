//regex_traits.hpp

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

#ifndef __YGGR_REGULAR_PARSE_REGEX_TRAITS_HPP__
#define __YGGR_REGULAR_PARSE_REGEX_TRAITS_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/null_t.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/regex.hpp>

namespace yggr
{
namespace regular_parse
{

template<typename T>
struct regex_traits
	: public boost::mpl::false_
{
	typedef mplex::null_type regex_type;

	typedef mplex::null_type value_type;
	typedef mplex::null_type traits_type;
	typedef mplex::null_type const_iterator;
	typedef mplex::null_type iterator;
	typedef mplex::null_type reference;
	typedef mplex::null_type const_reference;
	typedef mplex::null_type difference_type;
	typedef mplex::null_type size_type;
	typedef mplex::null_type flag_type;
	typedef mplex::null_type locale_type;
};

template<typename Char, typename Traits>
struct regex_traits< boost::basic_regex<Char, Traits> >
	: public boost::mpl::true_
{
	typedef boost::basic_regex<Char, Traits>		regex_type;

	typedef typename regex_type::value_type			value_type;
	typedef Traits									traits_type;
	typedef typename regex_type::const_iterator		const_iterator;
	typedef typename regex_type::iterator			iterator;
	typedef typename regex_type::reference			reference;
	typedef typename regex_type::const_reference	const_reference;
	typedef typename regex_type::difference_type	difference_type;
	typedef typename regex_type::size_type			size_type;
	typedef typename regex_type::flag_type			flag_type;
	typedef typename regex_type::locale_type		locale_type;
};

} // namespace regular_parse
} // namespace yggr

#endif // __YGGR_REGULAR_PARSE_REGEX_TRAITS_HPP__
