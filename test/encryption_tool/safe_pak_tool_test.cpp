//safe_pak_tool_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/container/vector.hpp>

#include <yggr/encryption_tool/blowfish_tool.hpp>
#include <yggr/encryption_tool/md5_tool.hpp>
#include <yggr/encryption_tool/safe_packet_tool.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(encryption_tool)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

typedef yggr::encryption_tool::blowfish_tool<> en_tool_type;
typedef yggr::encryption_tool::md5_tool chk_tool_type;
typedef yggr::encryption_tool::safe_packet_tool<en_tool_type, chk_tool_type> tool_type;


void test1(void)
{
	tool_type tool;
	tool.init();

	std::string str1 = "abcdefg";
	std::string str2 = str1;

	tool.encrypt(str2);
	yggr_test_assert(str1 != str2);

	tool.decrypt(str2);
	yggr_test_assert(str1 == str2);
}

void test2(void)
{
	tool_type tool;
	tool.init();

	int arr[10] = {1, 2, 3, 4, 5, 6 };
	std::vector<int> vt1(arr + 0, arr + 10);
	std::vector<int> vt2 = vt1;

	tool.encrypt(vt2);

	tool.decrypt(vt2);

	for(int i = 0; i != 10; ++i)
	{
		yggr_test_assert(vt1[i] == vt2[i]);
	}
}

void test3(void)
{
	tool_type tool;
	tool.init();

	double arr[10] = {1, 2, 3, 4, 5, 6 };
	std::vector<double> vt1(arr + 0, arr + 10);
	std::vector<double> vt2 = vt1;

	tool.encrypt(vt2);

	tool.decrypt(vt2);

	for(int i = 0; i != 10; ++i)
	{
		yggr_test_assert(vt1[i] == vt2[i]);
	}
}

int main(int argc, char* argv[])
{
	test1();
	test2();
	test3();

	wait_any_key(argc, argv);
	return 0;
}
