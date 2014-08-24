//segment_container_test.cpp

#include <iostream>

#include <vld.h>

#include <boost/interprocess/shared_memory_object.hpp>
#include <yggr/segment/segment_container.hpp>
#include <yggr/segment/segment_op_proxy.hpp>
#include <yggr/segment/segment_op.hpp>
#include <yggr/segment/segment_mode.hpp>
#include <boost/bind.hpp>

template<typename Seg>
void delete_segment(const std::string& name)
{
	typedef Seg seg_type;
	typedef yggr::segment::segment_deleter<std::string, seg_type> deleter_type;
	{ deleter_type del(name); }
}

void test_shared_memory_object1(void)
{
	typedef boost::interprocess::shared_memory_object seg_type;
	typedef yggr::segment::segment_container<std::string, seg_type> seg_cont_type;

	seg_cont_type cont("test_shared_memory_object", boost::bind(&delete_segment<seg_type>, "test_shared_memory_object"),
							yggr::segment::segment_op::create_only, yggr::segment::segment_mode::E_read_write);

	try
	{
		cont.segment().truncate(65536);
	}
	catch(const boost::interprocess::interprocess_exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	assert(cont.is_named_object() && !cont.empty());
	std::cout << cont.name() << std::endl; //test_shared_memory_object
	std::cout << cont.segment().get_name() << std::endl; //test_shared_memory_object
	std::cout << cont.size() << std::endl;

	cont.free();

	assert(cont.is_named_object());
	assert(cont.empty());
}

void test_shared_memory_object2(void)
{
	typedef boost::interprocess::shared_memory_object seg_type;
	typedef yggr::segment::segment_container<std::string, seg_type> seg_cont_type;

	seg_cont_type cont("test_shared_memory_object",
							yggr::segment::segment_op::create_only, yggr::segment::segment_mode::E_read_write);

	try
	{
		cont.segment().truncate(65536);
	}
	catch(const boost::interprocess::interprocess_exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	assert(cont.is_named_object() && !cont.empty());
	std::cout << cont.name() << std::endl; //test_shared_memory_object
	std::cout << cont.segment().get_name() << std::endl; //test_shared_memory_object
	std::cout << cont.size() << std::endl;

	cont.free();

	assert(cont.is_named_object());
	assert(cont.empty());
}

void test_managed_shared_memory1(void)
{
	typedef boost::interprocess::managed_shared_memory seg_type;
	typedef yggr::segment::segment_container<std::string, seg_type> seg_cont_type;

	seg_cont_type cont("test_managed_shared_memory", boost::bind(&delete_segment<seg_type>, "test_managed_shared_memory"),
							yggr::segment::segment_op::create_only, 65536);

	assert(cont.is_named_object() && !cont.empty());
	std::cout << cont.name() << std::endl;
	std::cout << cont.size() << std::endl;

	cont.free();
	assert(cont.is_named_object());
	assert(cont.empty());
}

void test_managed_shared_memory2(void)
{
	typedef boost::interprocess::managed_shared_memory seg_type;
	typedef yggr::segment::segment_container<std::string, seg_type> seg_cont_type;

	seg_cont_type cont("test_managed_shared_memory",
							yggr::segment::segment_op::create_only, 65536);

	assert(cont.is_named_object() && !cont.empty());
	std::cout << cont.name() << std::endl;
	std::cout << cont.size() << std::endl;

	cont.free();
	assert(cont.is_named_object());
	assert(cont.empty());
}

void test_file_mapping(void)
{
	typedef boost::interprocess::file_mapping seg_type;
	typedef yggr::segment::segment_container<std::string, seg_type> seg_cont_type;

	seg_cont_type cont("test_file_mapping", boost::bind(&delete_segment<seg_type>, "test_file_mapping"),
							yggr::segment::segment_op::create_only, 65536, yggr::segment::segment_mode::E_read_write);

	assert(cont.is_named_object() && !cont.empty());
	std::cout << cont.name() << std::endl;
	std::cout << cont.size() << std::endl;

	cont.free();
	assert(cont.is_named_object());
	assert(cont.empty());
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


#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/managed_external_buffer.hpp>
#include <yggr/segment/anonymous_segment_container.hpp>

void test_managed_external_buffer(void)
{
	typedef boost::interprocess::managed_external_buffer seg_type;
	typedef yggr::segment::anonymous_segment_container<seg_type> seg_cont_type;
	typedef boost::aligned_storage<65536>::type buffer_type;

	buffer_type buf;
	seg_cont_type cont(yggr::segment::segment_op::create_only, &buf, 65536);

	assert(!cont.is_named_object() && !cont.empty());
	std::cout << cont.size() << std::endl;

	cont.free();

	assert(!cont.is_named_object());
	assert(cont.empty());
}

#include <boost/interprocess/managed_heap_memory.hpp>

void test_managed_heap_memory(void)
{
	typedef boost::interprocess::managed_heap_memory seg_type;
	typedef yggr::segment::anonymous_segment_container<seg_type> seg_cont_type;

	seg_cont_type cont(65536);

	assert(!cont.is_named_object() && !cont.empty());
	std::cout << cont.size() << std::endl;

	cont.free();

	assert(!cont.is_named_object());
	assert(cont.empty());
}

#include <yggr/segment/anonymous_shared_memory.hpp>
void test_anonymous_shared_memory(void)
{
	typedef yggr::segment::anonymous_shared_memory seg_type;
	typedef yggr::segment::anonymous_segment_container<seg_type> seg_cont_type;

	seg_cont_type cont(65536);

	assert(!cont.is_named_object() && !cont.empty());
	std::cout << cont.size() << std::endl;

	cont.free();

	assert(!cont.is_named_object());
	assert(cont.empty());
}


int main(int argc, char* argv[])
{
	test_shared_memory_object1();
	test_shared_memory_object2();

	test_managed_shared_memory1();
	test_managed_shared_memory2();

	test_file_mapping();

	test_managed_mapped_file();

	test_managed_external_buffer();

	test_managed_heap_memory();

	test_anonymous_shared_memory();

	char cc = 0;
	std::cin >> cc;

	return 0;
}
