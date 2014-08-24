//proxy_mode_test.cpp

#include <iostream>

#include <boost/container/vector.hpp>
#include <boost/unordered_set.hpp>
#include <yggr/move/move.hpp>
#include <yggr/network/network_info.hpp>
#include <yggr/proxy/proxy_mode/proxy_modes.hpp>
#include <yggr/proxy/proxy_mode/proxy_mode_creator.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

typedef yggr::u64 owner_id_type;
typedef yggr::network::network_info<owner_id_type> owner_info_type;
typedef boost::unordered_set<owner_info_type> owner_info_container_type;
typedef boost::container::vector<owner_id_type> id_container_type;

typedef boost::mpl::vector
					<
						yggr::proxy::proxy_mode::proxy_mode_monopolize
						<
							owner_info_container_type
						>,

						yggr::proxy::proxy_mode::proxy_mode_source_hash
						<
							owner_info_container_type
						>,

						yggr::proxy::proxy_mode::proxy_mode_blanace
						<
							owner_info_container_type
						>,

						yggr::proxy::proxy_mode::proxy_mode_all
						<
							owner_info_container_type
						>
					> mode_vector_type;

typedef yggr::proxy::proxy_mode::proxy_mode_def proxy_mode_def_type;

typedef yggr::proxy::proxy_mode::proxy_mode_creator<
														mode_vector_type,
														owner_info_container_type
													> proxy_mode_creator_type;

typedef proxy_mode_creator_type::interface_proxy_mode_type imode_type;

int main(int argc, char* argv[])
{
	id_container_type idsc;
	owner_info_type info(owner_id_type(1));

	idsc.push_back(2);
	idsc.push_back(3);
	idsc.push_back(4);


	proxy_mode_creator_type creator;

	imode_type imode_null1 = creator(0);
	imode_type imode_null2 = creator(100);
	assert(!imode_null1);
	assert(!imode_null2);

	// monopolize
	{
		imode_type imode = creator(1);
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		owner_info_container_type cont(boost::move(imode->get_owner_info_container(info, idsc)));
#else
        owner_info_container_type cont(imode->get_owner_info_container(info, idsc));
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

		assert(cont.size() == 1);

		owner_info_type rst_info(*cont.begin());
		assert(rst_info.dst() == 2);
		assert(rst_info.src() == 1);
		assert(rst_info.owner_id() == 2);
	}

	// source_hash
	{
		imode_type imode = creator(2);
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		owner_info_container_type cont(boost::move(imode->get_owner_info_container(info, idsc)));
#else
        owner_info_container_type cont(imode->get_owner_info_container(info, idsc));
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

		assert(cont.size() == 1);

		owner_info_type rst_info(*cont.begin());
		assert(rst_info.dst() == 3);
		assert(rst_info.src() == 1);
		assert(rst_info.owner_id() == 3);
	}

	// blance
	{
		imode_type imode = creator(3);

		{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
			owner_info_container_type cont(boost::move(imode->get_owner_info_container(info, idsc)));
#else
            owner_info_container_type cont(imode->get_owner_info_container(info, idsc));
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

			assert(cont.size() == 1);

			owner_info_type rst_info(*cont.begin());
			assert(rst_info.dst() == 2);
			assert(rst_info.src() == 1);
			assert(rst_info.owner_id() == 2);
		}

		{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
			owner_info_container_type cont(boost::move(imode->get_owner_info_container(info, idsc)));
#else
            owner_info_container_type cont(imode->get_owner_info_container(info, idsc));
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

			assert(cont.size() == 1);

			owner_info_type rst_info(*cont.begin());
			assert(rst_info.dst() == 3);
			assert(rst_info.src() == 1);
			assert(rst_info.owner_id() == 3);
		}

		{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
			owner_info_container_type cont(boost::move(imode->get_owner_info_container(info, idsc)));
#else
            owner_info_container_type cont(imode->get_owner_info_container(info, idsc));
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

			assert(cont.size() == 1);

			owner_info_type rst_info(*cont.begin());
			assert(rst_info.dst() == 4);
			assert(rst_info.src() == 1);
			assert(rst_info.owner_id() == 4);
		}

		{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
			owner_info_container_type cont(boost::move(imode->get_owner_info_container(info, idsc)));
#else
            owner_info_container_type cont(imode->get_owner_info_container(info, idsc));
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

			assert(cont.size() == 1);

			owner_info_type rst_info(*cont.begin());
			assert(rst_info.dst() == 2);
			assert(rst_info.src() == 1);
			assert(rst_info.owner_id() == 2);
		}
	}

	// all
	{
		imode_type imode = creator(4);

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		owner_info_container_type cont(boost::move(imode->get_owner_info_container(info, idsc)));
#else
        owner_info_container_type cont(imode->get_owner_info_container(info, idsc));
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

		assert(cont.size() == idsc.size());

		id_container_type cmp_ids;
		for(int i = 0, isize = idsc.size(); i != isize; ++i)
		{
			owner_info_type rst_info(*cont.begin());
			cont.erase(cont.begin());

			assert(std::find(idsc.begin(), idsc.end(), rst_info.dst()) != idsc.end());
			cmp_ids.push_back(rst_info.dst());
			assert(rst_info.src() == 1);
			assert(std::find(idsc.begin(), idsc.end(), rst_info.owner_id()) != idsc.end());
		}

		assert(cmp_ids.size() == idsc.size());
		std::sort(cmp_ids.begin(), cmp_ids.end());
		assert(0 == memcmp(&idsc[0], &cmp_ids[0], sizeof(owner_id_type)* idsc.size()));
	}

	char cc = 0;
	std::cin >> cc;
	return 0;
}
