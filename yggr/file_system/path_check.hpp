//path_check.hpp

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

#ifndef __YGGR_FILE_SYSTEM_FILE_CHECK_HPP__
#define __YGGR_FILE_SYSTEM_FILE_CHECK_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/nonable/noncreateable.hpp>

#include <yggr/iterator_ex/is_reverse_iterator.hpp>
#include <yggr/iterator_ex/iterator.hpp>

#include <yggr/container_ex/allocator_t.hpp>
#include <yggr/container_ex/is_allocator.hpp>

#include <yggr/container/clear.hpp>
#include <yggr/container/reserve.hpp>

#include <yggr/tuple/tuple.hpp>

#include <yggr/charset/const_strings.hpp>
#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/utf8_string_view.hpp>

#include <yggr/regular_parse/regex_parse.hpp>
#include <yggr/regular_parse/u32regex_parse.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/range/functions.hpp>
#include <boost/type_traits/is_base_of.hpp>

#include <utility>
#include <algorithm>
#include <cassert>

#if defined(_DEBUG)
#include <iostream>
#endif // _DEBUG


/*
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!! patch match																																		!!!
!!! win:	^([a-zA-Z]:|[^*?/\\\"<>|:\x00-\x19\x7f]+)?([/\\](?![*?/\\\"<>|:\x00-\x09\x0b-\x0c\x0e-\x19\x7f])|[/\\][^*?/\\\"<>|:\x00-\x19\x7f]+)*$	!!!
!!! other:	^([^/\x00-\x19\x7f])*([/](?![/\x00-\x09\x0b-\x0c\x0e-\x19\x7f])|[/][^/\x00-\x19\x7f]+)*$												!!!
!!!																																					!!!
!!! fname match																																		!!!
!!! win:	^([a-zA-Z]:|[^*?/\\\"<>|:\x00-\x19\x7f]+){0,1}([/\\][^*?/\\\"<>|:\x00-\x19\x7f]+)*$														!!!
!!! other:	^([^/\x00-\x19\x7f])*([/][^/\x00-\x19\x7f]+)*$																							!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/

/*

"xxx/xxx/xxxxxxx"	check_file_name : true, check_path_name true
"xxx/xxx/xxx/"		check_file_name : false, check_path_name true

*/

#if !defined(YGGR_FILE_PATH_REGEX_STRING)
#	if defined(YGGR_AT_WINDOWS)
#		if !defined(YGGR_FILE_PATH_USE_BACK_SLANT_ONLY)
#			define YGGR_FILE_PATH_REGEX_STRING() \
	"^([a-zA-Z]:|[^*?/\\\\\\\"<>|:\\x00-\\x19\\x7f]+)?([/\\\\](?![*?/\\\\\\\"<>|:\\x00-\\x09\\x0b-\\x0c\\x0e-\\x19\\x7f])|[/\\\\][^*?/\\\\\\\"<>|:\\x00-\\x19\\x7f]+)*$"
#		else
#			define YGGR_FILE_PATH_REGEX_STRING() \
	"^([a-zA-Z]:|[^*?/\\\\\\\"<>|:\\x00-\\x19\\x7f]+)?([\\\\](?![*?/\\\\\\\"<>|:\\x00-\\x09\\x0b-\\x0c\\x0e-\\x19\\x7f])|[\\\\][^*?/\\\\\\\"<>|:\\x00-\\x19\\x7f]+)*$"
#		endif //YGGR_FILE_PATH_USE_BACK_SLANT_ONLY
#	else
#		define YGGR_FILE_PATH_REGEX_STRING() \
	"^([^/\\x00-\\x19\\x7f])*([/](?![/\\x00-\\x09\\x0b-\\x0c\\x0e-\\x19\\x7f])|[/][^/\\x00-\\x19\\x7f]+)*$"
#	endif // YGGR_AT_WINDOWS
#endif //YGGR_FILE_PATH_REGEX_STRING

#if !defined(YGGR_FILE_NAME_REGEX_STRING)
#	if defined(YGGR_AT_WINDOWS)
#		if !defined(YGGR_FILE_PATH_USE_BACK_SLANT_ONLY)
#			define YGGR_FILE_NAME_REGEX_STRING() \
	"^([a-zA-Z]:|[^*?/\\\\\\\"<>|:\\x00-\\x19\\x7f]+)?([/\\\\][^*?/\\\\\\\"<>|:\\x00-\\x19\\x7f]+)*$"
#		else
#			define YGGR_FILE_NAME_REGEX_STRING() \
	"^([a-zA-Z]:|[^*?/\\\\\\\"<>|:\\x00-\\x19\\x7f]+)?([\\\\][^*?/\\\\\\\"<>|:\\x00-\\x19\\x7f]+)*$"
#		endif //YGGR_FILE_PATH_USE_BACK_SLANT_ONLY
#	else
#		define YGGR_FILE_NAME_REGEX_STRING() \
	"^([^/\\x00-\\x19\\x7f])*([/][^/\\x00-\\x19\\x7f]+)*$"
#	endif // YGGR_AT_WINDOWS
#endif //YGGR_FILE_PATH_REGEX_STRING


namespace yggr
{
namespace file_system
{

namespace detail
{

template<typename Data>
struct split_data_checker
	: public container_ex::is_container<Data>
{
};

template<typename Iter>
struct split_data_checker< std::pair<Iter, Iter> >
	: public boost::mpl::false_
{
	BOOST_MPL_ASSERT((is_iterator<Iter>));
};

template<typename TData, typename TIter,
			bool is_container = split_data_checker<TData>::value>
struct split_data_creator;

template<typename TData, typename TIter>
struct split_data_creator<TData, TIter, false>
{
	typedef TData data_type;
	typedef TIter iter_type;
	typedef regular_parse::detail::iterator_helper<iter_type> iter_h_type;

	split_data_creator(iter_type i)
		: _mark_iter(i)
	{
	}

	~split_data_creator(void)
	{
	}

public:
	inline data_type operator()(iter_type s, iter_type e) const
	{
		return data_type(s, e);
	}

	template<typename Alloc> inline
	data_type operator()(iter_type s, iter_type e, const Alloc&) const
	{
		return data_type(s, e);
	}

	template<typename Iter> inline
	data_type operator()(Iter s, Iter e) const
	{
		return
			data_type(
				iter_h_type::make_iterator(regular_parse::extract_to_base_iterator(s), _mark_iter),
				iter_h_type::make_iterator(regular_parse::extract_to_base_iterator(e), _mark_iter) );
	}

	template<typename Iter, typename Alloc> inline
	data_type operator()(Iter s, Iter e, const Alloc&) const
	{
		return
			data_type(
				iter_h_type::make_iterator(regular_parse::extract_to_base_iterator(s), _mark_iter),
				iter_h_type::make_iterator(regular_parse::extract_to_base_iterator(e), _mark_iter) );
	}

protected:
	iter_type _mark_iter;
};

// alloc_helper
template<typename TData, typename TIter, typename TAlloc,
			bool alloc_enable = container_ex::is_allocator<TAlloc>::value>
struct alloc_helper;

template<typename TData, typename TIter, typename TAlloc>
struct alloc_helper<TData, TIter, TAlloc, true>
{
public:
	typedef TData data_type;
	typedef TIter iter_type;
	typedef TAlloc alloc_type;

	typedef regular_parse::detail::iterator_helper<iter_type> iter_h_type;
	typedef typename iter_h_type::org_iter_type src_iter_type;

	typedef regular_parse::detail::iterator_helper<src_iter_type> src_iter_h_type;
	typedef typename src_iter_h_type::value_type src_char_type;

	typedef regular_parse::detail::string_helper<data_type> data_h_type;
	typedef typename data_h_type::value_type dst_char_type;
	typedef typename data_h_type::base_cont_type base_data_type;

	typedef regular_parse::detail::string_helper<base_data_type> base_data_h_type;

	typedef
		regular_parse::detail::conv_iterator_helper
		<
			src_iter_type,
			dst_char_type,
			src_char_type
		>conv_iter_h_type;

	typedef typename conv_iter_h_type::result_type conv_iter_pair_type;

public:
	inline data_type operator()(iter_type s, iter_type e, const alloc_type& alloc) const
	{
		conv_iter_h_type conv_h;
		conv_iter_pair_type rst_pair = conv_h(iter_h_type::org(s), iter_h_type::org(e));

		data_type ret(alloc);
		base_data_type& base_ret = data_h_type::org(ret);
		std::copy(rst_pair.first, rst_pair.second, iterator_ex::inserter(base_ret, base_data_h_type::end(base_ret)));
		data_h_type::recount(ret);
		return ret;
	}

	template<typename Iter> inline
	data_type operator()(Iter s, Iter e, const alloc_type& alloc) const
	{
		conv_iter_h_type conv_h;
		conv_iter_pair_type rst_pair =
			conv_h(regular_parse::extract_to_base_iterator(s),
					regular_parse::extract_to_base_iterator(e));

		data_type ret(alloc);
		base_data_type& base_ret = data_h_type::org(ret);
		std::copy(rst_pair.first, rst_pair.second, iterator_ex::inserter(base_ret, base_data_h_type::end(base_ret)));
		data_h_type::recount(ret);
		return ret;
	}
};

template<typename TData, typename TIter, typename TAlloc>
struct alloc_helper<TData, TIter, TAlloc, false>
{
public:
	typedef TData data_type;
	typedef TIter iter_type;
	typedef mplex::null_type alloc_type;

	typedef regular_parse::detail::iterator_helper<iter_type> iter_h_type;
	typedef typename iter_h_type::org_iter_type src_iter_type;

	typedef regular_parse::detail::iterator_helper<src_iter_type> src_iter_h_type;
	typedef typename src_iter_h_type::value_type src_char_type;

	typedef regular_parse::detail::string_helper<data_type> data_h_type;
	typedef typename data_h_type::value_type dst_char_type;
	typedef typename data_h_type::base_cont_type base_data_type;

	typedef regular_parse::detail::string_helper<base_data_type> base_data_h_type;

	typedef
		regular_parse::detail::conv_iterator_helper
		<
			src_iter_type,
			dst_char_type,
			src_char_type
		>conv_iter_h_type;

	typedef typename conv_iter_h_type::result_type conv_iter_pair_type;

public:
	template<typename Alloc> inline
	data_type operator()(iter_type s, iter_type e, const Alloc&) const
	{
		conv_iter_h_type conv_h;
		conv_iter_pair_type rst_pair = conv_h(iter_h_type::org(s), iter_h_type::org(e));

		data_type ret;
		base_data_type& base_ret = data_h_type::org(ret);
		std::copy(rst_pair.first, rst_pair.second, iterator_ex::inserter(base_ret, base_data_h_type::end(base_ret)));
		data_h_type::recount(ret);
		return ret;
	}

	template<typename Iter, typename Alloc> inline
	data_type operator()(Iter s, Iter e, const Alloc&) const
	{
		conv_iter_h_type conv_h;
		conv_iter_pair_type rst_pair =
			conv_h(regular_parse::extract_to_base_iterator(s),
					regular_parse::extract_to_base_iterator(e));

		data_type ret;
		base_data_type& base_ret = data_h_type::org(ret);
		std::copy(rst_pair.first, rst_pair.second, iterator_ex::inserter(base_ret, base_data_h_type::end(base_ret)));
		data_h_type::recount(ret);
		return ret;
	}
};

template<typename TData, typename TIter>
struct split_data_creator<TData, TIter, true>
{
public:
	typedef TData data_type;
	typedef TIter iter_type;

public:
	template<typename MarkIter>
	split_data_creator(MarkIter iter)
	{
	}

	~split_data_creator(void)
	{
	}

public:
	template<typename Iter> inline
	data_type operator()(Iter s, Iter e) const
	{
		typedef alloc_helper<data_type, iter_type, mplex::null_type> alloc_h_type;

		alloc_h_type h;
		mplex::null_type null_obj;
		return h(s, e, null_obj);
	}

	// don't check allocator
	template<typename Iter, typename Alloc> inline
	data_type operator()(Iter s, Iter e, const Alloc& alloc) const
	{
		typedef alloc_helper<data_type, iter_type, Alloc> alloc_h_type;

		alloc_h_type h;
		return h(s, e, alloc);
	}
};

} // namespace detail

class path_check
	: private nonable::noncreateable
{
private:
	typedef path_check this_type;

protected:
	template<typename Char> inline
	static const Char* pro_s_str_regex_of_fpath(void)
	{
		YGGR_PP_CONST_STRING_LOCAL_DEF(str_re, YGGR_FILE_PATH_REGEX_STRING());
		return YGGR_PP_CONST_STRING_LOCAL_GET(str_re, Char);
	}

	template<typename Char> inline
	static const Char* pro_s_str_regex_of_fname(void)
	{
		YGGR_PP_CONST_STRING_LOCAL_DEF(str_re, YGGR_FILE_NAME_REGEX_STRING());
		return YGGR_PP_CONST_STRING_LOCAL_GET(str_re, Char);
	}

protected:
	template<typename Char> inline
	static const boost::basic_regex<Char>&
		pro_s_match_regex_of_fpath(void)
	{
		typedef boost::basic_regex<Char> regex_type;
		static const regex_type re =
			regular_parse::regex_parse::make_regex(this_type::pro_s_str_regex_of_fpath<Char>());
		return re;
	}

	template<typename Char> inline
	static const boost::basic_regex<Char>&
		pro_s_match_regex_of_fname(void)
	{
		typedef boost::basic_regex<Char> regex_type;
		static const regex_type re =
			regular_parse::regex_parse::make_regex(this_type::pro_s_str_regex_of_fname<Char>());
		return re;
	}

	template<typename Char> inline
	static const boost::basic_regex<Char>&
		pro_s_search_regex_of_fpath(void)
	{
		typedef boost::basic_regex<Char> regex_type;
		static const regex_type re =
			regular_parse::regex_parse::make_regex(
				this_type::pro_s_str_regex_of_fpath<Char>() + 1,
				this_type::pro_s_str_regex_of_fpath<Char>()
					+ charset::charset_base_foo::strlen(this_type::pro_s_str_regex_of_fpath<Char>()) - 1);
		return re;
	}

	template<typename Char> inline
	static const boost::basic_regex<Char>&
		pro_s_search_regex_of_fname(void)
	{
		typedef boost::basic_regex<Char> regex_type;
		static const regex_type re =
			regular_parse::regex_parse::make_regex(
				this_type::pro_s_str_regex_of_fname<Char>() + 1,
				this_type::pro_s_str_regex_of_fname<Char>()
					+ (charset::charset_base_foo::strlen(this_type::pro_s_str_regex_of_fname<Char>()) - 1));
		return re;
	}

#if defined(BOOST_HAS_ICU)

	inline static const regular_parse::u32regex_parse::regex_type&
		pro_s_match_u32regex_of_fpath(void)
	{
		typedef regular_parse::u32regex_parse::regex_type regex_type;
		static const regex_type re =
			regular_parse::u32regex_parse::make_regex(this_type::pro_s_str_regex_of_fpath<char>());
		return re;
	}

	inline static const regular_parse::u32regex_parse::regex_type&
		pro_s_match_u32regex_of_fname(void)
	{
		typedef regular_parse::u32regex_parse::regex_type regex_type;
		static const regex_type re =
			regular_parse::u32regex_parse::make_regex(this_type::pro_s_str_regex_of_fname<char>());
		return re;
	}

	inline static const regular_parse::u32regex_parse::regex_type&
		pro_s_search_u32regex_of_fpath(void)
	{
		typedef regular_parse::u32regex_parse::regex_type regex_type;
		static const regex_type re =
			regular_parse::u32regex_parse::make_regex(
				this_type::pro_s_str_regex_of_fpath<char>() + 1,
				this_type::pro_s_str_regex_of_fpath<char>()
					+ charset::charset_base_foo::strlen(this_type::pro_s_str_regex_of_fpath<char>()) - 1);
		return re;
	}

	inline static const regular_parse::u32regex_parse::regex_type&
		pro_s_search_u32regex_of_fname(void)
	{
		typedef regular_parse::u32regex_parse::regex_type regex_type;
		static const regex_type re =
			regular_parse::u32regex_parse::make_regex(
				this_type::pro_s_str_regex_of_fname<char>() + 1,
				this_type::pro_s_str_regex_of_fname<char>()
					+ charset::charset_base_foo::strlen(this_type::pro_s_str_regex_of_fname<char>()) - 1);
		return re;
	}

#endif // BOOST_HAS_ICU

public:

#if defined(BOOST_HAS_ICU)

	// check_file_name
	template<typename Iter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<Iter>,
				charset::is_not_any_utf8_string_reverse_iterator_t<Iter>
			>,
			bool
		>::type
		check_file_name(Iter s, Iter e)
	{
		try
		{
			return regular_parse::u32regex_parse::match(s, e, this_type::pro_s_match_u32regex_of_fname());
		}
#if defined(_DEBUG)
		catch(const std::runtime_error& e)
		{
			std::cerr << e.what() << std::endl;
			return false;
		}
#endif // _DEBUG
		catch(...)
		{
			return false;
		}
	}

	template<typename Iter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<Iter>,
				charset::is_not_any_utf8_string_reverse_iterator_t<Iter>
			>,
			bool
		>::type
		check_file_name(Iter s, Iter e,
							typename regular_parse::match_results_t_maker<Iter>::type& m)
	{
		try
		{
			return regular_parse::u32regex_parse::match(s, e, m, this_type::pro_s_match_u32regex_of_fname());
		}
#if defined(_DEBUG)
		catch(const std::runtime_error& e)
		{
			std::cerr << e.what() << std::endl;
			return false;
		}
#endif // _DEBUG
		catch(...)
		{
			return false;
		}
	}

	template<typename Target> inline
	static
	typename
		boost::enable_if
		<
			charset::is_generalized_string_t<Target>,
			bool
		>::type
		check_file_name(const Target& text)
	{
		try
		{
			return regular_parse::u32regex_parse::match(text, this_type::pro_s_match_u32regex_of_fname());
		}
#if defined(_DEBUG)
		catch(const std::runtime_error& e)
		{
			std::cerr << e.what() << std::endl;
			return false;
		}
#endif // _DEBUG
		catch(...)
		{
			return false;
		}
	}

	template<typename Target> inline
	static
	typename
		boost::enable_if
		<
			charset::is_generalized_string_t<Target>,
			bool
		>::type
		check_file_name(const Target& text,
						typename
							regular_parse::match_results_t_maker
							<
								typename regular_parse::detail::string_helper<Target>::citer_type
							>::type& m)
	{
		try
		{
			return regular_parse::u32regex_parse::match(text, m, this_type::pro_s_match_u32regex_of_fname());
		}
#if defined(_DEBUG)
		catch(const std::runtime_error& e)
		{
			std::cerr << e.what() << std::endl;
			return false;
		}
#endif // _DEBUG
		catch(...)
		{
			return false;
		}
	}

	// check_path_name
	template<typename Iter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<Iter>,
				charset::is_not_any_utf8_string_reverse_iterator_t<Iter>
			>,
			bool
		>::type
		check_path_name(Iter s, Iter e)
	{
		try
		{
			return regular_parse::u32regex_parse::match(s, e, this_type::pro_s_match_u32regex_of_fpath());
		}
#if defined(_DEBUG)
		catch(const std::runtime_error& e)
		{
			std::cerr << e.what() << std::endl;
			return false;
		}
#endif // _DEBUG
		catch(...)
		{
			return false;
		}
	}

	template<typename Iter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<Iter>,
				charset::is_not_any_utf8_string_reverse_iterator_t<Iter>
			>,
			bool
		>::type
		check_path_name(Iter s, Iter e,
							typename regular_parse::match_results_t_maker<Iter>::type& m)
	{
		try
		{
			return regular_parse::u32regex_parse::match(s, e, m, this_type::pro_s_match_u32regex_of_fpath());
		}
#if defined(_DEBUG)
		catch(const std::runtime_error& e)
		{
			std::cerr << e.what() << std::endl;
			return false;
		}
#endif // _DEBUG
		catch(...)
		{
			return false;
		}
	}

	template<typename Target> inline
	static
	typename
		boost::enable_if
		<
			charset::is_generalized_string_t<Target>,
			bool
		>::type
		check_path_name(const Target& text)
	{
		try
		{
			return regular_parse::u32regex_parse::match(text, this_type::pro_s_match_u32regex_of_fpath());
		}
#if defined(_DEBUG)
		catch(const std::runtime_error& e)
		{
			std::cerr << e.what() << std::endl;
			return false;
		}
#endif // _DEBUG
		catch(...)
		{
			return false;
		}
	}

	template<typename Target> inline
	static
	typename
		boost::enable_if
		<
			charset::is_generalized_string_t<Target>,
			bool
		>::type
		check_path_name(const Target& text,
						typename
							regular_parse::match_results_t_maker
							<
								typename regular_parse::detail::string_helper<Target>::citer_type
							>::type& m)
	{
		try
		{
			return regular_parse::u32regex_parse::match(text, m, this_type::pro_s_match_u32regex_of_fpath());
		}
#if defined(_DEBUG)
		catch(const std::runtime_error& e)
		{
			std::cerr << e.what() << std::endl;
			return false;
		}
#endif // _DEBUG
		catch(...)
		{
			return false;
		}
	}

#else

	// check_file_name
	template<typename Iter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<Iter>,
				charset::is_not_any_utf8_string_reverse_iterator_t<Iter>
			>,
			bool
		>::type
		check_file_name(Iter s, Iter e)
	{
		typedef Iter iter_type;
		typedef regular_parse::detail::iterator_helper<iter_type> iter_h_type;
		typedef typename iter_h_type::value_type now_char_type;

		assert(std::distance(s, e) >= 0);

		try
		{
			return regular_parse::regex_parse::match(
					iter_h_type::org(s), iter_h_type::org(e),
					this_type::pro_s_match_regex_of_fname<now_char_type>());
		}
#if defined(_DEBUG)
		catch(const std::runtime_error& e)
		{
			std::cerr << e.what() << std::endl;
			return false;
		}
#endif // _DEBUG
		catch(...)
		{
			return false;
		}
	}

	template<typename Iter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<Iter>,
				charset::is_not_any_utf8_string_reverse_iterator_t<Iter>
			>,
			bool
		>::type
		check_file_name(Iter s, Iter e,
							typename regular_parse::match_results_t_maker<Iter>::type& m)
	{
		typedef Iter iter_type;
		typedef regular_parse::detail::iterator_helper<iter_type> iter_h_type;
		typedef typename iter_h_type::value_type now_char_type;

		assert(std::distance(s, e) >= 0);

		try
		{
			return regular_parse::regex_parse::match(
					iter_h_type::org(s), iter_h_type::org(e), m,
					this_type::pro_s_match_regex_of_fname<now_char_type>());
		}
#if defined(_DEBUG)
		catch(const std::runtime_error& e)
		{
			std::cerr << e.what() << std::endl;
			return false;
		}
#endif // _DEBUG
		catch(...)
		{
			return false;
		}
	}

	template<typename Target> inline
	static
	typename
		boost::enable_if
		<
			charset::is_generalized_string_t<Target>,
			bool
		>::type
		check_file_name(const Target& text)
	{
		typedef Target target_type;
		typedef regular_parse::detail::string_helper<target_type> target_h_type;
		typedef typename target_h_type::value_type now_char_type;

		try
		{
			return regular_parse::regex_parse::match(
					target_h_type::begin(text), target_h_type::end(text),
					this_type::pro_s_match_regex_of_fname<now_char_type>());
		}
#if defined(_DEBUG)
		catch(const std::runtime_error& e)
		{
			std::cerr << e.what() << std::endl;
			return false;
		}
#endif // _DEBUG
		catch(...)
		{
			return false;
		}
	}

	template<typename Target> inline
	static
	typename
		boost::enable_if
		<
			charset::is_generalized_string_t<Target>,
			bool
		>::type
		check_file_name(const Target& text,
							typename
							regular_parse::match_results_t_maker
							<
								typename regular_parse::detail::string_helper<Target>::citer_type
							>::type& m)
	{
		typedef Target target_type;
		typedef regular_parse::detail::string_helper<target_type> target_h_type;
		typedef typename target_h_type::value_type now_char_type;

		try
		{
			return regular_parse::regex_parse::match(
					target_h_type::begin(text), target_h_type::end(text), m,
					this_type::pro_s_match_regex_of_fname<now_char_type>());
		}
#if defined(_DEBUG)
		catch(const std::runtime_error& e)
		{
			std::cerr << e.what() << std::endl;
			return false;
		}
#endif // _DEBUG
		catch(...)
		{
			return false;
		}
	}

	// check_path_name
	template<typename Iter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<Iter>,
				charset::is_not_any_utf8_string_reverse_iterator_t<Iter>
			>,
			bool
		>::type
		check_path_name(Iter s, Iter e)
	{
		typedef Iter iter_type;
		typedef regular_parse::detail::iterator_helper<iter_type> iter_h_type;
		typedef typename iter_h_type::value_type now_char_type;

		assert(std::distance(s, e) >= 0);

		try
		{
			return regular_parse::regex_parse::match(
					iter_h_type::org(s), iter_h_type::org(e),
					this_type::pro_s_match_regex_of_fpath<now_char_type>());
		}
#if defined(_DEBUG)
		catch(const std::runtime_error& e)
		{
			std::cerr << e.what() << std::endl;
			return false;
		}
#endif // _DEBUG
		catch(...)
		{
			return false;
		}
	}

	template<typename Iter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<Iter>,
				charset::is_not_any_utf8_string_reverse_iterator_t<Iter>
			>,
			bool
		>::type
		check_path_name(Iter s, Iter e,
							typename regular_parse::match_results_t_maker<Iter>::type& m)
	{
		typedef Iter iter_type;
		typedef regular_parse::detail::iterator_helper<iter_type> iter_h_type;
		typedef typename iter_h_type::value_type now_char_type;

		assert(std::distance(s, e) >= 0);

		try
		{
			return regular_parse::regex_parse::match(
					iter_h_type::org(s), iter_h_type::org(e), m,
					this_type::pro_s_match_regex_of_fpath<now_char_type>());
		}
#if defined(_DEBUG)
		catch(const std::runtime_error& e)
		{
			std::cerr << e.what() << std::endl;
			return false;
		}
#endif // _DEBUG
		catch(...)
		{
			return false;
		}
	}

	template<typename Target> inline
	static
	typename
		boost::enable_if
		<
			charset::is_generalized_string_t<Target>,
			bool
		>::type
		check_path_name(const Target& text)
	{
		typedef Target target_type;
		typedef regular_parse::detail::string_helper<target_type> target_h_type;
		typedef typename target_h_type::value_type now_char_type;

		try
		{
			return regular_parse::regex_parse::match(
					target_h_type::begin(text), target_h_type::end(text),
					this_type::pro_s_match_regex_of_fpath<now_char_type>());
		}
#if defined(_DEBUG)
		catch(const std::runtime_error& e)
		{
			std::cerr << e.what() << std::endl;
			return false;
		}
#endif // _DEBUG
		catch(...)
		{
			return false;
		}
	}

	template<typename Target> inline
	static
	typename
		boost::enable_if
		<
			charset::is_generalized_string_t<Target>,
			bool
		>::type
		check_path_name(const Target& text,
							typename
							regular_parse::match_results_t_maker
							<
								typename regular_parse::detail::string_helper<Target>::citer_type
							>::type& m)
	{
		typedef Target target_type;
		typedef regular_parse::detail::string_helper<target_type> target_h_type;
		typedef typename target_h_type::value_type now_char_type;

		try
		{
			return regular_parse::regex_parse::match(
					target_h_type::begin(text), target_h_type::end(text), m,
					this_type::pro_s_match_regex_of_fpath<now_char_type>());
		}
#if defined(_DEBUG)
		catch(const std::runtime_error& e)
		{
			std::cerr << e.what() << std::endl;
			return false;
		}
#endif // _DEBUG
		catch(...)
		{
			return false;
		}
	}

#endif // BOOST_HAS_ICU

protected:
	//pro_s_slant
	template<typename Char> inline
	static Char pro_s_slant(void)
	{
#	if	defined(YGGR_AT_WINDOWS) && defined(YGGR_FILE_PATH_USE_BACK_SLANT_ONLY)
		YGGR_PP_CONST_CHAR_LOCAL_DEF(mark, '\\');
#	else
		YGGR_PP_CONST_CHAR_LOCAL_DEF(mark, '/');
#	endif // YGGR_FILE_PATH_USE_BACK_SLANT_ONLY
		return YGGR_PP_CONST_CHAR_LOCAL_GET(mark, Char);
	}

	template<typename Char> inline
	static Char pro_s_system_slant(void)
	{
#	if	defined(YGGR_AT_WINDOWS)
		YGGR_PP_CONST_CHAR_LOCAL_DEF(mark, '\\');
#	else
		YGGR_PP_CONST_CHAR_LOCAL_DEF(mark, '/');
#	endif // YGGR_FILE_PATH_USE_BACK_SLANT_ONLY
		return YGGR_PP_CONST_CHAR_LOCAL_GET(mark, Char);
	}

	template<typename Char> inline
	static Char pro_s_back_slant(void)
	{
		YGGR_PP_CONST_CHAR_LOCAL_DEF(mark, '\\');
		return YGGR_PP_CONST_CHAR_LOCAL_GET(mark, Char);
	}

	// pro_s_slants
#ifdef YGGR_AT_WINDOWS
	template<typename Char> inline
	static const Char* pro_s_slants(void)
	{
#	if	defined(YGGR_FILE_PATH_USE_BACK_SLANT_ONLY)
		YGGR_PP_CONST_STRING_LOCAL_DEF(mark, "\\");
#	else
		YGGR_PP_CONST_STRING_LOCAL_DEF(mark, "/\\");
#	endif // YGGR_FILE_PATH_USE_BACK_SLANT_ONLY
		return YGGR_PP_CONST_STRING_LOCAL_GET(mark, Char);
	}

#else
	template<typename Char> inline
	static const Char* pro_s_slants(void)
	{
		YGGR_PP_CONST_STRING_LOCAL_DEF(mark, "/");
		return YGGR_PP_CONST_STRING_LOCAL_GET(mark, Char);
	}
#endif // YGGR_AT_WINDOWS

	template<typename Char> inline
	static const Char* pro_s_slants_begin(void)
	{
		return this_type::pro_s_slants<Char>();
	}

#if defined(YGGR_AT_WINDOWS)
	template<typename Char> inline
	static const Char* pro_s_slants_end(void)
	{
#	if	defined(YGGR_FILE_PATH_USE_BACK_SLANT_ONLY)
		return this_type::pro_s_slants<Char>() + 1;
#	else
		return this_type::pro_s_slants<Char>() + 2;
#	endif // YGGR_FILE_PATH_USE_BACK_SLANT_ONLY

	}
#else
	template<typename Char> inline
	static const Char* pro_s_slants_end(void)
	{
		return this_type::pro_s_slants<Char>() + 1;
	}
#endif // YGGR_AT_WINDOWS

	// pro_s_is_root_mark
	template<typename Char> inline
	static
	typename
		boost::enable_if
		<
			charset::is_can_be_conv_into_char<Char>,
			bool
		>::type
		pro_s_is_root_mark(Char ch)
	{
#if defined(YGGR_AT_WINDOWS)
		typedef Char char_type;
		YGGR_PP_CONST_CHAR_LOCAL_DEF(root_mark, ':');

		return (ch == YGGR_PP_CONST_CHAR_LOCAL_GET(root_mark, char_type));
#else
		return false;
#endif // YGGR_AT_WINDOWS
	}

	template<typename Char> inline
	static
	typename
		boost::disable_if
		<
			charset::is_can_be_conv_into_char<Char>,
			bool
		>::type
		pro_s_is_root_mark(const Char& ch)
	{
#if defined(YGGR_AT_WINDOWS)
		typedef Char char_obj_type;
		typedef typename char_obj_type::value_type char_type;
		YGGR_PP_CONST_CHAR_LOCAL_DEF(root_mark, ':');

		return (ch == YGGR_PP_CONST_CHAR_LOCAL_GET(root_mark, char_type));
#else
		return false;
#endif // YGGR_AT_WINDOWS
	}

	// pro_s_is_slants
	template<typename Char> inline
	static
	typename
		boost::enable_if
		<
			charset::is_can_be_conv_into_char<Char>,
			bool
		>::type
		pro_s_is_slants(Char ch)
	{
		typedef Char char_type;
		switch(ch)
		{
#if defined(YGGR_AT_WINDOWS)
		case static_cast<char_type>('\\'):
#	if !defined(YGGR_FILE_PATH_USE_BACK_SLANT_ONLY)
		case static_cast<char_type>('/'):
#	endif // YGGR_FILE_PATH_USE_BACK_SLANT_ONLY
#else
		case static_cast<char_type>('/'):
#endif // YGGR_AT_WINDOWS
			return true;
		default:
			return false;
		}
	}

	template<typename Char> inline
	static
	typename
		boost::disable_if
		<
			charset::is_can_be_conv_into_char<Char>,
			bool
		>::type
		pro_s_is_slants(const Char& ch)
	{
		typedef Char char_obj_type;
		typedef typename char_obj_type::value_type char_type;
#if defined(YGGR_AT_WINDOWS)
		YGGR_PP_CONST_CHAR_LOCAL_DEF(back_slant, '\\');
#	if !defined(YGGR_FILE_PATH_USE_BACK_SLANT_ONLY)
		YGGR_PP_CONST_CHAR_LOCAL_DEF(slant, '/');
#	endif // YGGR_FILE_PATH_USE_BACK_SLANT_ONLY
#else
		YGGR_PP_CONST_CHAR_LOCAL_DEF(slant, '/');
#endif // YGGR_AT_WINDOWS

		return
#if defined(YGGR_AT_WINDOWS)
			(ch == YGGR_PP_CONST_CHAR_LOCAL_GET(back_slant, char_type))
#	if !defined(YGGR_FILE_PATH_USE_BACK_SLANT_ONLY)
				|| (ch == YGGR_PP_CONST_CHAR_LOCAL_GET(slant, char_type))
#	endif // YGGR_FILE_PATH_USE_BACK_SLANT_ONLY
#else
			(ch == YGGR_PP_CONST_CHAR_LOCAL_GET(slant, char_type))
#endif // YGGR_AT_WINDOWS
				;
	}

	template<typename Iter, typename String> inline
	static bool pro_s_fill_string(const std::pair<Iter, Iter> ret, String& str)
	{
		typedef Iter iter_type;
		typedef String now_string_type;

		typedef regular_parse::detail::iterator_helper<iter_type> iter_h_type;
		typedef typename iter_h_type::value_type char_type;
		typedef typename iter_h_type::org_iter_type org_iter_type;
		typedef std::pair<iter_type, iter_type> data_type;

		typedef regular_parse::detail::string_helper<now_string_type> now_string_h_type;
		typedef typename now_string_h_type::value_type now_char_type;
		typedef typename now_string_h_type::base_cont_type now_base_cont_type;
		typedef regular_parse::detail::string_helper<now_base_cont_type> now_base_cont_h_type;
		typedef
			regular_parse::detail::conv_iterator_helper
			<
				org_iter_type,
				now_char_type,
				char_type
			> now_conv_iter_h_type;
		typedef typename now_conv_iter_h_type::result_type now_pairs_type;

		container::clear(str);

		if((ret.first != ret.second))
		{
			now_conv_iter_h_type h;
			now_base_cont_type& base_cont = now_string_h_type::org(str);
			org_iter_type s = iter_h_type::org(ret.first);
			org_iter_type e = iter_h_type::org(ret.second);
			std::ptrdiff_t size = std::distance(s, e);
			assert(size > 0);
			now_pairs_type conv_iter_pairs;
			try
			{
				conv_iter_pairs = h(s, e);
			}
#if defined(_DEBUG)
			catch(const std::runtime_error& e)
			{
				std::cerr << e.what() << std::endl;
				return false;
			}
#endif // _DEBUG
			catch(...)
			{
				return false;
			}

			container::reserve(base_cont, static_cast<size_type>(size));
			std::copy(conv_iter_pairs.first, conv_iter_pairs.second,
						iterator_ex::inserter(base_cont, now_base_cont_h_type::end(base_cont)));
			now_string_h_type::recount(str);
		}

		return true;
	}

	template<typename Iter, typename PathString, typename NameString> inline
	static bool pro_s_fill_path_and_name(const
											yggr::tuple
											<
												std::pair<Iter, Iter>,
												std::pair<Iter, Iter>,
												bool
											>& ret,
											PathString& path, NameString& name)
	{
		return yggr::get<2>(ret)
				&& this_type::pro_s_fill_string(yggr::get<0>(ret), path)
				&& this_type::pro_s_fill_string(yggr::get<1>(ret), name);
	}

	template<typename Iter, typename PathString> inline
	static bool pro_s_fill_path_or_name(const
										yggr::tuple
										<
											std::pair<Iter, Iter>,
											bool
										>& ret,
										PathString& path_or_name)
	{
		return yggr::get<1>(ret)
				&& this_type::pro_s_fill_string(yggr::get<0>(ret), path_or_name);
	}

	template<typename Iter> inline
	static yggr::tuple<std::pair<Iter, Iter>, bool>
		pro_s_conv_to_path_only(const
									yggr::tuple
									<
										std::pair<Iter, Iter>,
										std::pair<Iter, Iter>,
										bool
									>& ret)
	{
		return yggr::make_tuple(yggr::get<0>(ret), yggr::get<2>(ret));
	}

	template<typename Iter> inline
	static yggr::tuple<std::pair<Iter, Iter>, bool>
		pro_s_conv_to_name_only(const
									yggr::tuple
									<
										std::pair<Iter, Iter>,
										std::pair<Iter, Iter>,
										bool
									>& ret)
	{
		return yggr::make_tuple(yggr::get<1>(ret), yggr::get<2>(ret));
	}

public:

	// is_slants
	template<typename Char> inline
	static
	typename
		boost::enable_if
		<
			charset::is_can_be_conv_into_char<Char>,
			bool
		>::type
		is_slants(Char ch)
	{
		return this_type::pro_s_is_slants(ch);
	}

	template<typename Char> inline
	static
	typename
		boost::disable_if
		<
			charset::is_can_be_conv_into_char<Char>,
			bool
		>::type
		is_slants(const Char& ch)
	{
		return this_type::pro_s_is_slants(ch);
	}

	template<typename Char> inline
	static Char char_slant(void)
	{
		return this_type::pro_s_slant<Char>();
	}

	template<typename Char> inline
	static Char char_system_slant(void)
	{
		return this_type::pro_s_system_slant<Char>();
	}

	// parsing_file_path_and_name
	// These functions consider the last cell to be the file name
	template<typename Iter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<Iter>,
				boost::mpl::not_< is_reverse_iterator<Iter> >
			>,
			yggr::tuple
			<
				std::pair<Iter, Iter>,
				std::pair<Iter, Iter>,
				bool
			>
		>::type
		parsing_file_path_and_name(Iter s, Iter e)
	{
		typedef Iter iter_type;
		typedef regular_parse::detail::iterator_helper<iter_type> iter_h_type;
		typedef typename iter_h_type::value_type char_type;
		typedef std::pair<iter_type, iter_type> data_type;
		typedef typename regular_parse::match_results_t_maker<Iter>::type mresult_type;
		typedef typename mresult_type::const_iterator mresult_iter_type;
		typedef typename mresult_type::value_type sub_match_type;
		typedef std::reverse_iterator<iter_type> riter_type;

		assert(std::distance(s, e) >= 0);

		mresult_type mrst;
		if(s == e)
		{
			return yggr::make_tuple(data_type(s, s), data_type(s, s), true);
		}
		else if(this_type::check_path_name(s, e, mrst))
		{
			assert(mrst.size() == 3);
			const sub_match_type& eins_sub_math = mrst[1];
			const sub_match_type& zwei_sub_match = mrst[2];

			s32 match_case =
				(static_cast<size_type>(eins_sub_math.matched) << 1)
				| static_cast<size_type>(zwei_sub_match.matched);

			assert(match_case > 0);

			switch(match_case)
			{
			case 2: // eins_sub true, zwei_sub false
				{
					riter_type riter(iter_h_type::make_iterator(eins_sub_math.second, s));
					return this_type::pro_s_is_root_mark(*riter)?
								yggr::make_tuple(data_type(s, e), data_type(e, e), true)
								: yggr::make_tuple(data_type(s, s), data_type(s, e), true);
				}
			case 1: // eins_sub false, zwei_sub true
			case 3: // eins_sub true, zwei_sub true
				{
					iter_type last_slant = iter_h_type::make_iterator(zwei_sub_match.first, s);
					iter_type last_slant_next = last_slant;
					std::advance(last_slant_next, 1);
					return yggr::make_tuple(
							data_type(s, last_slant_next),
							data_type(last_slant_next, e),
							true);
				}
			default:
				assert(false);
				return yggr::make_tuple(data_type(e, e), data_type(e, e), false);
			}
		}
		else
		{
			return yggr::make_tuple(data_type(e, e), data_type(e, e), false);
		}
	}

	template<typename Iter, typename PathString, typename NameString> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::and_
				<
					is_iterator<Iter>,
					boost::mpl::not_< is_reverse_iterator<Iter> >
				>,
				boost::mpl::and_
				<
					boost::mpl::not_< boost::is_const<PathString> >,
					charset::is_generalized_string_t<PathString>
				>,
				boost::mpl::and_
				<
					boost::mpl::not_< boost::is_const<NameString> >,
					charset::is_generalized_string_t<NameString>
				>
			>,
			bool
		>::type
		parsing_file_path_and_name(Iter s, Iter e, PathString& path, NameString& name)
	{
		return this_type::pro_s_fill_path_and_name(
				this_type::parsing_file_path_and_name(s, e), path, name);
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
			yggr::tuple
			<
				std::pair
				<
					typename regular_parse::detail::string_helper<Target>::citer_type,
					typename regular_parse::detail::string_helper<Target>::citer_type
				>,
				std::pair
				<
					typename regular_parse::detail::string_helper<Target>::citer_type,
					typename regular_parse::detail::string_helper<Target>::citer_type
				>,
				bool
			>
		>::type
		parsing_file_path_and_name(const Target& text)
	{
		typedef Target target_type;
		typedef regular_parse::detail::string_helper<target_type> target_h_type;

		return this_type::parsing_file_path_and_name(
				target_h_type::begin(text), target_h_type::end(text));
	}

	template<typename Target, typename PathString, typename NameString> inline
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
					boost::mpl::not_< boost::is_const<PathString> >,
					charset::is_generalized_string_t<PathString>
				>,
				boost::mpl::and_
				<
					boost::mpl::not_< boost::is_const<NameString> >,
					charset::is_generalized_string_t<NameString>
				>
			>,
			bool
		>::type
		parsing_file_path_and_name(const Target& text, PathString& path, NameString& name)
	{
		return this_type::pro_s_fill_path_and_name(
				this_type::parsing_file_path_and_name(text), path, name);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	static
		yggr::tuple
		<
			std::pair
			<
				typename charset::utf8_string_impl<Char, Traits, Alloc>::const_iterator,
				typename charset::utf8_string_impl<Char, Traits, Alloc>::const_iterator
			>,
			std::pair
			<
				typename charset::utf8_string_impl<Char, Traits, Alloc>::const_iterator,
				typename charset::utf8_string_impl<Char, Traits, Alloc>::const_iterator
			>,
			bool
		>
		parsing_file_path_and_name(const charset::utf8_string_impl<Char, Traits, Alloc>& text)
	{
		return this_type::parsing_file_path_and_name(text.cbegin(), text.cend());
	}

	template<typename Char, typename Traits, typename Alloc,
				typename PathString, typename NameString> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::and_
				<
					boost::mpl::not_< boost::is_const<PathString> >,
					charset::is_generalized_string_t<PathString>
				>,
				boost::mpl::and_
				<
					boost::mpl::not_< boost::is_const<NameString> >,
					charset::is_generalized_string_t<NameString>
				>
			>,
			bool
		>::type
		parsing_file_path_and_name(const charset::utf8_string_impl<Char, Traits, Alloc>& text,
									PathString& path, NameString& name)
	{
		return this_type::pro_s_fill_path_and_name(
				this_type::parsing_file_path_and_name(text.cbegin(), text.cend()), path, name);
	}

	// parsing_file_path
	// These functions consider the last cell to be the file name
	template<typename Iter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<Iter>,
				boost::mpl::not_< is_reverse_iterator<Iter> >
			>,
			yggr::tuple
			<
				std::pair<Iter, Iter>,
				bool
			>
		>::type
		parsing_file_path(Iter s, Iter e)
	{
		return this_type::pro_s_conv_to_path_only(
				this_type::parsing_file_path_and_name(s, e));
	}

	template<typename Iter, typename String> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::and_
				<
					is_iterator<Iter>,
					boost::mpl::not_< is_reverse_iterator<Iter> >
				>,
				boost::mpl::and_
				<
					boost::mpl::not_< boost::is_const<String> >,
					charset::is_generalized_string_t<String>
				>
			>,
			bool
		>::type
		parsing_file_path(Iter s, Iter e, String& path)
	{
		return this_type::pro_s_fill_path_or_name(
				this_type::parsing_file_path(s, e), path);
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
			yggr::tuple
			<
				std::pair
				<
					typename regular_parse::detail::string_helper<Target>::citer_type,
					typename regular_parse::detail::string_helper<Target>::citer_type
				>,
				bool
			>
		>::type
		parsing_file_path(const Target& text)
	{
		return this_type::pro_s_conv_to_path_only(
				this_type::parsing_file_path_and_name(text));
	}

	template<typename Target, typename String> inline
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
					boost::mpl::not_< boost::is_const<String> >,
					charset::is_generalized_string_t<String>
				>
			>,
			bool
		>::type
		parsing_file_path(const Target& text, String& path)
	{
		return this_type::pro_s_fill_path_or_name(
				this_type::parsing_file_path(text), path);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	static
		yggr::tuple
		<
			std::pair
			<
				typename charset::utf8_string_impl<Char, Traits, Alloc>::const_iterator,
				typename charset::utf8_string_impl<Char, Traits, Alloc>::const_iterator
			>,
			bool
		>
		parsing_file_path(const charset::utf8_string_impl<Char, Traits, Alloc>& text)
	{
		return this_type::pro_s_conv_to_path_only(
				this_type::parsing_file_path_and_name(text));
	}

	template<typename Char, typename Traits, typename Alloc, typename String> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_< boost::is_const<String> >,
				charset::is_generalized_string_t<String>
			>,
			bool
		>::type
		parsing_file_path(const charset::utf8_string_impl<Char, Traits, Alloc>& text,
							String& path)
	{
		return this_type::pro_s_fill_path_or_name(
				this_type::parsing_file_path(text), path);
	}

	// parsing_file_name
	// These functions consider the last cell to be the file name
	template<typename Iter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<Iter>,
				boost::mpl::not_< is_reverse_iterator<Iter> >
			>,
			yggr::tuple
			<
				std::pair<Iter, Iter>,
				bool
			>
		>::type
		parsing_file_name(Iter s, Iter e)
	{
		return this_type::pro_s_conv_to_name_only(
				this_type::parsing_file_path_and_name(s, e));
	}

	template<typename Iter, typename String> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::and_
				<
					is_iterator<Iter>,
					boost::mpl::not_< is_reverse_iterator<Iter> >
				>,
				boost::mpl::and_
				<
					boost::mpl::not_< boost::is_const<String> >,
					charset::is_generalized_string_t<String>
				>
			>,
			bool
		>::type
		parsing_file_name(Iter s, Iter e, String& name)
	{
		return this_type::pro_s_fill_path_or_name(
				this_type::parsing_file_name(s, e), name);
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
			yggr::tuple
			<
				std::pair
				<
					typename regular_parse::detail::string_helper<Target>::citer_type,
					typename regular_parse::detail::string_helper<Target>::citer_type
				>,
				bool
			>
		>::type
		parsing_file_name(const Target& text)
	{
		return this_type::pro_s_conv_to_name_only(
				this_type::parsing_file_path_and_name(text));
	}

	template<typename Target, typename String> inline
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
					boost::mpl::not_< boost::is_const<String> >,
					charset::is_generalized_string_t<String>
				>
			>,
			bool
		>::type
		parsing_file_name(const Target& text, String& name)
	{
		return this_type::pro_s_fill_path_or_name(
				this_type::parsing_file_name(text), name);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	static
		yggr::tuple
		<
			std::pair
			<
				typename charset::utf8_string_impl<Char, Traits, Alloc>::const_iterator,
				typename charset::utf8_string_impl<Char, Traits, Alloc>::const_iterator
			>,
			bool
		>
		parsing_file_name(const charset::utf8_string_impl<Char, Traits, Alloc>& text)
	{
		return this_type::pro_s_conv_to_name_only(
				this_type::parsing_file_path_and_name(text));
	}

	template<typename Char, typename Traits, typename Alloc, typename String> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_< boost::is_const<String> >,
				charset::is_generalized_string_t<String>
			>,
			bool
		>::type
		parsing_file_name(const charset::utf8_string_impl<Char, Traits, Alloc>& text,
							String& name)
	{
		return this_type::pro_s_fill_path_or_name(
				this_type::parsing_file_name(text), name);
	}

	// parsing_dir_path_and_name
	// These functions assume that the last cell is the directory name
	template<typename Iter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<Iter>,
				boost::mpl::not_< is_reverse_iterator<Iter> >
			>,
			yggr::tuple
			<
				std::pair<Iter, Iter>,
				std::pair<Iter, Iter>,
				bool
			>
		>::type
		parsing_dir_path_and_name(Iter s, Iter e)
	{
		typedef Iter iter_type;
		typedef regular_parse::detail::iterator_helper<iter_type> iter_h_type;
		typedef typename iter_h_type::value_type char_type;
		typedef std::pair<iter_type, iter_type> data_type;
		typedef typename regular_parse::match_results_t_maker<Iter>::type mresult_type;
		typedef typename mresult_type::const_iterator mresult_iter_type;
		typedef typename mresult_type::value_type sub_match_type;
		typedef std::reverse_iterator<iter_type> riter_type;

		assert(std::distance(s, e) >= 0);

		mresult_type mrst;
		if(s == e)
		{
			return yggr::make_tuple(data_type(s, s), data_type(s, s), true);
		}
		else
		{
			riter_type rs(e);
			iter_type ne = e;
			if(this_type::pro_s_is_slants(*rs))
			{
				std::advance(rs, 1);
				if(this_type::pro_s_is_slants(*rs))
				{
					return yggr::make_tuple(data_type(e, e), data_type(e, e), false);
				}
				ne = rs.base();
			}

			if(s == ne)
			{
				return yggr::make_tuple(data_type(s, s), data_type(s, e), true);
			}
			else if(this_type::check_path_name(s, ne, mrst))
			{
				assert(mrst.size() == 3);
				const sub_match_type& eins_sub_math = mrst[1];
				const sub_match_type& zwei_sub_match = mrst[2];

				s32 match_case =
					(static_cast<size_type>(eins_sub_math.matched) << 1)
					| static_cast<size_type>(zwei_sub_match.matched);

				assert(match_case > 0);

				switch(match_case)
				{
				case 2: // eins_sub true, zwei_sub false
					return yggr::make_tuple(data_type(s, s), data_type(s, ne), true);
				case 1: // eins_sub false, zwei_sub true
				case 3: // eins_sub true, zwei_sub true
					{
						iter_type last_slant = iter_h_type::make_iterator(zwei_sub_match.first, s);
						iter_type last_slant_next = last_slant;
						std::advance(last_slant_next, 1);
						return yggr::make_tuple(
								data_type(s, last_slant_next),
								data_type(last_slant_next, ne),
								true);
					}
				default:
					assert(false);
					return yggr::make_tuple(data_type(e, e), data_type(e, e), false);
				}
			}
			else
			{
				return yggr::make_tuple(data_type(e, e), data_type(e, e), false);
			}
		}
	}

	template<typename Iter, typename PathString, typename NameString> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::and_
				<
					is_iterator<Iter>,
					boost::mpl::not_< is_reverse_iterator<Iter> >
				>,
				boost::mpl::and_
				<
					boost::mpl::not_< boost::is_const<PathString> >,
					charset::is_generalized_string_t<PathString>
				>,
				boost::mpl::and_
				<
					boost::mpl::not_< boost::is_const<NameString> >,
					charset::is_generalized_string_t<NameString>
				>
			>,
			bool
		>::type
		parsing_dir_path_and_name(Iter s, Iter e, PathString& path, NameString& name)
	{
		return this_type::pro_s_fill_path_and_name(
				this_type::parsing_dir_path_and_name(s, e), path, name);
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
			yggr::tuple
			<
				std::pair
				<
					typename regular_parse::detail::string_helper<Target>::citer_type,
					typename regular_parse::detail::string_helper<Target>::citer_type
				>,
				std::pair
				<
					typename regular_parse::detail::string_helper<Target>::citer_type,
					typename regular_parse::detail::string_helper<Target>::citer_type
				>,
				bool
			>
		>::type
		parsing_dir_path_and_name(const Target& text)
	{
		typedef Target target_type;
		typedef regular_parse::detail::string_helper<target_type> target_h_type;
		typedef typename target_h_type::citer_type iter_type;

		return this_type::parsing_dir_path_and_name(
				target_h_type::begin(text), target_h_type::end(text));
	}

	template<typename Target, typename PathString, typename NameString> inline
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
					boost::mpl::not_< boost::is_const<PathString> >,
					charset::is_generalized_string_t<PathString>
				>,
				boost::mpl::and_
				<
					boost::mpl::not_< boost::is_const<NameString> >,
					charset::is_generalized_string_t<NameString>
				>
			>,
			bool
		>::type
		parsing_dir_path_and_name(const Target& text, PathString& path, NameString& name)
	{
		return this_type::pro_s_fill_path_and_name(
				this_type::parsing_dir_path_and_name(text), path, name);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	static
		yggr::tuple
		<
			std::pair
			<
				typename charset::utf8_string_impl<Char, Traits, Alloc>::const_iterator,
				typename charset::utf8_string_impl<Char, Traits, Alloc>::const_iterator
			>,
			std::pair
			<
				typename charset::utf8_string_impl<Char, Traits, Alloc>::const_iterator,
				typename charset::utf8_string_impl<Char, Traits, Alloc>::const_iterator
			>,
			bool
		>
		parsing_dir_path_and_name(const charset::utf8_string_impl<Char, Traits, Alloc>& text)
	{
		return this_type::parsing_dir_path_and_name(text.cbegin(), text.cend());
	}

	template<typename Char, typename Traits, typename Alloc,
				typename PathString, typename NameString> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::and_
				<
					boost::mpl::not_< boost::is_const<PathString> >,
					charset::is_generalized_string_t<PathString>
				>,
				boost::mpl::and_
				<
					boost::mpl::not_< boost::is_const<NameString> >,
					charset::is_generalized_string_t<NameString>
				>
			>,
			bool
		>::type
		parsing_dir_path_and_name(const charset::utf8_string_impl<Char, Traits, Alloc>& text,
									PathString& path, NameString& name)
	{
		return this_type::pro_s_fill_path_and_name(
				this_type::parsing_dir_path_and_name(text), path, name);
	}

	// parsing_dir_path
	// These functions consider the last cell to be the directory name
	template<typename Iter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<Iter>,
				boost::mpl::not_< is_reverse_iterator<Iter> >
			>,
			yggr::tuple
			<
				std::pair<Iter, Iter>,
				bool
			>
		>::type
		parsing_dir_path(Iter s, Iter e)
	{
		return this_type::pro_s_conv_to_path_only(
				this_type::parsing_dir_path_and_name(s, e));
	}

	template<typename Iter, typename String> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::and_
				<
					is_iterator<Iter>,
					boost::mpl::not_< is_reverse_iterator<Iter> >
				>,
				boost::mpl::and_
				<
					boost::mpl::not_< boost::is_const<String> >,
					charset::is_generalized_string_t<String>
				>
			>,
			bool
		>::type
		parsing_dir_path(Iter s, Iter e, String& path)
	{
		return this_type::pro_s_fill_path_or_name(
				this_type::parsing_dir_path(s, e), path);
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
			yggr::tuple
			<
				std::pair
				<
					typename regular_parse::detail::string_helper<Target>::citer_type,
					typename regular_parse::detail::string_helper<Target>::citer_type
				>,
				bool
			>
		>::type
		parsing_dir_path(const Target& text)
	{
		return this_type::pro_s_conv_to_path_only(
				this_type::parsing_dir_path_and_name(text));
	}

	template<typename Target, typename String> inline
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
					boost::mpl::not_< boost::is_const<String> >,
					charset::is_generalized_string_t<String>
				>
			>,
			bool
		>::type
		parsing_dir_path(const Target& text, String& path)
	{
		return this_type::pro_s_fill_path_or_name(
				this_type::parsing_dir_path(text), path);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	static
		yggr::tuple
		<
			std::pair
			<
				typename charset::utf8_string_impl<Char, Traits, Alloc>::const_iterator,
				typename charset::utf8_string_impl<Char, Traits, Alloc>::const_iterator
			>,
			bool
		>
		parsing_dir_path(const charset::utf8_string_impl<Char, Traits, Alloc>& text)
	{
		return this_type::pro_s_conv_to_path_only(
				this_type::parsing_dir_path_and_name(text));
	}

	template<typename Char, typename Traits, typename Alloc, typename String> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_< boost::is_const<String> >,
				charset::is_generalized_string_t<String>
			>,
			bool
		>::type
		parsing_dir_path(const charset::utf8_string_impl<Char, Traits, Alloc>& text,
							String& path)
	{
		return this_type::pro_s_fill_path_or_name(
				this_type::parsing_dir_path(text), path);
	}

	// parsing_dir_name
	// These functions consider the last cell to be the directory name
	template<typename Iter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<Iter>,
				boost::mpl::not_< is_reverse_iterator<Iter> >
			>,
			yggr::tuple
			<
				std::pair<Iter, Iter>,
				bool
			>
		>::type
		parsing_dir_name(Iter s, Iter e)
	{
		return this_type::pro_s_conv_to_name_only(
				this_type::parsing_dir_path_and_name(s, e));
	}

	template<typename Iter, typename String> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::and_
				<
					is_iterator<Iter>,
					boost::mpl::not_< is_reverse_iterator<Iter> >
				>,
				boost::mpl::and_
				<
					boost::mpl::not_< boost::is_const<String> >,
					charset::is_generalized_string_t<String>
				>
			>,
			bool
		>::type
		parsing_dir_name(Iter s, Iter e, String& name)
	{
		return this_type::pro_s_fill_path_or_name(
				this_type::parsing_dir_name(s, e), name);
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
			yggr::tuple
			<
				std::pair
				<
					typename regular_parse::detail::string_helper<Target>::citer_type,
					typename regular_parse::detail::string_helper<Target>::citer_type
				>,
				bool
			>
		>::type
		parsing_dir_name(const Target& text)
	{
		return this_type::pro_s_conv_to_name_only(
				this_type::parsing_dir_path_and_name(text));
	}

	template<typename Target, typename String> inline
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
					boost::mpl::not_< boost::is_const<String> >,
					charset::is_generalized_string_t<String>
				>
			>,
			bool
		>::type
		parsing_dir_name(const Target& text, String& name)
	{
		return this_type::pro_s_fill_path_or_name(
				this_type::parsing_dir_name(text), name);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	static
		yggr::tuple
		<
			std::pair
			<
				typename charset::utf8_string_impl<Char, Traits, Alloc>::const_iterator,
				typename charset::utf8_string_impl<Char, Traits, Alloc>::const_iterator
			>,
			bool
		>
		parsing_dir_name(const charset::utf8_string_impl<Char, Traits, Alloc>& text)
	{
		return this_type::pro_s_conv_to_name_only(
				this_type::parsing_dir_path_and_name(text));
	}

	template<typename Char, typename Traits, typename Alloc, typename String> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_< boost::is_const<String> >,
				charset::is_generalized_string_t<String>
			>,
			bool
		>::type
		parsing_dir_name(const charset::utf8_string_impl<Char, Traits, Alloc>& text,
							String& name)
	{
		return this_type::pro_s_fill_path_or_name(
				this_type::parsing_dir_name(text), name);
	}

protected:

	// pro_s_split_path don't check allocator
	template<typename Container, typename Iter, typename Alloc>
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::and_
				<
					container_ex::is_container<Container>,
					boost::mpl::not_< boost::is_const<Container> >
				>,
				boost::mpl::and_
				<
					is_iterator<Iter>,
					boost::mpl::not_< is_reverse_iterator<Iter> >
				>
			>,
			size_type
		>::type
		pro_s_split_path(Container& cont, Iter s, Iter e, const Alloc& alloc)
	{
		typedef Container cont_type;
		typedef Iter iter_type;
		typedef regular_parse::detail::iterator_helper<iter_type> iter_h_type;
		typedef typename iter_h_type::org_iter_type org_iter_type;
		typedef typename boost::range_value<cont_type>::type data_type;
#if defined(BOOST_HAS_ICU)
		typedef regular_parse::u32regex_parse::regex_type regex_type;
#else
		typedef typename iter_h_type::value_type char_type;
		typedef boost::basic_regex<char_type> regex_type;
#endif // BOOST_HAS_ICU
		typedef typename
			regular_parse::regex_token_iterator_t_maker
			<
				iter_type,
				regex_type
			>::type regex_token_iter_type;
		typedef std::reverse_iterator<org_iter_type> riter_type;
		typedef detail::split_data_creator<data_type, iter_type> data_creator_type;

		assert(std::distance(s, e) >= 0);

		container::clear(cont);

		data_creator_type dc(s);

		if(s == e)
		{
			return (iterator_ex::inserter(cont, boost::end(cont)) = dc(s, s, alloc), 1);
		}
		else
		{
			org_iter_type org_s = iter_h_type::org(s);
			org_iter_type org_e = iter_h_type::org(e);

			regex_token_iter_type re_token_iter;
			try
			{
				re_token_iter =
#if defined(BOOST_HAS_ICU)
					regular_parse::u32regex_parse::make_regex_token_iterator(
						s, e,
						this_type::pro_s_search_u32regex_of_fpath(), 0,
						boost::match_default | boost::match_not_null);
#else
					regular_parse::regex_parse::make_regex_token_iterator(
						org_s, org_e,
						this_type::pro_s_search_regex_of_fpath<char_type>(), 0,
						boost::match_default | boost::match_not_null);
#endif // BOOST_HAS_ICU
			}
#if defined(_DEBUG)
			catch(const std::runtime_error& e)
			{
				std::cerr << e.what() << std::endl;
				return false;
			}
#endif // _DEBUG
			catch(...)
			{
				return 0;
			}

			if( (re_token_iter == regex_token_iter_type())
					|| (!(re_token_iter->matched))
					|| (re_token_iter->first != org_s) )
			{
				return 0;
			}
			else
			{
				size_type count = 0;
				riter_type riter;

				riter = riter_type(re_token_iter->second);
				if(this_type::pro_s_is_slants(*riter))
				{
					std::advance(riter, 1);
				}

				iterator_ex::inserter(cont, boost::end(cont))
					= dc(re_token_iter->first,
							org_s == riter.base()? re_token_iter->second : riter.base(),
							alloc);

				++re_token_iter;
				++count;

				for(regex_token_iter_type isize = regex_token_iter_type();
						(re_token_iter != isize) && (re_token_iter->matched);
						++re_token_iter, ++count)
				{
					riter = riter_type(re_token_iter->second);
					if(this_type::pro_s_is_slants(*riter))
					{
						std::advance(riter, 1);
						if(re_token_iter->first == riter.base())
						{
							return 0;
						}
					}

					iterator_ex::inserter(cont, boost::end(cont)) = dc(re_token_iter->first, riter.base(), alloc);
				}

				if(re_token_iter == regex_token_iter_type())
				{
					riter_type chk_riter(org_e);
					if(this_type::pro_s_is_slants(*chk_riter))
					{
						std::advance(chk_riter, 1);
					}
					return riter == chk_riter? count : 0;
				}
				else
				{
					return 0;
				}
			}
		}
	}

public:
	//split_path

	template<typename Container, typename Iter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::and_
				<
					container_ex::is_container<Container>,
					boost::mpl::not_< boost::is_const<Container> >
				>,
				boost::mpl::and_
				<
					is_iterator<Iter>,
					boost::mpl::not_< is_reverse_iterator<Iter> >
				>
			>,
			size_type
		>::type
		split_path(Container& cont, Iter s, Iter e)
	{
		mplex::null_type null_obj;
		return this_type::pro_s_split_path(cont, s, e, null_obj);
	}

	template<typename Container, typename Iter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::and_
				<
					container_ex::is_container<Container>,
					boost::mpl::not_< boost::is_const<Container> >
				>,
				boost::mpl::and_
				<
					is_iterator<Iter>,
					boost::mpl::not_< is_reverse_iterator<Iter> >
				>
			>,
			size_type
		>::type
		split_path(Container& cont, Iter s, Iter e,
					const
					typename
						allocator_t
						<
							typename boost::range_value<Container>::type
						>::type& alloc)
	{
		return this_type::pro_s_split_path(cont, s, e, alloc);
	}

	template<typename Container, typename Target> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::and_
				<
					container_ex::is_container<Container>,
					boost::mpl::not_< boost::is_const<Container> >
				>,
				boost::mpl::and_
				<
					charset::is_generalized_string_t<Target>,
					charset::is_not_utf8_string_t<Target>
				>
			>,
			size_type
		>::type
		split_path(Container& cont, const Target& text)
	{
		typedef Target target_type;
		typedef regular_parse::detail::string_helper<target_type> target_h_type;

		mplex::null_type null_obj;
		return this_type::pro_s_split_path(cont, target_h_type::begin(text), target_h_type::end(text), null_obj);
	}

	template<typename Container, typename Target> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::and_
				<
					container_ex::is_container<Container>,
					boost::mpl::not_< boost::is_const<Container> >
				>,
				boost::mpl::and_
				<
					charset::is_generalized_string_t<Target>,
					charset::is_not_utf8_string_t<Target>
				>
			>,
			size_type
		>::type
		split_path(Container& cont, const Target& text,
					const
					typename
						allocator_t
						<
							typename boost::range_value<Container>::type
						>::type& alloc)
	{
		typedef Target target_type;
		typedef regular_parse::detail::string_helper<target_type> target_h_type;

		return this_type::pro_s_split_path(cont, target_h_type::begin(text), target_h_type::end(text), alloc);
	}

	template<typename Container, typename Char, typename Traits, typename Alloc> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				container_ex::is_container<Container>,
				boost::mpl::not_< boost::is_const<Container> >
			>,
			size_type
		>::type
		split_path(Container& cont,
					const charset::utf8_string_impl<Char, Traits, Alloc>& text)
	{
		mplex::null_type null_obj;
		return this_type::pro_s_split_path(cont, text.cbegin(), text.cend(), null_obj);
	}

	template<typename Container, typename Char, typename Traits, typename Alloc> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				container_ex::is_container<Container>,
				boost::mpl::not_< boost::is_const<Container> >
			>,
			size_type
		>::type
		split_path(Container& cont,
					const charset::utf8_string_impl<Char, Traits, Alloc>& text,
					const
					typename
						allocator_t
						<
							typename boost::range_value<Container>::type
						>::type& alloc)
	{
		return this_type::pro_s_split_path(cont, text.cbegin(), text.cend(), alloc);
	}
};

} // namespace file_system
} // namespace yggr

#endif // __YGGR_FILE_SYSTEM_FILE_CHECK_HPP__
