// boyer_moore_horspool_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/charset/string.hpp>
#include <yggr/algorithm/boyer_moore_horspool.hpp>

#include <yggr/container/list.hpp>
#include <yggr/container/vector.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

void bmh_test1(void)
{
	std::string haystack("ABACAB is it everywhere!");
	std::string needle1("ACAB");
	std::string needle2("not ABA");


	{
		yggr::boyer_moore_horspool<std::string::iterator> bmh(needle1.begin(), needle1.end());

		if(bmh(haystack.begin(), haystack.end()).first != haystack.end())
		{
			std::cout << "Found '" << needle1 << "'  in '" << haystack << "' (boyer-moore-horspool)" << std::endl;
		}
		else
		{
			yggr_test_assert(false);
			std::cout << "Did NOT find '" << needle1 << "'  in '" << haystack << "' (boyer-moore-horspool)" << std::endl;
		}
	}

	{
		yggr::boyer_moore_horspool<std::string::iterator> bmh(needle2.begin(), needle2.end());

		if(bmh(haystack.begin(), haystack.end()).first != haystack.end())
		{
			yggr_test_assert(false);
			std::cout << "Found '" << needle2 << "'  in '" << haystack << "' (boyer-moore-horspool)" << std::endl;
		}
		else
		{
			std::cout << "Did NOT find '" << needle2 << "'  in '" << haystack << "' (boyer-moore-horspool)" << std::endl;
		}
	}

	{
		yggr::list<char> list_haystack(haystack.begin(), haystack.end());
		yggr::vector<char> vt_needle1(needle1.begin(), needle1.end());

		yggr::boyer_moore_horspool<yggr::vector<char>::iterator> bmh(vt_needle1.begin(), vt_needle1.end());

		if(bmh(list_haystack.begin(), list_haystack.end()).first != list_haystack.end())
		{
			std::cout << "Found '" << needle1 << "'  in '" << haystack << "' (boyer-moore-horspool)" << std::endl;
		}
		else
		{
			yggr_test_assert(false);
			std::cout << "Did NOT find '" << needle1 << "'  in '" << haystack << "' (boyer-moore-horspool)" << std::endl;
		}
	}

}

void bmh_test2(void)
{
	yggr::string haystack( "ABACAB is it everywhere!" );
	yggr::string needle1( "ACAB" );
	yggr::string needle2( "not ABA" );

	{
		yggr::boyer_moore_horspool<yggr::string::iterator> bmh(needle1.begin(), needle1.end());
		if(bmh(haystack.begin(), haystack.end()).first != haystack.end())
		{
			std::cout << "Found '" << needle1 << "'  in '" << haystack << "' (boyer_moore_horspool)" << std::endl;
		}
		else
		{
			yggr_test_assert(false);
			std::cout << "Did NOT find '" << needle1 << "'  in '" << haystack << "' (boyer_moore_horspool)" << std::endl;
		}
	}

	{
		if(yggr::boyer_moore_horspool_search(haystack.begin(), haystack.end(), needle1.begin(), needle1.end()).first != haystack.end())
		{
			std::cout << "Found '" << needle1 << "'  in '" << haystack << "' (boyer_moore_horspool)" << std::endl;
		}
		else
		{
			yggr_test_assert(false);
			std::cout << "Did NOT find '" << needle1 << "'  in '" << haystack << "' (boyer_moore_horspool)" << std::endl;
		}
	}

	{
		typedef std::pair<yggr::string::iterator, yggr::string::iterator> rst_type;
		typedef yggr::boyer_moore_horspool<yggr::string::iterator> bm_type;

		bm_type bmh(needle1.begin(), needle1.end());
		rst_type rst = yggr::boyer_moore_horspool_search(haystack.begin(), haystack.end(), bmh);

		if(rst.first != haystack.end())
		{
			std::cout << "Found '" << needle1 << "'  in '" << haystack << "' (boyer_moore_horspool)" << std::endl;
		}
		else
		{
			yggr_test_assert(false);
			std::cout << "Did NOT find '" << needle1 << "'  in '" << haystack << "' (boyer_moore_horspool)" << std::endl;
		}

		yggr_test_assert(rst.second != haystack.end());
		yggr_test_assert(*rst.second == ' ');
		yggr::string tmp_haystack(rst.second, haystack.end());

		rst = yggr::boyer_moore_horspool_search(rst.second, haystack.end(), bmh);

		if(rst.first != haystack.end())
		{
			yggr_test_assert(false);
			std::cout << "Found '" << needle1 << "'  in '" << tmp_haystack << "' (boyer_moore_horspool)" << std::endl;
		}
		else
		{
			std::cout << "Did NOT find '" << needle1 << "'  in '" << tmp_haystack << "' (boyer_moore_horspool)" << std::endl;
		}
	}
}

void bmh_test3(void)
{
	std::string str_haystack( "ABACAB is it everywhere!" );
	std::string str_needle1( "ACAB" );
	std::string str_needle2( "not ABA" );

	std::vector<char> haystack(str_haystack.begin(), str_haystack.end());
	std::list<char> needle1(str_needle1.begin(), str_needle1.end());
	std::list<char> needle2(str_needle2.begin(), str_needle2.end());

	{
		yggr::boyer_moore_horspool<std::list<char>::iterator> bmh(needle1.begin(), needle1.end());
		if(bmh(haystack.begin(), haystack.end()).first != haystack.end())
		{
			std::cout << "Found '" << str_needle1 << "'  in '" << str_haystack << "' (boyer_moore_horspool)" << std::endl;
		}
		else
		{
			yggr_test_assert(false);
			std::cout << "Did NOT find '" << str_needle1 << "'  in '" << str_haystack << "' (boyer_moore_horspool)" << std::endl;
		}
	}

	{
		if(yggr::boyer_moore_horspool_search(haystack.begin(), haystack.end(), 
											str_needle1.c_str(), str_needle1.c_str() + str_needle1.size()).first != haystack.end())
		{
			std::cout << "Found '" << str_needle1 << "'  in '" << str_haystack << "' (boyer_moore_horspool)" << std::endl;
		}
		else
		{
			yggr_test_assert(false);
			std::cout << "Did NOT find '" << str_needle1 << "'  in '" << str_haystack << "' (boyer_moore_horspool)" << std::endl;
		}
	}

	{
		typedef std::pair<std::vector<char>::iterator, std::vector<char>::iterator> rst_type;
		typedef yggr::boyer_moore_horspool<std::list<char>::iterator> bm_type;

		bm_type bmh(needle1.begin(), needle1.end());
		rst_type rst = yggr::boyer_moore_horspool_search(haystack.begin(), haystack.end(), bmh);

		if(rst.first != haystack.end())
		{
			std::cout << "Found '" << str_needle1 << "'  in '" << str_haystack << "' (boyer_moore_horspool)" << std::endl;
		}
		else
		{
			yggr_test_assert(false);
			std::cout << "Did NOT find '" << str_needle1 << "'  in '" << str_haystack << "' (boyer_moore_horspool)" << std::endl;
		}

		yggr_test_assert(rst.second != haystack.end());
		yggr_test_assert(*rst.second == ' ');

		rst = yggr::boyer_moore_horspool_search(rst.second, haystack.end(), bmh);

		if(rst.first != haystack.end())
		{
			yggr_test_assert(false);
			std::cout << "Found '" << str_needle1 << "'  in '" << str_haystack << "' (boyer_moore_horspool)" << std::endl;
		}
		else
		{
			std::cout << "Did NOT find '" << str_needle1 << "'  in '" << str_haystack << "' (boyer_moore_horspool)" << std::endl;
		}
	}
}

int main(int argc, char* argv[])
{
	bmh_test1();
	bmh_test2();
	bmh_test3();

	wait_any_key(argc, argv);
	return 0;
}
