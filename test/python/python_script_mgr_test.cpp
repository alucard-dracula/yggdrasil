//python_script_test.cpp

#include <yggr/base/yggrdef.h>
#include <test/wait_any_key/wait_any_key.hpp>

#if !((defined(YGGR_MSVC_USING_MTD_FLAG) && YGGR_MSVC_USING_MTD_FLAG) \
		|| (defined(YGGR_MSVC_USING_MT_FLAG) && YGGR_MSVC_USING_MT_FLAG))

#include <yggr/script/python_script.hpp>
#include <yggr/script/python_environment.hpp>
#include <yggr/script/script_mgr.hpp>
#include <yggr/charset/string.hpp>

#include <boost/python.hpp>

#include <boost/detail/lightweight_test.hpp>

#include <iostream>
#include <cassert>


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

script_mgr_type mgr;

void append_module(void)
{
	mgr.append_module(PYTHON_MODULE_NAME(pyst), PYTHON_MODULE_INIT_FOO(pyst));
}

void exec_test()
{
	std::string code(	"from pyst import *		\n"
						"def enter(input):			\n"
						"	output = s_t(1+1)		\n"
						"	return output			\n");

	bool bright = false;
	bright = mgr.insert("test", "enter", script_mgr_type::string_code_type(code));
	assert(bright);

	s_t st(100);

	s_t rst;
	bright = mgr.execute_rule<s_t>(rst, yggr::const_args_anchor("test"), st);
	assert(bright);
	std::cout << rst.i << std::endl;

	mgr.clear();

}

void exec_test2()
{
	script_mgr_type loacl_mgr;

	std::string code(	"from pyst import *		\n"
						"def enter(input):			\n"
						"	output = s_t(1+1)		\n"
						"	return output			\n");

	bool bright = false;
	bright = loacl_mgr.insert("test", "enter", script_mgr_type::string_code_type(code));
	assert(bright);

	s_t st(100);

	s_t rst;
	bright = loacl_mgr.execute_rule<s_t>(rst, yggr::const_args_anchor("test"), st);
	assert(bright);
	std::cout << rst.i << std::endl;

	loacl_mgr.clear();

}

int main(int argc, char *argv[])
{
	try
	{
		append_module();
	}
	catch(const std::runtime_error& e)
	{
		std::cout << e.what() << std::endl;
	}
	catch(...)
	{
		std::cout << "other exception" << std::endl;
	}

	bool chk = mgr.py_initialize();
	assert(chk);

	exec_test();
	exec_test2();

	chk = mgr.py_finalize();
	assert(chk);

	std::cout << "all_test_end" << std::endl;
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
