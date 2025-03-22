//md5_tool_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/charset/string.hpp>
#include <yggr/encryption_tool/md5_tool.hpp>

#include <yggr/packet/packet.hpp>
#include <yggr/archive/binary_archive_partner.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(encryption_tool)
#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(base)

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

	std::string out_str(32, 0);

	md5_buffer(str.c_str(), static_cast<yggr::u32>(str.length()), &sig[0]);

	md5_sig_to_string(reinterpret_cast<void*>(const_cast<char*>(sig.data())), 
						&out_str[0], static_cast<yggr::u32>(out_str.size()));

	//std::cout << sig << std::endl;
	std::cout << out_str << std::endl;

	std::string sig2(MD5_SIZE, 0);
	md5_sig_from_string(&sig2[0], out_str.c_str());

	std::string out_str2(32, 0);
	md5_sig_to_string(reinterpret_cast<void*>(const_cast<char*>(sig2.data())), 
						&out_str2[0], static_cast<yggr::u32>(out_str2.size()));
	std::cout << out_str2 << std::endl;

	yggr::encryption_tool::md5_tool tool;
	std::cout << tool.sig_to_code<std::string>(tool.encrypt(str)) << std::endl;

	std::cout << "-----------base_test succeed---------------" << std::endl;
}

void base_test2(void)
{
	int arr[10] = {1, 2, 3, 4, 5, 6};
	std::vector<int> vt(arr + 0, arr + 10);
	yggr::encryption_tool::md5_tool tool;

	std::cout << tool.sig_to_code<std::string>(tool.encrypt(vt)) << std::endl;
	std::cout << tool.sig_to_code<boost::container::string>(tool.encrypt(vt), 1) << std::endl;
	std::cout << tool.sig_to_code<yggr::utf8_string>(tool.encrypt(vt), 1) << std::endl;

#ifndef YGGR_NO_CWCHAR
	std::wcout << tool.sig_to_code<std::wstring>(tool.encrypt(vt)) << std::endl;
	std::wcout << tool.sig_to_code<boost::container::wstring>(tool.encrypt(vt), 1) << std::endl;
#endif // YGGR_NO_CWCHAR

	std::cout << "-----------base_test2 succeed---------------" << std::endl;
}

void base_test3(void)
{
	yggr::string vt;
	yggr::encryption_tool::md5_tool tool;

	std::cout << tool.sig_to_code<std::string>(tool.encrypt(vt)) << std::endl;

#ifndef YGGR_NO_CWCHAR
	std::wcout << tool.sig_to_code<std::wstring>(tool.encrypt(vt)) << std::endl;
#endif // YGGR_NO_CWCHAR

	std::cout << "-----------base_test3 succeed---------------" << std::endl;
}

void md5_tool_test(void)
{
	yggr::encryption_tool::md5_tool tool;
	std::string str("abcdefg");
	std::cout << tool.sig_to_code<std::string>(tool.encrypt(str)) << std::endl;
	std::string str2("abcdefg");
	std::string code(tool.sig_to_code<std::string>(tool.encrypt(str2)));
	std::cout<< tool.sig_to_code<boost::container::string>(tool.code_to_sig(code).first) << std::endl;

	std::cout << tool.sig_to_code<boost::container::string>(
					tool.code_to_sig(
						tool.sig_to_code<std::string>(
							tool.code_to_sig(code).first, 1 ) ).first ) << std::endl;

	std::cout << "-----------md5_tool_test succeed---------------" << std::endl;
}

// error md5 not unpak
void md5_pak_test(void)
{
	A a;
	opak o;
	o.save(a);

	yggr::encryption_tool::md5_tool tool;
	std::cout << tool.sig_to_code<std::string>(tool.encrypt(o.org_buf())) << std::endl;

	std::cout << "-----------md5_pak_test succeed---------------" << std::endl;
}

int	main(int argc, char **argv)
{

#ifndef YGGR_NO_CWCHAR
	TEST_PP_WIO_IMBUE("chs");
#endif // YGGR_NO_CWCHAR

	base_test();
	base_test2();
	base_test3();

	md5_tool_test();
	md5_pak_test();

	wait_any_key(argc, argv);
	return 0;
}
