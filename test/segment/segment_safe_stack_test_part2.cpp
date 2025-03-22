//segment_safe_stack_test.cpp

#include <assert.h>
#include <test/wait_any_key/wait_any_key.hpp>
#include <boost/bind.hpp>

#include <yggr/utility/swap.hpp>

#include <yggr/segment/segment_info.hpp>
#include <yggr/segment/segment_container.hpp>
#include <yggr/segment/segment_op_proxy.hpp>
#include <yggr/segment/segment_op.hpp>
#include <yggr/segment/segment_mode.hpp>

#include <yggr/container/deque.hpp>
#include <yggr/container/vector.hpp>
#include <yggr/container/list.hpp>
#include <yggr/container/options_deque.hpp>
#include <yggr/container/options_vector.hpp>

#include <yggr/safe_container/safe_stack.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#include YGGR_PP_LINK_LIB(segment)
#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(base)

template<typename Seg>
void delete_segment(const std::string& name)
{
	typedef Seg seg_type;
	typedef yggr::segment::segment_destroyer<seg_type> destroyer_type;

	if(name.empty())
	{
		return;
	}

	{ destroyer_type del(name.c_str()); }
}


//-------------------------test base-------------------------

template<typename Cont, typename SegCont>
void test_constructor(SegCont& seg_cont)
{
	typedef Cont stack_type;
	typedef SegCont seg_cont_type;

	typedef typename stack_type::base_type base_stack_type;
	typedef typename stack_type::container_type inner_container_type;
	typedef typename stack_type::allocator_type alloc_type;
	typedef typename stack_type::value_type val_type;

	alloc_type now_alloc(seg_cont.segment().get_segment_manager());

	//val_type arr[5] = {1, 2, 3, 4, 5};

	{

		stack_type cont(now_alloc);
		yggr_test_assert(cont.empty());
	}

	{
		typename stack_type::container_type init_cont(now_alloc);
		stack_type cont(init_cont);
		yggr_test_assert(cont.empty());
	}

	{
		base_stack_type base_cont(now_alloc);
		base_cont.push(10);
		base_cont.push(20);

		stack_type cont(boost::move(base_cont));
		yggr_test_assert(cont.size() == 2 && base_cont.size() == 0);
	}

	{
		base_stack_type base_cont(now_alloc);
		base_cont.push(10);
		base_cont.push(20);

		stack_type cont(boost::move(base_cont), base_cont.get_allocator());
		yggr_test_assert(cont.size() == 2 && base_cont.size() == 0);
	}

	{
		base_stack_type base_cont(now_alloc);
		base_cont.push(10);
		base_cont.push(20);

		stack_type cont(base_cont);
		yggr_test_assert(cont.size() == 2 && base_cont.size() == 2);
	}

	{
		base_stack_type base_cont(now_alloc);
		base_cont.push(10);
		base_cont.push(20);

		stack_type cont(base_cont, base_cont.get_allocator());
		yggr_test_assert(cont.size() == 2 && base_cont.size() == 2);
	}

	{
		stack_type cont0(now_alloc);
		cont0.push(10);
		cont0.push(20);

		stack_type cont(boost::move(cont0));
		yggr_test_assert(cont.size() == 2 && cont0.size() == 0);
	}

	{
		stack_type cont0(now_alloc);
		cont0.push(10);
		cont0.push(20);

		stack_type cont(boost::move(cont0), cont0.get_allocator());
		yggr_test_assert(cont.size() == 2 && cont0.size() == 0);
	}

	{
		stack_type cont0(now_alloc);
		cont0.push(10);
		cont0.push(20);

		stack_type cont(cont0);
		yggr_test_assert(cont.size() == 2 && cont0.size() == 2);
	}

	{
		stack_type cont0(now_alloc);
		cont0.push(10);
		cont0.push(20);

		stack_type cont(cont0, cont0.get_allocator());
		yggr_test_assert(cont.size() == 2 && cont0.size() == 2);
	}

	{
		stack_type cont11(now_alloc);
		cont11.push(10);
		cont11.push(20);
		stack_type base_cont11(cont11.load());

		// safe_copy_base
		yggr_test_assert(cont11 == base_cont11);
		yggr_test_assert(!(cont11 != base_cont11));
		yggr_test_assert(cont11 <= base_cont11);
		yggr_test_assert(!(cont11 < base_cont11));
		yggr_test_assert(cont11 >= base_cont11);
		yggr_test_assert(!(cont11 > base_cont11));

		// base_copy_safe
		yggr_test_assert(base_cont11 == cont11);
		yggr_test_assert(!(base_cont11 != cont11));
		yggr_test_assert(base_cont11 <= cont11);
		yggr_test_assert(!(base_cont11 < cont11));
		yggr_test_assert(base_cont11 >= cont11);
		yggr_test_assert(!(base_cont11 > cont11));

		// selfsafe_copy_selfsafe
		yggr_test_assert(cont11 == cont11);
		yggr_test_assert(!(cont11 != cont11));
		yggr_test_assert(cont11 <= cont11);
		yggr_test_assert(!(cont11 < cont11));
		yggr_test_assert(cont11 >= cont11);
		yggr_test_assert(!(cont11 > cont11));

		stack_type cont12(cont11);

		// safe_copy_safe
		yggr_test_assert(cont11 == cont12);
		yggr_test_assert(!(cont11 != cont12));
		yggr_test_assert(cont11 <= cont12);
		yggr_test_assert(!(cont11 < cont12));
		yggr_test_assert(cont11 >= cont12);
		yggr_test_assert(!(cont11 > cont12));

		stack_type cont13(cont11, cont11.get_allocator());

		// safe_copy_safe
		yggr_test_assert(cont11 == cont13);
		yggr_test_assert(!(cont11 != cont13));
		yggr_test_assert(cont11 <= cont13);
		yggr_test_assert(!(cont11 < cont13));
		yggr_test_assert(cont11 >= cont13);
		yggr_test_assert(!(cont11 > cont13));

		stack_type cont14(boost::move(cont12));

		// safe_copy_safe
		yggr_test_assert(cont11 == cont14);
		yggr_test_assert(!(cont11 != cont14));
		yggr_test_assert(cont11 <= cont14);
		yggr_test_assert(!(cont11 < cont14));
		yggr_test_assert(cont11 >= cont14);
		yggr_test_assert(!(cont11 > cont14));

		stack_type cont15(boost::move(cont14), cont14.get_allocator());

		// safe_copy_safe
		yggr_test_assert(cont11 == cont15);
		yggr_test_assert(!(cont11 != cont15));
		yggr_test_assert(cont11 <= cont15);
		yggr_test_assert(!(cont11 < cont15));
		yggr_test_assert(cont11 >= cont15);
		yggr_test_assert(!(cont11 > cont15));
	}

	std::cout << "---------------test_constructor end-----------------" << std::endl;
}

template<typename Cont, typename SegCont>
void test_capacity(SegCont& seg_cont)
{
	typedef Cont stack_type;
	typedef SegCont seg_cont_type;

	typedef typename stack_type::base_type base_stack_type;
	typedef typename stack_type::container_type inner_container_type;
	typedef typename stack_type::allocator_type alloc_type;
	typedef typename stack_type::value_type val_type;

	int arr[10] = {1, 2, 3, 4, 5};
	inner_container_type init_cont(arr + 0, arr + 10, alloc_type(seg_cont.segment().get_segment_manager()));

	stack_type cont1(init_cont);
	base_stack_type base_cont1(init_cont);

	yggr_test_assert(cont1.size() == base_cont1.size());

	std::cout << "---------------test_capacity end-----------------" << std::endl;
}

template<typename Cont, typename SegCont>
void test_modifiers(SegCont& seg_cont)
{
	typedef Cont stack_type;
	typedef SegCont seg_cont_type;

	typedef typename stack_type::base_type base_stack_type;
	typedef typename stack_type::container_type inner_container_type;
	typedef typename stack_type::allocator_type alloc_type;
	typedef typename stack_type::value_type val_type;

	alloc_type now_alloc(seg_cont.segment().get_segment_manager());

	stack_type cont(now_alloc);
	base_stack_type base_cont(now_alloc);

	cont.push(10);
	cont.push(20);
	cont.emplace(30);

	cont.emplace();
	yggr_test_assert(cont.top() == 0);
	cont.top() = 50;
	{
		int tmp = 0;
		yggr_test_assert(cont.get_top(tmp));
		yggr_test_assert(tmp == 50);
	}
	cont.pop();


	int n = 100;
	cont.push(boost::move(n));

	int m = 0;
	yggr_test_assert(cont.get_top(m));
	yggr_test_assert(m == n);
	yggr_test_assert(cont.pop());

	int k = 0;
	yggr_test_assert(cont.pop(k));
	yggr_test_assert(k == 30);

	cont.clear();
	yggr_test_assert(cont.empty());
	yggr_test_assert(!cont.pop());
	int e = 0;
	yggr_test_assert(!cont.pop(e));

	//emplace
	{
		stack_type ins_cont(now_alloc);
		ins_cont.emplace();
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.top() == 0);
	}

	{
		stack_type ins_cont(now_alloc);
		yggr_test_assert(ins_cont.emplace(10));
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.top() == 10);
	}

	std::cout << "---------------test_modifiers end-----------------" << std::endl;
}

template<typename Cont, typename SegCont>
void test_swap(SegCont& seg_cont)
{
	typedef Cont stack_type;
	typedef SegCont seg_cont_type;

	typedef typename stack_type::base_type base_stack_type;
	typedef typename stack_type::container_type inner_container_type;
	typedef typename stack_type::allocator_type alloc_type;
	typedef typename stack_type::value_type val_type;

	alloc_type now_alloc(seg_cont.segment().get_segment_manager());

	int arr1[10] = {1, 2, 3, 4, 5};
	int arr2[10] = {6, 7, 8, 9, 0};

	inner_container_type init_cont1(arr1 + 0, arr1 + 10, now_alloc);
	inner_container_type init_cont2(arr2 + 0, arr2 + 5, now_alloc);

	stack_type cont1(init_cont1);
	stack_type cont2(init_cont2);

	base_stack_type base_cont1(init_cont1);
	base_stack_type base_cont2(init_cont2);
	yggr_test_assert(cont1.size() == base_cont1.size());
	yggr_test_assert(cont1.size() != base_cont2.size());

	base_stack_type base_cont3(base_cont1);
	base_stack_type base_cont4(base_cont2);

	// ------------------ boost::swap test------------------------
	// base_type&& this_type&
	boost::swap(boost::move(base_cont4), cont1);
	yggr_test_assert(base_cont4.size() == base_cont1.size());

	// base_type& this_type&
	boost::swap(base_cont3, cont1);
	yggr_test_assert(base_cont3.size() == base_cont2.size());

	// this_type& base_type&&
	boost::swap(cont1, boost::move(base_cont3));
	yggr_test_assert(base_cont3.size() == base_cont1.size());

	// this_type& base_type&
	boost::swap(cont1, boost::move(base_cont4));
	yggr_test_assert(base_cont4.size() == base_cont2.size());

	// this_type& this_type&
	boost::swap(cont1, cont2);
	boost::swap(cont1, cont2);

	// ------------------ std::swap test------------------------
	// base_type&& this_type&
	std::swap(boost::move(base_cont4), cont1);
	yggr_test_assert(base_cont4.size() == base_cont1.size());

	// base_type& this_type&
	std::swap(base_cont3, cont1);
	yggr_test_assert(base_cont3.size() == base_cont2.size());

	// this_type& base_type&&
	std::swap(cont1, boost::move(base_cont3));
	yggr_test_assert(base_cont3.size() == base_cont1.size());

	// this_type& base_type&
	std::swap(cont1, boost::move(base_cont4));
	yggr_test_assert(base_cont4.size() == base_cont2.size());

	// this_type& this_type&
	std::swap(cont1, cont2);
	std::swap(cont1, cont2);

	std::cout << "---------------test_swap end-----------------" << std::endl;
}

template<typename Cont, typename SegCont>
void test_safe_other(SegCont& seg_cont)
{
	typedef Cont stack_type;
	typedef SegCont seg_cont_type;

	typedef typename stack_type::base_type base_stack_type;
	typedef typename stack_type::container_type inner_container_type;
	typedef typename stack_type::allocator_type alloc_type;
	typedef typename stack_type::value_type val_type;

	alloc_type now_alloc(seg_cont.segment().get_segment_manager());
	int arr1[10] = {1, 2, 3, 4, 5};

	inner_container_type init_cont1(arr1 + 0, arr1 + 10, now_alloc);

	stack_type cont(init_cont1);

	base_stack_type base_cont1 = cont.load();
	yggr_test_assert(base_cont1.top() == cont.top());

	base_stack_type base_cont2 = cont;
	yggr_test_assert(base_cont2.top() == cont.top());

	std::cout << "---------------test_safe_other end-----------------" << std::endl;
}

template<typename Cont, typename SegCont>
void debug_test(SegCont& seg_cont)
{
	typedef Cont stack_type;
	typedef SegCont seg_cont_type;

	test_constructor<stack_type>(seg_cont);
	test_capacity<stack_type>(seg_cont);
	test_modifiers<stack_type>(seg_cont);
	test_swap<stack_type>(seg_cont);
	test_safe_other<stack_type>(seg_cont);
}


int main(int argc, char* argv[])
{
	// test_mamaged_mapped_file
	{

		typedef boost::interprocess::managed_mapped_file seg_type;
		typedef yggr::segment::segment_container<seg_type> seg_cont_type;
		typedef boost::interprocess::allocator<int, seg_type::segment_manager> alloc_type;

		seg_cont_type seg_cont("../test_out/segment/test_mamaged_mapped_file_safe_stack",
								yggr::segment::segment_op::create_only(),
								65536,
								boost::bind(
									&delete_segment<seg_type>,
									"../test_out/segment/test_mamaged_mapped_file_safe_stack"));

		// yggr
		{
			typedef
				yggr::safe_container::safe_stack
				<
					int,
					boost::interprocess::interprocess_mutex,
					yggr::vector<int, alloc_type>
				> stack_type;

			debug_test<stack_type>(seg_cont);
		}

		{
			typedef
				yggr::safe_container::safe_stack
				<
					int,
					boost::interprocess::interprocess_mutex,
					yggr::options_vector<int, alloc_type>
				> stack_type;

			debug_test<stack_type>(seg_cont);
		}

		{
			typedef
				yggr::safe_container::safe_stack
				<
					int,
					boost::interprocess::interprocess_mutex,
					yggr::deque<int, alloc_type>
				> stack_type;

			debug_test<stack_type>(seg_cont);
		}

		{
			typedef
				yggr::safe_container::safe_stack
				<
					int,
					boost::interprocess::interprocess_mutex,
					yggr::options_deque<int, alloc_type>
				> stack_type;

			debug_test<stack_type>(seg_cont);
		}

		{
			typedef
				yggr::safe_container::safe_stack
				<
					int,
					boost::interprocess::interprocess_mutex,
					yggr::list<int, alloc_type>
				> stack_type;

			debug_test<stack_type>(seg_cont);
		}

		// boost
		{
			typedef
				yggr::safe_container::safe_stack
				<
					int,
					boost::interprocess::interprocess_mutex,
					boost::container::vector<int, alloc_type>
				> stack_type;

			debug_test<stack_type>(seg_cont);
		}

		{
			typedef
				yggr::safe_container::safe_stack
				<
					int,
					boost::interprocess::interprocess_mutex,
					boost::container::deque<int, alloc_type>
				> stack_type;

			debug_test<stack_type>(seg_cont);
		}

		{
			typedef
				yggr::safe_container::safe_stack
				<
					int,
					boost::interprocess::interprocess_mutex,
					boost::container::list<int, alloc_type>
				> stack_type;

			debug_test<stack_type>(seg_cont);
		}

		seg_cont.free();
		yggr_test_assert(seg_cont.is_named_enable_object());
		yggr_test_assert(seg_cont.empty());
	}

	wait_any_key(argc, argv);
	return 0;
}
