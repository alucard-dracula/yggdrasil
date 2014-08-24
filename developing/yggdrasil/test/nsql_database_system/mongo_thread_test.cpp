//mongo_thread_test.cpp

#error "this example need build data"
#include <iostream>

#include <yggr/nsql_database_system/mongodb_config.h>
#include <yggr/nsql_database_system/c_bson.hpp>
#include <yggr/nsql_database_system/mongo_accesser.hpp>

#include <yggr/packet/packet.hpp>

#include <yggr/archive/network_archive_partner.hpp>
#include <yggr/archive/bson_archive_partner.hpp>

typedef yggr::packet::packet<yggr::archive::archive_partner::network_oarchive_partner> opacket_type;
typedef yggr::packet::packet<yggr::archive::archive_partner::network_iarchive_partner> ipacket_type;

typedef yggr::packet::packet<yggr::archive::archive_partner::bson_oarchive_partner> o_bson_packet_type;
typedef yggr::packet::packet<yggr::archive::archive_partner::bson_iarchive_partner> i_bson_packet_type;

typedef yggr::nsql_database_system::c_mongo_connection connection_type;
typedef yggr::nsql_database_system::mongo_accesser<connection_type> accesser_type;

typedef yggr::ptr_single<accesser_type> accesser_single;
typedef accesser_single::obj_ptr_type accesser_ptr;

typedef std::vector<o_bson_packet_type> o_pak_vt_type;
typedef std::vector<i_bson_packet_type> i_pak_vt_type;

typedef yggr::nsql_database_system::c_bson c_bson;


void item_runner()
{
	accesser_ptr paccesser = accesser_single::get_ins();
	yggr::u32 i = 0;
	while(1)
	{
		c_bson b1,b2(true);
		b1.save("uid", i);
		b1.finish();

		i_bson_packet_type pak;
		if(!paccesser->find_one(pak, std::string("item"), std::string("user"), b1, b2))
		{
			std::cout << "fail" << std::endl;
		}

		++i;
	}
};

void skill_runner()
{
	accesser_ptr paccesser = accesser_single::get_ins();
	yggr::u32 i = 0;
	while(1)
	{
		c_bson b1, b2(true);
		b1.save("uid", i);
		b1.finish();

		i_bson_packet_type pak;
		if(!paccesser->find_one(pak, std::string("skill"), std::string("user"), b1, b2))
		{
			std::cout << "fail" << std::endl;
		}

		++i;
	}
};

void mission_runner()
{
	accesser_ptr paccesser = accesser_single::get_ins();
	yggr::u32 i = 0;
	while(1)
	{
		c_bson b1, b2(true);
		b1.save("uid", i);
		b1.finish();

		i_bson_packet_type pak;
		if(!paccesser->find_one(pak, std::string("mission_produce"), std::string("user"), b1, b2))
		{
			std::cout << "fail" << std::endl;
		}

		++i;
	}
};

void friends_runner()
{
	accesser_ptr paccesser = accesser_single::get_ins();
	yggr::u32 i = 0;
	while(1)
	{
		c_bson b1, b2(true);
		b1.save("uid", i);
		b1.finish();

		i_bson_packet_type pak;
		if(!paccesser->find_one(pak, std::string("friends"), std::string("user"), b1, b2))
		{
			std::cout << "fail" << std::endl;
		}

		++i;
	}
};

int main(int argc, char* argv[])
{
	connection_type::this_ptr_type pconn(new connection_type("127.0.0.1:10098"));
	pconn->connect();
	accesser_single::init_ins(pconn);

	boost::thread trd1(boost::bind(&item_runner));
	boost::thread trd2(boost::bind(&item_runner));
	boost::thread trd3(boost::bind(&skill_runner));
	boost::thread trd4(boost::bind(&skill_runner));
	boost::thread trd5(boost::bind(&mission_runner));
	boost::thread trd6(boost::bind(&mission_runner));
	boost::thread trd7(boost::bind(&friends_runner));
	boost::thread trd8(boost::bind(&friends_runner));

	char c = 0;
	std::cin >> c;

	return 0;
}
