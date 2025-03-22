// gen_callable_object_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/caller/gen_callable_object.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

void foo(void)
{
	std::cout << "foo" << std::endl;
}

int foo1(int n)
{
	return n + 1;
}

struct like_foo
{
	void operator()(void) const
	{
		std::cout << "like_foo" << std::endl;
	}
};

struct member_foo
{
	void foo(void) const
	{
		std::cout << "member_foo" << std::endl;
	}
};

int main(int argc, char* argv[])
{

	yggr::caller::gen_callable_object(&foo)();
	yggr_test_assert((11 == yggr::caller::gen_callable_object(&foo1)(10)));

	yggr::caller::gen_callable_object(like_foo())();

	member_foo m;
	//(m.(yggr::caller::gen_callable_object(&member_foo::foo)))();
	((&m)->*(&member_foo::foo))();
	(m.*(&member_foo::foo))();

	wait_any_key(argc, argv);
	return 0;
}