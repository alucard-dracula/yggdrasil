//s_t_ex.h
#ifndef __S_T_EX_H__
#define __S_T_EX_H__

#include <boost/python.hpp>
#include "s_t.h"

BOOST_PYTHON_MODULE(pyst)
{
    boost::python::class_<s_t>("s_t")
    .def(boost::python::init<>())
    .def(boost::python::init<int>())
    .def(boost::python::init<const s_t&>())
    .def_readwrite("i", &s_t::i);
}
#endif //__S_T_EX_H__
