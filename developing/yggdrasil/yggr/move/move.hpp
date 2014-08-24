//move.hpp

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

#ifndef __YGGR_MOVE_MOVE_HPP__
#define __YGGR_MOVE_MOVE_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/config.hpp> // must befor boost/move/move.hpp
#include <boost/move/move.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/or.hpp>
#include <boost/type_traits/is_class.hpp>
#include <boost/type_traits/is_union.hpp>

#define YGGR_PP_BASE_TYPE_FORCE_MOVE( __type__ ) \
	static __type__& forced_move(__type__& t) { \
		return t; }

namespace yggr
{
namespace move
{

struct move_helper
{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	template<typename T>
	static BOOST_RV_REF(T) forced_move(T& t)
	{
		return boost::forward<T>(t);
	}

	//template<typename T>
	//static BOOST_RV_REF(T) forced_move(BOOST_RV_REF(T) t)
	//{
	//	return boost::forward<T>(t);
	//}
#else
	template<typename T>
	static BOOST_RV_REF(T) forced_move(T& t)
	{
		BOOST_MPL_ASSERT((boost::mpl::or_< boost::is_class<T>, boost::is_union<T> >));
		typedef T value_type;
		typedef boost::rv<value_type> rv_type;

		return *static_cast< ::boost::rv<value_type>* >(&t);
	}

	YGGR_PP_BASE_TYPE_FORCE_MOVE(bool)
	YGGR_PP_BASE_TYPE_FORCE_MOVE(char)
	YGGR_PP_BASE_TYPE_FORCE_MOVE(wchar_t)
	YGGR_PP_BASE_TYPE_FORCE_MOVE(s8)
	YGGR_PP_BASE_TYPE_FORCE_MOVE(u8)
	YGGR_PP_BASE_TYPE_FORCE_MOVE(s16)
	YGGR_PP_BASE_TYPE_FORCE_MOVE(u16)
	YGGR_PP_BASE_TYPE_FORCE_MOVE(s32)
	YGGR_PP_BASE_TYPE_FORCE_MOVE(u32)
#	ifdef __ANDROID__
		YGGR_PP_BASE_TYPE_FORCE_MOVE(long int)
		YGGR_PP_BASE_TYPE_FORCE_MOVE(unsigned long int)
#	endif // __ANDROID__
	YGGR_PP_BASE_TYPE_FORCE_MOVE(s64)
	YGGR_PP_BASE_TYPE_FORCE_MOVE(u64)
	YGGR_PP_BASE_TYPE_FORCE_MOVE(f32)
	YGGR_PP_BASE_TYPE_FORCE_MOVE(f64)

	template<typename T>
	static T*& forced_move(T*& t)
	{
		return t;
	}
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES


};

} // namespace move
} // namespace yggr


#endif // __YGGR_MOVE_MOVE_HPP__
