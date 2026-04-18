// default_value_comparer_test.cpp

#include <yggr/base/yggrdef.h>
#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/utility/default_value_comparer.hpp>

#include <cassert>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER


int main(int argc, char *argv[])
{
	bool bchk = false;

	// equal_to
	{
		bchk = yggr::equal_to_default_value(0);
		assert(bchk);
		std::cout << "yggr::equal_to_default_value(0) " << bchk << std::endl;
	}

	// not_equal_to
	{
		bchk = yggr::not_equal_to_default_value(1);
		assert(bchk);
		std::cout << "yggr::equal_to_default_value(1) " << bchk << std::endl;
	}

	// less
	{
		bchk = yggr::less_default_value(-1);
		assert(bchk);
		std::cout << "yggr::less_default_value(-1) " << bchk << std::endl;
	}

	// less_equal
	{
		bchk = yggr::less_equal_default_value(-1);
		assert(bchk);
		std::cout << "yggr::less_equal_default_value(-1) " << bchk << std::endl;
	}

	{
		bchk = yggr::less_equal_default_value(0);
		assert(bchk);
		std::cout << "yggr::less_equal_default_value(0) " << bchk << std::endl;
	}

	// greater
	{
		bchk = yggr::greater_default_value(1);
		assert(bchk);
		std::cout << "yggr::greater_default_value(1) " << bchk << std::endl;
	}

	// greater_equal
	{
		bchk = yggr::greater_equal_default_value(1);
		assert(bchk);
		std::cout << "yggr::greater_equal_default_value(1) " << bchk << std::endl;
	}

	{
		bchk = yggr::greater_equal_default_value(0);
		assert(bchk);
		std::cout << "yggr::greater_equal_default_value(1) " << bchk << std::endl;
	}

	wait_any_key(argc, argv);
	return 0;
}
