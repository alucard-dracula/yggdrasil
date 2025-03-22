// textlow_fname.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/container/vector.hpp>
#include <yggr/charset/string.hpp>

#include <yggr/file_system/local_fsys_operators.hpp>
#include <yggr/regular_parse/regex_parse.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#define EXECUTE_PREVIEW() 1

typedef yggr::file_system::path_check file_check_type;
typedef yggr::file_system::local_fsys_operators local_fsys_operators_type;
typedef local_fsys_operators_type::path_type path_type;
typedef yggr::vector<path_type> path_list_type;
typedef path_list_type::iterator path_list_iter_type;


void change_low(const yggr::string& path)
{
	yggr::string full_name = path;

	yggr::string dname;
	yggr::string fname;
	file_check_type::parsing_file_path_and_name(full_name, dname, fname);

	yggr::string str_re("(A)|(B)|(C)|(D)|(E)|(F)|(G)|(H)|(I)|(J)|(K)|(L)|(M)|(N)|(O)|(P)|(Q)|(R)|(S)|(T)|(U)|(V)|(W)|(X)|(Y)|(Z)");
	yggr::string str_fmt("(?1a)(?2b)(?3c)(?4d)(?5e)(?6f)(?7g)(?8h)(?9i)(?10j)(?11k)(?12l)(?13m)(?14n)(?15o)(?16p)(?17q)(?18r)"
							"(?19s)(?20t)(?21u)(?22v)(?23w)(?24x)(?25y)(?26z)");

	//yggr::string str_re("(vcxproj)");
	//yggr::string str_fmt("(?1vcproj)");

	yggr::string new_fname(yggr::regular_parse::regex_parse::replace(
								fname,
								yggr::regular_parse::regex_parse::make_regex(str_re),
								str_fmt));

	if(new_fname == fname)
	{
		return;
	}

	yggr::string new_full_name = dname + new_fname;

#if EXECUTE_PREVIEW()
	std::cout << "---------------------------" << std::endl;
	std::cout << "old_name = " << full_name << std::endl;
	std::cout << "new_name = " << new_full_name << std::endl;
	std::cout << "---------------------------" << std::endl;

#else
    bool bright = false;

	try
	{
		bright =
			local_fsys_operators_type::rename_file(
				local_fsys_operators_type::make_path_object(full_name),
				local_fsys_operators_type::make_path_object(new_full_name));
	}
	catch(const boost::filesystem::filesystem_error& e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch(const yggr::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
#endif // EXECUTE_PREVIEW
};

void handler_for_each(const local_fsys_operators_type::directory_entry_type& entry,
						boost::uintmax_t current_count,
						local_fsys_operators_type::error_code_type* current_ec,
						path_list_type& pl)
{
	pl.push_back(entry.path());
}


void search_file(const yggr::string& path)
{
	path_list_type pl;
	try
	{
		local_fsys_operators_type::recursive_for_each(
			local_fsys_operators_type::make_path_object(path),
			boost::bind(&handler_for_each, _1, _2, _3, boost::ref(pl)));
	}
	catch(const yggr::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
		pl.clear();
	}

	for(path_list_iter_type p = pl.begin(); p != pl.end(); ++p)
	{
		change_low(yggr::string((*p).string().c_str()));
	}
};

int main(int argc, char* argv[])
{
	search_file("./");

	wait_any_key(argc, argv);
	return 0;
}
