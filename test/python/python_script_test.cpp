//python_script_test.cpp

#include <iostream>
#include <boost/python.hpp>

#include <boost/detail/lightweight_test.hpp>
#include <string>

#include <yggr/script/python_script.hpp>

//----------------------------------------------------
//#if defined(__MINGW32__) && (PY_VERSION_HEX >= 0x03000000)
//#   error "mingw not support this file!!"
//#endif // __MINGW32__

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


void exec_test()
{

	if(PyImport_AppendInittab("pyst", PyInit_pyst) == -1)
		 throw std::runtime_error("model init err");


	boost::python::dict grobal(boost::python::import("__main__").attr("__dict__"));

	std::string code(	"from pyst import *		\n"
						"def enter(input):			\n"
						"	output = s_t(1+1)		\n"
						"	return output			\n");

	s_t st(100);
	yggr::script::python::python_script py_sp("enter", yggr::script::python::python_script::string_code_type(code), grobal);
	s_t rst;
	rst = py_sp.execute_rule<s_t>(st);
	std::cout << rst.i << std::endl;
	assert(rst.i == 2);
	struct failed_value_type {};
	failed_value_type val;
	try
	{
		rst = py_sp.execute_rule<s_t>(val);
	}
	catch(const yggr::script::python::python_script::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}
	
	try
	{
		val = py_sp.execute_rule<failed_value_type>(st);
	}
	catch(const yggr::script::python::python_script::error_type& e)
	{
		std::cout << e.what() << std::endl;
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

	if(!Py_IsInitialized())
	{
		Py_Initialize();
	}

	exec_test();


	if(Py_IsInitialized())
	{
		Py_Finalize();
	}
	
	char cc = 0;
	std::cin >> cc;
	return boost::report_errors();
}
