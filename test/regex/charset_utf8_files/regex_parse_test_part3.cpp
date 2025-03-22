//regex_parse_search_test.cpp

#if defined(_MSC_VER)
#	error "this file not supports MSVC only, if you are using other compilers, please use the same name file in the ../"
#endif // _MSC_VER

#include <test/wait_any_key/wait_any_key.hpp>
#include <list>
#include <vector>
#include <sstream>
#include <cassert>

#include <boost/bind.hpp>

#include <yggr/charset/const_strings.hpp>

#include <yggr/regular_parse/regex_parse.hpp>

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

bool handler_compare(bool l, bool r)
{
	return l == r;
}

template<typename Container, typename Iter>
bool handler_split(Container& cont, const boost::sub_match<Iter>& m)
{
	typedef Container cont_type;
	typedef typename boost::range_value<cont_type>::type cont_value_type;

	if(m.matched)
	{
		std::inserter(cont, boost::end(cont)) = cont_value_type(m.first, m.second);
	}
	return true;
}

void test_split1_1(void)
{
	{
		typedef std::vector<std::string> vt_type;
		std::string text("aaa bbb");

		try
		{
			std::size_t count = 0;
			{
				vt_type vt;
				count =
					yggr::regular_parse::regex_parse::split(
						boost::bind(&handler_split<vt_type, std::string::const_iterator>, boost::ref(vt), _1),
						text);

				yggr_test_assert(vt.size() == 2);
				yggr_test_assert(vt[0] == "aaa");
				yggr_test_assert(vt[1] == "bbb");
			}

			{
				vt_type vt;
				count =
					yggr::regular_parse::regex_parse::split(
						boost::bind(&handler_split<vt_type, std::string::iterator>, boost::ref(vt), _1),
						text.begin(), text.end());

				yggr_test_assert(vt.size() == 2);
				yggr_test_assert(vt[0] == "aaa");
				yggr_test_assert(vt[1] == "bbb");
			}

			{
				vt_type vt;
				count =
					yggr::regular_parse::regex_parse::split(
						boost::bind(&handler_split<vt_type, const char*>, boost::ref(vt), _1),
						text.c_str());

				yggr_test_assert(vt.size() == 2);
				yggr_test_assert(vt[0] == "aaa");
				yggr_test_assert(vt[1] == "bbb");
			}

			{
				vt_type vt;
				count =
					yggr::regular_parse::regex_parse::split(
						boost::bind(&handler_split<vt_type, const char*>, boost::ref(vt), _1),
						"aaa bbb");

				yggr_test_assert(vt.size() == 2);
				yggr_test_assert(vt[0] == "aaa");
				yggr_test_assert(vt[1] == "bbb");
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
		typedef std::vector<std::wstring> vt_type;
		std::wstring text(L"你你你 好好好");

		try
		{
			std::size_t count = 0;
			{
				vt_type vt;
				count =
					yggr::regular_parse::regex_parse::split(
						boost::bind(&handler_split<vt_type, std::wstring::const_iterator>, boost::ref(vt), _1),
						text);

				yggr_test_assert(vt.size() == 2);
				yggr_test_assert(vt[0] == L"你你你");
				yggr_test_assert(vt[1] == L"好好好");
			}

			{
				vt_type vt;
				count =
					yggr::regular_parse::regex_parse::split(
						boost::bind(&handler_split<vt_type, std::wstring::iterator>, boost::ref(vt), _1),
						text.begin(), text.end());

				yggr_test_assert(vt.size() == 2);
				yggr_test_assert(vt[0] == L"你你你");
				yggr_test_assert(vt[1] == L"好好好");
			}

			{
				vt_type vt;
				count =
					yggr::regular_parse::regex_parse::split(
						boost::bind(&handler_split<vt_type, const wchar_t*>, boost::ref(vt), _1),
						text.c_str());

				yggr_test_assert(vt.size() == 2);
				yggr_test_assert(vt[0] == L"你你你");
				yggr_test_assert(vt[1] == L"好好好");
			}

			{
				vt_type vt;
				count =
					yggr::regular_parse::regex_parse::split(
						boost::bind(&handler_split<vt_type, const wchar_t*>, boost::ref(vt), _1),
						L"你你你 好好好");

				yggr_test_assert(vt.size() == 2);
				yggr_test_assert(vt[0] == L"你你你");
				yggr_test_assert(vt[1] == L"好好好");
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

	std::cout << "-------------test_split1_1 end------------" << std::endl;
}

void test_split3_1(void)
{
	{
		typedef std::list<std::string> list_type;
		std::string text("<a href=\"http://www.google.com\">google</a>");
		std::string str_re("<\\s*A\\s+[^>]*href\\s*=\\s*\"([^\"]*)\"");

		try
		{
			std::size_t count = 0;

			{
				list_type list;
				count =
					yggr::regular_parse::regex_parse::split(
						boost::bind(&handler_split<list_type, std::string::const_iterator>, boost::ref(list), _1),
						text,
						yggr::regular_parse::regex_parse::make_regex(
							str_re,
							yggr::regular_parse::regex_parse::regex_base_type::icase
							| yggr::regular_parse::regex_parse::regex_base_type::normal),
						1);

				yggr_test_assert(list.size() == 1);
				yggr_test_assert(list.front() == "http://www.google.com");
			}

			{
				list_type list;
				int arr[] = {1};
				count =
					yggr::regular_parse::regex_parse::split(
						boost::bind(&handler_split<list_type, std::string::iterator>, boost::ref(list), _1),
						text.begin(), text.end(),
						yggr::regular_parse::regex_parse::make_regex(
							str_re,
							yggr::regular_parse::regex_parse::regex_base_type::icase
							| yggr::regular_parse::regex_parse::regex_base_type::normal),
						arr);

				yggr_test_assert(list.size() == 1);
				yggr_test_assert(list.front() == "http://www.google.com");
			}

			{
				list_type list;
				int arr[] = {1};
				count =
					yggr::regular_parse::regex_parse::split(
						boost::bind(&handler_split<list_type, const char*>, boost::ref(list), _1),
						text.c_str(),
						yggr::regular_parse::regex_parse::make_regex(
							str_re,
							yggr::regular_parse::regex_parse::regex_base_type::icase
							| yggr::regular_parse::regex_parse::regex_base_type::normal),
						std::vector<yggr::u32>(arr, arr + 1));

				yggr_test_assert(list.size() == 1);
				yggr_test_assert(list.front() == "http://www.google.com");
			}

			{
				list_type list;
				int arr[] = {1};
				count =
					yggr::regular_parse::regex_parse::split(
						boost::bind(&handler_split<list_type, const char*>, boost::ref(list), _1),
						"<a href=\"http://www.google.com\">google</a>",
						yggr::regular_parse::regex_parse::make_regex(
							str_re,
							yggr::regular_parse::regex_parse::regex_base_type::icase
							| yggr::regular_parse::regex_parse::regex_base_type::normal),
						std::vector<yggr::f32>(arr, arr + 1));

				yggr_test_assert(list.size() == 1);
				yggr_test_assert(list.front() == "http://www.google.com");
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
		typedef std::list<std::wstring> list_type;
		std::wstring text(L"<a href=\"http://www.谷歌.com\">google</a>");
		std::wstring str_re(L"<\\s*A\\s+[^>]*href\\s*=\\s*\"([^\"]*)\"");

		try
		{
			std::size_t count = 0;

			{
				list_type list;
				count =
					yggr::regular_parse::regex_parse::split(
						boost::bind(&handler_split<list_type, std::wstring::const_iterator>, boost::ref(list), _1),
						text,
						yggr::regular_parse::regex_parse::make_regex(
							str_re,
							yggr::regular_parse::regex_parse::regex_base_type::icase
							| yggr::regular_parse::regex_parse::regex_base_type::normal),
						1);

				yggr_test_assert(list.size() == 1);
				yggr_test_assert(list.front() == L"http://www.谷歌.com");
			}

			{
				list_type list;
				int arr[] = {1};
				count =
					yggr::regular_parse::regex_parse::split(
						boost::bind(&handler_split<list_type, std::wstring::iterator>, boost::ref(list), _1),
						text.begin(), text.end(),
						yggr::regular_parse::regex_parse::make_regex(
							str_re,
							yggr::regular_parse::regex_parse::regex_base_type::icase
							| yggr::regular_parse::regex_parse::regex_base_type::normal),
						arr);

				yggr_test_assert(list.size() == 1);
				yggr_test_assert(list.front() == L"http://www.谷歌.com");
			}

			{
				list_type list;
				int arr[] = {1};
				count =
					yggr::regular_parse::regex_parse::split(
						boost::bind(&handler_split<list_type, const wchar_t*>, boost::ref(list), _1),
						text.c_str(),
						yggr::regular_parse::regex_parse::make_regex(
							str_re,
							yggr::regular_parse::regex_parse::regex_base_type::icase
							| yggr::regular_parse::regex_parse::regex_base_type::normal),
						std::vector<yggr::u32>(arr, arr + 1));

				yggr_test_assert(list.size() == 1);
				yggr_test_assert(list.front() == L"http://www.谷歌.com");
			}

			{
				list_type list;
				int arr[] = {1};
				count =
					yggr::regular_parse::regex_parse::split(
						boost::bind(&handler_split<list_type, const wchar_t*>, boost::ref(list), _1),
						L"<a href=\"http://www.谷歌.com\">google</a>",
						yggr::regular_parse::regex_parse::make_regex(
							str_re,
							yggr::regular_parse::regex_parse::regex_base_type::icase
							| yggr::regular_parse::regex_parse::regex_base_type::normal),
						std::vector<yggr::f32>(arr, arr + 1));

				yggr_test_assert(list.size() == 1);
				yggr_test_assert(list.front() == L"http://www.谷歌.com");
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

	std::cout << "-------------test_split3_1 end------------" << std::endl;
}

void test_split3_1_1(void)
{
	typedef std::vector<std::string> vt_type;

	std::string text("  aaa bbb,,ccc#ddd  ");
	std::string str_re("\\s+|,+|#+");

	try
	{
		std::size_t count = 0;
		{
			vt_type vt;
			count =
				yggr::regular_parse::regex_parse::split(
					boost::bind(&handler_split<vt_type, std::string::const_iterator>, boost::ref(vt), _1),
					text,
					yggr::regular_parse::regex_parse::make_regex(str_re));

			yggr_test_assert(vt.size() == 4);
			vt[0] == "aaa";
			vt[1] == "bbb";
			vt[2] == "ccc";
			vt[3] == "ddd";
		}

		{
			vt_type vt;
			count =
				yggr::regular_parse::regex_parse::split(
					boost::bind(&handler_split<vt_type, std::string::iterator>, boost::ref(vt), _1),
					text.begin(), text.end(),
					yggr::regular_parse::regex_parse::make_regex(str_re));

			yggr_test_assert(vt.size() == 4);
			vt[0] == "aaa";
			vt[1] == "bbb";
			vt[2] == "ccc";
			vt[3] == "ddd";
		}

		{
			vt_type vt;
			count =
				yggr::regular_parse::regex_parse::split(
					boost::bind(&handler_split<vt_type, const char*>, boost::ref(vt), _1),
					text.c_str(),
					yggr::regular_parse::regex_parse::make_regex(str_re));

			yggr_test_assert(vt.size() == 4);
			vt[0] == "aaa";
			vt[1] == "bbb";
			vt[2] == "ccc";
			vt[3] == "ddd";
		}

		{
			vt_type vt;
			count =
				yggr::regular_parse::regex_parse::split(
					boost::bind(&handler_split<vt_type, const char*>, boost::ref(vt), _1),
					"  aaa bbb,,ccc#ddd  ",
					yggr::regular_parse::regex_parse::make_regex(str_re));

			yggr_test_assert(vt.size() == 4);
			vt[0] == "aaa";
			vt[1] == "bbb";
			vt[2] == "ccc";
			vt[3] == "ddd";
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

#ifndef YGGR_NO_CWCHAR
	// ok, but I want to be lazy (*^_^*)
#endif // YGGR_NO_CWCHAR

	std::cout << "-------------test_split3_1_1 end------------" << std::endl;
}

void test_split3_1_2(void)
{
	typedef std::vector<std::string> vt_type;

	std::string text("  aaa bbb,,ccc#ddd  ");
	std::string str_re("\\s+|,+|#+");

	try
	{
		std::size_t count = 0;
		{
			vt_type vt;
			count =
				yggr::regular_parse::regex_parse::split(
					boost::bind(&handler_split<vt_type, std::string::const_iterator>, boost::ref(vt), _1),
					boost::bind(&handler_compare, _1, true),
					text,
					yggr::regular_parse::regex_parse::make_regex(str_re));

			yggr_test_assert(vt.size() == 4);
			vt[0] == "aaa";
			vt[1] == "bbb";
			vt[2] == "ccc";
			vt[3] == "ddd";
		}

		{
			vt_type vt;
			count =
				yggr::regular_parse::regex_parse::split(
					boost::bind(&handler_split<vt_type, std::string::iterator>, boost::ref(vt), _1),
					boost::bind(&handler_compare, _1, true),
					text.begin(), text.end(),
					yggr::regular_parse::regex_parse::make_regex(str_re) );

			yggr_test_assert(vt.size() == 4);
			vt[0] == "aaa";
			vt[1] == "bbb";
			vt[2] == "ccc";
			vt[3] == "ddd";
		}

		{
			vt_type vt;
			count =
				yggr::regular_parse::regex_parse::split(
					boost::bind(&handler_split<vt_type, const char*>, boost::ref(vt), _1),
					boost::bind(&handler_compare, _1, true),
					text.c_str(),
					yggr::regular_parse::regex_parse::make_regex(str_re) );

			yggr_test_assert(vt.size() == 4);
			vt[0] == "aaa";
			vt[1] == "bbb";
			vt[2] == "ccc";
			vt[3] == "ddd";
		}

		{
			vt_type vt;
			count =
				yggr::regular_parse::regex_parse::split(
					boost::bind(&handler_split<vt_type, const char*>, boost::ref(vt), _1),
					boost::bind(&handler_compare, _1, true),
					"  aaa bbb,,ccc#ddd  ",
					yggr::regular_parse::regex_parse::make_regex(str_re) );

			yggr_test_assert(vt.size() == 4);
			vt[0] == "aaa";
			vt[1] == "bbb";
			vt[2] == "ccc";
			vt[3] == "ddd";
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

#ifndef YGGR_NO_CWCHAR
	// ok, but I want to be lazy (*^_^*)
#endif // YGGR_NO_CWCHAR

	std::cout << "-------------test_split3_1_2 end------------" << std::endl;
}

void test_split3_1_3(void)
{
	typedef std::vector<std::string> vt_type;
	vt_type vt;
	std::string text("  aaa bbb,,ccc#ddd  ");
	std::string str_re("\\s+|,+|#+");

	try
	{
		std::size_t count = 0;
		{
			vt_type vt;
			count =
				yggr::regular_parse::regex_parse::split(
					boost::bind(&handler_split<vt_type, std::string::const_iterator>, boost::ref(vt), _1),
					text,
					yggr::regular_parse::regex_parse::make_regex(str_re),
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
				yggr::regular_parse::regex_parse::split(
					boost::bind(&handler_split<vt_type, std::string::iterator>, boost::ref(vt), _1),
					text.begin(), text.end(),
					yggr::regular_parse::regex_parse::make_regex(str_re),
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
				yggr::regular_parse::regex_parse::split(
					boost::bind(&handler_split<vt_type, const char*>, boost::ref(vt), _1),
					text.c_str(),
					yggr::regular_parse::regex_parse::make_regex(str_re),
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
				yggr::regular_parse::regex_parse::split(
					boost::bind(&handler_split<vt_type, const char*>, boost::ref(vt), _1),
					"  aaa bbb,,ccc#ddd  ",
					yggr::regular_parse::regex_parse::make_regex(str_re),
					-1);
			yggr_test_assert(vt.size() == 4);
			vt[0] == "aaa";
			vt[1] == "bbb";
			vt[2] == "ccc";
			vt[3] == "ddd";
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

#ifndef YGGR_NO_CWCHAR
	// ok, but I want to be lazy (*^_^*)
#endif // YGGR_NO_CWCHAR

	std::cout << "-------------test_split3_1_2 end------------" << std::endl;
}

void test_split3_1_4(void)
{
	typedef std::vector<std::string> vt_type;
	vt_type vt;
	std::string text("  aaa bbb,,ccc#ddd  ");
	std::string str_re("\\s+|,+|#+");

	try
	{
		std::size_t count = 0;
		{
			vt_type vt;
			count =
				yggr::regular_parse::regex_parse::split(
					boost::bind(&handler_split<vt_type, std::string::const_iterator>, boost::ref(vt), _1),
					boost::bind(&handler_compare, _1, true),
					text,
					yggr::regular_parse::regex_parse::make_regex(str_re),
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
				yggr::regular_parse::regex_parse::split(
					boost::bind(&handler_split<vt_type, std::string::iterator>, boost::ref(vt), _1),
					boost::bind(&handler_compare, _1, true),
					text.begin(), text.end(),
					yggr::regular_parse::regex_parse::make_regex(str_re),
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
				yggr::regular_parse::regex_parse::split(
					boost::bind(&handler_split<vt_type, const char*>, boost::ref(vt), _1),
					boost::bind(&handler_compare, _1, true),
					text.c_str(),
					yggr::regular_parse::regex_parse::make_regex(str_re),
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
				yggr::regular_parse::regex_parse::split(
					boost::bind(&handler_split<vt_type, const char*>, boost::ref(vt), _1),
					boost::bind(&handler_compare, _1, true),
					"  aaa bbb,,ccc#ddd  ",
					yggr::regular_parse::regex_parse::make_regex(str_re),
					-1);
			yggr_test_assert(vt.size() == 4);
			vt[0] == "aaa";
			vt[1] == "bbb";
			vt[2] == "ccc";
			vt[3] == "ddd";
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

#ifndef YGGR_NO_CWCHAR
	// ok, but I want to be lazy (*^_^*)
#endif // YGGR_NO_CWCHAR

	std::cout << "-------------test_split3_1_2 end------------" << std::endl;
}

int main(int argc, char* argv[])
{
#ifndef YGGR_NO_CWCHAR
	TEST_PP_WIO_IMBUE("chs");
#endif // YGGR_NO_CWCHAR

	test_split1_1();

	test_split3_1();
	test_split3_1_1();
	test_split3_1_2();
	test_split3_1_3();
	test_split3_1_4();

	wait_any_key(argc, argv);

	return 0;
}
