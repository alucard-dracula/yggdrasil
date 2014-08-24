//python_script_test.cpp


//#pragma comment(lib, "vld.lib")

#include <iostream>
#include <boost/python.hpp>

#include <boost/detail/lightweight_test.hpp>
#include <string>

#include <yggr/script/python_script.hpp>

//#ifdef _WIN32
//#   include <vld.h>
//#endif //_WIN32
//----------------------------------------------------
#if defined(__MINGW32__) && (PY_VERSION_HEX >= 0x03000000)
#   error "mingw not support this file!!"
#endif // __MINGW32__


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
	yggr::script::python::python_script py_sp("enter", yggr::script::python::python_script::string_code(code), grobal);
	//yggr::script::python::python_script py_sp2(py_sp); //this_copy_is_enable
	s_t rst;
	rst = py_sp.execute_rule<s_t>(st);
	//s_t rst(py_sp2.execute_rule<s_t>(st));
	std::cout << rst.i << std::endl;

}

int main(int argc, char *argv[])
{
	wchar_t pySearchPath[] = L"Python32";
	Py_SetPythonHome(pySearchPath);
	Py_Initialize();

	bool error_expected = false;

	if(boost::python::handle_exception(exec_test))
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

	//Py_Finalize();
	char cc = 0;
	std::cin >> cc;
	return boost::report_errors();
}
