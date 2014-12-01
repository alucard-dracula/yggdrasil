//params_xml_serialization.hpp

#include <iostream>
#include <vector>
#include <string>

#include <boost/serialization/access.hpp>
#include <boost/serialization/export.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/archive/xml_archive_partner.hpp>
#include <yggr/packet/packet.hpp>

#include <yggr/any_val/base_any_type_operator.hpp>
#include <yggr/any_val/any_val_operator.hpp>
#include <yggr/any_val/params.hpp>

#include <yggr/serialization/shared_ptr.hpp>
#include <yggr/serialization/vector.hpp>
#include <yggr/serialization/utility.hpp>

#include <yggr/any_val/params_io_wrap.hpp>

#include <yggr/ppex/concat.hpp>
#include <yggr/ppex/cast.hpp>
#include <yggr/ppex/cast_to_string.hpp>
#include <yggr/ppex/pp_debug.hpp>

//#include <yggr/type_traits/>

// warning: mingw use libboost_serialization-mgw47-mt-1_53.a //fix xml_gamma_error

#ifdef _MSC_VER
#   include <vld.h>
#endif //_MSC_VER

typedef yggr::packet::packet<yggr::archive::archive_partner::xml_oarchive_partner> opak;
typedef yggr::packet::packet<yggr::archive::archive_partner::xml_iarchive_partner> ipak;

class params_io_container
{
public:
	typedef yggr::any_val::any_val_operator<yggr::any_val::base_any_type_operator> any_val_op_type;
	typedef yggr::any_val::params<std::string, any_val_op_type> params_type;
	typedef yggr::any_val::basic_params_io_wrap<params_type> base_params_io_wrap_type;
	typedef	yggr::interface_ptr<base_params_io_wrap_type> interface_io_type;
	typedef interface_io_type::base_type io_ptr_type;
	typedef params_type::key_type key_type;

	typedef std::vector<io_ptr_type> io_ptr_vt_type;

	params_io_container(void)
	{
	}

	~params_io_container(void)
	{
	}


//这个函数第71行 修改你对应的结构的插入方法
#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename T ) > \
	bool append( YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, const key_type& key ), const params_type& params ) { \
		typedef yggr::packet::packet_info< \
					YGGR_PP_TEMPLATE_PARAMS_CUSTOM_TYPES( __n__, YGGR_PP_TEMPLATE_PARAMS_TYPES_MAKER ) > packet_info_type; \
		typedef yggr::any_val::params_io_wrap< params_type, packet_info_type > params_io_wrap_type; \
		interface_io_type io( new params_io_wrap_type() ); \
		params_io_wrap_type* pwrap = io; \
		if( !pwrap ) { return false; } \
		bool bright = pwrap->template init< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T ) >( YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, key), params ); \
		if( bright ){ \
			const io_ptr_type& ref_io_ptr = io; \
			_io_ptr_vt.push_back( ref_io_ptr ); } \
		return bright; }

#define YGGR_PP_TEMPLATE_CUSTOM_TYPE( __n__ ) \
	std::pair< key_type, BOOST_PP_CAT( T, __n__) >

#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE( )

#undef YGGR_PP_TEMPLATE_CUSTOM_TYPE

private:
	friend class boost::serialization::access;
	template<typename Archive>
	void serialize(Archive& ar, const yggr::u32 version)
	{
		ar & YGGR_SERIALIZE_NVP(_io_ptr_vt);
	}

public:
	io_ptr_vt_type _io_ptr_vt; //这个数据结构根据你自己的情况修改 例题里是vector
};

typedef params_io_container params_io_container_type;

typedef std::pair<std::string, int> int_pair_type;
typedef std::pair<std::string, float> float_pair_type;
typedef yggr::packet::packet_info<int_pair_type, int_pair_type> packet_info2_type;
typedef yggr::packet::packet_info<int_pair_type, int_pair_type, float_pair_type> packet_info3_type;
typedef yggr::packet::packet_info<float_pair_type, int_pair_type, int_pair_type, float_pair_type> packet_info4_type;

typedef yggr::any_val::params_io_wrap< params_io_container_type::params_type, packet_info2_type > params_io_wrap2_type;
typedef yggr::any_val::params_io_wrap< params_io_container_type::params_type, packet_info3_type > params_io_wrap3_type;
typedef yggr::any_val::params_io_wrap< params_io_container_type::params_type, packet_info4_type > params_io_wrap4_type;


BOOST_CLASS_EXPORT(YGGR_PP_CONCAT(yggr::packet::packet_info<int_pair_type, int_pair_type>))
BOOST_CLASS_EXPORT(params_io_wrap3_type)
BOOST_CLASS_EXPORT(params_io_wrap4_type)

void test_foo1(void)
{
	//std::cout << (YGGR_PP_DEBUG(BOOST_PP_LOCAL_MACRO(2))) << std::endl;
	typedef params_io_container_type::params_type params_type;
	params_type params2, params3, params4;

	params2["val1"] = ANY_SET(int, 10);
	params2["val2"] = ANY_SET(int, 100);

	params3["val1"] = ANY_SET(int, 10);
	params3["val2"] = ANY_SET(int, 100);
	params3["val3"] = ANY_SET(float, 100.5f);

	params4["val1"] = ANY_SET(float, 10.5f);
	params4["val2"] = ANY_SET(int, 100);
	params4["val3"] = ANY_SET(int, 1000);
	params4["val4"] = ANY_SET(float, 1000.5f);

	params_io_container_type cont;
	cont.append<int, int>(std::string("val1"), std::string("val2"), params2);
	cont.append<int, int, float>(std::string("val1"), std::string("val2"), std::string("val3"), params3);
	cont.append<float, int, int, float>(std::string("val1"), std::string("val2"),
											std::string("val3"), std::string("val4"), params4);

	opak o;

	boost::serialization::nvp<params_io_container_type> nvps("cont", cont);
	o.save(nvps);

	std::cout << o.val_buf() << std::endl;

	ipak i(o);
	params_io_container_type cont2;

	boost::serialization::nvp<params_io_container_type> nvpl("cont", cont2);
	i.load(nvpl);

	params_type params22(cont2._io_ptr_vt[0]->gen_params());
	params_type params32(cont2._io_ptr_vt[1]->gen_params());
	params_type params42(cont2._io_ptr_vt[2]->gen_params());

	assert(cont2._io_ptr_vt.size() == 3);

	assert(params2 == params22);
	assert(params3 == params32);
	assert(params4 == params42);
}



int main(int argc, char* argv[])
{
	test_foo1();

	char cc = 0;
	std::cin >> cc;
	return 0;
}
