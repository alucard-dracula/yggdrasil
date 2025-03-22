//safe_hashed_map_queue_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/container/list.hpp>

#include <yggr/safe_container/safe_hashed_map_queue.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(base)

void test_base1(void)
{
	typedef yggr::safe_container::safe_hashed_map_queue<int, int> queue_type;

	int arr[] = { 1, 2, 3, 4, 5, 6, 7 };
	int ids[] = {1, 2, 3};
	yggr::list<int> list(arr + 0, arr + 7);
	yggr::list<int> list_out;

	queue_type queue;

	queue.clear();

	for(int i = 0, isize = 7; i != isize; ++i)
	{
		queue.push(ids[i%3], arr[i]);
	}

	// discard swap
	//queue_type tmp;
	//tmp.swap(queue);
	//queue = tmp;

	int n = 0;
	for(;!queue.empty();)
	{
		queue.pop(n);
		list_out.push_back(n);
	}

	list_out.sort();

	yggr_test_assert(list == list_out);
}

int main(int argc, char* argv[])
{
	test_base1();

	wait_any_key(argc, argv);
	return 0;
}
