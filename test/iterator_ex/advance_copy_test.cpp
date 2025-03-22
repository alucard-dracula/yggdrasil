// advance_copy_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <yggr/base/yggrdef.h>
#include <yggr/container/vector.hpp>
#include <yggr/container/list.hpp>

#include <yggr/iterator_ex/iterator.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

void test(void)
{
	{
		typedef yggr::vector<yggr::s32> cont_type;
		cont_type cont;
		cont.push_back(10);
		cont.push_back(20);
		cont.push_back(30);

		yggr_test_assert((yggr::advance_copy(cont.begin(), 0) == cont.begin()));

		yggr_test_assert((*yggr::advance_copy(cont.begin(), 0) == 10));
		yggr_test_assert((*yggr::advance_copy(cont.begin(), 1) == 20));
		yggr_test_assert((*yggr::advance_copy(cont.begin(), 2) == 30));

		yggr_test_assert((yggr::advance_copy(cont.begin(), 3) == cont.end()));
	}

	{
		typedef yggr::list<yggr::s32> cont_type;
		cont_type cont;
		cont.push_back(10);
		cont.push_back(20);
		cont.push_back(30);

		yggr_test_assert((yggr::advance_copy(cont.begin(), 0) == cont.begin()));

		yggr_test_assert((*yggr::advance_copy(cont.begin(), 0) == 10));
		yggr_test_assert((*yggr::advance_copy(cont.begin(), 1) == 20));
		yggr_test_assert((*yggr::advance_copy(cont.begin(), 2) == 30));

		yggr_test_assert((yggr::advance_copy(cont.begin(), 3) == cont.end()));
	}
}

int main(int argc, char* argv[])
{
	test();

	wait_any_key(argc, argv);
	return 0;
}