//zip_lzma_test.cpp


#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/archive/network_archive_partner.hpp>
#include <yggr/packet/packet.hpp>

#include <yggr/compaction_tool/lzma_foo.hpp>
#include <yggr/compaction_tool/lzma_data.hpp>
#include <yggr/compaction_tool/lzma_tool.hpp>


#include <memory>
#include <string>
#include <cassert>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(compaction_tool)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

void test_lzma_base(void)
{
	unsigned char str[1024] = "123456789";
	unsigned char lzma[1024] = {0};
	unsigned char str2[1024] = {0};
	unsigned char prop[5] = {0};
	std::size_t prop_size = 5;

	std::size_t lzma_size = 1023;
	int rstzip =
		LzmaCompress(lzma, &lzma_size, str, strlen((const char*)str) + 1, prop, &prop_size,
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

	yggr_test_assert(memcmp(str, str2, 1024) == 0);

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
	int rstzip =
		yggr::compaction_tool::lzma_foo::compress(
			lzma, lzma_size, str, strlen((const char*)str) + 1, props, prop_size);

	yggr_test_assert(prop_size == 5);
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

	yggr_test_assert(memcmp(str, str2, 1024) == 0);

	std::cout << "test yggr ok" << std::endl;
}



void test_lzma_tool(void)
{
	yggr_test_assert(false);
#if 0
	typedef yggr::packet::packet<yggr::archive::archive_partner::network_iarchive_partner> i_info_pak_type;
	typedef yggr::packet::packet<yggr::archive::archive_partner::network_oarchive_partner> o_info_pak_type;
	typedef yggr::compaction_tool::lzma_data<> lzma_data_type;
	typedef yggr::compaction_tool::lzma_tool<lzma_data_type> lzma_tool_type;
	typedef lzma_tool_type::lzma_buf_type buf_type;
	buf_type org_buf = "abcdefg";

	buf_type use_buf(org_buf);

	lzma_tool_type tool;

	tool.compress<o_info_pak_type>(use_buf);

	yggr_test_assert(org_buf != use_buf);

	tool.uncompress<i_info_pak_type>(use_buf);

	yggr_test_assert(org_buf == use_buf);
	std::cout << "test lzma tool ok" << std::endl;

#endif // 0, 1
}

int main(int argc, char* argv[])
{
	test_lzma_base();
	test_lzma_yggr();
	//test_lzma_tool();

	wait_any_key(argc, argv);
	return 0;
}
