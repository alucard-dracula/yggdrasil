// dllmain.cpp : Defines the entry point for the DLL application.

#include "lua_calculator.hpp"

#include <yggr/base/ptr_single.hpp>
#include <yggr/network/pak_back_id.hpp>
#include <yggr/base/exception.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <yggr/script/lua_script.hpp>
#include <yggr/script/lua_environment.hpp>
#include <yggr/script/script_mgr.hpp>

#include <cal_svr_procotol_impl.hpp>

#include <yggr/base/error_make.hpp>

#include <set>

#if defined(_MSC_VER) && defined(_DEBUG)
#   include <vld.h>
#endif // defined(_MSC_VER) && defined(_DEBUG)

typedef yggr::network::network_packet::pak_back_id<yggr::u64> test_pak_type;

typedef yggr::script::script_mgr<std::string, yggr::script::lua::lua_script,
									yggr::script::lua::lua_environment> script_mgr_type;

typedef yggr::ptr_single<script_mgr_type> script_mgr_single_type;

struct Calculator : public boost::enable_shared_from_this<Calculator>
{
public:
	ERROR_MAKER_BEGIN("Calculator")
		ERROR_CODE_DEF_NON_CODE_BEGIN()
			ERROR_CODE_DEF(E_code_test)
		ERROR_CODE_DEF_NON_CODE_END()

		ERROR_CODE_MSG_BEGIN()
			ERROR_CODE_MSG(E_code_test, "E_code_test")
		ERROR_CODE_MSG_END()
	ERROR_MAKER_END()

	typedef yggr::u64 user_id_type;

	typedef std::multiset<user_id_type> user_id_container_type;

#include <yggr/support/runner_condition_and_result.ipp>



	friend class yggr::system_controller::ctrl_center;
	template<typename Tag, typename Ctrl_Center>
	void register_dispatchers(Ctrl_Center& cc)
	{
		CTRL_REG_DISPATCHER_HANDLER_PARAM_1(yggr::u64, cc, boost::asio::error::connection_reset,//10054
											yggr::system_controller::system_code::E_SESSION_SYSTEM,
											boost::bind(&Calculator::cal_10054_dispath<yggr::u64>, shared_from_this(), _1));
	}

	template<typename Runner, typename Action_Table, typename Recv_Handler>
	void register_cal_object(Action_Table& at, const Recv_Handler& handler)
	{
		typedef Runner runner_type;
		typedef Action_Table action_table_type;
		typedef Recv_Handler recv_handler_type;

		std::cout << "register_cal_object" << std::endl;
		at.template register_calculator<cdt_procotol_type, Recv_Handler>(handler,
																			boost::bind(&Calculator::cal_test_pak_type<user_id_type,
																							runner_type, recv_handler_type>,
																			shared_from_this(), _1, _2, _3, _4));
	}


	template<typename User_ID, typename Runner, typename Handler>
	void cal_test_pak_type(const User_ID& uid, const cdt_procotol_type& cdt, Runner* prunner, const Handler& handler)
	{
		std::cout << "calculate" << std::endl;

		rst_procotol_type rst;
		script_mgr_single_type::get_ins()->execute_rule("cal_num", cdt, rst);

		user_id_container_type uids;
		uids.insert(uid);
		handler(prunner, error(0), static_cast<yggr::u32>(E_CAL_RESULT), rst.data_info(), uids, rst);
	}

	template<typename User_ID>
	void cal_10054_dispath(const User_ID& id)
	{
		std::cout << "client id = [" << id << "] is removed" << std::endl;
	}
};

typedef yggr::ptr_single<Calculator> cal_sig_type;

void reg_cal_enter(runner_shared_info_ptr_type ptr,
							exception_ptr_single_type::obj_ptr_type ptr_ex,
							svr_ptr_single_type::obj_ptr_type ptr_svr)
{
	exception_ptr_single_type::init_ins(ptr_ex);
	svr_ptr_single_type::init_ins(ptr_svr);
	script_mgr_single_type::init_ins();
	script_mgr_single_type::get_ins()->append_module(script_mgr_type::E_lib_all);
	script_mgr_single_type::get_ins()->insert("cal_num", "cal", script_mgr_type::file_code_type("cal_num_test.lua"));

	if(ptr_svr)
	{
		ptr_svr->register_network_protocol<cdt_procotol_type>();
		ptr_svr->register_network_protocol<rst_procotol_type>();
	}

	if(ptr)
	{
		ptr->register_calculator(*cal_sig_type::init_ins());
	}
}

#include <yggr/dll_make/dll_main.ipp>
