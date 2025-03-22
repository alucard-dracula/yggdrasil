//tuple_comparer_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <yggr/container/unordered_set.hpp>
#include <yggr/tuple_ex/tuple.hpp> // must befor unordered before

#include <yggr/utility/swap.hpp>

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

namespace std
{

void swap(int& l, float& r)
{
	float t = l;
	l = r;
	r = t;
}

void swap(float& l, int& r)
{
	int t = l;
	l = r;
	r = t;
}

} // namespace std

namespace boost
{

void swap(int& l, float& r)
{
	float t = l;
	l = r;
	r = t;
}

void swap(float& l, int& r)
{
	int t = l;
	l = r;
	r = t;
}

} // namespace boost

void test_swap(void)
{
	// std tuple
#if !defined(YGGR_NO_CXX11_HDR_TUPLE)
	// boost tuple
	{
		std::tuple<int, int> t1(std::make_tuple(1, 1));
		std::tuple<int, int> t2(std::make_tuple(2, 2));

		std::tuple<int, int> t3(t1);
		std::tuple<int, int> t4(t2);

		yggr_test_assert(t1 == t3);
		yggr_test_assert(t2 == t4);

		std::swap(t1, t4);
		std::swap(t2, t3);

		yggr_test_assert(std::get<0>(t1) == 2);
		yggr_test_assert(std::get<0>(t4) == 1);
		yggr_test_assert(t1 == t3);
		yggr_test_assert(t2 == t4);

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	std::swap(t1, std::make_tuple(3, 3));
	std::swap(boost::move(boost::make_tuple(3, 3)), t3);

	std::swap(t2, std::make_tuple(4, 4));
	std::swap(std::make_tuple(4, 4), t4);

	yggr_test_assert(t1 == t3);
	yggr_test_assert(t2 == t4);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

	}

	{
		boost::tuple<int, int> ti1 = boost::make_tuple(2, 2);
		boost::tuple<float, float> tf1 = boost::make_tuple(2.0f, 2.0f);

		yggr::swap(ti1, tf1);

		yggr_test_assert(ti1 == tf1);
	}
#endif // #if !defined(YGGR_NO_CXX11_HDR_TUPLE)

	// boost tuple
	{
		boost::tuple<int, int> t1(boost::make_tuple(1, 1));
		boost::tuple<int, int> t2(boost::make_tuple(2, 2));

		boost::tuple<int, int> t3(t1);
		boost::tuple<int, int> t4(t2);

		yggr_test_assert(t1 == t3);
		yggr_test_assert(t2 == t4);

		std::swap(t1, t4);
		boost::swap(t2, t3);

		yggr_test_assert(boost::get<0>(t1) == 2);
		yggr_test_assert(boost::get<0>(t4) == 1);
		yggr_test_assert(t1 == t3);
		yggr_test_assert(t2 == t4);

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	std::swap(t1, boost::make_tuple(3, 3));
	std::swap(boost::move(boost::make_tuple(3, 3)), t3);

	std::swap(t2, boost::make_tuple(4, 4));
	std::swap(boost::make_tuple(4, 4), t4);

	yggr_test_assert(t1 == t3);
	yggr_test_assert(t2 == t4);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

	}

	{
		boost::tuple<int, int> ti1 = boost::make_tuple(2, 2);
		boost::tuple<float, float> tf1 = boost::make_tuple(2.0f, 2.0f);

		yggr::swap(ti1, tf1);

		yggr_test_assert(ti1 == tf1);
	}

	// yggr tuple
	{
		yggr::tuple<int, int> t1(yggr::make_tuple(1, 1));
		yggr::tuple<int, int> t2(yggr::make_tuple(2, 2));

		yggr::tuple<int, int> t3(t1);
		yggr::tuple<int, int> t4(t2);

		yggr_test_assert(t1 == t3);
		yggr_test_assert(t2 == t4);

		std::swap(t1, t4);
		yggr::swap(t2, t3);

		yggr_test_assert(yggr::get<0>(t1) == 2);
		yggr_test_assert(yggr::get<0>(t4) == 1);
		yggr_test_assert(t1 == t3);
		yggr_test_assert(t2 == t4);

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	std::swap(t1, yggr::make_tuple(3, 3));
	std::swap(boost::move(yggr::make_tuple(3, 3)), t3);

	std::swap(t2, yggr::make_tuple(4, 4));
	std::swap(yggr::make_tuple(4, 4), t4);

	yggr_test_assert(t1 == t3);
	yggr_test_assert(t2 == t4);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

	}

	{
		yggr::tuple<int, int> ti1 = yggr::make_tuple(2, 2);
		yggr::tuple<float, float> tf1 = yggr::make_tuple(2.0f, 2.0f);

		yggr::swap(ti1, tf1);

		yggr_test_assert(ti1 == tf1);
	}

	// diff tuple swap
#if !defined(YGGR_NO_CXX11_HDR_TUPLE)
	{
		std::tuple<int, int> t1_smp = std::make_tuple(1, 1);
		boost::tuple<int, int> t2_smp = boost::make_tuple(2, 2);

		std::tuple<int, int> t1 = t1_smp;
		boost::tuple<int, int> t2 = t2_smp;

		yggr::swap(t1, t2);
		yggr_test_assert(t1 == t2_smp);
		yggr_test_assert(t2 == t1_smp);

		yggr::swap(t2, t1);
		yggr_test_assert(t1 == t1_smp);
		yggr_test_assert(t2 == t2_smp);
 	}

	{
		std::tuple<int, int> t1_smp = std::make_tuple(1, 1);
		yggr::tuple<int, int> t2_smp = yggr::make_tuple(2, 2);

		std::tuple<int, int> t1 = t1_smp;
		yggr::tuple<int, int> t2 = t2_smp;

		yggr::swap(t1, t2);
		yggr_test_assert(t1 == t2_smp);
		yggr_test_assert(t2 == t1_smp);

		yggr::swap(t2, t1);
		yggr_test_assert(t1 == t1_smp);
		yggr_test_assert(t2 == t2_smp);
 	}

#endif // #if !defined(YGGR_NO_CXX11_HDR_TUPLE)

	{
		boost::tuple<int, int> t1_smp = boost::make_tuple(1, 1);
		yggr::tuple<int, int> t2_smp = yggr::make_tuple(2, 2);

		boost::tuple<int, int> t1 = t1_smp;
		yggr::tuple<int, int> t2 = t2_smp;

		yggr::swap(t1, t2);
		yggr_test_assert(t1 == t2_smp);
		yggr_test_assert(t2 == t1_smp);

		yggr::swap(t2, t1);
		yggr_test_assert(t1 == t1_smp);
		yggr_test_assert(t2 == t2_smp);
 	}
}

void test_unordered_container(void)
{

#if !defined(YGGR_NO_CXX11_HDR_UNORDERED_SET)
#	if !defined(YGGR_NO_CXX11_HDR_TUPLE)
	{
		typedef std::tuple<int, int> tuple_type;
		typedef std::unordered_set<tuple_type> tuple_set_type;

		typedef std::pair<tuple_set_type::iterator, bool> ins_ret_type;

		tuple_set_type set;

		{
			ins_ret_type rst = set.insert(std::make_tuple(10, 10));
			yggr_test_assert(rst.second);
		}

		{
			ins_ret_type rst = set.insert(std::make_tuple(10, 10));
			yggr_test_assert(!rst.second);
		}
	
		{
			ins_ret_type rst = set.insert(std::make_tuple(10, 1));
			yggr_test_assert(rst.second);
		}
	}
	
	{
		typedef std::tuple<int, int> tuple_type;
		typedef boost::unordered_set<tuple_type> tuple_set_type;

		typedef std::pair<tuple_set_type::iterator, bool> ins_ret_type;

		tuple_set_type set;

		{
			ins_ret_type rst = set.insert(std::make_tuple(10, 10));
			yggr_test_assert(rst.second);
		}

		{
			ins_ret_type rst = set.insert(std::make_tuple(10, 10));
			yggr_test_assert(!rst.second);
		}
	
		{
			ins_ret_type rst = set.insert(std::make_tuple(10, 1));
			yggr_test_assert(rst.second);
		}
	}

#	endif // YGGR_NO_CXX11_HDR_TUPLE

	{
		typedef boost::tuple<int, int> tuple_type;
		typedef std::unordered_set<tuple_type> tuple_set_type;

		typedef std::pair<tuple_set_type::iterator, bool> ins_ret_type;

		tuple_set_type set;

		{
			ins_ret_type rst = set.insert(boost::make_tuple(10, 10));
			yggr_test_assert(rst.second);
		}

		{
			ins_ret_type rst = set.insert(boost::make_tuple(10, 10));
			yggr_test_assert(!rst.second);
		}
	
		{
			ins_ret_type rst = set.insert(boost::make_tuple(10, 1));
			yggr_test_assert(rst.second);
		}
	}
	
	{
		typedef yggr::tuple<int, int> tuple_type;
		typedef std::unordered_set<tuple_type> tuple_set_type;

		typedef std::pair<tuple_set_type::iterator, bool> ins_ret_type;

		tuple_set_type set;

		{
			ins_ret_type rst = set.insert(yggr::make_tuple(10, 10));
			yggr_test_assert(rst.second);
		}

		{
			ins_ret_type rst = set.insert(yggr::make_tuple(10, 10));
			yggr_test_assert(!rst.second);
		}
	
		{
			ins_ret_type rst = set.insert(yggr::make_tuple(10, 1));
			yggr_test_assert(rst.second);
		}
	}

#endif // YGGR_NO_CXX11_HDR_UNORDERED_SET

	{
		typedef boost::tuple<int, int> tuple_type;
		typedef boost::unordered_set<tuple_type> tuple_set_type;

		typedef std::pair<tuple_set_type::iterator, bool> ins_ret_type;

		tuple_set_type set;

		{
			ins_ret_type rst = set.insert(boost::make_tuple(10, 10));
			yggr_test_assert(rst.second);
		}

		{
			ins_ret_type rst = set.insert(boost::make_tuple(10, 10));
			yggr_test_assert(!rst.second);
		}
	
		{
			ins_ret_type rst = set.insert(boost::make_tuple(10, 1));
			yggr_test_assert(rst.second);
		}
	}

	{
		typedef yggr::tuple<int, int> tuple_type;
		typedef boost::unordered_set<tuple_type> tuple_set_type;

		typedef std::pair<tuple_set_type::iterator, bool> ins_ret_type;

		tuple_set_type set;

		{
			ins_ret_type rst = set.insert(yggr::make_tuple(10, 10));
			yggr_test_assert(rst.second);
		}

		{
			ins_ret_type rst = set.insert(yggr::make_tuple(10, 10));
			yggr_test_assert(!rst.second);
		}
	
		{
			ins_ret_type rst = set.insert(yggr::make_tuple(10, 1));
			yggr_test_assert(rst.second);
		}
	}
}

int main(int argc, char* argv[])
{

#ifndef YGGR_NO_CWCHAR
	TEST_PP_WIO_IMBUE("chs");
#endif // YGGR_NO_CWCHAR

	test_compare();
	test_swap();
	test_unordered_container();

	wait_any_key(argc, argv);
	return 0;
}
