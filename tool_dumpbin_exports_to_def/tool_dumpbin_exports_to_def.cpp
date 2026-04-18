// tool_upgrade_vcxproj.cpp

#include <yggr/base/yggrdef.h>

#include <yggr/charset/utf8_string.hpp>
#include <yggr/container/map.hpp>

#include <yggr/file_system/local_fsys_operators.hpp>
#include <yggr/file_system/boost_filesystem_comp.hpp>

#include <iostream>
#include <sstream>
#include <cassert>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

//#if BOOST_VERSION < 105600
//#	error this tool not support boost 1.55 and older
//#endif //BOOST_VERSION < 105600

#define ONLY_MAKE_RUN_TEST() 0

typedef yggr::file_system::local_fsys_operators local_fsys_operators_type;
typedef local_fsys_operators_type::directory_entry_type directory_entry_type;
typedef local_fsys_operators_type::path_type path_type;

typedef yggr::map<yggr::u32, yggr::string> foo_name_map_type;


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

	return file_path.substr(pos, file_path.size() - pos);
}

// update

template<typename Buffer>
Buffer& read_from_file(Buffer& file_content, const yggr::utf8_string& fpath)
{
	try
	{
		local_fsys_operators_type::read_file_of_binary(local_fsys_operators_type::make_path_object(fpath), file_content);
	}
	catch(const local_fsys_operators_type::exception_type& e)
	{
		std::cerr << e.what() << std::endl;
		assert(false);
	}

	return file_content;
}

template<typename Buffer>
bool write_to_file(const yggr::utf8_string& fname, const Buffer& fbuf)
{
	try
	{
		local_fsys_operators_type::write_file_of_binary(
			local_fsys_operators_type::make_path_object(fname), fbuf,
			local_fsys_operators_type::options_type::dir_create_if_not_exists);
	}
	catch(const local_fsys_operators_type::exception_type& e)
	{
		std::cerr << e.what() << std::endl;
		assert(false);
		return false;
	}

	return true;
}


// args
// 0. exe path
// 1. dumpbin_exprots_file_path
// 2. def_file_path
// 3. new_dll_name ( optional )

int format_input_args(int argc, char* argv[], 
						yggr::utf8_string& dbe_file_path, 
						yggr::utf8_string& def_file_path,
						yggr::string& new_dll_name)
{
	if(argc < 3)
	{
		std::cout << "args failed" << std::endl;

		std::cout << "args1:" << "dumpbin exprots file path (e.g. dll_name.dll.def.dbe)" << std::endl;
		std::cout << "args2:" << "dumpbin exprots file path (e.g. dll_name.dll.def)" << std::endl;
		std::cout << "args3:" << "new_dll_name (optional e.g. mapped_dll_name)" << std::endl;

		return -1;
	}

	if(argv[1])
	{
		dbe_file_path = argv[1];
	}

	if(argv[2])
	{
		def_file_path = argv[2];
	}

	if(3 < argc && argv[3])
	{
		new_dll_name = argv[3];
	}
	else
	{
		new_dll_name = get_file_name(def_file_path.org_str());
	}

	return 0;
}

//foo_name_map_type

yggr::string& get_def_file_data(yggr::string& dbe_file_buf)
{
	typedef yggr::string::iterator iter_type;

	std::size_t pos_beg = dbe_file_buf.find("ordinal hint RVA      name");

	if(pos_beg == yggr::string::npos)
	{
		dbe_file_buf.clear();
		return dbe_file_buf;
	}

	pos_beg = dbe_file_buf.find("\n", pos_beg);

	if(pos_beg == yggr::string::npos)
	{
		dbe_file_buf.clear();
		return dbe_file_buf;
	}

	++pos_beg;

	std::size_t pos_end = dbe_file_buf.find("Summary", pos_beg);

	if(pos_end == yggr::string::npos)
	{
		dbe_file_buf.clear();
		return dbe_file_buf;
	}

	dbe_file_buf.erase(dbe_file_buf.begin() + pos_end, dbe_file_buf.end());

	dbe_file_buf.erase(dbe_file_buf.begin(), dbe_file_buf.begin() + pos_beg);


	//std::cout << dbe_file_buf << std::endl;

	return dbe_file_buf;

}

//yggr::string unwrap_foo_name(const yggr::string& foo_name)
//{
//
//	if(foo_name.size()
//		&& 4 < foo_name.size() 
//		&& foo_name.substr(0, 4) == "@ILT"
//		&& foo_name.back() == ')')
//	{
//		std::size_t left_bracket_pos = foo_name.find_last_of('(', foo_name.size() - 1);
//		if(left_bracket_pos != yggr::string::npos && left_bracket_pos < foo_name.size() - 1)
//		{
//			//std::cout << foo_name.substr(left_bracket_pos + 1, foo_name.size() - left_bracket_pos - 2) << std::endl;
//			return foo_name.substr(left_bracket_pos + 1, foo_name.size() - left_bracket_pos - 2);
//		}
//	}
//	
//	return foo_name;
//}

foo_name_map_type& parse_def_file_data(foo_name_map_type& foo_name_map, const yggr::string& dbe_file_buf)
{
	std::size_t pos_beg = yggr::string::npos, pos_mid = 0, pos_end = 0;

	for(std::size_t i = 1;; ++i)
	{
		std::stringstream ss;
		ss << i;

		pos_beg = dbe_file_buf.find(ss.str().c_str(), pos_end);

		if(pos_beg == yggr::string::npos)
		{
			break;
		}

		pos_end = dbe_file_buf.find("\n", pos_beg);

		if(pos_end == yggr::string::npos || pos_end <= pos_beg)
		{
			foo_name_map.clear();
			return foo_name_map;
		}

		pos_mid = pos_end;
		++pos_end;

		if(dbe_file_buf[pos_mid - 1] == '\r')
		{
			--pos_mid;
		}

		std::size_t pos_chk = dbe_file_buf.rfind(" = ", pos_mid);

		if(pos_chk != yggr::string::npos)
		{
			pos_mid = pos_chk;
			for(;pos_mid - 1 && dbe_file_buf[pos_mid - 1] == ' '; --pos_mid);
		}
		
		pos_beg = dbe_file_buf.rfind(' ', pos_mid - 1);
		if(pos_beg == yggr::string::npos 
			|| !(dbe_file_buf[pos_beg + 1])
			|| !(pos_beg + 1 < pos_mid))
		{
			foo_name_map.clear();
			return foo_name_map;
		}
		
		++pos_beg;
		assert(dbe_file_buf[pos_beg]);


		//foo_name_map.insert(std::make_pair(i, unwrap_foo_name(dbe_file_buf.substr(pos_beg, pos_mid - pos_beg))));

		//std::cout << dbe_file_buf.substr(pos_beg, pos_mid - pos_beg) << std::endl;
		foo_name_map.insert(std::make_pair(i, dbe_file_buf.substr(pos_beg, pos_mid - pos_beg)));
	}

	return foo_name_map;
}

yggr::string& make_def_file_data(yggr::string& def_file_data, 
									const foo_name_map_type& foo_name_map, 
									const yggr::string& new_dll_name)
{
	typedef foo_name_map_type::const_iterator foo_name_map_citer_type;

	std::stringstream ss;
	ss << "LIBRARY " << new_dll_name << "\n"
		<< "EXPORTS\n";

	for(foo_name_map_citer_type i = foo_name_map.begin(), isize = foo_name_map.end(); i != isize; ++i)
	{
		assert(i->second.size());
		ss << "\t" << i->second << " @" << i->first << "\n";
	}

	std::string tmp = ss.str();

	def_file_data.assign(tmp.begin(), tmp.end());

	return def_file_data;
}

int make_def_file(const yggr::utf8_string& dbe_file_path, 
					const yggr::utf8_string& def_file_path, 
					const yggr::string& new_dll_name)
{
	yggr::string dbe_file_buf;
	read_from_file(dbe_file_buf, dbe_file_path);

	if(get_def_file_data(dbe_file_buf).empty())
	{
		std::cout << "get_def_file_data failed" << std::endl;
		return -1;
	}

	foo_name_map_type foo_name_map;

	if(parse_def_file_data(foo_name_map, dbe_file_buf).empty())
	{
		std::cout << "parse_def_file_data failed" << std::endl;
		return -1;
	}

	yggr::string def_file_buf;

	if(make_def_file_data(def_file_buf, foo_name_map, new_dll_name).empty())
	{
		std::cout << "make_def_file_data failed" << std::endl;
		return -1;
	}

#if ONLY_MAKE_RUN_TEST()
	std::cout << def_file_buf << std::endl;
#else
	write_to_file(def_file_path, def_file_buf);
#endif // ONLY_MAKE_RUN_TEST

	return 0;
}

int main_detail(int argc, char* argv[])
{
	yggr::utf8_string dbe_file_path;
	yggr::utf8_string def_file_path;
	yggr::string new_dll_name;

	int ret = format_input_args(argc, argv, dbe_file_path, def_file_path, new_dll_name);

	if(ret)
	{
		return ret;
	}

	std::cout << dbe_file_path << std::endl;
	std::cout << def_file_path << std::endl;
	std::cout << new_dll_name << std::endl;

	ret = make_def_file(dbe_file_path, def_file_path, new_dll_name);

	return 0;
}

int main(int argc, char* argv[])
{
	int ret = main_detail(argc, argv);

	std::cout << "end" << std::endl;

	if(!(argc > 1))
	{
		char cc = 0;
		std::cin >> cc;
	}

	return ret;
}

