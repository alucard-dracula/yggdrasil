//hashed_link_table_save_test.cpp

#ifdef _DEBUG
#include <vld.h>
#endif // _DEBUG
//#pragma comment(lib, "vld.lib")

#include <iostream>
#include <list>

#include <yggr/normal_struct/hashed_link_table.hpp>
#include <yggr/normal_struct/link_table_io_helper.hpp>

#include <yggr/packet/packet.hpp>
#include <yggr/archive/binary_archive_partner.hpp>
#include <fstream>
#include <yggr/file_system/local_file_operator.hpp>

class store_int : public std::pair<int, int>
{
public:
	typedef std::pair<int, int> base_type;

	store_int(void)
	{
	}

	store_int(int nkey, int nn)
		: base_type(nkey, nn)
	{
	}

	store_int(const store_int& right)
		: base_type(right)
	{
	}

	~store_int(void)
	{
	}

	bool operator==(const store_int& right) const
	{
		return base_type::first == right.first;
	}

private:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, yggr::u32 version)
	{
		ar & first;
		ar & second;
	}
};

typedef yggr::normal_struct::hashed_link_table<int, int, int> link_table_type;
typedef yggr::normal_struct::link_table_io_helper<store_int, link_table_type> link_table_io_helper_type;

typedef yggr::packet::packet<yggr::archive::archive_partner::binary_oarchive_partner> opak_type;
typedef yggr::packet::packet<yggr::archive::archive_partner::binary_iarchive_partner> ipak_type;

int main(int argc, char* argv[])
{
	link_table_type lt;
	lt.add_relation(1, 1, 2, 2, 10, link_table_type::E_RELATION_BOTH);
	lt.add_relation(2, 2, 1, 1, 10, link_table_type::E_RELATION_BOTH);
	lt.add_relation(1, 1, 2, 2, 10, link_table_type::E_RELATION_BOTH);
	lt.add_relation(1, 1, 2, 2, 10, link_table_type::E_RELATION_TO);
	lt.add_relation(1, 1, 2, 2, 10, link_table_type::E_RELATION_FROM);
	lt.add_relation(2, 2, 3, 3, 14, link_table_type::E_RELATION_BOTH);
	lt.add_relation(4, 4, 5, 5, 10, link_table_type::E_RELATION_BOTH);

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

	link_table_type::link_table_ptr_list_type list;
	lt3.get_passed_sub_link_table(list);

	char cc = 0;
	std::cin >> cc;
	return 0;
}
