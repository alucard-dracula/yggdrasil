//make_ipp.cpp

#error "this files is not example it mend open source info use it!!!!!"

#include <iostream>
#include <cctype>
#include <string>
#include <sstream>
#include <list>
#include <yggr/file_system/local_file_operator.hpp>

template<typename Container>
void get_file_list(Container& cont)
{
	try
	{
		yggr::file_system::local_file_operator_type::search_files("./", cont);
	}
	catch(const compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

template<typename Container>
void create_ipp(const std::string& dir, const Container& cont)
{
	typedef Container cont_type;
	typedef typename cont_type::const_iterator citer_type;
	for(citer_type i = cont.begin(), isize = cont.end(); i != isize; ++i)
	{
		write_ipp(dir, *i);
	}
}

void write_ipp(const std::string& wfpath, const std::string& org_full_name)
{
	std::string comment =
"/****************************************************************************\n \
Copyright (c) 2014-2018 yggdrasil\n \
author: yang xu\n \
\n \
Permission is hereby granted, free of charge, to any person obtaining a copy\n \
of this software and associated documentation files (the \"Software\"), to deal\n \
in the Software without restriction, including without limitation the rights\n \
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell\n \
copies of the Software, and to permit persons to whom the Software is\n \
furnished to do so, subject to the following conditions:\n \
\n \
The above copyright notice and this permission notice shall be included in\n \
all copies or substantial portions of the Software.\n \
\n \
THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n \
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n \
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\n \
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n \
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n \
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN\n \
THE SOFTWARE.\n \
****************************************************************************/\n\n";

	std::string tital_maroc_if = "#ifndef ";
	std::string tital_maroc_def = "#define ";
	std::string tital_maroc_endif = "#endif // ";
	std::string tital = "__YGGR_BOOST_SRC_SERIALIZATION_";
	std::string tital_end = "__";

	std::string org_fname = "";
	std::string org_path = "";
	yggr::file_system::local_file_operator_type::parsing_file_path_and_name(org_full_name, org_path, org_fname);

	//std::string fname = org_fname + ".ipp";
	std::string fname = std::string("yggr_") + org_fname;
	std::string ftitle = fname;
	std::transform(ftitle.begin(),ftitle.end(), ftitle.begin(),std::toupper);
	std::replace(ftitle.begin(), ftitle.end(), '.', '_');

	std::string wtital = tital + ftitle + tital_end;

	std::string need_include = "#include <yggr/support/boost_ver_include.hpp>";

	std::string tital_include = "#include ";
	std::string include_maroc = std::string("YGGR_PP_BOOST_SRC_INCLUDE(serialization,YGGR_PP_BOOST_VERSION(),") + org_fname + ")";

	std::string include = "";


	std::stringstream ss;
	ss << "//" << fname << "\n\n"
		<< comment
		//<< tital_maroc_if << wtital << "\n"
		//<< tital_maroc_def << wtital << "\n\n"
		<< need_include << "\n"
		<< tital_include << include_maroc << "\n\n"
		//<< tital_maroc_endif << wtital << "\n"
		;

	//std::cout << ss.str() << std::endl;
	

	if(!yggr::file_system::local_file_operator_type::is_exists_path(wfpath))
	{
		bool bright = false;
		try
		{
			bright = yggr::file_system::local_file_operator_type::create_path(wfpath);
		}
		catch(const boost::filesystem::filesystem_error& e)
		{
			std::cerr << e.what() << std::endl;
			return;
		}
		catch(const compatibility::stl_exception& e)
		{
			std::cerr << e.what() << std::endl;
			return;
		}
		if(!bright)
		{
			return;
		}
	}

	try
	{
		yggr::file_system::local_file_operator_type::write_file_of_binary(wfpath + "/" + fname, ss.str());
	}
	catch(const boost::filesystem::filesystem_error& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}
	catch(const compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}
	std::cout << "write_file:" << (wfpath + "/" + fname) << std::endl;

}



int main(int argc, char* argv[])
{
	std::list<std::string> cont;
	get_file_list(cont);

	std::cout << cont.size() << std::endl;
	create_ipp("ipp_dir", cont);

	std::cout << "end" << std::endl;
	char cc = 0;
	std::cin >> cc;

	return 0;
}
