// string_view_test.cpp

#if !defined(_MSC_VER)
#	error "this file supports MSVC only, if you are using other compilers, please use the same name file in the charset_utf8_files"	
#endif // _MSC_VER

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/utf8_string_view.hpp>

#include <yggr/iterator_ex/iterator.hpp>
#include <yggr/bytes/byte_size.hpp>
#include <yggr/mplex/typename_replaceor.hpp>

#include <cassert>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

void test_construct(void)
{

	std::cout << "--------------test_construct begin-----------------" << std::endl;

	{
		yggr::charset::utf8_string_view nsv;
		std::cout << nsv << std::endl;
		yggr_test_assert(nsv.empty());
	}

	{
		yggr::charset::utf8_string_view nsv("abcde");
		std::cout << nsv << std::endl;
		yggr_test_assert(0 == memcmp("abcde", nsv.data(), nsv.size()));
	}

	{
		yggr::charset::utf8_string_view nsv("abcde", 3);
		std::cout << nsv << std::endl;
		yggr_test_assert(0 == memcmp("abc", nsv.data(), nsv.size()));
	}

	{
		yggr::charset::utf8_string org = "abcdeÄãºÃ";
		yggr::charset::utf8_string_view nsv(org);
		std::cout << nsv << std::endl;
		yggr_test_assert(0 == memcmp(org.data(), nsv.data(), nsv.size()));
	}

	{
		yggr::charset::utf8_string org = "abcdeÄãºÃ";
		yggr::charset::utf8_string_view nsv(org.data(), org.size());
		std::cout << nsv << std::endl;
		yggr_test_assert(0 == memcmp(org.data(), nsv.data(), nsv.size()));
	}

	{
		yggr::charset::utf8_string org = "abcdeÄãºÃ";
		yggr::charset::utf8_string_view nsv(org.data(), 5);
		std::cout << nsv << std::endl;
		yggr_test_assert(0 == memcmp(org.data(), nsv.data(), 5));
	}

	{
		yggr::charset::string org = "abcdc";
		yggr::charset::utf8_string_view nsv(org.begin(), org.end());
		std::cout << nsv << std::endl;
		yggr_test_assert(0 == memcmp(org.data(), nsv.data(), nsv.size()));
	}

	//{
	//	// failed init
	//	yggr::charset::string org = "abcdeÄãºÃ";
	//	yggr::charset::utf8_string_view nsv(org.begin(), org.end());
	//	std::cout << nsv << std::endl;
	//	yggr_test_assert(nsv.empty());
	//}

	{
		yggr::charset::string org = "abcdeÄãºÃ";
		yggr::charset::utf8_string_view nsv(org.begin(), org.begin() + 5);
		std::cout << nsv << std::endl;
		yggr_test_assert(0 == memcmp(org.data(), nsv.data(), 5));
	}

	{
		yggr::charset::utf8_string org = "abcdeÄãºÃ";
		yggr::charset::utf8_string_view nsv(org.begin(), org.end());
		std::cout << nsv << std::endl;
		yggr_test_assert(0 == memcmp(org.data(), nsv.data(), nsv.size()));
	}

	{
		yggr::charset::utf8_string org = "abcdeÄãºÃ";
		yggr::charset::utf8_string_view nsv(org.cbegin(), org.cend());
		std::cout << nsv << std::endl;
		yggr_test_assert(0 == memcmp(org.data(), nsv.data(), nsv.size()));
	}

	{
		yggr::charset::utf8_string org = "abcdeÄãºÃ";
		yggr::charset::utf8_string_view nsv(org.ncbegin(), org.ncend());
		std::cout << nsv << std::endl;
		yggr_test_assert(0 == memcmp(org.data(), nsv.data(), nsv.size()));
	}

	{
		yggr::charset::string_view org("abcde");
		yggr::charset::utf8_string_view nsv(org);
		std::cout << nsv << std::endl;
		yggr_test_assert(0 == memcmp(org.data(), nsv.data(), nsv.size()));
	}

	//{
	//	// failed init
	//	yggr::charset::string_view org("abcdeÄãºÃ");
	//	yggr::charset::utf8_string_view nsv(org);
	//	std::cout << nsv << std::endl;
	//	yggr_test_assert(nsv.empty());
	//}

	{
		yggr::charset::string_view org("abcdeÄãºÃ");
		yggr::charset::utf8_string_view nsv(org, 5);
		std::cout << nsv << std::endl;
		yggr_test_assert(0 == memcmp(org.data(), nsv.data(), 5));
	}

	{
		yggr::charset::string_view org("abcdeÄãºÃ");
		yggr::charset::utf8_string_view nsv(org, 5);
		std::cout << nsv << std::endl;
		yggr_test_assert(0 == memcmp(org.data(), nsv.data(), 5));
	}


	{
		yggr::charset::utf8_string org = "abcdeÄãºÃ";
		yggr::charset::utf8_string_view nsv(yggr::charset::utf8_string_view(org.ncbegin(), org.ncend()));
		std::cout << nsv << std::endl;
		yggr_test_assert(0 == memcmp(org.data(), nsv.data(), nsv.size()));
	}

	{
		yggr::charset::utf8_string org_0 = "abcdeÄãºÃ";
		yggr::charset::utf8_string_view org(org_0.ncbegin(), org_0.ncend());
		yggr::charset::utf8_string_view nsv(org);
		std::cout << nsv << std::endl;
		yggr_test_assert(0 == memcmp(org.data(), nsv.data(), nsv.size()));
	}

	std::cout << "--------------test_construct end-----------------" << std::endl;
}

void test_set_and_swap(void)
{
	std::cout << "--------------test_set_and_swap begin-----------------" << std::endl;

	// operator=
	{
		yggr::charset::utf8_string org("abcdeÄãºÃ");
		yggr::charset::utf8_string_view nsv;
		nsv = org;
		std::cout << nsv << std::endl;
		yggr_test_assert(org.data() == nsv.data());
		yggr_test_assert(0 == memcmp(org.data(), nsv.data(), nsv.size()));
	}

	{
		yggr::charset::string_view org("abcde", 3);
		yggr::charset::utf8_string_view nsv;
		nsv = org;
		std::cout << nsv << std::endl;
		yggr_test_assert(0 == memcmp(org.data(), nsv.data(), nsv.size()));
	}

	{
		yggr::charset::utf8_string org0("abcdeÄãºÃ");
		yggr::charset::utf8_string_view org(org0);
		yggr::charset::utf8_string_view nsv;
		nsv = boost::move(org);
		std::cout << nsv << std::endl;
		yggr_test_assert(org.data() == nsv.data());
		yggr_test_assert(0 == memcmp(org.data(), nsv.data(), nsv.size()));
	}

	{
		yggr::charset::utf8_string org0("abcdeÄãºÃ");
		yggr::charset::utf8_string_view org(org0);
		yggr::charset::utf8_string_view nsv;
		nsv = org;
		std::cout << nsv << std::endl;
		yggr_test_assert(org.data() == nsv.data());
		yggr_test_assert(0 == memcmp(org.data(), nsv.data(), nsv.size()));
	}

	// swap
	{
		yggr::charset::utf8_string org0("abcdeÄãºÃ0");
		yggr::charset::utf8_string org1("abcdeÄãºÃ1");
		yggr::charset::utf8_string_view nsv0(org0);
		yggr::charset::utf8_string_view nsv1(org1);
		nsv0.swap(boost::move(nsv1));
		std::cout << nsv0 << std::endl;
		std::cout << nsv1 << std::endl;
		yggr_test_assert(0 == memcmp(org0.data(), nsv1.data(), nsv1.size()));
		yggr_test_assert(0 == memcmp(org1.data(), nsv0.data(), nsv0.size()));
	}

	{
		yggr::charset::utf8_string org0("abcdeÄãºÃ0");
		yggr::charset::utf8_string org1("abcdeÄãºÃ1");
		yggr::charset::utf8_string_view nsv0(org0);
		yggr::charset::utf8_string_view nsv1(org1);
		::yggr::swap(nsv0, boost::move(nsv1));
		std::cout << nsv0 << std::endl;
		std::cout << nsv1 << std::endl;
		yggr_test_assert(0 == memcmp(org0.data(), nsv1.data(), nsv1.size()));
		yggr_test_assert(0 == memcmp(org1.data(), nsv0.data(), nsv0.size()));
	}

	{
		yggr::charset::utf8_string org0("abcdeÄãºÃ0");
		yggr::charset::utf8_string org1("abcdeÄãºÃ1");
		yggr::charset::utf8_string_view nsv0(org0);
		yggr::charset::utf8_string_view nsv1(org1);
		nsv0.swap(nsv1);
		std::cout << nsv0 << std::endl;
		std::cout << nsv1 << std::endl;
		yggr_test_assert(0 == memcmp(org0.data(), nsv1.data(), nsv1.size()));
		yggr_test_assert(0 == memcmp(org1.data(), nsv0.data(), nsv0.size()));
	}

	{
		yggr::charset::utf8_string org0("abcdeÄãºÃ0");
		yggr::charset::utf8_string org1("abcdeÄãºÃ1");
		yggr::charset::utf8_string_view nsv0(org0);
		yggr::charset::utf8_string_view nsv1(org1);
		::yggr::swap(nsv0, nsv1);
		std::cout << nsv0 << std::endl;
		std::cout << nsv1 << std::endl;
		yggr_test_assert(0 == memcmp(org0.data(), nsv1.data(), nsv1.size()));
		yggr_test_assert(0 == memcmp(org1.data(), nsv0.data(), nsv0.size()));
	}

	std::cout << "--------------test_set_and_swap end-----------------" << std::endl;
}

void test_iterators(void)
{
	std::cout << "--------------test_iterators begin-----------------" << std::endl;
	
	typedef yggr::charset::utf8_string_view::reverse_iterator riter_type;
	typedef yggr::charset::utf8_string_view::const_reverse_iterator criter_type;

	typedef yggr::charset::utf8_string_view::iterator iter_type;
	typedef yggr::charset::utf8_string_view::const_iterator citer_type;

	typedef iter_type::org_type org_iter_type;
	typedef citer_type::org_type org_citer_type;

	typedef yggr::mplex::typename_replaceor<riter_type, org_iter_type>::type org_riter_type;
	typedef yggr::mplex::typename_replaceor<criter_type, org_citer_type>::type org_criter_type;
	
	{
		yggr::charset::utf8_string org("abcde");
		yggr::charset::utf8_string_view nsv(org);

		std::cout << nsv << std::endl;
		
		yggr_test_assert(org.length() == nsv.length());
		yggr_test_assert(org.size() == nsv.size());
		yggr_test_assert(nsv.size() == nsv.length());

		yggr_test_assert(*boost::begin(nsv) == 'a');
		yggr_test_assert(*yggr::advance_copy(boost::begin(nsv), 1) == 'b');
		yggr_test_assert(*nsv.begin() == 'a');
		yggr_test_assert(*yggr::advance_copy(nsv.begin(), 1) == 'b');
		yggr_test_assert(std::distance(nsv.begin(), nsv.end()) == nsv.length());
		yggr_test_assert(std::distance(nsv.begin().org(), nsv.end().org()) == nsv.size());
		yggr_test_assert(yggr::byte_size(nsv.begin(), nsv.end()) == nsv.size());

		yggr_test_assert(*nsv.cbegin() == 'a');
		yggr_test_assert(*yggr::advance_copy(nsv.cbegin(), 1) == 'b');
		yggr_test_assert(std::distance(nsv.cbegin(), nsv.cend()) == nsv.length());
		yggr_test_assert(std::distance(nsv.cbegin().org(), nsv.cend().org()) == nsv.size());
		yggr_test_assert(yggr::byte_size(nsv.cbegin(), nsv.cend()) == nsv.size());

		yggr_test_assert(*nsv.rbegin() == 'e');
		yggr_test_assert(*yggr::advance_copy(nsv.rbegin(), 1) == 'd');
		yggr_test_assert(std::distance(nsv.rbegin(), nsv.rend()) == nsv.length());
		yggr_test_assert(std::distance(
				org_riter_type(nsv.rbegin().base().org()), 
				org_riter_type(nsv.rend().base().org())) == nsv.size());
		yggr_test_assert(yggr::byte_size(nsv.rbegin(), nsv.rend()) == nsv.size());

		yggr_test_assert(*nsv.crbegin() == 'e');
		yggr_test_assert(*yggr::advance_copy(nsv.crbegin(), 1) == 'd');
		yggr_test_assert(std::distance(nsv.crbegin(), nsv.crend()) == nsv.length());
		yggr_test_assert(std::distance(
				org_criter_type(nsv.crbegin().base().org()), 
				org_criter_type(nsv.crend().base().org())) == nsv.size());
		yggr_test_assert(yggr::byte_size(nsv.crbegin(), nsv.crend()) == nsv.size());
	}

	{
		yggr::charset::utf8_string org("abcdeÄãºÃ");
		yggr::charset::utf8_string_view nsv(org);

		std::cout << nsv << std::endl;
		
		yggr_test_assert(org.length() == nsv.length());
		yggr_test_assert(org.size() == nsv.size());
		yggr_test_assert(nsv.size() != nsv.length());

		yggr_test_assert(*boost::begin(nsv) == 'a');
		yggr_test_assert(*yggr::advance_copy(boost::begin(nsv), 1) == 'b');
		yggr_test_assert(*nsv.begin() == 'a');
		yggr_test_assert(*yggr::advance_copy(nsv.begin(), 1) == 'b');
		yggr_test_assert(std::distance(nsv.begin(), nsv.end()) == nsv.length());
		yggr_test_assert(std::distance(nsv.begin().org(), nsv.end().org()) == nsv.size());
		yggr_test_assert(yggr::byte_size(nsv.begin(), nsv.end()) == nsv.size());

		yggr_test_assert(*nsv.cbegin() == 'a');
		yggr_test_assert(*yggr::advance_copy(nsv.begin(), 1) == 'b');
		yggr_test_assert(std::distance(nsv.cbegin(), nsv.cend()) == nsv.length());
		yggr_test_assert(std::distance(nsv.cbegin().org(), nsv.cend().org()) == nsv.size());
		yggr_test_assert(yggr::byte_size(nsv.cbegin(), nsv.cend()) == nsv.size());

		yggr_test_assert(*nsv.rbegin() == yggr::charset::utf8_char("ºÃ"));
		yggr_test_assert(*yggr::advance_copy(nsv.rbegin(), 1) == yggr::charset::utf8_char("Äã"));
		yggr_test_assert(std::distance(nsv.rbegin(), nsv.rend()) == nsv.length());
		yggr_test_assert(std::distance(
				org_riter_type(nsv.rbegin().base().org()), 
				org_riter_type(nsv.rend().base().org())) == nsv.size());
		yggr_test_assert(yggr::byte_size(nsv.rbegin(), nsv.rend()) == nsv.size());

		yggr_test_assert(*nsv.crbegin() == yggr::charset::utf8_char("ºÃ"));
		yggr_test_assert(std::distance(nsv.crbegin(), nsv.crend()) == nsv.length());
		yggr_test_assert(std::distance(
				org_criter_type(nsv.crbegin().base().org()), 
				org_criter_type(nsv.crend().base().org())) == nsv.size());
		yggr_test_assert(yggr::byte_size(nsv.crbegin(), nsv.crend()) == nsv.size());
	}

	std::cout << "--------------test_iterators end-----------------" << std::endl;
}

void test_element_access(void)
{
	std::cout << "--------------test_element_access begin-----------------" << std::endl;

	{
		yggr::charset::utf8_string_view nsv("abcde");
		std::cout << nsv << std::endl;

		yggr_test_assert(nsv[0] == 'a');
		yggr_test_assert(nsv[1] == 'b');

		yggr_test_assert(nsv.at(0) == 'a');
		yggr_test_assert(nsv.at(1) == 'b');

		try
		{
			nsv.at(nsv.size());
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

		yggr_test_assert(nsv[0] == nsv.front());
		yggr_test_assert(*nsv.rbegin() == nsv.back());

		yggr_test_assert(*nsv.data() == nsv.front());
	}

	{
		yggr::charset::utf8_string org("abcdeÄãºÃ");
		yggr::charset::utf8_string_view nsv(org);
		std::cout << nsv << std::endl;

		yggr_test_assert(nsv[0] == 'a');
		yggr_test_assert(nsv[1] == 'b');

		//yggr_test_assert(nsv[nsv.length() - 1] == 'ºÃ'); // don't using 'Äã' != "Äã"
		yggr_test_assert(nsv[nsv.length() - 1] == "ºÃ");
		yggr_test_assert(nsv[nsv.length() - 2] == "Äã");

		yggr_test_assert(nsv.at(0) == 'a');
		yggr_test_assert(nsv.at(1) == 'b');

		yggr_test_assert(nsv.at(nsv.length() - 1) == "ºÃ");
		yggr_test_assert(nsv.at(nsv.length() - 2) == "Äã");

		try
		{
			nsv.at(nsv.size());
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

		yggr_test_assert(nsv[0] == nsv.front());
		yggr_test_assert(*nsv.rbegin() == nsv.back());

		yggr_test_assert(*nsv.data() == nsv.front());
	}

	std::cout << "--------------test_element_access end-----------------" << std::endl;
}

void test_capacity(void)
{
	std::cout << "--------------test_capacity begin-----------------" << std::endl;

	{
		yggr::charset::utf8_string org("abcde");
		yggr::charset::utf8_string_view nsv(org);
		std::cout << nsv << std::endl;

		yggr_test_assert(nsv.size() == 5);
		yggr_test_assert(nsv.length() == 5);

		//yggr::charset::string_view::size_type a = nsv.max_size();
		//yggr::charset::string_view::size_type b = yggr::mplex::numeric_limits<yggr::charset::string_view::size_type>::max_type::value;
		//yggr::charset::string_view::size_type c = yggr::charset::string_view::npos;
		yggr_test_assert(nsv.max_size() ==
				(std::min)(
					yggr::mplex::numeric_limits<yggr::charset::utf8_string_view::size_type>::max_type::value / sizeof(char),
					static_cast<yggr::charset::string_view::size_type>(
						yggr::mplex::numeric_limits<yggr::charset::utf8_string_view::difference_type>::max_type::value)));

		yggr_test_assert(!nsv.empty());
		
	}

	{
		yggr::charset::utf8_string org("abcdeÄãºÃ");
		yggr::charset::utf8_string_view nsv(org);
		std::cout << nsv << std::endl;

		yggr_test_assert(nsv.size() == 11);
		yggr_test_assert(nsv.length() == 7);

		//yggr::charset::string_view::size_type a = nsv.max_size();
		//yggr::charset::string_view::size_type b = yggr::mplex::numeric_limits<yggr::charset::string_view::size_type>::max_type::value;
		//yggr::charset::string_view::size_type c = yggr::charset::string_view::npos;
		yggr_test_assert(nsv.max_size() ==
				(std::min)(
					yggr::mplex::numeric_limits<yggr::charset::utf8_string_view::size_type>::max_type::value / sizeof(char),
					static_cast<yggr::charset::string_view::size_type>(
						yggr::mplex::numeric_limits<yggr::charset::utf8_string_view::difference_type>::max_type::value)));

		yggr_test_assert(!nsv.empty());
		
	}

	std::cout << "--------------test_capacity end-----------------" << std::endl;
}

void test_operations_copy(void)
{
	std::cout << "--------------test_operations_copy begin-----------------" << std::endl;

	{
		char str[1024] = {0};
		yggr::charset::utf8_string_view::size_type ret = 0;
		yggr::charset::utf8_string org("abcde");
		yggr::charset::utf8_string_view nsv(org);
		std::cout << nsv << std::endl;

		ret = nsv.copy(str, nsv.npos);
		std::cout << str << std::endl;
		yggr_test_assert(ret == nsv.size());
		yggr_test_assert(0 == memcmp(str, nsv.data(), nsv.size()));

		ret = nsv.copy(str, nsv.npos, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		std::cout << str << std::endl;
		yggr_test_assert(ret == nsv.size());
		yggr_test_assert(0 == memcmp(str, nsv.data(), nsv.size()));

		typedef yggr::charset::string_charset_helper<char*, const yggr::string&> now_string_charset_helper_type;
		now_string_charset_helper_type h_str = yggr::charset::make_string_charset_helper(str, YGGR_STR_UTF8_STRING_CHARSET_NAME());

		ret = nsv.copy(h_str, nsv.npos);
		std::cout << str << std::endl;
		yggr_test_assert(ret == nsv.size());
		yggr_test_assert(0 == memcmp(str, nsv.data(), nsv.size()));
	}

	{
		char str[1024] = {0};
		yggr::charset::utf8_string_view::size_type ret = 0;
		yggr::charset::utf8_string org("abcde");
		yggr::charset::utf8_string_view nsv(org);
		std::cout << nsv << std::endl;

		ret = nsv.copy(str, -1, 1);
		std::cout << str << std::endl;
		yggr_test_assert(ret == nsv.size() - 1);
		yggr_test_assert(0 == memcmp(str, nsv.data() + 1, nsv.size() - 1));

		typedef yggr::charset::string_charset_helper<char*, const yggr::string&> now_string_charset_helper_type;
		now_string_charset_helper_type h_str = yggr::charset::make_string_charset_helper(str, YGGR_STR_UTF8_STRING_CHARSET_NAME());

		ret = nsv.copy(h_str, -1, 1);
		std::cout << str << std::endl;
		yggr_test_assert(ret == nsv.size() - 1);
		yggr_test_assert(0 == memcmp(str, nsv.data() + 1, nsv.size() - 1));
	}

#ifndef YGGR_NO_CWCHAR

	{
		wchar_t str[1024] = {0};
		yggr::charset::utf8_string_view::size_type ret = 0;
		yggr::charset::utf8_string org( L"ÄãºÃÂð");
		yggr::charset::utf8_string_view nsv(org);
		
		ret = nsv.copy(str, nsv.npos);
		yggr_test_assert(ret == 3);
		str[ret] = 0;
		yggr_test_assert(0 == memcmp(str, L"ÄãºÃÂð", ret * sizeof(wchar_t)));

		typedef yggr::charset::string_charset_helper<wchar_t*, const yggr::string&> now_string_charset_helper_type;
		now_string_charset_helper_type h_str = yggr::charset::make_string_charset_helper(str, yggr::charset::charset_name_t<wchar_t>());

		ret = nsv.copy(h_str, nsv.npos);
		yggr_test_assert(ret == 3);
		str[ret] = 0;
		yggr_test_assert(0 == memcmp(str, L"ÄãºÃÂð", ret * sizeof(wchar_t)));
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "--------------test_operations_copy end-----------------" << std::endl;
}

void test_operations_substr(void)
{
	std::cout << "--------------test_operations_substr begin-----------------" << std::endl;

	{
		yggr::charset::utf8_string org("abcde");
		yggr::charset::utf8_string_view nsv(org);
		std::cout << nsv << std::endl;
		
		yggr_test_assert(nsv.substr() == nsv);
		yggr_test_assert((nsv.substr(0, -1, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == nsv));
		std::cout << nsv.substr() << std::endl;

		yggr_test_assert((nsv.substr<yggr::utf8_string_view>() == nsv));
		yggr_test_assert((nsv.substr<yggr::utf8_string_view>(0, -1, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == nsv));
		std::cout << nsv.substr<yggr::utf8_string_view>() << std::endl;

		yggr_test_assert((nsv.substr<yggr::utf8_string>() == org));
		yggr_test_assert((nsv.substr<yggr::utf8_string>(0, -1, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == org));
		std::cout << nsv.substr<yggr::utf8_string>() << std::endl;

		yggr_test_assert((nsv.substr<yggr::string>() == yggr::string("abcde")));
		yggr_test_assert((nsv.substr<yggr::string>(0, -1, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == yggr::string("abcde")));
		std::cout << nsv.substr<yggr::string>() << std::endl;
	}

	{
		yggr::charset::utf8_string org("abcdeÄãºÃ");
		yggr::charset::utf8_string_view nsv(org);
		std::cout << nsv << std::endl;

		yggr_test_assert(nsv.substr() == nsv);
		yggr_test_assert((nsv.substr(0, -1, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == nsv));
		std::cout << nsv.substr() << std::endl;

		yggr_test_assert((nsv.substr<yggr::utf8_string_view>() == nsv));
		yggr_test_assert((nsv.substr<yggr::utf8_string_view>(0, -1, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == nsv));
		std::cout << nsv.substr<yggr::utf8_string_view>() << std::endl;

		yggr_test_assert((nsv.substr<yggr::utf8_string>() == org));
		yggr_test_assert((nsv.substr<yggr::utf8_string>(0, -1, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == org));
		std::cout << nsv.substr<yggr::utf8_string>() << std::endl;

		yggr_test_assert((nsv.substr<yggr::string>() == yggr::string("abcdeÄãºÃ")));
		yggr_test_assert((nsv.substr<yggr::string>(0, -1, yggr::charset::charset_name_t<yggr::string>()) == yggr::string("abcdeÄãºÃ")));
		std::cout << nsv.substr<yggr::string>() << std::endl;
	}

	{
		yggr::charset::utf8_string org("abcde");
		yggr::charset::utf8_string_view nsv(org);
		std::cout << nsv << std::endl;

		yggr_test_assert(0 == memcmp("bcd", nsv.substr(1, 3).data(), nsv.substr(1, 3).size()));
		yggr_test_assert(0 == memcmp(
						"bcd", 
						nsv.substr(1, 3, YGGR_STR_UTF8_STRING_CHARSET_NAME()).data(), 
						nsv.substr(1, 3, YGGR_STR_UTF8_STRING_CHARSET_NAME()).size()));
		std::cout << nsv.substr(1, 3) << std::endl;

		yggr_test_assert((nsv.substr<yggr::utf8_string_view>(1, 3) == yggr::charset::utf8_string_view("bcd")));
		yggr_test_assert((nsv.substr<yggr::utf8_string_view>(1, 3, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == yggr::charset::utf8_string_view("bcd")));
		std::cout << nsv.substr<yggr::utf8_string_view>(1, 3) << std::endl;

		yggr_test_assert((nsv.substr<yggr::utf8_string>(1, 3) == org.substr(1, 3)));
		yggr_test_assert((nsv.substr<yggr::utf8_string>(1, 3, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == org.substr(1, 3)));
		std::cout << nsv.substr<yggr::utf8_string>(1, 3) << std::endl;

		yggr_test_assert((nsv.substr<yggr::string>(1, 3) == yggr::string("bcd")));
		yggr_test_assert((nsv.substr<yggr::string>(1, 3, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == org.substr(1, 3)));
		std::cout << nsv.substr<yggr::string>(1, 3) << std::endl;
	}

	{
		yggr::charset::utf8_string org("abcdeÄãºÃ");
		yggr::charset::utf8_string_view nsv(org);
		std::cout << nsv << std::endl;

		yggr_test_assert(0 == nsv.substr(1, 5).compare("bcdeÄã"));
		yggr_test_assert(0 == nsv.substr(1, 5, YGGR_STR_UTF8_STRING_CHARSET_NAME()).compare("bcdeÄã"));
		std::cout <<  nsv.substr(1, 5) << std::endl;

		yggr_test_assert((0 == nsv.substr<yggr::utf8_string_view>(1, 5).compare("bcdeÄã")));
		yggr_test_assert((0 == nsv.substr<yggr::utf8_string_view>(1, 5, YGGR_STR_UTF8_STRING_CHARSET_NAME()).compare("bcdeÄã")));
		std::cout << nsv.substr<yggr::utf8_string_view>(1, 5) << std::endl;

		yggr_test_assert((nsv.substr<yggr::utf8_string>(1, 5) == org.substr(1, 5)));
		yggr_test_assert((nsv.substr<yggr::utf8_string>(1, 5, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == org.substr(1, 5)));
		std::cout << nsv.substr<yggr::utf8_string>(1, 5) << std::endl;

		yggr_test_assert((nsv.substr<yggr::string>(1, 5) == yggr::string("bcdeÄã")));
		yggr_test_assert((nsv.substr<yggr::string>(1, 5, yggr::charset::charset_name_t<yggr::string>()) == yggr::string("bcdeÄã")));
		std::cout << nsv.substr<yggr::string>(1, 5) << std::endl;
	}

	{
		yggr::charset::utf8_string org("abcde");
		yggr::charset::utf8_string_view nsv(org);
		std::cout << nsv << std::endl;

		yggr::charset::utf8_string_view subnsv;

		try
		{
			subnsv = nsv.substr(nsv.size(), 1);
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	std::cout << "--------------test_operations_substr end-----------------" << std::endl;
}

void test_operations_compare(void)
{
	std::cout << "--------------test_operations_compare begin-----------------" << std::endl;

	// compare_eq
	{
		yggr::utf8_string org("utf8×Ö·û´®");
		yggr::utf8_string_view nsv(org);
		//nsv = "utf8×Ö·û´®";
		yggr_test_assert(nsv.compare_eq("utf8") == false);
		yggr_test_assert(nsv.compare_eq("utf8", YGGR_STR_UTF8_STRING_CHARSET_NAME()) == false);
		yggr_test_assert(nsv.compare_eq(
				yggr::charset::make_string_charset_helper(
					"utf8", YGGR_STR_UTF8_STRING_CHARSET_NAME())) == false);
		yggr_test_assert(nsv.compare_eq("utf8_string") == false);
		yggr_test_assert(nsv.compare_eq("utf8×Ö·û´®³¤") == false);
		yggr_test_assert(nsv.compare_eq("") == false);
		yggr_test_assert(nsv.compare_eq("utf8×Ö·û´®") == true);
		yggr_test_assert(nsv.compare_eq("utf8×Ö·û´®",  yggr::charset::charset_name_t<char>()) == true);
		yggr_test_assert(nsv.compare_eq(
				yggr::charset::make_string_charset_helper(
					"utf8×Ö·û´®",  yggr::charset::charset_name_t<char>())) == true);

#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(nsv.compare_eq(L"utf8") == false);
		yggr_test_assert(nsv.compare_eq(L"utf8", yggr::charset::charset_name_t<wchar_t>()) == false);
		yggr_test_assert(nsv.compare_eq(
				yggr::charset::make_string_charset_helper(
					L"utf8", yggr::charset::charset_name_t<wchar_t>())) == false);
		yggr_test_assert(nsv.compare_eq(L"utf8_string") == false);
		yggr_test_assert(nsv.compare_eq(L"utf8×Ö·û´®³¤") == false);
		yggr_test_assert(nsv.compare_eq(L"") == false);
		yggr_test_assert(nsv.compare_eq(L"utf8×Ö·û´®") == true);
		yggr_test_assert(nsv.compare_eq(L"utf8×Ö·û´®", yggr::charset::charset_name_t<wchar_t>()) == true);
		yggr_test_assert(nsv.compare_eq(
				yggr::charset::make_string_charset_helper(
					L"utf8×Ö·û´®", yggr::charset::charset_name_t<wchar_t>())) == true);
#else
        yggr_test_assert(nsv.compare_eq("utf8") == false );
		yggr_test_assert(nsv.compare_eq("utf8", yggr::charset::charset_name_t<char>()) == false);
		yggr_test_assert(nsv.compare_eq("utf8_string") == false );
		yggr_test_assert(nsv.compare_eq("utf8×Ö·û´®³¤") == false);
		yggr_test_assert(nsv.compare_eq("") == false);
		yggr_test_assert(nsv.compare_eq("utf8×Ö·û´®") == true);
		yggr_test_assert(nsv.compare_eq("utf8×Ö·û´®", yggr::charset::charset_name_t<char>()) == true);
		yggr_test_assert(nsv.compare_eq(
				yggr::charset::make_string_charset_helper(
					"utf8×Ö·û´®", yggr::charset::charset_name_t<char>())) == true);
#endif // YGGR_NO_CWCHAR

		yggr_test_assert(nsv.compare_eq(0, nsv.length(), "utf8", 0, 1) == false);
		yggr_test_assert(nsv.compare_eq(0, nsv.length(), "utf8", 0, 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == false);
		yggr_test_assert(nsv.compare_eq(0, nsv.length(), "utf8_string", 2, 7) == false);
		yggr_test_assert(nsv.compare_eq(0, nsv.length(), "utf8", 0, 0) == false);

		try
		{
			nsv.compare_eq(nsv.length() + 1, -1, "utf8", 0, 1);
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

		try
		{
			nsv.compare_eq(nsv.length() + 1, -1, "utf8", 0, 1, YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

		try
		{
			nsv.compare_eq(
				nsv.length() + 1, -1, 
				yggr::charset::make_string_charset_helper("utf8", YGGR_STR_UTF8_STRING_CHARSET_NAME()),
				0, 1);
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(nsv.compare_eq(0, nsv.length(), L"utf8", 0, 1) == false);
		yggr_test_assert(nsv.compare_eq(0, nsv.length(), L"utf8", 0, 1, yggr::charset::charset_name_t<char>()) == false);
		yggr_test_assert(nsv.compare_eq(
				0, nsv.length(), 
				yggr::charset::make_string_charset_helper(L"utf8", yggr::charset::charset_name_t<char>()), 
				0, 1) == false);
		yggr_test_assert(nsv.compare_eq(0, nsv.length(), L"utf8_string", 2, 7) == false);
		yggr_test_assert(nsv.compare_eq(0, nsv.length(), L"utf8×Ö·û´®³¤³¤³¤", 0, 8) == false);
		yggr_test_assert(nsv.compare_eq(0, nsv.length(), L"utf8", 0, 0) == false);
		yggr_test_assert(nsv.compare_eq(0, nsv.length(), L"³¤utf8×Ö·û´®³¤³¤", 1, 7) == true);

		try
		{
			nsv.compare_eq(nsv.length() + 1, -1, L"³¤utf8×Ö·û´®³¤³¤", 1, 7);
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

#else
        yggr_test_assert(nsv.compare_eq(0, nsv.length(), "utf8", 0, 1) == false );
		yggr_test_assert(nsv.compare_eq(0, nsv.length(), "utf8", 0, 1, yggr::charset::charset_name_t<char>()) == false );
		yggr_test_assert(nsv.compare_eq(
				0, nsv.length(), 
				yggr::charset::make_string_charset_helper("utf8", yggr::charset::charset_name_t<char>()), 
				0, 1) == false );
		yggr_test_assert(nsv.compare_eq(0, nsv.length(), "utf8_string", 2, 7) == false);
		yggr_test_assert(nsv.compare_eq(0, nsv.length(), "utf8", 0, 0) == false);

		try
		{
			nsv.compare_eq(nsv.length(), -1, "utf8_string", 2, 7);
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

		try
		{
			nsv.compare_eq(nsv.length(), -1, "utf8_string", 2, 7, yggr::charset::charset_name_t<char>());
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

		try
		{
			nsv.compare_eq(
				nsv.length(), -1, 
				yggr::charset::make_string_charset_helper("utf8_string", yggr::charset::charset_name_t<char>()),
				2, 7);
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

#endif // YGGR_NO_CWCHAR

#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(nsv.compare_eq(4, 2, "×Ö·û") == true);
		yggr_test_assert(nsv.compare_eq(4, 2, L"×Ö·û") == true);
		yggr_test_assert(nsv.compare_eq(4, 2, yggr::string_view("×Ö·û")) == true);
		yggr_test_assert(nsv.compare_eq(4, 2, yggr::wstring_view(L"×Ö·û")) == true);

		yggr_test_assert(nsv.compare_eq(4, 2, "×Ö·û", yggr::charset::charset_name_t<char>()) == true);
		yggr_test_assert(nsv.compare_eq(4, 2, L"×Ö·û", yggr::charset::charset_name_t<wchar_t>()) == true);
		yggr_test_assert(nsv.compare_eq(4, 2, yggr::string_view("×Ö·û"), yggr::charset::charset_name_t<yggr::string_view>()) == true);
		yggr_test_assert(nsv.compare_eq(4, 2, yggr::wstring_view(L"×Ö·û"), yggr::charset::charset_name_t<yggr::wstring_view>()) == true);

		yggr_test_assert(nsv.compare_eq(
				4, 2, 
				yggr::charset::make_string_charset_helper(
					"×Ö·û", yggr::charset::charset_name_t<char>())) == true);
		yggr_test_assert(nsv.compare_eq(
				4, 2, 
				yggr::charset::make_string_charset_helper(
					L"×Ö·û", yggr::charset::charset_name_t<wchar_t>())) == true);
		yggr_test_assert(nsv.compare_eq(
				4, 2,
				yggr::charset::make_string_charset_helper(
					yggr::string_view("×Ö·û"), yggr::charset::charset_name_t<yggr::string_view>())) == true);
		yggr_test_assert(nsv.compare_eq(
				4, 2, 
				yggr::charset::make_string_charset_helper(
					yggr::wstring_view(L"×Ö·û"), yggr::charset::charset_name_t<yggr::wstring_view>())) == true);

		yggr_test_assert(nsv.compare_eq(4, 2, "×Ö·û³¤") == false);
		yggr_test_assert(nsv.compare_eq(4, 2, L"×Ö·û³¤") == false);
		yggr_test_assert(nsv.compare_eq(4, 2, yggr::string_view("×Ö·û³¤")) == false);
		yggr_test_assert(nsv.compare_eq(4, 2, yggr::wstring_view(L"×Ö·û³¤")) == false);

		yggr_test_assert(nsv.compare_eq(4, 2, "×Ö·û³¤", yggr::charset::charset_name_t<char>()) == false);
		yggr_test_assert(nsv.compare_eq(4, 2, L"×Ö·û³¤", yggr::charset::charset_name_t<wchar_t>()) == false);
		yggr_test_assert(nsv.compare_eq(4, 2, yggr::string_view("×Ö·û³¤"), yggr::charset::charset_name_t<char>()) == false);
		yggr_test_assert(nsv.compare_eq(4, 2, yggr::wstring_view(L"×Ö·û³¤"), yggr::charset::charset_name_t<wchar_t>()) == false);

		yggr_test_assert(nsv.compare_eq(
				4, 2, 
				yggr::charset::make_string_charset_helper(
					"×Ö·û³¤", yggr::charset::charset_name_t<char>())) == false);
		yggr_test_assert(nsv.compare_eq(
				4, 2, 
				yggr::charset::make_string_charset_helper(
					L"×Ö·û³¤", yggr::charset::charset_name_t<wchar_t>())) == false);
		yggr_test_assert(nsv.compare_eq(
				4, 2, 
				yggr::charset::make_string_charset_helper(
					yggr::string_view("×Ö·û³¤"), yggr::charset::charset_name_t<char>())) == false);
		yggr_test_assert(nsv.compare_eq(
				4, 2, 
				yggr::charset::make_string_charset_helper(
					yggr::wstring_view(L"×Ö·û³¤"), yggr::charset::charset_name_t<wchar_t>())) == false);


		yggr_test_assert(nsv.compare_eq(4, 3, "×Ö·û") == false);
		yggr_test_assert(nsv.compare_eq(4, 3, L"×Ö·û") == false);
		yggr_test_assert(nsv.compare_eq(4, 3, yggr::string_view("×Ö·û")) == false);
		yggr_test_assert(nsv.compare_eq(4, 3, yggr::wstring_view(L"×Ö·û")) == false);

		yggr_test_assert(nsv.compare_eq(4, 2, yggr::utf8_string_view(yggr::utf8_string("×Ö·û"))) == true);
		yggr_test_assert(nsv.compare_eq(4, 2, yggr::utf8_string_view(yggr::utf8_string(L"×Ö·û"))) == true);

		yggr_test_assert(nsv.compare_eq(4, 2, yggr::utf8_string_view(yggr::utf8_string("×Ö·û³¤"))) == false);
		yggr_test_assert(nsv.compare_eq(4, 2, yggr::wstring_view(L"×Ö·û³¤")) == false);

		yggr_test_assert(nsv.compare_eq(4, 3, yggr::utf8_string_view(yggr::utf8_string("×Ö·û"))) == false);
		yggr_test_assert(nsv.compare_eq(4, 3, yggr::utf8_string_view(yggr::utf8_string(L"×Ö·û"))) == false);

		try
		{
			nsv.compare_eq(nsv.length(), 3, yggr::utf8_string_view(yggr::utf8_string(L"×Ö·û")));
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

#else
        yggr_test_assert(nsv.compare_eq(4, 2, "×Ö·û") == true);
		yggr_test_assert(nsv.compare_eq(4, 2, yggr::string_view("×Ö·û")) == true);

		yggr_test_assert(nsv.compare_eq(4, 2, "×Ö·û", yggr::charset::charset_name_t<char>()) == true);
		yggr_test_assert(nsv.compare_eq(4, 2, yggr::string_view("×Ö·û"), yggr::charset::charset_name_t<wchar_t>()) == true);

		yggr_test_assert(nsv.compare_eq(
				4, 2, 
				yggr::charset::make_string_charset_helper(
					"×Ö·û", yggr::charset::charset_name_t<char>())) == true);
		yggr_test_assert(nsv.compare_eq(
				4, 2,
				yggr::charset::make_string_charset_helper(
					yggr::string_view("×Ö·û"), yggr::charset::charset_name_t<wchar_t>())) == true);

		yggr_test_assert(nsv.compare_eq(4, 2, "×Ö·û³¤") == false);
		yggr_test_assert(nsv.compare_eq(4, 2, yggr::string_view("×Ö·û³¤")) == false);

		yggr_test_assert(nsv.compare_eq(4, 3, "×Ö·û") == false);
		yggr_test_assert(nsv.compare_eq(4, 3, yggr::string_view("×Ö·û")) == false);

		yggr_test_assert(nsv.compare_eq(4, 2, yggr::utf8_string_view(yggr::utf8_string("×Ö·û"))) == true);

		yggr_test_assert(nsv.compare_eq(4, 2, yggr::utf8_string_view(yggr::utf8_string("×Ö·û³¤"))) == false);

		yggr_test_assert(nsv.compare_eq(4, 3, yggr::utf8_string_view(yggr::utf8_string("×Ö·û"))) == false);

		try
		{
			nsv.compare_eq(nsv.length(), 2, "×Ö·û");
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

#endif // YGGR_NO_CWCHAR
	}

	// compare
	{
		yggr::utf8_string org("utf8×Ö·û´®");
		yggr::utf8_string_view nsv(org);

		//nsv = "utf8×Ö·û´®";
		// const char*
		yggr_test_assert(nsv.compare("utf8") > 0);
		yggr_test_assert(nsv.compare("utf8_string") > 0);
		yggr_test_assert(nsv.compare("utf8×Ö·û´®³¤") < 0);
		yggr_test_assert(nsv.compare("") > 0);
		yggr_test_assert(nsv.compare("utf8×Ö·û´®") == 0);

		yggr_test_assert(nsv.compare("utf8", YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);
		yggr_test_assert(nsv.compare("utf8_string", YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);
		yggr_test_assert(nsv.compare("utf8×Ö·û´®³¤", yggr::charset::charset_name_t<char>()) < 0);
		yggr_test_assert(nsv.compare("", YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);
		yggr_test_assert(nsv.compare("utf8×Ö·û´®", yggr::charset::charset_name_t<char>()) == 0);

		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper("utf8", YGGR_STR_UTF8_STRING_CHARSET_NAME())) > 0);
		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper("utf8_string", YGGR_STR_UTF8_STRING_CHARSET_NAME())) > 0);
		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper("utf8×Ö·û´®³¤", yggr::charset::charset_name_t<char>())) < 0);
		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper("", YGGR_STR_UTF8_STRING_CHARSET_NAME())) > 0);
		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper("utf8×Ö·û´®", yggr::charset::charset_name_t<char>())) == 0);

		// std::string
		yggr_test_assert(nsv.compare(std::string("utf8")) > 0);
		yggr_test_assert(nsv.compare(std::string("utf8_string")) > 0);
		yggr_test_assert(nsv.compare(std::string("utf8×Ö·û´®³¤")) < 0);
		yggr_test_assert(nsv.compare(std::string("")) > 0);
		yggr_test_assert(nsv.compare(std::string("utf8×Ö·û´®")) == 0);

		yggr_test_assert(nsv.compare(std::string("utf8"), YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);
		yggr_test_assert(nsv.compare(std::string("utf8_string"), YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);
		yggr_test_assert(nsv.compare(std::string("utf8×Ö·û´®³¤"), yggr::charset::charset_name_t<char>()) < 0);
		yggr_test_assert(nsv.compare(std::string(""), YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);
		yggr_test_assert(nsv.compare(std::string("utf8×Ö·û´®"), yggr::charset::charset_name_t<char>()) == 0);

		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(std::string("utf8"), YGGR_STR_UTF8_STRING_CHARSET_NAME())) > 0);
		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(std::string("utf8_string"), YGGR_STR_UTF8_STRING_CHARSET_NAME())) > 0);
		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(std::string("utf8×Ö·û´®³¤"), yggr::charset::charset_name_t<char>())) < 0);
		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(std::string(""), YGGR_STR_UTF8_STRING_CHARSET_NAME())) > 0);
		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(std::string("utf8×Ö·û´®"), yggr::charset::charset_name_t<char>())) == 0);

		// boost::container::string
		yggr_test_assert(nsv.compare(boost::container::string("utf8")) > 0);
		yggr_test_assert(nsv.compare(boost::container::string("utf8_string")) > 0);
		yggr_test_assert(nsv.compare(boost::container::string("utf8×Ö·û´®³¤")) < 0);
		yggr_test_assert(nsv.compare(boost::container::string("")) > 0);
		yggr_test_assert(nsv.compare(boost::container::string("utf8×Ö·û´®")) == 0);

		yggr_test_assert(nsv.compare(boost::container::string("utf8"), YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);
		yggr_test_assert(nsv.compare(boost::container::string("utf8_string"), YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);
		yggr_test_assert(nsv.compare(boost::container::string("utf8×Ö·û´®³¤"), yggr::charset::charset_name_t<char>()) < 0);
		yggr_test_assert(nsv.compare(boost::container::string(""), YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);
		yggr_test_assert(nsv.compare(boost::container::string("utf8×Ö·û´®"), yggr::charset::charset_name_t<char>()) == 0);

		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(boost::container::string("utf8"), YGGR_STR_UTF8_STRING_CHARSET_NAME())) > 0);
		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(boost::container::string("utf8_string"), YGGR_STR_UTF8_STRING_CHARSET_NAME())) > 0);
		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(boost::container::string("utf8×Ö·û´®³¤"), yggr::charset::charset_name_t<char>())) < 0);
		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(boost::container::string(""), YGGR_STR_UTF8_STRING_CHARSET_NAME())) > 0);
		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(boost::container::string("utf8×Ö·û´®"), yggr::charset::charset_name_t<char>())) == 0);

		// yggr::string
		yggr_test_assert(nsv.compare(yggr::string("utf8")) > 0);
		yggr_test_assert(nsv.compare(yggr::string("utf8_string")) > 0);
		yggr_test_assert(nsv.compare(yggr::string("utf8×Ö·û´®³¤")) < 0);
		yggr_test_assert(nsv.compare(yggr::string("")) > 0);
		yggr_test_assert(nsv.compare(yggr::string("utf8×Ö·û´®")) == 0);

		yggr_test_assert(nsv.compare(yggr::string("utf8"), YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);
		yggr_test_assert(nsv.compare(yggr::string("utf8_string"), YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);
		yggr_test_assert(nsv.compare(yggr::string("utf8×Ö·û´®³¤"), yggr::charset::charset_name_t<char>()) < 0);
		yggr_test_assert(nsv.compare(yggr::string(""), YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);
		yggr_test_assert(nsv.compare(yggr::string("utf8×Ö·û´®"), yggr::charset::charset_name_t<char>()) == 0);

		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(yggr::string("utf8"), YGGR_STR_UTF8_STRING_CHARSET_NAME())) > 0);
		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(yggr::string("utf8_string"), YGGR_STR_UTF8_STRING_CHARSET_NAME())) > 0);
		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(yggr::string("utf8×Ö·û´®³¤"), yggr::charset::charset_name_t<char>())) < 0);
		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(yggr::string(""), YGGR_STR_UTF8_STRING_CHARSET_NAME())) > 0);
		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(yggr::string("utf8×Ö·û´®"), yggr::charset::charset_name_t<char>())) == 0);

		// yggr::string_view
		yggr_test_assert(nsv.compare(yggr::string_view("utf8")) > 0);
		yggr_test_assert(nsv.compare(yggr::string_view("utf8_string")) > 0);
		yggr_test_assert(nsv.compare(yggr::string_view("utf8×Ö·û´®³¤")) < 0);
		yggr_test_assert(nsv.compare(yggr::string_view("")) > 0);
		yggr_test_assert(nsv.compare(yggr::string_view("utf8×Ö·û´®")) == 0);

		yggr_test_assert(nsv.compare(yggr::string_view("utf8"), YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);
		yggr_test_assert(nsv.compare(yggr::string_view("utf8_string"), YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);
		yggr_test_assert(nsv.compare(yggr::string_view("utf8×Ö·û´®³¤"), yggr::charset::charset_name_t<char>()) < 0);
		yggr_test_assert(nsv.compare(yggr::string_view(""), YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);
		yggr_test_assert(nsv.compare(yggr::string_view("utf8×Ö·û´®"), yggr::charset::charset_name_t<char>()) == 0);

		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(yggr::string_view("utf8"), YGGR_STR_UTF8_STRING_CHARSET_NAME())) > 0);
		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(yggr::string_view("utf8_string"), YGGR_STR_UTF8_STRING_CHARSET_NAME())) > 0);
		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(yggr::string_view("utf8×Ö·û´®³¤"), yggr::charset::charset_name_t<char>())) < 0);
		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(yggr::string_view(""), YGGR_STR_UTF8_STRING_CHARSET_NAME())) > 0);
		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(yggr::string_view("utf8×Ö·û´®"), yggr::charset::charset_name_t<char>())) == 0);

		// yggr::utf8_string
		yggr_test_assert(nsv.compare(yggr::utf8_string("utf8")) > 0);
		yggr_test_assert(nsv.compare(yggr::utf8_string("utf8_string")) > 0);
		yggr_test_assert(nsv.compare(yggr::utf8_string("utf8×Ö·û´®³¤")) < 0);
		yggr_test_assert(nsv.compare(yggr::utf8_string("")) > 0);
		yggr_test_assert(nsv.compare(yggr::utf8_string("utf8×Ö·û´®")) == 0);

		yggr_test_assert(nsv.compare(yggr::utf8_string("utf8"), YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);
		yggr_test_assert(nsv.compare(yggr::utf8_string("utf8_string"), YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);
		yggr_test_assert(nsv.compare(yggr::utf8_string("utf8×Ö·û´®³¤"), yggr::charset::charset_name_t<yggr::utf8_string>()) < 0);
		yggr_test_assert(nsv.compare(yggr::utf8_string(""), YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);
		yggr_test_assert(nsv.compare(yggr::utf8_string("utf8×Ö·û´®"), yggr::charset::charset_name_t<yggr::utf8_string>()) == 0);

		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(yggr::utf8_string("utf8"), YGGR_STR_UTF8_STRING_CHARSET_NAME())) > 0);
		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(yggr::utf8_string("utf8_string"), YGGR_STR_UTF8_STRING_CHARSET_NAME())) > 0);
		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(yggr::utf8_string("utf8×Ö·û´®³¤"), yggr::charset::charset_name_t<yggr::utf8_string>())) < 0);
		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(yggr::utf8_string(""), YGGR_STR_UTF8_STRING_CHARSET_NAME())) > 0);
		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(yggr::utf8_string("utf8×Ö·û´®"), yggr::charset::charset_name_t<yggr::utf8_string>())) == 0);

#ifndef YGGR_NO_CWCHAR
		// const char*
		yggr_test_assert(nsv.compare(L"utf8") > 0);
		yggr_test_assert(nsv.compare(L"utf8_string") > 0);
		yggr_test_assert(nsv.compare(L"utf8×Ö·û´®³¤") < 0);
		yggr_test_assert(nsv.compare(L"") > 0);
		yggr_test_assert(nsv.compare(L"utf8×Ö·û´®") == 0);

		yggr_test_assert(nsv.compare(L"utf8", yggr::charset::charset_name_t<wchar_t>()) > 0);
		yggr_test_assert(nsv.compare(L"utf8_string", yggr::charset::charset_name_t<wchar_t>()) > 0);
		yggr_test_assert(nsv.compare(L"utf8×Ö·û´®³¤", yggr::charset::charset_name_t<wchar_t>()) < 0);
		yggr_test_assert(nsv.compare(L"", yggr::charset::charset_name_t<wchar_t>()) > 0);
		yggr_test_assert(nsv.compare(L"utf8×Ö·û´®", yggr::charset::charset_name_t<wchar_t>()) == 0);

		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(L"utf8", yggr::charset::charset_name_t<wchar_t>())) > 0);
		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(L"utf8_string", yggr::charset::charset_name_t<wchar_t>())) > 0);
		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(L"utf8×Ö·û´®³¤", yggr::charset::charset_name_t<wchar_t>())) < 0);
		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(L"", yggr::charset::charset_name_t<wchar_t>())) > 0);
		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(L"utf8×Ö·û´®", yggr::charset::charset_name_t<wchar_t>())) == 0);

		// std::string
		yggr_test_assert(nsv.compare(std::wstring(L"utf8")) > 0);
		yggr_test_assert(nsv.compare(std::wstring(L"utf8_string")) > 0);
		yggr_test_assert(nsv.compare(std::wstring(L"utf8×Ö·û´®³¤")) < 0);
		yggr_test_assert(nsv.compare(std::wstring(L"")) > 0);
		yggr_test_assert(nsv.compare(std::wstring(L"utf8×Ö·û´®")) == 0);

		yggr_test_assert(nsv.compare(std::wstring(L"utf8"), yggr::charset::charset_name_t<wchar_t>()) > 0);
		yggr_test_assert(nsv.compare(std::wstring(L"utf8_string"), yggr::charset::charset_name_t<wchar_t>()) > 0);
		yggr_test_assert(nsv.compare(std::wstring(L"utf8×Ö·û´®³¤"), yggr::charset::charset_name_t<wchar_t>()) < 0);
		yggr_test_assert(nsv.compare(std::wstring(L""), yggr::charset::charset_name_t<wchar_t>()) > 0);
		yggr_test_assert(nsv.compare(std::wstring(L"utf8×Ö·û´®"), yggr::charset::charset_name_t<wchar_t>()) == 0);

		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(std::wstring(L"utf8"), yggr::charset::charset_name_t<wchar_t>())) > 0);
		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(std::wstring(L"utf8_string"), yggr::charset::charset_name_t<wchar_t>())) > 0);
		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(std::wstring(L"utf8×Ö·û´®³¤"), yggr::charset::charset_name_t<wchar_t>())) < 0);
		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(std::wstring(L""), yggr::charset::charset_name_t<wchar_t>())) > 0);
		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(std::wstring(L"utf8×Ö·û´®"), yggr::charset::charset_name_t<wchar_t>())) == 0);

		// boost::container::string
		yggr_test_assert(nsv.compare(boost::container::wstring(L"utf8")) > 0);
		yggr_test_assert(nsv.compare(boost::container::wstring(L"utf8_string")) > 0);
		yggr_test_assert(nsv.compare(boost::container::wstring(L"utf8×Ö·û´®³¤")) < 0);
		yggr_test_assert(nsv.compare(boost::container::wstring(L"")) > 0);
		yggr_test_assert(nsv.compare(boost::container::wstring(L"utf8×Ö·û´®")) == 0);

		yggr_test_assert(nsv.compare(boost::container::wstring(L"utf8"), yggr::charset::charset_name_t<wchar_t>()) > 0);
		yggr_test_assert(nsv.compare(boost::container::wstring(L"utf8_string"), yggr::charset::charset_name_t<wchar_t>()) > 0);
		yggr_test_assert(nsv.compare(boost::container::wstring(L"utf8×Ö·û´®³¤"), yggr::charset::charset_name_t<wchar_t>()) < 0);
		yggr_test_assert(nsv.compare(boost::container::wstring(L""), yggr::charset::charset_name_t<wchar_t>()) > 0);
		yggr_test_assert(nsv.compare(boost::container::wstring(L"utf8×Ö·û´®"), yggr::charset::charset_name_t<wchar_t>()) == 0);

		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(boost::container::wstring(L"utf8"), yggr::charset::charset_name_t<wchar_t>())) > 0);
		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(boost::container::wstring(L"utf8_string"), yggr::charset::charset_name_t<wchar_t>())) > 0);
		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(boost::container::wstring(L"utf8×Ö·û´®³¤"), yggr::charset::charset_name_t<wchar_t>())) < 0);
		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(boost::container::wstring(L""), yggr::charset::charset_name_t<wchar_t>())) > 0);
		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(boost::container::wstring(L"utf8×Ö·û´®"), yggr::charset::charset_name_t<wchar_t>())) == 0);

		// yggr::string
		yggr_test_assert(nsv.compare(yggr::wstring(L"utf8")) > 0);
		yggr_test_assert(nsv.compare(yggr::wstring(L"utf8_string")) > 0);
		yggr_test_assert(nsv.compare(yggr::wstring(L"utf8×Ö·û´®³¤")) < 0);
		yggr_test_assert(nsv.compare(yggr::wstring(L"")) > 0);
		yggr_test_assert(nsv.compare(yggr::wstring(L"utf8×Ö·û´®")) == 0);

		yggr_test_assert(nsv.compare(yggr::wstring(L"utf8"), yggr::charset::charset_name_t<wchar_t>()) > 0);
		yggr_test_assert(nsv.compare(yggr::wstring(L"utf8_string"), yggr::charset::charset_name_t<wchar_t>()) > 0);
		yggr_test_assert(nsv.compare(yggr::wstring(L"utf8×Ö·û´®³¤"), yggr::charset::charset_name_t<wchar_t>()) < 0);
		yggr_test_assert(nsv.compare(yggr::wstring(L""), yggr::charset::charset_name_t<wchar_t>()) > 0);
		yggr_test_assert(nsv.compare(yggr::wstring(L"utf8×Ö·û´®"), yggr::charset::charset_name_t<wchar_t>()) == 0);

		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(yggr::wstring(L"utf8"), yggr::charset::charset_name_t<wchar_t>())) > 0);
		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(yggr::wstring(L"utf8_string"), yggr::charset::charset_name_t<wchar_t>())) > 0);
		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(yggr::wstring(L"utf8×Ö·û´®³¤"), yggr::charset::charset_name_t<wchar_t>())) < 0);
		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(yggr::wstring(L""), yggr::charset::charset_name_t<wchar_t>())) > 0);
		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(yggr::wstring(L"utf8×Ö·û´®"), yggr::charset::charset_name_t<wchar_t>())) == 0);

		// yggr::string_view
		yggr_test_assert(nsv.compare(yggr::wstring_view(L"utf8")) > 0);
		yggr_test_assert(nsv.compare(yggr::wstring_view(L"utf8_string")) > 0);
		yggr_test_assert(nsv.compare(yggr::wstring_view(L"utf8×Ö·û´®³¤")) < 0);
		yggr_test_assert(nsv.compare(yggr::wstring_view(L"")) > 0);
		yggr_test_assert(nsv.compare(yggr::wstring_view(L"utf8×Ö·û´®")) == 0);

		yggr_test_assert(nsv.compare(yggr::wstring_view(L"utf8"), yggr::charset::charset_name_t<wchar_t>()) > 0);
		yggr_test_assert(nsv.compare(yggr::wstring_view(L"utf8_string"), yggr::charset::charset_name_t<wchar_t>()) > 0);
		yggr_test_assert(nsv.compare(yggr::wstring_view(L"utf8×Ö·û´®³¤"), yggr::charset::charset_name_t<wchar_t>()) < 0);
		yggr_test_assert(nsv.compare(yggr::wstring_view(L""), yggr::charset::charset_name_t<wchar_t>()) > 0);
		yggr_test_assert(nsv.compare(yggr::wstring_view(L"utf8×Ö·û´®"), yggr::charset::charset_name_t<wchar_t>()) == 0);

		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(yggr::wstring_view(L"utf8"), yggr::charset::charset_name_t<wchar_t>())) > 0);
		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(yggr::wstring_view(L"utf8_string"), yggr::charset::charset_name_t<wchar_t>())) > 0);
		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(yggr::wstring_view(L"utf8×Ö·û´®³¤"), yggr::charset::charset_name_t<wchar_t>())) < 0);
		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(yggr::wstring_view(L""), yggr::charset::charset_name_t<wchar_t>())) > 0);
		yggr_test_assert(nsv.compare(yggr::charset::make_string_charset_helper(yggr::wstring_view(L"utf8×Ö·û´®"), yggr::charset::charset_name_t<wchar_t>())) == 0);

#endif // YGGR_NO_CWCHAR

		// const char*
		yggr_test_assert(nsv.compare(0, nsv.length(), "utf8", 0, 1) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), "utf8_string", 2, 7) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), "utf8", 0, 0) > 0);

		yggr_test_assert(nsv.compare(0, nsv.length(), "utf8", 0, 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), "utf8_string", 2, 7, YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), "utf8", 0, 0, YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);

		yggr_test_assert(nsv.compare(
				0, nsv.length(), 
				yggr::charset::make_string_charset_helper("utf8", YGGR_STR_UTF8_STRING_CHARSET_NAME()), 
				0, 1) > 0);
		yggr_test_assert(nsv.compare(
				0, nsv.length(), 
				yggr::charset::make_string_charset_helper("utf8_string", YGGR_STR_UTF8_STRING_CHARSET_NAME()), 
				2, 7) > 0);
		yggr_test_assert(nsv.compare(
				0, nsv.length(),
				yggr::charset::make_string_charset_helper("utf8", YGGR_STR_UTF8_STRING_CHARSET_NAME()),
				0, 0) > 0);

		try
		{
			nsv.compare(nsv.length() + 1, -1, "utf8", 0, 0);
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

		try
		{
			nsv.compare(nsv.length() + 1, -1, "utf8", 0, 0, YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

		try
		{
			nsv.compare(
				nsv.length() + 1, -1, 
				yggr::charset::make_string_charset_helper("utf8", YGGR_STR_UTF8_STRING_CHARSET_NAME()), 
				0, 0);
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

		// std::string
		yggr_test_assert(nsv.compare(0, nsv.length(), std::string("utf8"), 0, 1) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), std::string("utf8_string"), 2, 7) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), std::string("utf8"), 0, 0) > 0);

		yggr_test_assert(nsv.compare(0, nsv.length(), std::string("utf8"), 0, 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), std::string("utf8_string"), 2, 7, YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), std::string("utf8"), 0, 0, YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);

		yggr_test_assert(nsv.compare(
				0, nsv.length(), 
				yggr::charset::make_string_charset_helper(std::string("utf8"), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 
				0, 1) > 0);
		yggr_test_assert(nsv.compare(
				0, nsv.length(), 
				yggr::charset::make_string_charset_helper(std::string("utf8_string"), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 
				2, 7) > 0);
		yggr_test_assert(nsv.compare(
				0, nsv.length(),
				yggr::charset::make_string_charset_helper(std::string("utf8"), YGGR_STR_UTF8_STRING_CHARSET_NAME()),
				0, 0) > 0);

		try
		{
			nsv.compare(nsv.length() + 1, -1, std::string("utf8"), 0, 0);
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

		try
		{
			nsv.compare(nsv.length() + 1, -1, std::string("utf8"), 0, 0, YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

		try
		{
			nsv.compare(
				nsv.length() + 1, -1, 
				yggr::charset::make_string_charset_helper(std::string("utf8"), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 
				0, 0);
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

		// boost::container::string
		yggr_test_assert(nsv.compare(0, nsv.length(), boost::container::string("utf8"), 0, 1) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), boost::container::string("utf8_string"), 2, 7) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), boost::container::string("utf8"), 0, 0) > 0);

		yggr_test_assert(nsv.compare(0, nsv.length(), boost::container::string("utf8"), 0, 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), boost::container::string("utf8_string"), 2, 7, YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), boost::container::string("utf8"), 0, 0, YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);

		yggr_test_assert(nsv.compare(
				0, nsv.length(), 
				yggr::charset::make_string_charset_helper(boost::container::string("utf8"), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 
				0, 1) > 0);
		yggr_test_assert(nsv.compare(
				0, nsv.length(), 
				yggr::charset::make_string_charset_helper(boost::container::string("utf8_string"), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 
				2, 7) > 0);
		yggr_test_assert(nsv.compare(
				0, nsv.length(),
				yggr::charset::make_string_charset_helper(boost::container::string("utf8"), YGGR_STR_UTF8_STRING_CHARSET_NAME()),
				0, 0) > 0);

		try
		{
			nsv.compare(nsv.length() + 1, -1, boost::container::string("utf8"), 0, 0);
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

		try
		{
			nsv.compare(nsv.length() + 1, -1, boost::container::string("utf8"), 0, 0, YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

		try
		{
			nsv.compare(
				nsv.length() + 1, -1, 
				yggr::charset::make_string_charset_helper(boost::container::string("utf8"), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 
				0, 0);
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

		// yggr::string
		yggr_test_assert(nsv.compare(0, nsv.length(), yggr::string("utf8"), 0, 1) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), yggr::string("utf8_string"), 2, 7) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), yggr::string("utf8"), 0, 0) > 0);

		yggr_test_assert(nsv.compare(0, nsv.length(), yggr::string("utf8"), 0, 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), yggr::string("utf8_string"), 2, 7, YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), yggr::string("utf8"), 0, 0, YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);

		yggr_test_assert(nsv.compare(
				0, nsv.length(), 
				yggr::charset::make_string_charset_helper(yggr::string("utf8"), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 
				0, 1) > 0);
		yggr_test_assert(nsv.compare(
				0, nsv.length(), 
				yggr::charset::make_string_charset_helper(yggr::string("utf8_string"), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 
				2, 7) > 0);
		yggr_test_assert(nsv.compare(
				0, nsv.length(),
				yggr::charset::make_string_charset_helper(yggr::string("utf8"), YGGR_STR_UTF8_STRING_CHARSET_NAME()),
				0, 0) > 0);

		try
		{
			nsv.compare(nsv.length() + 1, -1, yggr::string("utf8"), 0, 0);
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

		try
		{
			nsv.compare(nsv.length() + 1, -1, yggr::string("utf8"), 0, 0, YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

		try
		{
			nsv.compare(
				nsv.length() + 1, -1, 
				yggr::charset::make_string_charset_helper(yggr::string("utf8"), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 
				0, 0);
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

		// yggr::string_view
		yggr_test_assert(nsv.compare(0, nsv.length(), yggr::string_view("utf8"), 0, 1) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), yggr::string_view("utf8_string"), 2, 7) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), yggr::string_view("utf8"), 0, 0) > 0);

		yggr_test_assert(nsv.compare(0, nsv.length(), yggr::string_view("utf8"), 0, 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), yggr::string_view("utf8_string"), 2, 7, YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), yggr::string_view("utf8"), 0, 0, YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);

		yggr_test_assert(nsv.compare(
				0, nsv.length(), 
				yggr::charset::make_string_charset_helper(yggr::string_view("utf8"), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 
				0, 1) > 0);
		yggr_test_assert(nsv.compare(
				0, nsv.length(), 
				yggr::charset::make_string_charset_helper(yggr::string_view("utf8_string"), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 
				2, 7) > 0);
		yggr_test_assert(nsv.compare(
				0, nsv.length(),
				yggr::charset::make_string_charset_helper(yggr::string_view("utf8"), YGGR_STR_UTF8_STRING_CHARSET_NAME()),
				0, 0) > 0);

		try
		{
			nsv.compare(nsv.length() + 1, -1, yggr::string_view("utf8"), 0, 0);
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

		try
		{
			nsv.compare(nsv.length() + 1, -1, yggr::string_view("utf8"), 0, 0, YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

		try
		{
			nsv.compare(
				nsv.length() + 1, -1, 
				yggr::charset::make_string_charset_helper(yggr::string_view("utf8"), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 
				0, 0);
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

		// yggr::utf8_string
		yggr_test_assert(nsv.compare(0, nsv.length(), yggr::utf8_string("utf8"), 0, 1) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), yggr::utf8_string("utf8_string"), 2, 7) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), yggr::utf8_string("utf8"), 0, 0) > 0);

		yggr_test_assert(nsv.compare(0, nsv.length(), yggr::utf8_string("utf8"), 0, 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), yggr::utf8_string("utf8_string"), 2, 7, YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), yggr::utf8_string("utf8"), 0, 0, YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);

		yggr_test_assert(nsv.compare(
				0, nsv.length(), 
				yggr::charset::make_string_charset_helper(yggr::utf8_string("utf8"), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 
				0, 1) > 0);
		yggr_test_assert(nsv.compare(
				0, nsv.length(), 
				yggr::charset::make_string_charset_helper(yggr::utf8_string("utf8_string"), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 
				2, 7) > 0);
		yggr_test_assert(nsv.compare(
				0, nsv.length(),
				yggr::charset::make_string_charset_helper(yggr::utf8_string("utf8"), YGGR_STR_UTF8_STRING_CHARSET_NAME()),
				0, 0) > 0);

		try
		{
			nsv.compare(nsv.length() + 1, -1, yggr::utf8_string("utf8"), 0, 0);
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

		try
		{
			nsv.compare(nsv.length() + 1, -1, yggr::utf8_string("utf8"), 0, 0, YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

		try
		{
			nsv.compare(
				nsv.length() + 1, -1, 
				yggr::charset::make_string_charset_helper(yggr::utf8_string("utf8"), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 
				0, 0);
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

#ifndef YGGR_NO_CWCHAR
		// const char*
		yggr_test_assert(nsv.compare(0, nsv.length(), L"utf8", 0, 1) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), L"utf8_string", 2, 7) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), L"utf8×Ö·û´®³¤³¤³¤", 0, 8) < 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), L"utf8", 0, 0) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), L"³¤utf8×Ö·û´®³¤³¤", 1, 7) == 0);

		yggr_test_assert(nsv.compare(0, nsv.length(), L"utf8", 0, 1, yggr::charset::charset_name_t<wchar_t>()) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), L"utf8_string", 2, 7, yggr::charset::charset_name_t<wchar_t>()) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), L"utf8×Ö·û´®³¤³¤³¤", 0, 8, yggr::charset::charset_name_t<wchar_t>()) < 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), L"utf8", 0, 0, yggr::charset::charset_name_t<wchar_t>()) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), L"³¤utf8×Ö·û´®³¤³¤", 1, 7, yggr::charset::charset_name_t<wchar_t>()) == 0);

		yggr_test_assert(nsv.compare(
				0, nsv.length(), 
				yggr::charset::make_string_charset_helper(L"utf8", yggr::charset::charset_name_t<wchar_t>()), 
				0, 1) > 0);
		yggr_test_assert(nsv.compare(
				0, nsv.length(), 
				yggr::charset::make_string_charset_helper(L"utf8_string", yggr::charset::charset_name_t<wchar_t>()), 
				2, 7) > 0);
		yggr_test_assert(nsv.compare(
				0, nsv.length(), 
				yggr::charset::make_string_charset_helper(L"utf8×Ö·û´®³¤³¤³¤", yggr::charset::charset_name_t<wchar_t>()), 
				0, 8) < 0);
		yggr_test_assert(nsv.compare(
				0, nsv.length(), 
				yggr::charset::make_string_charset_helper(L"utf8", yggr::charset::charset_name_t<wchar_t>()), 
				0, 0) > 0);
		yggr_test_assert(nsv.compare(
				0, nsv.length(),
				yggr::charset::make_string_charset_helper(L"³¤utf8×Ö·û´®³¤³¤", yggr::charset::charset_name_t<wchar_t>()),
				1, 7) == 0);

		try
		{
			nsv.compare(nsv.length() + 1, -1, L"³¤utf8×Ö·û´®³¤³¤", 1, 7);
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

		try
		{
			nsv.compare(nsv.length() + 1, -1, L"³¤utf8×Ö·û´®³¤³¤", 1, 7, yggr::charset::charset_name_t<wchar_t>());
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

		try
		{
			nsv.compare(
				nsv.length() + 1, -1, 
				yggr::charset::make_string_charset_helper(
					L"³¤utf8×Ö·û´®³¤³¤", yggr::charset::charset_name_t<wchar_t>()), 
				1, 7);
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

		// std::string
		yggr_test_assert(nsv.compare(0, nsv.length(), std::wstring(L"utf8"), 0, 1) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), std::wstring(L"utf8_string"), 2, 7) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), std::wstring(L"utf8×Ö·û´®³¤³¤³¤"), 0, 8) < 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), std::wstring(L"utf8"), 0, 0) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), std::wstring(L"³¤utf8×Ö·û´®³¤³¤"), 1, 7) == 0);

		yggr_test_assert(nsv.compare(0, nsv.length(), std::wstring(L"utf8"), 0, 1, yggr::charset::charset_name_t<wchar_t>()) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), std::wstring(L"utf8_string"), 2, 7, yggr::charset::charset_name_t<wchar_t>()) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), std::wstring(L"utf8×Ö·û´®³¤³¤³¤"), 0, 8, yggr::charset::charset_name_t<wchar_t>()) < 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), std::wstring(L"utf8"), 0, 0, yggr::charset::charset_name_t<wchar_t>()) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), std::wstring(L"³¤utf8×Ö·û´®³¤³¤"), 1, 7, yggr::charset::charset_name_t<wchar_t>()) == 0);

		yggr_test_assert(nsv.compare(
				0, nsv.length(), 
				yggr::charset::make_string_charset_helper(std::wstring(L"utf8"), yggr::charset::charset_name_t<wchar_t>()), 
				0, 1) > 0);
		yggr_test_assert(nsv.compare(
				0, nsv.length(), 
				yggr::charset::make_string_charset_helper(std::wstring(L"utf8_string"), yggr::charset::charset_name_t<wchar_t>()), 
				2, 7) > 0);
		yggr_test_assert(nsv.compare(
				0, nsv.length(), 
				yggr::charset::make_string_charset_helper(std::wstring(L"utf8×Ö·û´®³¤³¤³¤"), yggr::charset::charset_name_t<wchar_t>()), 
				0, 8) < 0);
		yggr_test_assert(nsv.compare(
				0, nsv.length(), 
				yggr::charset::make_string_charset_helper(std::wstring(L"utf8"), yggr::charset::charset_name_t<wchar_t>()), 
				0, 0) > 0);
		yggr_test_assert(nsv.compare(
				0, nsv.length(),
				yggr::charset::make_string_charset_helper(std::wstring(L"³¤utf8×Ö·û´®³¤³¤"), yggr::charset::charset_name_t<wchar_t>()),
				1, 7) == 0);

		try
		{
			nsv.compare(nsv.length() + 1, -1, std::wstring(L"³¤utf8×Ö·û´®³¤³¤"), 1, 7);
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

		try
		{
			nsv.compare(nsv.length() + 1, -1, std::wstring(L"³¤utf8×Ö·û´®³¤³¤"), 1, 7, yggr::charset::charset_name_t<wchar_t>());
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

		try
		{
			nsv.compare(
				nsv.length() + 1, -1, 
				yggr::charset::make_string_charset_helper(
					std::wstring(L"³¤utf8×Ö·û´®³¤³¤"), yggr::charset::charset_name_t<wchar_t>()), 
				1, 7);
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}


		// boost::container::string
		yggr_test_assert(nsv.compare(0, nsv.length(), boost::container::wstring(L"utf8"), 0, 1) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), boost::container::wstring(L"utf8_string"), 2, 7) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), boost::container::wstring(L"utf8×Ö·û´®³¤³¤³¤"), 0, 8) < 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), boost::container::wstring(L"utf8"), 0, 0) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), boost::container::wstring(L"³¤utf8×Ö·û´®³¤³¤"), 1, 7) == 0);

		yggr_test_assert(nsv.compare(0, nsv.length(), boost::container::wstring(L"utf8"), 0, 1, yggr::charset::charset_name_t<wchar_t>()) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), boost::container::wstring(L"utf8_string"), 2, 7, yggr::charset::charset_name_t<wchar_t>()) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), boost::container::wstring(L"utf8×Ö·û´®³¤³¤³¤"), 0, 8, yggr::charset::charset_name_t<wchar_t>()) < 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), boost::container::wstring(L"utf8"), 0, 0, yggr::charset::charset_name_t<wchar_t>()) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), boost::container::wstring(L"³¤utf8×Ö·û´®³¤³¤"), 1, 7, yggr::charset::charset_name_t<wchar_t>()) == 0);

		yggr_test_assert(nsv.compare(
				0, nsv.length(), 
				yggr::charset::make_string_charset_helper(boost::container::wstring(L"utf8"), yggr::charset::charset_name_t<wchar_t>()), 
				0, 1) > 0);
		yggr_test_assert(nsv.compare(
				0, nsv.length(), 
				yggr::charset::make_string_charset_helper(boost::container::wstring(L"utf8_string"), yggr::charset::charset_name_t<wchar_t>()), 
				2, 7) > 0);
		yggr_test_assert(nsv.compare(
				0, nsv.length(), 
				yggr::charset::make_string_charset_helper(boost::container::wstring(L"utf8×Ö·û´®³¤³¤³¤"), yggr::charset::charset_name_t<wchar_t>()), 
				0, 8) < 0);
		yggr_test_assert(nsv.compare(
				0, nsv.length(), 
				yggr::charset::make_string_charset_helper(boost::container::wstring(L"utf8"), yggr::charset::charset_name_t<wchar_t>()), 
				0, 0) > 0);
		yggr_test_assert(nsv.compare(
				0, nsv.length(),
				yggr::charset::make_string_charset_helper(boost::container::wstring(L"³¤utf8×Ö·û´®³¤³¤"), yggr::charset::charset_name_t<wchar_t>()),
				1, 7) == 0);

		try
		{
			nsv.compare(nsv.length() + 1, -1, boost::container::wstring(L"³¤utf8×Ö·û´®³¤³¤"), 1, 7);
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

		try
		{
			nsv.compare(nsv.length() + 1, -1, boost::container::wstring(L"³¤utf8×Ö·û´®³¤³¤"), 1, 7, yggr::charset::charset_name_t<wchar_t>());
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

		try
		{
			nsv.compare(
				nsv.length() + 1, -1, 
				yggr::charset::make_string_charset_helper(
					boost::container::wstring(L"³¤utf8×Ö·û´®³¤³¤"), yggr::charset::charset_name_t<wchar_t>()), 
				1, 7);
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

		// yggr::string
		yggr_test_assert(nsv.compare(0, nsv.length(), yggr::wstring(L"utf8"), 0, 1) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), yggr::wstring(L"utf8_string"), 2, 7) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), yggr::wstring(L"utf8×Ö·û´®³¤³¤³¤"), 0, 8) < 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), yggr::wstring(L"utf8"), 0, 0) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), yggr::wstring(L"³¤utf8×Ö·û´®³¤³¤"), 1, 7) == 0);

		yggr_test_assert(nsv.compare(0, nsv.length(), yggr::wstring(L"utf8"), 0, 1, yggr::charset::charset_name_t<wchar_t>()) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), yggr::wstring(L"utf8_string"), 2, 7, yggr::charset::charset_name_t<wchar_t>()) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), yggr::wstring(L"utf8×Ö·û´®³¤³¤³¤"), 0, 8, yggr::charset::charset_name_t<wchar_t>()) < 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), yggr::wstring(L"utf8"), 0, 0, yggr::charset::charset_name_t<wchar_t>()) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), yggr::wstring(L"³¤utf8×Ö·û´®³¤³¤"), 1, 7, yggr::charset::charset_name_t<wchar_t>()) == 0);

		yggr_test_assert(nsv.compare(
				0, nsv.length(), 
				yggr::charset::make_string_charset_helper(yggr::wstring(L"utf8"), yggr::charset::charset_name_t<wchar_t>()), 
				0, 1) > 0);
		yggr_test_assert(nsv.compare(
				0, nsv.length(), 
				yggr::charset::make_string_charset_helper(yggr::wstring(L"utf8_string"), yggr::charset::charset_name_t<wchar_t>()), 
				2, 7) > 0);
		yggr_test_assert(nsv.compare(
				0, nsv.length(), 
				yggr::charset::make_string_charset_helper(yggr::wstring(L"utf8×Ö·û´®³¤³¤³¤"), yggr::charset::charset_name_t<wchar_t>()), 
				0, 8) < 0);
		yggr_test_assert(nsv.compare(
				0, nsv.length(), 
				yggr::charset::make_string_charset_helper(yggr::wstring(L"utf8"), yggr::charset::charset_name_t<wchar_t>()), 
				0, 0) > 0);
		yggr_test_assert(nsv.compare(
				0, nsv.length(),
				yggr::charset::make_string_charset_helper(yggr::wstring(L"³¤utf8×Ö·û´®³¤³¤"), yggr::charset::charset_name_t<wchar_t>()),
				1, 7) == 0);

		try
		{
			nsv.compare(nsv.length() + 1, -1, yggr::wstring(L"³¤utf8×Ö·û´®³¤³¤"), 1, 7);
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

		try
		{
			nsv.compare(nsv.length() + 1, -1, yggr::wstring(L"³¤utf8×Ö·û´®³¤³¤"), 1, 7, yggr::charset::charset_name_t<wchar_t>());
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

		try
		{
			nsv.compare(
				nsv.length() + 1, -1, 
				yggr::charset::make_string_charset_helper(
					yggr::wstring(L"³¤utf8×Ö·û´®³¤³¤"), yggr::charset::charset_name_t<wchar_t>()), 
				1, 7);
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

		// yggr::string_view
		yggr_test_assert(nsv.compare(0, nsv.length(), yggr::wstring_view(L"utf8"), 0, 1) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), yggr::wstring_view(L"utf8_string"), 2, 7) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), yggr::wstring_view(L"utf8×Ö·û´®³¤³¤³¤"), 0, 8) < 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), yggr::wstring_view(L"utf8"), 0, 0) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), yggr::wstring_view(L"³¤utf8×Ö·û´®³¤³¤"), 1, 7) == 0);

		yggr_test_assert(nsv.compare(0, nsv.length(), yggr::wstring_view(L"utf8"), 0, 1, yggr::charset::charset_name_t<wchar_t>()) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), yggr::wstring_view(L"utf8_string"), 2, 7, yggr::charset::charset_name_t<wchar_t>()) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), yggr::wstring_view(L"utf8×Ö·û´®³¤³¤³¤"), 0, 8, yggr::charset::charset_name_t<wchar_t>()) < 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), yggr::wstring_view(L"utf8"), 0, 0, yggr::charset::charset_name_t<wchar_t>()) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), yggr::wstring_view(L"³¤utf8×Ö·û´®³¤³¤"), 1, 7, yggr::charset::charset_name_t<wchar_t>()) == 0);

		yggr_test_assert(nsv.compare(
				0, nsv.length(), 
				yggr::charset::make_string_charset_helper(yggr::wstring_view(L"utf8"), yggr::charset::charset_name_t<wchar_t>()), 
				0, 1) > 0);
		yggr_test_assert(nsv.compare(
				0, nsv.length(), 
				yggr::charset::make_string_charset_helper(yggr::wstring_view(L"utf8_string"), yggr::charset::charset_name_t<wchar_t>()), 
				2, 7) > 0);
		yggr_test_assert(nsv.compare(
				0, nsv.length(), 
				yggr::charset::make_string_charset_helper(yggr::wstring_view(L"utf8×Ö·û´®³¤³¤³¤"), yggr::charset::charset_name_t<wchar_t>()), 
				0, 8) < 0);
		yggr_test_assert(nsv.compare(
				0, nsv.length(), 
				yggr::charset::make_string_charset_helper(yggr::wstring_view(L"utf8"), yggr::charset::charset_name_t<wchar_t>()), 
				0, 0) > 0);
		yggr_test_assert(nsv.compare(
				0, nsv.length(),
				yggr::charset::make_string_charset_helper(yggr::wstring_view(L"³¤utf8×Ö·û´®³¤³¤"), yggr::charset::charset_name_t<wchar_t>()),
				1, 7) == 0);

		try
		{
			nsv.compare(nsv.length() + 1, -1, yggr::wstring_view(L"³¤utf8×Ö·û´®³¤³¤"), 1, 7);
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

		try
		{
			nsv.compare(nsv.length() + 1, -1, yggr::wstring_view(L"³¤utf8×Ö·û´®³¤³¤"), 1, 7, yggr::charset::charset_name_t<wchar_t>());
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

		try
		{
			nsv.compare(
				nsv.length() + 1, -1, 
				yggr::charset::make_string_charset_helper(
					yggr::wstring_view(L"³¤utf8×Ö·û´®³¤³¤"), yggr::charset::charset_name_t<wchar_t>()), 
				1, 7);
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}
#else
        yggr_test_assert(nsv.compare(0, nsv.length(), "utf8", 0, 1) > 0 );
		yggr_test_assert(nsv.compare(0, nsv.length(), "utf8_string", 2, 7) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), "utf8", 0, 0) > 0);

		yggr_test_assert(nsv.compare(0, nsv.length(), "utf8", 0, 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0 );
		yggr_test_assert(nsv.compare(0, nsv.length(), "utf8_string", 2, 7, YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);
		yggr_test_assert(nsv.compare(0, nsv.length(), "utf8", 0, 0, YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);

		yggr_test_assert(nsv.compare(
				0, nsv.length(), 
				yggr::charset::make_string_charset_helper("utf8", YGGR_STR_UTF8_STRING_CHARSET_NAME()), 
				0, 1) > 0 );
		yggr_test_assert(nsv.compare(
				0, nsv.length(),
				yggr::charset::make_string_charset_helper("utf8_string", YGGR_STR_UTF8_STRING_CHARSET_NAME()), 
				2, 7) > 0);
		yggr_test_assert(nsv.compare(
				0, nsv.length(), 
				yggr::charset::make_string_charset_helper("utf8", YGGR_STR_UTF8_STRING_CHARSET_NAME()), 
				0, 0) > 0);

		try
		{
			nsv.compare(nsv.length() + 1, -1, "utf8", 0, 1);
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

		try
		{
			nsv.compare(nsv.length() + 1, -1, "utf8", 0, 1, YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

		try
		{
			nsv.compare(
				nsv.length() + 1, -1,
				yggr::charset::make_string_charset_helper("utf8", YGGR_STR_UTF8_STRING_CHARSET_NAME()), 
				0, 1);
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

#endif // YGGR_NO_CWCHAR

#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(nsv.compare(4, 2, "×Ö·û") == 0);
		yggr_test_assert(nsv.compare(4, 2, L"×Ö·û") == 0);
		yggr_test_assert(nsv.compare(4, 2, yggr::string_view("×Ö·û")) == 0);
		yggr_test_assert(nsv.compare(4, 2, yggr::wstring_view(L"×Ö·û")) == 0);

		yggr_test_assert(nsv.compare(4, 2, "×Ö·û", yggr::charset::charset_name_t<char>()) == 0);
		yggr_test_assert(nsv.compare(4, 2, L"×Ö·û", yggr::charset::charset_name_t<wchar_t>()) == 0);
		yggr_test_assert(nsv.compare(4, 2, yggr::string_view("×Ö·û"), yggr::charset::charset_name_t<yggr::string_view>()) == 0);
		yggr_test_assert(nsv.compare(4, 2, yggr::wstring_view(L"×Ö·û"), yggr::charset::charset_name_t<yggr::wstring_view>()) == 0);

		yggr_test_assert(nsv.compare(
				4, 2,
				yggr::charset::make_string_charset_helper("×Ö·û", yggr::charset::charset_name_t<char>())) == 0);
		yggr_test_assert(nsv.compare(
				4, 2, 
				yggr::charset::make_string_charset_helper(L"×Ö·û", yggr::charset::charset_name_t<wchar_t>())) == 0);
		yggr_test_assert(nsv.compare(
				4, 2,
				yggr::charset::make_string_charset_helper(
					yggr::string_view("×Ö·û"), yggr::charset::charset_name_t<yggr::string_view>())) == 0);
		yggr_test_assert(nsv.compare(
				4, 2, 
				yggr::charset::make_string_charset_helper(
					yggr::wstring_view(L"×Ö·û"), yggr::charset::charset_name_t<yggr::wstring_view>())) == 0);

		yggr_test_assert(nsv.compare(4, 2, "×Ö·û³¤") < 0);
		yggr_test_assert(nsv.compare(4, 2, L"×Ö·û³¤") < 0);
		yggr_test_assert(nsv.compare(4, 2, yggr::string_view("×Ö·û³¤")) < 0);
		yggr_test_assert(nsv.compare(4, 2, yggr::wstring_view(L"×Ö·û³¤")) < 0);

		yggr_test_assert(nsv.compare(4, 3, "×Ö·û") > 0);
		yggr_test_assert(nsv.compare(4, 3, L"×Ö·û") > 0);
		yggr_test_assert(nsv.compare(4, 3, yggr::string_view("×Ö·û")) > 0);
		yggr_test_assert(nsv.compare(4, 3, yggr::wstring_view(L"×Ö·û")) > 0);

		yggr_test_assert(nsv.compare(4, 2, yggr::utf8_string_view(yggr::utf8_string("×Ö·û"))) == 0);
		yggr_test_assert(nsv.compare(4, 2, yggr::utf8_string_view(yggr::utf8_string(L"×Ö·û"))) == 0);

		yggr_test_assert(nsv.compare(4, 2, yggr::utf8_string_view(yggr::utf8_string("×Ö·û³¤"))) < 0);
		yggr_test_assert(nsv.compare(4, 2, yggr::wstring_view(L"×Ö·û³¤")) < 0);

		yggr_test_assert(nsv.compare(4, 3, yggr::utf8_string_view(yggr::utf8_string("×Ö·û"))) > 0);
		yggr_test_assert(nsv.compare(4, 3, yggr::utf8_string_view(yggr::utf8_string(L"×Ö·û"))) > 0);

		try
		{
			nsv.compare(nsv.length(), 3, yggr::utf8_string_view(yggr::utf8_string(L"×Ö·û")));
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

		try
		{
			nsv.compare(
				nsv.length(), 3, 
				yggr::utf8_string_view(yggr::utf8_string(L"×Ö·û")), 
				YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

		try
		{
			nsv.compare(
				nsv.length(), 3, 
				yggr::charset::make_string_charset_helper(
					yggr::utf8_string_view(yggr::utf8_string(L"×Ö·û")), 
					YGGR_STR_UTF8_STRING_CHARSET_NAME()));
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

#else
        yggr_test_assert(nsv.compare(4, 2, "×Ö·û") == 0);
		yggr_test_assert(nsv.compare(4, 2, yggr::string_view("×Ö·û")) == 0);

		yggr_test_assert(nsv.compare(4, 2, "×Ö·û", yggr::charset::charset_name_t<char>()) == 0);
		yggr_test_assert(nsv.compare(4, 2, yggr::string_view("×Ö·û"), yggr::charset::charset_name_t<char>()) == 0);

		yggr_test_assert(nsv.compare(
				4, 2, 
				yggr::charset::make_string_charset_helper(
					"×Ö·û", yggr::charset::charset_name_t<char>())) == 0);
		yggr_test_assert(nsv.compare(
				4, 2, 
				yggr::charset::make_string_charset_helper(
					yggr::string_view("×Ö·û"), yggr::charset::charset_name_t<char>())) == 0);

		yggr_test_assert(nsv.compare(4, 2, "×Ö·û³¤") < 0);
		yggr_test_assert(nsv.compare(4, 2, yggr::string_view("×Ö·û³¤")) < 0);

		yggr_test_assert(nsv.compare(4, 3, "×Ö·û") > 0);
		yggr_test_assert(nsv.compare(4, 3, yggr::string_view("×Ö·û")) > 0);

		yggr_test_assert(nsv.compare(4, 2, yggr::utf8_string_view(yggr::utf8_string("×Ö·û"))) == 0);

		yggr_test_assert(nsv.compare(4, 2, yggr::utf8_string_view(yggr::utf8_string("×Ö·û³¤"))) < 0);

		yggr_test_assert(nsv.compare(4, 3, yggr::utf8_string_view(yggr::utf8_string("×Ö·û"))) > 0);

		try
		{
			nsv.compare(nsv.length(), 3, yggr::utf8_string_view(yggr::utf8_string("×Ö·û")));
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

#endif // YGGR_NO_CWCHAR
	}

	{
		yggr::utf8_string org("abcde");
		yggr::utf8_string_view nsv(org);

		// operator<=>
		// ==
		// const char*
		yggr_test_assert(nsv == "abcde");
		yggr_test_assert("abcde" == nsv);

		yggr_test_assert(nsv == yggr::charset::make_string_charset_helper("abcde"));
		yggr_test_assert(yggr::charset::make_string_charset_helper("abcde") == nsv);

		// std::string
		yggr_test_assert(nsv == std::string("abcde"));
		yggr_test_assert(std::string("abcde") == nsv);

		yggr_test_assert(nsv == yggr::charset::make_string_charset_helper(std::string("abcde")));
		yggr_test_assert(yggr::charset::make_string_charset_helper(std::string("abcde")) == nsv);

		// boost::container::string
		yggr_test_assert(nsv == boost::container::string("abcde"));
		yggr_test_assert(boost::container::string("abcde") == nsv);

		yggr_test_assert(nsv == yggr::charset::make_string_charset_helper(boost::container::string("abcde")));
		yggr_test_assert(yggr::charset::make_string_charset_helper(boost::container::string("abcde")) == nsv);

		// yggr::string
		yggr_test_assert(nsv == yggr::string("abcde"));
		yggr_test_assert(yggr::string("abcde") == nsv);

		yggr_test_assert(nsv == yggr::charset::make_string_charset_helper(yggr::string("abcde")));
		yggr_test_assert(yggr::charset::make_string_charset_helper(yggr::string("abcde")) == nsv);

		// yggr::utf8_string
		yggr_test_assert(nsv == yggr::utf8_string("abcde"));
		yggr_test_assert(yggr::utf8_string("abcde") == nsv);

		yggr_test_assert(nsv == yggr::charset::make_string_charset_helper(yggr::utf8_string("abcde")));
		yggr_test_assert(yggr::charset::make_string_charset_helper(yggr::utf8_string("abcde")) == nsv);

		// yggr::utf8_string_view
		yggr_test_assert(nsv == yggr::utf8_string_view(yggr::utf8_string("abcde")));

		yggr_test_assert(nsv == yggr::charset::make_string_charset_helper(yggr::utf8_string_view(yggr::utf8_string("abcde"))));

		// !=
		// const char*
		yggr_test_assert(nsv != "dabcde");
		yggr_test_assert("dabcde" != nsv);

		yggr_test_assert(nsv != yggr::charset::make_string_charset_helper("dabcde"));
		yggr_test_assert(yggr::charset::make_string_charset_helper("dabcde") != nsv);

		// std::string
		yggr_test_assert(nsv != std::string("dabcde"));
		yggr_test_assert(std::string("dabcde") != nsv);

		yggr_test_assert(nsv != yggr::charset::make_string_charset_helper(std::string("dabcde")));
		yggr_test_assert(yggr::charset::make_string_charset_helper(std::string("dabcde")) != nsv);

		// boost::container::string
		yggr_test_assert(nsv != boost::container::string("dabcde"));
		yggr_test_assert(boost::container::string("dabcde") != nsv);

		yggr_test_assert(nsv != yggr::charset::make_string_charset_helper(boost::container::string("dabcde")));
		yggr_test_assert(yggr::charset::make_string_charset_helper(boost::container::string("dabcde")) != nsv);

		// yggr::string
		yggr_test_assert(nsv != yggr::string("dabcde"));
		yggr_test_assert(yggr::string("dabcde") != nsv);

		yggr_test_assert(nsv != yggr::charset::make_string_charset_helper(yggr::string("dabcde")));
		yggr_test_assert(yggr::charset::make_string_charset_helper(yggr::string("dabcde")) != nsv);

		// yggr::utf8_string
		yggr_test_assert(nsv != yggr::utf8_string("dabcde"));
		yggr_test_assert(yggr::utf8_string("dabcde") != nsv);

		yggr_test_assert(nsv != yggr::charset::make_string_charset_helper(yggr::utf8_string("dabcde")));
		yggr_test_assert(yggr::charset::make_string_charset_helper(yggr::utf8_string("dabcde")) != nsv);

		// yggr::utf8_string_view
		yggr_test_assert(nsv != yggr::utf8_string_view(yggr::utf8_string("dabcde")));

		yggr_test_assert(nsv != yggr::charset::make_string_charset_helper(yggr::utf8_string_view(yggr::utf8_string("dabcde"))));
		
		// <
		// const char*
		yggr_test_assert(nsv < "dabcde");
		yggr_test_assert("abbde" < nsv);

		yggr_test_assert(nsv < yggr::charset::make_string_charset_helper("dabcde"));
		yggr_test_assert(yggr::charset::make_string_charset_helper("abbde") < nsv);

		// std::string
		yggr_test_assert(nsv < std::string("dabcde"));
		yggr_test_assert(std::string("abbde") < nsv);

		yggr_test_assert(nsv < yggr::charset::make_string_charset_helper(std::string("dabcde")));
		yggr_test_assert(yggr::charset::make_string_charset_helper(std::string("abbde")) < nsv);

		// boost::container::string
		yggr_test_assert(nsv < boost::container::string("dabcde"));
		yggr_test_assert(boost::container::string("abbde") < nsv);

		yggr_test_assert(nsv < yggr::charset::make_string_charset_helper(boost::container::string("dabcde")));
		yggr_test_assert(yggr::charset::make_string_charset_helper(boost::container::string("abbde")) < nsv);

		// yggr::string
		yggr_test_assert(nsv < yggr::string("dabcde"));
		yggr_test_assert(yggr::string("abbde") < nsv);

		yggr_test_assert(nsv < yggr::charset::make_string_charset_helper(yggr::string("dabcde")));
		yggr_test_assert(yggr::charset::make_string_charset_helper(yggr::string("abbde")) < nsv);

		// yggr::utf8_string
		yggr_test_assert(nsv < yggr::utf8_string("dabcde"));
		yggr_test_assert(yggr::utf8_string("abbde") < nsv);

		yggr_test_assert(nsv < yggr::charset::make_string_charset_helper(yggr::utf8_string("dabcde")));
		yggr_test_assert(yggr::charset::make_string_charset_helper(yggr::utf8_string("abbde")) < nsv);

		// yggr::utf8_string_view
		yggr_test_assert(nsv < yggr::utf8_string_view(yggr::utf8_string("dabcde")));

		yggr_test_assert(nsv < yggr::charset::make_string_charset_helper(yggr::utf8_string_view(yggr::utf8_string("dabcde"))));

		// <=
		// const char*
		yggr_test_assert(nsv <= "dabcde");
		yggr_test_assert("abcde" <= nsv);

		yggr_test_assert(nsv <= yggr::charset::make_string_charset_helper("dabcde"));
		yggr_test_assert(yggr::charset::make_string_charset_helper("abcde") <= nsv);

		// std::string
		yggr_test_assert(nsv <= std::string("dabcde"));
		yggr_test_assert(std::string("abcde") <= nsv);

		yggr_test_assert(nsv <= yggr::charset::make_string_charset_helper(std::string("dabcde")));
		yggr_test_assert(yggr::charset::make_string_charset_helper(std::string("abcde")) <= nsv);

		// boost::container::string
		yggr_test_assert(nsv <= boost::container::string("dabcde"));
		yggr_test_assert(boost::container::string("abcde") <= nsv);

		yggr_test_assert(nsv <= yggr::charset::make_string_charset_helper(boost::container::string("dabcde")));
		yggr_test_assert(yggr::charset::make_string_charset_helper(boost::container::string("abcde")) <= nsv);

		// yggr::string
		yggr_test_assert(nsv <= yggr::string("dabcde"));
		yggr_test_assert(yggr::string("abcde") <= nsv);

		yggr_test_assert(nsv <= yggr::charset::make_string_charset_helper(yggr::string("dabcde")));
		yggr_test_assert(yggr::charset::make_string_charset_helper(yggr::string("abcde")) <= nsv);

		// yggr::utf8_string
		yggr_test_assert(nsv <= yggr::utf8_string("dabcde"));
		yggr_test_assert(yggr::utf8_string("abcde") <= nsv);

		yggr_test_assert(nsv <= yggr::charset::make_string_charset_helper(yggr::utf8_string("dabcde")));
		yggr_test_assert(yggr::charset::make_string_charset_helper(yggr::utf8_string("abcde")) <= nsv);

		// yggr::utf8_string_view
		yggr_test_assert(nsv <= yggr::utf8_string_view(yggr::utf8_string("dabcde")));

		yggr_test_assert(nsv <= yggr::charset::make_string_charset_helper(yggr::utf8_string_view(yggr::utf8_string("dabcde"))));

		// >
		// const char*
		yggr_test_assert(nsv > "abcd");
		yggr_test_assert("abdde" > nsv);

		yggr_test_assert(nsv > yggr::charset::make_string_charset_helper("abcd"));
		yggr_test_assert(yggr::charset::make_string_charset_helper("abdde") > nsv);

		// std::string
		yggr_test_assert(nsv > std::string("abcd"));
		yggr_test_assert(std::string("abdde") > nsv);

		yggr_test_assert(nsv > yggr::charset::make_string_charset_helper(std::string("abcd")));
		yggr_test_assert(yggr::charset::make_string_charset_helper(std::string("abdde")) > nsv);

		// boost::container::string
		yggr_test_assert(nsv > boost::container::string("abcd"));
		yggr_test_assert(boost::container::string("abdde") > nsv);

		yggr_test_assert(nsv > yggr::charset::make_string_charset_helper(boost::container::string("abcd")));
		yggr_test_assert(yggr::charset::make_string_charset_helper(boost::container::string("abdde")) > nsv);

		// yggr::string
		yggr_test_assert(nsv > yggr::string("abcd"));
		yggr_test_assert(yggr::string("abdde") > nsv);

		yggr_test_assert(nsv > yggr::charset::make_string_charset_helper(yggr::string("abcd")));
		yggr_test_assert(yggr::charset::make_string_charset_helper(yggr::string("abdde")) > nsv);

		// yggr::utf8_string
		yggr_test_assert(nsv > yggr::utf8_string("abcd"));
		yggr_test_assert(yggr::utf8_string("abdde") > nsv);

		yggr_test_assert(nsv > yggr::charset::make_string_charset_helper(yggr::utf8_string("abcd")));
		yggr_test_assert(yggr::charset::make_string_charset_helper(yggr::utf8_string("abdde")) > nsv);

		// yggr::utf8_string_view
		yggr_test_assert(nsv > yggr::utf8_string_view(yggr::utf8_string("abcd")));

		yggr_test_assert(nsv > yggr::charset::make_string_charset_helper(yggr::utf8_string_view(yggr::utf8_string("abcd"))));

		// >=
		// const char*
		yggr_test_assert(nsv >= "abbde");
		yggr_test_assert("abcde" >= nsv);

		yggr_test_assert(nsv >= yggr::charset::make_string_charset_helper("abbde"));
		yggr_test_assert(yggr::charset::make_string_charset_helper("abcde") >= nsv);

		// std::string
		yggr_test_assert(nsv >= std::string("abbde"));
		yggr_test_assert(std::string("abcde") >= nsv);

		yggr_test_assert(nsv >= yggr::charset::make_string_charset_helper(std::string("abbde")));
		yggr_test_assert(yggr::charset::make_string_charset_helper(std::string("abcde")) >= nsv);

		// boost::container::string
		yggr_test_assert(nsv >= boost::container::string("abbde"));
		yggr_test_assert(boost::container::string("abcde") >= nsv);

		yggr_test_assert(nsv >= yggr::charset::make_string_charset_helper(boost::container::string("abbde")));
		yggr_test_assert(yggr::charset::make_string_charset_helper(boost::container::string("abcde")) >= nsv);

		// yggr::string
		yggr_test_assert(nsv >= yggr::string("abbde"));
		yggr_test_assert(yggr::string("abcde") >= nsv);

		yggr_test_assert(nsv >= yggr::charset::make_string_charset_helper(yggr::string("abbde")));
		yggr_test_assert(yggr::charset::make_string_charset_helper(yggr::string("abcde")) >= nsv);

		// yggr::utf8_string
		yggr_test_assert(nsv >= yggr::utf8_string("abbde"));
		yggr_test_assert(yggr::utf8_string("abcde") >= nsv);

		yggr_test_assert(nsv >= yggr::charset::make_string_charset_helper(yggr::utf8_string("abbde")));
		yggr_test_assert(yggr::charset::make_string_charset_helper(yggr::utf8_string("abcde")) >= nsv);

		// yggr::utf8_string_view
		yggr_test_assert(nsv >= yggr::utf8_string_view(yggr::utf8_string("abbde")));

		yggr_test_assert(nsv >= yggr::charset::make_string_charset_helper(yggr::utf8_string_view(yggr::utf8_string("abbde"))));
	}

	{
		yggr::utf8_string org("abcde");
		yggr::utf8_string_view nsv(org);

		// operator<=>
		yggr_test_assert(yggr::utf8_string_view() == yggr::utf8_string_view());
		yggr_test_assert(nsv == nsv.substr());
		yggr_test_assert(nsv == yggr::charset::make_string_charset_helper(nsv.substr(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(yggr::charset::make_string_charset_helper(nsv.substr(), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == nsv);

		yggr_test_assert(nsv != nsv.substr<yggr::utf8_string_view>(1));
		yggr_test_assert(nsv != yggr::charset::make_string_charset_helper(nsv.substr<yggr::utf8_string_view>(1), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(yggr::charset::make_string_charset_helper(nsv.substr<yggr::utf8_string_view>(1), YGGR_STR_UTF8_STRING_CHARSET_NAME()) != nsv);

		yggr_test_assert(nsv < nsv.substr(1));
		yggr_test_assert(nsv < yggr::charset::make_string_charset_helper(nsv.substr(1), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(yggr::charset::make_string_charset_helper(nsv.substr(1), YGGR_STR_UTF8_STRING_CHARSET_NAME()) > nsv);

		yggr_test_assert(nsv <= nsv.substr(1));
		yggr_test_assert(nsv <= yggr::charset::make_string_charset_helper(nsv.substr(1), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(yggr::charset::make_string_charset_helper(nsv.substr(1), YGGR_STR_UTF8_STRING_CHARSET_NAME()) >= nsv);

		yggr_test_assert(nsv <= nsv.substr());
		yggr_test_assert(nsv <= yggr::charset::make_string_charset_helper(nsv.substr(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(yggr::charset::make_string_charset_helper(nsv.substr(), YGGR_STR_UTF8_STRING_CHARSET_NAME()) >= nsv);

		yggr_test_assert(nsv.substr(1) > nsv);
		yggr_test_assert(yggr::charset::make_string_charset_helper(nsv.substr(1), YGGR_STR_UTF8_STRING_CHARSET_NAME()) > nsv);
		yggr_test_assert(nsv < yggr::charset::make_string_charset_helper(nsv.substr(1), YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(nsv.substr(1) >= nsv);
		yggr_test_assert(yggr::charset::make_string_charset_helper(nsv.substr(1), YGGR_STR_UTF8_STRING_CHARSET_NAME()) >= nsv);
		yggr_test_assert(nsv <= yggr::charset::make_string_charset_helper(nsv.substr(1), YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(nsv.substr() >= nsv);
		yggr_test_assert(yggr::charset::make_string_charset_helper(nsv.substr(), YGGR_STR_UTF8_STRING_CHARSET_NAME()) >= nsv);
		yggr_test_assert(nsv <= yggr::charset::make_string_charset_helper(nsv.substr(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
	}

	{
		yggr::utf8_string org("abcde");
		yggr::utf8_string_view nsv(org);

		// operator<=>
		yggr_test_assert(yggr::utf8_string_view().str() == yggr::utf8_string_view().str());
		yggr_test_assert(nsv.str() == nsv.substr());
		yggr_test_assert(nsv.str<yggr::utf8_string_view>() == nsv.substr<yggr::utf8_string_view>());
		yggr_test_assert(nsv.str<yggr::utf8_string>() == nsv.substr<yggr::utf8_string>());
		yggr_test_assert(nsv.str<yggr::string>() == nsv.substr<yggr::string>());
		yggr_test_assert(nsv.str<yggr::string>() != nsv.substr<yggr::string>(1));

		yggr_test_assert(nsv.str() < nsv.substr(1));
		yggr_test_assert(nsv.str<yggr::utf8_string_view>() < nsv.substr<yggr::utf8_string_view>(1));
		yggr_test_assert(nsv.str<yggr::utf8_string>() < nsv.substr<yggr::utf8_string>(1));
		yggr_test_assert(nsv.str<yggr::string>() < nsv.substr<yggr::string>(1));

		yggr_test_assert(nsv.str() <= nsv.substr(1));
		yggr_test_assert(nsv.str<yggr::utf8_string_view>() <= nsv.substr<yggr::utf8_string_view>(1));
		yggr_test_assert(nsv.str<yggr::utf8_string>() <= nsv.substr<yggr::utf8_string>(1));
		yggr_test_assert(nsv.str<yggr::string>() <= nsv.substr<yggr::string>(1));

		yggr_test_assert(nsv.substr(1) > nsv.str());
		yggr_test_assert(nsv.substr<yggr::utf8_string_view>(1) > nsv.str<yggr::utf8_string_view>());
		yggr_test_assert(nsv.substr<yggr::utf8_string>(1) > nsv.str<yggr::utf8_string>());
		yggr_test_assert(nsv.substr<yggr::string>(1) > nsv.str<yggr::string>());

		yggr_test_assert(nsv.substr(1, nsv.size() - 1) >= nsv.str());
		yggr_test_assert(nsv.substr<yggr::utf8_string_view>(1, nsv.size() - 1) >= nsv.str<yggr::utf8_string_view>());
		yggr_test_assert(nsv.substr<yggr::utf8_string>(1, nsv.size() - 1) >= nsv.str<yggr::utf8_string>());
		yggr_test_assert(nsv.substr<yggr::string>(1, nsv.size() - 1) >= nsv.str<yggr::string>());

		yggr_test_assert(nsv.substr() >= nsv);
		yggr_test_assert(nsv.substr<yggr::utf8_string_view>(0, nsv.size()) >= nsv);
	}

	std::cout << "--------------test_operations_compare end-----------------" << std::endl;
}

void test_operations_starts_ends_with(void)
{
	std::cout << "--------------test_operations_starts_ends_with begin-----------------" << std::endl;
	{
		yggr::utf8_string org("abcde");
		yggr::utf8_string_view nsv(org);
		std::cout << nsv << std::endl;

		yggr_test_assert(nsv.starts_with(nsv.substr()));
		yggr_test_assert(!nsv.starts_with(nsv.substr(1)));

		yggr_test_assert(nsv.starts_with(nsv.front()));
		yggr_test_assert(!nsv.starts_with(nsv.back()));

		yggr_test_assert(nsv.starts_with(nsv.substr().data()));
		yggr_test_assert(!nsv.starts_with(nsv.substr(1).data()));
	}

	{
		yggr::utf8_string org("ÄãabcdeºÃ");
		yggr::utf8_string_view nsv(org);
		std::cout << nsv << std::endl;

		yggr_test_assert(nsv.starts_with(nsv.substr()));
		yggr_test_assert(!nsv.starts_with(nsv.substr(1)));

		yggr_test_assert(nsv.starts_with(nsv.front()));
		yggr_test_assert(!nsv.starts_with(nsv.back()));

		yggr_test_assert(nsv.starts_with(nsv.substr().data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(!nsv.starts_with(nsv.substr(1).data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(nsv.starts_with(
				yggr::charset::make_string_charset_helper(
					nsv.substr().data(), YGGR_STR_UTF8_STRING_CHARSET_NAME())));
		yggr_test_assert(!nsv.starts_with(
				yggr::charset::make_string_charset_helper(
					nsv.substr(1).data(), YGGR_STR_UTF8_STRING_CHARSET_NAME())));
	}

	{
		yggr::utf8_string org("abcde");
		yggr::utf8_string_view nsv(org);
		std::cout << nsv << std::endl;

		yggr_test_assert(nsv.ends_with(nsv.substr()));
		yggr_test_assert(!nsv.ends_with(nsv.substr(1, 3)));

		yggr_test_assert(nsv.ends_with(nsv.back()));
		yggr_test_assert(!nsv.ends_with(nsv.front()));

		yggr_test_assert(nsv.ends_with(nsv.substr().data()));
		yggr_test_assert(nsv.ends_with(nsv.substr(1).data()));
		yggr_test_assert(!nsv.ends_with("edcba"));
	}

	{
		yggr::utf8_string org("ÄãabcdeºÃ");
		yggr::utf8_string_view nsv(org);
		std::cout << nsv << std::endl;

		yggr_test_assert(nsv.ends_with(nsv.substr()));
		yggr_test_assert(!nsv.ends_with(nsv.substr(1, 3)));

		yggr_test_assert(nsv.ends_with(nsv.back()));
		yggr_test_assert(!nsv.ends_with(nsv.front()));

		if(yggr::charset::charset_name_t<char>() != yggr::charset::charset_name_t<yggr::utf8_string>())
		{
			try
			{
				nsv.ends_with(nsv.substr().data());
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
		else
		{
			yggr_test_assert(nsv.ends_with(nsv.substr().data()));
		}

		yggr_test_assert(nsv.ends_with(nsv.substr().data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(nsv.ends_with(nsv.substr(1).data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(!nsv.ends_with("edcba"));

		yggr_test_assert(nsv.ends_with(
				yggr::charset::make_string_charset_helper(
					nsv.substr().data(), YGGR_STR_UTF8_STRING_CHARSET_NAME())));
		yggr_test_assert(nsv.ends_with(
				yggr::charset::make_string_charset_helper(
					nsv.substr(1).data(), YGGR_STR_UTF8_STRING_CHARSET_NAME())));
	}

	std::cout << "--------------test_operations_starts_ends_with end-----------------" << std::endl;
}


void test_operations_contains(void)
{
	std::cout << "--------------test_operations_contains begin-----------------" << std::endl;
	{
		yggr::utf8_string org_t("abcde");
		yggr::utf8_string_view nsv_t(org_t);

		yggr::utf8_string org_f("fgh");
		yggr::utf8_string_view nsv_f(org_f);

		std::cout << nsv_t << std::endl;
		std::cout << nsv_f << std::endl;

		yggr_test_assert(nsv_t.contains(nsv_t));
		yggr_test_assert(!nsv_f.contains(nsv_t));
		
		yggr_test_assert(nsv_t.contains(nsv_t.substr(1)));
		yggr_test_assert(!nsv_f.contains(nsv_t.substr(1)));

		yggr_test_assert(nsv_t.contains(nsv_t.front()));
		yggr_test_assert(!nsv_f.contains(nsv_t.front()));


		yggr_test_assert(nsv_t.contains(nsv_t.back()));
		yggr_test_assert(!nsv_f.contains(nsv_t.back()));

		yggr_test_assert(nsv_t.contains(nsv_t.data()));
		yggr_test_assert(!nsv_f.contains(nsv_t.data()));

		yggr_test_assert(nsv_t.contains(nsv_t.data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(!nsv_f.contains(nsv_t.data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
	}

	{
		yggr::utf8_string org_t("ÄãabcdeºÃ");
		yggr::utf8_string_view nsv_t(org_t);

		yggr::utf8_string org_f("×Öfgh·û");
		yggr::utf8_string_view nsv_f(org_f);

		std::cout << nsv_t << std::endl;
		std::cout << nsv_f << std::endl;

		yggr_test_assert(nsv_t.contains(nsv_t));
		yggr_test_assert(!nsv_t.contains(nsv_f));
		
		yggr_test_assert(nsv_t.contains(nsv_t.substr(1)));
		yggr_test_assert(!nsv_t.contains(nsv_f.substr(1)));

		yggr_test_assert(nsv_t.contains(nsv_t.front()));
		yggr_test_assert(!nsv_t.contains(nsv_f.front()));

		yggr_test_assert(nsv_t.contains(nsv_t.back()));
		yggr_test_assert(!nsv_t.contains(nsv_f.back()));

		if(yggr::charset::charset_name_t<char>() != yggr::charset::charset_name_t<yggr::utf8_string>())
		{
			try
			{
				nsv_t.contains(nsv_t.data());
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}

			try
			{
				nsv_t.contains(nsv_f.data());
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
		else
		{
			yggr_test_assert(nsv_t.contains(nsv_t.data()));
			yggr_test_assert(!nsv_t.contains(nsv_f.data()));
		}

		yggr_test_assert(nsv_t.contains(nsv_t.data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(!nsv_t.contains(nsv_f.data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(nsv_t.contains(
				yggr::charset::make_string_charset_helper(
					nsv_t.data(), YGGR_STR_UTF8_STRING_CHARSET_NAME())));
		yggr_test_assert(!nsv_t.contains(
				yggr::charset::make_string_charset_helper(
					nsv_f.data(), YGGR_STR_UTF8_STRING_CHARSET_NAME())));
	}

	std::cout << "--------------test_operations_contains end-----------------" << std::endl;
}

void test_operations_find(void)
{
	std::cout << "--------------test_operations_find begin-----------------" << std::endl;
	{
		yggr::charset::utf8_string org("abcde");
		yggr::charset::utf8_string_view nsv(org);
		std::cout << nsv << std::endl;

		// inline size_type find(const this_type& right, size_type pos = 0) const YGGR_NOEXCEPT_OR_NOTHROW
		yggr_test_assert(0 == nsv.find(nsv.substr()));
		yggr_test_assert(1 == nsv.find(nsv.substr(1)));

		yggr_test_assert(0 == nsv.find(nsv.substr(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(1 == nsv.find(nsv.substr(1), YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(0 == nsv.find(yggr::string_view("abcde")));
		yggr_test_assert(1 == nsv.find(yggr::string_view("bcde")));

		yggr_test_assert(0 == nsv.find(
						yggr::charset::make_string_charset_helper(
							nsv.substr(), YGGR_STR_UTF8_STRING_CHARSET_NAME())));
		yggr_test_assert(1 == nsv.find(
						yggr::charset::make_string_charset_helper(
							nsv.substr(1), YGGR_STR_UTF8_STRING_CHARSET_NAME())));

		yggr_test_assert(yggr::charset::string_view::npos == nsv.find(nsv.substr(), 1));
		yggr_test_assert(1 == nsv.find(nsv.substr(1), 1));

		yggr_test_assert(yggr::charset::string_view::npos == nsv.find(nsv.substr(), 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(1 == nsv.find(nsv.substr(1), 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(yggr::charset::string_view::npos 
				== nsv.find(
					yggr::charset::make_string_charset_helper(
						nsv.substr(), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 1));
		yggr_test_assert(1 == nsv.find(
						yggr::charset::make_string_charset_helper(
							nsv.substr(1), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 1));

		// inline size_type find(value_type c, size_type pos = 0) const YGGR_NOEXCEPT_OR_NOTHROW

		yggr_test_assert(0 == nsv.find('a'));
		yggr_test_assert(nsv.length() - 1 == nsv.find('e'));

		yggr_test_assert(0 == nsv.find('a', YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(nsv.length() - 1 == nsv.find('e', YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(0 == nsv.find(yggr::charset::make_string_charset_helper('a', yggr::charset::charset_name_t<char>())));
		yggr_test_assert(nsv.length() - 1 == nsv.find(yggr::charset::make_string_charset_helper('e', yggr::charset::charset_name_t<char>())));

		yggr_test_assert(0 == nsv.find(nsv.front()));
		yggr_test_assert(nsv.length() - 1 == nsv.find(nsv.back()));

		yggr_test_assert(yggr::charset::utf8_string_view::npos == nsv.find('a', 1));
		yggr_test_assert(nsv.length() - 1 == nsv.find('e', 1));

		yggr_test_assert(yggr::charset::utf8_string_view::npos == nsv.find(nsv.front(), 1));
		yggr_test_assert(nsv.length() - 1 == nsv.find(nsv.back(), 1));


		yggr_test_assert(0 == nsv.find(nsv.front().view()));
		yggr_test_assert(nsv.length() - 1 == nsv.find(nsv.back().view()));

		yggr_test_assert(0 == nsv.find(nsv.front().view(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(nsv.length() - 1 == nsv.find(nsv.back().view(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(0 == nsv.find(
						yggr::charset::make_string_charset_helper(
							nsv.front().view(), YGGR_STR_UTF8_STRING_CHARSET_NAME())));
		yggr_test_assert(nsv.length() - 1 == nsv.find(
									yggr::charset::make_string_charset_helper(
										nsv.back().view(), 
										YGGR_STR_UTF8_STRING_CHARSET_NAME())));

		yggr_test_assert(yggr::charset::utf8_string_view::npos == nsv.find(nsv.front().view(), 1));
		yggr_test_assert(nsv.length() - 1 == nsv.find(nsv.back().view(), 1));

		yggr_test_assert(yggr::charset::utf8_string_view::npos == nsv.find(nsv.front().view(), 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(nsv.length() - 1 == nsv.find(nsv.back().view(), 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(yggr::charset::utf8_string_view::npos == 
				nsv.find(
					yggr::charset::make_string_charset_helper(
						nsv.front().view(), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 1));
		yggr_test_assert(nsv.length() - 1 == 
				nsv.find(
					yggr::charset::make_string_charset_helper(
						nsv.back().view(), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 1));

		yggr_test_assert(0 == nsv.find('a'));
		yggr_test_assert(nsv.length() - 1 == nsv.find('e'));

		yggr_test_assert(0 == nsv.find('a', yggr::charset::charset_name_t<char>()));
		yggr_test_assert(nsv.length() - 1 == nsv.find('e', yggr::charset::charset_name_t<char>()));

		yggr_test_assert(0 == nsv.find(
						yggr::charset::make_string_charset_helper(
							'a', yggr::charset::charset_name_t<char>())));
		yggr_test_assert(nsv.length() - 1 == nsv.find(
									yggr::charset::make_string_charset_helper(
										'e', yggr::charset::charset_name_t<char>())));

#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(0 == nsv.find(L'a', yggr::charset::charset_name_t<wchar_t>()));
		yggr_test_assert(nsv.length() - 1 == nsv.find(L'e', yggr::charset::charset_name_t<wchar_t>()));

		yggr_test_assert(0 == nsv.find(
						yggr::charset::make_string_charset_helper(
							L'a', yggr::charset::charset_name_t<wchar_t>())));
		yggr_test_assert(nsv.length() - 1 == nsv.find(
									yggr::charset::make_string_charset_helper(
										L'e', yggr::charset::charset_name_t<wchar_t>())));

#endif // YGGR_NO_CWCHAR

		yggr_test_assert(yggr::charset::utf8_string_view::npos == nsv.find('a', 1));
		yggr_test_assert(nsv.length() - 1 == nsv.find('e', 1));

		yggr_test_assert(yggr::charset::utf8_string_view::npos == nsv.find('a', 1, yggr::charset::charset_name_t<char>()));
		yggr_test_assert(nsv.length() - 1 == nsv.find('e', 1, yggr::charset::charset_name_t<char>()));

		yggr_test_assert(yggr::charset::utf8_string_view::npos == 
				nsv.find(
					yggr::charset::make_string_charset_helper('a', yggr::charset::charset_name_t<char>()), 1));
		yggr_test_assert(nsv.length() - 1 == 
				nsv.find(
					yggr::charset::make_string_charset_helper('e', yggr::charset::charset_name_t<char>()), 1));

#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(yggr::charset::utf8_string_view::npos == nsv.find(L'a', 1, yggr::charset::charset_name_t<wchar_t>()));
		yggr_test_assert(nsv.length() - 1 == nsv.find(L'e', 1, yggr::charset::charset_name_t<wchar_t>()));

		yggr_test_assert(yggr::charset::utf8_string_view::npos == 
				nsv.find(
					yggr::charset::make_string_charset_helper(L'a', yggr::charset::charset_name_t<wchar_t>()), 1));
		yggr_test_assert(nsv.length() - 1 == 
				nsv.find(
					yggr::charset::make_string_charset_helper(L'e', yggr::charset::charset_name_t<wchar_t>()), 1));
#endif // YGGR_NO_CWCHAR

		// inline size_type find(const_pointer s, size_type pos, size_type n) const
		yggr_test_assert(0 == nsv.find(nsv.substr().data(), 0, nsv.substr().size()));
		yggr_test_assert(1 == nsv.find(nsv.substr(1).data(), 0, nsv.substr(1).size()));

		yggr_test_assert(0 == nsv.find(nsv.substr().data(), 0, nsv.substr().size(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(1 == nsv.find(nsv.substr(1).data(), 0, nsv.substr(1).size(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(0 == nsv.find(
						yggr::charset::make_string_charset_helper(
							nsv.substr().data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()),
						0, nsv.substr().size()));
		yggr_test_assert(1 == nsv.find(
						yggr::charset::make_string_charset_helper(
							nsv.substr(1).data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()),
						0, nsv.substr(1).size()));

		yggr_test_assert(yggr::charset::string_view::npos == nsv.find(nsv.substr().data(), 1, nsv.substr().size()));
		yggr_test_assert(1 == nsv.find(nsv.substr(1).data(), 1, nsv.substr(1).size()));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find("z", 0, 1));
		yggr_test_assert(2 == nsv.find("c", 0, 1));
		yggr_test_assert(0 == nsv.find("", 0, 0));

		yggr_test_assert(2 == nsv.find("c", 0, 1, yggr::charset::charset_name_t<char>()));
		yggr_test_assert(0 == nsv.find("", 0, 0, yggr::charset::charset_name_t<char>()));

		yggr_test_assert(2 == nsv.find(yggr::charset::make_string_charset_helper("c", yggr::charset::charset_name_t<char>()), 0, 1));
		yggr_test_assert(0 == nsv.find(yggr::charset::make_string_charset_helper("", yggr::charset::charset_name_t<char>()), 0, 0));

#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(2 == nsv.find(L"c", 0, 1));
		yggr_test_assert(0 == nsv.find(L"", 0, 0));

		yggr_test_assert(2 == nsv.find(L"c", 0, 1, yggr::charset::charset_name_t<wchar_t>()));
		yggr_test_assert(0 == nsv.find(L"", 0, 0, yggr::charset::charset_name_t<wchar_t>()));

		yggr_test_assert(2 == nsv.find(yggr::charset::make_string_charset_helper("c", yggr::charset::charset_name_t<char>()), 0, 1));
		yggr_test_assert(0 == nsv.find(yggr::charset::make_string_charset_helper("", yggr::charset::charset_name_t<char>()), 0, 0));
#endif // YGGR_NO_CWCHAR

		// inline size_type find(const_pointer s, size_type pos = 0) const
		yggr_test_assert(0 == nsv.find(nsv.substr().data(), 0));
		yggr_test_assert(1 == nsv.find(nsv.substr(1).data(), 0));

		yggr_test_assert(0 == nsv.find(nsv.substr().data(), 0, YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(1 == nsv.find(nsv.substr(1).data(), 0, YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(0 == nsv.find(yggr::charset::make_string_charset_helper(nsv.substr().data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 0));
		yggr_test_assert(1 == nsv.find(yggr::charset::make_string_charset_helper(nsv.substr(1).data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 0));

		yggr_test_assert(yggr::charset::string_view::npos == nsv.find(nsv.substr().data(), 1));
		yggr_test_assert(1 == nsv.find(nsv.substr(1).data(), 1));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find("z", 0));
		yggr_test_assert(2 == nsv.find("c", 0));

	}

	{
		yggr::charset::utf8_string org("ÄãabcdeºÃ");
		yggr::charset::utf8_string_view nsv(org);
		std::cout << nsv << std::endl;

		// inline size_type find(const this_type& right, size_type pos = 0) const YGGR_NOEXCEPT_OR_NOTHROW
		yggr_test_assert(0 == nsv.find(nsv.substr()));
		yggr_test_assert(1 == nsv.find(nsv.substr(1)));

		yggr_test_assert(0 == nsv.find(nsv.substr(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(1 == nsv.find(nsv.substr(1), YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(0 == nsv.find(
						yggr::charset::make_string_charset_helper(
							nsv.substr(), YGGR_STR_UTF8_STRING_CHARSET_NAME())));
		yggr_test_assert(1 == nsv.find(
						yggr::charset::make_string_charset_helper(
							nsv.substr(1), YGGR_STR_UTF8_STRING_CHARSET_NAME())));

		yggr_test_assert(yggr::charset::string_view::npos == nsv.find(nsv.substr(), 1));
		yggr_test_assert(1 == nsv.find(nsv.substr(1), 1));

		yggr_test_assert(yggr::charset::string_view::npos == nsv.find(nsv.substr(), 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(1 == nsv.find(nsv.substr(1), 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(yggr::charset::string_view::npos 
				== nsv.find(
					yggr::charset::make_string_charset_helper(
						nsv.substr(), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 1));
		yggr_test_assert(1 == nsv.find(
						yggr::charset::make_string_charset_helper(
							nsv.substr(1), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 1));

		// inline size_type find(value_type c, size_type pos = 0) const YGGR_NOEXCEPT_OR_NOTHROW
		yggr_test_assert(0 == nsv.find(nsv.front()));
		yggr_test_assert(nsv.length() - 1 == nsv.find(nsv.back()));

		yggr_test_assert(yggr::charset::utf8_string_view::npos == nsv.find(nsv.front(), 1));
		yggr_test_assert(nsv.length() - 1 == nsv.find(nsv.back(), 1));

		yggr_test_assert(0 == nsv.find(nsv.front().view()));
		yggr_test_assert(nsv.length() - 1 == nsv.find(nsv.back().view()));

		yggr_test_assert(0 == nsv.find(nsv.front().view(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(nsv.length() - 1 == nsv.find(nsv.back().view(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(0 == nsv.find(
						yggr::charset::make_string_charset_helper(
							nsv.front().view(), YGGR_STR_UTF8_STRING_CHARSET_NAME())));
		yggr_test_assert(nsv.length() - 1 == nsv.find(
									yggr::charset::make_string_charset_helper(
										nsv.back().view(), 
										YGGR_STR_UTF8_STRING_CHARSET_NAME())));

		yggr_test_assert(yggr::charset::utf8_string_view::npos == nsv.find(nsv.front().view(), 1));
		yggr_test_assert(nsv.length() - 1 == nsv.find(nsv.back().view(), 1));

		yggr_test_assert(yggr::charset::utf8_string_view::npos == nsv.find(nsv.front().view(), 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(nsv.length() - 1 == nsv.find(nsv.back().view(), 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(yggr::charset::utf8_string_view::npos == 
				nsv.find(
					yggr::charset::make_string_charset_helper(
						nsv.front().view(), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 1));
		yggr_test_assert(nsv.length() - 1 == 
				nsv.find(
					yggr::charset::make_string_charset_helper(
						nsv.back().view(), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 1));

		yggr_test_assert(1 == nsv.find('a'));
		yggr_test_assert(nsv.length() - 2 == nsv.find('e'));

		yggr_test_assert(1 == nsv.find('a', yggr::charset::charset_name_t<char>()));
		yggr_test_assert(nsv.length() - 2 == nsv.find('e', yggr::charset::charset_name_t<char>()));

		yggr_test_assert(1 == nsv.find(
						yggr::charset::make_string_charset_helper(
							'a', yggr::charset::charset_name_t<char>())));
		yggr_test_assert(nsv.length() - 2 == nsv.find(
									yggr::charset::make_string_charset_helper(
										'e', yggr::charset::charset_name_t<char>())));

#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(1 == nsv.find(L'a', yggr::charset::charset_name_t<wchar_t>()));
		yggr_test_assert(nsv.length() - 2 == nsv.find(L'e', yggr::charset::charset_name_t<wchar_t>()));

		yggr_test_assert(1 == nsv.find(
						yggr::charset::make_string_charset_helper(
							L'a', yggr::charset::charset_name_t<wchar_t>())));
		yggr_test_assert(nsv.length() - 2 == nsv.find(
									yggr::charset::make_string_charset_helper(
										L'e', yggr::charset::charset_name_t<wchar_t>())));

#endif // YGGR_NO_CWCHAR

		yggr_test_assert(yggr::charset::utf8_string_view::npos == nsv.find('a', 2));
		yggr_test_assert(nsv.length() - 2 == nsv.find('e', 1));

		yggr_test_assert(yggr::charset::utf8_string_view::npos == nsv.find('a', 2, yggr::charset::charset_name_t<char>()));
		yggr_test_assert(nsv.length() - 2 == nsv.find('e', 1, yggr::charset::charset_name_t<char>()));

		yggr_test_assert(yggr::charset::utf8_string_view::npos == 
				nsv.find(
					yggr::charset::make_string_charset_helper('a', yggr::charset::charset_name_t<char>()), 2));
		yggr_test_assert(nsv.length() - 2 == 
				nsv.find(
					yggr::charset::make_string_charset_helper('e', yggr::charset::charset_name_t<char>()), 1));

#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(yggr::charset::utf8_string_view::npos == nsv.find(L'a', 2, yggr::charset::charset_name_t<wchar_t>()));
		yggr_test_assert(nsv.length() - 2 == nsv.find(L'e', 1, yggr::charset::charset_name_t<wchar_t>()));

		yggr_test_assert(yggr::charset::utf8_string_view::npos == 
				nsv.find(
					yggr::charset::make_string_charset_helper(L'a', yggr::charset::charset_name_t<wchar_t>()), 2));
		yggr_test_assert(nsv.length() - 2 == 
				nsv.find(
					yggr::charset::make_string_charset_helper(L'e', yggr::charset::charset_name_t<wchar_t>()), 1));
#endif // YGGR_NO_CWCHAR

		// inline size_type find(const_pointer s, size_type pos, size_type n) const

		if(yggr::charset::charset_name_t<char>() != yggr::charset::charset_name_t<yggr::utf8_string>())
		{
			try
			{
				nsv.find(nsv.substr().data(), 0, nsv.substr().size());
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}

			try
			{
				nsv.find(nsv.substr(1).data(), 0, nsv.substr(1).size());
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
		else
		{
			yggr_test_assert(0 == nsv.find(nsv.substr().data(), 0, nsv.substr().size()));
			yggr_test_assert(1 == nsv.find(nsv.substr(1).data(), 0, nsv.substr(1).size()));
		}

		yggr_test_assert(0 == nsv.find(nsv.substr().data(), 0, nsv.substr().size(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(1 == nsv.find(nsv.substr(1).data(), 0, nsv.substr(1).size(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(0 == nsv.find(
						yggr::charset::make_string_charset_helper(
							nsv.substr().data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()),
						0, nsv.substr().size()));
		yggr_test_assert(1 == nsv.find(
						yggr::charset::make_string_charset_helper(
							nsv.substr(1).data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()),
						0, nsv.substr(1).size()));

		yggr_test_assert(0 == nsv.find("Äã", 0, yggr::charset::charset_base_foo::strlen("Äã")));
		yggr_test_assert(0 == nsv.find("", 0, 0));

		yggr_test_assert(0 == nsv.find("Äã", 0, yggr::charset::charset_base_foo::strlen("Äã"), yggr::charset::charset_name_t<char>()));
		yggr_test_assert(0 == nsv.find("", 0, 0, yggr::charset::charset_name_t<char>()));

		yggr_test_assert(0 == nsv.find(
						yggr::charset::make_string_charset_helper(
							"Äã", yggr::charset::charset_name_t<char>()),
						0, yggr::charset::charset_base_foo::strlen("Äã")));
		yggr_test_assert(0 == nsv.find(yggr::charset::make_string_charset_helper("", yggr::charset::charset_name_t<char>()), 0, 0));


#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(0 == nsv.find(L"Äã", 0, yggr::charset::charset_base_foo::strlen(L"Äã")));
		yggr_test_assert(0 == nsv.find(L"", 0, 0));

		yggr_test_assert(0 == nsv.find(L"Äã", 0, yggr::charset::charset_base_foo::strlen(L"Äã"), yggr::charset::charset_name_t<wchar_t>()));
		yggr_test_assert(0 == nsv.find(L"", 0, 0, yggr::charset::charset_name_t<wchar_t>()));

		yggr_test_assert(0 == nsv.find(
						yggr::charset::make_string_charset_helper(
							L"Äã", yggr::charset::charset_name_t<wchar_t>()),
						0, yggr::charset::charset_base_foo::strlen(L"Äã")));
		yggr_test_assert(0 == nsv.find(yggr::charset::make_string_charset_helper("", yggr::charset::charset_name_t<wchar_t>()), 0, 0));

#endif // YGGR_NO_CWCHAR

		
		// inline size_type find(const_pointer s, size_type pos = 0) const
		if(yggr::charset::charset_name_t<char>() != yggr::charset::charset_name_t<yggr::utf8_string>())
		{
			try
			{
				nsv.find(nsv.substr().data(), 0);
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}

			try
			{
				nsv.find(nsv.substr(1).data());
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
		else
		{
			yggr_test_assert(0 == nsv.find(nsv.substr().data(), 0));
			yggr_test_assert(1 == nsv.find(nsv.substr(1).data(), 0));
		}
		
		yggr_test_assert(0 == nsv.find(nsv.substr().data(), 0, YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(1 == nsv.find(nsv.substr(1).data(), 0, YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(0 == nsv.find(yggr::charset::make_string_charset_helper(nsv.substr().data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 0));
		yggr_test_assert(1 == nsv.find(yggr::charset::make_string_charset_helper(nsv.substr(1).data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 0));

		yggr_test_assert(yggr::charset::string_view::npos == nsv.find(nsv.substr().data(), 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(1 == nsv.find(nsv.substr(1).data(), 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find("z", 0));
		yggr_test_assert(3 == nsv.find("c", 0));

	}

	std::cout << "--------------test_operations_find end-----------------" << std::endl;
}


void test_operations_rfind(void)
{
	std::cout << "--------------test_operations_rfind begin-----------------" << std::endl;

	{
		yggr::utf8_string org("abcde");
		yggr::charset::utf8_string_view nsv(org);
		std::cout << nsv << std::endl;

		// inline size_type rfind(const this_type& right, size_type pos = this_type::npos) const YGGR_NOEXCEPT_OR_NOTHROW
		yggr_test_assert(0 == nsv.rfind(nsv.substr()));
		yggr_test_assert(1 == nsv.rfind(nsv.substr(1)));

		yggr_test_assert(0 == nsv.rfind(nsv.substr(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(1 == nsv.rfind(nsv.substr(1), YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(0 == nsv.rfind(yggr::charset::make_string_charset_helper(nsv.substr(), YGGR_STR_UTF8_STRING_CHARSET_NAME())));
		yggr_test_assert(1 == nsv.rfind(yggr::charset::make_string_charset_helper(nsv.substr(1), YGGR_STR_UTF8_STRING_CHARSET_NAME())));

		yggr_test_assert(0 == nsv.rfind(nsv.substr(), 1));
		yggr_test_assert(1 == nsv.rfind(nsv.substr(1), 1));

		yggr_test_assert(0 == nsv.rfind(nsv.substr(), 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(1 == nsv.rfind(nsv.substr(1), 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(0 == nsv.rfind(yggr::charset::make_string_charset_helper(nsv.substr(), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 1));
		yggr_test_assert(1 == nsv.rfind(yggr::charset::make_string_charset_helper(nsv.substr(1), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 1));

		// inline size_type rfind(value_type c, size_type pos = this_type::npos) const YGGR_NOEXCEPT_OR_NOTHROW

		yggr_test_assert(0 == nsv.rfind('a'));
		yggr_test_assert(nsv.size() - 1 == nsv.rfind('e'));

		yggr_test_assert(0 == nsv.rfind('a', YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(nsv.size() - 1 == nsv.rfind('e', YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(0 == nsv.rfind(yggr::charset::make_string_charset_helper('a', yggr::charset::charset_name_t<char>())));
		yggr_test_assert(nsv.size() - 1 == nsv.rfind('e', yggr::charset::charset_name_t<char>()));

		yggr_test_assert(0 == nsv.rfind(nsv.front()));
		yggr_test_assert(nsv.size() - 1 == nsv.rfind(nsv.back()));

		yggr_test_assert(0 == nsv.rfind(nsv.front(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(nsv.size() - 1 == nsv.rfind(nsv.back(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(0 == nsv.rfind(yggr::charset::make_string_charset_helper(nsv.front(), YGGR_STR_UTF8_STRING_CHARSET_NAME())));
		yggr_test_assert(nsv.size() - 1 == nsv.rfind(yggr::charset::make_string_charset_helper(nsv.back(), YGGR_STR_UTF8_STRING_CHARSET_NAME())));

		yggr_test_assert(0 == nsv.rfind('a', 1));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.rfind('e', 1));

		yggr_test_assert(0 == nsv.rfind(nsv.front(), 1));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.rfind(nsv.back(), 1));

		// inline size_type rfind(const_pointer s, size_type pos, size_type n) const
		yggr_test_assert(0 == nsv.rfind(nsv.substr().data(), 0, nsv.substr().size()));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.rfind(nsv.substr(1).data(), 0, nsv.substr(1).size()));

		yggr_test_assert(0 == nsv.rfind(nsv.substr().data(), 0, nsv.substr().size(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.rfind(nsv.substr(1).data(), 0, nsv.substr(1).size(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(0 == nsv.rfind(
						yggr::charset::make_string_charset_helper(
							nsv.substr().data(), yggr::charset::charset_name_t<char>()),
						0, nsv.substr().size()));
		yggr_test_assert(yggr::charset::string_view::npos == 
				nsv.rfind(
					yggr::charset::make_string_charset_helper(
						nsv.substr(1).data(), yggr::charset::charset_name_t<char>()), 
					0, nsv.substr(1).size()));

		yggr_test_assert(0 == nsv.rfind(nsv.substr().data(), 1, nsv.substr().size()));
		yggr_test_assert(1 == nsv.rfind(nsv.substr(1).data(), 1, nsv.substr(1).size()));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.rfind("z", 0, 1));
		yggr_test_assert(2 == nsv.find("c", 0, 1));
		yggr_test_assert(0 == nsv.rfind("", 0, 0));

		// inline size_type rfind(const_pointer s, size_type pos = 0) const
		yggr_test_assert(0 == nsv.rfind(nsv.substr().data(), 0));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.rfind(nsv.substr(1).data(), 0));

		yggr_test_assert(0 == nsv.rfind(nsv.substr().data(), 0, YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.rfind(nsv.substr(1).data(), 0, YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(0 == nsv.rfind(
						yggr::charset::make_string_charset_helper(
							nsv.substr().data(), yggr::charset::charset_name_t<char>()),
						0));
		yggr_test_assert(yggr::charset::string_view::npos == 
				nsv.rfind(
					yggr::charset::make_string_charset_helper(
						nsv.substr(1).data(), yggr::charset::charset_name_t<char>()),
					0));

		yggr_test_assert(0 == nsv.rfind(nsv.substr().data(), 1));
		yggr_test_assert(1 == nsv.rfind(nsv.substr(1).data(), 1));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.rfind("z", 0));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.rfind("c", 0));
	}

	{
		yggr::utf8_string org("ÄãabcdeºÃ");
		yggr::charset::utf8_string_view nsv(org);
		std::cout << nsv << std::endl;

		// inline size_type rfind(const this_type& right, size_type pos = this_type::npos) const YGGR_NOEXCEPT_OR_NOTHROW
		yggr_test_assert(0 == nsv.rfind(nsv.substr()));
		yggr_test_assert(1 == nsv.rfind(nsv.substr(1)));

		yggr_test_assert(0 == nsv.rfind(nsv.substr(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(1 == nsv.rfind(nsv.substr(1), YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(0 == nsv.rfind(yggr::charset::make_string_charset_helper(nsv.substr(), YGGR_STR_UTF8_STRING_CHARSET_NAME())));
		yggr_test_assert(1 == nsv.rfind(yggr::charset::make_string_charset_helper(nsv.substr(1), YGGR_STR_UTF8_STRING_CHARSET_NAME())));

		yggr_test_assert(0 == nsv.rfind(nsv.substr(), 1));
		yggr_test_assert(1 == nsv.rfind(nsv.substr(1), 1));

		yggr_test_assert(0 == nsv.rfind(nsv.substr(), 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(1 == nsv.rfind(nsv.substr(1), 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(0 == nsv.rfind(yggr::charset::make_string_charset_helper(nsv.substr(), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 1));
		yggr_test_assert(1 == nsv.rfind(yggr::charset::make_string_charset_helper(nsv.substr(1), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 1));

		// inline size_type rfind(value_type c, size_type pos = this_type::npos) const YGGR_NOEXCEPT_OR_NOTHROW

		yggr_test_assert(1 == nsv.rfind('a'));
		yggr_test_assert(nsv.length() - 2 == nsv.rfind('e'));

		yggr_test_assert(1 == nsv.rfind('a', YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(nsv.length() - 2 == nsv.rfind('e', YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(1 == nsv.rfind(yggr::charset::make_string_charset_helper('a', yggr::charset::charset_name_t<char>())));
		yggr_test_assert(nsv.length() - 2 == nsv.rfind('e', yggr::charset::charset_name_t<char>()));

#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(1 == nsv.rfind(L'a'));
		yggr_test_assert(nsv.length() - 2 == nsv.rfind(L'e'));

		yggr_test_assert(1 == nsv.rfind(L'a', yggr::charset::charset_name_t<wchar_t>()));
		yggr_test_assert(nsv.length() - 2 == nsv.rfind(L'e', yggr::charset::charset_name_t<wchar_t>()));

		yggr_test_assert(1 == nsv.rfind(yggr::charset::make_string_charset_helper(L'a', yggr::charset::charset_name_t<wchar_t>())));
		yggr_test_assert(nsv.length() - 2 == nsv.rfind(L'e', yggr::charset::charset_name_t<wchar_t>()));
#endif // YGGR_NO_CWCHAR

		yggr_test_assert(0 == nsv.rfind(nsv.front()));
		yggr_test_assert(nsv.length() - 1 == nsv.rfind(nsv.back()));

		yggr_test_assert(0 == nsv.rfind(nsv.front(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(nsv.length() - 1 == nsv.rfind(nsv.back(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(0 == nsv.rfind(yggr::charset::make_string_charset_helper(nsv.front(), YGGR_STR_UTF8_STRING_CHARSET_NAME())));
		yggr_test_assert(nsv.length() - 1 == nsv.rfind(yggr::charset::make_string_charset_helper(nsv.back(), YGGR_STR_UTF8_STRING_CHARSET_NAME())));

		yggr_test_assert(1 == nsv.rfind('a', 1));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.rfind('e', 1));

#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(1 == nsv.rfind(L'a', 1));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.rfind(L'e', 1));
#endif // YGGR_NO_CWCHAR

		yggr_test_assert(0 == nsv.rfind(nsv.front(), 1));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.rfind(nsv.back(), 1));

		// inline size_type rfind(const_pointer s, size_type pos, size_type n) const

		if(yggr::charset::charset_name_t<char>() != yggr::charset::charset_name_t<yggr::utf8_string>())
		{
			try
			{
				nsv.rfind(nsv.substr().data(), 0, nsv.substr().size());
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}

			try
			{
				nsv.rfind(nsv.substr(1).data(), 0, nsv.substr(1).size());
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
		else
		{
			yggr_test_assert(0 == nsv.rfind(nsv.substr().data(), 0, nsv.substr().size()));
			yggr_test_assert(yggr::charset::string_view::npos == nsv.rfind(nsv.substr(1).data(), 0, nsv.substr(1).size()));
		}

		yggr_test_assert(0 == nsv.rfind(nsv.substr().data(), 0, nsv.substr().size(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.rfind(nsv.substr(1).data(), 0, nsv.substr(1).size(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(0 == nsv.rfind(
						yggr::charset::make_string_charset_helper(
							nsv.substr().data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()),
						0, nsv.substr().size()));
		yggr_test_assert(yggr::charset::string_view::npos == 
				nsv.rfind(
					yggr::charset::make_string_charset_helper(
						nsv.substr(1).data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 
					0, nsv.substr(1).size()));

		if(yggr::charset::charset_name_t<char>() != yggr::charset::charset_name_t<yggr::utf8_string>())
		{
			try
			{
				nsv.rfind(nsv.substr().data(), 1, nsv.substr().size());
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}

			try
			{
				nsv.rfind(nsv.substr(1).data(), 1, nsv.substr(1).size());
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
		else
		{
			yggr_test_assert(0 == nsv.rfind(nsv.substr().data(), 1, nsv.substr().size()));
			yggr_test_assert(1 == nsv.rfind(nsv.substr(1).data(), 1, nsv.substr(1).size()));
		}

		yggr_test_assert(0 == nsv.rfind(nsv.substr().data(), 1, nsv.substr().size(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(1 == nsv.rfind(nsv.substr(1).data(), 1, nsv.substr(1).size(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(yggr::charset::string_view::npos == nsv.rfind("z", 0, 1));
		yggr_test_assert(3 == nsv.find("c", 0, 1));
		yggr_test_assert(0 == nsv.rfind("", 0, 0));

		// inline size_type rfind(const_pointer s, size_type pos = 0) const
		if(yggr::charset::charset_name_t<char>() != yggr::charset::charset_name_t<yggr::utf8_string>())
		{
			try
			{
				nsv.rfind(nsv.substr().data(), 0);
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}

			try
			{
				nsv.rfind(nsv.substr(1).data(), 0);
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
		else
		{
			yggr_test_assert(0 == nsv.rfind(nsv.substr().data(), 0));
			yggr_test_assert(yggr::charset::string_view::npos == nsv.rfind(nsv.substr(1).data(), 0));
		}

		yggr_test_assert(0 == nsv.rfind(nsv.substr().data(), 0, YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.rfind(nsv.substr(1).data(), 0, YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(0 == nsv.rfind(
						yggr::charset::make_string_charset_helper(
							nsv.substr().data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()),
						0));
		yggr_test_assert(yggr::charset::string_view::npos == 
				nsv.rfind(
					yggr::charset::make_string_charset_helper(
						nsv.substr(1).data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()),
					0));

		yggr_test_assert(0 == nsv.rfind(nsv.substr().data(), 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(1 == nsv.rfind(nsv.substr(1).data(), 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(yggr::charset::string_view::npos == nsv.rfind("z", 0));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.rfind("c", 0));
	}

	std::cout << "--------------test_operations_rfind end-----------------" << std::endl;
}

void test_operations_find_first_of(void)
{
	std::cout << "--------------test_operations_find_first_of begin-----------------" << std::endl;
	{
		yggr::charset::utf8_string org("abcde");
		yggr::charset::utf8_string_view nsv(org);
		std::cout << nsv << std::endl;

		// inline size_type find_first_of(const this_type& right, size_type pos = 0) const YGGR_NOEXCEPT_OR_NOTHROW
		yggr_test_assert(0 == nsv.find_first_of("ace"));
		yggr_test_assert(2 == nsv.find_first_of("ce"));

		yggr_test_assert(0 == nsv.find_first_of("ace", YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(2 == nsv.find_first_of("ce", YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(0 == nsv.find_first_of(yggr::charset::make_string_charset_helper("ace", YGGR_STR_UTF8_STRING_CHARSET_NAME())));
		yggr_test_assert(2 == nsv.find_first_of(yggr::charset::make_string_charset_helper("ce", YGGR_STR_UTF8_STRING_CHARSET_NAME())));

		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_first_of("fgh"));
		
		yggr_test_assert(2 == nsv.find_first_of("ce", 1));
		yggr_test_assert(2 == nsv.find_first_of("ce", 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(2 == nsv.find_first_of(yggr::charset::make_string_charset_helper("ce", YGGR_STR_UTF8_STRING_CHARSET_NAME()), 1));


		// inline size_type find_first_of(value_type c, size_type pos = 0) const YGGR_NOEXCEPT_OR_NOTHROW
		yggr_test_assert(0 == nsv.find_first_of('a'));
		yggr_test_assert(nsv.size() - 1 == nsv.find_first_of('e'));

		yggr_test_assert(0 == nsv.find_first_of('a', YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(nsv.size() - 1 == nsv.find_first_of('e', YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(0 == nsv.find_first_of(yggr::charset::make_string_charset_helper('a', yggr::charset::charset_name_t<char>())));
		yggr_test_assert(nsv.size() - 1 == nsv.find_first_of(yggr::charset::make_string_charset_helper('e', yggr::charset::charset_name_t<char>())));

		yggr_test_assert(0 == nsv.find_first_of(nsv.front()));
		yggr_test_assert(nsv.size() - 1 == nsv.find_first_of(nsv.back()));

		yggr_test_assert(0 == nsv.find_first_of(nsv.front()));
		yggr_test_assert(nsv.size() - 1 == nsv.find_first_of(nsv.back()));

		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_first_of('a', 1));
		yggr_test_assert(nsv.size() - 1 == nsv.find_first_of('e', 1));

		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_first_of(nsv.front(), 1));
		yggr_test_assert(nsv.size() - 1 == nsv.find_first_of(nsv.back(), 1));

		// inline size_type find_first_of(const_pointer s, size_type pos, size_type n) const
		yggr_test_assert(0 == nsv.find_first_of(nsv.substr().data(), 0, nsv.substr().size()));
		yggr_test_assert(1 == nsv.find_first_of(nsv.substr(1).data(), 0, nsv.substr(1).size()));

		yggr_test_assert(0 == nsv.find_first_of(nsv.substr().data(), 0, nsv.substr().size(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(1 == nsv.find_first_of(nsv.substr(1).data(), 0, nsv.substr(1).size(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(0 == nsv.find_first_of(
						yggr::charset::make_string_charset_helper(nsv.substr().data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 
						0, nsv.substr().size()));
		yggr_test_assert(1 == nsv.find_first_of(
						yggr::charset::make_string_charset_helper(nsv.substr(1).data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()),
						0, nsv.substr(1).size()));

		yggr_test_assert(1 == nsv.find_first_of(nsv.substr().data(), 1, nsv.substr().size()));
		yggr_test_assert(1 == nsv.find_first_of(nsv.substr(1).data(), 1, nsv.substr(1).size()));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_first_of("z", 0, 1));
		yggr_test_assert(2 == nsv.find_first_of("c", 0, 1));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_first_of("", 0, 0));

		// inline size_type find_first_of(const_pointer s, size_type pos = 0) const
		yggr_test_assert(0 == nsv.find_first_of(nsv.substr().data(), 0));
		yggr_test_assert(1 == nsv.find_first_of(nsv.substr(1).data(), 0));

		yggr_test_assert(0 == nsv.find_first_of(nsv.substr().data(), 0, YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(1 == nsv.find_first_of(nsv.substr(1).data(), 0, YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(0 == nsv.find_first_of(
						yggr::charset::make_string_charset_helper(
							nsv.substr().data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 0));
		yggr_test_assert(1 == nsv.find_first_of(
						yggr::charset::make_string_charset_helper(
							nsv.substr(1).data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 0));

		yggr_test_assert(1 == nsv.find_first_of(nsv.substr().data(), 1));
		yggr_test_assert(1 == nsv.find_first_of(nsv.substr(1).data(), 1));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_first_of("z", 0));
		yggr_test_assert(2 == nsv.find_first_of("c", 0));
	}

	{
		yggr::charset::utf8_string org("ÄãabcdeºÃ");
		yggr::charset::utf8_string_view nsv(org);
		std::cout << nsv << std::endl;

		// inline size_type find_first_of(const this_type& right, size_type pos = 0) const YGGR_NOEXCEPT_OR_NOTHROW
		yggr_test_assert(0 == nsv.find_first_of("Äãace"));
		yggr_test_assert(2 == nsv.find_first_of("bce"));

		yggr_test_assert(0 == nsv.find_first_of("Äãace", yggr::charset::charset_name_t<char>()));
		yggr_test_assert(2 == nsv.find_first_of("bce", yggr::charset::charset_name_t<char>()));

		yggr_test_assert(0 == nsv.find_first_of(
						yggr::charset::make_string_charset_helper(
							"Äãace", yggr::charset::charset_name_t<char>())));
		yggr_test_assert(2 == nsv.find_first_of(
						yggr::charset::make_string_charset_helper(
							"bce", yggr::charset::charset_name_t<char>())));

#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(0 == nsv.find_first_of(L"Äãace"));
		yggr_test_assert(2 == nsv.find_first_of(L"bce"));

		yggr_test_assert(0 == nsv.find_first_of(L"Äãace", yggr::charset::charset_name_t<wchar_t>()));
		yggr_test_assert(2 == nsv.find_first_of(L"bce", yggr::charset::charset_name_t<wchar_t>()));

		yggr_test_assert(0 == nsv.find_first_of(
						yggr::charset::make_string_charset_helper(
							L"Äãace", yggr::charset::charset_name_t<wchar_t>())));
		yggr_test_assert(2 == nsv.find_first_of(
						yggr::charset::make_string_charset_helper(
							L"bce", yggr::charset::charset_name_t<wchar_t>())));
#endif // YGGR_NO_CWCHAR

		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_first_of("fgh"));
		
		yggr_test_assert(2 == nsv.find_first_of("Äãbce", 1));
		yggr_test_assert(2 == nsv.find_first_of("Äãbce", 1, yggr::charset::charset_name_t<char>()));
		yggr_test_assert(2 == nsv.find_first_of(yggr::charset::make_string_charset_helper("Äãbce", yggr::charset::charset_name_t<char>()), 1));

#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(2 == nsv.find_first_of(L"Äãbce", 1));
		yggr_test_assert(2 == nsv.find_first_of(L"Äãbce", 1, yggr::charset::charset_name_t<wchar_t>()));
		yggr_test_assert(2 == nsv.find_first_of(
						yggr::charset::make_string_charset_helper(
							L"Äãbce", yggr::charset::charset_name_t<wchar_t>()), 1));
#endif // YGGR_NO_CWCHAR

		// inline size_type find_first_of(value_type c, size_type pos = 0) const YGGR_NOEXCEPT_OR_NOTHROW
		yggr_test_assert(0 == nsv.find_first_of(nsv.front()));
		yggr_test_assert(nsv.length() - 1 == nsv.find_first_of(nsv.back()));

		yggr_test_assert(0 == nsv.find_first_of(nsv.front()));
		yggr_test_assert(nsv.length() - 1 == nsv.find_first_of(nsv.back()));

		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_first_of(nsv.front(), 1));
		yggr_test_assert(nsv.length() - 1 == nsv.find_first_of(nsv.back(), 1));

		// inline size_type find_first_of(const_pointer s, size_type pos, size_type n) const

		if(yggr::charset::charset_name_t<char>() != yggr::charset::charset_name_t<yggr::utf8_string>())
		{
			try
			{
				nsv.find_first_of(nsv.substr().data(), 0, nsv.substr().size());
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}

			try
			{
				nsv.find_first_of(nsv.substr(1).data(), 0, nsv.substr(1).size());
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
		else
		{
			yggr_test_assert(0 == nsv.find_first_of(nsv.substr().data(), 0, nsv.substr().size()));
			yggr_test_assert(1 == nsv.find_first_of(nsv.substr(1).data(), 0, nsv.substr(1).size()));
		}

		yggr_test_assert(0 == nsv.find_first_of(nsv.substr().data(), 0, nsv.substr().size(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(1 == nsv.find_first_of(nsv.substr(1).data(), 0, nsv.substr(1).size(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(0 == nsv.find_first_of(
						yggr::charset::make_string_charset_helper(
							nsv.substr().data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 
						0, nsv.substr().size()));
		yggr_test_assert(1 == nsv.find_first_of(
						yggr::charset::make_string_charset_helper(
							nsv.substr(1).data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()),
						0, nsv.substr(1).size()));

		if(yggr::charset::charset_name_t<char>() != yggr::charset::charset_name_t<yggr::utf8_string>())
		{
			try
			{
				nsv.find_first_of(nsv.substr().data(), 1, nsv.substr().size());
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}

			try
			{
				nsv.find_first_of(nsv.substr(1).data(), 1, nsv.substr(1).size());
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
		else
		{
			yggr_test_assert(1 == nsv.find_first_of(nsv.substr().data(), 1, nsv.substr().size()));
			yggr_test_assert(1 == nsv.find_first_of(nsv.substr(1).data(), 1, nsv.substr(1).size()));
		}

		yggr_test_assert(1 == nsv.find_first_of(nsv.substr().data(), 1, nsv.substr().size(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(1 == nsv.find_first_of(nsv.substr(1).data(), 1, nsv.substr(1).size(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_first_of("z", 0, 1));
		yggr_test_assert(2 == nsv.find_first_of("b", 0, 1));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_first_of("", 0, 0));

		// inline size_type find_first_of(const_pointer s, size_type pos = 0) const
		if(yggr::charset::charset_name_t<char>() != yggr::charset::charset_name_t<yggr::utf8_string>())
		{
			try
			{
				nsv.find_first_of(nsv.substr().data(), 0);
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}

			try
			{
				nsv.find_first_of(nsv.substr(1).data(), 0);
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
		else
		{
			yggr_test_assert(0 == nsv.find_first_of(nsv.substr().data(), 0));
			yggr_test_assert(1 == nsv.find_first_of(nsv.substr(1).data(), 0));
		}

		yggr_test_assert(0 == nsv.find_first_of(nsv.substr().data(), 0, YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(1 == nsv.find_first_of(nsv.substr(1).data(), 0, YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(0 == nsv.find_first_of(
						yggr::charset::make_string_charset_helper(
							nsv.substr().data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 0));
		yggr_test_assert(1 == nsv.find_first_of(
						yggr::charset::make_string_charset_helper(
							nsv.substr(1).data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 0));

		if(yggr::charset::charset_name_t<char>() != yggr::charset::charset_name_t<yggr::utf8_string>())
		{
			try
			{
				nsv.find_first_of(nsv.substr().data(), 1);
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}

			try
			{
				nsv.find_first_of(nsv.substr(1).data(), 1);
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
		else
		{
			yggr_test_assert(1 == nsv.find_first_of(nsv.substr().data(), 1));
			yggr_test_assert(1 == nsv.find_first_of(nsv.substr(1).data(), 1));
		}

		yggr_test_assert(1 == nsv.find_first_of(nsv.substr().data(), 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(1 == nsv.find_first_of(nsv.substr(1).data(), 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_first_of("z", 0));
		yggr_test_assert(2 == nsv.find_first_of("b", 0));
	}

	std::cout << "--------------test_operations_find_first_of end-----------------" << std::endl;
}

void test_operations_find_last_of(void)
{
	std::cout << "--------------test_operations_find_last_of begin-----------------" << std::endl;

	{
		yggr::charset::utf8_string org("abcde");
		yggr::charset::utf8_string_view nsv(org);
		std::cout << nsv << std::endl;

		// inline size_type find_last_of(const this_type& right, size_type pos = 0) const YGGR_NOEXCEPT_OR_NOTHROW
		yggr_test_assert(4 == nsv.find_last_of("ace"));
		yggr_test_assert(4 == nsv.find_last_of("ce"));

		yggr_test_assert(4 == nsv.find_last_of("ace", YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(4 == nsv.find_last_of("ce", YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(4 == nsv.find_last_of(yggr::charset::make_string_charset_helper("ace", YGGR_STR_UTF8_STRING_CHARSET_NAME())));
		yggr_test_assert(4 == nsv.find_last_of(yggr::charset::make_string_charset_helper("ce", YGGR_STR_UTF8_STRING_CHARSET_NAME())));

		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of("fgh"));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of("ce", 1));

		// inline size_type find_last_of(value_type c, size_type pos = 0) const YGGR_NOEXCEPT_OR_NOTHROW
		yggr_test_assert(0 == nsv.find_last_of('a'));
		yggr_test_assert(nsv.size() - 1 == nsv.find_last_of('e'));

		yggr_test_assert(0 == nsv.find_last_of('a', YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(nsv.size() - 1 == nsv.find_last_of('e', YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(0 == nsv.find_last_of(yggr::charset::make_string_charset_helper('a', yggr::charset::charset_name_t<char>())));
		yggr_test_assert(nsv.size() - 1 == nsv.find_last_of(yggr::charset::make_string_charset_helper('e', yggr::charset::charset_name_t<char>())));

		yggr_test_assert(0 == nsv.find_last_of(nsv.front()));
		yggr_test_assert(nsv.size() - 1 == nsv.find_last_of(nsv.back()));

		yggr_test_assert(0 == nsv.find_last_of(nsv.front(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(nsv.size() - 1 == nsv.find_last_of(nsv.back(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(0 == nsv.find_last_of(yggr::charset::make_string_charset_helper(nsv.front(), YGGR_STR_UTF8_STRING_CHARSET_NAME())));
		yggr_test_assert(nsv.size() - 1 == nsv.find_last_of(yggr::charset::make_string_charset_helper(nsv.back(), YGGR_STR_UTF8_STRING_CHARSET_NAME())));

		yggr_test_assert(0 == nsv.find_last_of('a', 1));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of('e', 1));

		yggr_test_assert(0 == nsv.find_last_of(nsv.front(), 1));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of(nsv.back(), 1));

		// inline size_type find_last_of(const_pointer s, size_type pos, size_type n) const
		yggr_test_assert(0 == nsv.find_last_of(nsv.substr().data(), 0, nsv.substr().size()));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of(nsv.substr(1).data(), 0, nsv.substr(1).size()));

		yggr_test_assert(0 == nsv.find_last_of(
						nsv.substr().data(), 0, nsv.substr().size(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(yggr::charset::string_view::npos == 
					nsv.find_last_of(nsv.substr(1).data(), 0, nsv.substr(1).size(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(0 == nsv.find_last_of(
						yggr::charset::make_string_charset_helper(
							nsv.substr().data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 0, nsv.substr().size()));
		yggr_test_assert(yggr::charset::string_view::npos == 
				nsv.find_last_of(
					yggr::charset::make_string_charset_helper(nsv.substr(1).data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 
					0, nsv.substr(1).size()));

		yggr_test_assert(1 == nsv.find_last_of(nsv.substr().data(), 1, nsv.substr().size()));
		yggr_test_assert(1 == nsv.find_last_of(nsv.substr(1).data(), 1, nsv.substr(1).size()));

		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of("z", 0, 1));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of("c", 0, 1));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of("", 0, 0));

		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of("z", 0, 1, yggr::charset::charset_name_t<char>()));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of("c", 0, 1, yggr::charset::charset_name_t<char>()));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of("", 0, 0, yggr::charset::charset_name_t<char>()));

		yggr_test_assert(yggr::charset::string_view::npos == 
				nsv.find_last_of(yggr::charset::make_string_charset_helper("z", yggr::charset::charset_name_t<char>()), 0, 1));
		yggr_test_assert(yggr::charset::string_view::npos == 
				nsv.find_last_of(yggr::charset::make_string_charset_helper("c", yggr::charset::charset_name_t<char>()), 0, 1));
		yggr_test_assert(yggr::charset::string_view::npos == 
				nsv.find_last_of(yggr::charset::make_string_charset_helper("", yggr::charset::charset_name_t<char>()), 0, 0));

#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of(L"z", 0, 1, yggr::charset::charset_name_t<wchar_t>()));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of(L"c", 0, 1, yggr::charset::charset_name_t<wchar_t>()));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of(L"", 0, 0, yggr::charset::charset_name_t<wchar_t>()));

		yggr_test_assert(yggr::charset::string_view::npos == 
				nsv.find_last_of(yggr::charset::make_string_charset_helper(L"z", yggr::charset::charset_name_t<wchar_t>()), 0, 1));
		yggr_test_assert(yggr::charset::string_view::npos == 
				nsv.find_last_of(yggr::charset::make_string_charset_helper(L"c", yggr::charset::charset_name_t<wchar_t>()), 0, 1));
		yggr_test_assert(yggr::charset::string_view::npos == 
				nsv.find_last_of(yggr::charset::make_string_charset_helper(L"", yggr::charset::charset_name_t<wchar_t>()), 0, 0));
#endif // YGGR_NO_CWCHAR

		// inline size_type find_last_of(const_pointer s, size_type pos = 0) const
		yggr_test_assert(0 == nsv.find_last_of(nsv.substr().data(), 0));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of(nsv.substr(1).data(), 0));

		yggr_test_assert(0 == nsv.find_last_of(nsv.substr().data(), 0, YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of(nsv.substr(1).data(), 0, YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(0 == nsv.find_last_of(
						yggr::charset::make_string_charset_helper(
							nsv.substr().data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 
						0));
		yggr_test_assert(yggr::charset::string_view::npos == 
				nsv.find_last_of(
					yggr::charset::make_string_charset_helper(
						nsv.substr(1).data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()),
					0));

		yggr_test_assert(1 == nsv.find_last_of(nsv.substr().data(), 1));
		yggr_test_assert(1 == nsv.find_last_of(nsv.substr(1).data(), 1));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of("z", 0));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of("c", 0));

		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of("z", 0, yggr::charset::charset_name_t<char>()));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of("c", 0, yggr::charset::charset_name_t<char>()));
	}

	{
		yggr::charset::utf8_string org("ÄãabcdeºÃ");
		yggr::charset::utf8_string_view nsv(org);
		std::cout << nsv << std::endl;

		// inline size_type find_last_of(const this_type& right, size_type pos = 0) const YGGR_NOEXCEPT_OR_NOTHROW
		yggr_test_assert(6 == nsv.find_last_of("aceºÃ"));
		yggr_test_assert(6 == nsv.find_last_of("ceºÃ"));

		yggr_test_assert(6 == nsv.find_last_of("aceºÃ", yggr::charset::charset_name_t<char>()));
		yggr_test_assert(6 == nsv.find_last_of("ceºÃ", yggr::charset::charset_name_t<char>()));

		yggr_test_assert(6 == nsv.find_last_of(
						yggr::charset::make_string_charset_helper(
							"aceºÃ", yggr::charset::charset_name_t<char>())));
		yggr_test_assert(6 == nsv.find_last_of(
						yggr::charset::make_string_charset_helper(
							"ceºÃ", yggr::charset::charset_name_t<char>())));

#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(6 == nsv.find_last_of(L"aceºÃ", yggr::charset::charset_name_t<wchar_t>()));
		yggr_test_assert(6 == nsv.find_last_of(L"ceºÃ", yggr::charset::charset_name_t<wchar_t>()));

		yggr_test_assert(6 == nsv.find_last_of(
						yggr::charset::make_string_charset_helper(
							L"aceºÃ", yggr::charset::charset_name_t<wchar_t>())));
		yggr_test_assert(6 == nsv.find_last_of(
						yggr::charset::make_string_charset_helper(
							L"ceºÃ", yggr::charset::charset_name_t<wchar_t>())));
#endif // YGGR_NO_CWCHAR

		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of("fgh"));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of("ce", 1));

		// inline size_type find_last_of(value_type c, size_type pos = 0) const YGGR_NOEXCEPT_OR_NOTHROW
		yggr_test_assert(0 == nsv.find_last_of(nsv.front()));
		yggr_test_assert(nsv.length() - 1 == nsv.find_last_of(nsv.back()));

		yggr_test_assert(0 == nsv.find_last_of(nsv.front(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(nsv.length() - 1 == nsv.find_last_of(nsv.back(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(0 == nsv.find_last_of(
						yggr::charset::make_string_charset_helper(
							nsv.front(), YGGR_STR_UTF8_STRING_CHARSET_NAME())));
		yggr_test_assert(nsv.length() - 1 == nsv.find_last_of(
									yggr::charset::make_string_charset_helper(
										nsv.back(), YGGR_STR_UTF8_STRING_CHARSET_NAME())));

		yggr_test_assert(0 == nsv.find_last_of(nsv.front(), 1));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of(nsv.back(), 1));

		// inline size_type find_last_of(const_pointer s, size_type pos, size_type n) const

		if(yggr::charset::charset_name_t<char>() != yggr::charset::charset_name_t<yggr::utf8_string>())
		{
			try
			{
				nsv.find_last_of(nsv.substr().data(), 0, nsv.substr().size());
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}

			try
			{
				nsv.find_last_of(nsv.substr(1).data(), 0, nsv.substr(1).size());
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
		else
		{
			yggr_test_assert(0 == nsv.find_last_of(
					nsv.substr().data(), 0, nsv.substr().size()));
			yggr_test_assert(yggr::charset::string_view::npos == 
					nsv.find_last_of(nsv.substr(1).data(), 0, nsv.substr(1).size()));
		}

		yggr_test_assert(0 == nsv.find_last_of(
						nsv.substr().data(), 0, nsv.substr().size(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(yggr::charset::string_view::npos == 
					nsv.find_last_of(nsv.substr(1).data(), 0, nsv.substr(1).size(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(0 == nsv.find_last_of(
						yggr::charset::make_string_charset_helper(
							nsv.substr().data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 0, nsv.substr().size()));
		yggr_test_assert(yggr::charset::string_view::npos == 
				nsv.find_last_of(
					yggr::charset::make_string_charset_helper(nsv.substr(1).data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 
					0, nsv.substr(1).size()));

		if(yggr::charset::charset_name_t<char>() != yggr::charset::charset_name_t<yggr::utf8_string>())
		{
			try
			{
				nsv.find_last_of(nsv.substr().data(), 1, nsv.substr().size());
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}

			try
			{
				nsv.find_last_of(nsv.substr(1).data(), 1, nsv.substr(1).size());
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
		else
		{
			yggr_test_assert(1 == nsv.find_last_of(nsv.substr().data(), 1, nsv.substr().size()));
			yggr_test_assert(1 == nsv.find_last_of(nsv.substr(1).data(), 1, nsv.substr(1).size()));
		}

		yggr_test_assert(1 == nsv.find_last_of(nsv.substr().data(), 1, nsv.substr().size(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(1 == nsv.find_last_of(nsv.substr(1).data(), 1, nsv.substr(1).size(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(1 == nsv.find_last_of(
						yggr::charset::make_string_charset_helper(nsv.substr().data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()),
						1, nsv.substr().size()));
		yggr_test_assert(1 == nsv.find_last_of(
						yggr::charset::make_string_charset_helper(nsv.substr(1).data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()),
						1, nsv.substr(1).size()));

		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of("z", 0, 1));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of("c", 0, 1));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of("", 0, 0));

		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of("z", 0, 1, yggr::charset::charset_name_t<char>()));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of("c", 0, 1, yggr::charset::charset_name_t<char>()));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of("", 0, 0, yggr::charset::charset_name_t<char>()));

		yggr_test_assert(yggr::charset::string_view::npos == 
				nsv.find_last_of(yggr::charset::make_string_charset_helper("z", yggr::charset::charset_name_t<char>()), 0, 1));
		yggr_test_assert(yggr::charset::string_view::npos == 
				nsv.find_last_of(yggr::charset::make_string_charset_helper("c", yggr::charset::charset_name_t<char>()), 0, 1));
		yggr_test_assert(yggr::charset::string_view::npos == 
				nsv.find_last_of(yggr::charset::make_string_charset_helper("", yggr::charset::charset_name_t<char>()), 0, 0));

#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of(L"z", 0, 1, yggr::charset::charset_name_t<wchar_t>()));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of(L"c", 0, 1, yggr::charset::charset_name_t<wchar_t>()));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of(L"", 0, 0, yggr::charset::charset_name_t<wchar_t>()));

		yggr_test_assert(yggr::charset::string_view::npos == 
				nsv.find_last_of(yggr::charset::make_string_charset_helper(L"z", yggr::charset::charset_name_t<wchar_t>()), 0, 1));
		yggr_test_assert(yggr::charset::string_view::npos == 
				nsv.find_last_of(yggr::charset::make_string_charset_helper(L"c", yggr::charset::charset_name_t<wchar_t>()), 0, 1));
		yggr_test_assert(yggr::charset::string_view::npos == 
				nsv.find_last_of(yggr::charset::make_string_charset_helper(L"", yggr::charset::charset_name_t<wchar_t>()), 0, 0));
#endif // YGGR_NO_CWCHAR

		// inline size_type find_last_of(const_pointer s, size_type pos = 0) const
		if(yggr::charset::charset_name_t<char>() != yggr::charset::charset_name_t<yggr::utf8_string>())
		{
			try
			{
				nsv.find_last_of(nsv.substr().data(), 0);
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}

			try
			{
				nsv.find_last_of(nsv.substr(1).data(), 0);
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
		else
		{
			yggr_test_assert(0 == nsv.find_last_of(nsv.substr().data(), 0));
			yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of(nsv.substr(1).data(), 0));
		}

		yggr_test_assert(0 == nsv.find_last_of(nsv.substr().data(), 0, YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of(nsv.substr(1).data(), 0, YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(0 == nsv.find_last_of(
						yggr::charset::make_string_charset_helper(
							nsv.substr().data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 
						0));
		yggr_test_assert(yggr::charset::string_view::npos == 
				nsv.find_last_of(
					yggr::charset::make_string_charset_helper(
						nsv.substr(1).data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()),
					0));

		if(yggr::charset::charset_name_t<char>() != yggr::charset::charset_name_t<yggr::utf8_string>())
		{
			try
			{
				nsv.find_last_of(nsv.substr().data(), 1);
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}

			try
			{
				nsv.find_last_of(nsv.substr(1).data(), 1);
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
		else
		{
			yggr_test_assert(1 == nsv.find_last_of(nsv.substr().data(), 1));
			yggr_test_assert(1 == nsv.find_last_of(nsv.substr(1).data(), 1));
		}

		yggr_test_assert(1 == nsv.find_last_of(nsv.substr().data(), 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(1 == nsv.find_last_of(nsv.substr(1).data(), 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of("z", 0));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of("c", 0));

		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of("z", 0, yggr::charset::charset_name_t<char>()));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of("c", 0, yggr::charset::charset_name_t<char>()));
	}

	std::cout << "--------------test_operations_find_last_of end-----------------" << std::endl;
}

void test_operations_find_first_not_of(void)
{
	std::cout << "--------------test_operations_find_first_not_of begin-----------------" << std::endl;
	{
		yggr::charset::utf8_string org("abcde");
		yggr::charset::utf8_string_view nsv(org);
		std::cout << nsv << std::endl;

		// inline size_type find_first_not_of(const this_type& right, size_type pos = 0) const YGGR_NOEXCEPT_OR_NOTHROW
		yggr_test_assert(1 == nsv.find_first_not_of("ace"));
		yggr_test_assert(0 == nsv.find_first_not_of("ce"));

		yggr_test_assert(1 == nsv.find_first_not_of("ace", YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(0 == nsv.find_first_not_of("ce", YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(1 == nsv.find_first_not_of(yggr::charset::make_string_charset_helper("ace", yggr::charset::charset_name_t<char>())));
		yggr_test_assert(0 == nsv.find_first_not_of(yggr::charset::make_string_charset_helper("ce", yggr::charset::charset_name_t<char>())));

		yggr_test_assert(0 == nsv.find_first_not_of("fgh"));
		yggr_test_assert(1 == nsv.find_first_not_of("ce", 1));

		// inline size_type find_first_not_of(value_type c, size_type pos = 0) const YGGR_NOEXCEPT_OR_NOTHROW
		yggr_test_assert(1 == nsv.find_first_not_of('a'));
		yggr_test_assert(0 == nsv.find_first_not_of('e'));

		yggr_test_assert(1 == nsv.find_first_not_of('a', YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(0 == nsv.find_first_not_of('e', YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(1 == nsv.find_first_not_of(yggr::charset::make_string_charset_helper('a', yggr::charset::charset_name_t<char>())));
		yggr_test_assert(0 == nsv.find_first_not_of(yggr::charset::make_string_charset_helper('e', yggr::charset::charset_name_t<char>())));

		yggr_test_assert(1 == nsv.find_first_not_of(nsv.front()));
		yggr_test_assert(0 == nsv.find_first_not_of(nsv.back()));

		yggr_test_assert(1 == nsv.find_first_not_of(nsv.front(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(0 == nsv.find_first_not_of(nsv.back(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(1 == nsv.find_first_not_of(yggr::charset::make_string_charset_helper(nsv.front(), YGGR_STR_UTF8_STRING_CHARSET_NAME())));
		yggr_test_assert(0 == nsv.find_first_not_of(yggr::charset::make_string_charset_helper(nsv.back(), YGGR_STR_UTF8_STRING_CHARSET_NAME())));

		yggr_test_assert(1 == nsv.find_first_not_of('a', 1));
		yggr_test_assert(1 == nsv.find_first_not_of('e', 1));

		yggr_test_assert(1 == nsv.find_first_not_of(nsv.front(), 1));
		yggr_test_assert(1 == nsv.find_first_not_of(nsv.back(), 1));

		// inline size_type find_first_not_of(const_pointer s, size_type pos, size_type n) const
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_first_not_of(nsv.substr().data(), 0, nsv.substr().size()));
		yggr_test_assert(0 == nsv.find_first_not_of(nsv.substr(1).data(), 0, nsv.substr(1).size()));

		yggr_test_assert(yggr::charset::string_view::npos == 
				nsv.find_first_not_of(
					nsv.substr().data(), 0, nsv.substr().size(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(0 == nsv.find_first_not_of(
						nsv.substr(1).data(), 0, nsv.substr(1).size(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(yggr::charset::string_view::npos == 
				nsv.find_first_not_of(
					yggr::charset::make_string_charset_helper(nsv.substr().data(), yggr::charset::charset_name_t<char>()),
					0, nsv.substr().size()));
		yggr_test_assert(0 == nsv.find_first_not_of(
						yggr::charset::make_string_charset_helper(nsv.substr(1).data(), yggr::charset::charset_name_t<char>()),
						0, nsv.substr(1).size()));

		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_first_not_of(nsv.substr().data(), 1, nsv.substr().size()));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_first_not_of(nsv.substr(1).data(), 1, nsv.substr(1).size()));
		yggr_test_assert(0 == nsv.find_first_not_of("z", 0, 1));
		yggr_test_assert(0 == nsv.find_first_not_of("c", 0, 1));
		yggr_test_assert(0 == nsv.find_first_not_of("", 0, 0));

		// inline size_type find_first_not_of(const_pointer s, size_type pos = 0) const
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_first_not_of(nsv.substr().data(), 0));
		yggr_test_assert(0 == nsv.find_first_not_of(nsv.substr(1).data(), 0));

		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_first_not_of(nsv.substr().data(), 0, YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(0 == nsv.find_first_not_of(nsv.substr(1).data(), 0, YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(yggr::charset::string_view::npos == 
				nsv.find_first_not_of(
					nsv.substr().data(), 0, YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(0 == nsv.find_first_not_of(
						nsv.substr(1).data(), 0, YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(yggr::charset::string_view::npos == 
				nsv.find_first_not_of(
					yggr::charset::make_string_charset_helper(
						nsv.substr().data(), yggr::charset::charset_name_t<char>()),
					0));
		yggr_test_assert(0 == nsv.find_first_not_of(
						yggr::charset::make_string_charset_helper(
							nsv.substr(1).data(), yggr::charset::charset_name_t<char>()),
						0));

		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_first_not_of(nsv.substr().data(), 1));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_first_not_of(nsv.substr(1).data(), 1));
		yggr_test_assert(0 == nsv.find_first_not_of("z", 0));
		yggr_test_assert(0 == nsv.find_first_not_of("c", 0));
	}

	{
		yggr::charset::utf8_string org("ÄãabcdeºÃ");
		yggr::charset::utf8_string_view nsv(org);
		std::cout << nsv << std::endl;

		// inline size_type find_first_not_of(const this_type& right, size_type pos = 0) const YGGR_NOEXCEPT_OR_NOTHROW
		yggr_test_assert(1 == nsv.find_first_not_of("Äãce"));
		yggr_test_assert(0 == nsv.find_first_not_of("ce"));

		yggr_test_assert(1 == nsv.find_first_not_of("Äãce", yggr::charset::charset_name_t<char>()));
		yggr_test_assert(0 == nsv.find_first_not_of("ce", yggr::charset::charset_name_t<char>()));

		yggr_test_assert(1 == nsv.find_first_not_of(yggr::charset::make_string_charset_helper("Äãce", yggr::charset::charset_name_t<char>())));
		yggr_test_assert(0 == nsv.find_first_not_of(yggr::charset::make_string_charset_helper("ce", yggr::charset::charset_name_t<char>())));

#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(1 == nsv.find_first_not_of(L"Äãce"));
		yggr_test_assert(0 == nsv.find_first_not_of(L"ce"));

		yggr_test_assert(1 == nsv.find_first_not_of(L"Äãce", yggr::charset::charset_name_t<wchar_t>()));
		yggr_test_assert(0 == nsv.find_first_not_of(L"ce", yggr::charset::charset_name_t<wchar_t>()));

		yggr_test_assert(1 == nsv.find_first_not_of(yggr::charset::make_string_charset_helper(L"Äãce", yggr::charset::charset_name_t<wchar_t>())));
		yggr_test_assert(0 == nsv.find_first_not_of(yggr::charset::make_string_charset_helper(L"ce", yggr::charset::charset_name_t<wchar_t>())));
#endif // YGGR_NO_CWCHAR

		yggr_test_assert(0 == nsv.find_first_not_of("fgh"));
		yggr_test_assert(1 == nsv.find_first_not_of("ce", 1));

		// inline size_type find_first_not_of(value_type c, size_type pos = 0) const YGGR_NOEXCEPT_OR_NOTHROW
		yggr_test_assert(0 == nsv.find_first_not_of('a'));
		yggr_test_assert(0 == nsv.find_first_not_of('e'));

		yggr_test_assert(0 == nsv.find_first_not_of('a', yggr::charset::charset_name_t<char>()));
		yggr_test_assert(0 == nsv.find_first_not_of('e', yggr::charset::charset_name_t<char>()));

		yggr_test_assert(0 == nsv.find_first_not_of(
						yggr::charset::make_string_charset_helper('a', yggr::charset::charset_name_t<char>())));
		yggr_test_assert(0 == nsv.find_first_not_of(
						yggr::charset::make_string_charset_helper('e', yggr::charset::charset_name_t<char>())));

#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(0 == nsv.find_first_not_of(L'a'));
		yggr_test_assert(0 == nsv.find_first_not_of(L'e'));

		yggr_test_assert(0 == nsv.find_first_not_of(L'a', yggr::charset::charset_name_t<wchar_t>()));
		yggr_test_assert(0 == nsv.find_first_not_of(L'e', yggr::charset::charset_name_t<wchar_t>()));

		yggr_test_assert(0 == nsv.find_first_not_of(
						yggr::charset::make_string_charset_helper(L'a', yggr::charset::charset_name_t<wchar_t>())));
		yggr_test_assert(0 == nsv.find_first_not_of(
						yggr::charset::make_string_charset_helper(L'e', yggr::charset::charset_name_t<wchar_t>())));
#endif // YGGR_NO_CWCHAR

		yggr_test_assert(1 == nsv.find_first_not_of(nsv.front()));
		yggr_test_assert(0 == nsv.find_first_not_of(nsv.back()));

		yggr_test_assert(1 == nsv.find_first_not_of(nsv.front(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(0 == nsv.find_first_not_of(nsv.back(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(1 == nsv.find_first_not_of(yggr::charset::make_string_charset_helper(nsv.front(), YGGR_STR_UTF8_STRING_CHARSET_NAME())));
		yggr_test_assert(0 == nsv.find_first_not_of(yggr::charset::make_string_charset_helper(nsv.back(), YGGR_STR_UTF8_STRING_CHARSET_NAME())));

		yggr_test_assert(2 == nsv.find_first_not_of('a', 1));
		yggr_test_assert(1 == nsv.find_first_not_of('e', 1));

#ifndef YGGR_NO_CWCHAR

		yggr_test_assert(2 == nsv.find_first_not_of(L'a', 1));
		yggr_test_assert(1 == nsv.find_first_not_of(L'e', 1));

#endif // YGGR_NO_CWCHAR

		yggr_test_assert(1 == nsv.find_first_not_of(nsv.front(), 1));
		yggr_test_assert(1 == nsv.find_first_not_of(nsv.back(), 1));

		// inline size_type find_first_not_of(const_pointer s, size_type pos, size_type n) const

		if(yggr::charset::charset_name_t<char>() != yggr::charset::charset_name_t<yggr::utf8_string>())
		{
			try
			{
				nsv.find_first_not_of(nsv.substr().data(), 0, nsv.substr().size());
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}

			try
			{
				nsv.find_first_not_of(nsv.substr(1).data(), 0, nsv.substr(1).size());
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
		else
		{
			yggr_test_assert(
				yggr::charset::string_view::npos == 
					nsv.find_first_not_of(nsv.substr().data(), 0, nsv.substr().size()));
			yggr_test_assert(0 == nsv.find_first_not_of(nsv.substr(1).data(), 0, nsv.substr(1).size()));
		}

		yggr_test_assert(yggr::charset::string_view::npos == 
				nsv.find_first_not_of(
					nsv.substr().data(), 0, nsv.substr().size(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(0 == nsv.find_first_not_of(
						nsv.substr(1).data(), 0, nsv.substr(1).size(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(yggr::charset::string_view::npos == 
				nsv.find_first_not_of(
					yggr::charset::make_string_charset_helper(nsv.substr().data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()),
					0, nsv.substr().size()));
		yggr_test_assert(0 == nsv.find_first_not_of(
						yggr::charset::make_string_charset_helper(nsv.substr(1).data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()),
						0, nsv.substr(1).size()));

		if(yggr::charset::charset_name_t<char>() != yggr::charset::charset_name_t<yggr::utf8_string>())
		{
			try
			{
				nsv.find_first_not_of(nsv.substr().data(), 1, nsv.substr().size());
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}

			try
			{
				nsv.find_first_not_of(nsv.substr(1).data(), 1, nsv.substr(1).size());
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
		else
		{
			yggr_test_assert(yggr::charset::string_view::npos == 
				nsv.find_first_not_of(nsv.substr().data(), 1, nsv.substr().size()));
			yggr_test_assert(yggr::charset::string_view::npos == 
				nsv.find_first_not_of(nsv.substr(1).data(), 1, nsv.substr(1).size()));
		}

		yggr_test_assert(yggr::charset::string_view::npos == 
				nsv.find_first_not_of(nsv.substr().data(), 1, nsv.substr().size(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(yggr::charset::string_view::npos == 
				nsv.find_first_not_of(nsv.substr(1).data(), 1, nsv.substr(1).size(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(0 == nsv.find_first_not_of("z", 0, 1));
		yggr_test_assert(0 == nsv.find_first_not_of("c", 0, 1));
		yggr_test_assert(0 == nsv.find_first_not_of("", 0, 0));

#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(0 == nsv.find_first_not_of(L"z", 0, 1));
		yggr_test_assert(0 == nsv.find_first_not_of(L"c", 0, 1));
		yggr_test_assert(0 == nsv.find_first_not_of(L"", 0, 0));
#endif // YGGR_NO_CWCHAR

		// inline size_type find_first_not_of(const_pointer s, size_type pos = 0) const
		if(yggr::charset::charset_name_t<char>() != yggr::charset::charset_name_t<yggr::utf8_string>())
		{
			try
			{
				nsv.find_first_not_of(nsv.substr().data(), 0);
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}

			try
			{
				nsv.find_first_not_of(nsv.substr(1).data(), 0);
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
		else
		{
			yggr_test_assert(yggr::charset::string_view::npos == nsv.find_first_not_of(nsv.substr().data(), 0));
			yggr_test_assert(0 == nsv.find_first_not_of(nsv.substr(1).data(), 0));
		}

		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_first_not_of(nsv.substr().data(), 0, YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(0 == nsv.find_first_not_of(nsv.substr(1).data(), 0, YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(yggr::charset::string_view::npos == 
				nsv.find_first_not_of(
					nsv.substr().data(), 0, YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(0 == nsv.find_first_not_of(
						nsv.substr(1).data(), 0, YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(yggr::charset::string_view::npos == 
				nsv.find_first_not_of(
					yggr::charset::make_string_charset_helper(
						nsv.substr().data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()),
					0));
		yggr_test_assert(0 == nsv.find_first_not_of(
						yggr::charset::make_string_charset_helper(
							nsv.substr(1).data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()),
						0));

		if(yggr::charset::charset_name_t<char>() != yggr::charset::charset_name_t<yggr::utf8_string>())
		{
			try
			{
				nsv.find_first_not_of(nsv.substr().data(), 1);
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}
		
			try
			{
				nsv.find_first_not_of(nsv.substr(1).data(), 1);
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
		else
		{
			yggr_test_assert(yggr::charset::string_view::npos == nsv.find_first_not_of(nsv.substr().data(), 1));
			yggr_test_assert(yggr::charset::string_view::npos == nsv.find_first_not_of(nsv.substr(1).data(), 1));
		}

		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_first_not_of(nsv.substr().data(), 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_first_not_of(nsv.substr(1).data(), 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(0 == nsv.find_first_not_of("z", 0));
		yggr_test_assert(0 == nsv.find_first_not_of("c", 0));

#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(0 == nsv.find_first_not_of(L"z", 0));
		yggr_test_assert(0 == nsv.find_first_not_of(L"c", 0));
#endif // YGGR_NO_CWCHAR
	}

	std::cout << "--------------test_operations_find_first_not_of end-----------------" << std::endl;
}

void test_operations_find_last_not_of(void)
{
	std::cout << "--------------test_operations_find_last_not_of begin-----------------" << std::endl;
	{
		yggr::charset::utf8_string org("abcde");
		yggr::charset::utf8_string_view nsv(org);
		std::cout << nsv << std::endl;

		// inline size_type find_last_not_of(const this_type& right, size_type pos = 0) const YGGR_NOEXCEPT_OR_NOTHROW
		yggr_test_assert(3 == nsv.find_last_not_of("ace"));
		yggr_test_assert(3 == nsv.find_last_not_of("ce"));

		yggr_test_assert(3 == nsv.find_last_not_of("ace", YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(3 == nsv.find_last_not_of("ce", YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(3 == nsv.find_last_not_of(yggr::charset::make_string_charset_helper("ace", yggr::charset::charset_name_t<char>())));
		yggr_test_assert(3 == nsv.find_last_not_of(yggr::charset::make_string_charset_helper("ce", yggr::charset::charset_name_t<char>())));

		yggr_test_assert(nsv.size() - 1 == nsv.find_last_not_of("fgh"));
		yggr_test_assert(1 == nsv.find_last_not_of("ce", 1));

		// inline size_type find_last_not_of(value_type c, size_type pos = 0) const YGGR_NOEXCEPT_OR_NOTHROW
		yggr_test_assert(nsv.size() - 1 == nsv.find_last_not_of(nsv.front()));
		yggr_test_assert(nsv.size() - 2 == nsv.find_last_not_of(nsv.back()));

		yggr_test_assert(nsv.size() - 1 == nsv.find_last_not_of(nsv.front(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(nsv.size() - 2 == nsv.find_last_not_of(nsv.back(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(nsv.size() - 1 == nsv.find_last_not_of(
									yggr::charset::make_string_charset_helper(
										nsv.front(), YGGR_STR_UTF8_STRING_CHARSET_NAME())));
		yggr_test_assert(nsv.size() - 2 == nsv.find_last_not_of(
									yggr::charset::make_string_charset_helper(
										nsv.back(), YGGR_STR_UTF8_STRING_CHARSET_NAME())));

		yggr_test_assert(1 == nsv.find_last_not_of(nsv.front(), 1));
		yggr_test_assert(1 == nsv.find_last_not_of(nsv.back(), 1));

		// inline size_type find_last_not_of(const_pointer s, size_type pos, size_type n) const
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_not_of(nsv.substr().data(), 0, nsv.substr().size()));
		yggr_test_assert(0 == nsv.find_last_not_of(nsv.substr(1).data(), 0, nsv.substr(1).size()));

		yggr_test_assert(yggr::charset::string_view::npos == 
				nsv.find_last_not_of(
					nsv.substr().data(), 0, nsv.substr().size(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(0 == nsv.find_last_not_of(
						nsv.substr(1).data(), 0, nsv.substr(1).size(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(yggr::charset::string_view::npos == 
				nsv.find_last_not_of(
					yggr::charset::make_string_charset_helper(
						nsv.substr().data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 
					0, nsv.substr().size()));
		yggr_test_assert(0 == nsv.find_last_not_of(
						yggr::charset::make_string_charset_helper(
							nsv.substr(1).data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()),
						0, nsv.substr(1).size()));

		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_not_of(nsv.substr().data(), 1, nsv.substr().size()));
		yggr_test_assert(0 == nsv.find_last_not_of(nsv.substr(1).data(), 1, nsv.substr(1).size()));
		yggr_test_assert(0 == nsv.find_last_not_of("z", 0, 1));
		yggr_test_assert(0 == nsv.find_last_not_of("c", 0, 1));
		yggr_test_assert(0 == nsv.find_last_not_of("", 0, 0));

		// inline size_type find_last_not_of(const_pointer s, size_type pos = 0) const
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_not_of(nsv.substr().data(), 0));
		yggr_test_assert(0 == nsv.find_last_not_of(nsv.substr(1).data(), 0));

		yggr_test_assert(yggr::charset::string_view::npos == 
				nsv.find_last_not_of(
					nsv.substr().data(), 0, YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(0 == nsv.find_last_not_of(
						nsv.substr(1).data(), 0, YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(yggr::charset::string_view::npos == 
				nsv.find_last_not_of(
					yggr::charset::make_string_charset_helper(
						nsv.substr().data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 
					0));
		yggr_test_assert(0 == nsv.find_last_not_of(
						yggr::charset::make_string_charset_helper(
							nsv.substr(1).data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 
						0));

		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_not_of(nsv.substr().data(), 1));
		yggr_test_assert(0 == nsv.find_last_not_of(nsv.substr(1).data(), 1));
		yggr_test_assert(0 == nsv.find_last_not_of("z", 0));
		yggr_test_assert(0 == nsv.find_last_not_of("c", 0));
	}

	{
		yggr::charset::utf8_string org("ÄãabcdeºÃ");
		yggr::charset::utf8_string_view nsv(org);
		std::cout << nsv << std::endl;

		// inline size_type find_last_not_of(const this_type& right, size_type pos = 0) const YGGR_NOEXCEPT_OR_NOTHROW
		yggr_test_assert(6 == nsv.find_last_not_of("ace"));
		yggr_test_assert(6 == nsv.find_last_not_of("ce"));

		yggr_test_assert(6 == nsv.find_last_not_of("ace", yggr::charset::charset_name_t<char>()));
		yggr_test_assert(6 == nsv.find_last_not_of("ce", yggr::charset::charset_name_t<char>()));

		yggr_test_assert(6 == nsv.find_last_not_of(
						yggr::charset::make_string_charset_helper(
							"ace", yggr::charset::charset_name_t<char>())));
		yggr_test_assert(6 == nsv.find_last_not_of(
						yggr::charset::make_string_charset_helper(
							"ce", yggr::charset::charset_name_t<char>())));

#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(6 == nsv.find_last_not_of(L"ace", yggr::charset::charset_name_t<wchar_t>()));
		yggr_test_assert(6 == nsv.find_last_not_of(L"ce", yggr::charset::charset_name_t<wchar_t>()));

		yggr_test_assert(6 == nsv.find_last_not_of(
						yggr::charset::make_string_charset_helper(
							L"ace", yggr::charset::charset_name_t<wchar_t>())));
		yggr_test_assert(6 == nsv.find_last_not_of(
						yggr::charset::make_string_charset_helper(
							L"ce", yggr::charset::charset_name_t<wchar_t>())));
#endif // YGGR_NO_CWCHAR

		yggr_test_assert(nsv.length() - 1 == nsv.find_last_not_of("fgh"));
		yggr_test_assert(1 == nsv.find_last_not_of("ce", 1));

		// inline size_type find_last_not_of(value_type c, size_type pos = 0) const YGGR_NOEXCEPT_OR_NOTHROW
		yggr_test_assert(nsv.length() - 1 == nsv.find_last_not_of('a'));
		yggr_test_assert(nsv.length() - 1 == nsv.find_last_not_of('e'));

		yggr_test_assert(nsv.length() - 1 == nsv.find_last_not_of('a', yggr::charset::charset_name_t<char>()));
		yggr_test_assert(nsv.length() - 1 == nsv.find_last_not_of('e', yggr::charset::charset_name_t<char>()));

		yggr_test_assert(nsv.length() - 1 == nsv.find_last_not_of(
									yggr::charset::make_string_charset_helper(
										'a', yggr::charset::charset_name_t<char>())));
		yggr_test_assert(nsv.length() - 1 == nsv.find_last_not_of(
									yggr::charset::make_string_charset_helper(
										'e', yggr::charset::charset_name_t<char>())));

#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(nsv.length() - 1 == nsv.find_last_not_of('a', yggr::charset::charset_name_t<wchar_t>()));
		yggr_test_assert(nsv.length() - 1 == nsv.find_last_not_of('e', yggr::charset::charset_name_t<wchar_t>()));

		yggr_test_assert(nsv.length() - 1 == nsv.find_last_not_of(
									yggr::charset::make_string_charset_helper(
										'a', yggr::charset::charset_name_t<wchar_t>())));
		yggr_test_assert(nsv.length() - 1 == nsv.find_last_not_of(
									yggr::charset::make_string_charset_helper(
										'e', yggr::charset::charset_name_t<wchar_t>())));
#endif // YGGR_NO_CWCHAR

		yggr_test_assert(nsv.length() - 1 == nsv.find_last_not_of(nsv.front()));
		yggr_test_assert(nsv.length() - 2 == nsv.find_last_not_of(nsv.back()));

		yggr_test_assert(nsv.length() - 1 == nsv.find_last_not_of(nsv.front(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(nsv.length() - 2 == nsv.find_last_not_of(nsv.back(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(nsv.length() - 1 == nsv.find_last_not_of(
									yggr::charset::make_string_charset_helper(
										nsv.front(), YGGR_STR_UTF8_STRING_CHARSET_NAME())));
		yggr_test_assert(nsv.length() - 2 == nsv.find_last_not_of(
									yggr::charset::make_string_charset_helper(
										nsv.back(), YGGR_STR_UTF8_STRING_CHARSET_NAME())));

		yggr_test_assert(1 == nsv.find_last_not_of(nsv.front(), 1));
		yggr_test_assert(1 == nsv.find_last_not_of(nsv.back(), 1));

		// inline size_type find_last_not_of(const_pointer s, size_type pos, size_type n) const

		if(yggr::charset::charset_name_t<char>() != yggr::charset::charset_name_t<yggr::utf8_string>())
		{
			try
			{
				nsv.find_last_not_of(nsv.substr().data(), 0, nsv.substr().size());
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}

			try
			{
				nsv.find_last_not_of(nsv.substr(1).data(), 0, nsv.substr(1).size());
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
		else
		{
			yggr_test_assert(yggr::charset::string_view::npos == 
				nsv.find_last_not_of(nsv.substr().data(), 0, nsv.substr().size()));
			yggr_test_assert(0 == nsv.find_last_not_of(nsv.substr(1).data(), 0, nsv.substr(1).size()));
		}

		yggr_test_assert(yggr::charset::string_view::npos == 
				nsv.find_last_not_of(
					nsv.substr().data(), 0, nsv.substr().size(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(0 == nsv.find_last_not_of(
						nsv.substr(1).data(), 0, nsv.substr(1).size(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(yggr::charset::string_view::npos == 
				nsv.find_last_not_of(
					yggr::charset::make_string_charset_helper(
						nsv.substr().data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 
					0, nsv.substr().size()));
		yggr_test_assert(0 == nsv.find_last_not_of(
						yggr::charset::make_string_charset_helper(
							nsv.substr(1).data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()),
						0, nsv.substr(1).size()));
		
		if(yggr::charset::charset_name_t<char>() != yggr::charset::charset_name_t<yggr::utf8_string>())
		{
			try
			{
				nsv.find_last_not_of(nsv.substr().data(), 1, nsv.substr().size());
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}

			try
			{
				nsv.find_last_not_of(nsv.substr(1).data(), 1, nsv.substr(1).size());
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
		else
		{
			yggr_test_assert(yggr::charset::string_view::npos == 
				nsv.find_last_not_of(nsv.substr().data(), 1, nsv.substr().size()));
			yggr_test_assert(0 == nsv.find_last_not_of(nsv.substr(1).data(), 1, nsv.substr(1).size()));
		}

		yggr_test_assert(yggr::charset::string_view::npos == 
				nsv.find_last_not_of(nsv.substr().data(), 1, nsv.substr().size(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(0 == nsv.find_last_not_of(nsv.substr(1).data(), 1, nsv.substr(1).size(), YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(0 == nsv.find_last_not_of("z", 0, 1));
		yggr_test_assert(0 == nsv.find_last_not_of("c", 0, 1));
		yggr_test_assert(0 == nsv.find_last_not_of("", 0, 0));

		// inline size_type find_last_not_of(const_pointer s, size_type pos = 0) const
		if(yggr::charset::charset_name_t<char>() != yggr::charset::charset_name_t<yggr::utf8_string>())
		{
			try
			{
				nsv.find_last_not_of(nsv.substr().data(), 0);
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}

			try
			{
				nsv.find_last_not_of(nsv.substr(1).data(), 0);
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
		else
		{
			yggr_test_assert(yggr::charset::string_view::npos == 
				nsv.find_last_not_of(
					nsv.substr().data(), 0));
			yggr_test_assert(0 == nsv.find_last_not_of(
						nsv.substr(1).data(), 0));
		}

		yggr_test_assert(yggr::charset::string_view::npos == 
				nsv.find_last_not_of(
					nsv.substr().data(), 0, YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(0 == nsv.find_last_not_of(
						nsv.substr(1).data(), 0, YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(yggr::charset::string_view::npos == 
				nsv.find_last_not_of(
					yggr::charset::make_string_charset_helper(
						nsv.substr().data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 
					0));
		yggr_test_assert(0 == nsv.find_last_not_of(
						yggr::charset::make_string_charset_helper(
							nsv.substr(1).data(), YGGR_STR_UTF8_STRING_CHARSET_NAME()), 
						0));

		if(yggr::charset::charset_name_t<char>() != yggr::charset::charset_name_t<yggr::utf8_string>())
		{
			try
			{
				nsv.find_last_not_of(nsv.substr().data(), 1);
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}

			try
			{
				nsv.find_last_not_of(nsv.substr(1).data(), 1);
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
		else
		{
			yggr_test_assert(yggr::charset::string_view::npos == 
				nsv.find_last_not_of(nsv.substr().data(), 1));
			yggr_test_assert(0 == nsv.find_last_not_of(nsv.substr(1).data(), 1));
		}

		yggr_test_assert(yggr::charset::string_view::npos == 
				nsv.find_last_not_of(nsv.substr().data(), 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(0 == nsv.find_last_not_of(nsv.substr(1).data(), 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(0 == nsv.find_last_not_of("z", 0));
		yggr_test_assert(0 == nsv.find_last_not_of("c", 0));
	}

	std::cout << "--------------test_operations_find_last_not_of end-----------------" << std::endl;
}

void test_operations(void)
{
	std::cout << "--------------test_operations begin-----------------" << std::endl;

	test_operations_copy();
	test_operations_substr();
	test_operations_compare();
	test_operations_starts_ends_with();
	test_operations_contains();
	test_operations_find();
	test_operations_rfind();
	test_operations_find_first_of();
	test_operations_find_last_of();
	test_operations_find_first_not_of();
	test_operations_find_last_not_of();

	std::cout << "--------------test_operations end-----------------" << std::endl;
}

void test_hash(void)
{
	std::cout << "--------------test_hash begin-----------------" << std::endl;

	{
		yggr::charset::string_view nsv;
		std::cout << nsv.hash() << std::endl;
	}

	{
		yggr::charset::string_view nsv("abcde");
		std::cout << nsv.hash() << std::endl;
	}

	std::cout << "--------------test_hash end-----------------" << std::endl;
}

void test_str(void)
{
	std::cout << "--------------test_str begin-----------------" << std::endl;
	{
		yggr::charset::utf8_string org("abcde");
		yggr::charset::utf8_string_view nsv(org);
		std::cout << nsv << std::endl;

		yggr_test_assert(nsv.str() == nsv);
		yggr_test_assert(nsv.str(YGGR_STR_UTF8_STRING_CHARSET_NAME()) == nsv);

		yggr_test_assert(nsv.str<yggr::charset::utf8_string_view>() == nsv);
		yggr_test_assert(nsv.str<yggr::charset::utf8_string_view>(YGGR_STR_UTF8_STRING_CHARSET_NAME()) == nsv);

		yggr_test_assert(nsv.str<yggr::charset::string_view>().data() == org);
		yggr_test_assert(nsv.str<yggr::charset::string_view>(YGGR_STR_UTF8_STRING_CHARSET_NAME()).data() == org);

		yggr_test_assert(nsv.str<yggr::utf8_string>() == org);
		yggr_test_assert(nsv.str<yggr::utf8_string>(YGGR_STR_UTF8_STRING_CHARSET_NAME()) == org);

		yggr_test_assert(nsv.str<yggr::utf8_string>(org.get_allocator()) == org);
		yggr_test_assert(nsv.str<yggr::utf8_string>(org.get_allocator(), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == org);

		yggr_test_assert(nsv.str<yggr::string>() == org.str<yggr::string>());
		yggr_test_assert(nsv.str<yggr::utf8_string>(YGGR_STR_UTF8_STRING_CHARSET_NAME()) == org);

		yggr_test_assert(nsv.str<yggr::string>(org.get_allocator()) == org.str<yggr::string>());
		yggr_test_assert(nsv.str<yggr::utf8_string>(org.get_allocator(), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == org);
	}

	{
		yggr::charset::utf8_string org("ÄãabºÃcdeÂð");
		yggr::charset::utf8_string_view nsv(org);
		std::cout << nsv << std::endl;

		yggr_test_assert(nsv.str() == nsv);
		yggr_test_assert(nsv.str(YGGR_STR_UTF8_STRING_CHARSET_NAME()) == nsv);

		yggr_test_assert(nsv.str<yggr::charset::utf8_string_view>() == nsv);
		yggr_test_assert(nsv.str<yggr::charset::utf8_string_view>(YGGR_STR_UTF8_STRING_CHARSET_NAME()) == nsv);

		if(yggr::charset::charset_name_t<char>() != yggr::charset::charset_name_t<yggr::utf8_string>())
		{
			try
			{
				bool b = nsv.str<yggr::charset::string_view>().data() == org;
				assert(b);
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}

			try
			{
				bool b = nsv.str<yggr::charset::string_view>(YGGR_STR_UTF8_STRING_CHARSET_NAME()).data() == org;
				assert(b);
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
		else
		{
			yggr_test_assert(
				yggr::charset::make_string_charset_helper(
					nsv.str<yggr::charset::string_view>().data())
				== org);
			yggr_test_assert(
				yggr::charset::make_string_charset_helper(
					nsv.str<yggr::charset::string_view>(YGGR_STR_UTF8_STRING_CHARSET_NAME()).data())
				== org);
		}

		yggr_test_assert(
			yggr::charset::make_string_charset_helper(
				nsv.str<yggr::charset::string_view>().data(),
				YGGR_STR_UTF8_STRING_CHARSET_NAME())
			== org);
		yggr_test_assert(
			yggr::charset::make_string_charset_helper(
				nsv.str<yggr::charset::string_view>(YGGR_STR_UTF8_STRING_CHARSET_NAME()).data(),
				YGGR_STR_UTF8_STRING_CHARSET_NAME())
			== org);

		yggr_test_assert(nsv.str<yggr::utf8_string>() == org);
		yggr_test_assert(nsv.str<yggr::utf8_string>(YGGR_STR_UTF8_STRING_CHARSET_NAME()) == org);

		yggr_test_assert(nsv.str<yggr::utf8_string>(org.get_allocator()) == org);
		yggr_test_assert(nsv.str<yggr::utf8_string>(org.get_allocator(), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == org);

		yggr_test_assert(nsv.str<yggr::string>() == org.str<yggr::string>());
		
		if(yggr::charset::charset_name_t<char>() != yggr::charset::charset_name_t<yggr::utf8_string>())
		{
			try
			{
				bool b = nsv.str<yggr::string>(YGGR_STR_UTF8_STRING_CHARSET_NAME()) == org;
				assert(b);
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
		else
		{
			yggr_test_assert(
				yggr::charset::make_string_charset_helper(
					nsv.str<yggr::string>(YGGR_STR_UTF8_STRING_CHARSET_NAME()))
				== org);
		}

		yggr_test_assert(
			yggr::charset::make_string_charset_helper(
				nsv.str<yggr::string>(YGGR_STR_UTF8_STRING_CHARSET_NAME()),
				YGGR_STR_UTF8_STRING_CHARSET_NAME())
			== org);

		yggr_test_assert(nsv.str<yggr::string>(org.get_allocator()) == org.str<yggr::string>());
		yggr_test_assert(nsv.str<yggr::utf8_string>(org.get_allocator(), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == org);
	}

	std::cout << "--------------test_str end-----------------" << std::endl;
}

void test_other_foo(void)
{
	std::cout << "--------------test_other_foo begin-----------------" << std::endl;

	{
		yggr::charset::utf8_string org("abcde");
		yggr::charset::utf8_string_view nsv(org);

		yggr::charset::utf8_string_view::base_type& bnsv = nsv;
		yggr_test_assert(yggr::charset::make_string_charset_helper(bnsv, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == nsv);

		const yggr::charset::utf8_string_view::base_type& cbnsv = nsv;
		yggr_test_assert(yggr::charset::make_string_charset_helper(cbnsv, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == nsv);

		yggr::charset::utf8_string_view::base_type& bnsv2 = nsv.org_str();
		yggr_test_assert(yggr::charset::make_string_charset_helper(bnsv2, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == nsv);

		const yggr::charset::utf8_string_view::base_type& cbnsv2 = nsv.org_str();
		yggr_test_assert(yggr::charset::make_string_charset_helper(cbnsv2, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == nsv);

		yggr::charset::utf8_string_view::base_type& bnsv3 = nsv.org_view();
		yggr_test_assert(yggr::charset::make_string_charset_helper(bnsv3, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == nsv);

		const yggr::charset::utf8_string_view::base_type& cbnsv3 = nsv.org_view();
		yggr_test_assert(yggr::charset::make_string_charset_helper(cbnsv3, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == nsv);
	}
	
	std::cout << "--------------test_other_foo end-----------------" << std::endl;
}

void test_other(void)
{
	std::cout << "--------------test_operations begin-----------------" << std::endl;

	test_hash();
	test_str();
	test_other_foo();

	std::cout << "--------------test_operations end-----------------" << std::endl;
}


int main(int argc, char* argv[])
{
	test_construct();
	test_set_and_swap();
	test_iterators();
	test_element_access();
	test_capacity();
	test_operations();
	test_other();

	wait_any_key(argc, argv);
	return 0;
}