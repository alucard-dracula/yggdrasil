//game_mgr_test.cpp

#include <iostream>
#include <parser/parser_mgr.hpp>
#include <stl_exception.hpp>
#include <boost/bind.hpp>
#include <protocol/error_container.hpp>

//#include <vld.h>

typedef game::game_mgr::gm_def::gm_base_type_def<> gm_def_type;
typedef game::game_mgr::parser::parser_mgr<gm_def_type> parser_mgr_type;
typedef yggr::charset::utf8_string string_type;
typedef parser_mgr_type::command_type command_type;

typedef game::game_mgr::protocol::error_container<yggr::u32, yggr::charset::utf8_string> error_type;

parser_mgr_type::run_rst_type out_cmd(const string_type& uid, const yggr::u32& lv, const parser_mgr_type::command_type& command)
{
	std::cout << "uid = " << uid << " lv = " << lv << " command = " << command << std::endl;
	return parser_mgr_type::run_rst_type(0,  string_type("sucess"));
}


void test_parser_mgr(void)
{
	parser_mgr_type mgr;

	mgr.register_parser(string_type("item"), boost::bind(&out_cmd, _1, _2, _3));
	error_type err = mgr.run_command<error_type>(string_type("xy"), 10, string_type("item"), string_type("update name=��Ѫƿ add_count = 1"));

	std::cout << err.what() << std::endl;
}

#include <users/user_info.hpp>
#include <users/user_info_mgr.hpp>
#include <list>
#include <boost/tuple/tuple.hpp>

typedef yggr::charset::utf8_string string_type;
typedef game::game_mgr::users::user_info<gm_def_type> user_info_type;
typedef game::game_mgr::users::user_info_mgr<user_info_type> user_info_mgr_type;

typedef std::pair<string_type, yggr::u32> sys_lv_type; 
typedef std::list<sys_lv_type> sys_lv_list_type;

typedef std::list<string_type> sys_list_type;

void user_info_mgr_test(void)
{
	user_info_mgr_type user_mgr;

	user_mgr.add_user(string_type("xy"), string_type("123456"));
	user_mgr.add_user(string_type("xy2"), string_type("���"));
	std::cout << user_mgr.cmp_passwd(string_type("xy"), string_type("123456")) << std::endl;
	std::cout << user_mgr.cmp_passwd(string_type("xy2"), string_type("���")) << std::endl;
	std::cout << user_mgr.cmp_passwd(string_type("xy"), string_type("���")) << std::endl;

	sys_lv_list_type sys_lv_list;

	sys_lv_list.push_back(sys_lv_type(string_type("item"), 100));
	sys_lv_list.push_back(sys_lv_type(string_type("item2"), 200));
	sys_lv_list.push_back(sys_lv_type(string_type("item3"), 50));

	sys_list_type sys_list;
	sys_list.push_back(string_type("item"));
	sys_list.push_back(string_type("item2"));
	sys_list.push_back(string_type("item3"));

	user_mgr.add_user(string_type("xy3"), string_type("asdfg"), sys_lv_list);

	std::cout << user_mgr.get_system_level(string_type("xy3"), string_type("item")) << std::endl;
	assert(user_mgr.get_system_level(string_type("xy3"), string_type("item")) == 100);
	assert(user_mgr.get_system_level(string_type("xy3"), string_type("item2")) == 200);
	assert(user_mgr.get_system_level(string_type("xy3"), string_type("item3")) == 50);

	user_mgr.set_system_level(string_type("xy3"), string_type("item3"), 20);

	assert(user_mgr.get_system_level(string_type("xy3"), string_type("item3")) == 20);

	user_mgr.remove_system_level(string_type("xy3"), sys_list);

	assert(user_mgr.get_system_level(string_type("xy3"), string_type("item")) == 0);
	assert(user_mgr.get_system_level(string_type("xy3"), string_type("item2")) == 0);
	assert(user_mgr.get_system_level(string_type("xy3"), string_type("item3")) == 0);

	std::cout << user_mgr.remove_user(string_type("xy3")).second << std::endl;

	user_mgr.clear();

	int nnn = 0;
}

#include <system_gm/system_gm.hpp>
#include <protocol/error_container.hpp>
#include <protocol/login.hpp>
#include <list>

typedef game::game_mgr::system_gm::system_gm<yggr::u64, user_info_mgr_type, parser_mgr_type> system_gm_type;

typedef std::set<yggr::u32> ip_set_type;

//typedef boost::tuple<string_type, yggr::u32> sys_lv_type;
typedef std::list<sys_lv_type> sys_lv_list_type;

typedef game::game_mgr::protocol::error_container<yggr::u32, string_type> error_container_type;
typedef game::game_mgr::protocol::login<string_type, string_type> login_type;


void test_system_gm(void)
{
	system_gm_type sys_gm;

	sys_lv_list_type sys_lv_list;
	sys_lv_list.push_back(sys_lv_type(string_type("item"), 100));
	sys_lv_list.push_back(sys_lv_type(string_type("login"), 200));

	sys_lv_list_type sys_lv_list2;
	sys_lv_list.push_back(sys_lv_type(string_type("item"), 10));
	sys_lv_list.push_back(sys_lv_type(string_type("login"), 20));
	sys_lv_list2.push_back(sys_lv_type(string_type("item2"), 300));
	sys_lv_list2.push_back(sys_lv_type(string_type("login2"), 400));

	ip_set_type ip_set;
	ip_set.insert(0x11111111);
	ip_set.insert(0x22222222);

	ip_set_type ip_set2;
	ip_set2.insert(0x33333333);
	ip_set2.insert(0x66666666);

	error_container_type msg1(sys_gm.add_user<error_container_type>(string_type("xy"), string_type("12345"), sys_lv_list, ip_set));
	error_container_type msg2(sys_gm.add_user<error_container_type>(string_type("xy2"), string_type("12345"), sys_lv_list, ip_set));
	error_container_type msg3(sys_gm.add_user<error_container_type>(string_type("xy3"), string_type("12345"), sys_lv_list, ip_set));
	error_container_type msg4(sys_gm.add_user<error_container_type>(string_type("xy4"), string_type("12345"), sys_lv_list, ip_set));

	std::cout << msg1.what() << std::endl;
	std::cout << msg2.what() << std::endl;
	std::cout << msg3.what() << std::endl;
	std::cout << msg4.what() << std::endl;

	std::cout << sys_gm.get_system_level_list<error_container_type>(string_type("xy")).what() << std::endl;
	std::cout << sys_gm.get_system_level<error_container_type>(string_type("xy"), string_type("item")).what() << std::endl;
	std::cout << sys_gm.get_ip_limit<error_container_type>(string_type("xy")).what() << std::endl;
	std::cout << sys_gm.set_system_level<error_container_type>(string_type("xy"), sys_lv_list2).what() << std::endl;
	std::cout << sys_gm.get_system_level_list<error_container_type>(string_type("xy")).what() << std::endl;
	std::cout << sys_gm.insert_ip_limit<error_container_type>(string_type("xy"), ip_set2).what() << std::endl;
	std::cout << sys_gm.get_ip_limit<error_container_type>(string_type("xy")).what() << std::endl;
	std::cout << sys_gm.get_user_list<error_container_type>().what() << std::endl;

	login_type login(string_type("root"), string_type("root"));
	std::cout << sys_gm.login<error_container_type>(1000, 0, login).what() << std::endl;
	login_type login2(string_type("root"), string_type("root2"));
	std::cout << sys_gm.login<error_container_type>(2000, 0, login2).what() << std::endl;
}

#include <protocol/ctrl_cmd.hpp>

typedef game::game_mgr::protocol::ctrl_cmd<string_type, string_type> ctrl_cmd_type; 
typedef system_gm_type::script_file_code_type file_code_type;

void test_lua_system_gm(void)
{
	string_type str("12345fsafdsfdsaf");
	string_type str2(str);

	yggr::shared_ptr<system_gm_type> sys_gm_ptr(new system_gm_type());

	const std::string enter_foo = "gm_sys_parser";
	std::string code("./gm_sys_parser.lua");

	sys_gm_ptr->register_parser(string_type("gm"), enter_foo, file_code_type(code));

	login_type login(string_type("root"), string_type("root"));
	std::cout << sys_gm_ptr->login<error_container_type>(1000, 0, login).what() << std::endl;

	
	std::string sys_name = "gm";

	std::cout << "---------------------------------------" << std::endl;
	std::string command_add_user1 = "adduser --name user1 --pwd 123456 --sys_lv login = 100 item = 200 --ip_limit 192.168.2.1 192.168.2.2";
	std::string command_add_user2 = "adduser --name user2 --pwd 123456 --sys_lv login = 100 item = 200 --ip_limit 192.168.2.1 192.168.2.2";

	yggr::u32 ip = 3232236033; //192.168.2.1
	error_container_type rst_msg_add_user1(sys_gm_ptr->run_command<error_container_type>(1000, 
																				ctrl_cmd_type(sys_name,
																				string_type(command_add_user1))));
	std::cout << rst_msg_add_user1.code() << ", " << rst_msg_add_user1.what() << std::endl;

	error_container_type rst_msg_add_user2(sys_gm_ptr->run_command<error_container_type>(1000, 
																				ctrl_cmd_type(sys_name,
																				string_type(command_add_user2))));
	std::cout << rst_msg_add_user2.code() << ", " << rst_msg_add_user2.what() << std::endl;

	std::cout << sys_gm_ptr->get_user_list<error_container_type>().what() << std::endl;
	std::cout << sys_gm_ptr->get_system_level_list<error_container_type>(string_type("user1")).what() << std::endl;
	std::cout << sys_gm_ptr->get_ip_limit<error_container_type>(string_type("user1")).what() << std::endl;

	std::string command_get_user_list = "userlist";
	error_container_type rst_msg_user_list(sys_gm_ptr->run_command<error_container_type>(1000, 
																				ctrl_cmd_type(sys_name,
																				string_type(command_get_user_list))));

	std::cout << rst_msg_user_list.code() << ", " << rst_msg_user_list.what() << std::endl;

	std::cout << "---------------------------------------" << std::endl;

	//std::string command_remove_users = "rmuser --name user1 user2";
	//error_container_type rst_command_remove_users(sys_gm_ptr->run_command<error_container_type>(1000, 
	//																			ctrl_cmd_type(sys_name,
	//																			string_type(command_remove_users))));
	//std::cout << rst_command_remove_users.code() << ", " << rst_command_remove_users.what() << std::endl;

	error_container_type rst_msg_user_list2(sys_gm_ptr->run_command<error_container_type>(1000, 
																				ctrl_cmd_type(sys_name,
																				string_type(command_get_user_list))));
	std::cout << rst_msg_user_list2.code() << ", " << rst_msg_user_list2.what() << std::endl;
	
	std::cout << "---------------------------------------" << std::endl;

	std::string command_set_passwd = "setpwd --name user1 --pwd abcdefg";
	error_container_type rst_command_set_passwd(sys_gm_ptr->run_command<error_container_type>(1000, 
																				ctrl_cmd_type(sys_name,
																				string_type(command_set_passwd))));
	std::cout << rst_command_set_passwd.code() << ", " << rst_command_set_passwd.what() << std::endl;

	
	login_type login_user1(string_type("user1"), string_type("123456"));
	//login_type login_user1(string_type("user1"), string_type("abcdefg"));
	std::cout << sys_gm_ptr->login<error_container_type>(2000, ip, login_user1).what() << std::endl;

	std::cout << "---------------------------------------" << std::endl;

	std::string command_set_sys_lv = "setsyslv --name user1 --sys_lv item = 300 login=200 test = 1000";
	error_container_type rst_command_set_sys_lv(sys_gm_ptr->run_command<error_container_type>(1000, 
																				ctrl_cmd_type(sys_name,
																				string_type(command_set_sys_lv))));
	std::cout << rst_command_set_sys_lv.code() << ", " << rst_command_set_sys_lv.what() << std::endl;

	std::cout << sys_gm_ptr->get_system_level_list<error_container_type>(string_type("user1")).what() << std::endl;

	std::cout << "---------------------------------------" << std::endl;
	std::string command_get_sys_lv_list = "getsyslist --name user1";
	error_container_type rst_command_get_sys_lv_list(sys_gm_ptr->run_command<error_container_type>(1000, 
																				ctrl_cmd_type(sys_name,
																				string_type(command_get_sys_lv_list))));
	std::cout << rst_command_get_sys_lv_list.code() << ", " << rst_command_get_sys_lv_list.what() << std::endl;

	std::cout << "---------------------------------------" << std::endl;
	std::string command_get_sys_lv1 = "getsyslv --name user1 --sys item";
	error_container_type rst_command_get_sys_lv1(sys_gm_ptr->run_command<error_container_type>(1000, 
																				ctrl_cmd_type(sys_name,
																				string_type(command_get_sys_lv1))));
	std::cout << rst_command_get_sys_lv1.code() << ", " << rst_command_get_sys_lv1.what() << std::endl;

	std::cout << "---------------------------------------" << std::endl;
	std::string command_get_sys_lv2 = "getsyslv --name user1 --sys test2";
	error_container_type rst_command_get_sys_lv2(sys_gm_ptr->run_command<error_container_type>(1000, 
																				ctrl_cmd_type(sys_name,
																				string_type(command_get_sys_lv2))));
	std::cout << rst_command_get_sys_lv2.code() << ", " << rst_command_get_sys_lv2.what() << std::endl;

	std::cout << "---------------------------------------" << std::endl;
	std::string command_ins_ip_lim = "insiplim --name user1 --ip_limit 192.168.2.3 192.168.2.4";
	error_container_type rst_command_ins_ip_lim(sys_gm_ptr->run_command<error_container_type>(1000, 
																				ctrl_cmd_type(sys_name,
																				string_type(command_ins_ip_lim))));
	std::cout << rst_command_ins_ip_lim.code() << ", " << rst_command_ins_ip_lim.what() << std::endl;

	std::cout << sys_gm_ptr->get_ip_limit<error_container_type>(string_type("user1")).what() << std::endl;

	std::cout << "---------------------------------------" << std::endl;
	std::string command_get_ip_lim = "getiplim --name user1";
	error_container_type rst_command_get_ip_lim(sys_gm_ptr->run_command<error_container_type>(1000, 
																				ctrl_cmd_type(sys_name,
																				string_type(command_get_ip_lim))));
	std::cout << rst_command_get_ip_lim.code() << ", " << rst_command_get_ip_lim.what() << std::endl;
}

#include <database/database_def.hpp>
#include <database/db_container.hpp>

void test_db_and_gm_system(void)
{
	game::game_mgr::database::mongodb_io_system_single_type::init_ins(std::string("127.0.0.1:10098"));

	game::game_mgr::database::mongodb_io_system_single_ptr_type db_ptr = 
		game::game_mgr::database::mongodb_io_system_single_type::get_ins();
	game::game_mgr::database::mongodb_io_system_single_type::get_ins()->start();

	//string_type str("12345fsafdsfdsaf");
	//string_type str2(str);

	yggr::shared_ptr<system_gm_type> sys_gm_ptr(new system_gm_type());

	const std::string enter_foo = "gm_sys_parser";
	std::string code("./gm_sys_parser.lua");

	game::game_mgr::gm_def::gm_def_data def_data;
	string_type sys_name = def_data.sys_name();

	sys_gm_ptr->register_parser(sys_name, enter_foo, file_code_type(code));

	for(;;)
	{
		login_type login(string_type("root"), string_type("root"));
		if(!sys_gm_ptr->login<error_container_type>(1000, 0, login).code())
		{
			std::cout << "login ok" << std::endl;
			break;
		}
	}

	std::cout << "---------------------------------------" << std::endl;
	std::string command_add_user1 = "adduser --name user1 --pwd 123456 --sys_lv login = 100 item = 200 --ip_limit 192.168.2.1 192.168.2.2";
	std::string command_add_user2 = "adduser --name user2 --pwd 123456 --sys_lv login = 100 item = 200 --ip_limit 192.168.2.1 192.168.2.2";

	yggr::u32 ip = 3232236033; //192.168.2.1
	error_container_type rst_msg_add_user1(sys_gm_ptr->run_command<error_container_type>(1000, 
																				ctrl_cmd_type(sys_name,
																				string_type(command_add_user1))));
	std::cout << rst_msg_add_user1.code() << ", " << rst_msg_add_user1.what() << std::endl;

	error_container_type rst_msg_add_user2(sys_gm_ptr->run_command<error_container_type>(1000, 
																				ctrl_cmd_type(sys_name,
																				string_type(command_add_user2))));
	std::cout << rst_msg_add_user2.code() << ", " << rst_msg_add_user2.what() << std::endl;

	std::cout << sys_gm_ptr->get_user_list<error_container_type>().what() << std::endl;
	std::cout << sys_gm_ptr->get_system_level_list<error_container_type>(string_type("user1")).what() << std::endl;
	std::cout << sys_gm_ptr->get_ip_limit<error_container_type>(string_type("user1")).what() << std::endl;

	std::string command_get_user_list = "userlist";
	error_container_type rst_msg_user_list(sys_gm_ptr->run_command<error_container_type>(1000, 
																				ctrl_cmd_type(sys_name,
																				string_type(command_get_user_list))));

	std::cout << rst_msg_user_list.code() << ", " << rst_msg_user_list.what() << std::endl;

	std::cout << "---------------------------------------" << std::endl;

	//std::string command_remove_users = "rmuser --name user1 user2";
	//error_container_type rst_command_remove_users(sys_gm_ptr->run_command<error_container_type>(1000, 
	//																			ctrl_cmd_type(sys_name,
	//																			string_type(command_remove_users))));
	//std::cout << rst_command_remove_users.code() << ", " << rst_command_remove_users.what() << std::endl;

	error_container_type rst_msg_user_list2(sys_gm_ptr->run_command<error_container_type>(1000, 
																				ctrl_cmd_type(sys_name,
																				string_type(command_get_user_list))));
	std::cout << rst_msg_user_list2.code() << ", " << rst_msg_user_list2.what() << std::endl;
	
	std::cout << "---------------------------------------" << std::endl;

	std::string command_set_passwd = "setpwd --name user1 --pwd abcdefg";
	error_container_type rst_command_set_passwd(sys_gm_ptr->run_command<error_container_type>(1000, 
																				ctrl_cmd_type(sys_name,
																				string_type(command_set_passwd))));
	std::cout << rst_command_set_passwd.code() << ", " << rst_command_set_passwd.what() << std::endl;

	
	login_type login_user1(string_type("user1"), string_type("123456"));
	//login_type login_user1(string_type("user1"), string_type("abcdefg"));
	std::cout << sys_gm_ptr->login<error_container_type>(2000, ip, login_user1).what() << std::endl;

	std::cout << "---------------------------------------" << std::endl;

	std::string command_set_sys_lv = "setsyslv --name user1 --sys_lv item = 300 login=200 test = 1000";
	error_container_type rst_command_set_sys_lv(sys_gm_ptr->run_command<error_container_type>(1000, 
																				ctrl_cmd_type(sys_name,
																				string_type(command_set_sys_lv))));
	std::cout << rst_command_set_sys_lv.code() << ", " << rst_command_set_sys_lv.what() << std::endl;

	std::cout << sys_gm_ptr->get_system_level_list<error_container_type>(string_type("user1")).what() << std::endl;

	std::cout << "---------------------------------------" << std::endl;
	std::string command_get_sys_lv_list = "getsyslist --name user1";
	error_container_type rst_command_get_sys_lv_list(sys_gm_ptr->run_command<error_container_type>(1000, 
																				ctrl_cmd_type(sys_name,
																				string_type(command_get_sys_lv_list))));
	std::cout << rst_command_get_sys_lv_list.code() << ", " << rst_command_get_sys_lv_list.what() << std::endl;

	std::cout << "---------------------------------------" << std::endl;
	std::string command_get_sys_lv1 = "getsyslv --name user1 --sys item";
	error_container_type rst_command_get_sys_lv1(sys_gm_ptr->run_command<error_container_type>(1000, 
																				ctrl_cmd_type(sys_name,
																				string_type(command_get_sys_lv1))));
	std::cout << rst_command_get_sys_lv1.code() << ", " << rst_command_get_sys_lv1.what() << std::endl;

	std::cout << "---------------------------------------" << std::endl;
	std::string command_get_sys_lv2 = "getsyslv --name user1 --sys test2";
	error_container_type rst_command_get_sys_lv2(sys_gm_ptr->run_command<error_container_type>(1000, 
																				ctrl_cmd_type(sys_name,
																				string_type(command_get_sys_lv2))));
	std::cout << rst_command_get_sys_lv2.code() << ", " << rst_command_get_sys_lv2.what() << std::endl;

	std::cout << "---------------------------------------" << std::endl;
	std::string command_ins_ip_lim = "insiplim --name user1 --ip_limit 192.168.2.3 192.168.2.4";
	error_container_type rst_command_ins_ip_lim(sys_gm_ptr->run_command<error_container_type>(1000, 
																				ctrl_cmd_type(sys_name,
																				string_type(command_ins_ip_lim))));
	std::cout << rst_command_ins_ip_lim.code() << ", " << rst_command_ins_ip_lim.what() << std::endl;

	std::cout << sys_gm_ptr->get_ip_limit<error_container_type>(string_type("user1")).what() << std::endl;

	std::cout << "---------------------------------------" << std::endl;
	std::string command_get_ip_lim = "getiplim --name user1";
	error_container_type rst_command_get_ip_lim(sys_gm_ptr->run_command<error_container_type>(1000, 
																				ctrl_cmd_type(sys_name,
																				string_type(command_get_ip_lim))));
	std::cout << rst_command_get_ip_lim.code() << ", " << rst_command_get_ip_lim.what() << std::endl;

	std::cout << "---------------------------------------" << std::endl;
	//std::cout << sys_gm_ptr->commit_db<error_container_type>().what() << std::endl;

	std::string command_commit = "commit";
	error_container_type rst_command_commit(sys_gm_ptr->run_command<error_container_type>(1000, 
																				ctrl_cmd_type(sys_name,
																				string_type(command_commit))));
	std::cout << rst_command_commit.code() << ", " << rst_command_commit.what() << std::endl;

	char cc = 0;
	std::cin >> cc;
}


//--------------------------------------------------------------

#include <iostream>

#include <yggr/network/network_packet.hpp>

#include <yggr/network/connection.hpp>

#include <yggr/network/network_balance_tcp_config.hpp>

#include <yggr/network/tcp_ex_linker.hpp>

#include <yggr/archive/net_oarchive.hpp>
#include <yggr/archive/net_iarchive.hpp>

#include <yggr/server/session.hpp>
#include <yggr/server/session_mgr.hpp>

#include <yggr/server/tcp_balance_svr_handler.hpp>
#include <yggr/server/server.hpp>

#include <yggr/data_center/recver_handler_mgr_single_foo.hpp>
#include <yggr/data_center/task.hpp>
#include <yggr/data_center/direct_data_center.hpp>

#include <yggr/adapter/base_send_adapter.hpp>
#include <yggr/adapter/base_recv_adapter.hpp>
#include <yggr/adapter/adapter_mgr.hpp>

#include <yggr/thread/smart_work_runner.hpp>
#include <yggr/thread/thread_mgr.hpp>
#include <yggr/thread/work_thread.hpp>

#include <yggr/network/pak_back_id.hpp>
#include <yggr/base/exception.hpp>
#include <yggr/thread/io_runner.hpp>

#include <yggr/archive/network_archive_partner.hpp>
#include <yggr/base/ctrl_center.hpp>
#include <yggr/base/system_code.hpp>
#include <boost/asio/error.hpp>


#include <yggr/ids/base_ids_def.hpp>

#include <set>
#include <string>
#include <assert.h>

#ifdef _MSC_VER
#   ifdef _DEBUG
#	    include <vld.h>
#   endif //_DEBUG
#endif //_MSC_VER

typedef yggr::system_controller::ctrl_center ctrl_center_type;

typedef yggr::network::network_packet::network_packet<yggr::archive::archive_partner::network_oarchive_partner> opacket_type;
typedef yggr::network::network_packet::network_packet<yggr::archive::archive_partner::network_iarchive_partner> ipacket_type;

//typedef yggr::network::tcp_sender<opacket_type> sender_type;
//typedef yggr::network::tcp_recver<ipacket_type> recver_type;

typedef yggr::network::network_packet::pak_back_id<yggr::ids::id64_type> test_pak_type;

typedef opacket_type::base_type opack;
typedef ipacket_type::base_type ipack;

typedef yggr::data_center::task<opack::data_cmp_type, yggr::ids::id64_type> task_type;

typedef yggr::data_center::direct_data_center<task_type,
												yggr::data_center::recver_handler_mgr_single_foo,
												yggr::thread::smart_work_runner> data_center_type;
typedef yggr::adapter::adapter_mgr<data_center_type, ipack, boost::any, opack,
									yggr::adapter::base_send_adapter,
									yggr::adapter::base_recv_adapter
									> adapter_mgr_type;


typedef yggr::network::tcp_ex_linker<yggr::network::network_config::balance_tcpv4_config> linker_type;
typedef yggr::network::connection<linker_type, opacket_type, ipacket_type> conn_type;
//typedef yggr::network::connection<linker_type, opacket_type, ipacket_type, false> conn_type;
typedef yggr::server::session<conn_type, adapter_mgr_type> session_type;


typedef yggr::server::session_mgr<session_type> session_mgr_type;
typedef yggr::server::tcp_balance_svr_handler<test_pak_type, session_mgr_type> tcp_handler_type;
typedef yggr::server::server<tcp_handler_type> svr_type;

typedef yggr::ptr_single<svr_type> svr_ptr_single_type;

typedef data_center_type::work_runner_type work_runner_type;
typedef data_center_type::init_type data_center_init_type;
typedef work_runner_type::shared_info_ptr_type runner_shared_info_ptr_type;

typedef yggr::thread::work_thread<yggr::thread::boost_thread_config_type> work_thread_type;
typedef yggr::thread::thread_mgr<work_thread_type> trd_mgr_type;

#include <sys_wrap/sys_gm_calculater.hpp>

typedef game::game_mgr::sys_wrap::sys_gm_calculater<system_gm_type, svr_type> sys_gm_cal_type;
typedef yggr::shared_ptr<sys_gm_cal_type> sys_gm_cal_ptr_type;

void test_helper(void)
{
}

void test_gm_sys_calculater(void)
{
	typedef game::game_mgr::database::mongodb_io_system_single_type::obj_type::init_sig_pwd_type mongo_init_type;
	game::game_mgr::database::mongodb_io_system_single_type::init_ins(mongo_init_type(std::string("127.0.0.1:10098"), "xy", "123456"));

	game::game_mgr::database::mongodb_io_system_single_ptr_type db_ptr = 
		game::game_mgr::database::mongodb_io_system_single_type::get_ins();
	game::game_mgr::database::mongodb_io_system_single_type::get_ins()->start();

	//string_type str("12345fsafdsfdsaf");
	//string_type str2(str);

	yggr::shared_ptr<system_gm_type> sys_gm_ptr(new system_gm_type());

	const std::string enter_foo = "gm_sys_parser";
	std::string code("./gm_sys_parser.lua");

	game::game_mgr::gm_def::gm_def_data def_data;
	string_type sys_name = def_data.sys_name();
	sys_gm_ptr->register_parser(sys_name, enter_foo, file_code_type(code));

	sys_gm_cal_ptr_type sys_gm_cal_ptr(new sys_gm_cal_type(sys_gm_ptr));

	std::cout << "wait database" << std::endl;
	char cccc = 0;
	std::cin >> cccc;

	login_type login(string_type("root"), string_type("root"));
	sys_gm_cal_ptr->cal_login(1000, login, (void*)0, boost::bind(&test_helper));

	std::cout << "---------------------------------------" << std::endl;
	std::string command_add_user1 = "adduser --name user1 --pwd 123456 --sys_lv login = 100 item = 200 --ip_limit 192.168.2.1 192.168.2.2";
	std::string command_add_user2 = "adduser --name user2 --pwd 123456 --sys_lv login = 100 item = 200 --ip_limit 192.168.2.1 192.168.2.2";

	sys_gm_cal_ptr->cal_ctrl_cmd(1000, ctrl_cmd_type(sys_name,string_type(command_add_user1)), 
								(void*)0,  boost::bind(&test_helper));

	sys_gm_cal_ptr->cal_ctrl_cmd(1000, ctrl_cmd_type(sys_name,string_type(command_add_user2)), 
								(void*)0,  boost::bind(&test_helper));

	std::cout << sys_gm_ptr->get_user_list<error_container_type>().what() << std::endl;
	std::cout << sys_gm_ptr->get_system_level_list<error_container_type>(string_type("user1")).what() << std::endl;
	std::cout << sys_gm_ptr->get_ip_limit<error_container_type>(string_type("user1")).what() << std::endl;

	std::cout << "---------------------------------------" << std::endl;

	std::string command_get_user_list = "userlist";
	sys_gm_cal_ptr->cal_ctrl_cmd(1000, ctrl_cmd_type(sys_name,string_type(command_get_user_list)), 
								(void*)0,  boost::bind(&test_helper));

	std::cout << "---------------------------------------" << std::endl;

	////std::string command_remove_users = "rmuser --name user1 user2";
	////error_container_type rst_command_remove_users(sys_gm_ptr->run_command<error_container_type>(1000, 
	////																			ctrl_cmd_type(sys_name,
	////																			string_type(command_remove_users))));
	////std::cout << rst_command_remove_users.code() << ", " << rst_command_remove_users.what() << std::endl;

	//error_container_type rst_msg_user_list2(sys_gm_ptr->run_command<error_container_type>(1000, 
	//																			ctrl_cmd_type(sys_name,
	//																			string_type(command_get_user_list))));
	//std::cout << rst_msg_user_list2.code() << ", " << rst_msg_user_list2.what() << std::endl;


	std::cout << "---------------------------------------" << std::endl;

	std::string command_set_passwd = "setpwd --name user1 --pwd abcdefg";
	sys_gm_cal_ptr->cal_ctrl_cmd(1000, ctrl_cmd_type(sys_name,string_type(command_set_passwd)), 
								(void*)0,  boost::bind(&test_helper));

	login_type login_user1(string_type("user1"), string_type("123456"));
	sys_gm_cal_ptr->cal_login(2000, login_user1, (void*)0, boost::bind(&test_helper));

	std::cout << "---------------------------------------" << std::endl;

	std::string command_set_sys_lv = "setsyslv --name user1 --sys_lv item = 300 login=200 test = 1000";
	sys_gm_cal_ptr->cal_ctrl_cmd(1000, ctrl_cmd_type(sys_name,string_type(command_set_sys_lv)), 
								(void*)0,  boost::bind(&test_helper));

	std::cout << sys_gm_ptr->get_system_level_list<error_container_type>(string_type("user1")).what() << std::endl;

	std::cout << "---------------------------------------" << std::endl;
	
	std::string command_get_sys_lv_list = "getsyslist --name user1";
	sys_gm_cal_ptr->cal_ctrl_cmd(1000, ctrl_cmd_type(sys_name,string_type(command_get_sys_lv_list)), 
								(void*)0,  boost::bind(&test_helper));

	std::cout << "---------------------------------------" << std::endl;
	
	std::string command_get_sys_lv1 = "getsyslv --name user1 --sys item";
	sys_gm_cal_ptr->cal_ctrl_cmd(1000, ctrl_cmd_type(sys_name,string_type(command_get_sys_lv1)), 
								(void*)0,  boost::bind(&test_helper));

	std::cout << "---------------------------------------" << std::endl;

	std::string command_get_sys_lv2 = "getsyslv --name user1 --sys test2";
	sys_gm_cal_ptr->cal_ctrl_cmd(1000, ctrl_cmd_type(sys_name,string_type(command_get_sys_lv2)), 
								(void*)0,  boost::bind(&test_helper));

	std::cout << "---------------------------------------" << std::endl;
	
	std::string command_rm_sys_lv1 = "rmsyslv --name user1 --sys item test";
	sys_gm_cal_ptr->cal_ctrl_cmd(1000, ctrl_cmd_type(sys_name,string_type(command_rm_sys_lv1)), 
								(void*)0,  boost::bind(&test_helper));

	std::cout << "---------------------------------------" << std::endl;
	
	std::string command_get_sys_lv3 = "getsyslv --name user1 --sys test";
	sys_gm_cal_ptr->cal_ctrl_cmd(1000, ctrl_cmd_type(sys_name,string_type(command_get_sys_lv3)), 
								(void*)0,  boost::bind(&test_helper));


	std::cout << "---------------------------------------" << std::endl;

	std::string command_ins_ip_lim = "insiplim --name user1 --ip_limit 192.168.2.3 192.168.2.4";
	sys_gm_cal_ptr->cal_ctrl_cmd(1000, ctrl_cmd_type(sys_name,string_type(command_ins_ip_lim)), 
								(void*)0,  boost::bind(&test_helper));

	std::cout << sys_gm_ptr->get_ip_limit<error_container_type>(string_type("user1")).what() << std::endl;

	std::cout << "---------------------------------------" << std::endl;

	std::string command_get_ip_lim = "getiplim --name user1";
	sys_gm_cal_ptr->cal_ctrl_cmd(1000, ctrl_cmd_type(sys_name,string_type(command_get_ip_lim)), 
								(void*)0,  boost::bind(&test_helper));
	std::cout << "---------------------------------------" << std::endl;

	//{
	//std::cout << "rrrrrrrrrrrrrrrrr" << std::endl;
	//login_type login_user1_2(string_type("user1"), string_type("abcdefg"));
	//yggr::u32 ip = 3232236033; //192.168.2.1
	//std::cout << sys_gm_ptr->login<error_container_type>(2000, ip, login_user1_2).what() << std::endl;
	//std::cout << "rrrrrrrrrrrrrrrrr" << std::endl;
	//}

	std::string command_commit = "commit";
	sys_gm_cal_ptr->cal_ctrl_cmd(1000, ctrl_cmd_type(sys_name,string_type(command_commit)), 
								(void*)0,  boost::bind(&test_helper));

	std::cout << "---------------------------------------" << std::endl;

	login_type login_user1_2(string_type("user1"), string_type("abcdefg"));
	yggr::u32 ip = 3232236033; //192.168.2.1
	std::cout << sys_gm_ptr->login<error_container_type>(2000, ip, login_user1_2).what() << std::endl;

	std::string command_ins_ip_lim_user1 = "insiplim --name user1 --ip_limit 192.168.2.3 192.168.2.4";
	sys_gm_cal_ptr->cal_ctrl_cmd(2000, ctrl_cmd_type(sys_name,string_type(command_ins_ip_lim_user1)), 
								(void*)0,  boost::bind(&test_helper));

	std::string command_rm_ip_lim = "rmiplim --name user2 --ip_limit 192.168.2.1 192.168.2.4";
	sys_gm_cal_ptr->cal_ctrl_cmd(1000, ctrl_cmd_type(sys_name,string_type(command_rm_ip_lim)), 
								(void*)0,  boost::bind(&test_helper));

	login_type login_user2_1(string_type("user2"), string_type("123456"));
	std::cout << sys_gm_ptr->login<error_container_type>(3000, ip, login_user2_1).what() << std::endl;

	char cc = 0;
	std::cin >> cc;
}

int main(int argc, char* argv[])
{
	std::cout << (int)'0' << std::endl;
	//test_parser_mgr();

	//user_info_mgr_test();

	//test_system_gm();

	//test_lua_system_gm();

	//test_db_and_gm_system();
	
	//test_gm_sys_calculater();
	//yggr::time::time t(1338771805, 0);

	//std::tm tm(t.to_local_tm());

	//char str[64] = {0};
	//std::strftime(str, sizeof(str) - sizeof(char), "%Y-%m-%d %X", std::localtime(&tm));
	char cc = 0;
	std::cin >> cc;

	return 0;
}