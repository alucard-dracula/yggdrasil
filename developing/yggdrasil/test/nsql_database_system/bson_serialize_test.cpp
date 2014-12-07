//bson_test.cpp

#include <iostream>
#include <utility>
#include <string>

#include <yggr/nsql_database_system/mongodb_config.h>
#include <yggr/nsql_database_system/c_bson.hpp>

#include <yggr/packet/packet.hpp>
#include <yggr/archive/network_archive_partner.hpp>

//#include <yggr/archive/bson_oarchive.hpp>
//#include <yggr/archive/bson_iarchive.hpp>

#include <boost/serialization/shared_ptr.hpp>
#include <yggr/serialization/bson_data_type.hpp>
#include <yggr/serialization/array.hpp>
#include <yggr/serialization/vector.hpp>
#include <yggr/serialization/list.hpp>
#include <yggr/serialization/set.hpp>
#include <yggr/serialization/map.hpp>
#include <yggr/serialization/unordered_set.hpp>
#include <yggr/serialization/unordered_map.hpp>

#include <yggr/archive/bson_oarchive.hpp>
#include <yggr/archive/bson_iarchive.hpp>

#ifdef _MSC_VER
#include <vld.h>
#endif //_MSC_VER

typedef yggr::packet::packet<yggr::archive::archive_partner::network_oarchive_partner> opacket_type;
typedef yggr::packet::packet<yggr::archive::archive_partner::network_iarchive_partner> ipacket_type;

//typedef yggr::nsql_database_system::c_bson_buffer bson_buffer_type;

typedef yggr::archive::bson_oarchive<false> oar_type;
typedef yggr::archive::bson_iarchive<false> iar_type;

struct C
{
	C(void)
		: n(0)
	{
	}

	C(int n)
		: n(n)
	{
	}

	~C(void)
	{
	}

	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & YGGR_SERIALIZE_NVP(n);
	}

	yggr::s32 n;
};

BOOST_SERIALIZATION_SHARED_PTR(C)

struct type_test
{
	type_test(void)
	{
		_s8 = 100;
		_u8 = 200;
		_s16 = 10000;
		_u16 = 20000;
		_s32 = 1000000;
		_u32 = 2000000;

		_s64 = 1000000000;
		_u64 = 2000000000;

		_f32 = 100.12345f;
		_f64 = 200.1234567;

		_bool = true;

		_str = "abcdefg";
		_buf = "ABCDEFG";

		opacket_type opak;
		opak.save(100);
		_pak_buf = opak.org_buf();

		{
			for(int i = 0; i != 3; ++i)
			{
				ivt.push_back(i + 1);
				ilist.push_back(i + 1);
				iset.insert(i + 1);
				imset.insert(i + 1);
				imap[i + 1] = i + 1;
				immap.insert(std::multimap<yggr::u32, yggr::u32>::value_type(i + 1, i + 1));
				ihset.insert(i + 1);
				imhset.insert(i + 1);

				ihmap[i + 1] = i + 1;
				imhmap.insert(boost::unordered_multimap<yggr::u32, yggr::u32>::value_type(i + 1, i + 1));
			}
		}

		ivtvt.push_back(ivt);
		ivtvt.push_back(ivt);
		ivtvt.push_back(ivt);

		ipair.first = 300;
		ipair.second = 400;
		cptr = boost::shared_ptr<C>(new C(70000));
	}

	type_test(int n)
	{
		_s8 = 0;
		_u8 = 0;
		_s16 = 0;
		_u16 = 0;
		_s32 = 0;
		_u32 = 0;

		_s64 = 0;
		_u64 = 0;

		_f32 = 0;
		_f64 = 0;
	}

private:
	friend class boost::serialization::access;
	template<typename Archive>
	void serialize(Archive & ar, yggr::u32 version)
	{
		ar & YGGR_SERIALIZE_NVP(_s8);
		ar & YGGR_SERIALIZE_NVP(_u8);
		ar & YGGR_SERIALIZE_NVP(_s16);
		ar & YGGR_SERIALIZE_NVP(_u16);
		ar & YGGR_SERIALIZE_NVP(_s32);
		ar & YGGR_SERIALIZE_NVP(_u32);
		ar & YGGR_SERIALIZE_NVP(_s64);
		ar & YGGR_SERIALIZE_NVP(_u64);

		ar & YGGR_SERIALIZE_NVP(_f32);
		ar & YGGR_SERIALIZE_NVP(_f64);

		ar & YGGR_SERIALIZE_NVP(_str);
		ar & YGGR_SERIALIZE_NVP(_date);
		ar & YGGR_SERIALIZE_NVP(_time);
		ar & YGGR_SERIALIZE_NVP(_oid);


        YGGR_SERIALIZE_WRAP_NVP_AR(ar, yggr::nsql_database_system::c_bson_u8_binary_buffer, _buf);

		YGGR_SERIALIZE_WRAP_NVP_AR(ar, yggr::nsql_database_system::c_bson_bpak_binary_buffer, _pak_buf);

		ar & YGGR_SERIALIZE_NVP(ivt);

		ar & YGGR_SERIALIZE_NVP(ilist);

		ar & YGGR_SERIALIZE_NVP(iset);
		ar & YGGR_SERIALIZE_NVP(imset);
		ar & YGGR_SERIALIZE_NVP(ipair);
		ar & YGGR_SERIALIZE_NVP(imap);
		ar & YGGR_SERIALIZE_NVP(immap);

		ar & YGGR_SERIALIZE_NVP(ihset);
		ar & YGGR_SERIALIZE_NVP(imhset);

		ar & YGGR_SERIALIZE_NVP(ihmap);
		ar & YGGR_SERIALIZE_NVP(imhmap);

		ar & YGGR_SERIALIZE_NVP(ivtvt);

		ar & YGGR_SERIALIZE_NVP(cptr);
	}

public:
	yggr::s8 _s8;
	yggr::u8 _u8;
	yggr::s16 _s16;
	yggr::u16 _u16;
	yggr::s32 _s32;
	yggr::u32 _u32;
	yggr::s64 _s64;
	yggr::u64 _u64;

	yggr::f32 _f32;
	yggr::f64 _f64;

	bool _bool;

	std::string _str;

	yggr::nsql_database_system::c_bson_date _date;
	yggr::nsql_database_system::c_bson_time _time;
	yggr::nsql_database_system::c_bson_oid _oid;

	std::string _buf;

	std::string _pak_buf;

	std::vector<yggr::u32> ivt;
	std::list<yggr::u32> ilist;
	std::pair<yggr::u32, yggr::u32> ipair;
	std::set<yggr::u32> iset;
	std::multiset<yggr::u32> imset;
	std::map<yggr::u32, yggr::u32> imap;
	std::multimap<yggr::u32, yggr::u32> immap;
	boost::unordered_set<yggr::u32> ihset;
	boost::unordered_multiset<yggr::u32> imhset;
	boost::unordered_map<yggr::u32, yggr::u32> ihmap;
	boost::unordered_multimap<yggr::u32, yggr::u32> imhmap;

	std::vector<std::vector<yggr::u32> > ivtvt;

	boost::shared_ptr<C> cptr;
};

std::ostream& operator<<(std::ostream& os, const type_test& val)
{
	os << "s8 = " << (int)(val._s8) << '\n';
	os << "u8 = " << (int)(val._u8) << '\n';
	os << "s16 = " << val._s16 << '\n';
	os << "u16 = " << val._u16 << '\n';
	os << "s32 = " << val._s32 << '\n';
	os << "u32 = " << val._u32 << '\n';
	os << "s64 = " << val._s64 << '\n';
	os << "u64 = " << val._u64 << '\n';

	os << "f32 = " << val._f32 << '\n';
	os << "f64 = " << val._f64 << '\n';

	os << "bool =" << val._bool << '\n';

	os << "string = " << val._str << '\n';
	os << "date = " << val._date << '\n';
	os << "time = " << val._time << '\n';
	os << "oid = " << val._oid << '\n';
	os << "buf = " << val._buf	<< '\n';

	int a = 0;
	ipacket_type ipak(val._pak_buf);
	ipak.load(a);

	os << "pak_buf = " << a << '\n';

	os << "ivt = ";
	{
		for(int i = 0; i != val.ivt.size(); ++i)
		{
			os << val.ivt[i] << ' ';
		}

		os << '\n';
	}

	os << "ipair = ";
	os << val.ipair.first << "," << val.ipair.second << "\n";

	os << "ilist = ";
	{

		for(std::list<yggr::u32>::const_iterator i = val.ilist.begin(), isize = val.ilist.end(); i!= isize; ++i)
		{
			os << *i << ' ';
		}

		os << '\n';
	}

	os << "iset = ";
	{
		for(std::set<yggr::u32>::const_iterator i = val.iset.begin(), isize = val.iset.end(); i != isize; ++i)
		{
			os << *i << ' ';
		}

		os << "\n";
	}

	os << "imset = ";
	{
		for(std::multiset<yggr::u32>::const_iterator i = val.imset.begin(), isize = val.imset.end(); i != isize; ++i)
		{
			os << *i << ' ';
		}

		os << "\n";
	}


	os << "imap = ";
	{
		for(std::map<yggr::u32, yggr::u32>::const_iterator i = val.imap.begin(), isize = val.imap.end(); i != isize; ++i)
		{
			os << i->first << "," << i->second << ' ';
		}

		os << "\n";
	}

	os << "immap = ";
	{
		for(std::multimap<yggr::u32, yggr::u32>::const_iterator i = val.immap.begin(), isize = val.immap.end(); i != isize; ++i)
		{
			os << i->first << "," << i->second << ' ';
		}

		os << "\n";
	}

	os << "ihset = ";
	{
		for(boost::unordered_set<yggr::u32>::const_iterator i = val.ihset.begin(), isize = val.ihset.end(); i != isize; ++i)
		{
			os << *i << ' ';
		}

		os << "\n";
	}

	os << "imhset = ";
	{
		for(boost::unordered_multiset<yggr::u32>::const_iterator i = val.imhset.begin(), isize = val.imhset.end(); i != isize; ++i)
		{
			os << *i << ' ';
		}

		os << "\n";
	}

	os << "ihmap = ";
	{
		for(boost::unordered_map<yggr::u32, yggr::u32>::const_iterator i = val.ihmap.begin(), isize = val.ihmap.end(); i != isize; ++i)
		{
			os << i->first << "," << i->second << ' ';
		}

		os << "\n";
	}

	os << "imhmap = ";
	{
		for(boost::unordered_multimap<yggr::u32, yggr::u32>::const_iterator i = val.imhmap.begin(), isize = val.imhmap.end(); i != isize; ++i)
		{
			os << i->first << "," << i->second << ' ';
		}

		os << "\n";
	}

	os << "ivtvt = ";
	{
		for(std::vector< std::vector<yggr::u32> >::const_iterator i = val.ivtvt.begin(), isize = val.ivtvt.end(); i != isize; ++i)
		{
			for(std::vector<yggr::u32>::const_iterator j = i->begin(), jsize = i->end(); j != jsize; ++j)
			{
				std::cout << *j << ' ';
			}
		}
	}

	os << "\n";


	if(val.cptr)
	{
		os << val.cptr->n;
	}
	else
	{
		os << "val.cptr empty";
	}

	os << '\n';

	return os;
}

void test1(void)
{
	type_test test;
	yggr::nsql_database_system::c_bson buf;

	oar_type oar(buf);
	oar & test;
	buf.finish();

	std::cout << buf << std::endl;

	type_test test2(10);
	iar_type iar(buf);

	iar & test2;
	std::cout << test2 << std::endl;
}

void test2(void)
{
	type_test test;
	yggr::nsql_database_system::c_bson buf;

	oar_type oar(buf);
	oar & YGGR_SERIALIZE_NAME_NVP(typeid(type_test).name(), test);
	buf.finish();

	type_test test2(10);
	iar_type iar(buf);

	iar & YGGR_SERIALIZE_NAME_NVP(typeid(type_test).name(), test2);;
	std::cout << test2 << std::endl;
}

int main(int argc, char* argv[])
{
	test1();
	test2();

	char cc = 0;
	std::cin >> cc;
	return 0;
}
