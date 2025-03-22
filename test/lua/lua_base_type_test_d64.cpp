// lua_base_type_test.cpp

#include <yggr/base/yggrdef.h>
#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/charset/string.hpp>

#include <yggr/script/lua_script.hpp>
#include <yggr/script/lua_environment.hpp>
#include <yggr/script/script_mgr.hpp>

#include <yggr_lua_base_type.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#   include <vld.h>
#endif // _MSC_VER

#include YGGR_PP_LINK_LIB(base)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(script_lua)
#include YGGR_PP_LINK_LIB(system_controller)

typedef yggr::script::script_mgr<std::string, yggr::script::lua::lua_script,
									yggr::script::lua::lua_environment> script_mgr_type;

void test_typeinfo_basic(script_mgr_type& mgr)
{
	typedef yggr::lua::wrap_u64_type wrap_u64_type;
	typedef yggr::lua::wrap_s64_type wrap_s64_type;
	yggr::u32 na = 100;
	yggr::u64 a = na;
	yggr::u64 ra;

	bool bright = mgr.execute_rule(ra, yggr::const_args_anchor("lua_base_type_test_typeinfo"), a);
	yggr_test_assert(bright);
	std::cout << "C++ [" << ra << ", " << a << "]" << std::endl;
}

void test_s64(script_mgr_type& mgr)
{
	typedef yggr::lua::wrap_s64_type wrap_s64_type;
	typedef yggr::lua::wrap_u64_type wrap_u64_type;
	
	yggr::u32 na = 100;
	wrap_s64_type a(na);
	wrap_s64_type ra;

	bool bright = mgr.execute_rule(ra, yggr::const_args_anchor("lua_base_type_test_d64_val_or_ref"), a);
	yggr_test_assert(bright);
	std::cout << "C++ [" << ra << ", " << a << "]" << std::endl;
}

void test_u64(script_mgr_type& mgr)
{
	typedef yggr::lua::wrap_u64_type wrap_u64_type;
	typedef yggr::lua::wrap_s64_type wrap_s64_type;
	yggr::u32 na = 100;
	wrap_u64_type a(na);
	wrap_u64_type ra;

	bool bright = mgr.execute_rule(ra, yggr::const_args_anchor("lua_base_type_test_d64_val_or_ref"), a);
	yggr_test_assert(bright);
	std::cout << "C++ [" << ra << ", " << a << "]" << std::endl;
}

void test_s64_ref(script_mgr_type& mgr)
{
	typedef yggr::lua::wrap_s64_type wrap_s64_type;
	typedef yggr::lua::wrap_u64_type wrap_u64_type;
	
	yggr::u32 na = 100;
	wrap_s64_type a(na);
	wrap_s64_type ra;

	bool bright = mgr.execute_rule(ra, yggr::const_args_anchor("lua_base_type_test_d64_val_or_ref"), boost::ref(a));
	yggr_test_assert(bright);
	std::cout << "C++ [" << ra << ", " << a << "]" << std::endl;
}

void test_u64_ref(script_mgr_type& mgr)
{
	typedef yggr::lua::wrap_u64_type wrap_u64_type;
	typedef yggr::lua::wrap_s64_type wrap_s64_type;
	yggr::u32 na = 100;
	wrap_u64_type a(na);
	wrap_u64_type ra;

	bool bright = mgr.execute_rule(ra, yggr::const_args_anchor("lua_base_type_test_d64_val_or_ref"), boost::ref(a));
	yggr_test_assert(bright);
	std::cout << "C++ [" << ra << ", " << a << "]" << std::endl;
}

void test_s64_cref(script_mgr_type& mgr)
{
	typedef yggr::lua::wrap_s64_type wrap_s64_type;
	typedef yggr::lua::wrap_u64_type wrap_u64_type;
	
	yggr::u32 na = 100;
	wrap_s64_type a(na);
	wrap_s64_type ra;

	bool bright = mgr.execute_rule(ra, yggr::const_args_anchor("lua_base_type_test_d64_cref"), boost::cref(a));
	yggr_test_assert(bright);
	std::cout << "C++ [" << ra << ", " << a << "]" << std::endl;
}

void test_u64_cref(script_mgr_type& mgr)
{
	typedef yggr::lua::wrap_u64_type wrap_u64_type;
	typedef yggr::lua::wrap_s64_type wrap_s64_type;
	yggr::u32 na = 100;
	wrap_u64_type a(na);
	wrap_u64_type ra;

	bool bright = mgr.execute_rule(ra, yggr::const_args_anchor("lua_base_type_test_d64_cref"), boost::cref(a));
	yggr_test_assert(bright);
	std::cout << "C++ [" << ra << ", " << a << "]" << std::endl;
}

void test_s64_vector_allocator(script_mgr_type& mgr)
{
	typedef yggr::vector<yggr::s64> cont_type;
	typedef cont_type::allocator_type alloc_type;
	typedef yggr::lua_wrap::wrap_allocator<alloc_type> wrap_alloc_type;
	typedef yggr::lua_wrap::wrap_base_t_maker<alloc_type::size_type>::type wrap_size_type;

	cont_type cont;
	wrap_alloc_type alloc = cont.get_allocator();
	//alloc_type::size_type rst = 0; // using double or wrap_base_t recv result, because u64 has mistake
	wrap_size_type rst = 0;
	bool bright = mgr.execute_rule(rst, yggr::const_args_anchor("lua_base_type_test_s64_vt_allocator_val_or_ref_or_cref"), alloc);
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
	std::cout << "C++ " << alloc.max_size() << std::endl;
	yggr_test_assert(rst == alloc.max_size());
}

void test_u64_vector_allocator(script_mgr_type& mgr)
{
	typedef yggr::vector<yggr::u64> cont_type;
	typedef cont_type::allocator_type alloc_type;
	typedef yggr::lua_wrap::wrap_allocator<alloc_type> wrap_alloc_type;
	typedef yggr::lua_wrap::wrap_base_t_maker<alloc_type::size_type>::type wrap_size_type;

	cont_type cont;
	wrap_alloc_type alloc = cont.get_allocator();
	//alloc_type::size_type rst = 0; // using double or wrap_base_t recv result, because u64 has mistake
	wrap_size_type rst = 0;
	bool bright = mgr.execute_rule(rst, yggr::const_args_anchor("lua_base_type_test_u64_vt_allocator_val_or_ref_or_cref"), alloc);
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
	std::cout << "C++ " << alloc.max_size() << std::endl;
	yggr_test_assert(rst == alloc.max_size());
}

void test_s64_vector_allocator_ref(script_mgr_type& mgr)
{
	typedef yggr::vector<yggr::s64> cont_type;
	typedef cont_type::allocator_type alloc_type;
	typedef yggr::lua_wrap::wrap_allocator<alloc_type> wrap_alloc_type;
	typedef yggr::lua_wrap::wrap_base_t_maker<alloc_type::size_type>::type wrap_size_type;

	cont_type cont;
	wrap_alloc_type alloc = cont.get_allocator();
	//alloc_type::size_type rst = 0; // using double or wrap_base_t recv result, because u64 has mistake
	wrap_size_type rst = 0;
	bool bright = mgr.execute_rule(rst, yggr::const_args_anchor("lua_base_type_test_s64_vt_allocator_val_or_ref_or_cref"), boost::ref(alloc));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
	std::cout << "C++ " << alloc.max_size() << std::endl;
	yggr_test_assert(rst == alloc.max_size());
}

void test_u64_vector_allocator_ref(script_mgr_type& mgr)
{
	typedef yggr::vector<yggr::u64> cont_type;
	typedef cont_type::allocator_type alloc_type;
	typedef yggr::lua_wrap::wrap_allocator<alloc_type> wrap_alloc_type;
	typedef yggr::lua_wrap::wrap_base_t_maker<alloc_type::size_type>::type wrap_size_type;

	cont_type cont;
	wrap_alloc_type alloc = cont.get_allocator();
	//alloc_type::size_type rst = 0; // using double or wrap_base_t recv result, because u64 has mistake
	wrap_size_type rst = 0;
	bool bright = mgr.execute_rule(rst, yggr::const_args_anchor("lua_base_type_test_u64_vt_allocator_val_or_ref_or_cref"), boost::ref(alloc));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
	std::cout << "C++ " << alloc.max_size() << std::endl;
	yggr_test_assert(rst == alloc.max_size());
}

void test_s64_vector_allocator_cref(script_mgr_type& mgr)
{
	typedef yggr::vector<yggr::s64> cont_type;
	typedef cont_type::allocator_type alloc_type;
	typedef yggr::lua_wrap::wrap_allocator<alloc_type> wrap_alloc_type;
	typedef yggr::lua_wrap::wrap_base_t_maker<alloc_type::size_type>::type wrap_size_type;

	cont_type cont;
	wrap_alloc_type alloc = cont.get_allocator();
	//alloc_type::size_type rst = 0; // using double or wrap_base_t recv result, because u64 has mistake
	wrap_size_type rst = 0;
	bool bright = mgr.execute_rule(rst, yggr::const_args_anchor("lua_base_type_test_s64_vt_allocator_val_or_ref_or_cref"), boost::cref(alloc));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
	std::cout << "C++ " << alloc.max_size() << std::endl;
	yggr_test_assert(rst == alloc.max_size());
}

void test_u64_vector_allocator_cref(script_mgr_type& mgr)
{
	typedef yggr::vector<yggr::u64> cont_type;
	typedef cont_type::allocator_type alloc_type;
	typedef yggr::lua_wrap::wrap_allocator<alloc_type> wrap_alloc_type;
	typedef yggr::lua_wrap::wrap_base_t_maker<alloc_type::size_type>::type wrap_size_type;

	cont_type cont;
	wrap_alloc_type alloc = cont.get_allocator();
	//alloc_type::size_type rst = 0; // using double or wrap_base_t recv result, because u64 has mistake
	wrap_size_type rst = 0;
	bool bright = mgr.execute_rule(rst, yggr::const_args_anchor("lua_base_type_test_u64_vt_allocator_val_or_ref_or_cref"), boost::cref(alloc));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
	std::cout << "C++ " << alloc.max_size() << std::endl;
	yggr_test_assert(rst == alloc.max_size());
}

void test_s64_vector_iterator(script_mgr_type& mgr)
{
	typedef yggr::vector<yggr::s64> cont_type;
	typedef cont_type::iterator iter_type;
	typedef yggr::lua_wrap::wrap_random_access_iterator<iter_type> wrap_iter_type;

	yggr::s64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_iter_type iter_beg = cont.begin(), iter_end = cont.end();
	yggr::s64 rst = 0;
	bool bright = mgr.execute_rule(rst, yggr::const_args_anchor("lua_base_type_test_s64_vt_iterator_val_or_ref_or_cref"), iter_beg, iter_end);
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_u64_vector_iterator(script_mgr_type& mgr)
{
	typedef yggr::vector<yggr::u64> cont_type;
	typedef cont_type::iterator iter_type;
	typedef yggr::lua_wrap::wrap_random_access_iterator<iter_type> wrap_iter_type;

	yggr::u64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_iter_type iter_beg = cont.begin(), iter_end = cont.end();
	yggr::u64 rst = 0;
	bool bright = mgr.execute_rule(rst, yggr::const_args_anchor("lua_base_type_test_u64_vt_iterator_val_or_ref_or_cref"), iter_beg, iter_end);
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_s64_vector_iterator_ref(script_mgr_type& mgr)
{
	typedef yggr::vector<yggr::s64> cont_type;
	typedef cont_type::iterator iter_type;
	typedef yggr::lua_wrap::wrap_random_access_iterator<iter_type> wrap_iter_type;

	yggr::s64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_iter_type iter_beg = cont.begin(), iter_end = cont.end();
	yggr::s64 rst = 0;
	bool bright =
		mgr.execute_rule(
			rst,
			yggr::const_args_anchor("lua_base_type_test_s64_vt_iterator_val_or_ref_or_cref"), 
			boost::ref(iter_beg), boost::ref(iter_end));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_u64_vector_iterator_ref(script_mgr_type& mgr)
{
	typedef yggr::vector<yggr::u64> cont_type;
	typedef cont_type::iterator iter_type;
	typedef yggr::lua_wrap::wrap_random_access_iterator<iter_type> wrap_iter_type;

	yggr::u64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_iter_type iter_beg = cont.begin(), iter_end = cont.end();
	yggr::u64 rst = 0;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_u64_vt_iterator_val_or_ref_or_cref"), 
			boost::ref(iter_beg), boost::ref(iter_end));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_s64_vector_iterator_cref(script_mgr_type& mgr)
{
	typedef yggr::vector<yggr::s64> cont_type;
	typedef cont_type::iterator iter_type;
	typedef yggr::lua_wrap::wrap_random_access_iterator<iter_type> wrap_iter_type;

	yggr::s64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_iter_type iter_beg = cont.begin(), iter_end = cont.end();
	yggr::s64 rst = 0;
	bool bright = 
		mgr.execute_rule(
			rst,
			yggr::const_args_anchor("lua_base_type_test_s64_vt_iterator_val_or_ref_or_cref"), 
			boost::cref(iter_beg), boost::cref(iter_end));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_u64_vector_iterator_cref(script_mgr_type& mgr)
{
	typedef yggr::vector<yggr::u64> cont_type;
	typedef cont_type::iterator iter_type;
	typedef yggr::lua_wrap::wrap_random_access_iterator<iter_type> wrap_iter_type;

	yggr::u64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_iter_type iter_beg = cont.begin(), iter_end = cont.end();
	yggr::u64 rst = 0;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_u64_vt_iterator_val_or_ref_or_cref"), 
			boost::cref(iter_beg), boost::cref(iter_end));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_s64_vector_const_iterator(script_mgr_type& mgr)
{
	typedef yggr::vector<yggr::s64> cont_type;
	typedef cont_type::const_iterator citer_type;
	typedef yggr::lua_wrap::wrap_random_access_const_iterator<citer_type> wrap_citer_type;

	yggr::s64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_citer_type citer_beg = cont.begin(), citer_end = cont.end();
	yggr::s64 rst = 0;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_d64_vt_const_iterator_val_or_ref_or_cref"),
			citer_beg, citer_end);
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_u64_vector_const_iterator(script_mgr_type& mgr)
{
	typedef yggr::vector<yggr::u64> cont_type;
	typedef cont_type::const_iterator citer_type;
	typedef yggr::lua_wrap::wrap_random_access_const_iterator<citer_type> wrap_citer_type;

	yggr::u64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_citer_type citer_beg = cont.begin(), citer_end = cont.end();
	yggr::u64 rst = 0;
	bool bright = 
		mgr.execute_rule(
			rst,
			yggr::const_args_anchor("lua_base_type_test_d64_vt_const_iterator_val_or_ref_or_cref"), 
			citer_beg, citer_end);
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_s64_vector_const_iterator_ref(script_mgr_type& mgr)
{
	typedef yggr::vector<yggr::s64> cont_type;
	typedef cont_type::const_iterator citer_type;
	typedef yggr::lua_wrap::wrap_random_access_const_iterator<citer_type> wrap_citer_type;

	yggr::s64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_citer_type citer_beg = cont.begin(), citer_end = cont.end();
	yggr::s64 rst = 0;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_d64_vt_const_iterator_val_or_ref_or_cref"),
			boost::ref(citer_beg), boost::ref(citer_end));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_u64_vector_const_iterator_ref(script_mgr_type& mgr)
{
	typedef yggr::vector<yggr::u64> cont_type;
	typedef cont_type::const_iterator citer_type;
	typedef yggr::lua_wrap::wrap_random_access_const_iterator<citer_type> wrap_citer_type;

	yggr::u64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_citer_type citer_beg = cont.begin(), citer_end = cont.end();
	yggr::u64 rst = 0;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_d64_vt_const_iterator_val_or_ref_or_cref"), 
			boost::ref(citer_beg), boost::ref(citer_end));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_s64_vector_const_iterator_cref(script_mgr_type& mgr)
{
	typedef yggr::vector<yggr::s64> cont_type;
	typedef cont_type::const_iterator citer_type;
	typedef yggr::lua_wrap::wrap_random_access_const_iterator<citer_type> wrap_citer_type;

	yggr::s64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_citer_type citer_beg = cont.begin(), citer_end = cont.end();
	yggr::s64 rst = 0;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_d64_vt_const_iterator_val_or_ref_or_cref"),
			boost::cref(citer_beg), boost::cref(citer_end));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_u64_vector_const_iterator_cref(script_mgr_type& mgr)
{
	typedef yggr::vector<yggr::u64> cont_type;
	typedef cont_type::const_iterator citer_type;
	typedef yggr::lua_wrap::wrap_random_access_const_iterator<citer_type> wrap_citer_type;

	yggr::u64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_citer_type citer_beg = cont.begin(), citer_end = cont.end();
	yggr::u64 rst = 0;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_d64_vt_const_iterator_val_or_ref_or_cref"), 
			boost::cref(citer_beg), boost::cref(citer_end));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_s64_vector_reverse_iterator(script_mgr_type& mgr)
{
	typedef yggr::vector<yggr::s64> cont_type;
	typedef cont_type::reverse_iterator riter_type;
	typedef yggr::lua_wrap::wrap_reverse_iterator<riter_type> wrap_riter_type;

	yggr::s64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_riter_type iter_rbeg = cont.rbegin(), iter_rend = cont.rend();
	yggr::s64 rst = 0;
	bool bright = mgr.execute_rule(rst, yggr::const_args_anchor("lua_base_type_test_s64_vt_reverse_iterator_val_or_ref_or_cref"), iter_rbeg, iter_rend);
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_u64_vector_reverse_iterator(script_mgr_type& mgr)
{
	typedef yggr::vector<yggr::u64> cont_type;
	typedef cont_type::reverse_iterator riter_type;
	typedef yggr::lua_wrap::wrap_reverse_iterator<riter_type> wrap_riter_type;

	yggr::u64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_riter_type iter_rbeg = cont.rbegin(), iter_rend = cont.rend();
	yggr::u64 rst = 0;
	bool bright = mgr.execute_rule(rst, yggr::const_args_anchor("lua_base_type_test_u64_vt_reverse_iterator_val_or_ref_or_cref"), iter_rbeg, iter_rend);
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_s64_vector_reverse_iterator_ref(script_mgr_type& mgr)
{
	typedef yggr::vector<yggr::s64> cont_type;
	typedef cont_type::reverse_iterator riter_type;
	typedef yggr::lua_wrap::wrap_reverse_iterator<riter_type> wrap_riter_type;

	yggr::s64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_riter_type iter_rbeg = cont.rbegin(), iter_rend = cont.rend();
	yggr::s64 rst = 0;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_s64_vt_reverse_iterator_val_or_ref_or_cref"), 
			boost::ref(iter_rbeg), boost::ref(iter_rend));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_u64_vector_reverse_iterator_ref(script_mgr_type& mgr)
{
	typedef yggr::vector<yggr::u64> cont_type;
	typedef cont_type::reverse_iterator riter_type;
	typedef yggr::lua_wrap::wrap_reverse_iterator<riter_type> wrap_riter_type;

	yggr::u64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_riter_type iter_rbeg = cont.rbegin(), iter_rend = cont.rend();
	yggr::u64 rst = 0;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_u64_vt_reverse_iterator_val_or_ref_or_cref"), 
			boost::ref(iter_rbeg), boost::ref(iter_rend));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_s64_vector_reverse_iterator_cref(script_mgr_type& mgr)
{
	typedef yggr::vector<yggr::s64> cont_type;
	typedef cont_type::reverse_iterator riter_type;
	typedef yggr::lua_wrap::wrap_reverse_iterator<riter_type> wrap_riter_type;

	yggr::s64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_riter_type iter_rbeg = cont.rbegin(), iter_rend = cont.rend();
	yggr::s64 rst = 0;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_s64_vt_reverse_iterator_val_or_ref_or_cref"), 
			boost::cref(iter_rbeg), boost::cref(iter_rend));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_u64_vector_reverse_iterator_cref(script_mgr_type& mgr)
{
	typedef yggr::vector<yggr::u64> cont_type;
	typedef cont_type::reverse_iterator riter_type;
	typedef yggr::lua_wrap::wrap_reverse_iterator<riter_type> wrap_riter_type;

	yggr::u64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_riter_type iter_rbeg = cont.rbegin(), iter_rend = cont.rend();
	yggr::u64 rst = 0;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_u64_vt_reverse_iterator_val_or_ref_or_cref"), 
			boost::cref(iter_rbeg), boost::cref(iter_rend));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_s64_vector_const_reverse_iterator(script_mgr_type& mgr)
{
	typedef yggr::vector<yggr::s64> cont_type;
	typedef cont_type::const_reverse_iterator criter_type;
	typedef yggr::lua_wrap::wrap_reverse_iterator<criter_type> wrap_criter_type;

	yggr::s64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_criter_type criter_beg = cont.rbegin(), criter_end = cont.rend();
	yggr::s64 rst = 0;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_d64_vt_const_reverse_iterator_val_or_ref_or_cref"),
			criter_beg, criter_end);
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_u64_vector_const_reverse_iterator(script_mgr_type& mgr)
{
	typedef yggr::vector<yggr::u64> cont_type;
	typedef cont_type::const_reverse_iterator criter_type;
	typedef yggr::lua_wrap::wrap_reverse_iterator<criter_type> wrap_criter_type;

	yggr::u64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_criter_type criter_beg = cont.rbegin(), criter_end = cont.rend();
	yggr::u64 rst = 0;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_d64_vt_const_reverse_iterator_val_or_ref_or_cref"),
			criter_beg, criter_end);
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_s64_vector_const_reverse_iterator_ref(script_mgr_type& mgr)
{
	typedef yggr::vector<yggr::s64> cont_type;
	typedef cont_type::const_reverse_iterator criter_type;
	typedef yggr::lua_wrap::wrap_reverse_iterator<criter_type> wrap_criter_type;

	yggr::s64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_criter_type criter_beg = cont.rbegin(), criter_end = cont.rend();
	yggr::s64 rst = 0;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_d64_vt_const_reverse_iterator_val_or_ref_or_cref"),
			boost::ref(criter_beg), boost::ref(criter_end));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_u64_vector_const_reverse_iterator_ref(script_mgr_type& mgr)
{
	typedef yggr::vector<yggr::u64> cont_type;
	typedef cont_type::const_reverse_iterator criter_type;
	typedef yggr::lua_wrap::wrap_reverse_iterator<criter_type> wrap_criter_type;

	yggr::u64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_criter_type criter_beg = cont.rbegin(), criter_end = cont.rend();
	yggr::u64 rst = 0;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_d64_vt_const_reverse_iterator_val_or_ref_or_cref"),
			boost::ref(criter_beg), boost::ref(criter_end));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_s64_vector_const_reverse_iterator_cref(script_mgr_type& mgr)
{
	typedef yggr::vector<yggr::s64> cont_type;
	typedef cont_type::const_reverse_iterator criter_type;
	typedef yggr::lua_wrap::wrap_reverse_iterator<criter_type> wrap_criter_type;

	yggr::s64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_criter_type criter_beg = cont.rbegin(), criter_end = cont.rend();
	yggr::s64 rst = 0;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_d64_vt_const_reverse_iterator_val_or_ref_or_cref"),
			boost::cref(criter_beg), boost::cref(criter_end));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_u64_vector_const_reverse_iterator_cref(script_mgr_type& mgr)
{
	typedef yggr::vector<yggr::u64> cont_type;
	typedef cont_type::const_reverse_iterator criter_type;
	typedef yggr::lua_wrap::wrap_reverse_iterator<criter_type> wrap_criter_type;

	yggr::u64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_criter_type criter_beg = cont.rbegin(), criter_end = cont.rend();
	yggr::u64 rst = 0;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_d64_vt_const_reverse_iterator_val_or_ref_or_cref"),
			boost::cref(criter_beg), boost::cref(criter_end));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_s64_vector(script_mgr_type& mgr)
{
	typedef yggr::lua::s64_vector_type cont_type;

	yggr::s64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	cont_type rst;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_s64_vt_val_or_ref"),
			cont);
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_u64_vector(script_mgr_type& mgr)
{
	typedef yggr::lua::u64_vector_type cont_type;

	yggr::u64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	cont_type rst;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_u64_vt_val_or_ref"),
			cont);
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_s64_vector_ref(script_mgr_type& mgr)
{
	typedef yggr::lua::s64_vector_type cont_type;

	yggr::s64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	cont_type rst;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_s64_vt_val_or_ref"),
			boost::ref(cont));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_u64_vector_ref(script_mgr_type& mgr)
{
	typedef yggr::lua::u64_vector_type cont_type;

	yggr::u64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	cont_type rst;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_u64_vt_val_or_ref"),
			boost::ref(cont));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_s64_vector_cref(script_mgr_type& mgr)
{
	typedef yggr::lua::s64_vector_type cont_type;

	yggr::s64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	cont_type rst;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_d64_vt_cref"),
			boost::cref(cont));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_u64_vector_cref(script_mgr_type& mgr)
{
	typedef yggr::lua::u64_vector_type cont_type;

	yggr::u64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	cont_type rst;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_d64_vt_cref"),
			boost::cref(cont));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_s64_deque(script_mgr_type& mgr)
{
	typedef yggr::lua::s64_deque_type deque_type;

	yggr::s64 arr[] = {1, 2, 3, 4, 5};

	deque_type dq(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	deque_type rst;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_s64_dq_val_or_ref"),
			dq);
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_u64_deque(script_mgr_type& mgr)
{
	typedef yggr::lua::u64_deque_type deque_type;

	yggr::u64 arr[] = {1, 2, 3, 4, 5};

	deque_type dq(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	deque_type rst;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_u64_dq_val_or_ref"),
			dq);
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_s64_deque_ref(script_mgr_type& mgr)
{
	typedef yggr::lua::s64_deque_type deque_type;

	yggr::s64 arr[] = {1, 2, 3, 4, 5};

	deque_type dq(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	deque_type rst;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_s64_dq_val_or_ref"),
			boost::ref(dq));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_u64_deque_ref(script_mgr_type& mgr)
{
	typedef yggr::lua::u64_deque_type deque_type;

	yggr::u64 arr[] = {1, 2, 3, 4, 5};

	deque_type dq(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	deque_type rst;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_u64_dq_val_or_ref"),
			boost::ref(dq));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_s64_deque_cref(script_mgr_type& mgr)
{
	typedef yggr::lua::s64_deque_type deque_type;

	yggr::s64 arr[] = {1, 2, 3, 4, 5};

	deque_type dq(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	deque_type rst;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_d64_dq_cref"),
			boost::cref(dq));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_u64_deque_cref(script_mgr_type& mgr)
{
	typedef yggr::lua::u64_deque_type deque_type;

	yggr::u64 arr[] = {1, 2, 3, 4, 5};

	deque_type dq(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	deque_type rst;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_d64_dq_cref"),
			boost::cref(dq));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_s64_list_iterator(script_mgr_type& mgr)
{
	typedef yggr::list<yggr::s64> cont_type;
	typedef cont_type::iterator iter_type;
	typedef yggr::lua_wrap::wrap_bidirectional_iterator<iter_type> wrap_iter_type;

	yggr::s64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_iter_type iter_beg = cont.begin(), iter_end = cont.end();
	yggr::s64 rst = 0;
	bool bright = mgr.execute_rule(rst, yggr::const_args_anchor("lua_base_type_test_s64_lst_iterator_val_or_ref_or_cref"), iter_beg, iter_end);
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_u64_list_iterator(script_mgr_type& mgr)
{
	typedef yggr::list<yggr::u64> cont_type;
	typedef cont_type::iterator iter_type;
	typedef yggr::lua_wrap::wrap_bidirectional_iterator<iter_type> wrap_iter_type;

	yggr::u64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_iter_type iter_beg = cont.begin(), iter_end = cont.end();
	yggr::u64 rst = 0;
	bool bright = mgr.execute_rule(rst, yggr::const_args_anchor("lua_base_type_test_u64_lst_iterator_val_or_ref_or_cref"), iter_beg, iter_end);
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_s64_list_iterator_ref(script_mgr_type& mgr)
{
	typedef yggr::list<yggr::s64> cont_type;
	typedef cont_type::iterator iter_type;
	typedef yggr::lua_wrap::wrap_bidirectional_iterator<iter_type> wrap_iter_type;

	yggr::s64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_iter_type iter_beg = cont.begin(), iter_end = cont.end();
	yggr::s64 rst = 0;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_s64_lst_iterator_val_or_ref_or_cref"), 
			boost::ref(iter_beg), boost::ref(iter_end));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_u64_list_iterator_ref(script_mgr_type& mgr)
{
	typedef yggr::list<yggr::u64> cont_type;
	typedef cont_type::iterator iter_type;
	typedef yggr::lua_wrap::wrap_bidirectional_iterator<iter_type> wrap_iter_type;

	yggr::u64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_iter_type iter_beg = cont.begin(), iter_end = cont.end();
	yggr::u64 rst = 0;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_u64_lst_iterator_val_or_ref_or_cref"), 
			boost::ref(iter_beg), boost::ref(iter_end));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_s64_list_iterator_cref(script_mgr_type& mgr)
{
	typedef yggr::list<yggr::s64> cont_type;
	typedef cont_type::iterator iter_type;
	typedef yggr::lua_wrap::wrap_bidirectional_iterator<iter_type> wrap_iter_type;

	yggr::s64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_iter_type iter_beg = cont.begin(), iter_end = cont.end();
	yggr::s64 rst = 0;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_s64_lst_iterator_val_or_ref_or_cref"), 
			boost::cref(iter_beg), boost::cref(iter_end));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_u64_list_iterator_cref(script_mgr_type& mgr)
{
	typedef yggr::list<yggr::u64> cont_type;
	typedef cont_type::iterator iter_type;
	typedef yggr::lua_wrap::wrap_bidirectional_iterator<iter_type> wrap_iter_type;

	yggr::u64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_iter_type iter_beg = cont.begin(), iter_end = cont.end();
	yggr::u64 rst = 0;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_u64_lst_iterator_val_or_ref_or_cref"), 
			boost::cref(iter_beg), boost::cref(iter_end));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_s64_list_const_iterator(script_mgr_type& mgr)
{
	typedef yggr::list<yggr::s64> cont_type;
	typedef cont_type::const_iterator citer_type;
	typedef yggr::lua_wrap::wrap_bidirectional_const_iterator<citer_type> wrap_citer_type;

	yggr::s64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_citer_type citer_beg = cont.begin(), citer_end = cont.end();
	yggr::s64 rst = 0;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_d64_lst_const_iterator_val_or_ref_or_cref"),
			citer_beg, citer_end);
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_u64_list_const_iterator(script_mgr_type& mgr)
{
	typedef yggr::list<yggr::u64> cont_type;
	typedef cont_type::const_iterator citer_type;
	typedef yggr::lua_wrap::wrap_bidirectional_const_iterator<citer_type> wrap_citer_type;

	yggr::u64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_citer_type citer_beg = cont.begin(), citer_end = cont.end();
	yggr::u64 rst = 0;
	bool bright = 
		mgr.execute_rule(
			rst,
			yggr::const_args_anchor("lua_base_type_test_d64_lst_const_iterator_val_or_ref_or_cref"), 
			citer_beg, citer_end);
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_s64_list_const_iterator_ref(script_mgr_type& mgr)
{
	typedef yggr::list<yggr::s64> cont_type;
	typedef cont_type::const_iterator citer_type;
	typedef yggr::lua_wrap::wrap_bidirectional_const_iterator<citer_type> wrap_citer_type;

	yggr::s64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_citer_type citer_beg = cont.begin(), citer_end = cont.end();
	yggr::s64 rst = 0;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_d64_lst_const_iterator_val_or_ref_or_cref"),
			boost::ref(citer_beg), boost::ref(citer_end));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_u64_list_const_iterator_ref(script_mgr_type& mgr)
{
	typedef yggr::list<yggr::u64> cont_type;
	typedef cont_type::const_iterator citer_type;
	typedef yggr::lua_wrap::wrap_bidirectional_const_iterator<citer_type> wrap_citer_type;

	yggr::u64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_citer_type citer_beg = cont.begin(), citer_end = cont.end();
	yggr::u64 rst = 0;
	bool bright = 
		mgr.execute_rule(
			rst,
			yggr::const_args_anchor("lua_base_type_test_d64_lst_const_iterator_val_or_ref_or_cref"), 
			boost::ref(citer_beg), boost::ref(citer_end));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_s64_list_const_iterator_cref(script_mgr_type& mgr)
{
	typedef yggr::list<yggr::s64> cont_type;
	typedef cont_type::const_iterator citer_type;
	typedef yggr::lua_wrap::wrap_bidirectional_const_iterator<citer_type> wrap_citer_type;

	yggr::s64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_citer_type citer_beg = cont.begin(), citer_end = cont.end();
	yggr::s64 rst = 0;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_d64_lst_const_iterator_val_or_ref_or_cref"),
			boost::cref(citer_beg), boost::cref(citer_end));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_u64_list_const_iterator_cref(script_mgr_type& mgr)
{
	typedef yggr::list<yggr::u64> cont_type;
	typedef cont_type::const_iterator citer_type;
	typedef yggr::lua_wrap::wrap_bidirectional_const_iterator<citer_type> wrap_citer_type;

	yggr::u64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_citer_type citer_beg = cont.begin(), citer_end = cont.end();
	yggr::u64 rst = 0;
	bool bright = 
		mgr.execute_rule(
			rst,
			yggr::const_args_anchor("lua_base_type_test_d64_lst_const_iterator_val_or_ref_or_cref"), 
			boost::cref(citer_beg), boost::cref(citer_end));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_s64_lst_reverse_iterator(script_mgr_type& mgr)
{
	typedef yggr::list<yggr::s64> cont_type;
	typedef cont_type::reverse_iterator riter_type;
	typedef yggr::lua_wrap::wrap_reverse_iterator<riter_type> wrap_riter_type;

	yggr::s64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_riter_type iter_rbeg = cont.rbegin(), iter_rend = cont.rend();
	yggr::s64 rst = 0;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_s64_lst_reverse_iterator_val_or_ref_or_cref"), 
			iter_rbeg, iter_rend);
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_u64_lst_reverse_iterator(script_mgr_type& mgr)
{
	typedef yggr::list<yggr::u64> cont_type;
	typedef cont_type::reverse_iterator riter_type;
	typedef yggr::lua_wrap::wrap_reverse_iterator<riter_type> wrap_riter_type;

	yggr::u64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_riter_type iter_rbeg = cont.rbegin(), iter_rend = cont.rend();
	yggr::u64 rst = 0;
	bool bright = mgr.execute_rule(rst, yggr::const_args_anchor("lua_base_type_test_u64_lst_reverse_iterator_val_or_ref_or_cref"), iter_rbeg, iter_rend);
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_s64_lst_reverse_iterator_ref(script_mgr_type& mgr)
{
	typedef yggr::list<yggr::s64> cont_type;
	typedef cont_type::reverse_iterator riter_type;
	typedef yggr::lua_wrap::wrap_reverse_iterator<riter_type> wrap_riter_type;

	yggr::s64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_riter_type iter_rbeg = cont.rbegin(), iter_rend = cont.rend();
	yggr::s64 rst = 0;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_s64_lst_reverse_iterator_val_or_ref_or_cref"), 
			boost::ref(iter_rbeg), boost::ref(iter_rend));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_u64_lst_reverse_iterator_ref(script_mgr_type& mgr)
{
	typedef yggr::list<yggr::u64> cont_type;
	typedef cont_type::reverse_iterator riter_type;
	typedef yggr::lua_wrap::wrap_reverse_iterator<riter_type> wrap_riter_type;

	yggr::u64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_riter_type iter_rbeg = cont.rbegin(), iter_rend = cont.rend();
	yggr::u64 rst = 0;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_u64_lst_reverse_iterator_val_or_ref_or_cref"), 
			boost::ref(iter_rbeg), boost::ref(iter_rend));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_s64_lst_reverse_iterator_cref(script_mgr_type& mgr)
{
	typedef yggr::list<yggr::s64> cont_type;
	typedef cont_type::reverse_iterator riter_type;
	typedef yggr::lua_wrap::wrap_reverse_iterator<riter_type> wrap_riter_type;

	yggr::s64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_riter_type iter_rbeg = cont.rbegin(), iter_rend = cont.rend();
	yggr::s64 rst = 0;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_s64_lst_reverse_iterator_val_or_ref_or_cref"), 
			boost::cref(iter_rbeg), boost::cref(iter_rend));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_u64_lst_reverse_iterator_cref(script_mgr_type& mgr)
{
	typedef yggr::list<yggr::u64> cont_type;
	typedef cont_type::reverse_iterator riter_type;
	typedef yggr::lua_wrap::wrap_reverse_iterator<riter_type> wrap_riter_type;

	yggr::u64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_riter_type iter_rbeg = cont.rbegin(), iter_rend = cont.rend();
	yggr::u64 rst = 0;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_u64_lst_reverse_iterator_val_or_ref_or_cref"), 
			boost::cref(iter_rbeg), boost::cref(iter_rend));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_s64_list_const_reverse_iterator(script_mgr_type& mgr)
{
	typedef yggr::list<yggr::s64> cont_type;
	typedef cont_type::const_reverse_iterator criter_type;
	typedef yggr::lua_wrap::wrap_reverse_iterator<criter_type> wrap_criter_type;

	yggr::s64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_criter_type criter_beg = cont.rbegin(), criter_end = cont.rend();
	yggr::s64 rst = 0;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_d64_lst_const_reverse_iterator_val_or_ref_or_cref"),
			criter_beg, criter_end);
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_u64_list_const_reverse_iterator(script_mgr_type& mgr)
{
	typedef yggr::list<yggr::u64> cont_type;
	typedef cont_type::const_reverse_iterator criter_type;
	typedef yggr::lua_wrap::wrap_reverse_iterator<criter_type> wrap_criter_type;

	yggr::u64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_criter_type criter_beg = cont.rbegin(), criter_end = cont.rend();
	yggr::u64 rst = 0;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_d64_lst_const_reverse_iterator_val_or_ref_or_cref"),
			criter_beg, criter_end);
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_s64_list_const_reverse_iterator_ref(script_mgr_type& mgr)
{
	typedef yggr::list<yggr::s64> cont_type;
	typedef cont_type::const_reverse_iterator criter_type;
	typedef yggr::lua_wrap::wrap_reverse_iterator<criter_type> wrap_criter_type;

	yggr::s64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_criter_type criter_beg = cont.rbegin(), criter_end = cont.rend();
	yggr::s64 rst = 0;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_d64_lst_const_reverse_iterator_val_or_ref_or_cref"),
			boost::ref(criter_beg), boost::ref(criter_end));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_u64_list_const_reverse_iterator_ref(script_mgr_type& mgr)
{
	typedef yggr::list<yggr::u64> cont_type;
	typedef cont_type::const_reverse_iterator criter_type;
	typedef yggr::lua_wrap::wrap_reverse_iterator<criter_type> wrap_criter_type;

	yggr::u64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_criter_type criter_beg = cont.rbegin(), criter_end = cont.rend();
	yggr::u64 rst = 0;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_d64_lst_const_reverse_iterator_val_or_ref_or_cref"),
			boost::ref(criter_beg), boost::ref(criter_end));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_s64_list_const_reverse_iterator_cref(script_mgr_type& mgr)
{
	typedef yggr::list<yggr::s64> cont_type;
	typedef cont_type::const_reverse_iterator criter_type;
	typedef yggr::lua_wrap::wrap_reverse_iterator<criter_type> wrap_criter_type;

	yggr::s64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_criter_type criter_beg = cont.rbegin(), criter_end = cont.rend();
	yggr::s64 rst = 0;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_d64_lst_const_reverse_iterator_val_or_ref_or_cref"),
			boost::cref(criter_beg), boost::cref(criter_end));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_u64_list_const_reverse_iterator_cref(script_mgr_type& mgr)
{
	typedef yggr::list<yggr::u64> cont_type;
	typedef cont_type::const_reverse_iterator criter_type;
	typedef yggr::lua_wrap::wrap_reverse_iterator<criter_type> wrap_criter_type;

	yggr::u64 arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	wrap_criter_type criter_beg = cont.rbegin(), criter_end = cont.rend();
	yggr::u64 rst = 0;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_d64_lst_const_reverse_iterator_val_or_ref_or_cref"),
			boost::cref(criter_beg), boost::cref(criter_end));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_s64_list(script_mgr_type& mgr)
{
	typedef yggr::lua::s64_list_type list_type;

	yggr::s64 arr[] = {1, 2, 3, 4, 5};

	list_type lst(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	list_type rst;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_s64_lst_val_or_ref"),
			lst);
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_u64_list(script_mgr_type& mgr)
{
	typedef yggr::lua::u64_list_type list_type;

	yggr::u64 arr[] = {1, 2, 3, 4, 5};

	list_type lst(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	list_type rst;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_u64_lst_val_or_ref"),
			lst);
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_s64_list_ref(script_mgr_type& mgr)
{
	typedef yggr::lua::s64_list_type list_type;

	yggr::s64 arr[] = {1, 2, 3, 4, 5};

	list_type lst(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	list_type rst;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_s64_lst_val_or_ref"),
			boost::ref(lst));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_u64_list_ref(script_mgr_type& mgr)
{
	typedef yggr::lua::u64_list_type list_type;

	yggr::u64 arr[] = {1, 2, 3, 4, 5};

	list_type lst(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	list_type rst;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_u64_lst_val_or_ref"),
			boost::ref(lst));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_s64_list_cref(script_mgr_type& mgr)
{
	typedef yggr::lua::s64_list_type list_type;

	yggr::s64 arr[] = {1, 2, 3, 4, 5};

	list_type lst(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	list_type rst;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_d64_lst_cref"),
			boost::cref(lst));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void test_u64_list_cref(script_mgr_type& mgr)
{
	typedef yggr::lua::u64_list_type list_type;

	yggr::u64 arr[] = {1, 2, 3, 4, 5};

	list_type lst(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	list_type rst;
	bool bright = 
		mgr.execute_rule(
			rst, 
			yggr::const_args_anchor("lua_base_type_test_d64_lst_cref"),
			boost::cref(lst));
	yggr_test_assert(bright);
	std::cout << "C++ " << rst << std::endl;
}

void exec_test()
{
	script_mgr_type mgr;

	mgr.append_module(script_mgr_type::E_lib_all);

	yggr::string code_test_typeinfo("./lua/lua_base_type_test_typeinfo.lua");
	yggr::string code_test_d64_val_or_ref("./lua/lua_base_type_test_d64_val_or_ref.lua");
	yggr::string code_test_d64_cref("./lua/lua_base_type_test_d64_cref.lua");

	yggr::string code_test_s64_vt_allocator("./lua/lua_base_type_test_s64_vt_allocator_val_or_ref_or_cref.lua");
	yggr::string code_test_u64_vt_allocator("./lua/lua_base_type_test_u64_vt_allocator_val_or_ref_or_cref.lua");

	yggr::string code_test_s64_vt_iterator("./lua/lua_base_type_test_s64_vt_iterator_val_or_ref_or_cref.lua");
	yggr::string code_test_u64_vt_iterator("./lua/lua_base_type_test_u64_vt_iterator_val_or_ref_or_cref.lua");
	yggr::string code_test_d64_vt_const_iterator("./lua/lua_base_type_test_d64_vt_const_iterator_val_or_ref_or_cref.lua");

	yggr::string code_test_s64_vt_reverse_iterator("./lua/lua_base_type_test_s64_vt_reverse_iterator_val_or_ref_or_cref.lua");
	yggr::string code_test_u64_vt_reverse_iterator("./lua/lua_base_type_test_u64_vt_reverse_iterator_val_or_ref_or_cref.lua");
	yggr::string code_test_d64_vt_const_reverse_iterator("./lua/lua_base_type_test_d64_vt_const_reverse_iterator_val_or_ref_or_cref.lua");

	yggr::string code_test_s64_vt("./lua/lua_base_type_test_s64_vt_val_or_ref.lua");
	yggr::string code_test_u64_vt("./lua/lua_base_type_test_u64_vt_val_or_ref.lua");
	yggr::string code_test_d64_vt_cref("./lua/lua_base_type_test_d64_vt_cref.lua");

	yggr::string code_test_s64_dq("./lua/lua_base_type_test_s64_dq_val_or_ref.lua");
	yggr::string code_test_u64_dq("./lua/lua_base_type_test_u64_dq_val_or_ref.lua");
	yggr::string code_test_d64_dq_cref("./lua/lua_base_type_test_d64_dq_cref.lua");

	yggr::string code_test_s64_lst_iterator("./lua/lua_base_type_test_s64_lst_iterator_val_or_ref_or_cref.lua");
	yggr::string code_test_u64_lst_iterator("./lua/lua_base_type_test_u64_lst_iterator_val_or_ref_or_cref.lua");
	yggr::string code_test_d64_lst_const_iterator("./lua/lua_base_type_test_d64_lst_const_iterator_val_or_ref_or_cref.lua");

	yggr::string code_test_s64_lst_reverse_iterator("./lua/lua_base_type_test_s64_lst_reverse_iterator_val_or_ref_or_cref.lua");
	yggr::string code_test_u64_lst_reverse_iterator("./lua/lua_base_type_test_u64_lst_reverse_iterator_val_or_ref_or_cref.lua");
	yggr::string code_test_d64_lst_const_reverse_iterator("./lua/lua_base_type_test_d64_lst_const_reverse_iterator_val_or_ref_or_cref.lua");

	yggr::string code_test_s64_lst("./lua/lua_base_type_test_s64_lst_val_or_ref.lua");
	yggr::string code_test_u64_lst("./lua/lua_base_type_test_u64_lst_val_or_ref.lua");
	yggr::string code_test_d64_lst_cref("./lua/lua_base_type_test_d64_lst_cref.lua");

    try
	{
		mgr.insert("lua_base_type_test_typeinfo", "calculate", script_mgr_type::file_code_type(code_test_typeinfo), 0);

		// d64
		mgr.insert("lua_base_type_test_d64_val_or_ref", "calculate", script_mgr_type::file_code_type(code_test_d64_val_or_ref), 0);
		mgr.insert("lua_base_type_test_d64_cref", "calculate", script_mgr_type::file_code_type(code_test_d64_cref), 0);

		mgr.insert("lua_base_type_test_s64_vt_allocator_val_or_ref_or_cref", "calculate", script_mgr_type::file_code_type(code_test_s64_vt_allocator), 0);
		mgr.insert("lua_base_type_test_u64_vt_allocator_val_or_ref_or_cref", "calculate", script_mgr_type::file_code_type(code_test_u64_vt_allocator), 0);

		mgr.insert("lua_base_type_test_s64_vt_iterator_val_or_ref_or_cref", "calculate", script_mgr_type::file_code_type(code_test_s64_vt_iterator), 0);
		mgr.insert("lua_base_type_test_u64_vt_iterator_val_or_ref_or_cref", "calculate", script_mgr_type::file_code_type(code_test_u64_vt_iterator), 0);
		mgr.insert("lua_base_type_test_d64_vt_const_iterator_val_or_ref_or_cref", "calculate", script_mgr_type::file_code_type(code_test_d64_vt_const_iterator), 0);

		mgr.insert("lua_base_type_test_s64_vt_reverse_iterator_val_or_ref_or_cref", "calculate", script_mgr_type::file_code_type(code_test_s64_vt_reverse_iterator), 0);
		mgr.insert("lua_base_type_test_u64_vt_reverse_iterator_val_or_ref_or_cref", "calculate", script_mgr_type::file_code_type(code_test_u64_vt_reverse_iterator), 0);
		mgr.insert("lua_base_type_test_d64_vt_const_reverse_iterator_val_or_ref_or_cref", "calculate", script_mgr_type::file_code_type(code_test_d64_vt_const_reverse_iterator), 0);

		mgr.insert("lua_base_type_test_s64_vt_val_or_ref", "calculate", script_mgr_type::file_code_type(code_test_s64_vt), 0);
		mgr.insert("lua_base_type_test_u64_vt_val_or_ref", "calculate", script_mgr_type::file_code_type(code_test_u64_vt), 0);
		mgr.insert("lua_base_type_test_d64_vt_cref", "calculate", script_mgr_type::file_code_type(code_test_d64_vt_cref), 0);

		mgr.insert("lua_base_type_test_s64_dq_val_or_ref", "calculate", script_mgr_type::file_code_type(code_test_s64_dq), 0);
		mgr.insert("lua_base_type_test_u64_dq_val_or_ref", "calculate", script_mgr_type::file_code_type(code_test_u64_dq), 0);
		mgr.insert("lua_base_type_test_d64_dq_cref", "calculate", script_mgr_type::file_code_type(code_test_d64_dq_cref), 0);

		mgr.insert("lua_base_type_test_s64_lst_iterator_val_or_ref_or_cref", "calculate", script_mgr_type::file_code_type(code_test_s64_lst_iterator), 0);
		mgr.insert("lua_base_type_test_u64_lst_iterator_val_or_ref_or_cref", "calculate", script_mgr_type::file_code_type(code_test_u64_lst_iterator), 0);
		mgr.insert("lua_base_type_test_d64_lst_const_iterator_val_or_ref_or_cref", "calculate", script_mgr_type::file_code_type(code_test_d64_lst_const_iterator), 0);

		mgr.insert("lua_base_type_test_s64_lst_reverse_iterator_val_or_ref_or_cref", "calculate", script_mgr_type::file_code_type(code_test_s64_lst_reverse_iterator), 0);
		mgr.insert("lua_base_type_test_u64_lst_reverse_iterator_val_or_ref_or_cref", "calculate", script_mgr_type::file_code_type(code_test_u64_lst_reverse_iterator), 0);
		mgr.insert("lua_base_type_test_d64_lst_const_reverse_iterator_val_or_ref_or_cref", "calculate", script_mgr_type::file_code_type(code_test_d64_lst_const_reverse_iterator), 0);

		mgr.insert("lua_base_type_test_s64_lst_val_or_ref", "calculate", script_mgr_type::file_code_type(code_test_s64_lst), 0);
		mgr.insert("lua_base_type_test_u64_lst_val_or_ref", "calculate", script_mgr_type::file_code_type(code_test_u64_lst), 0);
		mgr.insert("lua_base_type_test_d64_lst_cref", "calculate", script_mgr_type::file_code_type(code_test_d64_lst_cref), 0);

    }
    catch(const yggr::compatibility::stl_exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

	test_typeinfo_basic(mgr);

	//----------d64-----------

	test_s64(mgr);
	test_u64(mgr);
	test_s64_ref(mgr);
	test_u64_ref(mgr);
	test_s64_cref(mgr);
	test_u64_cref(mgr);

	test_s64_vector_allocator(mgr);
	test_u64_vector_allocator(mgr);
	test_s64_vector_allocator_ref(mgr);
	test_u64_vector_allocator_ref(mgr);
	test_s64_vector_allocator_cref(mgr);
	test_u64_vector_allocator_cref(mgr);

	test_s64_vector_iterator(mgr);
	test_u64_vector_iterator(mgr);
	test_s64_vector_iterator_ref(mgr);
	test_u64_vector_iterator_ref(mgr);
	test_s64_vector_iterator_cref(mgr);
	test_u64_vector_iterator_cref(mgr);

	test_s64_vector_const_iterator(mgr);
	test_u64_vector_const_iterator(mgr);
	test_s64_vector_const_iterator_ref(mgr);
	test_u64_vector_const_iterator_ref(mgr);
	test_s64_vector_const_iterator_cref(mgr);
	test_u64_vector_const_iterator_cref(mgr);

	test_s64_vector_reverse_iterator(mgr);
	test_u64_vector_reverse_iterator(mgr);
	test_s64_vector_reverse_iterator_ref(mgr);
	test_u64_vector_reverse_iterator_ref(mgr);
	test_s64_vector_reverse_iterator_cref(mgr);
	test_u64_vector_reverse_iterator_cref(mgr);

	test_s64_vector_const_reverse_iterator(mgr);
	test_u64_vector_const_reverse_iterator(mgr);
	test_s64_vector_const_reverse_iterator_ref(mgr);
	test_u64_vector_const_reverse_iterator_ref(mgr);
	test_s64_vector_const_reverse_iterator_cref(mgr);
	test_u64_vector_const_reverse_iterator_cref(mgr);

	test_s64_vector(mgr);
	test_u64_vector(mgr);
	test_s64_vector_ref(mgr);
	test_u64_vector_ref(mgr);
	test_s64_vector_cref(mgr);
	test_u64_vector_cref(mgr);

	test_s64_deque(mgr);
	test_u64_deque(mgr);
	test_s64_deque_ref(mgr);
	test_u64_deque_ref(mgr);
	test_s64_deque_cref(mgr);
	test_u64_deque_cref(mgr);

	test_s64_list_iterator(mgr);
	test_u64_list_iterator(mgr);
	test_s64_list_iterator_ref(mgr);
	test_u64_list_iterator_ref(mgr);
	test_s64_list_iterator_cref(mgr);
	test_u64_list_iterator_cref(mgr);

	test_s64_list_const_iterator(mgr);
	test_u64_list_const_iterator(mgr);
	test_s64_list_const_iterator_ref(mgr);
	test_u64_list_const_iterator_ref(mgr);
	test_s64_list_const_iterator_cref(mgr);
	test_u64_list_const_iterator_cref(mgr);

	test_s64_lst_reverse_iterator(mgr);
	test_u64_lst_reverse_iterator(mgr);
	test_s64_lst_reverse_iterator_ref(mgr);
	test_u64_lst_reverse_iterator_ref(mgr);
	test_s64_lst_reverse_iterator_cref(mgr);
	test_u64_lst_reverse_iterator_cref(mgr);

	test_s64_list_const_reverse_iterator(mgr);
	test_u64_list_const_reverse_iterator(mgr);
	test_s64_list_const_reverse_iterator_ref(mgr);
	test_u64_list_const_reverse_iterator_ref(mgr);
	test_s64_list_const_reverse_iterator_cref(mgr);
	test_u64_list_const_reverse_iterator_cref(mgr);

	test_s64_list(mgr);
	test_u64_list(mgr);
	test_s64_list_ref(mgr);
	test_u64_list_ref(mgr);
	test_s64_list_cref(mgr);
	test_u64_list_cref(mgr);

	std::cout << "all test end wait free..." << std::endl;

}

int main(int argc, char *argv[])
{
	// !!! MINGW will release the loaded DLL again after the process ends !!!
#if defined(__MINGW32__)
    HMODULE hmod = LoadLibrary("yggr_lua_base_type.dll");
    if(!hmod)
    {
        std::cerr << "load yggr_lua_base_type.dll faied" << std::endl;
        return -1;
    }
#endif // #if defined(__MINGW32__)

	exec_test();

	wait_any_key(argc, argv);
	return 0;
}
