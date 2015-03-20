//win_process_foo_test.cpp

#ifndef _MSC_VER
#	error "this file test at msvc only !!!"
#endif //_MSC_VER

#include <yggr/base/yggrdef.h>
#include <iostream>
#include <yggr/process/this_process.hpp>
#include <windows.h>


#include <boost/bind.hpp>
#include <yggr/file_system/local_file_operator.hpp>

#define YGGR_SEH_INCLUDE
#include <yggr/seh/win_seh_helper.hpp>

#ifdef _MSC_VER
#include <vld.h>
#endif // _MSC_VER
int filter(unsigned int code, struct _EXCEPTION_POINTERS *ep)
{
   if(code == EXCEPTION_ACCESS_VIOLATION)
   {
		yggr::seh::win_seh_helper::dump_call_stack_type call_stack;
		yggr::seh::win_seh_helper::dump_call_stack(ep, call_stack);
		//std::cout << yggr::seh::win_seh_helper::format_dump_call_stack_msg(call_stack) << std::endl;
		try
		{
			yggr::file_system::local_file_operator_type::write_file_of_binary("dump.txt", yggr::seh::win_seh_helper::format_dump_call_stack_msg(call_stack));
		}
		catch(const boost::filesystem::filesystem_error& e)
		{
			std::cerr << e.what() << std::endl;
		}
		catch(const compatibility::stl_exception& e)
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

int main(int argc, char* argv[])
{
	safe_call(boost::bind(&test));

	char cc = 0;
	std::cin >> cc;
	return 0;
}
