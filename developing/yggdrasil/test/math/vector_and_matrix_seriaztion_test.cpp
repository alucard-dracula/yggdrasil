//vector_seriaztion_and_matrix_test.cpp

#include <iostream>
#include <yggr/base/yggrdef.h>
#include <yggr/packet/packet.hpp>
#include <yggr/archive/binary_archive_partner.hpp>
#include <yggr/archive/text_archive_partner.hpp>
#include <yggr/archive/network_archive_partner.hpp>

#include <yggr/serialization/nvp.hpp>

#include <yggr/math/vector2d.hpp>
#include <yggr/math/vector3d.hpp>

#include <yggr/math/matrix2d.hpp>
#include <yggr/math/matrix3d.hpp>

#include <cassert>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#define TEST_NUM 0

#if TEST_NUM == 1
	typedef yggr::packet::packet<yggr::archive::archive_partner::binary_oarchive_partner> opak_type;
	typedef yggr::packet::packet<yggr::archive::archive_partner::binary_iarchive_partner> ipak_type;
#elif TEST_NUM == 2
	typedef yggr::packet::packet<yggr::archive::archive_partner::network_oarchive_partner> opak_type;
	typedef yggr::packet::packet<yggr::archive::archive_partner::network_iarchive_partner> ipak_type;
#else
	typedef yggr::packet::packet<yggr::archive::archive_partner::text_oarchive_partner> opak_type;
	typedef yggr::packet::packet<yggr::archive::archive_partner::text_iarchive_partner> ipak_type;
#endif // TEST_NUM

void vector2d_ser_test(void)
{
	typedef yggr::math::vector2d<yggr::s32> vec_type;

	vec_type vec(100, 200);
	opak_type opak;

	opak.save(vec);

	std::cout << opak.org_buf() << std::endl;

	//-------------------------------------------------

	vec_type vec2;

	ipak_type ipak(opak);

	ipak.load(vec2);

	assert(vec == vec2);

	std::cout << vec2 << std::endl;
}

void vector3d_ser_test(void)
{
	typedef yggr::math::vector3d<yggr::s32> vec_type;

	vec_type vec(100, 200, 300);
	opak_type opak;

	opak.save(vec);

	std::cout << opak.org_buf() << std::endl;

	//-------------------------------------------------

	vec_type vec2;

	ipak_type ipak(opak);

	ipak.load(vec2);

	assert(vec == vec2);

	std::cout << vec2 << std::endl;
}

void matrix2d_ser_test(void)
{
	typedef yggr::math::matrix2d<yggr::s32> mat_type;
	mat_type mat;

	opak_type opak;
	opak.save(mat);

	std::cout << opak.org_buf() << std::endl;

	mat_type mat2(0);
	ipak_type ipak(opak);
	ipak.load(mat2);

	assert(mat == mat2);
	std::cout << mat2 << std::endl;
}

void matrix3d_ser_test(void)
{
	typedef yggr::math::matrix3d<yggr::s32> mat_type;
	mat_type mat;

	opak_type opak;
	opak.save(mat);

	std::cout << opak.org_buf() << std::endl;

	mat_type mat2(0);
	ipak_type ipak(opak);
	ipak.load(mat2);

	assert(mat == mat2);
	std::cout << mat2 << std::endl;
}

int main(int argc, char* argv[])
{
	vector2d_ser_test();
	vector3d_ser_test();

	matrix2d_ser_test();
	matrix3d_ser_test();

	char cc = 0;
	std::cin >> cc;
	return 0;
}
