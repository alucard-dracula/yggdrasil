// create_object_test.cpp

#include <typeinfo>
#include <cassert>
#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/base/obj_creator.hpp>
#include <yggr/charset/string.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

void test(void)
{
	yggr_test_assert((yggr::create_object<int>() == 0));
	yggr_test_assert((yggr::create_object<int>(1) == 1));

	yggr_test_assert((yggr::static_create_object<int>() == 0));
	yggr_test_assert((yggr::static_create_object<int>(1) == 1));
	yggr_test_assert((yggr::static_create_object<int>(2) == 1));

	yggr_test_assert((yggr::static_const_create_object<int>() == 0));
	yggr_test_assert((yggr::static_const_create_object<int>(1) == 1));
	yggr_test_assert((yggr::static_const_create_object<int>(2) == 1));

	yggr_test_assert((typeid(yggr::create_object<void>()) == typeid(void)));
	yggr_test_assert((typeid(yggr::create_object<void>(1, 2, 3, 4)) == typeid(void)));

	yggr_test_assert((typeid(yggr::static_create_object<void>()) == typeid(void)));
	yggr_test_assert((typeid(yggr::static_create_object<void>(1, 2, 3, 4)) == typeid(void)));

	yggr_test_assert((typeid(yggr::static_const_create_object<void>()) == typeid(void)));
	yggr_test_assert((typeid(yggr::static_const_create_object<void>(1, 2, 3, 4)) == typeid(void)));

	yggr_test_assert((yggr::create_object<yggr::string>() == yggr::string()));
	yggr_test_assert((yggr::create_object<yggr::string>("abc") == yggr::string("abc")));

	yggr_test_assert((yggr::static_create_object<yggr::string>() == yggr::string()));
	yggr_test_assert((yggr::static_create_object<yggr::string>("abc") == yggr::string("abc")));
	yggr_test_assert((yggr::static_create_object<yggr::string>("def") == yggr::string("abc")));

	yggr_test_assert((yggr::static_const_create_object<yggr::string>() == yggr::string()));
	yggr_test_assert((yggr::static_const_create_object<yggr::string>("abc") == yggr::string("abc")));
	yggr_test_assert((yggr::static_const_create_object<yggr::string>("def") == yggr::string("abc")));
}

int main(int argc, char* argv[])
{
	test();
	
	wait_any_key(argc, argv);
	return 0;
}