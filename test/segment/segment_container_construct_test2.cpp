//segment_container_construct_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <algorithm>
#include <boost/bind.hpp>

#include <yggr/segment/segment_info.hpp>
#include <yggr/segment/segment_container.hpp>
#include <yggr/segment/segment_op_proxy.hpp>
#include <yggr/segment/segment_op.hpp>
#include <yggr/segment/segment_mode.hpp>

#include <boost/unordered_set.hpp>
#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#   include <vld.h>
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

template<typename Val>
struct out_uset
{
    void operator()(const Val& val) const
    {
        std::cout << val << std::endl;
    }
};

void test_managed_shared_memory(void)
{
	typedef boost::interprocess::managed_shared_memory seg_type;
	typedef yggr::segment::segment_container<seg_type> seg_cont_type;
	typedef boost::interprocess::allocator<int, seg_type::segment_manager> alloc_type;
	typedef boost::unordered_set<int, boost::hash<int>, std::equal_to<int>, alloc_type> set_type;
	typedef set_type::iterator uset_iter_type;
	typedef set_type::const_iterator uset_citer_type;

	seg_cont_type cont("test_managed_shared_memory",
							yggr::segment::segment_op::create_only(), 
							65536,
							boost::bind(&delete_segment<seg_type>, 
											"test_managed_shared_memory") );

	yggr_test_assert(cont.is_named_enable_object() && !cont.empty());
	std::cout << "cont name = " << cont.name() << std::endl;
	std::cout << "cont size = " << cont.size() << std::endl;

	set_type* pset = cont.segment().template construct<set_type>("uset")(alloc_type(cont.segment().get_segment_manager()));

	yggr_test_assert(pset);
	yggr_test_assert(pset->empty());

	for(int i = 0, isize = 10; i != isize; ++i)
	{
		pset->insert(i + 1);
	}

	yggr_test_assert(!pset->empty());

	std::for_each(pset->begin(), pset->end(), out_uset<int>());

	set_type* pset2 = cont.segment().find<set_type>("uset").first;
	yggr_test_assert(pset2);

	std::for_each(pset2->begin(), pset2->end(), out_uset<int>());

	pset->clear();

	yggr_test_assert(pset->empty());

	cont.segment().destroy_ptr(pset);
	pset = 0;

	cont.free();
	yggr_test_assert(cont.is_named_enable_object());
	yggr_test_assert(cont.empty());

	std::cout << "--------------------------test_managed_shared_memory end----------------------" << std::endl;
}

void test_managed_mapped_file(void)
{
	typedef boost::interprocess::managed_mapped_file seg_type;
	typedef yggr::segment::segment_container<seg_type> seg_cont_type;
	typedef boost::interprocess::allocator<int, seg_type::segment_manager> alloc_type;
	typedef boost::unordered_set<int, boost::hash<int>, std::equal_to<int>, alloc_type> set_type;
	typedef set_type::iterator uset_iter_type;
	typedef set_type::const_iterator uset_citer_type;

	seg_cont_type cont("../test_out/segment/test_mamaged_mapped_file",
						yggr::segment::segment_op::create_only(), 
						65536,
						boost::bind(&delete_segment<seg_type>,
						"../test_out/segment/test_mamaged_mapped_file"));

	yggr_test_assert(cont.is_named_enable_object() && !cont.empty());
	std::cout << "cont name = " << cont.name() << std::endl;
	std::cout << "cont size = " << cont.size() << std::endl;

	set_type* pset = cont.segment().template construct<set_type>("uset")(alloc_type(cont.segment().get_segment_manager()));

	yggr_test_assert(pset);
	yggr_test_assert(pset->empty());

	for(int i = 0, isize = 10; i != isize; ++i)
	{
		pset->insert(i + 1);
	}

	yggr_test_assert(!pset->empty());

	std::for_each(pset->begin(), pset->end(), out_uset<int>());

	set_type* pset2 = cont.segment().find<set_type>("uset").first;
	yggr_test_assert(pset2);
	std::for_each(pset2->begin(), pset2->end(), out_uset<int>());

	pset->clear();

	yggr_test_assert(pset->empty());

	cont.segment().destroy_ptr(pset);
	pset = 0;

	cont.free();
	yggr_test_assert(cont.is_named_enable_object());
	yggr_test_assert(cont.empty());

	std::cout << "--------------------------test_managed_mapped_file end----------------------" << std::endl;
}

//memory pool
void test_managed_heap_memory(void)
{
	typedef boost::interprocess::managed_heap_memory seg_type;
	typedef yggr::segment::segment_container<seg_type> seg_cont_type;
	typedef boost::interprocess::allocator<int, seg_type::segment_manager> alloc_type;
	typedef boost::unordered_set<int, boost::hash<int>, std::equal_to<int>, alloc_type> set_type;
	typedef set_type::iterator uset_iter_type;
	typedef set_type::const_iterator uset_citer_type;

	seg_cont_type cont(65536);

	yggr_test_assert(!cont.is_named_enable_object() && !cont.empty());
	std::cout << "cont name = " << cont.name() << std::endl;
	std::cout << "cont size = " << cont.size() << std::endl;

	set_type* pset = cont.segment().template construct<set_type>("uset")(alloc_type(cont.segment().get_segment_manager()));

	yggr_test_assert(pset);
	yggr_test_assert(pset->empty());

	for(int i = 0, isize = 10; i != isize; ++i)
	{
		pset->insert(i + 1);
	}

	yggr_test_assert(!pset->empty());

	std::for_each(pset->begin(), pset->end(), out_uset<int>());

	set_type* pset2 = cont.segment().find<set_type>("uset").first;
	yggr_test_assert(pset2);
	std::for_each(pset2->begin(), pset2->end(), out_uset<int>());

	pset->clear();

	yggr_test_assert(pset->empty());

	cont.segment().destroy_ptr(pset);
	pset = 0;

	cont.free();
	yggr_test_assert(!cont.is_named_enable_object());
	yggr_test_assert(cont.empty());

	std::cout << "--------------------------test_managed_heap_memory end----------------------" << std::endl;
}

int main(int argc, char* argv[])
{

	std::cout 
		<< "page_size = "
		<< yggr::segment::segment_info::s_get_page_size() 
		<< std::endl;

	test_managed_shared_memory();
	test_managed_mapped_file();
	test_managed_heap_memory();

	wait_any_key(argc, argv);
	return 0;
}
