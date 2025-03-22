// boyer_moore_test.cpp

#if !defined(_MSC_VER)
#	error "this file supports MSVC only, if you are using other compilers, please use the same name file in the charset_utf8_files"	
#endif // _MSC_VER

//#define YGGR_NO_CWCHAR

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/boyer_moore.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

void bm_test(void)
{
	{
		yggr::utf8_string haystack( "ABACAB is it everywhere!" );
		yggr::utf8_string needle1( "ACAB" );
		yggr::utf8_string needle2( "not ABA" );

		{
			yggr::boyer_moore<yggr::utf8_string::iterator> bm(needle1.begin(), needle1.end());
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
			if(yggr::boyer_moore_search(haystack.cbegin(), haystack.cend(), needle1.cbegin(), needle1.cend()).first != haystack.cend())
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
			typedef std::pair<yggr::utf8_string::const_iterator, yggr::utf8_string::const_iterator> rst_type;
			typedef yggr::boyer_moore<yggr::utf8_string::iterator> bm_type;

			bm_type bm(needle1.begin(), needle1.end());
			rst_type rst = yggr::boyer_moore_search(haystack.cbegin(), haystack.cend(), bm);

			if(rst.first != haystack.cend())
			{
				std::cout << "Found '" << needle1 << "'  in '" << haystack << "' (boyer_moore)" << std::endl;
			}
			else
			{
				yggr_test_assert(false);
				std::cout << "Did NOT find '" << needle1 << "'  in '" << haystack << "' (boyer_moore)" << std::endl;
			}

			yggr_test_assert(rst.second != haystack.cend());
			yggr_test_assert(*rst.second == ' ');

			yggr::utf8_string tmp_haystack(rst.second, haystack.cend());
			rst = yggr::boyer_moore_search(rst.second, haystack.cend(), bm);

			if(rst.first != haystack.cend())
			{
				yggr_test_assert(false);
				std::cout << "Found '" << needle1 << "'  in '" << tmp_haystack << "' (boyer_moore)" << std::endl;
			}
			else
			{
				std::cout << "Did NOT find '" << needle1 << "'  in '" << tmp_haystack << "' (boyer_moore)" << std::endl;
			}
		}
	}

#ifndef YGGR_NO_CWCHAR
	{
		yggr::utf8_string haystack( L"ABACAB你C你好 is it  任何地方!" );
		yggr::utf8_string needle1( L"你C你好" );
		yggr::utf8_string needle2( L"not 你好" );

		{
			yggr::boyer_moore<yggr::utf8_string::iterator> bm(needle1.begin(), needle1.end());
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
			if(yggr::boyer_moore_search(haystack.cbegin(), haystack.cend(), needle1.cbegin(), needle1.cend()).first != haystack.cend())
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
			typedef std::pair<yggr::utf8_string::const_iterator, yggr::utf8_string::const_iterator> rst_type;
			typedef yggr::boyer_moore<yggr::utf8_string::iterator> bm_type;

			bm_type bm(needle1.begin(), needle1.end());
			rst_type rst = yggr::boyer_moore_search(haystack.cbegin(), haystack.cend(), bm);

			if(rst.first != haystack.cend())
			{
				std::cout << "Found '" << needle1 << "'  in '" << haystack << "' (boyer_moore)" << std::endl;
			}
			else
			{
				yggr_test_assert(false);
				std::cout << "Did NOT find '" << needle1 << "'  in '" << haystack << "' (boyer_moore)" << std::endl;
			}

			yggr_test_assert(rst.second != haystack.cend());
			yggr_test_assert(*rst.second == ' ');

			yggr::utf8_string tmp_haystack(rst.second, haystack.cend());
			rst = yggr::boyer_moore_search(rst.second, haystack.cend(), bm);

			if(rst.first != haystack.cend())
			{
				yggr_test_assert(false);
				std::cout << "Found '" << needle1 << "'  in '" << tmp_haystack << "' (boyer_moore)" << std::endl;
			}
			else
			{
				std::cout << "Did NOT find '" << needle1 << "'  in '" << tmp_haystack << "' (boyer_moore)" << std::endl;
			}
		}
	}
#endif // YGGR_NO_CWCHAR
}

int main(int argc, char* argv[])
{
	bm_test();

	wait_any_key(argc, argv);
	return 0;
}
