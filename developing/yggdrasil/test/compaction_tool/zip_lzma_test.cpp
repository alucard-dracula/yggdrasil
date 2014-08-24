//zip_lzma_test.cpp

#include <memory>
#include <string>
#include <cassert>
#include <iostream>

#include <yggr/compaction_tool/lzma_foo.hpp>
#include <yggr/compaction_tool/lzma_data.hpp>
#include <yggr/compaction_tool/lzma_tool.hpp>

#include <yggr/packet/packet.hpp>
#include <yggr/archive/network_archive_partner.hpp>

#ifdef _MSC_VER
#	include <vld.h>

#	ifdef _DEBUG
#		pragma comment(lib, "libyggr_compaction_tool-vc100-d.lib")
#		pragma comment(lib, "liblzma-vc100-d.lib")
#	else
#		pragma comment(lib, "libyggr_compaction_tool-vc100.lib")
#		pragma comment(lib, "liblzma-vc100.lib")
#	endif // _DEBUG

#endif // _MSC_VER

void test_lzma_base(void)
{
	unsigned char str[1024] = "123456789";
	unsigned char lzma[1024] = {0};
	unsigned char str2[1024] = {0};
	unsigned char prop[5] = {0};
	std::size_t prop_size = 5;

	std::size_t lzma_size = 1023;
	int rstzip = LzmaCompress(lzma, &lzma_size, str, strlen((const char*)str) + 1, prop, &prop_size, 
							5, 1 << 24, 3, 0, 2, 32, 2);

	if(rstzip != SZ_OK)
	{
		std::cerr << "zip error" << std::endl;
		return;
	}

	std::size_t dst_size = 10;
	int rstunzip = LzmaUncompress(str2, &dst_size, lzma, &lzma_size, prop, prop_size);

	if(rstunzip != SZ_OK)
	{
		std::cerr << "unzip error" << std::endl;
		return;
	}

	assert(memcmp(str, str2, 1024) == 0);

	std::cout << "test base ok" << std::endl;
}

void test_lzma_yggr(void)
{
	unsigned char str[1024] = "123456789";
	unsigned char lzma[1024] = {0};
	unsigned char str2[1024] = {0};
	
	yggr::compaction_tool::lzma_props props;

	//std::size_t prop_size = 5;

	std::size_t lzma_size = 1023;
	std::size_t prop_size = 5;
	int rstzip = yggr::compaction_tool::lzma_foo::compress(lzma, lzma_size, str, strlen((const char*)str) + 1, props, prop_size);

	assert(prop_size == 5);
	if(rstzip != SZ_OK)
	{
		std::cerr << "zip error" << std::endl;
		return;
	}

	std::size_t dst_size = 10;
	int rstunzip = yggr::compaction_tool::lzma_foo::uncompress(str2, dst_size, lzma, lzma_size, props);

	if(rstunzip != SZ_OK)
	{
		std::cerr << "unzip error" << std::endl;
		return;
	}

	assert(memcmp(str, str2, 1024) == 0);

	std::cout << "test yggr ok" << std::endl;
}



void test_lzma_tool(void)
{
	typedef yggr::packet::packet<yggr::archive::archive_partner::network_iarchive_partner> i_info_pak_type;
	typedef yggr::packet::packet<yggr::archive::archive_partner::network_oarchive_partner> o_info_pak_type;
	typedef yggr::compaction_tool::lzma_data<> lzma_data_type;
	typedef yggr::compaction_tool::lzma_tool<lzma_data_type> lzma_tool_type;
	typedef lzma_tool_type::lzma_buf_type buf_type;
	buf_type org_buf = "abcdefg";

	buf_type use_buf(org_buf);

	lzma_tool_type tool;

	tool.compress<o_info_pak_type>(use_buf);

	assert(org_buf != use_buf);

	tool.uncompress<i_info_pak_type>(use_buf);

	assert(org_buf == use_buf);
	std::cout << "test lzma tool ok" << std::endl;
}

int main(int argc, char* argv[])
{
	test_lzma_base();
	test_lzma_yggr();
	test_lzma_tool();
	char cc = 0;
	std::cin >> cc;
	return 0;
}