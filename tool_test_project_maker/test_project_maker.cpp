// test_project_maker.cpp

#include <iostream>
#include <sstream>

#include <yggr/base/yggrdef.h>

#include <yggr/ids/uuid.hpp>
#include <yggr/ids/id_generator.hpp>

#include <yggr/charset/string.hpp>
#include <yggr/charset/utf8_string.hpp>
#include <yggr/container/set.hpp>
#include <yggr/container/unordered_set.hpp>
#include <yggr/regular_parse/regex_parse.hpp>
#include <yggr/file_system/local_fsys_operators.hpp>

#include <yggr/archive/xml_archive_partner.hpp>
#include <yggr/packet/packet.hpp>

#include "test_project_maker_cfg.hpp"

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(base)

// 1 �ļ�·����ȡ
// 2 �淶���ļ���ȡ
// 3 ·������
// 4 �ų�Ŀ¼����
// 5 �ų��ļ�����

typedef yggr::file_system::local_fsys_operators local_fsys_operators_type;
typedef local_fsys_operators_type::directory_entry_type directory_entry_type;
typedef local_fsys_operators_type::path_type path_type;

typedef yggr::ids::uuid uuid_type;
typedef yggr::ids::id_generator<uuid_type, yggr::ids::parse_uuid_genner::lagged_fibonacci607_type> uuid_gen_type;

typedef yggr::unordered_set<yggr::utf8_string> filter_list_type;
typedef yggr::set<path_type> file_list_type;

typedef test_project_maker_cfg test_project_maker_cfg_type;


bool handler_for_each_search_cpp_files_filter(local_fsys_operators_type::recursive_directory_iterator_type dir_iter,
												const local_fsys_operators_type::directory_entry_type& entry,
												boost::uintmax_t current_count,
												local_fsys_operators_type::error_code_type* current_ec,
												const filter_list_type& dir_filter)
{
	return dir_filter.find(yggr::utf8_string(entry.path().filename().native())) != dir_filter.end();
}


void handler_for_each_search_cpp_files(const local_fsys_operators_type::directory_entry_type& entry,
										boost::uintmax_t current_count,
										local_fsys_operators_type::error_code_type* current_ec,
										const filter_list_type& dir_filter,
										const filter_list_type& file_filter,
										file_list_type& flist)
{
	typedef local_fsys_operators_type::path_type path_type;

	const path_type& fpath = entry.path();

	if(!(fpath.has_extension() && fpath.extension() == ".cpp"))
	{
		return;
	}
	yggr::utf8_string fpath_name(fpath.native());
	yggr::utf8_string fdir_name(fpath.parent_path().native());
	yggr::utf8_string fname(fpath.filename().native());
	yggr::utf8_string fparent_dir_name(fpath.parent_path().filename().native());

	std::cout << "search: " << fpath_name << std::endl;
	//std::cout << "fpath_name: " << fpath_name << std::endl;
	//std::cout << "fdir_name: "<< fdir_name << std::endl;
	//std::cout << "fname: "<< fname << std::endl;
	//std::cout << "fparent_dir_name: "<< fparent_dir_name << std::endl;

	if(dir_filter.find(fparent_dir_name) != dir_filter.end()
		|| file_filter.find(fname) != file_filter.end())
	{
		return;
	}

	flist.insert(fpath);

	return;
}

file_list_type& search_cpp_files(file_list_type& file_list,
									const yggr::utf8_string& root_dir,
									const filter_list_type& dir_filter, 
									const filter_list_type& file_filter)
{

	path_type root_dir_path = local_fsys_operators_type::make_path_object(root_dir);

	try
	{
		local_fsys_operators_type::recursive_for_each(
			root_dir_path, 
			boost::bind(
				&handler_for_each_search_cpp_files, _1, _2, _3,
				boost::cref(dir_filter), boost::cref(file_filter), boost::ref(file_list)),
			boost::bind(
				&handler_for_each_search_cpp_files_filter, _1, _2, _3, _4, boost::cref(dir_filter)),
			local_fsys_operators_type::file_types_options_type::general_file);
	}
	catch(const local_fsys_operators_type::exception_type& e)
	{
		std::cerr << e.what() << std::endl;
		assert(false);
	}

	return file_list;
}

yggr::utf8_string cpp_path_to_project_name(const path_type& fpath)
{
	yggr::utf8_string dname(fpath.parent_path().stem().native());
	yggr::utf8_string fname(fpath.stem().native());

	return dname + "_" + fname;
}

yggr::utf8_string cpp_path_to_file_name(const path_type& fpath, const yggr::utf8_string& str_root_dir)
{
	yggr::utf8_string fname(fpath.native());

	if((2 < fname.length()) && (fname[0] == ".") && (fname[1] == "/" || fname[1] == "\\"))
	{
		fname.erase(fname.begin(), fname.begin() + 2);
	}
	else if(str_root_dir.length() < fname.length()
			&& 3 < str_root_dir.length()
			&& fname.substr(0, 2) == str_root_dir.substr(0, 2)
			&& fname.substr(0, 2) == ".."
			&& fname.substr(3, str_root_dir.size() - 3) == str_root_dir.substr(3, str_root_dir.size() - 3))
	{
		fname.erase(fname.begin(), fname.begin() + str_root_dir.length() + 1);
	}

	return fname;
}

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

yggr::string& read_sample_file(yggr::string& file_content, const yggr::utf8_string& fpath)
{
	return read_from_file(file_content, fpath);
}

yggr::string& make_regex_re(yggr::string& out, const yggr::string& str)
{
	std::stringstream ss;
	ss << "(" << str << ")";

	try
	{
		out = yggr::regular_parse::regex_parse::replace(
					ss.str(), yggr::regular_parse::regex_parse::make_regex("\\\\"), "\\\\\\\\");
	}
	catch(const yggr::regular_parse::regex_parse::error_type& e)
	{
		std::cerr << e.what() << std::endl;
		assert(false);
	}
	catch(...)
	{
		assert(false);
	}
	
	return out;
}

yggr::string& make_regex_fmt(yggr::string& out, const yggr::string& str)
{
	std::stringstream ss;
	ss << "(?1" << str << ")";

	try
	{
		out = yggr::regular_parse::regex_parse::replace(
					ss.str(), yggr::regular_parse::regex_parse::make_regex("\\\\"), "\\\\\\\\");
	}
	catch(const yggr::regular_parse::regex_parse::error_type& e)
	{
		std::cerr << e.what() << std::endl;
		assert(false);
	}
	catch(...)
	{
		assert(false);
	}
	
	return out;
}

yggr::string& sample_file_to_regular_file(yggr::string& regular,
											const yggr::string sample, 
											const yggr::string& str_re, 
											const yggr::string& str_fmt,
											const yggr::string& str_fre,
											const yggr::string& str_ffmt)
{
	yggr::string regular_tmp;
	try
	{
		regular_tmp = 
			yggr::regular_parse::regex_parse::replace(
				sample, yggr::regular_parse::regex_parse::make_regex(str_re), str_fmt);

		regular = 
			yggr::regular_parse::regex_parse::replace(
				regular_tmp, yggr::regular_parse::regex_parse::make_regex(str_fre), str_ffmt);

	}
	catch(const yggr::regular_parse::regex_parse::error_type& e)
	{
		std::cerr << e.what() << std::endl;
		assert(false);
	}
	catch(...)
	{
		assert(false);
	}

	return regular;
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

yggr::string& sample_sln_proj_to_regular_sln_proj(yggr::string& regular,
													const yggr::string sample, 
													const yggr::string& dir_re, 
													const yggr::string& dir_fmt,
													const yggr::string& proj_re,
													const yggr::string& proj_fmt,
													const yggr::string& id_re,
													const yggr::string& id_fmt)
{
	yggr::string regular_tmp;
	try
	{
		regular_tmp = 
			yggr::regular_parse::regex_parse::replace(
				sample, yggr::regular_parse::regex_parse::make_regex(dir_re), dir_fmt);

		regular_tmp = 
			yggr::regular_parse::regex_parse::replace(
				regular_tmp, yggr::regular_parse::regex_parse::make_regex(id_re), id_fmt);

		regular = 
			yggr::regular_parse::regex_parse::replace(
				regular_tmp, yggr::regular_parse::regex_parse::make_regex(proj_re), proj_fmt);

	}
	catch(const yggr::regular_parse::regex_parse::error_type& e)
	{
		std::cerr << e.what() << std::endl;
		assert(false);
	}
	catch(...)
	{
		assert(false);
	}

	return regular;
}

yggr::string hex_str_upper(const yggr::string& str)
{
	typedef boost::basic_regex<char> regex_type;

	static regex_type re = 
		yggr::regular_parse::regex_parse::make_regex(
			"(a)|(b)|(c)|(d)|(e)|(f)");
	static yggr::string str_fmt("(?1A)(?2B)(?3C)(?4D)(?5E)(?6F)");


	try
	{
		return yggr::regular_parse::regex_parse::replace(str, re, str_fmt);
	}
	catch(const yggr::regular_parse::regex_parse::error_type& e)
	{
		std::cerr << e.what() << std::endl;
		assert(false);
	}
	catch(...)
	{
		assert(false);
	}

	return str;
};

yggr::string gen_str_uuid(void)
{
	std::stringstream ss;
	uuid_gen_type gen;
	ss << gen();

	return hex_str_upper(yggr::string(ss.str().c_str(), ss.str().size()));
}

yggr::string path_to_current_dir_name(const path_type& path)
{
	return yggr::utf8_string(path.stem().native()).org_str();
}

static const char* test_project_maker_cfg_mark = "test_project_maker_cfg";

bool write_cfg_file(const yggr::utf8_string& fname, const test_project_maker_cfg_type& cfg)
{
	typedef yggr::packet::packet<yggr::archive::archive_partner::xml_oarchive_partner> opak_type;

	opak_type opak;
	opak.save(YGGR_SERIALIZATION_NAME_NVP(test_project_maker_cfg_mark, cfg));

	return write_to_file(fname, opak.org_buf());
}

bool read_cfg_file(const yggr::utf8_string& fname, test_project_maker_cfg_type& cfg)
{
	typedef yggr::packet::packet<yggr::archive::archive_partner::xml_iarchive_partner> ipak_type;

	ipak_type ipak;
	return 
		read_from_file(ipak.org_buf(), fname).size()
		&& ipak.load(YGGR_SERIALIZATION_NAME_NVP(test_project_maker_cfg_mark, cfg));
}

static yggr::utf8_string cfg_fname("test_project_maker_cfg.xml");

#define ONLY_MAKE_RUN_TEST_BAT() 1

int main_detail(int argc, char* argv[])
{
	if(argc > 1)
	{
		cfg_fname = yggr::utf8_string(argv[1], YGGR_STR_UTF8_STRING_CHARSET_NAME());
	}

	if(!local_fsys_operators_type::is_exists(local_fsys_operators_type::make_path_object(cfg_fname)))
	{
		test_project_maker_cfg_type cfg_sample = test_project_maker_cfg_sample();
		write_cfg_file(cfg_fname, cfg_sample);
		std::cout << "gen config file succeed, please modify it" << std::endl;
		return -1;
	}

	test_project_maker_cfg_type cfg;

	if(!read_cfg_file(cfg_fname, cfg))
	{
		std::cout << "read config file failed" << std::endl;
		return -1;
	}

	file_list_type file_list;

	search_cpp_files(file_list, cfg.root_dir_, cfg.dir_filter_, cfg.file_filter_);

	
	yggr::string sample_vcxproj;
	yggr::string sample_filters;

	yggr::string regular_vcxproj;
	yggr::string regular_filters;

	read_sample_file(sample_vcxproj, cfg.sample_vcxproj_);
	read_sample_file(sample_filters, cfg.sample_vcxporj_filter_);

	yggr::string str_re;
	yggr::string str_fmt;

	yggr::string str_fre;
	yggr::string str_ffmt;

	yggr::utf8_string proj_name;
	yggr::utf8_string fname;

	make_regex_re(str_re, cfg.sample_vcxproj_re_);
	make_regex_re(str_fre, cfg.sample_vcxproj_fre_);

	yggr::string sample_sln_begin;
	yggr::string sample_sln_end;

	yggr::string sample_project_begin;
	yggr::string sample_project_end;

	read_sample_file(sample_sln_begin, cfg.sample_sln_begin_);
	read_sample_file(sample_sln_end, cfg.sample_sln_end_);

	read_sample_file(sample_project_begin, cfg.sample_sln_proj_begin_);
	read_sample_file(sample_project_end, cfg.sample_sln_proj_end_);

	yggr::string dir_fmt = 
		path_to_current_dir_name(
			local_fsys_operators_type::system_complete(
				local_fsys_operators_type::make_path_object(cfg.root_dir_)));

	yggr::string regular_proj_beg;

	std::stringstream ss_sln;
	ss_sln << sample_sln_begin << "\n";

	std::stringstream ss_bat_vc100_x64d;
	std::stringstream ss_bat_vc100_x64;
	std::stringstream ss_bat_vc100_x32d;
	std::stringstream ss_bat_vc100_x32;

	std::stringstream ss_bat_vc143_x64d;
	std::stringstream ss_bat_vc143_x64;
	std::stringstream ss_bat_vc143_x32d;
	std::stringstream ss_bat_vc143_x32;

	std::stringstream ss_bat_mgw14_x64d;
	std::stringstream ss_bat_mgw14_x64;
	std::stringstream ss_bat_mgw14_x32d;
	std::stringstream ss_bat_mgw14_x32;

	std::stringstream ss_sh_gcc14_x64d;
	std::stringstream ss_sh_gcc14_x64;
	std::stringstream ss_sh_gcc14_x32d;
	std::stringstream ss_sh_gcc14_x32;

	ss_bat_vc100_x64d << "cd /d %~dp0/test\n" << "rm -fr ../test_out\n" << "cp -fr ../Debug-x64/vc100/yggr_lua_base_type.dll ./\n";
	ss_bat_vc100_x64 << "cd /d %~dp0/test\n" << "rm -fr ../test_out\n" << "cp -fr ../Release-x64/vc100/yggr_lua_base_type.dll ./\n";
	ss_bat_vc100_x32d << "cd /d %~dp0/test\n" << "rm -fr ../test_out\n" << "cp -fr ../Debug-Win32/vc100/yggr_lua_base_type.dll ./\n";
	ss_bat_vc100_x32 << "cd /d %~dp0/test\n" << "rm -fr ../test_out\n" << "cp -fr ../Release-Win32/vc100/yggr_lua_base_type.dll ./\n";

	ss_bat_vc143_x64d << "cd /d %~dp0/test\n" << "rm -fr ../test_out\n" << "cp -fr ../Debug-x64/vc143/yggr_lua_base_type.dll ./\n";
	ss_bat_vc143_x64 << "cd /d %~dp0/test\n" << "rm -fr ../test_out\n" << "cp -fr ../Release-x64/vc143/yggr_lua_base_type.dll ./\n";
	ss_bat_vc143_x32d << "cd /d %~dp0/test\n" << "rm -fr ../test_out\n" << "cp -fr ../Debug-Win32/vc143/yggr_lua_base_type.dll ./\n";
	ss_bat_vc143_x32 << "cd /d %~dp0/test\n" << "rm -fr ../test_out\n" << "cp -fr ../Release-Win32/vc143/yggr_lua_base_type.dll ./\n";

	ss_bat_mgw14_x64d << "cd /d %~dp0/test\n" << "rm -fr ../test_out\n" << "cp -fr ../Debug-x64/mgw14/yggr_lua_base_type.dll ./\n";
	ss_bat_mgw14_x64 << "cd /d %~dp0/test\n" << "rm -fr ../test_out\n" << "cp -fr ../Release-x64/mgw14/yggr_lua_base_type.dll ./\n";
	ss_bat_mgw14_x32d << "cd /d %~dp0/test\n" << "rm -fr ../test_out\n" << "cp -fr ../Debug-Win32/mgw14/yggr_lua_base_type.dll ./\n";
	ss_bat_mgw14_x32 << "cd /d %~dp0/test\n" << "rm -fr ../test_out\n" << "cp -fr ../Release-Win32/mgw14/yggr_lua_base_type.dll ./\n";

	ss_sh_gcc14_x64d
		<< "#!/bin/sh\n" 
		<< "source ./begin_test_env_x64d.sh\n"
		<< "rm -f Debug-x64/gcc14/yggr_lua_base_type.so\n"
		<< "rm -f Debug-x64/gcc14/mongo_crypt_v1.so\n"
		<< "ln -s libyggr_lua_base_type.so Debug-x64/gcc14/yggr_lua_base_type.so\n" 
		<< "ln -s /usr/local/lib64/mongo_crypt/mongo_crypt_v1.so  Debug-x64/gcc14/mongo_crypt_v1.so\n" 
		<< "cd ./test\n"
		<< "rm -fr ../test_out\n"
		<< "rm -f yggr_lua_base_type.so\n"
		<< "rm -f mongo_crypt_v1.so\n"
		<< "ln -s ../Debug-x64/gcc14/libyggr_lua_base_type.so yggr_lua_base_type.so\n"
		<< "ln -s /usr/local/lib64/mongo_crypt/mongo_crypt_v1.so mongo_crypt_v1.so\n";

	ss_sh_gcc14_x64
		<< "#!/bin/sh\n" 
		<< "source ./begin_test_env_x64.sh\n"
		<< "rm -f Release-x64/gcc14/yggr_lua_base_type.so\n"
		<< "rm -f Release-x64/gcc14/mongo_crypt_v1.so\n"
		<< "ln -s libyggr_lua_base_type.so Release-x64/gcc14/yggr_lua_base_type.so\n" 
		<< "ln -s /usr/local/lib64/mongo_crypt/mongo_crypt_v1.so  Release-x64/gcc14/mongo_crypt_v1.so\n" 
		<< "cd ./test\n"
		<< "rm -fr ../test_out\n"
		<< "rm -f yggr_lua_base_type.so\n"
		<< "rm -f mongo_crypt_v1.so\n"
		<< "ln -s ../Release-x64/gcc14/libyggr_lua_base_type.so yggr_lua_base_type.so\n"
		<< "ln -s /usr/local/lib64/mongo_crypt/mongo_crypt_v1.so mongo_crypt_v1.so\n";

	ss_sh_gcc14_x32d
		<< "#!/bin/sh\n" 
		<< "source ./begin_test_env_x32d.sh\n"
		<< "rm -f Debug-x32/gcc14/yggr_lua_base_type.so\n"
		<< "ln -s libyggr_lua_base_type.so Debug-x32/gcc14/yggr_lua_base_type.so\n"  
		<< "cd ./test\n"
		<< "rm -fr ../test_out\n"
		<< "rm -f yggr_lua_base_type.so\n"
		<< "rm -f mongo_crypt_v1.so\n"
		<< "ln -s ../Debug-x32/gcc14/libyggr_lua_base_type.so yggr_lua_base_type.so\n";

	ss_sh_gcc14_x32
		<< "#!/bin/sh\n" 
		<< "source ./begin_test_env_x32.sh\n"
		<< "rm -f Release-x32/gcc14/yggr_lua_base_type.so\n"
		<< "ln -s libyggr_lua_base_type.so Release-x32/gcc14/yggr_lua_base_type.so\n"  
		<< "cd ./test\n"
		<< "rm -fr ../test_out\n"
		<< "rm -f yggr_lua_base_type.so\n"
		<< "rm -f mongo_crypt_v1.so\n"
		<< "ln -s ../Release-x32/gcc14/libyggr_lua_base_type.so yggr_lua_base_type.so\n";

	for(file_list_type::const_iterator i = file_list.begin(), isize = file_list.end(); i != isize; ++i)
	{

		std::cout << "fix: " << *i << std::endl;
		proj_name = cpp_path_to_project_name(*i);

#if !ONLY_MAKE_RUN_TEST_BAT()
		make_regex_re(str_fmt, proj_name.org_str());
		make_regex_re(str_ffmt, cpp_path_to_file_name(*i, cfg.root_dir_));

		sample_file_to_regular_file(regular_vcxproj, sample_vcxproj, str_re, str_fmt, str_fre, str_ffmt);
		sample_file_to_regular_file(regular_filters, sample_filters, str_re, str_fmt, str_fre, str_ffmt);

		write_to_file(cfg.root_dir_ + "/" + proj_name + ".vcxproj", regular_vcxproj);
		write_to_file(cfg.root_dir_ + "/" + proj_name + ".vcxproj.filters", regular_filters);

		ss_sln << sample_sln_proj_to_regular_sln_proj(
					regular_proj_beg, 
					sample_project_begin, 
					cfg.sample_sln_smp_dir_, dir_fmt,
					cfg.sample_sln_smp_proj_, proj_name.org_str(),
					cfg.sample_sln_smp_id_, gen_str_uuid() ) << "\n";

		ss_sln << sample_project_end << "\n";

#endif // ONLY_MAKE_RUN_TEST_BAT

		ss_bat_vc100_x64d << "\"../Debug-x64/vc100/" << proj_name << ".exe\" --batch-test\n";
		ss_bat_vc100_x64 << "\"../Release-x64/vc100/" << proj_name << ".exe\" --batch-test\n";
		ss_bat_vc100_x32d << "\"../Debug-Win32/vc100/" << proj_name << ".exe\" --batch-test\n";
		ss_bat_vc100_x32 << "\"../Release-Win32/vc100/" << proj_name << ".exe\" --batch-test\n";

		ss_bat_vc143_x64d << "\"../Debug-x64/vc143/" << proj_name << ".exe\" --batch-test\n";
		ss_bat_vc143_x64 << "\"../Release-x64/vc143/" << proj_name << ".exe\" --batch-test\n";
		ss_bat_vc143_x32d << "\"../Debug-Win32/vc143/" << proj_name << ".exe\" --batch-test\n";
		ss_bat_vc143_x32 << "\"../Release-Win32/vc143/" << proj_name << ".exe\" --batch-test\n";

		ss_bat_mgw14_x64d << "\"../Debug-x64/mgw14/" << proj_name << ".exe\" --batch-test\n";
		ss_bat_mgw14_x64 << "\"../Release-x64/mgw14/" << proj_name << ".exe\" --batch-test\n";
		ss_bat_mgw14_x32d << "\"../Debug-Win32/mgw14/" << proj_name << ".exe\" --batch-test\n";
		ss_bat_mgw14_x32 << "\"../Release-Win32/mgw14/" << proj_name << ".exe\" --batch-test\n";

		ss_sh_gcc14_x64d << "../Debug-x64/gcc14/" << proj_name << " --batch-test\n";
		ss_sh_gcc14_x64 << "../Release-x64/gcc14/" << proj_name << " --batch-test\n";
		ss_sh_gcc14_x32d << "../Debug-x32/gcc14/" << proj_name << " --batch-test\n";
		ss_sh_gcc14_x32 << "../Release-x32/gcc14/" << proj_name << " --batch-test\n";
	}

#if !ONLY_MAKE_RUN_TEST_BAT()
	ss_sln << sample_sln_end;
	write_to_file(cfg.regular_sln_fname_, yggr::string(ss_sln.str().c_str(), ss_sln.str().size()));
#endif // ONLY_MAKE_RUN_TEST_BAT

	ss_bat_vc100_x64d << "echo \'---------all test end-----------------\'\ncd ..\n";
	ss_bat_vc100_x64 << "echo \'---------all test end-----------------\'\ncd ..\n";
	ss_bat_vc100_x32d << "echo \'---------all test end-----------------\'\ncd ..\n";
	ss_bat_vc100_x32 << "echo \'---------all test end-----------------\'\ncd ..\n";

	ss_bat_vc143_x64d << "echo \'---------all test end-----------------\'\ncd ..\n";
	ss_bat_vc143_x64 << "echo \'---------all test end-----------------\'\ncd ..\n";
	ss_bat_vc143_x32d << "echo \'---------all test end-----------------\'\ncd ..\n";
	ss_bat_vc143_x32 << "echo \'---------all test end-----------------\'\ncd ..\n";

	ss_bat_mgw14_x64d << "echo \'---------all test end-----------------\'\ncd ..\n";
	ss_bat_mgw14_x64 << "echo \'---------all test end-----------------\'\ncd ..\n";
	ss_bat_mgw14_x32d << "echo \'---------all test end-----------------\'\ncd ..\n";
	ss_bat_mgw14_x32 << "echo \'---------all test end-----------------\'\ncd ..\n";

	ss_sh_gcc14_x64d << "echo \"---------all test end-----------------\"\ncd ..\nsource end_test_env.sh\n";
	ss_sh_gcc14_x64 << "echo \"---------all test end-----------------\"\ncd ..\nsource end_test_env.sh\n";
	ss_sh_gcc14_x32d << "echo \"---------all test end-----------------\"\ncd ..\nsource end_test_env.sh\n";
	ss_sh_gcc14_x32 << "echo \"---------all test end-----------------\"\ncd ..\nsource end_test_env.sh\n";

#if !ONLY_MAKE_RUN_TEST_BAT()
//#if 1
	write_to_file(cfg.test_bat_fname_ + "_vc100_x64d.bat", yggr::string(ss_bat_vc100_x64d.str().c_str(), ss_bat_vc100_x64d.str().size()));
	write_to_file(cfg.test_bat_fname_ + "_vc100_x64.bat", yggr::string(ss_bat_vc100_x64.str().c_str(), ss_bat_vc100_x64.str().size()));
	write_to_file(cfg.test_bat_fname_ + "_vc100_x32d.bat", yggr::string(ss_bat_vc100_x32d.str().c_str(), ss_bat_vc100_x32d.str().size()));
	write_to_file(cfg.test_bat_fname_ + "_vc100_x32.bat", yggr::string(ss_bat_vc100_x32.str().c_str(), ss_bat_vc100_x32.str().size()));

	write_to_file(cfg.test_bat_fname_ + "_vc143_x64d.bat", yggr::string(ss_bat_vc143_x64d.str().c_str(), ss_bat_vc143_x64d.str().size()));
	write_to_file(cfg.test_bat_fname_ + "_vc143_x64.bat", yggr::string(ss_bat_vc143_x64.str().c_str(), ss_bat_vc143_x64.str().size()));
	write_to_file(cfg.test_bat_fname_ + "_vc143_x32d.bat", yggr::string(ss_bat_vc143_x32d.str().c_str(), ss_bat_vc143_x32d.str().size()));
	write_to_file(cfg.test_bat_fname_ + "_vc143_x32.bat", yggr::string(ss_bat_vc143_x32.str().c_str(), ss_bat_vc143_x32.str().size()));

	write_to_file(cfg.test_bat_fname_ + "_mgw14_x64d.bat", yggr::string(ss_bat_mgw14_x64d.str().c_str(), ss_bat_mgw14_x64d.str().size()));
	write_to_file(cfg.test_bat_fname_ + "_mgw14_x64.bat", yggr::string(ss_bat_mgw14_x64.str().c_str(), ss_bat_mgw14_x64.str().size()));
	write_to_file(cfg.test_bat_fname_ + "_mgw14_x32d.bat", yggr::string(ss_bat_mgw14_x32d.str().c_str(), ss_bat_mgw14_x32d.str().size()));
	write_to_file(cfg.test_bat_fname_ + "_mgw14_x32.bat", yggr::string(ss_bat_mgw14_x32.str().c_str(), ss_bat_mgw14_x32.str().size()));

	write_to_file(cfg.test_bat_fname_ + "_gcc14_x64d.sh", yggr::string(ss_sh_gcc14_x64d.str().c_str(), ss_sh_gcc14_x64d.str().size()));
	write_to_file(cfg.test_bat_fname_ + "_gcc14_x64.sh", yggr::string(ss_sh_gcc14_x64.str().c_str(), ss_sh_gcc14_x64.str().size()));
	write_to_file(cfg.test_bat_fname_ + "_gcc14_x32d.sh", yggr::string(ss_sh_gcc14_x32d.str().c_str(), ss_sh_gcc14_x32d.str().size()));
	write_to_file(cfg.test_bat_fname_ + "_gcc14_x32.sh", yggr::string(ss_sh_gcc14_x32.str().c_str(), ss_sh_gcc14_x32.str().size()));
#endif // ONLY_MAKE_RUN_TEST_BAT

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