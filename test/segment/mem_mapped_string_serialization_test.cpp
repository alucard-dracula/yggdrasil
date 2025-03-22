//mem_mapped_string_serialization_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <boost/bind.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/packet/packet.hpp>

#include <yggr/segment/segment_info.hpp>
#include <yggr/segment/segment_container.hpp>
#include <yggr/segment/segment_op_proxy.hpp>
#include <yggr/segment/segment_op.hpp>
#include <yggr/segment/segment_mode.hpp>

#include <yggr/archive/binary_archive_partner.hpp>
#include <yggr/archive/network_archive_partner.hpp>
#include <yggr/archive/text_archive_partner.hpp>
#include <yggr/archive/xml_archive_partner.hpp>

#if !(defined(YGGR_AT_ANDROID) || defined(YGGR_AT_IOS) || defined(YGGR_TEST_DISABLE_NON_GENERIC_SUPPORT))
#	include <yggr/archive/bson_archive_partner.hpp>
#endif // #if !(defined(YGGR_AT_ANDROID) || defined(YGGR_AT_IOS) || defined(YGGR_TEST_DISABLE_NON_GENERIC_SUPPORT))

#include <yggr/serialization/nvp.hpp>

#if 1
#	define YGGR_STRING_SERIALIZATION_NOT_PRIMITIVE
#endif // 0, 1

#include <yggr/serialization/string.hpp>
#include <yggr/serialization/utf8_string.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#   include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(segment)
#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
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


template<typename Alloc>
struct A
{
	typedef Alloc alloc_type;
	typedef boost::container::basic_string<char, std::char_traits<char>, alloc_type> boost_string_type;
	typedef yggr::charset::utf8_string_impl<char, std::char_traits<char>, alloc_type> yggr_utf8_string_type;


	A(const alloc_type& alloc)
		: boost_str("boost_str", alloc), 
			yggr_utf8_str("yggr_utf8_str", alloc)
	{
		std::cout << (boost::serialization::implementation_level< yggr_utf8_string_type >::value) << std::endl;
	}

	A(int n, const alloc_type& alloc)
		: boost_str("", alloc), 
			yggr_utf8_str("", alloc)
	{
	}

	~A()
	{
	}

	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & YGGR_SERIALIZATION_NVP(boost_str);
		ar & YGGR_SERIALIZATION_NVP(yggr_utf8_str);
	}

	boost_string_type boost_str;
	yggr_utf8_string_type yggr_utf8_str;

};

template<typename Alloc>
bool operator==(const A<Alloc>& l, const A<Alloc>& r)
{
	return l.boost_str == r.boost_str
			&& l.yggr_utf8_str == r.yggr_utf8_str;
}

template<typename OPak, typename IPak>
void check(void)
{
	typedef OPak opak_type;
	typedef IPak ipak_type;

	typedef boost::interprocess::managed_shared_memory seg_type;
	typedef yggr::segment::segment_container<seg_type> seg_cont_type;
	typedef boost::interprocess::allocator<char, seg_type::segment_manager> alloc_type;

	typedef A<alloc_type> a_type;

	seg_cont_type cont("test_managed_shared_memory",
							yggr::segment::segment_op::create_only(), 
							65536,
							boost::bind(&delete_segment<seg_type>, 
											"test_managed_shared_memory") );

	yggr_test_assert(cont.is_named_enable_object() && !cont.empty());
	a_type* pa_i = cont.segment().template construct<a_type>("test_a_i")(alloc_type(cont.segment().get_segment_manager()));
	a_type* pa_o = cont.segment().template construct<a_type>("test_a_o")(0, alloc_type(cont.segment().get_segment_manager()));

	//start test
	opak_type o;
	o.save(YGGR_SERIALIZATION_NAME_NVP("A", *pa_i));

	std::cout << o.org_buf() << "\n" << std::endl;

	ipak_type i(o);

	yggr_test_assert(!(*pa_i == *pa_o));

	i.load(YGGR_SERIALIZATION_NAME_NVP("A", *pa_o));

	yggr_test_assert((*pa_i == *pa_o));

	//end test

	cont.segment().destroy_ptr(pa_i);
	cont.segment().destroy_ptr(pa_o);
	pa_i = 0;
	pa_o = 0;

	cont.free();
	yggr_test_assert(cont.is_named_enable_object());
	yggr_test_assert(cont.empty());
}

#ifndef BOOST_NO_STD_WSTRING

template<typename OPak, typename IPak>
void check_w(void)
{
	typedef OPak opak_type;
	typedef IPak ipak_type;

	typedef boost::interprocess::managed_shared_memory seg_type;
	typedef yggr::segment::segment_container<seg_type> seg_cont_type;
	typedef boost::interprocess::allocator<char, seg_type::segment_manager> alloc_type;

	typedef A<alloc_type> a_type;

	seg_cont_type cont("test_managed_shared_memory",
							yggr::segment::segment_op::create_only(), 
							65536,
							boost::bind(&delete_segment<seg_type>, 
											"test_managed_shared_memory") );

	yggr_test_assert(cont.is_named_enable_object() && !cont.empty());
	a_type* pa_i = cont.segment().template construct<a_type>("test_a_i")(alloc_type(cont.segment().get_segment_manager()));
	a_type* pa_o = cont.segment().template construct<a_type>("test_a_o")(0, alloc_type(cont.segment().get_segment_manager()));

	//start test
	opak_type o;
	o.save(YGGR_SERIALIZATION_NAME_NVP("A", *pa_i));
	
	std::wcout << o.org_buf() << L"\n" << std::endl;

	ipak_type i(o);

	yggr_test_assert(!(*pa_i == *pa_o));

	i.load(YGGR_SERIALIZATION_NAME_NVP("A", *pa_o));

	yggr_test_assert((*pa_i == *pa_o));

	//end test

	cont.segment().destroy_ptr(pa_i);
	cont.segment().destroy_ptr(pa_o);
	pa_i = 0;
	pa_o = 0;

	cont.free();
	yggr_test_assert(cont.is_named_enable_object());
	yggr_test_assert(cont.empty());
}

#endif // BOOST_NO_STD_WSTRING

void test_binary_archive(void)
{
	typedef yggr::packet::packet<yggr::archive::archive_partner::binary_oarchive_partner> opak_type;
	typedef yggr::packet::packet<yggr::archive::archive_partner::binary_iarchive_partner> ipak_type;

	check<opak_type, ipak_type>();

	std::cout << "test_binary_archive success" << std::endl;
}

#ifndef BOOST_NO_STD_WSTRING

void test_binary_warchive(void)
{
	typedef yggr::packet::packet<yggr::archive::archive_partner::binary_woarchive_partner> opak_type;
	typedef yggr::packet::packet<yggr::archive::archive_partner::binary_wiarchive_partner> ipak_type;

	check_w<opak_type, ipak_type>();

	std::cout << "test_binary_warchive success" << std::endl;
}

#endif // BOOST_NO_STD_WSTRING


void test_network_archive(void)
{
	typedef yggr::packet::packet<yggr::archive::archive_partner::network_oarchive_partner> opak_type;
	typedef yggr::packet::packet<yggr::archive::archive_partner::network_iarchive_partner> ipak_type;

	check<opak_type, ipak_type>();

	std::cout << "test_network_archive success" << std::endl;
}

void test_text_archive(void)
{
	typedef yggr::packet::packet<yggr::archive::archive_partner::text_oarchive_partner> opak_type;
	typedef yggr::packet::packet<yggr::archive::archive_partner::text_iarchive_partner> ipak_type;

	check<opak_type, ipak_type>();

	std::cout << "test_text_archive success" << std::endl;
}

#ifndef BOOST_NO_STD_WSTRING

void test_text_warchive(void)
{
	typedef yggr::packet::packet<yggr::archive::archive_partner::text_woarchive_partner> opak_type;
	typedef yggr::packet::packet<yggr::archive::archive_partner::text_wiarchive_partner> ipak_type;

	check_w<opak_type, ipak_type>();

	std::cout << "test_text_warchive success" << std::endl;
}

#endif // BOOST_NO_STD_WSTRING


void test_xml_archive(void)
{
	typedef yggr::packet::packet<yggr::archive::archive_partner::xml_oarchive_partner> opak_type;
	typedef yggr::packet::packet<yggr::archive::archive_partner::xml_iarchive_partner> ipak_type;

	check<opak_type, ipak_type>();

	std::cout << "test_xml_archive success" << std::endl;
}

#ifndef BOOST_NO_STD_WSTRING

void test_xml_warchive(void)
{
	typedef yggr::packet::packet<yggr::archive::archive_partner::xml_woarchive_partner> opak_type;
	typedef yggr::packet::packet<yggr::archive::archive_partner::xml_wiarchive_partner> ipak_type;

	check_w<opak_type, ipak_type>();

	std::cout << "test_xml_warchive success" << std::endl;
}

#endif // BOOST_NO_STD_WSTRING

#if !(defined(YGGR_AT_ANDROID) || defined(YGGR_AT_IOS) || defined(YGGR_TEST_DISABLE_NON_GENERIC_SUPPORT))

void test_bson_archive(void)
{
	typedef yggr::packet::packet<yggr::archive::archive_partner::bson_oarchive_partner> opak_type;
	typedef yggr::packet::packet<yggr::archive::archive_partner::bson_iarchive_partner> ipak_type;

	check<opak_type, ipak_type>();

	std::cout << "test_bson_archive success" << std::endl;
}

#endif // #if !(defined(YGGR_AT_ANDROID) || defined(YGGR_AT_IOS) || defined(YGGR_TEST_DISABLE_NON_GENERIC_SUPPORT))

int main(int argc, char* argv[])
{

#ifndef YGGR_NO_CWCHAR
	TEST_PP_WIO_IMBUE("chs");
#endif // YGGR_NO_CWCHAR

	test_binary_archive();
	test_network_archive();
	test_text_archive();
	test_xml_archive();

#if !(defined(YGGR_AT_ANDROID) || defined(YGGR_AT_IOS) || defined(YGGR_TEST_DISABLE_NON_GENERIC_SUPPORT))
	test_bson_archive();
#endif // #if !(defined(YGGR_AT_ANDROID) || defined(YGGR_AT_IOS) || defined(YGGR_TEST_DISABLE_NON_GENERIC_SUPPORT))

#ifndef BOOST_NO_STD_WSTRING
	test_binary_warchive();
	test_text_warchive();
	test_xml_warchive();
#endif // BOOST_NO_STD_WSTRING

	wait_any_key(argc, argv);
	return 0;
}
