//basic_string_adp_helper_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/charset/basic_string_adp_helper.hpp>

#include <boost/iterator/reverse_iterator.hpp>

#include <iterator>
#include <typeinfo>

#include <yggr/compile_link/linker.hpp>


#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(charset)

template<typename String>
void test_assign(void)
{
	typedef String string_type;
	typedef typename string_type::value_type value_type;

	// assign(n, char)
	{
		string_type str = "abc";
		yggr::charset::basic_string_assign(str, 2, str[0]);
		yggr_test_assert(str == "aa");
		yggr_test_assert(str.size() == 2);
	}

	// assign(const char*)
	{
		string_type str = "abc";
		yggr::charset::basic_string_assign(str, "");
		yggr_test_assert(str == "");
		yggr_test_assert(str.size() == 0);
	}

	{
		string_type str = "abc";
		str.push_back(0);
		yggr::charset::basic_string_assign(str, str.c_str() + 1);
		yggr_test_assert(str == "bc");
		yggr_test_assert(str.size() == 2);
	}

	// assign(const char*, n)
	{
		string_type str = "abc";
		yggr::charset::basic_string_assign(str, str.c_str(), 2);
		yggr_test_assert(str == "ab");
		yggr_test_assert(str.size() == 2);
	}

	{
		string_type str = "abc";
		yggr::charset::basic_string_assign(str, str.c_str(), 0);
		yggr_test_assert(str == "");
		yggr_test_assert(str.size() == 0);
	}

	// assign(const string&) and assign(const string&&)
	{
		string_type str = "abc";
		yggr::charset::basic_string_assign(str, str);
		yggr_test_assert(str == "abc");
		yggr_test_assert(str.size() == 3);
	}

	{
		string_type str = "abc";
		yggr::charset::basic_string_assign(str, boost::move(str));
		yggr_test_assert(str == "abc");
		yggr_test_assert(str.size() == 3);
	}

	// assign(const std::string&, subpos, sublen)

	{
		string_type str = "abc";
		yggr::charset::basic_string_assign(str, str, 0, 3);
		yggr_test_assert(str == "abc");
		yggr_test_assert(str.size() == 3);
	}

	// assign(iters, itere)
	{
		string_type str = "abc";
		yggr::charset::basic_string_assign(str, str.begin() + 1, str.end());
		yggr_test_assert(str == "bc");
		yggr_test_assert(str.size() == 2);
	}

	{
		string_type str = "abc";
		yggr::charset::basic_string_assign(str, str.c_str() + 1, str.c_str() + str.size());
		yggr_test_assert(str == "bc");
		yggr_test_assert(str.size() == 2);
	}

	{
		string_type str = "abc";
		yggr::charset::basic_string_assign(str, str.rbegin() + 1, str.rend());
		yggr_test_assert(str == "ba");
		yggr_test_assert(str.size() == 2);
	}

	{
		string_type str = "abc";
		yggr::charset::basic_string_assign(str, str.rbegin(), str.rend());
		yggr_test_assert(str == "cba");
		yggr_test_assert(str.size() == 3);
	}

	{
		string_type str = "abc";
		yggr::charset::basic_string_assign(str, std::reverse_iterator<const char*>(str.c_str() + 3),
												std::reverse_iterator<const char*>(str.c_str()));
		yggr_test_assert(str == "cba");
		yggr_test_assert(str.size() == 3);
	}

	{
		string_type str = "abc";
		yggr::charset::basic_string_assign(str, boost::reverse_iterator<const char*>(str.c_str() + 3),
												boost::reverse_iterator<const char*>(str.c_str()));
		yggr_test_assert(str == "cba");
		yggr_test_assert(str.size() == 3);
	}


	std::cout << "test_assign " << typeid(string_type).name() << " success" << std::endl;
}

template<typename String>
void test_insert(void)
{
	typedef String string_type;
	typedef typename string_type::value_type value_type;

	// insert(xxx, n, char)
	{
		string_type str = "abc";
		yggr::charset::basic_string_insert(str, 1, 3, str[0]);
		yggr_test_assert(str == "aaaabc");
		yggr_test_assert(str.size() == 6);
	}

	{
		string_type str = "abc";
		yggr::charset::basic_string_insert(str, 3, 3, str[0]);
		yggr_test_assert(str == "abcaaa");
		yggr_test_assert(str.size() == 6);
	}

	{
		string_type str = "abc";
		yggr::charset::basic_string_insert(str, str.begin() + 1, 3, str[0]);
		yggr_test_assert(str == "aaaabc");
		yggr_test_assert(str.size() == 6);
	}

	{
		string_type str = "abc";
		yggr::charset::basic_string_insert(str, str.end(), 3, str[0]);
		yggr_test_assert(str == "abcaaa");
		yggr_test_assert(str.size() == 6);
	}

	// insert(iterp, char)
	{
		string_type str = "abc";
		yggr::charset::basic_string_insert(str, str.begin() + 1, str[0]);
		yggr_test_assert(str == "aabc");
		yggr_test_assert(str.size() == 4);
	}

	{
		string_type str = "abc";
		yggr::charset::basic_string_insert(str, str.end(), str[0]);
		yggr_test_assert(str == "abca");
		yggr_test_assert(str.size() == 4);
	}

	// insert(pos, const char*)
	{
		string_type str = "abc";
		str[1] = 0;
		yggr::charset::basic_string_insert(str, 1, str.c_str());
		yggr_test_assert(str == string_type("aa\0c", 4));
		yggr_test_assert(str.size() == 4);
	}

	{
		string_type str = "abc";
		yggr::charset::basic_string_insert(str, 1, str.c_str());
		yggr_test_assert(str == "aabcbc");
		yggr_test_assert(str.size() == 6);
	}

	// insert(pos, const char*, n)
	{
		string_type str = "abc";
		str[1] = 0;
		yggr::charset::basic_string_insert(str, 1, str.c_str(), 2);
		yggr_test_assert(str == string_type("aa\0\0c", 5));
		yggr_test_assert(str.size() == 5);
	}

	{
		string_type str = "abc";
		yggr::charset::basic_string_insert(str, 1, str.c_str(), 2);
		yggr_test_assert(str == "aabbc");
		yggr_test_assert(str.size() == 5);
	}

	// insert(pos, const string&)
	{
		string_type str = "abc";
		str[1] = 0;
		yggr::charset::basic_string_insert(str, 1, str);
		yggr_test_assert(str == string_type("aa\0c\0c", 6));
		yggr_test_assert(str.size() == 6);
	}

	{
		string_type str = "abc";
		yggr::charset::basic_string_insert(str, 1, str);
		yggr_test_assert(str == "aabcbc");
		yggr_test_assert(str.size() == 6);
	}

	// insert(pos, const string&, subpos, sublen)
	{
		string_type str = "abc";
		str[1] = 0;
		yggr::charset::basic_string_insert(str, 1, str, 0, 3);
		yggr_test_assert(str == string_type("aa\0c\0c", 6));
		yggr_test_assert(str.size() == 6);
	}

	{
		string_type str = "abc";
		yggr::charset::basic_string_insert(str, 1, str, 1, 2);
		yggr_test_assert(str == "abcbc");
		yggr_test_assert(str.size() == 5);
	}

	// insert(iterp, iters, itere)
	{
		string_type str = "abc";
		yggr::charset::basic_string_insert(str, str.begin() + 1, str.rbegin(), str.rend());
		yggr_test_assert(str == string_type("acbabc", 6));
		yggr_test_assert(str.size() == 6);
	}

	{
		string_type str = "abc";
		yggr::charset::basic_string_insert(str, str.begin() + 1,
											std::reverse_iterator<const char*>(str.c_str() + 3),
											std::reverse_iterator<const char*>(str.c_str() + 0));
		yggr_test_assert(str == string_type("acbabc", 6));
		yggr_test_assert(str.size() == 6);
	}

	{
		string_type str = "abc";
		yggr::charset::basic_string_insert(str, str.begin() + 1,
											boost::reverse_iterator<const char*>(str.c_str() + 3),
											boost::reverse_iterator<const char*>(str.c_str() + 0));
		yggr_test_assert(str == string_type("acbabc", 6));
		yggr_test_assert(str.size() == 6);
	}

	std::cout << "test_insert " << typeid(string_type).name() << " success" << std::endl;
}


template<typename String>
void test_repace(void)
{
	typedef String string_type;
	typedef typename string_type::value_type value_type;

	//(xxx, xxx, n, char)
	{
		string_type str = "abc";
		yggr::charset::basic_string_replace(str, 1, 1, 3, 'd');
		yggr_test_assert(str == "adddc");
		yggr_test_assert(str.size() == 5);
	}

	{
		string_type str = "abc";
		yggr::charset::basic_string_replace(str, str.begin() + 1, str.begin() + 2, 3, 'd');
		yggr_test_assert(str == "adddc");
		yggr_test_assert(str.size() == 5);
	}

	//(xxx, xxx, const char*)
	{
		string_type str = "abc";
		yggr::charset::basic_string_replace(str, 1, 1, str.c_str());
		yggr_test_assert(str == "aabcc");
		yggr_test_assert(str.size() == 5);
	}

	{
		string_type str = "abc";
		str.push_back(0);
		yggr::charset::basic_string_replace(str, 0, str.size(), str.c_str());
		yggr_test_assert(str == "abc");
		yggr_test_assert(str.size() == 3);
	}

	{
		string_type str = "abc";
		yggr::charset::basic_string_replace(str, str.begin() + 1, str.begin() + 2, str.c_str());
		yggr_test_assert(str == "aabcc");
		yggr_test_assert(str.size() == 5);
	}

	{
		string_type str = "abc";
		str.push_back(0);
		yggr::charset::basic_string_replace(str, str.begin(), str.end(), str.c_str());
		yggr_test_assert(str == "abc");
		yggr_test_assert(str.size() == 3);
	}

	//(xxx, xxx, const char*, n)
	{
		string_type str = "abc";
		yggr::charset::basic_string_replace(str, 1, 1, str.c_str(), 3);
		yggr_test_assert(str == "aabcc");
		yggr_test_assert(str.size() == 5);
	}

	{
		string_type str = "abc";
		str.push_back(0);
		yggr::charset::basic_string_replace(str, 0, str.size(), str.c_str(), 3);
		yggr_test_assert(str == "abc");
		yggr_test_assert(str.size() == 3);
	}

	{
		string_type str = "abc";
		yggr::charset::basic_string_replace(str, str.begin() + 1, str.begin() + 2, str.c_str(), 3);
		yggr_test_assert(str == "aabcc");
		yggr_test_assert(str.size() == 5);
	}

	{
		string_type str = "abc";
		str.push_back(0);
		yggr::charset::basic_string_replace(str, str.begin(), str.end(), str.c_str(), 3);
		yggr_test_assert(str == "abc");
		yggr_test_assert(str.size() == 3);
	}

	//(xxx, xxx, string)
	{
		string_type str = "abc";
		yggr::charset::basic_string_replace(str, 1, 1, str);
		yggr_test_assert(str == "aabcc");
		yggr_test_assert(str.size() == 5);
	}

	{
		string_type str = "abc";
		str.push_back(0);
		yggr::charset::basic_string_replace(str, 0, str.size(), str);
		str.erase(str.size() - 1);
		yggr_test_assert(str == "abc");
		yggr_test_assert(str.size() == 3);
	}

	{
		string_type str = "abc";
		yggr::charset::basic_string_replace(str, str.begin() + 1, str.begin() + 2, str);
		yggr_test_assert(str == "aabcc");
		yggr_test_assert(str.size() == 5);
	}

	{
		string_type str = "abc";
		str.push_back(0);
		yggr::charset::basic_string_replace(str, str.begin(), str.end(), str);
		str.erase(str.size() - 1);
		yggr_test_assert(str == "abc");
		yggr_test_assert(str.size() == 3);
	}

	//(pos, len, string, subpos, sublen)
	{
		string_type str = "abc";
		yggr::charset::basic_string_replace(str, 1, 1, str, 0, 3);
		yggr_test_assert(str == "aabcc");
		yggr_test_assert(str.size() == 5);
	}

	{
		string_type str = "abc";
		str.push_back(0);
		yggr::charset::basic_string_replace(str, 0, str.size(), str, 0, 3);
		yggr_test_assert(str == "abc");
		yggr_test_assert(str.size() == 3);
	}

	//(i1, i2, s, e)
	{
		string_type str = "abc";
		yggr::charset::basic_string_replace(str, str.begin() + 1, str.begin() + 2, str.begin(), str.end());
		yggr_test_assert(str == "aabcc");
		yggr_test_assert(str.size() == 5);
	}

	{
		string_type str = "abc";
		str.push_back(0);
		yggr::charset::basic_string_replace(str, str.begin(), str.end(), str.begin(), str.begin() + 3);
		yggr_test_assert(str == "abc");
		yggr_test_assert(str.size() == 3);
	}

	{
		string_type str = "abc";
		yggr::charset::basic_string_replace(str, str.begin(), str.end(), str.rbegin(), str.rend());
		yggr_test_assert(str == "cba");
		yggr_test_assert(str.size() == 3);
	}

	{
		string_type str = "abcde";
		yggr::charset::basic_string_replace(str, str.begin(), str.end(), str.rbegin() + 1, str.rbegin() + (1 + 3));
		yggr_test_assert(str == "dcb");
		yggr_test_assert(str.size() == 3);
	}

	{
		string_type str = "abcde";
		yggr::charset::basic_string_replace(str, str.begin() + 1, str.begin() + (1 + 3), str.rbegin() + 1, str.rbegin() + (1 + 3));
		yggr_test_assert(str == "adcbe");
		yggr_test_assert(str.size() == 5);
	}

	{
		string_type str = "abcde";
		yggr::charset::basic_string_replace(str, str.begin() + 1, str.begin() + (1 + 3),
												std::reverse_iterator<const char*>(str.c_str() + (1 + 3)),
												std::reverse_iterator<const char*>(str.c_str() + 1));
		yggr_test_assert(str == "adcbe");
		yggr_test_assert(str.size() == 5);
	}

	{
		string_type str = "abcde";
		yggr::charset::basic_string_replace(str, str.begin() + 1, str.begin() + (1 + 3),
												boost::reverse_iterator<const char*>(str.c_str() + (1 + 3)),
												boost::reverse_iterator<const char*>(str.c_str() + 1));
		yggr_test_assert(str == "adcbe");
		yggr_test_assert(str.size() == 5);
	}

	std::cout << "test_repace " << typeid(string_type).name() << " success" << std::endl;
}

template<typename String>
void test_find(void)
{
	typedef String string_type;
	typedef typename string_type::value_type value_type;

	std::cout << "test_find " << typeid(string_type).name() << " success" << std::endl;
}

int main(int argc, char* argv[])
{

#if 1

	std::cout << "-------test_repace s---------" << std::endl;
	test_repace<std::string>();
	std::cout << "----------------" << std::endl;
	test_repace<boost::container::string>();
	std::cout << "-------test_repace e---------" << std::endl;

#endif // 0, 1

#if 1

	std::cout << "\n-------test_assign s---------" << std::endl;
	test_assign<std::string>();
	std::cout << "----------------" << std::endl;
	test_assign<boost::container::string>();
	std::cout << "-------test_assign e---------" << std::endl;

#endif // 0, 1

#if 1

	std::cout << "\n-------test_insert s---------" << std::endl;
	test_insert<std::string>();
	std::cout << "----------------" << std::endl;
	test_insert<boost::container::string>();
	std::cout << "-------test_insert e---------" << std::endl;

#endif // 0, 1

	wait_any_key(argc, argv);
	return 0;
}
