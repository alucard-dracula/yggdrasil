// file_operator.cpp

#include "file_operator.hpp"

#if !(BOOST_VERSION < 105600)

namespace file_op
{

yggr::string& format_string_file_path(yggr::string& fpath)
{
	typedef yggr::string string_type;

	for(string_type::iterator i = fpath.begin(), isize = fpath.end(); i != isize; ++i)
	{
		if(*i == '\\')
		{
			*i = '/';
		}
	}

	return fpath;
}

yggr::utf8_string& format_string_file_path(yggr::utf8_string& fpath)
{
	format_string_file_path(fpath.org_str());
	return fpath;
}

yggr::utf8_string make_string_file_path(const yggr::utf8_string& fpath)
{
	yggr::utf8_string tmp = fpath;
	format_string_file_path(tmp.org_str());
	return tmp;
}

yggr::string get_file_dir(const yggr::string& file_path)
{
	std::size_t pos1 = file_path.rfind('/');
	std::size_t pos2 = file_path.rfind('\\');

	std::size_t pos = 
		(std::max)(
			(pos1 == yggr::string::npos? 0 : pos1), 
			(pos2 == yggr::string::npos? 0 : pos2));

	return pos == 0?  yggr::string(".") : file_path.substr(0, pos);
}

yggr::string get_file_name(const yggr::string& file_path)
{
	std::size_t pos1 = file_path.rfind('/');
	std::size_t pos2 = file_path.rfind('\\');

	std::size_t pos = 
		(std::max)(
			(pos1 == yggr::string::npos? 0 : pos1), 
			(pos2 == yggr::string::npos? 0 : pos2));

	if(pos < file_path.size())
	{
		if(file_path[pos] == '/' || file_path[pos] == '\\')
		{
			pos += 1;
		}
	}

	return file_path.substr(pos, file_path.size());
}

yggr::string get_file_name_no_ext(const yggr::string& file_path)
{
	yggr::string str_fname = get_file_name(file_path);

	std::size_t pos = str_fname.rfind('.');
	return str_fname.substr(0, pos);
}

bool is_yggr_proj_check(const yggr::utf8_string& proj_dir)
{	
	try
	{
		if((proj_dir.org_str())[proj_dir.size() - 1] == '\\'
			|| (proj_dir.org_str())[proj_dir.size() - 1] == '/')
		{
			return local_fsys_operators_type::is_exists(local_fsys_operators_type::make_path_object(proj_dir + "../yggr.sln"));
		}
		else
		{
			return local_fsys_operators_type::is_exists(local_fsys_operators_type::make_path_object(proj_dir + "/../yggr.sln"));
		}
	}
	catch(...)
	{
		return false;
	}
}

}// namespace file_op

#endif // BOOST_VERSION < 105600

