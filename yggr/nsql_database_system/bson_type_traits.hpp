//bson_type_traits.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_BSON_TYPE_TRAITS_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_BSON_TYPE_TRAITS_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/nsql_database_system/bson_config.hpp>
#include <yggr/nsql_database_system/bson_typeid.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/type_traits/remove_cv.hpp>

namespace yggr
{
namespace nsql_database_system
{

template<typename T>
struct is_bson_inside_type
	: public 
		boost::mpl::bool_
		<
			(static_cast<u32>(bson_typeid_def::E_BSON_TYPE_EOD)
				!= bson_typeid< typename boost::remove_cv<T>::type >::value)
		>
{
};

// inside splice don't check E_BSON_TYPE_DOCUMENT this is bson used
template<typename T>
struct is_bson_inside_splice
	: public 
		boost::mpl::bool_
		<
			((static_cast<u32>(bson_typeid_def::E_BSON_TYPE_ARRAY)
				== bson_typeid< typename boost::remove_cv<T>::type >::value)
			||
			(static_cast<u32>(bson_typeid_def::E_BSON_TYPE_SPLICE_OBJECT)
				== bson_typeid< typename boost::remove_cv<T>::type >::value))
		>
{
};

template<typename T>
struct is_bson_splice_t
	: public 
		boost::mpl::bool_
		<
			((static_cast<u32>(bson_typeid_def::E_BSON_TYPE_EOD)
				== bson_typeid< typename boost::remove_cv<T>::type >::value)
			||
			(static_cast<u32>(bson_typeid_def::E_BSON_TYPE_SPLICE_OBJECT)
				== bson_typeid< typename boost::remove_cv<T>::type >::value))
		>
{
};

struct bson_mark_splice
{
	bson_mark_splice(void)
	{
		memset(&bs_, 0, sizeof(bson_t));
	}

	~bson_mark_splice(void)
	{
	}

	mutable bson_t bs_;
};

struct bson_mark_nil {};

template<typename T>
struct bson_value_mark
	: public
			boost::mpl::if_
		<
			typename is_bson_inside_splice<T>::type,
			bson_mark_splice, bson_mark_nil 
		>
{
};

template<typename Mark>
struct bson_mark_begin
{
public:
	typedef Mark mark_type;
public:
	bson_mark_begin(u32 t, mark_type& mark)
		: t_(t), mark_(mark)
	{
	}

	~bson_mark_begin(void)
	{
	}

public:
	u32 t_;
	mark_type& mark_;
};

template<typename Mark>
struct bson_mark_end
{
public:
	typedef Mark mark_type;
public:
	bson_mark_end(u32 t, mark_type& mark)
		: t_(t), mark_(mark)
	{
	}

	~bson_mark_end(void)
	{
	}

public:
	u32 t_;
	mark_type& mark_;
};

} // namespace nsql_database_system
} // namespace yggr

#endif // __YGGR_NSQL_DATABASE_SYSTEM_BSON_TYPE_TRAITS_HPP__
