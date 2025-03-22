// char16_32_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/base/yggrdef.h>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

int main(int argc, char* argv[])
{
#if YGGR_CPP_VER_11 <= YGGR_CPP_VERSION

	{
		char16_t a = 'a';
		char32_t b = 'b';

		assert(a == 'a');
		assert(b == 'b');
	}

#else

    std::cout << "cpp03 and older not support char16_t char32_t" << std::endl;

#endif // YGGR_CPP_VER_11 <= YGGR_CPP_VERSION

	wait_any_key(argc, argv);
	return 0;
}
