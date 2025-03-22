//string_serialization_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <boost/mpl/bool.hpp>
#include <boost/utility/enable_if.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/packet/packet.hpp>

#include <yggr/archive/binary_archive_partner.hpp>
#include <yggr/archive/network_archive_partner.hpp>
#include <yggr/archive/text_archive_partner.hpp>
#include <yggr/archive/xml_archive_partner.hpp>

#if !(defined(YGGR_AT_ANDROID) || defined(YGGR_AT_IOS) || defined(YGGR_TEST_DISABLE_NON_GENERIC_SUPPORT))
#	include <yggr/archive/bson_archive_partner.hpp>
#endif // #if !(defined(YGGR_AT_ANDROID) || defined(YGGR_AT_IOS) || defined(YGGR_TEST_DISABLE_NON_GENERIC_SUPPORT))

#if 0
#	define YGGR_STRING_SERIALIZATION_NOT_PRIMITIVE
#endif // 0, 1

#include <yggr/serialization/char8_t.hpp>
#include <yggr/serialization/string.hpp>
#include <yggr/serialization/utf8_string.hpp>
#include <yggr/serialization/nvp.hpp>

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
		: stl_str("stl_str"),
			boost_str("boost_str"),
			yggr_str("yggr_str"),

#ifndef BOOST_NO_STD_WSTRING
			stl_wstr(L"stl_wstr"),
			boost_wstr(L"boost_wstr"),
			yggr_wstr(L"yggr_wstr"),
#endif // BOOST_NO_STD_WSTRING

#ifndef YGGR_NO_U8STRING_T
			stl_u8str(u8"stl_u8str"),
			boost_u8str(u8"boost_u8str"),
			yggr_u8str(u8"yggr_u8str"),
#endif // YGGR_NO_U8STRING_T

#ifndef YGGR_NO_U16STRING_T
			stl_u16str(u"stl_u16str"),
			boost_u16str(u"boost_u16str"),
			yggr_u16str(u"yggr_u16str"),
#endif // YGGR_NO_U16STRING_T

#ifndef YGGR_NO_U32STRING_T
			stl_u32str(U"stl_u32str"),
			boost_u32str(U"boost_u32str"),
			yggr_u32str(U"yggr_u32str"),
#endif // YGGR_NO_U32STRING_T

			yggr_utf8_str("yggr_utf8_str")
	{
		memset(org_str, 0, sizeof(org_str));
		std::string torg_str("org_str");
		memcpy(org_str, torg_str.data(), torg_str.size());

#ifndef BOOST_NO_CWCHAR
		memset(org_wstr, 0, sizeof(org_wstr));
		std::wstring torg_wstr(L"org_wstr");
		memcpy(org_wstr, torg_wstr.data(), torg_wstr.size() * sizeof(wchar_t));
#endif // BOOST_NO_CWCHAR

#ifndef YGGR_NO_CHAR8_T
		memset(org_u8str, 0, sizeof(org_u8str));
		std::u8string torg_u8str(u8"org_u8str");
		memcpy(org_u8str, torg_u8str.data(), torg_u8str.size() * sizeof(yggr::c8));
#endif // YGGR_NO_CHAR8_T

#ifndef YGGR_NO_CHAR16_T
		memset(org_u16str, 0, sizeof(org_u16str));
		std::u16string torg_u16str(u"org_u16str");
		memcpy(org_u16str, torg_u16str.data(), torg_u16str.size() * sizeof(yggr::c16));
#endif // YGGR_NO_CHAR16_T

#ifndef YGGR_NO_CHAR32_T
		memset(org_u32str, 0, sizeof(org_u32str));
		std::u32string torg_u32str(U"org_u32str");
		memcpy(org_u32str, torg_u32str.data(), torg_u32str.size() * sizeof(yggr::c32));
#endif // YGGR_NO_CHAR16_T

	}

	A(int n)
		: stl_str(""),
			boost_str(""),
			yggr_str(""),

#ifndef BOOST_NO_STD_WSTRING
			stl_wstr(L""),
			boost_wstr(L""),
			yggr_wstr(L""),
#endif // BOOST_NO_STD_WSTRING

#ifndef YGGR_NO_U8STRING_T
			stl_u8str(u8""),
			boost_u8str(u8""),
			yggr_u8str(u8""),
#endif // YGGR_NO_U8STRING_T

#ifndef YGGR_NO_U16STRING_T
			stl_u16str(u""),
			boost_u16str(u""),
			yggr_u16str(u""),
#endif // YGGR_NO_U16STRING_T

#ifndef YGGR_NO_U32STRING_T
			stl_u32str(U""),
			boost_u32str(U""),
			yggr_u32str(U""),
#endif // YGGR_NO_U32STRING_T

			yggr_utf8_str("")
	{
		memset(org_str, 0, sizeof(org_str));

#ifndef BOOST_NO_CWCHAR
		memset(org_wstr, 0, sizeof(org_wstr));
#endif // BOOST_NO_CWCHAR

#ifndef YGGR_NO_CHAR8_T
		memset(org_u8str, 0, sizeof(org_u8str));
#endif // YGGR_NO_CHAR8_T

#ifndef YGGR_NO_CHAR16_T
		memset(org_u16str, 0, sizeof(org_u16str));
#endif // YGGR_NO_CHAR16_T

#ifndef YGGR_NO_CHAR32_T
		memset(org_u32str, 0, sizeof(org_u32str));
#endif // YGGR_NO_CHAR16_T
	}

	~A()
	{
	}

	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & YGGR_SERIALIZATION_NVP(org_str);
		ar & YGGR_SERIALIZATION_NVP(stl_str);
		ar & YGGR_SERIALIZATION_NVP(boost_str);
		ar & YGGR_SERIALIZATION_NVP(yggr_str);

#ifndef BOOST_NO_CWCHAR
		ar & YGGR_SERIALIZATION_NVP(org_wstr);
#endif // BOOST_NO_CWCHAR

#ifndef BOOST_NO_STD_WSTRING
		ar & YGGR_SERIALIZATION_NVP(stl_wstr);
		ar & YGGR_SERIALIZATION_NVP(boost_wstr);
		ar & YGGR_SERIALIZATION_NVP(yggr_wstr);
#endif // BOOST_NO_STD_WSTRING

#ifndef YGGR_NO_CHAR8_T
		ar & YGGR_SERIALIZATION_NVP(org_u8str);
#endif // YGGR_NO_CHAR8_T

#ifndef YGGR_NO_U8STRING_T
		ar & YGGR_SERIALIZATION_NVP(stl_u8str);
		ar & YGGR_SERIALIZATION_NVP(boost_u8str);
		ar & YGGR_SERIALIZATION_NVP(yggr_u8str);
#endif // YGGR_NO_U8STRING_T

#ifndef YGGR_NO_CHAR16_T
		ar & YGGR_SERIALIZATION_NVP(org_u16str);
#endif // YGGR_NO_CHAR16_T

#ifndef YGGR_NO_U16STRING_T
		ar & YGGR_SERIALIZATION_NVP(stl_u16str);
		ar & YGGR_SERIALIZATION_NVP(boost_u16str);
		ar & YGGR_SERIALIZATION_NVP(yggr_u16str);
#endif // YGGR_NO_U16STRING_T

#ifndef YGGR_NO_CHAR32_T
		ar & YGGR_SERIALIZATION_NVP(org_u32str);
#endif // YGGR_NO_CHAR32_T

#ifndef YGGR_NO_U32STRING_T
		ar & YGGR_SERIALIZATION_NVP(stl_u32str);
		ar & YGGR_SERIALIZATION_NVP(boost_u32str);
		ar & YGGR_SERIALIZATION_NVP(yggr_u32str);
#endif // YGGR_NO_U32STRING_T

		ar & YGGR_SERIALIZATION_NVP(yggr_utf8_str);
	}

// char
	char org_str[20];
	std::string stl_str;
	boost::container::string boost_str;
	yggr::string yggr_str;

// wchar_t
#ifndef BOOST_NO_CWCHAR
	wchar_t org_wstr[20];
#endif // BOOST_NO_CWCHAR

#ifndef BOOST_NO_STD_WSTRING
	std::wstring stl_wstr;
	boost::container::wstring boost_wstr;
	yggr::wstring yggr_wstr;
#endif // BOOST_NO_STD_WSTRING

// c8
#ifndef YGGR_NO_CHAR8_T
	yggr::c8 org_u8str[20];
#endif // YGGR_NO_CHAR8_T

#ifndef YGGR_NO_U8STRING_T
	std::u8string stl_u8str;
	boost::container::u8string boost_u8str;
	yggr::u8string yggr_u8str;
#endif // YGGR_NO_U8STRING_T

// c16
#ifndef YGGR_NO_CHAR16_T
	yggr::c16 org_u16str[20];
#endif // YGGR_NO_CHAR16_T

#ifndef YGGR_NO_U16STRING_T
	std::u16string stl_u16str;
	boost::container::u16string boost_u16str;
	yggr::u16string yggr_u16str;
#endif // YGGR_NO_U16STRING_T

// c32
#ifndef YGGR_NO_CHAR32_T
	yggr::c16 org_u32str[20];
#endif // YGGR_NO_CHAR32_T

#ifndef YGGR_NO_U32STRING_T
	std::u32string stl_u32str;
	boost::container::u32string boost_u32str;
	yggr::u32string yggr_u32str;
#endif // YGGR_NO_U32STRING_T

// utf8_string
	yggr::utf8_string yggr_utf8_str;
};

bool operator==(const A& l, const A& r)
{
	bool bchk = true;

	bchk = bchk && (0 == memcmp(l.org_str, r.org_str, sizeof(l.org_str)));
	bchk = bchk && (l.stl_str == r.stl_str);
	bchk = bchk && (l.boost_str == r.boost_str);
	bchk = bchk && (l.yggr_str == r.yggr_str);

#ifndef BOOST_NO_CWCHAR
	bchk = bchk && (0 == memcmp(l.org_wstr, r.org_wstr, sizeof(l.org_wstr)));
#endif // BOOST_NO_CWCHAR

#ifndef BOOST_NO_STD_WSTRING
	bchk = bchk && (l.stl_wstr == r.stl_wstr);
	bchk = bchk && (l.boost_wstr == r.boost_wstr);
	bchk = bchk && (l.yggr_wstr == r.yggr_wstr);
#endif // BOOST_NO_STD_WSTRING

#ifndef YGGR_NO_CHAR8_T
	bchk = bchk && (0 == memcmp(l.org_u8str, r.org_u8str, sizeof(l.org_u8str)));
#endif // YGGR_NO_CHAR8_T

#ifndef YGGR_NO_U8STRING_T
	bchk = bchk && (l.stl_u8str == r.stl_u8str);
	bchk = bchk && (l.boost_u8str == r.boost_u8str);
	bchk = bchk && (l.yggr_u8str == r.yggr_u8str);
#endif // YGGR_NO_U8STRING_T

#ifndef YGGR_NO_CHAR16_T
	bchk = bchk && (0 == memcmp(l.org_u16str, r.org_u16str, sizeof(l.org_u16str)));
#endif // YGGR_NO_CHAR16_T

#ifndef YGGR_NO_U16STRING_T
	bchk = bchk && (l.stl_u16str == r.stl_u16str);
	bchk = bchk && (l.boost_u16str == r.boost_u16str);
	bchk = bchk && (l.yggr_u16str == r.yggr_u16str);
#endif // YGGR_NO_U16STRING_T

#ifndef YGGR_NO_CHAR32_T
	bchk = bchk && (0 == memcmp(l.org_u32str, r.org_u32str, sizeof(l.org_u32str)));
#endif // YGGR_NO_CHAR32_T

#ifndef YGGR_NO_U32STRING_T
	bchk = bchk && (l.stl_u32str == r.stl_u32str);
	bchk = bchk && (l.boost_u32str == r.boost_u32str);
	bchk = bchk && (l.yggr_u32str == r.yggr_u32str);
#endif // YGGR_NO_U32STRING_T

	bchk = bchk && (l.yggr_utf8_str == r.yggr_utf8_str);

	return bchk;
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

//#ifndef BOOST_NO_STD_WSTRING
//
//void test_binary_warchive(void)
//{
//	typedef yggr::packet::packet<yggr::archive::archive_partner::binary_woarchive_partner> opak_type;
//	typedef yggr::packet::packet<yggr::archive::archive_partner::binary_wiarchive_partner> ipak_type;
//
//	check_w<opak_type, ipak_type>();
//
//	std::cout << "test_binary_warchive success" << std::endl;
//}
//
//#endif // BOOST_NO_STD_WSTRING

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

//#ifndef BOOST_NO_STD_WSTRING
//
//void test_text_warchive(void)
//{
//	typedef yggr::packet::packet<yggr::archive::archive_partner::text_woarchive_partner> opak_type;
//	typedef yggr::packet::packet<yggr::archive::archive_partner::text_wiarchive_partner> ipak_type;
//
//	check_w<opak_type, ipak_type>();
//
//	std::cout << "test_text_warchive success" << std::endl;
//}
//
//#endif // BOOST_NO_STD_WSTRING

void test_xml_archive(void)
{
	typedef yggr::packet::packet<yggr::archive::archive_partner::xml_oarchive_partner> opak_type;
	typedef yggr::packet::packet<yggr::archive::archive_partner::xml_iarchive_partner> ipak_type;

	check<opak_type, ipak_type>();

	std::cout << "test_xml_archive success" << std::endl;
}

//#ifndef BOOST_NO_STD_WSTRING
//
//void test_xml_warchive(void)
//{
//	typedef yggr::packet::packet<yggr::archive::archive_partner::xml_woarchive_partner> opak_type;
//	typedef yggr::packet::packet<yggr::archive::archive_partner::xml_wiarchive_partner> ipak_type;
//
//	check_w<opak_type, ipak_type>();
//
//	std::cout << "test_xml_warchive success" << std::endl;
//}
//
//#endif // BOOST_NO_STD_WSTRING

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
	test_binary_archive();
	test_network_archive();
	test_text_archive();
	test_xml_archive();

#if !(defined(YGGR_AT_ANDROID) || defined(YGGR_AT_IOS) || defined(YGGR_TEST_DISABLE_NON_GENERIC_SUPPORT))
	test_bson_archive();
#endif // #if !(defined(YGGR_AT_ANDROID) || defined(YGGR_AT_IOS) || defined(YGGR_TEST_DISABLE_NON_GENERIC_SUPPORT))

//#ifndef BOOST_NO_STD_WSTRING
//	test_binary_warchive();
//	test_text_warchive();
//	test_xml_warchive();
//#endif // BOOST_NO_STD_WSTRING

	wait_any_key(argc, argv);
	return 0;
}
