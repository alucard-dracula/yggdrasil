//proxy_mode_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/container/vector.hpp>
#include <yggr/container/unordered_set.hpp>
#include <yggr/move/move.hpp>
#include <yggr/network/network_info.hpp>
#include <yggr/proxy/proxy_mode/proxy_modes.hpp>
#include <yggr/proxy/proxy_mode/proxy_mode_creator.hpp>
#include <yggr/proxy/proxy_mode/proxy_mode_owner_id_store.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

typedef yggr::u64 owner_id_type;
typedef yggr::network::network_info<owner_id_type> owner_info_type;
typedef yggr::unordered_set<owner_info_type> owner_info_container_type;

typedef yggr::proxy::proxy_mode::proxy_mode_owner_id_store<owner_info_container_type> proxy_mode_data_store_type;
typedef proxy_mode_data_store_type::store_type id_container_type;

typedef 
	boost::mpl::vector
	<
		yggr::proxy::proxy_mode::proxy_mode_monopolize
		<
			proxy_mode_data_store_type
		>,

		yggr::proxy::proxy_mode::proxy_mode_source_hash
		<
			proxy_mode_data_store_type
		>,

		yggr::proxy::proxy_mode::proxy_mode_blanace
		<
			proxy_mode_data_store_type
		>,

		yggr::proxy::proxy_mode::proxy_mode_all
		<
			proxy_mode_data_store_type
		>
	> mode_vector_type;

typedef yggr::proxy::proxy_mode::proxy_mode_def proxy_mode_def_type;

typedef 
	yggr::proxy::proxy_mode::proxy_mode_creator
	<
		mode_vector_type,
		proxy_mode_data_store_type
	> proxy_mode_creator_type;

typedef proxy_mode_creator_type::interface_proxy_mode_type imode_type;

int main(int argc, char* argv[])
{
	id_container_type idsc;
	owner_info_type info(owner_id_type(1));

	idsc.insert(2);
	idsc.insert(3);
	idsc.insert(4);

	proxy_mode_creator_type creator;

	imode_type imode_null1 = creator(0);
	imode_type imode_null2 = creator(100);
	yggr_test_assert(!imode_null1);
	yggr_test_assert(!imode_null2);

	// monopolize
	{
		imode_type imode = creator(1);

		owner_info_container_type cont;
		imode->get_owner_info_container(cont, info, idsc);

		yggr_test_assert(cont.size() == 1);

		owner_info_type rst_info(*cont.begin());
		yggr_test_assert(rst_info.owner_id(1) == 2);
		yggr_test_assert(rst_info.owner_id(0) == 1);
		yggr_test_assert(rst_info.owner_id() == 2);
	}

	// source_hash
	{
		imode_type imode = creator(2);

		owner_info_container_type cont;
		imode->get_owner_info_container(cont, info, idsc);

		yggr_test_assert(cont.size() == 1);

		owner_info_type rst_info(*cont.begin());
		yggr_test_assert(rst_info.owner_id(1) == 3);
		yggr_test_assert(rst_info.owner_id(0) == 1);
		yggr_test_assert(rst_info.owner_id() == 3);
	}

	// blance
	{
		imode_type imode = creator(3);

		{
			owner_info_container_type cont;
			imode->get_owner_info_container(cont, info, idsc);

			yggr_test_assert(cont.size() == 1);

			owner_info_type rst_info(*cont.begin());
			yggr_test_assert(rst_info.owner_id(1) == 2);
			yggr_test_assert(rst_info.owner_id(0) == 1);
			yggr_test_assert(rst_info.owner_id() == 2);
		}

		{
			owner_info_container_type cont;
			imode->get_owner_info_container(cont, info, idsc);

			yggr_test_assert(cont.size() == 1);

			owner_info_type rst_info(*cont.begin());
			yggr_test_assert(rst_info.owner_id(1) == 3);
			yggr_test_assert(rst_info.owner_id(0) == 1);
			yggr_test_assert(rst_info.owner_id() == 3);
		}

		{
			owner_info_container_type cont;
			imode->get_owner_info_container(cont, info, idsc);

			yggr_test_assert(cont.size() == 1);

			owner_info_type rst_info(*cont.begin());
			yggr_test_assert(rst_info.owner_id(1) == 4);
			yggr_test_assert(rst_info.owner_id(0) == 1);
			yggr_test_assert(rst_info.owner_id() == 4);
		}

		{
			owner_info_container_type cont;
			imode->get_owner_info_container(cont, info, idsc);

			yggr_test_assert(cont.size() == 1);

			owner_info_type rst_info(*cont.begin());
			yggr_test_assert(rst_info.owner_id(1) == 2);
			yggr_test_assert(rst_info.owner_id(0) == 1);
			yggr_test_assert(rst_info.owner_id() == 2);
		}
	}

	// all
	{
		imode_type imode = creator(4);

		owner_info_container_type cont;
		imode->get_owner_info_container(cont, info, idsc);

		yggr_test_assert(cont.size() == idsc.size());

		id_container_type cmp_ids;
		for(int i = 0, isize = idsc.size(); i != isize; ++i)
		{
			owner_info_type rst_info(*cont.begin());
			cont.erase(cont.begin());

			yggr_test_assert(idsc.find(rst_info.owner_id(1)) != idsc.end());
			cmp_ids.insert(rst_info.owner_id(1));
			yggr_test_assert(rst_info.owner_id(0) == 1);
			yggr_test_assert(idsc.find(rst_info.owner_id()) != idsc.end());
		}

		yggr_test_assert(cmp_ids.size() == idsc.size());
	}

	wait_any_key(argc, argv);
	return 0;
}
