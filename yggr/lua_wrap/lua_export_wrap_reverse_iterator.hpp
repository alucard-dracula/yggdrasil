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

#ifndef __YGGR_LUA_WRAP_LUA_EXPORT_WRAP_REVERSE_ITERATOR_HPP__
#define __YGGR_LUA_WRAP_LUA_EXPORT_WRAP_REVERSE_ITERATOR_HPP__

#include <yggr/mplex/static_assert.hpp>

#include <yggr/iterator_ex/iterator_category_check.hpp>
#include <yggr/iterator_ex/is_const_iterator.hpp>

#include <yggr/lua_wrap/lua_export_typeid.hpp>
#include <yggr/lua_wrap/wrap_base_t.hpp>
#include <yggr/lua_wrap/wrap_reverse_iterator.hpp>

#include <boost/utility/enable_if.hpp>

#include <luabind/luabind.hpp>
#include <luabind/lua_include.hpp>
#include <luabind/operator.hpp>

namespace yggr
{
namespace lua_wrap
{

//-------------wrap_random_access_iterator------------------------

template<typename T, typename X1, typename X2, typename X3> inline
typename 
	boost::enable_if
	<
		iterator_ex::is_random_access_iterator<T>,
		luabind::class_<T, X1, X2, X3>&
	>::type
	export_wrap_reverse_iterator_set_value_detail(luabind::class_<T, X1, X2, X3>& cobj)
{
	typedef T wrap_riter_type;

	cobj
		.def("set_value", 
				static_cast
				< 
					wrap_riter_type& (wrap_riter_type::*)(const typename wrap_riter_type::value_type&) const 
				>(&wrap_riter_type::set_value))

		.def("set_value",
				static_cast
				< 
					wrap_riter_type& (wrap_riter_type::*)(double, const typename wrap_riter_type::value_type&) const 
				>(&wrap_riter_type::template set_value<double>))
	;

	return cobj;
}

template<typename T, typename X1, typename X2, typename X3> inline
typename 
	boost::disable_if
	<
		iterator_ex::is_random_access_iterator<T>,
		luabind::class_<T, X1, X2, X3>&
	>::type
	export_wrap_reverse_iterator_set_value_detail(luabind::class_<T, X1, X2, X3>& cobj)
{
	typedef T wrap_riter_type;

	cobj
		.def("set_value", 
				static_cast
				< 
					wrap_riter_type& (wrap_riter_type::*)(const typename wrap_riter_type::value_type&) const 
				>(&wrap_riter_type::set_value))
	;

	return cobj;
}

template<typename T, typename X1, typename X2, typename X3> inline
typename 
	boost::disable_if
	<
		iterator_ex::is_const_iterator<T>,
		luabind::class_<T, X1, X2, X3>&
	>::type
	export_wrap_reverse_iterator_set_value(luabind::class_<T, X1, X2, X3>& cobj)
{
	return export_wrap_reverse_iterator_set_value_detail(cobj);
}

template<typename T, typename X1, typename X2, typename X3> inline
typename 
	boost::enable_if
	<
		iterator_ex::is_const_iterator<T>,
		luabind::class_<T, X1, X2, X3>&
	>::type
	export_wrap_reverse_iterator_set_value(luabind::class_<T, X1, X2, X3>& cobj)
{
	return cobj;
}

template<typename T, typename X1, typename X2, typename X3> inline
typename
	boost::enable_if
	<
		iterator_ex::is_random_access_iterator<T>,
		luabind::class_<T, X1, X2, X3>&
	>::type
	export_wrap_reverse_iterator(luabind::class_<T, X1, X2, X3>& cobj)
{
	typedef T wrap_riter_type;

	cobj
		.def(luabind::constructor<>())
		.def(luabind::constructor<const wrap_riter_type&>())

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
					
		.def("swap", static_cast<void (wrap_riter_type::*)(wrap_riter_type&)>(&wrap_riter_type::swap))
					
		.def("set_iterator",
				static_cast<wrap_riter_type& (wrap_riter_type::*)(const wrap_riter_type&)>(
					&wrap_riter_type::set_iterator))
		.def("copy", &wrap_riter_type::copy)

		.def("get_value", 
				static_cast
				<
					typename 
						boost::mpl::if_
						<
							boost::is_class<typename wrap_riter_type::value_type>,
							typename wrap_riter_type::reference,
							typename wrap_riter_type::value_type
						>::type (wrap_riter_type::*)(void) const
				>(&wrap_riter_type::get_value))

		.def("get_value", 
				static_cast
				<
					typename 
						boost::mpl::if_
						<
							boost::is_class<typename wrap_riter_type::value_type>,
							typename wrap_riter_type::reference,
							typename wrap_riter_type::value_type
						>::type (wrap_riter_type::*)(double) const
				>(&wrap_riter_type::template get_value<double>))

		.def("head_increment", &wrap_riter_type::head_increment)
		.def("tail_increment", &wrap_riter_type::tail_increment)

		.def("head_decrement", &wrap_riter_type::head_decrement)
		.def("tail_decrement", &wrap_riter_type::tail_decrement)

		.def("plus", static_cast<wrap_riter_type (wrap_riter_type::*)(double) const>(&wrap_riter_type::template plus<double>))

		.def("plus_set", static_cast<wrap_riter_type& (wrap_riter_type::*)(double)>(&wrap_riter_type::template plus_set<double>))

		.def("minus", static_cast<wrap_riter_type (wrap_riter_type::*)(double) const>(&wrap_riter_type::template minus<double>))
		.def("minus", static_cast<typename wrap_riter_type::difference_type (wrap_riter_type::*)(const wrap_riter_type&) const>(
						&wrap_riter_type::minus))
					
		.def("minus_set", static_cast<wrap_riter_type& (wrap_riter_type::*)(double)>(&wrap_riter_type::template minus_set<double>))

		.def("wraped_minus", 
				static_cast
				<
					typename lua_wrap::wrap_base_t_maker<typename wrap_riter_type::difference_type>::type
						(wrap_riter_type::*)(const wrap_riter_type&) const
				>(&wrap_riter_type::wraped_minus))

		.def("compare_eq", static_cast<bool (wrap_riter_type::*)(const wrap_riter_type&) const>(
							&wrap_riter_type::compare_eq))

		.def("compare", static_cast<s32 (wrap_riter_type::*)(const wrap_riter_type&) const>(
							&wrap_riter_type::compare))

		.def("next", &wrap_riter_type::next)
		.def("prev", &wrap_riter_type::prev)
	;

	return export_wrap_reverse_iterator_set_value(cobj);
}

template<typename T, typename X1, typename X2, typename X3> inline
typename
	boost::disable_if
	<
		iterator_ex::is_random_access_iterator<T>,
		luabind::class_<T, X1, X2, X3>&
	>::type
	export_wrap_reverse_iterator(luabind::class_<T, X1, X2, X3>& cobj)
{
	typedef T wrap_riter_type;

	cobj
		.def(luabind::constructor<>())
		.def(luabind::constructor<const wrap_riter_type&>())

		.def(luabind::tostring(luabind::const_self))

		.def(luabind::const_self == luabind::const_self)
					
		.def("swap", static_cast<void (wrap_riter_type::*)(wrap_riter_type&)>(&wrap_riter_type::swap))
					
		.def("set_iterator",
				static_cast<wrap_riter_type& (wrap_riter_type::*)(const wrap_riter_type&)>(
					&wrap_riter_type::set_iterator))
		.def("copy", &wrap_riter_type::copy)

		.def("get_value", 
				static_cast
				<
					typename 
						boost::mpl::if_
						<
							boost::is_class<typename wrap_riter_type::value_type>,
							typename wrap_riter_type::reference,
							typename wrap_riter_type::value_type
						>::type (wrap_riter_type::*)(void) const
				>(&wrap_riter_type::get_value))

		.def("head_increment", &wrap_riter_type::head_increment)
		.def("tail_increment", &wrap_riter_type::tail_increment)

		.def("head_decrement", &wrap_riter_type::head_decrement)
		.def("tail_decrement", &wrap_riter_type::tail_decrement)


		.def("compare_eq", static_cast<bool (wrap_riter_type::*)(const wrap_riter_type&) const>(
							&wrap_riter_type::compare_eq))

		.def("next", &wrap_riter_type::next)
		.def("prev", &wrap_riter_type::prev)
	;

	return export_wrap_reverse_iterator_set_value(cobj);
}

template<typename R, typename T, typename X1, typename X2, typename X3> inline
luabind::class_<T, X1, X2, X3>&
	export_wrap_reverse_iterator_construct_other_iterator(luabind::class_<T, X1, X2, X3>& cobj)
{
	typedef T wrap_riter_type;
	typedef R wrap_iter_type;

	cobj.def(luabind::constructor<const wrap_iter_type&>());

	return cobj;
}

template<typename R, typename T, typename X1, typename X2, typename X3> inline
typename
	boost::enable_if
	<
		iterator_ex::is_random_access_iterator<T>,
		luabind::class_<T, X1, X2, X3>&
	>::type
	export_wrap_reverse_iterator_r_other_reverse_iterator(luabind::class_<T, X1, X2, X3>& cobj)
{
	typedef T wrap_riter_type;
	typedef R other_wrap_riter_type;

	cobj
		.def(luabind::const_self < luabind::other<other_wrap_riter_type>())
		.def(luabind::const_self <= luabind::other<other_wrap_riter_type>())
		.def(luabind::const_self == luabind::other<other_wrap_riter_type>())

		.def(luabind::const_self - luabind::other<other_wrap_riter_type>())

		.def("minus", static_cast<typename wrap_riter_type::difference_type (wrap_riter_type::*)(const other_wrap_riter_type&) const>(
						&wrap_riter_type::template minus<other_wrap_riter_type>))

		.def("wraped_minus", 
				static_cast
				<
					typename lua_wrap::wrap_base_t_maker<typename wrap_riter_type::difference_type>::type
						(wrap_riter_type::*)(const other_wrap_riter_type&) const
				>(&wrap_riter_type::template wraped_minus<other_wrap_riter_type>))
					
		.def("compare_eq", static_cast<bool (wrap_riter_type::*)(const other_wrap_riter_type&) const>(
							&wrap_riter_type::template compare_eq<other_wrap_riter_type>))

		.def("compare", static_cast<s32 (wrap_riter_type::*)(const other_wrap_riter_type&) const>(
							&wrap_riter_type::template compare<other_wrap_riter_type>))
	;

	return cobj;
}

template<typename R, typename T, typename X1, typename X2, typename X3> inline
typename
	boost::disable_if
	<
		iterator_ex::is_random_access_iterator<T>,
		luabind::class_<T, X1, X2, X3>&
	>::type
	export_wrap_reverse_iterator_r_other_reverse_iterator(luabind::class_<T, X1, X2, X3>& cobj)
{
	typedef T wrap_riter_type;
	typedef R other_wrap_riter_type;

	cobj
		.def(luabind::const_self == luabind::other<other_wrap_riter_type>())
					
		.def("compare_eq", static_cast<bool (wrap_riter_type::*)(const other_wrap_riter_type&) const>(
							&wrap_riter_type::template compare_eq<other_wrap_riter_type>))
	;

	return cobj;
}


template<typename R, typename T, typename X1, typename X2, typename X3> inline
typename 
	boost::enable_if
	<
		iterator_ex::is_random_access_iterator<T>,
		luabind::class_<T, X1, X2, X3>&
	>::type
	export_wrap_reverse_iterator_r_wrap_base_t_set_value_detail(luabind::class_<T, X1, X2, X3>& cobj)
{
	typedef T wrap_riter_type;
	typedef R wrap_base_type;

	cobj
		.def("set_value",
				static_cast
				< 
					wrap_riter_type& (wrap_riter_type::*)(const wrap_base_type&, const typename wrap_riter_type::value_type&) const 
				>(&wrap_riter_type::template set_value<wrap_base_type>))
	;

	return cobj;
}

template<typename R, typename T, typename X1, typename X2, typename X3> inline
typename 
	boost::disable_if
	<
		iterator_ex::is_random_access_iterator<T>,
		luabind::class_<T, X1, X2, X3>&
	>::type
	export_wrap_reverse_iterator_r_wrap_base_t_set_value_detail(luabind::class_<T, X1, X2, X3>& cobj)
{
	return cobj;
}

template<typename R, typename T, typename X1, typename X2, typename X3> inline
typename 
	boost::disable_if
	<
		iterator_ex::is_const_iterator<T>,
		luabind::class_<T, X1, X2, X3>&
	>::type
	export_wrap_reverse_iterator_r_wrap_base_t_set_value(luabind::class_<T, X1, X2, X3>& cobj)
{
	return export_wrap_reverse_iterator_r_wrap_base_t_set_value_detail<R>(cobj);
}

template<typename R, typename T, typename X1, typename X2, typename X3> inline
typename 
	boost::enable_if
	<
		iterator_ex::is_const_iterator<T>,
		luabind::class_<T, X1, X2, X3>&
	>::type
	export_wrap_reverse_iterator_r_wrap_base_t_set_value(luabind::class_<T, X1, X2, X3>& cobj)
{
	return cobj;
}

template<typename R, typename T, typename X1, typename X2, typename X3> inline
typename 
	boost::enable_if
	<
		iterator_ex::is_random_access_iterator<T>,
		luabind::class_<T, X1, X2, X3>&
	>::type
	export_wrap_reverse_iterator_r_wrap_base_t(luabind::class_<T, X1, X2, X3>& cobj)
{
	typedef T wrap_riter_type;
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
							boost::is_class<typename wrap_riter_type::value_type>,
							typename wrap_riter_type::reference,
							typename wrap_riter_type::value_type
						>::type (wrap_riter_type::*)(const wrap_base_type&) const
				>(&wrap_riter_type::template get_value<wrap_base_type>))

		.def("plus", static_cast<wrap_riter_type (wrap_riter_type::*)(const wrap_base_type&) const>(
						&wrap_riter_type::template plus<wrap_base_type>))

		.def("plus_set", static_cast<wrap_riter_type& (wrap_riter_type::*)(const wrap_base_type&)>(
							&wrap_riter_type::template plus_set<wrap_base_type>))

		.def("minus", static_cast<wrap_riter_type (wrap_riter_type::*)(const wrap_base_type&) const>(
						&wrap_riter_type::template minus<wrap_base_type>))
					
		.def("minus_set", static_cast<wrap_riter_type& (wrap_riter_type::*)(const wrap_base_type&)>(
							&wrap_riter_type::template minus_set<wrap_base_type>))
	;

	return export_wrap_reverse_iterator_r_wrap_base_t_set_value<R>(cobj);
}

template<typename R, typename T, typename X1, typename X2, typename X3> inline
typename 
	boost::disable_if
	<
		iterator_ex::is_random_access_iterator<T>,
		luabind::class_<T, X1, X2, X3>&
	>::type
	export_wrap_reverse_iterator_r_wrap_base_t(luabind::class_<T, X1, X2, X3>& cobj)
{
	return export_wrap_reverse_iterator_r_wrap_base_t_set_value<R>(cobj);
}

} // namespace lua_wrap
} // namespace yggr

#endif // __YGGR_LUA_WRAP_LUA_EXPORT_WRAP_REVERSE_ITERATOR_HPP__
