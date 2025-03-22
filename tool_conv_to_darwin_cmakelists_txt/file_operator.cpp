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

	return file_path.substr(pos);
}

yggr::string get_file_name_no_ext(const yggr::string& file_path)
{
	yggr::string str_fname = get_file_name(file_path);

	std::size_t pos = str_fname.rfind('.');
	return str_fname.substr(0, pos);
}

yggr::utf8_string get_file_dir(const yggr::utf8_string& file_path)
{
	return yggr::utf8_string(get_file_dir(file_path.org_str()), YGGR_STR_UTF8_STRING_CHARSET_NAME());
}

yggr::utf8_string get_file_name(const yggr::utf8_string& file_path)
{
	return yggr::utf8_string(get_file_name(file_path.org_str()), YGGR_STR_UTF8_STRING_CHARSET_NAME());
}

yggr::utf8_string get_file_name_no_ext(const yggr::utf8_string& file_path)
{
	return yggr::utf8_string(get_file_name_no_ext(file_path.org_str()), YGGR_STR_UTF8_STRING_CHARSET_NAME());
}

bool is_yggr_proj_check(const yggr::string& proj_dir)
{	
	try
	{
		if(proj_dir[proj_dir.size() - 1] == '\\'
			|| proj_dir[proj_dir.size() - 1] == '/')
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

bool is_yggr_sln_check(const yggr::string& sln_dir)
{	
	try
	{
		if(sln_dir[sln_dir.size() - 1] == '\\'
			|| sln_dir[sln_dir.size() - 1] == '/')
		{
			return local_fsys_operators_type::is_exists(local_fsys_operators_type::make_path_object(sln_dir + "yggr.sln"));
		}
		else
		{
			return local_fsys_operators_type::is_exists(local_fsys_operators_type::make_path_object(sln_dir + "/yggr.sln"));
		}
	}
	catch(...)
	{
		return false;
	}
}

bool is_parent_path(const fsys_path_type& lpath, const fsys_path_type& rpath)
{
	return 
		lpath.has_filename() 
		&& rpath.has_filename()
		&& rpath.has_parent_path()
		&& lpath == rpath.parent_path();

}

bool check_file_ext(const yggr::string& str_fpath, const yggr::string& str_ext)
{
	return
		str_ext.size() < str_fpath.size()
		&& (0 == str_fpath.compare(str_fpath.size() - str_ext.size(), str_ext.size(), str_ext));
}

bool check_file_ext(const fsys_path_type& fpath, const yggr::string& str_ext)
{
	yggr::utf8_string str_fpath(fpath.native());

	return check_file_ext(str_fpath.org_str(), str_ext);
}

bool copy_file(const yggr::utf8_string& src, const yggr::utf8_string& dst)
{
	std::size_t write_byte_size = 0;
	try
	{
		return
			local_fsys_operators_type::copy(
				local_fsys_operators_type::make_path_object(src),
				local_fsys_operators_type::make_path_object(dst),
				(local_fsys_operators_type::options_type::file_replace_if_exists
					| local_fsys_operators_type::options_type::dir_create_if_not_exists ) );
	}
	catch(const local_fsys_operators_type::exception_type& e)
	{
		std::cerr << e.what() << std::endl;
		assert(false);
		return false;
	}
}

}// namespace file_op

#endif // BOOST_VERSION < 105600

