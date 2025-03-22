// xml_pak_test_of_container_midx.cpp

#define YGGR_SERIALIZE_SIZE32
#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/base/yggrdef.h>

#include <yggr/packet/packet.hpp>

#include <yggr/serialization/nvp.hpp>

#include <yggr/archive/xml_archive_partner.hpp>

#include <yggr/serialization/string.hpp>
#include <yggr/serialization/utf8_string.hpp>
#include <yggr/serialization/multi_index_container.hpp>


#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)
// TEST 1, 2, 3
#define TEST 1


typedef yggr::packet::packet<yggr::archive::archive_partner::xml_oarchive_partner> opak_type;
typedef yggr::packet::packet<yggr::archive::archive_partner::xml_iarchive_partner> ipak_type;

#include <yggr/serialization/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/member.hpp>

struct A
{
	struct tag_prm1{};
	struct tag_prm2{};
	struct tag_prm3{};
	A(void)
		: prm1(), prm2()
	{
	}

	A(int a, int b)
		: prm1(a), prm2(b)
	{
	}

	A(const A& right)
		: prm1(right.prm1), prm2(right.prm2)
	{
	}

	~A(void)
	{
	}

	bool operator==(const A& right) const
	{
		return prm1 == right.prm1 && prm2 == right.prm2;
	}

	bool operator!=(const A& right) const
	{
		return !(prm1 == right.prm1 && prm2 == right.prm2);
	}

private:
	friend class boost::serialization::access;
	template<typename Archive>
	void serialize(Archive& ar, const yggr::u32 ver)
	{
		ar & YGGR_SERIALIZATION_NVP(prm1);
		ar & YGGR_SERIALIZATION_NVP(prm2);
	}

public:
	int prm1;
	int prm2;
};

std::ostream& operator<<(std::ostream& os, const A& na)
{
	os << "prm1 = " << na.prm1 << " prm2 = " << na.prm2;
	return os;
}

template<typename Midx>
struct midx_container
{
public:
	typedef Midx mi_cont_type;

private:
	typedef midx_container this_type;

public:
	bool compare(const this_type& right) const
	{
		typedef typename boost::multi_index::nth_index_const_iterator<mi_cont_type, 2>::type cmp_iter_type;
		cmp_iter_type cmp_iter;
		for(typename mi_cont_type::const_iterator i = midx.begin(), isize = midx.end(); i != isize; ++i)
		{
			cmp_iter = right.midx.template get<2>().find(i->prm1);
			if(cmp_iter == right.midx.template get<2>().end() || *cmp_iter != *i)
			{
				return false;
			}
		}

		return true;
	}

private:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const yggr::u32 ver)
	{
		ar & YGGR_SERIALIZATION_NVP(midx);
	}

public:
	mi_cont_type midx;
};

template<typename Midx>
bool operator==(const midx_container<Midx>& l, const midx_container<Midx>& r)
{
	return l.compare(r);
}

template<typename Midx>
bool operator!=(const midx_container<Midx>& l, const midx_container<Midx>& r)
{
	return !l.compare(r);
}


void test_multi_index_container_hash(void)
{
	typedef
		boost::multi_index::multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::hashed_unique<
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::hashed_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>,

				boost::multi_index::hashed_unique<
					boost::multi_index::tag<A::tag_prm3>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >
			>
		> midx_type;

	int arr[10] = {1, 2, 3, 4, 5};

	midx_container<midx_type> cont;

	for(int i = 0; i != 10; ++i)
	{
		cont.midx.get<2>().insert(A(arr[i], arr[i]));
	}

	bool btest = false;

	opak_type opak;
	btest = opak.save(YGGR_SERIALIZATION_NAME_NVP("container", cont.midx));
	yggr_test_assert(btest);

	std::cout << opak.org_buf() << std::endl;

	midx_container<midx_type> cont_chk;

	ipak_type ipak(opak);
	btest = ipak.load(YGGR_SERIALIZATION_NAME_NVP("container", cont_chk.midx));
	yggr_test_assert(btest);

	yggr_test_assert(cont == cont_chk);
}

void test_multi_index_container_ordered(void)
{
	typedef
		boost::multi_index::multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::ordered_unique<
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::ordered_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>,

				boost::multi_index::hashed_unique<
					boost::multi_index::tag<A::tag_prm3>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >
			>
		> midx_type;

	int arr[10] = {1, 2, 3, 4, 5};

	midx_container<midx_type> cont;

	for(int i = 0; i != 10; ++i)
	{
		cont.midx.get<2>().insert(A(arr[i], arr[i]));
	}

	bool btest = false;

	opak_type opak;
	btest = opak.save(YGGR_SERIALIZATION_NAME_NVP("container", cont.midx));
	yggr_test_assert(btest);

	std::cout << opak.org_buf() << std::endl;

	midx_container<midx_type> cont_chk;

	ipak_type ipak(opak);
	btest = ipak.load(YGGR_SERIALIZATION_NAME_NVP("container", cont_chk.midx));
	yggr_test_assert(btest);

	yggr_test_assert(cont == cont_chk);
}

void test_multi_index_container_seq(void)
{
	typedef
		boost::multi_index::multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::sequenced<
					boost::multi_index::tag<A::tag_prm1> >,

				boost::multi_index::ordered_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>,

				boost::multi_index::hashed_unique<
					boost::multi_index::tag<A::tag_prm3>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >
			>
		> midx_type;

	int arr[10] = {1, 2, 3, 4, 5};

	midx_container<midx_type> cont;

	for(int i = 0; i != 10; ++i)
	{
		cont.midx.get<2>().insert(A(arr[i], arr[i]));
	}

	bool btest = false;

	opak_type opak;
	btest = opak.save(YGGR_SERIALIZATION_NAME_NVP("container", cont.midx));
	yggr_test_assert(btest);

	std::cout << opak.org_buf() << std::endl;

	midx_container<midx_type> cont_chk;

	ipak_type ipak(opak);
	btest = ipak.load(YGGR_SERIALIZATION_NAME_NVP("container", cont_chk.midx));
	yggr_test_assert(btest);

	yggr_test_assert(cont == cont_chk);
}

void test_multi_index_container_rnd(void)
{
	typedef
		boost::multi_index::multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::random_access<
					boost::multi_index::tag<A::tag_prm1> >,

				boost::multi_index::hashed_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>,

				boost::multi_index::hashed_unique<
					boost::multi_index::tag<A::tag_prm3>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >
			>
		> midx_type;

	int arr[10] = {1, 2, 3, 4, 5};

	midx_container<midx_type> cont;

	for(int i = 0; i != 10; ++i)
	{
		cont.midx.get<2>().insert(A(arr[i], arr[i]));
	}

	bool btest = false;

	opak_type opak;
	btest = opak.save(YGGR_SERIALIZATION_NAME_NVP("container", cont.midx));
	yggr_test_assert(btest);

	std::cout << opak.org_buf() << std::endl;

	midx_container<midx_type> cont_chk;

	ipak_type ipak(opak);
	btest = ipak.load(YGGR_SERIALIZATION_NAME_NVP("container", cont_chk.midx));
	yggr_test_assert(btest);

	yggr_test_assert(cont == cont_chk);

}

int main(int argc, char* argv[])
{
	test_multi_index_container_hash();
	test_multi_index_container_ordered();
	test_multi_index_container_seq();
	test_multi_index_container_rnd();

	wait_any_key(argc, argv);
	return 0;
}
