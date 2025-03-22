//lua_export_wrap_list.hpp

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


#ifndef __YGGR_LUA_WRAP_LUA_EXPORT_WRAP_LIST_HPP__
#define __YGGR_LUA_WRAP_LUA_EXPORT_WRAP_LIST_HPP__

#include <yggr/mplex/static_assert.hpp>

#include <yggr/lua_wrap/wrap_base_t.hpp>
#include <yggr/lua_wrap/lua_export_typeid.hpp>
#include <yggr/lua_wrap/wrap_list.hpp>

#include <boost/type_traits/is_same.hpp>

#include <luabind/luabind.hpp>
#include <luabind/lua_include.hpp>
#include <luabind/operator.hpp>

namespace yggr
{
namespace lua_wrap
{

template<typename R, typename T, typename X1, typename X2, typename X3> inline
luabind::class_<T, X1, X2, X3>&
	export_wrap_list_r_wrap_base_t(luabind::class_<T, X1, X2, X3>& cobj)
{
	BOOST_MPL_ASSERT((detail::is_wrap_list<T>));
	typedef T wrap_list_type;
	typedef R wrap_base_type;

	typedef typename wrap_list_type::value_type value_type;
	
	typedef typename
		boost::mpl::if_
		<
			boost::is_class<value_type>,
			value_type&,
			value_type
		>::type return_value_ref_type;

	typedef typename
		boost::mpl::if_
		<
			boost::is_class<value_type>,
			const value_type&,
			value_type
		>::type return_value_cref_type;

	typedef typename wrap_list_type::wrap_allocator_type wrap_allocator_type;
	typedef typename wrap_list_type::wrap_iterator_type wrap_iterator_type;
	typedef typename wrap_list_type::wrap_const_iterator_type wrap_const_iterator_type;

	typedef R other_iterator_type;

	cobj
		.def(luabind::constructor<const wrap_base_type&, const value_type&>())
		.def(luabind::constructor<const wrap_base_type&, const value_type&, const wrap_allocator_type&>())

		.def(luabind::constructor<const wrap_base_type&>())
		.def(luabind::constructor<const wrap_base_type&, const wrap_allocator_type&>())

		.def("assign", 
				static_cast<void (wrap_list_type::*)(const wrap_base_type&, const value_type&)>(&wrap_list_type::assign))

		.def("insert", 
				static_cast<wrap_iterator_type (wrap_list_type::*)(
					wrap_iterator_type, const wrap_base_type&, const value_type&)>(
						&wrap_list_type::template wraped_insert<wrap_iterator_type, wrap_base_type>))
		.def("insert", 
				static_cast<wrap_iterator_type (wrap_list_type::*)(
					wrap_const_iterator_type, const wrap_base_type&, const value_type&)>(
						&wrap_list_type::template wraped_insert<wrap_const_iterator_type, wrap_base_type>))

		.def("resize", static_cast<void (wrap_list_type::*)(const wrap_base_type&)>(&wrap_list_type::template resize<wrap_base_type>))
		.def("resize", static_cast<void (wrap_list_type::*)(const wrap_base_type&, const value_type&)>(&wrap_list_type::template resize<wrap_base_type>))
	;

	return cobj;
}


template<typename R, typename T, typename X1, typename X2, typename X3> inline
luabind::class_<T, X1, X2, X3>&
	export_wrap_list_r_other_iterator(luabind::class_<T, X1, X2, X3>& cobj)
{
	BOOST_MPL_ASSERT((detail::is_wrap_list<T>));
	typedef T wrap_list_type;
	typedef typename wrap_list_type::wrap_allocator_type wrap_allocator_type;
	typedef typename wrap_list_type::wrap_iterator_type wrap_iterator_type;
	typedef typename wrap_list_type::wrap_const_iterator_type wrap_const_iterator_type;

	typedef R other_iterator_type;

	cobj
		.def(luabind::constructor<other_iterator_type, other_iterator_type>())
		.def(luabind::constructor<other_iterator_type, other_iterator_type, const wrap_allocator_type& >())

		.def("assign", static_cast<void (wrap_list_type::*)(other_iterator_type, other_iterator_type)>(&wrap_list_type::assign))

		.def("insert", 
				static_cast<wrap_iterator_type (wrap_list_type::*)(
					wrap_iterator_type, other_iterator_type, other_iterator_type)>(
						&wrap_list_type::template wraped_insert<wrap_iterator_type, other_iterator_type>))
		.def("insert", 
				static_cast<wrap_iterator_type (wrap_list_type::*)(
					wrap_const_iterator_type, other_iterator_type, other_iterator_type)>(
						&wrap_list_type::template wraped_insert<wrap_const_iterator_type, other_iterator_type>))
	;

	return cobj;
}

template<typename T, typename X1, typename X2, typename X3> inline
luabind::class_<T, X1, X2, X3>&
	export_wrap_list(luabind::class_<T, X1, X2, X3>& cobj)
{
	BOOST_MPL_ASSERT((detail::is_wrap_list<T>));
	typedef T wrap_list_type;

	typedef typename wrap_list_type::value_type value_type;

	typedef typename
		boost::mpl::if_
		<
			boost::is_class<value_type>,
			value_type&,
			value_type
		>::type return_value_ref_type;

	typedef typename
		boost::mpl::if_
		<
			boost::is_class<value_type>,
			const value_type&,
			value_type
		>::type return_value_cref_type;

	typedef typename wrap_list_type::size_type size_type;
	typedef typename wrap_list_type::wrap_size_type wrap_size_type;

	typedef typename wrap_list_type::wrap_allocator_type wrap_allocator_type;
	typedef typename wrap_list_type::wrap_iterator_type wrap_iterator_type;
	typedef typename wrap_list_type::wrap_const_iterator_type wrap_const_iterator_type;
	typedef typename wrap_list_type::wrap_reverse_iterator_type wrap_reverse_iterator_type;
	typedef typename wrap_list_type::wrap_const_reverse_iterator_type wrap_const_reverse_iterator_type;

	cobj
		.def(luabind::constructor<>())
		.def(luabind::constructor<const wrap_allocator_type&>())

		.def(luabind::constructor<size_type, const value_type&>())
		.def(luabind::constructor<size_type, const value_type&, const wrap_allocator_type&>())

		.def(luabind::constructor<size_type>())
		.def(luabind::constructor<size_type, const wrap_allocator_type&>())

		.def(luabind::constructor<wrap_iterator_type, wrap_iterator_type>())
		.def(luabind::constructor<wrap_const_iterator_type, wrap_const_iterator_type>())
		.def(luabind::constructor<wrap_reverse_iterator_type, wrap_reverse_iterator_type>())
		.def(luabind::constructor<wrap_const_reverse_iterator_type, wrap_const_reverse_iterator_type>())

		.def(luabind::constructor<wrap_iterator_type, wrap_iterator_type, const wrap_allocator_type& >())
		.def(luabind::constructor<wrap_const_iterator_type, wrap_const_iterator_type, const wrap_allocator_type&>())
		.def(luabind::constructor<wrap_reverse_iterator_type, wrap_reverse_iterator_type, const wrap_allocator_type&>())
		.def(luabind::constructor<wrap_const_reverse_iterator_type, wrap_const_reverse_iterator_type, const wrap_allocator_type&>())

		.def(luabind::constructor<const wrap_list_type&>())
		.def(luabind::constructor<const wrap_list_type&, const wrap_allocator_type&>())

		.def(luabind::tostring(luabind::const_self))

		.def(luabind::const_self <= luabind::const_self)
		.def(luabind::const_self < luabind::const_self)
		.def(luabind::const_self == luabind::const_self)

		.def("swap", static_cast<void (wrap_list_type::*)(wrap_list_type&)>(&wrap_list_type::swap))

		.def("copy", &wrap_list_type::copy)

		.def("assign", static_cast<void (wrap_list_type::*)(size_type, const value_type&)>(&wrap_list_type::assign))
		.def("assign", static_cast<void (wrap_list_type::*)(wrap_iterator_type, wrap_iterator_type)>(&wrap_list_type::assign))
		.def("assign", static_cast<void (wrap_list_type::*)(wrap_const_iterator_type, wrap_const_iterator_type)>(&wrap_list_type::assign))
		.def("assign", static_cast<void (wrap_list_type::*)(wrap_reverse_iterator_type, wrap_reverse_iterator_type)>(&wrap_list_type::assign))
		.def("assign", static_cast<void (wrap_list_type::*)(wrap_const_reverse_iterator_type, wrap_const_reverse_iterator_type)>(&wrap_list_type::assign))

		.def("get_allocator", &wrap_list_type::get_wraped_allocator)

		.def("front", static_cast<return_value_ref_type (wrap_list_type::*)(void)>(&wrap_list_type::wraped_front))
		.def("front", static_cast<return_value_cref_type (wrap_list_type::*)(void) const>(&wrap_list_type::front))

		.def("back", static_cast<return_value_ref_type (wrap_list_type::*)(void)>(&wrap_list_type::wraped_back))
		.def("back", static_cast<return_value_cref_type (wrap_list_type::*)(void) const>(&wrap_list_type::back))

		.def("set_front", &wrap_list_type::set_front)
		.def("set_back", &wrap_list_type::set_back)

		.def("begin_", static_cast<wrap_iterator_type (wrap_list_type::*)(void)>(&wrap_list_type::wraped_begin))
		.def("begin_", static_cast<wrap_const_iterator_type (wrap_list_type::*)(void) const>(&wrap_list_type::wraped_begin))
		.def("cbegin_", static_cast<wrap_const_iterator_type (wrap_list_type::*)(void) const>(&wrap_list_type::wraped_cbegin))

		.def("end_", static_cast<wrap_iterator_type (wrap_list_type::*)(void)>(&wrap_list_type::wraped_end))
		.def("end_", static_cast<wrap_const_iterator_type (wrap_list_type::*)(void) const>(&wrap_list_type::wraped_end))
		.def("cend_", static_cast<wrap_const_iterator_type (wrap_list_type::*)(void) const>(&wrap_list_type::wraped_cend))

		.def("rbegin_", static_cast<wrap_reverse_iterator_type (wrap_list_type::*)(void)>(&wrap_list_type::wraped_rbegin))
		.def("rbegin_", static_cast<wrap_const_reverse_iterator_type (wrap_list_type::*)(void) const>(&wrap_list_type::wraped_rbegin))
		.def("crbegin_", static_cast<wrap_const_reverse_iterator_type (wrap_list_type::*)(void) const>(&wrap_list_type::wraped_crbegin))

		.def("rend_", static_cast<wrap_reverse_iterator_type (wrap_list_type::*)(void)>(&wrap_list_type::wraped_rend))
		.def("rend_", static_cast<wrap_const_reverse_iterator_type (wrap_list_type::*)(void) const>(&wrap_list_type::wraped_rend))
		.def("crend_", static_cast<wrap_const_reverse_iterator_type (wrap_list_type::*)(void) const>(&wrap_list_type::wraped_crend))

		.def("empty", &wrap_list_type::empty)
		.def("size", &wrap_list_type::size)
		.def("wraped_size", &wrap_list_type::wraped_size)
		.def("max_size", &wrap_list_type::max_size)
		.def("wraped_max_size", &wrap_list_type::wraped_max_size)

		.def("clear", &wrap_list_type::clear)

		.def("insert", 
				static_cast<wrap_iterator_type (wrap_list_type::*)(
					wrap_iterator_type, const value_type&)>(&wrap_list_type::template wraped_insert<wrap_iterator_type>))
		.def("insert", 
				static_cast<wrap_iterator_type (wrap_list_type::*)(
					wrap_const_iterator_type, const value_type&)>(&wrap_list_type::template wraped_insert<wrap_const_iterator_type>))

		.def("insert", 
				static_cast<wrap_iterator_type (wrap_list_type::*)(
					wrap_iterator_type, double, const value_type&)>(
						&wrap_list_type::template wraped_insert<wrap_iterator_type, double>))
		.def("insert", 
				static_cast<wrap_iterator_type (wrap_list_type::*)(
					wrap_const_iterator_type, double, const value_type&)>(
						&wrap_list_type::template wraped_insert<wrap_const_iterator_type, double>))

		.def("insert", 
				static_cast<wrap_iterator_type (wrap_list_type::*)(
					wrap_iterator_type, wrap_iterator_type, wrap_iterator_type)>(
						&wrap_list_type::template wraped_insert<wrap_iterator_type, wrap_iterator_type>))
		.def("insert", 
				static_cast<wrap_iterator_type (wrap_list_type::*)(
					wrap_iterator_type, wrap_const_iterator_type, wrap_const_iterator_type)>(
						&wrap_list_type::template wraped_insert<wrap_iterator_type, wrap_const_iterator_type>))
		.def("insert", 
				static_cast<wrap_iterator_type (wrap_list_type::*)(
					wrap_iterator_type, wrap_reverse_iterator_type, wrap_reverse_iterator_type)>(
						&wrap_list_type::template wraped_insert<wrap_iterator_type, wrap_reverse_iterator_type>))
		.def("insert", 
				static_cast<wrap_iterator_type (wrap_list_type::*)(
					wrap_iterator_type, wrap_const_reverse_iterator_type, wrap_const_reverse_iterator_type)>(
						&wrap_list_type::template wraped_insert<wrap_iterator_type, wrap_const_reverse_iterator_type>))

		.def("insert", 
				static_cast<wrap_iterator_type (wrap_list_type::*)(
					wrap_const_iterator_type, wrap_iterator_type, wrap_iterator_type)>(
						&wrap_list_type::template wraped_insert<wrap_const_iterator_type, wrap_iterator_type>))
		.def("insert", 
				static_cast<wrap_iterator_type (wrap_list_type::*)(
					wrap_const_iterator_type, wrap_const_iterator_type, wrap_const_iterator_type)>(
						&wrap_list_type::template wraped_insert<wrap_const_iterator_type, wrap_const_iterator_type>))
		.def("insert", 
				static_cast<wrap_iterator_type (wrap_list_type::*)(
					wrap_const_iterator_type, wrap_reverse_iterator_type, wrap_reverse_iterator_type)>(
						&wrap_list_type::template wraped_insert<wrap_const_iterator_type, wrap_reverse_iterator_type>))
		.def("insert", 
				static_cast<wrap_iterator_type (wrap_list_type::*)(
					wrap_const_iterator_type, wrap_const_reverse_iterator_type, wrap_const_reverse_iterator_type)>(
						&wrap_list_type::template wraped_insert<wrap_const_iterator_type, wrap_const_reverse_iterator_type>))

		.def("erase", 
				static_cast<wrap_iterator_type (wrap_list_type::*)(wrap_iterator_type)>(
					&wrap_list_type::template wraped_erase<wrap_iterator_type>))
		.def("erase", 
				static_cast<wrap_iterator_type (wrap_list_type::*)(wrap_const_iterator_type)>(
					&wrap_list_type::template wraped_erase<wrap_const_iterator_type>))

		.def("erase", 
				static_cast<wrap_iterator_type (wrap_list_type::*)(wrap_iterator_type, wrap_iterator_type)>(
					&wrap_list_type::template wraped_erase<wrap_iterator_type, wrap_iterator_type>))
		.def("erase", 
				static_cast<wrap_iterator_type (wrap_list_type::*)(wrap_iterator_type, wrap_const_iterator_type)>(
					&wrap_list_type::template wraped_erase<wrap_iterator_type, wrap_const_iterator_type>))
		.def("erase", 
				static_cast<wrap_iterator_type (wrap_list_type::*)(wrap_const_iterator_type, wrap_iterator_type)>(
					&wrap_list_type::template wraped_erase<wrap_const_iterator_type, wrap_iterator_type>))
		.def("erase", 
				static_cast<wrap_iterator_type (wrap_list_type::*)(wrap_const_iterator_type, wrap_const_iterator_type)>(
					&wrap_list_type::template wraped_erase<wrap_const_iterator_type, wrap_const_iterator_type>))

		.def("push_back", static_cast<void (wrap_list_type::*)(const value_type&)>(&wrap_list_type::push_back))
		.def("pop_back", &wrap_list_type::pop_back)

		.def("push_front", static_cast<void (wrap_list_type::*)(const value_type&)>(&wrap_list_type::push_front))
		.def("pop_front", &wrap_list_type::pop_front)

		.def("resize", static_cast<void (wrap_list_type::*)(double)>(&wrap_list_type::template resize<double>))
		.def("resize", static_cast<void (wrap_list_type::*)(double, const value_type&)>(&wrap_list_type::template resize<double>))

		.def("merge", static_cast<void (wrap_list_type::*)(wrap_list_type&)>(&wrap_list_type::merge))
		.def("merge_equal_to", &wrap_list_type::merge_equal_to)
		.def("merge_not_equal_to", &wrap_list_type::merge_not_equal_to)
		.def("merge_less", &wrap_list_type::merge_less)
		.def("merge_less_equal", &wrap_list_type::merge_less_equal)
		.def("merge_greater", &wrap_list_type::merge_greater)
		.def("merge_greater_equal", &wrap_list_type::merge_greater_equal)

		.def("splice", 
				static_cast<
					void (wrap_list_type::*)(
						wrap_iterator_type, wrap_list_type&)>(
					&wrap_list_type::template splice<wrap_iterator_type>))
		.def("splice", 
				static_cast<
					void (wrap_list_type::*)(
						wrap_const_iterator_type, wrap_list_type&)>(
					&wrap_list_type::template splice<wrap_const_iterator_type>))

		.def("splice", 
				static_cast<
					void (wrap_list_type::*)(
						wrap_iterator_type, wrap_list_type&, wrap_iterator_type)>(
					&wrap_list_type::template splice<wrap_iterator_type, wrap_iterator_type>))
		.def("splice", 
				static_cast<
					void (wrap_list_type::*)(
						wrap_iterator_type, wrap_list_type&, wrap_const_iterator_type)>(
					&wrap_list_type::template splice<wrap_iterator_type, wrap_const_iterator_type>))
		.def("splice", 
				static_cast<
					void (wrap_list_type::*)(
						wrap_const_iterator_type, wrap_list_type&, wrap_iterator_type)>(
					&wrap_list_type::template splice<wrap_const_iterator_type, wrap_iterator_type>))
		.def("splice", 
				static_cast<
					void (wrap_list_type::*)(
						wrap_const_iterator_type, wrap_list_type&, wrap_const_iterator_type)>(
					&wrap_list_type::template splice<wrap_const_iterator_type, wrap_const_iterator_type>))

		.def("splice", 
				static_cast<
					void (wrap_list_type::*)(
						wrap_iterator_type, wrap_list_type&, wrap_iterator_type, wrap_iterator_type)>(
					&wrap_list_type::template splice<wrap_iterator_type, wrap_iterator_type, wrap_iterator_type>))
		.def("splice", 
				static_cast<
					void (wrap_list_type::*)(
						wrap_iterator_type, wrap_list_type&, wrap_iterator_type, wrap_const_iterator_type)>(
					&wrap_list_type::template splice<wrap_iterator_type, wrap_iterator_type, wrap_const_iterator_type>))
		.def("splice", 
				static_cast<
					void (wrap_list_type::*)(
						wrap_iterator_type, wrap_list_type&, wrap_const_iterator_type, wrap_iterator_type)>(
					&wrap_list_type::template splice<wrap_iterator_type, wrap_const_iterator_type, wrap_iterator_type>))
		.def("splice", 
				static_cast<
					void (wrap_list_type::*)(
						wrap_iterator_type, wrap_list_type&, wrap_const_iterator_type, wrap_const_iterator_type)>(
					&wrap_list_type::template splice<wrap_iterator_type, wrap_const_iterator_type, wrap_const_iterator_type>))

		.def("splice", 
				static_cast<
					void (wrap_list_type::*)(
						wrap_const_iterator_type, wrap_list_type&, wrap_iterator_type, wrap_iterator_type)>(
					&wrap_list_type::template splice<wrap_const_iterator_type, wrap_iterator_type, wrap_iterator_type>))
		.def("splice", 
				static_cast<
					void (wrap_list_type::*)(
						wrap_const_iterator_type, wrap_list_type&, wrap_iterator_type, wrap_const_iterator_type)>(
					&wrap_list_type::template splice<wrap_const_iterator_type, wrap_iterator_type, wrap_const_iterator_type>))
		.def("splice", 
				static_cast<
					void (wrap_list_type::*)(
						wrap_const_iterator_type, wrap_list_type&, wrap_const_iterator_type, wrap_iterator_type)>(
					&wrap_list_type::template splice<wrap_const_iterator_type, wrap_const_iterator_type, wrap_iterator_type>))
		.def("splice", 
				static_cast<
					void (wrap_list_type::*)(
						wrap_const_iterator_type, wrap_list_type&, wrap_const_iterator_type, wrap_const_iterator_type)>(
					&wrap_list_type::template splice<wrap_const_iterator_type, wrap_const_iterator_type, wrap_const_iterator_type>))

		.def("remove", &wrap_list_type::remove)
		.def("wraped_remove", &wrap_list_type::wraped_remove)

		.def("reverse", &wrap_list_type::reverse)

		.def("unique", static_cast< size_type (wrap_list_type::*)(void) >(&wrap_list_type::unique))
		.def("unique_equal_to", &wrap_list_type::unique_equal_to)
		.def("unique_not_equal_to", &wrap_list_type::unique_not_equal_to)
		.def("unique_less", &wrap_list_type::unique_less)
		.def("unique_less_equal", &wrap_list_type::unique_less_equal)
		.def("unique_greater", &wrap_list_type::unique_greater)
		.def("unique_greater_equal", &wrap_list_type::unique_greater_equal)

		.def("wraped_unique", static_cast< wrap_size_type (wrap_list_type::*)(void) >(&wrap_list_type::wraped_unique))
		.def("wraped_unique_equal_to", &wrap_list_type::wraped_unique_equal_to)
		.def("wraped_unique_not_equal_to", &wrap_list_type::wraped_unique_not_equal_to)
		.def("wraped_unique_less", &wrap_list_type::wraped_unique_less)
		.def("wraped_unique_less_equal", &wrap_list_type::wraped_unique_less_equal)
		.def("wraped_unique_greater", &wrap_list_type::wraped_unique_greater)
		.def("wraped_unique_greater_equal", &wrap_list_type::wraped_unique_greater_equal)

		.def("sort", static_cast<void (wrap_list_type::*)(void) >(&wrap_list_type::sort))
		.def("sort_aesc", &wrap_list_type::sort_aesc)
		.def("sort_desc", &wrap_list_type::sort_desc)
	;

	return cobj;
}

} // namespace lua_wrap
} // namespace yggr

#endif //__YGGR_LUA_WRAP_LUA_EXPORT_WRAP_LIST_HPP__
