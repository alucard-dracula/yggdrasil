// time_serialization_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/base/yggrdef.h>

#include <yggr/archive/xml_archive_partner.hpp>

//#if !(defined(YGGR_AT_ANDROID) || defined(YGGR_AT_IOS) || defined(YGGR_TEST_DISABLE_NON_GENERIC_SUPPORT))
#	include <yggr/archive/bson_archive_partner.hpp>
//#endif //#if !(defined(YGGR_AT_ANDROID) || defined(YGGR_AT_IOS) || defined(YGGR_TEST_DISABLE_NON_GENERIC_SUPPORT))

#include <yggr/packet/packet.hpp>

#include <yggr/serialization/nvp.hpp>

#include <yggr/serialization/time.hpp>
#include <yggr/serialization/utility.hpp>

// warning: mingw use libboost_serialization-mgw47-mt-1_53.a //fix xml_gamma_error

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

typedef yggr::packet::packet<yggr::archive::archive_partner::xml_oarchive_partner> opak_type;
typedef yggr::packet::packet<yggr::archive::archive_partner::xml_iarchive_partner> ipak_type;

//#if !(defined(YGGR_AT_ANDROID) || defined(YGGR_AT_IOS) || defined(YGGR_TEST_DISABLE_NON_GENERIC_SUPPORT))

typedef yggr::packet::packet<yggr::archive::archive_partner::bson_oarchive_partner> bson_opak_type;
typedef yggr::packet::packet<yggr::archive::archive_partner::bson_iarchive_partner> bson_ipak_type;

typedef yggr::packet::packet<yggr::archive::archive_partner::bson_nld_oarchive_partner> bson_nld_opak_type;

//#endif //#if !(defined(YGGR_AT_ANDROID) || defined(YGGR_AT_IOS) || defined(YGGR_TEST_DISABLE_NON_GENERIC_SUPPORT))

typedef yggr::time::time time_type;

struct A
{
	A(void)
		: tm(0, 0)
	{
	}

	A(const time_type& ntm)
		: tm(ntm)
	{
	}

	~A(void)
	{
	}

	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & YGGR_SERIALIZATION_NVP(tm);
	}

	time_type tm;
};


void test_foo1(void)
{
	time_type tm_src;
	opak_type o;

	o.save(YGGR_SERIALIZATION_NAME_NVP("time_data", tm_src));

	std::cout << o.org_buf() << std::endl;

	ipak_type i(o);
	time_type tm_dst(0, 0);

	i.load(YGGR_SERIALIZATION_NAME_NVP("time_data", tm_dst));

	yggr_test_assert(tm_src == tm_dst);

	std::cout << tm_dst.to_string() << std::endl;
}

//#if !(defined(YGGR_AT_ANDROID) || defined(YGGR_AT_IOS) || defined(YGGR_TEST_DISABLE_NON_GENERIC_SUPPORT))

void test_foo1_bson(void)
{
	time_type tm_src;
	bson_opak_type o;

	o.save(YGGR_SERIALIZATION_NAME_NVP("time_data", tm_src));

	std::cout << o.org_buf() << std::endl;

	bson_ipak_type i(o);
	time_type tm_dst(0, 0);

	i.load(YGGR_SERIALIZATION_NAME_NVP("time_data", tm_dst));

	yggr_test_assert(tm_src == tm_dst);

	std::cout << tm_dst.to_string() << std::endl;
}

void test_foo1_bson_nld(void)
{
	time_type tm_src;
	bson_nld_opak_type o;

	o.save(YGGR_SERIALIZATION_NAME_NVP("time_data", tm_src));

	std::cout << o.org_buf() << std::endl;
}

//#endif //#if !(defined(YGGR_AT_ANDROID) || defined(YGGR_AT_IOS) || defined(YGGR_TEST_DISABLE_NON_GENERIC_SUPPORT))

void test_foo2(void)
{
	A a = time_type();
	opak_type o;
	o.save(YGGR_SERIALIZATION_NAME_NVP("A", a));

	std::cout << o.org_buf() << std::endl;

	ipak_type i(o);

	A a2;
	i.load(YGGR_SERIALIZATION_NAME_NVP("A", a2));

	yggr_test_assert((a.tm == a2.tm));

	std::cout << a2.tm.to_string() << std::endl;
}

//#if !(defined(YGGR_AT_ANDROID) || defined(YGGR_AT_IOS) || defined(YGGR_TEST_DISABLE_NON_GENERIC_SUPPORT))

void test_foo2_bson(void)
{
	A a = time_type();
	bson_opak_type o;
	o.save(YGGR_SERIALIZATION_NAME_NVP("A", a));

	std::cout << o.org_buf() << std::endl;

	bson_ipak_type i(o);

	A a2;
	i.load(YGGR_SERIALIZATION_NAME_NVP("A", a2));

	yggr_test_assert(a.tm == a2.tm);

	std::cout << a2.tm.to_string() << std::endl;
}

void test_foo2_bson_nld(void)
{
	A a = time_type();
	bson_nld_opak_type o;
	o.save(YGGR_SERIALIZATION_NAME_NVP("A", a));

	std::cout << o.org_buf() << std::endl;
}

//#endif //#if !(defined(YGGR_AT_ANDROID) || defined(YGGR_AT_IOS) || defined(YGGR_TEST_DISABLE_NON_GENERIC_SUPPORT))


int main(int argc, char* argv[])
{
	test_foo1();

//#if !(defined(YGGR_AT_ANDROID) || defined(YGGR_AT_IOS) || defined(YGGR_TEST_DISABLE_NON_GENERIC_SUPPORT))
	test_foo1_bson();
	test_foo1_bson_nld();
//#endif //#if !(defined(YGGR_AT_ANDROID) || defined(YGGR_AT_IOS) || defined(YGGR_TEST_DISABLE_NON_GENERIC_SUPPORT))

	test_foo2();

//#if !(defined(YGGR_AT_ANDROID) || defined(YGGR_AT_IOS) || defined(YGGR_TEST_DISABLE_NON_GENERIC_SUPPORT))
	test_foo2_bson();
	test_foo2_bson_nld();
//#endif //#if !(defined(YGGR_AT_ANDROID) || defined(YGGR_AT_IOS) || defined(YGGR_TEST_DISABLE_NON_GENERIC_SUPPORT))

	wait_any_key(argc, argv);
	return 0;
}
