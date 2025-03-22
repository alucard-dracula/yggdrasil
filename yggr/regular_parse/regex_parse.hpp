//regex_parse.hpp

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

#ifndef __YGGR_REGULAR_PARSE_REGEX_PARSE_HPP__
#define __YGGR_REGULAR_PARSE_REGEX_PARSE_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/compatibility/stl_exception.hpp>
#include <yggr/bind/bind.hpp>

#include <yggr/type_traits/is_convertible_size_t.hpp>
#include <yggr/mplex/typename_replaceor.hpp>

#include <yggr/func/is_callable.hpp>
#include <yggr/iterator_ex/is_iterator.hpp>
#include <yggr/iterator_ex/iterator_category_check.hpp>

#include <yggr/container_ex/allocator_t.hpp>
#include <yggr/container/get_allocator.hpp>
#include <yggr/container_ex/is_container.hpp>
#include <yggr/container_ex/is_allocator.hpp>

#include <yggr/charset/const_strings.hpp>
#include <yggr/charset/string.hpp>

#include <yggr/regular_parse/re_config.hpp>

#include <yggr/regular_parse/detail/string_helper.hpp>
#include <yggr/regular_parse/detail/iterator_helper.hpp>

#include <yggr/regular_parse/iterator_adp.hpp>
#include <yggr/regular_parse/regex_iterator_t_maker.hpp>
#include <yggr/regular_parse/match_results_t_maker.hpp>

#include <boost/utility/enable_if.hpp>

#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/if.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_class.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/remove_cv.hpp>

#if defined(YGGR_NO_CWCHAR) || defined(YGGR_NO_WSTRING)
#	if !defined(BOOST_NO_WREGEX)
#		define BOOST_NO_WREGEX
#	endif // BOOST_NO_WREGEX
#endif // defined(YGGR_NO_CWCHAR) || defined(YGGR_NO_WSTRING)

#include <boost/regex.hpp>

#include <cassert>
#include <iterator>
#include <algorithm>

/*
*	please all function use try catch of call
*	example:
*	try
*	{
*		regex_parse::foo()
*	}
*	catch(regex_parse::error_type)
*	{
*	}
*	catch(...) or catch(yggr::stl_exceptrion)
*	{
*	}
*/

// !!!don't append utf8_string, because it need icu support
namespace yggr
{
namespace regular_parse
{

class regex_parse
{
public:
	typedef boost::regbase regex_base_type;
	typedef regex_base_type::flag_type regex_flag_type;
	typedef boost::regex_constants::match_flag_type match_flag_type;
	typedef boost::regex_error error_type;

private:
	typedef regex_parse this_type;

public:
	//-----------------------------make_regex----------------------------------------

	/*!!! don't spilt boost::basic_regex to string_type, because char* ang iter is conflict !!!*/

	// regex_type make_regex(text_s, text_e, re_flag);
	template<typename Iter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<Iter>,
				charset::is_not_any_utf8_string_iterator_t<Iter>
			>,
			typename detail::iterator_helper<Iter>::regex_type
		>::type
		make_regex(Iter s, Iter e, regex_flag_type re_flag = regex_base_type::normal)
	{
		typedef detail::iterator_helper<Iter> iter_h_type;
		typedef typename iter_h_type::regex_type ret_type;
		return ret_type(s, e, re_flag);
	}

	// regex_type make_regex(text, re_flag);
	template<typename StrRegex> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::is_generalized_string_t<StrRegex>,
				charset::is_not_utf8_string_t<StrRegex>
			>,
			boost::basic_regex<typename detail::string_helper<StrRegex>::value_type>
		>::type
		make_regex(const StrRegex& str, regex_flag_type re_flag = regex_base_type::normal)
	{
		typedef detail::string_helper<StrRegex> regex_h_type;
		typedef typename regex_h_type::value_type char_type;
		typedef boost::basic_regex<char_type> ret_type;

		return ret_type(regex_h_type::begin(str), regex_h_type::end(str), re_flag);
	}

	//-----------------------------match-----------------------------------------------

	// bool match(text_s, text_e, re, mflag);
	template<typename BiIter, typename Traits> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_bidirectional_iterator<BiIter>,
				charset::is_not_any_utf8_string_iterator_t<BiIter>
			>,
			bool
		>::type
		match(BiIter text_s, BiIter text_e,
				const boost::basic_regex<typename detail::iterator_helper<BiIter>::value_type, Traits>& re,
				match_flag_type mth_flag = boost::match_default)
	{
		return boost::regex_match(text_s, text_e, re, mth_flag);
	}

	// bool match(text_s, text_e, result, re, mflag);
	template<typename BiIter, typename Alloc, typename Traits> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_bidirectional_iterator<BiIter>,
				charset::is_not_any_utf8_string_iterator_t<BiIter>
			>,
			bool
		>::type
		match(BiIter text_s, BiIter text_e,
				boost::match_results<BiIter, Alloc>& m,
				const boost::basic_regex<typename detail::iterator_helper<BiIter>::value_type, Traits>& re,
				match_flag_type mth_flag = boost::match_default)
	{
		return boost::regex_match(text_s, text_e, m, re, mth_flag);
	}

	// bool match(text, re, mflag);
	template<typename Target, typename Traits> inline
	static
	typename
		boost::enable_if
		<
			charset::is_not_utf8_string_t<Target>,
			bool
		>::type
		match(const Target& text,
				const boost::basic_regex<typename detail::string_helper<Target>::value_type, Traits>& re,
				match_flag_type mth_flag = boost::match_default)
	{
		typedef Target target_type;
		typedef detail::string_helper<target_type> target_h_type;

		return
			this_type::match(
				target_h_type::begin(text), target_h_type::end(text), re, mth_flag);
	}

	// bool match(text, result, re, mflag);
	template<typename Target, typename Alloc, typename Traits> inline
	static
	typename
		boost::enable_if
		<
			charset::is_not_utf8_string_t<Target>,
			bool
		>::type
		match(const Target& text,
				boost::match_results<typename detail::string_helper<Target>::citer_type, Alloc>& m,
				const boost::basic_regex<typename detail::string_helper<Target>::value_type, Traits>& re,
				match_flag_type mth_flag = boost::match_default)
	{
		typedef Target target_type;
		typedef detail::string_helper<target_type> target_h_type;

		return
			this_type::match(
				target_h_type::begin(text), target_h_type::end(text), m, re, mth_flag);
	}

	//---------------------------------search--------------------------------------

	// bool search(text_s, text_e, re, mflag);
	template<typename BiIter, typename Traits> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_bidirectional_iterator<BiIter>,
				charset::is_not_any_utf8_string_iterator_t<BiIter>
			>,
			bool
		>::type
		search(BiIter text_s, BiIter text_e,
				const boost::basic_regex<typename detail::iterator_helper<BiIter>::value_type, Traits>& re,
				match_flag_type mth_flag = boost::match_default)
	{
		return boost::regex_search(text_s, text_e, re, mth_flag);
	}

	// bool search(text_s, text_e, result, re, mflag);
	template<typename BiIter, typename Alloc, typename Traits> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_bidirectional_iterator<BiIter>,
				charset::is_not_any_utf8_string_iterator_t<BiIter>
			>,
			bool
		>::type
		search(BiIter text_s, BiIter text_e,
				boost::match_results<BiIter, Alloc>& m,
				const boost::basic_regex<typename detail::iterator_helper<BiIter>::value_type, Traits>& re,
				match_flag_type mth_flag = boost::match_default)
	{
		return boost::regex_search(text_s, text_e, m, re, mth_flag);
	}

	// bool search(text, re, mflag);
	template<typename Target, typename Traits> inline
	static
	typename
		boost::enable_if
		<
			charset::is_not_utf8_string_t<Target>,
			bool
		>::type
		search(const Target& text,
				const boost::basic_regex<typename detail::string_helper<Target>::value_type, Traits>& re,
				match_flag_type mth_flag = boost::match_default)
	{
		typedef Target target_type;
		typedef detail::string_helper<target_type> target_h_type;

		return
			this_type::search(
				target_h_type::begin(text), target_h_type::end(text), re, mth_flag);
	}

	// bool search(text, result, re, mflag);
	template<typename Target, typename Alloc, typename Traits> inline
	static
	typename
		boost::enable_if
		<
			charset::is_not_utf8_string_t<Target>,
			bool
		>::type
		search(const Target& text,
				boost::match_results<typename detail::string_helper<Target>::citer_type, Alloc>& m,
				const boost::basic_regex<typename detail::string_helper<Target>::value_type, Traits>& re,
				match_flag_type mth_flag = boost::match_default)
	{
		typedef Target target_type;
		typedef detail::string_helper<target_type> target_h_type;

		return
			this_type::search(
				target_h_type::begin(text), target_h_type::end(text), m, re, mth_flag);
	}

	//---------------------------------replace--------------------------------------

	/*
	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	!!! the "Formatter" using "const Formatter&",										!!!
	!!!	because org function using Formatter fmt and the inner using const Formatter	!!!
	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	*/

	// out_iter replace(out_iter, text_s, text_e, re, mth_flag);
	template<typename OutIter, typename BiIter,
				typename Traits, typename Formatter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::and_
				<
					is_output_iterator<OutIter>,
					charset::is_not_any_utf8_string_iterator_t<OutIter>
				>,
				boost::mpl::and_
				<
					is_bidirectional_iterator<BiIter>,
					charset::is_not_any_utf8_string_iterator_t<BiIter>
				>,
				boost::mpl::and_
				<
					charset::is_generalized_string_t<Formatter>,
					charset::is_not_utf8_string_t<Formatter>
				>
			>,
			OutIter
		>::type
		replace(OutIter out_iter,
				BiIter s, BiIter e,
				const boost::basic_regex<typename detail::iterator_helper<BiIter>::value_type, Traits>& re,
				const Formatter& str_fmt,
				match_flag_type mth_flag = boost::match_default | boost::format_all)
	{
		return boost::regex_replace(out_iter, s, e, re, str_fmt, mth_flag);
	}

	// basic_string replace(text_s, text_e, re, str_fmt, mth_flag)
	template<typename BiIter, typename Traits, typename Formatter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::and_
				<
					is_bidirectional_iterator<BiIter>,
					charset::is_not_any_utf8_string_iterator_t<BiIter>
				>,
				boost::mpl::and_
				<
					charset::is_generalized_string_t<Formatter>,
					charset::is_not_utf8_string_t<Formatter>
				>
			>,
			typename detail::iterator_helper<BiIter>::result_cont_type
		>::type
		replace(BiIter s, BiIter e,
					const boost::basic_regex<typename detail::iterator_helper<BiIter>::value_type, Traits>& re,
					const Formatter& str_fmt,
					match_flag_type mth_flag = boost::match_default | boost::format_all)
	{
		typedef BiIter target_iter_type;
		typedef detail::iterator_helper<target_iter_type> target_iter_h_type;
		typedef typename target_iter_h_type::result_cont_type ret_type;

		ret_type result;
		boost::YGGR_BOOST_REGEX_DETAIL_NS::string_out_iterator< ret_type > i(result);
		boost::regex_replace(i, s, e, re, str_fmt, mth_flag);
		return result;
	}

	// basic_string replace(basic_string_alloc, text_s, text_e, re, str_fmt, mth_flag);
	template<typename Alloc, typename BiIter, typename Traits, typename Formatter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				container_ex::is_allocator<Alloc>,
				boost::mpl::and_
				<
					is_bidirectional_iterator<BiIter>,
					charset::is_not_any_utf8_string_iterator_t<BiIter>
				>,
				boost::mpl::and_
				<
					charset::is_generalized_string_t<Formatter>,
					charset::is_not_utf8_string_t<Formatter>
				>
			>,
			typename
				boost::mpl::if_
				<
					boost::is_same
					<
						typename
							container_ex::allocator_t
							<
								typename detail::iterator_helper<BiIter>::result_cont_type
							>::type,
						Alloc
					>,
					typename detail::iterator_helper<BiIter>::result_cont_type,
					typename
						mplex::typename_replaceor
						<
							typename detail::iterator_helper<BiIter>::result_cont_type,
							mplex::_keep, mplex::_keep, Alloc
						>::type
				>::type
		>::type
		replace(const Alloc& alloc,
					BiIter s, BiIter e,
					const boost::basic_regex<typename detail::iterator_helper<BiIter>::value_type, Traits>& re,
					const Formatter& str_fmt,
					match_flag_type mth_flag = boost::match_default | boost::format_all)
	{
		typedef BiIter target_iter_type;
		typedef detail::iterator_helper<target_iter_type> target_iter_h_type;
		typedef typename target_iter_h_type::result_cont_type result_cont_type;

		typedef typename
			boost::mpl::if_
			<
				boost::is_same
				<
					typename
						container_ex::allocator_t
						<
							typename detail::iterator_helper<BiIter>::result_cont_type
						>::type,
					Alloc
				>,
				typename detail::iterator_helper<BiIter>::result_cont_type,
				typename
					mplex::typename_replaceor
					<
						typename detail::iterator_helper<BiIter>::result_cont_type,
						mplex::_keep, mplex::_keep, Alloc
					>::type
			>::type ret_type;

		ret_type result(alloc);
		boost::YGGR_BOOST_REGEX_DETAIL_NS::string_out_iterator< ret_type > i(result);
		boost::regex_replace(i, s, e, re, str_fmt, mth_flag);
		return result;
	}

	// String replace(text_s, text_e, re, str_fmt, mth_flag);
	template<typename String, typename BiIter, typename Traits, typename Formatter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::and_
				<
					container_ex::is_container<String>,
					charset::is_not_utf8_string_t<String>
				>,
				boost::mpl::and_
				<
					is_bidirectional_iterator<BiIter>,
					charset::is_not_any_utf8_string_iterator_t<BiIter>
				>,
				boost::mpl::and_
				<
					charset::is_generalized_string_t<Formatter>,
					charset::is_not_utf8_string_t<Formatter>
				>
			>,
			String
		>::type
		replace(BiIter s, BiIter e,
					const boost::basic_regex<typename detail::iterator_helper<BiIter>::value_type, Traits>& re,
					const Formatter& str_fmt,
					match_flag_type mth_flag = boost::match_default | boost::format_all)
	{
		typedef String ret_type;
		typedef BiIter target_iter_type;
		typedef detail::iterator_helper<target_iter_type> target_iter_h_type;
		typedef typename target_iter_h_type::value_type char_type;

		typedef typename boost::range_value<ret_type>::type chk_char_type;

		BOOST_MPL_ASSERT((boost::mpl::bool_<(sizeof(char_type) == sizeof(chk_char_type))>));

		ret_type result;
		boost::YGGR_BOOST_REGEX_DETAIL_NS::string_out_iterator< ret_type > i(result);
		boost::regex_replace(i, s, e, re, str_fmt, mth_flag);
		return result;
	}

	// String replace(String_alloc, text_s, text_e, re, str_fmt, mth_flag);

	template<typename String, typename Alloc,
				typename BiIter, typename Traits, typename Formatter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::and_
				<
					container_ex::is_container<String>,
					charset::is_not_utf8_string_t<String>,
					container_ex::is_allocator<Alloc>
				>,
				boost::mpl::and_
				<
					is_bidirectional_iterator<BiIter>,
					charset::is_not_any_utf8_string_iterator_t<BiIter>
				>,
				boost::mpl::and_
				<
					charset::is_generalized_string_t<Formatter>,
					charset::is_not_utf8_string_t<Formatter>
				>
			>,
			String
		>::type
		replace(const Alloc& alloc,
					BiIter s, BiIter e,
					const boost::basic_regex<typename detail::iterator_helper<BiIter>::value_type, Traits>& re,
					const Formatter& str_fmt,
					match_flag_type mth_flag = boost::match_default | boost::format_all)
	{
		typedef String ret_type;
		typedef BiIter target_iter_type;
		typedef detail::iterator_helper<target_iter_type> target_iter_h_type;
		typedef typename target_iter_h_type::value_type char_type;

		typedef typename boost::range_value<ret_type>::type chk_char_type;

		BOOST_MPL_ASSERT((boost::mpl::bool_<(sizeof(char_type) == sizeof(chk_char_type))>));

		ret_type result(alloc);
		boost::YGGR_BOOST_REGEX_DETAIL_NS::string_out_iterator< ret_type > i(result);
		boost::regex_replace(i, s, e, re, str_fmt, mth_flag);
		return result;
	}

	// out_iter replace(out_iter, text, re, fmt, mth_flag)
	template<typename OutIter, typename Target,
				typename Traits, typename Formatter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::and_
				<
					is_output_iterator<OutIter>,
					charset::is_not_any_utf8_string_iterator_t<OutIter>
				>,
				boost::mpl::and_
				<
					charset::is_generalized_string_t<Target>,
					charset::is_not_utf8_string_t<Target>
				>,
				boost::mpl::and_
				<
					charset::is_generalized_string_t<Formatter>,
					charset::is_not_utf8_string_t<Formatter>
				>
			>,
			OutIter
		>::type
		replace(OutIter out_iter, const Target& text,
				const boost::basic_regex<typename detail::string_helper<Target>::value_type, Traits>& re,
				const Formatter& str_fmt,
				match_flag_type mth_flag = boost::match_default | boost::format_all)
	{
		typedef Target target_type;
		typedef detail::string_helper<target_type> target_h_type;

		return
			this_type::replace(
				out_iter,
				target_h_type::begin(text), target_h_type::end(text),
				re,
				str_fmt, mth_flag);
	}

	// basic_string replace(text, re, str_fmt, mth_flag);
	template<typename Target, typename Traits, typename Formatter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::and_
				<
					charset::is_generalized_string_t<Target>,
					charset::is_not_utf8_string_t<Target>
				>,
				boost::mpl::and_
				<
					charset::is_generalized_string_t<Formatter>,
					charset::is_not_utf8_string_t<Formatter>
				>
			>,
			charset::basic_string<typename detail::string_helper<Target>::value_type>
		>::type
		replace(const Target& text,
				const boost::basic_regex<typename detail::string_helper<Target>::value_type, Traits>& re,
				const Formatter& str_fmt,
				match_flag_type mth_flag = boost::match_default | boost::format_all)
	{
		typedef Target target_type;
		typedef detail::string_helper<target_type> target_h_type;
		typedef typename target_h_type::value_type char_type;
		typedef charset::basic_string<char_type> ret_type;

		ret_type result;
		boost::YGGR_BOOST_REGEX_DETAIL_NS::string_out_iterator< ret_type > i(result);
		this_type::replace(
				i,
				target_h_type::begin(text), target_h_type::end(text),
				re,
				str_fmt, mth_flag);
		return result;
	}

	// basic_string replace(basic_string_alloc, text, re, str_fmt, mth_flag);
	template<typename Alloc, typename Target, typename Traits, typename Formatter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				container_ex::is_allocator<Alloc>,
				boost::mpl::and_
				<
					charset::is_generalized_string_t<Target>,
					charset::is_not_utf8_string_t<Target>
				>,
				boost::mpl::and_
				<
					charset::is_generalized_string_t<Formatter>,
					charset::is_not_utf8_string_t<Formatter>
				>
			>,
			typename
				boost::mpl::if_
				<
					boost::is_same
					<
						typename
							container_ex::allocator_t
							<
								charset::basic_string<typename detail::string_helper<Target>::value_type>
							>::type,
						Alloc
					>,
					charset::basic_string<typename detail::string_helper<Target>::value_type>,
					typename
						mplex::typename_replaceor
						<
							charset::basic_string<typename detail::string_helper<Target>::value_type>,
							mplex::_keep, mplex::_keep, Alloc
						>::type
				>::type
		>::type
		replace(const Alloc& alloc,
				const Target& text,
				const boost::basic_regex<typename detail::string_helper<Target>::value_type, Traits>& re,
				const Formatter& str_fmt,
				match_flag_type mth_flag = boost::match_default | boost::format_all)
	{
		typedef Target target_type;
		typedef detail::string_helper<target_type> target_h_type;
		typedef typename target_h_type::value_type char_type;
		typedef charset::basic_string<char_type> result_cont_type;

		typedef typename
			boost::mpl::if_
			<
				boost::is_same
				<
					typename
						container_ex::allocator_t
						<
							charset::basic_string<typename detail::string_helper<Target>::value_type>
						>::type,
					Alloc
				>,
				charset::basic_string<typename detail::string_helper<Target>::value_type>,
				typename
					mplex::typename_replaceor
					<
						charset::basic_string<typename detail::string_helper<Target>::value_type>,
						mplex::_keep, mplex::_keep, Alloc
					>::type
			>::type ret_type;

		ret_type result(alloc);
		boost::YGGR_BOOST_REGEX_DETAIL_NS::string_out_iterator< ret_type > i(result);
		this_type::replace(
				i,
				target_h_type::begin(text), target_h_type::end(text),
				re,
				str_fmt, mth_flag);
		return result;
	}

	// String replace(text, re, str_fmt, mth_flag);
	template<typename String, typename Target, typename Traits, typename Formatter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::and_
				<
					container_ex::is_container<String>,
					charset::is_not_utf8_string_t<String>,
					boost::mpl::not_< boost::is_same<String, Target> >
				>,
				boost::mpl::and_
				<
					charset::is_generalized_string_t<Target>,
					charset::is_not_utf8_string_t<Target>
				>,
				boost::mpl::and_
				<
					charset::is_generalized_string_t<Formatter>,
					charset::is_not_utf8_string_t<Formatter>
				>
			>,
			String
		>::type
		replace(const Target& text,
				const boost::basic_regex<typename detail::string_helper<Target>::value_type, Traits>& re,
				const Formatter& str_fmt,
				match_flag_type mth_flag = boost::match_default | boost::format_all)
	{
		typedef String ret_type;
		typedef Target target_type;
		typedef detail::string_helper<target_type> target_h_type;
		typedef typename target_h_type::value_type char_type;
		typedef typename boost::range_value<ret_type>::type chk_char_type;

		BOOST_MPL_ASSERT((boost::mpl::bool_<(sizeof(char_type) == sizeof(chk_char_type))>));

		ret_type result;
		boost::YGGR_BOOST_REGEX_DETAIL_NS::string_out_iterator< ret_type > i(result);
		this_type::replace(
				i,
				target_h_type::begin(text), target_h_type::end(text),
				re,
				str_fmt, mth_flag);
		return result;
	}

	// String replace(String_alloc, text, re, str_fmt, mth_flag);
	template<typename String, typename Alloc,
				typename Target, typename Traits, typename Formatter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::and_
				<
					container_ex::is_container<String>,
					charset::is_not_utf8_string_t<String>,
					container_ex::is_allocator<Alloc>
				>,
				boost::mpl::and_
				<
					charset::is_generalized_string_t<Target>,
					charset::is_not_utf8_string_t<Target>
				>,
				boost::mpl::and_
				<
					charset::is_generalized_string_t<Formatter>,
					charset::is_not_utf8_string_t<Formatter>
				>
			>,
			String
		>::type
		replace(const Alloc& alloc,
				const Target& text,
				const boost::basic_regex<typename detail::string_helper<Target>::value_type, Traits>& re,
				const Formatter& str_fmt,
				match_flag_type mth_flag = boost::match_default | boost::format_all)
	{
		typedef String ret_type;
		typedef Target target_type;
		typedef detail::string_helper<target_type> target_h_type;
		typedef typename target_h_type::value_type char_type;
		typedef typename boost::range_value<ret_type>::type chk_char_type;

		BOOST_MPL_ASSERT((boost::mpl::bool_<(sizeof(char_type) == sizeof(chk_char_type))>));

		ret_type result(alloc);
		boost::YGGR_BOOST_REGEX_DETAIL_NS::string_out_iterator< ret_type > i(result);
		this_type::replace(
				i,
				target_h_type::begin(text), target_h_type::end(text),
				re,
				str_fmt, mth_flag);
		return result;
	}

	//---------------------------------format--------------------------------------

	// out_iter format(out_iter, mrst, str_fmt, mth_flag);
	template<typename OutIter, typename BiIter, typename Alloc, typename Formatter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::and_
				<
					is_output_iterator<OutIter>,
					charset::is_not_any_utf8_string_iterator_t<OutIter>
				>,
				boost::mpl::and_
				<
					charset::is_generalized_string_t<Formatter>,
					charset::is_not_utf8_string_t<Formatter>
				>
			>,
			OutIter
		>::type
		format(OutIter out_iter,
				const boost::match_results<BiIter, Alloc>& mrst,
				const Formatter& str_fmt,
				match_flag_type flags = boost::format_all)
	{
		return mrst.format(out_iter, str_fmt, flags);
	}

	// basic_string format(mrst, str_fmt, mth_flag);
	template<typename BiIter, typename Alloc, typename Formatter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::is_generalized_string_t<Formatter>,
				charset::is_not_utf8_string_t<Formatter>
			>,
			charset::basic_string<typename boost::match_results<BiIter, Alloc>::char_type>
		>::type
		format(const boost::match_results<BiIter, Alloc>& mrst,
				const Formatter& str_fmt,
				match_flag_type flags = boost::format_all)
	{
		typedef boost::match_results<BiIter, Alloc> match_results_type;
		typedef typename match_results_type::char_type char_type;
		typedef charset::basic_string<char_type> ret_type;

		ret_type result;
		boost::YGGR_BOOST_REGEX_DETAIL_NS::string_out_iterator< ret_type > i(result);
		mrst.format(i, str_fmt, flags);
		return result;
	}

	// basic_string format(basic_string_alloc, mrst, str_fmt, mth_flag);
	template<typename RetAlloc, typename BiIter, typename Alloc, typename Formatter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				container_ex::is_allocator<RetAlloc>,
				charset::is_generalized_string_t<Formatter>,
				charset::is_not_utf8_string_t<Formatter>
			>,
			typename
				boost::mpl::if_
				<
					boost::is_same
					<
						typename
							container_ex::allocator_t
							<
								charset::basic_string<typename boost::match_results<BiIter, Alloc>::char_type>
							>::type,
						RetAlloc
					>,
					charset::basic_string<typename boost::match_results<BiIter, Alloc>::char_type>,
					typename
						mplex::typename_replaceor
						<
							charset::basic_string<typename boost::match_results<BiIter, Alloc>::char_type>,
							mplex::_keep, mplex::_keep, RetAlloc
						>::type
				>::type
		>::type
		format(const RetAlloc& alloc,
				const boost::match_results<BiIter, Alloc>& mrst,
				const Formatter& str_fmt,
				match_flag_type flags = boost::format_all)
	{
		typedef boost::match_results<BiIter, Alloc> match_results_type;
		typedef typename match_results_type::char_type char_type;

		typedef typename
			boost::mpl::if_
			<
				boost::is_same
				<
					typename
						container_ex::allocator_t
						<
							charset::basic_string<typename boost::match_results<BiIter, Alloc>::char_type>
						>::type,
					RetAlloc
				>,
				charset::basic_string<typename boost::match_results<BiIter, Alloc>::char_type>,
				typename
					mplex::typename_replaceor
					<
						charset::basic_string<typename boost::match_results<BiIter, Alloc>::char_type>,
						mplex::_keep, mplex::_keep, RetAlloc
					>::type
			>::type ret_type;

		ret_type result(alloc);
		boost::YGGR_BOOST_REGEX_DETAIL_NS::string_out_iterator< ret_type > i(result);
		mrst.format(i, str_fmt, flags);
		return result;
	}

	// String format(mrst, str_fmt, mth_flag);
	template<typename String, typename BiIter, typename Alloc, typename Formatter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::and_
				<
					container_ex::is_container<String>,
					charset::is_not_utf8_string_t<String>
				>,
				boost::mpl::and_
				<
					charset::is_generalized_string_t<Formatter>,
					charset::is_not_utf8_string_t<Formatter>
				>
			>,
			String
		>::type
		format(const boost::match_results<BiIter, Alloc>& mrst,
				const Formatter& str_fmt,
				match_flag_type flags = boost::format_all)
	{
		typedef String ret_type;
		typedef boost::match_results<BiIter, Alloc> match_result_type;
		typedef typename match_result_type::char_type char_type;

		typedef typename boost::range_value<ret_type>::type chk_char_type;

		BOOST_MPL_ASSERT((boost::mpl::bool_<(sizeof(char_type) == sizeof(chk_char_type))>));

		ret_type result;
		boost::YGGR_BOOST_REGEX_DETAIL_NS::string_out_iterator< ret_type > i(result);
		mrst.format(i, str_fmt, flags);
		return result;
	}

	// String format(String_alloc, mrst, str_fmt, mth_flag);
	template<typename String, typename RetAlloc, typename BiIter, typename Alloc, typename Formatter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::and_
				<
					container_ex::is_container<String>,
					charset::is_not_utf8_string_t<String>,
					container_ex::is_allocator<RetAlloc>
				>,
				boost::mpl::and_
				<
					charset::is_generalized_string_t<Formatter>,
					charset::is_not_utf8_string_t<Formatter>
				>
			>,
			String
		>::type
		format(const RetAlloc& alloc,
				const boost::match_results<BiIter, Alloc>& mrst,
				const Formatter& str_fmt,
				match_flag_type flags = boost::format_all)
	{
		typedef String ret_type;
		typedef boost::match_results<BiIter, Alloc> match_result_type;
		typedef typename match_result_type::char_type char_type;

		typedef typename boost::range_value<ret_type>::type chk_char_type;

		BOOST_MPL_ASSERT((boost::mpl::bool_<(sizeof(char_type) == sizeof(chk_char_type))>));

		ret_type result(alloc);
		boost::YGGR_BOOST_REGEX_DETAIL_NS::string_out_iterator< ret_type > i(result);
		mrst.format(i, str_fmt, flags);
		return result;
	}

	//------------------------------make_regex_iterator------------------------

	//regex_iterator make_regex_iterator(text_s, text_e, re, mth_flag);
	template<typename BiIter, typename Traits> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_bidirectional_iterator<BiIter>,
				charset::is_not_any_utf8_string_iterator_t<BiIter>
			>,
			boost::regex_iterator<BiIter, typename detail::iterator_helper<BiIter>::value_type, Traits>
		>::type
		make_regex_iterator(BiIter s, BiIter e,
								const
									boost::basic_regex
									<
										typename detail::iterator_helper<BiIter>::value_type,
										Traits
									>& re,
								match_flag_type mth_flag = boost::match_default)
	{
		typedef BiIter bi_iter_type;
		typedef Traits traits_type;
		typedef detail::iterator_helper<bi_iter_type> bi_iter_h_type;
		typedef typename bi_iter_h_type::value_type char_type;
		typedef boost::regex_iterator<bi_iter_type, char_type, traits_type> ret_type;

		return ret_type(s, e, re, mth_flag);
	}

	// regex_iterator make_regex_iterator(text, re, mth_flag);
	template<typename Target, typename Traits> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::is_generalized_string_t<Target>,
				charset::is_not_utf8_string_t<Target>
			>,
			boost::regex_iterator
			<
				typename detail::string_helper<Target>::citer_type,
				typename detail::string_helper<Target>::value_type,
				Traits
			>
		>::type
		make_regex_iterator(const Target& text,
								const
									boost::basic_regex
									<
										typename detail::string_helper<Target>::value_type,
										Traits
									>& re,
								match_flag_type mth_flag = boost::match_default)
	{
		typedef Target target_type;
		typedef Traits traits_type;
		typedef detail::string_helper<target_type> target_h_type;
		typedef typename target_h_type::value_type char_type;
		typedef typename target_h_type::citer_type citer_type;
		typedef boost::regex_iterator<citer_type, char_type, traits_type> ret_type;

		return ret_type(target_h_type::begin(text), target_h_type::end(text), re, mth_flag);
	}


	//------------------------------grep---------------------------------------

	// size_type grep(handler, regex_iter);
	template<typename Handler, typename BiIter, typename Traits>
	static
	typename
		boost::enable_if
		<
			is_callable<Handler>,
			size_type
		>::type
		grep(const Handler& handler,
				boost::regex_iterator
				<
					BiIter,
					typename detail::iterator_helper<BiIter>::value_type,
					Traits
				> regex_iter)
	{
		typedef BiIter bi_iter_type;
		typedef Traits traits_type;
		typedef detail::iterator_helper<bi_iter_type> bi_iter_h_type;
		typedef typename bi_iter_h_type::value_type char_type;
		typedef boost::regex_iterator<bi_iter_type, char_type, traits_type> regex_iter_type;

		size_type count = 0;

		for(regex_iter_type i = regex_iter, isize = regex_iter_type();
				(i != isize) && (0 != static_cast<int>(handler(*i)));
				++i, ++count);

		return count;
	}

	// size_typ grep(handler, pred, regex_iterator)
	template<typename Handler, typename Pred, typename BiIter, typename Traits>
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_callable<Handler>,
				is_callable<Pred>
			>,
			size_type
		>::type
		grep(const Handler& handler, const Pred& pred,
				boost::regex_iterator
				<
					BiIter,
					typename detail::iterator_helper<BiIter>::value_type,
					Traits
				> regex_iter)
	{
		typedef BiIter bi_iter_type;
		typedef Traits traits_type;
		typedef detail::iterator_helper<bi_iter_type> bi_iter_h_type;
		typedef typename bi_iter_h_type::value_type char_type;
		typedef boost::regex_iterator<bi_iter_type, char_type, traits_type> regex_iter_type;

		size_type count = 0;

		for(regex_iter_type i = regex_iter, isize = regex_iter_type();
				(i != isize) && pred(handler(*i));
				++i, ++count);

		return count;
	}

	// size_type grep(handler, text_s, text_e, re, mth_flag);
	template<typename Handler, typename BiIter, typename Traits> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::and_
				<
					is_bidirectional_iterator<BiIter>,
					charset::is_not_any_utf8_string_iterator_t<BiIter>
				>,
				is_callable<Handler>
			>,
			size_type
		>::type
		grep(const Handler& handler,
				BiIter text_s, BiIter text_e,
				const boost::basic_regex<typename detail::iterator_helper<BiIter>::value_type, Traits>& re,
				match_flag_type mth_flag = boost::match_default)
	{
		return this_type::grep(handler, this_type::make_regex_iterator(text_s, text_e, re, mth_flag));
	}

	// size_type grep(handler, pred, text_s, text_e, re, mth_flag);
	template<typename Handler, typename Pred, typename BiIter, typename Traits> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::and_
				<
					is_callable<Handler>,
					is_callable<Pred>
				>,
				boost::mpl::and_
				<
					is_bidirectional_iterator<BiIter>,
					charset::is_not_any_utf8_string_iterator_t<BiIter>
				>
			>,
			size_type
		>::type
		grep(const Handler& handler, const Pred& pred,
				BiIter text_s, BiIter text_e,
				const boost::basic_regex<typename detail::iterator_helper<BiIter>::value_type, Traits>& re,
				match_flag_type mth_flag = boost::match_default)
	{
		return this_type::grep(handler, pred, this_type::make_regex_iterator(text_s, text_e, re, mth_flag));
	}

	// size_type grep(handler, text, re, mth_flag)
	template<typename Handler, typename Target, typename Traits>
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::and_
				<
					charset::is_generalized_string_t<Target>,
					charset::is_not_utf8_string_t<Target>
				>,
				is_callable<Handler>
			>,
			size_type
		>::type
		grep(const Handler& handler,
				const Target& text,
				const boost::basic_regex<typename detail::string_helper<Target>::value_type, Traits>& re,
				match_flag_type mth_flag = boost::match_default)
	{
		return this_type::grep(handler, this_type::make_regex_iterator(text, re, mth_flag));
	}

	// size_type grep(handler, pred, text, re, mth_flag)
	template<typename Handler, typename Pred, typename Target, typename Traits> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::and_
				<
					charset::is_generalized_string_t<Target>,
					charset::is_not_utf8_string_t<Target>
				>,
				boost::mpl::and_
				<
					is_callable<Handler>,
					is_callable<Pred>
				>
			>,
			size_type
		>::type
		grep(const Handler& handler, const Pred& pred,
				const Target& text,
				const boost::basic_regex<typename detail::string_helper<Target>::value_type, Traits>& re,
				match_flag_type mth_flag = boost::match_default)
	{
		return this_type::grep(handler, pred, this_type::make_regex_iterator(text, re, mth_flag));
	}

	//----------------------------------make_regex_token_iterator---------------

	// regex_token_iterator make_regex_token_iterator(text_s, text_e, re, submatch, mth_flag)
	template<typename BiIter, typename Traits> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_bidirectional_iterator<BiIter>,
				charset::is_not_any_utf8_string_iterator_t<BiIter>
			>,
			boost::regex_token_iterator<BiIter, typename detail::iterator_helper<BiIter>::value_type, Traits>
		>::type
		make_regex_token_iterator(BiIter s, BiIter e,
									const
										boost::basic_regex
										<
											typename detail::iterator_helper<BiIter>::value_type,
											Traits
										>& re,
									int submatch = 0,
									match_flag_type mth_flag = boost::match_default)
	{
		typedef BiIter bi_iter_type;
		typedef Traits traits_type;
		typedef detail::iterator_helper<bi_iter_type> bi_iter_h_type;
		typedef typename bi_iter_h_type::value_type char_type;
		typedef boost::regex_token_iterator<bi_iter_type, char_type, traits_type> ret_type;

		return ret_type(s, e, re, submatch, mth_flag);
	}

	// regex_token_iterator make_regex_token_iterator(text, re, submatch, mth_flag)
	template<typename Target, typename Traits> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::is_generalized_string_t<Target>,
				charset::is_not_utf8_string_t<Target>
			>,
			boost::regex_token_iterator
			<
				typename detail::string_helper<Target>::citer_type,
				typename detail::string_helper<Target>::value_type,
				Traits
			>
		>::type
		make_regex_token_iterator(const Target& text,
									const
										boost::basic_regex
										<
											typename detail::string_helper<Target>::value_type,
											Traits
										>& re,
									int submatch = 0,
									match_flag_type mth_flag = boost::match_default)
	{
		typedef Target target_type;
		typedef Traits traits_type;
		typedef detail::string_helper<target_type> target_h_type;
		typedef typename target_h_type::value_type char_type;
		typedef typename target_h_type::citer_type citer_type;
		typedef boost::regex_token_iterator<citer_type, char_type, traits_type> ret_type;

		return ret_type(target_h_type::begin(text), target_h_type::end(text), re, submatch, mth_flag);
	}

	// regex_token_iterator make_regex_token_iterator(text_s, text_e, re, submatch, mth_flag)
	template<typename BiIter, typename Traits, typename SubMatch> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_bidirectional_iterator<BiIter>,
				charset::is_not_any_utf8_string_iterator_t<BiIter>,
				is_convertible_size_t<SubMatch>
			>,
			boost::regex_token_iterator<BiIter, typename detail::iterator_helper<BiIter>::value_type, Traits>
		>::type
		make_regex_token_iterator(BiIter s, BiIter e,
									const
										boost::basic_regex
										<
											typename detail::iterator_helper<BiIter>::value_type,
											Traits
										>& re,
									SubMatch submatch,
									match_flag_type mth_flag = boost::match_default)
	{
		typedef BiIter bi_iter_type;
		typedef Traits traits_type;
		typedef detail::iterator_helper<bi_iter_type> bi_iter_h_type;
		typedef typename bi_iter_h_type::value_type char_type;
		typedef boost::regex_token_iterator<bi_iter_type, char_type, traits_type> ret_type;

		return ret_type(s, e, re, submatch, mth_flag);
	}

	// regex_token_iterator make_regex_token_iterator(text, re, submatch, mth_flag)
	template<typename Target, typename Traits, typename SubMatch> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::is_generalized_string_t<Target>,
				charset::is_not_utf8_string_t<Target>,
				is_convertible_size_t<SubMatch>
			>,
			boost::regex_token_iterator
			<
				typename detail::string_helper<Target>::citer_type,
				typename detail::string_helper<Target>::value_type,
				Traits
			>
		>::type
		make_regex_token_iterator(const Target& text,
									const
										boost::basic_regex
										<
											typename detail::string_helper<Target>::value_type,
											Traits
										>& re,
									SubMatch submatch,
									match_flag_type mth_flag = boost::match_default)
	{
		typedef Target target_type;
		typedef Traits traits_type;
		typedef detail::string_helper<target_type> target_h_type;
		typedef typename target_h_type::value_type char_type;
		typedef typename target_h_type::citer_type citer_type;
		typedef boost::regex_token_iterator<citer_type, char_type, traits_type> ret_type;

		return ret_type(target_h_type::begin(text), target_h_type::end(text), re, submatch, mth_flag);
	}

	// regex_token_iterator make_regex_token_iterator(text_s, text_e, re, submatch, mth_flag)
	template<typename BiIter, typename Traits> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_bidirectional_iterator<BiIter>,
				charset::is_not_any_utf8_string_iterator_t<BiIter>
			>,
			boost::regex_token_iterator<BiIter, typename detail::iterator_helper<BiIter>::value_type, Traits>
		>::type
		make_regex_token_iterator(BiIter s, BiIter e,
									const
										boost::basic_regex
										<
											typename detail::iterator_helper<BiIter>::value_type,
											Traits
										>& re,
									const std::vector<int>& submatch,
									match_flag_type mth_flag = boost::match_default)
	{
		typedef BiIter bi_iter_type;
		typedef Traits traits_type;
		typedef detail::iterator_helper<bi_iter_type> bi_iter_h_type;
		typedef typename bi_iter_h_type::value_type char_type;
		typedef boost::regex_token_iterator<bi_iter_type, char_type, traits_type> ret_type;

		return ret_type(s, e, re, submatch, mth_flag);
	}

	// regex_token_iterator make_regex_token_iterator(text, re, submatch, mth_flag)
	template<typename Target, typename Traits> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::is_generalized_string_t<Target>,
				charset::is_not_utf8_string_t<Target>
			>,
			boost::regex_token_iterator
			<
				typename detail::string_helper<Target>::citer_type,
				typename detail::string_helper<Target>::value_type,
				Traits
			>
		>::type
		make_regex_token_iterator(const Target& text,
									const
										boost::basic_regex
										<
											typename detail::string_helper<Target>::value_type,
											Traits
										>& re,
									const std::vector<int>& submatch,
									match_flag_type mth_flag = boost::match_default)
	{
		typedef Target target_type;
		typedef Traits traits_type;
		typedef detail::string_helper<target_type> target_h_type;
		typedef typename target_h_type::value_type char_type;
		typedef typename target_h_type::citer_type citer_type;
		typedef boost::regex_token_iterator<citer_type, char_type, traits_type> ret_type;

		return ret_type(target_h_type::begin(text), target_h_type::end(text), re, submatch, mth_flag);
	}

	// regex_token_iterator make_regex_token_iterator(text_s, text_e, re, submatch, mth_flag)
	template<typename BiIter, typename Traits, std::size_t N> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_bidirectional_iterator<BiIter>,
				charset::is_not_any_utf8_string_iterator_t<BiIter>
			>,
			boost::regex_token_iterator<BiIter, typename detail::iterator_helper<BiIter>::value_type, Traits>
		>::type
		make_regex_token_iterator(BiIter s, BiIter e,
									const
										boost::basic_regex
										<
											typename detail::iterator_helper<BiIter>::value_type,
											Traits
										>& re,
									const int(&submatch)[N],
									match_flag_type mth_flag = boost::match_default)
	{
		typedef BiIter bi_iter_type;
		typedef Traits traits_type;
		typedef detail::iterator_helper<bi_iter_type> bi_iter_h_type;
		typedef typename bi_iter_h_type::value_type char_type;
		typedef boost::regex_token_iterator<bi_iter_type, char_type, traits_type> ret_type;

		return ret_type(s, e, re, submatch, mth_flag);
	}

	// regex_token_iterator make_regex_token_iterator(text, re, submatch, mth_flag)
	template<typename Target, typename Traits, std::size_t N> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::is_generalized_string_t<Target>,
				charset::is_not_utf8_string_t<Target>
			>,
			boost::regex_token_iterator
			<
				typename detail::string_helper<Target>::citer_type,
				typename detail::string_helper<Target>::value_type,
				Traits
			>
		>::type
		make_regex_token_iterator(const Target& text,
									const
										boost::basic_regex
										<
											typename detail::string_helper<Target>::value_type,
											Traits
										>& re,
									const int(&submatch)[N],
									match_flag_type mth_flag = boost::match_default)
	{
		typedef Target target_type;
		typedef Traits traits_type;
		typedef detail::string_helper<target_type> target_h_type;
		typedef typename target_h_type::value_type char_type;
		typedef typename target_h_type::citer_type citer_type;
		typedef boost::regex_token_iterator<citer_type, char_type, traits_type> ret_type;

		return ret_type(target_h_type::begin(text), target_h_type::end(text), re, submatch, mth_flag);
	}

	// regex_token_iterator make_regex_token_iterator(text_s, text_e, re, submatch, mth_flag)
	template<typename BiIter, typename Traits, typename SubMatch> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_bidirectional_iterator<BiIter>,
				charset::is_not_any_utf8_string_iterator_t<BiIter>,
				container_ex::is_container<SubMatch>
			>,
			boost::regex_token_iterator<BiIter, typename detail::iterator_helper<BiIter>::value_type, Traits>
		>::type
		make_regex_token_iterator(BiIter s, BiIter e,
									const
										boost::basic_regex
										<
											typename detail::iterator_helper<BiIter>::value_type,
											Traits
										>& re,
									const SubMatch& submatch,
									match_flag_type mth_flag = boost::match_default)
	{
		typedef BiIter bi_iter_type;
		typedef Traits traits_type;
		typedef detail::iterator_helper<bi_iter_type> bi_iter_h_type;
		typedef typename bi_iter_h_type::value_type char_type;
		typedef boost::regex_token_iterator<bi_iter_type, char_type, traits_type> ret_type;
		typedef std::vector<int> submatch_vt_type;

		submatch_vt_type vt(boost::begin(submatch), boost::end(submatch));
		return ret_type(s, e, re, vt, mth_flag);
	}

	// regex_token_iterator make_regex_token_iterator(text, re, submatch, mth_flag)
	template<typename Target, typename Traits, typename SubMatch> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::is_generalized_string_t<Target>,
				charset::is_not_utf8_string_t<Target>,
				container_ex::is_container<SubMatch>
			>,
			boost::regex_token_iterator
			<
				typename detail::string_helper<Target>::citer_type,
				typename detail::string_helper<Target>::value_type,
				Traits
			>
		>::type
		make_regex_token_iterator(const Target& text,
									const
										boost::basic_regex
										<
											typename detail::string_helper<Target>::value_type,
											Traits
										>& re,
									const SubMatch& submatch,
									match_flag_type mth_flag = boost::match_default)
	{
		typedef Target target_type;
		typedef Traits traits_type;
		typedef detail::string_helper<target_type> target_h_type;
		typedef typename target_h_type::value_type char_type;
		typedef typename target_h_type::citer_type citer_type;
		typedef boost::regex_token_iterator<citer_type, char_type, traits_type> ret_type;
		typedef std::vector<int> submatch_vt_type;

		submatch_vt_type vt(boost::begin(submatch), boost::end(submatch));
		return ret_type(target_h_type::begin(text), target_h_type::end(text), re, vt, mth_flag);
	}

	//----------------------------------split--------------------------------

private:
	template<typename Regex> inline
	static const Regex& default_split_regex(void)
	{
		typedef Regex regex_type;
		typedef typename regex_type::value_type char_type;

		YGGR_PP_CONST_STRING_LOCAL_DEF(spaces, "\\s+");
		static const regex_type re = this_type::make_regex(YGGR_PP_CONST_STRING_LOCAL_GET(spaces, char_type));
		return re;
	}

public:
	// size_type split(handler, token_iter)
	template<typename Handler, typename BiIter, typename Traits>
	static
	typename
		boost::enable_if
		<
			is_callable<Handler>,
			size_type
		>::type
		split(const Handler& handler,
				boost::regex_token_iterator
				<
					BiIter,
					typename detail::iterator_helper<BiIter>::value_type,
					Traits
				> token_iter)
	{
		typedef BiIter bi_iter_type;
		typedef Traits traits_type;
		typedef detail::iterator_helper<BiIter> bi_iter_h_type;
		typedef typename bi_iter_h_type::value_type char_type;
		typedef boost::regex_token_iterator<bi_iter_type, char_type, traits_type> regex_token_iter_type;

		size_type count = 0;
		for(regex_token_iter_type i = token_iter, isize = regex_token_iter_type();
				(i != isize) && (0 != static_cast<int>(handler(*i)));
				++i, ++count);

		return count;
	}

	// size_type split(handler, pred, token_iter)
	template<typename Handler, typename Pred, typename BiIter, typename Traits>
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_callable<Handler>,
				is_callable<Pred>
			>,
			size_type
		>::type
		split(const Handler& handler,
				const Pred& pred,
				boost::regex_token_iterator
				<
					BiIter,
					typename detail::iterator_helper<BiIter>::value_type,
					Traits
				> token_iter)
	{
		typedef BiIter bi_iter_type;
		typedef Traits traits_type;
		typedef detail::iterator_helper<BiIter> bi_iter_h_type;
		typedef typename bi_iter_h_type::value_type char_type;
		typedef boost::regex_token_iterator<bi_iter_type, char_type, traits_type> regex_token_iter_type;

		size_type count = 0;
		for(regex_token_iter_type i = token_iter, isize = regex_token_iter_type();
				i != isize && pred(handler(*i));
				++i, ++count);

		return count;
	}

	// size_type split(handler, text_s, text_e)
	template<typename Handler, typename BiIter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_callable<Handler>,
				is_bidirectional_iterator<BiIter>,
				charset::is_not_any_utf8_string_iterator_t<BiIter>
			>,
			size_type
		>::type
		split(const Handler& handler, BiIter text_s, BiIter text_e)
	{
		typedef BiIter bi_iter_type;
		typedef detail::iterator_helper<bi_iter_type> bi_iter_h_type;
		typedef typename bi_iter_h_type::value_type char_type;
		typedef boost::basic_regex<char_type> regex_type;

		return
			this_type::split(
				handler,
				this_type::make_regex_token_iterator(
					text_s, text_e, this_type::default_split_regex<regex_type>(), -1));
	}

	// size_type split(handler, text)
	template<typename Handler, typename Target> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_callable<Handler>,
				charset::is_generalized_string_t<Target>,
				charset::is_not_utf8_string_t<Target>
			>,
			size_type
		>::type
		split(const Handler& handler, const Target& text)
	{
		typedef Target target_type;
		typedef detail::string_helper<target_type> target_h_type;
		typedef typename target_h_type::value_type char_type;
		typedef boost::basic_regex<char_type> regex_type;

		return
			this_type::split(
				handler,
				this_type::make_regex_token_iterator(
					text, this_type::default_split_regex<regex_type>(), -1));
	}

	// size_type split(handler, pred, text_s, text_e)
	template<typename Handler, typename Pred, typename BiIter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::and_
				<
					is_callable<Handler>,
					is_callable<Pred>
				>,
				boost::mpl::and_
				<
					is_bidirectional_iterator<BiIter>,
					charset::is_not_any_utf8_string_iterator_t<BiIter>
				>
			>,
			size_type
		>::type
		split(const Handler& handler, const Pred& pred, BiIter text_s, BiIter text_e)
	{
		typedef BiIter bi_iter_type;
		typedef detail::iterator_helper<bi_iter_type> bi_iter_h_type;
		typedef typename bi_iter_h_type::value_type char_type;
		typedef boost::basic_regex<char_type> regex_type;

		return
			this_type::split(
				handler, pred,
				this_type::make_regex_token_iterator(
					text_s, text_e, this_type::default_split_regex<regex_type>(), -1));
	}

	// size_type split(handler, pred, text)
	template<typename Handler, typename Pred, typename Target> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::and_
				<
					is_callable<Handler>,
					is_callable<Pred>
				>,
				boost::mpl::and_
				<
					charset::is_generalized_string_t<Target>,
					charset::is_not_utf8_string_t<Target>
				>
			>,
			size_type
		>::type
		split(const Handler& handler, const Pred& pred, const Target& text)
	{
		typedef Target target_type;
		typedef detail::string_helper<target_type> target_h_type;
		typedef typename target_h_type::value_type char_type;
		typedef boost::basic_regex<char_type> regex_type;

		return
			this_type::split(
				handler, pred,
				this_type::make_regex_token_iterator(
					text, this_type::default_split_regex<regex_type>(), -1));
	}

	// size_type split(handler, text_s, text_e, re, mth_flag)
	template<typename Handler, typename BiIter, typename Traits> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_callable<Handler>,
				is_bidirectional_iterator<BiIter>,
				charset::is_not_any_utf8_string_iterator_t<BiIter>
			>,
			size_type
		>::type
		split(const Handler& handler, BiIter text_s, BiIter text_e,
				const
					boost::basic_regex
					<
						typename detail::iterator_helper<BiIter>::value_type,
						Traits
					>& re,
				match_flag_type mth_flag = boost::match_default)
	{
		return
			this_type::split(
				handler,
				this_type::make_regex_token_iterator(text_s, text_e, re, -1, mth_flag));
	}

	// size_type split(handler, text, re, mth_flag)
	template<typename Handler, typename Target, typename Traits> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_callable<Handler>,
				charset::is_generalized_string_t<Target>,
				charset::is_not_utf8_string_t<Target>
			>,
			size_type
		>::type
		split(const Handler& handler, const Target& text,
				const
					boost::basic_regex
					<
						typename detail::string_helper<Target>::value_type,
						Traits
					>& re,
				match_flag_type mth_flag = boost::match_default)
	{
		return
			this_type::split(
				handler,
				this_type::make_regex_token_iterator(text, re, -1, mth_flag));
	}

	// size_type split(handler, pred, text_s, text_e, re, mth_flag)
	template<typename Handler, typename Pred, typename BiIter, typename Traits> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::and_
				<
					is_callable<Handler>,
					is_callable<Pred>
				>,
				boost::mpl::and_
				<
					is_bidirectional_iterator<BiIter>,
					charset::is_not_any_utf8_string_iterator_t<BiIter>
				>
			>,
			size_type
		>::type
		split(const Handler& handler, const Pred& pred,
				BiIter text_s, BiIter text_e,
				const
					boost::basic_regex
					<
						typename detail::iterator_helper<BiIter>::value_type,
						Traits
					>& re,
				match_flag_type mth_flag = boost::match_default)
	{
		return
			this_type::split(
				handler, pred,
				this_type::make_regex_token_iterator(text_s, text_e, re, -1, mth_flag) );
	}

	// size_type split(handler, pred, text, re, mth_flag)
	template<typename Handler, typename Pred, typename Target, typename Traits> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::and_
				<
					is_callable<Handler>,
					is_callable<Pred>
				>,
				boost::mpl::and_
				<
					charset::is_generalized_string_t<Target>,
					charset::is_not_utf8_string_t<Target>
				>
			>,
			size_type
		>::type
		split(const Handler& handler, const Pred& pred,
				const Target& text,
				const
					boost::basic_regex
					<
						typename detail::string_helper<Target>::value_type,
						Traits
					>& re,
				match_flag_type mth_flag = boost::match_default)
	{
		return
			this_type::split(
				handler, pred,
				this_type::make_regex_token_iterator(text, re, -1, mth_flag) );
	}

	// size_type split(handler, text_s, text_e, re, submatch, mth_flag)
	template<typename Handler, typename BiIter, typename Traits, typename SubMatch> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_callable<Handler>,
				boost::mpl::and_
				<
					is_bidirectional_iterator<BiIter>,
					charset::is_not_any_utf8_string_iterator_t<BiIter>
				>,
				boost::mpl::or_
				<
					container_ex::is_container<SubMatch>,
					is_convertible_size_t<SubMatch>
				>
			>,
			size_type
		>::type
		split(const Handler& handler, BiIter text_s, BiIter text_e,
				const
					boost::basic_regex
					<
						typename detail::iterator_helper<BiIter>::value_type,
						Traits
					>& re,
				const SubMatch& submatch,
				match_flag_type mth_flag = boost::match_default)
	{
		return
			this_type::split(
				handler,
				this_type::make_regex_token_iterator(text_s, text_e, re, submatch, mth_flag));
	}

	// size_type split(handler, text, re, submatch, mth_flag);
	template<typename Handler, typename Target, typename Traits, typename SubMatch> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_callable<Handler>,
				boost::mpl::and_
				<
					charset::is_generalized_string_t<Target>,
					charset::is_not_utf8_string_t<Target>
				>,
				boost::mpl::or_
				<
					container_ex::is_container<SubMatch>,
					is_convertible_size_t<SubMatch>
				>
			>,
			size_type
		>::type
		split(const Handler& handler, const Target& text,
				const
					boost::basic_regex
					<
						typename detail::string_helper<Target>::value_type,
						Traits
					>& re,
				const SubMatch& submatch,
				match_flag_type mth_flag = boost::match_default)
	{
		return
			this_type::split(
				handler,
				this_type::make_regex_token_iterator(text, re, submatch, mth_flag));
	}

	// size_type split(handler, pred, text_s, text_e, re, submatch, mth_flag)
	template<typename Handler, typename Pred, typename BiIter, typename Traits, typename SubMatch> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::and_
				<
					is_callable<Handler>,
					is_callable<Pred>
				>,
				boost::mpl::and_
				<
					is_bidirectional_iterator<BiIter>,
					charset::is_not_any_utf8_string_iterator_t<BiIter>
				>,
				boost::mpl::or_
				<
					container_ex::is_container<SubMatch>,
					is_convertible_size_t<SubMatch>
				>
			>,
			size_type
		>::type
		split(const Handler& handler, const Pred& pred,
				BiIter text_s, BiIter text_e,
				const
					boost::basic_regex
					<
						typename detail::iterator_helper<BiIter>::value_type,
						Traits
					>& re,
				const SubMatch& submatch,
				match_flag_type mth_flag = boost::match_default)
	{
		return
			this_type::split(
				handler, pred,
				this_type::make_regex_token_iterator(text_s, text_e, re, submatch, mth_flag) );
	}

	// size_type split(handler, pred, text, re, submatch, mth_flag);
	template<typename Handler, typename Pred, typename Target, typename Traits, typename SubMatch> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::and_
				<
					is_callable<Handler>,
					is_callable<Pred>
				>,
				boost::mpl::and_
				<
					charset::is_generalized_string_t<Target>,
					charset::is_not_utf8_string_t<Target>
				>,
				boost::mpl::or_
				<
					container_ex::is_container<SubMatch>,
					is_convertible_size_t<SubMatch>
				>
			>,
			size_type
		>::type
		split(const Handler& handler, const Pred& pred,
				const Target& text,
				const
					boost::basic_regex
					<
						typename detail::string_helper<Target>::value_type,
						Traits
					>& re,
				const SubMatch& submatch,
				match_flag_type mth_flag = boost::match_default)
	{
		return
			this_type::split(
				handler, pred,
				this_type::make_regex_token_iterator(text, re, submatch, mth_flag) );
	}

};

} // namespace regular_parse
} // namespace yggr

#endif //__YGGR_REGULAR_PARSE_REGEX_PARSE_HPP__
