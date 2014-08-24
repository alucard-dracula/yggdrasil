//CltTest

#include <boost/system/system_error.hpp>
#include <iostream>

#include <yggr/network/network_packet.hpp>
#include <yggr/network/network_tcp_config.hpp>

#include <yggr/archive/net_oarchive.hpp>
#include <yggr/archive/net_iarchive.hpp>

#include <yggr/network/connection.hpp>
#include <yggr/network/tcp_linker.hpp>

#include <yggr/client/tcp_clt_handler.hpp>
#include <yggr/client/client.hpp>

#include <yggr/archive/network_archive_partner.hpp>

#include <yggr/safe_struct/safe_queue.hpp>

#include <yggr/data_center/recver_handler_mgr_single_foo.hpp>
#include <yggr/data_center/task.hpp>
#include <yggr/data_center/auto_recv_data_center.hpp>
#include <yggr/data_center/val_saver.hpp>

#include <yggr/adapter/base_send_adapter.hpp>
#include <yggr/adapter/base_recv_adapter.hpp>
#include <yggr/adapter/adapter_mgr.hpp>

#include <yggr/network/pak_back_id.hpp>

#include <yggr/thread/work_runner.hpp>
#include <yggr/thread/thread_mgr.hpp>
#include <yggr/thread/work_thread.hpp>
#include <yggr/thread/boost_thread_config.hpp>

#include <yggr/base/ctrl_center.hpp>
#include <boost/asio/error.hpp>

#include <signal.h>

#include <yggr/log/yggr_exception_log_accesser.hpp>

#include <boost/exception/exception.hpp>

#include <yggr/ids/base_ids_def.hpp>
#include <set>

#ifdef _MSC_VER
#include <vld.h>
#endif //_MSC_VER

typedef yggr::system_controller::ctrl_center ctrl_center_type;

typedef yggr::network::network_packet::network_packet<yggr::archive::archive_partner::network_oarchive_partner> opacket_type;
typedef yggr::network::network_packet::network_packet<yggr::archive::archive_partner::network_iarchive_partner> ipacket_type;


typedef yggr::network::tcp_linker<yggr::network::network_config::tcpv4_config> linker_type;
typedef yggr::network::connection<linker_type, opacket_type, ipacket_type> conn_type;

typedef opacket_type::base_type opack;
typedef ipacket_type::base_type ipack;

template<typename ID, typename Pak_ID = yggr::u32, typename Ver = yggr::u16>
class err_pak_back_id
{
public:
	typedef Pak_ID pak_id_type;
	typedef Ver pak_ver_type;
	typedef ID id_type;

private:
	typedef err_pak_back_id this_type;

public:
//private:
	enum
	{
		E_pak_id_start = 0x00008000,
		E_pak_id = E_pak_id_start + 1,

		E_id_compile_u32 = 0xffffffff
	};

	enum
	{
		E_version_start = 0x0000000,
		E_now_ver,

		E_ver_compile_u32 = 0xffffffff
	};
private:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, yggr::u32 version)
	{
		ar & id;
		ar & str;
	}

public:

	err_pak_back_id(void)
		: id(0), str("abcdefg")
	{
	}

	explicit err_pak_back_id(const id_type& nid)
		: id(nid), str("abcdefg")
	{
	}

	err_pak_back_id(const this_type& pak)
		: id(pak.id), str(pak.str)
	{
	}

	~err_pak_back_id(void)
	{
	}

	const this_type& operator=(const this_type& right)
	{
		id = right.id;
		str = right.str;
		return *this;
	}

#include <yggr/support/task_import.ipp>
#include <yggr/support/condition_fast.ipp>
	id_type id;
	std::string str;
};

typedef err_pak_back_id<yggr::ids::id64_type> test_pak_type;

typedef yggr::data_center::task<opack::data_cmp_type, yggr::ids::id64_type> task_type;
typedef yggr::safe_struct::safe_queue<task_type> task_container_type;
typedef yggr::data_center::val_saver<task_container_type> val_saver_type;

typedef yggr::data_center::auto_recv_data_center<val_saver_type, yggr::data_center::recver_handler_mgr_single_foo> data_center_type;

typedef yggr::adapter::adapter_mgr<data_center_type, ipack, boost::any, opack,
									yggr::adapter::base_send_adapter,
									yggr::adapter::base_recv_adapter
									> adapter_mgr_type;

typedef yggr::client::tcp_clt_handler<test_pak_type, adapter_mgr_type, conn_type>  tcp_clt_handler_type;
typedef yggr::client::client<tcp_clt_handler_type> clt_type;

typedef yggr::ptr_single<clt_type> clt_ptr_single_type;

typedef yggr::thread::work_runner<data_center_type> work_runner_type;
typedef work_runner_type::shared_info_ptr_type runner_shared_info_ptr_type;

typedef yggr::thread::work_thread<yggr::thread::boost_thread_config_type> work_thread_type;
typedef yggr::thread::thread_mgr<work_thread_type> trd_mgr_type;

static yggr::u32 send_count = 0;

struct Calculator
{
public:
	class error
	{
	public:
		enum
		{
			E_success = 0,
			E_code_test,
			E_compile_u32 = 0xffffffff
		};

		typedef std::string err_string;

	public:
		error(yggr::u32 code)
			:_code(code), _class_name("[class Calculator]")
		{
		}

		error(yggr::u32 code, const err_string& omsg)
			:_code(code), _other_msg(omsg),  _class_name("[class Calculator]")
		{
		}

		error(const error& err)
			: _code(err._code), _other_msg(err._other_msg),  _class_name(err._class_name)
		{
		}

		~error(void)
		{
		}

		operator bool(void) const
		{
			return _code != 0;
		}

		yggr::u32 code(void) const
		{
			return _code;
		}


		const err_string message(void) const
		{
			err_string msg = _class_name + " " + _other_msg;
			switch(_code)
			{
			case E_code_test:
				msg += " ------ E_code_test";
				break;
			default:
				msg += " ------  unknow error";
			}

			return msg;
		}

		const err_string what(void) const
		{
			return message();
		}

	private:
		yggr::u32 _code;
		err_string _other_msg;
		const err_string _class_name;
	};
	//class error E----------------------------------------------------------

	typedef error error_type;

	typedef yggr::u64 user_id_type;

	typedef std::multiset<user_id_type> user_id_container_type;


#include <yggr/support/runner_condition_and_result.ipp>


	template<typename Runner, typename Action_Table, typename Recv_Handler>
	void register_cal_object(Action_Table& at, Recv_Handler handler)
	{
		typedef Runner runner_type;
		typedef Action_Table action_table_type;
		typedef Recv_Handler recv_handler_type;


		at.template register_calculator<test_pak_type, Recv_Handler>(handler,
															boost::bind(&Calculator::cal_test_pak_type<user_id_type, runner_type, recv_handler_type>,
																			this, _1, _2, _3, _4));
	}


	template<typename User_ID, typename Runner, typename Handler>
	void cal_test_pak_type(const User_ID& uid, const test_pak_type& cdt, Runner* prunner, Handler handler)
	{
		//static int count = 0;
		static int i = 0;
		if(i != send_count)
		{
			++i;
			std::cout << "-------------calculate " << i << "-----------------------"<< std::endl;
			std::cout << "id = " << cdt.id << std::endl;

			clt_ptr_single_type::obj_ptr_type ptr = clt_ptr_single_type::get_c_ins();

			if(ptr)
			{
				try
				{
					std::cout << (ptr->str_local_socket_info()) << std::endl;
					std::cout << (ptr->str_remote_socket_info()) << std::endl;
				}
				catch(const compatibility::stl_exception& e)
				{
					std::cerr << e.what() << std::endl;
				}
				catch(const boost::system::system_error& e)
				{
					std::cerr << e.what() << std::endl;
				}
			}

			user_id_container_type uids;
			uids.insert(uid);
			handler(prunner, error(0), static_cast<yggr::u32>(E_CAL_RESULT), cdt.data_type(), uids, cdt);
		}

		return;
	}
};

//void sign_print(int i)
//{
//    //std::cout << i << std::endl;
//    yggr::ptr_single<yggr::exception::exception>::obj_ptr_type pe = yggr::ptr_single<yggr::exception::exception>::get_ins();
//    if(pe)
//    {
//        //pe->post(i);
//        std::cout << i << std::endl;
//    }
//}

int main(int argc, char* argv[])
{
//    for(int i = 1; i != 138; ++i)
//    {
//        signal(i, sign_print);
//    }
//    //signal();
	typedef yggr::ptr_single<data_center_type> data_center_single;
	typedef yggr::ptr_single<ctrl_center_type> ctrl_center_single;
	typedef yggr::ptr_single<yggr::log::yggr_exception_log_accesser_type> log_acc_single;


	ctrl_center_single::init_ins();
	yggr::log::yggr_exception_log_accesser_type::init_type initlog;
	log_acc_single::init_ins(initlog);
	log_acc_single::obj_ptr_type plog = log_acc_single::get_ins();

	yggr::ptr_single<yggr::exception::exception>::init_ins();


	initlog.push_back("ccc.txt");
	initlog.push_back("ddd.txt");


	if(plog)
	{
		plog->register_msg(10061, initlog.front());
		plog->register_msg(10054, initlog.back());
	}

	std::cout << "please input num" << std::endl;
	std::cin >> send_count;

	//ctrl_center_single::init_ins();

	yggr::ptr_single<yggr::exception::exception>::obj_ptr_type pe = yggr::ptr_single<yggr::exception::exception>::get_ins();

    trd_mgr_type trd_mgr;

	data_center_single::init_ins();

	try
	{
		std::string str_socket("127.0.0.1 : 9000");
		//std::cin >> ip;
		//clt_type::init_type clt_init(str_socket);

		clt_ptr_single_type::init_ins((clt_type::init_type(str_socket, *(data_center_single::get_ins()))));
		clt_type& clt = *(clt_ptr_single_type::get_ins());

		runner_shared_info_ptr_type shared_info_ptr(work_runner_type::create_work_runner_shared_info(2));

		Calculator my_cal;
		shared_info_ptr->register_calculator(my_cal);

		clt.register_network_protocol<test_pak_type>();


		trd_mgr
			.add_thread()
            .add_thread()
			.add_thread()
			.add_thread();

		trd_mgr.startup_all<work_runner_type>(work_runner_type::init_type(*(data_center_single::get_ins()), shared_info_ptr));

		clt.run();

		char cc = 0;
		std::cin >> cc;

		trd_mgr.stop_all();

		ctrl_center_single::uninstall();
		clt_ptr_single_type::uninstall();
		data_center_single::uninstall();

	}
	catch(const compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch(const boost::system::system_error& e)
	{
		std::cerr << e.what() << std::endl;
	}

	yggr::ptr_single<yggr::exception::exception>::uninstall();
	log_acc_single::uninstall();

	return 0;
}
