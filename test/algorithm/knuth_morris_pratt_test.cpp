//knuth_morris_pratt_test.cpp

#define YGGR_ALGORITHM_DEBUG
#define YGGR_ALGORITHM_KNUTH_MORRIS_PRATT_DEBUG

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/charset/string.hpp>
#include <yggr/container/list.hpp>
#include <yggr/container/vector.hpp>
#include <yggr/algorithm/knuth_morris_pratt.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

void kmp_test(void)
{
	std::string haystack( "ABACAB is it everywhere!" );
	std::string needle1( "ACAB" );
	std::string needle2( "not ABA" );

	{
		yggr::knuth_morris_pratt<std::string::iterator> kmp(needle1.begin(), needle1.end());
		if(kmp(haystack.begin(), haystack.end()).first != haystack.end())
		{
			std::cout << "Found '" << needle1 << "'  in '" << haystack << "' (knuth_morris_pratt)" << std::endl;
		}
		else
		{
			yggr_test_assert(false);
			std::cout << "Did NOT find '" << needle1 << "'  in '" << haystack << "' (knuth_morris_pratt)" << std::endl;
		}
	}

	{
		if(yggr::knuth_morris_pratt_search(haystack.begin(), haystack.end(), needle1.begin(), needle1.end()).first != haystack.end())
		{
			std::cout << "Found '" << needle1 << "'  in '" << haystack << "' (knuth_morris_pratt)" << std::endl;
		}
		else
		{
			yggr_test_assert(false);
			std::cout << "Did NOT find '" << needle1 << "'  in '" << haystack << "' (knuth_morris_pratt)" << std::endl;
		}
	}

	{
		typedef std::pair<std::string::iterator, std::string::iterator> rst_type;
		typedef yggr::knuth_morris_pratt<std::string::iterator> kmp_type;

		kmp_type kmp(needle1.begin(), needle1.end());
		rst_type rst = yggr::knuth_morris_pratt_search(haystack.begin(), haystack.end(), kmp);

		if(rst.first != haystack.end())
		{
			std::cout << "Found '" << needle1 << "'  in '" << haystack << "' (knuth_morris_pratt)" << std::endl;
		}
		else
		{
			yggr_test_assert(false);
			std::cout << "Did NOT find '" << needle1 << "'  in '" << haystack << "' (knuth_morris_pratt)" << std::endl;
		}

		yggr_test_assert(rst.second != haystack.end());
		yggr_test_assert(*rst.second == ' ');

		rst = yggr::knuth_morris_pratt_search(rst.second, haystack.end(), kmp);

		if(rst.first != haystack.end())
		{
			yggr_test_assert(false);
			std::cout << "Found '" << needle1 << "'  in '" << haystack << "' (knuth_morris_pratt)" << std::endl;
		}
		else
		{
			std::cout << "Did NOT find '" << needle1 << "'  in '" << haystack << "' (knuth_morris_pratt)" << std::endl;
		}
	}
}

void kmp_test2(void)
{
	yggr::string str_haystack( "ABACAB is it everywhere!" );
	yggr::string str_needle1( "ACAB" );
	yggr::string str_needle2( "not ABA" );

	std::vector<char> haystack(str_haystack.begin(), str_haystack.end());
	std::list<char> needle1(str_needle1.begin(), str_needle1.end());
	std::list<char> needle2(str_needle2.begin(), str_needle2.end());

	{
		yggr::knuth_morris_pratt<std::list<char>::iterator> kmp(needle1.begin(), needle1.end());
		if(kmp(haystack.begin(), haystack.end()).first != haystack.end())
		{
			std::cout << "Found '" << str_needle1 << "'  in '" << str_haystack << "' (knuth_morris_pratt)" << std::endl;
		}
		else
		{
			yggr_test_assert(false);
			std::cout << "Did NOT find '" << str_needle1 << "'  in '" << str_haystack << "' (knuth_morris_pratt)" << std::endl;
		}
	}

	{
		if(yggr::knuth_morris_pratt_search(haystack.begin(), haystack.end(), 
											str_needle1.c_str(), str_needle1.c_str() + str_needle1.size()).first != haystack.end())
		{
			std::cout << "Found '" << str_needle1 << "'  in '" << str_haystack << "' (knuth_morris_pratt)" << std::endl;
		}
		else
		{
			yggr_test_assert(false);
			std::cout << "Did NOT find '" << str_needle1 << "'  in '" << str_haystack << "' (knuth_morris_pratt)" << std::endl;
		}
	}

	{
		typedef std::pair<std::vector<char>::iterator, std::vector<char>::iterator> rst_type;
		typedef yggr::knuth_morris_pratt<std::list<char>::iterator> kmp_type;

		kmp_type kmp(needle1.begin(), needle1.end());
		rst_type rst = yggr::knuth_morris_pratt_search(haystack.begin(), haystack.end(), kmp);

		if(rst.first != haystack.end())
		{
			std::cout << "Found '" << str_needle1 << "'  in '" << str_haystack << "' (knuth_morris_pratt)" << std::endl;
		}
		else
		{
			yggr_test_assert(false);
			std::cout << "Did NOT find '" << str_needle1 << "'  in '" << str_haystack << "' (knuth_morris_pratt)" << std::endl;
		}

		yggr_test_assert(rst.second != haystack.end());
		yggr_test_assert(*rst.second == ' ');

		rst = yggr::knuth_morris_pratt_search(rst.second, haystack.end(), kmp);

		if(rst.first != haystack.end())
		{
			yggr_test_assert(false);
			std::cout << "Found '" << str_needle1 << "'  in '" << str_haystack << "' (knuth_morris_pratt)" << std::endl;
		}
		else
		{
			std::cout << "Did NOT find '" << str_needle1 << "'  in '" << str_haystack << "' (knuth_morris_pratt)" << std::endl;
		}
	}
}

int main(int argc, char* argv[])
{
	kmp_test();
	kmp_test2();

	wait_any_key(argc, argv);
	return 0;
}