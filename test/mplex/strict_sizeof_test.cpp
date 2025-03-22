//strict_sizeof_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>
#include <yggr/mplex/strict_sizeof.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#define TEST_STRICT_SIZEF(__type__) \
	yggr_test_assert(yggr::mplex::strict_sizeof<__type__>::value == sizeof(__type__))


class A
{
};

struct B
{
};

union C
{
	int n;
	float m;
};

int main(int argc, char* argv[])
{
	TEST_STRICT_SIZEF(char);
	TEST_STRICT_SIZEF(wchar_t);
	TEST_STRICT_SIZEF(yggr::s8);
	TEST_STRICT_SIZEF(yggr::u8);
	TEST_STRICT_SIZEF(yggr::s16);
	TEST_STRICT_SIZEF(yggr::u16);
	TEST_STRICT_SIZEF(yggr::s32);
	TEST_STRICT_SIZEF(yggr::u32);

	//TEST_STRICT_SIZEF(A); // static assert false
	//TEST_STRICT_SIZEF(B); // static assert false

	TEST_STRICT_SIZEF(C);
	TEST_STRICT_SIZEF(A*);
	TEST_STRICT_SIZEF(B*);

	wait_any_key(argc, argv);
	return 0;
}
