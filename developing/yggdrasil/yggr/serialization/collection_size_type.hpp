//yggr serialization collection_size_type.hpp

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

#ifndef __YGGR_SERIALIZATION_COLLECTION_SIZE_TYPE_HPP__
#define __YGGR_SERIALIZATION_COLLECTION_SIZE_TYPE_HPP__

#include <boost/serialization/collection_size_type.hpp>
#include <yggr/base/yggrdef.h>

namespace yggr 
{ 
namespace serialization 
{

class collection_size_type
{
private:
    typedef ser_size_type base_type;
    base_type t;
public:
    collection_size_type(): t(0) {};
    explicit collection_size_type(const std::size_t & t_) : 
        t(t_)
    {}
    collection_size_type(const collection_size_type & t_) : 
        t(t_.t)
    {}
    collection_size_type & operator=(const collection_size_type & rhs){
        t = rhs.t; 
        return *this;
    }
    collection_size_type & operator=(const unsigned int & rhs){
        t = rhs; 
        return *this;
    }
    // used for text output
    operator base_type () const {
        return t;
    }                
    // used for text input
    operator base_type & () {
        return t;
    }                
    bool operator==(const collection_size_type & rhs) const {
        return t == rhs.t;
    } 
    bool operator<(const collection_size_type & rhs) const {
        return t < rhs.t;
    }   
};

} //namespace serialization
} // namespace yggr

BOOST_CLASS_IMPLEMENTATION(yggr::serialization::collection_size_type, primitive_type)
BOOST_IS_BITWISE_SERIALIZABLE(yggr::serialization::collection_size_type)

#endif //__YGGR_SERIALIZATION_COLLECTION_SIZE_TYPE_HPP__

#include <yggr/serialization/collection_size_type_bson_impl.hpp>


