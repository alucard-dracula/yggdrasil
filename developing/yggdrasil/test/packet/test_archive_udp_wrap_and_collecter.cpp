//test_archive_udp_wrap_and_collecter.cpp

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <boost/serialization/access.hpp>
#include <boost/move/move.hpp>
#include <boost/lexical_cast.hpp>

#include <yggr/base/random.h>
#include <yggr/network/network_config/network_balance_tcp_config.hpp>
#include <yggr/network/start_data/pak_back_id.hpp>

#include <yggr/archive/net_oarchive.hpp>
#include <yggr/archive/net_iarchive.hpp>
#include <yggr/archive/network_archive_partner.hpp>

#include <yggr/network/support/data_info_def.hpp>
#include <yggr/network/network_info.hpp>
#include <yggr/network/network_packet.hpp>
#include <yggr/network/udp_packet_wrap.hpp>
#include <yggr/network/udp_packets_collecter.hpp>

#include <yggr/task_center/task_info.hpp>
#include <yggr/serialization/nvp.hpp>
#include <yggr/task_center/support/task_import.hpp>
#include <yggr/network/start_data/start_data_def.hpp>


#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

const int e_lenth = 20;

struct sdata
{
	typedef yggr::task_center::default_task_data_info_type task_data_info_type;
	typedef sdata this_type;

	enum
	{
		E_pak_id_start = 0xdadadada,
		E_pak_id,

		E_id_compile_u32 = 0xffffffff
	};

	enum
	{
		E_version_start = 0x0000000,
		E_now_ver,

		E_ver_compile_u32 = 0xffffffff
	};

	sdata(void)
	{
		memset(str1, 'z', 2047);
		str1[2047] = 0;
	}

	~sdata(void)
	{
	}

	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, yggr::u32 version)
	{
		ar & YGGR_SERIALIZE_NVP(str1);
		ar & YGGR_SERIALIZE_NVP(str2);
	}


	YGGR_FAST_CAL_TASK_IMPORT(task_data_info_type, this_type, E_pak_id, E_now_ver)

	char str1[2048];
	std::string str2;
};

typedef yggr::network::start_data::pak_back_id<yggr::ids::id64_type> test_pak_type;

typedef yggr::network::network_info<yggr::ids::id64_type> network_info_type;
typedef yggr::network::support::network_data_info_def<yggr::u32, yggr::u16> data_info_def_type;

typedef yggr::network::network_packet<
				yggr::archive::archive_partner::network_oarchive_partner,
				data_info_def_type,
				network_info_type
			> opak_type;

typedef yggr::network::network_packet<
				yggr::archive::archive_partner::network_iarchive_partner,
				data_info_def_type,
				network_info_type
			> ipak_type;

typedef yggr::network::udp_packet_wrap<opak_type::buf_type> opak_wrap_type;
typedef yggr::network::udp_packet_wrap<ipak_type::buf_type> ipak_wrap_type;

typedef opak_wrap_type::data_buf_type data_buf_type;

typedef yggr::network::udp_packets_collecter<ipak_wrap_type> ipak_wrap_collecter_type;
typedef yggr::network::udp_pak_id udp_pak_id_type;

template<typename Packet>
void get_packet(Packet& pak)
{
	sdata data2;
	pak.load(data2);

	std::cout << data2.str1 << "," << data2.str2 << std::endl;
}

void test1(void)
{
	yggr::random::init();

	typedef std::vector<data_buf_type> buf_vt_type;

	ipak_wrap_collecter_type collecter;

	sdata data1;

	strncpy(data1.str1, "aaaaaaaaaaaa", e_lenth - 1);
	data1.str2 = "bbbb";

	sdata data2;

	strncpy(data2.str1, "bbbbbbbbbbbbb", e_lenth - 1);
	data2.str2 = "cccccccc";

	std::cout << data1.str1 << "," << data1.str2 << std::endl;
	std::cout << data2.str1 << "," << data2.str2 << std::endl;

	opak_type opak1(network_info_type(yggr::u64(10)), data1);
	opak_type opak2(network_info_type(yggr::u64(10)), data2);


	opak_wrap_type opak_wrap1(opak1);
	opak_wrap_type opak_wrap2(opak2);


	buf_vt_type buf_vt(opak_wrap1.count() + opak_wrap2.count());
	{
		int idx = 0;
		{
			for(int i = 0; i != opak_wrap1.count(); ++i)
			{
				buf_vt[idx] = opak_wrap1[i];
				++idx;
			}
		}

		{
			for(int i = 0; i != opak_wrap2.count(); ++i)
			{
				buf_vt[idx] = opak_wrap2[i];
				++idx;
			}
		}
	}

	std::random_shuffle(buf_vt.begin(), buf_vt.end());

	for(int i = 0; i < buf_vt.size(); ++i)
	{
		//udp_pak_id_type id(buf_vt[i]);
		//collecter.collect(id, buf_vt[i], boost::bind(get_packet<ipak_type>, _1));
		collecter.collect(buf_vt[i]);
	}

}

int main(int argc, char* argv[])
{
	test1();

	char cc = 0;
	std::cin >> cc;
	return 0;
}
