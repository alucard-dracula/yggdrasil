//index_generator.hpp

/****************************************************************************
Copyright (c) 2010-2024 yggdrasil

author: xu yang

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

#ifndef __YGGR_CONTAINER_INDEX_GENERATOR_HPP__
#define __YGGR_CONTAINER_INDEX_GENERATOR_HPP__

#include <yggr/base/yggrdef.h>

#include <boost/multi_index/indexed_by.hpp>

#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/global_fun.hpp>
#include <boost/multi_index/composite_key.hpp>

namespace yggr
{
namespace container
{

using boost::multi_index::indexed_by;

using boost::multi_index::identity;

using boost::multi_index::member;
using boost::multi_index::member_offset;

using boost::multi_index::mem_fun;
using boost::multi_index::const_mem_fun;
using boost::multi_index::mem_fun_explicit;
using boost::multi_index::const_mem_fun_explicit;

using boost::multi_index::global_fun;

using boost::multi_index::composite_key;
using boost::multi_index::composite_key_result;
using boost::multi_index::composite_key_equal_to;
using boost::multi_index::composite_key_result_equal_to;
using boost::multi_index::composite_key_compare;
using boost::multi_index::composite_key_result_less;
using boost::multi_index::composite_key_result_greater;
using boost::multi_index::composite_key_hash;
using boost::multi_index::composite_key_result_hash;


} // namespace container
} // namespace yggr


#define YGGR_INDEX_GENERATOR_MEMBER BOOST_MULTI_INDEX_MEMBER

#define YGGR_INDEX_GENERATOR_MEM_FUN BOOST_MULTI_INDEX_MEM_FUN
#define YGGR_INDEX_GENERATOR_CONST_MEM_FUN BOOST_MULTI_INDEX_CONST_MEM_FUN

#endif // __YGGR_CONTAINER_INDEX_GENERATOR_HPP__