// dllmain.cpp : Defines the entry point for the DLL application.

//#include "lua_calculator.hpp"
//
//#include <boost/atomic.hpp>
//
//#include <yggr/base/ref_count_info.h>
//
//#include <yggr/script/lua_script.hpp>
//#include <yggr/script/lua_environment.hpp>
//#include <yggr/script/script_mgr.hpp>
//
//#include "lua_cal_svr_procotol_impl.hpp"
//
//#include <yggr/compile_link/linker.hpp>
//
//#if defined(_MSC_VER)
//#   include <vld.h>
//#endif // defined(_MSC_VER) 
//
//#include YGGR_PP_LINK_LIB(base)
//#include YGGR_PP_LINK_LIB(time)
//#include YGGR_PP_LINK_LIB(ids)
//#include YGGR_PP_LINK_LIB(charset)
//#include YGGR_PP_LINK_LIB(exception)
//#include YGGR_PP_LINK_LIB(system_controller)
//#include YGGR_PP_LINK_LIB(script_lua)
//
//typedef yggr::ids::inner_process_id inner_process_id_type;
//typedef yggr::ids::id_generator<inner_process_id_type> inner_process_id_gen_type;
//typedef yggr::ptr_single<inner_process_id_type> inner_process_id_single_type;
//typedef yggr::network::start_data::pak_back_id<yggr::ids::id64_type> test_pak_type;
//
//// lua script_mgr
//typedef yggr::script::script_mgr<std::string, yggr::script::lua::lua_script,
//									yggr::script::lua::lua_environment> script_mgr_type;
//
//typedef yggr::ptr_single<script_mgr_type> script_mgr_single_type;
//
//struct Calculator
//	: public boost::enable_shared_from_this<Calculator>
//{
//private:
//	typedef yggr::ref_count_info<> ref_count_info_type;
//private:
//	typedef Calculator this_type;
//
//public:
//	ERROR_MAKER_BEGIN("LUA_Svr_Calculator")
//		ERROR_CODE_DEF_NON_CODE_BEGIN()
//			ERROR_CODE_DEF(E_code_test)
//		ERROR_CODE_DEF_NON_CODE_END()
//
//		ERROR_CODE_MSG_BEGIN()
//			ERROR_CODE_MSG(E_code_test, "code test")
//		ERROR_CODE_MSG_END()
//	ERROR_MAKER_END()
//
//	typedef network_info_type owner_info_type;
//
//	typedef boost::unordered_set<owner_info_type> owner_info_container_type;
//
//	friend class yggr::system_controller::ctrl_center;
//
//	template<typename Tag, typename CtrlCenter>
//	void register_dispatchers(CtrlCenter& cc)
//	{
//		CTRL_HANDLER(Tag, cc, boost::asio::error::invalid_argument,
//								yggr::system_controller::system_code::E_SESSION_SYSTEM,
//								boost::bind(&this_type::cal_network_dispath,
//												this_type::shared_from_this(), _1));
//
//		CTRL_HANDLER(Tag, cc, boost::asio::error::message_size,
//								yggr::system_controller::system_code::E_SESSION_SYSTEM,
//								boost::bind(&this_type::cal_network_dispath,
//												this_type::shared_from_this(), _1));
//
//		CTRL_HANDLER(Tag, cc, boost::asio::error::name_too_long,
//								yggr::system_controller::system_code::E_SESSION_SYSTEM,
//								boost::bind(&this_type::cal_network_dispath,
//												this_type::shared_from_this(), _1));
//
//		CTRL_HANDLER(Tag, cc, boost::asio::error::network_down,
//								yggr::system_controller::system_code::E_SESSION_SYSTEM,
//								boost::bind(&this_type::cal_network_dispath,
//												this_type::shared_from_this(), _1));
//
//		CTRL_HANDLER(Tag, cc, boost::asio::error::no_buffer_space,
//								yggr::system_controller::system_code::E_SESSION_SYSTEM,
//								boost::bind(&this_type::cal_network_dispath,
//												this_type::shared_from_this(), _1));
//
//		CTRL_HANDLER(Tag, cc, boost::asio::error::no_memory,
//								yggr::system_controller::system_code::E_SESSION_SYSTEM,
//								boost::bind(&this_type::cal_network_dispath,
//												this_type::shared_from_this(), _1));
//
//		CTRL_HANDLER(Tag, cc, boost::asio::error::shut_down,
//								yggr::system_controller::system_code::E_SESSION_SYSTEM,
//								boost::bind(&this_type::cal_network_dispath,
//												this_type::shared_from_this(), _1));
//
//		CTRL_HANDLER(Tag, cc, boost::asio::error::timed_out,
//								yggr::system_controller::system_code::E_SESSION_SYSTEM,
//								boost::bind(&this_type::cal_network_dispath,
//												this_type::shared_from_this(), _1));
//
//		CTRL_HANDLER(Tag, cc, boost::asio::error::connection_reset,
//								yggr::system_controller::system_code::E_SESSION_SYSTEM,
//								boost::bind(&this_type::cal_network_dispath,
//												this_type::shared_from_this(), _1));
//
//		CTRL_HANDLER(Tag, cc, boost::asio::error::connection_aborted,
//								yggr::system_controller::system_code::E_SESSION_SYSTEM,
//								boost::bind(&this_type::cal_network_dispath,
//												this_type::shared_from_this(), _1));
//
//		CTRL_HANDLER(Tag, cc, boost::asio::error::operation_aborted,
//								yggr::system_controller::system_code::E_SESSION_SYSTEM,
//								boost::bind(&this_type::cal_network_dispath,
//												this_type::shared_from_this(), _1));
//
//        CTRL_HANDLER(Tag, cc, 2,
//								yggr::system_controller::system_code::E_SESSION_SYSTEM,
//								boost::bind(&this_type::cal_network_dispath,
//												this_type::shared_from_this(), _1));
//
//         CTRL_HANDLER(Tag, cc, 104,
//								yggr::system_controller::system_code::E_SESSION_SYSTEM,
//								boost::bind(&this_type::cal_network_dispath,
//												this_type::shared_from_this(), _1));
//
//		 CTRL_HANDLER(Tag, cc, 121,
//								yggr::system_controller::system_code::E_SESSION_SYSTEM,
//								boost::bind(&this_type::cal_network_dispath,
//												this_type::shared_from_this(), _1));
//
//		 CTRL_HANDLER(Tag, cc, 0x60009004,
//								yggr::system_controller::system_code::E_SESSION_SYSTEM,
//								boost::bind(&this_type::cal_network_dispath,
//												this_type::shared_from_this(), _1));
//
//		 CTRL_HANDLER(Tag, cc, 0x60020001,
//								yggr::system_controller::system_code::E_SESSION_SYSTEM,
//								boost::bind(&this_type::cal_network_dispath,
//												this_type::shared_from_this(), _1));
//
//		 CTRL_HANDLER(Tag, cc, session_type::close_code_type::value,
//								yggr::system_controller::system_code::E_SESSION_SYSTEM,
//								boost::bind(&this_type::cal_network_dispath,
//												this_type::shared_from_this(), _1));
//	}
//
//	template<typename Runner, typename Action_Table, typename Recv_Handler>
//	void register_cal_object(Action_Table& at, const Recv_Handler& handler)
//	{
//		typedef Runner runner_type;
//		typedef Action_Table action_table_type;
//		typedef Recv_Handler recv_handler_type;
//
//		at.template register_calculator<cdt_procotol_type, Recv_Handler>(
//				handler, boost::bind(&this_type::cal_test_pak_type<runner_type,
//																	recv_handler_type>,
//									this_type::shared_from_this(), _1, _2, _3, _4));
//
//	}
//
//	template<typename Action_Table>
//	void unregister_cal_object(Action_Table& at)
//	{
//		at.template unregister_calculator<cdt_procotol_type>();
//	}
//
//	//void print_socket_info(const session_mgr_type::smap_type::base_type& base,
//	//							session_mgr_type::smap_citer_type iter) const
//	//{
//	//	try
//	//	{
//	//		std::cout << iter->second->str_local_socket_info() << std::endl;
//	//		std::cout << iter->second->str_remote_socket_info() << std::endl;
//	//	}
//	//	catch(const compatibility::stl_exception& e)
//	//	{
//	//		std::cerr << e.what() << std::endl;
//	//	}
//	//	catch(const boost::system::error_code& e)
//	//	{
//	//		std::cerr << e.message() << std::endl;
//	//	}
//	//}
//
//	template<typename Runner, typename Handler>
//	void cal_test_pak_type(const owner_info_type& owner, const cdt_procotol_type& cdt, Runner* prunner, const Handler& handler)
//	{
//		_count.grab();
//
//		std::cout << "calculate: " << cdt << std::endl;
//
//		rst_procotol_type rst;
//		script_mgr_single_type::get_ins()->execute_rule(rst, yggr::const_args_anchor("cal_num"), boost::cref(cdt));
//
//		owner_info_container_type owners;
//		owners.insert(owner);
//		YGGR_PP_TASK_CENTER_BACK_TASK(handler, prunner, error_maker_type::make_error(0),
//										yggr::task_center::runtime_task_type::E_CAL_RESULT,
//										owners, rst);
//		_count.drop();
//	}
//
//	inline void cal_network_dispath(const yggr::u64& owner)
//	{
//		_count.grab();
//		std::cout << "10054 client id = [" << owner << "] is removed" << std::endl;
//		_count.drop();
//	}
//
//	void join(void)
//	{
//		for(;!_count.empty(););
//	}
//
//	ref_count_info_type _count;
//};
//
//typedef yggr::ptr_single<Calculator> cal_sig_type;
//
//YGGR_DLL_API void YGGR_MODULE_CALL_T() reg_cal_enter(runner_shared_info_ptr_type ptr, single_container_type& singles)
//{
//	inner_process_id_gen_type gen;
//	inner_process_id_single_type::init_ins(gen());
//	if(!ptr || !singles.init_ins())
//	{
//		return;
//	}
//
//	cal_sig_type::init_ins();
//	ctrl_center_single_type::get_ins()->register_dispatchers(*cal_sig_type::get_ins());
//
//	// lua load
//	script_mgr_single_type::init_ins();
//	script_mgr_single_type::get_ins()->append_module(script_mgr_type::E_lib_all);
//	script_mgr_single_type::get_ins()->insert("cal_num", "cal", script_mgr_type::file_code_type("cal_num_test.lua"), 0);
//
//	svr_ptr_single_type::get_ins()->register_network_protocol<test_pak_type>();
//	svr_ptr_single_type::get_ins()->register_network_protocol<cdt_procotol_type>();
//	svr_ptr_single_type::get_ins()->register_network_protocol<rst_procotol_type>();
//	ptr->register_calculator(*cal_sig_type::get_ins());
//}
//
//YGGR_DLL_API void YGGR_MODULE_CALL_T() unreg_cal_enter(runner_shared_info_ptr_type ptr)
//{
//	if(!ptr)
//	{
//		return;
//	}
//
//	svr_ptr_single_type::get_ins()->unregister_network_protocol<test_pak_type>();
//	svr_ptr_single_type::get_ins()->unregister_network_protocol<cdt_procotol_type>();
//	svr_ptr_single_type::get_ins()->unregister_network_protocol<rst_procotol_type>();
//
//	ptr->unregister_calculator(*cal_sig_type::get_ins());
//	ctrl_center_single_type::get_ins()->unregister_dispatchers(*cal_sig_type::get_ins());
//
//	cal_sig_type::get_ins()->join();
//	cal_sig_type::uninstall();
//
//	inner_process_id_single_type::uninstall();
//	single_container_type::uninstall();
//
//	// lua uninstall
//	script_mgr_single_type::uninstall();
//}

#include "lua_calculator_detail.hpp"

#include <yggr/compile_link/linker.hpp>

#if defined(_MSC_VER)
#   include <vld.h>
#endif // defined(_MSC_VER) 

#include YGGR_PP_LINK_LIB(base)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(script_lua)

#include <yggr/dll_make/dll_main.ipp>
