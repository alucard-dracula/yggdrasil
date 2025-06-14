//vector_and_matrix_nvp_seriaztion_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <cassert>

#include <yggr/base/yggrdef.h>
#include <yggr/packet/packet.hpp>
#include <yggr/archive/xml_archive_partner.hpp>

//#if !(defined(YGGR_AT_ANDROID) || defined(YGGR_AT_IOS) || defined(YGGR_TEST_DISABLE_NON_GENERIC_SUPPORT))
#	include <yggr/archive/bson_archive_partner.hpp>
//#endif //#if !(defined(YGGR_AT_ANDROID) || defined(YGGR_AT_IOS) || defined(YGGR_TEST_DISABLE_NON_GENERIC_SUPPORT))

#include <yggr/serialization/nvp.hpp>

#include <yggr/math/vector2d.hpp>
#include <yggr/math/vector3d.hpp>

#include <yggr/math/matrix2d.hpp>
#include <yggr/math/matrix3d.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#   include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

template<typename OPak, typename IPak>
void vector2d_ser_test(void)
{
	typedef OPak opak_type;
	typedef IPak ipak_type;

	typedef yggr::math::vector2d<yggr::s32> vec_type;

	vec_type vec(100, 200);
	opak_type opak;

	opak.save(YGGR_SERIALIZATION_NAME_NVP("vector2d", vec));

	std::cout << opak.org_buf() << std::endl;

	//-------------------------------------------------

	vec_type vec2;

	ipak_type ipak(opak);

	ipak.load(YGGR_SERIALIZATION_NAME_NVP("vector2d", vec2));

	yggr_test_assert(vec == vec2);

	std::cout << vec2 << std::endl;
}

template<typename OPak, typename IPak>
void vector3d_ser_test(void)
{
	typedef OPak opak_type;
	typedef IPak ipak_type;

	typedef yggr::math::vector3d<yggr::s32> vec_type;

	vec_type vec(100, 200, 300);
	opak_type opak;

	opak.save(YGGR_SERIALIZATION_NAME_NVP("vector3d", vec));

	std::cout << opak.org_buf() << std::endl;

	//-------------------------------------------------

	vec_type vec2;

	ipak_type ipak(opak);

	ipak.load(YGGR_SERIALIZATION_NAME_NVP("vector3d", vec2));

	yggr_test_assert(vec == vec2);

	std::cout << vec2 << std::endl;
}

template<typename OPak, typename IPak>
void matrix2d_ser_test(void)
{
	typedef OPak opak_type;
	typedef IPak ipak_type;

	typedef yggr::math::matrix2d<yggr::s32> mat_type;
	mat_type mat;

	opak_type opak;
	opak.save(YGGR_SERIALIZATION_NAME_NVP("matrix2d", mat));

	std::cout << opak.org_buf() << std::endl;

	mat_type mat2(0);
	ipak_type ipak(opak);
	ipak.load(YGGR_SERIALIZATION_NAME_NVP("matrix2d", mat2));

	yggr_test_assert(mat == mat2);
	std::cout << mat2 << std::endl;
}

template<typename OPak, typename IPak>
void matrix3d_ser_test(void)
{
	typedef OPak opak_type;
	typedef IPak ipak_type;

	typedef yggr::math::matrix3d<yggr::s32> mat_type;
	mat_type mat;

	opak_type opak;
	opak.save(YGGR_SERIALIZATION_NAME_NVP("matrix3d", mat));

	std::cout << opak.org_buf() << std::endl;

	mat_type mat2(0);
	ipak_type ipak(opak);
	ipak.load(YGGR_SERIALIZATION_NAME_NVP("matrix3d", mat2));

	yggr_test_assert(mat == mat2);
	std::cout << mat2 << std::endl;
}

int main(int argc, char* argv[])
{

	typedef yggr::packet::packet<yggr::archive::archive_partner::xml_oarchive_partner> xml_opak_type;
	typedef yggr::packet::packet<yggr::archive::archive_partner::xml_iarchive_partner> xml_ipak_type;

	vector2d_ser_test<xml_opak_type, xml_ipak_type>();
	vector3d_ser_test<xml_opak_type, xml_ipak_type>();

	matrix2d_ser_test<xml_opak_type, xml_ipak_type>();
	matrix3d_ser_test<xml_opak_type, xml_ipak_type>();

//#if !(defined(YGGR_AT_ANDROID) || defined(YGGR_AT_IOS) || defined(YGGR_TEST_DISABLE_NON_GENERIC_SUPPORT))
	typedef yggr::packet::packet<yggr::archive::archive_partner::bson_oarchive_partner> bson_opak_type;
	typedef yggr::packet::packet<yggr::archive::archive_partner::bson_iarchive_partner> bson_ipak_type;

	vector2d_ser_test<bson_opak_type, bson_ipak_type>();
	vector3d_ser_test<bson_opak_type, bson_ipak_type>();

	matrix2d_ser_test<bson_opak_type, bson_ipak_type>();
	matrix3d_ser_test<bson_opak_type, bson_ipak_type>();

//#endif //#if !(defined(YGGR_AT_ANDROID) || defined(YGGR_AT_IOS) || defined(YGGR_TEST_DISABLE_NON_GENERIC_SUPPORT))

	std::cout << "all test end" << std::endl;

	wait_any_key(argc, argv);
	return 0;
}
