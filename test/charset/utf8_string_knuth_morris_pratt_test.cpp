//knuth_morris_pratt_test.cpp

#if !defined(_MSC_VER)
#	error "this file supports MSVC only, if you are using other compilers, please use the same name file in the charset_utf8_files"	
#endif // _MSC_VER

//#define YGGR_NO_CWCHAR

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/knuth_morris_pratt.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

void kmp_test(void)
{
	{
		yggr::utf8_string haystack( "ABACAB is it everywhere!" );
		yggr::utf8_string needle1( "ACAB" );
		yggr::utf8_string needle2( "not ABA" );

		{
			yggr::knuth_morris_pratt<yggr::utf8_string::iterator> kmp(needle1.begin(), needle1.end());
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
			if(yggr::knuth_morris_pratt_search(haystack.cbegin(), haystack.cend(), needle1.cbegin(), needle1.cend()).first != haystack.cend())
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
			typedef std::pair<yggr::utf8_string::const_iterator, yggr::utf8_string::const_iterator> rst_type;
			typedef yggr::knuth_morris_pratt<yggr::utf8_string::iterator> kmp_type;

			kmp_type kmp(needle1.begin(), needle1.end());
			rst_type rst = yggr::knuth_morris_pratt_search(haystack.cbegin(), haystack.cend(), kmp);

			if(rst.first != haystack.cend())
			{
				std::cout << "Found '" << needle1 << "'  in '" << haystack << "' (knuth_morris_pratt)" << std::endl;
			}
			else
			{
				yggr_test_assert(false);
				std::cout << "Did NOT find '" << needle1 << "'  in '" << haystack << "' (knuth_morris_pratt)" << std::endl;
			}

			yggr_test_assert(rst.second != haystack.cend());
			yggr_test_assert(*rst.second == ' ');

			yggr::utf8_string tmp_haystack(rst.second, haystack.cend());
			rst = yggr::knuth_morris_pratt_search(rst.second, haystack.cend(), kmp);

			if(rst.first != haystack.cend())
			{
				yggr_test_assert(false);
				std::cout << "Found '" << needle1 << "'  in '" << tmp_haystack << "' (knuth_morris_pratt)" << std::endl;
			}
			else
			{
				std::cout << "Did NOT find '" << needle1 << "'  in '" << tmp_haystack << "' (knuth_morris_pratt)" << std::endl;
			}
		}
	}

#ifndef YGGR_NO_CWCHAR
	{
		yggr::utf8_string haystack( L"ABACAB你C你好 is it  任何地方!" );
		yggr::utf8_string needle1( L"你C你好" );
		yggr::utf8_string needle2( L"not 你好" );

		{
			yggr::knuth_morris_pratt<yggr::utf8_string::iterator> kmp(needle1.begin(), needle1.end());
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
			if(yggr::knuth_morris_pratt_search(haystack.cbegin(), haystack.cend(), needle1.cbegin(), needle1.cend()).first != haystack.cend())
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
			typedef std::pair<yggr::utf8_string::const_iterator, yggr::utf8_string::const_iterator> rst_type;
			typedef yggr::knuth_morris_pratt<yggr::utf8_string::iterator> kmp_type;

			kmp_type kmp(needle1.begin(), needle1.end());
			rst_type rst = yggr::knuth_morris_pratt_search(haystack.cbegin(), haystack.cend(), kmp);

			if(rst.first != haystack.cend())
			{
				std::cout << "Found '" << needle1 << "'  in '" << haystack << "' (knuth_morris_pratt)" << std::endl;
			}
			else
			{
				yggr_test_assert(false);
				std::cout << "Did NOT find '" << needle1 << "'  in '" << haystack << "' (knuth_morris_pratt)" << std::endl;
			}

			yggr_test_assert(rst.second != haystack.cend());
			yggr_test_assert(*rst.second == ' ');

			yggr::utf8_string tmp_haystack(rst.second, haystack.cend());
			rst = yggr::knuth_morris_pratt_search(rst.second, haystack.cend(), kmp);

			if(rst.first != haystack.cend())
			{
				yggr_test_assert(false);
				std::cout << "Found '" << needle1 << "'  in '" << tmp_haystack << "' (knuth_morris_pratt)" << std::endl;
			}
			else
			{
				std::cout << "Did NOT find '" << needle1 << "'  in '" << tmp_haystack << "' (knuth_morris_pratt)" << std::endl;
			}
		}
	}
#endif // YGGR_NO_CWCHAR
}

int main(int argc, char* argv[])
{
	kmp_test();

	wait_any_key(argc, argv);
	return 0;
}
