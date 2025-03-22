// utf8_regex_parse_test_part2.cpp

#if defined(_MSC_VER)
#	error "this file not supports MSVC only, if you are using other compilers, please use the same name file in the ../"
#endif // _MSC_VER

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

void test_utf8_format(void)
{
	{
		typedef std::basic_stringstream<char> ss_type;
		typedef std::ostream_iterator<char> out_iter_type;
		yggr::utf8_string str_re("(a)|(b)");
		yggr::utf8_string str_fmt("(?1A)(?2B)");
		yggr::utf8_string text("aaacccbbb");

		yggr::utf8_string yggr_rst;
		std::string stl_rst;

		try
		{
			{
				typedef yggr::regular_parse::match_results_t_maker<yggr::utf8_string::const_iterator>::type match_results_type;

				match_results_type rst;
				bool bret =
					yggr::regular_parse::u32regex_parse::search(
							text, rst,
							yggr::regular_parse::u32regex_parse::make_regex(str_re));
				yggr_test_assert(bret);

				{
					yggr_rst = yggr::regular_parse::u32regex_parse::format(rst, str_fmt);
					yggr_test_assert(yggr_rst == "A");

					yggr_rst = yggr::regular_parse::u32regex_parse::format(yggr_rst.get_allocator(), rst, str_fmt);
					yggr_test_assert(yggr_rst == "A");

					yggr_rst = yggr::regular_parse::u32regex_parse::format<yggr::utf8_string>(rst, str_fmt);
					yggr_test_assert(yggr_rst == "A");

					yggr_rst = yggr::regular_parse::u32regex_parse::format<yggr::utf8_string>(yggr_rst.get_allocator(), rst, str_fmt);
					yggr_test_assert(yggr_rst == "A");

					stl_rst = yggr::regular_parse::u32regex_parse::format<std::string>(rst, str_fmt);
					yggr_test_assert(stl_rst == "A");

					stl_rst = yggr::regular_parse::u32regex_parse::format<std::string>(stl_rst.get_allocator(), rst, str_fmt);
					yggr_test_assert(stl_rst == "A");

					ss_type ss;
					yggr::regular_parse::u32regex_parse::format(out_iter_type(ss), rst, str_fmt);
					yggr_test_assert(yggr::utf8_string(ss.str(), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == "A");
				}

				{
					yggr_rst = yggr::regular_parse::u32regex_parse::format(rst, str_fmt.c_str());
					yggr_test_assert(yggr_rst == "A");

					yggr_rst = yggr::regular_parse::u32regex_parse::format(yggr_rst.get_allocator(), rst, str_fmt.c_str());
					yggr_test_assert(yggr_rst == "A");

					yggr_rst = yggr::regular_parse::u32regex_parse::format<yggr::utf8_string>(rst, str_fmt.c_str());
					yggr_test_assert(yggr_rst == "A");

					yggr_rst = yggr::regular_parse::u32regex_parse::format<yggr::utf8_string>(yggr_rst.get_allocator(), rst, str_fmt.c_str());
					yggr_test_assert(yggr_rst == "A");

					stl_rst = yggr::regular_parse::u32regex_parse::format<std::string>(rst, str_fmt.c_str());
					yggr_test_assert(stl_rst == "A");

					stl_rst = yggr::regular_parse::u32regex_parse::format<std::string>(stl_rst.get_allocator(), rst, str_fmt.c_str());
					yggr_test_assert(stl_rst == "A");

					ss_type ss;
					yggr::regular_parse::u32regex_parse::format(out_iter_type(ss), rst, str_fmt.c_str());
					yggr_test_assert(yggr::utf8_string(ss.str(), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == "A");
				}

				{
					yggr_rst = yggr::regular_parse::u32regex_parse::format(rst, "(?1A)(?2B)");
					yggr_test_assert(yggr_rst == "A");

					yggr_rst = yggr::regular_parse::u32regex_parse::format(yggr_rst.get_allocator(), rst, "(?1A)(?2B)");
					yggr_test_assert(yggr_rst == "A");

					yggr_rst = yggr::regular_parse::u32regex_parse::format<yggr::utf8_string>(rst, "(?1A)(?2B)");
					yggr_test_assert(yggr_rst == "A");

					yggr_rst = yggr::regular_parse::u32regex_parse::format<yggr::utf8_string>(yggr_rst.get_allocator(), rst, "(?1A)(?2B)");
					yggr_test_assert(yggr_rst == "A");

					stl_rst = yggr::regular_parse::u32regex_parse::format<std::string>(rst, "(?1A)(?2B)");
					yggr_test_assert(stl_rst == "A");

					stl_rst = yggr::regular_parse::u32regex_parse::format<std::string>(stl_rst.get_allocator(), rst, "(?1A)(?2B)");
					yggr_test_assert(stl_rst == "A");

					ss_type ss;
					yggr::regular_parse::u32regex_parse::format(out_iter_type(ss), rst, "(?1A)(?2B)");
					yggr_test_assert(yggr::utf8_string(ss.str(), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == "A");
				}
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
		yggr::utf8_string str_fmt(L"(?1您)(?2_)");
		yggr::utf8_string text(L"你你你好好好吗吗吗");

		yggr::utf8_string yggr_rst;
		std::wstring stl_rst;

		try
		{
			{
				typedef yggr::regular_parse::match_results_t_maker<yggr::utf8_string::const_iterator>::type match_results_type;

				match_results_type rst;
				bool bret =
					yggr::regular_parse::u32regex_parse::search(
							text, rst,
							yggr::regular_parse::u32regex_parse::make_regex(str_re));
				yggr_test_assert(bret);

				{
					{
						std::string str = yggr::regular_parse::u32regex_parse::format<std::string>(rst, str_fmt.str<std::wstring>());
						yggr_test_assert(yggr::utf8_string(str, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == L"您");

						stl_rst = yggr::regular_parse::u32regex_parse::format<std::wstring>(rst, str_fmt.str<std::wstring>());
						yggr_test_assert(stl_rst == L"您");
					}

					yggr_rst = yggr::regular_parse::u32regex_parse::format(rst, str_fmt);
					yggr_test_assert(yggr_rst == L"您");

					yggr_rst = yggr::regular_parse::u32regex_parse::format(yggr_rst.get_allocator(), rst, str_fmt);
					yggr_test_assert(yggr_rst == L"您");

					stl_rst = yggr::regular_parse::u32regex_parse::format<std::wstring>(rst, str_fmt);
					yggr_test_assert(stl_rst == L"您");

					stl_rst = yggr::regular_parse::u32regex_parse::format<std::wstring>(stl_rst.get_allocator(), rst, str_fmt);
					yggr_test_assert(stl_rst == L"您");

					ss_type ss;
					yggr::regular_parse::u32regex_parse::format(out_iter_type(ss), rst, str_fmt);
					yggr_test_assert(yggr::utf8_string(ss.str(), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == L"您");
				}

				{
					yggr_rst = yggr::regular_parse::u32regex_parse::format(rst, str_fmt.c_str());
					yggr_test_assert(yggr_rst == L"您");

					yggr_rst = yggr::regular_parse::u32regex_parse::format(yggr_rst.get_allocator(), rst, str_fmt.c_str());
					yggr_test_assert(yggr_rst == L"您");

					stl_rst = yggr::regular_parse::u32regex_parse::format<std::wstring>(rst, str_fmt.c_str());
					yggr_test_assert(stl_rst == L"您");

					stl_rst = yggr::regular_parse::u32regex_parse::format<std::wstring>(stl_rst.get_allocator(), rst, str_fmt.c_str());
					yggr_test_assert(stl_rst == L"您");

					ss_type ss;
					yggr::regular_parse::u32regex_parse::format(out_iter_type(ss), rst, str_fmt.c_str());
					yggr_test_assert(yggr::utf8_string(ss.str(), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == L"您");
				}

				{
					yggr_rst = yggr::regular_parse::u32regex_parse::format(rst, L"(?1您)(?2_)");
					yggr_test_assert(yggr_rst == L"您");

					yggr_rst = yggr::regular_parse::u32regex_parse::format(yggr_rst.get_allocator(), rst, L"(?1您)(?2_)");
					yggr_test_assert(yggr_rst == L"您");

					stl_rst = yggr::regular_parse::u32regex_parse::format<std::wstring>(rst, L"(?1您)(?2_)");
					yggr_test_assert(stl_rst == L"您");

					stl_rst = yggr::regular_parse::u32regex_parse::format<std::wstring>(stl_rst.get_allocator(), rst, L"(?1您)(?2_)");
					yggr_test_assert(stl_rst == L"您");

					ss_type ss;
					yggr::regular_parse::u32regex_parse::format(out_iter_type(ss), rst, L"(?1您)(?2_)");
					yggr_test_assert(yggr::utf8_string(ss.str(), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == L"您");
				}
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

	std::cout << "------------test_utf8_format end----------" << std::endl;
}

void test_utf8_format2(void)
{
	{
		yggr::utf8_string str_re("(a)|(b)");
		yggr::utf8_string str_fmt("(?1A)(?2B)");
		yggr::utf8_string text("aaacccbbb");

		try
		{
			{
				typedef yggr::regular_parse::match_results_t_maker<yggr::utf8_string::const_iterator>::type match_results_type;

				match_results_type rst;
				yggr::utf8_string::const_iterator start = text.begin(), last = text.end();
				yggr::string str_rst;

				for(; yggr::regular_parse::u32regex_parse::search(
							start, last, rst,
							yggr::regular_parse::u32regex_parse::make_regex(str_re)); start = rst[0].second)
				{
					str_rst = yggr::regular_parse::u32regex_parse::format<yggr::string>(rst, str_fmt);
					if(*start == 'a')
					{
						yggr_test_assert(str_rst == "A");
					}

					if(*start == 'b')
					{
						yggr_test_assert(str_rst == "B");
					}

					std::cout << str_rst << std::endl;
				}
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
		yggr::utf8_string str_fmt(L"(?1您)(?2_)");
		yggr::utf8_string text(L"你你你好好好吗吗吗");

		try
		{
			{
				typedef yggr::regular_parse::match_results_t_maker<yggr::utf8_string::const_iterator>::type match_results_type;

				match_results_type rst;
				yggr::utf8_string::const_iterator start = text.begin(), last = text.end();
				yggr::utf8_string str_rst;

				for(; yggr::regular_parse::u32regex_parse::search(
							start, last, rst,
							yggr::regular_parse::u32regex_parse::make_regex(str_re)); start = rst[0].second)
				{
					str_rst = yggr::regular_parse::u32regex_parse::format(rst, str_fmt);
					if(*start == L'你')
					{
						yggr_test_assert(str_rst == L"您");
					}

					if(*start == L'吗')
					{
						yggr_test_assert(str_rst == L"_");
					}

					std::wcout << str_rst << std::endl;
				}
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

	std::cout << "------------test_utf8_format2 end----------" << std::endl;
}

void test_utf8_make_regex_iterator(void)
{
	{
		yggr::utf8_string str_re("^[[:space:]]*"
						"(template[[:space:]]*<[^;:{]+>[[:space:]]*)?"
						"(class|struct)[[:space:]]*"
						"("
						"\\<\\w+\\>"
						"("
						"[[:blank:]]*\\([^)]*\\)"
						")?"
						"[[:space:]]*"
						")*"
						"(\\<\\w*\\>)[[:space:]]*"
						"(<[^;:{]+>)?[[:space:]]*"
						"(\\{|:[^;\\{()]*\\{)"
						);

		yggr::utf8_string text("template <typename T> \nclass A \n{\n}; template <typename T> \nclass B \n{\n};");

		try
		{
			{
				typedef
					yggr::regular_parse::regex_iterator_t_maker
					<
						yggr::utf8_string::const_iterator,
						yggr::regular_parse::u32regex_parse::regex_type
					>::type regex_iter_type;

				regex_iter_type iter
					= yggr::regular_parse::u32regex_parse::make_regex_iterator(
						text,
						yggr::regular_parse::u32regex_parse::make_regex(str_re));

				for(int i = 0; iter != regex_iter_type(); ++iter, ++i)
				{
					if(i == 0)
					{
						yggr_test_assert(std::string((*iter)[5].first, (*iter)[5].second) == "A");
						int nnn  = 0; // disable optimization
						assert(!nnn);
					}

					if(i == 1)
					{
						yggr_test_assert(std::string((*iter)[5].first, (*iter)[5].second) == "B");
						int nnn  = 0; // disable optimization
						assert(!nnn);
					}

					std::cout << std::string((*iter)[5].first, (*iter)[5].second) << std::endl;
					std::cout << std::string((*iter)[6].first, (*iter)[6].second) << std::endl;
					std::cout <<  yggr::utf8_string::const_iterator((*iter)[5].first) - text.cbegin() << std::endl;
				}
			}

			{
				typedef
					yggr::regular_parse::regex_iterator_t_maker
					<
						yggr::utf8_string::iterator,
						yggr::regular_parse::u32regex_parse::regex_type
					>::type regex_iter_type;

				regex_iter_type iter
					= yggr::regular_parse::u32regex_parse::make_regex_iterator(
						text.begin(), text.end(),
						yggr::regular_parse::u32regex_parse::make_regex(str_re));

				for(int i = 0; iter != regex_iter_type(); ++iter, ++i)
				{
					if(i == 0)
					{
						yggr_test_assert(std::string((*iter)[5].first, (*iter)[5].second) == "A");
						int nnn  = 0; // disable optimization
						assert(!nnn);
					}

					if(i == 1)
					{
						yggr_test_assert(std::string((*iter)[5].first, (*iter)[5].second) == "B");
						int nnn  = 0; // disable optimization
						assert(!nnn);
					}

					std::cout << std::string((*iter)[5].first, (*iter)[5].second) << std::endl;
					std::cout << std::string((*iter)[6].first, (*iter)[6].second) << std::endl;
					std::cout << yggr::utf8_string::const_iterator((*iter)[5].first) - text.cbegin() << std::endl;
				}
			}

			{
				typedef
					yggr::regular_parse::regex_iterator_t_maker
					<
						const char*,
						yggr::regular_parse::u32regex_parse::regex_type
					>::type regex_iter_type;

				regex_iter_type iter
					= yggr::regular_parse::u32regex_parse::make_regex_iterator(
						text.c_str(),
						yggr::regular_parse::u32regex_parse::make_regex(str_re));

				for(int i = 0;iter != regex_iter_type(); ++iter, ++i)
				{
					if(i == 0)
					{
						yggr_test_assert(std::string((*iter)[5].first, (*iter)[5].second) == "A");
						int nnn  = 0; // disable optimization
						assert(!nnn);
					}

					if(i == 1)
					{
						yggr_test_assert(std::string((*iter)[5].first, (*iter)[5].second) == "B");
						int nnn  = 0; // disable optimization
						assert(!nnn);
					}

					std::cout << std::string((*iter)[5].first, (*iter)[5].second) << std::endl;
					std::cout << std::string((*iter)[6].first, (*iter)[6].second) << std::endl;
					std::cout << yggr::utf8_string::const_iterator((*iter)[5].first) - text.cbegin() << std::endl;
				}
			}

			{
				typedef
					yggr::regular_parse::regex_iterator_t_maker
					<
						const char*,
						yggr::regular_parse::u32regex_parse::regex_type
					>::type regex_iter_type;

				regex_iter_type iter
					= yggr::regular_parse::u32regex_parse::make_regex_iterator(
						"template <typename T> \nclass A \n{\n}; template <typename T> \nclass B \n{\n};",
						yggr::regular_parse::u32regex_parse::make_regex(str_re));

				for(int i = 0;iter != regex_iter_type(); ++iter, ++i)
				{
					if(i == 0)
					{
						yggr_test_assert(std::string((*iter)[5].first, (*iter)[5].second) == "A");
						int nnn  = 0; // disable optimization
						assert(!nnn);
					}

					if(i == 1)
					{
						yggr_test_assert(std::string((*iter)[5].first, (*iter)[5].second) == "B");
						int nnn  = 0; // disable optimization
						assert(!nnn);
					}
					std::cout << std::string((*iter)[5].first, (*iter)[5].second) << std::endl;
					std::cout << std::string((*iter)[6].first, (*iter)[6].second) << std::endl;
				}
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

	std::cout << "------------test_utf8_make_regex_iterator end----------" << std::endl;
}

#include <map>

typedef std::map<yggr::utf8_string, yggr::utf8_string::difference_type, std::less<yggr::utf8_string> > map_type;


template<typename Iter>
bool handler_grep1(map_type& m, Iter base,
						const typename yggr::regular_parse::match_results_t_maker<Iter>::type& what)
{
	m[yggr::utf8_string(what[5].first, what[5].second, YGGR_STR_UTF8_STRING_CHARSET_NAME())
		+ yggr::utf8_string(what[6].first, what[6].second, YGGR_STR_UTF8_STRING_CHARSET_NAME())]
		= Iter(what[5].first) - base;
	return true;
}

template<typename Iter, typename String>
bool handler_grep2(map_type& m, Iter base, String& str,
						const typename yggr::regular_parse::match_results_t_maker<Iter>::type& what)
{
	m[yggr::utf8_string(what[5].first, what[5].second, YGGR_STR_UTF8_STRING_CHARSET_NAME())
		+ yggr::utf8_string(what[6].first, what[6].second, YGGR_STR_UTF8_STRING_CHARSET_NAME())]
		= Iter(str, what[5].first) - base;
	return true;
}


void test_utf8_grep_1(void)
{
	map_type map;
	yggr::utf8_string str_re("^[[:space:]]*"
								"(template[[:space:]]*<[^;:{]+>[[:space:]]*)?"
								"(class|struct)[[:space:]]*"
								"("
								"\\<\\w+\\>"
								"("
								"[[:blank:]]*\\([^)]*\\)"
								")?"
								"[[:space:]]*"
								")*"
								"(\\<\\w*\\>)[[:space:]]*"
								"(<[^;:{]+>)?[[:space:]]*"
								"(\\{|:[^;\\{()]*\\{)"
								);

	yggr::utf8_string text("template <typename T> \nclass A \n{\n}; template <typename T> \nclass B \n{\n};");

	try
	{
		std::size_t count = 0;
		count =
			yggr::regular_parse::u32regex_parse::grep(
				boost::bind(&handler_grep1<yggr::utf8_string::const_iterator>, boost::ref(map), text.cbegin(), _1),
				text,
				yggr::regular_parse::u32regex_parse::make_regex(str_re));
		yggr_test_assert(count == 2);
		yggr_test_assert(map["A"] == 29);
		yggr_test_assert(map["B"] == 66);

		count =
			yggr::regular_parse::u32regex_parse::grep(
				boost::bind(&handler_grep2<yggr::utf8_string::const_iterator, const yggr::utf8_string>,
								boost::ref(map), text.cbegin(), boost::cref(text), _1),
				text,
				yggr::regular_parse::u32regex_parse::make_regex(str_re));
		yggr_test_assert(count == 2);
		yggr_test_assert(map["A"] == 29);
		yggr_test_assert(map["B"] == 66);

		count =
			yggr::regular_parse::u32regex_parse::grep(
				boost::bind(&handler_grep2<yggr::utf8_string::iterator, yggr::utf8_string>,
								boost::ref(map), text.begin(), boost::ref(text), _1),
				text.begin(), text.end(),
				yggr::regular_parse::u32regex_parse::make_regex(str_re));
		yggr_test_assert(count == 2);
		yggr_test_assert(map["A"] == 29);
		yggr_test_assert(map["B"] == 66);

		count =
			yggr::regular_parse::u32regex_parse::grep(
				boost::bind(&handler_grep1<yggr::utf8_string::non_const_iterator>, boost::ref(map), text.ncbegin(), _1),
				text.begin(), text.end(),
				yggr::regular_parse::u32regex_parse::make_regex(str_re));
		yggr_test_assert(count == 2);
		yggr_test_assert(map["A"] == 29);
		yggr_test_assert(map["B"] == 66);

		count =
			yggr::regular_parse::u32regex_parse::grep(
				boost::bind(&handler_grep2<yggr::utf8_string::non_const_iterator, yggr::utf8_string>,
								boost::ref(map), text.ncbegin(), boost::ref(text), _1),
				text.begin(), text.end(),
				yggr::regular_parse::u32regex_parse::make_regex(str_re));
		yggr_test_assert(count == 2);
		yggr_test_assert(map["A"] == 29);
		yggr_test_assert(map["B"] == 66);

		count =
			yggr::regular_parse::u32regex_parse::grep(
				boost::bind(&handler_grep1<const char*>, boost::ref(map), text.c_str(), _1),
				text.c_str(),
				yggr::regular_parse::u32regex_parse::make_regex(str_re));
		yggr_test_assert(count == 2);
		yggr_test_assert(map["A"] == 29);
		yggr_test_assert(map["B"] == 66);

		char ctext[] = "template <typename T> \nclass A \n{\n}; template <typename T> \nclass B \n{\n};";
		count =
			yggr::regular_parse::u32regex_parse::grep(
				boost::bind(&handler_grep1<const char*>, boost::ref(map), &ctext[0], _1),
				ctext,
				yggr::regular_parse::u32regex_parse::make_regex(str_re));
		yggr_test_assert(count == 2);
		yggr_test_assert(map["A"] == 29);
		yggr_test_assert(map["B"] == 66);
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

	yggr_test_assert(map.size() == 2);

	for(map_type::iterator i = map.begin(), isize = map.end(); i != isize; ++i)
	{
		std::cout << i->first << " : " << i->second << std::endl;
	}

	std::cout << "-------------test_utf8_grep_1 end------------" << std::endl;
}

void test_utf8_grep_2(void)
{
	map_type map;
	yggr::utf8_string str_re("^[[:space:]]*"
								"(template[[:space:]]*<[^;:{]+>[[:space:]]*)?"
								"(class|struct)[[:space:]]*"
								"("
								"\\<\\w+\\>"
								"("
								"[[:blank:]]*\\([^)]*\\)"
								")?"
								"[[:space:]]*"
								")*"
								"(\\<\\w*\\>)[[:space:]]*"
								"(<[^;:{]+>)?[[:space:]]*"
								"(\\{|:[^;\\{()]*\\{)"
								);

	yggr::utf8_string text("template <typename T> \nclass A \n{\n}; template <typename T> \nclass B \n{\n};");

	std::vector<char> cvt(text.size(), 0);
	memcpy(&cvt[0], &text[0], text.size());

	try
	{
		std::size_t count = 0;
		count =
			yggr::regular_parse::u32regex_parse::grep(
				boost::bind(&handler_grep1<std::vector<char>::const_iterator>, boost::ref(map), cvt.begin(), _1),
				cvt,
				yggr::regular_parse::u32regex_parse::make_regex(str_re));
		yggr_test_assert(count == 2);
		yggr_test_assert(map["A"] == 29);
		yggr_test_assert(map["B"] == 66);

		count =
			yggr::regular_parse::u32regex_parse::grep(
				boost::bind(&handler_grep1<std::vector<char>::iterator>, boost::ref(map), cvt.begin(), _1),
				cvt.begin(), cvt.end(),
				yggr::regular_parse::u32regex_parse::make_regex(str_re));
		yggr_test_assert(count == 2);
		yggr_test_assert(map["A"] == 29);
		yggr_test_assert(map["B"] == 66);

		cvt.push_back(0);
		count =
			yggr::regular_parse::u32regex_parse::grep(
				boost::bind(&handler_grep1<const char*>, boost::ref(map), &cvt[0], _1),
				&cvt[0],
				yggr::regular_parse::u32regex_parse::make_regex(str_re));
		yggr_test_assert(count == 2);
		yggr_test_assert(map["A"] == 29);
		yggr_test_assert(map["B"] == 66);
	}
	catch(const yggr::regular_parse::u32regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
	}
	catch(yggr::stl_exception& e)
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

	yggr_test_assert(map.size() == 2);

	for(map_type::iterator i = map.begin(), isize = map.end(); i != isize; ++i)
	{
		std::cout << i->first << " : " << i->second << std::endl;
	}

	std::cout << "-------------test_utf8_grep_2 end------------" << std::endl;
}

bool handler_compare(bool l, bool r)
{
	return l == r;
}

void test_utf8_grep_3(void)
{
	map_type map;
	yggr::utf8_string str_re("^[[:space:]]*"
						"(template[[:space:]]*<[^;:{]+>[[:space:]]*)?"
						"(class|struct)[[:space:]]*"
						"("
						"\\<\\w+\\>"
						"("
						"[[:blank:]]*\\([^)]*\\)"
						")?"
						"[[:space:]]*"
						")*"
						"(\\<\\w*\\>)[[:space:]]*"
						"(<[^;:{]+>)?[[:space:]]*"
						"(\\{|:[^;\\{()]*\\{)"
						);

	yggr::utf8_string text("template <typename T> \nclass A \n{\n}; template <typename T> \nclass B \n{\n};");

	try
	{
		std::size_t count = 0;
		count =
			yggr::regular_parse::u32regex_parse::grep(
				boost::bind(&handler_grep1<yggr::utf8_string::const_iterator>, boost::ref(map), text.begin(), _1),
				boost::bind(&handler_compare, _1, true),
				text,
				yggr::regular_parse::u32regex_parse::make_regex(str_re));
		yggr_test_assert(count == 2);
		yggr_test_assert(map["A"] == 29);
		yggr_test_assert(map["B"] == 66);

		count =
			yggr::regular_parse::u32regex_parse::grep(
				boost::bind(&handler_grep2<yggr::utf8_string::iterator, yggr::utf8_string>,
								boost::ref(map), text.begin(), boost::ref(text), _1),
				boost::bind(&handler_compare, _1, true),
				text.begin(), text.end(),
				yggr::regular_parse::u32regex_parse::make_regex(str_re));
		yggr_test_assert(count == 2);
		yggr_test_assert(map["A"] == 29);
		yggr_test_assert(map["B"] == 66);

		count =
			yggr::regular_parse::u32regex_parse::grep(
				boost::bind(&handler_grep1<yggr::utf8_string::non_const_iterator>, boost::ref(map), text.begin(), _1),
				boost::bind(&handler_compare, _1, true),
				text.begin(), text.end(),
				yggr::regular_parse::u32regex_parse::make_regex(str_re));
		yggr_test_assert(count == 2);
		yggr_test_assert(map["A"] == 29);
		yggr_test_assert(map["B"] == 66);

		count =
			yggr::regular_parse::u32regex_parse::grep(
				boost::bind(&handler_grep1<const char*>, boost::ref(map), text.c_str(), _1),
				boost::bind(&handler_compare, _1, true),
				text.c_str(),
				yggr::regular_parse::u32regex_parse::make_regex(str_re));
		yggr_test_assert(count == 2);
		yggr_test_assert(map["A"] == 29);
		yggr_test_assert(map["B"] == 66);
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

	yggr_test_assert(map.size() == 2);

	for(map_type::iterator i = map.begin(), isize = map.end(); i != isize; ++i)
	{
		std::cout << i->first << " : " << i->second << std::endl;
	}

	std::cout << "-------------test_utf8_grep_3 end------------" << std::endl;
}

void test_utf8_make_regex_token_iterator(void)
{
	{
		yggr::utf8_string str_re = "\\s+";
		yggr::utf8_string text = "aaa bbb";

		try
		{
			{
				typedef
					yggr::regular_parse::regex_token_iterator_t_maker
					<
						yggr::utf8_string::const_iterator,
						yggr::regular_parse::u32regex_parse::regex_type
					>::type regex_token_iter_type;

				std::vector<yggr::utf8_string> chk_vt;

				regex_token_iter_type iter =
					yggr::regular_parse::u32regex_parse::make_regex_token_iterator(
						text,
						yggr::regular_parse::u32regex_parse::make_regex(str_re), -1);

				for(regex_token_iter_type i = iter, isize = regex_token_iter_type(); i != isize; ++i)
				{
					std::cout << yggr::utf8_string((*i).first, (*i).second, YGGR_STR_UTF8_STRING_CHARSET_NAME()) << std::endl;
					chk_vt.push_back(yggr::utf8_string((*i).first, (*i).second, YGGR_STR_UTF8_STRING_CHARSET_NAME()));
				}

				yggr_test_assert(chk_vt.size() == 2);
				yggr_test_assert(chk_vt[0] == "aaa");
				yggr_test_assert(chk_vt[1] == "bbb");
			}

			{
				typedef
					yggr::regular_parse::regex_token_iterator_t_maker
					<
						yggr::utf8_string::iterator,
						yggr::regular_parse::u32regex_parse::regex_type
					>::type regex_token_iter_type;

				std::vector<yggr::utf8_string> chk_vt;

				regex_token_iter_type iter =
					yggr::regular_parse::u32regex_parse::make_regex_token_iterator(
						text.begin(), text.end(),
						yggr::regular_parse::u32regex_parse::make_regex(str_re), -1);

				for(regex_token_iter_type i = iter, isize = regex_token_iter_type(); i != isize; ++i)
				{
					std::cout << yggr::utf8_string((*i).first, (*i).second, YGGR_STR_UTF8_STRING_CHARSET_NAME()) << std::endl;
					chk_vt.push_back(yggr::utf8_string((*i).first, (*i).second, YGGR_STR_UTF8_STRING_CHARSET_NAME()));
				}

				yggr_test_assert(chk_vt.size() == 2);
				yggr_test_assert(chk_vt[0] == "aaa");
				yggr_test_assert(chk_vt[1] == "bbb");
			}

			{
				typedef
					yggr::regular_parse::regex_token_iterator_t_maker
					<
						yggr::utf8_string::non_const_iterator,
						yggr::regular_parse::u32regex_parse::regex_type
					>::type regex_token_iter_type;

				std::vector<yggr::utf8_string> chk_vt;

				regex_token_iter_type iter =
					yggr::regular_parse::u32regex_parse::make_regex_token_iterator(
						text.begin(), text.end(),
						yggr::regular_parse::u32regex_parse::make_regex(str_re), -1);

				for(regex_token_iter_type i = iter, isize = regex_token_iter_type(); i != isize; ++i)
				{
					std::cout << yggr::utf8_string((*i).first, (*i).second, YGGR_STR_UTF8_STRING_CHARSET_NAME()) << std::endl;
					chk_vt.push_back(yggr::utf8_string((*i).first, (*i).second, YGGR_STR_UTF8_STRING_CHARSET_NAME()));
				}

				yggr_test_assert(chk_vt.size() == 2);
				yggr_test_assert(chk_vt[0] == "aaa");
				yggr_test_assert(chk_vt[1] == "bbb");
			}

			{
				typedef
					yggr::regular_parse::regex_token_iterator_t_maker
					<
						const char*,
						yggr::regular_parse::u32regex_parse::regex_type
					>::type regex_token_iter_type;

				std::vector<yggr::utf8_string> chk_vt;

				regex_token_iter_type iter =
					yggr::regular_parse::u32regex_parse::make_regex_token_iterator(
						text.c_str(),
						yggr::regular_parse::u32regex_parse::make_regex(str_re), -1);

				for(regex_token_iter_type i = iter, isize = regex_token_iter_type(); i != isize; ++i)
				{
					std::cout << yggr::utf8_string((*i).first, (*i).second, YGGR_STR_UTF8_STRING_CHARSET_NAME()) << std::endl;
					chk_vt.push_back(yggr::utf8_string((*i).first, (*i).second, YGGR_STR_UTF8_STRING_CHARSET_NAME()));
				}

				yggr_test_assert(chk_vt.size() == 2);
				yggr_test_assert(chk_vt[0] == "aaa");
				yggr_test_assert(chk_vt[1] == "bbb");
			}

			{
				typedef
					yggr::regular_parse::regex_token_iterator_t_maker
					<
						const char*,
						yggr::regular_parse::u32regex_parse::regex_type
					>::type regex_token_iter_type;

				std::vector<yggr::utf8_string> chk_vt;

				regex_token_iter_type iter =
					yggr::regular_parse::u32regex_parse::make_regex_token_iterator(
						"aaa bbb",
						yggr::regular_parse::u32regex_parse::make_regex(str_re), -1);

				for(regex_token_iter_type i = iter, isize = regex_token_iter_type(); i != isize; ++i)
				{
					std::cout << yggr::utf8_string((*i).first, (*i).second, YGGR_STR_UTF8_STRING_CHARSET_NAME()) << std::endl;
					chk_vt.push_back(yggr::utf8_string((*i).first, (*i).second, YGGR_STR_UTF8_STRING_CHARSET_NAME()));
				}

				yggr_test_assert(chk_vt.size() == 2);
				yggr_test_assert(chk_vt[0] == "aaa");
				yggr_test_assert(chk_vt[1] == "bbb");
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
		yggr::utf8_string str_re = L"\\s+";
		yggr::utf8_string text = L"你你你 好好好";

		try
		{
			{
				typedef
					yggr::regular_parse::regex_token_iterator_t_maker
					<
						yggr::utf8_string::const_iterator,
						yggr::regular_parse::u32regex_parse::regex_type
					>::type regex_token_iter_type;

				std::vector<yggr::utf8_string> chk_vt;

				regex_token_iter_type iter =
					yggr::regular_parse::u32regex_parse::make_regex_token_iterator(
						text,
						yggr::regular_parse::u32regex_parse::make_regex(str_re), -1);

				for(regex_token_iter_type i = iter, isize = regex_token_iter_type(); i != isize; ++i)
				{
					std::cout << yggr::utf8_string((*i).first, (*i).second, YGGR_STR_UTF8_STRING_CHARSET_NAME()) << std::endl;
					chk_vt.push_back(yggr::utf8_string((*i).first, (*i).second, YGGR_STR_UTF8_STRING_CHARSET_NAME()));
				}

				yggr_test_assert(chk_vt.size() == 2);
				yggr_test_assert(chk_vt[0] == L"你你你");
				yggr_test_assert(chk_vt[1] == L"好好好");
			}

			{
				typedef
					yggr::regular_parse::regex_token_iterator_t_maker
					<
						yggr::utf8_string::iterator,
						yggr::regular_parse::u32regex_parse::regex_type
					>::type regex_token_iter_type;

				std::vector<yggr::utf8_string> chk_vt;

				regex_token_iter_type iter =
					yggr::regular_parse::u32regex_parse::make_regex_token_iterator(
						text.begin(), text.end(),
						yggr::regular_parse::u32regex_parse::make_regex(str_re), -1);

				for(regex_token_iter_type i = iter, isize = regex_token_iter_type(); i != isize; ++i)
				{
					std::cout << yggr::utf8_string((*i).first, (*i).second, YGGR_STR_UTF8_STRING_CHARSET_NAME()) << std::endl;
					chk_vt.push_back(yggr::utf8_string((*i).first, (*i).second, YGGR_STR_UTF8_STRING_CHARSET_NAME()));
				}

				yggr_test_assert(chk_vt.size() == 2);
				yggr_test_assert(chk_vt[0] == L"你你你");
				yggr_test_assert(chk_vt[1] == L"好好好");
			}

			{
				typedef
					yggr::regular_parse::regex_token_iterator_t_maker
					<
						yggr::utf8_string::non_const_iterator,
						yggr::regular_parse::u32regex_parse::regex_type
					>::type regex_token_iter_type;

				std::vector<yggr::utf8_string> chk_vt;

				regex_token_iter_type iter =
					yggr::regular_parse::u32regex_parse::make_regex_token_iterator(
						text.begin(), text.end(),
						yggr::regular_parse::u32regex_parse::make_regex(str_re), -1);

				for(regex_token_iter_type i = iter, isize = regex_token_iter_type(); i != isize; ++i)
				{
					std::cout << yggr::utf8_string((*i).first, (*i).second, YGGR_STR_UTF8_STRING_CHARSET_NAME()) << std::endl;
					chk_vt.push_back(yggr::utf8_string((*i).first, (*i).second, YGGR_STR_UTF8_STRING_CHARSET_NAME()));
				}

				yggr_test_assert(chk_vt.size() == 2);
				yggr_test_assert(chk_vt[0] == L"你你你");
				yggr_test_assert(chk_vt[1] == L"好好好");
			}

			{
				typedef
					yggr::regular_parse::regex_token_iterator_t_maker
					<
						const char*,
						yggr::regular_parse::u32regex_parse::regex_type
					>::type regex_token_iter_type;

				std::vector<yggr::utf8_string> chk_vt;

				regex_token_iter_type iter =
					yggr::regular_parse::u32regex_parse::make_regex_token_iterator(
						text.c_str(),
						yggr::regular_parse::u32regex_parse::make_regex(str_re), -1);

				for(regex_token_iter_type i = iter, isize = regex_token_iter_type(); i != isize; ++i)
				{
					std::cout << yggr::utf8_string((*i).first, (*i).second, YGGR_STR_UTF8_STRING_CHARSET_NAME()) << std::endl;
					chk_vt.push_back(yggr::utf8_string((*i).first, (*i).second, YGGR_STR_UTF8_STRING_CHARSET_NAME()));
				}

				yggr_test_assert(chk_vt.size() == 2);
				yggr_test_assert(chk_vt[0] == L"你你你");
				yggr_test_assert(chk_vt[1] == L"好好好");
			}

			{
				typedef
					yggr::regular_parse::regex_token_iterator_t_maker
					<
						const wchar_t*,
						yggr::regular_parse::u32regex_parse::regex_type
					>::type regex_token_iter_type;

				std::vector<yggr::utf8_string> chk_vt;

				regex_token_iter_type iter =
					yggr::regular_parse::u32regex_parse::make_regex_token_iterator(
						L"你你你 好好好",
						yggr::regular_parse::u32regex_parse::make_regex(str_re), -1);

				for(regex_token_iter_type i = iter, isize = regex_token_iter_type(); i != isize; ++i)
				{
					std::cout << yggr::utf8_string((*i).first, (*i).second) << std::endl;
					chk_vt.push_back(yggr::utf8_string((*i).first, (*i).second));
				}
				yggr_test_assert(chk_vt.size() == 2);
				yggr_test_assert(chk_vt[0] == L"你你你");
				yggr_test_assert(chk_vt[1] == L"好好好");
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

	{
		yggr::utf8_string str_re("<\\s*A\\s+[^>]*href\\s*=\\s*\"([^\"]*)\"\\s*>(\\w*)\\s*<.*>");
		yggr::utf8_string text("<a href=\"http://www.google.com\">google</a>");
		std::vector<yggr::utf8_string> chk_vt;
		try
		{
			{
				typedef
					yggr::regular_parse::regex_token_iterator_t_maker
					<
						yggr::utf8_string::const_iterator,
						yggr::regular_parse::u32regex_parse::regex_type
					>::type regex_token_iter_type;

				int submatchs[] = {1, 2};
				regex_token_iter_type iter =
					yggr::regular_parse::u32regex_parse::make_regex_token_iterator(text,
						yggr::regular_parse::u32regex_parse::make_regex(
							str_re,
							yggr::regular_parse::u32regex_parse::regex_base_type::normal
								| yggr::regular_parse::u32regex_parse::regex_base_type::icase),
						submatchs,
						boost::match_default);

				for(regex_token_iter_type i = iter, isize = regex_token_iter_type(); i != isize; ++i)
				{
					std::cout << yggr::utf8_string((*i).first, (*i).second, YGGR_STR_UTF8_STRING_CHARSET_NAME()) << std::endl;
					chk_vt.push_back(yggr::utf8_string((*i).first, (*i).second, YGGR_STR_UTF8_STRING_CHARSET_NAME()));
				}
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

		yggr_test_assert(chk_vt.size() == 2);
		yggr_test_assert(chk_vt[0] == "http://www.google.com");
		yggr_test_assert(chk_vt[1] == "google");
	}

	std::cout << "-------------test_utf8_make_regex_token_iterator end------------" << std::endl;
}

template<typename Container, typename Iter>
bool handler_split(Container& cont,
					const typename yggr::regular_parse::sub_match_t_maker<Iter>::type& m)
{
	typedef Container cont_type;
	typedef typename boost::range_value<cont_type>::type cont_value_type;
	typedef typename yggr::regular_parse::sub_match_t_maker<Iter>::type sub_match_type;

	typedef typename
		boost::mpl::if_c
		<
			sizeof(typename sub_match_type::value_type) == 1,
			yggr::charset::charset_name_t<yggr::utf8_string>,
			yggr::charset::charset_name_t<typename sub_match_type::value_type>
		>::type charset_name_type;


	if(m.matched)
	{
	    charset_name_type charset_name;
		std::inserter(cont, boost::end(cont)) = cont_value_type(m.first, m.second, charset_name);
	}
	return true;
}

void test_utf8_split1_1(void)
{
	{
		typedef std::vector<yggr::utf8_string> vt_type;

		yggr::utf8_string text("aaa bbb");

		try
		{
			std::size_t count = 0;
			{
				vt_type vt;
				count =
					yggr::regular_parse::u32regex_parse::split(
						boost::bind(&handler_split<vt_type, yggr::utf8_string::const_iterator>, boost::ref(vt), _1),
						text);

				yggr_test_assert(vt.size() == 2);
				yggr_test_assert(vt[0] == "aaa");
				yggr_test_assert(vt[1] == "bbb");
			}

			{
				vt_type vt;
				count =
					yggr::regular_parse::u32regex_parse::split(
						boost::bind(&handler_split<vt_type, yggr::utf8_string::iterator>, boost::ref(vt), _1),
						text.begin(), text.end());

				yggr_test_assert(vt.size() == 2);
				yggr_test_assert(vt[0] == "aaa");
				yggr_test_assert(vt[1] == "bbb");
			}

			{
				vt_type vt;
				count =
					yggr::regular_parse::u32regex_parse::split(
						boost::bind(&handler_split<vt_type, yggr::utf8_string::non_const_iterator>, boost::ref(vt), _1),
						text.begin(), text.end());

				yggr_test_assert(vt.size() == 2);
				yggr_test_assert(vt[0] == "aaa");
				yggr_test_assert(vt[1] == "bbb");
			}

			{
				vt_type vt;
				count =
					yggr::regular_parse::u32regex_parse::split(
						boost::bind(&handler_split<vt_type, const char*>, boost::ref(vt), _1),
						text.c_str());

				yggr_test_assert(vt.size() == 2);
				yggr_test_assert(vt[0] == "aaa");
				yggr_test_assert(vt[1] == "bbb");
			}

			{
				vt_type vt;
				count =
					yggr::regular_parse::u32regex_parse::split(
						boost::bind(&handler_split<vt_type, const char*>, boost::ref(vt), _1),
						"aaa bbb");

				yggr_test_assert(vt.size() == 2);
				yggr_test_assert(vt[0] == "aaa");
				yggr_test_assert(vt[1] == "bbb");
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
		typedef std::vector<yggr::utf8_string> vt_type;
		yggr::utf8_string text(L"你你你 好好好");

		try
		{
			std::size_t count = 0;
			{
				vt_type vt;
				count =
					yggr::regular_parse::u32regex_parse::split(
						boost::bind(&handler_split<vt_type, yggr::utf8_string::const_iterator>, boost::ref(vt), _1),
						text);

				yggr_test_assert(vt.size() == 2);
				yggr_test_assert(vt[0] == L"你你你");
				yggr_test_assert(vt[1] == L"好好好");
			}

			{
				vt_type vt;
				count =
					yggr::regular_parse::u32regex_parse::split(
						boost::bind(&handler_split<vt_type, yggr::utf8_string::iterator>, boost::ref(vt), _1),
						text.begin(), text.end());

				yggr_test_assert(vt.size() == 2);
				yggr_test_assert(vt[0] == L"你你你");
				yggr_test_assert(vt[1] == L"好好好");
			}

			{
				vt_type vt;
				count =
					yggr::regular_parse::u32regex_parse::split(
						boost::bind(&handler_split<vt_type, yggr::utf8_string::non_const_iterator>, boost::ref(vt), _1),
						text.begin(), text.end());

				yggr_test_assert(vt.size() == 2);
				yggr_test_assert(vt[0] == L"你你你");
				yggr_test_assert(vt[1] == L"好好好");
			}

			{
				vt_type vt;
				count =
					yggr::regular_parse::u32regex_parse::split(
						boost::bind(&handler_split<vt_type, const char*>, boost::ref(vt), _1),
						text.c_str());

				yggr_test_assert(vt.size() == 2);
				yggr_test_assert(vt[0] == L"你你你");
				yggr_test_assert(vt[1] == L"好好好");
			}

			{
				vt_type vt;
				count =
					yggr::regular_parse::u32regex_parse::split(
						boost::bind(&handler_split<vt_type, const wchar_t*>, boost::ref(vt), _1),
						L"你你你 好好好");

				yggr_test_assert(vt.size() == 2);
				yggr_test_assert(vt[0] == L"你你你");
				yggr_test_assert(vt[1] == L"好好好");
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

	std::cout << "-------------test_utf8_split1_1 end------------" << std::endl;
}

void test_utf8_split3_1(void)
{
	{
		typedef std::list<yggr::utf8_string> list_type;
		yggr::utf8_string text("<a href=\"http://www.google.com\">google</a>");
		yggr::utf8_string str_re("<\\s*A\\s+[^>]*href\\s*=\\s*\"([^\"]*)\"");

		try
		{
			std::size_t count = 0;

			{
				list_type list;
				count =
					yggr::regular_parse::u32regex_parse::split(
						boost::bind(&handler_split<list_type, yggr::utf8_string::const_iterator>, boost::ref(list), _1),
						text,
						yggr::regular_parse::u32regex_parse::make_regex(
							str_re,
							yggr::regular_parse::u32regex_parse::regex_base_type::icase
							| yggr::regular_parse::u32regex_parse::regex_base_type::normal),
						1);

				yggr_test_assert(list.size() == 1);
				yggr_test_assert(list.front() == "http://www.google.com");
			}

			{
				list_type list;
				int arr[] = {1};
				count =
					yggr::regular_parse::u32regex_parse::split(
						boost::bind(&handler_split<list_type, yggr::utf8_string::iterator>, boost::ref(list), _1),
						text.begin(), text.end(),
						yggr::regular_parse::u32regex_parse::make_regex(
							str_re,
							yggr::regular_parse::u32regex_parse::regex_base_type::icase
							| yggr::regular_parse::u32regex_parse::regex_base_type::normal),
						arr);

				yggr_test_assert(list.size() == 1);
				yggr_test_assert(list.front() == "http://www.google.com");
			}

			{
				list_type list;
				int arr[] = {1};
				count =
					yggr::regular_parse::u32regex_parse::split(
						boost::bind(&handler_split<list_type, yggr::utf8_string::non_const_iterator>, boost::ref(list), _1),
						text.begin(), text.end(),
						yggr::regular_parse::u32regex_parse::make_regex(
							str_re,
							yggr::regular_parse::u32regex_parse::regex_base_type::icase
							| yggr::regular_parse::u32regex_parse::regex_base_type::normal),
						arr);

				yggr_test_assert(list.size() == 1);
				yggr_test_assert(list.front() == "http://www.google.com");
			}

			{
				list_type list;
				int arr[] = {1};
				count =
					yggr::regular_parse::u32regex_parse::split(
						boost::bind(&handler_split<list_type, const char*>, boost::ref(list), _1),
						text.c_str(),
						yggr::regular_parse::u32regex_parse::make_regex(
							str_re,
							yggr::regular_parse::u32regex_parse::regex_base_type::icase
							| yggr::regular_parse::u32regex_parse::regex_base_type::normal),
						std::vector<yggr::u32>(arr, arr + 1));

				yggr_test_assert(list.size() == 1);
				yggr_test_assert(list.front() == "http://www.google.com");
			}

			{
				list_type list;
				int arr[] = {1};
				count =
					yggr::regular_parse::u32regex_parse::split(
						boost::bind(&handler_split<list_type, const char*>, boost::ref(list), _1),
						"<a href=\"http://www.google.com\">google</a>",
						yggr::regular_parse::u32regex_parse::make_regex(
							str_re,
							yggr::regular_parse::u32regex_parse::regex_base_type::icase
							| yggr::regular_parse::u32regex_parse::regex_base_type::normal),
						std::vector<yggr::f32>(arr, arr + 1));

				yggr_test_assert(list.size() == 1);
				yggr_test_assert(list.front() == "http://www.google.com");
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
		typedef std::list<yggr::utf8_string> list_type;
		yggr::utf8_string text(L"<a href=\"http://www.谷歌.com\">google</a>");
		yggr::utf8_string str_re(L"<\\s*A\\s+[^>]*href\\s*=\\s*\"([^\"]*)\"");

		try
		{
			std::size_t count = 0;

			{
				list_type list;
				count =
					yggr::regular_parse::u32regex_parse::split(
						boost::bind(&handler_split<list_type, yggr::utf8_string::const_iterator>, boost::ref(list), _1),
						text,
						yggr::regular_parse::u32regex_parse::make_regex(
							str_re,
							yggr::regular_parse::u32regex_parse::regex_base_type::icase
							| yggr::regular_parse::u32regex_parse::regex_base_type::normal),
						1);

				yggr_test_assert(list.size() == 1);
				yggr_test_assert(list.front() == L"http://www.谷歌.com");
			}

			{
				list_type list;
				int arr[] = {1};
				count =
					yggr::regular_parse::u32regex_parse::split(
						boost::bind(&handler_split<list_type, yggr::utf8_string::iterator>, boost::ref(list), _1),
						text.begin(), text.end(),
						yggr::regular_parse::u32regex_parse::make_regex(
							str_re,
							yggr::regular_parse::u32regex_parse::regex_base_type::icase
							| yggr::regular_parse::u32regex_parse::regex_base_type::normal),
						arr);

				yggr_test_assert(list.size() == 1);
				yggr_test_assert(list.front() == L"http://www.谷歌.com");
			}

			{
				list_type list;
				int arr[] = {1};
				count =
					yggr::regular_parse::u32regex_parse::split(
						boost::bind(&handler_split<list_type, yggr::utf8_string::non_const_iterator>, boost::ref(list), _1),
						text.begin(), text.end(),
						yggr::regular_parse::u32regex_parse::make_regex(
							str_re,
							yggr::regular_parse::u32regex_parse::regex_base_type::icase
							| yggr::regular_parse::u32regex_parse::regex_base_type::normal),
						arr);

				yggr_test_assert(list.size() == 1);
				yggr_test_assert(list.front() == L"http://www.谷歌.com");
			}

			{
				list_type list;
				int arr[] = {1};
				count =
					yggr::regular_parse::u32regex_parse::split(
						boost::bind(&handler_split<list_type, const char*>, boost::ref(list), _1),
						text.c_str(),
						yggr::regular_parse::u32regex_parse::make_regex(
							str_re,
							yggr::regular_parse::u32regex_parse::regex_base_type::icase
							| yggr::regular_parse::u32regex_parse::regex_base_type::normal),
						std::vector<yggr::u32>(arr, arr + 1));

				yggr_test_assert(list.size() == 1);
				yggr_test_assert(list.front() == L"http://www.谷歌.com");
			}

			{
				list_type list;
				int arr[] = {1};
				count =
					yggr::regular_parse::u32regex_parse::split(
						boost::bind(&handler_split<list_type, const wchar_t*>, boost::ref(list), _1),
						L"<a href=\"http://www.谷歌.com\">google</a>",
						yggr::regular_parse::u32regex_parse::make_regex(
							str_re,
							yggr::regular_parse::u32regex_parse::regex_base_type::icase
							| yggr::regular_parse::u32regex_parse::regex_base_type::normal),
						std::vector<yggr::f32>(arr, arr + 1));

				yggr_test_assert(list.size() == 1);
				yggr_test_assert(list.front() == L"http://www.谷歌.com");
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

	std::cout << "-------------test_utf8_split3_1 end------------" << std::endl;
}

void test_utf8_split3_1_1(void)
{
	typedef std::vector<yggr::utf8_string> vt_type;

	yggr::utf8_string text("  aaa bbb,,ccc#ddd  ");
	yggr::utf8_string str_re("\\s+|,+|#+");

	try
	{
		std::size_t count = 0;
		{
			vt_type vt;
			count =
				yggr::regular_parse::u32regex_parse::split(
					boost::bind(&handler_split<vt_type, yggr::utf8_string::const_iterator>, boost::ref(vt), _1),
					text,
					yggr::regular_parse::u32regex_parse::make_regex(str_re));

			yggr_test_assert(vt.size() == 4);
			vt[0] == "aaa";
			vt[1] == "bbb";
			vt[2] == "ccc";
			vt[3] == "ddd";
		}

		{
			vt_type vt;
			count =
				yggr::regular_parse::u32regex_parse::split(
					boost::bind(&handler_split<vt_type, yggr::utf8_string::iterator>, boost::ref(vt), _1),
					text.begin(), text.end(),
					yggr::regular_parse::u32regex_parse::make_regex(str_re));

			yggr_test_assert(vt.size() == 4);
			vt[0] == "aaa";
			vt[1] == "bbb";
			vt[2] == "ccc";
			vt[3] == "ddd";
		}

		{
			vt_type vt;
			count =
				yggr::regular_parse::u32regex_parse::split(
					boost::bind(&handler_split<vt_type, yggr::utf8_string::non_const_iterator>, boost::ref(vt), _1),
					text.begin(), text.end(),
					yggr::regular_parse::u32regex_parse::make_regex(str_re));

			yggr_test_assert(vt.size() == 4);
			vt[0] == "aaa";
			vt[1] == "bbb";
			vt[2] == "ccc";
			vt[3] == "ddd";
		}

		{
			vt_type vt;
			count =
				yggr::regular_parse::u32regex_parse::split(
					boost::bind(&handler_split<vt_type, const char*>, boost::ref(vt), _1),
					text.c_str(),
					yggr::regular_parse::u32regex_parse::make_regex(str_re));

			yggr_test_assert(vt.size() == 4);
			vt[0] == "aaa";
			vt[1] == "bbb";
			vt[2] == "ccc";
			vt[3] == "ddd";
		}

		{
			vt_type vt;
			count =
				yggr::regular_parse::u32regex_parse::split(
					boost::bind(&handler_split<vt_type, const char*>, boost::ref(vt), _1),
					"  aaa bbb,,ccc#ddd  ",
					yggr::regular_parse::u32regex_parse::make_regex(str_re));

			yggr_test_assert(vt.size() == 4);
			vt[0] == "aaa";
			vt[1] == "bbb";
			vt[2] == "ccc";
			vt[3] == "ddd";
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

#ifndef YGGR_NO_CWCHAR
	// ok, but I want to be lazy (*^_^*)
#endif // YGGR_NO_CWCHAR

	std::cout << "-------------test_utf8_split3_1_1 end------------" << std::endl;
}

void test_utf8_split3_1_2(void)
{
	typedef std::vector<yggr::utf8_string> vt_type;

	yggr::utf8_string text("  aaa bbb,,ccc#ddd  ");
	yggr::utf8_string str_re("\\s+|,+|#+");

	try
	{
		std::size_t count = 0;
		{
			vt_type vt;
			count =
				yggr::regular_parse::u32regex_parse::split(
					boost::bind(&handler_split<vt_type, yggr::utf8_string::const_iterator>, boost::ref(vt), _1),
					boost::bind(&handler_compare, _1, true),
					text,
					yggr::regular_parse::u32regex_parse::make_regex(str_re));

			yggr_test_assert(vt.size() == 4);
			vt[0] == "aaa";
			vt[1] == "bbb";
			vt[2] == "ccc";
			vt[3] == "ddd";
		}

		{
			vt_type vt;
			count =
				yggr::regular_parse::u32regex_parse::split(
					boost::bind(&handler_split<vt_type, yggr::utf8_string::iterator>, boost::ref(vt), _1),
					boost::bind(&handler_compare, _1, true),
					text.begin(), text.end(),
					yggr::regular_parse::u32regex_parse::make_regex(str_re) );

			yggr_test_assert(vt.size() == 4);
			vt[0] == "aaa";
			vt[1] == "bbb";
			vt[2] == "ccc";
			vt[3] == "ddd";
		}

		{
			vt_type vt;
			count =
				yggr::regular_parse::u32regex_parse::split(
					boost::bind(&handler_split<vt_type, yggr::utf8_string::non_const_iterator>, boost::ref(vt), _1),
					boost::bind(&handler_compare, _1, true),
					text.begin(), text.end(),
					yggr::regular_parse::u32regex_parse::make_regex(str_re) );

			yggr_test_assert(vt.size() == 4);
			vt[0] == "aaa";
			vt[1] == "bbb";
			vt[2] == "ccc";
			vt[3] == "ddd";
		}

		{
			vt_type vt;
			count =
				yggr::regular_parse::u32regex_parse::split(
					boost::bind(&handler_split<vt_type, const char*>, boost::ref(vt), _1),
					boost::bind(&handler_compare, _1, true),
					text.c_str(),
					yggr::regular_parse::u32regex_parse::make_regex(str_re) );

			yggr_test_assert(vt.size() == 4);
			vt[0] == "aaa";
			vt[1] == "bbb";
			vt[2] == "ccc";
			vt[3] == "ddd";
		}

		{
			vt_type vt;
			count =
				yggr::regular_parse::u32regex_parse::split(
					boost::bind(&handler_split<vt_type, const char*>, boost::ref(vt), _1),
					boost::bind(&handler_compare, _1, true),
					"  aaa bbb,,ccc#ddd  ",
					yggr::regular_parse::u32regex_parse::make_regex(str_re) );

			yggr_test_assert(vt.size() == 4);
			vt[0] == "aaa";
			vt[1] == "bbb";
			vt[2] == "ccc";
			vt[3] == "ddd";
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

#ifndef YGGR_NO_CWCHAR
	// ok, but I want to be lazy (*^_^*)
#endif // YGGR_NO_CWCHAR

	std::cout << "-------------test_utf8_split3_1_2 end------------" << std::endl;
}

void test_utf8_split3_1_3(void)
{
	typedef std::vector<yggr::utf8_string> vt_type;
	vt_type vt;
	yggr::utf8_string text("  aaa bbb,,ccc#ddd  ");
	yggr::utf8_string str_re("\\s+|,+|#+");

	try
	{
		std::size_t count = 0;
		{
			vt_type vt;
			count =
				yggr::regular_parse::u32regex_parse::split(
					boost::bind(&handler_split<vt_type, yggr::utf8_string::const_iterator>, boost::ref(vt), _1),
					text,
					yggr::regular_parse::u32regex_parse::make_regex(str_re),
					-1);
			yggr_test_assert(vt.size() == 4);
			vt[0] == "aaa";
			vt[1] == "bbb";
			vt[2] == "ccc";
			vt[3] == "ddd";
		}

		{
			vt_type vt;
			count =
				yggr::regular_parse::u32regex_parse::split(
					boost::bind(&handler_split<vt_type, yggr::utf8_string::iterator>, boost::ref(vt), _1),
					text.begin(), text.end(),
					yggr::regular_parse::u32regex_parse::make_regex(str_re),
					-1);
			yggr_test_assert(vt.size() == 4);
			vt[0] == "aaa";
			vt[1] == "bbb";
			vt[2] == "ccc";
			vt[3] == "ddd";
		}

		{
			vt_type vt;
			count =
				yggr::regular_parse::u32regex_parse::split(
					boost::bind(&handler_split<vt_type, yggr::utf8_string::non_const_iterator>, boost::ref(vt), _1),
					text.begin(), text.end(),
					yggr::regular_parse::u32regex_parse::make_regex(str_re),
					-1);
			yggr_test_assert(vt.size() == 4);
			vt[0] == "aaa";
			vt[1] == "bbb";
			vt[2] == "ccc";
			vt[3] == "ddd";
		}

		{
			vt_type vt;
			count =
				yggr::regular_parse::u32regex_parse::split(
					boost::bind(&handler_split<vt_type, const char*>, boost::ref(vt), _1),
					text.c_str(),
					yggr::regular_parse::u32regex_parse::make_regex(str_re),
					-1);
			yggr_test_assert(vt.size() == 4);
			vt[0] == "aaa";
			vt[1] == "bbb";
			vt[2] == "ccc";
			vt[3] == "ddd";
		}

		{
			vt_type vt;
			count =
				yggr::regular_parse::u32regex_parse::split(
					boost::bind(&handler_split<vt_type, const char*>, boost::ref(vt), _1),
					"  aaa bbb,,ccc#ddd  ",
					yggr::regular_parse::u32regex_parse::make_regex(str_re),
					-1);
			yggr_test_assert(vt.size() == 4);
			vt[0] == "aaa";
			vt[1] == "bbb";
			vt[2] == "ccc";
			vt[3] == "ddd";
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

#ifndef YGGR_NO_CWCHAR
	// ok, but I want to be lazy (*^_^*)
#endif // YGGR_NO_CWCHAR

	std::cout << "-------------test_utf8_split3_1_3 end------------" << std::endl;
}

void test_utf8_split3_1_4(void)
{
	typedef std::vector<yggr::utf8_string> vt_type;
	vt_type vt;
	yggr::utf8_string text("  aaa bbb,,ccc#ddd  ");
	yggr::utf8_string str_re("\\s+|,+|#+");

	try
	{
		std::size_t count = 0;
		{
			vt_type vt;
			count =
				yggr::regular_parse::u32regex_parse::split(
					boost::bind(&handler_split<vt_type, yggr::utf8_string::const_iterator>, boost::ref(vt), _1),
					boost::bind(&handler_compare, _1, true),
					text,
					yggr::regular_parse::u32regex_parse::make_regex(str_re),
					-1);
			yggr_test_assert(vt.size() == 4);
			vt[0] == "aaa";
			vt[1] == "bbb";
			vt[2] == "ccc";
			vt[3] == "ddd";
		}

		{
			vt_type vt;
			count =
				yggr::regular_parse::u32regex_parse::split(
					boost::bind(&handler_split<vt_type, yggr::utf8_string::iterator>, boost::ref(vt), _1),
					boost::bind(&handler_compare, _1, true),
					text.begin(), text.end(),
					yggr::regular_parse::u32regex_parse::make_regex(str_re),
					-1);
			yggr_test_assert(vt.size() == 4);
			vt[0] == "aaa";
			vt[1] == "bbb";
			vt[2] == "ccc";
			vt[3] == "ddd";
		}

		{
			vt_type vt;
			count =
				yggr::regular_parse::u32regex_parse::split(
					boost::bind(&handler_split<vt_type, yggr::utf8_string::non_const_iterator>, boost::ref(vt), _1),
					boost::bind(&handler_compare, _1, true),
					text.begin(), text.end(),
					yggr::regular_parse::u32regex_parse::make_regex(str_re),
					-1);
			yggr_test_assert(vt.size() == 4);
			vt[0] == "aaa";
			vt[1] == "bbb";
			vt[2] == "ccc";
			vt[3] == "ddd";
		}

		{
			vt_type vt;
			count =
				yggr::regular_parse::u32regex_parse::split(
					boost::bind(&handler_split<vt_type, const char*>, boost::ref(vt), _1),
					boost::bind(&handler_compare, _1, true),
					text.c_str(),
					yggr::regular_parse::u32regex_parse::make_regex(str_re),
					-1);
			yggr_test_assert(vt.size() == 4);
			vt[0] == "aaa";
			vt[1] == "bbb";
			vt[2] == "ccc";
			vt[3] == "ddd";
		}

		{
			vt_type vt;
			count =
				yggr::regular_parse::u32regex_parse::split(
					boost::bind(&handler_split<vt_type, const char*>, boost::ref(vt), _1),
					boost::bind(&handler_compare, _1, true),
					"  aaa bbb,,ccc#ddd  ",
					yggr::regular_parse::u32regex_parse::make_regex(str_re),
					-1);
			yggr_test_assert(vt.size() == 4);
			vt[0] == "aaa";
			vt[1] == "bbb";
			vt[2] == "ccc";
			vt[3] == "ddd";
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

#ifndef YGGR_NO_CWCHAR
	// ok, but I want to be lazy (*^_^*)
#endif // YGGR_NO_CWCHAR

	std::cout << "-------------test_utf8_split3_1_4 end------------" << std::endl;
}

void test_utf8_split4(void)
{
	typedef std::vector<yggr::utf8_string> vt_type;
	vt_type vt;
	yggr::utf8_string text("aaa/");
	yggr::utf8_string str_re("[/\\\\]");

	try
	{
		std::size_t count = 0;
		{
			vt_type vt;
			count =
				yggr::regular_parse::u32regex_parse::split(
					boost::bind(&handler_split<vt_type, yggr::utf8_string::const_iterator>, boost::ref(vt), _1),
					boost::bind(&handler_compare, _1, true),
					text,
					yggr::regular_parse::u32regex_parse::make_regex(str_re),
					-1);
			yggr_test_assert(vt.size() == 1);
			vt[0] == "aaa";
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

#ifndef YGGR_NO_CWCHAR
	// ok, but I want to be lazy (*^_^*)
#endif // YGGR_NO_CWCHAR

	std::cout << "-------------test_utf8_split4 end------------" << std::endl;
}

int main(int argc, char* argv[])
{
#ifndef YGGR_NO_CWCHAR
	TEST_PP_WIO_IMBUE("chs");
#endif // YGGR_NO_CWCHAR
	//boost::u32regex r = boost::make_u32regex("(?:\\A|.*\\\\)([^\\\\]+)");

	test_utf8_format();
	test_utf8_format2();

	test_utf8_make_regex_iterator();

	test_utf8_grep_1();
	test_utf8_grep_2();
	test_utf8_grep_3();

	test_utf8_make_regex_token_iterator();

	test_utf8_split1_1();

	test_utf8_split3_1();
	test_utf8_split3_1_1();
	test_utf8_split3_1_2();
	test_utf8_split3_1_3();
	test_utf8_split3_1_4();
	test_utf8_split4();

	wait_any_key(argc, argv);
	return 0;
}
