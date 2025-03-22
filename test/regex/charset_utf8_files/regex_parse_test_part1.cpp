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

// ------------------------------test match---------------------------------
void test_match(void)
{
	{
		yggr::string text("abb");
		std::string str_re("(a)(?(1)BB|ccc)");

		try
		{
			bool bret = true;

			bret = bret &&
				yggr::regular_parse::regex_parse::match(
							text,
							yggr::regular_parse::regex_parse::make_regex(
								str_re,
								yggr::regular_parse::regex_parse::regex_base_type::icase
									| yggr::regular_parse::regex_parse::regex_base_type::normal));

			bret = bret &&
				yggr::regular_parse::regex_parse::match(
							text.begin(), text.end(),
							yggr::regular_parse::regex_parse::make_regex(
								str_re.begin(), str_re.end(),
								yggr::regular_parse::regex_parse::regex_base_type::icase
									| yggr::regular_parse::regex_parse::regex_base_type::normal));

			bret = bret &&
				yggr::regular_parse::regex_parse::match(
							text.c_str(),
							yggr::regular_parse::regex_parse::make_regex(
								str_re.c_str(),
								yggr::regular_parse::regex_parse::regex_base_type::icase
									| yggr::regular_parse::regex_parse::regex_base_type::normal));

			bret = bret &&
				yggr::regular_parse::regex_parse::match(
							"abb",
							yggr::regular_parse::regex_parse::make_regex(
								"(a)(?(1)BB|ccc)",
								yggr::regular_parse::regex_parse::regex_base_type::icase
									| yggr::regular_parse::regex_parse::regex_base_type::normal));

			yggr_test_assert(bret);
		}
		catch(const yggr::regular_parse::regex_parse::error_type& e)
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
		std::wstring str_re(L"(你)(?(1)好好|吗吗吗)");
		yggr::wstring text(L"你好好");

		try
		{
			bool bret = true;

			bret = bret &&
				yggr::regular_parse::regex_parse::match(
							text,
							yggr::regular_parse::regex_parse::make_regex(
								str_re,
								yggr::regular_parse::regex_parse::regex_base_type::icase
									| yggr::regular_parse::regex_parse::regex_base_type::normal));

			bret = bret &&
				yggr::regular_parse::regex_parse::match(
							text.begin(), text.end(),
							yggr::regular_parse::regex_parse::make_regex(
								str_re.begin(), str_re.end(),
								yggr::regular_parse::regex_parse::regex_base_type::icase
									| yggr::regular_parse::regex_parse::regex_base_type::normal));

			bret = bret &&
				yggr::regular_parse::regex_parse::match(
							text.c_str(),
							yggr::regular_parse::regex_parse::make_regex(
								str_re.c_str(),
								yggr::regular_parse::regex_parse::regex_base_type::icase
									| yggr::regular_parse::regex_parse::regex_base_type::normal));

			bret = bret &&
				yggr::regular_parse::regex_parse::match(
							L"你好好",
							yggr::regular_parse::regex_parse::make_regex(
								L"(你)(?(1)好好|吗吗吗)",
								yggr::regular_parse::regex_parse::regex_base_type::icase
									| yggr::regular_parse::regex_parse::regex_base_type::normal));

			yggr_test_assert(bret);
		}
		catch(const yggr::regular_parse::regex_parse::error_type& e)
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

	std::cout << "-------------test_match end------------" << std::endl;
}

template<typename Iter, typename Alloc>
void handler_match(bool bret, const boost::match_results<Iter, Alloc>& m)
{
	typedef typename boost::iterator_value<Iter>::type char_type;

	if(!bret)
	{
		get_ostream<char_type>() << "!!!empty match!!!" << std::endl;
	}
	else
	{
		for(int i = 0, isize = static_cast<int>(m.size()); i < isize; ++i)
		{
			get_ostream<char_type>() << "i = " << i << " : " << m[i] << std::endl;
		}
	}
}

void test_match2(void)
{
	{
		std::string text("aaaabbbb");
		yggr::string str_re("(a*)(b*)");

		try
		{
			bool bret = true;
			{
				boost::match_results<std::string::const_iterator> rst;
				bret = bret &&
					yggr::regular_parse::regex_parse::match(
						text, rst,
						yggr::regular_parse::regex_parse::make_regex(str_re));
				handler_match(bret, rst);
			}

			{
				boost::match_results<std::string::iterator> rst;
				bret = bret &&
					yggr::regular_parse::regex_parse::match(
						text.begin(), text.end(), rst,
						yggr::regular_parse::regex_parse::make_regex(str_re.begin(), str_re.end()));
				handler_match(bret, rst);
			}

			{
				boost::match_results<const char*> rst;
				bret = bret &&
					yggr::regular_parse::regex_parse::match(
						text.c_str(), rst,
						yggr::regular_parse::regex_parse::make_regex(str_re.c_str()));
				handler_match(bret, rst);
			}

			{
				boost::match_results<const char*> rst;
				bret = bret &&
					yggr::regular_parse::regex_parse::match(
						"aaaabbbb", rst,
						yggr::regular_parse::regex_parse::make_regex("(a*)(b*)"));
				handler_match(bret, rst);
			}

			yggr_test_assert(bret);
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
		std::wstring text(L"你你你你好好好好");
		yggr::wstring str_re(L"(你*)(好*)");

		try
		{
			bool bret = true;
			{
				boost::match_results<std::wstring::const_iterator> rst;
				bret = bret &&
					yggr::regular_parse::regex_parse::match(
						text, rst,
						yggr::regular_parse::regex_parse::make_regex(str_re));
				handler_match(bret, rst);
			}

			{
				boost::match_results<std::wstring::iterator> rst;
				bret = bret &&
					yggr::regular_parse::regex_parse::match(
						text.begin(), text.end(), rst,
						yggr::regular_parse::regex_parse::make_regex(str_re.begin(), str_re.end()));
				handler_match(bret, rst);
			}

			{
				boost::match_results<const wchar_t*> rst;
				bret = bret &&
					yggr::regular_parse::regex_parse::match(
						text.c_str(), rst,
						yggr::regular_parse::regex_parse::make_regex(str_re.c_str()));
				handler_match(bret, rst);
			}

			{
				boost::match_results<const wchar_t*> rst;
				bret = bret &&
					yggr::regular_parse::regex_parse::match(
						L"你你你你好好好好", rst,
						yggr::regular_parse::regex_parse::make_regex(L"(你*)(好*)"));
				handler_match(bret, rst);
			}

			yggr_test_assert(bret);
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

	std::cout << "-------------test_match2 end------------" << std::endl;
}

void test_match3(void)
{
	std::string str_re("[[:alpha:]]*");
	{
		std::vector<std::string::value_type> vt;
		vt.push_back('a');
		vt.push_back('b');
		vt.push_back('c');

		try
		{
			bool bret =
				yggr::regular_parse::regex_parse::match(
					vt, yggr::regular_parse::regex_parse::make_regex(str_re));
			yggr_test_assert(bret);
		}
		catch(yggr::regular_parse::regex_parse::error_type& e)
		{
			std::cout << e.what() << std::endl;
		}
		catch(...)
		{
			yggr_test_assert(false);
		}
	}

	{
		std::vector<std::string::value_type> vt;
		vt.push_back('a');
		vt.push_back('b');
		vt.push_back('c');
		vt.push_back('0'); // result false

		try
		{
			bool bret =
				yggr::regular_parse::regex_parse::match(
					vt, yggr::regular_parse::regex_parse::make_regex(str_re));
			yggr_test_assert(!bret);
		}
		catch(yggr::regular_parse::regex_parse::error_type& e)
		{
			std::cout << e.what() << std::endl;
		}
		catch(...)
		{
			yggr_test_assert(false);
		}
	}

	std::cout << "-------------test_match3 end------------" << std::endl;
}

void test_match4(void)
{
	std::string str_re("[[:alpha:]]*");
	{
		std::vector<std::string::value_type> vt;
		vt.push_back('a');
		vt.push_back('b');
		vt.push_back('c');

		try
		{
			boost::match_results<std::vector<std::string::value_type>::const_iterator> rst;
			bool bret =
				yggr::regular_parse::regex_parse::match(
					vt, rst, yggr::regular_parse::regex_parse::make_regex(str_re));
			handler_match(bret, rst);
			yggr_test_assert(bret);
		}
		catch(yggr::regular_parse::regex_parse::error_type& e)
		{
			std::cout << e.what() << std::endl;
		}
		catch(...)
		{
			yggr_test_assert(false);
		}
	}

	{
		std::vector<std::string::value_type> vt;
		vt.push_back('a');
		vt.push_back('b');
		vt.push_back('c');
		vt.push_back('0'); // result false

		try
		{
			boost::match_results<std::vector<std::string::value_type>::const_iterator> rst;
			bool bret =
				yggr::regular_parse::regex_parse::match(
					vt, rst, yggr::regular_parse::regex_parse::make_regex(str_re));
			handler_match(bret, rst);
			yggr_test_assert(!bret);
		}
		catch(yggr::regular_parse::regex_parse::error_type& e)
		{
			std::cout << e.what() << std::endl;
		}
		catch(...)
		{
			yggr_test_assert(false);
		}
	}

	std::cout << "-------------test_match4 end------------" << std::endl;
}

// ------------------------------test search---------------------------------

void test_search(void)
{
{
		yggr::string text("abb");
		std::string str_re("(a)(?(1)BB|ccc)");

		try
		{
			bool bret = true;

			bret = bret &&
				yggr::regular_parse::regex_parse::search(
							text,
							yggr::regular_parse::regex_parse::make_regex(
								str_re,
								yggr::regular_parse::regex_parse::regex_base_type::icase
									| yggr::regular_parse::regex_parse::regex_base_type::normal));

			bret = bret &&
				yggr::regular_parse::regex_parse::search(
							text.begin(), text.end(),
							yggr::regular_parse::regex_parse::make_regex(
								str_re.begin(), str_re.end(),
								yggr::regular_parse::regex_parse::regex_base_type::icase
									| yggr::regular_parse::regex_parse::regex_base_type::normal));

			bret = bret &&
				yggr::regular_parse::regex_parse::search(
							text.c_str(),
							yggr::regular_parse::regex_parse::make_regex(
								str_re.c_str(),
								yggr::regular_parse::regex_parse::regex_base_type::icase
									| yggr::regular_parse::regex_parse::regex_base_type::normal));

			bret = bret &&
				yggr::regular_parse::regex_parse::search(
							"abb",
							yggr::regular_parse::regex_parse::make_regex(
								"(a)(?(1)BB|ccc)",
								yggr::regular_parse::regex_parse::regex_base_type::icase
									| yggr::regular_parse::regex_parse::regex_base_type::normal));

			yggr_test_assert(bret);
		}
		catch(const yggr::regular_parse::regex_parse::error_type& e)
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
		std::wstring str_re(L"(你)(?(1)好好|吗吗吗)");
		yggr::wstring text(L"你好好");

		try
		{
			bool bret = true;

			bret = bret &&
				yggr::regular_parse::regex_parse::search(
							text,
							yggr::regular_parse::regex_parse::make_regex(
								str_re,
								yggr::regular_parse::regex_parse::regex_base_type::icase
									| yggr::regular_parse::regex_parse::regex_base_type::normal));

			bret = bret &&
				yggr::regular_parse::regex_parse::search(
							text.begin(), text.end(),
							yggr::regular_parse::regex_parse::make_regex(
								str_re.begin(), str_re.end(),
								yggr::regular_parse::regex_parse::regex_base_type::icase
									| yggr::regular_parse::regex_parse::regex_base_type::normal));

			bret = bret &&
				yggr::regular_parse::regex_parse::search(
							text.c_str(),
							yggr::regular_parse::regex_parse::make_regex(
								str_re.c_str(),
								yggr::regular_parse::regex_parse::regex_base_type::icase
									| yggr::regular_parse::regex_parse::regex_base_type::normal));

			bret = bret &&
				yggr::regular_parse::regex_parse::search(
							L"你好好",
							yggr::regular_parse::regex_parse::make_regex(
								L"(你)(?(1)好好|吗吗吗)",
								yggr::regular_parse::regex_parse::regex_base_type::icase
									| yggr::regular_parse::regex_parse::regex_base_type::normal));

			yggr_test_assert(bret);
		}
		catch(const yggr::regular_parse::regex_parse::error_type& e)
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

	std::cout << "-------------test_search end------------" << std::endl;
}

template<typename Iter, typename Alloc>
void handler_search(bool bret, const boost::match_results<Iter, Alloc>& m)
{
	typedef typename boost::iterator_value<Iter>::type char_type;

	if(!bret)
	{
		get_ostream<char_type>() << "!!!empty match!!!" << std::endl;
	}
	else
	{
		for(int i = 0, isize = static_cast<int>(m.size()); i < isize; ++i)
		{
			get_ostream<char_type>() << "i = " << i << " : " << m[i] << std::endl;
		}
	}
}

void test_search2(void)
{
	{
		std::string text("aaaabbbb");
		yggr::string str_re("(a*)(b*)");

		try
		{
			bool bret = true;
			{
				boost::match_results<std::string::const_iterator> rst;
				bret = bret &&
					yggr::regular_parse::regex_parse::search(
						text, rst,
						yggr::regular_parse::regex_parse::make_regex(str_re));
				handler_search(bret, rst);
			}

			{
				boost::match_results<std::string::iterator> rst;
				bret = bret &&
					yggr::regular_parse::regex_parse::search(
						text.begin(), text.end(), rst,
						yggr::regular_parse::regex_parse::make_regex(str_re.begin(), str_re.end()));
				handler_search(bret, rst);
			}

			{
				boost::match_results<const char*> rst;
				bret = bret &&
					yggr::regular_parse::regex_parse::search(
						text.c_str(), rst,
						yggr::regular_parse::regex_parse::make_regex(str_re.c_str()));
				handler_search(bret, rst);
			}

			{
				boost::match_results<const char*> rst;
				bret = bret &&
					yggr::regular_parse::regex_parse::search(
						"aaaabbbb", rst,
						yggr::regular_parse::regex_parse::make_regex("(a*)(b*)"));
				handler_search(bret, rst);
			}

			yggr_test_assert(bret);
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
		std::wstring text(L"你你你你好好好好");
		yggr::wstring str_re(L"(你*)(好*)");

		try
		{
			bool bret = true;
			{
				boost::match_results<std::wstring::const_iterator> rst;
				bret = bret &&
					yggr::regular_parse::regex_parse::search(
						text, rst,
						yggr::regular_parse::regex_parse::make_regex(str_re));
				handler_search(bret, rst);
			}

			{
				boost::match_results<std::wstring::iterator> rst;
				bret = bret &&
					yggr::regular_parse::regex_parse::search(
						text.begin(), text.end(), rst,
						yggr::regular_parse::regex_parse::make_regex(str_re.begin(), str_re.end()));
				handler_search(bret, rst);
			}

			{
				boost::match_results<const wchar_t*> rst;
				bret = bret &&
					yggr::regular_parse::regex_parse::search(
						text.c_str(), rst,
						yggr::regular_parse::regex_parse::make_regex(str_re.c_str()));
				handler_search(bret, rst);
			}

			{
				boost::match_results<const wchar_t*> rst;
				bret = bret &&
					yggr::regular_parse::regex_parse::search(
						L"你你你你好好好好", rst,
						yggr::regular_parse::regex_parse::make_regex(L"(你*)(好*)"));
				handler_search(bret, rst);
			}

			yggr_test_assert(bret);
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

	std::cout << "-------------test_search2 end------------" << std::endl;
}

void test_search3(void)
{
	std::string str_re("[[:alpha:]]*");
	{
		std::vector<std::string::value_type> vt;
		vt.push_back('a');
		vt.push_back('b');
		vt.push_back('c');

		try
		{
			bool bret =
				yggr::regular_parse::regex_parse::search(
					vt, yggr::regular_parse::regex_parse::make_regex(str_re));
			yggr_test_assert(bret);
		}
		catch(yggr::regular_parse::regex_parse::error_type& e)
		{
			std::cout << e.what() << std::endl;
		}
		catch(...)
		{
			yggr_test_assert(false);
		}
	}

	{
		std::vector<std::string::value_type> vt;
		vt.push_back('a');
		vt.push_back('b');
		vt.push_back('c');
		vt.push_back('0'); // result false

		try
		{
			bool bret =
				yggr::regular_parse::regex_parse::search(
					vt, yggr::regular_parse::regex_parse::make_regex(str_re));
			yggr_test_assert(bret);
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

	std::cout << "-------------test_search3 end------------" << std::endl;
}

void test_search4(void)
{
	std::string str_re("[[:alpha:]]*");
	{
		std::vector<std::string::value_type> vt;
		vt.push_back('a');
		vt.push_back('b');
		vt.push_back('c');

		try
		{
			boost::match_results<std::vector<std::string::value_type>::const_iterator> rst;
			bool bret =
				yggr::regular_parse::regex_parse::search(
					vt, rst, yggr::regular_parse::regex_parse::make_regex(str_re));
			handler_match(bret, rst);
			yggr_test_assert(bret);
		}
		catch(yggr::regular_parse::regex_parse::error_type& e)
		{
			std::cout << e.what() << std::endl;
		}
		catch(...)
		{
			yggr_test_assert(false);
		}
	}

	{
		std::vector<std::string::value_type> vt;
		vt.push_back('a');
		vt.push_back('b');
		vt.push_back('c');
		vt.push_back('0');

		try
		{
			boost::match_results<std::vector<std::string::value_type>::const_iterator> rst;
			bool bret =
				yggr::regular_parse::regex_parse::search(
					vt, rst, yggr::regular_parse::regex_parse::make_regex(str_re));
			handler_match(bret, rst);
			yggr_test_assert(bret);
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

	std::cout << "-------------test_search4 end------------" << std::endl;
}

// ------------------------------test replace---------------------------------
void test_replace1(void)
{
	{
		yggr::string str_re("(a)|(b)");
		std::string str_fmt("(?1_)(?2_)");
		yggr::string text("aaacccbbb");

		yggr::string yggr_rst;
		std::string stl_rst;

		try
		{
			{
				yggr_rst = yggr::regular_parse::regex_parse::replace(
								text, yggr::regular_parse::regex_parse::make_regex(str_re), str_fmt);
				yggr_test_assert(yggr_rst == "___ccc___");

				yggr_rst = yggr::regular_parse::regex_parse::replace(
								yggr_rst.get_allocator(),
								text, yggr::regular_parse::regex_parse::make_regex(str_re),
								str_fmt);
				yggr_test_assert(yggr_rst == "___ccc___");

				yggr_rst = yggr::regular_parse::regex_parse::replace<yggr::string>(
								text, yggr::regular_parse::regex_parse::make_regex(str_re), str_fmt);
				yggr_test_assert(yggr_rst == "___ccc___");

				yggr_rst = yggr::regular_parse::regex_parse::replace<yggr::string>(
								yggr_rst.get_allocator(),
								text, yggr::regular_parse::regex_parse::make_regex(str_re),
								str_fmt);
				yggr_test_assert(yggr_rst == "___ccc___");

				stl_rst = yggr::regular_parse::regex_parse::replace<std::string>(
								text, yggr::regular_parse::regex_parse::make_regex(str_re), str_fmt);
				yggr_test_assert(stl_rst == "___ccc___");

				stl_rst = yggr::regular_parse::regex_parse::replace<std::string>(
								stl_rst.get_allocator(),
								text, yggr::regular_parse::regex_parse::make_regex(str_re),
								str_fmt);
				yggr_test_assert(stl_rst == "___ccc___");
			}

			{
				yggr_rst = yggr::regular_parse::regex_parse::replace(
								text.begin(), text.end(),
								yggr::regular_parse::regex_parse::make_regex(
									str_re.begin(), str_re.end()),
								str_fmt);
				yggr_test_assert(yggr_rst == "___ccc___");

				yggr_rst = yggr::regular_parse::regex_parse::replace(
								yggr_rst.get_allocator(),
								text.begin(), text.end(),
								yggr::regular_parse::regex_parse::make_regex(
									str_re.begin(), str_re.end()),
								str_fmt);
				yggr_test_assert(yggr_rst == "___ccc___");

				yggr_rst = yggr::regular_parse::regex_parse::replace<yggr::string>(
								text.begin(), text.end(),
								yggr::regular_parse::regex_parse::make_regex(
									str_re.begin(), str_re.end()),
								str_fmt);
				yggr_test_assert(yggr_rst == "___ccc___");

				yggr_rst = yggr::regular_parse::regex_parse::replace<yggr::string>(
								yggr_rst.get_allocator(),
								text.begin(), text.end(),
								yggr::regular_parse::regex_parse::make_regex(
									str_re.begin(), str_re.end()),
								str_fmt);
				yggr_test_assert(yggr_rst == "___ccc___");

				stl_rst = yggr::regular_parse::regex_parse::replace<std::string>(
								text.begin(), text.end(),
								yggr::regular_parse::regex_parse::make_regex(
									str_re.begin(), str_re.end()),
								str_fmt);
				yggr_test_assert(stl_rst == "___ccc___");

				stl_rst = yggr::regular_parse::regex_parse::replace<std::string>(
								stl_rst.get_allocator(),
								text.begin(), text.end(),
								yggr::regular_parse::regex_parse::make_regex(
									str_re.begin(), str_re.end()),
								str_fmt);
				yggr_test_assert(stl_rst == "___ccc___");
			}

			{
				yggr_rst = yggr::regular_parse::regex_parse::replace(
								text.c_str(),
								yggr::regular_parse::regex_parse::make_regex(
									str_re.c_str()),
								str_fmt);
				yggr_test_assert(yggr_rst == "___ccc___");

				yggr_rst = yggr::regular_parse::regex_parse::replace(
								yggr_rst.get_allocator(),
								text.c_str(),
								yggr::regular_parse::regex_parse::make_regex(
									str_re.c_str()),
								str_fmt);
				yggr_test_assert(yggr_rst == "___ccc___");

				yggr_rst = yggr::regular_parse::regex_parse::replace<yggr::string>(
								text.c_str(),
								yggr::regular_parse::regex_parse::make_regex(
									str_re.c_str()),
								str_fmt);
				yggr_test_assert(yggr_rst == "___ccc___");

				yggr_rst = yggr::regular_parse::regex_parse::replace<yggr::string>(
								yggr_rst.get_allocator(),
								text.c_str(),
								yggr::regular_parse::regex_parse::make_regex(
									str_re.c_str()),
								str_fmt);
				yggr_test_assert(yggr_rst == "___ccc___");

				stl_rst = yggr::regular_parse::regex_parse::replace<std::string>(
								text.c_str(),
								yggr::regular_parse::regex_parse::make_regex(
									str_re.c_str()),
								str_fmt);
				yggr_test_assert(stl_rst == "___ccc___");

				stl_rst = yggr::regular_parse::regex_parse::replace<std::string>(
								stl_rst.get_allocator(),
								text.c_str(),
								yggr::regular_parse::regex_parse::make_regex(
									str_re.c_str()),
								str_fmt);
				yggr_test_assert(stl_rst == "___ccc___");
			}

			{
				yggr_rst = yggr::regular_parse::regex_parse::replace(
								"aaacccbbb",
								yggr::regular_parse::regex_parse::make_regex("(a)|(b)"),
								"(?1_)(?2_)");
				yggr_test_assert(yggr_rst == "___ccc___");

				yggr_rst = yggr::regular_parse::regex_parse::replace(
								yggr_rst.get_allocator(),
								"aaacccbbb",
								yggr::regular_parse::regex_parse::make_regex("(a)|(b)"),
								"(?1_)(?2_)");
				yggr_test_assert(yggr_rst == "___ccc___");

				yggr_rst = yggr::regular_parse::regex_parse::replace<yggr::string>(
								"aaacccbbb",
								yggr::regular_parse::regex_parse::make_regex("(a)|(b)"),
								"(?1_)(?2_)");
				yggr_test_assert(yggr_rst == "___ccc___");

				yggr_rst = yggr::regular_parse::regex_parse::replace<yggr::string>(
								yggr_rst.get_allocator(),
								"aaacccbbb",
								yggr::regular_parse::regex_parse::make_regex("(a)|(b)"),
								"(?1_)(?2_)");
				yggr_test_assert(yggr_rst == "___ccc___");

				stl_rst = yggr::regular_parse::regex_parse::replace<std::string>(
								"aaacccbbb",
								yggr::regular_parse::regex_parse::make_regex("(a)|(b)"),
								"(?1_)(?2_)");
				yggr_test_assert(stl_rst == "___ccc___");

				stl_rst = yggr::regular_parse::regex_parse::replace<std::string>(
								stl_rst.get_allocator(),
								"aaacccbbb",
								yggr::regular_parse::regex_parse::make_regex("(a)|(b)"),
								"(?1_)(?2_)");
				yggr_test_assert(stl_rst == "___ccc___");
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
		yggr::wstring str_re(L"(你)|(吗)");
		std::wstring str_fmt(L"(?1_)(?2_)");
		yggr::wstring text(L"你你你好好好吗吗吗");

		yggr::wstring yggr_rst;
		std::wstring stl_rst;

		try
		{
			{
				yggr_rst = yggr::regular_parse::regex_parse::replace(
								text, yggr::regular_parse::regex_parse::make_regex(str_re), str_fmt);
				yggr_test_assert(yggr_rst == L"___好好好___");

				yggr_rst = yggr::regular_parse::regex_parse::replace(
								yggr_rst.get_allocator(),
								text, yggr::regular_parse::regex_parse::make_regex(str_re),
								str_fmt);
				yggr_test_assert(yggr_rst == L"___好好好___");

				yggr_rst = yggr::regular_parse::regex_parse::replace<yggr::wstring>(
								text, yggr::regular_parse::regex_parse::make_regex(str_re), str_fmt);
				yggr_test_assert(yggr_rst == L"___好好好___");

				yggr_rst = yggr::regular_parse::regex_parse::replace<yggr::wstring>(
								yggr_rst.get_allocator(),
								text, yggr::regular_parse::regex_parse::make_regex(str_re),
								str_fmt);
				yggr_test_assert(yggr_rst == L"___好好好___");

				stl_rst = yggr::regular_parse::regex_parse::replace<std::wstring>(
								text, yggr::regular_parse::regex_parse::make_regex(str_re), str_fmt);
				yggr_test_assert(stl_rst == L"___好好好___");

				stl_rst = yggr::regular_parse::regex_parse::replace<std::wstring>(
								stl_rst.get_allocator(),
								text, yggr::regular_parse::regex_parse::make_regex(str_re),
								str_fmt);
				yggr_test_assert(stl_rst == L"___好好好___");
			}

			{
				yggr_rst = yggr::regular_parse::regex_parse::replace(
								text.begin(), text.end(),
								yggr::regular_parse::regex_parse::make_regex(
									str_re.begin(), str_re.end()),
								str_fmt);
				yggr_test_assert(yggr_rst == L"___好好好___");

				yggr_rst = yggr::regular_parse::regex_parse::replace(
								yggr_rst.get_allocator(),
								text.begin(), text.end(),
								yggr::regular_parse::regex_parse::make_regex(
									str_re.begin(), str_re.end()),
								str_fmt);
				yggr_test_assert(yggr_rst == L"___好好好___");

				yggr_rst = yggr::regular_parse::regex_parse::replace<yggr::wstring>(
								text.begin(), text.end(),
								yggr::regular_parse::regex_parse::make_regex(
									str_re.begin(), str_re.end()),
								str_fmt);
				yggr_test_assert(yggr_rst == L"___好好好___");

				yggr_rst = yggr::regular_parse::regex_parse::replace<yggr::wstring>(
								yggr_rst.get_allocator(),
								text.begin(), text.end(),
								yggr::regular_parse::regex_parse::make_regex(
									str_re.begin(), str_re.end()),
								str_fmt);
				yggr_test_assert(yggr_rst == L"___好好好___");

				stl_rst = yggr::regular_parse::regex_parse::replace<std::wstring>(
								text.begin(), text.end(),
								yggr::regular_parse::regex_parse::make_regex(
									str_re.begin(), str_re.end()),
								str_fmt);
				yggr_test_assert(stl_rst == L"___好好好___");

				stl_rst = yggr::regular_parse::regex_parse::replace<std::wstring>(
								stl_rst.get_allocator(),
								text.begin(), text.end(),
								yggr::regular_parse::regex_parse::make_regex(
									str_re.begin(), str_re.end()),
								str_fmt);
				yggr_test_assert(stl_rst == L"___好好好___");
			}

			{
				yggr_rst = yggr::regular_parse::regex_parse::replace(
								text.c_str(),
								yggr::regular_parse::regex_parse::make_regex(
									str_re.c_str()),
								str_fmt);
				yggr_test_assert(yggr_rst == L"___好好好___");

				yggr_rst = yggr::regular_parse::regex_parse::replace(
								yggr_rst.get_allocator(),
								text.c_str(),
								yggr::regular_parse::regex_parse::make_regex(
									str_re.c_str()),
								str_fmt);
				yggr_test_assert(yggr_rst == L"___好好好___");

				yggr_rst = yggr::regular_parse::regex_parse::replace<yggr::wstring>(
								text.c_str(),
								yggr::regular_parse::regex_parse::make_regex(
									str_re.c_str()),
								str_fmt);
				yggr_test_assert(yggr_rst == L"___好好好___");

				yggr_rst = yggr::regular_parse::regex_parse::replace<yggr::wstring>(
								yggr_rst.get_allocator(),
								text.c_str(),
								yggr::regular_parse::regex_parse::make_regex(
									str_re.c_str()),
								str_fmt);
				yggr_test_assert(yggr_rst == L"___好好好___");

				stl_rst = yggr::regular_parse::regex_parse::replace<std::wstring>(
								text.c_str(),
								yggr::regular_parse::regex_parse::make_regex(
									str_re.c_str()),
								str_fmt);
				yggr_test_assert(stl_rst == L"___好好好___");

				stl_rst = yggr::regular_parse::regex_parse::replace<std::wstring>(
								stl_rst.get_allocator(),
								text.c_str(),
								yggr::regular_parse::regex_parse::make_regex(
									str_re.c_str()),
								str_fmt);
				yggr_test_assert(stl_rst == L"___好好好___");
			}

			{
				yggr_rst = yggr::regular_parse::regex_parse::replace(
								L"你你你好好好吗吗吗",
								yggr::regular_parse::regex_parse::make_regex(L"(你)|(吗)"),
								L"(?1_)(?2_)");
				yggr_test_assert(yggr_rst == L"___好好好___");

				yggr_rst = yggr::regular_parse::regex_parse::replace(
								yggr_rst.get_allocator(),
								L"你你你好好好吗吗吗",
								yggr::regular_parse::regex_parse::make_regex(L"(你)|(吗)"),
								L"(?1_)(?2_)");
				yggr_test_assert(yggr_rst == L"___好好好___");

				yggr_rst = yggr::regular_parse::regex_parse::replace<yggr::wstring>(
								L"你你你好好好吗吗吗",
								yggr::regular_parse::regex_parse::make_regex(L"(你)|(吗)"),
								L"(?1_)(?2_)");
				yggr_test_assert(yggr_rst == L"___好好好___");

				yggr_rst = yggr::regular_parse::regex_parse::replace<yggr::wstring>(
								yggr_rst.get_allocator(),
								L"你你你好好好吗吗吗",
								yggr::regular_parse::regex_parse::make_regex(L"(你)|(吗)"),
								L"(?1_)(?2_)");
				yggr_test_assert(yggr_rst == L"___好好好___");

				stl_rst = yggr::regular_parse::regex_parse::replace<std::wstring>(
								L"你你你好好好吗吗吗",
								yggr::regular_parse::regex_parse::make_regex(L"(你)|(吗)"),
								L"(?1_)(?2_)");
				yggr_test_assert(stl_rst == L"___好好好___");

				stl_rst = yggr::regular_parse::regex_parse::replace<std::wstring>(
								stl_rst.get_allocator(),
								L"你你你好好好吗吗吗",
								yggr::regular_parse::regex_parse::make_regex(L"(你)|(吗)"),
								L"(?1_)(?2_)");
				yggr_test_assert(stl_rst == L"___好好好___");
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

	std::cout << "-------------test_replace1 end------------" << std::endl;
}

void test_replace2(void)
{

	{
		typedef std::basic_stringstream<char> ss_type;
		typedef std::ostream_iterator<char> out_iter_type;

		yggr::string str_re("(a)|(b)");
		std::string str_fmt("(?1_)(?2_)");
		yggr::string text("aaacccbbb");

		std::string str_tail = "fixed";

		try
		{
			{
				ss_type ss;

				out_iter_type out_iter =
					yggr::regular_parse::regex_parse::replace(
						out_iter_type(ss), text,
						yggr::regular_parse::regex_parse::make_regex(str_re), str_fmt);
				std::copy(str_tail.begin(), str_tail.end(), out_iter);
				yggr_test_assert(ss.str() == "___ccc___fixed");
			}

			{
				ss_type ss;

				out_iter_type out_iter =
					yggr::regular_parse::regex_parse::replace(
						out_iter_type(ss), text.begin(), text.end(),
						yggr::regular_parse::regex_parse::make_regex(str_re), str_fmt);
				std::copy(str_tail.begin(), str_tail.end(), out_iter);
				yggr_test_assert(ss.str() == "___ccc___fixed");
			}

			{
				ss_type ss;

				out_iter_type out_iter =
					yggr::regular_parse::regex_parse::replace(
						out_iter_type(ss), text.c_str(),
						yggr::regular_parse::regex_parse::make_regex(str_re), str_fmt);
				std::copy(str_tail.begin(), str_tail.end(), out_iter);
				yggr_test_assert(ss.str() == "___ccc___fixed");
			}

			{
				ss_type ss;

				out_iter_type out_iter =
					yggr::regular_parse::regex_parse::replace(
						out_iter_type(ss), "aaacccbbb",
						yggr::regular_parse::regex_parse::make_regex(str_re), str_fmt);
				std::copy(str_tail.begin(), str_tail.end(), out_iter);
				yggr_test_assert(ss.str() == "___ccc___fixed");
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

		yggr::wstring str_re(L"(你)|(吗)");
		std::wstring str_fmt(L"(?1_)(?2_)");
		yggr::wstring text(L"你你你好好好吗吗吗");

		std::wstring str_tail = L"处理了";

		try
		{
			{
				ss_type ss;

				out_iter_type out_iter =
					yggr::regular_parse::regex_parse::replace(
						out_iter_type(ss), text,
						yggr::regular_parse::regex_parse::make_regex(str_re), str_fmt);
				std::copy(str_tail.begin(), str_tail.end(), out_iter);
				yggr_test_assert(ss.str() == L"___好好好___处理了");
			}

			{
				ss_type ss;

				out_iter_type out_iter =
					yggr::regular_parse::regex_parse::replace(
						out_iter_type(ss), text.begin(), text.end(),
						yggr::regular_parse::regex_parse::make_regex(str_re), str_fmt);
				std::copy(str_tail.begin(), str_tail.end(), out_iter);
				yggr_test_assert(ss.str() == L"___好好好___处理了");
			}

			{
				ss_type ss;

				out_iter_type out_iter =
					yggr::regular_parse::regex_parse::replace(
						out_iter_type(ss), text.c_str(),
						yggr::regular_parse::regex_parse::make_regex(str_re), str_fmt);
				std::copy(str_tail.begin(), str_tail.end(), out_iter);
				yggr_test_assert(ss.str() == L"___好好好___处理了");
			}

			{
				ss_type ss;

				out_iter_type out_iter =
					yggr::regular_parse::regex_parse::replace(
						out_iter_type(ss), L"你你你好好好吗吗吗",
						yggr::regular_parse::regex_parse::make_regex(str_re), str_fmt);
				std::copy(str_tail.begin(), str_tail.end(), out_iter);
				yggr_test_assert(ss.str() == L"___好好好___处理了");
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

#endif //YGGR_NO_CWCHAR

	std::cout << "-------------test_replace2 end------------" << std::endl;
}

void test_replac_of_name(void)
{
	std::string str_re("(?<a_mark>a)|(?<b_mark>b)");
	std::string str_fmt("(?{a_mark}$+{b_mark})(?{b_mark}$+{a_mark})");
	std::string text("aaacccbbb");

	try
	{
		std::cout <<
			yggr::regular_parse::regex_parse::replace(
				text, yggr::regular_parse::regex_parse::make_regex(str_re), str_fmt) << std::endl;
	}
	catch(yggr::regular_parse::regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}

	std::cout << "-------------test_replac_of_name end------------" << std::endl;
}

int main(int argc, char* argv[])
{
#ifndef YGGR_NO_CWCHAR
	TEST_PP_WIO_IMBUE("chs");
#endif // YGGR_NO_CWCHAR
	test_match();
	test_match2();
	test_match3();
	test_match4();

	test_search();
	test_search2();
	test_search3();
	test_search4();

	test_replace1();
	test_replace2();
	test_replac_of_name();

	wait_any_key(argc, argv);
	return 0;
}
