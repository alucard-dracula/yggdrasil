//bson_pak_test.cpp

#if !defined(_MSC_VER)
#	error "this file supports MSVC only, if you are using other compilers, please use the same name file in the charset_utf8_files"	
#endif // _MSC_VER


#define YGGR_SERIALIZE_SIZE32

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/packet/packet.hpp>

#include <yggr/archive/network_archive_partner.hpp>
#include <yggr/archive/bson_archive_partner.hpp>

#include <yggr/serialization/nvp.hpp>
#include <yggr/serialization/utf8_string.hpp>


#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

typedef yggr::packet::packet<yggr::archive::archive_partner::network_oarchive_partner> opacket_type;
typedef yggr::packet::packet<yggr::archive::archive_partner::network_iarchive_partner> ipacket_type;

typedef yggr::packet::packet<yggr::archive::archive_partner::bson_oarchive_partner> o_bson_packet_type;
typedef yggr::packet::packet<yggr::archive::archive_partner::bson_iarchive_partner> i_bson_packet_type;


struct type_test
{
	type_test(void)
		: str_("Hello,  ¿ΩÁ")
	{
	}

	type_test(int n)
	{
	}

private:
	friend class boost::serialization::access;
	template<typename Archive>
	void serialize(Archive & ar, yggr::u32 version)
	{
		ar & YGGR_SERIALIZATION_NVP(str_);
	}

public:
	yggr::charset::utf8_string str_;
};

std::ostream& operator<<(std::ostream& os, const type_test& val)
{
	os << val.str_;
	return os;
}

int main(int argc, char* argv[])
{
	type_test test;
	std::cout << test << std::endl;
	o_bson_packet_type opak;
	opak.save(YGGR_SERIALIZATION_NAME_NVP("test_tt", test));

	type_test test2(10);

	i_bson_packet_type ipak(opak);


	ipak.load(YGGR_SERIALIZATION_NAME_NVP("test_tt", test2));

	std::cout << test2 << std::endl;

	std::cout << test2.str_ << std::endl;

	yggr_test_assert(test.str_ == test2.str_);

	wait_any_key(argc, argv);
	return 0;
}
