//bson_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#define BUILD_MONGODB_DRIVER_YGGR_VER

#include <bson.h>
#include <yggr/nsql_database_system/c_bson.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(charset)

void test_base(void)
{
	bson_t /*b1,*/ b2 = {0};
	bson_init(&b2);
	bson_append_int32(&b2, "aaa", -1, 10);

	std::cout << b2 << std::endl;

	bson_destroy(&b2);

	std::cout << "completed" << std::endl;
}

void test_base2(void)
{
	bson_t /*b1,*/ b2;
	yggr::nsql_database_system::bson_native_ex::s_bson_init(&b2);

	char buf[] = {'a', 'b', 'c'};

	bson_append_binary(&b2, "binary", -1, BSON_SUBTYPE_BINARY, reinterpret_cast<const yggr::u8*>(buf), 0);
	std::cout << b2 << std::endl;

	bson_destroy(&b2);

	std::cout << "completed" << std::endl;
}

void test_base3(void)
{
	bson_t /*b1,*/ b2 = {0};
	bson_init(&b2);

	const char* str = "abc";

	bson_append_utf8(&b2, "utf8", -1, str, -1);
	std::cout << b2 << std::endl;

	bson_iter_t iter = {0};
	bson_iter_init(&iter, &b2);

	do
	{
		yggr::u32 len = 0;
		char* p = bson_iter_dup_utf8(&iter, &len);

		if(p)
		{
			std::cout << p << std::endl;
		}

		bson_free(p);

	} while(bson_iter_next(&iter));

	bson_destroy(&b2);

	std::cout << "completed" << std::endl;
}


int main(int argc, char* argv[])
{
	test_base();
	test_base2();
	test_base3();

	wait_any_key(argc, argv);
	return 0;
}
