//network_info_test.cpp

#include <string>
#include <iostream>
#include <yggr/network/network_info.hpp>
#include <cassert>

int main(int argc, char* argv[])
{
	typedef yggr::network::network_info<yggr::u64> netinfo_type;

	netinfo_type info(yggr::u64(20));
	assert(info.owner_id() == 20);

	netinfo_type info2(boost::move(info));
	assert(info2.owner_id() == 20);

	std::string str(info2.buf(), info2.buf() + info2.buf_size());
	netinfo_type info3(boost::move(str));
	assert(info3.owner_id() == 20);

	char cc = 0;
	std::cin >> cc;
	return 0;
}
