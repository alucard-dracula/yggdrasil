//python_script_test.cpp

#if defined(WIN32) && defined(_DEBUG)
#include <vld.h>
#endif // _WIN32

#include <iostream>
#include <boost/python.hpp>

#include <boost/detail/lightweight_test.hpp>
#include <string>

#include <yggr/script/python_script.hpp>
#include <yggr/script/python_environment.hpp>
#include <yggr/script/script_mgr.hpp>

#if defined(__MINGW32__) && (PY_VERSION_HEX >= 0x03000000)
#   error "mingw not support this file!!"
#endif // __MINGW32__

//----------------------------------------------------

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


typedef yggr::script::script_mgr<std::string, yggr::script::python::python_script,
									yggr::script::python::python_environment> script_mgr_type;


void exec_test()
{
	script_mgr_type mgr;
	mgr.append_module(PYTHON_MODULE_NAME(pyst), PYTHON_MODULE_INIT_FOO(pyst));

	std::string code(	"from pyst import *		\n"
						"def enter(input):			\n"
						"	output = s_t(1+1)		\n"
						"	return output			\n");

	//std::string code(	"from pyst import *		\n"
	//					"def enter(input):			\n"
	//					"	output = 1+1			\n"
	//					"	return output			\n");

	mgr.insert("test", "enter", script_mgr_type::string_code_type(code));

	s_t st(100);

	s_t rst;
	mgr.execute_rule("test", st, rst);
	std::cout << rst.i << std::endl;

}

int main(int argc, char *argv[])
{
	//Py_Initialize();

	bool error_expected = false;

	if(error_expected = boost::python::handle_exception(exec_test))
	{
		try
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
		catch(...)
		{
			std::cout << "error !!!!" << std::endl;
		}
	}

	//Py_Finalize();
	char cc = 0;
	std::cin >> cc;
	return boost::report_errors();
}