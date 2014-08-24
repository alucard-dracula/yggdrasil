//python_script_test.cpp


#if defined(WIN32) && defined(_DEBUG)
#   include <vld.h>
#endif // _DEBUG

#include <iostream>
#include <boost/python.hpp>

#include <boost/detail/lightweight_test.hpp>
#include <string>

#include <yggr/script/python_script.hpp>
#include <yggr/script/python_environment.hpp>
#include <yggr/script/script_mgr.hpp>

#ifdef LINUX
#   include <../pyd_export/s_t_ex.h>
//#   include <../pyd_export/s_t.h>
#else
#   include <../pyd_export/s_t.h>
#endif // LINUX

#if defined(__MINGW32__) && (PY_VERSION_HEX >= 0x03000000)
#   error "mingw not support this file!!"
#endif // __MINGW32__


typedef yggr::script::script_mgr<std::string, yggr::script::python::python_script, yggr::script::python::python_environment> script_mgr_type;

void exec_test()
{
	script_mgr_type mgr;
	script_mgr_type mgr2;

#ifdef LINUX
	mgr.append_module(PYTHON_MODULE_NAME(pyst), PYTHON_MODULE_INIT_FOO(pyst));
#endif // LINUX

	std::string code("./test.py");

	std::string code2(	"from pyst import *		\n"
						"def enter(input):			\n"
						"	output = s_t(1+2)		\n"
						"	return output			\n");

    try
    {
		mgr.insert("test", "enter", script_mgr_type::file_code_type(code));
		mgr.insert("test2","enter", script_mgr_type::string_code_type(code2));
    }
    catch(const compatibility::stl_exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

	s_t st(100);

	s_t rst;
	mgr.execute_rule("test", st, rst);
	std::cout << rst.i << std::endl;

	mgr.execute_rule("test2", st, rst);
	std::cout << rst.i << std::endl;

}

int main(int argc, char *argv[])
{
	Py_Initialize();

    //try
    //{
    //exec_test();
    //}
//    catch(const compatibility::stl_exception& e)
//    {
//        std::cerr << e.what() << std::endl;
//    }
	bool error_expected = false;

	if(error_expected = boost::python::handle_exception(exec_test))
	{
		if (PyErr_Occurred())
		{
			if (!error_expected)
				BOOST_ERROR("Python Error detected");
			PyErr_Print();
		}
		else
		{
			BOOST_ERROR("A C++ exception was thrown  for which "
						"there was no exception translator registered.");
		}
	}

	//PyImport_Cleanup();
	Py_Finalize();
	char cc = 0;
	std::cin >> cc;
	return boost::report_errors();
}
