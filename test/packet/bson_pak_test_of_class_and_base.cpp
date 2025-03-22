// bson_pak_test_of_class_and_base.cpp

#define YGGR_SERIALIZE_SIZE32

#include <test/wait_any_key/wait_any_key.hpp>
#include <string>
#include <vector>

#include <yggr/base/yggrdef.h>

#include <yggr/packet/packet.hpp>
#include <yggr/archive/network_archive_partner.hpp>

#include <yggr/archive/bson_oarchive.hpp>
#include <yggr/archive/bson_iarchive.hpp>

#include <yggr/serialization/nvp.hpp>
#include <yggr/archive/bson_archive_partner.hpp>

#include <yggr/serialization/string.hpp>
#include <yggr/serialization/utf8_string.hpp>

#include <yggr/serialization/shared_ptr.hpp>

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


struct type_test
{
	type_test(void)
	{
		_s8 = 100;
		_char = 'a';
		_u8 = 200;
		_s16 = 10000;
		_u16 = 20000;
		_s32 = 1000000;
		_u32 = 2000000;

		_s64 = 1000000000;
		_u64 = 2000000000;

		_f32 = 100.12345f;
		_f64 = 200.1234567;

		_str = "abcdefg";
		_buf = "ABCDEFG";

		_regex = yggr::nsql_database_system::c_bson_regex("^asdf", "imx");
		_code = yggr::nsql_database_system::c_bson_code("function aaa(){}");
		_symbol = yggr::nsql_database_system::c_bson_symbol("symbol");

		yggr::nsql_database_system::c_bson scope;
		scope.save("i", 123);
		scope.finish();

		_wscope = yggr::nsql_database_system::c_bson_code_w_scope<yggr::nsql_database_system::c_bson>("function(){return i}", scope);
		_timestamp = yggr::nsql_database_system::c_bson_timestamp(100, 200);

		opacket_type opak;
		opak.save(100);
		_pak_buf = opak.org_buf();

	}

	type_test(int n)
	{
		_s8 = 0;
		_char = 0;
		_u8 = 0;
		_s16 = 0;
		_u16 = 0;
		_s32 = 0;
		_u32 = 0;

		_s64 = 0;
		_u64 = 0;

		_f32 = 0;
		_f64 = 0;

		_date = 0;
		_time = 0;
	}

private :
	friend class boost::serialization::access;
	template<typename Archive>
	void serialize(Archive & ar, yggr::u32 version)
    {
        typedef Archive archive_type;

		typedef
			yggr::nsql_database_system::c_bson_binary_buffer_ref
			<
				yggr::nsql_database_system::bson_binary_typeid_def::E_BSON_BIN_TYPE_U8_BUFFER,
				std::string
			> c_bson_u8_binary_buffer_ref_type;

		typedef
			yggr::nsql_database_system::c_bson_binary_buffer_ref
			<
				yggr::nsql_database_system::bson_binary_typeid_def::E_BSON_BIN_TYPE_PACKET,
				std::string
			> c_bson_pak_binary_buffer_ref_type;

		ar & YGGR_SERIALIZATION_NVP(_s8);
		ar & YGGR_SERIALIZATION_NVP(_char);
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
		ar & YGGR_SERIALIZATION_NVP(_date);
		ar & YGGR_SERIALIZATION_NVP(_time);
		ar & YGGR_SERIALIZATION_NVP(_oid);

		ar & YGGR_SERIALIZATION_NVP(_null);
		ar & YGGR_SERIALIZATION_NVP(_undefined);

		ar & YGGR_SERIALIZATION_NVP(_regex);
		ar & YGGR_SERIALIZATION_NVP(_code);
		ar & YGGR_SERIALIZATION_NVP(_symbol);
		ar & YGGR_SERIALIZATION_NVP(_wscope);

		ar & YGGR_SERIALIZATION_NVP(_timestamp);

        c_bson_u8_binary_buffer_ref_type bin_buffer(_buf);
        ar & YGGR_SERIALIZATION_NVP(bin_buffer);
		c_bson_pak_binary_buffer_ref_type bin_pak(_pak_buf);
        ar & YGGR_SERIALIZATION_NVP(bin_pak);

	}

public:
	yggr::s8 _s8;
	char _char;
	yggr::u8 _u8;
	yggr::s16 _s16;
	yggr::u16 _u16;
	yggr::s32 _s32;
	yggr::u32 _u32;
	yggr::s64 _s64;
	yggr::u64 _u64;

	yggr::f32 _f32;
	yggr::f64 _f64;

	std::string _str;

	yggr::nsql_database_system::c_bson_date _date;
	yggr::nsql_database_system::c_bson_time _time;
	yggr::nsql_database_system::c_bson_oid _oid;

	yggr::nsql_database_system::c_bson_null _null;
	yggr::nsql_database_system::c_bson_undefined _undefined;

	yggr::nsql_database_system::c_bson_regex _regex;
	yggr::nsql_database_system::c_bson_code _code;
	yggr::nsql_database_system::c_bson_symbol _symbol;
	yggr::nsql_database_system::c_bson_code_w_scope<yggr::nsql_database_system::c_bson> _wscope;

	yggr::nsql_database_system::c_bson_timestamp _timestamp;

	std::string _buf;

	std::string _pak_buf;

};


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
class tt
{
public:
public:

private:
	friend class boost::serialization::access;
	template<typename Archive>
	void serialize(Archive & ar, yggr::u32 version)
	{
		ar & YGGR_SERIALIZATION_NVP(a);
		ar & YGGR_SERIALIZATION_NVP(b);
		ar & YGGR_SERIALIZATION_NVP(c);
		ar & YGGR_SERIALIZATION_NVP(ptr);
	}

public:
	yggr::u16 a;
	yggr::u64 b;
	yggr::u64 c;

	yggr::shared_ptr<in_val_t> ptr;
};

std::ostream& operator<<(std::ostream& os, const type_test& val)
{
	os << "s8 = " << val._s8 << '\n';
	os << "char = " << val._char << '\n';
	os << "u8 = " << (int)val._u8 << '\n';
	os << "s16 = " << val._s16 << '\n';
	os << "u16 = " << val._u16 << '\n';
	os << "s32 = " << val._s32 << '\n';
	os << "u32 = " << val._u32 << '\n';
	os << "s64 = " << val._s64 << '\n';
	os << "u64 = " << val._u64 << '\n';

	os << "f32 = " << val._f32 << '\n';
	os << "f64 = " << val._f64 << '\n';

	os << "string = " << val._str << '\n';
	os << "date = " << val._date << '\n';
	os << "time = " << val._time << '\n';
	os << "oid = " << val._oid << '\n';

	os << "null = " << '\n';
	os << "undefine = " << '\n';

	os << "regex = " << val._regex.regex() << " " << val._regex.options() << "\n";
	os << "code = " << val._code.code() << "\n";
	os << "symbol = " << val._symbol.str() << "\n";

	//int n = 0;

	//yggr::nsql_database_system::c_bson::const_iterator iter(val._wscope.scope());
	//iter.load("i", n);

	//os << "scope = " << val._wscope.code() << ", " << n << "\n";

	os << "scope = " << val._wscope << "\n";

	os << "timestamp = " << val._timestamp.i << ", " << val._timestamp.t << "\n";


	os << "buf = " << val._buf	<< '\n';

	int a = 0;
	ipacket_type ipak(val._pak_buf);
	ipak.load(a);

	os << "pak_buf = " << a << '\n';
	return os;
}

void test1_1(void)
{
	type_test test; //create unknow object
	o_bson_packet_type opak;
	opak.save(YGGR_SERIALIZATION_NAME_NVP("test_ttt", test));

	std::cout << opak.org_buf() << std::endl;

	type_test test2(10);
	i_bson_packet_type ipak(opak);

	ipak.load(YGGR_SERIALIZATION_NAME_NVP("test_ttt", test2));
	std::cout << test2 << std::endl;

}

void test1_2(void)
{
	type_test test; //create unknow object
	o_bson_packet_type opak;
	opak.save(test);

	std::cout << opak.org_buf() << std::endl;

	type_test test2(10);
	i_bson_packet_type ipak(opak);
	ipak.load(test2);
	std::cout << test2 << std::endl;
}

void test2(void)
{
	tt t1;
	t1.a = 10;
	t1.b = 20;
	t1.c = 30;
	t1.ptr = yggr::shared_ptr<in_val_t>(new in_val_t(100));

	o_bson_packet_type opak;
	opak.save(YGGR_SERIALIZATION_NAME_NVP("tt", t1));

	std::cout << "org_buf = " << opak.org_buf() << std::endl;

	tt t2;
	t2.a = 0;
	t2.b = 0;
	t2.c = 0;
	std::cout << t2.a << ", "<< t2.b << ", "<< t2.c << std::endl;

	i_bson_packet_type ipak(opak); //

	ipak.load(YGGR_SERIALIZATION_NAME_NVP("tt", t2));

	std::cout << t2.a << ", "<< t2.b << ", "<< t2.c << ", " << (*t2.ptr).n << std::endl;
}

int main(int argc, char* argv[])
{
	test1_1();
	test1_2();
	test2();

	wait_any_key(argc, argv);
	return 0;
}
