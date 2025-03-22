// tool_update_vcxproj.cpp

#include <yggr/base/yggrdef.h>

#if defined(YGGR_AT_WINDOWS)

#include "tool_suo_batch_build_selector_cfg.hpp"

#include <yggr/charset/utf8_string.hpp>
#include <yggr/file_system/local_fsys_operators.hpp>

#include <yggr/archive/xml_archive_partner.hpp>
#include <yggr/packet/packet.hpp>

#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>

#include <cassert>
#include <algorithm>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

BOOST_MPL_ASSERT((boost::mpl::bool_<(sizeof(wchar_t) == 2)>));

#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(base)


typedef yggr::file_system::local_fsys_operators local_fsys_operators_type;
typedef local_fsys_operators_type::directory_entry_type directory_entry_type;
typedef local_fsys_operators_type::path_type path_type;

typedef tool_suo_batch_build_selector_cfg tool_suo_batch_build_selector_cfg_type;
typedef tool_suo_batch_build_selector_cfg_type::mark_type mark_type;

static const char* tool_suo_batch_build_selector_cfg_mark = "suo_batch_build_selector_cfg";

static boost::array<yggr::wstring, 4> g_mark_name_str;

void init_mark_name_str(void)
{
	g_mark_name_str[0] = yggr::wstring(L"Debug|x64.fBatchBld");
	g_mark_name_str[0].push_back(0);
	g_mark_name_str[0]+=(L"=\3");

	g_mark_name_str[1] = yggr::wstring(L"Release|x64.fBatchBld");
	g_mark_name_str[1].push_back(0);
	g_mark_name_str[1]+=(L"=\3");

	g_mark_name_str[2] = yggr::wstring(L"Debug|Win32.fBatchBld");
	g_mark_name_str[2].push_back(0);
	g_mark_name_str[2]+=(L"=\3");

	g_mark_name_str[3] = yggr::wstring(L"Release|Win32.fBatchBld");
	g_mark_name_str[3].push_back(0);
	g_mark_name_str[3]+=(L"=\3");


	//yggr::wstring(L"Debug|x64.fBatchBld\0=\3"),
	//yggr::wstring(L"Release|x64.fBatchBld\0=\3"),
	//yggr::wstring(L"Debug|Win32.fBatchBld\0=\3"),
	//yggr::wstring(L"Release|Win32.fBatchBld\0=\3") };
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

bool move_file(const yggr::utf8_string& src, const yggr::utf8_string& dst)
{
	bool bchk = false;
	try
	{
		bchk =
			local_fsys_operators_type::move_file(
				local_fsys_operators_type::make_path_object(src),
				local_fsys_operators_type::make_path_object(dst),
				local_fsys_operators_type::options_type::file_replace_if_exists_not_dir);
	}
	catch(const local_fsys_operators_type::exception_type& e)
	{
		std::cerr << e.what() << std::endl;
		assert(false);
		return false;
	}

	return bchk;
}

std::size_t fix_batch_build_mark(yggr::string& suo_buf, const yggr::wstring& mark_name, yggr::u16 mark)
{
	assert(mark_name.size());
	if(suo_buf.empty() || mark_name.empty())
	{
		return 0;
	}

	char* suo_beg = boost::addressof(suo_buf[0]);
	char* suo_end = suo_beg + suo_buf.size();

	std::size_t mark_name_size = mark_name.size() * sizeof(yggr::u16);
	const char* mark_name_beg = reinterpret_cast<const char*>(boost::addressof(mark_name[0]));
	const char* mark_name_end = mark_name_beg + mark_name_size;

	std::size_t fix_count = 0;
	do
	{
		suo_beg = std::search(suo_beg, suo_end, mark_name_beg, mark_name_end);

		if(suo_beg != suo_end)
		{
			suo_beg += mark_name_size;
			*(reinterpret_cast<yggr::u16*>(suo_beg)) = mark;
			++fix_count;
			suo_beg += sizeof(yggr::u16);
		}
	} while(suo_beg != suo_end);

	return fix_count;

}

std::size_t fix_batch_build(yggr::string& suo_buf, const mark_type& mark)
{
	assert(mark.size() == g_mark_name_str.size());
	if(mark.size() != g_mark_name_str.size())
	{
		return 0;
	}

	std::size_t fix_count = 0;
	for(std::size_t i = 0, isize = mark.size(); i != isize; ++i)
	{
		fix_count += fix_batch_build_mark(suo_buf, g_mark_name_str[i], static_cast<yggr::u16>(mark[i]));
	}

	return fix_count;
}

bool write_cfg_file(const yggr::utf8_string& fname, const tool_suo_batch_build_selector_cfg_type& cfg)
{
	typedef yggr::packet::packet<yggr::archive::archive_partner::xml_oarchive_partner> opak_type;

	opak_type opak;
	opak.save(YGGR_SERIALIZATION_NAME_NVP(tool_suo_batch_build_selector_cfg_mark, cfg));

	return write_to_file(fname, opak.org_buf());
}

bool read_cfg_file(const yggr::utf8_string& fname, tool_suo_batch_build_selector_cfg_type& cfg)
{
	typedef yggr::packet::packet<yggr::archive::archive_partner::xml_iarchive_partner> ipak_type;

	ipak_type ipak;
	return
		read_from_file(ipak.org_buf(), fname).size()
		&& ipak.load(YGGR_SERIALIZATION_NAME_NVP(tool_suo_batch_build_selector_cfg_mark, cfg));
}

static yggr::utf8_string cfg_fname("tool_suo_batch_build_selector_cfg.xml");

int main_detail(int argc, char* argv[])
{
	if(argc > 1)
	{
		cfg_fname = yggr::utf8_string(argv[1], YGGR_STR_UTF8_STRING_CHARSET_NAME());
	}

	if(!local_fsys_operators_type::is_exists(local_fsys_operators_type::make_path_object(cfg_fname)))
	{
		tool_suo_batch_build_selector_cfg_type cfg_sample = tool_suo_batch_build_selector_cfg_sample();
		write_cfg_file(cfg_fname, cfg_sample);
		std::cout << "gen config file succeed, please modify it" << std::endl;
		return -1;
	}

	tool_suo_batch_build_selector_cfg_type cfg;

	if(!read_cfg_file(cfg_fname, cfg))
	{
		std::cout << "read config file failed" << std::endl;
		return -1;
	}

	yggr::string suo_buf;
	if(!(read_from_file(suo_buf, cfg.file_suo_).size()))
	{
		std::cout << "read " << cfg.file_suo_ << " failed" << std::endl;
	}

	std::size_t fix_count = fix_batch_build(suo_buf, cfg.batch_mark_);

	std::cout << "fixed: " << fix_count << std::endl;
	//assert((fix_count % cfg.batch_mark_.size()) == 0);

	if(fix_count)
	{
		yggr::utf8_string tmp_out_fname = cfg.file_suo_ + ".tmp";
		write_to_file(tmp_out_fname, suo_buf);
		move_file(tmp_out_fname, cfg.file_suo_);
	}

	return 0;
}


int main(int argc, char* argv[])
{
	init_mark_name_str();
	int ret = main_detail(argc, argv);

	std::cout << "end" << std::endl;

	char cc = 0;
	std::cin >> cc;

	return ret;
}

#else

#include <iostream>

int main(int argc, char* argv[])
{
	std::cout << "this tool support windows only" << std::endl;

	char cc = 0;
	std::cin >> cc;
	return 0;
}

#endif // YGGR_AT_WINDOWS

