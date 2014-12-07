//packet_cypher_tool_test.cpp

#include <iostream>
#include <yggr/encryption_tool/blowfish_tool.hpp>
#include <yggr/encryption_tool/md5_tool.hpp>
#include <yggr/encryption_tool/safe_packet_tool.hpp>
#include <yggr/encryption_tool/packet_cypher_tool.hpp>
#include <vector>

#include <yggr/packet/packet.hpp>
#include <yggr/archive/binary_archive_partner.hpp>

#include <yggr/serialization/vector.hpp>


#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

typedef yggr::encryption_tool::blowfish_tool<> en_tool_type;
typedef yggr::encryption_tool::md5_tool chk_tool_type;
typedef yggr::encryption_tool::safe_packet_tool<en_tool_type, chk_tool_type> safe_pak_tool_type;

typedef yggr::packet::packet<yggr::archive::archive_partner::binary_oarchive_partner> opak_type;
typedef yggr::packet::packet<yggr::archive::archive_partner::binary_iarchive_partner> ipak_type;

#if 1
typedef yggr::encryption_tool::packet_cypher_tool<en_tool_type> tool_type;
#else
typedef yggr::encryption_tool::packet_cypher_tool<safe_pak_tool_type> tool_type;
#endif //1

//compile error
//void test_error(void)
//{
//	typedef yggr::encryption_tool::packet_cypher_tool<chk_tool_type> error_type;
//	error_type err;
//}

void test1(void)
{
	tool_type tool;
	tool.init();

	std::string str1 = "abcdefg";


	opak_type opak;
	opak.save(str1);

	tool.encrypt(opak);

	ipak_type ipak(opak);

	tool.decrypt(ipak);

	std::string str2;
	ipak.load(str2);

	assert(str1 == str2);
}

void test2(void)
{
	tool_type tool;
	tool.init();

	int arr[10] = {1, 2, 3, 4, 5, 6 };
	std::vector<int> vt1(arr, arr + 10);

	opak_type opak;
	opak.save(vt1);

	tool.encrypt(opak);

	ipak_type ipak(opak);

	tool.decrypt(ipak);

	std::vector<int> vt2;
	ipak.load(vt2);

	for(int i = 0; i != 10; ++i)
	{
		assert(vt1[i] == vt2[i]);
	}
}

void test3(void)
{
	tool_type tool;
	tool.init();

	double arr[10] = {1, 2, 3, 4, 5, 6 };
	std::vector<double> vt1(arr, arr + 10);
	opak_type opak;
	opak.save(vt1);

	tool.encrypt(opak);

	ipak_type ipak(opak);

	tool.decrypt(ipak);

	std::vector<double> vt2;
	ipak.load(vt2);

	for(int i = 0; i != 10; ++i)
	{
		assert(vt1[i] == vt2[i]);
	}
}

int main(int argc, char* argv[])
{
	test1();
	test2();
	test3();

	std::cout << "completed"  << std::endl;
	char cc = 0;
	std::cin >> cc;

	return 0;
}
