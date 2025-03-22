//sha1_tool_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/charset/string.hpp>
#include <yggr/encryption_tool/sha1_tool.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(encryption_tool)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

int main(int argc, char* argv[])
{
	typedef yggr::encryption_tool::sha1_tool tool_type;

#ifndef YGGR_NO_CWCHAR
	TEST_PP_WIO_IMBUE("chs");
#endif // YGGR_NO_CWCHAR

	tool_type tool;
	std::string data("The quick brown fox jumps over the lazy dog");

	std::cout << tool.sig_to_code<std::string>(tool.encrypt(data)) << std::endl;

	tool_type::sig_type sig = tool.encrypt(data);
	yggr::string code = tool.sig_to_code<yggr::string>(sig);
	std::cout << code << std::endl;
	tool_type::sig_type sig2 = tool.code_to_sig(code).first;
	yggr_test_assert(0 == memcmp(&sig[0], &sig2[0], tool.sig_size()));
	yggr::string code2 = tool.sig_to_code(sig2);
	std::cout << code2 << std::endl;

	std::cout << tool.sig_to_code<std::string>(tool.encrypt(data)) << std::endl;
	std::cout << tool.sig_to_code<boost::container::string>(tool.encrypt(data), 1) << std::endl;
	std::cout << tool.sig_to_code<yggr::utf8_string>(tool.encrypt(data), 1) << std::endl;
	std::cout << tool.sig_to_code<yggr::utf8_string>(tool.encrypt(data), code.get_allocator(), 1) << std::endl;

#ifndef YGGR_NO_CWCHAR
	std::wcout << tool.sig_to_code<std::wstring>(tool.encrypt(data)) << std::endl;
	std::wcout << tool.sig_to_code<boost::container::wstring>(tool.encrypt(data), 1) << std::endl;

	sig2 = tool.code_to_sig(tool.sig_to_code<std::wstring>(tool.encrypt(data))).first;
	yggr_test_assert(0 == memcmp(&sig[0], &sig2[0], tool.sig_size()));

	sig2 = tool.code_to_sig(tool.sig_to_code<boost::container::wstring>(tool.encrypt(data))).first;
	yggr_test_assert(0 == memcmp(&sig[0], &sig2[0], tool.sig_size()));

#endif // YGGR_NO_CWCHAR

	wait_any_key(argc, argv);
	return 0;
}