//network_info_test.cpp

#include <string>
#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/network/network_info.hpp>
#include <cassert>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

int main(int argc, char* argv[])
{
	typedef yggr::network::network_info<yggr::u64> netinfo_type;

	netinfo_type info(yggr::u64(20));
	yggr_test_assert(info.owner_id() == 20);

	netinfo_type info2(boost::move(info));
	yggr_test_assert(info2.owner_id() == 20);

	std::string str(info2.buf(), info2.buf() + info2.buf_size());
	netinfo_type info3(boost::move(str));
	yggr_test_assert(info3.owner_id() == 20);

	info3.push(yggr::u64(30));
	yggr_test_assert(info3.owner_id() == 30);

	yggr_test_assert(info3.size() == 2);
	yggr_test_assert(info3.owner_id(0) == info3.front());
	yggr_test_assert(info3.owner_id(1) == info3.back());

	yggr::u64 id = info3.pop();
	yggr_test_assert(id == 30);
	yggr_test_assert(info3.owner_id() == 20);

	id = info3.pop();
	yggr_test_assert(id == 20);
	yggr_test_assert(info3.buf_size() == yggr::mplex::strict_sizeof<netinfo_type::time_type>::value);

#if !defined(_DEBUG)
	yggr_test_assert(info3.owner_id() == 0);
#endif // _DEBUG

	std::cout << std::hex << info3.time() << std::endl;

	wait_any_key(argc, argv);
	return 0;
}
