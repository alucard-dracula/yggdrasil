//bson_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <utility>
#include <string>

#include <yggr/nsql_database_system/mongodb_config.hpp>
#include <yggr/nsql_database_system/c_bson.hpp>

#include <yggr/packet/packet.hpp>
#include <yggr/archive/network_archive_partner.hpp>

#include <yggr/serialization/shared_ptr.hpp>
#include <yggr/serialization/bson_inner_data.hpp>
#include <yggr/serialization/array.hpp>
#include <yggr/serialization/vector.hpp>
#include <yggr/serialization/list.hpp>
#include <yggr/serialization/set.hpp>
#include <yggr/serialization/map.hpp>
#include <yggr/serialization/unordered_set.hpp>
#include <yggr/serialization/unordered_map.hpp>

#include <yggr/archive/bson_oarchive.hpp>
#include <yggr/archive/bson_iarchive.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

typedef yggr::packet::packet<yggr::archive::archive_partner::network_oarchive_partner> opacket_type;
typedef yggr::packet::packet<yggr::archive::archive_partner::network_iarchive_partner> ipacket_type;

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
		ar & YGGR_SERIALIZATION_NVP(n);
	}

	yggr::s32 n;
};

BOOST_SERIALIZATION_SHARED_PTR(C)

struct type_test
{
	type_test(void)
		: _regex("this is a regex", "this is an opts"),
			_code("this is a code"),
			_symbol("this is a symbol"),
			_code_w_scope_bson("this is a code_w_scope_bson",
								yggr::nsql_database_system::c_bson("{\"aaa\" : 10}")),
			_code_w_scope_c_bson("this is a code_w_scope_c_bson",
									yggr::nsql_database_system::c_bson("{\"bbb\" : 20}")),
			_dbpointer_oid("this is a dbpointer_oid", yggr::nsql_database_system::c_bson_oid()),
			_dbpointer_coid("this is a dbpointer_coid", yggr::nsql_database_system::c_bson_oid()),
			_bson_value(std::string("c_bson_value")),
			_pak_buf_ref(_pak_buf)
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

		_timeval_basic.tv_sec = 100;
		_timeval_basic.tv_usec = 1; // out put is 0

		_oid_basic = _oid;
		bson_value_copy(&_bson_value, &_bson_value_basic);

		_str = "abcdefg";

		_decimal128 = "1234567890";
		_decimal128_basic = _decimal128;

		_buf = "ABCDEFG";

		opacket_type opak;
		opak.save(100);
		_pak_buf = opak.org_buf();

		{
			for(int i = 0; i != 3; ++i)
			{
				char_arr[i] = i + 1;
				int_arr[i] =  i + 1;
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
		cptr = yggr::shared_ptr<C>(new C(70000));
	}

	type_test(int n)
		: _date(0),
			_time(0),
			_timeval(0, 0),
			_oid(yggr::nsql_database_system::c_bson_oid::s_gen_empty_oid()),
			_pak_buf_ref(_pak_buf)
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

		memset(&_decimal128_basic, 0, sizeof(_decimal128_basic));

		_timeval_basic.tv_sec = 0;
		_timeval_basic.tv_usec = 0;

		memset(char_arr, 0, sizeof(char_arr));
		memset(int_arr, 0, sizeof(int_arr));

		memset(&_oid_basic, 0, sizeof(bson_oid_t));
		memset(&_bson_value_basic, 0, sizeof(bson_value_t));
	}

	~type_test(void)
	{
		yggr::nsql_database_system::bson_value_native_ex::s_bson_value_clear(&_bson_value_basic);
		//bson_value_destroy(&_bson_value_basic);
	}

private:
	friend class boost::serialization::access;
	template<typename Archive>
	void serialize(Archive & ar, yggr::u32 version)
	{
		ar & YGGR_SERIALIZATION_NVP(_s8);
		ar & YGGR_SERIALIZATION_NVP(_u8);
		ar & YGGR_SERIALIZATION_NVP(_s16);
		ar & YGGR_SERIALIZATION_NVP(_u16);
		ar & YGGR_SERIALIZATION_NVP(_s32);
		ar & YGGR_SERIALIZATION_NVP(_u32);
		ar & YGGR_SERIALIZATION_NVP(_s64);
		ar & YGGR_SERIALIZATION_NVP(_u64);

		ar & YGGR_SERIALIZATION_NVP(_f32);
		ar & YGGR_SERIALIZATION_NVP(_f64);

		ar & YGGR_SERIALIZATION_NVP(_str);

		ar & YGGR_SERIALIZATION_NVP(_decimal128);
		ar & YGGR_SERIALIZATION_NVP(_decimal128_basic);

		ar & YGGR_SERIALIZATION_NVP(_date);
		ar & YGGR_SERIALIZATION_NVP(_time);

		ar & YGGR_SERIALIZATION_NVP(_timeval);
		ar & YGGR_SERIALIZATION_NVP(_timeval_basic);
	
		ar & YGGR_SERIALIZATION_NVP(_oid);
		ar & YGGR_SERIALIZATION_NVP(_oid_basic);

		ar & YGGR_SERIALIZATION_NVP(_regex);
		ar & YGGR_SERIALIZATION_NVP(_code);
		ar & YGGR_SERIALIZATION_NVP(_symbol);
		ar & YGGR_SERIALIZATION_NVP(_code_w_scope_bson);
		ar & YGGR_SERIALIZATION_NVP(_code_w_scope_c_bson);

		ar & YGGR_SERIALIZATION_NVP(_dbpointer_oid);
		ar & YGGR_SERIALIZATION_NVP(_dbpointer_coid);

		ar & YGGR_SERIALIZATION_NVP(_bson_value);
		ar & YGGR_SERIALIZATION_NVP(_bson_value_basic);
		
		ar & YGGR_SERIALIZATION_NVP(_null);
		ar & YGGR_SERIALIZATION_NVP(_undefined);

		ar & YGGR_SERIALIZATION_NVP(_minkey);
		ar & YGGR_SERIALIZATION_NVP(_maxkey);

		ar & YGGR_SERIALIZATION_NVP(_buf);
		ar & YGGR_SERIALIZATION_NVP(_pak_buf_ref);

		//don't forgot YGGR_SERIALIZATION_WRAP_NVP_AR

		ar & YGGR_SERIALIZATION_NVP(char_arr);
		ar & YGGR_SERIALIZATION_NVP(int_arr);

		ar & YGGR_SERIALIZATION_NVP(ivt);

		ar & YGGR_SERIALIZATION_NVP(ilist);

		ar & YGGR_SERIALIZATION_NVP(iset);
		ar & YGGR_SERIALIZATION_NVP(imset);
		ar & YGGR_SERIALIZATION_NVP(ipair);
		ar & YGGR_SERIALIZATION_NVP(imap);
		ar & YGGR_SERIALIZATION_NVP(immap);

		ar & YGGR_SERIALIZATION_NVP(ihset);
		ar & YGGR_SERIALIZATION_NVP(imhset);

		ar & YGGR_SERIALIZATION_NVP(ihmap);
		ar & YGGR_SERIALIZATION_NVP(imhmap);

		ar & YGGR_SERIALIZATION_NVP(ivtvt);

		ar & YGGR_SERIALIZATION_NVP(cptr);
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

	yggr::nsql_database_system::c_bson_decimal128 _decimal128;
	yggr::nsql_database_system::c_bson_decimal128::org_type _decimal128_basic;
	yggr::nsql_database_system::c_bson_date _date;
	yggr::nsql_database_system::c_bson_time _time;
	yggr::nsql_database_system::c_bson_timeval _timeval;
	yggr::nsql_database_system::c_bson_timeval::org_type _timeval_basic;
	yggr::nsql_database_system::c_bson_oid _oid;
	yggr::nsql_database_system::c_bson_oid::org_type _oid_basic;

	yggr::nsql_database_system::c_bson_regex _regex;
	yggr::nsql_database_system::c_bson_code _code;
	yggr::nsql_database_system::c_bson_symbol _symbol;
	yggr::nsql_database_system::c_bson_code_w_scope<bson_t> _code_w_scope_bson;
	yggr::nsql_database_system::c_bson_code_w_scope<
		yggr::nsql_database_system::c_bson> _code_w_scope_c_bson;

	yggr::nsql_database_system::c_bson_dbpointer<bson_oid_t> _dbpointer_oid;
	yggr::nsql_database_system::c_bson_dbpointer<
		yggr::nsql_database_system::c_bson_oid> _dbpointer_coid;

	yggr::nsql_database_system::c_bson_value _bson_value;
	yggr::nsql_database_system::c_bson_value::org_type _bson_value_basic;

	yggr::nsql_database_system::c_bson_null _null;
	yggr::nsql_database_system::c_bson_undefined _undefined;

	yggr::nsql_database_system::c_bson_minkey _minkey;
	yggr::nsql_database_system::c_bson_maxkey _maxkey;

	yggr::nsql_database_system::c_bson_u8_binary_buffer _buf;

	std::string _pak_buf;
	yggr::nsql_database_system::c_bson_binary_buffer_ref
		<
			yggr::nsql_database_system::bson_binary_typeid_def::E_BSON_BIN_TYPE_PACKET,
			std::string
		> _pak_buf_ref;

	char char_arr[3];
	int int_arr[3];

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

	yggr::shared_ptr<C> cptr;
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

	os << "decimal128 = " << val._decimal128 << '\n';
	os << "basic_decimal128 = " << val._decimal128_basic << '\n';

	os << "date = " << val._date << '\n';
	os << "time = " << val._time << '\n';

	os << "timeval = " << val._timeval << '\n';
	os << "basic_timeval = " << val._timeval_basic.tv_sec << ", " << val._timeval_basic.tv_usec << '\n';
	
	os << "oid = " << val._oid << '\n';
	os << "oid_basic = " << val._oid_basic << '\n';

	os << "regex = " << val._regex << '\n';
	os << "code = " << val._code << '\n';
	os << "symbol = " << val._symbol << '\n';
	os << "code_w_scope_bson = " << val._code_w_scope_bson << '\n';
	os << "code_w_scope_c_bson = " << val._code_w_scope_c_bson << '\n';

	os << "dbpointer_oid = " << val._dbpointer_oid << '\n';
	os << "dbpointer_coid = " << val._dbpointer_coid << '\n';

	os << "bson_value = " << val._bson_value << '\n';
	os << "bson_value_basic = " << val._bson_value_basic << '\n';
	
	os << "null = " << val._null << '\n';
	os << "undefined = " << val._undefined << '\n';

	os << "minkey = " << val._minkey << '\n';
	os << "maxkey = " << val._maxkey << '\n';

	os << "buf = " << val._buf	<< '\n';
	os << "buf_ref = " << val._pak_buf_ref << '\n';

	int a = 0;
	ipacket_type ipak(val._pak_buf);
	ipak.load(a);

	os << "pak_buf = " << a << '\n';

	os << "char_arr = ";
	{
		for(int i = 0; i != val.ivt.size(); ++i)
		{
			os << static_cast<int>(val.char_arr[i]) << ' ';
		}

		os << '\n';
	}

	os << "int_arr = ";
	{
		for(int i = 0; i != val.ivt.size(); ++i)
		{
			os << val.int_arr[i] << ' ';
		}

		os << '\n';
	}

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
	oar & YGGR_SERIALIZATION_NAME_NVP(typeid(type_test).name(), test);

	std::cout << buf << std::endl;

	type_test test2(10);
	iar_type iar(buf);

	iar & YGGR_SERIALIZATION_NAME_NVP(typeid(type_test).name(), test2);;
	std::cout << test2 << std::endl;
}

#include <yggr/smart_ptr_ex/weak_ptr.hpp>

int main(int argc, char* argv[])
{
	test1();
	test2();

	wait_any_key(argc, argv);
	return 0;
}
