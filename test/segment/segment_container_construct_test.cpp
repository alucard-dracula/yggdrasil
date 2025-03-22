//segment_container_construct_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/segment/segment_info.hpp>
#include <yggr/segment/segment_container.hpp>
#include <yggr/segment/segment_op_proxy.hpp>
#include <yggr/segment/segment_op.hpp>
#include <yggr/segment/segment_mode.hpp>

#include <yggr/safe_container/safe_vector.hpp>

#include <boost/bind.hpp>

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

template<typename Iter>
void out_it(const Iter& iter)
{
    std::cout << (*iter) << std::endl;
}

template<typename Iter>
struct out_vector
{
    void operator()(const Iter& iter) const
    {
        std::cout << (*iter) << std::endl;
    }
};

void test_managed_shared_memory(void)
{
	typedef boost::interprocess::managed_shared_memory seg_type;
	typedef yggr::segment::segment_container<seg_type> seg_cont_type;
	typedef boost::interprocess::allocator<int, seg_type::segment_manager> alloc_type;
	typedef yggr::safe_container::safe_vector<int, boost::interprocess::interprocess_mutex, alloc_type> vt_type;
	typedef vt_type::iterator vt_iter_type;
	typedef vt_type::const_iterator vt_citer_type;

	seg_cont_type cont("test_managed_shared_memory",
							yggr::segment::segment_op::create_only(),
							65536,
							boost::bind(&delete_segment<seg_type>,
											"test_managed_shared_memory") );

	yggr_test_assert(cont.is_named_enable_object() && !cont.empty());
	std::cout << "cont name = " << cont.name() << std::endl;
	std::cout << "cont size = " << cont.size() << std::endl;

	vt_type* pvt = cont.segment().template construct<vt_type>("vector")(alloc_type(cont.segment().get_segment_manager()));

	yggr_test_assert(pvt);
	yggr_test_assert(pvt->empty());

	for(int i = 0, isize = 10; i != isize; ++i)
	{
		pvt->push_back(i + 1);
	}

	yggr_test_assert(!pvt->empty());

    pvt->for_each_iterator(out_vector<vt_iter_type>());

	vt_type* pvt2 = cont.segment().find<vt_type>("vector").first;
	yggr_test_assert(pvt2);
	pvt2->for_each_iterator(out_vector<vt_iter_type>());

	pvt->clear();

	yggr_test_assert(pvt->empty());

	cont.segment().destroy_ptr(pvt);
	pvt = 0;

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
	typedef yggr::safe_container::safe_vector<int, boost::interprocess::interprocess_mutex, alloc_type> vt_type;
	typedef vt_type::iterator vt_iter_type;
	typedef vt_type::const_iterator vt_citer_type;

	seg_cont_type cont("../test_out/segment/test_mamaged_mapped_file",
						yggr::segment::segment_op::create_only(),
						65536,
						boost::bind(
							&delete_segment<seg_type>,
							"../test_out/segment/test_mamaged_mapped_file"));

	yggr_test_assert(cont.is_named_enable_object() && !cont.empty());
	std::cout << "cont name = " << cont.name() << std::endl;
	std::cout << "cont size = " << cont.size() << std::endl;

	vt_type* pvt = cont.segment().template construct<vt_type>("vector")(alloc_type(cont.segment().get_segment_manager()));

	yggr_test_assert(pvt);
	yggr_test_assert(pvt->empty());

	for(int i = 0, isize = 10; i != isize; ++i)
	{
		pvt->push_back(i + 1);
	}

	yggr_test_assert(!pvt->empty());

    pvt->for_each_iterator(out_vector<vt_iter_type>());

	vt_type* pvt2 = cont.segment().find<vt_type>("vector").first;
	yggr_test_assert(pvt2);
	pvt2->for_each_iterator(out_vector<vt_iter_type>());

	pvt->clear();

	yggr_test_assert(pvt->empty());

	cont.segment().destroy_ptr(pvt);
	pvt = 0;

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
	typedef yggr::safe_container::safe_vector<int, boost::interprocess::interprocess_mutex, alloc_type> vt_type;
	typedef vt_type::iterator vt_iter_type;
	typedef vt_type::const_iterator vt_citer_type;

	seg_cont_type cont(65536);

	yggr_test_assert(!cont.is_named_enable_object() && !cont.empty());
	std::cout << "cont name = " << cont.name() << std::endl;
	std::cout << "cont size = " << cont.size() << std::endl;

	vt_type* pvt = cont.segment().template construct<vt_type>("vector")(alloc_type(cont.segment().get_segment_manager()));

	yggr_test_assert(pvt);
	yggr_test_assert(pvt->empty());

	for(int i = 0, isize = 10; i != isize; ++i)
	{
		pvt->push_back(i + 1);
	}

	yggr_test_assert(!pvt->empty());

    pvt->for_each_iterator(out_vector<vt_iter_type>());

	vt_type* pvt2 = cont.segment().find<vt_type>("vector").first;
	yggr_test_assert(pvt2);
	pvt2->for_each_iterator(out_vector<vt_iter_type>());

	pvt->clear();

	yggr_test_assert(pvt->empty());

	cont.segment().destroy_ptr(pvt);
	pvt = 0;

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
