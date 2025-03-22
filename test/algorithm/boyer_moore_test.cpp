// boyer_moore_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/charset/string.hpp>
#include <yggr/algorithm/boyer_moore.hpp>

#include <yggr/algorithm/boyer_moore.hpp>

#include <yggr/container/list.hpp>
#include <yggr/container/vector.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

void bm_test1(void)
{
	std::string haystack("ABACAB is it everywhere!");
	std::string needle1("ACAB");
	std::string needle2("not ABA");


	{
		yggr::boyer_moore<std::string::iterator> bm(needle1.begin(), needle1.end());

		if(bm(haystack.begin(), haystack.end()).first != haystack.end())
		{
			std::cout << "Found '" << needle1 << "'  in '" << haystack << "' (boyer-moore 1)" << std::endl;
		}
		else
		{
			yggr_test_assert(false);
			std::cout << "Did NOT find '" << needle1 << "'  in '" << haystack << "' (boyer-moore 1)" << std::endl;
		}
	}

	{
		yggr::boyer_moore<std::string::iterator> bm(needle2.begin(), needle2.end());

		if(bm(haystack.begin(), haystack.end()).first != haystack.end())
		{
			yggr_test_assert(false);
			std::cout << "Found '" << needle1 << "'  in '" << haystack << "' (boyer-moore 1)" << std::endl;
		}
		else
		{
			std::cout << "Did NOT find '" << needle1 << "'  in '" << haystack << "' (boyer-moore 1)" << std::endl;
		}
	}

	{
		yggr::list<char> list_haystack(haystack.begin(), haystack.end());
		yggr::vector<char> vt_needle1(needle1.begin(), needle1.end());

		yggr::boyer_moore<yggr::vector<char>::iterator> bm(vt_needle1.begin(), vt_needle1.end());

		if(bm(list_haystack.begin(), list_haystack.end()).first != list_haystack.end())
		{
			std::cout << "Found '" << needle1 << "'  in '" << haystack << "' (boyer-moore 1)" << std::endl;
		}
		else
		{
			yggr_test_assert(false);
			std::cout << "Did NOT find '" << needle1 << "'  in '" << haystack << "' (boyer-moore 1)" << std::endl;
		}
	}

}

void bm_test2(void)
{
	yggr::string haystack( "ABACAB is it everywhere!" );
	yggr::string needle1( "ACAB" );
	yggr::string needle2( "not ABA" );

	{
		yggr::boyer_moore<yggr::string::iterator> bm(needle1.begin(), needle1.end());
		if(bm(haystack.begin(), haystack.end()).first != haystack.end())
		{
			std::cout << "Found '" << needle1 << "'  in '" << haystack << "' (boyer_moore)" << std::endl;
		}
		else
		{
			yggr_test_assert(false);
			std::cout << "Did NOT find '" << needle1 << "'  in '" << haystack << "' (boyer_moore)" << std::endl;
		}
	}

	{
		if(yggr::boyer_moore_search(haystack.begin(), haystack.end(), needle1.begin(), needle1.end()).first != haystack.end())
		{
			std::cout << "Found '" << needle1 << "'  in '" << haystack << "' (boyer_moore)" << std::endl;
		}
		else
		{
			yggr_test_assert(false);
			std::cout << "Did NOT find '" << needle1 << "'  in '" << haystack << "' (boyer_moore)" << std::endl;
		}
	}

	{
		typedef std::pair<yggr::string::iterator, yggr::string::iterator> rst_type;
		typedef yggr::boyer_moore<yggr::string::iterator> bm_type;

		bm_type bm(needle1.begin(), needle1.end());
		rst_type rst = yggr::boyer_moore_search(haystack.begin(), haystack.end(), bm);

		if(rst.first != haystack.end())
		{
			std::cout << "Found '" << needle1 << "'  in '" << haystack << "' (boyer_moore)" << std::endl;
		}
		else
		{
			yggr_test_assert(false);
			std::cout << "Did NOT find '" << needle1 << "'  in '" << haystack << "' (boyer_moore)" << std::endl;
		}

		yggr_test_assert(rst.second != haystack.end());
		yggr_test_assert(*rst.second == ' ');

		rst = yggr::boyer_moore_search(rst.second, haystack.end(), bm);

		if(rst.first != haystack.end())
		{
			yggr_test_assert(false);
			std::cout << "Found '" << needle1 << "'  in '" << haystack << "' (boyer_moore)" << std::endl;
		}
		else
		{
			std::cout << "Did NOT find '" << needle1 << "'  in '" << haystack << "' (boyer_moore)" << std::endl;
		}
	}
}

void bm_test3(void)
{
	std::string str_haystack( "ABACAB is it everywhere!" );
	std::string str_needle1( "ACAB" );
	std::string str_needle2( "not ABA" );

	std::vector<char> haystack(str_haystack.begin(), str_haystack.end());
	std::list<char> needle1(str_needle1.begin(), str_needle1.end());
	std::list<char> needle2(str_needle2.begin(), str_needle2.end());

	{
		yggr::boyer_moore<std::list<char>::iterator> bm(needle1.begin(), needle1.end());
		if(bm(haystack.begin(), haystack.end()).first != haystack.end())
		{
			std::cout << "Found '" << str_needle1 << "'  in '" << str_haystack << "' (boyer_moore)" << std::endl;
		}
		else
		{
			yggr_test_assert(false);
			std::cout << "Did NOT find '" << str_needle1 << "'  in '" << str_haystack << "' (boyer_moore)" << std::endl;
		}
	}

	{
		if(yggr::boyer_moore_search(haystack.begin(), haystack.end(), 
											str_needle1.c_str(), str_needle1.c_str() + str_needle1.size()).first != haystack.end())
		{
			std::cout << "Found '" << str_needle1 << "'  in '" << str_haystack << "' (boyer_moore)" << std::endl;
		}
		else
		{
			yggr_test_assert(false);
			std::cout << "Did NOT find '" << str_needle1 << "'  in '" << str_haystack << "' (boyer_moore)" << std::endl;
		}
	}

	{
		typedef std::pair<std::vector<char>::iterator, std::vector<char>::iterator> rst_type;
		typedef yggr::boyer_moore<std::list<char>::iterator> bm_type;

		bm_type bm(needle1.begin(), needle1.end());
		rst_type rst = yggr::boyer_moore_search(haystack.begin(), haystack.end(), bm);

		if(rst.first != haystack.end())
		{
			std::cout << "Found '" << str_needle1 << "'  in '" << str_haystack << "' (boyer_moore)" << std::endl;
		}
		else
		{
			yggr_test_assert(false);
			std::cout << "Did NOT find '" << str_needle1 << "'  in '" << str_haystack << "' (boyer_moore)" << std::endl;
		}

		yggr_test_assert(rst.second != haystack.end());
		yggr_test_assert(*rst.second == ' ');

		rst = yggr::boyer_moore_search(rst.second, haystack.end(), bm);

		if(rst.first != haystack.end())
		{
			yggr_test_assert(false);
			std::cout << "Found '" << str_needle1 << "'  in '" << str_haystack << "' (boyer_moore)" << std::endl;
		}
		else
		{
			std::cout << "Did NOT find '" << str_needle1 << "'  in '" << str_haystack << "' (boyer_moore)" << std::endl;
		}
	}
}

int main(int argc, char* argv[])
{
	bm_test1();
	bm_test2();
	bm_test3();

	wait_any_key(argc, argv);
	return 0;
}