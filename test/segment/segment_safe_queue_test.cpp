//segment_safe_queue_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <boost/bind.hpp>

#include <yggr/utility/swap.hpp>

#include <yggr/segment/segment_info.hpp>
#include <yggr/segment/segment_container.hpp>
#include <yggr/segment/segment_op_proxy.hpp>
#include <yggr/segment/segment_op.hpp>
#include <yggr/segment/segment_mode.hpp>

#include <yggr/container/deque.hpp>
#include <yggr/container/list.hpp>
#include <yggr/container/options_deque.hpp>

#include <yggr/safe_container/safe_queue.hpp>

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
	typedef Cont queue_type;
	typedef SegCont seg_cont_type;

	typedef typename queue_type::base_type base_queue_type;
	typedef typename queue_type::container_type inner_container_type;
	typedef typename queue_type::allocator_type alloc_type;
	typedef typename queue_type::value_type val_type;

	alloc_type now_alloc(seg_cont.segment().get_segment_manager());

	//val_type arr[5] = {1, 2, 3, 4, 5};

	{
		queue_type cont(now_alloc);
		yggr_test_assert(cont.empty());
	}

	{
		base_queue_type init_cont(now_alloc);
		queue_type cont(init_cont);
		yggr_test_assert(cont.empty());
	}

	// base_cont
	{
		base_queue_type base_cont(now_alloc);
		base_cont.push(10);
		base_cont.push(20);

		queue_type cont(boost::move(base_cont));
		yggr_test_assert(cont.size() == 2 && base_cont.size() == 0);
	}

	{
		base_queue_type base_cont(now_alloc);
		base_cont.push(10);
		base_cont.push(20);

		queue_type cont(boost::move(base_cont), base_cont.get_allocator());
		yggr_test_assert(cont.size() == 2 && base_cont.size() == 0);
	}

	{
		base_queue_type base_cont(now_alloc);
		base_cont.push(10);
		base_cont.push(20);

		queue_type cont(base_cont);
		yggr_test_assert(cont.size() == 2 && base_cont.size() == 2);
	}

	{
		base_queue_type base_cont(now_alloc);
		base_cont.push(10);
		base_cont.push(20);

		queue_type cont(base_cont, base_cont.get_allocator());
		yggr_test_assert(cont.size() == 2 && base_cont.size() == 2);
	}

	// this_cont
	{
		queue_type cont0(now_alloc);
		cont0.push(10);
		cont0.push(20);

		queue_type cont(boost::move(cont0));
		yggr_test_assert(cont.size() == 2 && cont0.size() == 0);
	}

	{
		queue_type cont0(now_alloc);
		cont0.push(10);
		cont0.push(20);

		queue_type cont(boost::move(cont0), cont0.get_allocator());
		yggr_test_assert(cont.size() == 2 && cont0.size() == 0);
	}

	{
		queue_type cont0(now_alloc);
		cont0.push(10);
		cont0.push(20);

		queue_type cont(cont0);
		yggr_test_assert(cont.size() == 2 && cont0.size() == 2);
	}

	{
		queue_type cont0(now_alloc);
		cont0.push(10);
		cont0.push(20);

		queue_type cont(cont0, cont0.get_allocator());
		yggr_test_assert(cont.size() == 2 && cont0.size() == 2);
	}

	{
		queue_type cont11(now_alloc);
		cont11.push(10);
		cont11.push(20);
		queue_type base_cont11(cont11);

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

		queue_type cont12(cont11);

		// safe_copy_safe
		yggr_test_assert(cont11 == cont12);
		yggr_test_assert(!(cont11 != cont12));
		yggr_test_assert(cont11 <= cont12);
		yggr_test_assert(!(cont11 < cont12));
		yggr_test_assert(cont11 >= cont12);
		yggr_test_assert(!(cont11 > cont12));

		queue_type cont13(cont11, cont11.get_allocator());

		// safe_copy_safe
		yggr_test_assert(cont11 == cont13);
		yggr_test_assert(!(cont11 != cont13));
		yggr_test_assert(cont11 <= cont13);
		yggr_test_assert(!(cont11 < cont13));
		yggr_test_assert(cont11 >= cont13);
		yggr_test_assert(!(cont11 > cont13));

		queue_type cont14(boost::move(cont12));

		// safe_copy_safe
		yggr_test_assert(cont11 == cont14);
		yggr_test_assert(!(cont11 != cont14));
		yggr_test_assert(cont11 <= cont14);
		yggr_test_assert(!(cont11 < cont14));
		yggr_test_assert(cont11 >= cont14);
		yggr_test_assert(!(cont11 > cont14));

		queue_type cont15(boost::move(cont14), cont14.get_allocator());

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
	typedef Cont queue_type;
	typedef SegCont seg_cont_type;

	typedef typename queue_type::base_type base_queue_type;
	typedef typename queue_type::container_type inner_container_type;
	typedef typename queue_type::allocator_type alloc_type;
	typedef typename queue_type::value_type val_type;

	alloc_type now_alloc(seg_cont.segment().get_segment_manager());

	int arr[10] = {1, 2, 3, 4, 5};
	typename queue_type::container_type init_cont(arr + 0, arr + 10, now_alloc);

	queue_type cont1(init_cont);
	base_queue_type base_cont1(init_cont);

	yggr_test_assert(cont1.size() == base_cont1.size());

	int f = 0, b = 0;
	yggr_test_assert(cont1.get_front(f));
	yggr_test_assert(cont1.get_back(b));

	yggr_test_assert(f == base_cont1.front() && b == base_cont1.back());

	cont1.clear();
	yggr_test_assert(!cont1.get_front(f));
	yggr_test_assert(!cont1.get_back(b));

	std::cout << "---------------test_capacity end-----------------" << std::endl;
}

template<typename Cont, typename SegCont>
void test_modifiers(SegCont& seg_cont)
{
	typedef Cont queue_type;
	typedef SegCont seg_cont_type;

	typedef typename queue_type::base_type base_queue_type;
	typedef typename queue_type::container_type inner_container_type;
	typedef typename queue_type::allocator_type alloc_type;
	typedef typename queue_type::value_type val_type;

	alloc_type now_alloc(seg_cont.segment().get_segment_manager());

	queue_type cont(now_alloc);
	base_queue_type base_cont(now_alloc);

	cont.push(10);
	cont.push(20);
	cont.push(30);

	int n = 100;
	cont.push(boost::move(n));

	int f = 0, b = 0;
	yggr_test_assert(cont.get_front(f));
	yggr_test_assert(cont.get_back(b));
	yggr_test_assert(f == 10);
	yggr_test_assert(b == n);
	yggr_test_assert(cont.front() == f);
	yggr_test_assert(cont.back() == b);
	yggr_test_assert(cont.pop());

	int k = 0;
	yggr_test_assert(cont.pop(k));
	yggr_test_assert(k == 20);

	cont.back() = 200;
	yggr_test_assert(cont.back() == 200);

	cont.front() = 300;
	yggr_test_assert(cont.front() == 300);

	cont.clear();
	yggr_test_assert(cont.empty());
	yggr_test_assert(!cont.pop());
	int e = 0;
	yggr_test_assert(!cont.pop(e));

	//emplace
	{
		queue_type ins_cont(now_alloc);
		ins_cont.emplace();
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.front() == 0);
		yggr_test_assert(ins_cont.back() == 0);
	}

	{
		queue_type ins_cont(now_alloc);
		yggr_test_assert(ins_cont.emplace(10));
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.front() == 10);
		yggr_test_assert(ins_cont.back() == 10);
	}

	std::cout << "---------------test_modifiers end-----------------" << std::endl;
}

template<typename Cont, typename SegCont>
void test_swap(SegCont& seg_cont)
{
	typedef Cont queue_type;
	typedef SegCont seg_cont_type;

	typedef typename queue_type::base_type base_queue_type;
	typedef typename queue_type::container_type inner_container_type;
	typedef typename queue_type::allocator_type alloc_type;
	typedef typename queue_type::value_type val_type;

	alloc_type now_alloc(seg_cont.segment().get_segment_manager());

	int arr1[10] = {1, 2, 3, 4, 5};
	int arr2[10] = {6, 7, 8, 9, 0};

	typename queue_type::container_type init_cont1(arr1 + 0, arr1 + 10, now_alloc);
	typename queue_type::container_type init_cont2(arr2 + 0, arr2 + 5, now_alloc);

	queue_type cont1(init_cont1);
	queue_type cont2(init_cont2);

	base_queue_type base_cont1(init_cont1);
	base_queue_type base_cont2(init_cont2);
	yggr_test_assert(cont1.size() == base_cont1.size());
	yggr_test_assert(cont1.size() != base_cont2.size());

	base_queue_type base_cont3(base_cont1);
	base_queue_type base_cont4(base_cont2);

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

	// this_type&& this_type&
	boost::swap(boost::move(cont1), cont2);

	// this_type& this_type&&
	boost::swap(cont1, boost::move(cont2));

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

	// this_type&& this_type&
	std::swap(boost::move(cont1), cont2);

	// this_type& this_type&&
	std::swap(cont1, boost::move(cont2));

	// ------------------ yggr::swap test------------------------
	// base_type&& this_type&
	yggr::swap(boost::move(base_cont4), cont1);
	yggr_test_assert(base_cont4.size() == base_cont1.size());

	// base_type& this_type&
	yggr::swap(base_cont3, cont1);
	yggr_test_assert(base_cont3.size() == base_cont2.size());

	// this_type& base_type&&
	yggr::swap(cont1, boost::move(base_cont3));
	yggr_test_assert(base_cont3.size() == base_cont1.size());

	// this_type& base_type&
	yggr::swap(cont1, boost::move(base_cont4));
	yggr_test_assert(base_cont4.size() == base_cont2.size());

	// this_type& this_type&
	yggr::swap(cont1, cont2);
	yggr::swap(cont1, cont2);

	// this_type&& this_type&
	yggr::swap(boost::move(cont1), cont2);

	// this_type& this_type&&
	yggr::swap(cont1, boost::move(cont2));

	std::cout << "---------------test_swap end-----------------" << std::endl;
}

template<typename Cont, typename SegCont>
void test_safe_other(SegCont& seg_cont)
{
	typedef Cont queue_type;
	typedef SegCont seg_cont_type;

	typedef typename queue_type::base_type base_queue_type;
	typedef typename queue_type::container_type inner_container_type;
	typedef typename queue_type::allocator_type alloc_type;
	typedef typename queue_type::value_type val_type;

	alloc_type now_alloc(seg_cont.segment().get_segment_manager());

	int arr1[10] = {1, 2, 3, 4, 5};

	typename queue_type::container_type init_cont1(arr1 + 0, arr1 + 10, now_alloc);

	queue_type cont(init_cont1);

	base_queue_type base_cont1 = cont.load();
	yggr_test_assert(base_cont1.front() == cont.front());
	yggr_test_assert(base_cont1.back() == cont.back());

	base_queue_type base_cont2 = cont;
	yggr_test_assert(base_cont2.front() == cont.front());
	yggr_test_assert(base_cont2.back() == cont.back());

	std::cout << "---------------test_safe_other end-----------------" << std::endl;
}

template<typename Cont, typename SegCont>
void debug_test(SegCont& seg_cont)
{
	typedef Cont queue_type;
	typedef SegCont seg_cont_type;

	test_constructor<queue_type>(seg_cont);
	test_capacity<queue_type>(seg_cont);
	test_modifiers<queue_type>(seg_cont);
	test_swap<queue_type>(seg_cont);
	test_safe_other<queue_type>(seg_cont);
}

int main(int argc, char* argv[])
{
	// managed_shared_memory
	{
		typedef boost::interprocess::managed_shared_memory seg_type;
		typedef yggr::segment::segment_container<seg_type> seg_cont_type;
		typedef boost::interprocess::allocator<int, seg_type::segment_manager> alloc_type;

		seg_cont_type seg_cont("test_managed_shared_memory_safe_queue",
								yggr::segment::segment_op::create_only(),
								65536,
								boost::bind(
									&delete_segment<seg_type>,
									"test_managed_shared_memory_safe_queue") );
		// yggr

		{
			typedef
				yggr::safe_container::safe_queue
				<
					int,
					boost::interprocess::interprocess_mutex,
					yggr::list<int, alloc_type>
				> queue_type;

			debug_test<queue_type>(seg_cont);
		}

		{
			typedef
				yggr::safe_container::safe_queue
				<
					int,
					boost::interprocess::interprocess_mutex,
					yggr::deque<int, alloc_type>
				> queue_type;

			debug_test<queue_type>(seg_cont);
		}

		{
			typedef
				yggr::safe_container::safe_queue
				<
					int,
					boost::interprocess::interprocess_mutex,
					yggr::options_deque<int, alloc_type>
				> queue_type;

			debug_test<queue_type>(seg_cont);
		}

		// boost

		{
			typedef
				yggr::safe_container::safe_queue
				<
					int,
					boost::interprocess::interprocess_mutex,
					boost::container::list<int, alloc_type>
				> queue_type;

			debug_test<queue_type>(seg_cont);
		}

		{
			typedef
				yggr::safe_container::safe_queue
				<
					int,
					boost::interprocess::interprocess_mutex,
					boost::container::deque<int, alloc_type>
				> queue_type;

			debug_test<queue_type>(seg_cont);
		}

		seg_cont.free();
		yggr_test_assert(seg_cont.is_named_enable_object());
		yggr_test_assert(seg_cont.empty());
	}


	// test_mamaged_mapped_file
	{

		typedef boost::interprocess::managed_mapped_file seg_type;
		typedef yggr::segment::segment_container<seg_type> seg_cont_type;
		typedef boost::interprocess::allocator<int, seg_type::segment_manager> alloc_type;

		seg_cont_type seg_cont("../test_out/segment/test_mamaged_mapped_file_safe_queue",
								yggr::segment::segment_op::create_only(),
								65536,
								boost::bind(
									&delete_segment<seg_type>,
									"../test_out/segment/test_mamaged_mapped_file_safe_queue"));

		// yggr

		{
			typedef
				yggr::safe_container::safe_queue
				<
					int,
					boost::interprocess::interprocess_mutex,
					yggr::list<int, alloc_type>
				> queue_type;

			debug_test<queue_type>(seg_cont);
		}

		{
			typedef
				yggr::safe_container::safe_queue
				<
					int,
					boost::interprocess::interprocess_mutex,
					yggr::deque<int, alloc_type>
				> queue_type;

			debug_test<queue_type>(seg_cont);
		}

		{
			typedef
				yggr::safe_container::safe_queue
				<
					int,
					boost::interprocess::interprocess_mutex,
					yggr::options_deque<int, alloc_type>
				> queue_type;

			debug_test<queue_type>(seg_cont);
		}

		// boost

		{
			typedef
				yggr::safe_container::safe_queue
				<
					int,
					boost::interprocess::interprocess_mutex,
					boost::container::list<int, alloc_type>
				> queue_type;

			debug_test<queue_type>(seg_cont);
		}

		{
			typedef
				yggr::safe_container::safe_queue
				<
					int,
					boost::interprocess::interprocess_mutex,
					boost::container::deque<int, alloc_type>
				> queue_type;

			debug_test<queue_type>(seg_cont);
		}

		seg_cont.free();
		yggr_test_assert(seg_cont.is_named_enable_object());
		yggr_test_assert(seg_cont.empty());
	}

	// test_managed_heap_memory
	{
		typedef boost::interprocess::managed_heap_memory seg_type;
		typedef yggr::segment::segment_container<seg_type> seg_cont_type;
		typedef boost::interprocess::allocator<int, seg_type::segment_manager> alloc_type;

		seg_cont_type seg_cont(65536);

		// yggr

		{
			typedef
				yggr::safe_container::safe_queue
				<
					int,
					boost::interprocess::interprocess_mutex,
					yggr::list<int, alloc_type>
				> queue_type;

			debug_test<queue_type>(seg_cont);
		}

		{
			typedef
				yggr::safe_container::safe_queue
				<
					int,
					boost::interprocess::interprocess_mutex,
					yggr::deque<int, alloc_type>
				> queue_type;

			debug_test<queue_type>(seg_cont);
		}

		{
			typedef
				yggr::safe_container::safe_queue
				<
					int,
					boost::interprocess::interprocess_mutex,
					yggr::options_deque<int, alloc_type>
				> queue_type;

			debug_test<queue_type>(seg_cont);
		}

		// boost

		{
			typedef
				yggr::safe_container::safe_queue
				<
					int,
					boost::interprocess::interprocess_mutex,
					boost::container::list<int, alloc_type>
				> queue_type;

			debug_test<queue_type>(seg_cont);
		}

		{
			typedef
				yggr::safe_container::safe_queue
				<
					int,
					boost::interprocess::interprocess_mutex,
					boost::container::deque<int, alloc_type>
				> queue_type;

			debug_test<queue_type>(seg_cont);
		}

		seg_cont.free();
		yggr_test_assert(!seg_cont.is_named_enable_object());
		yggr_test_assert(seg_cont.empty());
	}

	wait_any_key(argc, argv);
	return 0;
}
