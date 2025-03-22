//packet_info_key_test.hpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <boost/unordered_map.hpp>
#include <yggr/packet/packet_info.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

int main(int argc, char* argv[])
{
	typedef yggr::packet::packet_info<yggr::u32, yggr::u16> info_type;
	typedef boost::unordered_map<info_type, int> map_type;
	typedef map_type::value_type ins_val_type;

	map_type map;
	map.insert(ins_val_type(info_type(), 0));

	yggr_test_assert(map.size() == 1);

	wait_any_key(argc, argv);
	return 0;
}