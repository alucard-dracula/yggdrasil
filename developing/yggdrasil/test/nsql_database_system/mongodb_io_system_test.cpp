//mongodb_io_system_test.cpp

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include <yggr/nsql_database_system/mongodb_config.h>
#include <yggr/nsql_database_system/c_bson.hpp>

#include <yggr/nsql_database_system/c_mongo_connection.hpp>
#include <yggr/nsql_database_system/mongo_accesser.hpp>
#include <yggr/nsql_database_system/mongodb_io_system.hpp>

#include <yggr/packet/packet.hpp>
#include <yggr/archive/network_archive_partner.hpp>
#include <yggr/archive/bson_archive_partner.hpp>

#include <yggr/sync_system/sync_default_config.hpp>
#include <yggr/sync_system/sync_helper.hpp>
#include <yggr/sync_system/sync_container_mgr.hpp>

#include <yggr/serialization/nvp.hpp>
#include <yggr/serialization/bson_data_type.hpp>
#include <yggr/serialization/array.hpp>
#include <yggr/serialization/vector.hpp>
#include <yggr/serialization/list.hpp>
#include <yggr/serialization/set.hpp>
#include <yggr/serialization/map.hpp>
#include <yggr/serialization/unordered_set.hpp>
#include <yggr/serialization/unordered_map.hpp>
#include <yggr/serialization/utf8_string.hpp>
#include <yggr/serialization/string.hpp>


#ifdef _MSC_VER
#   include <vld.h>
#endif //_MSC_VER

typedef yggr::sync_system::default_params_type params_type;
typedef yggr::sync_system::default_sync_helper_type helper_type;
typedef yggr::sync_system::default_container_mgr_type container_mgr_type;

typedef yggr::nsql_database_system::c_mongo_connection conn_type;
typedef yggr::nsql_database_system::mongodb_io_system<conn_type, container_mgr_type> mongodb_io_system_type;

typedef yggr::packet::packet<yggr::archive::archive_partner::network_oarchive_partner> opacket_type;
typedef yggr::packet::packet<yggr::archive::archive_partner::network_iarchive_partner> ipacket_type;

typedef yggr::packet::packet<yggr::archive::archive_partner::bson_oarchive_partner> o_bson_packet_type;
typedef yggr::packet::packet<yggr::archive::archive_partner::bson_iarchive_partner> i_bson_packet_type;

struct type_test
{
	type_test(void)
	{
		_s8 = 97;
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

		opacket_type opak;
		opak.save(100);
		_pak_buf = opak.org_buf();

		for(int i = 0; i != 3; ++i)
		{
			ivt.push_back(i + 1);
		}

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

	template<typename Archive>
	void serialize(Archive & ar, yggr::u32 version)
	{
		ar & YGGR_SERIALIZE_NAME_NVP("s8", _s8);
		ar & YGGR_SERIALIZE_NAME_NVP("u8", _u8);
		ar & YGGR_SERIALIZE_NAME_NVP("s16", _s16);
		ar & YGGR_SERIALIZE_NAME_NVP("u16", _u16);
		ar & YGGR_SERIALIZE_NAME_NVP("s32", _s32);
		ar & YGGR_SERIALIZE_NAME_NVP("u32", _u32);
		ar & YGGR_SERIALIZE_NAME_NVP("s64", _s64);
		ar & YGGR_SERIALIZE_NAME_NVP("u64", _u64);

		ar & YGGR_SERIALIZE_NAME_NVP("f32", _f32);
		ar & YGGR_SERIALIZE_NAME_NVP("f64", _f64);

		ar & YGGR_SERIALIZE_NAME_NVP("str", _str);

		YGGR_SERIALIZE_WRAP_NVP_AR(ar, yggr::nsql_database_system::c_bson_u8_binary_buffer, _buf);
		YGGR_SERIALIZE_WRAP_NVP_AR(ar, yggr::nsql_database_system::c_bson_bpak_binary_buffer, _pak_buf);

		ar & YGGR_SERIALIZE_NAME_NVP("ivt", ivt);
	}

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

	std::string _str;

	yggr::nsql_database_system::c_bson_date _date;
	yggr::nsql_database_system::c_bson_time _time;
	yggr::nsql_database_system::c_bson_oid _oid;

	std::string _buf;

	std::string _pak_buf;

	std::vector<yggr::u32> ivt;

};

std::ostream& operator<<(std::ostream& os, const type_test& val)
{
	os << "s8 = " << val._s8 << '\n';
	os << "u8 = " << val._u8 << '\n';
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
	os << "buf = " << val._buf	<< '\n';

	int a = 0;
	ipacket_type ipak(val._pak_buf);
	ipak.load(a);

	os << "pak_buf = " << a << '\n';

	os << "ivt = ";
	for(int i= 0; i != val.ivt.size(); ++i)
	{
		os << val.ivt[i] << " ";
	}

	os << "\n";
	return os;
}

class base_database_container
{
public:

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

	class save_data
	{
	public:
		save_data(type_test& nt)
			: t(nt)
		{
		}

		bool operator>>(helper_type& helper) const
		{
			helper.result()["data"] = t;
			return true;
		}

		type_test& t;
	};

	class save_back_data
	{
	public:
		save_back_data(void)
		{
		}

		bool operator<<(const helper_type& helper) const
		{
			//fix helper context;
			return true;
		}
	};

	class load_data
	{
	public:
		load_data(void)
		{
		}

		bool operator<<(helper_type& helper)
		{
			vt.swap(helper.result().get_val<test_vt_type>("datas"));
			return true;
		}

		bool operator<<(const helper_type& helper)
		{
			vt = helper.result().get_val<test_vt_type>("datas");
			return true;
		}

		test_vt_type vt;
	};

	class load_cdt
	{
	public:
		load_cdt(const yggr::nsql_database_system::c_bson& b1, const yggr::nsql_database_system::c_bson& b2)
			: _b1(b1), _b2(b2)
		{
		}

		~load_cdt(void)
		{
		}

		bool operator>>(helper_type& helper) const
		{
			helper.condition()["bson1"] = _b1;
			helper.condition()["bson2"] = _b2;
			return true;
		}

		const yggr::nsql_database_system::c_bson& _b1;
		const yggr::nsql_database_system::c_bson& _b2;
	};

	template<typename Tuple>
	base_database_container(const std::string& db_name, const std::string& coln_name)
		:db(db_name), coln(coln_name)
	{
	}

	template<typename Tuple>
	base_database_container(const Tuple& tuple)
		:db(boost::get<0>(tuple)), coln(boost::get<1>(tuple))
	{
	}

	~base_database_container(void)
	{
		_vt.clear();
	}

	template<typename Accesser, typename Helper>
	bool save(Accesser& accesser, Helper& helper)
	{
		helper.result()["abc"] = 100;
		_vt.push_back(helper.result().template get_val<type_test>("data"));
		return true;
	}

	template<typename Accesser, typename Helper>
	bool load(Accesser& accesser, Helper& helper)
	{
		if(_vt.empty())
		{
			Saver saver(_vt);

			//yggr::nsql_database_system::c_bson *pb1 = helper.result().template get_val_ptr<yggr::nsql_database_system::c_bson>("bson1"),
			//									*pb2 = helper.result().template get_val_ptr<yggr::nsql_database_system::c_bson>("bson2");
			yggr::nsql_database_system::c_bson *pb1 = helper.condition().template get_val_ptr<yggr::nsql_database_system::c_bson>("bson1"),
												*pb2 = helper.condition().template get_val_ptr<yggr::nsql_database_system::c_bson>("bson2");

			if(pb1 && pb2)
			{
				accesser.find(saver, db, coln, *pb1, *pb2);
			}
			else
			{
				yggr::nsql_database_system::c_bson b1,b2;
				accesser.find(saver, db, coln, b1, b2);
			}
		}

		helper.result()["datas"] = _vt;

		return true;
	}

	template<typename Accesser, typename Helper>
	bool load(Accesser& accesser, Helper& helper) const
	{
		if(_vt.empty())
		{
			return false;
		}

		helper.result()["datas"] = _vt;
		return true;
	}

	//template<typename Accesser>
	//bool execute(Accesser& accesser)
	//{
	//	accesser.remove_all(db, coln);
	//	return true;
	//}

	template<typename Accesser, typename Helper>
	bool execute(Accesser& accesser, const Helper& helper, helper_type& out_helper)
	{
		if(_vt.empty())
		{
			return false;
		}

		accesser.remove_all(db, coln);

		std::list<o_bson_packet_type> pak_list;

		for(yggr::size_type i = 0, isize = _vt.size(); i != isize; ++i)
		{
			o_bson_packet_type pak;
			pak.save(_vt[i]);
			pak_list.push_back(pak);
		}

		accesser.insert(db, coln, pak_list);
		_vt.clear();
		return true;
	}

	static std::string type_id(void)
	{
		return std::string("test");
	}

	std::string db;
	std::string coln;
	std::vector<type_test> _vt;
};

void out(bool bright, const base_database_container::load_data& ld)
{
	if(!bright)
	{
		return;
	}
	for(int i = 0, isize = ld.vt.size(); i != isize; ++i)
	{
		std::cout << ld.vt[i] << std::endl;
	}
}

void save_out(bool bright, const base_database_container::save_back_data& sd)
{
	if(!bright)
	{
	}
	std::cout << "--------------------saved-------------------------" << std::endl;
}

int main(int argc, char* argv[])
{
	//mongodb_io_system_type io_sys;
	mongodb_io_system_type io_sys(std::string("127.0.0.1:10098"));
	//mongodb_io_system_type io_sys("127.0.0.1:10098", "xy", "123456", 5);

	io_sys.start();

	io_sys.reg_container<base_database_container>(boost::make_tuple(std::string("test"), std::string("foo")));
	base_database_container::test_vt_type vt;

	for(int i = 0, isize = 100; i != isize; ++i)
	{
		type_test t;
		bool bright = io_sys.save<base_database_container::save_back_data>(
																base_database_container::type_id(),
																base_database_container::save_data(t),
																boost::bind(&save_out, _1, _2));
		assert(bright);	
	}

	//{
	//	// wait save end // use wait commit all item
	//	char cc = 0;
	//	std::cin >>cc;
	//}
	io_sys.commit(base_database_container::type_id());

	yggr::nsql_database_system::c_bson b1, b2;
	io_sys.load<base_database_container::load_data>(base_database_container::type_id(),
														base_database_container::load_cdt(b1, b2),
														boost::bind(&out, _1, _2));

	io_sys.load<base_database_container::load_data>(base_database_container::type_id(), boost::bind(&out, _1, _2));

	char cc = 0;
	std::cin >> cc;

	io_sys.stop();

	return 0;
}
