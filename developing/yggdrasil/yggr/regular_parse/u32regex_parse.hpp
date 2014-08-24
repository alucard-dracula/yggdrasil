//u32regex_parse.hpp

#ifndef __YGGR_REGULAR_PARSE_U32REGEX_PARSE_HPP__
#define __YGGR_REGULAR_PARSE_U32REGEX_PARSE_HPP__

#ifdef __MINGW32__
#	error "mingw32 not support this!!!"
#endif // __MINGW32__

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
*/

#ifdef BOOST_HAS_ICU

#ifdef _MSC_VER
#	include <yggr/support/max_min_undef.ipp>
#endif // _MSC_VER

#include <limits>
#include <iterator>

#include <boost/regex.hpp>
#include <boost/ref.hpp>
#include <boost/regex/config.hpp>
#include <boost/regex/icu.hpp>

#include <yggr/charset/string.hpp>
#include <yggr/charset/utf8_string.hpp>
#include <yggr/base/error_make.hpp>

#if defined _MSC_VER
#	pragma comment(lib, "icuuc.lib")
#	pragma comment(lib, "icuin.lib")
#	pragma comment(lib, "icudt.lib")
#endif // _MSC_VER


namespace yggr
{
namespace regular_parse
{

class u32regex_parse
{
private:
	typedef boost::u32regex regex_type;
public:

	typedef boost::regbase regex_base_type;
	typedef regex_type::flag_type regex_flag_type;
	typedef boost::regex_constants::match_flag_type match_flag_type;
	typedef boost::regex_constants::match_flag_type replace_flag_type;
	typedef boost::regex_constants::match_flag_type split_flag_type;
	typedef boost::regex_constants::match_flag_type grep_flag_type;
	typedef boost::regex_error error_type;

private:
	typedef u32regex_parse this_type;

private:
	template<typename Container>
	struct org_begin
	{
		typedef typename Container::iterator iter_type;
		typedef typename Container::const_iterator citer_type;
		
		iter_type operator()(Container& cont) const
		{
			return cont.begin();
		}

		citer_type operator()(const Container& cont) const
		{
			return cont.begin();
		}
	};

	template<typename Container>
	struct org_end
	{
		typedef typename Container::iterator iter_type;
		typedef typename Container::const_iterator citer_type;
		
		iter_type operator()(Container& cont) const
		{
			return cont.end();
		}

		citer_type operator()(const Container& cont) const
		{
			return cont.end();
		}
	};

	template<typename Char, typename Traits, typename Alloc>
	struct org_begin< charset::utf8_string_impl<Char, Traits, Alloc> >
	{
		typedef charset::utf8_string_impl<Char, Traits, Alloc> cont_type;
		typedef typename cont_type::base_type cont_base_type;
		typedef typename cont_base_type::iterator iter_type;
		typedef typename cont_base_type::const_iterator citer_type;

		iter_type operator()(cont_type& cont) const
		{
			return cont.base_type::begin();
		}

		citer_type operator()(const cont_type& cont) const
		{
			return cont.base_type::begin();
		}
	};

	template<typename Char, typename Traits, typename Alloc>
	struct org_end< charset::utf8_string_impl<Char, Traits, Alloc> >
	{
		typedef charset::utf8_string_impl<Char, Traits, Alloc> cont_type;
		typedef typename cont_type::base_type cont_base_type;
		typedef typename cont_base_type::iterator iter_type;
		typedef typename cont_base_type::const_iterator citer_type;

		iter_type operator()(cont_type& cont) const
		{
			return cont.base_type::end();
		}

		citer_type operator()(const cont_type& cont) const
		{
			return cont.base_type::end();
		}
	};

public:

//--------------------match----------------------------
	template<typename Target,
				typename Char, typename Traits, typename Alloc>
	static bool match(const Target& target, 
						const charset::utf8_string_impl<Char, Traits, Alloc>& str_regex,
						regex_flag_type re_flag = regex_base_type::normal,
						match_flag_type mth_flag = boost::match_default)
	{
		typedef Target target_type;
		typedef typename target_type::value_type target_value_type;
		typedef typename target_type::const_iterator target_citer_type;

		typedef charset::utf8_string_impl<Char, Traits, Alloc> string_type;

		BOOST_MPL_ASSERT((boost::is_same<target_value_type, Char>));

	
		regex_type re(boost::make_u32regex(str_regex.c_str(), re_flag));
		boost::match_results<target_citer_type> m;

		return boost::re_detail::do_regex_match(org_begin<target_type>()(target),
													org_end<target_type>()(target),
													m, re, mth_flag, 
													static_cast<
														boost::mpl::int_<
															sizeof(*(org_begin<target_type>()(target)))> const*>(0));
	
	}

	template<typename Char, typename Traits, typename Alloc>
	static bool match(const charset::utf8_string_impl<Char, Traits, Alloc>& target, 
						const charset::utf8_string_impl<Char, Traits, Alloc>& str_regex,
						regex_flag_type re_flag = regex_base_type::normal,
						match_flag_type mth_flag = boost::match_default)
	{
		typedef charset::utf8_string_impl<Char, Traits, Alloc> string_type;

		
		regex_type re(boost::make_u32regex(str_regex.c_str(), re_flag));
		return boost::u32regex_match(target.c_str(), re, mth_flag);

	}

	template<typename Target,
				typename Char, typename Traits, typename Alloc,
				typename Handler>
	static bool match(const Target& target,
						const charset::utf8_string_impl<Char, Traits, Alloc>& str_regex,
						regex_flag_type re_flag, match_flag_type mth_flag,
						const Handler& handler)
	{
		typedef Target target_type;
		typedef typename target_type::value_type target_value_type;
		typedef typename target_type::const_iterator target_citer_type;

		typedef charset::utf8_string_impl<Char, Traits, Alloc> string_type;

		BOOST_MPL_ASSERT((boost::is_same<target_value_type, Char>));

	
		regex_type re(boost::make_u32regex(str_regex.c_str(), re_flag));
		boost::match_results<target_citer_type> m;

		bool bret = boost::re_detail::do_regex_match(org_begin<target_type>()(target),
														org_end<target_type>()(target),
														m, re, mth_flag, 
														static_cast<
															boost::mpl::int_<
																sizeof(*(org_begin<target_type>()(target)))> const*>(0));

		return handler(bret, m);
	}

	template< typename Char, typename Traits, typename Alloc,
				typename Handler>
	static bool match(const charset::utf8_string_impl<Char, Traits, Alloc>& target,
						const charset::utf8_string_impl<Char, Traits, Alloc>& str_regex,
						regex_flag_type re_flag, match_flag_type mth_flag,
						const Handler& handler)
	{
		typedef charset::utf8_string_impl<Char, Traits, Alloc> string_type;
		typedef typename string_type::base_type string_base_type;
		typedef typename string_base_type::const_iterator string_citer_type;
		typedef boost::match_results<string_citer_type> match_rst_type;

		
		regex_type re(boost::make_u32regex(str_regex.c_str(), re_flag));
		match_rst_type m;

		bool bret = boost::re_detail::do_regex_match(org_begin<string_type>()(target),
														org_end<string_type>()(target), 
														m, re, mth_flag,
														static_cast<boost::mpl::int_<sizeof(Char)> const*>(0));
		return handler(bret, m);
	}

//---------------------------------replace--------------------------------------

	template<typename Char, typename Traits, typename Alloc>
	static charset::utf8_string_impl<Char, Traits, Alloc> 
		replace(const charset::utf8_string_impl<Char, Traits, Alloc>& text, 
					const charset::utf8_string_impl<Char, Traits, Alloc>& str_regex, 
					const charset::utf8_string_impl<Char, Traits, Alloc>& str_fmt,
					regex_flag_type re_flag = regex_base_type::normal,
					replace_flag_type rl_flag = boost::match_default | boost::format_all)
	{
		typedef charset::utf8_string_impl<Char, Traits, Alloc> string_type;
		typedef typename string_type::base_type base_type;

		
		regex_type re(boost::make_u32regex(str_regex.c_str(), re_flag));

		string_type result;
			
		base_type& ref_base = result;
		boost::re_detail::string_out_iterator<base_type> i(ref_base);
		boost::u32regex_replace(i, org_begin<string_type>()(text),
									org_end<string_type>()(text),
									re, str_fmt.c_str(), rl_flag);
		result.recount_size();
		return result;
	}

	template<typename Char, typename Traits, typename Alloc, 
				typename Handler>
	static charset::utf8_string_impl<Char, Traits, Alloc>
		replace(const charset::utf8_string_impl<Char, Traits, Alloc>& text, 
				const charset::utf8_string_impl<Char, Traits, Alloc>& str_regex, 
				const charset::utf8_string_impl<Char, Traits, Alloc>& str_fmt,
				regex_flag_type re_flag, replace_flag_type rl_flag, const Handler& handler)
	{
		return handler(	this_type::replace<Char, Traits, Alloc>
							(text, str_regex, str_fmt, re_flag, rl_flag) );
	}

	template<typename Out,
				typename Char, typename Traits, typename Alloc>
	static Out& replace(Out& out, 
							const charset::utf8_string_impl<Char, Traits, Alloc>& target, 
							const charset::utf8_string_impl<Char, Traits, Alloc>& str_regex,
							const charset::utf8_string_impl<Char, Traits, Alloc>& str_fmt,
							regex_flag_type re_flag = regex_type::normal,
							replace_flag_type rl_flag = boost::match_default | boost::format_all)
	{
		typedef charset::utf8_string_impl<Char, Traits, Alloc> string_type;

		typedef Out out_type;
		typedef typename out_type::iterator out_iter_type;

		out_iter_type oi(out);

		
		regex_type re(boost::make_u32regex(str_regex.c_str(), re_flag));
		boost::u32regex_replace(oi, 
								org_begin<string_type>()(target), 
								org_end<string_type>()(target), 
								re, str_fmt.c_str(), rl_flag);
		return out;
	}

	template<typename Out,
				typename Char, typename Traits, typename Alloc,
				typename Handler>
	static Out& replace(Out& out, const charset::utf8_string_impl<Char, Traits, Alloc>& target, 
							const charset::utf8_string_impl<Char, Traits, Alloc>& str_regex, 
							const charset::utf8_string_impl<Char, Traits, Alloc>& str_fmt,
							regex_flag_type re_flag, replace_flag_type rl_flag, const Handler& handler)
	{
		return handler( this_type::replace(out, target, str_regex, str_fmt, re_flag, rl_flag) );
	}

//-----------------------------search------------------------------------

	template<typename Target,
				typename Char, typename Traits, typename Alloc>
	static bool search(const Target& target, 
						const charset::utf8_string_impl<Char, Traits, Alloc>& str_regex,
						regex_flag_type re_flag = regex_base_type::normal,
						match_flag_type mth_flag = boost::match_default)
	{
		typedef Target target_type;
		typedef typename target_type::value_type target_value_type;
		typedef typename target_type::const_iterator target_citer_type;

		typedef boost::match_results<target_citer_type> match_rst_type;

		BOOST_MPL_ASSERT((boost::is_same<target_value_type, Char>));

	
		match_rst_type m;
		regex_type re(boost::make_u32regex(str_regex.c_str(), re_flag));

		return boost::u32regex_search(org_begin<target_type>()(target), 
										org_end<target_type>()(target), 
										m, re, mth_flag);
	}

	template<typename Char, typename Traits, typename Alloc>
	static bool search(const charset::utf8_string_impl<Char, Traits, Alloc>& target, 
						const charset::utf8_string_impl<Char, Traits, Alloc>& str_regex,
						regex_flag_type re_flag = regex_base_type::normal,
						match_flag_type mth_flag = boost::match_default)
	{
		typedef charset::utf8_string_impl<Char, Traits, Alloc> string_type;
		typedef typename string_type::base_type string_base_type;
		typedef typename string_base_type::const_iterator string_citer_type;

		typedef boost::match_results<string_citer_type> match_rst_type;

		match_rst_type m;
		regex_type re(boost::make_u32regex(str_regex.c_str(), re_flag));

		return boost::u32regex_search(org_begin<string_type>()(target), 
										org_end<string_type>()(target), 
										m, re, mth_flag);
	}

	template<typename Target,
				typename Char, typename Traits, typename Alloc,
				typename Handler>
	static bool search(const Target& target, 
						const charset::utf8_string_impl<Char, Traits, Alloc>& str_regex,
						regex_flag_type re_flag, match_flag_type mth_flag,
						const Handler& handler)
	{
		typedef Target target_type;
		typedef typename target_type::value_type target_value_type;
		typedef typename target_type::const_iterator target_citer_type;

		typedef boost::match_results<target_citer_type> match_rst_type;

		BOOST_MPL_ASSERT((boost::is_same<target_value_type, Char>));

		regex_type re(boost::make_u32regex(str_regex.c_str(), re_flag));
		match_rst_type m;
		bool bret = boost::u32regex_search(org_begin<target_type>()(target),
											org_end<target_type>()(target), 
											m, re, mth_flag);
		return handler(bret, m);
	}

	template<typename Char, typename Traits, typename Alloc,
				typename Handler>
	static bool search(const charset::utf8_string_impl<Char, Traits, Alloc>& target, 
						const charset::utf8_string_impl<Char, Traits, Alloc>& str_regex,
						regex_flag_type re_flag, match_flag_type mth_flag,
						const Handler& handler)
	{
		typedef charset::utf8_string_impl<Char, Traits, Alloc> string_type;
		typedef typename string_type::base_type string_base_type;
		typedef typename string_base_type::const_iterator string_citer_type;

		typedef boost::match_results<string_citer_type> match_rst_type;

		regex_type re(boost::make_u32regex(str_regex.c_str(), re_flag));
		match_rst_type m;
		bool bret = boost::u32regex_search(org_begin<string_type>()(target),
											org_end<string_type>()(target), 
											m, re, mth_flag);
		return handler(bret, m);
	}
};

} // namespace regular_parse
} // namespace yggr

#endif // BOOST_HAS_ICU
#endif //__YGGR_REGULAR_PARSE_U32REGEX_PARSE_HPP__
