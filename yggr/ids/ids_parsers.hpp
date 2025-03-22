//ids_parsers.hpp

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

#ifndef __YGGR_IDS_IDS_PARSERS_HPP__
#define __YGGR_IDS_IDS_PARSERS_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/static_assert.hpp>

#include <boost/multi_index/key_extractors.hpp>
#include <typeinfo>

namespace yggr
{
namespace ids
{
namespace ids_parsers
{

using boost::multi_index::identity;
using boost::multi_index::member;
using boost::multi_index::member_offset;
using boost::multi_index::const_mem_fun;
using boost::multi_index::mem_fun;
using boost::multi_index::const_mem_fun_explicit;
using boost::multi_index::mem_fun_explicit;
using boost::multi_index::global_fun;
using boost::multi_index::composite_key;
using boost::multi_index::composite_key_result;


template<typename Ret>
struct use_typeid_name
{
	typedef Ret result_type;

	template<typename Obj> inline
	result_type operator()(const Obj& obj) const
	{
		return result_type(typeid(Obj).name());
	}
};

template<>
struct use_typeid_name<const char*>
{
	typedef const char* result_type;

	template<typename Obj> inline
	result_type operator()(const Obj& obj) const
	{
		return typeid(Obj).name();
	}
};

template<typename Ret, typename Object,
			const std::type_info& (Object::*info_get_foo)(void) const>
struct use_object_typeid_name
{
	typedef Ret result_type;
	typedef Object obj_type;

	inline result_type operator()(const obj_type& obj) const
	{
		return result_type(((obj.*info_get_foo)()).name());
	}
};

template<typename Object,
			const std::type_info& (Object::*info_get_foo)(void) const>
struct use_object_typeid_name<const char*, Object, info_get_foo>
{
	typedef const char* result_type;
	typedef Object obj_type;

	inline result_type operator()(const obj_type& obj) const
	{
		return ((obj.*info_get_foo)()).name();
	}
};

struct use_typeid
{
	typedef const std::type_info& result_type;

	template<typename Obj> inline
	result_type operator()(const Obj& obj) const
	{
		return typeid(Obj);
	}
};

template<typename Parser>
struct reg_type_parser
{
	typedef Parser parser_type;
	typedef typename parser_type::result_type result_type;

	template<typename T> inline
	result_type operator()(void) const
	{
		parser_type foo;
#if defined(_MSC_VER) && (YGGR_CPP_VERSION < YGGR_CPP_VER_20)
		return foo.operator()<T>();
#else
		return foo.template operator()<T>();
#endif //#if defined(_MSC_VER) && (YGGR_CPP_VERSION < YGGR_CPP_VER_20)
	}
};

} // ids_parsers
} // namespace ids
} // namespace yggr

#define YGGR_IDS_PARSER_MEMBER BOOST_MULTI_INDEX_MEMBER
#define YGGR_IDS_PARSER_CONST_MEM_FOO BOOST_MULTI_INDEX_CONST_MEM_FUN
#define YGGR_IDS_PARSER_MEM_FOO BOOST_MULTI_INDEX_MEM_FUN
#define YGGR_IDS_PARSER_GLOBAL_FOO( __value__, __type__, __foo__ ) \
	boost::multi_index::global_fun< __value__, __type__, __foo__ >

#define YGGR_IDS_PARSER_USE_TYPEID_NAME_CONV_RET(__ret__) \
	yggr::ids::ids_parsers::use_typeid_name< __ret__ >

#define YGGR_IDS_PARSER_USE_TYPEID_NAME() \
	yggr::ids::ids_parsers::use_typeid_name< const char* >

#define YGGR_IDS_PARSER_USE_OBJECT_TYPEID_NAME_CONV_RET(__ret__, __obj__, __info_foo_name__) \
	yggr::ids::ids_parsers::use_object_typeid_name< \
		__ret__, __obj__, &__obj__::__info_foo_name__ >

#define YGGR_IDS_PARSER_USE_OBJECT_TYPEID_NAME(__obj__, __info_foo_name__) \
	yggr::ids::ids_parsers::use_object_typeid_name< \
		const char*, __obj__, &__obj__::__info_foo_name__ >

#endif // __YGGR_IDS_IDS_PARSERS_HPP__
