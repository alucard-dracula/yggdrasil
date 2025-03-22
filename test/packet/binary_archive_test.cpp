//text_archive_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/base/yggrdef.h>
#include <yggr/packet/packet.hpp>
#include <yggr/archive/binary_archive_partner.hpp>
#include <yggr/archive/xml_archive_partner.hpp>
#include <yggr/archive/network_archive_partner.hpp>

#include <boost/serialization/shared_ptr.hpp>
#include <yggr/serialization/nvp.hpp>
#include <yggr/serialization/vector.hpp>
#include <yggr/serialization/string.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#   include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)


struct C
{
	C(void)
		: n(200)
	{
	}

	C(int n)
		: n(0)
	{
	}

	~C(void)
	{
	}

	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & YGGR_SERIALIZATION_NVP(n);
	}

	int n;
};

BOOST_SERIALIZATION_SHARED_PTR(C)

struct A
{
	A(void)
		:a(100), f(123.5f), str("Hello"), str2("Hello2"), ptr(new C()), org_ptr(new C())
	{
		memset(arr, 'a', sizeof(arr));
	}

	A(int n)
		: a(0), f(0.0), str(""), ptr(new C(10)), org_ptr(0)
	{
		memset(arr, 0, sizeof(arr));
	}

	~A()
	{
		if(org_ptr)
		{
			delete org_ptr;
		}
	}

	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & YGGR_SERIALIZATION_NVP(a);
		ar & YGGR_SERIALIZATION_NVP(f);
		ar & YGGR_SERIALIZATION_NVP(str);
		ar & YGGR_SERIALIZATION_NVP(str2);
		ar & YGGR_SERIALIZATION_NVP(ptr);
		ar & YGGR_SERIALIZATION_NVP(arr);
		ar & YGGR_SERIALIZATION_NVP(org_ptr);
	}

	int a;
	float f;
	std::string str;
	boost::container::string str2;
	yggr::shared_ptr<C> ptr;
	char arr[10];
	C* org_ptr;
};

template<typename OPak, typename IPak>
void test_foo1(void)
{
	typedef OPak opak_type;
	typedef IPak ipak_type;

	int a = 100;
	opak_type o;
	o.save(a);

	std::cout << o.org_buf() << std::endl;

	ipak_type i(o);
	int b = 0;
	i.load(b);

	std::cout << b << std::endl;

	yggr_test_assert(a == b);
}

template<typename OPak, typename IPak>
void test_foo2(void)
{
	typedef OPak opak_type;
	typedef IPak ipak_type;

	A a;
	opak_type o;
	o.save(a);
	std::cout << o.org_buf() << std::endl;

	ipak_type i(o);

	A a2(10);
	i.load(a2);

	//std::cout << a2.str << "," << a2.str2 << std::endl;

	yggr_test_assert(a2.arr[0] == 'a');
	std::cout << a2.a << "," << a2.f << "," << a2.str << "," << a2.str2 << "," << (*(a2.ptr)).n 
				<< (*(a2.org_ptr)).n << std::endl;
}

template<typename OPak, typename IPak>
void test_foo2_2(void)
{
	typedef OPak opak_type;
	typedef IPak ipak_type;

	A a;
	opak_type o;
	o.save(a);
	std::cout << o.org_buf() << std::endl;

	ipak_type ti(o);
	ipak_type i;
	i.swap(o);

	A a2(10);
	i.load(a2);

	std::cout << a2.a << "," << a2.f << "," << a2.str << "," << a2.str2 << std::endl;
}

template<typename OPak, typename IPak>
void test_foo3(void)
{
	typedef OPak opak_type;
	typedef IPak ipak_type;

	std::vector<int> ivt;
	ivt.push_back(1);
	ivt.push_back(2);
	ivt.push_back(3);

	opak_type o;
	o.save(ivt);
	std::cout << o.org_buf() << std::endl;

	ipak_type i(o);

	std::vector<int> ivt2;
	i.load(ivt2);

	for(std::size_t i = 0, isize = ivt2.size(); i != isize; ++i)
	{
		std::cout << ivt2[i] << std::endl;
	}
}

int main(int argc, char* argv[])
{
	{
		typedef yggr::packet::packet<yggr::archive::archive_partner::binary_oarchive_partner> opak_type;
		typedef yggr::packet::packet<yggr::archive::archive_partner::binary_iarchive_partner> ipak_type;
		
		test_foo1<opak_type, ipak_type>();
		test_foo2<opak_type, ipak_type>();
		test_foo2_2<opak_type, ipak_type>();
		test_foo3<opak_type, ipak_type>();
	}

	{
		typedef yggr::packet::packet<yggr::archive::archive_partner::network_oarchive_partner> opak_type;
		typedef yggr::packet::packet<yggr::archive::archive_partner::network_iarchive_partner> ipak_type;
		
		test_foo1<opak_type, ipak_type>();
		test_foo2<opak_type, ipak_type>();
		test_foo2_2<opak_type, ipak_type>();
		test_foo3<opak_type, ipak_type>();
	}

	wait_any_key(argc, argv);
	return 0;
}
