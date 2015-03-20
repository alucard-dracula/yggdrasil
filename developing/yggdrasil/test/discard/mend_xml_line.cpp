//mend_xml_line.cpp

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include <yggr/file_system/local_file_operator.hpp>
#include <boost/progress.hpp>

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        std::cerr << "input params !!" << std::endl;

        return 0;
    }

    std::vector<std::string>  paths;

	try
	{
		yggr::file_system::local_file_operator_type::search_files(argv[1], paths);
	}
	catch(const compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
		return -1;
	}

    int i =0, isize = paths.size();
    boost::progress_display pd(isize);
    std::string line("\r\n");
    for(; i != isize; ++i, ++pd)
    {
        std::ifstream ifs;
        ifs.open(paths[i].c_str());
        std::string str_file;

        for(int j = 0;!ifs.eof(); ++j)
        {
            char buf[1024] = {0};
            ifs.getline(buf, 1023);

            if(!j)
            {
                buf[std::find(buf, buf+strlen(buf), '>') - buf] = 0;
                std::stringstream ss;
                ss << buf << " type = \"" << argv[2] << "\" >\r\n";
                str_file += ss.str();
                continue;
            }

            int len = strlen(buf);
            if(buf[len - 1] == '\n' || buf[len - 1] =='\r')
            {
                buf[len - 1] = 0;
            }

            if(buf[len - 2] == '\n' || buf[len - 2] =='\r')
            {
                buf[len - 2] = 0;
            }

            str_file += buf;
            str_file += line;
        }

        ifs.close();

        std::ofstream ofs;
        ofs.open(paths[i].c_str(), std::ios::binary | std::ios::out);
        ofs.write(str_file.c_str(), str_file.size());
        ofs.close();

    }

    paths.clear();

    std::cout << "endl" << std::endl;

    return 0;
}
