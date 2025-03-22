//lua_export_wrap_random_access_iterator.hpp

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

#ifndef __YGGR_LUA_WRAP_LUA_EXPORT_WRAP_RANDOM_ACCESS_ITERATOR_HPP__
#define __YGGR_LUA_WRAP_LUA_EXPORT_WRAP_RANDOM_ACCESS_ITERATOR_HPP__

#include <yggr/mplex/static_assert.hpp>

#include <yggr/lua_wrap/lua_export_typeid.hpp>
#include <yggr/lua_wrap/wrap_base_t.hpp>
#include <yggr/lua_wrap/wrap_random_access_iterator.hpp>
#include <yggr/lua_wrap/lua_export_wrap_iterator_l_wrap_base_t.hpp>

#include <boost/type_traits/is_same.hpp>

#include <luabind/luabind.hpp>
#include <luabind/lua_include.hpp>
#include <luabind/operator.hpp>

namespace yggr
{
namespace lua_wrap
{

//-------------wrap_random_access_iterator------------------------

template<typename T, typename X1, typename X2, typename X3> inline
luabind::class_<T, X1, X2, X3>&
	export_wrap_random_access_iterator(luabind::class_<T, X1, X2, X3>& cobj)
{
	typedef T wrap_iter_type;

	cobj
		.def(luabind::constructor<>())
		.def(luabind::constructor<const wrap_iter_type&>())

		.def(luabind::tostring(luabind::const_self))

		.def(luabind::const_self < luabind::const_self)
		.def(luabind::const_self <= luabind::const_self)
		.def(luabind::const_self == luabind::const_self)

		.def(double() + luabind::const_self)
		.def(luabind::const_self + double())
		
		// !!!about export to lua userdata, can only using export luabind::const_self op other<>,!!!
		// !!!luabind not support export luabind::other<> op const::self in lua userdata. !!!
		//.def(luabind::other<s64_wrap_idx_type>() + luabind::const_self)
		//.def(luabind::other<u64_wrap_idx_type>() + luabind::const_self)

		.def(luabind::const_self - double())
		.def(luabind::const_self - luabind::const_self)
					
		.def("swap", static_cast<void (wrap_iter_type::*)(wrap_iter_type&)>(&wrap_iter_type::swap))
					
		.def("set_iterator",
				static_cast<wrap_iter_type& (wrap_iter_type::*)(const wrap_iter_type&)>(
					&wrap_iter_type::set_iterator))
		.def("copy", &wrap_iter_type::copy)

		.def("get_value", 
				static_cast
				<
					typename 
						boost::mpl::if_
						<
							boost::is_class<typename wrap_iter_type::value_type>,
							typename wrap_iter_type::reference,
							typename wrap_iter_type::value_type
						>::type (wrap_iter_type::*)(void) const
				>(&wrap_iter_type::get_value))

		.def("get_value", 
				static_cast
				<
					typename 
						boost::mpl::if_
						<
							boost::is_class<typename wrap_iter_type::value_type>,
							typename wrap_iter_type::reference,
							typename wrap_iter_type::value_type
						>::type (wrap_iter_type::*)(double) const
				>(&wrap_iter_type::template get_value<double>))

		.def("set_value", 
				static_cast
				< 
					wrap_iter_type& (wrap_iter_type::*)(const typename wrap_iter_type::value_type&) const 
				>(&wrap_iter_type::set_value))

		.def("set_value",
				static_cast
				< 
					wrap_iter_type& (wrap_iter_type::*)(double, const typename wrap_iter_type::value_type&) const 
				>(&wrap_iter_type::template set_value<double>))

		.def("head_increment", &wrap_iter_type::head_increment)
		.def("tail_increment", &wrap_iter_type::tail_increment)

		.def("head_decrement", &wrap_iter_type::head_decrement)
		.def("tail_decrement", &wrap_iter_type::tail_decrement)

		.def("plus", static_cast<wrap_iter_type (wrap_iter_type::*)(double) const>(&wrap_iter_type::template plus<double>))

		.def("plus_set", static_cast<wrap_iter_type& (wrap_iter_type::*)(double)>(&wrap_iter_type::template plus_set<double>))

		.def("minus", static_cast<wrap_iter_type (wrap_iter_type::*)(double) const>(&wrap_iter_type::template minus<double>))
		.def("minus", static_cast<typename wrap_iter_type::difference_type (wrap_iter_type::*)(const wrap_iter_type&) const>(
						&wrap_iter_type::minus))
					
		.def("minus_set", static_cast<wrap_iter_type& (wrap_iter_type::*)(double)>(&wrap_iter_type::template minus_set<double>))

		.def("wraped_minus", 
				static_cast
				<
					typename lua_wrap::wrap_base_t_maker<typename wrap_iter_type::difference_type>::type
						(wrap_iter_type::*)(const wrap_iter_type&) const
				>(&wrap_iter_type::wraped_minus))

		.def("compare_eq", static_cast<bool (wrap_iter_type::*)(const wrap_iter_type&) const>(
							&wrap_iter_type::compare_eq))

		.def("compare", static_cast<s32 (wrap_iter_type::*)(const wrap_iter_type&) const>(
							&wrap_iter_type::compare))

		.def("next", &wrap_iter_type::next)
		.def("prev", &wrap_iter_type::prev)
	;

	return cobj;
}

template<typename R, typename T, typename X1, typename X2, typename X3> inline
luabind::class_<T, X1, X2, X3>&
	export_wrap_random_access_iterator_r_const_iterator(luabind::class_<T, X1, X2, X3>& cobj)
{
	typedef T wrap_iter_type;
	typedef R wrap_citer_type;

	cobj
		.def(luabind::const_self < luabind::other<wrap_citer_type>())
		.def(luabind::const_self <= luabind::other<wrap_citer_type>())
		.def(luabind::const_self == luabind::other<wrap_citer_type>())

		.def(luabind::const_self - luabind::other<wrap_citer_type>())

		.def("minus", static_cast<typename wrap_iter_type::difference_type (wrap_iter_type::*)(const wrap_citer_type&) const>(
						&wrap_iter_type::template minus<wrap_citer_type>))

		.def("wraped_minus", 
				static_cast
				<
					typename lua_wrap::wrap_base_t_maker<typename wrap_iter_type::difference_type>::type
						(wrap_iter_type::*)(const wrap_citer_type&) const
				>(&wrap_iter_type::template wraped_minus<wrap_citer_type>))
					
		.def("compare_eq", static_cast<bool (wrap_iter_type::*)(const wrap_citer_type&) const>(
							&wrap_iter_type::template compare_eq<wrap_citer_type>))

		.def("compare", static_cast<s32 (wrap_iter_type::*)(const wrap_citer_type&) const>(
							&wrap_iter_type::template compare<wrap_citer_type>))
	;

	return cobj;
}

template<typename R, typename T, typename X1, typename X2, typename X3> inline
luabind::class_<T, X1, X2, X3>&
	export_wrap_random_access_iterator_r_wrap_base_t(luabind::class_<T, X1, X2, X3>& cobj)
{
	typedef T wrap_iter_type;
	typedef R wrap_base_type;
	
	cobj
		.def(luabind::const_self + luabind::other<wrap_base_type>())

		.def(luabind::const_self - luabind::other<wrap_base_type>())
					
		.def("get_value", 
				static_cast
				<
					typename 
						boost::mpl::if_
						<
							boost::is_class<typename wrap_iter_type::value_type>,
							typename wrap_iter_type::reference,
							typename wrap_iter_type::value_type
						>::type (wrap_iter_type::*)(const wrap_base_type&) const
				>(&wrap_iter_type::template get_value<wrap_base_type>))

		.def("set_value",
				static_cast
				< 
					wrap_iter_type& (wrap_iter_type::*)(const wrap_base_type&, const typename wrap_iter_type::value_type&) const 
				>(&wrap_iter_type::template set_value<wrap_base_type>))

		.def("plus", static_cast<wrap_iter_type (wrap_iter_type::*)(const wrap_base_type&) const>(
						&wrap_iter_type::template plus<wrap_base_type>))

		.def("plus_set", static_cast<wrap_iter_type& (wrap_iter_type::*)(const wrap_base_type&)>(
							&wrap_iter_type::template plus_set<wrap_base_type>))

		.def("minus", static_cast<wrap_iter_type (wrap_iter_type::*)(const wrap_base_type&) const>(
						&wrap_iter_type::template minus<wrap_base_type>))
					
		.def("minus_set", static_cast<wrap_iter_type& (wrap_iter_type::*)(const wrap_base_type&)>(
							&wrap_iter_type::template minus_set<wrap_base_type>))
	;

	return cobj;
}

//-------------wrap_random_access_const_iterator------------------------

template<typename T, typename X1, typename X2, typename X3> inline
luabind::class_<T, X1, X2, X3>&
	export_wrap_random_access_const_iterator(luabind::class_<T, X1, X2, X3>& cobj)
{
	typedef T wrap_citer_type;

	cobj
		.def(luabind::constructor<>())
		.def(luabind::constructor<const wrap_citer_type&>())

		.def(luabind::tostring(luabind::const_self))

		.def(luabind::const_self < luabind::const_self)
		.def(luabind::const_self <= luabind::const_self)
		.def(luabind::const_self == luabind::const_self)

		.def(double() + luabind::const_self)
		.def(luabind::const_self + double())

		.def(luabind::const_self - double())
		.def(luabind::const_self - luabind::const_self)
					
		.def("swap", static_cast<void (wrap_citer_type::*)(wrap_citer_type&)>(&wrap_citer_type::swap))
		
		.def("set_iterator",
				static_cast<wrap_citer_type& (wrap_citer_type::*)(const wrap_citer_type&)>(
					&wrap_citer_type::set_iterator))

		.def("copy", &wrap_citer_type::copy)

		.def("get_value", 
				static_cast
				<
					typename 
						boost::mpl::if_
						<
							boost::is_class<typename wrap_citer_type::value_type>,
							typename wrap_citer_type::reference,
							typename wrap_citer_type::value_type
						>::type (wrap_citer_type::*)(void) const
				>(&wrap_citer_type::get_value))

		.def("get_value", 
				static_cast
				<
					typename 
						boost::mpl::if_
						<
							boost::is_class<typename wrap_citer_type::value_type>,
							typename wrap_citer_type::reference,
							typename wrap_citer_type::value_type
						>::type (wrap_citer_type::*)(double) const
				>(&wrap_citer_type::template get_value<double>))

		.def("head_increment", &wrap_citer_type::head_increment)
		.def("tail_increment", &wrap_citer_type::tail_increment)

		.def("head_decrement", &wrap_citer_type::head_decrement)
		.def("tail_decrement", &wrap_citer_type::tail_decrement)

		.def("plus", static_cast<wrap_citer_type (wrap_citer_type::*)(double) const>(&wrap_citer_type::template plus<double>))

		.def("plus_set", static_cast<wrap_citer_type& (wrap_citer_type::*)(double)>(&wrap_citer_type::template plus_set<double>))

		.def("minus", static_cast<wrap_citer_type (wrap_citer_type::*)(double) const>(&wrap_citer_type::template minus<double>))
		.def("minus", static_cast<typename wrap_citer_type::difference_type (wrap_citer_type::*)(const wrap_citer_type&) const>(
						&wrap_citer_type::minus))
					
		.def("minus_set", static_cast<wrap_citer_type& (wrap_citer_type::*)(double)>(&wrap_citer_type::template minus_set<double>))

		.def("wraped_minus", 
				static_cast
				<
					typename lua_wrap::wrap_base_t_maker<typename wrap_citer_type::difference_type>::type
						(wrap_citer_type::*)(const wrap_citer_type&) const
				>(&wrap_citer_type::wraped_minus))
		
		.def("compare_eq", static_cast<bool (wrap_citer_type::*)(const wrap_citer_type&) const>(
							&wrap_citer_type::compare_eq))

		.def("compare", static_cast<s32 (wrap_citer_type::*)(const wrap_citer_type&) const>(
							&wrap_citer_type::compare))

		.def("next", &wrap_citer_type::next)
		.def("prev", &wrap_citer_type::prev)
	;

	return cobj;
}

template<typename R, typename T, typename X1, typename X2, typename X3> inline
luabind::class_<T, X1, X2, X3>&
	export_wrap_random_access_const_iterator_r_iterator(luabind::class_<T, X1, X2, X3>& cobj)
{
	typedef T wrap_citer_type;
	typedef R wrap_iter_type;

	cobj
		.def(luabind::constructor<const wrap_iter_type&>())

		.def(luabind::const_self < luabind::other<const wrap_iter_type&>())
		.def(luabind::const_self <= luabind::other<const wrap_iter_type&>())
		.def(luabind::const_self == luabind::other<const wrap_iter_type&>())

		.def(luabind::const_self - luabind::other<const wrap_iter_type&>())
		
		.def("set_iterator",
				static_cast<wrap_citer_type& (wrap_citer_type::*)(const wrap_iter_type&)>(
					&wrap_citer_type::template set_iterator<wrap_iter_type>))

		.def("minus", static_cast<typename wrap_citer_type::difference_type (wrap_citer_type::*)(const wrap_iter_type&) const>(
						&wrap_citer_type::template minus<wrap_iter_type>))

		.def("wraped_minus", 
				static_cast
				<
					typename lua_wrap::wrap_base_t_maker<typename wrap_citer_type::difference_type>::type
						(wrap_citer_type::*)(const wrap_iter_type&) const
				>(&wrap_citer_type::template wraped_minus<wrap_iter_type>))

		.def("compare_eq", static_cast<bool (wrap_citer_type::*)(const wrap_iter_type&) const>(
							&wrap_citer_type::template compare_eq<wrap_iter_type>))

		.def("compare", static_cast<s32 (wrap_citer_type::*)(const wrap_iter_type&) const>(
							&wrap_citer_type::template compare<wrap_iter_type>))
		;
	return cobj;
}

template<typename R, typename T, typename X1, typename X2, typename X3> inline
luabind::class_<T, X1, X2, X3>&
	export_wrap_random_access_const_iterator_r_wrap_base_t(luabind::class_<T, X1, X2, X3>& cobj)
{
	typedef T wrap_citer_type;
	typedef R wrap_base_type;

	cobj
		.def(luabind::const_self + luabind::other<wrap_base_type>())

		.def(luabind::const_self - luabind::other<wrap_base_type>())
		
		.def("get_value", 
				static_cast
				<
					typename 
						boost::mpl::if_
						<
							boost::is_class<typename wrap_citer_type::value_type>,
							typename wrap_citer_type::reference,
							typename wrap_citer_type::value_type
						>::type (wrap_citer_type::*)(const wrap_base_type&) const
				>(&wrap_citer_type::template get_value<wrap_base_type>))

		.def("plus", static_cast<wrap_citer_type (wrap_citer_type::*)(const wrap_base_type&) const>(
						&wrap_citer_type::template plus<wrap_base_type>))

		.def("plus_set", static_cast<wrap_citer_type& (wrap_citer_type::*)(const wrap_base_type&)>(
							&wrap_citer_type::template plus_set<wrap_base_type>))

		.def("minus", static_cast<wrap_citer_type (wrap_citer_type::*)(const wrap_base_type&) const>(
						&wrap_citer_type::template minus<wrap_base_type>))
					
		.def("minus_set", static_cast<wrap_citer_type& (wrap_citer_type::*)(const wrap_base_type&)>(
							&wrap_citer_type::template minus_set<wrap_base_type>))
	;

	return cobj;
}

} // namespace lua_wrap
} // namespace yggr

#endif // __YGGR_LUA_WRAP_LUA_EXPORT_WRAP_RANDOM_ACCESS_ITERATOR_HPP__
