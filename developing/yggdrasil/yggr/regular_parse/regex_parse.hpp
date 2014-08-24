//regex_parse.hpp

#ifndef __YGGR_REGULAR_PARSE_REGEX_PARSE_HPP__
#define __YGGR_REGULAR_PARSE_REGEX_PARSE_HPP__

#ifdef _MSC_VER
#	include <yggr/support/max_min_undef.ipp>
#endif // _MSC_VER

#include <iterator>
#include <limits>

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/if.hpp>

#include <boost/regex.hpp>
#include <boost/ref.hpp>

#include <yggr/charset/string.hpp>

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
	typedef boost::regex_constants::match_flag_type replace_flag_type;
	typedef boost::regex_constants::match_flag_type split_flag_type;
	typedef boost::regex_constants::match_flag_type grep_flag_type;
	typedef boost::regex_error error_type;

private:
	typedef regex_parse this_type;

public:
//-----------------------------match-----------------------------------------------
	template<typename Target, typename Char>
	static bool match(const Target& target, const Char* str_regex,
						regex_flag_type re_flag = regex_base_type::normal,
						match_flag_type mth_flag = boost::match_default)
	{
		typedef std::basic_string<Char> string_type;
		return this_type::match<Target,
								Char,
								std::char_traits<Char>,
								std::allocator<Char>,
								std::basic_string
								>
								(target, string_type(str_regex), re_flag, mth_flag);
	}

	template<typename Target,
				typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static bool match(const Target& target, const Basic_String<Char, Traits, Alloc>& str_regex,
						regex_flag_type re_flag = regex_base_type::normal,
						match_flag_type mth_flag = boost::match_default)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		BOOST_MPL_ASSERT((charset::is_not_utf8_string<Target>));
		BOOST_MPL_ASSERT((charset::is_not_utf8_string<string_type>));
		BOOST_MPL_ASSERT((boost::is_same<typename Target::value_type, Char>));

		typedef Target target_type;
		typedef boost::basic_regex< Char, boost::regex_traits<Char> > regex_type;


		regex_type re(str_regex.begin(), str_regex.end(), re_flag);
		return boost::regex_match(target.begin(), target.end(), re, mth_flag);

	}

	template<typename Target, typename Char, typename Handler>
	static bool match(const Target& target, const Char* str_regex,
						regex_flag_type re_flag, match_flag_type mth_flag,
						const Handler& handler)
	{
		typedef std::basic_string<Char> string_type;
		return this_type::match<Target,
								Char,
								std::char_traits<Char>,
								std::allocator<Char>,
								Handler>
								(target, string_type(str_regex), re_flag, mth_flag, handler);
	}

	template<typename Target,
				typename Char, typename Traits, typename Alloc,
				typename Handler,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static bool match(const Target& target, const Basic_String<Char, Traits, Alloc>& str_regex,
						regex_flag_type re_flag, match_flag_type mth_flag,
						const Handler& handler)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		BOOST_MPL_ASSERT((charset::is_not_utf8_string<Target>));
		BOOST_MPL_ASSERT((charset::is_not_utf8_string<string_type>));
		BOOST_MPL_ASSERT((boost::is_same<typename Target::value_type, Char>));

		typedef Target target_type;
		typedef typename target_type::const_iterator target_citer_type;

		typedef boost::basic_regex< Char, boost::regex_traits<Char> > regex_type;
		typedef boost::match_results< target_citer_type > match_rst_type;

		regex_type re(str_regex.begin(), str_regex.end(), re_flag);
		match_rst_type m;
		bool bret = boost::regex_match(target.begin(), target.end(), m, re, mth_flag);
		return handler(bret, m);
	}

//---------------------------------replace--------------------------------------

	template<typename Target, typename Char, typename Formatter >
	static Target replace(const Target& text, const Char* str_regex,
							const Formatter& str_fmt,
							regex_flag_type re_flag = regex_base_type::normal,
							replace_flag_type rl_flag = boost::match_default | boost::format_all)
	{
		typedef std::basic_string<Char> string_type;
		return this_type::replace<Target,
									Char,
									std::char_traits<Char>,
									std::allocator<Char>,
									Formatter,
									std::basic_string>
									(text, string_type(str_regex), str_fmt, re_flag, rl_flag);
	}

	template<typename Target,
				typename Char, typename Traits, typename Alloc,
				typename Formatter,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static Target replace(const Target& text, const Basic_String<Char, Traits, Alloc>& str_regex,
							const Formatter& str_fmt,
							regex_flag_type re_flag = regex_base_type::normal,
							replace_flag_type rl_flag = boost::match_default | boost::format_all)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		BOOST_MPL_ASSERT((charset::is_not_utf8_string<Target>));
		BOOST_MPL_ASSERT((charset::is_not_utf8_string<string_type>));
		BOOST_MPL_ASSERT((boost::is_same<typename Target::value_type, Char>));

		typedef Target target_type;
		typedef boost::basic_regex< Char, boost::regex_traits<Char> > regex_type;

		regex_type re(str_regex.begin(), str_regex.end(), re_flag);
		target_type result;
		boost::re_detail::string_out_iterator< target_type > i(result);
		boost::regex_replace(i, text.begin(), text.end(), re, str_fmt, rl_flag);
		return result;
	}

	template<typename Target, typename Char, typename Formatter, typename Handler>
	static Target replace(const Target& target, Char* str_regex,
								const Formatter& str_fmt,
								regex_flag_type re_flag, replace_flag_type rl_flag, const Handler& handler)
	{
		typedef std::basic_string<Char> string_type;
		return this_type::replace<Target,
									Char,
									std::char_traits<Char>,
									std::allocator<Char>,
									Formatter,
									Handler,
									std::basic_string>
									(target, string_type(str_regex), str_fmt, re_flag, rl_flag, handler);
	}

	template<typename Target,
				typename Char, typename Traits, typename Alloc,
				typename Formatter,
				typename Handler,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String>
	static Target replace(const Target& target, const Basic_String<Char, Traits, Alloc>& str_regex,
								const Formatter& str_fmt,
								regex_flag_type re_flag, replace_flag_type rl_flag, const Handler& handler)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		BOOST_MPL_ASSERT((charset::is_not_utf8_string<Target>));
		BOOST_MPL_ASSERT((charset::is_not_utf8_string<string_type>));
		BOOST_MPL_ASSERT((boost::is_same<typename Target::value_type, Char>));

		typedef Target target_type;
		typedef boost::basic_regex< Char, boost::regex_traits<Char> > regex_type;


		regex_type re(str_regex.begin(), str_regex.end(), re_flag);
		target_type result;
		boost::re_detail::string_out_iterator< target_type > i(result);
		boost::regex_replace(i, target.begin(), target.end(), re, str_fmt, rl_flag);
		return handler(result);

	}

	template<typename Out, typename Target, typename Char, typename Formatter>
	static Out& replace(Out& out, const Target& target, const Char* str_regex,
							const Formatter& str_fmt,
							regex_flag_type re_flag = regex_base_type::normal,
							replace_flag_type rl_flag = boost::match_default | boost::format_all)
	{
		typedef std::basic_string<Char> string_type;
		return this_type::replace<Out, Target,
									Char,
									std::char_traits<Char>,
									std::allocator<Char>,
									Formatter,
									std::basic_string>
									(out, target, string_type(str_regex), str_fmt, re_flag, rl_flag);
	}

	template<typename Out, typename Target,
				typename Char, typename Traits, typename Alloc,
				typename Formatter,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String>
	static Out& replace(Out& out, const Target& target, const Basic_String<Char, Traits, Alloc>& str_regex,
							const Formatter& str_fmt,
							regex_flag_type re_flag = regex_base_type::normal,
							replace_flag_type rl_flag = boost::match_default | boost::format_all)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		BOOST_MPL_ASSERT((charset::is_not_utf8_string<Target>));
		BOOST_MPL_ASSERT((charset::is_not_utf8_string<string_type>));
		BOOST_MPL_ASSERT((boost::is_same<typename Target::value_type, Char>));

		typedef Out out_type;
		typedef typename out_type::iterator out_iter_type;

		typedef Target target_type;
		typedef typename target_type::const_iterator target_citer_type;

		typedef boost::basic_regex< Char, boost::regex_traits<Char> > regex_type;

		out_iter_type oi(out);

		regex_type re(str_regex.begin(), str_regex.end(), re_flag);
		boost::regex_replace(oi, target.begin(), target.end(), re, str_fmt, rl_flag);
		return out;
	}

	template<typename Out, typename Target, typename Char, typename Formatter, typename Handler>
	static Out& replace(Out& out, const Target& target, const Char* str_regex,
							const Formatter& str_fmt,
							regex_flag_type re_flag, replace_flag_type rl_flag, const Handler& handler)
	{
		typedef std::basic_string<Char> string_type;
		return this_type::replace<Out, Target,
									Char,
									std::char_traits<Char>,
									std::allocator<Char>,
									Formatter,
									Handler,
									std::basic_string>
									(out, target, string_type(str_regex), str_fmt, re_flag, rl_flag, handler);
	}

	template<typename Out, typename Target,
				typename Char, typename Traits, typename Alloc,
				typename Formatter,
				typename Handler,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String>
	static Out& replace(Out& out, const Target& target, const Basic_String<Char, Traits, Alloc>& str_regex,
							const Formatter& str_fmt,
							regex_flag_type re_flag, replace_flag_type rl_flag, const Handler& handler)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		BOOST_MPL_ASSERT((charset::is_not_utf8_string<Target>));
		BOOST_MPL_ASSERT((charset::is_not_utf8_string<string_type>));
		BOOST_MPL_ASSERT((boost::is_same<typename Target::value_type, Char>));

		typedef Out out_type;
		typedef typename out_type::iterator out_iter_type;

		typedef Target target_type;
		typedef typename target_type::const_iterator target_citer_type;

		typedef boost::basic_regex< Char, boost::regex_traits<Char> > regex_type;

		out_iter_type oi(out);

		target_citer_type begin(target.begin());
		target_citer_type end(target.end());

		regex_type re(str_regex.begin(), str_regex.end(), re_flag);
		boost::regex_replace(oi, begin, end, re, str_fmt, rl_flag);
		return handler(out);
	}

//----------------------------------split--------------------------------
private:

	template<typename T1, typename T2>
	struct split_back_text
	{
		void operator()(T1& t1, const T2& t2)
		{
			t1.assign(t2.begin(), t2.end());
		}
	};

	template<typename T>
	struct split_back_text<T, T>
	{
		void operator()(T& t1, const T& t2)
		{
			return;
		}
	};

	template<typename T1, typename T2>
	struct split_init_text
	{
		T1 operator()(const T2& t)
		{
			return T1(t.begin(), t.end());
		}
	};

	template<typename T>
	struct split_init_text<T, T>
	{
		T& operator()(T& t)
		{
			return t;
		}
	};

public:
	template<typename Container, typename Text>
	static size_type split(Container& container, const Text& text)
	{
		typedef Text text_type;
		typedef typename text_type::value_type char_type;
		typedef std::basic_string<char_type> string_type;

		typedef Container container_type;
		typedef typename container_type::value_type cont_val_type;
		typedef typename cont_val_type::value_type cont_char_type;

		BOOST_MPL_ASSERT((charset::is_not_utf8_string<text_type>));
		BOOST_MPL_ASSERT((boost::is_same<char_type, cont_char_type>));

		string_type tmp(text.begin(), text.end());

		return boost::regex_split(std::back_inserter(container), tmp);
	}

	template<typename Container, typename Text>
	static size_type split(Container& container, const boost::reference_wrapper<Text>& text)
	{
		typedef Text text_type;
		typedef typename text_type::value_type char_type;
		typedef std::basic_string<char_type> string_type;

		typedef Container container_type;
		typedef typename container_type::value_type cont_val_type;
		typedef typename cont_val_type::value_type cont_char_type;

		typedef typename boost::mpl::if_<typename boost::is_same<string_type,
															text_type>::type,
											string_type&,
											string_type>::type use_text_type;

		BOOST_MPL_ASSERT((charset::is_not_utf8_string<text_type>));
		BOOST_MPL_ASSERT((boost::is_same<char_type, cont_char_type>));

		text_type& ref_text = text;
		use_text_type use_text = split_init_text<string_type, text_type>()(ref_text);

		size_type size = boost::regex_split(std::back_inserter(container), use_text);
		if(size)
		{
			split_back_text<text_type, string_type> back;
			back(ref_text, use_text);
		}
		return size;
	}

	template<typename Container, typename Text, typename Handler>
	static size_type split(Container& container, const Text& text, const Handler& handler)
	{
		typedef Text text_type;
		typedef typename text_type::value_type char_type;
		typedef std::basic_string<char_type> string_type;

		typedef Container container_type;
		typedef typename container_type::value_type cont_val_type;
		typedef typename cont_val_type::value_type cont_char_type;

		BOOST_MPL_ASSERT((charset::is_not_utf8_string<text_type>));
		BOOST_MPL_ASSERT((boost::is_same<char_type, cont_char_type>));

		string_type tmp(text.begin(), text.end());

		size_type size = boost::regex_split(std::back_inserter(container), tmp);
		return handler(container, size);

	}

	template<typename Container, typename Text, typename Handler>
	static size_type split(Container& container,
							const boost::reference_wrapper<Text>& text,
							const Handler& handler)
	{
		typedef Text text_type;
		typedef typename text_type::value_type char_type;
		typedef std::basic_string<char_type> string_type;

		typedef Container container_type;
		typedef typename container_type::value_type cont_val_type;
		typedef typename cont_val_type::value_type cont_char_type;

		typedef typename boost::mpl::if_<typename boost::is_same<string_type,
															text_type>::type,
									string_type&,
									string_type>::type use_text_type;

		BOOST_MPL_ASSERT((charset::is_not_utf8_string<text_type>));
		BOOST_MPL_ASSERT((boost::is_same<char_type, cont_char_type>));

		text_type& ref_text = text;
		use_text_type use_text = split_init_text<string_type, text_type>()(ref_text);

		size_type size = boost::regex_split(std::back_inserter(container), use_text);
		if(size)
		{
			split_back_text<text_type, string_type> back;
			back(ref_text, use_text);
		}

		return handler(container, size);
	}

public:
	template<typename Container, typename Text, typename Char>
	static size_type split(Container& container, const Text& text,
							const Char* str_regex,
							regex_flag_type re_flag = regex_base_type::normal | regex_base_type::icase,
							split_flag_type sp_flag = boost::match_default,
							size_type sp_size = std::numeric_limits<size_type>::max())
	{
		typedef std::basic_string<Char> string_type;
		return this_type::prv_split<Container, Text,
										Char, std::char_traits<Char>, std::allocator<Char>,
										std::basic_string>
				(container, text, string_type(str_regex), re_flag, sp_flag, sp_size);
	}

	template<typename Container, typename Text, typename Char>
	static size_type split(Container& container,  const boost::reference_wrapper<Text>& text,
							const Char* str_regex,
							regex_flag_type re_flag = regex_base_type::normal | regex_base_type::icase,
							split_flag_type sp_flag = boost::match_default,
							size_type sp_size = std::numeric_limits<size_type>::max())
	{
		typedef std::basic_string<Char> string_type;
		return this_type::prv_split<Container, Text,
										Char, std::char_traits<Char>, std::allocator<Char>,
										std::basic_string>
				(container, text, string_type(str_regex), re_flag, sp_flag, sp_size);
	}

	template<typename Container, typename Text,
				typename Char, typename Traits, typename Alloc>
	static size_type split(Container& container, const Text& text,
							const std::basic_string<Char, Traits, Alloc>& str_regex,
							regex_flag_type re_flag = regex_base_type::normal | regex_base_type::icase,
							split_flag_type sp_flag = boost::match_default,
							size_type sp_size = std::numeric_limits<size_type>::max())
	{
		return this_type::prv_split<Container, Text, Char, Traits, Alloc, std::basic_string>
				(container, text, str_regex, re_flag, sp_flag, sp_size);
	}

	template<typename Container, typename Text,
				typename Char, typename Traits, typename Alloc>
	static size_type split(Container& container, const boost::reference_wrapper<Text>& text,
							const std::basic_string<Char, Traits, Alloc>& str_regex,
							regex_flag_type re_flag = regex_base_type::normal | regex_base_type::icase,
							split_flag_type sp_flag = boost::match_default,
							size_type sp_size = std::numeric_limits<size_type>::max())
	{
		return this_type::prv_split<Container, Text, Char, Traits, Alloc, std::basic_string>
				(container, text, str_regex, re_flag, sp_flag, sp_size);
	}

	template<typename Container, typename Text,
				typename Char, typename Traits, typename Alloc>
	static size_type split(Container& container, const Text& text,
							const boost::container::basic_string<Char, Traits, Alloc>& str_regex,
							regex_flag_type re_flag = regex_base_type::normal | regex_base_type::icase,
							split_flag_type sp_flag = boost::match_default,
							size_type sp_size = std::numeric_limits<size_type>::max())
	{
		return this_type::prv_split<Container, Text, Char, Traits, Alloc, boost::container::basic_string>
				(container, text, str_regex, re_flag, sp_flag, sp_size);
	}

	template<typename Container, typename Text,
				typename Char, typename Traits, typename Alloc>
	static size_type split(Container& container, const boost::reference_wrapper<Text>& text,
							const boost::container::basic_string<Char, Traits, Alloc>& str_regex,
							regex_flag_type re_flag = regex_base_type::normal | regex_base_type::icase,
							split_flag_type sp_flag = boost::match_default,
							size_type sp_size = std::numeric_limits<size_type>::max())
	{
		return this_type::prv_split<Container, Text, Char, Traits, Alloc, boost::container::basic_string>
				(container, text, str_regex, re_flag, sp_flag, sp_size);
	}

private:
	template<typename Container, typename Text,
				typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static size_type prv_split(Container& container, const Text& text,
							const Basic_String<Char, Traits, Alloc>& str_regex,
							regex_flag_type re_flag,
							split_flag_type sp_flag,
							size_type sp_size)
	{
		typedef Text text_type;
		typedef typename text_type::value_type char_type;
		typedef std::basic_string<char_type> string_type;

		typedef Container container_type;
		typedef typename container_type::value_type cont_val_type;
		typedef typename cont_val_type::value_type cont_char_type;

		typedef Basic_String<Char, Traits, Alloc> string_regex_type;

		typedef boost::basic_regex< Char, boost::regex_traits<Char> > regex_type;

		BOOST_MPL_ASSERT((charset::is_not_utf8_string<string_regex_type>));
		BOOST_MPL_ASSERT((charset::is_not_utf8_string<text_type>));
		BOOST_MPL_ASSERT((boost::is_same<char_type, cont_char_type>));
		BOOST_MPL_ASSERT((boost::is_same<char_type, Char>));

		string_type tmp(text.begin(), text.end());


		regex_type re(str_regex.begin(), str_regex.end(), re_flag);
		return boost::regex_split(std::back_inserter(container), tmp, re, sp_flag);
	}

	template<typename Container, typename Text,
				typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String>
	static size_type prv_split(Container& container, const boost::reference_wrapper<Text>& text,
							const Basic_String<Char, Traits, Alloc>& str_regex,
							regex_flag_type re_flag,
							split_flag_type sp_flag,
							size_type sp_size)
	{

		typedef Text text_type;
		typedef typename text_type::value_type char_type;
		typedef std::basic_string<char_type> string_type;

		typedef Container container_type;
		typedef typename container_type::value_type cont_val_type;
		typedef typename cont_val_type::value_type cont_char_type;

		typedef Basic_String<Char, Traits, Alloc> string_regex_type;

		typedef boost::basic_regex< Char, boost::regex_traits<Char> > regex_type;

		typedef typename boost::mpl::if_<typename boost::is_same<string_type,
															text_type>::type,
											string_type&,
											string_type>::type use_text_type;

		BOOST_MPL_ASSERT((charset::is_not_utf8_string<string_regex_type>));
		BOOST_MPL_ASSERT((charset::is_not_utf8_string<text_type>));
		BOOST_MPL_ASSERT((boost::is_same<char_type, cont_char_type>));
		BOOST_MPL_ASSERT((boost::is_same<char_type, Char>));

		text_type& ref_text = text;
		use_text_type use_text = split_init_text<string_type, text_type>()(ref_text);

		regex_type re(str_regex.begin(), str_regex.end(), re_flag);
		size_type size = boost::regex_split(std::back_inserter(container), use_text, re, sp_flag);
		if(size)
		{
			split_back_text<text_type, string_type> back;
			back(ref_text, use_text);
		}
		return size;
	}

public:
	template<typename Container, typename Text, typename Char, typename Handler>
	static size_type split(Container& container, const Text& text,
							const Char* str_regex,
							regex_flag_type re_flag, split_flag_type sp_flag, size_type sp_size,
							const Handler& handler)
	{
		typedef std::basic_string<Char> string_type;
		return this_type::split<Container, Text,
									Char,
									std::char_traits<Char>,
									std::allocator<Char>,
									Handler,
									std::basic_string>
									(container, text, string_type(str_regex),
										re_flag, sp_flag, sp_size, handler);
	}

	template<typename Container, typename Text,
				typename Char, typename Traits, typename Alloc,
				typename Handler,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String>
	static size_type split(Container& container, const Text& text,
							const Basic_String<Char, Traits, Alloc>& str_regex,
							regex_flag_type re_flag, split_flag_type sp_flag, size_type sp_size,
							const Handler& handler)
	{
		typedef Text text_type;
		typedef typename text_type::value_type char_type;
		typedef std::basic_string<char_type> string_type;

		typedef Container container_type;
		typedef typename container_type::value_type cont_val_type;
		typedef typename cont_val_type::value_type cont_char_type;

		typedef Basic_String<Char, Traits, Alloc> string_regex_type;

		typedef boost::basic_regex< Char, boost::regex_traits<Char> > regex_type;

		BOOST_MPL_ASSERT((charset::is_not_utf8_string<string_regex_type>));
		BOOST_MPL_ASSERT((charset::is_not_utf8_string<text_type>));
		BOOST_MPL_ASSERT((boost::is_same<char_type, cont_char_type>));
		BOOST_MPL_ASSERT((boost::is_same<char_type, Char>));

		string_type tmp(text.begin(), text.end());

		regex_type re(str_regex.begin(), str_regex.end(), re_flag);
		size_type size = boost::regex_split(std::back_inserter(container), tmp, re, sp_flag);
		return handler(container, size);

	}

	template<typename Container, typename Text, typename Char, typename Handler>
	static size_type split(Container& container, const boost::reference_wrapper<Text>& text,
							const Char* str_regex,
							regex_flag_type re_flag, split_flag_type sp_flag, size_type sp_size,
							const Handler& handler)
	{
		typedef std::basic_string<Char> string_type;
		return this_type::split<Container, Text,
									Char,
									std::char_traits<Char>,
									std::allocator<Char>,
									Handler,
									std::basic_string>
									(container, text, string_type(str_regex),
										re_flag, sp_flag, sp_size, handler);

	}

	template<typename Container, typename Text,
				typename Char, typename Traits, typename Alloc,
				typename Handler,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static size_type split(Container& container, const boost::reference_wrapper<Text>& text,
							const Basic_String<Char, Traits, Alloc>& str_regex,
							regex_flag_type re_flag, split_flag_type sp_flag, size_type sp_size,
							const Handler& handler)
	{
		typedef Text text_type;
		typedef typename text_type::value_type char_type;
		typedef std::basic_string<char_type> string_type;

		typedef Container container_type;
		typedef typename container_type::value_type cont_val_type;
		typedef typename cont_val_type::value_type cont_char_type;

		typedef Basic_String<Char, Traits, Alloc> string_regex_type;

		typedef boost::basic_regex< Char, boost::regex_traits<Char> > regex_type;

		typedef typename boost::mpl::if_<typename boost::is_same<string_type,
															text_type>::type,
											string_type&,
											string_type>::type use_text_type;

		BOOST_MPL_ASSERT((charset::is_not_utf8_string<string_regex_type>));
		BOOST_MPL_ASSERT((charset::is_not_utf8_string<text_type>));
		BOOST_MPL_ASSERT((boost::is_same<char_type, cont_char_type>));
		BOOST_MPL_ASSERT((boost::is_same<char_type, Char>));

		text_type& ref_text = text;
		use_text_type use_text = split_init_text<string_type, text_type>()(ref_text);


		regex_type re(str_regex.begin(), str_regex.end(), re_flag);
		size_type size = boost::regex_split(std::back_inserter(container), use_text, re, sp_flag);
		if(size)
		{
			split_back_text<text_type, string_type> back;
			back(ref_text, use_text);
		}

		return handler(container, size);
	}

//------------------------------grep---------------------------------------

	template<typename Handler, typename Target, typename Char>
	static size_type grep(const Handler& handler, const Target& target,
							const Char* str_regex,
							regex_flag_type re_flag = regex_base_type::normal,
							grep_flag_type grep_flag = boost::match_default)
	{
		typedef std::basic_string<Char> string_type;
		return this_type::grep<Handler, Target,
								Char,
								std::char_traits<Char>,
								std::allocator<Char>,
								std::basic_string >
								(handler, target, string_type(str_regex),
									re_flag, grep_flag);
	}

	template<typename Handler, typename Target,
				typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static size_type grep(const Handler& handler, const Target& target,
							const Basic_String<Char, Traits, Alloc>& str_regex,
							regex_flag_type re_flag = regex_base_type::normal,
							grep_flag_type grep_flag = boost::match_default)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		BOOST_MPL_ASSERT((charset::is_not_utf8_string<Target>));
		BOOST_MPL_ASSERT((charset::is_not_utf8_string<string_type>));
		BOOST_MPL_ASSERT((boost::is_same<typename Target::value_type, Char>));

		typedef Target target_type;
		typedef boost::basic_regex< Char, boost::regex_traits<Char> > regex_type;

		regex_type re(str_regex.begin(), str_regex.end(), re_flag);
		return boost::regex_grep(handler, target.begin(), target.end(), re, grep_flag);
	}
};

} // namespace regular_parse
} // namespace yggr

#endif //__YGGR_REGULAR_PARSE_REGEX_PARSE_HPP__
