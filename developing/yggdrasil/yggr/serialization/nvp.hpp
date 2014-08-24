//archive_nvp.hpp

/****************************************************************************
Copyright (c) 2014-2018 yggdrasil

author: yang xu

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef __YGGR_SERIALIZATION_NVP_HPP__
#define __YGGR_SERIALIZATION_NVP_HPP__

#include <yggr/move/config.hpp>
#include <boost/serialization/nvp.hpp>

namespace yggr
{
namespace serialization
{

using boost::serialization::nvp;

} // namespace serialization
} // namespace yggr

#define YGGR_SERIALIZE_NVP BOOST_SERIALIZATION_NVP
#define YGGR_SERIALIZE_BASE_OBJECT_NVP BOOST_SERIALIZATION_BASE_OBJECT_NVP
#define YGGR_SERIALIZE_NAME_NVP(__name__, __val__) boost::serialization::make_nvp(__name__, __val__)

#define YGGR_SERIALIZE_NVP_AR(__ar__, __val__) \
    __ar__ & YGGR_SERIALIZE_NVP(__val__)

#define YGGR_SERIALIZE_NAME_NVP_AR(__ar__, __name__, __val__) \
    __ar__ & YGGR_SERIALIZE_NAME_NVP(__name__, __val__)

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES

#   define YGGR_SERIALIZE_WRAP_NVP( __wrap__, __val__ ) \
        boost::serialization::make_nvp(BOOST_PP_STRINGIZE( __val__ ), __wrap__( __val__ ))

#   define YGGR_SERIALIZE_NAME_WRAP_NVP(__name__, __wrap__, __val__ ) \
        boost::serialization::make_nvp(__name__, __wrap__(__val__))

#endif // BOOST_SERIALIZATION_NVP

#   define YGGR_SERIALIZE_WRAP_NVP_AR(__ar__, __wrap__, __val__ ) \
       { __wrap__ wrap_obj_tmp(__val__); __ar__ & boost::serialization::make_nvp(BOOST_PP_STRINGIZE( __val__ ), wrap_obj_tmp); }

#   define YGGR_SERIALIZE_NAME_WRAP_NVP_AR(__name__, __wrap__, __val__ ) \
       { __wrap__ wrap_obj_tmp(__val__); __ar__ & boost::serialization::make_nvp(__name__, wrap_obj_tmp); }

#endif //__YGGR_SERIALIZATION_NVP_HPP__
