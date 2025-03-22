//test_lua_base_type_gcc_failed.cpp

//#include "yggr_lua_base_type.hpp"

#include <yggr/base/yggrdef.h>
#include <yggr_lua_base_type/yggr_lua_typeinfo.hpp>
#include <yggr_lua_base_type/yggr_lua_typeid_basic.hpp>


//#include <yggr_lua_base_type/yggr_lua_d32.hpp>

//#ifndef __YGGR_LUA_YGGR_LUA_D32_HPP__
//#define __YGGR_LUA_YGGR_LUA_D32_HPP__
//
//#include <yggr/base/yggrdef.h>
//
//#include <yggr/mplex/static_assert.hpp>
//#include <yggr/script/base_lua_environment.hpp>
//
//#include <yggr/lua_wrap/wrap_vector.hpp>
//#include <yggr/lua_wrap/wrap_deque.hpp>
//#include <yggr/lua_wrap/wrap_list.hpp>
//
//#include <boost/mpl/assert.hpp>
//#include <boost/type_traits/is_same.hpp>
//
//#include <lua.hpp>
//
//namespace yggr
//{
//namespace lua
//{
//
//// vector
//typedef yggr::lua_wrap::wrap_vector<yggr::s32> s32_vector_type;
//typedef s32_vector_type::wrap_allocator_type wrap_s32_vector_allocator_type;
//typedef s32_vector_type::wrap_iterator_type wrap_s32_vector_iterator_type;
//typedef s32_vector_type::wrap_const_iterator_type wrap_s32_vector_const_iterator_type;
//typedef s32_vector_type::wrap_reverse_iterator_type wrap_s32_vector_reverse_iterator_type;
//typedef s32_vector_type::wrap_const_reverse_iterator_type wrap_s32_vector_const_reverse_iterator_type;
//
//typedef yggr::lua_wrap::wrap_vector<yggr::u32> u32_vector_type;
//typedef u32_vector_type::wrap_allocator_type wrap_u32_vector_allocator_type;
//typedef u32_vector_type::wrap_iterator_type wrap_u32_vector_iterator_type;
//typedef u32_vector_type::wrap_const_iterator_type wrap_u32_vector_const_iterator_type;
//typedef u32_vector_type::wrap_reverse_iterator_type wrap_u32_vector_reverse_iterator_type;
//typedef u32_vector_type::wrap_const_reverse_iterator_type wrap_u32_vector_const_reverse_iterator_type;
//
//// deque
//typedef yggr::lua_wrap::wrap_deque<yggr::s32> s32_deque_type;
//typedef s32_deque_type::wrap_allocator_type wrap_s32_deque_allocator_type;
//typedef s32_deque_type::wrap_iterator_type wrap_s32_deque_iterator_type;
//typedef s32_deque_type::wrap_const_iterator_type wrap_s32_deque_const_iterator_type;
//typedef s32_deque_type::wrap_reverse_iterator_type wrap_s32_deque_reverse_iterator_type;
//typedef s32_deque_type::wrap_const_reverse_iterator_type wrap_s32_deque_const_reverse_iterator_type;
//
//typedef yggr::lua_wrap::wrap_deque<yggr::u32> u32_deque_type;
//typedef u32_deque_type::wrap_allocator_type wrap_u32_deque_allocator_type;
//typedef u32_deque_type::wrap_iterator_type wrap_u32_deque_iterator_type;
//typedef u32_deque_type::wrap_const_iterator_type wrap_u32_deque_const_iterator_type;
//typedef u32_deque_type::wrap_reverse_iterator_type wrap_u32_deque_reverse_iterator_type;
//typedef u32_deque_type::wrap_const_reverse_iterator_type wrap_u32_deque_const_reverse_iterator_type;
//
//// list
//typedef yggr::lua_wrap::wrap_list<yggr::s32> s32_list_type;
//typedef s32_list_type::wrap_allocator_type wrap_s32_list_allocator_type;
//typedef s32_list_type::wrap_iterator_type wrap_s32_list_iterator_type;
//typedef s32_list_type::wrap_const_iterator_type wrap_s32_list_const_iterator_type;
//typedef s32_list_type::wrap_reverse_iterator_type wrap_s32_list_reverse_iterator_type;
//typedef s32_list_type::wrap_const_reverse_iterator_type wrap_s32_list_const_reverse_iterator_type;
//
//typedef yggr::lua_wrap::wrap_list<yggr::u32> u32_list_type;
//typedef u32_list_type::wrap_allocator_type wrap_u32_list_allocator_type;
//typedef u32_list_type::wrap_iterator_type wrap_u32_list_iterator_type;
//typedef u32_list_type::wrap_const_iterator_type wrap_u32_list_const_iterator_type;
//typedef u32_list_type::wrap_reverse_iterator_type wrap_u32_list_reverse_iterator_type;
//typedef u32_list_type::wrap_const_reverse_iterator_type wrap_u32_list_const_reverse_iterator_type;
//
//BOOST_MPL_ASSERT((boost::is_same<wrap_s32_vector_allocator_type, wrap_s32_deque_allocator_type>));
//BOOST_MPL_ASSERT((boost::is_same<wrap_u32_vector_allocator_type, wrap_u32_deque_allocator_type>));
//
//BOOST_MPL_ASSERT((boost::is_same<wrap_s32_vector_allocator_type, wrap_s32_list_allocator_type>));
//BOOST_MPL_ASSERT((boost::is_same<wrap_u32_vector_allocator_type, wrap_u32_list_allocator_type>));
//
//typedef wrap_s32_vector_allocator_type wrap_s32_allocator_type;
//typedef wrap_u32_vector_allocator_type wrap_u32_allocator_type;
//
//void export_d32_s32_allocator(lua_State* L);
//void export_d32_u32_allocator(lua_State* L);
//
//namespace detail
//{
//
//void export_d32_s32_vector_wrap_s32_vector_iterator(void* pcobj);
//void export_d32_s32_vector_wrap_s32_vector_const_iterator(void* pcobj);
//void export_d32_s32_vector_wrap_s32_vector_reverse_iterator(void* pcobj);
//void export_d32_s32_vector_wrap_s32_vector_const_reverse_iterator(void* pcobj);
//void export_d32_s32_vector_s32_vector(void* pcobj);
//
//void export_d32_u32_vector_wrap_u32_vector_iterator(void* pcobj);
//void export_d32_u32_vector_wrap_u32_vector_const_iterator(void* pcobj);
//void export_d32_u32_vector_wrap_u32_vector_reverse_iterator(void* pcobj);
//void export_d32_u32_vector_wrap_u32_vector_const_reverse_iterator(void* pcobj);
//void export_d32_u32_vector_u32_vector(void* pcobj);
//
//} // namespace detail
//
//void export_d32_s32_vector(lua_State* L);
//void export_d32_u32_vector(lua_State* L);
//
//namespace detail
//{
//
//void export_d32_s32_list_wrap_s32_list_iterator(void* pcobj);
//void export_d32_s32_list_wrap_s32_list_const_iterator(void* pcobj);
//void export_d32_s32_list_wrap_s32_list_reverse_iterator(void* pcobj);
//void export_d32_s32_list_wrap_s32_list_const_reverse_iterator(void* pcobj);
//void export_d32_s32_list_s32_list(void* pcobj);
//
//void export_d32_u32_list_wrap_u32_list_iterator(void* pcobj);
//void export_d32_u32_list_wrap_u32_list_const_iterator(void* pcobj);
//void export_d32_u32_list_wrap_u32_list_reverse_iterator(void* pcobj);
//void export_d32_u32_list_wrap_u32_list_const_reverse_iterator(void* pcobj);
//void export_d32_u32_list_u32_list(void* pcobj);
//
//} // namespace detail
//
//void export_d32_s32_list(lua_State* L);
//void export_d32_u32_list(lua_State* L);
//
//
//namespace detail
//{
//
//void export_d32_s32_queue_wrap_s32_queue_iterator(void* pcobj);
//void export_d32_s32_queue_wrap_s32_queue_const_iterator(void* pcobj);
//void export_d32_s32_queue_wrap_s32_queue_reverse_iterator(void* pcobj);
//void export_d32_s32_queue_wrap_s32_queue_const_reverse_iterator(void* pcobj);
//void export_d32_s32_queue_s32_queue(void* pcobj);
//
//void export_d32_u32_queue_wrap_u32_queue_iterator(void* pcobj);
//void export_d32_u32_queue_wrap_u32_queue_const_iterator(void* pcobj);
//void export_d32_u32_queue_wrap_u32_queue_reverse_iterator(void* pcobj);
//void export_d32_u32_queue_wrap_u32_queue_const_reverse_iterator(void* pcobj);
//void export_d32_u32_queue_u32_queue(void* pcobj);
//
//} // namespace detail
//
//void export_d32_s32_deque(lua_State* L);
//void export_d32_u32_deque(lua_State* L);
//
//
//} // namespace lua
//} // namespace yggr
//
//
//#endif //__YGGR_LUA_YGGR_LUA_D32_HPP__

//#include <yggr_lua_base_type/yggr_lua_d64.hpp>

//#ifndef __YGGR_LUA_YGGR_LUA_D64_HPP__
//#define __YGGR_LUA_YGGR_LUA_D64_HPP__
//
//#include <yggr/base/yggrdef.h>
//
//#include <yggr/mplex/static_assert.hpp>
//#include <yggr/script/base_lua_environment.hpp>
//
//#include <yggr/lua_wrap/wrap_vector.hpp>
//#include <yggr/lua_wrap/wrap_deque.hpp>
//#include <yggr/lua_wrap/wrap_list.hpp>
//
//#include <boost/mpl/assert.hpp>
//#include <boost/type_traits/is_same.hpp>
//#include <lua.hpp>
//
//namespace yggr
//{
//namespace lua
//{
//
//typedef lua_wrap::wrap_base_t_maker<yggr::s64>::type wrap_s64_type;
//typedef lua_wrap::wrap_base_t_maker<yggr::u64>::type wrap_u64_type;
//
//// vector
//typedef yggr::lua_wrap::wrap_vector<yggr::s64> s64_vector_type;
//typedef s64_vector_type::wrap_allocator_type wrap_s64_vector_allocator_type;
//typedef s64_vector_type::wrap_iterator_type wrap_s64_vector_iterator_type;
//typedef s64_vector_type::wrap_const_iterator_type wrap_s64_vector_const_iterator_type;
//typedef s64_vector_type::wrap_reverse_iterator_type wrap_s64_vector_reverse_iterator_type;
//typedef s64_vector_type::wrap_const_reverse_iterator_type wrap_s64_vector_const_reverse_iterator_type;
//
//typedef yggr::lua_wrap::wrap_vector<yggr::u64> u64_vector_type;
//typedef u64_vector_type::wrap_allocator_type wrap_u64_vector_allocator_type;
//typedef u64_vector_type::wrap_iterator_type wrap_u64_vector_iterator_type;
//typedef u64_vector_type::wrap_const_iterator_type wrap_u64_vector_const_iterator_type;
//typedef u64_vector_type::wrap_reverse_iterator_type wrap_u64_vector_reverse_iterator_type;
//typedef u64_vector_type::wrap_const_reverse_iterator_type wrap_u64_vector_const_reverse_iterator_type;
//
//// deque
//typedef yggr::lua_wrap::wrap_deque<yggr::s64> s64_deque_type;
//typedef s64_deque_type::wrap_allocator_type wrap_s64_deque_allocator_type;
//typedef s64_deque_type::wrap_iterator_type wrap_s64_deque_iterator_type;
//typedef s64_deque_type::wrap_const_iterator_type wrap_s64_deque_const_iterator_type;
//typedef s64_deque_type::wrap_reverse_iterator_type wrap_s64_deque_reverse_iterator_type;
//typedef s64_deque_type::wrap_const_reverse_iterator_type wrap_s64_deque_const_reverse_iterator_type;
//
//typedef yggr::lua_wrap::wrap_deque<yggr::u64> u64_deque_type;
//typedef u64_deque_type::wrap_allocator_type wrap_u64_deque_allocator_type;
//typedef u64_deque_type::wrap_iterator_type wrap_u64_deque_iterator_type;
//typedef u64_deque_type::wrap_const_iterator_type wrap_u64_deque_const_iterator_type;
//typedef u64_deque_type::wrap_reverse_iterator_type wrap_u64_deque_reverse_iterator_type;
//typedef u64_deque_type::wrap_const_reverse_iterator_type wrap_u64_deque_const_reverse_iterator_type;
//
//// list
//typedef yggr::lua_wrap::wrap_list<yggr::s64> s64_list_type;
//typedef s64_list_type::wrap_allocator_type wrap_s64_list_allocator_type;
//typedef s64_list_type::wrap_iterator_type wrap_s64_list_iterator_type;
//typedef s64_list_type::wrap_const_iterator_type wrap_s64_list_const_iterator_type;
//typedef s64_list_type::wrap_reverse_iterator_type wrap_s64_list_reverse_iterator_type;
//typedef s64_list_type::wrap_const_reverse_iterator_type wrap_s64_list_const_reverse_iterator_type;
//
//typedef yggr::lua_wrap::wrap_list<yggr::u64> u64_list_type;
//typedef u64_list_type::wrap_allocator_type wrap_u64_list_allocator_type;
//typedef u64_list_type::wrap_iterator_type wrap_u64_list_iterator_type;
//typedef u64_list_type::wrap_const_iterator_type wrap_u64_list_const_iterator_type;
//typedef u64_list_type::wrap_reverse_iterator_type wrap_u64_list_reverse_iterator_type;
//typedef u64_list_type::wrap_const_reverse_iterator_type wrap_u64_list_const_reverse_iterator_type;
//
//BOOST_MPL_ASSERT((boost::is_same<wrap_s64_vector_allocator_type, wrap_s64_deque_allocator_type>));
//BOOST_MPL_ASSERT((boost::is_same<wrap_u64_vector_allocator_type, wrap_u64_deque_allocator_type>));
//
//BOOST_MPL_ASSERT((boost::is_same<wrap_s64_vector_allocator_type, wrap_s64_list_allocator_type>));
//BOOST_MPL_ASSERT((boost::is_same<wrap_u64_vector_allocator_type, wrap_u64_list_allocator_type>));
//
//typedef wrap_s64_vector_allocator_type wrap_s64_allocator_type;
//typedef wrap_u64_vector_allocator_type wrap_u64_allocator_type;
//
//void export_d64_s64(lua_State* L);
//void export_d64_u64(lua_State* L);
//
//void export_d64_s64_allocator(lua_State* L);
//void export_d64_u64_allocator(lua_State* L);
//
//namespace detail
//{
//
//void export_d64_s64_vector_wrap_s64_vector_iterator(void* pcobj);
//void export_d64_s64_vector_wrap_s64_vector_const_iterator(void* pcobj);
//void export_d64_s64_vector_wrap_s64_vector_reverse_iterator(void* pcobj);
//void export_d64_s64_vector_wrap_s64_vector_const_reverse_iterator(void* pcobj);
//void export_d64_s64_vector_s64_vector(void* pcobj);
//
//void export_d64_u64_vector_wrap_u64_vector_iterator(void* pcobj);
//void export_d64_u64_vector_wrap_u64_vector_const_iterator(void* pcobj);
//void export_d64_u64_vector_wrap_u64_vector_reverse_iterator(void* pcobj);
//void export_d64_u64_vector_wrap_u64_vector_const_reverse_iterator(void* pcobj);
//void export_d64_u64_vector_u64_vector(void* pcobj);
//
//} // namespace detail
//
//void export_d64_s64_vector(lua_State* L);
//void export_d64_u64_vector(lua_State* L);
//
//namespace detail
//{
//
//void export_d64_s64_queue_wrap_s64_queue_iterator(void* pcobj);
//void export_d64_s64_queue_wrap_s64_queue_const_iterator(void* pcobj);
//void export_d64_s64_queue_wrap_s64_queue_reverse_iterator(void* pcobj);
//void export_d64_s64_queue_wrap_s64_queue_const_reverse_iterator(void* pcobj);
//void export_d64_s64_queue_s64_queue(void* pcobj);
//
//void export_d64_u64_queue_wrap_u64_queue_iterator(void* pcobj);
//void export_d64_u64_queue_wrap_u64_queue_const_iterator(void* pcobj);
//void export_d64_u64_queue_wrap_u64_queue_reverse_iterator(void* pcobj);
//void export_d64_u64_queue_wrap_u64_queue_const_reverse_iterator(void* pcobj);
//void export_d64_u64_queue_u64_queue(void* pcobj);
//
//} // namespace detail
//
//void export_d64_s64_deque(lua_State* L);
//void export_d64_u64_deque(lua_State* L);
//
//namespace detail
//{
//
//void export_d64_s64_list_wrap_s64_list_iterator(void* pcobj);
//void export_d64_s64_list_wrap_s64_list_const_iterator(void* pcobj);
//void export_d64_s64_list_wrap_s64_list_reverse_iterator(void* pcobj);
//void export_d64_s64_list_wrap_s64_list_const_reverse_iterator(void* pcobj);
//void export_d64_s64_list_s64_list(void* pcobj);
//
//void export_d64_u64_list_wrap_u64_list_iterator(void* pcobj);
//void export_d64_u64_list_wrap_u64_list_const_iterator(void* pcobj);
//void export_d64_u64_list_wrap_u64_list_reverse_iterator(void* pcobj);
//void export_d64_u64_list_wrap_u64_list_const_reverse_iterator(void* pcobj);
//void export_d64_u64_list_u64_list(void* pcobj);
//
//} // namespace detail
//
//void export_d64_s64_list(lua_State* L);
//void export_d64_u64_list(lua_State* L);
//
//} // namespace lua
//} // namespace yggr
//
//#endif //__YGGR_LUA_YGGR_LUA_D64_HPP__

//-------
//#include <yggr/script/base_lua_environment.hpp>
//
//#include <luabind/luabind.hpp>
//#include <luabind/operator.hpp>
//#include <luabind/lua_include.hpp>
//#include <lua.hpp>
//
//#include <iostream>

//#if defined(_MSC_VER)
//#	include <vld.h>
//#endif // _MSC_VER
//
//#include <yggr/compile_link/linker.hpp>
//
//#include YGGR_PP_LINK_LIB(script_lua)
//#include YGGR_PP_LINK_LIB(charset)
//#include YGGR_PP_LINK_LIB(time)
//#include YGGR_PP_LINK_LIB(base)
//
//#include <yggr/dll_make/dll_mark_define.hpp>
//
//#include <yggr/dll_make/dll_make_begin.ipp>
//
//YGGR_DLL_API int LUA_MODULE_INIT_FOO(yggr_lua_base_type)(lua_State* L);
//
//#include <yggr/dll_make/dll_make_end.ipp>
//
//
//int LUA_MODULE_INIT_FOO(yggr_lua_base_type)(lua_State* L)
//{
//
//#ifdef _DEBUG
//	std::cout << "luaopen_yggr_lua_base_type" << std::endl;
//#endif //_DEBUG
//
//	luabind::open(L);
//
//	yggr::lua::export_type_info(L);
//	yggr::lua::export_typeid_basic(L);
//
//	yggr::lua::export_d32_s32_allocator(L);
//	yggr::lua::export_d32_u32_allocator(L);
//
//	yggr::lua::export_d32_s32_vector(L);
//	yggr::lua::export_d32_u32_vector(L);
//
//	yggr::lua::export_d32_s32_deque(L);
//	yggr::lua::export_d32_u32_deque(L);
//
//	yggr::lua::export_d32_s32_list(L);
//	yggr::lua::export_d32_u32_list(L);
//
//	yggr::lua::export_d64_s64(L);
//	yggr::lua::export_d64_u64(L);
//
//	yggr::lua::export_d64_s64_allocator(L);
//	yggr::lua::export_d64_u64_allocator(L);
//
//	yggr::lua::export_d64_s64_vector(L);
//	yggr::lua::export_d64_u64_vector(L);
//
//	yggr::lua::export_d64_s64_deque(L);
//	yggr::lua::export_d64_u64_deque(L);
//
//	yggr::lua::export_d64_s64_list(L);
//	yggr::lua::export_d64_u64_list(L);
//
//	return 0;
//}

//#include <yggr/lua_wrap/wrap_vector.hpp>
//#include <yggr/lua_wrap/wrap_deque.hpp>
//#include <yggr/lua_wrap/wrap_list.hpp>
//
//typedef yggr::container::vector<yggr::s32> s32_vt_type;
//typedef yggr::container

int main(int argc, char* argv[])
{
	char cc = 0;
	std::cin >> cc;
	return 0;
}
