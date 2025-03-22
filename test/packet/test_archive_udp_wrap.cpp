//test_archive_udp_wrap.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <string>
#include <vector>
#include <algorithm>

#include <boost/serialization/access.hpp>
#include <boost/move/move.hpp>
#include <boost/lexical_cast.hpp>

#include <yggr/base/random.h>
#include <yggr/algorithm/shuffle.hpp>
#include <yggr/network/network_config/network_balance_tcp_config.hpp>
#include <yggr/network/start_data/pak_back_id.hpp>

#include <yggr/archive/net_oarchive.hpp>
#include <yggr/archive/net_iarchive.hpp>
#include <yggr/archive/network_archive_partner.hpp>

#include <yggr/network/support/data_info_def.hpp>
#include <yggr/network/network_info.hpp>
#include <yggr/network/network_packet.hpp>
#include <yggr/network/udp_packet_wrap.hpp>

#include <yggr/task_center/task_info.hpp>
#include <yggr/serialization/nvp.hpp>
#include <yggr/task_center/support/task_import.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#   include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(encryption_tool)
#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

struct sdata
{
	typedef yggr::task_center::default_task_data_info_type task_data_info_type;
	typedef sdata this_type;

	enum
	{
		E_pak_id_start = 0xdadadada,
		//E_pak_id,

		E_id_compile_u32 = 0xffffffff
	};

	enum
	{
		E_version_start = 0x0000000,
		//E_now_ver,

		E_ver_compile_u32 = 0xffffffff
	};

	sdata(void)
	{
		memset(str1, 'z', 2047);
		str1[2047] = 0;
	}

	bool operator==(const sdata& right) const
	{
		return memcmp(str1, right.str1, 2048) == 0 && str2 == right.str2;
	}

	~sdata(void)
	{
	}

	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, yggr::u32 version)
	{
		ar & YGGR_SERIALIZATION_NVP(str1);
		ar & YGGR_SERIALIZATION_NVP(str2);
	}

	YGGR_FAST_CAL_TASK_IMPORT(task_data_info_type, this_type, (E_pak_id_start + 1), (E_version_start + 1))

	char str1[2048];
	std::string str2;
};


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

void test1(void)
{
	sdata data1;

	strncpy(data1.str1, "aaaaaaaaaaaa", 1023);
	data1.str2 = "bbbb";

	opak_type opak(network_info_type(yggr::u64(10)), data1);

	opak_wrap_type opak_wrap(opak);

	ipak_wrap_type ipak_wrap(opak_wrap);

	ipak_type ipak(ipak_wrap.get_packet<opak_type>());

	sdata data2;
	ipak.load(data2);

	yggr_test_assert(data1 == data2);
	std::cout << data1.str1 << "," << data1.str2 << std::endl;
	std::cout << data2.str1 << "," << data2.str2 << std::endl;
}

void test2(void)
{
	yggr::random::init();

	typedef std::vector<data_buf_type> buf_vt_type;

	sdata data1;

	strncpy(data1.str1, "aaaaaaaaaaaa", 1023);
	data1.str2 = "bbbb";

	opak_type opak(network_info_type(yggr::u64(10)), data1);


	opak_wrap_type opak_wrap(opak);


	buf_vt_type buf_vt(opak_wrap.count());
	{
		for(int i = 0; i != buf_vt.size(); ++i)
		{
			buf_vt[i] = opak_wrap[i];
		}
	}

	yggr::shuffle(buf_vt.begin(), buf_vt.end());


	ipak_wrap_type ipak_wrap(buf_vt[0]);
	for(int i = 1; i < buf_vt.size(); ++i)
	{
		ipak_wrap.collect(buf_vt[i]);
	}

	ipak_type ipak;
	ipak_wrap >> ipak;

	sdata data2;
	ipak.load(data2);

	std::cout << data1.str1 << "," << data1.str2 << std::endl;
	std::cout << data2.str1 << "," << data2.str2 << std::endl;
}

int main(int argc, char* argv[])
{
	test1();
	test2();

	wait_any_key(argc, argv);
	return 0;
}
