// vcsln_parser.cpp

#include "vcsln_parser.hpp"

#if !(BOOST_VERSION < 105600)

#include "file_operator.hpp"

#include <utility>

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <yggr/iterator_ex/iterator.hpp>
#include <yggr/algorithm/knuth_morris_pratt.hpp>

namespace vcsln
{

bool check_vcxproj_pair(const vcxproj_map_type::key_type& key, 
						const vcxproj_map_type::mapped_type& mapped)
{
	//.vcxproj
	return 
		(key.length() < mapped.length())
		&& (mapped.org_str().substr(mapped.size() - 8) == ".vcxproj");
}

template<typename Iter> inline
Iter parse_vcxproj_pair(vcxproj_map_type& out, Iter s, Iter e)
{
	typedef vcxproj_map_type::key_type key_type;
	typedef vcxproj_map_type::mapped_type mapped_type;

	typedef Iter iter_type;

	if(s == e)
	{
		return e;
	}

	iter_type i = s, isize = e;
	key_type key;
	mapped_type mapped;
	// key
	{
		if((i = std::find(i, isize, '=')) == isize) return e;
		if((i = std::find(yggr::advance_copy(i, 1), isize, '\"')) == isize) return e;
		if((isize = std::find(yggr::advance_copy(i, 1), isize, '\"')) == e) return e;

		key.org_str().assign(yggr::advance_copy(i, 1), isize);
		key.recount_length();
	}

	i = yggr::advance_copy(isize, 1);
	isize = e;

	//value
	{
		if((i = std::find(i, isize, '\"')) == isize) return e;
		if((isize = std::find(yggr::advance_copy(i, 1), isize, '\"')) == e) return e;

		mapped.org_str().assign(yggr::advance_copy(i, 1), isize);
		file_op::format_string_file_path(mapped.org_str());
		mapped.recount_length();
	}

	return 
		((check_vcxproj_pair(key, mapped) 
			&& out.insert(std::make_pair(key, mapped)).second))? 
		isize : e;


}

vcxproj_map_type& get_vcxproj_map_from_buffer(vcxproj_map_type& out, const yggr::utf8_string& file_data)
{
	typedef yggr::utf8_string::base_type org_string_type;
	typedef org_string_type::const_iterator org_str_citer_type;
	typedef yggr::knuth_morris_pratt<org_str_citer_type> kmp_type;
	typedef std::pair<org_str_citer_type, org_str_citer_type> kmp_return_type;

	BOOST_MPL_ASSERT((boost::is_same<org_string_type, ptree_string_type>));

	static const org_string_type s_str_project("Project(");

	static const kmp_type s_kmp_project(s_str_project.begin(), s_str_project.end());

	if(file_data.empty())
	{
		return out;
	}

	const org_string_type& str_fdata = file_data.org_str();

	kmp_return_type proj_i_pair = s_kmp_project(str_fdata.begin(), str_fdata.end());

	if(proj_i_pair.second == str_fdata.end())
	{
		return out;
	}

	org_str_citer_type proj_isize = str_fdata.end();

	vcxproj_map_type tmp_out;
	for(;proj_i_pair.second != proj_isize; proj_i_pair = s_kmp_project(yggr::advance_copy(proj_i_pair.second, 1), proj_isize))
	{
		if((proj_i_pair.second = parse_vcxproj_pair(tmp_out, proj_i_pair.second, proj_isize)) == proj_isize)
		{
			return out;
		}

		assert(proj_i_pair.second < proj_isize);
	}

	std::size_t chk_size = out.size() + tmp_out.size();

	out.insert(tmp_out.begin(), tmp_out.end());

	if(out.size() != chk_size)
	{
		vcxproj_map_type tmp;
		tmp.swap(out);
	}

	return out;
}

vcxproj_map_type& get_vcxproj_map_from_file(vcxproj_map_type& out, const yggr::utf8_string& fname)
{
	if(fname.empty())
	{
		return out;
	}

	yggr::utf8_string file_data;

	if(file_op::read_from_file(file_data.org_str(), fname).empty())
	{
		return out;
	}

	file_data.recount_length();

	return get_vcxproj_map_from_buffer(out, file_data);
}

} // namespace vcsln

#endif // !(BOOST_VERSION < 105600)