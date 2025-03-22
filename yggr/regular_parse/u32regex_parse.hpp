//u32regex_parse.hpp

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

#ifndef __YGGR_REGULAR_PARSE_U32REGEX_PARSE_HPP__
#define __YGGR_REGULAR_PARSE_U32REGEX_PARSE_HPP__

//#ifdef __MINGW32__
//#	error "mingw32 not support this!!!"
//#endif // __MINGW32__


/*
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!! The validity of the icase option for									!!!
!!!		BOOST REGEX requires reference to the IBM-ICU UnicodeData.txt file	!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/

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

#include <yggr/base/yggrdef.h>

#ifdef BOOST_HAS_ICU

#include <yggr/mplex/static_assert.hpp>
#include <yggr/compatibility/stl_exception.hpp>
#include <yggr/container_ex/allocator_t.hpp>

#include <yggr/type_traits/is_convertible_size_t.hpp>
#include <yggr/mplex/typename_replaceor.hpp>

#include <yggr/func/is_callable.hpp>
#include <yggr/iterator_ex/is_iterator.hpp>
#include <yggr/iterator_ex/iterator_category_check.hpp>
#include <yggr/iterator_ex/output_iterator_value.hpp>

#include <yggr/container/get_allocator.hpp>
#include <yggr/container/vector.hpp>

#include <yggr/container_ex/is_container.hpp>
#include <yggr/container_ex/is_allocator.hpp>

#include <yggr/charset/utf8_string.hpp>

#include <yggr/regular_parse/re_config.hpp>
#include <yggr/regular_parse/detail/conv_iterator_helper.hpp>
#include <yggr/regular_parse/detail/string_helper.hpp>
#include <yggr/regular_parse/detail/iterator_helper.hpp>

#include <yggr/regular_parse/iterator_adp.hpp>
#include <yggr/regular_parse/regex_iterator_t_maker.hpp>
#include <yggr/regular_parse/match_results_t_maker.hpp>

#include <boost/ref.hpp>

#include <boost/range/functions.hpp>
#include <boost/range/value_type.hpp>
#include <boost/range/iterator.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/less_equal.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_base_of.hpp>

#if defined(YGGR_NO_CWCHAR) || defined(YGGR_NO_WSTRING)
#	if !defined(BOOST_NO_WREGEX)
#		define BOOST_NO_WREGEX
#	endif // BOOST_NO_WREGEX
#endif // defined(YGGR_NO_CWCHAR) || defined(YGGR_NO_WSTRING)

#include <boost/regex/config.hpp>
#include <boost/regex.hpp>
#include <boost/regex/icu.hpp>

#include <iterator>

//output_iterator_value extern of string_out_iterator
namespace yggr
{
namespace iterator_ex
{

template<typename S>
struct output_iterator_value< boost::YGGR_BOOST_REGEX_DETAIL_NS::string_out_iterator<S> >
{
private:
	typedef boost::YGGR_BOOST_REGEX_DETAIL_NS::string_out_iterator<S> iter_type;
public:
	typedef typename iter_type::value_type type;
};

} // namespace iterator_ex
} // namespace yggr

namespace yggr
{
namespace regular_parse
{

namespace detail
{

template<int N>
struct int_t_ptr_maker
{
public:
	YGGR_CONSTEXPR_OR_INLINE
	operator boost::mpl::int_<N> const* (void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return static_cast<boost::mpl::int_<N> const*>(0);
	}

#if !defined(BOOST_REGEX_CXX03)
	YGGR_CONSTEXPR_OR_INLINE
	operator std::integral_constant<int, N> const* (void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return static_cast<std::integral_constant<int, N> const*>(0);
	}
#endif // BOOST_REGEX_CXX03
};

} // namespace detail

class u32regex_parse
{
public:
	typedef boost::u32regex regex_type;

public:
	typedef boost::regbase regex_base_type;
	typedef regex_type::flag_type regex_flag_type;
	typedef boost::regex_constants::match_flag_type match_flag_type;
	typedef boost::regex_error error_type;

private:
	typedef u32regex_parse this_type;

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
				is_bidirectional_iterator<Iter>,
				boost::mpl::less_equal
				<
					typename detail::iterator_helper<Iter>::id_type,
					boost::mpl::size_t<3>
				>
			>,
			regex_type
		>::type
		make_regex(Iter s, Iter e, regex_flag_type re_flag = regex_base_type::normal)
	{
		typedef detail::iterator_helper<Iter> iter_h_type;
		return boost::make_u32regex(iter_h_type::org(s), iter_h_type::org(e), re_flag);
	}

	// regex_type make_regex(text, re_flag);
	template<typename StrRegex> inline
	static
	typename
		boost::enable_if
		<
			charset::is_generalized_string_t<StrRegex>,
			regex_type
		>::type
		make_regex(const StrRegex& str, regex_flag_type re_flag = regex_base_type::normal)
	{
		typedef detail::string_helper<StrRegex> regex_h_type;
		return boost::make_u32regex(regex_h_type::begin(str), regex_h_type::end(str), re_flag);
	}

	//-----------------------------match-----------------------------------------------

	// bool match(text_s, text_e, re, mflag);
	template<typename BiIter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_bidirectional_iterator<BiIter>,
				boost::mpl::less_equal
				<
					typename detail::iterator_helper<BiIter>::id_type,
					boost::mpl::size_t<3>
				>
			>,
			bool
		>::type
		match(BiIter text_s, BiIter text_e,
				const regex_type& re,
				match_flag_type mth_flag = boost::match_default)
	{
		typedef BiIter bi_iter_type;
		typedef detail::iterator_helper<bi_iter_type> bi_iter_h_type;
		return
			boost::u32regex_match(
				bi_iter_h_type::org(text_s), bi_iter_h_type::org(text_e), re, mth_flag);
	}

	// bool match(text_s, text_e, result, re, mflag);
	template<typename BiIter, typename Alloc> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_bidirectional_iterator<BiIter>,
				boost::mpl::less_equal
				<
					typename detail::iterator_helper<BiIter>::id_type,
					boost::mpl::size_t<3>
				>
			>,
			bool
		>::type
		match(BiIter text_s, BiIter text_e,
				boost::match_results<typename detail::iterator_helper<BiIter>::org_iter_type, Alloc>& m,
				const regex_type& re,
				match_flag_type mth_flag = boost::match_default)
	{
		typedef BiIter bi_iter_type;
		typedef detail::iterator_helper<bi_iter_type> bi_iter_h_type;

		return
			boost::u32regex_match(
				bi_iter_h_type::org(text_s), bi_iter_h_type::org(text_e), m, re, mth_flag);
	}

	// bool match(text, re, mflag);
	template<typename Target> inline
	static
	typename
		boost::enable_if
		<
			charset::is_generalized_string_t<Target>,
			bool
		>::type
		match(const Target& text,
				const regex_type& re,
				match_flag_type mth_flag = boost::match_default)
	{
		typedef Target target_type;
		typedef detail::string_helper<target_type> target_h_type;

		return
			this_type::match(
				target_h_type::begin(text), target_h_type::end(text), re, mth_flag);
	}

	// bool match(text, result, re, mflag);
	template<typename Target, typename Alloc> inline
	static
	typename
		boost::enable_if
		<
			charset::is_generalized_string_t<Target>,
			bool
		>::type
		match(const Target& text,
				boost::match_results<typename detail::string_helper<Target>::citer_type, Alloc>& m,
				const regex_type& re,
				match_flag_type mth_flag = boost::match_default)
	{
		typedef Target target_type;
		typedef detail::string_helper<target_type> target_h_type;

		return
			this_type::match(
				target_h_type::begin(text), target_h_type::end(text), m, re, mth_flag);
	}


	//-----------------------------search------------------------------------

	// bool search(text_s, text_e, re, mflag);
	template<typename BiIter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_bidirectional_iterator<BiIter>,
				boost::mpl::less_equal
				<
					typename detail::iterator_helper<BiIter>::id_type,
					boost::mpl::size_t<3>
				>
			>,
			bool
		>::type
		search(BiIter text_s, BiIter text_e,
				const regex_type& re,
				match_flag_type mth_flag = boost::match_default)
	{
		typedef BiIter bi_iter_type;
		typedef detail::iterator_helper<bi_iter_type> bi_iter_h_type;
		return
			boost::u32regex_search(
				bi_iter_h_type::org(text_s), bi_iter_h_type::org(text_e), re, mth_flag);
	}

	// bool search(text_s, text_e, result, re, mflag);
	template<typename BiIter, typename Alloc> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_bidirectional_iterator<BiIter>,
				boost::mpl::less_equal
				<
					typename detail::iterator_helper<BiIter>::id_type,
					boost::mpl::size_t<3>
				>
			>,
			bool
		>::type
		search(BiIter text_s, BiIter text_e,
				boost::match_results<typename detail::iterator_helper<BiIter>::org_iter_type, Alloc>& m,
				const regex_type& re,
				match_flag_type mth_flag = boost::match_default)
	{
		typedef BiIter bi_iter_type;
		typedef detail::iterator_helper<bi_iter_type> bi_iter_h_type;

		return
			boost::u32regex_search(
				bi_iter_h_type::org(text_s), bi_iter_h_type::org(text_e), m, re, mth_flag);
	}

	// bool search(text, re, mflag);
	template<typename Target> inline
	static
	typename
		boost::enable_if
		<
			charset::is_generalized_string_t<Target>,
			bool
		>::type
		search(const Target& text,
				const regex_type& re,
				match_flag_type mth_flag = boost::match_default)
	{
		typedef Target target_type;
		typedef detail::string_helper<target_type> target_h_type;

		return
			this_type::search(
				target_h_type::begin(text), target_h_type::end(text), re, mth_flag);
	}

	// bool search(text, result, re, mflag);
	template<typename Target, typename Alloc> inline
	static
	typename
		boost::enable_if
		<
			charset::is_generalized_string_t<Target>,
			bool
		>::type
		search(const Target& text,
				boost::match_results<typename detail::string_helper<Target>::citer_type, Alloc>& m,
				const regex_type& re,
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

private:
	/*
	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	!!!	why need this function,																										!!!
	!!!	because "make_utf32_out(out, static_cast<mpl::int_<sizeof(*first)> const*>(0))												!!!
	!!!	it can not be based on input_iterator to determine the type of output_iterator												!!!
	!!!		need to be based on output_iterator itself to decide																	!!!
	!!!																																!!!
	!!!	the native function it look like this:																						!!!
	!!!	template <class OutputIterator, class Iterator, class charT>																!!!
	!!!	inline OutputIterator u32regex_replace(OutputIterator out,																	!!!
	!!!							 Iterator first,																					!!!
	!!!							 Iterator last,																						!!!
	!!!							 const u32regex& e,																					!!!
	!!!							 const std::basic_string<charT>& fmt,																!!!
	!!!							 match_flag_type flags = match_default) {															!!!
	!!!	   return BOOST_REGEX_DETAIL_NS::extract_output_base																		!!!
	!!!		(																														!!!
	!!!		  BOOST_REGEX_DETAIL_NS::do_regex_replace(																				!!!
	!!!			 BOOST_REGEX_DETAIL_NS::make_utf32_out(out, static_cast<mpl::int_<sizeof(*first)> const*>(0)),						!!!
	!!!			 BOOST_REGEX_DETAIL_NS::make_utf32_seq(first, last, static_cast<mpl::int_<sizeof(*first)> const*>(0)),				!!!
	!!!			 e,																													!!!
	!!!			 BOOST_REGEX_DETAIL_NS::make_utf32_seq(fmt.begin(), fmt.end(), static_cast<mpl::int_<sizeof(charT)> const*>(0)),	!!!
	!!!			 flags)																												!!!
	!!!		  ); }																													!!!
	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	*/

	template<typename OutIter, typename Iter, typename Formatter> inline
	static OutIter u32regex_replace_ex(OutIter out,
										Iter first,
										Iter last,
										const regex_type& e,
										const Formatter& fmt,
										match_flag_type flags = boost::match_default)
	{
		typedef OutIter out_iter_type;
		typedef Iter iter_type;
		typedef Formatter fmt_type;

		typedef detail::iterator_helper<iter_type> iter_h_type;
		typedef detail::string_helper<fmt_type> fmt_h_type;

		typedef typename iter_h_type::value_type second_char_type;

		typedef typename output_iterator_value<out_iter_type>::type chk_out_iter_value_type;
		typedef typename
			boost::mpl::if_
			<
				boost::is_base_of<mplex::null_type, chk_out_iter_value_type>,
				second_char_type,
				chk_out_iter_value_type
			>::type out_char_type;

		typedef typename fmt_h_type::value_type fmt_value_type;

		//return
		//	boost::YGGR_BOOST_REGEX_DETAIL_NS::extract_output_base(
		//		boost::YGGR_BOOST_REGEX_DETAIL_NS::do_regex_replace(
		//			boost::YGGR_BOOST_REGEX_DETAIL_NS::make_utf32_out(
		//				out, static_cast<boost::mpl::int_<sizeof(out_char_type)> const*>(0)),
		//			boost::YGGR_BOOST_REGEX_DETAIL_NS::make_utf32_seq(
		//				iter_h_type::org(first), iter_h_type::org(last),
		//				static_cast<boost::mpl::int_<sizeof(second_char_type)> const*>(0)),
		//			e,
		//			boost::YGGR_BOOST_REGEX_DETAIL_NS::make_utf32_seq(
		//				fmt_h_type::begin(fmt), fmt_h_type::end(fmt),
		//				static_cast<boost::mpl::int_<sizeof(fmt_value_type)> const*>(0)),
		//			flags) );

		return
			boost::YGGR_BOOST_REGEX_DETAIL_NS::extract_output_base(
				boost::YGGR_BOOST_REGEX_DETAIL_NS::do_regex_replace(
					boost::YGGR_BOOST_REGEX_DETAIL_NS::make_utf32_out(
						out, detail::int_t_ptr_maker<sizeof(out_char_type)>()),
					boost::YGGR_BOOST_REGEX_DETAIL_NS::make_utf32_seq(
						iter_h_type::org(first), iter_h_type::org(last),
						detail::int_t_ptr_maker<sizeof(second_char_type)>()),
					e,
					boost::YGGR_BOOST_REGEX_DETAIL_NS::make_utf32_seq(
						fmt_h_type::begin(fmt), fmt_h_type::end(fmt),
						detail::int_t_ptr_maker<sizeof(fmt_value_type)>()),
					flags) );
	}

public:

	// out_iter replace(out_iter, text_s, text_e, re, mth_flag);
	template<typename OutIter, typename BiIter, typename Formatter> inline
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
				is_bidirectional_iterator<BiIter>,
				charset::is_generalized_string_t<Formatter>
			>,
			OutIter
		>::type
		replace(OutIter out_iter,
				BiIter s, BiIter e,
				const regex_type& re,
				const Formatter& str_fmt,
				match_flag_type mth_flag = boost::match_default | boost::format_all)
	{
		return this_type::u32regex_replace_ex(out_iter, s, e, re, str_fmt, mth_flag);
	}

	// utf8_string replace(text_s, text_e, re, str_fmt, mth_flag)
	template<typename BiIter, typename Formatter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_bidirectional_iterator<BiIter>,
				charset::is_generalized_string_t<Formatter>
			>,
			charset::utf8_string
		>::type
		replace(BiIter s, BiIter e,
					const regex_type& re,
					const Formatter& str_fmt,
					match_flag_type mth_flag = boost::match_default | boost::format_all)
	{
		typedef charset::utf8_string ret_type;
		typedef detail::string_helper<ret_type> ret_h_type;
		typedef ret_h_type::base_cont_type base_ret_type;

		ret_type result;
		boost::YGGR_BOOST_REGEX_DETAIL_NS::string_out_iterator< base_ret_type > i(ret_h_type::org(result));
		this_type::replace(i, s, e, re, str_fmt, mth_flag);
		ret_h_type::recount(result);
		return result;
	}

	// utf8_string replace(basic_string_alloc, text_s, text_e, re, str_fmt, mth_flag);
	template<typename BiIter, typename Alloc, typename Formatter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				container_ex::is_allocator<Alloc>,
				is_bidirectional_iterator<BiIter>,
				charset::is_generalized_string_t<Formatter>
			>,
			typename
				boost::mpl::if_
				<
					boost::is_same
					<
						container_ex::allocator_t
						<
							charset::utf8_string
						>::type,
						Alloc
					>,
					charset::utf8_string,
					typename
						mplex::typename_replaceor
						<
							charset::utf8_string,
							mplex::_keep, mplex::_keep, Alloc
						>::type
				>::type
		>::type
		replace(const Alloc& alloc,
				BiIter s, BiIter e,
				const regex_type& re,
				const Formatter& str_fmt,
				match_flag_type mth_flag = boost::match_default | boost::format_all)
	{
		typedef charset::utf8_string result_cont_type;
		typedef Alloc alloc_type;

		typedef typename
			boost::mpl::if_
			<
				boost::is_same
				<
					container_ex::allocator_t
					<
						charset::utf8_string
					>::type,
					Alloc
				>,
				charset::utf8_string,
				typename
					mplex::typename_replaceor
					<
						charset::utf8_string,
						mplex::_keep, mplex::_keep, Alloc
					>::type
			>::type ret_type;

		typedef detail::string_helper<ret_type> ret_h_type;
		typedef typename ret_h_type::base_cont_type base_ret_type;

		ret_type result(alloc);
		boost::YGGR_BOOST_REGEX_DETAIL_NS::string_out_iterator< base_ret_type > i(ret_h_type::org(result));
		this_type::replace(i, s, e, re, str_fmt, mth_flag);
		ret_h_type::recount(result);
		return result;
	}

	// String replace(text_s, text_e, re, str_fmt, mth_flag);
	template<typename String, typename BiIter, typename Formatter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_container<String>,
				is_bidirectional_iterator<BiIter>,
				charset::is_generalized_string_t<Formatter>
			>,
			String
		>::type
		replace(BiIter s, BiIter e,
					const regex_type& re,
					const Formatter& str_fmt,
					match_flag_type mth_flag = boost::match_default | boost::format_all)
	{
		typedef String ret_type;
		typedef detail::string_helper<ret_type> ret_h_type;
		typedef typename ret_h_type::base_cont_type base_ret_type;

		ret_type result;
		boost::YGGR_BOOST_REGEX_DETAIL_NS::string_out_iterator< base_ret_type > i(ret_h_type::org(result));
		this_type::replace(i, s, e, re, str_fmt, mth_flag);
		ret_h_type::recount(result);
		return result;
	}

	// String replace(String_alloc, text_s, text_e, re, str_fmt, mth_flag);
	template<typename String, typename Alloc, typename BiIter, typename Formatter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::and_
				<
					is_container<String>,
					container_ex::is_allocator<Alloc>
				>,
				is_bidirectional_iterator<BiIter>,
				charset::is_generalized_string_t<Formatter>
			>,
			String
		>::type
		replace(const Alloc& alloc,
					BiIter s, BiIter e,
					const regex_type& re,
					const Formatter& str_fmt,
					match_flag_type mth_flag = boost::match_default | boost::format_all)
	{
		typedef String ret_type;
		typedef detail::string_helper<ret_type> ret_h_type;
		typedef typename ret_h_type::base_cont_type base_ret_type;

		ret_type result(alloc);
		boost::YGGR_BOOST_REGEX_DETAIL_NS::string_out_iterator< base_ret_type > i(ret_h_type::org(result));
		this_type::replace(i, s, e, re, str_fmt, mth_flag);
		ret_h_type::recount(result);
		return result;
	}

	// out_iter replace(out_iter, text, re, fmt, mth_flag)
	template<typename OutIter, typename Target, typename Formatter> inline
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
				charset::is_generalized_string_t<Target>,
				charset::is_generalized_string_t<Formatter>
			>,
			OutIter
		>::type
		replace(OutIter out_iter, const Target& text,
				const regex_type& re,
				const Formatter& str_fmt,
				match_flag_type mth_flag = boost::match_default | boost::format_all)
	{
		typedef Target target_type;
		typedef detail::string_helper<target_type> target_h_type;

		return
			this_type::replace(
				out_iter,
				target_h_type::begin(text), target_h_type::end(text),
				re, str_fmt, mth_flag);
	}

	// utf8_string replace(text, re, str_fmt, mth_flag);
	template<typename Target, typename Formatter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::is_generalized_string_t<Target>,
				charset::is_generalized_string_t<Formatter>
			>,
			charset::utf8_string
		>::type
		replace(const Target& text,
				const regex_type& re,
				const Formatter& str_fmt,
				match_flag_type mth_flag = boost::match_default | boost::format_all)
	{
		typedef Target target_type;
		typedef charset::utf8_string ret_type;

		typedef detail::string_helper<target_type> target_h_type;
		typedef detail::string_helper<ret_type> ret_h_type;
		typedef ret_h_type::base_cont_type base_ret_type;

		ret_type result;
		boost::YGGR_BOOST_REGEX_DETAIL_NS::string_out_iterator< base_ret_type > i(ret_h_type::org(result));
		this_type::replace(
				i,
				target_h_type::begin(text), target_h_type::end(text),
				re, str_fmt, mth_flag);
		ret_h_type::recount(result);
		return result;
	}

	// utf8_string replace(basic_string_alloc, text, re, str_fmt, mth_flag);
	template<typename Target, typename Alloc, typename Formatter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				container_ex::is_allocator<Alloc>,
				charset::is_generalized_string_t<Target>,
				charset::is_generalized_string_t<Formatter>
			>,
			typename
				boost::mpl::if_
				<
					boost::is_same
					<
						container_ex::allocator_t
						<
							charset::utf8_string
						>::type,
						Alloc
					>,
					charset::utf8_string,
					typename
						mplex::typename_replaceor
						<
							charset::utf8_string,
							mplex::_keep, mplex::_keep, Alloc
						>::type
				>::type
		>::type
		replace(const Alloc& alloc,
				const Target& text,
				const regex_type& re,
				const Formatter& str_fmt,
				match_flag_type mth_flag = boost::match_default | boost::format_all)
	{
		typedef Target target_type;
		typedef Alloc alloc_type;
		typedef charset::utf8_string result_cont_type;

		typedef detail::string_helper<target_type> target_h_type;

		typedef typename
			boost::mpl::if_
			<
				boost::is_same
				<
					container_ex::allocator_t
					<
						charset::utf8_string
					>::type,
					Alloc
				>,
				charset::utf8_string,
				typename
					mplex::typename_replaceor
					<
						charset::utf8_string,
						mplex::_keep, mplex::_keep, Alloc
					>::type
			>::type ret_type;

		typedef detail::string_helper<ret_type> ret_h_type;
		typedef typename ret_h_type::base_cont_type base_ret_type;

		ret_type result(alloc);
		boost::YGGR_BOOST_REGEX_DETAIL_NS::string_out_iterator< base_ret_type > i(ret_h_type::org(result));
		this_type::replace(
				i,
				target_h_type::begin(text), target_h_type::end(text),
				re, str_fmt, mth_flag);
		ret_h_type::recount(result);
		return result;
	}

	// String replace(text, re, str_fmt, mth_flag);
	template<typename String, typename Target, typename Formatter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::and_
				<
					is_container<String>,
					charset::is_generalized_string_t<Target>,
					boost::mpl::not_< boost::is_same<String, Target> >
				>,
				charset::is_generalized_string_t<Formatter>
			>,
			String
		>::type
		replace(const Target& text,
				const regex_type& re,
				const Formatter& str_fmt,
				match_flag_type mth_flag = boost::match_default | boost::format_all)
	{
		typedef String ret_type;
		typedef Target target_type;

		typedef detail::string_helper<target_type> target_h_type;
		typedef detail::string_helper<ret_type> ret_h_type;
		typedef typename ret_h_type::base_cont_type base_ret_type;

		ret_type result;
		boost::YGGR_BOOST_REGEX_DETAIL_NS::string_out_iterator< base_ret_type > i(ret_h_type::org(result));
		this_type::replace(
				i,
				target_h_type::begin(text), target_h_type::end(text),
				re, str_fmt, mth_flag);
		ret_h_type::recount(result);
		return result;
	}

	// String replace(String_alloc, text, re, str_fmt, mth_flag);
	template<typename String, typename Target, typename Alloc, typename Formatter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::and_
				<
					is_container<String>,
					charset::is_generalized_string_t<Target>,
					boost::mpl::not_<boost::is_same<String, Target> >,
					container_ex::is_allocator<Alloc>
				>,
				charset::is_generalized_string_t<Formatter>
			>,
			String
		>::type
		replace(const Alloc& alloc,
				const Target& text,
				const regex_type& re,
				const Formatter& str_fmt,
				match_flag_type mth_flag = boost::match_default | boost::format_all)
	{
		typedef String ret_type;
		typedef Target target_type;

		typedef detail::string_helper<target_type> target_h_type;
		typedef detail::string_helper<ret_type> ret_h_type;

		typedef typename ret_h_type::base_cont_type base_ret_type;

		ret_type result(alloc);
		boost::YGGR_BOOST_REGEX_DETAIL_NS::string_out_iterator< base_ret_type > i(ret_h_type::org(result));
		this_type::replace(
				i,
				target_h_type::begin(text), target_h_type::end(text),
				re, str_fmt, mth_flag);
		ret_h_type::recount(result);
		return result;
	}

	//---------------------------------format--------------------------------------

private:
	// u32regex_format_ex_detail
	template<typename OutIter, typename BiIter, typename Alloc, typename FmtIter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_output_iterator<OutIter>,
				charset::is_not_any_utf8_string_iterator_t<OutIter>
			>,
			OutIter
		>::type
		u32regex_format_ex_detail(OutIter out_iter,
									const boost::match_results<BiIter, Alloc>& mrst,
									const std::pair<FmtIter, FmtIter>& fmt_iters,
									match_flag_type flags, const int*)
	{
		typedef OutIter out_iter_type;
		typedef BiIter bi_iter_type;
		typedef FmtIter fmt_iter_type;

		typedef detail::iterator_helper<bi_iter_type> bi_iter_h_type;
		typedef typename bi_iter_h_type::value_type char_type;

		typedef container::vector<char_type> fmt_type;
		typedef boost::basic_regex<char_type> re_type;

		typedef detail::iterator_helper<fmt_iter_type> fmt_iter_h_type;
		typedef typename fmt_iter_h_type::value_type chk_val_type;

		typedef typename output_iterator_value<out_iter_type>::type chk_val2_type;

		BOOST_MPL_ASSERT((boost::mpl::bool_<(sizeof(char_type) == sizeof(chk_val_type))>));
		BOOST_MPL_ASSERT((boost::mpl::bool_<(sizeof(char_type) == sizeof(chk_val2_type))>));

		fmt_type fmt(fmt_iters.first, fmt_iters.second);
		re_type re("");
		return
			boost::YGGR_BOOST_REGEX_DETAIL_NS::regex_format_imp(
				out_iter, mrst, fmt.begin(), fmt.end(), flags, re.get_traits());
	}

	template<typename OutIter, typename BiIter, typename Alloc, typename FmtIter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_output_iterator<OutIter>,
				charset::is_not_any_utf8_string_iterator_t<OutIter>
			>,
			OutIter
		>::type
		u32regex_format_ex_detail(OutIter out_iter,
									const boost::match_results<BiIter, Alloc>& mrst,
									const std::pair<FmtIter, FmtIter>& fmt_iters,
									match_flag_type flags, const char*)
	{
		typedef OutIter out_iter_type;
		typedef BiIter bi_iter_type;
		typedef FmtIter fmt_iter_type;

		typedef detail::iterator_helper<bi_iter_type> bi_iter_h_type;
		typedef typename bi_iter_h_type::value_type char_type;

		typedef boost::basic_regex<char_type> re_type;

		typedef detail::iterator_helper<fmt_iter_type> fmt_iter_h_type;
		typedef typename fmt_iter_h_type::value_type chk_val_type;

		typedef typename output_iterator_value<out_iter_type>::type chk_val2_type;

		BOOST_MPL_ASSERT((boost::mpl::bool_<(sizeof(char_type) == sizeof(chk_val_type))>));
		BOOST_MPL_ASSERT((boost::mpl::bool_<(sizeof(char_type) == sizeof(chk_val2_type))>));

		re_type re("");
		return
			boost::YGGR_BOOST_REGEX_DETAIL_NS::regex_format_imp(
				out_iter, mrst, fmt_iters.first, fmt_iters.second, flags, re.get_traits());
	}

	// u32regex_format_ex
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
					charset::is_not_any_utf8_string_iterator_t<OutIter>,
					boost::is_same
					<
						typename output_iterator_value<OutIter>::type,
						typename detail::iterator_helper<BiIter>::value_type
					>
				>,
				charset::is_generalized_string_t<Formatter>
			>,
			OutIter
		>::type
		u32regex_format_ex(OutIter out_iter,
				const boost::match_results<BiIter, Alloc>& mrst,
				const Formatter& str_fmt,
				match_flag_type flags = boost::format_all)
	{
		typedef BiIter bi_iter_type;
		typedef Formatter fmt_type;

		typedef detail::iterator_helper<bi_iter_type> bi_iter_h_type;
		typedef detail::string_helper<fmt_type> fmt_h_type;

		typedef typename bi_iter_h_type::value_type char_type;
		typedef typename fmt_h_type::value_type fmt_value_type;
		typedef typename fmt_h_type::citer_type fmt_iter_type;

		typedef detail::conv_iterator_helper<fmt_iter_type, char_type, fmt_value_type> conv_iter_h_type;

		typedef typename
			boost::mpl::if_c
			<
				sizeof(char_type) == sizeof(fmt_value_type),
				const char*,
				const int*
			>::type mark_type;


		conv_iter_h_type h;
		return
			this_type::u32regex_format_ex_detail(
				out_iter,
				mrst,
				h(fmt_h_type::begin(str_fmt), fmt_h_type::end(str_fmt)),
				flags, mark_type(0));
	}

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
					charset::is_not_any_utf8_string_iterator_t<OutIter>,
					boost::mpl::not_
					<
						boost::is_same
						<
							typename output_iterator_value<OutIter>::type,
							typename detail::iterator_helper<BiIter>::value_type
						>
					>
				>,
				charset::is_generalized_string_t<Formatter>
			>,
			OutIter
		>::type
		u32regex_format_ex(OutIter out_iter,
				const boost::match_results<BiIter, Alloc>& mrst,
				const Formatter& str_fmt,
				match_flag_type flags = boost::format_all)
	{
		typedef OutIter out_iter_type;
		typedef BiIter bi_iter_type;
		typedef Formatter fmt_type;

		typedef detail::iterator_helper<bi_iter_type> bi_iter_h_type;
		typedef detail::string_helper<fmt_type> fmt_h_type;

		typedef typename bi_iter_h_type::value_type char_type;

		typedef typename output_iterator_value<out_iter_type>::type chk_out_iter_value_type;
		typedef typename
			boost::mpl::if_
			<
				boost::is_base_of<mplex::null_type, chk_out_iter_value_type>,
				char_type,
				chk_out_iter_value_type
			>::type out_char_type;

		typedef charset::basic_string<char_type> mid_string_type;
		typedef typename mid_string_type::iterator mid_string_iter_type;
		typedef boost::YGGR_BOOST_REGEX_DETAIL_NS::string_out_iterator<mid_string_type> mid_out_iter_type;

		typedef typename fmt_h_type::value_type fmt_value_type;
		typedef typename fmt_h_type::citer_type fmt_iter_type;

		typedef detail::conv_iterator_helper<fmt_iter_type, char_type, fmt_value_type> fmt_conv_iter_h_type;
		typedef detail::conv_iterator_helper<mid_string_iter_type, out_char_type, char_type> out_conv_iter_h_type;
		typedef typename out_conv_iter_h_type::result_type mid_conv_iters_type;

		typedef typename
			boost::mpl::if_c
			<
				sizeof(char_type) == sizeof(fmt_value_type),
				const char*,
				const int*
			>::type mark_type;

		fmt_conv_iter_h_type fmt_conv;
		out_conv_iter_h_type out_conv;

		mid_string_type mid_result;

		this_type::u32regex_format_ex_detail(
				mid_out_iter_type(mid_result),
				mrst,
				fmt_conv(fmt_h_type::begin(str_fmt), fmt_h_type::end(str_fmt)),
				flags, mark_type(0));

		mid_conv_iters_type mid_iters = out_conv(boost::begin(mid_result), boost::end(mid_result));
		return std::copy(mid_iters.first, mid_iters.second, out_iter);
	}

public:
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
				charset::is_generalized_string_t<Formatter>
			>,
			OutIter
		>::type
		format(OutIter out_iter,
				const boost::match_results<BiIter, Alloc>& mrst,
				const Formatter& str_fmt,
				match_flag_type flags = boost::format_all)
	{
		return this_type::u32regex_format_ex(out_iter, mrst, str_fmt, flags);
	}

	// utf8_string format(mrst, str_fmt, mth_flag);
	template<typename BiIter, typename Alloc, typename Formatter> inline
	static
	typename
		boost::enable_if
		<
			charset::is_generalized_string_t<Formatter>,
			charset::utf8_string
		>::type
		format(const boost::match_results<BiIter, Alloc>& mrst,
				const Formatter& str_fmt,
				match_flag_type flags = boost::format_all)
	{
		typedef charset::utf8_string ret_type;
		typedef detail::string_helper<ret_type> ret_h_type;
		typedef ret_h_type::base_cont_type base_ret_type;

		ret_type result;
		boost::YGGR_BOOST_REGEX_DETAIL_NS::string_out_iterator< base_ret_type > i(ret_h_type::org(result));
		this_type::u32regex_format_ex(i, mrst, str_fmt, flags);
		ret_h_type::recount(result);
		return result;
	}

	// utf8_string format(basic_string_alloc, mrst, str_fmt, mth_flag);
	template<typename RetAlloc, typename BiIter, typename Alloc, typename Formatter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				container_ex::is_allocator<RetAlloc>,
				charset::is_generalized_string_t<Formatter>
			>,
			typename
				boost::mpl::if_
				<
					boost::is_same
					<
						container_ex::allocator_t
						<
							charset::utf8_string
						>::type,
						RetAlloc
					>,
					charset::utf8_string,
					typename
						mplex::typename_replaceor
						<
							charset::utf8_string,
							mplex::_keep, mplex::_keep, RetAlloc
						>::type
				>::type
		>::type
		format(const RetAlloc& alloc,
				const boost::match_results<BiIter, Alloc>& mrst,
				const Formatter& str_fmt,
				match_flag_type flags = boost::format_all)
	{
		typedef Formatter fmt_type;

		typedef typename
			boost::mpl::if_
			<
				boost::is_same
				<
					container_ex::allocator_t
					<
						charset::utf8_string
					>::type,
					RetAlloc
				>,
				charset::utf8_string,
				typename
					mplex::typename_replaceor
					<
						charset::utf8_string,
						mplex::_keep, mplex::_keep, RetAlloc
					>::type
			>::type ret_type;

		typedef detail::string_helper<ret_type> ret_h_type;
		typedef typename ret_h_type::base_cont_type base_ret_type;

		ret_type result(alloc);
		boost::YGGR_BOOST_REGEX_DETAIL_NS::string_out_iterator< base_ret_type > i(ret_h_type::org(result));
		this_type::u32regex_format_ex(i, mrst, str_fmt, flags);
		ret_h_type::recount(result);
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
				is_container<String>,
				charset::is_generalized_string_t<Formatter>
			>,
			String
		>::type
		format(const boost::match_results<BiIter, Alloc>& mrst,
				const Formatter& str_fmt,
				match_flag_type flags = boost::format_all)
	{
		typedef String ret_type;
		typedef detail::string_helper<ret_type> ret_h_type;
		typedef typename ret_h_type::base_cont_type base_ret_type;

		ret_type result;
		boost::YGGR_BOOST_REGEX_DETAIL_NS::string_out_iterator< base_ret_type > i(ret_h_type::org(result));
		this_type::u32regex_format_ex(i, mrst, str_fmt, flags);
		ret_h_type::recount(result);
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
					is_container<String>,
					container_ex::is_allocator<RetAlloc>
				>,
				charset::is_generalized_string_t<Formatter>
			>,
			String
		>::type
		format(const RetAlloc& alloc,
				const boost::match_results<BiIter, Alloc>& mrst,
				const Formatter& str_fmt,
				match_flag_type flags = boost::format_all)
	{
		typedef String ret_type;
		typedef detail::string_helper<ret_type> ret_h_type;
		typedef typename ret_h_type::base_cont_type base_ret_type;

		ret_type result(alloc);
		boost::YGGR_BOOST_REGEX_DETAIL_NS::string_out_iterator< base_ret_type > i(ret_h_type::org(result));
		this_type::u32regex_format_ex(i, mrst, str_fmt, flags);
		ret_h_type::recount(result);
		return result;
	}

	//------------------------------make_regex_iterator------------------------

	//regex_iterator make_regex_iterator(text_s, text_e, re, mth_flag);
	template<typename BiIter> inline
	static
	typename
		boost::enable_if
		<
			is_bidirectional_iterator<BiIter>,
			typename regex_iterator_t_maker<BiIter, regex_type>::type
		>::type
		make_regex_iterator(BiIter s, BiIter e,
								const regex_type& re,
								match_flag_type mth_flag = boost::match_default)
	{
		typedef BiIter bi_iter_type;
		typedef detail::iterator_helper<bi_iter_type> bi_iter_h_type;
		typedef typename regex_iterator_t_maker<bi_iter_type, regex_type>::type ret_type;

		return ret_type(bi_iter_h_type::org(s), bi_iter_h_type::org(e), re, mth_flag);
	}

	// regex_iterator make_regex_iterator(text, re, mth_flag);
	template<typename Target> inline
	static
	typename
		boost::enable_if
		<
			charset::is_generalized_string_t<Target>,
			typename
				regex_iterator_t_maker
				<
					typename detail::string_helper<Target>::citer_type,
					regex_type
				>::type
		>::type
		make_regex_iterator(const Target& text,
								const regex_type& re,
								match_flag_type mth_flag = boost::match_default)
	{

		typedef Target target_type;
		typedef detail::string_helper<target_type> target_h_type;
		typedef typename target_h_type::citer_type bi_iter_type;

		typedef typename regex_iterator_t_maker<bi_iter_type, regex_type>::type ret_type;

		return ret_type(target_h_type::begin(text), target_h_type::end(text), re, mth_flag);
	}

	//------------------------------grep---------------------------------------

	// size_type grep(handler, regex_iter);
	template<typename Handler, typename BiIter>
	static
	typename
		boost::enable_if
		<
			is_callable<Handler>,
			size_type
		>::type
		grep(const Handler& handler,
				boost::u32regex_iterator<BiIter> regex_iter)
	{
		typedef BiIter bi_iter_type;
		typedef boost::u32regex_iterator<bi_iter_type> regex_iter_type;

		size_type count = 0;

		for(regex_iter_type i = regex_iter, isize = regex_iter_type();
				(i != isize) && (0 != static_cast<int>(handler(*i)));
				++i, ++count);

		return count;
	}

	// size_typ grep(handler, pred, regex_iterator)
	template<typename Handler, typename Pred, typename BiIter>
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
				boost::u32regex_iterator<BiIter> regex_iter)
	{
		typedef BiIter bi_iter_type;
		typedef boost::u32regex_iterator<bi_iter_type> regex_iter_type;

		size_type count = 0;

		for(regex_iter_type i = regex_iter, isize = regex_iter_type();
				(i != isize) && pred(handler(*i));
				++i, ++count);

		return count;
	}

	// size_type grep(handler, text_s, text_e, re, mth_flag);
	template<typename Handler, typename BiIter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_bidirectional_iterator<BiIter>,
				is_callable<Handler>
			>,
			size_type
		>::type
		grep(const Handler& handler,
				BiIter text_s, BiIter text_e,
				const regex_type& re,
				match_flag_type mth_flag = boost::match_default)
	{
		typedef BiIter bi_iter_type;
		typedef detail::iterator_helper<BiIter> bi_iter_h_type;

		return this_type::grep(handler, this_type::make_regex_iterator(text_s, text_e, re, mth_flag));
	}

	// size_type grep(handler, pred, text_s, text_e, re, mth_flag);
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
				is_bidirectional_iterator<BiIter>
			>,
			size_type
		>::type
		grep(const Handler& handler, const Pred& pred,
				BiIter text_s, BiIter text_e,
				const regex_type& re,
				match_flag_type mth_flag = boost::match_default)
	{
		return this_type::grep(handler, pred, this_type::make_regex_iterator(text_s, text_e, re, mth_flag));
	}

	// size_type grep(handler, text, re, mth_flag)
	template<typename Handler, typename Target> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::is_generalized_string_t<Target>,
				is_callable<Handler>
			>,
			size_type
		>::type
		grep(const Handler& handler,
				const Target& text,
				const regex_type& re,
				match_flag_type mth_flag = boost::match_default)
	{
		return this_type::grep(handler, this_type::make_regex_iterator(text, re, mth_flag));
	}

	// size_type grep(handler, pred, text, re, mth_flag)
	template<typename Handler, typename Pred, typename Target> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::is_generalized_string_t<Target>,
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
				const regex_type& re,
				match_flag_type mth_flag = boost::match_default)
	{
		return this_type::grep(handler, pred, this_type::make_regex_iterator(text, re, mth_flag));
	}

	//----------------------------------make_regex_token_iterator---------------

	// regex_token_iterator make_regex_token_iterator(text_s, text_e, re, submatch, mth_flag)
	template<typename BiIter> inline
	static
	typename
		boost::enable_if
		<
			is_bidirectional_iterator<BiIter>,
			typename regex_token_iterator_t_maker<BiIter, regex_type>::type
		>::type
		make_regex_token_iterator(BiIter s, BiIter e,
									const regex_type& re,
									int submatch = 0,
									match_flag_type mth_flag = boost::match_default)
	{
		typedef BiIter bi_iter_type;
		typedef detail::iterator_helper<bi_iter_type> bi_iter_h_type;
		typedef typename regex_token_iterator_t_maker<bi_iter_type, regex_type>::type ret_type;

		return ret_type(bi_iter_h_type::org(s), bi_iter_h_type::org(e), re, submatch, mth_flag);
	}

	// regex_token_iterator make_regex_token_iterator(text, re, submatch, mth_flag)
	template<typename Target> inline
	static
	typename
		boost::enable_if
		<
			charset::is_generalized_string_t<Target>,
			typename
				regex_token_iterator_t_maker
				<
					typename detail::string_helper<Target>::citer_type,
					regex_type
				>::type
		>::type
		make_regex_token_iterator(const Target& text,
									const regex_type& re,
									int submatch = 0,
									match_flag_type mth_flag = boost::match_default)
	{
		typedef Target target_type;
		typedef detail::string_helper<target_type> target_h_type;
		typedef typename target_h_type::citer_type bi_iter_type;
		typedef typename regex_token_iterator_t_maker<bi_iter_type, regex_type>::type ret_type;

		return ret_type(target_h_type::begin(text), target_h_type::end(text), re, submatch, mth_flag);
	}

	// regex_token_iterator make_regex_token_iterator(text_s, text_e, re, submatch, mth_flag)
	template<typename BiIter, typename SubMatch> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_bidirectional_iterator<BiIter>,
				is_convertible_size_t<SubMatch>
			>,
			typename regex_token_iterator_t_maker<BiIter, regex_type>::type
		>::type
		make_regex_token_iterator(BiIter s, BiIter e,
									const regex_type& re,
									SubMatch submatch,
									match_flag_type mth_flag = boost::match_default)
	{
		typedef BiIter bi_iter_type;
		typedef detail::iterator_helper<bi_iter_type> bi_iter_h_type;
		typedef typename regex_token_iterator_t_maker<bi_iter_type, regex_type>::type ret_type;

		return ret_type(bi_iter_h_type::org(s), bi_iter_h_type::org(e), re, submatch, mth_flag);
	}

	// regex_token_iterator make_regex_token_iterator(text, re, submatch, mth_flag)
	template<typename Target, typename SubMatch> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::is_generalized_string_t<Target>,
				is_convertible_size_t<SubMatch>
			>,
			typename
				regex_token_iterator_t_maker
				<
					typename detail::string_helper<Target>::citer_type,
					regex_type
				>::type
		>::type
		make_regex_token_iterator(const Target& text,
									const regex_type& re,
									SubMatch submatch,
									match_flag_type mth_flag = boost::match_default)
	{
		typedef Target target_type;
		typedef detail::string_helper<target_type> target_h_type;
		typedef typename target_h_type::citer_type bi_iter_type;
		typedef typename regex_token_iterator_t_maker<bi_iter_type, regex_type>::type ret_type;

		return ret_type(target_h_type::begin(text), target_h_type::end(text), re, submatch, mth_flag);
	}

	// regex_token_iterator make_regex_token_iterator(text_s, text_e, re, submatch, mth_flag)
	template<typename BiIter> inline
	static
	typename
		boost::enable_if
		<
			is_bidirectional_iterator<BiIter>,
			typename regex_token_iterator_t_maker<BiIter, regex_type>::type
		>::type
		make_regex_token_iterator(BiIter s, BiIter e,
									const regex_type& re,
									const std::vector<int>& submatch,
									match_flag_type mth_flag = boost::match_default)
	{
		typedef BiIter bi_iter_type;
		typedef detail::iterator_helper<bi_iter_type> bi_iter_h_type;
		typedef typename regex_token_iterator_t_maker<bi_iter_type, regex_type>::type ret_type;

		return ret_type(bi_iter_h_type::org(s), bi_iter_h_type::org(e), re, submatch, mth_flag);
	}

	// regex_token_iterator make_regex_token_iterator(text, re, submatch, mth_flag)
	template<typename Target, typename Traits> inline
	static
	typename
		boost::enable_if
		<

			charset::is_generalized_string_t<Target>,
			typename
				regex_token_iterator_t_maker
				<
					typename detail::string_helper<Target>::citer_type,
					regex_type
				>::type
		>::type
		make_regex_token_iterator(const Target& text,
									const regex_type& re,
									const std::vector<int>& submatch,
									match_flag_type mth_flag = boost::match_default)
	{
		typedef Target target_type;
		typedef detail::string_helper<target_type> target_h_type;
		typedef typename target_h_type::citer_type bi_iter_type;
		typedef typename regex_token_iterator_t_maker<bi_iter_type, regex_type>::type ret_type;

		return ret_type(target_h_type::begin(text), target_h_type::end(text), re, submatch, mth_flag);
	}

	// regex_token_iterator make_regex_token_iterator(text_s, text_e, re, submatch, mth_flag)
	template<typename BiIter, std::size_t N> inline
	static
	typename
		boost::enable_if
		<
			is_bidirectional_iterator<BiIter>,
			typename regex_token_iterator_t_maker<BiIter, regex_type>::type
		>::type
		make_regex_token_iterator(BiIter s, BiIter e,
									const regex_type& re,
									const int(&submatch)[N],
									match_flag_type mth_flag = boost::match_default)
	{
		typedef BiIter bi_iter_type;
		typedef detail::iterator_helper<bi_iter_type> bi_iter_h_type;
		typedef typename regex_token_iterator_t_maker<bi_iter_type, regex_type>::type ret_type;

		return ret_type(bi_iter_h_type::org(s), bi_iter_h_type::org(e), re, submatch, mth_flag);
	}

	// regex_token_iterator make_regex_token_iterator(text, re, submatch, mth_flag)
	template<typename Target, std::size_t N> inline
	static
	typename
		boost::enable_if
		<
			charset::is_generalized_string_t<Target>,
			typename
				regex_token_iterator_t_maker
				<
					typename detail::string_helper<Target>::citer_type,
					regex_type
				>::type
		>::type
		make_regex_token_iterator(const Target& text,
									const regex_type& re,
									const int(&submatch)[N],
									match_flag_type mth_flag = boost::match_default)
	{
		typedef Target target_type;
		typedef detail::string_helper<target_type> target_h_type;
		typedef typename target_h_type::citer_type bi_iter_type;
		typedef typename regex_token_iterator_t_maker<bi_iter_type, regex_type>::type ret_type;

		return ret_type(target_h_type::begin(text), target_h_type::end(text), re, submatch, mth_flag);
	}

	// regex_token_iterator make_regex_token_iterator(text_s, text_e, re, submatch, mth_flag)
	template<typename BiIter, typename SubMatch> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_bidirectional_iterator<BiIter>,
				is_container<SubMatch>
			>,
			typename regex_token_iterator_t_maker<BiIter, regex_type>::type
		>::type
		make_regex_token_iterator(BiIter s, BiIter e,
									const regex_type& re,
									const SubMatch& submatch,
									match_flag_type mth_flag = boost::match_default)
	{
		typedef BiIter bi_iter_type;
		typedef detail::iterator_helper<bi_iter_type> bi_iter_h_type;
		typedef typename regex_token_iterator_t_maker<bi_iter_type, regex_type>::type ret_type;
		typedef std::vector<int> submatch_vt_type;

		submatch_vt_type vt(boost::begin(submatch), boost::end(submatch));
		return ret_type(bi_iter_h_type::org(s), bi_iter_h_type::org(e), re, vt, mth_flag);
	}

	// regex_token_iterator make_regex_token_iterator(text, re, submatch, mth_flag)
	template<typename Target, typename SubMatch> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::is_generalized_string_t<Target>,
				is_container<SubMatch>
			>,
			typename
				regex_token_iterator_t_maker
				<
					typename detail::string_helper<Target>::citer_type,
					regex_type
				>::type
		>::type
		make_regex_token_iterator(const Target& text,
									const regex_type& re,
									const SubMatch& submatch,
									match_flag_type mth_flag = boost::match_default)
	{
		typedef Target target_type;
		typedef detail::string_helper<target_type> target_h_type;
		typedef typename target_h_type::citer_type bi_iter_type;
		typedef typename regex_token_iterator_t_maker<bi_iter_type, regex_type>::type ret_type;
		typedef std::vector<int> submatch_vt_type;

		submatch_vt_type vt(boost::begin(submatch), boost::end(submatch));
		return ret_type(target_h_type::begin(text), target_h_type::end(text), re, vt, mth_flag);
	}

	//----------------------------------split--------------------------------

private:
	inline static const regex_type& default_split_regex(void)
	{
		static const regex_type re = this_type::make_regex("\\s+");
		return re;
	}

public:

	// size_type split(handler, token_iter)
	template<typename Handler, typename BiIter>
	static
	typename
		boost::enable_if
		<
			is_callable<Handler>,
			size_type
		>::type
		split(const Handler& handler,
				boost::u32regex_token_iterator<BiIter> token_iter)
	{
		typedef BiIter bi_iter_type;
		typedef boost::u32regex_token_iterator<BiIter> regex_token_iter_type;

		size_type count = 0;

		for(regex_token_iter_type i = token_iter, isize = regex_token_iter_type();
				(i != isize) && (0 != static_cast<int>(handler(*i)));
				++i, ++count);

		return count;
	}

	// size_type split(handler, pred, token_iter)
	template<typename Handler, typename Pred, typename BiIter>
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
				boost::u32regex_token_iterator<BiIter> token_iter)
	{
		typedef BiIter bi_iter_type;
		typedef boost::u32regex_token_iterator<BiIter> regex_token_iter_type;

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
				is_bidirectional_iterator<BiIter>
			>,
			size_type
		>::type
		split(const Handler& handler, BiIter text_s, BiIter text_e)
	{
		return
			this_type::split(
				handler,
				this_type::make_regex_token_iterator(
					text_s, text_e, this_type::default_split_regex(), -1));
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
				charset::is_generalized_string_t<Target>
			>,
			size_type
		>::type
		split(const Handler& handler, const Target& text)
	{
		return
			this_type::split(
				handler,
				this_type::make_regex_token_iterator(
					text, this_type::default_split_regex(), -1));
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
				is_bidirectional_iterator<BiIter>
			>,
			size_type
		>::type
		split(const Handler& handler, const Pred& pred, BiIter text_s, BiIter text_e)
	{
		return
			this_type::split(
				handler, pred,
				this_type::make_regex_token_iterator(
					text_s, text_e, this_type::default_split_regex(), -1));
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
				charset::is_generalized_string_t<Target>
			>,
			size_type
		>::type
		split(const Handler& handler, const Pred& pred, const Target& text)
	{
		return
			this_type::split(
				handler, pred,
				this_type::make_regex_token_iterator(
					text, this_type::default_split_regex(), -1));
	}

	// size_type split(handler, text_s, text_e, re, mth_flag)
	template<typename Handler, typename BiIter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_callable<Handler>,
				is_bidirectional_iterator<BiIter>
			>,
			size_type
		>::type
		split(const Handler& handler, BiIter text_s, BiIter text_e,
				const regex_type& re,
				match_flag_type mth_flag = boost::match_default)
	{
		return
			this_type::split(
				handler,
				this_type::make_regex_token_iterator(text_s, text_e, re, -1, mth_flag));
	}

	// size_type split(handler, text, re, mth_flag)
	template<typename Handler, typename Target> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_callable<Handler>,
				charset::is_generalized_string_t<Target>
			>,
			size_type
		>::type
		split(const Handler& handler, const Target& text,
				const regex_type& re,
				match_flag_type mth_flag = boost::match_default)
	{
		return
			this_type::split(
				handler,
				this_type::make_regex_token_iterator(text, re, -1, mth_flag));
	}

	// size_type split(handler, pred, text_s, text_e, re, mth_flag)
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
				is_bidirectional_iterator<BiIter>
			>,
			size_type
		>::type
		split(const Handler& handler, const Pred& pred,
				BiIter text_s, BiIter text_e,
				const regex_type& re,
				match_flag_type mth_flag = boost::match_default)
	{
		return
			this_type::split(
				handler, pred,
				this_type::make_regex_token_iterator(text_s, text_e, re, -1, mth_flag) );
	}

	// size_type split(handler, pred, text, re, mth_flag)
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
				charset::is_generalized_string_t<Target>
			>,
			size_type
		>::type
		split(const Handler& handler, const Pred& pred,
				const Target& text,
				const regex_type& re,
				match_flag_type mth_flag = boost::match_default)
	{
		return
			this_type::split(
				handler, pred,
				this_type::make_regex_token_iterator(text, re, -1, mth_flag) );
	}

	// size_type split(handler, text_s, text_e, re, submatch, mth_flag)
	template<typename Handler, typename BiIter, typename SubMatch> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_callable<Handler>,
				is_bidirectional_iterator<BiIter>,
				boost::mpl::or_
				<
					is_container<SubMatch>,
					is_convertible_size_t<SubMatch>
				>
			>,
			size_type
		>::type
		split(const Handler& handler, BiIter text_s, BiIter text_e,
				const regex_type& re,
				const SubMatch& submatch,
				match_flag_type mth_flag = boost::match_default)
	{
		return
			this_type::split(
				handler,
				this_type::make_regex_token_iterator(text_s, text_e, re, submatch, mth_flag));
	}

	// size_type split(handler, text, re, submatch, mth_flag);
	template<typename Handler, typename Target, typename SubMatch> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_callable<Handler>,
				charset::is_generalized_string_t<Target>,
				boost::mpl::or_
				<
					is_container<SubMatch>,
					is_convertible_size_t<SubMatch>
				>
			>,
			size_type
		>::type
		split(const Handler& handler, const Target& text,
				const regex_type& re,
				const SubMatch& submatch,
				match_flag_type mth_flag = boost::match_default)
	{
		return
			this_type::split(
				handler,
				this_type::make_regex_token_iterator(text, re, submatch, mth_flag));
	}

	// size_type split(handler, pred, text_s, text_e, re, submatch, mth_flag)
	template<typename Handler, typename Pred, typename BiIter, typename SubMatch> inline
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
				is_bidirectional_iterator<BiIter>,
				boost::mpl::or_
				<
					is_container<SubMatch>,
					is_convertible_size_t<SubMatch>
				>
			>,
			size_type
		>::type
		split(const Handler& handler, const Pred& pred,
				BiIter text_s, BiIter text_e,
				const regex_type& re,
				const SubMatch& submatch,
				match_flag_type mth_flag = boost::match_default)
	{
		return
			this_type::split(
				handler, pred,
				this_type::make_regex_token_iterator(text_s, text_e, re, submatch, mth_flag) );
	}

	// size_type split(handler, pred, text, re, submatch, mth_flag);
	template<typename Handler, typename Pred, typename Target, typename SubMatch> inline
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
				charset::is_generalized_string_t<Target>,
				boost::mpl::or_
				<
					is_container<SubMatch>,
					is_convertible_size_t<SubMatch>
				>
			>,
			size_type
		>::type
		split(const Handler& handler, const Pred& pred,
				const Target& text,
				const regex_type& re,
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

#endif // BOOST_HAS_ICU
#endif //__YGGR_REGULAR_PARSE_U32REGEX_PARSE_HPP__
