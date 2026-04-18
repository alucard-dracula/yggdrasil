//python_script_test.cpp

#include <yggr/base/yggrdef.h>
#include <test/wait_any_key/wait_any_key.hpp>

#if !((defined(YGGR_MSVC_USING_MTD_FLAG) && YGGR_MSVC_USING_MTD_FLAG) \
		|| (defined(YGGR_MSVC_USING_MT_FLAG) && YGGR_MSVC_USING_MT_FLAG) \
		|| defined(YGGR_AT_ANDROID))

#include <yggr/script/python_script.hpp>
#include <yggr/script/python_environment.hpp>
#include <yggr/script/script_mgr.hpp>

#include <pyd_export/s_t_ex.h>

#include <yggr/charset/string.hpp>
#include <yggr/compatibility/stl_exception.hpp>

#include <yggr/ppex/cast.hpp>
#include <yggr/ppex/cast_to_string.hpp>

#include <boost/python.hpp>
#include <boost/detail/lightweight_test.hpp>

#include <iostream>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(script_python)
#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

typedef yggr::script::script_mgr<std::string, yggr::script::python::python_script, yggr::script::python::python_environment> script_mgr_type;

void exec_test(script_mgr_type& mgr)
{
	script_mgr_type mgr2;

#if defined(_MSC_VER)
	std::string test_code_cl_mark = "vc" YGGR_PP_CAST(YGGR_MSVC_NUMBER(),YGGR_PP_CAST_TO_STRING);
	std::string test_fname_cl_mark = "-vc" YGGR_PP_CAST(YGGR_MSVC_NUMBER(),YGGR_PP_CAST_TO_STRING);
#elif defined(__MINGW32__)
	std::string test_code_cl_mark = "mgw15";
	std::string test_fname_cl_mark = "-mgw15";
#elif defined(__linux__)
#	if defined(__ANDROID__) || defined(ANDROID)
		std::string test_code_cl_mark = "clang";
		std::string test_fname_cl_mark = "-clang";
#	else
		std::string test_code_cl_mark = "gcc15";
		std::string test_fname_cl_mark = "-gcc15";
#	endif // __ANDROID__
#elif defined(__APPLE__) || defined(__APPLE_CC__)
	std::string test_code_cl_mark = "clang-darwin";
	std::string test_fname_cl_mark = "-clang-darwin";
#else
	std::string test_code_cl_mark = "";
	std::string test_fname_cl_mark = "";
#endif // _MSC_VER

#if defined(_DEBUG)
	std::string test_code_debug_mark = "Debug";
	std::string test_fname_debug_mark = "-debug";
#else
	std::string test_code_debug_mark = "Release";
	std::string test_fname_debug_mark = "-release";
#endif // _DEBUG

	std::string code = "./python/test" + test_fname_cl_mark + test_fname_debug_mark + ".py";


	std::string test_code_var_exe_mark = "var_exe_mark = \"" + test_code_debug_mark + "\"\n";
	std::string test_code_var_cl_mark = "var_cl_mark = \"" + test_code_cl_mark + "\"\n";

	std::string code2(	"import os\n"
						"import sys\n"
						"import platform\n"
						"\n"
						"current_path = os.getcwd()\n"
						"current_dir_name = os.path.basename(current_path)\n"
						"\n"
						"var_arch = platform.architecture()[0]\n"
						"var_prefix = sys.prefix\n"
						"\n"
						"var_parent_path = os.path.dirname(current_path)\n"
						"var_parent_dir_name = os.path.basename(var_parent_path)\n"
						"\n"
						+ test_code_var_exe_mark
						+ test_code_var_cl_mark
						+
						"\n"
						"if var_arch == \"64bit\":\n"
						"	var_arch_mark = \"-x64\"\n"
						"	var_arch_num = \"64\"\n"
						"else:\n"
						"	var_arch_mark = \"-x32\"\n"
						"	var_arch_num = \"\"\n"
						"\n"
						"print(os.path.basename(os.path.dirname(current_path)))\n"
						"\n"
						"if current_dir_name == 'python' and var_parent_dir_name == 'test':\n"
						"	var_pyd_prefix = \"/../../\" + var_exe_mark + var_arch_mark + \"/\" + var_cl_mark\n"
						"elif current_dir_name == 'test' and var_parent_dir_name == 'yggdrasil' :\n"
						"	var_pyd_prefix = \"/../\" + var_exe_mark + var_arch_mark + \"/\" + var_cl_mark\n"
						"else:\n"
						"	var_pyd_prefix = \"\"\n"
						"\n"
						"pyd_path = current_path + var_pyd_prefix\n"
						"\n"
						"if platform.system() == \"Windows\":\n"
						"	dll_path = \"D:/third_part/boost/bin\" + var_arch_num\n"
						"elif platform.system() == \"linux\" : \n"
						"	dll_path = os.environ['HOME'] + \"/third_part/boost/lib\" + var_arch_num\n"
						"elif platform.system() == \"Android\" :\n" 
						"	dll_path = current_path\n"
						"elif platform.system() == \"darwin\" : \n"
						"	dll_path = os.environ['HOME'] + \"/third_part/boost/lib\"\n"
						"else :\n"
						"	dll_path = current_path\n"
						"\n"
						"print(\"current_path: \" + pyd_path)\n"
						"print(\"pyd_search_path: \" + pyd_path)\n"
						"print(\"dll_search_path: \" + dll_path)\n"
						"\n"
						"if platform.system() == \"Windows\":\n"
						"	os.add_dll_directory(dll_path)\n"
						"\n"
						"sys.path.append(pyd_path)"
						"\n"
						"from pyst import *\n"
						"\n"
						"def enter(input):\n"
						"	output = s_t(1+1)\n"
						"	return output\n");

	try
	{
		mgr.insert("test", "enter", script_mgr_type::file_code_type(code));
		mgr.insert("test2","enter", script_mgr_type::string_code_type(code2));

		mgr2.insert("test", "enter", script_mgr_type::file_code_type(code));
		mgr2.insert("test2","enter", script_mgr_type::string_code_type(code2));
	}
	catch(const yggr::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch(...)
	{
		std::cerr << "seh error" << std::endl;
	}

	s_t st(100);

	s_t rst;
	mgr.execute_rule(rst, yggr::const_args_anchor("test"), st);
	std::cout << rst.i << std::endl;

	 mgr.execute_rule(rst, yggr::const_args_anchor("test2"), st);
	 std::cout << rst.i << std::endl;

	 mgr2.execute_rule(rst, yggr::const_args_anchor("test"), st);
	 std::cout << rst.i << std::endl;

	 mgr2.execute_rule(rst, yggr::const_args_anchor("test2"), st);
	 std::cout << rst.i << std::endl;

}

int main(int argc, char *argv[])
{
	script_mgr_type mgr;

	bool chk = mgr.py_initialize();
	assert(chk);

	mgr.append_module(PYTHON_MODULE_NAME(pyst), PYTHON_MODULE_INIT_FOO(pyst));

	try
	{
		exec_test(mgr);
	}
	catch(const yggr::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	bool error_expected = false;

	if(error_expected = boost::python::handle_exception(boost::bind(&exec_test, boost::ref(mgr))))
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

	wait_any_key(argc, argv);
	
	chk = mgr.py_finalize();
	assert(chk);

	return boost::report_errors();
}

#else

int main(int argc, char **argv)
{
//#if defined(YGGR_AT_ANDROID)
//	std::cout << "!!!!this test not support android termux, because boost::python has a unknow bug!!!!" << std::endl;
//	std::cout << "!!!!using boost::python so if return value: segment failed!!!!" << std::endl;
//	std::cout << "!!!!using boost::python stati lib error already set!!!!" << std::endl;
//#else
	std::cout << "!!!!script python not support msvc /mtd or /mt!!!!" << std::endl;
//#endif // YGGR_AT_ANDROID
	wait_any_key(argc, argv);
	return 0;
}

#endif // YGGR_MSVC_USING_MTD_FLAG