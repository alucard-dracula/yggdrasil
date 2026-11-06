//tuple_comparer_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <yggr/tuple_ex/tuple.hpp> 

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

void test_compare(void)
{
#if !defined(YGGR_NO_CXX11_HDR_TUPLE)
	// std::tuple cmp other
	{
		// ==
		yggr_test_assert(std::make_tuple(1) == std::make_tuple(1));
		yggr_test_assert(std::make_tuple(1) == boost::make_tuple(1));
		yggr_test_assert(std::make_tuple(1) == yggr::make_tuple(1));

		yggr_test_assert(std::make_tuple(1, 1) == std::make_tuple(1, 1));
		yggr_test_assert(std::make_tuple(1, 1) == boost::make_tuple(1, 1));
		yggr_test_assert(std::make_tuple(1, 1) == yggr::make_tuple(1, 1));

		yggr_test_assert(std::make_tuple(1, 1) == std::make_tuple(1.0, 1.0));
		yggr_test_assert(std::make_tuple(1, 1) == boost::make_tuple(1.0, 1.0));
		yggr_test_assert(std::make_tuple(1, 1) == yggr::make_tuple(1.0, 1.0));

		//yggr_test_assert(!(std::make_tuple(1, 1) == std::make_tuple(1.0))); // org not support
		yggr_test_assert(!(std::make_tuple(1, 1) == boost::make_tuple(1.0))); 
		yggr_test_assert(!(std::make_tuple(1, 1) == yggr::make_tuple(1.0))); 

		yggr_test_assert(std::make_tuple(1, 1, 1, 1, 2) == std::make_tuple(1.0, 1.0, 1.0, 1.0, 2.0));
		yggr_test_assert(std::make_tuple(1, 1, 1, 1, 2) == boost::make_tuple(1.0, 1.0, 1.0, 1.0, 2.0));
		yggr_test_assert(std::make_tuple(1, 1, 1, 1, 2) == yggr::make_tuple(1.0, 1.0, 1.0, 1.0, 2.0));

		// !=
		yggr_test_assert(std::make_tuple(2) != std::make_tuple(1));
		yggr_test_assert(std::make_tuple(2) != boost::make_tuple(1));
		yggr_test_assert(std::make_tuple(2) != yggr::make_tuple(1));

		yggr_test_assert(std::make_tuple(1, 2) != std::make_tuple(1, 1));
		yggr_test_assert(std::make_tuple(1, 2) != boost::make_tuple(1, 1));
		yggr_test_assert(std::make_tuple(1, 2) != yggr::make_tuple(1, 1));

		yggr_test_assert(std::make_tuple(1, 2) != std::make_tuple(1.0, 1.0));
		yggr_test_assert(std::make_tuple(1, 2) != boost::make_tuple(1.0, 1.0));
		yggr_test_assert(std::make_tuple(1, 2) != yggr::make_tuple(1.0, 1.0));

		//yggr_test_assert(std::make_tuple(1, 1) != std::make_tuple(1.0)); // org not support
		yggr_test_assert(std::make_tuple(1, 1) != boost::make_tuple(1.0));
		yggr_test_assert(std::make_tuple(1, 1) != yggr::make_tuple(1.0));

		yggr_test_assert(std::make_tuple(1, 1, 1, 1, 2) != std::make_tuple(1.0, 1.0, 1.0, 1.0, 3.0));
		yggr_test_assert(std::make_tuple(1, 1, 1, 1, 2) != boost::make_tuple(1.0, 1.0, 1.0, 1.0, 3.0));
		yggr_test_assert(std::make_tuple(1, 1, 1, 1, 2) != yggr::make_tuple(1.0, 1.0, 1.0, 1.0, 3.0));

		// <
		yggr_test_assert(std::make_tuple(1) < std::make_tuple(2));
		yggr_test_assert(std::make_tuple(1) < boost::make_tuple(2));
		yggr_test_assert(std::make_tuple(1) < yggr::make_tuple(2));

		yggr_test_assert(std::make_tuple(1, 1) < std::make_tuple(1, 2));
		yggr_test_assert(std::make_tuple(1, 1) < boost::make_tuple(1, 2));
		yggr_test_assert(std::make_tuple(1, 1) < yggr::make_tuple(1, 2));

		yggr_test_assert(std::make_tuple(1, 1) < std::make_tuple(2.0, 1.0));
		yggr_test_assert(std::make_tuple(1, 1) < boost::make_tuple(2.0, 1.0));
		yggr_test_assert(std::make_tuple(1, 1) < yggr::make_tuple(2.0, 1.0));

		//yggr_test_assert(std::make_tuple(1, 1) < std::make_tuple(2.0)); // not support
		//yggr_test_assert(std::make_tuple(1, 1) < boost::make_tuple(2.0)); // not support
		//yggr_test_assert(std::make_tuple(1, 1) < yggr::make_tuple(2.0));  // not support

		yggr_test_assert(std::make_tuple(1, 1, 1, 1, 2) < std::make_tuple(1.0, 1.0, 1.0, 1.0, 3.0));
		yggr_test_assert(std::make_tuple(1, 1, 1, 1, 2) < boost::make_tuple(1.0, 1.0, 1.0, 1.0, 3.0));
		yggr_test_assert(std::make_tuple(1, 1, 1, 1, 2) < yggr::make_tuple(1.0, 1.0, 1.0, 1.0, 3.0));

		// <= diff l r
		yggr_test_assert(std::make_tuple(1) <= std::make_tuple(2));
		yggr_test_assert(std::make_tuple(1) <= boost::make_tuple(2));
		yggr_test_assert(std::make_tuple(1) <= yggr::make_tuple(2));

		yggr_test_assert(std::make_tuple(1, 1) <= std::make_tuple(1, 2));
		yggr_test_assert(std::make_tuple(1, 1) <= boost::make_tuple(1, 2));
		yggr_test_assert(std::make_tuple(1, 1) <= yggr::make_tuple(1, 2));

		yggr_test_assert(std::make_tuple(1, 1) <= std::make_tuple(2.0, 1.0));
		yggr_test_assert(std::make_tuple(1, 1) <= boost::make_tuple(2.0, 1.0));
		yggr_test_assert(std::make_tuple(1, 1) <= yggr::make_tuple(2.0, 1.0));

		//yggr_test_assert(std::make_tuple(1, 1) <= std::make_tuple(2.0)); // not support
		//yggr_test_assert(std::make_tuple(1, 1) <= boost::make_tuple(2.0)); // not support
		//yggr_test_assert(std::make_tuple(1, 1) <= yggr::make_tuple(2.0));  // not support

		yggr_test_assert(std::make_tuple(1, 1, 1, 1, 2) <= std::make_tuple(1.0, 1.0, 1.0, 1.0, 3.0));
		yggr_test_assert(std::make_tuple(1, 1, 1, 1, 2) <= boost::make_tuple(1.0, 1.0, 1.0, 1.0, 3.0));
		yggr_test_assert(std::make_tuple(1, 1, 1, 1, 2) <= yggr::make_tuple(1.0, 1.0, 1.0, 1.0, 3.0));
		
		// <= same l r
		yggr_test_assert(std::make_tuple(1) <= std::make_tuple(1));
		yggr_test_assert(std::make_tuple(1) <= boost::make_tuple(1));
		yggr_test_assert(std::make_tuple(1) <= yggr::make_tuple(1));

		yggr_test_assert(std::make_tuple(1, 1) <= std::make_tuple(1, 1));
		yggr_test_assert(std::make_tuple(1, 1) <= boost::make_tuple(1, 1));
		yggr_test_assert(std::make_tuple(1, 1) <= yggr::make_tuple(1, 1));

		yggr_test_assert(std::make_tuple(1, 1) <= std::make_tuple(1.0, 1.0));
		yggr_test_assert(std::make_tuple(1, 1) <= boost::make_tuple(1.0, 1.0));
		yggr_test_assert(std::make_tuple(1, 1) <= yggr::make_tuple(1.0, 1.0));

		//yggr_test_assert(std::make_tuple(1, 1) <= std::make_tuple(1.0)); // not support
		//yggr_test_assert(std::make_tuple(1, 1) <= boost::make_tuple(1.0)); // not support
		//yggr_test_assert(std::make_tuple(1, 1) <= yggr::make_tuple(1.0)); // not support

		yggr_test_assert(std::make_tuple(1, 1, 1, 1, 2) <= std::make_tuple(1.0, 1.0, 1.0, 1.0, 2.0));
		yggr_test_assert(std::make_tuple(1, 1, 1, 1, 2) <= boost::make_tuple(1.0, 1.0, 1.0, 1.0, 2.0));
		yggr_test_assert(std::make_tuple(1, 1, 1, 1, 2) <= yggr::make_tuple(1.0, 1.0, 1.0, 1.0, 2.0));

		// >
		yggr_test_assert(std::make_tuple(2) > std::make_tuple(1));
		yggr_test_assert(std::make_tuple(2) > boost::make_tuple(1));
		yggr_test_assert(std::make_tuple(2) > yggr::make_tuple(1));

		yggr_test_assert(std::make_tuple(1, 2) > std::make_tuple(1, 1));
		yggr_test_assert(std::make_tuple(2, 1) > boost::make_tuple(1, 1));
		yggr_test_assert(std::make_tuple(1, 2) > yggr::make_tuple(1, 1));

		yggr_test_assert(std::make_tuple(1, 2) > std::make_tuple(1.0, 1.0));
		yggr_test_assert(std::make_tuple(1, 2) > boost::make_tuple(1.0, 1.0));
		yggr_test_assert(std::make_tuple(1, 2) > yggr::make_tuple(1.0, 1.0));

		//yggr_test_assert(std::make_tuple(1, 1) > std::make_tuple(1.0)); // not support
		//yggr_test_assert(std::make_tuple(1, 1) > boost::make_tuple(1.0)); // not support
		//yggr_test_assert(std::make_tuple(1, 1) > yggr::make_tuple(1.0)); // not support

		yggr_test_assert(std::make_tuple(1, 1, 1, 1, 3) > std::make_tuple(1.0, 1.0, 1.0, 1.0, 2.0));
		yggr_test_assert(std::make_tuple(1, 1, 1, 1, 3) > boost::make_tuple(1.0, 1.0, 1.0, 1.0, 2.0));
		yggr_test_assert(std::make_tuple(1, 1, 1, 1, 3) > yggr::make_tuple(1.0, 1.0, 1.0, 1.0, 2.0));

		// >= diff l r
		yggr_test_assert(std::make_tuple(2) >= std::make_tuple(1));
		yggr_test_assert(std::make_tuple(2) >= boost::make_tuple(1));
		yggr_test_assert(std::make_tuple(2) >= yggr::make_tuple(1));

		yggr_test_assert(std::make_tuple(1, 2) >= std::make_tuple(1, 1));
		yggr_test_assert(std::make_tuple(2, 1) >= boost::make_tuple(1, 1));
		yggr_test_assert(std::make_tuple(1, 2) >= yggr::make_tuple(1, 1));

		yggr_test_assert(std::make_tuple(1, 2) >= std::make_tuple(1.0, 1.0));
		yggr_test_assert(std::make_tuple(1, 2) >= boost::make_tuple(1.0, 1.0));
		yggr_test_assert(std::make_tuple(1, 2) >= yggr::make_tuple(1.0, 1.0));

		//yggr_test_assert(std::make_tuple(1, 1) >= std::make_tuple(1.0)); // not support
		//yggr_test_assert(std::make_tuple(1, 1) >= boost::make_tuple(1.0)); // not support
		//yggr_test_assert(std::make_tuple(1, 1) >= yggr::make_tuple(1.0)); // not support

		yggr_test_assert(std::make_tuple(1, 1, 1, 1, 3) >= std::make_tuple(1.0, 1.0, 1.0, 1.0, 2.0));
		yggr_test_assert(std::make_tuple(1, 1, 1, 1, 3) >= boost::make_tuple(1.0, 1.0, 1.0, 1.0, 2.0));
		yggr_test_assert(std::make_tuple(1, 1, 1, 1, 3) >= yggr::make_tuple(1.0, 1.0, 1.0, 1.0, 2.0));

		// >= same l r
		yggr_test_assert(std::make_tuple(1) >= std::make_tuple(1));
		yggr_test_assert(std::make_tuple(1) >= boost::make_tuple(1));
		yggr_test_assert(std::make_tuple(1) >= yggr::make_tuple(1));

		yggr_test_assert(std::make_tuple(1, 1) >= std::make_tuple(1, 1));
		yggr_test_assert(std::make_tuple(1, 1) >= boost::make_tuple(1, 1));
		yggr_test_assert(std::make_tuple(1, 1) >= yggr::make_tuple(1, 1));

		yggr_test_assert(std::make_tuple(1, 1) >= std::make_tuple(1.0, 1.0));
		yggr_test_assert(std::make_tuple(1, 1) >= boost::make_tuple(1.0, 1.0));
		yggr_test_assert(std::make_tuple(1, 1) >= yggr::make_tuple(1.0, 1.0));

		//yggr_test_assert(std::make_tuple(1, 1) >= std::make_tuple(1.0)); // not support
		//yggr_test_assert(std::make_tuple(1, 1) >= boost::make_tuple(1.0)); // not support
		//yggr_test_assert(std::make_tuple(1, 1) >= yggr::make_tuple(1.0)); // not support

		yggr_test_assert(std::make_tuple(1, 1, 1, 1, 2) >= std::make_tuple(1.0, 1.0, 1.0, 1.0, 2.0));
		yggr_test_assert(std::make_tuple(1, 1, 1, 1, 2) >= boost::make_tuple(1.0, 1.0, 1.0, 1.0, 2.0));
		yggr_test_assert(std::make_tuple(1, 1, 1, 1, 2) >= yggr::make_tuple(1.0, 1.0, 1.0, 1.0, 2.0));

		{
			std::equal_to< std::tuple<int, int> > eq;
			bool bchk = eq(std::make_tuple(1, 2), std::make_tuple(1, 2));
			yggr_test_assert(bchk);
		}

		std::cout << std::make_tuple(1, 2, 3, 4, 5) << std::endl;

#	ifdef YGGR_NO_CWCHAR
		std::wcout << std::make_tuple(1, 2, 3, 4, 5) << std::endl;
#	endif // YGGR_NO_CWCHAR
	}
#endif // YGGR_NO_CXX11_HDR_TUPLE

	{
#if !defined(YGGR_NO_CXX11_HDR_TUPLE)
		// ==
		yggr_test_assert(boost::make_tuple(1) == std::make_tuple(1));
		yggr_test_assert(boost::make_tuple(1, 1) == std::make_tuple(1, 1));
		yggr_test_assert(boost::make_tuple(1, 1) == std::make_tuple(1.0, 1.0));
		yggr_test_assert(!(boost::make_tuple(1, 1) == std::make_tuple(1.0))); 
		yggr_test_assert(boost::make_tuple(1, 1, 1, 1, 2) == std::make_tuple(1.0, 1.0, 1.0, 1.0, 2.0));

		// !=
		yggr_test_assert(boost::make_tuple(2) != std::make_tuple(1));
		yggr_test_assert(boost::make_tuple(1, 2) != std::make_tuple(1, 1));
		yggr_test_assert(boost::make_tuple(1, 2) != std::make_tuple(1.0, 1.0));
		yggr_test_assert(boost::make_tuple(1, 1) != std::make_tuple(1.0)); // org not support
		yggr_test_assert(boost::make_tuple(1, 1, 1, 1, 2) != std::make_tuple(1.0, 1.0, 1.0, 1.0, 3.0));

		// <
		yggr_test_assert(boost::make_tuple(1) < std::make_tuple(2));
		yggr_test_assert(boost::make_tuple(1, 1) < std::make_tuple(1, 2));
		yggr_test_assert(boost::make_tuple(1, 1) < std::make_tuple(2.0, 1.0));
		//yggr_test_assert(boost::make_tuple(1, 1) < std::make_tuple(2.0)); // not support
		yggr_test_assert(boost::make_tuple(1, 1, 1, 1, 2) < std::make_tuple(1.0, 1.0, 1.0, 1.0, 3.0));

		// <= diff l r
		yggr_test_assert(boost::make_tuple(1) <= std::make_tuple(2));
		yggr_test_assert(boost::make_tuple(1, 1) <= std::make_tuple(1, 2));
		yggr_test_assert(boost::make_tuple(1, 1) <= std::make_tuple(2.0, 1.0));
		//yggr_test_assert(boost::make_tuple(1, 1) <= std::make_tuple(2.0)); // not support
		yggr_test_assert(boost::make_tuple(1, 1, 1, 1, 2) <= std::make_tuple(1.0, 1.0, 1.0, 1.0, 3.0));

		// <= same l r
		yggr_test_assert(boost::make_tuple(1) <= std::make_tuple(1));
		yggr_test_assert(boost::make_tuple(1, 1) <= std::make_tuple(1, 1));
		yggr_test_assert(boost::make_tuple(1, 1) <= std::make_tuple(1.0, 1.0));
		//yggr_test_assert(boost::make_tuple(1, 1) <= std::make_tuple(1.0)); // not support
		yggr_test_assert(boost::make_tuple(1, 1, 1, 1, 2) <= std::make_tuple(1.0, 1.0, 1.0, 1.0, 2.0));

		// >
		yggr_test_assert(boost::make_tuple(2) > std::make_tuple(1));
		yggr_test_assert(boost::make_tuple(1, 2) > std::make_tuple(1, 1));
		yggr_test_assert(boost::make_tuple(1, 2) > std::make_tuple(1.0, 1.0));
		//yggr_test_assert(boost::make_tuple(1, 1) > std::make_tuple(1.0)); // not support
		yggr_test_assert(boost::make_tuple(1, 1, 1, 1, 3) > std::make_tuple(1.0, 1.0, 1.0, 1.0, 2.0));

		// >= diff l r
		yggr_test_assert(boost::make_tuple(2) >= std::make_tuple(1));
		yggr_test_assert(boost::make_tuple(1, 2) >= std::make_tuple(1, 1));
		yggr_test_assert(boost::make_tuple(1, 2) >= std::make_tuple(1.0, 1.0));
		//yggr_test_assert(boost::make_tuple(1, 1) >= std::make_tuple(1.0)); // not support
		yggr_test_assert(boost::make_tuple(1, 1, 1, 1, 3) >= std::make_tuple(1.0, 1.0, 1.0, 1.0, 2.0));

		// >= same l r
		yggr_test_assert(boost::make_tuple(1) >= std::make_tuple(1));
		yggr_test_assert(boost::make_tuple(1, 1) >= std::make_tuple(1, 1));
		yggr_test_assert(boost::make_tuple(1, 1) >= std::make_tuple(1.0, 1.0));
		//yggr_test_assert(boost::make_tuple(1, 1) >= std::make_tuple(1.0)); // not support
		yggr_test_assert(boost::make_tuple(1, 1, 1, 1, 2) >= std::make_tuple(1.0, 1.0, 1.0, 1.0, 2.0));

#endif // YGGR_NO_CXX11_HDR_TUPLE
		// ==
		
		yggr_test_assert(boost::make_tuple(1) == boost::make_tuple(1));
		yggr_test_assert(boost::make_tuple(1) == yggr::make_tuple(1));

		yggr_test_assert(boost::make_tuple(1, 1) == boost::make_tuple(1, 1));
		yggr_test_assert(boost::make_tuple(1, 1) == yggr::make_tuple(1, 1));

		yggr_test_assert(boost::make_tuple(1, 1) == boost::make_tuple(1.0, 1.0));
		yggr_test_assert(boost::make_tuple(1, 1) == yggr::make_tuple(1.0, 1.0));

		yggr_test_assert(!(boost::make_tuple(1, 1) == boost::make_tuple(1.0))); // org not support
		yggr_test_assert(!(boost::make_tuple(1, 1) == yggr::make_tuple(1.0))); 

		yggr_test_assert(boost::make_tuple(1, 1, 1, 1, 2) == boost::make_tuple(1.0, 1.0, 1.0, 1.0, 2.0));
		yggr_test_assert(boost::make_tuple(1, 1, 1, 1, 2) == yggr::make_tuple(1.0, 1.0, 1.0, 1.0, 2.0));

		// !=
		yggr_test_assert(boost::make_tuple(2) != boost::make_tuple(1));
		yggr_test_assert(boost::make_tuple(2) != yggr::make_tuple(1));

		yggr_test_assert(boost::make_tuple(1, 2) != boost::make_tuple(1, 1));
		yggr_test_assert(boost::make_tuple(1, 2) != yggr::make_tuple(1, 1));

		yggr_test_assert(boost::make_tuple(1, 2) != boost::make_tuple(1.0, 1.0));
		yggr_test_assert(boost::make_tuple(1, 2) != yggr::make_tuple(1.0, 1.0));
		
		yggr_test_assert(boost::make_tuple(1, 1) != boost::make_tuple(1.0)); // org not support
		yggr_test_assert(boost::make_tuple(1, 1) != yggr::make_tuple(1.0)); // org not support

		yggr_test_assert(boost::make_tuple(1, 1, 1, 1, 2) != boost::make_tuple(1.0, 1.0, 1.0, 1.0, 3.0));
		yggr_test_assert(boost::make_tuple(1, 1, 1, 1, 2) != yggr::make_tuple(1.0, 1.0, 1.0, 1.0, 3.0));

		// <
		yggr_test_assert(boost::make_tuple(1) < boost::make_tuple(2));
		yggr_test_assert(boost::make_tuple(1) < yggr::make_tuple(2));

		yggr_test_assert(boost::make_tuple(1, 1) < boost::make_tuple(1, 2));
		yggr_test_assert(boost::make_tuple(1, 1) < yggr::make_tuple(1, 2));
		
		yggr_test_assert(boost::make_tuple(1, 1) < boost::make_tuple(2.0, 1.0));
		yggr_test_assert(boost::make_tuple(1, 1) < yggr::make_tuple(2.0, 1.0));
		
		//yggr_test_assert(boost::make_tuple(1, 1) < boost::make_tuple(2.0)); // not support
		//yggr_test_assert(boost::make_tuple(1, 1) < yggr::make_tuple(2.0));  // not support
		
		yggr_test_assert(boost::make_tuple(1, 1, 1, 1, 2) < boost::make_tuple(1.0, 1.0, 1.0, 1.0, 3.0));
		yggr_test_assert(boost::make_tuple(1, 1, 1, 1, 2) < yggr::make_tuple(1.0, 1.0, 1.0, 1.0, 3.0));

		// <= diff l r
		yggr_test_assert(boost::make_tuple(1) <= boost::make_tuple(2));
		yggr_test_assert(boost::make_tuple(1) <= yggr::make_tuple(2));

		yggr_test_assert(boost::make_tuple(1, 1) <= boost::make_tuple(1, 2));
		yggr_test_assert(boost::make_tuple(1, 1) <= yggr::make_tuple(1, 2));

		yggr_test_assert(boost::make_tuple(1, 1) <= boost::make_tuple(2.0, 1.0));
		yggr_test_assert(boost::make_tuple(1, 1) <= yggr::make_tuple(2.0, 1.0));

		//yggr_test_assert(boost::make_tuple(1, 1) <= boost::make_tuple(2.0)); // not support
		//yggr_test_assert(boost::make_tuple(1, 1) <= yggr::make_tuple(2.0));  // not support

		yggr_test_assert(boost::make_tuple(1, 1, 1, 1, 2) <= boost::make_tuple(1.0, 1.0, 1.0, 1.0, 3.0));
		yggr_test_assert(boost::make_tuple(1, 1, 1, 1, 2) <= yggr::make_tuple(1.0, 1.0, 1.0, 1.0, 3.0));

		// <= same l r
		yggr_test_assert(boost::make_tuple(1) <= boost::make_tuple(1));
		yggr_test_assert(boost::make_tuple(1) <= yggr::make_tuple(1));

		yggr_test_assert(boost::make_tuple(1, 1) <= boost::make_tuple(1, 1));
		yggr_test_assert(boost::make_tuple(1, 1) <= yggr::make_tuple(1, 1));

		yggr_test_assert(boost::make_tuple(1, 1) <= boost::make_tuple(1.0, 1.0));
		yggr_test_assert(boost::make_tuple(1, 1) <= yggr::make_tuple(1.0, 1.0));

		//yggr_test_assert(boost::make_tuple(1, 1) <= boost::make_tuple(1.0)); // not support
		//yggr_test_assert(boost::make_tuple(1, 1) <= yggr::make_tuple(1.0)); // not support

		yggr_test_assert(boost::make_tuple(1, 1, 1, 1, 2) <= boost::make_tuple(1.0, 1.0, 1.0, 1.0, 2.0));
		yggr_test_assert(boost::make_tuple(1, 1, 1, 1, 2) <= yggr::make_tuple(1.0, 1.0, 1.0, 1.0, 2.0));

		// >

		yggr_test_assert(boost::make_tuple(2) > boost::make_tuple(1));
		yggr_test_assert(boost::make_tuple(2) > yggr::make_tuple(1));
		
		yggr_test_assert(boost::make_tuple(2, 1) > boost::make_tuple(1, 1));
		yggr_test_assert(boost::make_tuple(1, 2) > yggr::make_tuple(1, 1));
		
		yggr_test_assert(boost::make_tuple(1, 2) > boost::make_tuple(1.0, 1.0));
		yggr_test_assert(boost::make_tuple(1, 2) > yggr::make_tuple(1.0, 1.0));

		//yggr_test_assert(boost::make_tuple(1, 1) > boost::make_tuple(1.0)); // not support
		//yggr_test_assert(boost::make_tuple(1, 1) > yggr::make_tuple(1.0)); // not support

		yggr_test_assert(boost::make_tuple(1, 1, 1, 1, 3) > boost::make_tuple(1.0, 1.0, 1.0, 1.0, 2.0));
		yggr_test_assert(boost::make_tuple(1, 1, 1, 1, 3) > yggr::make_tuple(1.0, 1.0, 1.0, 1.0, 2.0));

		// >= diff l r
		yggr_test_assert(boost::make_tuple(2) >= boost::make_tuple(1));
		yggr_test_assert(boost::make_tuple(2) >= yggr::make_tuple(1));

		yggr_test_assert(boost::make_tuple(2, 1) >= boost::make_tuple(1, 1));
		yggr_test_assert(boost::make_tuple(1, 2) >= yggr::make_tuple(1, 1));

		yggr_test_assert(boost::make_tuple(1, 2) >= boost::make_tuple(1.0, 1.0));
		yggr_test_assert(boost::make_tuple(1, 2) >= yggr::make_tuple(1.0, 1.0));

		//yggr_test_assert(boost::make_tuple(1, 1) >= boost::make_tuple(1.0)); // not support
		//yggr_test_assert(boost::make_tuple(1, 1) >= yggr::make_tuple(1.0)); // not support

		yggr_test_assert(boost::make_tuple(1, 1, 1, 1, 3) >= boost::make_tuple(1.0, 1.0, 1.0, 1.0, 2.0));
		yggr_test_assert(boost::make_tuple(1, 1, 1, 1, 3) >= yggr::make_tuple(1.0, 1.0, 1.0, 1.0, 2.0));

		// >= same l r
		yggr_test_assert(boost::make_tuple(1) >= boost::make_tuple(1));
		yggr_test_assert(boost::make_tuple(1) >= yggr::make_tuple(1));

		yggr_test_assert(boost::make_tuple(1, 1) >= boost::make_tuple(1, 1));
		yggr_test_assert(boost::make_tuple(1, 1) >= yggr::make_tuple(1, 1));

		yggr_test_assert(boost::make_tuple(1, 1) >= boost::make_tuple(1.0, 1.0));
		yggr_test_assert(boost::make_tuple(1, 1) >= yggr::make_tuple(1.0, 1.0));

		//yggr_test_assert(boost::make_tuple(1, 1) >= boost::make_tuple(1.0)); // not support
		//yggr_test_assert(boost::make_tuple(1, 1) >= yggr::make_tuple(1.0)); // not support

		yggr_test_assert(boost::make_tuple(1, 1, 1, 1, 2) >= boost::make_tuple(1.0, 1.0, 1.0, 1.0, 2.0));
		yggr_test_assert(boost::make_tuple(1, 1, 1, 1, 2) >= yggr::make_tuple(1.0, 1.0, 1.0, 1.0, 2.0));

		{
			std::equal_to< boost::tuple<int, int> > eq;
			bool bchk = eq(boost::make_tuple(1, 2), boost::make_tuple(1, 2));
			yggr_test_assert(bchk);
		}

		std::cout << boost::make_tuple(1, 2, 3, 4, 5) << std::endl;

#	ifdef YGGR_NO_CWCHAR
		std::wcout << boost::make_tuple(1, 2, 3, 4, 5) << std::endl;
#	endif // YGGR_NO_CWCHAR
	}

	{
#if !defined(YGGR_NO_CXX11_HDR_TUPLE)
		// ==
		yggr_test_assert(yggr::make_tuple(1) == std::make_tuple(1));
		yggr_test_assert(yggr::make_tuple(1, 1) == std::make_tuple(1, 1));
		yggr_test_assert(yggr::make_tuple(1, 1) == std::make_tuple(1.0, 1.0));
		yggr_test_assert(!(yggr::make_tuple(1, 1) == std::make_tuple(1.0))); 
		yggr_test_assert(yggr::make_tuple(1, 1, 1, 1, 2) == std::make_tuple(1.0, 1.0, 1.0, 1.0, 2.0));

		// !=
		yggr_test_assert(yggr::make_tuple(2) != std::make_tuple(1));
		yggr_test_assert(yggr::make_tuple(1, 2) != std::make_tuple(1, 1));
		yggr_test_assert(yggr::make_tuple(1, 2) != std::make_tuple(1.0, 1.0));
		yggr_test_assert(yggr::make_tuple(1, 1) != std::make_tuple(1.0)); // org not support
		yggr_test_assert(yggr::make_tuple(1, 1, 1, 1, 2) != std::make_tuple(1.0, 1.0, 1.0, 1.0, 3.0));

		// <
		yggr_test_assert(yggr::make_tuple(1) < std::make_tuple(2));
		yggr_test_assert(yggr::make_tuple(1, 1) < std::make_tuple(1, 2));
		yggr_test_assert(yggr::make_tuple(1, 1) < std::make_tuple(2.0, 1.0));
		//yggr_test_assert(yggr::make_tuple(1, 1) < std::make_tuple(2.0)); // not support
		yggr_test_assert(yggr::make_tuple(1, 1, 1, 1, 2) < std::make_tuple(1.0, 1.0, 1.0, 1.0, 3.0));

		// <= diff l r
		yggr_test_assert(yggr::make_tuple(1) <= std::make_tuple(2));
		yggr_test_assert(yggr::make_tuple(1, 1) <= std::make_tuple(1, 2));
		yggr_test_assert(yggr::make_tuple(1, 1) <= std::make_tuple(2.0, 1.0));
		//yggr_test_assert(yggr::make_tuple(1, 1) <= std::make_tuple(2.0)); // not support
		yggr_test_assert(yggr::make_tuple(1, 1, 1, 1, 2) <= std::make_tuple(1.0, 1.0, 1.0, 1.0, 3.0));

		// <= same l r
		yggr_test_assert(yggr::make_tuple(1) <= std::make_tuple(1));
		yggr_test_assert(yggr::make_tuple(1, 1) <= std::make_tuple(1, 1));
		yggr_test_assert(yggr::make_tuple(1, 1) <= std::make_tuple(1.0, 1.0));
		//yggr_test_assert(yggr::make_tuple(1, 1) <= std::make_tuple(1.0)); // not support
		yggr_test_assert(yggr::make_tuple(1, 1, 1, 1, 2) <= std::make_tuple(1.0, 1.0, 1.0, 1.0, 2.0));

		// >
		yggr_test_assert(yggr::make_tuple(2) > std::make_tuple(1));
		yggr_test_assert(yggr::make_tuple(1, 2) > std::make_tuple(1, 1));
		yggr_test_assert(yggr::make_tuple(1, 2) > std::make_tuple(1.0, 1.0));
		//yggr_test_assert(yggr::make_tuple(1, 1) > std::make_tuple(1.0)); // not support
		yggr_test_assert(yggr::make_tuple(1, 1, 1, 1, 3) > std::make_tuple(1.0, 1.0, 1.0, 1.0, 2.0));

		// >= diff l r
		yggr_test_assert(yggr::make_tuple(2) >= std::make_tuple(1));
		yggr_test_assert(yggr::make_tuple(1, 2) >= std::make_tuple(1, 1));
		yggr_test_assert(yggr::make_tuple(1, 2) >= std::make_tuple(1.0, 1.0));
		//yggr_test_assert(yggr::make_tuple(1, 1) >= std::make_tuple(1.0)); // not support
		yggr_test_assert(yggr::make_tuple(1, 1, 1, 1, 3) >= std::make_tuple(1.0, 1.0, 1.0, 1.0, 2.0));

		// >= same l r
		yggr_test_assert(yggr::make_tuple(1) >= std::make_tuple(1));
		yggr_test_assert(yggr::make_tuple(1, 1) >= std::make_tuple(1, 1));
		yggr_test_assert(yggr::make_tuple(1, 1) >= std::make_tuple(1.0, 1.0));
		//yggr_test_assert(yggr::make_tuple(1, 1) >= std::make_tuple(1.0)); // not support
		yggr_test_assert(yggr::make_tuple(1, 1, 1, 1, 2) >= std::make_tuple(1.0, 1.0, 1.0, 1.0, 2.0));

#endif // YGGR_NO_CXX11_HDR_TUPLE
		// ==
		
		yggr_test_assert(yggr::make_tuple(1) == boost::make_tuple(1));
		yggr_test_assert(yggr::make_tuple(1) == yggr::make_tuple(1));

		yggr_test_assert(yggr::make_tuple(1, 1) == boost::make_tuple(1, 1));
		yggr_test_assert(yggr::make_tuple(1, 1) == yggr::make_tuple(1, 1));

		yggr_test_assert(yggr::make_tuple(1, 1) == boost::make_tuple(1.0, 1.0));
		yggr_test_assert(yggr::make_tuple(1, 1) == yggr::make_tuple(1.0, 1.0));

		yggr_test_assert(!(boost::make_tuple(1, 1) == boost::make_tuple(1.0))); // org not support
		yggr_test_assert(!(boost::make_tuple(1, 1) == yggr::make_tuple(1.0))); 

		yggr_test_assert(yggr::make_tuple(1, 1, 1, 1, 2) == boost::make_tuple(1.0, 1.0, 1.0, 1.0, 2.0));
		yggr_test_assert(yggr::make_tuple(1, 1, 1, 1, 2) == yggr::make_tuple(1.0, 1.0, 1.0, 1.0, 2.0));

		// !=
		yggr_test_assert(yggr::make_tuple(2) != boost::make_tuple(1));
		yggr_test_assert(yggr::make_tuple(2) != yggr::make_tuple(1));

		yggr_test_assert(yggr::make_tuple(1, 2) != boost::make_tuple(1, 1));
		yggr_test_assert(yggr::make_tuple(1, 2) != yggr::make_tuple(1, 1));

		yggr_test_assert(yggr::make_tuple(1, 2) != boost::make_tuple(1.0, 1.0));
		yggr_test_assert(yggr::make_tuple(1, 2) != yggr::make_tuple(1.0, 1.0));
		
		yggr_test_assert(yggr::make_tuple(1, 1) != boost::make_tuple(1.0)); // org not support
		yggr_test_assert(yggr::make_tuple(1, 1) != yggr::make_tuple(1.0)); // org not support

		yggr_test_assert(yggr::make_tuple(1, 1, 1, 1, 2) != boost::make_tuple(1.0, 1.0, 1.0, 1.0, 3.0));
		yggr_test_assert(yggr::make_tuple(1, 1, 1, 1, 2) != yggr::make_tuple(1.0, 1.0, 1.0, 1.0, 3.0));

		// <
		yggr_test_assert(yggr::make_tuple(1) < boost::make_tuple(2));
		yggr_test_assert(yggr::make_tuple(1) < yggr::make_tuple(2));

		yggr_test_assert(yggr::make_tuple(1, 1) < boost::make_tuple(1, 2));
		yggr_test_assert(yggr::make_tuple(1, 1) < yggr::make_tuple(1, 2));
		
		yggr_test_assert(yggr::make_tuple(1, 1) < boost::make_tuple(2.0, 1.0));
		yggr_test_assert(yggr::make_tuple(1, 1) < yggr::make_tuple(2.0, 1.0));
		
		//yggr_test_assert(yggr::make_tuple(1, 1) < boost::make_tuple(2.0)); // not support
		//yggr_test_assert(yggr::make_tuple(1, 1) < yggr::make_tuple(2.0));  // not support
		
		yggr_test_assert(yggr::make_tuple(1, 1, 1, 1, 2) < boost::make_tuple(1.0, 1.0, 1.0, 1.0, 3.0));
		yggr_test_assert(yggr::make_tuple(1, 1, 1, 1, 2) < yggr::make_tuple(1.0, 1.0, 1.0, 1.0, 3.0));

		// <= diff l r
		yggr_test_assert(yggr::make_tuple(1) <= boost::make_tuple(2));
		yggr_test_assert(yggr::make_tuple(1) <= yggr::make_tuple(2));

		yggr_test_assert(yggr::make_tuple(1, 1) <= boost::make_tuple(1, 2));
		yggr_test_assert(yggr::make_tuple(1, 1) <= yggr::make_tuple(1, 2));

		yggr_test_assert(yggr::make_tuple(1, 1) <= boost::make_tuple(2.0, 1.0));
		yggr_test_assert(yggr::make_tuple(1, 1) <= yggr::make_tuple(2.0, 1.0));

		//yggr_test_assert(yggr::make_tuple(1, 1) <= boost::make_tuple(2.0)); // not support
		//yggr_test_assert(yggr::make_tuple(1, 1) <= yggr::make_tuple(2.0));  // not support

		yggr_test_assert(yggr::make_tuple(1, 1, 1, 1, 2) <= boost::make_tuple(1.0, 1.0, 1.0, 1.0, 3.0));
		yggr_test_assert(yggr::make_tuple(1, 1, 1, 1, 2) <= yggr::make_tuple(1.0, 1.0, 1.0, 1.0, 3.0));

		// <= same l r
		yggr_test_assert(yggr::make_tuple(1) <= boost::make_tuple(1));
		yggr_test_assert(yggr::make_tuple(1) <= yggr::make_tuple(1));

		yggr_test_assert(yggr::make_tuple(1, 1) <= boost::make_tuple(1, 1));
		yggr_test_assert(yggr::make_tuple(1, 1) <= yggr::make_tuple(1, 1));

		yggr_test_assert(yggr::make_tuple(1, 1) <= boost::make_tuple(1.0, 1.0));
		yggr_test_assert(yggr::make_tuple(1, 1) <= yggr::make_tuple(1.0, 1.0));

		//yggr_test_assert(yggr::make_tuple(1, 1) <= boost::make_tuple(1.0)); // not support
		//yggr_test_assert(yggr::make_tuple(1, 1) <= yggr::make_tuple(1.0)); // not support

		yggr_test_assert(yggr::make_tuple(1, 1, 1, 1, 2) <= boost::make_tuple(1.0, 1.0, 1.0, 1.0, 2.0));
		yggr_test_assert(yggr::make_tuple(1, 1, 1, 1, 2) <= yggr::make_tuple(1.0, 1.0, 1.0, 1.0, 2.0));

		// >

		yggr_test_assert(yggr::make_tuple(2) > boost::make_tuple(1));
		yggr_test_assert(yggr::make_tuple(2) > yggr::make_tuple(1));
		
		yggr_test_assert(yggr::make_tuple(2, 1) > boost::make_tuple(1, 1));
		yggr_test_assert(yggr::make_tuple(1, 2) > yggr::make_tuple(1, 1));
		
		yggr_test_assert(yggr::make_tuple(1, 2) > boost::make_tuple(1.0, 1.0));
		yggr_test_assert(yggr::make_tuple(1, 2) > yggr::make_tuple(1.0, 1.0));

		//yggr_test_assert(yggr::make_tuple(1, 1) > boost::make_tuple(1.0)); // not support
		//yggr_test_assert(yggr::make_tuple(1, 1) > yggr::make_tuple(1.0)); // not support

		yggr_test_assert(yggr::make_tuple(1, 1, 1, 1, 3) > boost::make_tuple(1.0, 1.0, 1.0, 1.0, 2.0));
		yggr_test_assert(yggr::make_tuple(1, 1, 1, 1, 3) > yggr::make_tuple(1.0, 1.0, 1.0, 1.0, 2.0));

		// >= diff l r
		yggr_test_assert(yggr::make_tuple(2) >= boost::make_tuple(1));
		yggr_test_assert(yggr::make_tuple(2) >= yggr::make_tuple(1));

		yggr_test_assert(yggr::make_tuple(2, 1) >= boost::make_tuple(1, 1));
		yggr_test_assert(yggr::make_tuple(1, 2) >= yggr::make_tuple(1, 1));

		yggr_test_assert(yggr::make_tuple(1, 2) >= boost::make_tuple(1.0, 1.0));
		yggr_test_assert(yggr::make_tuple(1, 2) >= yggr::make_tuple(1.0, 1.0));

		//yggr_test_assert(yggr::make_tuple(1, 1) >= boost::make_tuple(1.0)); // not support
		//yggr_test_assert(yggr::make_tuple(1, 1) >= yggr::make_tuple(1.0)); // not support

		yggr_test_assert(yggr::make_tuple(1, 1, 1, 1, 3) >= boost::make_tuple(1.0, 1.0, 1.0, 1.0, 2.0));
		yggr_test_assert(yggr::make_tuple(1, 1, 1, 1, 3) >= yggr::make_tuple(1.0, 1.0, 1.0, 1.0, 2.0));

		// >= same l r
		yggr_test_assert(yggr::make_tuple(1) >= boost::make_tuple(1));
		yggr_test_assert(yggr::make_tuple(1) >= yggr::make_tuple(1));

		yggr_test_assert(yggr::make_tuple(1, 1) >= boost::make_tuple(1, 1));
		yggr_test_assert(yggr::make_tuple(1, 1) >= yggr::make_tuple(1, 1));

		yggr_test_assert(yggr::make_tuple(1, 1) >= boost::make_tuple(1.0, 1.0));
		yggr_test_assert(yggr::make_tuple(1, 1) >= yggr::make_tuple(1.0, 1.0));

		//yggr_test_assert(yggr::make_tuple(1, 1) >= boost::make_tuple(1.0)); // not support
		//yggr_test_assert(yggr::make_tuple(1, 1) >= yggr::make_tuple(1.0)); // not support

		yggr_test_assert(yggr::make_tuple(1, 1, 1, 1, 2) >= boost::make_tuple(1.0, 1.0, 1.0, 1.0, 2.0));
		yggr_test_assert(yggr::make_tuple(1, 1, 1, 1, 2) >= yggr::make_tuple(1.0, 1.0, 1.0, 1.0, 2.0));

		{
			std::equal_to< yggr::tuple<int, int> > eq;
			bool bchk = eq(yggr::make_tuple(1, 2), yggr::make_tuple(1, 2));
			yggr_test_assert(bchk);
		}

		std::cout << yggr::make_tuple(1, 2, 3, 4, 5) << std::endl;

#	ifdef YGGR_NO_CWCHAR
		std::wcout << yggr::make_tuple(1, 2, 3, 4, 5) << std::endl;
#	endif // YGGR_NO_CWCHAR
	}
}

int main(int argc, char* argv[])
{

#ifndef YGGR_NO_CWCHAR
	TEST_PP_WIO_IMBUE("chs");
#endif // YGGR_NO_CWCHAR

	test_compare();

	wait_any_key(argc, argv);
	return 0;
}
