//lua_export_wrap_vector.hpp

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

#ifndef __YGGR_LUA_WRAP_LUA_EXPORT_WRAP_VECTOR_HPP__
#define __YGGR_LUA_WRAP_LUA_EXPORT_WRAP_VECTOR_HPP__

#include <yggr/lua_wrap/lua_export_wrap_iterator.hpp>
#include <yggr/lua_wrap/lua_export_wrap_allocator.hpp>
#include <yggr/lua_wrap/wrap_vector.hpp>
#include <boost/preprocessor/cat.hpp>
#include <yggr/ppex/cast_to_string.hpp>

/*

*/
#define EXPORT_WRAP_VECTOR_TO_LUA( __cpp_vt_type__, __lua_vt_type__ ) \
	EXPORT_WRAP_VALUE_RANDOM_ACCESS_ITERATORS_TO_LUA( __cpp_vt_type__::wrap_iterator_type, \
														__cpp_vt_type__::wrap_const_iterator_type, \
														BOOST_PP_CAT( __lua_vt_type__, _iterator ), \
														BOOST_PP_CAT( __lua_vt_type__, _const_iterator ) ) \
		, \
	EXPORT_WRAP_VALUE_RANDOM_ACCESS_REVERSE_ITERATORS_TO_LUA( __cpp_vt_type__::wrap_reverse_iterator_type, \
																__cpp_vt_type__::wrap_const_reverse_iterator_type, \
																BOOST_PP_CAT( __lua_vt_type__, _reverse_iterator ), \
																BOOST_PP_CAT( __lua_vt_type__, _const_reverse_iterator ) ) \
		, \
	EXPORT_WRAP_ALLOCATOR_TO_LUA( __cpp_vt_type__::wrap_allocator_type, BOOST_PP_CAT( __lua_vt_type__, _allocator ) ) \
		, \
	luabind::class_< __cpp_vt_type__ >( YGGR_PP_CAST_TO_STRING( __lua_vt_type__ ) ) \
		.def( luabind::constructor<  >(  ) ) \
		.def( luabind::constructor< const __cpp_vt_type__::wrap_allocator_type& >(  ) ) \
		.def( luabind::constructor< const __cpp_vt_type__::wrap_size_type&, const __cpp_vt_type__::wrap_val_type& >(  ) ) \
		.def( luabind::constructor< const __cpp_vt_type__::wrap_size_type&, const __cpp_vt_type__::wrap_val_type&, const __cpp_vt_type__::wrap_allocator_type& >(  ) ) \
		.def( luabind::constructor< const __cpp_vt_type__::wrap_iterator_type&, const __cpp_vt_type__::wrap_iterator_type& >(  ) ) \
		.def( luabind::constructor< const __cpp_vt_type__::wrap_const_iterator_type&, const __cpp_vt_type__::wrap_const_iterator_type& >(  ) ) \
		.def( luabind::constructor< const __cpp_vt_type__::wrap_reverse_iterator_type&, const __cpp_vt_type__::wrap_reverse_iterator_type& >(  ) ) \
		.def( luabind::constructor< const __cpp_vt_type__::wrap_const_reverse_iterator_type&, const __cpp_vt_type__::wrap_const_reverse_iterator_type& >(  ) ) \
		.def( luabind::constructor< const __cpp_vt_type__::wrap_iterator_type&, const __cpp_vt_type__::wrap_iterator_type&, const __cpp_vt_type__::wrap_allocator_type& >(  ) ) \
		.def( luabind::constructor< const __cpp_vt_type__::wrap_const_iterator_type&, const __cpp_vt_type__::wrap_const_iterator_type&, const __cpp_vt_type__::wrap_allocator_type& >(  ) ) \
		.def( luabind::constructor< const __cpp_vt_type__::wrap_reverse_iterator_type&, const __cpp_vt_type__::wrap_reverse_iterator_type&, const __cpp_vt_type__::wrap_allocator_type& >(  ) ) \
		.def( luabind::constructor< const __cpp_vt_type__::wrap_const_reverse_iterator_type&, const __cpp_vt_type__::wrap_const_reverse_iterator_type&, const __cpp_vt_type__::wrap_allocator_type& >(  ) ) \
		.def( luabind::constructor< const __cpp_vt_type__& >(  ) ) \
		.def( luabind::tostring( luabind::const_self ) ) \
		.def( "begin_", ( __cpp_vt_type__::wrap_iterator_type ( __cpp_vt_type__::* )( void ) )&__cpp_vt_type__::begin ) \
		.def( "begin_", ( __cpp_vt_type__::wrap_const_iterator_type ( __cpp_vt_type__::* )( void ) const )&__cpp_vt_type__::begin ) \
		.def( "end_", ( __cpp_vt_type__::wrap_iterator_type ( __cpp_vt_type__::* )( void ) )&__cpp_vt_type__::end ) \
		.def( "end_", ( __cpp_vt_type__::wrap_const_iterator_type ( __cpp_vt_type__::* )( void ) const )&__cpp_vt_type__::end ) \
		.def( "rbegin_", ( __cpp_vt_type__::wrap_reverse_iterator_type ( __cpp_vt_type__::* )( void ) )&__cpp_vt_type__::rbegin ) \
		.def( "rbegin_", ( __cpp_vt_type__::wrap_const_reverse_iterator_type ( __cpp_vt_type__::* )( void ) const )&__cpp_vt_type__::rbegin ) \
		.def( "rend_", ( __cpp_vt_type__::wrap_reverse_iterator_type ( __cpp_vt_type__::* )( void ) )&__cpp_vt_type__::rend ) \
		.def( "rend_", ( __cpp_vt_type__::wrap_const_reverse_iterator_type ( __cpp_vt_type__::* )( void ) const )&__cpp_vt_type__::rend ) \
		.def( "size", &__cpp_vt_type__::size ) \
		.def( "max_size", &__cpp_vt_type__::max_size ) \
		.def( "resize", &__cpp_vt_type__::resize ) \
		.def( "capacity", &__cpp_vt_type__::capacity ) \
		.def( "reserve", &__cpp_vt_type__::reserve ) \
		.def( "empty", &__cpp_vt_type__::empty ) \
		.def( "get_index_of_val", &__cpp_vt_type__::get_index_of_val ) \
		.def( "get_val_of_index", &__cpp_vt_type__::get_val_of_index ) \
		.def( "at", ( __cpp_vt_type__::wrap_val_type& ( __cpp_vt_type__::* )( const __cpp_vt_type__::wrap_size_type& ) )&__cpp_vt_type__::at ) \
		.def( "at", ( const __cpp_vt_type__::wrap_val_type& ( __cpp_vt_type__::* )( const __cpp_vt_type__::wrap_size_type& ) const )&__cpp_vt_type__::at ) \
		.def( "set_val_of_index", &__cpp_vt_type__::set_val_of_index ) \
		.def( "front", ( __cpp_vt_type__::wrap_val_type& ( __cpp_vt_type__::* )( void ) )&__cpp_vt_type__::front ) \
		.def( "front", ( const __cpp_vt_type__::wrap_val_type& ( __cpp_vt_type__::* )( void ) const )&__cpp_vt_type__::front ) \
		.def( "back", ( __cpp_vt_type__::wrap_val_type& ( __cpp_vt_type__::* )( void ) )&__cpp_vt_type__::back ) \
		.def( "back", ( const __cpp_vt_type__::wrap_val_type& ( __cpp_vt_type__::* )( void ) const )&__cpp_vt_type__::back ) \
		.def( "assign", ( void ( __cpp_vt_type__::* )( const __cpp_vt_type__::wrap_size_type&, const __cpp_vt_type__::wrap_val_type& ) )&__cpp_vt_type__::assign ) \
		.def( "assign", ( void ( __cpp_vt_type__::* )( const __cpp_vt_type__::wrap_iterator_type&, const __cpp_vt_type__::wrap_iterator_type& ) )&__cpp_vt_type__::assign ) \
		.def( "assign", ( void ( __cpp_vt_type__::* )( const __cpp_vt_type__::wrap_const_iterator_type&, const __cpp_vt_type__::wrap_const_iterator_type& ) )&__cpp_vt_type__::assign ) \
		.def( "assign", ( void ( __cpp_vt_type__::* )( const __cpp_vt_type__::wrap_reverse_iterator_type&, const __cpp_vt_type__::wrap_reverse_iterator_type& ) )&__cpp_vt_type__::assign ) \
		.def( "assign", ( void ( __cpp_vt_type__::* )( const __cpp_vt_type__::wrap_const_reverse_iterator_type&, const __cpp_vt_type__::wrap_const_reverse_iterator_type& ) )&__cpp_vt_type__::assign ) \
		.def( "push_back", &__cpp_vt_type__::push_back ) \
		.def( "pop_back", &__cpp_vt_type__::pop_back ) \
		.def( "insert", ( __cpp_vt_type__::wrap_iterator_type ( __cpp_vt_type__::* )( const __cpp_vt_type__::wrap_iterator_type&, const __cpp_vt_type__::wrap_val_type& ) )&__cpp_vt_type__::insert ) \
		.def( "insert", ( void ( __cpp_vt_type__::* )( const __cpp_vt_type__::wrap_iterator_type&, const __cpp_vt_type__::wrap_size_type&, const __cpp_vt_type__::wrap_val_type& ) )&__cpp_vt_type__::insert ) \
		.def( "insert", ( void ( __cpp_vt_type__::* )( const __cpp_vt_type__::wrap_iterator_type&, const __cpp_vt_type__::wrap_size_type&, const __cpp_vt_type__::wrap_val_type& ) )&__cpp_vt_type__::insert ) \
		.def( "insert", ( void ( __cpp_vt_type__::* )( const __cpp_vt_type__::wrap_iterator_type&, const __cpp_vt_type__::wrap_iterator_type&, const __cpp_vt_type__::wrap_iterator_type& ) )&__cpp_vt_type__::insert ) \
		.def( "insert", ( void ( __cpp_vt_type__::* )( const __cpp_vt_type__::wrap_iterator_type&, const __cpp_vt_type__::wrap_const_iterator_type&, const __cpp_vt_type__::wrap_const_iterator_type& ) )&__cpp_vt_type__::insert ) \
		.def( "insert", ( void ( __cpp_vt_type__::* )( const __cpp_vt_type__::wrap_iterator_type&, const __cpp_vt_type__::wrap_reverse_iterator_type&, const __cpp_vt_type__::wrap_reverse_iterator_type& ) )&__cpp_vt_type__::insert ) \
		.def( "insert", ( void ( __cpp_vt_type__::* )( const __cpp_vt_type__::wrap_iterator_type&, const __cpp_vt_type__::wrap_const_reverse_iterator_type&, const __cpp_vt_type__::wrap_const_reverse_iterator_type& ) )&__cpp_vt_type__::insert ) \
		.def( "erase", ( bool ( __cpp_vt_type__::* )( const __cpp_vt_type__::wrap_size_type&, const __cpp_vt_type__::wrap_size_type& ) )&__cpp_vt_type__::erase ) \
		.def( "erase", ( __cpp_vt_type__::wrap_iterator_type ( __cpp_vt_type__::* )( const __cpp_vt_type__::wrap_iterator_type& ) )&__cpp_vt_type__::erase ) \
		.def( "erase", ( __cpp_vt_type__::wrap_iterator_type ( __cpp_vt_type__::* )( const __cpp_vt_type__::wrap_iterator_type&, const __cpp_vt_type__::wrap_iterator_type& ) )&__cpp_vt_type__::erase ) \
		.def( "swap", &__cpp_vt_type__::swap ) \
		.def( "clear", &__cpp_vt_type__::clear ) \
		.def( "get_allocator", &__cpp_vt_type__::get_allocator ) \
		.def( "sort", ( void ( __cpp_vt_type__::* )( yggr::u32 ) )&__cpp_vt_type__::sort ) \
		.def( "sort", ( void ( __cpp_vt_type__::* )( const __cpp_vt_type__::wrap_size_type&, const __cpp_vt_type__::wrap_size_type&, yggr::u32 ) )&__cpp_vt_type__::sort ) \
		.def( "sort", ( void ( __cpp_vt_type__::* )( const __cpp_vt_type__::wrap_iterator_type&, const __cpp_vt_type__::wrap_iterator_type&, yggr::u32 ) )&__cpp_vt_type__::sort ) \
		.def( "random_shuffle", ( void ( __cpp_vt_type__::* )( void ) )&__cpp_vt_type__::random_shuffle ) \
		.def( "random_shuffle", ( void ( __cpp_vt_type__::* )( const __cpp_vt_type__::wrap_size_type&, const __cpp_vt_type__::wrap_size_type& ) )&__cpp_vt_type__::random_shuffle ) \
		.def( "random_shuffle", ( void ( __cpp_vt_type__::* )( const __cpp_vt_type__::wrap_iterator_type&, const __cpp_vt_type__::wrap_iterator_type& ) )&__cpp_vt_type__::random_shuffle ) \
		.def( "reverse", ( void ( __cpp_vt_type__::* )( void ) )&__cpp_vt_type__::reverse ) \
		.def( "reverse", ( void ( __cpp_vt_type__::* )( const __cpp_vt_type__::wrap_size_type&, const __cpp_vt_type__::wrap_size_type& ) )&__cpp_vt_type__::reverse ) \
		.def( "reverse", ( void ( __cpp_vt_type__::* )( const __cpp_vt_type__::wrap_iterator_type&, const __cpp_vt_type__::wrap_iterator_type& ) )&__cpp_vt_type__::reverse )

namespace yggr
{
namespace lua_wrap
{
} // namespace lua_wrap
} // namespace yggr

#endif //__YGGR_LUA_WRAP_LUA_EXPORT_WRAP_VECTOR_HPP__