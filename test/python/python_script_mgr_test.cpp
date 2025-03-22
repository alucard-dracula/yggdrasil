//python_script_test.cpp

#include <iostream>
#include <boost/python.hpp>

#include <boost/detail/lightweight_test.hpp>
#include <string>
#include <cassert>

#include <yggr/script/python_script.hpp>
#include <yggr/script/python_environment.hpp>
#include <yggr/script/script_mgr.hpp>

#if defined(__MINGW32__) && (PY_VERSION_HEX >= 0x03000000)
#   error "mingw not support this file!!"
#endif // __MINGW32__

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

	bool bright = false;
	bright = mgr.insert("test", "enter", script_mgr_type::string_code_type(code));
	assert(bright);

	s_t st(100);

	s_t rst;
	bright = mgr.execute_rule<s_t>(rst, "test", st);
	assert(bright);
	std::cout << rst.i << std::endl;

	mgr.clear();

}

int main(int argc, char *argv[])
{
	Py_Initialize();

	exec_test();

	Py_Finalize();

	std::cout << "all_test_end" << std::endl;
	char cc = 0;
	std::cin >> cc;
	return boost::report_errors();
}
