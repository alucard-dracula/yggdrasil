// queue_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>
#include <yggr/container/queue.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

void test_queue(void)
{
	typedef std::queue<int> stl_queue_type;
	typedef yggr::container::queue<int> queue_type;

	queue_type queue;

	queue.push(1);
	queue.push(2);
	queue.emplace(3);

	stl_queue_type stl_queue = queue;

	yggr_test_assert(queue.front() == 1);
	yggr_test_assert(queue.back() == 3);
	queue.pop();
	yggr_test_assert(queue.front() == 2);
	yggr_test_assert(queue.back() == 3);
	queue.pop();
	yggr_test_assert(queue.front() == 3);
	yggr_test_assert(queue.back() == 3);
	queue.pop();
	yggr_test_assert(queue.empty());

	yggr_test_assert(stl_queue.front() == 1);
	yggr_test_assert(stl_queue.back() == 3);
	stl_queue.pop();
	yggr_test_assert(stl_queue.front() == 2);
	yggr_test_assert(stl_queue.back() == 3);
	stl_queue.pop();
	yggr_test_assert(stl_queue.front() == 3);
	yggr_test_assert(stl_queue.back() == 3);
	stl_queue.pop();
	yggr_test_assert(stl_queue.empty());
}

void test_priority_queue(void)
{
	typedef std::priority_queue<int> stl_queue_type;
	typedef yggr::container::priority_queue<int> queue_type;

	queue_type queue;

	queue.emplace(3);
	queue.push(1);
	queue.emplace(2);
	
	stl_queue_type stl_queue = queue;

	yggr_test_assert(queue.top() == 3);
	queue.pop();
	yggr_test_assert(queue.top() == 2);
	queue.pop();
	yggr_test_assert(queue.top() == 1);
	queue.pop();
	yggr_test_assert(queue.empty());

	yggr_test_assert(stl_queue.top() == 3);
	stl_queue.pop();
	yggr_test_assert(stl_queue.top() == 2);
	stl_queue.pop();
	yggr_test_assert(stl_queue.top() == 1);
	stl_queue.pop();
	yggr_test_assert(stl_queue.empty());
}

int main(int argc, char* argv[])
{
	test_queue();
	test_priority_queue();

	wait_any_key(argc, argv);
	return 0;
}