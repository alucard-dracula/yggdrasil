// shared_ptr_bson_serialization_test.cpp

#define YGGR_SERIALIZE_SIZE32

#include <test/wait_any_key/wait_any_key.hpp>
#include <string>
#include <vector>

#include <yggr/base/yggrdef.h>

#include <yggr/packet/packet.hpp>
#include <yggr/archive/network_archive_partner.hpp>

#include <yggr/archive/bson_oarchive.hpp>
#include <yggr/archive/bson_iarchive.hpp>

#include <yggr/serialization/nvp.hpp>
#include <yggr/archive/bson_archive_partner.hpp>

#include <yggr/serialization/string.hpp>
#include <yggr/serialization/utf8_string.hpp>

#include <yggr/serialization/shared_ptr.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

typedef yggr::packet::packet<yggr::archive::archive_partner::network_oarchive_partner> opacket_type;
typedef yggr::packet::packet<yggr::archive::archive_partner::network_iarchive_partner> ipacket_type;

typedef yggr::packet::packet<yggr::archive::archive_partner::bson_oarchive_partner> o_bson_packet_type;
typedef yggr::packet::packet<yggr::archive::archive_partner::bson_iarchive_partner> i_bson_packet_type;


struct in_val_t
{
	in_val_t(void)
		: n(0)
	{
	}

	in_val_t(int nn)
		: n(nn)
	{
	}

	in_val_t(const in_val_t& right)
		: n(right.n)
	{
	}
private:
	friend class boost::serialization::access;
	template<typename Archive>
	void serialize(Archive & ar, yggr::u32 version)
	{
		ar & YGGR_SERIALIZATION_NVP(n);
	}
public:
	int n;
};

struct E
{
public:

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & YGGR_SERIALIZATION_NVP(pval);
	}

public:
	yggr::shared_ptr<in_val_t> pval;
};

void test_shared_ptr(void)
{
	E e1;
	e1.pval = yggr::shared_ptr<in_val_t>(new in_val_t(100));
		
	o_bson_packet_type o;
	o.save(e1);

	std::cout << o.org_buf() << std::endl;

	E e3;

	i_bson_packet_type i(o);

	i.load(e3);

	yggr_test_assert(e3.pval);
	yggr_test_assert(e3.pval->n == 100);
	
}

void test_shared_empty_ptr(void)
{
	E e1;

	o_bson_packet_type o;
	o.save(e1);

	std::cout << o.org_buf() << std::endl;

	E e3;

	i_bson_packet_type i(o);

	i.load(e3);

	yggr_test_assert(!e3.pval);
	
}

int main(int argc, char* argv[])
{
	test_shared_ptr();
	test_shared_empty_ptr();

	wait_any_key(argc, argv);
	return 0;
}