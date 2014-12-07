//utf8_char_test.cpp

#ifndef _MSC_VER
#	error "this file test msvc only !!!!"
#endif // _MSC_VER

#include <iostream>

#include <boost/interprocess/shared_memory_object.hpp>
#include <yggr/segment/segment_container.hpp>
#include <yggr/segment/segment_op_proxy.hpp>
#include <yggr/segment/segment_op.hpp>
#include <yggr/segment/segment_mode.hpp>
#include <boost/bind.hpp>

#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
//#include <boost/container/string.hpp>
#include <yggr/charset/string.hpp>
#include <boost/move/move.hpp>
#include <boost/container/vector.hpp>

#ifdef _MSC_VER
#pragma comment(lib, "libyggr_base-vc100-d.lib")
#pragma comment(lib, "libyggr_time-vc100-d.lib")
#endif // _MSC_VER

template<typename Seg>
void delete_segment(const std::string& name)
{
	typedef Seg seg_type;
	typedef yggr::segment::segment_deleter<std::string, seg_type> deleter_type;
	{ deleter_type del(name); }
}

void test_managed_shared_memory(void)
{
	typedef boost::container::vector<int> vt_type;
	typedef vt_type::iterator::value_type vt_val_type;
	typedef yggr::string::iterator iter_type;
	//typedef iter_type::value_type val_type;
	typedef boost::interprocess::managed_shared_memory seg_type;
	typedef yggr::segment::segment_container<std::string, seg_type> seg_cont_type;
	typedef boost::interprocess::allocator<char, seg_type::segment_manager> alloc_type;

	typedef yggr::charset::basic_string<char, std::char_traits<char>, std::allocator<char> > string_type;
	typedef yggr::charset::basic_string<char, std::char_traits<char>, alloc_type> memstring_type;


	seg_cont_type cont("test_managed_shared_memory", boost::bind(&delete_segment<seg_type>, "test_managed_shared_memory"),
							yggr::segment::segment_op::create_only, 65536);

	assert(cont.is_named_object() && !cont.empty());
	std::cout << "cont name" << cont.name() << std::endl;
	std::cout << "cont size" << cont.size() << std::endl;

	//boost::container::string str("abc");
	yggr::string str("abc");
	memstring_type* pstr = cont.segment().construct<memstring_type>("memstring")(str.begin(), str.end(), alloc_type(cont.segment().get_segment_manager()));

	assert(pstr);

	//pstr->assign(str); //not enable
	assert(str.size() == 3);

	std::cout << *pstr << std::endl;

	cont.segment().destroy_ptr(pstr);
	pstr = 0;


	cont.free();
	assert(cont.is_named_object());
	assert(cont.empty());

	std::cout << "--------------------------test_managed_shared_memory end----------------------" << std::endl;
}

#include <yggr/charset/utf8_char_impl.hpp>
#include <yggr/charset/utf8_string_impl.hpp>


void test_utf8_char(void)
{
	typedef yggr::charset::utf8_char_impl<> utf8_char_type;
	std::cout << typeid(yggr::string::const_iterator).name() << std::endl;
	utf8_char_type c1, c2;
	c1.swap(c2);
}

int main(int argc, char* argv[])
{
	test_managed_shared_memory();
	test_utf8_char();
	char cc = 0;
	std::cin >> cc;
	return 0;
}
