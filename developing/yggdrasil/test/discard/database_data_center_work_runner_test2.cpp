//adapter_data_center_work_runner_test.cpp //这种方案odbc 不支持

#error "odbc don't support this mode and this file is oldder!!!!!!!!!!"
#include <iostream>


#define USE_MYSQL

#if defined(_MSC_VER)
#if defined(_DEBUG)
#   pragma comment(lib, "libdtl-vc100-d.lib")
#else
#	pragma comment(lib, "libdtl-vc100.lib")
#endif // _DEBUG
#   pragma comment(lib, "odbc32.lib")
#endif //_MSC_VER

#include <boost/asio.hpp>
#include <iostream>

#include <yggr/base/yggrdef.h>


#include <yggr/tesk_center/task.hpp>
#include <yggr/tesk_center/auto_recv_data_center.hpp>
#include <yggr/tesk_center/key_val_saver.hpp>
#include <yggr/safe_struct/safe_hash_map_queue.hpp>

#include <yggr/tesk_center/recver_handler_mgr_single_foo.hpp>


#include <yggr/database_system/database_command.hpp>
#include <yggr/database_system/database_calculater.hpp>
#include <yggr/database_system/database_operator.hpp>
#include <yggr/database_system/database_connect.hpp>

#include <yggr/thread/boost_thread_config.hpp>
#include <yggr/thread/work_thread.hpp>
#include <yggr/thread/work_runner.hpp>
#include <yggr/thread/thread_mgr.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>

#ifdef WIN32
#include <vld.h>
#endif //WIN32

typedef yggr::database_system::database_command<yggr::u32, yggr::u16> database_command_type;

typedef yggr::data_center::task<database_command_type::cmp_type, yggr::u64> task_type;
typedef yggr::safe_struct::safe_hash_map_queue<task_type::id_type, task_type> task_container_type;
typedef yggr::data_center::key_val_saver<task_container_type> val_saver_type;
typedef yggr::data_center::auto_recv_data_center<val_saver_type, yggr::data_center::recver_handler_mgr_single_foo> data_center_type;


typedef yggr::thread::work_runner<data_center_type> work_runner_type;
typedef work_runner_type::shared_info_ptr_type runner_shared_info_ptr_type;

typedef yggr::thread::work_thread<yggr::thread::boost_thread_config_type> work_thread_type;
typedef yggr::thread::thread_mgr<work_thread_type> trd_mgr_type;

typedef dtl::DBConnection real_conn_type;
typedef yggr::database_system::database_connect<real_conn_type> conn_type;
typedef yggr::database_system::database_operator<conn_type> db_op_type;

typedef yggr::database_system::database_calculater<yggr::u64, database_command_type, db_op_type> database_calculater_type;


class item_user_load_table										//角色道具读取
{
public:
	std::string		m_strName;				//玩家名称
	yggr::u32		m_nBagSize;				//包裹大小
	yggr::u32		m_nStoreSize;			//仓库大小
	yggr::u32		m_nGold;				//金钱
	yggr::u32		m_nSCB;					//商城币
	yggr::s64		m_nItemId;				//道具唯一id
	yggr::u32		m_nSaveTable;			//存储格
	yggr::u32		m_nBasicId;				//基础Id
	yggr::u32		m_nNum;					//数量
	yggr::u32		m_nEndur;				//耐久

	item_user_load_table(void)
		:m_nGold(0), m_nSCB(0), m_nBagSize(0), m_nStoreSize(0)
		,m_nItemId(0), m_nSaveTable(0), m_nBasicId(0), m_nNum(0), m_nEndur(0)
	{
	}

	item_user_load_table(std::string name, yggr::u32 gold, yggr::u32 scb,
						yggr::u32 bag_size, yggr::u32 store_size,yggr::s64 id,
						yggr::u32 save, yggr::u32 basic_id, yggr::u32 num, yggr::u32 endur)
		:m_strName(name), m_nGold(0), m_nSCB(0), m_nBagSize(0), m_nStoreSize(0)
		,m_nItemId(id), m_nSaveTable(save), m_nBasicId(basic_id), m_nNum(num), m_nEndur(endur)
	{
	}

	virtual ~item_user_load_table(void)
	{
	}
};

namespace dtl
{

template<>
class DefaultBCA<item_user_load_table>
{
public:
	void operator()(BoundIOs& cols, item_user_load_table& row_buf)
	{
		cols["uname"]		== row_buf.m_strName;
		cols["gold"]		== row_buf.m_nGold;
		cols["scb"]			== row_buf.m_nSCB;
		cols["bag_size"]	== row_buf.m_nBagSize;
		cols["store_size"]	== row_buf.m_nStoreSize;
		cols["iid"]			== row_buf.m_nItemId;
		cols["save_table"]	== row_buf.m_nSaveTable;
		cols["basic_iid"]	== row_buf.m_nBasicId;
		cols["num"]			== row_buf.m_nNum;
		cols["endur"]		== row_buf.m_nEndur;
	}
};
} //namespace dtl


typedef std::vector<item_user_load_table> item_user_load_table_vector;

void load_item_by_player(bool bright, const item_user_load_table_vector& rows)
{
	std::cout << bright << std::endl;
};

void select_t_s_test(void)
{

#	if defined(USE_MSSQL)
	std::string cstr = "DRIVER={SQL Server};Server=.;Database=Game;Uid=sa;Pwd=12345;";
#	elif defined(USE_MYSQL)
	std::string cstr = "DRIVER={MySQL ODBC 5.1 Driver};SERVER=localhost;DATABASE=Game;UID=root;PASSWORD=12345;OPTION=3;";
#	else
	std::string cstr;
#	endif // WIN32

	if(cstr.empty())
	{
		std::cerr << "dns empty" << std::endl;
	}


	//yggr::exception::exception::init_type init(3, 0);
	yggr::ptr_single<yggr::exception::exception>::init_ins();
	typedef yggr::ptr_single<data_center_type> data_center_single;

	data_center_single::init_ins();

	runner_shared_info_ptr_type shared_info_ptr(work_runner_type::create_work_runner_shared_info(2));


	database_calculater_type::register_select_view<item_user_load_table>("LoadItemByPlayer",
			boost::bind(&load_item_by_player, _1, _2));

	boost::shared_ptr<database_calculater_type> my_cal(new database_calculater_type(cstr));
	//work_runner_type::register_calculater(my_cal);
	shared_info_ptr->register_calculater(*my_cal);

	trd_mgr_type trd_mgr;
	trd_mgr
		.add_thread()
		.add_thread()
		.add_thread();

	//trd_mgr.startup_all<work_runner_type>(*(data_center_single::get_ins()));
	trd_mgr.startup_all<work_runner_type>(work_runner_type::init_type(*(data_center_single::get_ins()), shared_info_ptr));

	std::vector<std::string> tb_names;

	/*std::string tb1("(select t1.user_name uname, t1.user_gold gold, "
					"t1.user_scb scb from item_coin_save t1,"
					"(select user_name, max(item_coin_save.this_time) max_time "
					"from item_coin_save group by user_name) tmp1 "
					"where t1.user_name = tmp1.user_name "
					"and t1.this_time = tmp1.max_time) new1");


	std::string tb2("(select t2.user_name, t2.user_bag_size bag_size, "
					"t2.user_store_size store_size from item_state_save t2, "
					"(select user_name, max(item_state_save.this_time) max_time "
					"from item_state_save group by user_name) tmp2 "
					"where t2.user_name = tmp2.user_name "
					"and t2.this_time = tmp2.max_time) new2");


	std::string tb3("(select t3.user_name, t3.item_id iid, "
					"t3.item_save_table save_table from item_user_save t3, "
					"(select user_name, max(item_user_save.this_time) max_time "
					"from item_user_save group by user_name) tmp3 "
					"where t3.user_name = tmp3.user_name "
					"and t3.this_time = tmp3.max_time) new3");


	std::string tb4("(select t4.item_id, t4.item_basic_id basic_iid, "
					"t4.item_num num, t4.item_endur endur from item_save t4, "
					"(select item_id, max(item_save.this_time) max_time "
					"from item_save group by item_id) tmp4 "
					"where t4.item_id = tmp4.item_id "
					"and t4.this_time = tmp4.max_time) new4");*/

	std::string tb1("(select t1.user_name uname, t1.user_gold gold, t1.user_scb scb from "
					"item_coin_save t1, "
					"(select user_name, max(this_time) max_time from "
					"item_coin_save group by user_name) tmp1 "
					"where t1.user_name = tmp1.user_name and t1.this_time = tmp1.max_time) new1");


	std::string tb2("(select t2.user_name uname2, t2.user_bag_size bag_size, t2.user_store_size store_size from "
					"item_state_save t2, "
					"(select user_name, max(this_time) max_time from "
					"item_state_save group by user_name) tmp2 "
					"where t2.user_name = tmp2.user_name and t2.this_time = tmp2.max_time) new2");


	std::string tb3("(select t3.user_name uname3, t3.item_id iid, t3.item_save_table save_table from "
					"item_user_save t3, "
					"(select user_name, max(this_time) max_time from "
					"item_user_save group by user_name) tmp3 "
					"where t3.user_name = tmp3.user_name and t3.this_time = tmp3.max_time) new3");


	std::string tb4("(select t4.item_id item_id, t4.item_basic_id basic_iid, t4.item_num num, t4.item_endur endur from "
					"item_save t4, "
					"(select item_id, max(this_time) max_time from "
					"item_save group by item_id) tmp4 "
					"where t4.item_id = tmp4.item_id and t4.this_time = tmp4.max_time) new4");

	tb_names.push_back(tb1);
	tb_names.push_back(tb2);
	tb_names.push_back(tb3);
	tb_names.push_back(tb4);

	std::string slt_str("where new1.uname = new2.uname2 and new1.uname = new3.uname3 and new3.iid = new4.item_id "
						"AND new1.uname = \'John\'");

	data_center_single::get_ins()->send(0x00001,
		database_command_type("LoadItemByPlayer",
		typeid(item_user_load_table).name(), tb_names, slt_str));


	tb_names.clear();

	char cc = 0;
	std::cin >> cc;

	trd_mgr.stop_all();

	data_center_single::uninstall();
}

int main(int argc, char* argv[])
{
	select_t_s_test();

	{
		char cc = 0;
		std::cin >> cc;
	}
	return 0;

}


