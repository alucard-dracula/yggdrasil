// shared_ptr_serialization_test.cpp

//#define YGGR_SERIALIZE_SIZE32 //add this test 64bit ser_size_type is 32bit

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/base/yggrdef.h>
#include <yggr/archive/xml_archive_partner.hpp>
#include <yggr/packet/packet.hpp>
#include <yggr/serialization/nvp.hpp>

#include <yggr/charset/utf8_string.hpp>
#include <yggr/serialization/string.hpp>
#include <yggr/serialization/utf8_string.hpp>
#include <yggr/serialization/shared_ptr.hpp>
#include <yggr/serialization/vector.hpp>
#include <yggr/serialization/array.hpp>

#include <yggr/file_system/local_fsys_operators.hpp>

// warning: mingw use libboost_serialization-mgw47-mt-1_53.a //fix xml_gamma_error

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

typedef yggr::packet::packet<yggr::archive::archive_partner::xml_oarchive_partner> opak;
typedef yggr::packet::packet<yggr::archive::archive_partner::xml_iarchive_partner> ipak;

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

#if 0
struct F
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
	yggr::shared_ptr<int> pval; // track_never
};
#endif // 0, 1


void test_shared_ptr(void)
{
	E e1;

	opak o;
	o.save(YGGR_SERIALIZATION_NAME_NVP("shared_ptr", e1));

	std::cout << o.org_buf() << std::endl;

	E e3;

	ipak i(o);

	i.load(YGGR_SERIALIZATION_NAME_NVP("shared_ptr", e3));

	yggr_test_assert(!e3.pval);
	
}


void test_shared_ptr2(void)
{
	E e1;
	e1.pval = yggr::shared_ptr<in_val_t>(new in_val_t(100));
		
	opak o;
	o.save(YGGR_SERIALIZATION_NAME_NVP("shared_ptr", e1));

	std::cout << o.org_buf() << std::endl;

	E e3;

	ipak i(o);

	i.load(YGGR_SERIALIZATION_NAME_NVP("shared_ptr", e3));

	yggr_test_assert(e3.pval);
	yggr_test_assert(e3.pval->n == 100);
	
}

#if 0
void test_shared_ptr3(void)
{
	F f1;

	opak o;
	o.save(YGGR_SERIALIZATION_NAME_NVP("shared_ptr", f1));

	std::cout << o.org_buf() << std::endl;

	E f2;

	ipak i(o);

	i.load(YGGR_SERIALIZATION_NAME_NVP("shared_ptr", f2));

	yggr_test_assert(!f2.pval);
	
}

#endif // 0, 1

int main(int argc, char* argv[])
{
	test_shared_ptr();
	test_shared_ptr2();

	wait_any_key(argc, argv);
	return 0;
}
