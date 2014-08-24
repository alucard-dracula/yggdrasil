//md5_tool_test.cpp

#include <iostream>
#include <string>
#include <md5.h>

#ifdef _MSC_VER
#ifdef _DEBUG
#	pragma comment(lib, "libmd5-vc100-d.lib")
#else
#	pragma comment(lib, "libmd5-vc100.lib")
#endif // _DEBUG
#endif // _MSC_VER

#include <yggr/encryption_tool/md5_tool.hpp>

#include <yggr/packet/packet.hpp>
#include <yggr/archive/binary_archive_partner.hpp>

#ifdef _MSC_VER
#   include <vld.h>
#endif //_MSC_VER

typedef yggr::packet::packet<yggr::archive::archive_partner::binary_oarchive_partner> opak;
typedef yggr::packet::packet<yggr::archive::archive_partner::binary_iarchive_partner> ipak;

struct A
{
	A(void)
		:a(100), f(123.5f), str("Hello")
	{
	}

	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & a;
		ar & f;
		ar & str;
	}

	int a;
	float f;
	std::string str;

};



void base_test(void)
{
	std::string sig(MD5_SIZE, 0);
	std::string str = "abcdefghj";
	//std::cin >> str;

	std::string out_str(33, 0);

	md5_buffer(str.c_str(),str.length(), &sig[0]);

	md5_sig_to_string((void*)sig.data(), &out_str[0], out_str.size());

	//std::cout << sig << std::endl;
	std::cout << out_str << std::endl;

	std::string sig2(MD5_SIZE, 0);
	md5_sig_from_string(&sig2[0], out_str.c_str());

	std::string out_str2(33, 0);
	md5_sig_to_string((void*)sig2.data(), &out_str2[0], out_str2.size());
	std::cout << out_str2 << std::endl;

	yggr::encryption_tool::md5_tool tool;
	std::cout << tool.sig_to_code<std::string>(tool.encrypt(str)) << std::endl;
}

void base_test2(void)
{
	int arr[10] = {1, 2, 3, 4, 5, 6};
	std::vector<int> vt(arr, arr + 10);
	yggr::encryption_tool::md5_tool tool;

#ifndef __MINGW32__
	std::wcout.imbue(std::locale("chs"));
	std::wcout << tool.sig_to_code<std::wstring>(tool.encrypt(vt)) << std::endl;
	std::wcout << tool.sig_to_code<boost::container::wstring>(tool.encrypt(vt)) << std::endl;
#endif // __MINGW32__
}

void md5_tool_test(void)
{
	yggr::encryption_tool::md5_tool tool;
	std::string str("abcdefg");
	std::cout << tool.sig_to_code<std::string>(tool.encrypt(str)) << std::endl;
	std::string str2("abcdefg");
	std::string code(tool.sig_to_code<std::string>(tool.encrypt(str2)));
	std::cout<< tool.sig_to_code<boost::container::string>(tool.code_to_sig(code)) << std::endl;
}

// error md5 not unpak
void md5_pak_test(void)
{
	A a;
	opak o;
	o.save(a);

	yggr::encryption_tool::md5_tool tool;
	std::cout << tool.sig_to_code<std::string>(tool.encrypt(o.org_buf())) << std::endl;
}

int	main(int argc, char **argv)
{

	base_test();
	base_test2();

	md5_tool_test();

	md5_pak_test();

	char cc = 0;
	std::cin >> cc;

	return 0;
}
