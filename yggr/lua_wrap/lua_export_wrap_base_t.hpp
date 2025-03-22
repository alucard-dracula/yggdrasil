//lua_export_wrap_base_t.hpp

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

#ifndef __YGGR_LUA_WRAP_LUA_EXPORT_WRAP_BASE_T_HPP__
#define __YGGR_LUA_WRAP_LUA_EXPORT_WRAP_BASE_T_HPP__

#include <yggr/mplex/static_assert.hpp>

#include <yggr/lua_wrap/wrap_base_t.hpp>
#include <yggr/lua_wrap/lua_export_typeid.hpp>

#include <yggr/lua_wrap/wrap_random_access_iterator.hpp>

#include <boost/type_traits/is_same.hpp>

#include <luabind/luabind.hpp>
#include <luabind/lua_include.hpp>
#include <luabind/operator.hpp>

namespace yggr
{
namespace lua_wrap
{

template<typename T, typename X1, typename X2, typename X3> inline
luabind::class_<T, X1, X2, X3>&
	export_wrap_base_t(luabind::class_<T, X1, X2, X3>& cobj)
{
	BOOST_MPL_ASSERT((detail::is_wrap_base_t<T>));
	typedef T wrap_base_type;

	cobj
		.def(luabind::constructor<>())
		.def(luabind::constructor<typename wrap_base_type::value_type>())
		.def(luabind::constructor<const std::string&>())
		.def(luabind::constructor<typename wrap_base_type::half_type, typename wrap_base_type::half_type>())
		.def(luabind::constructor<const wrap_base_type&>())

		.def(luabind::tostring(luabind::const_self))

		.def(-luabind::const_self)
		.def(luabind::const_self + luabind::const_self)
		.def(luabind::const_self - luabind::const_self)
		.def(luabind::const_self * luabind::const_self)
		.def(luabind::const_self / luabind::const_self)
		.def(luabind::const_self <= luabind::const_self)
		.def(luabind::const_self < luabind::const_self)
		.def(luabind::const_self == luabind::const_self)

		.def(double() + luabind::const_self)
		.def(double() - luabind::const_self)
		.def(double() * luabind::const_self)
		.def(double() / luabind::const_self)

		// !!!luabind not support a_type compare b_type, see lua foo luaV_equalval!!!
		//.def(double() == luabind::const_self)
		//.def(double() < luabind::const_self)
		//.def(double() <= luabind::const_self)
				
		.def(luabind::const_self + double())
		.def(luabind::const_self - double())
		.def(luabind::const_self * double())
		.def(luabind::const_self / double())

		// !!!luabind not support a_type compare b_type, see lua foo luaV_equalval!!!
		//.def(luabind::const_self == double())
		//.def(luabind::const_self < double())
		//.def(luabind::const_self <= double())

		.def(bool() + luabind::const_self)
		.def(bool() - luabind::const_self)
		.def(bool() * luabind::const_self)
		.def(bool() / luabind::const_self)

		.def(luabind::const_self + bool())
		.def(luabind::const_self - bool())
		.def(luabind::const_self * bool())
		.def(luabind::const_self / bool())

		.def("swap", static_cast<void (wrap_base_type::*)(wrap_base_type&)>(&wrap_base_type::swap))
		.def("copy", &wrap_base_type::copy)
		.def("set_value",static_cast<wrap_base_type& (wrap_base_type::*)(bool)>(
				&wrap_base_type::template set_value<bool>))
		.def("set_value",static_cast<wrap_base_type& (wrap_base_type::*)(double)>(
				&wrap_base_type::template set_value<double>))
		.def("set_value",static_cast<wrap_base_type& (wrap_base_type::*)(
				const wrap_base_type&)>(&wrap_base_type::set_value))
		.def("get_value", &wrap_base_type::get_value)

		.def("to_boolean", &wrap_base_type::to_boolean)

		.def("positive", &wrap_base_type::positive)
		.def("negate", &wrap_base_type::negate)

		.def("head_increment", &wrap_base_type::head_increment)
		.def("tail_increment", &wrap_base_type::tail_increment)

		.def("head_decrement", &wrap_base_type::head_decrement)
		.def("tail_decrement", &wrap_base_type::tail_decrement)

		.def("plus", static_cast<wrap_base_type (wrap_base_type::*)(bool) const>(&wrap_base_type::template plus<bool>))
		.def("plus", static_cast<wrap_base_type (wrap_base_type::*)(double) const>(&wrap_base_type::template plus<double>))
		.def("plus", static_cast<wrap_base_type (wrap_base_type::*)(const wrap_base_type&) const>(&wrap_base_type::plus))

		.def("plus_set", static_cast<wrap_base_type& (wrap_base_type::*)(bool)>(&wrap_base_type::template plus_set<bool>))
		.def("plus_set", static_cast<wrap_base_type& (wrap_base_type::*)(double)>(&wrap_base_type::template plus_set<double>))
		.def("plus_set", 
				static_cast<wrap_base_type& (wrap_base_type::*)(const wrap_base_type&)>(
					&wrap_base_type::template plus_set<wrap_base_type>))

		.def("minus", static_cast<wrap_base_type (wrap_base_type::*)(bool) const>(&wrap_base_type::template minus<bool>))
		.def("minus", static_cast<wrap_base_type (wrap_base_type::*)(double) const>(&wrap_base_type::template minus<double>))
		.def("minus", static_cast<wrap_base_type (wrap_base_type::*)(const wrap_base_type&) const>(&wrap_base_type::minus))

		.def("minus_set", static_cast<wrap_base_type& (wrap_base_type::*)(bool)>(&wrap_base_type::template minus_set<bool>))
		.def("minus_set", static_cast<wrap_base_type& (wrap_base_type::*)(double)>(&wrap_base_type::template minus_set<double>))
		.def("minus_set", 
				static_cast<wrap_base_type& (wrap_base_type::*)(const wrap_base_type&)>(
					&wrap_base_type::template minus_set<wrap_base_type>))
				
		.def("multiplies", static_cast<wrap_base_type (wrap_base_type::*)(bool) const>(&wrap_base_type::template multiplies<bool>))
		.def("multiplies", static_cast<wrap_base_type (wrap_base_type::*)(double) const>(&wrap_base_type::template multiplies<double>))
		.def("multiplies", static_cast<wrap_base_type (wrap_base_type::*)(const wrap_base_type&) const>(&wrap_base_type::multiplies))

		.def("multiplies_set", static_cast<wrap_base_type& (wrap_base_type::*)(bool)>(&wrap_base_type::template multiplies_set<bool>))
		.def("multiplies_set", static_cast<wrap_base_type& (wrap_base_type::*)(double)>(&wrap_base_type::template multiplies_set<double>))
		.def("multiplies_set", 
				static_cast<wrap_base_type& (wrap_base_type::*)(const wrap_base_type&)>(
					&wrap_base_type::template multiplies_set<wrap_base_type>))

		.def("divides", static_cast<wrap_base_type (wrap_base_type::*)(bool) const>(&wrap_base_type::template divides<bool>))
		.def("divides", static_cast<wrap_base_type (wrap_base_type::*)(double) const>(&wrap_base_type::template divides<double>))
		.def("divides", static_cast<wrap_base_type (wrap_base_type::*)(const wrap_base_type&) const>(&wrap_base_type::divides))

		.def("divides_set", static_cast<wrap_base_type& (wrap_base_type::*)(bool)>(&wrap_base_type::template divides_set<bool>))
		.def("divides_set", static_cast<wrap_base_type& (wrap_base_type::*)(double)>(&wrap_base_type::template divides_set<double>))
		.def("divides_set", 
				static_cast<wrap_base_type& (wrap_base_type::*)(const wrap_base_type&)>(
					&wrap_base_type::template divides_set<wrap_base_type>))

		.def("modulus", static_cast<wrap_base_type (wrap_base_type::*)(bool) const>(&wrap_base_type::template modulus<bool>))
		.def("modulus", static_cast<wrap_base_type (wrap_base_type::*)(double) const>(&wrap_base_type::template modulus<double>))
		.def("modulus", static_cast<wrap_base_type (wrap_base_type::*)(const wrap_base_type&) const>(&wrap_base_type::modulus))

		.def("modulus_set", static_cast<wrap_base_type& (wrap_base_type::*)(bool)>(&wrap_base_type::template modulus_set<bool>))
		.def("modulus_set", static_cast<wrap_base_type& (wrap_base_type::*)(double)>(&wrap_base_type::template modulus_set<double>))
		.def("modulus_set", 
				static_cast<wrap_base_type& (wrap_base_type::*)(const wrap_base_type&)>(
					&wrap_base_type::template modulus_set<wrap_base_type>))

		.def("logic_and", static_cast<bool (wrap_base_type::*)(bool) const>(&wrap_base_type::template logic_and<bool>))
		.def("logic_and", static_cast<bool (wrap_base_type::*)(double) const>(&wrap_base_type::template logic_and<double>))
		.def("logic_and", static_cast<bool (wrap_base_type::*)(const wrap_base_type&) const>(&wrap_base_type::logic_and))

		.def("logic_or", static_cast<bool (wrap_base_type::*)(bool) const>(&wrap_base_type::template logic_or<bool>))
		.def("logic_or", static_cast<bool (wrap_base_type::*)(double) const>(&wrap_base_type::template logic_or<double>))
		.def("logic_or", static_cast<bool (wrap_base_type::*)(const wrap_base_type&) const>(&wrap_base_type::logic_or))

		.def("logic_not", &wrap_base_type::logic_not)

		.def("compare_eq", static_cast<bool (wrap_base_type::*)(bool) const>(&wrap_base_type::template compare_eq<bool>))
		.def("compare_eq", static_cast<bool (wrap_base_type::*)(double) const>(&wrap_base_type::template compare_eq<double>))
		.def("compare_eq", static_cast<bool (wrap_base_type::*)(const wrap_base_type&) const>(&wrap_base_type::compare_eq))

		.def("compare", static_cast<s32 (wrap_base_type::*)(bool) const>(&wrap_base_type::template compare<bool>))
		.def("compare", static_cast<s32 (wrap_base_type::*)(double) const>(&wrap_base_type::template compare<double>))
		.def("compare", static_cast<s32 (wrap_base_type::*)(const wrap_base_type&) const>(&wrap_base_type::compare))

		.def("bitwise_left_shift", static_cast<wrap_base_type (wrap_base_type::*)(bool) const>(&wrap_base_type::template bitwise_left_shift<bool>))
		.def("bitwise_left_shift", static_cast<wrap_base_type (wrap_base_type::*)(double) const>(&wrap_base_type::template bitwise_left_shift<double>))
		.def("bitwise_left_shift", static_cast<wrap_base_type (wrap_base_type::*)(const wrap_base_type&) const>(&wrap_base_type::bitwise_left_shift))

		.def("bitwise_left_shift_set", static_cast<wrap_base_type& (wrap_base_type::*)(bool)>(&wrap_base_type::template bitwise_left_shift_set<bool>))
		.def("bitwise_left_shift_set", static_cast<wrap_base_type& (wrap_base_type::*)(double)>(&wrap_base_type::template bitwise_left_shift_set<double>))
		.def("bitwise_left_shift_set", 
				static_cast<wrap_base_type& (wrap_base_type::*)(const wrap_base_type&)>(
					&wrap_base_type::template bitwise_left_shift_set<wrap_base_type>))

		.def("bitwise_right_shift", static_cast<wrap_base_type (wrap_base_type::*)(bool) const>(&wrap_base_type::template bitwise_right_shift<bool>))
		.def("bitwise_right_shift", static_cast<wrap_base_type (wrap_base_type::*)(double) const>(&wrap_base_type::template bitwise_right_shift<double>))
		.def("bitwise_right_shift", static_cast<wrap_base_type (wrap_base_type::*)(const wrap_base_type&) const>(&wrap_base_type::template bitwise_right_shift))

		.def("bitwise_right_shift_set", static_cast<wrap_base_type& (wrap_base_type::*)(bool)>(&wrap_base_type::template bitwise_right_shift_set<bool>))
		.def("bitwise_right_shift_set", static_cast<wrap_base_type& (wrap_base_type::*)(double)>(&wrap_base_type::template bitwise_right_shift_set<double>))
		.def("bitwise_right_shift_set", 
				static_cast<wrap_base_type& (wrap_base_type::*)(const wrap_base_type&)>(
					&wrap_base_type::template bitwise_right_shift_set<wrap_base_type>))

		.def("bitwise_not", &wrap_base_type::bitwise_not)

		.def("bitwise_and", static_cast<wrap_base_type (wrap_base_type::*)(bool) const>(&wrap_base_type::template bitwise_and<bool>))
		.def("bitwise_and", static_cast<wrap_base_type (wrap_base_type::*)(double) const>(&wrap_base_type::template bitwise_and<double>))
		.def("bitwise_and", static_cast<wrap_base_type (wrap_base_type::*)(const wrap_base_type&) const>(&wrap_base_type::bitwise_and))

		.def("bitwise_and_set", static_cast<wrap_base_type& (wrap_base_type::*)(bool)>(&wrap_base_type::template bitwise_and_set<bool>))
		.def("bitwise_and_set", static_cast<wrap_base_type& (wrap_base_type::*)(double)>(&wrap_base_type::template bitwise_and_set<double>))
		.def("bitwise_and_set", 
				static_cast<wrap_base_type& (wrap_base_type::*)(const wrap_base_type&)>(
					&wrap_base_type::template bitwise_and_set<wrap_base_type>))

		.def("bitwise_or", static_cast<wrap_base_type (wrap_base_type::*)(bool) const>(&wrap_base_type::template bitwise_or<bool>))
		.def("bitwise_or", static_cast<wrap_base_type (wrap_base_type::*)(double) const>(&wrap_base_type::template bitwise_or<double>))
		.def("bitwise_or", static_cast<wrap_base_type (wrap_base_type::*)(const wrap_base_type&) const>(&wrap_base_type::bitwise_or))

		.def("bitwise_or_set", static_cast<wrap_base_type& (wrap_base_type::*)(bool)>(&wrap_base_type::template bitwise_or_set<bool>))
		.def("bitwise_or_set", static_cast<wrap_base_type& (wrap_base_type::*)(double)>(&wrap_base_type::template bitwise_or_set<double>))
		.def("bitwise_or_set", 
				static_cast<wrap_base_type& (wrap_base_type::*)(const wrap_base_type&)>(
					&wrap_base_type::template bitwise_or_set<wrap_base_type>))

		.def("bitwise_xor", static_cast<wrap_base_type (wrap_base_type::*)(bool) const>(&wrap_base_type::template bitwise_xor<bool>))
		.def("bitwise_xor", static_cast<wrap_base_type (wrap_base_type::*)(double) const>(&wrap_base_type::template bitwise_xor<double>))
		.def("bitwise_xor", static_cast<wrap_base_type (wrap_base_type::*)(const wrap_base_type&) const>(&wrap_base_type::bitwise_xor))

		.def("bitwise_xor_set", static_cast<wrap_base_type& (wrap_base_type::*)(bool)>(&wrap_base_type::template bitwise_xor_set<bool>))
		.def("bitwise_xor_set", static_cast<wrap_base_type& (wrap_base_type::*)(double)>(&wrap_base_type::template bitwise_xor_set<double>))
		.def("bitwise_xor_set", 
				static_cast<wrap_base_type& (wrap_base_type::*)(const wrap_base_type&)>(
					&wrap_base_type::template bitwise_xor_set<wrap_base_type>))
	;

	return cobj;
}

template<typename OT, typename T, typename X1, typename X2, typename X3> inline
luabind::class_<T, X1, X2, X3>&
	export_wrap_base_t_other(luabind::class_<T, X1, X2, X3>& cobj)
{
	BOOST_MPL_ASSERT((detail::is_wrap_base_t<T>));
	BOOST_MPL_ASSERT((detail::is_wrap_base_t<OT>));

	typedef T wrap_base_type;
	typedef OT other_wrap_base_type;

	typedef typename wrap_base_type::value_type value_type;
	typedef typename other_wrap_base_type::value_type other_value_type;

	cobj
		.def(luabind::constructor<const other_wrap_base_type&>())
		
		.def(luabind::const_self + luabind::other<other_wrap_base_type>())
		.def(luabind::const_self - luabind::other<other_wrap_base_type>())
		.def(luabind::const_self * luabind::other<other_wrap_base_type>())
		.def(luabind::const_self / luabind::other<other_wrap_base_type>())
		.def(luabind::const_self <= luabind::other<other_wrap_base_type>())
		.def(luabind::const_self < luabind::other<other_wrap_base_type>())
		.def(luabind::const_self == luabind::other<other_wrap_base_type>())

		.def("swap", &wrap_base_type::template swap<other_value_type>)

		.def("set_value",static_cast<wrap_base_type& (wrap_base_type::*)(
				const other_wrap_base_type&)>(&wrap_base_type::template set_value<other_wrap_base_type>))
		
		.def("plus",
				static_cast<
					typename wrap_base_t_maker< typename plus_operator_result< value_type, other_value_type >::type >::type 
						(wrap_base_type::*)(const other_wrap_base_type&) const>(&wrap_base_type::template plus<other_wrap_base_type>))
		
		.def("plus_set", 
				static_cast<wrap_base_type& (wrap_base_type::*)(const other_wrap_base_type&)>(
					&wrap_base_type::template plus_set<other_wrap_base_type>))
		
		.def("minus",
				static_cast<
					typename wrap_base_t_maker< typename minus_operator_result< value_type, other_value_type >::type >::type 
						(wrap_base_type::*)(const other_wrap_base_type&) const>(&wrap_base_type::template minus<other_wrap_base_type>))
		
		.def("minus_set", 
				static_cast<wrap_base_type& (wrap_base_type::*)(const other_wrap_base_type&)>(
					&wrap_base_type::template minus_set<other_wrap_base_type>))
		
		.def("multiplies",
				static_cast<
					typename wrap_base_t_maker< typename multiplies_operator_result< value_type, other_value_type >::type >::type 
						(wrap_base_type::*)(const other_wrap_base_type&) const>(&wrap_base_type::template multiplies<other_wrap_base_type>))
		
		.def("multiplies_set", 
				static_cast<wrap_base_type& (wrap_base_type::*)(const other_wrap_base_type&)>(
					&wrap_base_type::template multiplies_set<other_wrap_base_type>))
		
		.def("divides",
				static_cast<
					typename wrap_base_t_maker< typename divides_operator_result< value_type, other_value_type >::type >::type 
						(wrap_base_type::*)(const other_wrap_base_type&) const>(&wrap_base_type::template divides<other_wrap_base_type>))
		
		.def("divides_set", 
				static_cast<wrap_base_type& (wrap_base_type::*)(const other_wrap_base_type&)>(
					&wrap_base_type::template divides_set<other_wrap_base_type>))
		
		.def("modulus",
				static_cast<
					typename wrap_base_t_maker< typename modulus_operator_result< value_type, other_value_type >::type >::type 
						(wrap_base_type::*)(const other_wrap_base_type&) const>(&wrap_base_type::template modulus<other_wrap_base_type>))
		
		.def("modulus_set", 
				static_cast<wrap_base_type& (wrap_base_type::*)(const other_wrap_base_type&)>(
					&wrap_base_type::template modulus_set<other_wrap_base_type>))
		
		.def("logic_and",
				static_cast<
					bool (wrap_base_type::*)(const other_wrap_base_type&) const>(&wrap_base_type::template logic_and<other_wrap_base_type>))
		
		.def("logic_or",
				static_cast<
					bool (wrap_base_type::*)(const other_wrap_base_type&) const>(&wrap_base_type::template logic_or<other_wrap_base_type>))
		
		.def("compare_eq",
				static_cast<
					bool (wrap_base_type::*)(const other_wrap_base_type&) const>(&wrap_base_type::template compare_eq<other_wrap_base_type>))
		
		.def("compare",
				static_cast<
					s32 (wrap_base_type::*)(const other_wrap_base_type&) const>(&wrap_base_type::template compare<other_wrap_base_type>))
		
		.def("bitwise_left_shift",
				static_cast<
					typename wrap_base_t_maker< typename bitwise_left_shift_operator_result< value_type, other_value_type >::type >::type 
						(wrap_base_type::*)(const other_wrap_base_type&) const>(
							&wrap_base_type::template bitwise_left_shift<other_wrap_base_type>))
		
		.def("bitwise_left_shift_set", 
				static_cast<wrap_base_type& (wrap_base_type::*)(const other_wrap_base_type&)>(
					&wrap_base_type::template bitwise_left_shift_set<other_wrap_base_type>))
		
		.def("bitwise_right_shift",
				static_cast<
					typename wrap_base_t_maker< typename bitwise_right_shift_operator_result< value_type, other_value_type >::type >::type 
						(wrap_base_type::*)(const other_wrap_base_type&) const>(
							&wrap_base_type::template bitwise_right_shift<other_wrap_base_type>))
		
		.def("bitwise_right_shift_set", 
				static_cast<wrap_base_type& (wrap_base_type::*)(const other_wrap_base_type&)>(
					&wrap_base_type::template bitwise_right_shift_set<other_wrap_base_type>))
		
		.def("bitwise_and",
				static_cast<
					typename wrap_base_t_maker< typename bitwise_and_operator_result< value_type, other_value_type >::type >::type 
						(wrap_base_type::*)(const other_wrap_base_type&) const>(
							&wrap_base_type::template bitwise_and<other_wrap_base_type>))
	
		.def("bitwise_and_set", 
				static_cast<wrap_base_type& (wrap_base_type::*)(const other_wrap_base_type&)>(
					&wrap_base_type::template bitwise_and_set<other_wrap_base_type>))
		
		.def("bitwise_or",
				static_cast<
					typename wrap_base_t_maker< typename bitwise_or_operator_result< value_type, other_value_type >::type >::type 
						(wrap_base_type::*)(const other_wrap_base_type&) const>(
							&wrap_base_type::template bitwise_or<other_wrap_base_type>))
		
		.def("bitwise_or_set", 
				static_cast<wrap_base_type& (wrap_base_type::*)(const other_wrap_base_type&)>(
					&wrap_base_type::template bitwise_or_set<other_wrap_base_type>))
		
		.def("bitwise_xor",
				static_cast<
					typename wrap_base_t_maker< typename bitwise_xor_operator_result< value_type, other_value_type >::type >::type 
						(wrap_base_type::*)(const other_wrap_base_type&) const>(
							&wrap_base_type::template bitwise_xor<other_wrap_base_type>))
		
		.def("bitwise_xor_set", 
				static_cast<wrap_base_type& (wrap_base_type::*)(const other_wrap_base_type&)>(
					&wrap_base_type::template bitwise_xor_set<other_wrap_base_type>))
	;
	return cobj;
}

} // namespace lua_wrap
} // namespace yggr

#endif //__YGGR_LUA_WRAP_LUA_EXPORT_WRAP_BASE_T_HPP__