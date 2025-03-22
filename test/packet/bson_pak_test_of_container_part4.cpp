//bson_pak_test_of_container_part4.cpp

//#define MONGO_STATIC_BUILD

#define YGGR_SERIALIZE_SIZE32

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/base/yggrdef.h>

#include <yggr/packet/packet.hpp>
#include <yggr/archive/network_archive_partner.hpp>

#include <yggr/serialization/nvp.hpp>

#include <yggr/archive/bson_archive_partner.hpp>

#include <yggr/serialization/string.hpp>
#include <yggr/serialization/utf8_string.hpp>

//BOOST_MPL_ASSERT((yggr::nsql_database_system::is_bson_inside_type<yggr::serialization::collection_size_type>));

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

template<typename Key, typename Val>
std::ostream& operator<<(std::ostream& os, const std::pair<Key, Val>& p)
{
	std::cout << "[ " << p.first << " : " << p.second << " ]" << std::endl;
	return os;
}

#include <yggr/serialization/vector.hpp>
#include <yggr/serialization/list.hpp>
#include <yggr/serialization/deque.hpp>
#include <yggr/serialization/array.hpp>
#include <yggr/serialization/set.hpp>
#include <yggr/serialization/map.hpp>
#include <yggr/serialization/unordered_set.hpp>
#include <yggr/serialization/unordered_map.hpp>

#include <yggr/serialization/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
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

template<typename Container>
void out_multi_index_container(const Container& cont)
{
	typedef Container cont_type;
	typedef typename cont_type::const_iterator cont_iter_type;
	for(cont_iter_type i = cont.begin(), isize = cont.end(); i != isize; ++i)
	{
		std::cout << *i << std::endl;
	}
}

template<typename Container>
void check_multi_index_container(const Container& cont1, const Container& cont2)
{
	typedef Container cont_type;
	typedef typename boost::multi_index::nth_index_const_iterator<cont_type, 2>::type cmp_citer_type;
	typedef typename cont_type::const_iterator cont_iter_type;

	cmp_citer_type cmp_iter;
	for(cont_iter_type i = cont1.begin(), isize = cont1.end(); i != isize; ++i)
	{
		cmp_iter = cont2.template get<2>().find(i->prm1);
		yggr_test_assert(cmp_iter != cont2.template get<2>().end());
		yggr_test_assert(*i == *cmp_iter);
	}
}

template<typename Container>
void test_multi_index_of_bson(void)
{
	int arr[10] = {1, 2, 3, 4, 5};
	Container cont1;
	for(int i = 0; i != 10; ++i)
	{
		cont1.template get<2>().insert(A(arr[i], arr[i]));
	}

	out_multi_index_container(cont1);
	o_bson_packet_type opak;
	opak.save(YGGR_SERIALIZATION_NAME_NVP("container", cont1));

	std::cout << opak.org_buf() << std::endl;

	Container cont2;
	yggr_test_assert(cont2.empty());
	i_bson_packet_type ipak(opak);
	ipak.load(YGGR_SERIALIZATION_NAME_NVP("container", cont2));

	yggr_test_assert(cont1.size() == cont2.size());
	check_multi_index_container(cont1, cont2);
	out_multi_index_container(cont2);
}

template<typename Container>
void test_multi_index_of_network(void)
{
	int arr[10] = {1, 2, 3, 4, 5};
	Container cont1;
	for(int i = 0; i != 10; ++i)
	{
		cont1.template get<2>().insert(A(arr[i], arr[i]));
	}

	out_multi_index_container(cont1);
	opacket_type opak;
	opak.save(cont1);

	std::cout << opak.org_buf() << std::endl;

	Container cont2;
	yggr_test_assert(cont2.empty());
	ipacket_type ipak(opak);
	ipak.load(cont2);

	yggr_test_assert(cont1.size() == cont2.size());
	check_multi_index_container(cont1, cont2);
	out_multi_index_container(cont2);
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
		> mi_cont_type;

	test_multi_index_of_network<mi_cont_type>();
	test_multi_index_of_bson<mi_cont_type>();
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
		> mi_cont_type;

	test_multi_index_of_network<mi_cont_type>();
	test_multi_index_of_bson<mi_cont_type>();
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
		> mi_cont_type;

	test_multi_index_of_network<mi_cont_type>();
	test_multi_index_of_bson<mi_cont_type>();
}

void test_multi_index_container_rnd(void)
{
	typedef
		boost::multi_index::multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::random_access<
					boost::multi_index::tag<A::tag_prm1> >,

				boost::multi_index::ordered_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>,

				boost::multi_index::hashed_unique<
					boost::multi_index::tag<A::tag_prm3>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >
			>
		> mi_cont_type;

	test_multi_index_of_network<mi_cont_type>();
	test_multi_index_of_bson<mi_cont_type>();
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
