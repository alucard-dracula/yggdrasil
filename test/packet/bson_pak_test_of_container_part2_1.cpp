//bson_pak_test_of_container_part2.cpp

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
		yggr_test_assert((*i) == (*j));
	}
}

template<typename Container>
void test_liner_container_of_bson(void)
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
	check_container(cont1, cont2);
	//out_container(cont2);
}

template<typename Container>
void test_liner_container_of_network(void)
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
	check_container(cont1, cont2);
	//out_container(cont2);
}

template<typename Container>
void test_string_liner_container_of_bson(void)
{
	yggr::string arr[10] = {"str_1", "str_3", "str_4", "str_6", "str_9"};
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
	check_container(cont1, cont2);
}

template<typename Container>
void test_string_liner_container_of_network(void)
{
	yggr::string arr[10] = {"str_1", "str_3", "str_4", "str_6", "str_9"};
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
	check_container(cont1, cont2);
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
		yggr_test_assert((*j) == (*j2));
	}
}

template<typename Container>
void test_liner_container2_of_bson(void)
{
	typedef Container container_type;
	typedef typename container_type::value_type container2_type;
	int arr[10] = {1, 3, 4, 6, 9};
	container2_type tmp(arr + 0, arr + 10);
	Container cont1;
	for(int i = 0; i != 10; ++i)
	{
		cont1.push_back(tmp);
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
	check_container2(cont1, cont2);
	//out_container(cont2);
}

template<typename Container>
void test_liner_container2_of_network(void)
{
	typedef Container container_type;
	typedef typename container_type::value_type container2_type;

	int arr[10] = {1, 3, 4, 6, 9};
	container2_type tmp(arr + 0, arr + 10);
	Container cont1;
	for(int i = 0; i != 10; ++i)
	{
		cont1.push_back(tmp);
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

	yggr_test_assert(cont2.size() == cont1.size());
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

	opak.save(YGGR_SERIALIZATION_NAME_NVP("container", cont1));

	std::cout << "org_buf = " << opak.org_buf() << std::endl;

	Container cont2;
	i_bson_packet_type ipak(opak);
	ipak.load(YGGR_SERIALIZATION_NAME_NVP("container", cont2));

	yggr_test_assert(cont2.size() == cont1.size());
	check_container(cont1, cont2);
	//out_container(cont2);
}

#include <yggr/serialization/array.hpp>

void test_string_liner(void)
{
	//BOOST_MPL_ASSERT((yggr::nsql_database_system::is_bson_inside_type<yggr::serialization::collection_size_type>));

	test_string_liner_container_of_network< std::vector<yggr::string> >();
	test_string_liner_container_of_network< std::list<yggr::string> >();
	test_string_liner_container_of_network< std::deque<yggr::string> >();
	test_string_liner_container_of_network< boost::container::vector<yggr::string> >();
	test_string_liner_container_of_network< boost::container::list<yggr::string> >();
	test_string_liner_container_of_network< boost::container::deque<yggr::string> >();

	test_string_liner_container_of_bson< std::vector<yggr::string> >();
	test_string_liner_container_of_bson< std::list<yggr::string> >();
	test_string_liner_container_of_bson< std::deque<yggr::string> >();
	test_string_liner_container_of_bson< boost::container::vector<yggr::string> >();
	test_string_liner_container_of_bson< boost::container::list<yggr::string> >();
	test_string_liner_container_of_bson< boost::container::deque<yggr::string> >();
}

#include <yggr/serialization/set.hpp>
void test_unliner(void)
{
	test_liner_container_of_network< std::set<int> >();
	test_liner_container_of_network< boost::container::set<int> >();
	test_liner_container_of_network< std::multiset<int> >();
	test_liner_container_of_network< boost::container::multiset<int> >();

	test_liner_container_of_bson< std::set<int> >();
	test_liner_container_of_bson< boost::container::set<int> >();
	test_liner_container_of_bson< std::multiset<int> >();
	test_liner_container_of_bson< boost::container::multiset<int> >();
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

	opak.save(YGGR_SERIALIZATION_NAME_NVP("container", cont1));

	std::cout << "org_buf = " << opak.org_buf() << std::endl;

	Container cont2;
	yggr_test_assert(cont2.empty());
	i_bson_packet_type ipak(opak);
	ipak.load(YGGR_SERIALIZATION_NAME_NVP("container", cont2));

	yggr_test_assert(!(cont1.empty() || cont2.empty()));
	yggr_test_assert(cont2.size() == cont1.size());
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
	yggr_test_assert(cont2.empty());
	ipacket_type ipak(opak);
	ipak.load(cont2);

	yggr_test_assert(!(cont1.empty() || cont2.empty()));
	yggr_test_assert(cont2.size() == cont1.size());
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

	opak.save(YGGR_SERIALIZATION_NAME_NVP("container", cont1));

	std::cout << "org_buf = " << opak.org_buf() << std::endl;

	Container cont2;
	yggr_test_assert(cont2.empty());
	i_bson_packet_type ipak(opak);
	ipak.load(YGGR_SERIALIZATION_NAME_NVP("container", cont2));

	yggr_test_assert(!(cont1.empty() || cont2.empty()));
	yggr_test_assert(cont2.size() == cont1.size());
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
	yggr_test_assert(cont2.empty());
	ipacket_type ipak(opak);
	ipak.load(cont2);

	yggr_test_assert(!(cont1.empty() || cont2.empty()));
	yggr_test_assert(cont2.size() == cont1.size());
	check_container(cont1, cont2);
	//out_container(cont2);
}

//void test_key_value(void)
//{
//	test_key_value_container_of_network<std::map<int, int> >();
//	test_key_value_container_of_network<std::multimap<int, int> >();
//	test_key_value_container_of_network<boost::container::map<int, int> >();
//	test_key_value_container_of_network<boost::container::multimap<int, int> >();
//
//	test_key_value_container_of_bson<std::map<int, int> >();
//	test_key_value_container_of_bson<std::multimap<int, int> >();
//	test_key_value_container_of_bson<boost::container::map<int, int> >();
//	test_key_value_container_of_bson<boost::container::multimap<int, int> >();
//
//	test_key_value_container2_of_network<std::map<int, std::map<int, int> > >();
//	test_key_value_container2_of_bson<std::map<int, std::map<int, int> > >();
//
//	test_key_value_container2_of_network<boost::container::map<int, boost::container::map<int, int> > >();
//	test_key_value_container2_of_bson<boost::container::map<int, boost::container::map<int, int> > >();
//
//	test_key_value_container2_of_network<boost::container::map<int, std::map<int, int> > >();
//	test_key_value_container2_of_bson<boost::container::map<int, std::map<int, int> > >();
//
//	test_key_value_container2_of_network<boost::container::map<int, std::multimap<int, int> > >();
//	test_key_value_container2_of_bson<boost::container::map<int, std::multimap<int, int> > >();
//}

int main(int argc, char* argv[])
{
	test_string_liner();
	test_unliner();
	//test_key_value();

	wait_any_key(argc, argv);
	return 0;
}
