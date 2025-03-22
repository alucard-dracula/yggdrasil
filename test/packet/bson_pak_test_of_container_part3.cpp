//bson_pak_test_of_container_part3.cpp

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

template<typename T>
struct unordered_get_key
{
	const T& operator()(const T& t) const
	{
		return t;
	}
};

template<typename Key, typename Val>
struct unordered_get_key< std::pair<Key, Val> >
{
	const Key& operator()(const std::pair<Key, Val>& p) const
	{
		return p.first;
	}
};

template<typename Container>
void unordered_check_container(const Container& cont1, const Container& cont2)
{
	typedef Container container_type;
	typedef typename container_type::value_type value_type;
	typedef typename container_type::const_iterator iter_type;
	typedef unordered_get_key<value_type> get_key_type;

	for(iter_type i = cont1.begin(), isize = cont1.end(); i != isize; ++i)
	{
		cont2.find(get_key_type()(*i));
	}
}

template<typename Container>
void test_unordered_unliner_container_of_bson(void)
{
	int arr[10] = {1, 3, 4, 6, 9};
	Container cont1(arr + 0, arr + 10);

	o_bson_packet_type opak;

	opak.save(YGGR_SERIALIZATION_NAME_NVP("container", cont1));

	std::cout << "org_buf = " << opak.org_buf() << std::endl;

	Container cont2;
	yggr_test_assert(cont2.empty());
	i_bson_packet_type ipak(opak);
	ipak.load(YGGR_SERIALIZATION_NAME_NVP("container", cont2));

	yggr_test_assert(!(cont1.empty() || cont2.empty()));
	yggr_test_assert(cont2.size() == cont1.size());
	yggr_test_assert(cont2.bucket_count() == cont1.bucket_count());
	unordered_check_container(cont1, cont2);
	//out_container(cont2);
}

template<typename Container>
void test_unordered_unliner_container_of_network(void)
{
	int arr[10] = {1, 3, 4, 6, 9};
	Container cont1(arr + 0, arr + 10);

	opacket_type opak;

	opak.save(cont1);

	std::cout << "org_buf = " << opak.org_buf() << std::endl;

	Container cont2;
	yggr_test_assert(cont2.empty());
	ipacket_type ipak(opak);
	ipak.load(cont2);

	yggr_test_assert(!(cont1.empty() || cont2.empty()));
	yggr_test_assert(cont2.size() == cont1.size());
	yggr_test_assert(cont2.bucket_count() == cont1.bucket_count());
	unordered_check_container(cont1, cont2);
	//out_container(cont2);
}

void test_unordered_unliner(void)
{
	test_unordered_unliner_container_of_network< boost::unordered_set<int> >();
	test_unordered_unliner_container_of_network< boost::unordered_multiset<int> >();

	test_unordered_unliner_container_of_bson< boost::unordered_set<int> >();
	test_unordered_unliner_container_of_bson< boost::unordered_multiset<int> >();
}

#include <yggr/serialization/unordered_map.hpp>

template<typename Container>
void test_unordered_key_value_container_of_bson(void)
{
	typedef typename Container::value_type value_type;
	int arr[10] = {1, 3, 4, 6, 9};
	Container cont1;
	for(int i = 0; i != 10; ++i)
	{
		cont1.insert(value_type(arr[i], arr[i]));
	}

	o_bson_packet_type opak;

	opak.save(YGGR_SERIALIZATION_NAME_NVP("container", cont1));

	std::cout << "org_buf = " << opak.org_buf() << std::endl;

	Container cont2;
	yggr_test_assert(cont2.empty());
	i_bson_packet_type ipak(opak);
	ipak.load(YGGR_SERIALIZATION_NAME_NVP("container", cont2));

	yggr_test_assert(!(cont1.empty() || cont2.empty()));
	yggr_test_assert(cont2.size() == cont1.size());
	unordered_check_container(cont1, cont2);
	//out_container(cont2);
}

template<typename Container>
void test_unordered_key_value_container_of_network(void)
{
	typedef typename Container::value_type value_type;
	int arr[10] = {1, 3, 4, 6, 9};
	Container cont1;
	for(int i = 0; i != 10; ++i)
	{
		cont1.insert(value_type(arr[i], arr[i]));
	}

	opacket_type opak;

	opak.save(cont1);

	std::cout << "org_buf = " << opak.org_buf() << std::endl;

	Container cont2;
	yggr_test_assert(cont2.empty());
	ipacket_type ipak(opak);
	ipak.load(cont2);

	yggr_test_assert(!(cont1.empty() || cont2.empty()));
	yggr_test_assert(cont2.size() == cont1.size());
	unordered_check_container(cont1, cont2);
	//out_container(cont2);
}

template<typename Container>
void test_unordered_key_value_container2_of_bson(void)
{
	typedef Container container_type;
	typedef typename container_type::value_type pair_type;
	typedef typename pair_type::second_type container2_type;
	typedef typename container2_type::value_type value_type;
	int arr[10] = {1, 3, 4, 6, 9};
	Container cont1;
	container2_type tmp;
	for(int j = 0; j < 10; ++j)
	{
		tmp.insert(value_type(arr[j], arr[j]));
	}

	for(int i = 0; i != 10; ++i)
	{
		cont1.insert(pair_type(arr[i], tmp));
	}

	o_bson_packet_type opak;

	opak.save(YGGR_SERIALIZATION_NAME_NVP("container", cont1));

	std::cout << "org_buf = " << opak.org_buf() << std::endl;

	Container cont2;
	yggr_test_assert(cont2.empty());
	i_bson_packet_type ipak(opak);
	ipak.load(YGGR_SERIALIZATION_NAME_NVP("container", cont2));

	yggr_test_assert(!(cont1.empty() || cont2.empty()));
	yggr_test_assert(cont2.size() == cont1.size());
	unordered_check_container(cont1, cont2);
	//out_container(cont2);
}

template<typename Container>
void test_unordered_key_value_container2_of_network(void)
{
	typedef Container container_type;
	typedef typename container_type::value_type pair_type;
	typedef typename pair_type::second_type container2_type;
	typedef typename container2_type::value_type value_type;
	int arr[10] = {1, 3, 4, 6, 9};
	Container cont1;
	container2_type tmp;
	for(int j = 0; j < 10; ++j)
	{
		tmp.insert(value_type(arr[j], arr[j]));
	}

	for(int i = 0; i != 10; ++i)
	{
		cont1.insert(pair_type(arr[i], tmp));
	}

	opacket_type opak;

	opak.save(cont1);

	std::cout << "org_buf = " << opak.org_buf() << std::endl;

	Container cont2;
	yggr_test_assert(cont2.empty());
	ipacket_type ipak(opak);
	ipak.load(cont2);

	yggr_test_assert(!(cont1.empty() || cont2.empty()));
	yggr_test_assert(cont2.size() == cont1.size());
	unordered_check_container(cont1, cont2);
	//out_container(cont2);
}

void test_unordered_key_value(void)
{
	test_unordered_key_value_container_of_network<boost::unordered_map<int, int> >();
	test_unordered_key_value_container_of_network<boost::unordered_multimap<int, int> >();

	test_unordered_key_value_container_of_bson<boost::unordered_map<int, int> >();
	test_unordered_key_value_container_of_bson<boost::unordered_multimap<int, int> >();

	test_unordered_key_value_container2_of_network<boost::unordered_map<int, boost::unordered_map<int, int> > >();
	test_unordered_key_value_container2_of_bson<boost::unordered_multimap<int, boost::unordered_multimap<int, int> > >();

	test_unordered_key_value_container2_of_network<boost::unordered_map<int, boost::unordered_multimap<int, int> > >();
	test_unordered_key_value_container2_of_bson<boost::unordered_multimap<int, boost::unordered_map<int, int> > >();

	test_unordered_key_value_container2_of_network<boost::unordered_multimap<int, std::map<int, int> > >();
	test_unordered_key_value_container2_of_bson<boost::unordered_map<int, std::multimap<int, int> > >();
}

int main(int argc, char* argv[])
{
	test_unordered_unliner();
	test_unordered_key_value();

	wait_any_key(argc, argv);
	return 0;
}
