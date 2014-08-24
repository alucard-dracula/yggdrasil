//lua_export_wrap_iterator.hpp

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

#ifndef __YGGR_LUA_WRAP_LUA_EXPORT_WRAP_ITERATOR_HPP__
#define __YGGR_LUA_WRAP_LUA_EXPORT_WRAP_ITERATOR_HPP__

#include <yggr/lua_wrap/wrap_iterator.hpp>
#include <boost/preprocessor/cat.hpp>
#include <yggr/ppex/cast_to_string.hpp>

//----------------------------random_access_iters--------------------------------
#define EXPORT_WRAP_VALUE_RANDOM_ACCESS_ITERATOR_TO_LUA( __cpp_iter_type__, __lua_iter_type__ ) \
	luabind::class_< __cpp_iter_type__ >( YGGR_PP_CAST_TO_STRING( __lua_iter_type__ ) ) \
		.def( luabind::constructor<  >(  ) ) \
		.def( luabind::constructor< const __cpp_iter_type__& >(  ) ) \
		.def( luabind::tostring( luabind::const_self ) ) \
		.def( luabind::const_self == luabind::const_self ) \
		.def( luabind::const_self + luabind::other< const __cpp_iter_type__::wrap_diff_type& >(  ) ) \
		.def( luabind::const_self - luabind::other< const __cpp_iter_type__::wrap_diff_type& >(  ) ) \
		.def( luabind::const_self - luabind::const_self ) \
		.def( "next", &__cpp_iter_type__::next ) \
		.def( "before", &__cpp_iter_type__::before ) \
		.def( "get_val", &__cpp_iter_type__::get_val ) \
		.def( "set_val", &__cpp_iter_type__::set_val )

#define EXPORT_WRAP_VALUE_RANDOM_ACCESS_CONST_ITERATOR_TO_LUA( __cpp_citer_type__, __lua_citer_type__ ) \
	luabind::class_< __cpp_citer_type__ >( YGGR_PP_CAST_TO_STRING( __lua_citer_type__ ) ) \
		.def( luabind::constructor<  >(  ) ) \
		.def( luabind::constructor< const __cpp_citer_type__& >(  ) ) \
		.def( luabind::tostring( luabind::const_self ) ) \
		.def( luabind::const_self == luabind::const_self ) \
		.def( luabind::const_self + luabind::other< const __cpp_citer_type__::wrap_diff_type& >(  ) ) \
		.def( luabind::const_self - luabind::other< const __cpp_citer_type__::wrap_diff_type& >(  ) ) \
		.def( luabind::const_self - luabind::const_self ) \
		.def( "next", &__cpp_citer_type__::next ) \
		.def( "before", &__cpp_citer_type__::before ) \
		.def( "get_val", &__cpp_citer_type__::get_val )

#define EXPORT_WRAP_VALUE_RANDOM_ACCESS_ITERATORS_TO_LUA( __cpp_iter_type__, __cpp_citer_type__, __lua_iter_type__, __lua_citer_type__ ) \
	EXPORT_WRAP_VALUE_RANDOM_ACCESS_ITERATOR_TO_LUA( __cpp_iter_type__, __lua_iter_type__ ) \
		.def( luabind::const_self == luabind::other< __cpp_citer_type__ >(  ) ) \
			, \
	EXPORT_WRAP_VALUE_RANDOM_ACCESS_CONST_ITERATOR_TO_LUA( __cpp_citer_type__, __lua_citer_type__ ) \
		.def( luabind::const_self == luabind::other< __cpp_iter_type__ >(  ) )

//----------------------------random_access_reverse_iters--------------------------------

#define EXPORT_WRAP_VALUE_RANDOM_ACCESS_REVERSE_ITERATOR_TO_LUA EXPORT_WRAP_VALUE_RANDOM_ACCESS_ITERATOR_TO_LUA
#define EXPORT_WRAP_VALUE_RANDOM_ACCESS_CONST_REVERSE_ITERATOR_TO_LUA EXPORT_WRAP_VALUE_RANDOM_ACCESS_CONST_ITERATOR_TO_LUA
#define EXPORT_WRAP_VALUE_RANDOM_ACCESS_REVERSE_ITERATORS_TO_LUA EXPORT_WRAP_VALUE_RANDOM_ACCESS_ITERATORS_TO_LUA

//----------------------------normal_iters--------------------------------

#define EXPORT_WRAP_VALUE_ITERATOR_TO_LUA( __cpp_iter_type__, __lua_iter_type__ ) \
	luabind::class_< __cpp_iter_type__ >( YGGR_PP_CAST_TO_STRING( __lua_iter_type__ ) ) \
		.def( luabind::constructor<  >(  ) ) \
		.def( luabind::constructor< const __cpp_iter_type__& >(  ) ) \
		.def( luabind::tostring( luabind::const_self ) ) \
		.def( luabind::const_self == luabind::const_self ) \
		.def( "next", &__cpp_iter_type__::next ) \
		.def( "before", &__cpp_iter_type__::before ) \
		.def( "get_val", &__cpp_iter_type__::get_val ) \
		.def( "set_val", &__cpp_iter_type__::set_val )

#define EXPORT_WRAP_VALUE_CONST_ITERATOR_TO_LUA( __cpp_citer_type__, __lua_citer_type__ ) \
	luabind::class_< __cpp_citer_type__ >( YGGR_PP_CAST_TO_STRING( __lua_citer_type__ ) ) \
		.def( luabind::constructor<  >(  ) ) \
		.def( luabind::constructor< const __cpp_citer_type__& >(  ) ) \
		.def( luabind::tostring( luabind::const_self ) ) \
		.def( luabind::const_self == luabind::const_self ) \
		.def( "next", &__cpp_citer_type__::next ) \
		.def( "before", &__cpp_citer_type__::before ) \
		.def( "get_val", &__cpp_citer_type__::get_val )

#define EXPORT_WRAP_VALUE_ITERATORS_TO_LUA( __cpp_iter_type__, __cpp_citer_type__, __lua_iter_type__, __lua_citer_type__ ) \
	EXPORT_WRAP_VALUE_ITERATOR_TO_LUA( __cpp_iter_type__, __lua_iter_type__ ) \
		.def( luabind::const_self == luabind::other< __cpp_citer_type__ >(  ) ) \
			, \
	EXPORT_WRAP_VALUE_CONST_ITERATOR_TO_LUA( __cpp_citer_type__, __lua_citer_type__ ) \
		.def( luabind::const_self == luabind::other< __cpp_iter_type__ >(  ) )

//----------------------------normal_reverse_iters--------------------------------
#define EXPORT_WRAP_VALUE_REVERSE_ITERATOR_TO_LUA EXPORT_WRAP_VALUE_ITERATOR_TO_LUA

#define EXPORT_WRAP_VALUE_CONST_REVERSE_ITERATOR_TO_LUA EXPORT_WRAP_VALUE_CONST_ITERATOR_TO_LUA

#define EXPORT_WRAP_VALUE_REVERSE_ITERATORS_TO_LUA EXPORT_WRAP_VALUE_ITERATORS_TO_LUA

//----------------------------key_value_iters--------------------------------
#define EXPORT_WRAP_KEY_VALUE_ITERATOR_TO_LUA( __cpp_iter_type__, __lua_iter_type__ ) \
	luabind::class_< __cpp_iter_type__ >( YGGR_PP_CAST_TO_STRING( __lua_iter_type__ ) ) \
		.def( luabind::constructor<  >(  ) ) \
		.def( luabind::constructor< const __cpp_iter_type__& >(  ) ) \
		.def( luabind::tostring( luabind::const_self ) ) \
		.def( luabind::const_self == luabind::const_self ) \
		.def( "next", &__cpp_iter_type__::next ) \
		.def( "before", &__cpp_iter_type__::before ) \
		.def( "get_key", &__cpp_iter_type__::get_key ) \
		.def( "get_val", &__cpp_iter_type__::get_val ) \
		.def( "set_val", &__cpp_iter_type__::set_val )

#define EXPORT_WRAP_KEY_VALUE_CONST_ITERATOR_TO_LUA( __cpp_citer_type__, __lua_citer_type__ ) \
	luabind::class_< __cpp_citer_type__ >( YGGR_PP_CAST_TO_STRING( __lua_citer_type__ ) ) \
		.def( luabind::constructor<  >(  ) ) \
		.def( luabind::constructor< const __cpp_citer_type__& >(  ) ) \
		.def( luabind::tostring( luabind::const_self ) ) \
		.def( luabind::const_self == luabind::const_self ) \
		.def( "next", &__cpp_citer_type__::next ) \
		.def( "before", &__cpp_citer_type__::before ) \
		.def( "get_key", &__cpp_iter_type__::get_key ) \
		.def( "get_val", &__cpp_citer_type__::get_val )

#define EXPORT_WRAP_KEY_VALUE_ITERATORS_TO_LUA( __cpp_iter_type__, __cpp_citer_type__, __lua_iter_type__, __lua_citer_type__ ) \
	EXPORT_WRAP_KEY_VALUE_ITERATOR_TO_LUA( __cpp_iter_type__, __lua_iter_type__ ) \
		.def( luabind::const_self == luabind::other< __cpp_citer_type__ >(  ) ) \
			, \
	EXPORT_WRAP_KEY_VALUE_CONST_ITERATOR_TO_LUA( __cpp_citer_type__, __lua_citer_type__ ) \
		.def( luabind::const_self == luabind::other< __cpp_iter_type__ >(  ) )

//----------------------------key_value_reverse_iters--------------------------------

#define EXPORT_WRAP_KEY_VALUE_REVERSE_ITERATOR_TO_LUA EXPORT_WRAP_KEY_VALUE_ITERATOR_TO_LUA

#define EXPORT_WRAP_KEY_VALUE_CONST_REVERSE_ITERATOR_TO_LUA EXPORT_WRAP_KEY_VALUE_CONST_ITERATOR_TO_LUA

#define EXPORT_WRAP_KEY_VALUE_REVERSE_ITERATORS_TO_LUA EXPORT_WRAP_KEY_VALUE_ITERATORS_TO_LUA

namespace yggr
{
namespace lua_wrap
{
} // namespace lua_wrap
} // namespace yggr

#endif //__YGGR_LUA_WRAP_LUA_EXPORT_WRAP_ITERATOR_HPP__