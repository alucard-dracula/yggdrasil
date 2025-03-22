//lua_wrap_allocator_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/container/vector.hpp>
#include <yggr/lua_wrap/wrap_allocator.hpp>

#include <cassert>
#include <typeinfo>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

template<typename T>
void test_construct(void)
{
	typedef T cont_type;
	typedef typename cont_type::value_type value_type;
	typedef typename cont_type::allocator_type alloc_type;
	typedef yggr::lua_wrap::wrap_allocator<alloc_type> wrap_alloc_type;

	value_type arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));

	{
		wrap_alloc_type alloc;
		yggr_test_assert((*alloc.address(cont[0]) == cont[0]));
	}

	{
		wrap_alloc_type alloc = cont.get_allocator();
		yggr_test_assert((*alloc.address(cont[0]) == cont[0]));
	}

	{
		wrap_alloc_type alloc0 = cont.get_allocator();
		wrap_alloc_type alloc = alloc0;
		yggr_test_assert((*alloc.address(cont[0]) == cont[0]));
	}

	std::cout << "------test_construct [" << typeid(wrap_alloc_type).name() << "] end------" << std::endl;
}


template<typename T>
void test_operator_val(void)
{
	typedef T cont_type;
	typedef typename cont_type::value_type value_type;
	typedef typename cont_type::allocator_type alloc_type;
	typedef yggr::lua_wrap::wrap_allocator<alloc_type> wrap_alloc_type;

	value_type arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));

	{
		wrap_alloc_type alloc = cont.get_allocator();
		wrap_alloc_type& alloc_ref = alloc;
		const wrap_alloc_type& alloc_cref = alloc;
		yggr_test_assert((*alloc_ref.address(cont[0]) == cont[0]));
		yggr_test_assert((*alloc_cref.address(cont[0]) == cont[0]));
	}

	std::cout << "------test_operator_val [" << typeid(wrap_alloc_type).name() << "] end------" << std::endl;
}

template<typename T>
void test_member_foo(void)
{
	typedef T cont_type;
	typedef typename cont_type::value_type value_type;
	typedef typename cont_type::allocator_type alloc_type;
	typedef yggr::lua_wrap::wrap_allocator<alloc_type> wrap_alloc_type;

	value_type arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));

	// address
	{
		wrap_alloc_type alloc = cont.get_allocator();
		wrap_alloc_type& alloc_ref = alloc;
		const wrap_alloc_type& alloc_cref = alloc;
		yggr_test_assert((*alloc_ref.address(cont[0]) == cont[0]));
		yggr_test_assert((*alloc_cref.address(cont[0]) == cont[0]));
	}

	// allocate, deallocate
	{
		wrap_alloc_type alloc = cont.get_allocator();
		value_type* pval = alloc.allocate(sizeof(arr) / sizeof(arr[0]));
		memcpy(pval, arr, sizeof(arr));
		yggr_test_assert((0 == memcmp(arr, pval, sizeof(arr))));
		alloc.deallocate(pval, sizeof(arr) / sizeof(arr[0]));
	}

	{
		wrap_alloc_type alloc = cont.get_allocator();
		value_type* pval = alloc.allocate(sizeof(arr) / sizeof(arr[0]));
		memcpy(pval, arr, sizeof(arr));
		yggr_test_assert((0 == memcmp(arr, pval, sizeof(arr))));
		alloc.deallocate(pval, sizeof(arr) / sizeof(arr[0]));
	}

	{
		wrap_alloc_type alloc = cont.get_allocator();
		value_type* pval = alloc.allocate(sizeof(arr) / sizeof(arr[0]), 0);
		memcpy(pval, arr, sizeof(arr));
		yggr_test_assert((0 == memcmp(arr, pval, sizeof(arr))));
		alloc.deallocate(pval, sizeof(arr) / sizeof(arr[0]));
	}

	// max_size
	{
		wrap_alloc_type alloc = cont.get_allocator();
		std::cout << alloc.max_size() << std::endl;
		std::cout << static_cast<double>(alloc.max_size()) << std::endl;
		yggr_test_assert(alloc.max_size() == static_cast<double>(alloc.max_size()));
		yggr_test_assert(static_cast<double>(alloc.max_size()) == alloc.max_size());
	}

	// wraped_max_size
	{
		wrap_alloc_type alloc = cont.get_allocator();
		std::cout << alloc.wraped_max_size() << std::endl;
		yggr_test_assert(alloc.wraped_max_size() == alloc.max_size());
		yggr_test_assert(alloc.max_size() == alloc.wraped_max_size());
	}

	// construct
	{
		wrap_alloc_type alloc = cont.get_allocator();
		value_type* pval = alloc.allocate(1);
		alloc.construct(pval, 100);
		yggr_test_assert(*pval == 100);
		alloc.destroy(pval);
		alloc.deallocate(pval, 1);
	}

	std::cout << "------test_operator_val [" << typeid(wrap_alloc_type).name() << "] end------" << std::endl;
}

template<typename T>
void test_operator_op(void)
{
	typedef T cont_type;
	typedef typename cont_type::value_type value_type;
	typedef typename cont_type::allocator_type alloc_type;
	typedef yggr::lua_wrap::wrap_allocator<alloc_type> wrap_alloc_type;

	value_type arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));

	{
		wrap_alloc_type alloc = cont.get_allocator();
		yggr_test_assert(alloc == cont.get_allocator());
		yggr_test_assert(cont.get_allocator() == alloc);
		yggr_test_assert(alloc == wrap_alloc_type(cont.get_allocator()));

		yggr_test_assert(!(alloc != cont.get_allocator()));
		yggr_test_assert(!(cont.get_allocator() != alloc));
		yggr_test_assert(!(alloc != wrap_alloc_type(cont.get_allocator())));
	}

	std::cout << "------test_operator_op [" << typeid(wrap_alloc_type).name() << "] end------" << std::endl;
}

int main(int argc, char* argv[])
{
	test_construct< yggr::vector<yggr::s64> >();
	test_construct< yggr::vector<yggr::u64> >();

	test_operator_val< yggr::vector<yggr::s64> >();
	test_operator_val< yggr::vector<yggr::u64> >();

	test_member_foo< yggr::vector<yggr::s64> >();
	test_member_foo< yggr::vector<yggr::u64> >();

	test_operator_op< yggr::vector<yggr::s64> >();
	test_operator_op< yggr::vector<yggr::u64> >();

	wait_any_key(argc, argv);
	return 0;
}
