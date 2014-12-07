//adapter_data_center_work_runner_test.cpp

#define USE_MYSQL

#include <iostream>

#include <yggr/network/socket_conflict_fixer.hpp>
#include <yggr/database_system/dtl_include_helper.hpp>

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/base/exception.hpp>

#include <yggr/database_system/database_connect.hpp>
#include <yggr/database_system/database_operator.hpp>
#include <yggr/database_system/database_command.hpp>
#include <yggr/database_system/database_calculator.hpp>

#include <yggr/thread/boost_thread_config.hpp>
#include <yggr/thread/dynamic_work_runner.hpp>
#include <yggr/thread/work_thread.hpp>
#include <yggr/thread/thread_mgr.hpp>

#include <yggr/task_center/task.hpp>
#include <yggr/task_center/task_creator.hpp>
#include <yggr/task_center/task_center.hpp>
#include <yggr/task_center/task_store_op.hpp>
#include <yggr/task_center/task_saver.hpp>
#include <yggr/task_center/recver_handler_mgr.hpp>

#include <yggr/ids/uuid.hpp>
#include <yggr/ids/base_ids_def.hpp>
#include <yggr/network/network_info.hpp>

#include <yggr/safe_container/safe_hash_map_queue.hpp>

#ifdef _MSC_VER
#include <vld.h>
#endif //_MSC_VER

#if defined(_MSC_VER)
#if defined(_DEBUG)
#	pragma comment(lib, "libdtl-vc100-d.lib")
#else
#	pragma comment(lib, "libdtl-vc100.lib")
#endif //_DEBUG
#endif // _MSC_VER

typedef yggr::network::network_info<yggr::ids::id64_type> network_info_type;

typedef yggr::database_system::database_command<yggr::u32, yggr::u16> database_command_type;

typedef yggr::ids::id_generator<yggr::ids::uuid,
								yggr::ids::parse_uuid_genner::lagged_fibonacci607_type> task_id_gen_type;

typedef yggr::task_center::task<yggr::ids::uuid, network_info_type,
									yggr::task_center::default_task_data_info_type> task_type;

typedef yggr::safe_container::safe_hash_map_queue<task_type::id_type, task_type> task_container_type;
//typedef yggr::safe_container::safe_hash_map_queue<network_info_type::owner_id_type, task_type> task_container_type;

typedef yggr::task_center::key_value_store<task_container_type::value_type, task_type::task_id_getter> task_store_type;

typedef yggr::task_center::task_saver<yggr::task_center::type_traits::mark_saver_condition, task_type,
										task_container_type, task_store_type> cdt_saver_type;

typedef yggr::task_center::recver_handler_mgr<yggr::task_center::type_traits::mark_saver_result, task_type> rst_saver_type;

typedef yggr::task_center::task_creator<task_id_gen_type, task_type> task_creator_type;
typedef yggr::task_center::task_center<task_creator_type, cdt_saver_type, rst_saver_type> task_center_type;

typedef yggr::task_center::support::task_data_info_parser<yggr::task_center::default_task_data_info_type> task_data_info_parser_type;

YGGR_PP_THREAD_ACTRION_HELPER_MAKE_REG_ID_PARSER(action_reg_string_parser,
													std::string,
													task_data_info_parser_type,
													class_name)

typedef yggr::thread::action_helper::action_reg_string_parser action_reg_id_parser_type;

typedef yggr::thread::action_helper::action_id_parser<action_reg_id_parser_type,
														task_type::class_name_getter> action_id_parser_type;

typedef yggr::thread::dynamic_work_runner<task_center_type, action_id_parser_type> work_runner_type;

typedef work_runner_type::shared_info_ptr_type runner_shared_info_ptr_type;

typedef yggr::thread::work_thread<yggr::thread::boost_thread_config_type> work_thread_type;
typedef yggr::thread::thread_mgr<work_thread_type> trd_mgr_type;

typedef dtl::DBConnection real_conn_type;
typedef yggr::database_system::database_connect<real_conn_type> conn_type;
typedef yggr::database_system::database_operator<conn_type> db_op_type;
typedef yggr::database_system::database_calculator<network_info_type, database_command_type, db_op_type> database_calculator_type;


class t_s
{
public:
	yggr::s32 uid;
	std::string uname;
	yggr::s64 utm;
};

std::ostream& operator<<(std::ostream& os, const t_s& ts)
{
	os << ts.uid <<","<< ts.uname<<","<<ts.utm;
	return os;
}

namespace dtl
{
template<>
class DefaultBCA<t_s>
{
public:
	void operator()(dtl::BoundIOs& cols, t_s& row_buf){
		cols["uid"] == row_buf.uid;
		cols["uname"] == row_buf.uname;
		cols["utm"] == row_buf.utm;
	}
};

template<>
class DefaultBPA<t_s>
{
public:
	void operator()(dtl::BoundIOs& cols, t_s& row_buf)
	{
		cols[0] << row_buf.uid;
	}
};

} // namespace dtl

void out_result_t_s(bool bright, const std::vector<t_s>& rows)
{
	std::cout << "----------------out_result_t_s-------------" << std::endl;
	int isize = rows.size();
	for(int i = 0; i != isize; ++i)
	{
		std::cout << rows[i] << std::endl;
	}
}

bool get_insert_rows(std::vector<t_s>& rows)
{
	t_s ts;
	ts.uid = 100;
	ts.uname = "zzz";
	ts.utm = 100;

	rows.push_back(ts);
	return true;
}

bool get_update_rows(std::vector<t_s>& rows)
{
	t_s ts;
	ts.uid = 100;
	ts.uname = "kkk";
	ts.utm = 100;

	rows.push_back(ts);
	return true;
}

bool get_delete_rows(std::vector<t_s>& rows)
{
	t_s ts;
	ts.uid = 100;
	ts.uname = "kkk";
	ts.utm = 100;

	rows.push_back(ts);
	return true;
}

//---------------------------------------------------------------

class t_s2
{
public:

	yggr::s32 uid;
	std::string uname;
	//yggr::s32 utm;
};

std::ostream& operator<<(std::ostream& os, const t_s2& ts)
{
	os << ts.uid <<","<< ts.uname;
	return os;
}

namespace dtl
{
template<>
class DefaultBCA<t_s2>
{
public:
	void operator()(dtl::BoundIOs& cols, t_s2& row_buf){
		cols["uid"] == row_buf.uid;
		cols["uname"] == row_buf.uname;

	}
};
} // namespace dtl

void out_result_t_s2(bool bright, const std::vector<t_s2>& rows)
{
	std::cout << "----------------out_result_t_s2-------------" << std::endl;
	int isize = rows.size();
	for(int i = 0; i != isize; ++i)
	{
		std::cout << rows[i] << std::endl;
	}
}

void select_t_s_test(void)
{
#	if defined(USE_MSSQL)
	std::string cstr = "DRIVER={SQL Server};Server=.;Database=Game;Uid=sa;Pwd=123456;";
#	elif defined(USE_MYSQL)
	std::string cstr = "DRIVER={MySQL ODBC 5.1 Driver};SERVER=localhost;DATABASE=Game;UID=root;PASSWORD=123456;OPTION=3;";
#	else
	std::string cstr;
#	endif // WIN32

	if(cstr.empty())
	{
		std::cerr << "dns empty" << std::endl;
	}


	typedef yggr::ptr_single<task_center_type> task_center_single;

	task_center_single::init_ins();

	//work_runner_type::init_slow_runner(2);
	runner_shared_info_ptr_type shared_info_ptr(work_runner_type::create_work_runner_shared_info(2));

	database_calculator_type::register_select_view<t_s>("select", boost::bind(out_result_t_s, _1, _2));
	database_calculator_type::register_insert_view<t_s>("insert", boost::bind(get_insert_rows, _1));
	database_calculator_type::register_update_view<t_s>("update", boost::bind(get_update_rows, _1));
	database_calculator_type::register_select_view<t_s2>("select", boost::bind(out_result_t_s2, _1, _2));
	database_calculator_type::register_delete_view<t_s>("delete", boost::bind(get_delete_rows, _1));
	database_calculator_type::register_select_view<t_s2>("select", boost::bind(out_result_t_s2, _1, _2));

	boost::shared_ptr<database_calculator_type> my_cal(new database_calculator_type(cstr));
	shared_info_ptr->register_calculator(*my_cal);


	trd_mgr_type trd_mgr;
	trd_mgr
		.add_thread()
		.add_thread()
		.add_thread()
		.add_thread();

	trd_mgr.startup_all<work_runner_type>(*(task_center_single::get_ins()), shared_info_ptr);


	std::vector<std::string> tb_names;
	tb_names.push_back("test2");

	//test1
	{
	task_center_single::get_ins()
		->send<task_center_type::cdt_mark_type>(network_info_type(yggr::ids::id64_type(0x00000001)),
													database_command_type("delete", typeid(t_s).name(), tb_names, ""));

	task_center_single::get_ins()
		->send<task_center_type::cdt_mark_type>(network_info_type(yggr::ids::id64_type(0x00000001)),
													database_command_type("insert", typeid(t_s).name(), tb_names, ""));

	task_center_single::get_ins()
		->send<task_center_type::cdt_mark_type>(network_info_type(yggr::ids::id64_type(0x00000001)),
													database_command_type("select", typeid(t_s).name(), tb_names, ""));

	task_center_single::get_ins()
		->send<task_center_type::cdt_mark_type>(network_info_type(yggr::ids::id64_type(0x00000001)),
													database_command_type("update", typeid(t_s).name(), tb_names, "where uid = (?)"));

	task_center_single::get_ins()
		->send<task_center_type::cdt_mark_type>(network_info_type(yggr::ids::id64_type(0x00000001)),
													database_command_type("select", typeid(t_s).name(), tb_names, ""));

	}

	//test2
	{
		tb_names.push_back("(select uname tname, max(utm) mutm from test2 group by uname) tmp");
		task_center_single::get_ins()
			->send<task_center_type::cdt_mark_type>(network_info_type(yggr::ids::id64_type(0x00000001)),
														database_command_type("select", typeid(t_s2).name(), tb_names,
																				"where uname = tname and utm = mutm"));
	}

	char cc = 0;
	std::cin >> cc;

	trd_mgr.stop_all();

	std::cout << "end" << std::endl;
	task_center_single::uninstall();
}


#include <boost/any.hpp>
int main(int argc, char* argv[])
{
	select_t_s_test();
	return 0;
}


