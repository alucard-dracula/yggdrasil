//win_process_foo_test.cpp

#include <yggr/base/yggrdef.h>
#include <test/wait_any_key/wait_any_key.hpp>

#if defined(_MSC_VER)

#if !defined(YGGR_USE_SEH)
#define YGGR_USE_SEH
#endif //YGGR_USE_SEH

#if defined(_DEBUG) && !defined(YGGR_INCLUDE_SEH_DEBUG)
#define YGGR_INCLUDE_SEH_DEBUG
#endif // #if defined(_MSC_VER) && defined(_DEBUG)  && !defined(YGGR_INCLUDE_SEH_DEBUG)

#include <yggr/process/this_process.hpp>
#include <windows.h>


#include <boost/bind.hpp>
#include <yggr/file_system/local_fsys_operators.hpp>

#define YGGR_SEH_INCLUDE
#include <yggr/seh/seh_helper_win.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#include YGGR_PP_LINK_LIB(seh)
#include YGGR_PP_LINK_LIB(charset)

int filter(unsigned int code, struct _EXCEPTION_POINTERS *ep)
{
   if(code == EXCEPTION_ACCESS_VIOLATION)
   {
		yggr::seh::seh_helper_win::dump_call_stack_type call_stack;
		yggr::seh::seh_helper_win::dump_call_stack(ep, call_stack);

		std::string out_msg = yggr::seh::seh_helper_win::format_dump_call_stack_msg(call_stack);
		std::cout << out_msg << std::endl;
		try
		{
			yggr::file_system::local_fsys_operators::write_file_of_binary(
				yggr::file_system::local_fsys_operators::make_path_object("../test_out/seh/dump.txt"),
				out_msg,
				yggr::file_system::local_fsys_operators::options_type::dir_create_if_not_exists);
		}
		catch(const boost::filesystem::filesystem_error& e)
		{
			std::cerr << e.what() << std::endl;
		}
		catch(const ::yggr::stl_exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
		return EXCEPTION_EXECUTE_HANDLER;
   }
   else
   {
       return EXCEPTION_CONTINUE_SEARCH;
   };
}

void test2(void)
{
	int *p = 0;
	*p = 0;
}

void test1(void)
{
	test2();
}

void test(void)
{
	test1();
}

template<typename Handler1>
bool safe_call(const Handler1& foo_handler )
{
	__try
	{
		__try
		{
			foo_handler();
			return true;
		}
		__finally
		{
		}
	}
	__except(filter(GetExceptionCode(), GetExceptionInformation()))
	{
	}
	return false;
}

int g_argc = 0;

int main(int argc, char* argv[])
{
	g_argc = argc;

	safe_call(boost::bind(&test));

	wait_any_key(g_argc, argv);
	return 0;
}

#else

int main(int argc, char* argv[])
{
	std::cout << "this test using msvc only" << std::endl;
	wait_any_key(argc, argv);
	return 0;
}

#endif // _MSC_VER
