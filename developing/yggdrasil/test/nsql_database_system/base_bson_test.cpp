//bson_test.cpp

#define MONGO_USE_LONG_LONG_INT
#define MONGO_STATIC_BUILD

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#include <iostream>
#include <bson.h>

#ifdef _MSC_VER
#pragma comment(lib, "libbson-vc100-d.lib")
#endif // _MSC_VER
int main(int argc, char* argv[])
{
	bson b1, b2;
	bson_empty(&b1);
	bson_init(&b2);
	bson_append_int(&b2, "aaa", 10);

	//bson_destroy(&b1); // b1 is empty don't need destory
	bson_destroy(&b2);

	std::cout << "completed" << std::endl;

	char cc = 0;
	std::cin >> cc;
	return 0;
}
