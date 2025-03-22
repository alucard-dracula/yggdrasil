//timer_mgr_test.cpp

#include <yggr/base/yggrdef.h>
#include <test/wait_any_key/wait_any_key.hpp>

#if defined(_MSC_VER) || defined(YGGR_AT_LINUX)

#define YGGR_USE_SEH

#if defined(_MSC_VER) && defined(_DEBUG)
#define YGGR_INCLUDE_SEH_DEBUG
#endif // _MSC_VER


#include <yggr/time/time.hpp>
#include <yggr/time/timer.hpp>
#include <yggr/time/timer_mgr.hpp>
#include <yggr/ids/base_ids_def.hpp>
#include <yggr/ids/id_generator.hpp>

#include <yggr/thread/boost_thread_config.hpp>
#include <yggr/thread/this_thread.hpp>

#include <yggr/seh/seh.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(seh)
#include YGGR_PP_LINK_LIB(base)
#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(ids)

struct task_pak
{
	typedef yggr::time::time time_type;
	typedef yggr::time::timer<time_type> timer_type;

	task_pak(const time_type& step)
		: _step(step)
	{
	}

	~task_pak(void)
	{
	}

	yggr::u32 operator()(void)
	{
		if(_step < _timer.elapsed())
		{
			std::cout << "runit" << std::endl;
			int* p = 0;
			*p = 100;
			_timer.restart();
			return yggr::time::timer_task_state::E_timer_task_ran;
		}

		return yggr::time::timer_task_state::E_timer_task_not_ran;
	}

private:
	time_type _step;
	timer_type _timer;
};

int main(int argc, char* argv[])
{
	typedef yggr::thread::this_thread<boost::thread> this_thread_type;
	typedef yggr::thread::boost_thread_config_type thread_config_type;

	typedef yggr::ids::id64_type id_type;
	typedef yggr::time::timer_mgr<id_type> timer_mgr_type;

	yggr::seh::seh_type::init_ins();

#if defined(YGGR_AT_WINDOWS)
	yggr::seh::seh_type::s_register_code(yggr::seh::seh_type::seh_sys_codes_type::E_thread_access_violation);
#elif defined(YGGR_AT_LINUX)
	yggr::seh::seh_type::s_register_code(yggr::seh::seh_type::seh_sys_codes_type::E_SIGSEGV);
#else
#	error "please add this plant code.!!!"
#endif // defined(WIN32) || defined(WIN64) || defined(WINDOWS)

	timer_mgr_type tm_mgr;
	tm_mgr.start();

	id_type id = tm_mgr.add_task<task_pak>(yggr::time::time(3, 0));

	if(!detail::wait_any_key_detail(argc, argv))
	{
		this_thread_type::sleep(thread_config_type::time_type(10, 0));
	}

	tm_mgr.exit_task(id);

	std::cout << "erased" << std::endl;
	std::cout << "wait some time check timer task is running" << std::endl;

	if(!detail::wait_any_key_detail(argc, argv))
	{
		this_thread_type::sleep(thread_config_type::time_type(10, 0));
	}

	tm_mgr.stop();

	yggr::seh::seh_type::uninstall();
	std::cout << "stopped" << std::endl;

	wait_any_key(argc, argv);
	return 0;
}

#else

int main(int argc, char* argv[])
{
	std::cout << "now plantfrom not support seh" << std::endl;

	wait_any_key(argc, argv);
	return 0;
}

#endif // #if defined(_MSC_VER) || defined(YGGR_AT_LINUX)
