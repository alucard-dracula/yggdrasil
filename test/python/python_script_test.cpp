//python_script_test.cpp

#include <yggr/base/yggrdef.h>
#include <test/wait_any_key/wait_any_key.hpp>

#if !((defined(YGGR_MSVC_USING_MTD_FLAG) && YGGR_MSVC_USING_MTD_FLAG) \
		|| (defined(YGGR_MSVC_USING_MT_FLAG) && YGGR_MSVC_USING_MT_FLAG))

#include <yggr/script/python_script.hpp>
#include <yggr/charset/string.hpp>

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


class s_t
{
public:
	s_t(void)
		: i(100)
	{
	}

	s_t(int ni)
		: i(ni)
	{
	}

	s_t(const s_t& st)
		: i(st.i)
	{
	}

	~s_t(void)
	{
	}

	const s_t& operator=(const s_t& st)
	{
		i = st.i;
		return *this;
	}

	int i;
};

BOOST_PYTHON_MODULE(pyst)
{
	boost::python::class_<s_t>("s_t")
		.def(boost::python::init<>())
		.def(boost::python::init<int>())
		.def(boost::python::init<const s_t&>())
		.def_readwrite("i", &s_t::i);
}

void append_module(void)
{
	if(PyImport_AppendInittab("pyst", &PyInit_pyst) == -1)
		 throw std::runtime_error("model init err");
}

struct failed_value_type {};

void exec_test(void)
{

	boost::python::dict grobal(boost::python::import("__main__").attr("__dict__"));

	std::string code(	"from pyst import *\n"
						"def enter(input):\n"
						"\toutput = s_t(1+1)\n"
						"\treturn output\n");

	s_t st(100);
	yggr::script::python::python_script py_sp("enter", yggr::script::python::python_script::string_code_type(code), grobal);
	s_t rst;
	rst = py_sp.execute_rule<s_t>(st);
	std::cout << rst.i << std::endl;
	assert(rst.i == 2);

	struct failed_value_type {}; // Here GCC does not support type declarations inside functions; this declaration will cause SFINAE parsing errors.

	failed_value_type val;
	try
	{
		rst = py_sp.execute_rule<s_t>(val);
	}
	catch(const yggr::script::python::python_script::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}
	catch(...)
	{
		std::cout << "seh error" << std::endl;
	}

	try
	{
		val = py_sp.execute_rule<failed_value_type>(st);
	}
	catch(const yggr::script::python::python_script::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}
	catch(...)
	{
		std::cout << "seh error" << std::endl;
	}
}


int main(int argc, char *argv[])
{
	// if Py_Initialize fails - unable to load the file system codec occured
	// 1 use the following code
	//{
	//	wchar_t pySearchPath[] = L"D:/Python32";
	//	Py_SetPythonHome(pySearchPath);
	//}

	//2 set system environment var like
	//	PYTHONHOME = d:\Python32
	//	PYTHONPATH = d:\Python32\DLLs;d:\Python32\Lib;d:\Python32\Lib\site-packages

	try
	{
		append_module(); // PyImport_AppendInittab needs to be called before Py_IsInitialized
	}
	catch(const std::runtime_error& e)
	{
		std::cout << e.what() << std::endl;
	}

	if(!Py_IsInitialized())
	{
		Py_Initialize();// call only once
	}

	exec_test();

	if(Py_IsInitialized())
	{
		Py_Finalize(); // call only once
	}

	wait_any_key(argc, argv);
	return boost::report_errors();
}

#else

int main(int argc, char **argv)
{
	std::cout << "!!!!script python not support msvc /mtd or /mt!!!!" << std::endl;
	wait_any_key(argc, argv);
	return 0;
}

#endif // YGGR_MSVC_USING_MTD_FLAG
