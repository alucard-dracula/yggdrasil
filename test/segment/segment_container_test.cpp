//segment_container_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/segment/segment_container.hpp>
#include <yggr/segment/segment_op_proxy.hpp>
#include <yggr/segment/segment_op.hpp>
#include <yggr/segment/segment_mode.hpp>

#include <yggr/smart_ptr_ex/shared_ptr.hpp>

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
void destory_segment(const std::string& name)
{
	typedef Seg seg_type;
	typedef yggr::segment::segment_destroyer<seg_type> segment_destroyer_type;
	{ segment_destroyer_type destoryer(name.c_str()); }
}

void test_shared_memory_object1(void)
{
	typedef boost::interprocess::shared_memory_object seg_type;
	typedef yggr::segment::segment_container<seg_type> seg_cont_type;

	seg_cont_type cont("test_shared_memory_object",
							yggr::segment::segment_op::create_only(),
							yggr::segment::segment_mode::E_read_write,
							boost::bind(&destory_segment<seg_type>,
											"test_shared_memory_object"));

	try
	{
		cont.segment().truncate(65536);
	}
	catch(const boost::interprocess::interprocess_exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	yggr_test_assert(cont.is_named_enable_object() && !cont.empty());
	std::cout << cont.name() << std::endl; //test_shared_memory_object
	std::cout << cont.segment().get_name() << std::endl; //test_shared_memory_object
	std::cout << cont.size() << std::endl;

	cont.free();

	yggr_test_assert(cont.is_named_enable_object());
	yggr_test_assert(cont.empty());
}

void test_shared_memory_object2(void)
{
	typedef boost::interprocess::shared_memory_object seg_type;
	typedef yggr::segment::segment_container<seg_type> seg_cont_type;

	seg_cont_type cont("test_shared_memory_object",
							yggr::segment::segment_op::create_only(),
							yggr::segment::segment_mode::E_read_write);

	try
	{
		cont.segment().truncate(65536);
	}
	catch(const boost::interprocess::interprocess_exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	yggr_test_assert(cont.is_named_enable_object() && !cont.empty());
	std::cout << cont.name() << std::endl; //test_shared_memory_object
	std::cout << cont.segment().get_name() << std::endl; //test_shared_memory_object
	std::cout << cont.size() << std::endl;

	cont.free();

	yggr_test_assert(cont.is_named_enable_object());
	yggr_test_assert(cont.empty());
}

void test_managed_shared_memory1(void)
{
	typedef boost::interprocess::managed_shared_memory seg_type;
	typedef yggr::segment::segment_container<seg_type> seg_cont_type;

	seg_cont_type cont("test_managed_shared_memory",
							yggr::segment::segment_op::create_only(), 65536,
							boost::bind(&destory_segment<seg_type>, "test_managed_shared_memory"));

	yggr_test_assert(cont.is_named_enable_object() && !cont.empty());
	std::cout << cont.name() << std::endl;
	std::cout << cont.size() << std::endl;

	cont.free();
	yggr_test_assert(cont.is_named_enable_object());
	yggr_test_assert(cont.empty());
}

void test_managed_shared_memory2(void)
{
	typedef boost::interprocess::managed_shared_memory seg_type;
	typedef yggr::segment::segment_container<seg_type> seg_cont_type;

	seg_cont_type cont("test_managed_shared_memory",
							yggr::segment::segment_op::create_only(), 65536);

	yggr_test_assert(cont.is_named_enable_object() && !cont.empty());
	std::cout << cont.name() << std::endl;
	std::cout << cont.size() << std::endl;

	cont.free();
	yggr_test_assert(cont.is_named_enable_object());
	yggr_test_assert(cont.empty());
}

void test_file_mapping(void)
{
	typedef boost::interprocess::file_mapping seg_type;
	typedef yggr::segment::segment_container<seg_type> seg_cont_type;

	seg_cont_type cont("../test_out/segment/test_file_mapping",
						yggr::segment::segment_op::create_only(),
						65536,
						yggr::segment::segment_mode::E_read_write,
						boost::bind(&destory_segment<seg_type>,
						"../test_out/segment/test_file_mapping") );

	yggr_test_assert(cont.is_named_enable_object() && !cont.empty());
	std::cout << cont.name() << std::endl;
	std::cout << cont.size() << std::endl;

	cont.free();
	yggr_test_assert(cont.is_named_enable_object());
	yggr_test_assert(cont.empty());
}

void test_managed_mapped_file(void)
{
	typedef boost::interprocess::managed_mapped_file seg_type;
	typedef yggr::segment::segment_container<seg_type> seg_cont_type;

	seg_cont_type cont("../test_out/segment/test_mamaged_mapped_file",
						yggr::segment::segment_op::create_only(),
						65536,
						boost::bind(&destory_segment<seg_type>,
						"../test_out/segment/test_mamaged_mapped_file"));

	yggr_test_assert(cont.is_named_enable_object() && !cont.empty());
	std::cout << cont.name() << std::endl;
	std::cout << cont.size() << std::endl;

	cont.free();
	yggr_test_assert(cont.is_named_enable_object());
	yggr_test_assert(cont.empty());
}


//#include <boost/interprocess/allocators/allocator.hpp>
//#include <boost/interprocess/managed_external_buffer.hpp>
//#include <yggr/segment/anonymous_segment_container.hpp>

//#include <yggr/ppex/pp_debug.hpp>
void test_managed_external_buffer(void)
{
	typedef boost::interprocess::managed_external_buffer seg_type;
	typedef yggr::segment::segment_container<seg_type> seg_cont_type;
	typedef yggr::aligned_storage<65536> aligned_type;
	typedef aligned_type::type buffer_type;

	typedef yggr::shared_ptr<buffer_type> buf_ptr_type;

#if 0
	// boost/interprocess/managed_external_buffer.hpp line 67
	// 32bit app stack align failed (  BOOST_ASSERT((0 == (((std::size_t)addr) & (AllocationAlgorithm::Alignment - size_type(1u)))));)
	buffer_type buf;
	seg_cont_type cont(yggr::segment::segment_op::create_only(), buf.address(), aligned_type::size);
#else
	buf_ptr_type pbuf(yggr_nothrow_new buffer_type());
	seg_cont_type cont(yggr::segment::segment_op::create_only(), pbuf->address(), aligned_type::size);
#endif // 0, 1

	yggr_test_assert(!cont.is_named_enable_object() && !cont.empty());
	std::cout << "test_managed_external_buffer" << std::endl;
	std::cout << cont.size() << std::endl;

	cont.free();

	yggr_test_assert(!cont.is_named_enable_object());
	yggr_test_assert(cont.empty());
}

void test_managed_heap_memory(void)
{
	typedef boost::interprocess::managed_heap_memory seg_type;
	typedef yggr::segment::segment_container<seg_type> seg_cont_type;

	seg_cont_type cont(65536);

	yggr_test_assert(!cont.is_named_enable_object() && !cont.empty());
	std::cout << "test_managed_heap_memory" << std::endl;
	std::cout << cont.size() << std::endl;

	cont.free();

	yggr_test_assert(!cont.is_named_enable_object());
	yggr_test_assert(cont.empty());
}

//#include <yggr/segment/anonymous_shared_memory.hpp>
void test_anonymous_shared_memory(void)
{
	typedef yggr::segment::anonymous_shared_memory seg_type;
	typedef yggr::segment::segment_container<seg_type> seg_cont_type;

	seg_cont_type cont(65536);

	yggr_test_assert(!cont.is_named_enable_object() && !cont.empty());
	std::cout << "test_anonymous_shared_memory" << std::endl;
	std::cout << cont.size() << std::endl;

	cont.free();

	yggr_test_assert(!cont.is_named_enable_object());
	yggr_test_assert(cont.empty());
}

void test_message_queue(void)
{
	typedef boost::interprocess::message_queue seg_type;
	typedef yggr::segment::segment_container<seg_type> seg_cont_type;

	seg_cont_type cont("test_anonymous_shared_memory",
						yggr::segment::segment_op::create_only(),
						20,
						sizeof(int));

	yggr_test_assert(cont.is_named_enable_object() && !cont.empty());
	std::cout << cont.name() << std::endl;
	std::cout << cont.size() << std::endl;

	cont.free();

	yggr_test_assert(cont.is_named_enable_object());
	yggr_test_assert(cont.empty());
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

	test_message_queue();

	wait_any_key(argc, argv);
	return 0;
}
