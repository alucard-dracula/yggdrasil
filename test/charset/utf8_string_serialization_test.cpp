//string_serialization_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>


#include <yggr/base/yggrdef.h>
#include <yggr/packet/packet.hpp>

#include <yggr/archive/binary_archive_partner.hpp>
#include <yggr/archive/network_archive_partner.hpp>
#include <yggr/archive/text_archive_partner.hpp>
#include <yggr/archive/xml_archive_partner.hpp>

#if !(defined(YGGR_AT_ANDROID) || defined(YGGR_AT_IOS) || defined(YGGR_TEST_DISABLE_NON_GENERIC_SUPPORT))
#	include <yggr/archive/bson_archive_partner.hpp>
#endif //#if !(defined(YGGR_AT_ANDROID) || defined(YGGR_AT_IOS) || defined(YGGR_TEST_DISABLE_NON_GENERIC_SUPPORT))

#include <yggr/serialization/nvp.hpp>

#if 0
#	define YGGR_STRING_SERIALIZATION_NOT_PRIMITIVE
#endif // 0, 1

#include <yggr/serialization/utf8_string.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#   include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

struct A
{
	A(void)
		: yggr_utf8_str("u")
	{
	}

	A(int n)
		: yggr_utf8_str()
	{
	}

	~A(void)
	{
	}

	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & YGGR_SERIALIZATION_NVP(yggr_utf8_str);
	}

	yggr::utf8_string yggr_utf8_str;

};

bool operator==(const A& l, const A& r)
{
	return l.yggr_utf8_str == r.yggr_utf8_str;
}

template<typename OPak, typename IPak>
void check(void)
{
	typedef OPak opak_type;
	typedef IPak ipak_type;
	A a;
	opak_type o;
	o.save(YGGR_SERIALIZATION_NAME_NVP("A", a));
	std::cout << o.org_buf() << std::endl;

	ipak_type i(o);

	A a2(0);

	yggr_test_assert(!(a == a2));

	i.load(YGGR_SERIALIZATION_NAME_NVP("A", a2));

	yggr_test_assert(a == a2);
}

#ifndef BOOST_NO_STD_WSTRING

template<typename OPak, typename IPak>
void check_w(void)
{
	typedef OPak opak_type;
	typedef IPak ipak_type;
	A a;
	opak_type o;
	o.save(YGGR_SERIALIZATION_NAME_NVP("A", a));

	std::wcout << o.org_buf() << std::endl;

	ipak_type i(o);

	A a2(0);

	yggr_test_assert(!(a == a2));

	i.load(YGGR_SERIALIZATION_NAME_NVP("A", a2));

	yggr_test_assert(a == a2);
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
