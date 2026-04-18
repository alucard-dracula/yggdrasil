// vcsln_parser.cpp

#include "vcsln_parser.hpp"

#if !(BOOST_VERSION < 105600)

namespace vcsln
{

sln_kv_map_type::value_type make_sln_kv_value(const yggr::string& str_data)
{
	typedef sln_kv_map_type::value_type pair_type;

	yggr::string key;
	yggr::string val;

	std::size_t mid_idx = str_data.find("=");

	key.assign(str_data.begin(), str_data.begin() + mid_idx);

	val.assign(str_data.begin() + mid_idx + 1, str_data.end());

	return pair_type(boost::move(key), boost::move(val));
}

bool split_sln(yggr::string& str_global_beg,
				sln_kv_map_type& global_sln_cfg,
				yggr::string& str_global_mid,
				sln_kv_map_type& global_proj_cfg,
				yggr::string& str_global_end,
				const yggr::string& str_sln_data)
{
	//static const yggr::string gsec_beg_mark = "GlobalSection";
	static const yggr::string gsec_end_mark = "EndGlobalSection";

	static const yggr::string sln_cfg_mark = "SolutionConfigurationPlatforms";
	static const yggr::string proj_mark = "ProjectConfigurationPlatforms";


	if(str_sln_data.empty())
	{
		return false;
	}

	std::size_t step_idx = 0;

	// str_global_beg
	{
		std::size_t sln_cfg_mark_idx = str_sln_data.find(sln_cfg_mark, step_idx);
		if(sln_cfg_mark_idx == yggr::string::npos)
		{
			return false;
		}

		std::size_t global_beg_eidx = str_sln_data.find("\n", sln_cfg_mark_idx + sln_cfg_mark.size());

		if(global_beg_eidx == yggr::string::npos)
		{
			return false;
		}


		str_global_beg.assign(str_sln_data.begin() + step_idx, str_sln_data.begin() + (global_beg_eidx + 1));

		step_idx = global_beg_eidx + 1;

		//std::cout << str_global_beg << std::endl;
	}

	//global_sln_cfg
	{
		std::size_t end_sec = str_sln_data.find(gsec_end_mark, step_idx);

		std::size_t sidx = step_idx, eidx = 0;

		for(;;)
		{
			eidx = str_sln_data.find("\n", sidx);

			if(eidx < end_sec)
			{
				if(sidx < eidx)
				{
					global_sln_cfg.insert(make_sln_kv_value(str_sln_data.substr(sidx, eidx + 1 - sidx)));

					sidx = eidx + 1;

				}
				else
				{
					return false;
				}
			}
			else
			{
				break;
			}
		}

		step_idx = end_sec;
	}

	// str_global_mid
	{
		std::size_t sln_cfg_mark_idx = str_sln_data.find(proj_mark, step_idx);
		if(sln_cfg_mark_idx == yggr::string::npos)
		{
			return false;
		}

		std::size_t global_mid_eidx = str_sln_data.find("\n", sln_cfg_mark_idx + proj_mark.size());

		if(global_mid_eidx == yggr::string::npos)
		{
			return false;
		}

		str_global_mid.assign(str_sln_data.begin() + step_idx, str_sln_data.begin() + (global_mid_eidx + 1));

		step_idx = global_mid_eidx + 1;
	}

	//global_sln_cfg
	{
		std::size_t end_sec = str_sln_data.find(gsec_end_mark, step_idx);

		std::size_t sidx = step_idx, eidx = 0;

		for(;;)
		{
			eidx = str_sln_data.find("\n", sidx);

			if(eidx < end_sec)
			{
				if(sidx < eidx)
				{
					global_proj_cfg.insert(
						make_sln_kv_value(str_sln_data.substr(sidx, eidx + 1 - sidx)));

					sidx = eidx + 1;

				}
				else
				{
					return false;
				}
			}
			else
			{
				break;
			}
		}

		step_idx = end_sec;
	}

	// str_global_end
	{
		str_global_end.assign(str_sln_data.begin() + step_idx, str_sln_data.end());
	}

	return true;
}

yggr::string& merge_sln(yggr::string& sln_data,
							const yggr::string& str_global_beg,
							const sln_kv_map_type& global_sln_cfg,
							const yggr::string& str_global_mid,
							const sln_kv_map_type& global_proj_cfg,
							const yggr::string& str_global_end)
{
	typedef sln_kv_map_type::const_iterator sln_kv_map_citer_type;

	sln_data += str_global_beg;

	{
		for(sln_kv_map_citer_type i = global_sln_cfg.begin(), isize = global_sln_cfg.end(); i != isize; ++i)
		{
			sln_data += i->first;
			sln_data += "=";
			sln_data += i->second;
		}
	}

	sln_data += str_global_mid;

	{
		for(sln_kv_map_citer_type i = global_proj_cfg.begin(), isize = global_proj_cfg.end(); i != isize; ++i)
		{
			sln_data += i->first;
			sln_data += "=";
			sln_data += i->second;
		}
	}

	sln_data += str_global_end;

	return sln_data;
}


} // namespace vcsln

#endif // !(BOOST_VERSION < 105600)

