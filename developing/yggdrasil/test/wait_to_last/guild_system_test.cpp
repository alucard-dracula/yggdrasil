//guild_system_test.cpp

#include <iostream>
#include <assert.h>

#include <guild/guild.hpp>
#include <guild/guild_security.hpp>

#ifdef _WIN32
#   include <vld.h>
#endif //_WIN32

void test_guild(void)
{
	typedef game::guild::guild_technology::level_type thy_level_type;
	typedef game::guild::guild_technology::level_vt_type thy_level_vt_type;
	typedef game::guild::guild_technology::val_vt_type thy_val_vt_type;
	typedef game::guild::guild_technology::val_vt_vt_type thy_val_vt_vt_type;

	typedef game::guild::guild::technology_honour_type thy_honour_type;
	typedef game::guild::guild::technology_honour_vt_type thy_honour_vt_type;
	typedef game::guild::guild::technology_honour_vt_vt_type thy_honour_vt_vt_type;

	thy_honour_vt_type thy_honour_pow_vt;
	thy_honour_pow_vt.push_back(thy_honour_type(1, 500));
	thy_honour_pow_vt.push_back(thy_honour_type(1, 2000));
	thy_honour_pow_vt.push_back(thy_honour_type(1, 4000));
	thy_honour_pow_vt.push_back(thy_honour_type(1, 6500));
	thy_honour_pow_vt.push_back(thy_honour_type(1, 9800));

	thy_honour_vt_type thy_honour_def_vt;
	thy_honour_def_vt.push_back(thy_honour_type(1, 600));
	thy_honour_def_vt.push_back(thy_honour_type(1, 2100));
	thy_honour_def_vt.push_back(thy_honour_type(1, 4200));
	thy_honour_def_vt.push_back(thy_honour_type(1, 6800));
	thy_honour_def_vt.push_back(thy_honour_type(1, 10000));

	thy_honour_vt_type thy_honour_pot_vt;
	thy_honour_pot_vt.push_back(thy_honour_type(1, 550));
	thy_honour_pot_vt.push_back(thy_honour_type(1, 2000));
	thy_honour_pot_vt.push_back(thy_honour_type(1, 4200));
	thy_honour_pot_vt.push_back(thy_honour_type(1, 7000));
	thy_honour_pot_vt.push_back(thy_honour_type(1, 11000));

	thy_honour_vt_type thy_honour_cure_vt;
	thy_honour_cure_vt.push_back(thy_honour_type(1, 550));
	thy_honour_cure_vt.push_back(thy_honour_type(1, 2000));
	thy_honour_cure_vt.push_back(thy_honour_type(1, 4200));
	thy_honour_cure_vt.push_back(thy_honour_type(1, 7000));
	thy_honour_cure_vt.push_back(thy_honour_type(1, 11000));

	thy_honour_vt_vt_type thy_honour_vt_vt;
	thy_honour_vt_vt.push_back(thy_honour_pow_vt);
	thy_honour_vt_vt.push_back(thy_honour_def_vt);
	thy_honour_vt_vt.push_back(thy_honour_pot_vt);
	thy_honour_vt_vt.push_back(thy_honour_cure_vt);

	game::guild::guild::member_id_type uid = 0x0000000000000001;

	game::guild::guild::level_init_type lv_init(1, 10);
	game::guild::guild::devil_soul_init_type devil_soul_init(0, 9999999);

	game::guild::guild::area_captures_init_type areas_init;
	game::guild::guild::recipe_captures_init_type recipe_init;
	game::guild::guild::description_init_type description_init(yggr::charset::utf8_string("天秀网络科技"),
																	yggr::charset::utf8_string("赶快把项目做完"));

	thy_level_type thy_lv(0, 5);

	thy_level_vt_type thy_level_vt;
	thy_level_vt.push_back(thy_lv);
	thy_level_vt.push_back(thy_lv);
	thy_level_vt.push_back(thy_lv);
	thy_level_vt.push_back(thy_lv);


	thy_val_vt_type th_val_vt;
	th_val_vt.push_back(10);
	th_val_vt.push_back(20);
	th_val_vt.push_back(30);
	th_val_vt.push_back(40);
	th_val_vt.push_back(50);

	thy_val_vt_vt_type thy_val_vt_vt;

	thy_val_vt_vt.push_back(th_val_vt);
	thy_val_vt_vt.push_back(th_val_vt);
	thy_val_vt_vt.push_back(th_val_vt);
	thy_val_vt_vt.push_back(th_val_vt);

	game::guild::guild::technology_init_type technology_init(thy_level_vt, thy_val_vt_vt);

	game::guild::guild::devil_soul_val_vt_type devil_soul_val_vt;

	devil_soul_val_vt.push_back(100);
	devil_soul_val_vt.push_back(200);
	devil_soul_val_vt.push_back(300);
	devil_soul_val_vt.push_back(400);
	devil_soul_val_vt.push_back(500);
	devil_soul_val_vt.push_back(600);
	devil_soul_val_vt.push_back(700);
	devil_soul_val_vt.push_back(800);
	devil_soul_val_vt.push_back(900);


	game::guild::guild::request_list_init_type request_list_init;
	game::guild::guild::request_list_init_type invitation_list_init;

	game::guild::guild::member_count_vt_type member_count_vt;
	member_count_vt.push_back(5);
	member_count_vt.push_back(10);
	member_count_vt.push_back(15);
	member_count_vt.push_back(20);
	member_count_vt.push_back(25);
	member_count_vt.push_back(30);
	member_count_vt.push_back(35);
	member_count_vt.push_back(40);
	member_count_vt.push_back(45);
	member_count_vt.push_back(50);

	game::guild::guild_members_type::member_map_type member_map;
	game::guild::guild::members_init_type member_init(3, member_count_vt[0], 50);
	game::guild::guild::technology_need_lv_vt_type technology_need_lv_vt;
	game::guild::guild::technology_need_lv_vt_vt_type technology_need_lv_vt_vt;

	//game::guild::guild::devil_soul_val_type dsv(1, 100);
	technology_need_lv_vt.push_back(game::guild::guild::technology_need_type(1, 100));
	technology_need_lv_vt.push_back(game::guild::guild::technology_need_type(3, 100));
	technology_need_lv_vt.push_back(game::guild::guild::technology_need_type(5, 300));
	technology_need_lv_vt.push_back(game::guild::guild::technology_need_type(7, 500));
	technology_need_lv_vt.push_back(game::guild::guild::technology_need_type(9, 700));
	//technology_need_lv_vt.push_back(game::guild::guild::technology_need_type(9, 900));

	technology_need_lv_vt_vt.push_back(technology_need_lv_vt);
	technology_need_lv_vt_vt.push_back(technology_need_lv_vt);
	technology_need_lv_vt_vt.push_back(technology_need_lv_vt);
	technology_need_lv_vt_vt.push_back(technology_need_lv_vt);

	game::guild::guild::guild_id_type guild_id = description_init.name;
	game::guild::guild gld(guild_id, uid, description_init, lv_init, devil_soul_init, member_init, areas_init, recipe_init,
							technology_init, request_list_init, invitation_list_init, devil_soul_val_vt, member_count_vt,
							technology_need_lv_vt_vt, thy_honour_vt_vt);

	std::cout << gld.add_devil_souls(99999999) << std::endl;

	std::cout << gld.level_up() << std::endl;
	std::cout << gld.level_up() << std::endl;
	std::cout << gld.level_up() << std::endl;
	std::cout << gld.level_up() << std::endl;
	std::cout << gld.level_up() << std::endl;
	std::cout << gld.level_up() << std::endl;
	std::cout << gld.level_up() << std::endl;
	std::cout << gld.level_up() << std::endl;
	std::cout << gld.level_up() << std::endl;
	std::cout << gld.level_up() << std::endl; //level_full

	std::cout << gld.add_honour(uid, 1000) << std::endl;
	std::cout << gld.add_honour(uid + 1, 1000) << std::endl;

	std::cout << gld.level_up_technology(game::guild::guild_technology_type::E_index_technology_pow) << std::endl;
	std::cout << gld.level_up_technology(game::guild::guild_technology_type::E_index_technology_pow) << std::endl;
	std::cout << gld.level_up_technology(game::guild::guild_technology_type::E_index_technology_pow) << std::endl;
	std::cout << gld.level_up_technology(game::guild::guild_technology_type::E_index_technology_pow) << std::endl;
	std::cout << gld.level_up_technology(game::guild::guild_technology_type::E_index_technology_pow) << std::endl;
	std::cout << gld.level_up_technology(game::guild::guild_technology_type::E_index_technology_pow) << std::endl; //level_full


	std::cout << gld.level_up_technology(game::guild::guild_technology_type::E_index_technology_def) << std::endl;
	std::cout << gld.level_up_technology(game::guild::guild_technology_type::E_index_technology_pot) << std::endl;
	std::cout << gld.level_up_technology(game::guild::guild_technology_type::E_index_technology_cure) << std::endl;

	gld.insert_areas((yggr::u32)0x0000aea1);
	gld.insert_areas((yggr::u32)0x0000aea2);

	gld.remove_areas((yggr::u32)0x0000aea1);
	gld.remove_areas((yggr::u32)0x0000aea2);

	std::cout << gld.add_member(0x00000002) << std::endl;
	std::cout << gld.add_member(0x00000003) << std::endl;
	std::cout << gld.add_member(0x00000004) << std::endl;
	std::cout << gld.add_member(0x00000005) << std::endl;
	std::cout << gld.add_member(0x00000006) << std::endl; //member_size 6
	std::cout << gld.add_member(0x00000006) << std::endl;

	std::cout << gld.remove_member(0x00000004) << std::endl; //member_size 5
	std::cout << gld.remove_member(0x0000000e) << std::endl;

	std::cout << gld.remove_member(uid) << std::endl;

	//std::cout << gld.change_master(uid, 0x00000002) << std::endl;
	//std::cout << gld.change_master(uid, 0x00000002) << std::endl;

	//std::cout << gld.is_master(uid) << std::endl;
	//std::cout << gld.is_master(0x00000002) << std::endl;
	//std::cout << gld.is_member(uid) << std::endl;

	std::cout << gld.appoint_deputy_master(0x00000002) << std::endl;
	std::cout << gld.appoint_deputy_master(0x00000003) << std::endl;
	std::cout << gld.appoint_deputy_master(0x00000005) << std::endl;
	std::cout << gld.appoint_deputy_master(0x00000006) << std::endl;

	std::cout << gld.is_deputy_master(0x00000002) << std::endl;
	std::cout << gld.is_deputy_master(0x00000003) << std::endl;
	std::cout << gld.is_deputy_master(0x00000005) << std::endl;
	std::cout << gld.is_deputy_master(0x00000006) << std::endl;

	std::cout << gld.dismissal_deputy_master(0x00000001) << std::endl;

	std::cout << gld.dismissal_deputy_master(0x00000005) << std::endl;
	std::cout << gld.dismissal_deputy_master(0x0000000e) << std::endl;

	std::cout << gld.request(uid) << std::endl;
	std::cout << gld.request(0x00000010) << std::endl;
	std::cout << gld.request(0x00000011) << std::endl;
	std::cout << gld.request(0x00000012) << std::endl;
	std::cout << gld.request(0x00000012) << std::endl;

	std::cout << gld.invitation(0x0000021) <<std::endl;
	std::cout << gld.invitation(0x0000022) <<std::endl;
	std::cout << gld.invitation(0x0000023) <<std::endl;
	std::cout << gld.invitation(0x0000023) <<std::endl;

	std::cout << gld.add_member_of_request(0x00000010) << std::endl; // member_size 6
	std::cout << gld.add_member_of_request(0x0000000e) << std::endl;
	std::cout << gld.add_member_of_request(uid) << std::endl;

	std::cout << gld.add_member_of_invitation(0x00000021) <<std::endl;// member_size 7;
	std::cout << gld.add_member_of_invitation(0x0000000e) <<std::endl;
	std::cout << gld.add_member_of_invitation(uid) << std::endl;

	std::cout << gld.invitation(0x00000011) << std::endl; // member_size 8
	std::cout << gld.request(0x0000022) << std::endl; // member_size 9

	std::cout << gld.unrequest(0x00000012) << std::endl;
	std::cout << gld.uninvitation(0x00000012) << std::endl;
}

void test_guild_security(void)
{
	typedef game::guild::guild_technology::level_type thy_level_type;
	typedef game::guild::guild_technology::level_vt_type thy_level_vt_type;
	typedef game::guild::guild_technology::val_vt_type thy_val_vt_type;
	typedef game::guild::guild_technology::val_vt_vt_type thy_val_vt_vt_type;

	typedef game::guild::guild::technology_honour_type thy_honour_type;
	typedef game::guild::guild::technology_honour_vt_type thy_honour_vt_type;
	typedef game::guild::guild::technology_honour_vt_vt_type thy_honour_vt_vt_type;

	thy_honour_vt_type thy_honour_pow_vt;
	thy_honour_pow_vt.push_back(thy_honour_type(1, 500));
	thy_honour_pow_vt.push_back(thy_honour_type(1, 2000));
	thy_honour_pow_vt.push_back(thy_honour_type(1, 4000));
	thy_honour_pow_vt.push_back(thy_honour_type(1, 6500));
	thy_honour_pow_vt.push_back(thy_honour_type(1, 9800));

	thy_honour_vt_type thy_honour_def_vt;
	thy_honour_def_vt.push_back(thy_honour_type(1, 600));
	thy_honour_def_vt.push_back(thy_honour_type(1, 2100));
	thy_honour_def_vt.push_back(thy_honour_type(1, 4200));
	thy_honour_def_vt.push_back(thy_honour_type(1, 6800));
	thy_honour_def_vt.push_back(thy_honour_type(1, 10000));

	thy_honour_vt_type thy_honour_pot_vt;
	thy_honour_pot_vt.push_back(thy_honour_type(1, 550));
	thy_honour_pot_vt.push_back(thy_honour_type(1, 2000));
	thy_honour_pot_vt.push_back(thy_honour_type(1, 4200));
	thy_honour_pot_vt.push_back(thy_honour_type(1, 7000));
	thy_honour_pot_vt.push_back(thy_honour_type(1, 11000));

	thy_honour_vt_type thy_honour_cure_vt;
	thy_honour_cure_vt.push_back(thy_honour_type(1, 550));
	thy_honour_cure_vt.push_back(thy_honour_type(1, 2000));
	thy_honour_cure_vt.push_back(thy_honour_type(1, 4200));
	thy_honour_cure_vt.push_back(thy_honour_type(1, 7000));
	thy_honour_cure_vt.push_back(thy_honour_type(1, 11000));

	thy_honour_vt_vt_type thy_honour_vt_vt;
	thy_honour_vt_vt.push_back(thy_honour_pow_vt);
	thy_honour_vt_vt.push_back(thy_honour_def_vt);
	thy_honour_vt_vt.push_back(thy_honour_pot_vt);
	thy_honour_vt_vt.push_back(thy_honour_cure_vt);


	game::guild::guild::member_id_type uid = 0x0000000000000001;

	game::guild::guild::level_init_type lv_init(1, 10);
	game::guild::guild::devil_soul_init_type devil_soul_init(9999999, 9999999);
	game::guild::guild::area_captures_init_type areas_init;
	game::guild::guild::recipe_captures_init_type recipe_init;
	game::guild::guild::description_init_type description_init(yggr::charset::utf8_string("天秀网络科技"),
																	yggr::charset::utf8_string("赶快把项目做完"));

	thy_level_type thy_lv(0, 5);

	thy_level_vt_type thy_level_vt;
	thy_level_vt.push_back(thy_lv);
	thy_level_vt.push_back(thy_lv);
	thy_level_vt.push_back(thy_lv);
	thy_level_vt.push_back(thy_lv);


	thy_val_vt_type th_val_vt;
	th_val_vt.push_back(10);
	th_val_vt.push_back(20);
	th_val_vt.push_back(30);
	th_val_vt.push_back(40);
	th_val_vt.push_back(50);

	thy_val_vt_vt_type thy_val_vt_vt;

	thy_val_vt_vt.push_back(th_val_vt);
	thy_val_vt_vt.push_back(th_val_vt);
	thy_val_vt_vt.push_back(th_val_vt);
	thy_val_vt_vt.push_back(th_val_vt);

	game::guild::guild::technology_init_type technology_init(thy_level_vt, thy_val_vt_vt);

	game::guild::guild::devil_soul_val_vt_type devil_soul_val_vt;

	devil_soul_val_vt.push_back(100);
	devil_soul_val_vt.push_back(200);
	devil_soul_val_vt.push_back(300);
	devil_soul_val_vt.push_back(400);
	devil_soul_val_vt.push_back(500);
	devil_soul_val_vt.push_back(600);
	devil_soul_val_vt.push_back(700);
	devil_soul_val_vt.push_back(800);
	devil_soul_val_vt.push_back(900);


	game::guild::guild::request_list_init_type request_list_init;
	game::guild::guild::request_list_init_type invitation_list_init;

	game::guild::guild::member_count_vt_type member_count_vt;
	member_count_vt.push_back(5);
	member_count_vt.push_back(10);
	member_count_vt.push_back(15);
	member_count_vt.push_back(20);
	member_count_vt.push_back(25);
	member_count_vt.push_back(30);
	member_count_vt.push_back(35);
	member_count_vt.push_back(40);
	member_count_vt.push_back(45);
	member_count_vt.push_back(50);

	game::guild::guild_members_type::member_map_type member_map;
	game::guild::guild::members_init_type member_init(3, member_count_vt[0], 50);
	game::guild::guild::technology_need_lv_vt_type technology_need_lv_vt;
	game::guild::guild::technology_need_lv_vt_vt_type technology_need_lv_vt_vt;

	//game::guild::guild::devil_soul_val_type dsv(1, 100);
	technology_need_lv_vt.push_back(game::guild::guild::technology_need_type(1, 100));
	technology_need_lv_vt.push_back(game::guild::guild::technology_need_type(3, 100));
	technology_need_lv_vt.push_back(game::guild::guild::technology_need_type(5, 300));
	technology_need_lv_vt.push_back(game::guild::guild::technology_need_type(7, 500));
	technology_need_lv_vt.push_back(game::guild::guild::technology_need_type(9, 700));
	//technology_need_lv_vt.push_back(game::guild::guild::technology_need_type(9, 900));

	technology_need_lv_vt_vt.push_back(technology_need_lv_vt);
	technology_need_lv_vt_vt.push_back(technology_need_lv_vt);
	technology_need_lv_vt_vt.push_back(technology_need_lv_vt);
	technology_need_lv_vt_vt.push_back(technology_need_lv_vt);


	game::guild::guild::guild_id_type guild_id = description_init.name;

	game::guild::guild_security::init_type gld_init(guild_id, uid, description_init, lv_init, devil_soul_init, member_init, areas_init, recipe_init,
							technology_init, request_list_init, invitation_list_init, devil_soul_val_vt, member_count_vt,
							technology_need_lv_vt_vt, thy_honour_vt_vt);

	game::guild::guild_security sgld(uid, gld_init);

	//-------------------------------------------function_test-------------------------------------

	game::guild::guild::member_id_type master_id = uid;
	game::guild::guild::member_id_type dmaster_id1 = 0x000000002;
	game::guild::guild::member_id_type dmaster_id2 = 0x000000003;

	game::guild::guild::member_id_type new_master_id = 0x00000009;

	game::guild::guild::member_id_type member_id1 = 0x00000006;
	game::guild::guild::member_id_type member_id2 = 0x00000007;
	game::guild::guild::member_id_type member_id3 = 0x00000008;
	game::guild::guild::member_id_type member_id9 = 0x00000099;


	assert(sgld.request(master_id)); //error
	assert(!sgld.request(dmaster_id1));
	assert(!sgld.request(dmaster_id2));

	assert(!sgld.request(member_id1));
	assert(sgld.request(member_id1)); //error

	assert(!sgld.request(member_id2));

	assert(!sgld.invitation(master_id, new_master_id));
	assert(sgld.add_member_of_request(new_master_id, new_master_id)); //error
	assert(!sgld.add_member_of_invitation(new_master_id));

	assert(sgld.invitation(master_id, dmaster_id1) == game::guild::guild_error::E_guild_members_pertain_to_the_guild);
	assert(!sgld.invitation(master_id, member_id3));

	assert(!sgld.add_member_of_request(master_id, dmaster_id2));
	assert(sgld.add_member_of_invitation(dmaster_id1));//error

	assert(!sgld.appoint_deputy_master(master_id, dmaster_id1));
	assert(sgld.invitation(dmaster_id1, member_id2)== game::guild::guild_error::E_guild_members_pertain_to_the_guild);

	assert(sgld.add_member_of_invitation(member_id3)); //error need level

	assert(!sgld.set_description(master_id, yggr::charset::utf8_string("abcd")));
	assert(!sgld.set_description(dmaster_id1, yggr::charset::utf8_string("efgh")));
	assert(sgld.set_description(member_id1, yggr::charset::utf8_string("error"))); //error

	assert(!sgld.request(member_id9));
	assert(!sgld.unrequest(member_id9));

	assert(!sgld.request(member_id9));
	assert(!sgld.refuse_request(dmaster_id1, member_id9));

	assert(!sgld.invitation(dmaster_id1, member_id9));
	assert(!sgld.uninvitation(member_id9));

	assert(!sgld.change_master(master_id, new_master_id));
	assert(sgld.remove_member(master_id, new_master_id)); //error
	//assert( sgld.remove_member(new_master_id, master_id));
	//assert( sgld.remove_member(new_master_id, dmaster_id1));

	assert(!sgld.level_up(new_master_id));
	assert(sgld.level_up(master_id)); //error

	assert(!sgld.level_up_technology(new_master_id, game::guild::guild_technology_type::E_index_technology_pow));
	assert(sgld.level_up_technology(master_id, game::guild::guild_technology_type::E_index_technology_pow)); //error

	assert(!sgld.level_up_technology(new_master_id, game::guild::guild_technology_type::E_index_technology_def));
	assert(sgld.level_up_technology(master_id, game::guild::guild_technology_type::E_index_technology_def)); //error

	assert(!sgld.level_up_technology(new_master_id, game::guild::guild_technology_type::E_index_technology_pot));
	assert(sgld.level_up_technology(master_id, game::guild::guild_technology_type::E_index_technology_pot)); //error

	assert(!sgld.level_up_technology(new_master_id, game::guild::guild_technology_type::E_index_technology_cure));
	assert(sgld.level_up_technology(master_id, game::guild::guild_technology_type::E_index_technology_cure)); //error
}

#include <guild/guild_default.hpp>
#include <guild/guild_mgr.hpp>
#include <yggr/thread/boost_thread_config.hpp>
#include <yggr/thread/thread_mgr.hpp>
#include <yggr/thread/work_thread.hpp>

#include <yggr/time/time.hpp>
#include <yggr/time/timer.hpp>

#include <yggr/thread/boost_thread_config.hpp>
#include <yggr/time/timer_mgr.hpp>
#include <yggr/base/ptr_single.hpp>
#include <yggr/thread/timer_runner.hpp>

#include <yggr/ids/inner_process_id.hpp>

enum
{
	E_area_num = 3,
	E_guild_num = 8,
	E_guild_member_num = 50

};

typedef yggr::ids::inner_process_id time_task_id_type;
//typedef yggr::u64 time_task_id_type;

typedef yggr::thread::work_thread<yggr::thread::boost_thread_config_type> work_thread_type;
typedef yggr::thread::thread_mgr<work_thread_type> trd_mgr_type;

typedef yggr::time::timer_mgr<time_task_id_type, yggr::thread::boost_thread_config_type> timer_mgr_type;
typedef yggr::ptr_single<timer_mgr_type> timer_ptr_single_type;

//typedef yggr::time::timer_mgr<> timer_mgr_type;
//typedef yggr::thread::timer_runner<timer_mgr_type> timer_runner_type;



typedef game::guild::guild_default guild_default_type;
typedef game::guild::guild_mgr<guild_default_type> guild_mgr_type;

typedef game::guild::guild_technology::level_type thy_level_type;
typedef game::guild::guild_technology::level_vt_type thy_level_vt_type;
typedef game::guild::guild_technology::val_vt_type thy_val_vt_type;
typedef game::guild::guild_technology::val_vt_vt_type thy_val_vt_vt_type;

typedef game::guild::guild::technology_honour_type thy_honour_type;
typedef game::guild::guild::technology_honour_vt_type thy_honour_vt_type;
typedef game::guild::guild::technology_honour_vt_vt_type thy_honour_vt_vt_type;

typedef guild_mgr_type::guild_battle_type guild_battle_type;
typedef guild_battle_type::request_condition_type request_cdt_type;
typedef guild_battle_type::battle_award_type battle_award_type;
typedef guild_battle_type::area_id_type aid_type;
typedef guild_battle_type::member_id_type uid_type;
typedef guild_battle_type::guild_id_type gid_type;
typedef guild_battle_type::join_condition_type join_cdt_type;

#include <set>

std::set<yggr::u64> losser;

int battle_count = 0;

#include <yggr/safe_struct/safe_list.hpp>
class timed_get_users
{
public:
	typedef yggr::time::time time_type;
	typedef yggr::time::timer<time_type> timer_type;

	class user_container
	{
	public:
		typedef std::list<uid_type> user_list_type;

	public:
		user_container(void)
		{
		}

		user_container(const user_list_type& win, const user_list_type& loss)
			: _win(win), _loss(loss)
		{
		}

		user_container(const user_container& right)
			: _win(right._win), _loss(right._loss)
		{
		}

		~user_container(void)
		{
		}

		const user_list_type& winners(void) const
		{
			return _win;
		}

		const user_list_type& losers(void) const
		{
			return _loss;
		}
	private:
		user_list_type _win;
		user_list_type _loss;
	};

	typedef yggr::safe_struct::safe_list<user_container> user_container_list_type;

	typedef class _tag_init
	{
	public:
		_tag_init(const aid_type& aid, const time_type& end_tm_step_eins, const time_type& end_tm_step_zwei,
					const time_type& group_check_tm_step, const time_type& chk_tm_step, guild_mgr_type& guild_mgr)
			: _aid(aid), _end_tm_step_eins(end_tm_step_eins), _end_tm_step_zwei(end_tm_step_zwei),
			_group_check_tm_step(group_check_tm_step), _chk_tm_step(chk_tm_step), _guild_mgr(guild_mgr)
		{
		}

	/*	_tag_init(const _tag_init& init)
			: _aid(init._aid), _end_tm_step_eins(init._end_tm_step_eins), _end_tm_step_zwei(init._end_tm_step_zwei),
			_group_check_tm_step(init._group_check_tm_step), _chk_tm_step(init._chk_tm_step), _guild_mgr(init._guild_mgr)
		{
		}*/

		_tag_init(const _tag_init& right)
			:  _aid(right._aid), _end_tm_step_eins(right._end_tm_step_eins), _end_tm_step_zwei(right._end_tm_step_zwei),
			_group_check_tm_step(right._group_check_tm_step), _chk_tm_step(right._chk_tm_step), _guild_mgr(right._guild_mgr)
		{
		}

		~_tag_init(void)
		{
		}

	public:
		aid_type _aid;
		time_type _end_tm_step_eins;
		time_type _end_tm_step_zwei;
		time_type _group_check_tm_step;
		time_type _chk_tm_step;
		guild_mgr_type& _guild_mgr;

	} init_type;

private:
	typedef timed_get_users this_type;
public:
	timed_get_users(const aid_type& aid, const time_type& end_tm_step_eins, const time_type& end_tm_step_zwei,
					const time_type& group_check_tm_step, const time_type& chk_tm_step, guild_mgr_type& guild_mgr)
		: _aid(aid), _key(), _end_tm_step_eins(end_tm_step_eins), _end_tm_step_zwei(end_tm_step_zwei),
			_group_check_tm_step(group_check_tm_step), _chk_tm_step(chk_tm_step), _guild_mgr(guild_mgr), _brun(true),
			_bchange_state(false), _bend(false)
	{
	}

	timed_get_users(const init_type& init)
		: _aid(init._aid), _key(), _end_tm_step_eins(init._end_tm_step_eins),
			_end_tm_step_zwei(init._end_tm_step_zwei), _group_check_tm_step(init._group_check_tm_step),
			_chk_tm_step(init._chk_tm_step), _guild_mgr(init._guild_mgr), _brun(true),
			_bchange_state(false), _bend(false)
	{
	}

	~timed_get_users(void)
	{
	}

	void set_id(const time_task_id_type& id)
	{
		_key = id;
	}

	//bool runnable(void) const //是否进行处理
	//{
	//	return _brun;
	//}

	yggr::u32 operator()(void) // 定时处理函数 返回值  true 继续 false  删除定时功能
	{
		/*static size_type last_losser_size = 0;

		if(last_losser_size != losser.size())
		{
			std::cout << "now losser size = " << losser.size() <<" -------!!!------- " << std::endl;
			last_losser_size = losser.size();
		}*/

		yggr::u32 sat = timer_mgr_type::E_timer_task_not_ran;
		if(_timer_group_check.elapsed() >= _group_check_tm_step)
		{
			get_group();
			back_battle_result();
			_timer_group_check.restart();
			sat = timer_mgr_type::E_timer_task_ran;
		}

		if(_timer_check_lose.elapsed() >= _chk_tm_step)
		{
			check_lossers();
			_timer_check_lose.restart();
			sat = timer_mgr_type::E_timer_task_ran;
		}

		if(!_bchange_state)
		{

			if(_timer_check_win.elapsed() >= _end_tm_step_eins)
			{
				/*std::set<yggr::u64>::const_iterator iters = losser.begin();
				std::set<yggr::u64>::const_iterator itere  = losser.end();
				for(int i = 1; i <= 150;)
				{
					if(iters == itere)
					{
						std::cout << "not found = " << i << std::endl;
						++i;
						continue;
					}
					if(i != *iters)
					{
						std::cout << "not found = " << i << std::endl;
					}
					else
					{
						++ iters;
					}

					++i;
				}*/

				check_winner();
				sat = timer_mgr_type::E_timer_task_ran;
			}
		}

		if(_bend)
		{
			_guild_mgr.stop_battle(_aid);
			_guild_mgr.back_battle_award(_aid);
			sat = timer_mgr_type::E_timer_task_remove;
		}

		return sat;
	}

	void get_group(void)
	{
		_guild_mgr.get_battle_group(_aid, boost::bind(&this_type::out_group, this, _1));
	}

	void out_group(const guild_battle_type::battle_rule_type::battle_group_list_type& list)
	{
		typedef guild_battle_type::battle_rule_type::battle_group_list_type list_type;
		typedef list_type::const_iterator citer_type;
		typedef list_type::value_type list_val_type;

		typedef list_val_type::first_type::const_iterator list_val_citer_type;

		//std::cout << list.size() << std::endl;

//		if(list.empty())
//		{
//			int nnn = 0;
//		}

		for(citer_type i = list.begin(), isize = list.end(); i != isize; ++i)
		{

//			if(i->first.empty())
//			{
//				int nnn = 0;
//			}
			{
				for(list_val_citer_type j = i->first.begin(), jsize = i->first.end(); j != jsize; ++j)
				{
					std::cout << *j << ",";
				}
			}

			std::cout << std::endl;

			{
				for(list_val_citer_type j = i->second.begin(), jsize = i->second.end(); j != jsize; ++j)
				{
					std::cout << *j << ",";
				}
			}

			if(i->first.front() == 41 || i->second.front() == 41)
			{
				int nnn = 0;
			}
			user_container container(i->first, i->second);
			_container_list.push_back(container); //送战斗模拟
			std::cout << std::endl;
			std::cout << "------------------------------" <<std::endl;

			const user_container::user_list_type& ulist = container.losers();

			for(user_container::user_list_type::const_iterator i = ulist.begin(), isize = ulist.end(); i != isize; ++i)
			{
				losser.insert(*i);
			}

			std::cout << "now losser size = " << losser.size() <<" -------!!!------- " << std::endl;
		}
	}

	bool back_battle_result(void)
	{
		if(_container_list.empty())
		{
			return false;
		}

		_container_list.use_handler(boost::bind(&this_type::handler_back_result, this, _1));

		return true;

	}

	void handler_back_result(user_container_list_type::base_type& base)
	{
		typedef user_container_list_type::base_type base_type;
		typedef base_type::const_iterator citer_type;

		for(citer_type i = base.begin(), isize = base.end(); i != isize; ++i)
		{
			if((*i).winners().front() == 41 && (*i).losers().front() == 91)
			{
				int nnn = 0;
			}
			_guild_mgr.back_battle_result(_aid, *i);
		}

		base.clear();
	}

	void check_lossers(void) // // 0 normal, 1 empty_warning, 2 loss, 3 win
	{
		_guild_mgr.fix_battle_loss(_aid, boost::make_tuple(boost::bind(&this_type::handler_normal, this, _1, _2, _3, _4, _5),
															boost::bind(&this_type::handler_empty_warring, this, _1, _2, _3, _4, _5),
															boost::bind(&this_type::handler_loss, this, _1, _2, _3, _4, _5),
															boost::bind(&this_type::handler_win, this, _1, _2, _3, _4, _5)));
	}

	void check_winner(void) // 0 lost , 1 win, 2 draw, 3 draw_fix
	{

		_guild_mgr.fix_battle_win(_aid, boost::make_tuple(boost::bind(&this_type::handler_loss, this, _1, _2, _3, _4, _5),
															boost::bind(&this_type::handler_win, this, _1, _2, _3, _4, _5),
															boost::bind(&this_type::handler_normal, this, _1, _2, _3, _4, _5),
															boost::bind(&this_type::handler_draw_fix, this)));

		std::cout << "change state !!!" << std::endl;
		_guild_mgr.change_battle_state(_aid);
		_bchange_state = true;
//		if(!_container_list.empty())
//		{
//			int nnn = 0;
//		}
	}

	void handler_empty_warring(const gid_type& gid, yggr::u32 count, yggr::u32 win, yggr::u32 loss, yggr::u32 grade)
	{
		std::cout << gid << " is empty please join it hurry!!!!" << std::endl;
	}

	void handler_loss(const gid_type& gid, yggr::u32 count, yggr::u32 win, yggr::u32 loss, yggr::u32 grade)
	{
		_gid_list.push_back(gid);
		std::cout << gid << " is loss!!!!" << std::endl;
	}

	void handler_normal(const gid_type& gid, yggr::u32 count, yggr::u32 win, yggr::u32 loss, yggr::u32 grade)
	{
		std::cout << gid << ":\n"
				  << "\tnow_count = " << count << "\n"
				  << "\twin = " << win << "\n"
				  << "\tloss = " << loss << "\n"
				  << "\tgrade = " << grade<< std::endl;
	}

	void handler_win(const gid_type& gid, yggr::u32 count, yggr::u32 win, yggr::u32 loss, yggr::u32 grade)
	{
		_bend = true;
		_bchange_state = true;

		_winner_gid = gid;

		std::cout << "winner is " << _winner_gid << std::endl;
		std::cout << _winner_gid << ":\n"
				  << "\tnow_count = " << count << "\n"
				  << "\twin = " << win << "\n"
				  << "\tloss = " << loss << "\n"
				  << "\tgrade = " << grade<< std::endl;
	}

	void handler_draw_fix(void)
	{
		//_bchange_state = true;
		std::cout << "draw" << std::endl;
	}


private:
	aid_type _aid;
	time_task_id_type _key;

	timer_type _timer_group_check;
	timer_type _timer_check_lose;
	timer_type _timer_check_win;

	time_type _end_tm_step_eins; //第一状态结束定时器
	time_type _end_tm_step_zwei; // 第二状态定时器
	time_type _group_check_tm_step;
	time_type _chk_tm_step;
	guild_mgr_type& _guild_mgr;

	std::list<gid_type> _gid_list;
	gid_type _winner_gid;

	bool _bchange_state;
	bool _bend;
	bool _brun;

	user_container_list_type _container_list; // use non safe struct is ok
};

bool join_battle(guild_mgr_type& guild_mgr, uid_type uids[][E_guild_member_num], aid_type aid)
{
	bool bjoin = false;
	for(int i = 0; i != E_guild_num; ++i)
	{
		if(i > 2)
		{
			continue;
		}
		for(int j = 0; j != E_guild_member_num; ++j)
		{
			//assert(!guild_mgr.join_battle(uids[i][j], aid));
			if(guild_mgr.join_battle(uids[i][j], aid) == 0)
			{
				bjoin = true;
			}
		}
	}

	std::cout << "end join" <<std::endl;
	return bjoin;
}

//bool join_battle(guild_mgr_type& guild_mgr, uid_type uids[][E_guild_member_num], aid_type aid)
//{
//	for(int j = 0; j != 6; ++j)
//	{
//		guild_mgr.join_battle(uids[0][j], aid);
//	}
//
//	for(int j = 0; j != 7; ++j)
//	{
//		guild_mgr.join_battle(uids[1][j], aid);
//	}
//
//	/*for(int j = 0; j != 5; ++j)
//	{
//		guild_mgr.join_battle(uids[2][j], aid);
//	}*/
//	std::cout << "end join" <<std::endl;
//	return false;
//}


void test_guild_mgr3(void)
{
	//typedef game::guild::guild_default guild_default_type;
	//typedef game::guild::guild_mgr<guild_default_type> guild_mgr_type;

	//typedef game::guild::guild_technology::level_type thy_level_type;
	//typedef game::guild::guild_technology::level_vt_type thy_level_vt_type;
	//typedef game::guild::guild_technology::val_vt_type thy_val_vt_type;
	//typedef game::guild::guild_technology::val_vt_vt_type thy_val_vt_vt_type;

	//typedef game::guild::guild::technology_honour_type thy_honour_type;
	//typedef game::guild::guild::technology_honour_vt_type thy_honour_vt_type;
	//typedef game::guild::guild::technology_honour_vt_vt_type thy_honour_vt_vt_type;

	thy_honour_vt_type thy_honour_pow_vt;
	thy_honour_pow_vt.push_back(thy_honour_type(1, 500));
	thy_honour_pow_vt.push_back(thy_honour_type(1, 2000));
	thy_honour_pow_vt.push_back(thy_honour_type(1, 4000));
	thy_honour_pow_vt.push_back(thy_honour_type(1, 6500));
	thy_honour_pow_vt.push_back(thy_honour_type(1, 9800));

	thy_honour_vt_type thy_honour_def_vt;
	thy_honour_def_vt.push_back(thy_honour_type(1, 600));
	thy_honour_def_vt.push_back(thy_honour_type(1, 2100));
	thy_honour_def_vt.push_back(thy_honour_type(1, 4200));
	thy_honour_def_vt.push_back(thy_honour_type(1, 6800));
	thy_honour_def_vt.push_back(thy_honour_type(1, 10000));

	thy_honour_vt_type thy_honour_pot_vt;
	thy_honour_pot_vt.push_back(thy_honour_type(1, 550));
	thy_honour_pot_vt.push_back(thy_honour_type(1, 2000));
	thy_honour_pot_vt.push_back(thy_honour_type(1, 4200));
	thy_honour_pot_vt.push_back(thy_honour_type(1, 7000));
	thy_honour_pot_vt.push_back(thy_honour_type(1, 11000));

	thy_honour_vt_type thy_honour_cure_vt;
	thy_honour_cure_vt.push_back(thy_honour_type(1, 550));
	thy_honour_cure_vt.push_back(thy_honour_type(1, 2000));
	thy_honour_cure_vt.push_back(thy_honour_type(1, 4200));
	thy_honour_cure_vt.push_back(thy_honour_type(1, 7000));
	thy_honour_cure_vt.push_back(thy_honour_type(1, 11000));

	thy_honour_vt_vt_type thy_honour_vt_vt;
	thy_honour_vt_vt.push_back(thy_honour_pow_vt);
	thy_honour_vt_vt.push_back(thy_honour_def_vt);
	thy_honour_vt_vt.push_back(thy_honour_pot_vt);
	thy_honour_vt_vt.push_back(thy_honour_cure_vt);


	game::guild::guild::member_id_type uid = 0x0000000000000001;

	game::guild::guild::level_init_type lv_init(1, 10);
	game::guild::guild::devil_soul_init_type devil_soul_init(9999999, 9999999);
	game::guild::guild::area_captures_init_type areas_init;
	game::guild::guild::recipe_captures_init_type recipe_init;


	thy_level_type thy_lv(0, 5);

	thy_level_vt_type thy_level_vt;
	thy_level_vt.push_back(thy_lv);
	thy_level_vt.push_back(thy_lv);
	thy_level_vt.push_back(thy_lv);
	thy_level_vt.push_back(thy_lv);


	thy_val_vt_type th_val_vt;
	th_val_vt.push_back(10);
	th_val_vt.push_back(20);
	th_val_vt.push_back(30);
	th_val_vt.push_back(40);
	th_val_vt.push_back(50);

	thy_val_vt_vt_type thy_val_vt_vt;

	thy_val_vt_vt.push_back(th_val_vt);
	thy_val_vt_vt.push_back(th_val_vt);
	thy_val_vt_vt.push_back(th_val_vt);
	thy_val_vt_vt.push_back(th_val_vt);

	game::guild::guild::technology_init_type technology_init(thy_level_vt, thy_val_vt_vt);

	game::guild::guild::devil_soul_val_vt_type devil_soul_val_vt;

	devil_soul_val_vt.push_back(100);
	devil_soul_val_vt.push_back(200);
	devil_soul_val_vt.push_back(300);
	devil_soul_val_vt.push_back(400);
	devil_soul_val_vt.push_back(500);
	devil_soul_val_vt.push_back(600);
	devil_soul_val_vt.push_back(700);
	devil_soul_val_vt.push_back(800);
	devil_soul_val_vt.push_back(900);


	game::guild::guild::request_list_init_type request_list_init;
	game::guild::guild::request_list_init_type invitation_list_init;

	game::guild::guild::member_count_vt_type member_count_vt;
	member_count_vt.push_back(5);
	member_count_vt.push_back(10);
	member_count_vt.push_back(15);
	member_count_vt.push_back(20);
	member_count_vt.push_back(25);
	member_count_vt.push_back(30);
	member_count_vt.push_back(35);
	member_count_vt.push_back(40);
	member_count_vt.push_back(45);
	member_count_vt.push_back(50);

	game::guild::guild_members_type::member_map_type member_map;
	game::guild::guild::members_init_type member_init(3, member_count_vt[0], 50);
	game::guild::guild::technology_need_lv_vt_type technology_need_lv_vt;
	game::guild::guild::technology_need_lv_vt_vt_type technology_need_lv_vt_vt;

	//game::guild::guild::devil_soul_val_type dsv(1, 100);
	technology_need_lv_vt.push_back(game::guild::guild::technology_need_type(1, 100));
	technology_need_lv_vt.push_back(game::guild::guild::technology_need_type(3, 100));
	technology_need_lv_vt.push_back(game::guild::guild::technology_need_type(5, 300));
	technology_need_lv_vt.push_back(game::guild::guild::technology_need_type(7, 500));
	technology_need_lv_vt.push_back(game::guild::guild::technology_need_type(9, 700));
	//technology_need_lv_vt.push_back(game::guild::guild::technology_need_type(9, 900));

	technology_need_lv_vt_vt.push_back(technology_need_lv_vt);
	technology_need_lv_vt_vt.push_back(technology_need_lv_vt);
	technology_need_lv_vt_vt.push_back(technology_need_lv_vt);
	technology_need_lv_vt_vt.push_back(technology_need_lv_vt);

	guild_default_type guild_default(lv_init, devil_soul_init, member_init, areas_init, recipe_init,
										technology_init, request_list_init, invitation_list_init,
										devil_soul_val_vt, member_count_vt, technology_need_lv_vt_vt,
										thy_honour_vt_vt);


	guild_mgr_type::init_type guild_mgr_init(guild_default, 5);
	guild_mgr_type guild_mgr(guild_mgr_init);

	/*typedef guild_mgr_type::guild_battle_type guild_battle_type;
	typedef guild_battle_type::request_condition_type request_cdt_type;
	typedef guild_battle_type::battle_award_type battle_award_type;
	typedef guild_battle_type::area_id_type aid_type;
	typedef guild_battle_type::member_id_type uid_type;
	typedef guild_battle_type::guild_id_type gid_type;
	typedef guild_battle_type::join_condition_type join_cdt_type;*/


	aid_type aid_arr[E_area_num] = {1, 2, 3};

	gid_type gid_arr[E_guild_num] = {gid_type("a"), gid_type("b"), gid_type("c"), gid_type("d"),
							gid_type("e"), gid_type("f"), gid_type("g"), gid_type("h")
						};

	uid_type uid_arr[E_guild_num][50] = {0};

	{
		for(int i = 0; i != E_guild_num; ++i)
		{
			for(int j = 0; j != E_guild_member_num; ++j)
			{
				uid_arr[i][j] = i * E_guild_member_num + j + 1;
				if(j == 0)
				{
#               ifdef _DEBUG
					assert(!guild_mgr.create_guild(uid_arr[i][j], game::guild::guild::description_init_type(gid_arr[i], gid_arr[i])));
					assert(!guild_mgr.level_up(uid_arr[i][j]));
					assert(!guild_mgr.level_up(uid_arr[i][j]));
					assert(!guild_mgr.level_up(uid_arr[i][j]));
					assert(!guild_mgr.level_up(uid_arr[i][j]));
					assert(!guild_mgr.level_up(uid_arr[i][j]));
					assert(!guild_mgr.level_up(uid_arr[i][j]));
					assert(!guild_mgr.level_up(uid_arr[i][j]));
					assert(!guild_mgr.level_up(uid_arr[i][j]));
					assert(!guild_mgr.level_up(uid_arr[i][j]));
#               else
					(!guild_mgr.create_guild(uid_arr[i][j], game::guild::guild::description_init_type(gid_arr[i], gid_arr[i])));
					(!guild_mgr.level_up(uid_arr[i][j]));
					(!guild_mgr.level_up(uid_arr[i][j]));
					(!guild_mgr.level_up(uid_arr[i][j]));
					(!guild_mgr.level_up(uid_arr[i][j]));
					(!guild_mgr.level_up(uid_arr[i][j]));
					(!guild_mgr.level_up(uid_arr[i][j]));
					(!guild_mgr.level_up(uid_arr[i][j]));
					(!guild_mgr.level_up(uid_arr[i][j]));
					(!guild_mgr.level_up(uid_arr[i][j]));
#               endif //_DEBUG
				}
				else
				{
#               ifdef _DEBUG
					assert(!guild_mgr.request_add_guild(uid_arr[i][j], gid_arr[i]));
					assert(!guild_mgr.add_member_of_request(uid_arr[i][0], uid_arr[i][j]));
#               else
                    (!guild_mgr.request_add_guild(uid_arr[i][j], gid_arr[i]));
					(!guild_mgr.add_member_of_request(uid_arr[i][0], uid_arr[i][j]));
#               endif //_DEBUG
				}
			}
		}
	}

	request_cdt_type cdt(500, 8);
	battle_award_type award[E_area_num];
	award[0].insert_areas(aid_arr[0]);
	award[0].insert_recipes((yggr::u32)1);
	award[0].insert_recipes((yggr::u32)2);

	award[0].insert_items_of_winner(game::guild::guild_item_type(1, 10, true));
	award[0].insert_items_of_winner(game::guild::guild_item_type(1, 10, true));
	award[0].insert_items_of_winner(game::guild::guild_item_type(2, 10, true));


	award[1].insert_areas(aid_arr[1]);
	award[1].insert_recipes((yggr::u32)1);
	award[1].insert_recipes((yggr::u32)2);

	award[1].insert_items_of_all(game::guild::guild_item_type(1, 10, true));
	award[1].insert_items_of_all(game::guild::guild_item_type(1, 10, true));
	award[1].insert_items_of_all(game::guild::guild_item_type(2, 10, true));

	award[2].insert_areas(aid_arr[2]);
	award[2].insert_recipes((yggr::u32)1);
	award[2].insert_recipes((yggr::u32)2);

	award[2].insert_items_of_all(game::guild::guild_item_type(1, 10, true));
	award[2].insert_items_of_all(game::guild::guild_item_type(1, 10, true));
	award[2].insert_items_of_all(game::guild::guild_item_type(2, 10, true));

	join_cdt_type join_cdt(100, 3);

	//guild_battle_type guild_battle;
#ifdef _DEBUG
	assert(!guild_mgr.add_battle_area(aid_arr[0], cdt, 5, award[0], join_cdt));
	assert(!guild_mgr.add_battle_area(aid_arr[1], cdt, 5, award[1], join_cdt));
	assert(!guild_mgr.add_battle_area(aid_arr[2], cdt, 5, award[2], join_cdt));
#else
    (!guild_mgr.add_battle_area(aid_arr[0], cdt, 5, award[0], join_cdt));
	(!guild_mgr.add_battle_area(aid_arr[1], cdt, 5, award[1], join_cdt));
	(!guild_mgr.add_battle_area(aid_arr[2], cdt, 5, award[2], join_cdt));
#endif // _DEBUG
	{
		for(int i = 0; i != E_guild_num; ++i)
		{
#   ifdef _DEBUG
			assert(!guild_mgr.request_battle(uid_arr[i][0], aid_arr[0]));
#   else
            (!guild_mgr.request_battle(uid_arr[i][0], aid_arr[0]));
#   endif //_DEBUG
		}
	}

	//assert(!guild_mgr.change_battle_state(aid_arr[0]));
#ifdef _DEBUG
	assert(!guild_mgr.start_battle(aid_arr[0]));
#else
    (!guild_mgr.start_battle(aid_arr[0]));
#endif //_DEBUG
	timer_mgr_type timer_mgr;
	timer_mgr.start();

	timed_get_users::init_type init(aid_arr[0], timed_get_users::time_type(30, 0), timed_get_users::time_type(60, 0),
										timed_get_users::time_type(1, 0), timed_get_users::time_type(10, 0), guild_mgr);

	//trd_mgr_type trd_mgr;
	//trd_mgr.add_thread();
	//trd_mgr.startup_all<timer_runner_type, timer_mgr_type>(timer_mgr);

	//join_battle(guild_mgr, uid_arr, aid_arr[0]);

	//guild_mgr.change_battle_state(aid_arr[0]);

	time_task_id_type id = timer_mgr.add_task<timed_get_users>(init);

	//timer_mgr.erase(id); //remove test

	//boost::shared_ptr<timed_get_users> user_get_ptr(new timed_get_users(aid_arr[0], yggr::time::time(1, 0), guild_mgr));
	//timed_get_users user_geter(aid_arr[0], yggr::time::time(1, 0), guild_mgr);
	//timer_mgr.addTask(user_geter);

	for(;join_battle(guild_mgr, uid_arr, aid_arr[0]);)
	{
		char cc = 0;
		std::cin >> cc;
	}
}

int main(int argc, char* argv[])
{

	//！！！！！！！！！！！！！！！！！需要调整初始化的模式
	//test_guild();
	//test_guild_security();

	test_guild_mgr3();

	char cc = 0;
	std::cin >> cc;
	return 0;
}


//
//#include <yggr/charset/utf8_string.h>
//#include <boost/unordered_map.hpp>
//
//int main(void)
//{
//	typedef boost::unordered_map<yggr::charset::utf8_string, yggr::charset::utf8_string> map_type;
//	map_type map;
//	map[yggr::charset::utf8_string("abc")] = yggr::charset::utf8_string("abc");
//}


//// Copyright 2005-2008 Daniel James.
//// Distributed under the Boost Software License, Version 1.0. (See accompanying
//// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//#include <boost/functional/hash.hpp>
//#include <cassert>
//
//// This example illustrates how to customise boost::hash portably, so that
//// it'll work on both compilers that don't implement argument dependent lookup
//// and compilers that implement strict two-phase template instantiation.
//
//namespace foo
//{
//    template <class T>
//    class custom_type
//    {
//        T value;
//    public:
//        custom_type(T x) : value(x) {}
//
//        std::size_t hash() const
//        {
//            boost::hash<T> hasher;
//            return hasher(value);
//        }
//    };
//}
//
//#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
//namespace boost
//#else
//namespace foo
//#endif
//{
//    template <class T>
//    std::size_t hash_value(foo::custom_type<T> x)
//    {
//        return x.hash();
//    }
//}
//
//int main()
//{
//    foo::custom_type<int> x(1), y(2), z(1);
//
//    boost::hash<foo::custom_type<int> > hasher;
//
//    assert(hasher(x) == hasher(x));
//    assert(hasher(x) != hasher(y));
//    assert(hasher(x) == hasher(z));
//
//    return 0;
//}
