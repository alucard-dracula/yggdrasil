//test_archive.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <string>
#include <algorithm>

#include <boost/serialization/access.hpp>
#include <yggr/move/move.hpp>
#include <boost/lexical_cast.hpp>

#include <yggr/base/random.h>
#include <yggr/network/network_config/network_balance_tcp_config.hpp>
#include <yggr/network/start_data/pak_back_id.hpp>

#include <yggr/archive/network_archive_partner.hpp>

#include <yggr/network/support/data_info_def.hpp>
#include <yggr/network/network_info.hpp>
#include <yggr/network/network_packet.hpp>

#include <yggr/task_center/task_info.hpp>
#include <yggr/serialization/nvp.hpp>
#include <yggr/task_center/support/task_import.hpp>

#include <yggr/serialization/vector.hpp>
#include <yggr/serialization/list.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(system_controller)
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
	}

	~sdata(void)
	{
	}

	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, yggr::u32 version)
	{
		ar & YGGR_SERIALIZATION_NVP(u32vt);
		ar & YGGR_SERIALIZATION_NVP(u32list);
	}

	YGGR_FAST_CAL_TASK_IMPORT(task_data_info_type, this_type, (E_pak_id_start + 1), (E_version_start + 1))

	std::vector<yggr::u32> u32vt;
	std::list<yggr::u32> u32list;
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


int main(int argc, char* argv[])
{
	sdata data1;

	data1.u32vt.push_back(10);
	data1.u32list.push_back(10);

	opak_type opak(network_info_type(yggr::u64(10)), data1);

	ipak_type ipak(opak);

	sdata data2;
	ipak.load(data2);

	std::cout << data2.u32vt.front() << std::endl;
	std::cout << data2.u32list.front() << std::endl;

	wait_any_key(argc, argv);
	return 0;
}
