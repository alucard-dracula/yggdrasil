//file_system_test.cpp
#include <iostream>
#include <string>
#include <fstream>
#include <yggr/file_system/ofile_mgr.hpp>
#include <yggr/file_system/ifile_mgr.hpp>

#include <yggr/packet/packet.hpp>
#include <yggr/archive/binary_archive_partner.hpp>
#include <yggr/serialization/vector.hpp>
#include <sstream>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

typedef yggr::packet::packet<yggr::archive::archive_partner::binary_oarchive_partner> opak_type;
typedef yggr::packet::packet<yggr::archive::archive_partner::binary_iarchive_partner> ipak_type;
typedef yggr::file_system::ofile_mgr ofile_mgr_type;
typedef yggr::file_system::ifile_mgr ifile_mgr_type;

struct A
{
public:
	A(void)
		: n(0), f(0.0f)
	{
	}
	A(int a)
		: n(100), f(20.12345), str("abcdefg")
	{
	}

	~A(void)
	{
	}

private:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, yggr::u32 version)
	{
		ar & n;
		ar & str;
		ar & f;
	}

public:
	int n;
	std::string str;
	float f;
};

int file_size = 0;

void write(void)
{
	ofile_mgr_type ofmgr("abc.fsys");

	//ofile_mgr_type ofmgr;


	A a(1);
	opak_type opak;
	opak.save(a);

	for(int i = 0; i < file_size; ++i)
	{
		std::stringstream ss;
		ss << i << ".bin";
		ofmgr.add_file(ss.str(), ofile_mgr_type::E_File_Binary, opak.org_buf());
	}

	//ofmgr.open("abc.fsys");
	ofmgr.rm_file("3.bin");
	ofmgr.save();

	ofile_mgr_type::file_list_type file_list;
	ofmgr.get_file_list(file_list);


	ifile_mgr_type::file_list_type::iterator isize = file_list.end();
	for(ifile_mgr_type::file_list_type::iterator i = file_list.begin(); i != isize; ++i)
	{
		std::cout << *i << std::endl;
	}

	ofmgr.close();
}


void read(void)
{

	ifile_mgr_type ifmgr("abc.fsys", 100);

	for(int i = 0; i < file_size; ++i)
	{
		std::stringstream ss;
		ss << i << ".bin";

		//if(i == 3)
		//{
		//	int nnn = 0;
		//}

		ifile_mgr_type::buf_type buf;
		if(ifmgr.get_file(ss.str(), buf))
		{
			ipak_type ipak(buf);
			A a;
			ipak.load(a);
			std::cout << a.n << "," << a.f << "," << a.str << std::endl;
		}
		else
		{
			std::cout << "file not found" << std::endl;
		}
	}

	ifile_mgr_type::file_list_type file_list;
	ifmgr.get_file_list(file_list);

	ifile_mgr_type::file_list_type::iterator isize = file_list.end();
	for(ifile_mgr_type::file_list_type::iterator i = file_list.begin(); i != isize; ++i)
	{
		std::cout << *i << std::endl;
	}

	ifmgr.close();
}

int main(int argc, char* argv[])
{
	std::cin >> file_size;
	write();
	std::cout << "write end" << std::endl;
	char c = 0;
	std::cin >> c;
	read();
	std::cout << "read end" << std::endl;
	char cc = 0;
	std::cin >> cc;
	return 0;
}
