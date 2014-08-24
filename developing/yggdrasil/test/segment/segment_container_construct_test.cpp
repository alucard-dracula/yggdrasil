//segment_container_construct_test.cpp

#include <iostream>

//#include <vector>
#include <boost/interprocess/shared_memory_object.hpp>
#include <yggr/segment/segment_container.hpp>
#include <yggr/segment/segment_op_proxy.hpp>
#include <yggr/segment/segment_op.hpp>
#include <yggr/segment/segment_mode.hpp>
#include <boost/bind.hpp>

//#define BOOST_CONTAINER_DOXYGEN_INVOKED

#include <yggr/safe_container/safe_vector.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

template<typename Seg>
void delete_segment(const std::string& name)
{
	typedef Seg seg_type;
	typedef yggr::segment::segment_deleter<std::string, seg_type> deleter_type;
	{ deleter_type del(name); }
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
	typedef yggr::segment::segment_container<std::string, seg_type> seg_cont_type;
	typedef boost::interprocess::allocator<int, seg_type::segment_manager> alloc_type;
	typedef yggr::safe_container::safe_vector<int, boost::interprocess::interprocess_mutex, alloc_type> vt_type;
	typedef vt_type::iterator vt_iter_type;
	typedef vt_type::const_iterator vt_citer_type;



	seg_cont_type cont("test_managed_shared_memory", boost::bind(&delete_segment<seg_type>, "test_managed_shared_memory"),
							yggr::segment::segment_op::create_only, 65536);

	assert(cont.is_named_object() && !cont.empty());
	std::cout << "cont name" << cont.name() << std::endl;
	std::cout << "cont size" << cont.size() << std::endl;

	vt_type* pvt = cont.segment().construct<vt_type>("vector")(alloc_type(cont.segment().get_segment_manager()));

	assert(pvt);
	assert(pvt->empty());

	for(int i = 0, isize = 10; i != isize; ++i)
	{
		pvt->push_back(i + 1);
	}

	assert(!pvt->empty());


    //pvt->use_handler_of_all(boost::bind(&out_it<vt_iter_type>, _1));
    pvt->use_handler_of_all(out_vector<vt_iter_type>());

	vt_type* pvt2 = cont.segment().find<vt_type>("vector").first;
	assert(pvt2);
	pvt2->use_handler_of_all(out_vector<vt_iter_type>());

	pvt->clear();

	assert(pvt->empty());

	cont.segment().destroy_ptr(pvt);
	pvt = 0;

	cont.free();
	assert(cont.is_named_object());
	assert(cont.empty());

	std::cout << "--------------------------test_managed_shared_memory end----------------------" << std::endl;
}

void test_managed_mapped_file(void)
{
	typedef boost::interprocess::managed_mapped_file seg_type;
	typedef yggr::segment::segment_container<std::string, seg_type> seg_cont_type;

	seg_cont_type cont("test_mamaged_mapped_file", boost::bind(&delete_segment<seg_type>, "test_mamaged_mapped_file"),
							yggr::segment::segment_op::create_only, 65536);

	assert(cont.is_named_object() && !cont.empty());
	std::cout << cont.name() << std::endl;
	std::cout << cont.size() << std::endl;

	cont.free();
	assert(cont.is_named_object());
	assert(cont.empty());
}

int main(int argc, char* argv[])
{

	test_managed_shared_memory();
	test_managed_mapped_file();

	char cc = 0;
	std::cin >> cc;

	return 0;
}
