//iterator_helper.hpp

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

#ifndef __YGGR_REGULAR_PARSE_DETAIL_ITERATOR_HELPER_HPP__
#define __YGGR_REGULAR_PARSE_DETAIL_ITERATOR_HELPER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/iterator_ex/is_iterator.hpp>

#include <yggr/mplex/null_t.hpp>
#include <yggr/mplex/typename_replaceor.hpp>

#include <yggr/charset/utf8_string.hpp>

#include <boost/range/value_type.hpp>
#include <boost/range/iterator.hpp>

#include <boost/type_traits/remove_cv.hpp>
#include <boost/mpl/size_t.hpp>

#include <boost/regex.hpp>

#ifdef BOOST_HAS_ICU
#	include <boost/regex/icu.hpp>
#endif // BOOST_HAS_ICU

namespace yggr
{
namespace regular_parse
{
namespace detail
{

template<typename T, 
			std::size_t iterator_id
				= (static_cast<std::size_t>(is_iterator<T>::value))
					| (static_cast<std::size_t>(charset::is_any_utf8_string_iterator_t<T>::value) << 1)
					| (static_cast<std::size_t>(charset::is_any_utf8_string_reverse_iterator_t<T>::value) << 2)
	>
struct iterator_helper
{
	typedef boost::mpl::size_t<0> id_type;

	typedef T iter_type;
	typedef iter_type org_iter_type;
	typedef mplex::null_type value_type;
	typedef mplex::null_type result_cont_type;
	typedef mplex::null_type regex_type;
};


template<typename T>
struct iterator_helper<T, static_cast<std::size_t>(1)>
{
private:
	typedef typename boost::iterator_value<T>::type cv_value_type;
	
public:
	typedef boost::mpl::size_t<1> id_type;
	typedef T iter_type;
	typedef iter_type org_iter_type;
	typedef typename boost::remove_cv<cv_value_type>::type value_type;
	typedef typename charset::basic_string<value_type> result_cont_type;
	typedef boost::basic_regex<value_type> regex_type;

	inline static org_iter_type org(iter_type i)
	{
		return i;
	}

	inline static iter_type make_iterator(org_iter_type i)
	{
		return i;
	}

	template<typename Container> inline
	static iter_type make_iterator(org_iter_type i, Container&)
	{
		return i;
	}

	inline static iter_type make_iterator(org_iter_type i, iter_type)
	{
		return i;
	}
};

template<typename T>
struct iterator_helper<T, static_cast<std::size_t>(3)>
{
private:
	typedef typename boost::iterator_value<T>::type cv_value_type;
	
public:
	typedef boost::mpl::size_t<3> id_type;
	typedef T iter_type;
	typedef typename iter_type::org_type org_iter_type;
	typedef typename boost::remove_cv<cv_value_type>::type value_type;
	typedef typename charset::utf8_string_impl<value_type> result_cont_type;

#ifdef BOOST_HAS_ICU
	typedef boost::u32regex regex_type;
#else
	typedef mplex::null_type regex_type;
#endif // BOOST_HAS_ICU

	inline static org_iter_type org(iter_type i)
	{
		return i.org();
	}

	inline static iter_type make_iterator(org_iter_type i)
	{
		return iter_type(i);
	}

	template<typename Container> inline
	static iter_type make_iterator(org_iter_type i, Container&)
	{
		return iter_type(i);
	}

	inline static iter_type make_iterator(org_iter_type i, iter_type)
	{
		return iter_type(i);
	}

	inline static iter_type make_iterator(iter_type i)
	{
		return i;
	}

	template<typename Container> inline
	static iter_type make_iterator(iter_type i, Container&)
	{
		return i;
	}

	inline static iter_type make_iterator(iter_type i, iter_type)
	{
		return i;
	}
};

template<typename Char, typename Basic_Iterator>
struct iterator_helper
	<
		charset::utf8_string_modify_iterator<Char, Basic_Iterator>, 
		static_cast<std::size_t>(3)
	>
{
private:
	typedef charset::utf8_string_modify_iterator<Char, Basic_Iterator> t_type;
	typedef typename boost::iterator_value<t_type>::type cv_value_type;
	
public:
	typedef boost::mpl::size_t<3> id_type;
	typedef t_type iter_type;
	typedef typename iter_type::org_type org_iter_type;
	typedef typename boost::remove_cv<cv_value_type>::type value_type;
	typedef typename charset::utf8_string_impl<value_type> result_cont_type;

#ifdef BOOST_HAS_ICU
	typedef boost::u32regex regex_type;
#else
	typedef mplex::null_type regex_type;
#endif // BOOST_HAS_ICU

	inline static org_iter_type org(iter_type i)
	{
		return i.org();
	}

	inline static iter_type make_iterator(org_iter_type i)
	{
		return iter_type(i);
	}

	inline static iter_type make_iterator(org_iter_type i, 
											typename iter_type::utf8_string_type& cont)
	{
		return iter_type(cont, i);
	}

	inline static iter_type make_iterator(org_iter_type i, iter_type j)
	{
		return iter_type(j.src_string(), i);
	}

	inline static iter_type make_iterator(iter_type i)
	{
		return i;
	}

	inline static iter_type make_iterator(iter_type i, 
											typename iter_type::utf8_string_type&)
	{
		return i;
	}

	inline static iter_type make_iterator(iter_type i, iter_type)
	{
		return i;
	}
};


} // namespace detail
} // namespace regular_parse
} // namespace yggr

#endif // __YGGR_REGULAR_PARSE_DETAIL_ITERATOR_HELPER_HPP__
