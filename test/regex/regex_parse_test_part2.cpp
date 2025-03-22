//regex_parse_search_test.cpp

#if !defined(_MSC_VER)
#	error "this file supports MSVC only, if you are using other compilers, please use the same name file in the charset_utf8_files"	
#endif // _MSC_VER

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/bind/bind.hpp>
#include <yggr/charset/const_strings.hpp>
#include <yggr/regular_parse/regex_parse.hpp>

#include <list>
#include <vector>
#include <sstream>
#include <cassert>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(charset)


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

void test_format(void)
{
	{
		typedef std::basic_stringstream<char> ss_type;
		typedef std::ostream_iterator<char> out_iter_type;
		yggr::string str_re("(a)|(b)");
		std::string str_fmt("(?1A)(?2B)");
		yggr::string text("aaacccbbb");

		yggr::string yggr_rst;
		std::string stl_rst;

		try
		{
			{
				typedef boost::match_results<yggr::string::const_iterator> match_results_type;

				match_results_type rst;

				bool bret =
					yggr::regular_parse::regex_parse::search(
							text, rst,
							yggr::regular_parse::regex_parse::make_regex(str_re));
				yggr_test_assert(bret);

				{
					yggr_rst = yggr::regular_parse::regex_parse::format(rst, str_fmt);
					yggr_test_assert(yggr_rst == "A");

					yggr_rst = yggr::regular_parse::regex_parse::format(yggr_rst.get_allocator(), rst, str_fmt);
					yggr_test_assert(yggr_rst == "A");

					yggr_rst = yggr::regular_parse::regex_parse::format<yggr::string>(rst, str_fmt);
					yggr_test_assert(yggr_rst == "A");

					yggr_rst = yggr::regular_parse::regex_parse::format<yggr::string>(yggr_rst.get_allocator(), rst, str_fmt);
					yggr_test_assert(yggr_rst == "A");

					stl_rst = yggr::regular_parse::regex_parse::format<std::string>(rst, str_fmt);
					yggr_test_assert(stl_rst == "A");

					stl_rst = yggr::regular_parse::regex_parse::format<std::string>(stl_rst.get_allocator(), rst, str_fmt);
					yggr_test_assert(stl_rst == "A");

					ss_type ss;
					yggr::regular_parse::regex_parse::format(out_iter_type(ss), rst, str_fmt);
					yggr_test_assert(ss.str() == "A");
				}

				{
					yggr_rst = yggr::regular_parse::regex_parse::format(rst, str_fmt.c_str());
					yggr_test_assert(yggr_rst == "A");

					yggr_rst = yggr::regular_parse::regex_parse::format(yggr_rst.get_allocator(), rst, str_fmt.c_str());
					yggr_test_assert(yggr_rst == "A");

					yggr_rst = yggr::regular_parse::regex_parse::format<yggr::string>(rst, str_fmt.c_str());
					yggr_test_assert(yggr_rst == "A");

					yggr_rst = yggr::regular_parse::regex_parse::format<yggr::string>(yggr_rst.get_allocator(), rst, str_fmt.c_str());
					yggr_test_assert(yggr_rst == "A");

					stl_rst = yggr::regular_parse::regex_parse::format<std::string>(rst, str_fmt.c_str());
					yggr_test_assert(stl_rst == "A");

					stl_rst = yggr::regular_parse::regex_parse::format<std::string>(stl_rst.get_allocator(), rst, str_fmt.c_str());
					yggr_test_assert(stl_rst == "A");

					ss_type ss;
					yggr::regular_parse::regex_parse::format(out_iter_type(ss), rst, str_fmt.c_str());
					yggr_test_assert(ss.str() == "A");
				}

				{
					yggr_rst = yggr::regular_parse::regex_parse::format(rst, "(?1A)(?2B)");
					yggr_test_assert(yggr_rst == "A");

					yggr_rst = yggr::regular_parse::regex_parse::format(yggr_rst.get_allocator(), rst, "(?1A)(?2B)");
					yggr_test_assert(yggr_rst == "A");

					yggr_rst = yggr::regular_parse::regex_parse::format<yggr::string>(rst, "(?1A)(?2B)");
					yggr_test_assert(yggr_rst == "A");

					yggr_rst = yggr::regular_parse::regex_parse::format<yggr::string>(yggr_rst.get_allocator(), rst, "(?1A)(?2B)");
					yggr_test_assert(yggr_rst == "A");

					stl_rst = yggr::regular_parse::regex_parse::format<std::string>(rst, "(?1A)(?2B)");
					yggr_test_assert(stl_rst == "A");

					stl_rst = yggr::regular_parse::regex_parse::format<std::string>(stl_rst.get_allocator(), rst, "(?1A)(?2B)");
					yggr_test_assert(stl_rst == "A");

					ss_type ss;
					yggr::regular_parse::regex_parse::format(out_iter_type(ss), rst, "(?1A)(?2B)");
					yggr_test_assert(ss.str() == "A");
				}
			}
		}
		catch(yggr::regular_parse::regex_parse::error_type& e)
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
		typedef std::basic_stringstream<wchar_t> ss_type;
		typedef std::ostream_iterator<wchar_t, wchar_t> out_iter_type;
		yggr::wstring str_re(L"(Äã)|(Âğ)");
		std::wstring str_fmt(L"(?1Äú)(?2_)");
		yggr::wstring text(L"ÄãÄãÄãºÃºÃºÃÂğÂğÂğ");

		yggr::wstring yggr_rst;
		std::wstring stl_rst;

		try
		{
			{
				typedef boost::match_results<yggr::wstring::const_iterator> match_results_type;

				match_results_type rst;

				bool bret =
					yggr::regular_parse::regex_parse::search(
							text, rst,
							yggr::regular_parse::regex_parse::make_regex(str_re));
				yggr_test_assert(bret);

				{
					yggr_rst = yggr::regular_parse::regex_parse::format(rst, str_fmt);
					yggr_test_assert(yggr_rst == L"Äú");

					yggr_rst = yggr::regular_parse::regex_parse::format(yggr_rst.get_allocator(), rst, str_fmt);
					yggr_test_assert(yggr_rst == L"Äú");

					yggr_rst = yggr::regular_parse::regex_parse::format<yggr::wstring>(rst, str_fmt);
					yggr_test_assert(yggr_rst == L"Äú");

					yggr_rst = yggr::regular_parse::regex_parse::format<yggr::wstring>(yggr_rst.get_allocator(), rst, str_fmt);
					yggr_test_assert(yggr_rst == L"Äú");

					stl_rst = yggr::regular_parse::regex_parse::format<std::wstring>(rst, str_fmt);
					yggr_test_assert(stl_rst == L"Äú");

					stl_rst = yggr::regular_parse::regex_parse::format<std::wstring>(stl_rst.get_allocator(), rst, str_fmt);
					yggr_test_assert(stl_rst == L"Äú");

					ss_type ss;
					yggr::regular_parse::regex_parse::format(out_iter_type(ss), rst, str_fmt);
					yggr_test_assert(ss.str() == L"Äú");
				}

				{
					yggr_rst = yggr::regular_parse::regex_parse::format(rst, str_fmt.c_str());
					yggr_test_assert(yggr_rst == L"Äú");

					yggr_rst = yggr::regular_parse::regex_parse::format(yggr_rst.get_allocator(), rst, str_fmt.c_str());
					yggr_test_assert(yggr_rst == L"Äú");

					yggr_rst = yggr::regular_parse::regex_parse::format<yggr::wstring>(rst, str_fmt.c_str());
					yggr_test_assert(yggr_rst == L"Äú");

					yggr_rst = yggr::regular_parse::regex_parse::format<yggr::wstring>(yggr_rst.get_allocator(), rst, str_fmt.c_str());
					yggr_test_assert(yggr_rst == L"Äú");

					stl_rst = yggr::regular_parse::regex_parse::format<std::wstring>(rst, str_fmt.c_str());
					yggr_test_assert(stl_rst == L"Äú");

					stl_rst = yggr::regular_parse::regex_parse::format<std::wstring>(stl_rst.get_allocator(), rst, str_fmt.c_str());
					yggr_test_assert(stl_rst == L"Äú");

					ss_type ss;
					yggr::regular_parse::regex_parse::format(out_iter_type(ss), rst, str_fmt.c_str());
					yggr_test_assert(ss.str() == L"Äú");
				}

				{
					yggr_rst = yggr::regular_parse::regex_parse::format(rst, L"(?1Äú)(?2_)");
					yggr_test_assert(yggr_rst == L"Äú");

					yggr_rst = yggr::regular_parse::regex_parse::format(yggr_rst.get_allocator(), rst, L"(?1Äú)(?2_)");
					yggr_test_assert(yggr_rst == L"Äú");

					yggr_rst = yggr::regular_parse::regex_parse::format<yggr::wstring>(rst, L"(?1Äú)(?2_)");
					yggr_test_assert(yggr_rst == L"Äú");

					yggr_rst = yggr::regular_parse::regex_parse::format<yggr::wstring>(yggr_rst.get_allocator(), rst, L"(?1Äú)(?2_)");
					yggr_test_assert(yggr_rst == L"Äú");

					stl_rst = yggr::regular_parse::regex_parse::format<std::wstring>(rst, L"(?1Äú)(?2_)");
					yggr_test_assert(stl_rst == L"Äú");

					stl_rst = yggr::regular_parse::regex_parse::format<std::wstring>(stl_rst.get_allocator(), rst, L"(?1Äú)(?2_)");
					yggr_test_assert(stl_rst == L"Äú");

					ss_type ss;
					yggr::regular_parse::regex_parse::format(out_iter_type(ss), rst, L"(?1Äú)(?2_)");
					yggr_test_assert(ss.str() == L"Äú");
				}
			}
		}
		catch(yggr::regular_parse::regex_parse::error_type& e)
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

	std::cout << "------------test_format end----------" << std::endl;
}

void test_format2(void)
{
	{
		yggr::string str_re("(a)|(b)");
		std::string str_fmt("(?1A)(?2B)");
		yggr::string text("aaacccbbb");

		try
		{
			{
				typedef boost::match_results<yggr::string::const_iterator> match_results_type;
				match_results_type rst;

				yggr::string::const_iterator start = text.begin(), last = text.end();
				yggr::string str_rst;

				for(; yggr::regular_parse::regex_parse::search(
							start, last, rst,
							yggr::regular_parse::regex_parse::make_regex(str_re)); start = rst[0].second)
				{
					str_rst = yggr::regular_parse::regex_parse::format(rst, str_fmt);
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
		catch(yggr::regular_parse::regex_parse::error_type& e)
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
		yggr::wstring str_re(L"(Äã)|(Âğ)");
		std::wstring str_fmt(L"(?1Äú)(?2_)");
		yggr::wstring text(L"ÄãÄãÄãºÃºÃºÃÂğÂğÂğ");

		try
		{
			{
				typedef boost::match_results<yggr::wstring::const_iterator> match_results_type;
				match_results_type rst;

				yggr::wstring::const_iterator start = text.begin(), last = text.end();
				yggr::wstring str_rst;

				for(; yggr::regular_parse::regex_parse::search(
							start, last, rst,
							yggr::regular_parse::regex_parse::make_regex(str_re)); start = rst[0].second)
				{
					str_rst = yggr::regular_parse::regex_parse::format(rst, str_fmt);
					if(*start == L'Äã')
					{
						yggr_test_assert(str_rst == L"Äú");
					}

					if(*start == L'Âğ')
					{
						yggr_test_assert(str_rst == L"_");
					}

					std::wcout << str_rst << std::endl;
				}
			}
		}
		catch(yggr::regular_parse::regex_parse::error_type& e)
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

	std::cout << "------------test_format2 end----------" << std::endl;
}

void test_make_regex_iterator(void)
{
	{
		yggr::string str_re("^[[:space:]]*"
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

		yggr::string text("template <typename T> \nclass A \n{\n}; template <typename T> \nclass B \n{\n};");

		try
		{
			{
				typedef boost::regex_iterator<yggr::string::const_iterator> regex_iter_type;
				regex_iter_type iter
					= yggr::regular_parse::regex_parse::make_regex_iterator(
						text,
						yggr::regular_parse::regex_parse::make_regex(str_re));

				for(int i = 0;iter != regex_iter_type(); ++iter, ++i)
				{
					if(i == 0)
					{
						yggr_test_assert(std::string((*iter)[5].first, (*iter)[5].second) == "A");
						int nnn  = 0; // disable optimization
					}

					if(i == 1)
					{
						yggr_test_assert(std::string((*iter)[5].first, (*iter)[5].second) == "B");
						int nnn  = 0; // disable optimization
					}
					std::cout << std::string((*iter)[5].first, (*iter)[5].second) << std::endl;
					std::cout << std::string((*iter)[6].first, (*iter)[6].second) << std::endl;
					std::cout << (*iter)[5].first - text.begin() << std::endl;
				}
			}

			{
				typedef boost::regex_iterator<yggr::string::iterator> regex_iter_type;
				regex_iter_type iter
					= yggr::regular_parse::regex_parse::make_regex_iterator(
						text.begin(), text.end(),
						yggr::regular_parse::regex_parse::make_regex(str_re));

				for(int i = 0;iter != regex_iter_type(); ++iter, ++i)
				{
					if(i == 0)
					{
						yggr_test_assert(std::string((*iter)[5].first, (*iter)[5].second) == "A");
						int nnn  = 0; // disable optimization
					}

					if(i == 1)
					{
						yggr_test_assert(std::string((*iter)[5].first, (*iter)[5].second) == "B");
						int nnn  = 0; // disable optimization
					}
					std::cout << std::string((*iter)[5].first, (*iter)[5].second) << std::endl;
					std::cout << std::string((*iter)[6].first, (*iter)[6].second) << std::endl;
					std::cout << (*iter)[5].first - text.begin() << std::endl;
				}
			}

			{
				typedef boost::regex_iterator<const char*> regex_iter_type;
				regex_iter_type iter
					= yggr::regular_parse::regex_parse::make_regex_iterator(
						text.c_str(),
						yggr::regular_parse::regex_parse::make_regex(str_re));

				for(int i = 0;iter != regex_iter_type(); ++iter, ++i)
				{
					if(i == 0)
					{
						yggr_test_assert(std::string((*iter)[5].first, (*iter)[5].second) == "A");
						int nnn  = 0; // disable optimization
					}

					if(i == 1)
					{
						yggr_test_assert(std::string((*iter)[5].first, (*iter)[5].second) == "B");
						int nnn  = 0; // disable optimization
					}
					std::cout << std::string((*iter)[5].first, (*iter)[5].second) << std::endl;
					std::cout << std::string((*iter)[6].first, (*iter)[6].second) << std::endl;
					std::cout << (*iter)[5].first - text.begin() << std::endl;
				}
			}

			{
				typedef boost::regex_iterator<const char*> regex_iter_type;
				regex_iter_type iter
					= yggr::regular_parse::regex_parse::make_regex_iterator(
						"template <typename T> \nclass A \n{\n}; template <typename T> \nclass B \n{\n};",
						yggr::regular_parse::regex_parse::make_regex(str_re));

				for(int i = 0;iter != regex_iter_type(); ++iter, ++i)
				{
					if(i == 0)
					{
						yggr_test_assert(std::string((*iter)[5].first, (*iter)[5].second) == "A");
						int nnn  = 0; // disable optimization
					}

					if(i == 1)
					{
						yggr_test_assert(std::string((*iter)[5].first, (*iter)[5].second) == "B");
						int nnn  = 0; // disable optimization
					}
					std::cout << std::string((*iter)[5].first, (*iter)[5].second) << std::endl;
					std::cout << std::string((*iter)[6].first, (*iter)[6].second) << std::endl;
				}
			}
		}
		catch(yggr::regular_parse::regex_parse::error_type& e)
		{
			std::cout << e.what() << std::endl;
			yggr_test_assert(false);
		}
		catch(...)
		{
			yggr_test_assert(false);
		}
	}

	std::cout << "------------test_make_regex_iterator end----------" << std::endl;
}

#include <map>

typedef std::map<std::string, std::string::difference_type, std::less<std::string> > map_type;


template<typename Iter>
bool handler_grep1(map_type& m, Iter base,
						const boost::match_results<Iter>& what)
{
	m[std::string(what[5].first, what[5].second) + std::string(what[6].first, what[6].second)] = what[5].first - base;
	return true;
}


void test_grep_1(void)
{
	map_type map;
	std::string str_re("^[[:space:]]*"
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

	std::string text("template <typename T> \nclass A \n{\n}; template <typename T> \nclass B \n{\n};");

	try
	{
		std::size_t count = 0;
		count =
			yggr::regular_parse::regex_parse::grep(
				boost::bind(&handler_grep1<std::string::const_iterator>, boost::ref(map), text.begin(), _1),
				text,
				yggr::regular_parse::regex_parse::make_regex(str_re));
		yggr_test_assert(count == 2);
		yggr_test_assert(map["A"] == 29);
		yggr_test_assert(map["B"] == 66);

		count =
			yggr::regular_parse::regex_parse::grep(
				boost::bind(&handler_grep1<std::string::iterator>, boost::ref(map), text.begin(), _1),
				text.begin(), text.end(),
				yggr::regular_parse::regex_parse::make_regex(str_re));
		yggr_test_assert(count == 2);
		yggr_test_assert(map["A"] == 29);
		yggr_test_assert(map["B"] == 66);

		count =
			yggr::regular_parse::regex_parse::grep(
				boost::bind(&handler_grep1<const char*>, boost::ref(map), text.c_str(), _1),
				text.c_str(),
				yggr::regular_parse::regex_parse::make_regex(str_re));
		yggr_test_assert(count == 2);
		yggr_test_assert(map["A"] == 29);
		yggr_test_assert(map["B"] == 66);

		char ctext[] = "template <typename T> \nclass A \n{\n}; template <typename T> \nclass B \n{\n};";
		count =
			yggr::regular_parse::regex_parse::grep(
				boost::bind(&handler_grep1<const char*>, boost::ref(map), &ctext[0], _1),
				ctext,
				yggr::regular_parse::regex_parse::make_regex(str_re));
		yggr_test_assert(count == 2);
		yggr_test_assert(map["A"] == 29);
		yggr_test_assert(map["B"] == 66);
	}
	catch(yggr::regular_parse::regex_parse::error_type& e)
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

	std::cout << "-------------test_grep_1 end------------" << std::endl;
}

void test_grep_2(void)
{
	map_type map;
	std::string str_re("^[[:space:]]*"
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

	std::string text("template <typename T> \nclass A \n{\n}; template <typename T> \nclass B \n{\n};");

	std::vector<char> cvt(text.size(), 0);
	memcpy(&cvt[0], &text[0], text.size());

	try
	{
		std::size_t count = 0;
		count =
			yggr::regular_parse::regex_parse::grep(
				boost::bind(&handler_grep1<std::vector<char>::const_iterator>, boost::ref(map), cvt.begin(), _1),
				cvt,
				yggr::regular_parse::regex_parse::make_regex(str_re));
		yggr_test_assert(count == 2);
		yggr_test_assert(map["A"] == 29);
		yggr_test_assert(map["B"] == 66);

		count =
			yggr::regular_parse::regex_parse::grep(
				boost::bind(&handler_grep1<std::vector<char>::iterator>, boost::ref(map), cvt.begin(), _1),
				cvt.begin(), cvt.end(),
				yggr::regular_parse::regex_parse::make_regex(str_re));
		yggr_test_assert(count == 2);
		yggr_test_assert(map["A"] == 29);
		yggr_test_assert(map["B"] == 66);

		cvt.push_back(0);
		count =
			yggr::regular_parse::regex_parse::grep(
				boost::bind(&handler_grep1<const char*>, boost::ref(map), &cvt[0], _1),
				&cvt[0],
				yggr::regular_parse::regex_parse::make_regex(str_re));
		yggr_test_assert(count == 2);
		yggr_test_assert(map["A"] == 29);
		yggr_test_assert(map["B"] == 66);
	}
	catch(yggr::regular_parse::regex_parse::error_type& e)
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

	std::cout << "-------------test_grep_2 end------------" << std::endl;
}

bool handler_compare(bool l, bool r)
{
	return l == r;
}

void test_grep_3(void)
{
	map_type map;
	std::string str_re("^[[:space:]]*"
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

	std::string text("template <typename T> \nclass A \n{\n}; template <typename T> \nclass B \n{\n};");

	try
	{
		std::size_t count = 0;
		count =
			yggr::regular_parse::regex_parse::grep(
				boost::bind(&handler_grep1<std::string::const_iterator>, boost::ref(map), text.begin(), _1),
				boost::bind(&handler_compare, _1, true),
				text,
				yggr::regular_parse::regex_parse::make_regex(str_re));
		yggr_test_assert(count == 2);
		yggr_test_assert(map["A"] == 29);
		yggr_test_assert(map["B"] == 66);

		count =
			yggr::regular_parse::regex_parse::grep(
				boost::bind(&handler_grep1<std::string::iterator>, boost::ref(map), text.begin(), _1),
				boost::bind(&handler_compare, _1, true),
				text.begin(), text.end(),
				yggr::regular_parse::regex_parse::make_regex(str_re));
		yggr_test_assert(count == 2);
		yggr_test_assert(map["A"] == 29);
		yggr_test_assert(map["B"] == 66);

		count =
			yggr::regular_parse::regex_parse::grep(
				boost::bind(&handler_grep1<const char*>, boost::ref(map), text.c_str(), _1),
				boost::bind(&handler_compare, _1, true),
				text.c_str(),
				yggr::regular_parse::regex_parse::make_regex(str_re));
		yggr_test_assert(count == 2);
		yggr_test_assert(map["A"] == 29);
		yggr_test_assert(map["B"] == 66);

		char ctext[] = "template <typename T> \nclass A \n{\n}; template <typename T> \nclass B \n{\n};";
		count =
			yggr::regular_parse::regex_parse::grep(
				boost::bind(&handler_grep1<const char*>, boost::ref(map), &ctext[0], _1),
				boost::bind(&handler_compare, _1, true),
				ctext,
				yggr::regular_parse::regex_parse::make_regex(str_re));
		yggr_test_assert(count == 2);
		yggr_test_assert(map["A"] == 29);
		yggr_test_assert(map["B"] == 66);
	}
	catch(yggr::regular_parse::regex_parse::error_type& e)
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

	std::cout << "-------------test_grep_3 end------------" << std::endl;
}

//<a href="http://www.google.com">google</a>

void test_make_regex_token_iterator(void)
{
	{
		std::string str_re = "\\s+";
		yggr::string text = "aaa bbb";

		try
		{
			{
				typedef boost::regex_token_iterator<yggr::string::const_iterator> regex_token_iter_type;
				std::vector<std::string> chk_vt;

				regex_token_iter_type iter =
					yggr::regular_parse::regex_parse::make_regex_token_iterator(
						text,
						yggr::regular_parse::regex_parse::make_regex(str_re), -1);

				for(regex_token_iter_type i = iter, isize = regex_token_iter_type(); i != isize; ++i)
				{
					std::cout << *i << std::endl;
					chk_vt.push_back(std::string((*i).first, (*i).second));
				}

				yggr_test_assert(chk_vt.size() == 2);
				yggr_test_assert(chk_vt[0] == "aaa");
				yggr_test_assert(chk_vt[1] == "bbb");
			}

			{
				typedef boost::regex_token_iterator<yggr::string::iterator> regex_token_iter_type;
				std::vector<std::string> chk_vt;

				regex_token_iter_type iter =
					yggr::regular_parse::regex_parse::make_regex_token_iterator(
						text.begin(), text.end(),
						yggr::regular_parse::regex_parse::make_regex(str_re), -1);

				for(regex_token_iter_type i = iter, isize = regex_token_iter_type(); i != isize; ++i)
				{
					std::cout << *i << std::endl;
					chk_vt.push_back(std::string((*i).first, (*i).second));
				}

				yggr_test_assert(chk_vt.size() == 2);
				yggr_test_assert(chk_vt[0] == "aaa");
				yggr_test_assert(chk_vt[1] == "bbb");
			}

			{
				typedef boost::regex_token_iterator<const char*> regex_token_iter_type;
				std::vector<std::string> chk_vt;

				regex_token_iter_type iter =
					yggr::regular_parse::regex_parse::make_regex_token_iterator(
						text.c_str(),
						yggr::regular_parse::regex_parse::make_regex(str_re), -1);

				for(regex_token_iter_type i = iter, isize = regex_token_iter_type(); i != isize; ++i)
				{
					std::cout << *i << std::endl;
					chk_vt.push_back(std::string((*i).first, (*i).second));
				}

				yggr_test_assert(chk_vt.size() == 2);
				yggr_test_assert(chk_vt[0] == "aaa");
				yggr_test_assert(chk_vt[1] == "bbb");
			}

			{
				typedef boost::regex_token_iterator<const char*> regex_token_iter_type;
				std::vector<std::string> chk_vt;

				regex_token_iter_type iter =
					yggr::regular_parse::regex_parse::make_regex_token_iterator(
						"aaa bbb",
						yggr::regular_parse::regex_parse::make_regex(str_re), -1);

				for(regex_token_iter_type i = iter, isize = regex_token_iter_type(); i != isize; ++i)
				{
					std::cout << *i << std::endl;
					chk_vt.push_back(std::string((*i).first, (*i).second));
				}
				yggr_test_assert(chk_vt.size() == 2);
				yggr_test_assert(chk_vt[0] == "aaa");
				yggr_test_assert(chk_vt[1] == "bbb");
			}
		}
		catch(yggr::regular_parse::regex_parse::error_type& e)
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
		std::wstring str_re = L"\\s+";
		yggr::wstring text = L"ÄãÄãÄã ºÃºÃºÃ";

		try
		{
			{
				typedef boost::regex_token_iterator<yggr::wstring::const_iterator> regex_token_iter_type;
				std::vector<std::wstring> chk_vt;

				regex_token_iter_type iter =
					yggr::regular_parse::regex_parse::make_regex_token_iterator(
						text,
						yggr::regular_parse::regex_parse::make_regex(str_re), -1);

				for(regex_token_iter_type i = iter, isize = regex_token_iter_type(); i != isize; ++i)
				{
					std::wcout << *i << std::endl;
					chk_vt.push_back(std::wstring((*i).first, (*i).second));
				}

				yggr_test_assert(chk_vt.size() == 2);
				yggr_test_assert(chk_vt[0] == L"ÄãÄãÄã");
				yggr_test_assert(chk_vt[1] == L"ºÃºÃºÃ");
			}

			{
				typedef boost::regex_token_iterator<yggr::wstring::iterator> regex_token_iter_type;
				std::vector<std::wstring> chk_vt;

				regex_token_iter_type iter =
					yggr::regular_parse::regex_parse::make_regex_token_iterator(
						text.begin(), text.end(),
						yggr::regular_parse::regex_parse::make_regex(str_re), -1);

				for(regex_token_iter_type i = iter, isize = regex_token_iter_type(); i != isize; ++i)
				{
					std::wcout << *i << std::endl;
					chk_vt.push_back(std::wstring((*i).first, (*i).second));
				}

				yggr_test_assert(chk_vt.size() == 2);
				yggr_test_assert(chk_vt[0] == L"ÄãÄãÄã");
				yggr_test_assert(chk_vt[1] == L"ºÃºÃºÃ");
			}

			{
				typedef boost::regex_token_iterator<const wchar_t*> regex_token_iter_type;
				std::vector<std::wstring> chk_vt;

				regex_token_iter_type iter =
					yggr::regular_parse::regex_parse::make_regex_token_iterator(
						text.c_str(),
						yggr::regular_parse::regex_parse::make_regex(str_re), -1);

				for(regex_token_iter_type i = iter, isize = regex_token_iter_type(); i != isize; ++i)
				{
					std::wcout << *i << std::endl;
					chk_vt.push_back(std::wstring((*i).first, (*i).second));
				}

				yggr_test_assert(chk_vt.size() == 2);
				yggr_test_assert(chk_vt[0] == L"ÄãÄãÄã");
				yggr_test_assert(chk_vt[1] == L"ºÃºÃºÃ");
			}

			{
				typedef boost::regex_token_iterator<const wchar_t*> regex_token_iter_type;
				std::vector<std::wstring> chk_vt;

				regex_token_iter_type iter =
					yggr::regular_parse::regex_parse::make_regex_token_iterator(
						L"ÄãÄãÄã ºÃºÃºÃ",
						yggr::regular_parse::regex_parse::make_regex(str_re), -1);

				for(regex_token_iter_type i = iter, isize = regex_token_iter_type(); i != isize; ++i)
				{
					std::wcout << *i << std::endl;
					chk_vt.push_back(std::wstring((*i).first, (*i).second));
				}
				yggr_test_assert(chk_vt.size() == 2);
				yggr_test_assert(chk_vt[0] == L"ÄãÄãÄã");
				yggr_test_assert(chk_vt[1] == L"ºÃºÃºÃ");
			}
		}
		catch(yggr::regular_parse::regex_parse::error_type& e)
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
		std::string str_re("<\\s*A\\s+[^>]*href\\s*=\\s*\"([^\"]*)\"\\s*>(\\w*)\\s*<.*>");
		std::string text("<a href=\"http://www.google.com\">google</a>");
		std::vector<std::string> chk_vt;
		try
		{
			{
				typedef boost::regex_token_iterator<std::string::const_iterator> regex_token_iter_type;
				int submatchs[] = {1, 2};
				regex_token_iter_type iter =
					yggr::regular_parse::regex_parse::make_regex_token_iterator(text,
						yggr::regular_parse::regex_parse::make_regex(
							str_re,
							yggr::regular_parse::regex_parse::regex_base_type::normal
								| yggr::regular_parse::regex_parse::regex_base_type::icase),
						submatchs,
						boost::match_default);

				for(regex_token_iter_type i = iter, isize = regex_token_iter_type(); i != isize; ++i)
				{
					std::cout << *i << std::endl;
					chk_vt.push_back(std::string((*i).first, (*i).second));
				}
			}
		}
		catch(yggr::regular_parse::regex_parse::error_type& e)
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

	std::cout << "-------------test_make_regex_token_iterator end------------" << std::endl;
}

int main(int argc, char* argv[])
{

#ifndef YGGR_NO_CWCHAR
	TEST_PP_WIO_IMBUE("chs");
#endif // YGGR_NO_CWCHAR

	test_format();
	test_format2();

	test_make_regex_iterator();

	test_grep_1();
	test_grep_2();
	test_grep_3();

	test_make_regex_token_iterator();

	wait_any_key(argc, argv);

	return 0;
}
