// textlow.cpp

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
	yggr::string fname = path;
	yggr::string buf;
	//local_fsys_operators_type::file_size_type size = -1;

	try
	{
		local_fsys_operators_type::read_file_of_binary(
			local_fsys_operators_type::make_path_object(fname),
			buf);
	}
	catch(const boost::filesystem::filesystem_error& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}
	catch(const yggr::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}

	std::cout << buf << std::endl;

	yggr::string str_re("(A)|(B)|(C)|(D)|(E)|(F)|(G)|(H)|(I)|(J)|(K)|(L)|(M)|(N)|(O)|(P)|(Q)|(R)|(S)|(T)|(U)|(V)|(W)|(X)|(Y)|(Z)");
	yggr::string str_fmt("(?1a)(?2b)(?3c)(?4d)(?5e)(?6f)(?7g)(?8h)(?9i)(?10j)(?11k)(?12l)(?13m)(?14n)(?15o)(?16p)(?17q)(?18r)"
						"(?19s)(?20t)(?21u)(?22v)(?23w)(?24x)(?25y)(?26z)");

	yggr::string new_buf(yggr::regular_parse::regex_parse::replace(
								buf,
								yggr::regular_parse::regex_parse::make_regex(str_re),
								str_fmt));


#if EXECUTE_PREVIEW()
	std::cout << new_buf << std::endl;
#else
	try
	{
		local_fsys_operators_type::write_file_of_binary(
			local_fsys_operators_type::make_path_object(fname),
			new_buf);
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


bool is_hit_file(const yggr::string& fname, const yggr::string& ftype)
{
	return (4 < fname.length()) && (fname.substr(fname.length() - 4, 4) == ftype);
}

void handler_for_each(const local_fsys_operators_type::directory_entry_type& entry,
						boost::uintmax_t current_count,
						local_fsys_operators_type::error_code_type* current_ec,
						path_list_type& pl,
						const yggr::string& ftype)
{
	if(is_hit_file(entry.path().string().c_str(), ftype))
	{
		pl.push_back(entry.path());
	}
}

void search_file(const yggr::string& path, const yggr::string& ftype)
{
	path_list_type pl;
	try
	{
		local_fsys_operators_type::recursive_for_each(
				local_fsys_operators_type::make_path_object(path),
				boost::bind(
					&handler_for_each,
					_1,
					_2,
					_3,
					boost::ref(pl),
					boost::cref(ftype)));
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
	yggr::string tftype;

	if(!((argc > 1) && (yggr::string("--batch-test") == argv[argc - 1])))
	{
		std::cin >> tftype;
	}
	else
	{
		tftype = "txt";
	}

	yggr::string ftype = yggr::string(".") + tftype;

	search_file("./", ftype);

	wait_any_key(argc, argv);
	return 0;
}
