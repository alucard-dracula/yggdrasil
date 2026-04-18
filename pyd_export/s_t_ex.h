//s_t_ex.h
#ifndef __S_T_EX_H__
#define __S_T_EX_H__

#include <yggr/base/yggrdef.h>

#if (defined(YGGR_MSVC_USING_MTD_FLAG) && YGGR_MSVC_USING_MTD_FLAG) \
		|| (defined(YGGR_MSVC_USING_MT_FLAG) && YGGR_MSVC_USING_MT_FLAG)

#include <boost/preprocessor/cat.hpp>

#if defined BOOST_PYTHON_MODULE
#	undef BOOST_PYTHON_MODULE
#endif // BOOST_PYTHON_MODULE
#define BOOST_PYTHON_MODULE(name) \
	void* BOOST_PP_CAT(init,name)(void)   

BOOST_PYTHON_MODULE(pyst)
{
	assert(false);
	return 0;
}

#else

#include <boost/python.hpp>
#include "s_t.h"

#include <iostream>

// !!!termux error test don't deleted!!!
// 
//#if defined(YGGR_AT_ANDROID)
//
//struct s_t_to_python 
//{
//    static PyObject* convert(const s_t& st)
//    {
//    return boost::python::incref(boost::python::object(st.i).ptr());
//    }
//};
//
//// Conversion from python int to s_t
//struct s_t_from_python 
//{
//  s_t_from_python()
//  {
//    boost::python::converter::registry::push_back(
//        &convertible,
//        &construct,
//        boost::python::type_id< s_t >());
//  }
//
//  static void* convertible(PyObject* obj_ptr)
//  {
//#if PY_VERSION_HEX >= 0x03000000
//    if (!PyLong_Check(obj_ptr)) return 0;
//#else
//    if (!PyInt_Check(obj_ptr)) return 0;
//#endif
//    return obj_ptr;
//  }
//
//  static void construct(
//      PyObject* obj_ptr,
//      boost::python::converter::rvalue_from_python_stage1_data* data)
//  {
//    void* storage = (
//        (boost::python::converter::rvalue_from_python_storage< s_t >*)
//        data)-> storage.bytes;
//
//#if PY_VERSION_HEX >= 0x03000000
//    new (storage) s_t((int)PyLong_AsLong(obj_ptr));
//#else
//    new (storage) s_t((int)PyInt_AsLong(obj_ptr));
//#endif
//    data->convertible = storage;
//  }
//};
//
//#endif // YGGR_AT_ANDROID

BOOST_PYTHON_MODULE(pyst)
{
// !!!termux error test don't deleted!!!

// #if defined(YGGR_AT_ANDROID)
//    boost::python::to_python_converter<s_t, s_t_to_python>();
//    s_t_from_python();
//#endif // #if defined(YGGR_AT_ANDROID)

    std::cout << "pyst init" << std::endl;
    boost::python::class_<s_t>("s_t")
        .def(boost::python::init<>())
        .def(boost::python::init<int>())
        .def(boost::python::init<const s_t&>())
        .def_readwrite("i", &s_t::i);
}

#endif // YGGR_MSVC_USING_MTD_FLAG
#endif //__S_T_EX_H__
