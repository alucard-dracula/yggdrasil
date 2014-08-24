////bson_pak_test.cpp
//

//#define MONGO_STATIC_BUILD

#define YGGR_SERIALIZE_SIZE32
#include <iostream>
#include <yggr/base/yggrdef.h>

#include <yggr/packet/packet.hpp>
#include <yggr/archive/network_archive_partner.hpp>

#include <yggr/serialization/nvp.hpp>

#include <yggr/archive/bson_archive_partner.hpp>

#include <yggr/serialization/string.hpp>
#include <yggr/serialization/utf8_string.hpp>
#include <yggr/serialization/bson_data_type.hpp>

//BOOST_MPL_ASSERT((yggr::nsql_database_system::is_bson_inside_type<yggr::serialization::collection_size_type>));

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

// TEST 1, 2, 3
#define TEST 1

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

template<typename Container>
void out_container(const Container& cont)
{
	typedef Container container_type;
	typedef typename container_type::const_iterator iter_type;

	for(iter_type i = cont.begin(), isize = cont.end(); i != isize; ++i)
	{
		std::cout << *i << std::endl;
	}
}

template<typename Container>
void check_container(const Container& cont1, const Container& cont2)
{
	typedef Container container_type;
	typedef typename container_type::const_iterator iter_type;

	for(iter_type i = cont1.begin(), isize = cont1.end(), j = cont2.begin();
			i != isize; ++i, ++j)
	{
		assert((*i) == (*j));
	}
}

template<typename Container>
void test_liner_container_of_bson(void)
{
	int arr[10] = {1, 3, 4, 6, 9};
	Container cont1(arr, arr + 10);

	o_bson_packet_type opak;

	opak.save(YGGR_SERIALIZE_NAME_NVP("container", cont1));

	std::cout << "org_buf = " << opak.org_buf() << std::endl;

	Container cont2;
	assert(cont2.empty());
	i_bson_packet_type ipak(opak);
	ipak.load(YGGR_SERIALIZE_NAME_NVP("container", cont2));

	assert(!(cont1.empty() || cont2.empty()));
	assert(cont2.size() == cont1.size());
	check_container(cont1, cont2);
	//out_container(cont2);
}

template<typename Container>
void test_liner_container_of_network(void)
{
	int arr[10] = {1, 3, 4, 6, 9};
	Container cont1(arr, arr + 10);

	opacket_type opak;

	opak.save(cont1);

	std::cout << "org_buf = " << opak.org_buf() << std::endl;

	Container cont2;
	assert(cont2.empty());
	ipacket_type ipak(opak);
	ipak.load(cont2);

	assert(!(cont1.empty() || cont2.empty()));
	assert(cont2.size() == cont1.size());
	check_container(cont1, cont2);
	//out_container(cont2);
}

template<typename Container>
void out_container2(const Container& cont)
{
	typedef Container container_type;
	typedef typename container_type::const_iterator iter_type;
	typedef typename container_type::value_type container2_type;
	typedef typename container2_type::const_iterator iter2_type;

	for(iter_type i = cont.begin(), isize = cont.end(); i != isize; ++i)
	{
		for(iter2_type j = (*i).begin(), jsize = (*i).end(); j != jsize; ++j)
		{
			std::cout << *j << std::endl;
		}
	}
}

template<typename Container>
void check_container2(const Container& cont1, const Container& cont2)
{
	typedef Container container_type;
	typedef typename container_type::const_iterator iter_type;
	typedef typename container_type::value_type container2_type;
	typedef typename container2_type::const_iterator iter2_type;

	for(iter_type i = cont1.begin(), isize = cont1.end(), i2 = cont2.begin();
			i != isize; ++i, ++i2)
	{
		for(iter2_type j = (*i).begin(), jsize = (*i).end(), j2 = (*i2).begin();
			j != jsize; ++j, ++j2)
		assert((*j) == (*j2));
	}
}

template<typename Container>
void test_liner_container2_of_bson(void)
{
	typedef Container container_type;
	typedef typename container_type::value_type container2_type;
	int arr[10] = {1, 3, 4, 6, 9};
	container2_type tmp(arr, arr + 10);
	Container cont1;
	for(int i = 0; i != 10; ++i)
	{
		cont1.push_back(tmp);
	}

	o_bson_packet_type opak;
	opak.save(YGGR_SERIALIZE_NAME_NVP("container", cont1));

	std::cout << "org_buf = " << opak.org_buf() << std::endl;

	Container cont2;
	assert(cont2.empty());
	i_bson_packet_type ipak(opak);
	ipak.load(YGGR_SERIALIZE_NAME_NVP("container", cont2));

	assert(!(cont1.empty() || cont2.empty()));
	assert(cont2.size() == cont1.size());
	check_container2(cont1, cont2);
	//out_container(cont2);
}

template<typename Container>
void test_liner_container2_of_network(void)
{
	typedef Container container_type;
	typedef typename container_type::value_type container2_type;

	int arr[10] = {1, 3, 4, 6, 9};
	container2_type tmp(arr, arr + 10);
	Container cont1;
	for(int i = 0; i != 10; ++i)
	{
		cont1.push_back(tmp);
	}

	opacket_type opak;
	opak.save(cont1);

	std::cout << "org_buf = " << opak.org_buf() << std::endl;

	Container cont2;
	assert(cont2.empty());
	ipacket_type ipak(opak);
	ipak.load(cont2);

	assert(!(cont1.empty() || cont2.empty()));
	assert(cont2.size() == cont1.size());
	check_container(cont1, cont2);
	//out_container(cont2);
}

template<typename Container>
void test_array_container_of_network(void)
{
	int arr[10] = {1, 3, 4, 6, 9};
	Container cont1;
	memcpy(&cont1[0], arr, sizeof(int) * 10);

	opacket_type opak;

	opak.save(cont1);

	std::cout << "org_buf = " << opak.org_buf() << std::endl;

	Container cont2;
	ipacket_type ipak(opak);
	ipak.load(cont2);

	assert(cont2.size() == cont1.size());
	check_container(cont1, cont2);
	//out_container(cont2);
}

template<typename Container>
void test_array_container_of_bson(void)
{
	int arr[10] = {1, 3, 4, 6, 9};
	Container cont1;
	memcpy(&cont1[0], arr, sizeof(int) * 10);

	o_bson_packet_type opak;

	opak.save(YGGR_SERIALIZE_NAME_NVP("container", cont1));

	std::cout << "org_buf = " << opak.org_buf() << std::endl;

	Container cont2;
	i_bson_packet_type ipak(opak);
	ipak.load(YGGR_SERIALIZE_NAME_NVP("container", cont2));

	assert(cont2.size() == cont1.size());
	check_container(cont1, cont2);
	//out_container(cont2);
}

#include <yggr/serialization/array.hpp>
void test_liner(void)
{
	//BOOST_MPL_ASSERT((yggr::nsql_database_system::is_bson_inside_type<yggr::serialization::collection_size_type>));

#if defined(_MSC_VER) || TEST == 1
	test_liner_container_of_network< std::vector<int> >();
	test_liner_container_of_network< std::list<int> >();
	test_liner_container_of_network< std::deque<int> >();
	test_liner_container_of_network< boost::container::vector<int> >();
	test_liner_container_of_network< boost::container::list<int> >();
	test_liner_container_of_network< boost::container::deque<int> >();

	test_liner_container_of_bson< std::vector<int> >();
	test_liner_container_of_bson< std::list<int> >();
	test_liner_container_of_bson< std::deque<int> >();
	test_liner_container_of_bson< boost::container::vector<int> >();
	test_liner_container_of_bson< boost::container::list<int> >();
	test_liner_container_of_bson< boost::container::deque<int> >();

	test_liner_container2_of_network< std::vector<std::vector<int> > >();
	test_liner_container2_of_network< std::list<std::list<int> > >();
	test_liner_container2_of_network< std::deque<std::deque<int> > >();
	test_liner_container2_of_network< boost::container::vector<boost::container::vector<int> > >();
	test_liner_container2_of_network< boost::container::list<boost::container::list<int> > >();
	test_liner_container2_of_network< boost::container::deque<boost::container::deque<int> > >();

	test_liner_container2_of_bson< std::vector<std::vector<int> > >();
	test_liner_container2_of_bson< std::list<std::list<int> > >();
	test_liner_container2_of_bson< std::deque<std::deque<int> > >();
	test_liner_container2_of_bson< boost::container::vector<boost::container::vector<int> > >();
	test_liner_container2_of_bson< boost::container::list<boost::container::list<int> > >();
	test_liner_container2_of_bson< boost::container::deque<boost::container::deque<int> > >();

	test_liner_container2_of_network< std::vector<std::list<int> > >();
	test_liner_container2_of_network< std::list<boost::container::vector<int> > >();
	test_liner_container2_of_network< boost::container::vector<std::vector<int> > >();
	test_liner_container2_of_network< boost::container::list<std::list<int> > >();

	test_liner_container2_of_bson< std::vector<std::list<int> > >();
	test_liner_container2_of_bson< std::list<boost::container::vector<int> > >();
	test_liner_container2_of_bson< boost::container::vector<std::list<int> > >();
	test_liner_container2_of_bson< boost::container::list<std::list<int> > >();

	test_liner_container_of_network< std::vector<bool> >();
	test_liner_container_of_network< boost::container::vector<bool> >();

	test_liner_container_of_network< std::vector<bool> >();
	test_liner_container_of_network< std::list<bool> >();

	test_liner_container_of_bson< std::vector<bool> >();
	test_liner_container_of_bson< boost::container::vector<bool> >();

	test_array_container_of_network< boost::array<int, 10> >();
	test_array_container_of_bson< boost::array<int, 10> >();

#endif // TEST == 1

}

#include <yggr/serialization/set.hpp>
void test_unliner(void)
{
#if defined(_MSC_VER) || TEST == 2
	test_liner_container_of_network< std::set<int> >();
	test_liner_container_of_network< boost::container::set<int> >();
	test_liner_container_of_network< std::multiset<int> >();
	test_liner_container_of_network< boost::container::multiset<int> >();

	test_liner_container_of_bson< std::set<int> >();
	test_liner_container_of_bson< boost::container::set<int> >();
	test_liner_container_of_bson< std::multiset<int> >();
	test_liner_container_of_bson< boost::container::multiset<int> >();
#endif // TEST == 2
}

#include <yggr/serialization/map.hpp>

template<typename Container>
void test_key_value_container_of_bson(void)
{
	typedef typename Container::value_type value_type;
	int arr[10] = {1, 3, 4, 6, 9};
	Container cont1;
	for(int i = 0; i != 10; ++i)
	{
		cont1.insert(value_type(arr[i], arr[i]));
	}

	o_bson_packet_type opak;

	opak.save(YGGR_SERIALIZE_NAME_NVP("container", cont1));

	std::cout << "org_buf = " << opak.org_buf() << std::endl;

	Container cont2;
	assert(cont2.empty());
	i_bson_packet_type ipak(opak);
	ipak.load(YGGR_SERIALIZE_NAME_NVP("container", cont2));

	assert(!(cont1.empty() || cont2.empty()));
	assert(cont2.size() == cont1.size());
	check_container(cont1, cont2);
	//out_container(cont2);
}

template<typename Container>
void test_key_value_container_of_network(void)
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
	assert(cont2.empty());
	ipacket_type ipak(opak);
	ipak.load(cont2);

	assert(!(cont1.empty() || cont2.empty()));
	assert(cont2.size() == cont1.size());
	check_container(cont1, cont2);
	//out_container(cont2);
}

template<typename Container>
void test_key_value_container2_of_bson(void)
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

	opak.save(YGGR_SERIALIZE_NAME_NVP("container", cont1));

	std::cout << "org_buf = " << opak.org_buf() << std::endl;

	Container cont2;
	assert(cont2.empty());
	i_bson_packet_type ipak(opak);
	ipak.load(YGGR_SERIALIZE_NAME_NVP("container", cont2));

	assert(!(cont1.empty() || cont2.empty()));
	assert(cont2.size() == cont1.size());
	check_container(cont1, cont2);
	//out_container(cont2);
}

template<typename Container>
void test_key_value_container2_of_network(void)
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
	assert(cont2.empty());
	ipacket_type ipak(opak);
	ipak.load(cont2);

	assert(!(cont1.empty() || cont2.empty()));
	assert(cont2.size() == cont1.size());
	check_container(cont1, cont2);
	//out_container(cont2);
}

void test_key_value(void)
{
#if defined(_MSC_VER) || TEST == 2
	test_key_value_container_of_network<std::map<int, int> >();
	test_key_value_container_of_network<std::multimap<int, int> >();
	test_key_value_container_of_network<boost::container::map<int, int> >();
	test_key_value_container_of_network<boost::container::multimap<int, int> >();

	test_key_value_container_of_bson<std::map<int, int> >();
	test_key_value_container_of_bson<std::multimap<int, int> >();
	test_key_value_container_of_bson<boost::container::map<int, int> >();
	test_key_value_container_of_bson<boost::container::multimap<int, int> >();

	test_key_value_container2_of_network<std::map<int, std::map<int, int> > >();
	test_key_value_container2_of_bson<std::map<int, std::map<int, int> > >();

	test_key_value_container2_of_network<boost::container::map<int, boost::container::map<int, int> > >();
	test_key_value_container2_of_bson<boost::container::map<int, boost::container::map<int, int> > >();

	test_key_value_container2_of_network<boost::container::map<int, std::map<int, int> > >();
	test_key_value_container2_of_bson<boost::container::map<int, std::map<int, int> > >();

	test_key_value_container2_of_network<boost::container::map<int, std::multimap<int, int> > >();
	test_key_value_container2_of_bson<boost::container::map<int, std::multimap<int, int> > >();

#endif // TEST == 2
}

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
	Container cont1(arr, arr + 10);

	o_bson_packet_type opak;

	opak.save(YGGR_SERIALIZE_NAME_NVP("container", cont1));

	std::cout << "org_buf = " << opak.org_buf() << std::endl;

	Container cont2;
	assert(cont2.empty());
	i_bson_packet_type ipak(opak);
	ipak.load(YGGR_SERIALIZE_NAME_NVP("container", cont2));

	assert(!(cont1.empty() || cont2.empty()));
	assert(cont2.size() == cont1.size());
	assert(cont2.bucket_count() == cont1.bucket_count());
	unordered_check_container(cont1, cont2);
	//out_container(cont2);
}

template<typename Container>
void test_unordered_unliner_container_of_network(void)
{
	int arr[10] = {1, 3, 4, 6, 9};
	Container cont1(arr, arr + 10);

	opacket_type opak;

	opak.save(cont1);

	std::cout << "org_buf = " << opak.org_buf() << std::endl;

	Container cont2;
	assert(cont2.empty());
	ipacket_type ipak(opak);
	ipak.load(cont2);

	assert(!(cont1.empty() || cont2.empty()));
	assert(cont2.size() == cont1.size());
	assert(cont2.bucket_count() == cont1.bucket_count());
	unordered_check_container(cont1, cont2);
	//out_container(cont2);
}

void test_unordered_unliner(void)
{
#if defined(_MSC_VER) || TEST == 2
	test_unordered_unliner_container_of_network< boost::unordered_set<int> >();
	test_unordered_unliner_container_of_network< boost::unordered_multiset<int> >();

	test_unordered_unliner_container_of_bson< boost::unordered_set<int> >();
	test_unordered_unliner_container_of_bson< boost::unordered_multiset<int> >();
#endif // TEST == 2
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

	opak.save(YGGR_SERIALIZE_NAME_NVP("container", cont1));

	std::cout << "org_buf = " << opak.org_buf() << std::endl;

	Container cont2;
	assert(cont2.empty());
	i_bson_packet_type ipak(opak);
	ipak.load(YGGR_SERIALIZE_NAME_NVP("container", cont2));

	assert(!(cont1.empty() || cont2.empty()));
	assert(cont2.size() == cont1.size());
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
	assert(cont2.empty());
	ipacket_type ipak(opak);
	ipak.load(cont2);

	assert(!(cont1.empty() || cont2.empty()));
	assert(cont2.size() == cont1.size());
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

	opak.save(YGGR_SERIALIZE_NAME_NVP("container", cont1));

	std::cout << "org_buf = " << opak.org_buf() << std::endl;

	Container cont2;
	assert(cont2.empty());
	i_bson_packet_type ipak(opak);
	ipak.load(YGGR_SERIALIZE_NAME_NVP("container", cont2));

	assert(!(cont1.empty() || cont2.empty()));
	assert(cont2.size() == cont1.size());
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
	assert(cont2.empty());
	ipacket_type ipak(opak);
	ipak.load(cont2);

	assert(!(cont1.empty() || cont2.empty()));
	assert(cont2.size() == cont1.size());
	unordered_check_container(cont1, cont2);
	//out_container(cont2);
}

void test_unordered_key_value(void)
{
#if defined(_MSC_VER) || TEST == 3
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
#endif // TEST == 2
}

#include <yggr/serialization/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>

struct A
{
	struct tag_prm1{};
	struct tag_prm2{};
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
		ar & YGGR_SERIALIZE_NVP(prm1);
		ar & YGGR_SERIALIZE_NVP(prm2);
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
	typedef typename cont_type::const_iterator cont_iter_type;
	for(cont_iter_type i = cont1.begin(), isize = cont1.end(); i != isize; ++i)
	{
		assert(cont2.find(i->prm1) != cont2.end());
	}
}

template<typename Container>
void test_multi_index_of_bson(void)
{
	int arr[10] = {1, 2, 3, 4, 5};
	Container cont1;
	for(int i = 0; i != 10; ++i)
	{
		cont1.insert(A(arr[i], arr[i]));
	}

	out_multi_index_container(cont1);
	o_bson_packet_type opak;
	opak.save(YGGR_SERIALIZE_NAME_NVP("container", cont1));

	std::cout << opak.org_buf() << std::endl;

	Container cont2;
	assert(cont2.empty());
	i_bson_packet_type ipak(opak);
	ipak.load(YGGR_SERIALIZE_NAME_NVP("container", cont2));

	assert(cont1.size() == cont2.size());
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
		cont1.insert(A(arr[i], arr[i]));
	}

	out_multi_index_container(cont1);
	opacket_type opak;
	opak.save(cont1);

	std::cout << opak.org_buf() << std::endl;

	Container cont2;
	assert(cont2.empty());
	ipacket_type ipak(opak);
	ipak.load(cont2);

	assert(cont1.size() == cont2.size());
	check_multi_index_container(cont1, cont2);
	out_multi_index_container(cont2);
}

void test_multi_index_container(void)
{
#if defined(_MSC_VER) || TEST == 3
	typedef boost::multi_index::multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::hashed_unique<
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::hashed_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>
			>
		> mi_cont_type;

	test_multi_index_of_network<mi_cont_type>();
	test_multi_index_of_bson<mi_cont_type>();
#endif // TEST == 3
}

int main(int argc, char* argv[])
{
	test_liner();
	test_unliner();
	test_key_value();
	test_unordered_unliner();
	test_unordered_key_value();
	test_multi_index_container();


	//int a = 100;
	//char buf[sizeof(int)] = {0};
	//int* pb = new(buf)int;

	//*pb = a;

	//std::cout << *pb << std::endl;

	char cc = 0;
	std::cin >> cc;
	return 0;
}
