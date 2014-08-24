//mongo_connection.cpp

#include <iostream>
#include <yggr/charset/string.hpp>

#include <yggr/nsql_database_system/mongodb_config.h>
#include <yggr/nsql_database_system/c_bson.hpp>

#include <yggr/packet/packet.hpp>
#include <yggr/archive/network_archive_partner.hpp>

#include <yggr/archive/bson_oarchive.hpp>
#include <yggr/archive/bson_iarchive.hpp>
#include <yggr/archive/bson_archive_partner.hpp>

#include <yggr/nsql_database_system/c_mongo_connection.hpp>
#include <yggr/nsql_database_system/mongo_accesser.hpp>

#include <yggr/serialization/vector.hpp>
#include <yggr/serialization/utf8_string.hpp>

//#include <yggr/nsql_database_system/mongo_iterator.hpp>

#include <vector>

#ifdef _MSC_VER
#include <vld.h>
#endif //_MSC_VER

#ifdef _MSC_VER
#ifdef _DEBUG
#pragma comment(lib, "libmongoc-vc100-d.lib")
#pragma comment(lib, "libbson-vc100-d.lib")

#else
#pragma comment(lib, "libmongoc-vc100.lib")
#pragma comment(lib, "libbson-vc100.lib")

#endif // _DEBUG
#endif // _MSC_VER

typedef yggr::packet::packet<yggr::archive::archive_partner::network_oarchive_partner> opacket_type;
typedef yggr::packet::packet<yggr::archive::archive_partner::network_iarchive_partner> ipacket_type;

typedef yggr::packet::packet<yggr::archive::archive_partner::bson_oarchive_partner> o_bson_packet_type;
typedef yggr::packet::packet<yggr::archive::archive_partner::bson_iarchive_partner> i_bson_packet_type;


struct type_test
{
	type_test(void)
		:_str("≤‚ ‘")
	{
	}

	type_test(int n)
	{
	}

	template<typename Archive>
	void serialize(Archive & ar, yggr::u32 version)
	{
		ar & YGGR_SERIALIZE_NAME_NVP("string", _str);
	}

	//std::string _str;
	yggr::charset::utf8_string _str;

};

std::ostream& operator<<(std::ostream& os, const type_test& val)
{

	os << "string = " << val._str << '\n';
	return os;
}

typedef yggr::nsql_database_system::c_mongo_connection connection_type;
typedef yggr::nsql_database_system::mongo_accesser<connection_type> accesser_type;

typedef std::vector<type_test> test_vt_type;
class Saver
{
public:
	typedef i_bson_packet_type buf_type;

public:

	Saver(test_vt_type& rvt)
		: now_idx(0), vt(rvt)
	{
	}

	Saver& operator<<(yggr::s32 size)
	{
		vt.resize(size);
		return *this;
	}

	Saver& operator<<(buf_type& buf)
	{
		if(now_idx >= vt.size())
		{
			return *this;
		}

		//std::cout << buf.org_buf() << std::endl;
		buf.load(vt[now_idx]);
		++now_idx;

		return *this;
	}

	Saver& operator+=(buf_type& buf)
	{
		//std::cout << buf.org_buf() << std::endl;
		test_vt_type::value_type val;
		buf.load(val);
		vt.push_back(val);
		++now_idx;

		return *this;
	}

private:
	yggr::u32 now_idx;
	test_vt_type& vt;
};


void test(void)
{
	//yggr::exception::exception::init_type init(1, 0);
	yggr::ptr_single<yggr::exception::exception>::init_ins();
	connection_type::this_ptr_type pconn(new connection_type("127.0.0.1:10098"));
	pconn->connect();

	accesser_type accesser(pconn);

	{
		for(int i = 0; i < 2; ++i)
		{
			o_bson_packet_type opak;
			type_test test;
			opak.save(test);
			accesser.insert(std::string("test"), std::string("foo2"), opak);
		}
	}


	test_vt_type vt;

	yggr::nsql_database_system::c_bson b1(true), b2(true);
	Saver saver(vt);
	accesser.find(saver, std::string("test"), std::string("foo2"), b1, b2, 2, 0, 0);

	{
		std::cout << vt.size() << std::endl;
		for(int i = 0; i < vt.size(); ++i)
		{
			std::cout << vt[i] << std::endl;
		}
	}

	//accesser.remove(std::string("test"), std::string("foo2"), b2);

	vt.clear();

	 yggr::ptr_single<yggr::exception::exception>::uninstall();
}

int main(int argc, char* argv[])
{
	test();

	char cc = 0;
	std::cin >> cc;


	return 0;
}
