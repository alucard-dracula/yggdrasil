// utf8_regex_parse_test_part1.cpp

#if !defined(_MSC_VER)
#	error "this file supports MSVC only, if you are using other compilers, please use the same name file in the charset_utf8_files"	
#endif // _MSC_VER

//#ifdef __MINGW32__
//#   error "mingw not support this file!!"
//#endif // __MINGW32__

#define BOOST_HAS_ICU

#include <test/wait_any_key/wait_any_key.hpp>
#include <vector>
#include <utility>
#include <sstream>
#include <cassert>

#include <boost/bind.hpp>

#include <yggr/charset/string.hpp>
#include <yggr/charset/utf8_string.hpp>
#include <yggr/regular_parse/u32regex_parse.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(regular_parse)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

template<typename Char>
typename boost::enable_if<boost::mpl::bool_<(sizeof(Char) == sizeof(char))>, std::basic_ostream<Char>& >::type
	get_ostream(void)
{
	return std::cout;
}

#ifndef YGGR_NO_CWCHAR

template<typename Char>
typename boost::enable_if<boost::mpl::bool_<(sizeof(Char) == sizeof(wchar_t))>, std::basic_ostream<Char>& >::type
	get_ostream(void)
{
	return std::wcout;
}

#endif // YGGR_NO_CWCHAR

void test_utf8_match(void)
{
	{
		yggr::utf8_string text("hello,world");
		yggr::utf8_string str_re_right("^h.*");
		yggr::utf8_string str_re_faild("^kh.*");
		try
		{
			bool bret = false;
			// right test
			bret = 
				yggr::regular_parse::u32regex_parse::match(
					text,
					yggr::regular_parse::u32regex_parse::make_regex(str_re_right));

			yggr_test_assert(bret);

			bret = 
				yggr::regular_parse::u32regex_parse::match(
					text.begin(), text.end(),
					yggr::regular_parse::u32regex_parse::make_regex(str_re_right.begin(), str_re_right.end()));

			yggr_test_assert(bret);

			bret = 
				yggr::regular_parse::u32regex_parse::match(
					text.c_str(),
					yggr::regular_parse::u32regex_parse::make_regex(str_re_right.c_str()));

			yggr_test_assert(bret);

			bret = 
				yggr::regular_parse::u32regex_parse::match(
					"hello,world",
					yggr::regular_parse::u32regex_parse::make_regex("^h.*"));

			yggr_test_assert(bret);

			// faild test
			bret = 
				yggr::regular_parse::u32regex_parse::match(
					text,
					yggr::regular_parse::u32regex_parse::make_regex(str_re_faild));

			yggr_test_assert(!bret);

			bret = 
				yggr::regular_parse::u32regex_parse::match(
					text.begin(), text.end(),
					yggr::regular_parse::u32regex_parse::make_regex(str_re_faild.begin(), str_re_faild.end()));

			yggr_test_assert(!bret);

			bret = 
				yggr::regular_parse::u32regex_parse::match(
					text.c_str(),
					yggr::regular_parse::u32regex_parse::make_regex(str_re_faild.c_str()));

			yggr_test_assert(!bret);

			bret = 
				yggr::regular_parse::u32regex_parse::match(
					"hello,world",
					yggr::regular_parse::u32regex_parse::make_regex("^kh.*"));

			yggr_test_assert(!bret);
		}
		catch(const yggr::regular_parse::u32regex_parse::error_type& e)
		{
			std::cout << e.what() << std::endl;
			yggr_test_assert(false);
		}
		catch(const std::exception& e)
		{
		    std::cout << e.what() << std::endl;
		    yggr_test_assert(false);
		}
		catch(...)
		{
			yggr_test_assert(false);
		}
	}

// this condition is compiled only for the text encoding and the specific function has nothing to do, 
//	in this file the definition of the same semantics
#ifndef YGGR_NO_CWCHAR 
	{
		yggr::utf8_string text(L"你好,世界");
		yggr::utf8_string str_re_right(L"^你.*");
		yggr::utf8_string str_re_faild(L"^k你.*");
		try
		{
			bool bret = false;
			// right test
			bret = 
				yggr::regular_parse::u32regex_parse::match(
					text,
					yggr::regular_parse::u32regex_parse::make_regex(str_re_right));

			yggr_test_assert(bret);

			bret = 
				yggr::regular_parse::u32regex_parse::match(
					text.begin(), text.end(),
					yggr::regular_parse::u32regex_parse::make_regex(str_re_right.begin(), str_re_right.end()));

			yggr_test_assert(bret);

			bret = 
				yggr::regular_parse::u32regex_parse::match(
					text.c_str(),
					yggr::regular_parse::u32regex_parse::make_regex(str_re_right.c_str()));

			yggr_test_assert(bret);

			bret = 
				yggr::regular_parse::u32regex_parse::match(
					L"你好,世界",
					yggr::regular_parse::u32regex_parse::make_regex(L"^你.*"));

			yggr_test_assert(bret);

			// faild test
			bret = 
				yggr::regular_parse::u32regex_parse::match(
					text,
					yggr::regular_parse::u32regex_parse::make_regex(str_re_faild));

			yggr_test_assert(!bret);

			bret = 
				yggr::regular_parse::u32regex_parse::match(
					text.begin(), text.end(),
					yggr::regular_parse::u32regex_parse::make_regex(str_re_faild.begin(), str_re_faild.end()));

			yggr_test_assert(!bret);

			bret = 
				yggr::regular_parse::u32regex_parse::match(
					text.c_str(),
					yggr::regular_parse::u32regex_parse::make_regex(str_re_faild.c_str()));

			yggr_test_assert(!bret);

			bret = 
				yggr::regular_parse::u32regex_parse::match(
					L"你好,世界",
					yggr::regular_parse::u32regex_parse::make_regex(L"^k你.*"));
		}
		catch(const yggr::regular_parse::u32regex_parse::error_type& e)
		{
			std::cout << e.what() << std::endl;
			yggr_test_assert(false);
		}
		catch(const std::exception& e)
		{
		    std::cout << e.what() << std::endl;
		    yggr_test_assert(false);
		}
		catch(...)
		{
			yggr_test_assert(false);
		}
	}

#endif // YGGR_NO_CWCHAR
	std::cout << "----------test_utf8_match succeed-----------" << std::endl;
}

template<typename Iter, typename Alloc>
void handler_match(bool bret, const boost::match_results<Iter, Alloc>& m)
{
	typedef typename boost::iterator_value<Iter>::type char_type;
	typedef typename
		boost::mpl::if_c
		<
			sizeof(char_type) == 1,
			yggr::charset::charset_name_t<yggr::utf8_string>,
			yggr::charset::charset_name_t<char_type>
		>::type charset_name_type;

	if(!bret)
	{
		get_ostream<char_type>() << "!!!empty match!!!" << std::endl;
	}
	else
	{
		for(int i = 0, isize = static_cast<int>(m.size()); i < isize; ++i)
		{
			get_ostream<char_type>() 
				<< "i = " << i 
				<< " : " 
				<< yggr::utf8_string(m[i].first, m[i].second, charset_name_type()) << std::endl;
		}
	}
}

void test_utf8_match2(void)
{
	{
		yggr::utf8_string text("hello,world");
		yggr::utf8_string str_re_right("^h.*");
		try
		{
			bool bret = false;
			
			{
				typedef yggr::regular_parse::match_results_t_maker<yggr::utf8_string::const_iterator>::type result_type;

				result_type rst;
				bret = 
					yggr::regular_parse::u32regex_parse::match(
						text, rst,
						yggr::regular_parse::u32regex_parse::make_regex(str_re_right));

				yggr_test_assert(bret);
				handler_match(bret, rst);
			}

			{
				typedef yggr::regular_parse::match_results_t_maker<yggr::utf8_string::iterator>::type result_type;

				result_type rst;

				bret = 
					yggr::regular_parse::u32regex_parse::match(
						text.begin(), text.end(), rst,
						yggr::regular_parse::u32regex_parse::make_regex(str_re_right.begin(), str_re_right.end()));

				yggr_test_assert(bret);
				handler_match(bret, rst);
			}

			{
				typedef yggr::regular_parse::match_results_t_maker<const char*>::type result_type;

				result_type rst;

				bret = 
					yggr::regular_parse::u32regex_parse::match(
						text.c_str(), rst,
						yggr::regular_parse::u32regex_parse::make_regex(str_re_right.c_str()));

				yggr_test_assert(bret);
				handler_match(bret, rst);
			}

			{
				typedef yggr::regular_parse::match_results_t_maker<const char*>::type result_type;

				result_type rst;
				bret = 
					yggr::regular_parse::u32regex_parse::match(
						"hello,world", rst,
						yggr::regular_parse::u32regex_parse::make_regex("^h.*"));

				yggr_test_assert(bret);
				handler_match(bret, rst);
			}
		}
		catch(const yggr::regular_parse::u32regex_parse::error_type& e)
		{
			std::cout << e.what() << std::endl;
			yggr_test_assert(false);
		}
		catch(const std::exception& e)
		{
		    std::cout << e.what() << std::endl;
		    yggr_test_assert(false);
		}
		catch(...)
		{
			yggr_test_assert(false);
		}
	}

#ifndef YGGR_NO_CWCHAR 

	{
		yggr::utf8_string text(L"你好,世界");
		yggr::utf8_string str_re_right(L"^你.*");
		try
		{
			bool bret = false;
			
			{
				typedef yggr::regular_parse::match_results_t_maker<yggr::utf8_string::const_iterator>::type result_type;

				result_type rst;
				bret = 
					yggr::regular_parse::u32regex_parse::match(
						text, rst,
						yggr::regular_parse::u32regex_parse::make_regex(str_re_right));

				yggr_test_assert(bret);
				handler_match(bret, rst);
			}

			{
				typedef yggr::regular_parse::match_results_t_maker<yggr::utf8_string::iterator>::type result_type;

				result_type rst;

				bret = 
					yggr::regular_parse::u32regex_parse::match(
						text.begin(), text.end(), rst,
						yggr::regular_parse::u32regex_parse::make_regex(str_re_right.begin(), str_re_right.end()));

				yggr_test_assert(bret);
				handler_match(bret, rst);
			}

			{
				typedef yggr::regular_parse::match_results_t_maker<const char*>::type result_type;

				result_type rst;
				bret = 
					yggr::regular_parse::u32regex_parse::match(
						text.c_str(), rst,
						yggr::regular_parse::u32regex_parse::make_regex(str_re_right.c_str()));

				yggr_test_assert(bret);
				handler_match(bret, rst);
			}

			{
				typedef yggr::regular_parse::match_results_t_maker<const wchar_t*>::type result_type;

				result_type rst;
				bret = 
					yggr::regular_parse::u32regex_parse::match(
						L"你好,世界", rst,
						yggr::regular_parse::u32regex_parse::make_regex(L"^你.*"));

				yggr_test_assert(bret);
				handler_match(bret, rst);
			}
		}
		catch(const yggr::regular_parse::u32regex_parse::error_type& e)
		{
			std::cout << e.what() << std::endl;
			yggr_test_assert(false);
		}
		catch(const std::exception& e)
		{
		    std::cout << e.what() << std::endl;
		    yggr_test_assert(false);
		}
		catch(...)
		{
			yggr_test_assert(false);
		}
	}

#endif // YGGR_NO_CWCHAR
	
	std::cout << "----------test_utf8_match2 succeed-----------" << std::endl;
}

void test_utf8_match3(void)
{
	{
		yggr::utf8_string text("hello,world");
		std::vector< yggr::utf8_string::value_type> stl_vt(text.c_str(), text.c_str() + text.size());
		yggr::vector<yggr::utf8_string::value_type> yggr_vt(text.c_str(), text.c_str() + text.size());

		{
			yggr::utf8_string str_re("^h.*");
		
			try
			{
				yggr_test_assert(yggr::regular_parse::u32regex_parse::match(
						stl_vt, 
						yggr::regular_parse::u32regex_parse::make_regex(
							str_re)) == 1);

				yggr_test_assert(yggr::regular_parse::u32regex_parse::match(
						yggr_vt, 
						yggr::regular_parse::u32regex_parse::make_regex(
							str_re)) == 1);
			}
			catch(const yggr::regular_parse::u32regex_parse::error_type& e)
			{
				std::cout << e.what() << std::endl;
				yggr_test_assert(false);
			}
			catch(const std::exception& e)
			{
				std::cout << e.what() << std::endl;
				yggr_test_assert(false);
			}
			catch(...)
			{
				yggr_test_assert(false);
			}
		}

		{
			yggr::utf8_string str_re("^kh.*");
			try
			{
				yggr_test_assert(yggr::regular_parse::u32regex_parse::match(
						stl_vt, 
						yggr::regular_parse::u32regex_parse::make_regex(
							str_re)) == 0);

				yggr_test_assert(yggr::regular_parse::u32regex_parse::match(
						yggr_vt, 
						yggr::regular_parse::u32regex_parse::make_regex(
							str_re)) == 0);
			}
			catch(const yggr::regular_parse::u32regex_parse::error_type& e)
			{
				std::cout << e.what() << std::endl;
				yggr_test_assert(false);
			}
			catch(const std::exception& e)
			{
				std::cout << e.what() << std::endl;
				yggr_test_assert(false);
			}
			catch(...)
			{
				yggr_test_assert(false);
			}
		}
	}
#ifndef YGGR_NO_CWCHAR
	{
		yggr::utf8_string text(L"你好,世界");
		std::vector< yggr::utf8_string::value_type> stl_vt(text.c_str(), text.c_str() + text.size());
		yggr::vector<yggr::utf8_string::value_type> yggr_vt(text.c_str(), text.c_str() + text.size());

		{
			yggr::utf8_string str_re(L"你.*");
		
			try
			{
				yggr_test_assert(yggr::regular_parse::u32regex_parse::match(
						stl_vt, 
						yggr::regular_parse::u32regex_parse::make_regex(
							str_re)) == 1);

				yggr_test_assert(yggr::regular_parse::u32regex_parse::match(
						yggr_vt, 
						yggr::regular_parse::u32regex_parse::make_regex(
							str_re)) == 1);
			}
			catch(const yggr::regular_parse::u32regex_parse::error_type& e)
			{
				std::cout << e.what() << std::endl;
				yggr_test_assert(false);
			}
			catch(const std::exception& e)
			{
				std::cout << e.what() << std::endl;
				yggr_test_assert(false);
			}
			catch(...)
			{
				yggr_test_assert(false);
			}
		}

		{
			yggr::utf8_string str_re(L"^k你.*");
			try
			{
				yggr_test_assert(yggr::regular_parse::u32regex_parse::match(
						stl_vt, 
						yggr::regular_parse::u32regex_parse::make_regex(
							str_re)) == 0);

				yggr_test_assert(yggr::regular_parse::u32regex_parse::match(
						yggr_vt, 
						yggr::regular_parse::u32regex_parse::make_regex(
							str_re)) == 0);
			}
			catch(const yggr::regular_parse::u32regex_parse::error_type& e)
			{
				std::cout << e.what() << std::endl;
				yggr_test_assert(false);
			}
			catch(const std::exception& e)
			{
				std::cout << e.what() << std::endl;
				yggr_test_assert(false);
			}
			catch(...)
			{
				yggr_test_assert(false);
			}
		}
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "----------test_utf8_match3 succeed-----------" << std::endl;
}

void test_utf8_match4(void)
{
	yggr::utf8_string str_re("[[:alpha:]]*");
	{
		yggr::utf8_string vt;
		vt.push_back('a');
		vt.push_back('b');
		vt.push_back('c');

		try
		{
			typedef yggr::regular_parse::match_results_t_maker<yggr::utf8_string::const_iterator>::type result_type;

			result_type rst;
			bool bret = 
				yggr::regular_parse::u32regex_parse::match(
					vt, rst, yggr::regular_parse::u32regex_parse::make_regex(str_re));
			yggr_test_assert(bret);
			handler_match(bret, rst);
		}
		catch(const yggr::regular_parse::u32regex_parse::error_type& e)
		{
			std::cout << e.what() << std::endl;
		}
		catch(const std::exception& e)
		{
			std::cout << e.what() << std::endl;
			yggr_test_assert(false);
		}
		catch(...)
		{
			yggr_test_assert(false);
		}
	}

	{
		yggr::utf8_string vt;
		vt.push_back('a');
		vt.push_back('b');
		vt.push_back('c');
		vt.push_back('0'); // result false

		try
		{
			typedef yggr::regular_parse::match_results_t_maker<yggr::utf8_string::const_iterator>::type result_type;

			result_type rst;
			bool bret = 
				yggr::regular_parse::u32regex_parse::match(
					vt, rst, yggr::regular_parse::u32regex_parse::make_regex(str_re));
			yggr_test_assert(!bret);
			handler_match(bret, rst);
		}
		catch(const yggr::regular_parse::u32regex_parse::error_type& e)
		{
			std::cout << e.what() << std::endl;
		}
		catch(const std::exception& e)
		{
			std::cout << e.what() << std::endl;
			yggr_test_assert(false);
		}
		catch(...)
		{
			yggr_test_assert(false);
		}
	}

	std::cout << "-------------test_match4 end------------" << std::endl;
}

void test_utf8_search(void)
{
	{
		yggr::utf8_string text("hello,world");
		yggr::utf8_string str_re_right("^h.*");
		yggr::utf8_string str_re_faild("^kh.*");
		try
		{
			bool bret = false;
			// right test
			bret = 
				yggr::regular_parse::u32regex_parse::search(
					text,
					yggr::regular_parse::u32regex_parse::make_regex(str_re_right));

			yggr_test_assert(bret);

			bret = 
				yggr::regular_parse::u32regex_parse::search(
					text.begin(), text.end(),
					yggr::regular_parse::u32regex_parse::make_regex(str_re_right.begin(), str_re_right.end()));

			yggr_test_assert(bret);

			bret = 
				yggr::regular_parse::u32regex_parse::search(
					text.c_str(),
					yggr::regular_parse::u32regex_parse::make_regex(str_re_right.c_str()));

			yggr_test_assert(bret);

			bret = 
				yggr::regular_parse::u32regex_parse::search(
					"hello,world",
					yggr::regular_parse::u32regex_parse::make_regex("^h.*"));

			yggr_test_assert(bret);

			// faild test
			bret = 
				yggr::regular_parse::u32regex_parse::search(
					text,
					yggr::regular_parse::u32regex_parse::make_regex(str_re_faild));

			yggr_test_assert(!bret);

			bret = 
				yggr::regular_parse::u32regex_parse::search(
					text.begin(), text.end(),
					yggr::regular_parse::u32regex_parse::make_regex(str_re_faild.begin(), str_re_faild.end()));

			yggr_test_assert(!bret);

			bret = 
				yggr::regular_parse::u32regex_parse::search(
					text.c_str(),
					yggr::regular_parse::u32regex_parse::make_regex(str_re_faild.c_str()));

			yggr_test_assert(!bret);

			bret = 
				yggr::regular_parse::u32regex_parse::search(
					"hello,world",
					yggr::regular_parse::u32regex_parse::make_regex("^kh.*"));

			yggr_test_assert(!bret);
		}
		catch(const yggr::regular_parse::u32regex_parse::error_type& e)
		{
			std::cout << e.what() << std::endl;
			yggr_test_assert(false);
		}
		catch(const std::exception& e)
		{
			std::cout << e.what() << std::endl;
			yggr_test_assert(false);
		}
		catch(...)
		{
			yggr_test_assert(false);
		}
	}

// this condition is compiled only for the text encoding and the specific function has nothing to do, 
//	in this file the definition of the same semantics
#ifndef YGGR_NO_CWCHAR 
	{
		yggr::utf8_string text(L"你好,世界");
		yggr::utf8_string str_re_right(L"^你.*");
		yggr::utf8_string str_re_faild(L"^k你.*");
		try
		{
			bool bret = false;
			// right test
			bret = 
				yggr::regular_parse::u32regex_parse::search(
					text,
					yggr::regular_parse::u32regex_parse::make_regex(str_re_right));

			yggr_test_assert(bret);

			bret = 
				yggr::regular_parse::u32regex_parse::search(
					text.begin(), text.end(),
					yggr::regular_parse::u32regex_parse::make_regex(str_re_right.begin(), str_re_right.end()));

			yggr_test_assert(bret);

			bret = 
				yggr::regular_parse::u32regex_parse::search(
					text.c_str(),
					yggr::regular_parse::u32regex_parse::make_regex(str_re_right.c_str()));

			yggr_test_assert(bret);

			bret = 
				yggr::regular_parse::u32regex_parse::search(
					L"你好,世界",
					yggr::regular_parse::u32regex_parse::make_regex(L"^你.*"));

			yggr_test_assert(bret);

			// faild test
			bret = 
				yggr::regular_parse::u32regex_parse::search(
					text,
					yggr::regular_parse::u32regex_parse::make_regex(str_re_faild));

			yggr_test_assert(!bret);

			bret = 
				yggr::regular_parse::u32regex_parse::search(
					text.begin(), text.end(),
					yggr::regular_parse::u32regex_parse::make_regex(str_re_faild.begin(), str_re_faild.end()));

			yggr_test_assert(!bret);

			bret = 
				yggr::regular_parse::u32regex_parse::search(
					text.c_str(),
					yggr::regular_parse::u32regex_parse::make_regex(str_re_faild.c_str()));

			yggr_test_assert(!bret);

			bret = 
				yggr::regular_parse::u32regex_parse::search(
					L"你好,世界",
					yggr::regular_parse::u32regex_parse::make_regex(L"^k你.*"));
		}
		catch(const yggr::regular_parse::u32regex_parse::error_type& e)
		{
			std::cout << e.what() << std::endl;
			yggr_test_assert(false);
		}
		catch(const std::exception& e)
		{
			std::cout << e.what() << std::endl;
			yggr_test_assert(false);
		}
		catch(...)
		{
			yggr_test_assert(false);
		}
	}

#endif // YGGR_NO_CWCHAR
	std::cout << "----------test_utf8_search succeed-----------" << std::endl;
}

template<typename Iter, typename Alloc>
void handler_search(bool bret, const boost::match_results<Iter, Alloc>& m)
{
	typedef typename boost::iterator_value<Iter>::type char_type;
	typedef typename
		boost::mpl::if_c
		<
			sizeof(char_type) == 1,
			yggr::charset::charset_name_t<yggr::utf8_string>,
			yggr::charset::charset_name_t<char_type>
		>::type charset_name_type;

	if(!bret)
	{
		get_ostream<char_type>() << "!!!empty match!!!" << std::endl;
	}
	else
	{
		for(int i = 0, isize = static_cast<int>(m.size()); i < isize; ++i)
		{
			get_ostream<char_type>() 
				<< "i = " << i 
				<< " : " 
				<< yggr::utf8_string(m[i].first, m[i].second, charset_name_type()) << std::endl;
		}
	}
}

void test_utf8_search2(void)
{
	{
		yggr::utf8_string text("hello,world");
		yggr::utf8_string str_re_right("^h.*");
		try
		{
			bool bret = false;
			
			{
				typedef yggr::regular_parse::match_results_t_maker<yggr::utf8_string::const_iterator>::type result_type;
				result_type rst;
				bret = 
					yggr::regular_parse::u32regex_parse::search(
						text, rst,
						yggr::regular_parse::u32regex_parse::make_regex(str_re_right));

				yggr_test_assert(bret);
				handler_search(bret, rst);
			}

			{
				typedef yggr::regular_parse::match_results_t_maker<yggr::utf8_string::iterator>::type result_type;
				
				result_type rst;
				bret = 
					yggr::regular_parse::u32regex_parse::search(
						text.begin(), text.end(), rst,
						yggr::regular_parse::u32regex_parse::make_regex(str_re_right.begin(), str_re_right.end()));

				yggr_test_assert(bret);
				handler_search(bret, rst);
			}

			{
				typedef yggr::regular_parse::match_results_t_maker<const char*>::type result_type;

				result_type rst;
				bret = 
					yggr::regular_parse::u32regex_parse::search(
						text.c_str(), rst,
						yggr::regular_parse::u32regex_parse::make_regex(str_re_right.c_str()));

				yggr_test_assert(bret);
				handler_search(bret, rst);
			}

			{
				typedef yggr::regular_parse::match_results_t_maker<const char*>::type result_type;

				result_type rst;
				bret = 
					yggr::regular_parse::u32regex_parse::search(
						"hello,world", rst,
						yggr::regular_parse::u32regex_parse::make_regex("^h.*"));

				yggr_test_assert(bret);
				handler_search(bret, rst);
			}
		}
		catch(const yggr::regular_parse::u32regex_parse::error_type& e)
		{
			std::cout << e.what() << std::endl;
			yggr_test_assert(false);
		}
		catch(const std::exception& e)
		{
			std::cout << e.what() << std::endl;
			yggr_test_assert(false);
		}
		catch(...)
		{
			yggr_test_assert(false);
		}
	}

#ifndef YGGR_NO_CWCHAR 

	{
		yggr::utf8_string text(L"你好,世界");
		yggr::utf8_string str_re_right(L"^你.*");
		try
		{
			bool bret = false;
			
			{
				typedef yggr::regular_parse::match_results_t_maker<yggr::utf8_string::const_iterator>::type result_type;

				result_type rst;
				bret = 
					yggr::regular_parse::u32regex_parse::search(
						text, rst,
						yggr::regular_parse::u32regex_parse::make_regex(str_re_right));

				yggr_test_assert(bret);
				handler_search(bret, rst);
			}

			{
				typedef yggr::regular_parse::match_results_t_maker<yggr::utf8_string::iterator>::type result_type;

				result_type rst;
				bret = 
					yggr::regular_parse::u32regex_parse::search(
						text.begin(), text.end(), rst,
						yggr::regular_parse::u32regex_parse::make_regex(str_re_right.begin(), str_re_right.end()));

				yggr_test_assert(bret);
				handler_search(bret, rst);
			}

			{
				typedef yggr::regular_parse::match_results_t_maker<const char*>::type result_type;

				result_type rst;
				bret = 
					yggr::regular_parse::u32regex_parse::search(
						text.c_str(), rst,
						yggr::regular_parse::u32regex_parse::make_regex(str_re_right.c_str()));

				yggr_test_assert(bret);
				handler_search(bret, rst);
			}

			{
				typedef yggr::regular_parse::match_results_t_maker<const wchar_t*>::type result_type;

				result_type rst;
				bret = 
					yggr::regular_parse::u32regex_parse::search(
						L"你好,世界", rst,
						yggr::regular_parse::u32regex_parse::make_regex(L"^你.*"));

				yggr_test_assert(bret);
				handler_search(bret, rst);
			}
		}
		catch(const yggr::regular_parse::u32regex_parse::error_type& e)
		{
			std::cout << e.what() << std::endl;
			yggr_test_assert(false);
		}
		catch(const std::exception& e)
		{
			std::cout << e.what() << std::endl;
			yggr_test_assert(false);
		}
		catch(...)
		{
			yggr_test_assert(false);
		}
	}

#endif // YGGR_NO_CWCHAR
	
	std::cout << "----------test_utf8_search2 succeed-----------" << std::endl;
}

void test_utf8_search3(void)
{
	{
		yggr::utf8_string text("hello,world");
		std::vector< yggr::utf8_string::value_type> stl_vt(text.c_str(), text.c_str() + text.size());
		yggr::vector<yggr::utf8_string::value_type> yggr_vt(text.c_str(), text.c_str() + text.size());

		{
			yggr::utf8_string str_re("^h.*");
		
			try
			{
				yggr_test_assert(yggr::regular_parse::u32regex_parse::search(
						stl_vt, 
						yggr::regular_parse::u32regex_parse::make_regex(
							str_re)) == 1);

				yggr_test_assert(yggr::regular_parse::u32regex_parse::search(
						yggr_vt, 
						yggr::regular_parse::u32regex_parse::make_regex(
							str_re)) == 1);
			}
			catch(const yggr::regular_parse::u32regex_parse::error_type& e)
			{
				std::cout << e.what() << std::endl;
				yggr_test_assert(false);
			}
			catch(const std::exception& e)
			{
				std::cout << e.what() << std::endl;
				yggr_test_assert(false);
			}
			catch(...)
			{
				yggr_test_assert(false);
			}
		}

		{
			yggr::utf8_string str_re("^kh.*");
			try
			{
				yggr_test_assert(yggr::regular_parse::u32regex_parse::search(
						stl_vt, 
						yggr::regular_parse::u32regex_parse::make_regex(
							str_re)) == 0);

				yggr_test_assert(yggr::regular_parse::u32regex_parse::search(
						yggr_vt, 
						yggr::regular_parse::u32regex_parse::make_regex(
							str_re)) == 0);
			}
			catch(const yggr::regular_parse::u32regex_parse::error_type& e)
			{
				std::cout << e.what() << std::endl;
				yggr_test_assert(false);
			}
			catch(const std::exception& e)
			{
				std::cout << e.what() << std::endl;
				yggr_test_assert(false);
			}
			catch(...)
			{
				yggr_test_assert(false);
			}
		}
	}
#ifndef YGGR_NO_CWCHAR
	{
		yggr::utf8_string text(L"你好,世界");
		std::vector< yggr::utf8_string::value_type> stl_vt(text.c_str(), text.c_str() + text.size());
		yggr::vector<yggr::utf8_string::value_type> yggr_vt(text.c_str(), text.c_str() + text.size());

		{
			yggr::utf8_string str_re(L"你.*");
		
			try
			{
				yggr_test_assert(yggr::regular_parse::u32regex_parse::search(
						stl_vt, 
						yggr::regular_parse::u32regex_parse::make_regex(
							str_re)) == 1);

				yggr_test_assert(yggr::regular_parse::u32regex_parse::search(
						yggr_vt, 
						yggr::regular_parse::u32regex_parse::make_regex(
							str_re)) == 1);
			}
			catch(const yggr::regular_parse::u32regex_parse::error_type& e)
			{
				std::cout << e.what() << std::endl;
				yggr_test_assert(false);
			}
			catch(const std::exception& e)
			{
				std::cout << e.what() << std::endl;
				yggr_test_assert(false);
			}
			catch(...)
			{
				yggr_test_assert(false);
			}
		}

		{
			yggr::utf8_string str_re(L"^k你.*");
			try
			{
				yggr_test_assert(yggr::regular_parse::u32regex_parse::search(
						stl_vt, 
						yggr::regular_parse::u32regex_parse::make_regex(
							str_re)) == 0);

				yggr_test_assert(yggr::regular_parse::u32regex_parse::search(
						yggr_vt, 
						yggr::regular_parse::u32regex_parse::make_regex(
							str_re)) == 0);
			}
			catch(const yggr::regular_parse::u32regex_parse::error_type& e)
			{
				std::cout << e.what() << std::endl;
				yggr_test_assert(false);
			}
			catch(const std::exception& e)
			{
				std::cout << e.what() << std::endl;
				yggr_test_assert(false);
			}
			catch(...)
			{
				yggr_test_assert(false);
			}
		}
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "----------test_utf8_search3 succeed-----------" << std::endl;
}

void test_utf8_search4(void)
{
	yggr::utf8_string str_re("[[:alpha:]]*");
	{
		yggr::utf8_string vt;
		vt.push_back('a');
		vt.push_back('b');
		vt.push_back('c');

		try
		{
			typedef yggr::regular_parse::match_results_t_maker<yggr::utf8_string::const_iterator>::type result_type;

			result_type rst;
			bool bret = 
				yggr::regular_parse::u32regex_parse::search(
					vt, rst, yggr::regular_parse::u32regex_parse::make_regex(str_re));
			yggr_test_assert(bret);
			handler_search(bret, rst);
		}
		catch(const yggr::regular_parse::u32regex_parse::error_type& e)
		{
			std::cout << e.what() << std::endl;
		}
		catch(const std::exception& e)
		{
			std::cout << e.what() << std::endl;
			yggr_test_assert(false);
		}
		catch(...)
		{
			yggr_test_assert(false);
		}
	}

	{
		yggr::utf8_string vt;
		vt.push_back('a');
		vt.push_back('b');
		vt.push_back('c');
		vt.push_back('0');

		try
		{
			typedef yggr::regular_parse::match_results_t_maker<yggr::utf8_string::const_iterator>::type result_type;
			result_type rst;
			bool bret = 
				yggr::regular_parse::u32regex_parse::search(
					vt, rst, yggr::regular_parse::u32regex_parse::make_regex(str_re));
			yggr_test_assert(bret);
			handler_search(bret, rst);
		}
		catch(const yggr::regular_parse::u32regex_parse::error_type& e)
		{
			std::cout << e.what() << std::endl;
		}
		catch(const std::exception& e)
		{
			std::cout << e.what() << std::endl;
			yggr_test_assert(false);
		}
		catch(...)
		{
			yggr_test_assert(false);
		}
	}

	std::cout << "-------------test_utf8_search4 end------------" << std::endl;
}

// ------------------------------test replace---------------------------------
void test_utf8_replace1(void)
{
	{
		yggr::utf8_string str_re("(a)|(b)");
		yggr::utf8_string str_fmt("(?1_)(?2_)");
		yggr::utf8_string text("aaacccbbb");

		yggr::utf8_string yggr_rst;
		std::string stl_rst;

		try
		{
			{
				yggr_rst = yggr::regular_parse::u32regex_parse::replace(
								text, yggr::regular_parse::u32regex_parse::make_regex(str_re), str_fmt);
				yggr_test_assert(yggr_rst == "___ccc___");

				yggr_rst = yggr::regular_parse::u32regex_parse::replace(
								yggr_rst.get_allocator(),
								text, yggr::regular_parse::u32regex_parse::make_regex(str_re),
								str_fmt);
				yggr_test_assert(yggr_rst == "___ccc___");

				yggr_rst = yggr::regular_parse::u32regex_parse::replace<yggr::utf8_string>(
								text, yggr::regular_parse::u32regex_parse::make_regex(str_re), str_fmt);
				yggr_test_assert(yggr_rst == "___ccc___");

				yggr_rst = yggr::regular_parse::u32regex_parse::replace<yggr::utf8_string>(
								yggr_rst.get_allocator(),
								text, yggr::regular_parse::u32regex_parse::make_regex(str_re),
								str_fmt);
				yggr_test_assert(yggr_rst == "___ccc___");

				stl_rst = yggr::regular_parse::u32regex_parse::replace<std::string>(
								text, yggr::regular_parse::u32regex_parse::make_regex(str_re), str_fmt);
				yggr_test_assert(stl_rst == "___ccc___");

				stl_rst = yggr::regular_parse::u32regex_parse::replace<std::string>(
								stl_rst.get_allocator(),
								text, yggr::regular_parse::u32regex_parse::make_regex(str_re),
								str_fmt);
				yggr_test_assert(stl_rst == "___ccc___");
			}

			{
				yggr_rst = yggr::regular_parse::u32regex_parse::replace(
								text.begin(), text.end(),
								yggr::regular_parse::u32regex_parse::make_regex(
									str_re.begin(), str_re.end()),
								str_fmt);
				yggr_test_assert(yggr_rst == "___ccc___");

				yggr_rst = yggr::regular_parse::u32regex_parse::replace(
								yggr_rst.get_allocator(),
								text.begin(), text.end(),
								yggr::regular_parse::u32regex_parse::make_regex(
									str_re.begin(), str_re.end()),
								str_fmt);
				yggr_test_assert(yggr_rst == "___ccc___");

				yggr_rst = yggr::regular_parse::u32regex_parse::replace<yggr::utf8_string>(
								text.begin(), text.end(),
								yggr::regular_parse::u32regex_parse::make_regex(
									str_re.begin(), str_re.end()),
								str_fmt);
				yggr_test_assert(yggr_rst == "___ccc___");

				yggr_rst = yggr::regular_parse::u32regex_parse::replace<yggr::utf8_string>(
								yggr_rst.get_allocator(),
								text.begin(), text.end(),
								yggr::regular_parse::u32regex_parse::make_regex(
									str_re.begin(), str_re.end()),
								str_fmt);
				yggr_test_assert(yggr_rst == "___ccc___");

				stl_rst = yggr::regular_parse::u32regex_parse::replace<std::string>(
								text.begin(), text.end(),
								yggr::regular_parse::u32regex_parse::make_regex(
									str_re.begin(), str_re.end()),
								str_fmt);
				yggr_test_assert(stl_rst == "___ccc___");

				stl_rst = yggr::regular_parse::u32regex_parse::replace<std::string>(
								stl_rst.get_allocator(),
								text.begin(), text.end(),
								yggr::regular_parse::u32regex_parse::make_regex(
									str_re.begin(), str_re.end()),
								str_fmt);
				yggr_test_assert(stl_rst == "___ccc___");
			}

			{
				yggr_rst = yggr::regular_parse::u32regex_parse::replace(
								text.c_str(),
								yggr::regular_parse::u32regex_parse::make_regex(
									str_re.c_str()),
								str_fmt);
				yggr_test_assert(yggr_rst == "___ccc___");

				yggr_rst = yggr::regular_parse::u32regex_parse::replace(
								yggr_rst.get_allocator(), 
								text.c_str(),
								yggr::regular_parse::u32regex_parse::make_regex(
									str_re.c_str()),
								str_fmt);
				yggr_test_assert(yggr_rst == "___ccc___");

				yggr_rst = yggr::regular_parse::u32regex_parse::replace<yggr::utf8_string>(
								text.c_str(),
								yggr::regular_parse::u32regex_parse::make_regex(
									str_re.c_str()),
								str_fmt);
				yggr_test_assert(yggr_rst == "___ccc___");

				yggr_rst = yggr::regular_parse::u32regex_parse::replace<yggr::utf8_string>(
								yggr_rst.get_allocator(), 
								text.c_str(),
								yggr::regular_parse::u32regex_parse::make_regex(
									str_re.c_str()),
								str_fmt);
				yggr_test_assert(yggr_rst == "___ccc___");

				stl_rst = yggr::regular_parse::u32regex_parse::replace<std::string>(
								text.c_str(),
								yggr::regular_parse::u32regex_parse::make_regex(
									str_re.c_str()),
								str_fmt);
				yggr_test_assert(stl_rst == "___ccc___");

				stl_rst = yggr::regular_parse::u32regex_parse::replace<std::string>(
								stl_rst.get_allocator(),
								text.c_str(),
								yggr::regular_parse::u32regex_parse::make_regex(
									str_re.c_str()),
								str_fmt);
				yggr_test_assert(stl_rst == "___ccc___");
			}

			{
				yggr_rst = yggr::regular_parse::u32regex_parse::replace(
								"aaacccbbb",
								yggr::regular_parse::u32regex_parse::make_regex("(a)|(b)"),
								"(?1_)(?2_)");
				yggr_test_assert(yggr_rst == "___ccc___");

				yggr_rst = yggr::regular_parse::u32regex_parse::replace(
								yggr_rst.get_allocator(),
								"aaacccbbb",
								yggr::regular_parse::u32regex_parse::make_regex("(a)|(b)"),
								"(?1_)(?2_)");
				yggr_test_assert(yggr_rst == "___ccc___");

				yggr_rst = yggr::regular_parse::u32regex_parse::replace<yggr::utf8_string>(
								"aaacccbbb",
								yggr::regular_parse::u32regex_parse::make_regex("(a)|(b)"),
								"(?1_)(?2_)");
				yggr_test_assert(yggr_rst == "___ccc___");

				yggr_rst = yggr::regular_parse::u32regex_parse::replace<yggr::utf8_string>(
								yggr_rst.get_allocator(),
								"aaacccbbb",
								yggr::regular_parse::u32regex_parse::make_regex("(a)|(b)"),
								"(?1_)(?2_)");
				yggr_test_assert(yggr_rst == "___ccc___");

				stl_rst = yggr::regular_parse::u32regex_parse::replace<std::string>(
								"aaacccbbb",
								yggr::regular_parse::u32regex_parse::make_regex("(a)|(b)"),
								"(?1_)(?2_)");
				yggr_test_assert(stl_rst == "___ccc___");

				stl_rst = yggr::regular_parse::u32regex_parse::replace<std::string>(
								stl_rst.get_allocator(),
								"aaacccbbb",
								yggr::regular_parse::u32regex_parse::make_regex("(a)|(b)"),
								"(?1_)(?2_)");
				yggr_test_assert(stl_rst == "___ccc___");
			}
		}
		catch(const yggr::regular_parse::u32regex_parse::error_type& e)
		{
			std::cout << e.what() << std::endl;
			yggr_test_assert(false);
		}
		catch(const std::exception& e)
		{
			std::cout << e.what() << std::endl;
			yggr_test_assert(false);
		}
		catch(...)
		{
			yggr_test_assert(false);
		}
	}


#ifndef YGGR_NO_CWCHAR
	{
		yggr::utf8_string str_re(L"(你)|(吗)");
		yggr::utf8_string str_fmt(L"(?1_)(?2_)");
		yggr::utf8_string text(L"你你你好好好吗吗吗");

		yggr::utf8_string yggr_rst;
		std::wstring stl_rst;

		try
		{
			{
				yggr_rst = yggr::regular_parse::u32regex_parse::replace(
								text, yggr::regular_parse::u32regex_parse::make_regex(str_re), str_fmt);
				yggr_test_assert(yggr_rst == L"___好好好___");

				yggr_rst = yggr::regular_parse::u32regex_parse::replace(
								yggr_rst.get_allocator(),
								text, yggr::regular_parse::u32regex_parse::make_regex(str_re),
								str_fmt);
				yggr_test_assert(yggr_rst == L"___好好好___");

				yggr_rst = yggr::regular_parse::u32regex_parse::replace<yggr::utf8_string>(
								text, yggr::regular_parse::u32regex_parse::make_regex(str_re), str_fmt);
				yggr_test_assert(yggr_rst == L"___好好好___");

				yggr_rst = yggr::regular_parse::u32regex_parse::replace<yggr::utf8_string>(
								yggr_rst.get_allocator(),
								text, yggr::regular_parse::u32regex_parse::make_regex(str_re),
								str_fmt);
				yggr_test_assert(yggr_rst == L"___好好好___");

				stl_rst = yggr::regular_parse::u32regex_parse::replace<std::wstring>(
								text, yggr::regular_parse::u32regex_parse::make_regex(str_re), str_fmt);
				yggr_test_assert(stl_rst == L"___好好好___");

				stl_rst = yggr::regular_parse::u32regex_parse::replace<std::wstring>(
								stl_rst.get_allocator(),
								text, yggr::regular_parse::u32regex_parse::make_regex(str_re),
								str_fmt);
				yggr_test_assert(stl_rst == L"___好好好___");
			}

			{
				yggr_rst = yggr::regular_parse::u32regex_parse::replace(
								text.begin(), text.end(),
								yggr::regular_parse::u32regex_parse::make_regex(
									str_re.begin(), str_re.end()),
								str_fmt);
				yggr_test_assert(yggr_rst == L"___好好好___");

				yggr_rst = yggr::regular_parse::u32regex_parse::replace(
								yggr_rst.get_allocator(),
								text.begin(), text.end(),
								yggr::regular_parse::u32regex_parse::make_regex(
									str_re.begin(), str_re.end()),
								str_fmt);
				yggr_test_assert(yggr_rst == L"___好好好___");

				yggr_rst = yggr::regular_parse::u32regex_parse::replace<yggr::utf8_string>(
								text.begin(), text.end(),
								yggr::regular_parse::u32regex_parse::make_regex(
									str_re.begin(), str_re.end()),
								str_fmt);
				yggr_test_assert(yggr_rst == L"___好好好___");

				yggr_rst = yggr::regular_parse::u32regex_parse::replace<yggr::utf8_string>(
								yggr_rst.get_allocator(),
								text.begin(), text.end(),
								yggr::regular_parse::u32regex_parse::make_regex(
									str_re.begin(), str_re.end()),
								str_fmt);
				yggr_test_assert(yggr_rst == L"___好好好___");

				stl_rst = yggr::regular_parse::u32regex_parse::replace<std::wstring>(
								text.begin(), text.end(),
								yggr::regular_parse::u32regex_parse::make_regex(
									str_re.begin(), str_re.end()),
								str_fmt);
				yggr_test_assert(stl_rst == L"___好好好___");

				stl_rst = yggr::regular_parse::u32regex_parse::replace<std::wstring>(
								stl_rst.get_allocator(),
								text.begin(), text.end(),
								yggr::regular_parse::u32regex_parse::make_regex(
									str_re.begin(), str_re.end()),
								str_fmt);
				yggr_test_assert(stl_rst == L"___好好好___");
			}

			{
				yggr_rst = yggr::regular_parse::u32regex_parse::replace(
								text.c_str(),
								yggr::regular_parse::u32regex_parse::make_regex(
									str_re.c_str()),
								str_fmt);
				yggr_test_assert(yggr_rst == L"___好好好___");

				yggr_rst = yggr::regular_parse::u32regex_parse::replace(
								yggr_rst.get_allocator(),
								text.c_str(),
								yggr::regular_parse::u32regex_parse::make_regex(
									str_re.c_str()),
								str_fmt);
				yggr_test_assert(yggr_rst == L"___好好好___");

				yggr_rst = yggr::regular_parse::u32regex_parse::replace<yggr::utf8_string>(
								text.c_str(),
								yggr::regular_parse::u32regex_parse::make_regex(
									str_re.c_str()),
								str_fmt);
				yggr_test_assert(yggr_rst == L"___好好好___");

				yggr_rst = yggr::regular_parse::u32regex_parse::replace<yggr::utf8_string>(
								yggr_rst.get_allocator(),
								text.c_str(),
								yggr::regular_parse::u32regex_parse::make_regex(
									str_re.c_str()),
								str_fmt);
				yggr_test_assert(yggr_rst == L"___好好好___");

				stl_rst = yggr::regular_parse::u32regex_parse::replace<std::wstring>(
								text.c_str(),
								yggr::regular_parse::u32regex_parse::make_regex(
									str_re.c_str()),
								str_fmt);
				yggr_test_assert(stl_rst == L"___好好好___");

				stl_rst = yggr::regular_parse::u32regex_parse::replace<std::wstring>(
								stl_rst.get_allocator(),
								text.c_str(),
								yggr::regular_parse::u32regex_parse::make_regex(
									str_re.c_str()),
								str_fmt);
				yggr_test_assert(stl_rst == L"___好好好___");
			}

			{
				yggr_rst = yggr::regular_parse::u32regex_parse::replace(
								L"你你你好好好吗吗吗",
								yggr::regular_parse::u32regex_parse::make_regex(L"(你)|(吗)"),
								L"(?1_)(?2_)");
				yggr_test_assert(yggr_rst == L"___好好好___");

				yggr_rst = yggr::regular_parse::u32regex_parse::replace(
								yggr_rst.get_allocator(),
								L"你你你好好好吗吗吗",
								yggr::regular_parse::u32regex_parse::make_regex(L"(你)|(吗)"),
								L"(?1_)(?2_)");
				yggr_test_assert(yggr_rst == L"___好好好___");

				yggr_rst = yggr::regular_parse::u32regex_parse::replace<yggr::utf8_string>(
								L"你你你好好好吗吗吗",
								yggr::regular_parse::u32regex_parse::make_regex(L"(你)|(吗)"),
								L"(?1_)(?2_)");
				yggr_test_assert(yggr_rst == L"___好好好___");

				yggr_rst = yggr::regular_parse::u32regex_parse::replace<yggr::utf8_string>(
								yggr_rst.get_allocator(),
								L"你你你好好好吗吗吗",
								yggr::regular_parse::u32regex_parse::make_regex(L"(你)|(吗)"),
								L"(?1_)(?2_)");
				yggr_test_assert(yggr_rst == L"___好好好___");

				stl_rst = yggr::regular_parse::u32regex_parse::replace<std::wstring>(
								L"你你你好好好吗吗吗",
								yggr::regular_parse::u32regex_parse::make_regex(L"(你)|(吗)"),
								L"(?1_)(?2_)");
				yggr_test_assert(stl_rst == L"___好好好___");

				stl_rst = yggr::regular_parse::u32regex_parse::replace<std::wstring>(
								stl_rst.get_allocator(),
								L"你你你好好好吗吗吗",
								yggr::regular_parse::u32regex_parse::make_regex(L"(你)|(吗)"),
								L"(?1_)(?2_)");
				yggr_test_assert(stl_rst == L"___好好好___");
			}
		}
		catch(const yggr::regular_parse::u32regex_parse::error_type& e)
		{
			std::cout << e.what() << std::endl;
			yggr_test_assert(false);
		}
		catch(const std::exception& e)
		{
			std::cout << e.what() << std::endl;
			yggr_test_assert(false);
		}
		catch(...)
		{
			yggr_test_assert(false);
		}
	}
#endif // YGGR_NO_CWCHAR

	std::cout << "-------------test_replace1 end------------" << std::endl;
}

void test_utf8_replace2(void)
{

	{
		typedef std::basic_stringstream<char> ss_type;
		typedef std::ostream_iterator<char> out_iter_type;

		yggr::utf8_string str_re("(a)|(b)");
		yggr::utf8_string str_fmt("(?1_)(?2_)");
		yggr::utf8_string text("aaacccbbb");

		yggr::utf8_string str_tail = "fixed";

		try
		{
			{
				ss_type ss;

				out_iter_type out_iter =
					yggr::regular_parse::u32regex_parse::replace(
						out_iter_type(ss), text,
						yggr::regular_parse::u32regex_parse::make_regex(str_re), str_fmt);
				std::copy(str_tail.begin().org(), str_tail.end().org(), out_iter);
				yggr_test_assert(yggr::utf8_string(ss.str(), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == "___ccc___fixed");
			}

			{
				ss_type ss;

				out_iter_type out_iter =
					yggr::regular_parse::u32regex_parse::replace(
						out_iter_type(ss), text.begin(), text.end(),
						yggr::regular_parse::u32regex_parse::make_regex(str_re), str_fmt);
				std::copy(str_tail.begin().org(), str_tail.end().org(), out_iter);
				yggr_test_assert(yggr::utf8_string(ss.str(), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == "___ccc___fixed");
			}

			{
				ss_type ss;

				out_iter_type out_iter =
					yggr::regular_parse::u32regex_parse::replace(
						out_iter_type(ss), text.c_str(),
						yggr::regular_parse::u32regex_parse::make_regex(str_re), str_fmt);
				std::copy(str_tail.begin().org(), str_tail.end().org(), out_iter);
				yggr_test_assert(yggr::utf8_string(ss.str(), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == "___ccc___fixed");
			}

			{
				ss_type ss;

				out_iter_type out_iter =
					yggr::regular_parse::u32regex_parse::replace(
						out_iter_type(ss), "aaacccbbb",
						yggr::regular_parse::u32regex_parse::make_regex(str_re), str_fmt);
				std::copy(str_tail.begin().org(), str_tail.end().org(), out_iter);
				yggr_test_assert(yggr::utf8_string(ss.str(), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == "___ccc___fixed");
			}

		}
		catch(const yggr::regular_parse::u32regex_parse::error_type& e)
		{
			std::cout << e.what() << std::endl;
			yggr_test_assert(false);
		}
		catch(const std::exception& e)
		{
			std::cout << e.what() << std::endl;
			yggr_test_assert(false);
		}
		catch(...)
		{
			yggr_test_assert(false);
		}
	}

#ifndef YGGR_NO_CWCHAR

	{
		typedef std::basic_stringstream<char> ss_type;
		typedef std::ostream_iterator<char> out_iter_type;

		yggr::utf8_string str_re(L"(你)|(吗)");
		yggr::utf8_string str_fmt(L"(?1一)(?2二)");
		yggr::utf8_string text(L"你你你好好好吗吗吗");

		yggr::utf8_string str_tail = L"处理了";

		try
		{
			{
				ss_type ss;

				out_iter_type out_iter =
					yggr::regular_parse::u32regex_parse::replace(
						out_iter_type(ss), text,
						yggr::regular_parse::u32regex_parse::make_regex(str_re), str_fmt);
				std::copy(str_tail.begin().org(), str_tail.end().org(), out_iter);
				yggr_test_assert(yggr::utf8_string(ss.str(), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == L"一一一好好好二二二处理了");
			}

			{
				ss_type ss;

				out_iter_type out_iter =
					yggr::regular_parse::u32regex_parse::replace(
						out_iter_type(ss), text.begin(), text.end(),
						yggr::regular_parse::u32regex_parse::make_regex(str_re), str_fmt);
				std::copy(str_tail.begin().org(), str_tail.end().org(), out_iter);
				yggr_test_assert(yggr::utf8_string(ss.str(), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == L"一一一好好好二二二处理了");
			}

			{
				ss_type ss;

				out_iter_type out_iter =
					yggr::regular_parse::u32regex_parse::replace(
						out_iter_type(ss), text.c_str(),
						yggr::regular_parse::u32regex_parse::make_regex(str_re), str_fmt);
				std::copy(str_tail.begin().org(), str_tail.end().org(), out_iter);
				yggr_test_assert(yggr::utf8_string(ss.str(), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == L"一一一好好好二二二处理了");
			}

			{
				ss_type ss;
				out_iter_type out_iter =
					yggr::regular_parse::u32regex_parse::replace(
						out_iter_type(ss), L"你你你好好好吗吗吗",
						yggr::regular_parse::u32regex_parse::make_regex(str_re), str_fmt);
				std::copy(str_tail.begin().org(), str_tail.end().org(), out_iter);
				yggr_test_assert(yggr::utf8_string(ss.str(), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == L"一一一好好好二二二处理了");
			}
		}
		catch(const yggr::regular_parse::u32regex_parse::error_type& e)
		{
			std::cout << e.what() << std::endl;
			yggr_test_assert(false);
		}
		catch(const std::exception& e)
		{
			std::cout << e.what() << std::endl;
			yggr_test_assert(false);
		}
		catch(...)
		{
			yggr_test_assert(false);
		}
	}

#endif //YGGR_NO_CWCHAR

	std::cout << "-------------test_utf8_replace2 end------------" << std::endl;
}

void test_utf8_replac_of_name(void)
{
	{
		yggr::utf8_string str_re("(?<a_mark>a)|(?<b_mark>b)");
		yggr::utf8_string str_fmt("(?{a_mark}$+{b_mark})(?{b_mark}$+{a_mark})");
		yggr::utf8_string text("aaacccbbb");

		try
		{
			std::cout <<
				yggr::regular_parse::u32regex_parse::replace(
					text, yggr::regular_parse::u32regex_parse::make_regex(str_re), str_fmt) << std::endl;
		}
		catch(const yggr::regular_parse::u32regex_parse::error_type& e)
		{
			std::cout << e.what() << std::endl;
		}
		catch(const std::exception& e)
		{
			std::cout << e.what() << std::endl;
			yggr_test_assert(false);
		}
		catch(...)
		{
			yggr_test_assert(false);
		}
	}

#ifndef YGGR_NO_CWCHAR

	{
		yggr::utf8_string str_re(L"(?<a_mark>你)|(?<b_mark>吗)");
		yggr::utf8_string str_fmt("(?{a_mark}$+{b_mark})(?{b_mark}$+{a_mark})");
		yggr::utf8_string text(L"你你你好好好吗吗吗");

		try
		{
			std::cout <<
				yggr::regular_parse::u32regex_parse::replace(
					text, yggr::regular_parse::u32regex_parse::make_regex(str_re), str_fmt) << std::endl;
		}
		catch(const yggr::regular_parse::u32regex_parse::error_type& e)
		{
			std::cout << e.what() << std::endl;
		}
		catch(const std::exception& e)
		{
			std::cout << e.what() << std::endl;
			yggr_test_assert(false);
		}
		catch(...)
		{
			yggr_test_assert(false);
		}
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "-------------test_utf8_replac_of_name end------------" << std::endl;
}

int main(int argc, char* argv[])
{

#ifndef YGGR_NO_CWCHAR
	TEST_PP_WIO_IMBUE("chs");
#endif // YGGR_NO_CWCHAR

	//boost::u32regex r = boost::make_u32regex("(?:\\A|.*\\\\)([^\\\\]+)");

	test_utf8_match();
	test_utf8_match2();
	test_utf8_match3();
	test_utf8_match4();

	test_utf8_search();
	test_utf8_search2();
	test_utf8_search3();
	test_utf8_search4();

	test_utf8_replace1();
	test_utf8_replace2();
	test_utf8_replac_of_name();

	wait_any_key(argc, argv);
	return 0;
}
