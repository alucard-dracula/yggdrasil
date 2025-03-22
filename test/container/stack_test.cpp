//stack_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>
#include <yggr/container/stack.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

void test(void)
{
	typedef std::stack<int> stl_stack_type;
	typedef yggr::container::stack<int> stack_type;

	stack_type stack;

	stack.push(1);
	stack.push(2);
	stack.emplace(3);

	stl_stack_type stl_stack = stack;

	yggr_test_assert(stack.top() == 3);
	stack.pop();
	yggr_test_assert(stack.top() == 2);
	stack.pop();
	yggr_test_assert(stack.top() == 1);
	stack.pop();
	yggr_test_assert(stack.empty());

	yggr_test_assert(stl_stack.top() == 3);
	stl_stack.pop();
	yggr_test_assert(stl_stack.top() == 2);
	stl_stack.pop();
	yggr_test_assert(stl_stack.top() == 1);
	stl_stack.pop();
	yggr_test_assert(stl_stack.empty());
}

int main(int argc, char* argv[])
{
	test();

	std::cout << "all test succeed" << std::endl;

	wait_any_key(argc, argv);
	return 0;
}