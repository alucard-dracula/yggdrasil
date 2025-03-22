// test_compare_using_less.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

void test(void)
{
	int a = 10, b = 10, c = 20;

	// operator <=
	std::cout << "operator <= : " << std::endl;
	yggr_test_assert((!(b < a) == (a <= b))); // t == t
	yggr_test_assert((!(c < a) == (a <= c))); // t == t

	yggr_test_assert((!(a < b) == (b <= a))); // t == t
	yggr_test_assert((!(a < c) == (c <= a))); // f == f

	std::cout << !(b < a) << (a <= b) << std::endl; // 11
	std::cout << !(c < a) << (a <= c) << std::endl; // 11

	std::cout << !(a < b) << (b <= a) << std::endl; // 11
	std::cout << !(a < c) << (c <= a) << std::endl; // 00

	// operator >
	std::cout << "operator > : " << std::endl;
	yggr_test_assert(((b < a) == (a > b))); // f == f
	yggr_test_assert(((c < a) == (a > c))); // f == f

	yggr_test_assert(((a < b) == (b > a))); // f == f
	yggr_test_assert(((a < c) == (c > a))); // t == t

	std::cout << (b < a) << (a > b) << std::endl; // 00
	std::cout << (c < a) << (a > c) << std::endl; // 00

	std::cout << (a < b) << (b > a) << std::endl; // 00
	std::cout << (a < c) << (c > a) << std::endl; // 11

	// operator >=
	std::cout << "operator >= : " << std::endl;
	yggr_test_assert((!(a < b) == (a >= b))); // t == t
	yggr_test_assert((!(a < c) == (a >= c))); // f == f

	yggr_test_assert((!(b < a) == (b >= a))); // t == t
	yggr_test_assert((!(c < a) == (c >= a))); // t == t

	std::cout << !(a < b) << (a >= b) << std::endl; // 11
	std::cout << !(a < c) << (a >= c) << std::endl; // 00

	std::cout << !(b < a) << (b >= a) << std::endl; // 11
	std::cout << !(c < a) << (c >= a) << std::endl; // 11
}

int main(int argc, char* argv[])
{
	test();

	wait_any_key(argc, argv);
	return 0;
}