//py_struct_test.cpp

#include <iostream>
#include <boost/python.hpp>

#include <boost/detail/lightweight_test.hpp>
#include <string>

#ifdef _WIN32
#   include <vld.h>
#   pragma comment(lib, "vld.lib")
#endif //_WIN32
//----------------------------------------------------

#if defined(__MINGW32__) && (PY_VERSION_HEX >= 0x03000000)
#   error "mingw not support this file!!"
#endif // __MINGW32__


using namespace boost::python;

class A
{
public:
	int i;
};

struct B
{
	A a;
	B(const A& a_):a(a_){}
};

// Converter from A to python int
struct BToPython
#if defined BOOST_PYTHON_SUPPORTS_PY_SIGNATURES //unnecessary overhead if py signatures are not supported
   : converter::to_python_target_type<A>  //inherits get_pytype
#endif
{
  static PyObject* convert(const B& b)
  {
    return incref(object(b.a).ptr());
  }
};

// Conversion from python int to A
struct BFromPython
{
  BFromPython()
  {
    boost::python::converter::registry::push_back
		( &convertible
        , &construct
        , type_id< B >()
#if defined BOOST_PYTHON_SUPPORTS_PY_SIGNATURES //invalid if py signatures are not supported
        , &converter::expected_from_python_type<A>::get_pytype//convertible to A can be converted to B
#endif
        );
  }

  static void* convertible(PyObject* obj_ptr)
  {
      extract<const A&> ex(obj_ptr);
      if (!ex.check()) return 0;
      return obj_ptr;
  }

  static void construct(
      PyObject* obj_ptr,
      converter::rvalue_from_python_stage1_data* data)
  {
    void* storage = (
        (converter::rvalue_from_python_storage< B >*)data)-> storage.bytes;

    extract<const A&> ex(obj_ptr);
    new (storage) B(ex());
    data->convertible = storage;
  }
};


B func(const B& b) { return b ; }

BOOST_PYTHON_MODULE(pytype_function_ext)
{
  to_python_converter< B , BToPython
#if defined BOOST_PYTHON_SUPPORTS_PY_SIGNATURES //invalid if py signatures are not supported
             ,true
#endif
             >(); //has get_pytype
  BFromPython();

  class_<A>("A") ;

  def("func", &func);
  //def("i", &A::i);

}


void exec_test6()
{

	if (PyImport_AppendInittab("pytype_function_ext", PyInit_pytype_function_ext) == -1)
    throw std::runtime_error("Failed to add embedded_hello to the interpreter's "
                 "builtin modules");
	boost::python::dict local;

	boost::python::dict param;



	boost::python::object result2 = boost::python::exec(	"from pytype_function_ext import *		\n"
															"def out_dict(dict):					\n"
															"	m =  dict['aaa']					\n"
															"	m.i = 2048						    \n"
															"	print(m.i)							\n"
															"	return m						    \n",
															boost::python::object(boost::python::import("__main__").attr("__dict__")), local);
	A a;
	a.i = 1024;

	B b(a);
	param.setdefault("aaa", b);

	boost::python::object foo = local["out_dict"];
	//std::string ostr(PyString_AS_STRING(foo(param).ptr()));
	boost::python::object rst = foo(param);

	A& st = boost::python::extract<A&>(rst.ptr());

	std::cout << st.i<< std::endl;


	//std::cout << ostr << std::endl;

}

//------------------------------------------------------

//boost::python::dict global;
class s_t //: public boost::python::wrapper<s_t>
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
	int i;
};

BOOST_PYTHON_MODULE(pyst)
{
	//boost::python::class_<s_t> s_t("s_t");

	boost::python::class_<s_t>("s_t")
		.def(init<>())
		.def(init<int>())
		.def(init<const s_t&>())
		.def_readwrite("i", &s_t::i);
}


void exec_test5()
{
	if (PyImport_AppendInittab("pyst", PyInit_pyst) == -1)
				throw std::runtime_error("Failed to add embedded_hello to the interpreter's "
                 "builtin modules");
	boost::python::dict local1;
	boost::python::dict local2;
	boost::python::dict local3;

	boost::python::dict grobal( boost::python::object(boost::python::import("__main__").attr("__dict__")));

	local2 = grobal.copy();
	//local3 = grobal;



	boost::python::dict param;

	boost::python::object result1 = boost::python::exec(
													"i = 1",
													grobal, local1);

	//std::cout << (boost::python::extract<int>(grobal["i"])) << std::endl;
	std::cout << (boost::python::extract<int>(local1["i"])) << std::endl;


	//std::cout << (boost::python::extract<int>(grobal["i"])) << std::endl;
	//boost::python::object result3 = boost::python::exec(	"from pyst import *		\n",
	//														local2, local2);
	boost::python::object result2 = boost::python::exec(	"from pyst import *		\n"
															"i = 2					    \n"
															"def out_dict(dict):		\n"
															"	k = s_t(500)			\n"
															"	k.i = 2048				\n"
															"	print(k.i)				\n"
															"	return k				\n",
															local2, local2);
															//boost::python::object(boost::python::import("__main__").attr("__dict__")), local);


	//std::cout << (boost::python::extract<int>(grobal["i"])) << std::endl;
	std::cout << (boost::python::extract<int>(local2["i"])) << std::endl;

	/*boost::python::object result3 = boost::python::exec(
													"i = 3",
													grobal, local3);*/

	//std::cout << (boost::python::extract<int>(grobal["i"])) << std::endl;
	//std::cout << (boost::python::extract<int>(local3["i"])) << std::endl;

	s_t tst;
	tst.i = 1000;
	param["aaa"] = tst;

	boost::python::object foo = local2["out_dict"];
	boost::python::object rst = foo(param);

	s_t& st = boost::python::extract<s_t&>(rst.ptr());
	std::cout << st.i << std::endl;


	//std::cout << (boost::python::extract<int>(local["i"])) << std::endl;
}

void exec_test4()
{

	boost::python::dict local;

	boost::python::object result2 = boost::python::exec(
															"class s_t:					\n"
															"	i = 0					\n"
															"	def __init__(self):		\n"
															"		i = self.i			\n"
															"	def __init__(self, ni):	\n"
															"		self.i = ni			\n"
															"def out_st(obj):			\n"
															"	return obj.i			\n"
															"def out_dict(dict):		\n"
															"	return dict['aaa']		\n",
															boost::python::object(boost::python::import("__main__").attr("__dict__")), local);

	boost::python::object obj_type = local["s_t"];
	boost::python::object obj = obj_type(100);

	std::cout << (boost::python::extract<int>(local["out_st"](obj))) << std::endl;

}
void exec_test3()
{
	//boost::python::object main;
	boost::python::dict local;

	boost::python::object result2 = boost::python::exec(
													"i = 1000",
													boost::python::object(boost::python::import("__main__").attr("__dict__")), local);


	std::cout << boost::python::extract<int>(local["i"]) << std::endl;

}
void exec_test2()
{

	boost::python::dict local;

	boost::python::object result = boost::python::exec(
													"i = 100							\n"
													"def change_list(l):				\n"
													"	isize = len(l)					\n"
													"	for i in range(0, isize):		\n"
													"		l[i] = l[i] +1				\n"
													"def test():						\n"
													"	return 'python'					\n"
													"class abc:							\n"
													"	def test(self, str):			\n"
													"		return str + 'py'			\n"
													"	def test2(self, str1, i):		\n"
													"		return str1 + str(i) + 'py' \n",
													boost::python::object(boost::python::import("__main__").attr("__dict__")), local);


	int k = boost::python::extract<int>(local["i"]);
	std::cout << k << std::endl;



	boost::python::object py_obj_type = local["abc"];
	boost::python::object obj = py_obj_type();


	std::cout << (boost::python::call_method<std::string>(obj.ptr(), "test", "kkk")) << std::endl;
	std::cout << (boost::python::call_method<std::string>(obj.ptr(), "test2", "kkk", 100)) << std::endl;

	boost::python::object foo = local["test"];
	std::cout << (boost::python::call<std::string>(foo.ptr())) << std::endl;


	boost::python::list list;
	list.append(10);
	list.append(20);
	list.append(30);

	boost::python::list list2(list);
	boost::python::object foo2 = local["change_list"];
	boost::python::call<void>(foo2.ptr(), list);

	list.count(0);
	int isize =  PyList_Size(list2.ptr());

	for(int i = 0; i < isize; ++i)
	{
		std::cout << boost::python::extract<int>(PyList_GetItem(list2.ptr(), i)) << std::endl;
	}

	std::cout << "success!" << std::endl;
}


int main(int argc, char *argv[])
{

	Py_Initialize();

	bool error_expected = false;

	//if(boost::python::handle_exception(exec_test3) || boost::python::handle_exception(exec_test2))
	if(boost::python::handle_exception(exec_test5)/* || boost::python::handle_exception(exec_test3)*/)
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

	char cc = 0;
	std::cin >> cc;
	// Boost.Python doesn't support Py_Finalize yet, so don't call it!
	return boost::report_errors();
	//return 0;
}
