//link_table_save_test.cpp

#ifdef _DEBUG
#include <vld.h>
#endif // _DEBUG
//#pragma comment(lib, "vld.lib")

#include <iostream>
#include <list>

#include <yggr/normal_struct/link_table.hpp>
#include <yggr/normal_struct/link_table_io_helper.hpp>

#include <yggr/packet/packet.hpp>
#include <yggr/archive/binary_archive_partner.hpp>
#include <fstream>
#include <yggr/file_system/local_file_operator.hpp>

typedef yggr::normal_struct::link_table<int, int> link_table_type;
typedef yggr::normal_struct::link_table_io_helper<int, link_table_type> link_table_io_helper_type;

typedef yggr::packet::packet<yggr::archive::archive_partner::binary_oarchive_partner> opak_type;
typedef yggr::packet::packet<yggr::archive::archive_partner::binary_iarchive_partner> ipak_type;

int main(int argc, char* argv[])
{
	link_table_type lt;
	lt.add_relation(1, 2, 1, link_table_type::E_RELATION_BOTH);
	lt.add_relation(2, 3, 1, link_table_type::E_RELATION_BOTH);

	link_table_io_helper_type helper;

	lt.get_all_relation(helper);

	link_table_type lt2;

	lt2.set_all_relation(helper);

	opak_type opak;
	opak.save(helper);

	ipak_type ipak(opak);

	link_table_io_helper_type helper2;

	ipak.load(helper2);

	link_table_type lt3;
	lt3.set_all_relation(helper2);

	char cc = 0;
	std::cin >> cc;
	return 0;
}
