//fuzzy_maker.hpp

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

#ifndef __YGGR_FUZZY_FUZZY_MAKER_HPP__
#define __YGGR_FUZZY_FUZZY_MAKER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/nonable/noncreateable.hpp>
#include <yggr/container_ex/is_allocator.hpp>
#include <yggr/container_ex/allocator_t.hpp>
#include <yggr/regular_parse/regex_parse.hpp>

#ifdef BOOST_HAS_ICU
#	include <yggr/regular_parse/u32regex_parse.hpp>
#endif //BOOST_HAS_ICU

#ifdef BOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4624)
#endif // BOOST_MSVC

namespace yggr
{
namespace fuzzy
{

template<typename Config>
class fuzzy_maker
	: private nonable::noncreateable
{
public:
	typedef Config cfg_type;
	typedef boost::regex_error error_type;

private:
	typedef fuzzy_maker this_type;

public:
	template<typename Char> inline
	static const boost::basic_regex<Char>& s_regex(void)
	{
		typedef boost::basic_regex<Char> regex_type;
		static const regex_type re(regular_parse::regex_parse::make_regex(
									/*typename*/ cfg_type::template s_str_re<Char>()));
		return re;
	}

#ifdef BOOST_HAS_ICU

	inline static const regular_parse::u32regex_parse::regex_type& 
		s_u32regex(void)
	{
		typedef regular_parse::u32regex_parse::regex_type regex_type;
		static const regex_type re(regular_parse::u32regex_parse::make_regex(
									/*typename*/ cfg_type::template s_str_re<char>()));
		return re;
	}

#endif // BOOST_HAS_ICU

public:

	template<typename BiIter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_bidirectional_iterator<BiIter>,
				charset::is_not_any_utf8_string_iterator_t<BiIter>
			>,
			typename regular_parse::detail::iterator_helper<BiIter>::result_cont_type
		>::type
		make_string(BiIter s, BiIter e)
	{
		typedef BiIter bi_iter_type;
		typedef regular_parse::detail::iterator_helper<bi_iter_type> iter_h_type;
		typedef typename iter_h_type::value_type char_type;
		typedef typename iter_h_type::result_cont_type ret_type;

		return 
			regular_parse::regex_parse::replace<ret_type>(
				s, e, 
				this_type::s_regex<char_type>(), 
				/*typename*/ cfg_type::template s_fmt<char_type>());
	}

	template<typename BiIter, typename Alloc> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_bidirectional_iterator<BiIter>,
				charset::is_not_any_utf8_string_iterator_t<BiIter>,
				container_ex::is_allocator<Alloc>
			>,
			typename
				boost::mpl::if_
				<
					boost::is_same
					<
						typename 
							container_ex::allocator_t
							<
								typename regular_parse::detail::iterator_helper<BiIter>::result_cont_type
							>::type,
						Alloc
					>,
					typename regular_parse::detail::iterator_helper<BiIter>::result_cont_type,
					typename
						mplex::typename_replaceor
						<
							typename regular_parse::detail::iterator_helper<BiIter>::result_cont_type,
							mplex::_keep, mplex::_keep, Alloc
						>::type
				>::type
		>::type
		make_string(const Alloc& alloc, BiIter s, BiIter e)
	{
		typedef BiIter bi_iter_type;
		typedef regular_parse::detail::iterator_helper<bi_iter_type> iter_h_type;
		typedef typename iter_h_type::value_type char_type;
		typedef typename iter_h_type::result_cont_type result_cont_type;

		typedef typename
			boost::mpl::if_
			<
				boost::is_same
				<
					typename 
						container_ex::allocator_t
						<
							typename regular_parse::detail::iterator_helper<BiIter>::result_cont_type
						>::type,
					Alloc
				>,
				typename regular_parse::detail::iterator_helper<BiIter>::result_cont_type,
				typename
					mplex::typename_replaceor
					<
						typename regular_parse::detail::iterator_helper<BiIter>::result_cont_type,
						mplex::_keep, mplex::_keep, Alloc
					>::type
			>::type ret_type;

		return 
			regular_parse::regex_parse::replace<ret_type>(
				alloc, s, e, 
				this_type::s_regex<char_type>(), 
				/*typename*/ cfg_type::template s_fmt<char_type>());
	}

	template<typename String, typename BiIter> inline
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
				>
			>,
			String
		>::type
		make_string(BiIter s, BiIter e)
	{
		typedef BiIter bi_iter_type;
		typedef regular_parse::detail::iterator_helper<bi_iter_type> iter_h_type;
		typedef typename iter_h_type::value_type char_type;
		typedef String ret_type;

		return 
			regular_parse::regex_parse::replace<ret_type>(
				s, e, 
				this_type::s_regex<char_type>(), 
				/*typename*/ cfg_type::template s_fmt<char_type>());
	}

	template<typename String, typename Alloc, typename BiIter> inline
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
				>
			>,
			String
		>::type
		make_string(const Alloc& alloc, BiIter s, BiIter e)
	{
		typedef BiIter bi_iter_type;
		typedef regular_parse::detail::iterator_helper<bi_iter_type> iter_h_type;
		typedef typename iter_h_type::value_type char_type;
		typedef String ret_type;
		return 
			regular_parse::regex_parse::replace<ret_type>(
				alloc, s, e, 
				this_type::s_regex<char_type>(), 
				/*typename*/ cfg_type::template s_fmt<char_type>());
	}

	template<typename Target> inline
	static 
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::is_generalized_string_t<Target>,
				charset::is_not_utf8_string_t<Target>
			>,
			charset::basic_string<typename regular_parse::detail::string_helper<Target>::value_type>
		>::type
		make_string(const Target& str)
	{
		typedef Target target_type;
		typedef regular_parse::detail::string_helper<target_type> target_h_type;
		typedef typename target_h_type::value_type char_type;
		typedef charset::basic_string<char_type> ret_type;

		return 
			regular_parse::regex_parse::replace<ret_type>(
				str, 
				this_type::s_regex<char_type>(), 
				/*typename*/ cfg_type::template s_fmt<char_type>());
	}

	template<typename Target, typename Alloc> inline
	static 
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::is_generalized_string_t<Target>,
				charset::is_not_utf8_string_t<Target>,
				container_ex::is_allocator<Alloc>
			>,
			typename
				boost::mpl::if_
				<
					boost::is_same
					<
						typename 
							container_ex::allocator_t
							<
								charset::basic_string<typename regular_parse::detail::string_helper<Target>::value_type>
							>::type,
						Alloc
					>,
					charset::basic_string<typename regular_parse::detail::string_helper<Target>::value_type>,
					typename
						mplex::typename_replaceor
						<
							charset::basic_string<typename regular_parse::detail::string_helper<Target>::value_type>,
							mplex::_keep, mplex::_keep, Alloc
						>::type
				>::type
		>::type
		make_string(const Alloc& alloc, const Target& str)
	{
		typedef Target target_type;
		typedef regular_parse::detail::string_helper<target_type> target_h_type;
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
							charset::basic_string<typename regular_parse::detail::string_helper<Target>::value_type>
						>::type,
					Alloc
				>,
				charset::basic_string<typename regular_parse::detail::string_helper<Target>::value_type>,
				typename
					mplex::typename_replaceor
					<
						charset::basic_string<typename regular_parse::detail::string_helper<Target>::value_type>,
						mplex::_keep, mplex::_keep, Alloc
					>::type
			>::type ret_type;

		return 
			regular_parse::regex_parse::replace<ret_type>(
				alloc, str, 
				this_type::s_regex<char_type>(), 
				/*typename*/ cfg_type::template s_fmt<char_type>());
	}

	template<typename String, typename Target> inline
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
					boost::mpl::not_< boost::is_same<String, Target> >,
					charset::is_generalized_string_t<Target>,
					charset::is_not_utf8_string_t<Target>
				>
			>,
			String
		>::type
		make_string(const Target& str)
	{
		typedef Target target_type;
		typedef regular_parse::detail::string_helper<target_type> target_h_type;
		typedef typename target_h_type::value_type char_type;
		typedef String ret_type;

		return 
			regular_parse::regex_parse::replace<ret_type>(
				str, 
				this_type::s_regex<char_type>(), 
				/*typename*/ cfg_type::template s_fmt<char_type>());
	}

	template<typename String, typename Alloc, typename Target> inline
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
					boost::mpl::not_< boost::is_same<String, Target> >,
					charset::is_generalized_string_t<Target>,
					charset::is_not_utf8_string_t<Target>
				>
			>,
			String
		>::type
		make_string(const Alloc& alloc, const Target& str)
	{
		typedef Target target_type;
		typedef regular_parse::detail::string_helper<target_type> target_h_type;
		typedef typename target_h_type::value_type char_type;
		typedef String ret_type;

		return 
			regular_parse::regex_parse::replace<ret_type>(
				alloc, str, 
				this_type::s_regex<char_type>(), 
				/*typename*/ cfg_type::template s_fmt<char_type>());
	}

	//make_regex
	template<typename BiIter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_bidirectional_iterator<BiIter>,
				charset::is_not_any_utf8_string_iterator_t<BiIter>
			>,
			boost::basic_regex<typename regular_parse::detail::iterator_helper<BiIter>::value_type>
		>::type
		make_regex(BiIter s, BiIter e)
	{
		return 
			regular_parse::regex_parse::make_regex(
				this_type::make_string(s, e));
	}

	template<typename Target> inline
	static 
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::is_generalized_string_t<Target>,
				charset::is_not_utf8_string_t<Target>
			>,
			boost::basic_regex<typename regular_parse::detail::string_helper<Target>::value_type>
		>::type
		make_regex(const Target& str)
	{
		return 
			regular_parse::regex_parse::make_regex(
				this_type::make_string(str));
	}

#ifdef BOOST_HAS_ICU

	template<typename BiIter> inline
	static
	typename
		boost::enable_if
		<
			is_bidirectional_iterator<BiIter>,
			charset::utf8_string
		>::type
		make_u32_string(BiIter s, BiIter e)
	{
		typedef BiIter bi_iter_type;
		typedef regular_parse::detail::iterator_helper<bi_iter_type> iter_h_type;
		typedef typename iter_h_type::value_type char_type;
		typedef charset::utf8_string ret_type;
		return 
			regular_parse::u32regex_parse::replace<ret_type>(
				s, e, 
				this_type::s_u32regex(), 
				/*typename*/ cfg_type::template s_fmt<char_type>());
	}

	template<typename BiIter, typename Alloc> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_bidirectional_iterator<BiIter>,
				container_ex::is_allocator<Alloc>
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
		make_u32_string(const Alloc& alloc, BiIter s, BiIter e)
	{
		typedef BiIter bi_iter_type;
		typedef regular_parse::detail::iterator_helper<bi_iter_type> iter_h_type;
		typedef typename iter_h_type::value_type char_type;
		typedef typename charset::utf8_string result_cont_type;

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

		return 
			regular_parse::u32regex_parse::replace<ret_type>(
				alloc, s, e, 
				this_type::s_u32regex(), 
				/*typename*/ cfg_type::template s_fmt<char_type>());
	}

	template<typename String, typename BiIter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				container_ex::is_container<String>,
				is_bidirectional_iterator<BiIter>
			>,
			String
		>::type
		make_u32_string(BiIter s, BiIter e)
	{
		typedef BiIter bi_iter_type;
		typedef regular_parse::detail::iterator_helper<bi_iter_type> iter_h_type;
		typedef typename iter_h_type::value_type char_type;
		typedef String ret_type;
		return 
			regular_parse::u32regex_parse::replace<ret_type>(
				s, e, 
				this_type::s_u32regex(), 
				/*typename*/ cfg_type::template s_fmt<char_type>());
	}

	template<typename String, typename Alloc, typename BiIter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::and_
				<
					container_ex::is_container<String>,
					container_ex::is_allocator<Alloc>
				>,
				is_bidirectional_iterator<BiIter>
			>,
			String
		>::type
		make_u32_string(const Alloc& alloc, BiIter s, BiIter e)
	{
		typedef BiIter bi_iter_type;
		typedef regular_parse::detail::iterator_helper<bi_iter_type> iter_h_type;
		typedef typename iter_h_type::value_type char_type;
		typedef String ret_type;
		return 
			regular_parse::u32regex_parse::replace<ret_type>(
				alloc, s, e, 
				this_type::s_u32regex(), 
				/*typename*/ cfg_type::template s_fmt<char_type>());
	}

	template<typename Target> inline
	static 
	typename 
		boost::enable_if
		<
			charset::is_generalized_string_t<Target>,
			charset::utf8_string
		>::type
		make_u32_string(const Target& str)
	{
		typedef Target target_type;
		typedef regular_parse::detail::string_helper<target_type> target_h_type;
		typedef typename target_h_type::value_type char_type;
		typedef charset::utf8_string ret_type;

		return 
			regular_parse::u32regex_parse::replace<ret_type>(
				str, 
				this_type::s_u32regex(), 
				/*typename*/ cfg_type::template s_fmt<char_type>());
	}

	template<typename Target, typename Alloc> inline
	static 
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::is_generalized_string_t<Target>,
				container_ex::is_allocator<Alloc>
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
		make_u32_string(const Alloc& alloc, const Target& str)
	{
		typedef Target target_type;
		typedef regular_parse::detail::string_helper<target_type> target_h_type;
		typedef typename target_h_type::value_type char_type;
		typedef charset::utf8_string result_cont_type;

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

		return 
			regular_parse::u32regex_parse::replace<ret_type>(
				alloc, str, 
				this_type::s_u32regex(), 
				/*typename*/ cfg_type::template s_fmt<char_type>());
	}

	template<typename String, typename Target> inline
	static 
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				container_ex::is_container<String>,
				boost::mpl::and_
				<
					boost::mpl::not_< boost::is_same<String, Target> >,
					charset::is_generalized_string_t<Target>
				>
			>,
			String
		>::type
		make_u32_string(const Target& str)
	{
		typedef Target target_type;
		typedef regular_parse::detail::string_helper<target_type> target_h_type;
		typedef typename target_h_type::value_type char_type;
		typedef String ret_type;

		return 
			regular_parse::u32regex_parse::replace<ret_type>(
				str, 
				this_type::s_u32regex(), 
				/*typename*/ cfg_type::template s_fmt<char_type>());
	}

	template<typename String, typename Alloc, typename Target> inline
	static 
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::and_
				<
					container_ex::is_container<String>,
					container_ex::is_allocator<Alloc>
				>,
				boost::mpl::and_
				<
					boost::mpl::not_< boost::is_same<String, Target> >,
					charset::is_generalized_string_t<Target>
				>
			>,
			String
		>::type
		make_u32_string(const Alloc& alloc, const Target& str)
	{
		typedef Target target_type;
		typedef regular_parse::detail::string_helper<target_type> target_h_type;
		typedef typename target_h_type::value_type char_type;
		typedef String ret_type;

		return 
			regular_parse::u32regex_parse::replace<ret_type>(
				alloc, str, 
				this_type::s_u32regex(), 
				/*typename*/ cfg_type::template s_fmt<char_type>());
	}

	// make_u32regex
	template<typename BiIter> inline
	static
	typename
		boost::enable_if
		<
			is_bidirectional_iterator<BiIter>,
			regular_parse::u32regex_parse::regex_type
		>::type
		make_u32regex(BiIter s, BiIter e)
	{
		return 
			regular_parse::u32regex_parse::make_regex(
				this_type::make_u32_string(s, e));
	}

	template<typename Target> inline
	static 
	typename 
		boost::enable_if
		<
			charset::is_generalized_string_t<Target>,
			regular_parse::u32regex_parse::regex_type
		>::type
		make_u32regex(const Target& str)
	{
		return 
			regular_parse::u32regex_parse::make_regex(
				this_type::make_u32_string(str));
	}

#endif // BOOST_HAS_ICU

};

} // namespace fuzzy
} // namespace yggr

#ifdef BOOST_MSVC
#	pragma warning(pop)
#endif // BOOST_MSVC

#endif //__YGGR_FUZZY_FUZZY_MAKER_HPP__
