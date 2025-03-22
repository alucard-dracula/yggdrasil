// geo_bezier_curve_test.cpp

#define YGGR_MATH_F32_DEFAULT_PRECISION() 0.0001f

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/math/vector2d.hpp>
#include <yggr/math/vector3d.hpp>
#include <yggr/geometry/geo_bezier_curve.hpp>

#include <yggr/archive/xml_archive_partner.hpp>
#include <yggr/packet/packet.hpp>
#include <yggr/serialization/nvp.hpp>
#include <yggr/serialization/array.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

template<typename T>
void test_2d(void)
{
	typedef yggr::math::vector2d<T> point_type;
	typedef yggr::geometry::geo_bezier_curve<point_type, 3> bezier_curve_type;
	typedef typename bezier_curve_type::point_array_type point_array_type;
	typedef typename bezier_curve_type::result_point_type result_point_type;
	typedef yggr::math::flaw_comparer<result_point_type> cmp_type;
	typedef yggr::math::flaw_comparer<yggr::f32> f32_cmp_type;

	typedef yggr::math::matrix2d<T> mat_type;

	point_array_type arr = {point_type(0, 240), point_type(210, 5), point_type(420, 475), point_type(640, 240)};
	bezier_curve_type bezier(arr);

	{
		mat_type mat(1);

		bezier_curve_type rst;

		rst = bezier * mat;
		yggr_test_assert(rst == bezier);

		rst *= mat;
		yggr_test_assert(rst == bezier);
	}

	result_point_type p1, p2;
	for(int i = 0, isize = 11, icount = 10; i != isize; ++i)
	{
		p1 = bezier[std::make_pair(i, icount)];
		p2 = bezier[static_cast<yggr::f32>(i) / icount];
		std::cout << p1 << std::endl;
		std::cout << p2 << std::endl;
		yggr_test_assert(cmp_type(p1) == p2);
		yggr_test_assert(f32_cmp_type(p1.w) == 1.0f);

		p1 = bezier(std::make_pair(i, icount));
		p2 = bezier(static_cast<yggr::f32>(i) / icount);
		std::cout << p1 << std::endl;
		std::cout << p2 << std::endl;
		yggr_test_assert(cmp_type(p1) == p2);
		yggr_test_assert(f32_cmp_type(p1.w) == 0.0f);

		p1 = yggr::geometry::geo_bezier_curve_value(bezier, i, icount);
		p2 = yggr::geometry::geo_bezier_curve_value(bezier, static_cast<yggr::f32>(i) / icount);
		std::cout << p1 << std::endl;
		std::cout << p2 << std::endl;
		yggr_test_assert(cmp_type(p1) == p2);
		yggr_test_assert(f32_cmp_type(p1.w) == 1.0f);

		p1 = yggr::geometry::geo_bezier_curve_derivation(bezier, i, icount);
		p2 = yggr::geometry::geo_bezier_curve_derivation(bezier, static_cast<yggr::f32>(i) / icount);
		std::cout << p1 << std::endl;
		std::cout << p2 << std::endl;
		yggr_test_assert(cmp_type(p1) == p2);
		yggr_test_assert(f32_cmp_type(p1.w) == 0.0f);

		p1 = yggr::geometry::geo_bezier_curve_value(boost::begin(arr), boost::end(arr), i, icount);
		p2 = yggr::geometry::geo_bezier_curve_value(boost::begin(arr), boost::end(arr), static_cast<yggr::f32>(i) / icount);
		std::cout << p1 << std::endl;
		std::cout << p2 << std::endl;
		yggr_test_assert(cmp_type(p1) == p2);
		yggr_test_assert(f32_cmp_type(p1.w) == 1.0f);

		p1 = yggr::geometry::geo_bezier_curve_derivation(boost::begin(arr), boost::end(arr), i, icount);
		p2 = yggr::geometry::geo_bezier_curve_derivation(boost::begin(arr), boost::end(arr), static_cast<yggr::f32>(i) / icount);
		std::cout << p1 << std::endl;
		std::cout << p2 << std::endl;
		yggr_test_assert(cmp_type(p1) == p2);
		yggr_test_assert(f32_cmp_type(p1.w) == 0.0f);

		p1 = yggr::geometry::geo_bezier_curve_value(arr, i, icount);
		p2 = yggr::geometry::geo_bezier_curve_value(arr, static_cast<yggr::f32>(i) / icount);
		std::cout << p1 << std::endl;
		std::cout << p2 << std::endl;
		yggr_test_assert(cmp_type(p1) == p2);
		yggr_test_assert(f32_cmp_type(p1.w) == 1.0f);

		p1 = yggr::geometry::geo_bezier_curve_derivation(arr, i, icount);
		p2 = yggr::geometry::geo_bezier_curve_derivation(arr, static_cast<yggr::f32>(i) / icount);
		std::cout << p1 << std::endl;
		std::cout << p2 << std::endl;
		yggr_test_assert(cmp_type(p1) == p2);
		yggr_test_assert(f32_cmp_type(p1.w) == 0.0f);
	}
}

template<typename T>
void test_2d_zwei(void)
{
	typedef yggr::math::vector2d<T> point_type;
	typedef yggr::geometry::geo_bezier_curve<point_type, 3> bezier_curve_type;
	typedef typename bezier_curve_type::point_array_type point_array_type;
	typedef typename bezier_curve_type::result_point_type result_point_type;
	typedef yggr::math::flaw_comparer<result_point_type> cmp_type;
	typedef yggr::math::flaw_comparer<yggr::f32> f32_cmp_type;

	point_array_type arr = {point_type(0, 240), point_type(210, 5), point_type(420, 475), point_type(640, 240)};
	bezier_curve_type bezier(arr);
	bezier_curve_type bezier_zwei(point_type(0, 240), point_type(210, 5), point_type(420, 475), point_type(640, 240));

	yggr_test_assert(bezier.equal_to(bezier_zwei));
	yggr_test_assert(bezier == bezier_zwei);
	yggr_test_assert(~bezier == ~bezier_zwei);
	yggr_test_assert(~bezier != bezier_zwei);


	bezier_curve_type bezier_tmp1;
	bezier_curve_type bezier_tmp2(boost::begin(arr), boost::end(arr));
	bezier_curve_type bezier_tmp3(boost::move(bezier_tmp2));
	bezier_curve_type bezier_tmp4(bezier_tmp3);

	result_point_type p1, p2;
	for(int i = 0, isize = 11, icount = 10; i != isize; ++i)
	{
		p1 = bezier[std::make_pair(i, icount)];
		p2 = bezier_zwei[std::make_pair(i, icount)];
		std::cout << p1 << std::endl;
		std::cout << p2 << std::endl;
		yggr_test_assert(cmp_type(p1) == p2);
		yggr_test_assert(f32_cmp_type(p1.w) == 1.0f);

		p1 = bezier[static_cast<yggr::f32>(i) / icount];
		p2 = bezier_zwei[static_cast<yggr::f32>(i) / icount];
		std::cout << p1 << std::endl;
		std::cout << p2 << std::endl;
		yggr_test_assert(cmp_type(p1) == p2);
		yggr_test_assert(f32_cmp_type(p1.w) == 1.0f);

		p1 = bezier(std::make_pair(i, icount));
		p2 = bezier_zwei(std::make_pair(i, icount));
		std::cout << p1 << std::endl;
		std::cout << p2 << std::endl;
		yggr_test_assert(cmp_type(p1) == p2);
		yggr_test_assert(f32_cmp_type(p1.w) == 0.0f);

		p1 = bezier(static_cast<yggr::f32>(i) / icount);
		p2 = bezier_zwei(static_cast<yggr::f32>(i) / icount);
		std::cout << p1 << std::endl;
		std::cout << p2 << std::endl;
		yggr_test_assert(cmp_type(p1) == p2);
		yggr_test_assert(f32_cmp_type(p1.w) == 0.0f);
	}
}

template<typename T>
void test_3d(void)
{
	typedef yggr::math::vector3d<T> point_type;
	typedef yggr::geometry::geo_bezier_curve<point_type, 3> bezier_curve_type;
	typedef typename bezier_curve_type::point_array_type point_array_type;
	typedef typename bezier_curve_type::result_point_type result_point_type;
	typedef yggr::math::flaw_comparer<result_point_type> cmp_type;
	typedef yggr::math::flaw_comparer<yggr::f32> f32_cmp_type;

	typedef yggr::math::matrix3d<T> mat_type;

	point_array_type arr = {point_type(0, 240, 100), point_type(210, 5, 100), point_type(420, 475, 100), point_type(640, 240, 100)};
	bezier_curve_type bezier(arr);

	{
		mat_type mat(1);

		bezier_curve_type rst;

		rst = bezier * mat;
		yggr_test_assert(rst == bezier);

		rst *= mat;
		yggr_test_assert(rst == bezier);
	}

	result_point_type p1, p2;
	for(int i = 0, isize = 11, icount = 10; i != isize; ++i)
	{
		p1 = bezier[std::make_pair(i, icount)];
		p2 = bezier[static_cast<yggr::f32>(i) / icount];
		std::cout << p1 << std::endl;
		std::cout << p2 << std::endl;
		yggr_test_assert(cmp_type(p1) == p2);
		yggr_test_assert(f32_cmp_type(p1.w) == 1.0f);

		p1 = bezier[static_cast<yggr::f32>(i) / icount];
		p2 = bezier[static_cast<yggr::f32>(i) / icount];
		std::cout << p1 << std::endl;
		std::cout << p2 << std::endl;
		yggr_test_assert(cmp_type(p1) == p2);
		yggr_test_assert(f32_cmp_type(p1.w) == 1.0f);

		p1 = bezier(std::make_pair(i, icount));
		p2 = bezier(static_cast<yggr::f32>(i) / icount);
		std::cout << p1 << std::endl;
		std::cout << p2 << std::endl;
		yggr_test_assert(cmp_type(p1) == p2);
		yggr_test_assert(f32_cmp_type(p1.w) == 0.0f);

		p1 = yggr::geometry::geo_bezier_curve_value(bezier, i, icount);
		p2 = yggr::geometry::geo_bezier_curve_value(bezier, static_cast<yggr::f32>(i) / icount);
		std::cout << p1 << std::endl;
		std::cout << p2 << std::endl;
		yggr_test_assert(cmp_type(p1) == p2);
		yggr_test_assert(f32_cmp_type(p1.w) == 1.0f);

		p1 = yggr::geometry::geo_bezier_curve_derivation(bezier, i, icount);
		p2 = yggr::geometry::geo_bezier_curve_derivation(bezier, static_cast<yggr::f32>(i) / icount);
		std::cout << p1 << std::endl;
		std::cout << p2 << std::endl;
		yggr_test_assert(cmp_type(p1) == p2);
		yggr_test_assert(f32_cmp_type(p1.w) == 0.0f);

		p1 = yggr::geometry::geo_bezier_curve_value(boost::begin(arr), boost::end(arr), i, icount);
		p2 = yggr::geometry::geo_bezier_curve_value(boost::begin(arr), boost::end(arr), static_cast<yggr::f32>(i) / icount);
		std::cout << p1 << std::endl;
		std::cout << p2 << std::endl;
		yggr_test_assert(cmp_type(p1) == p2);
		yggr_test_assert(f32_cmp_type(p1.w) == 1.0f);

		p1 = yggr::geometry::geo_bezier_curve_derivation(boost::begin(arr), boost::end(arr), i, icount);
		p2 = yggr::geometry::geo_bezier_curve_derivation(boost::begin(arr), boost::end(arr), static_cast<yggr::f32>(i) / icount);
		std::cout << p1 << std::endl;
		std::cout << p2 << std::endl;
		yggr_test_assert(cmp_type(p1) == p2);
		yggr_test_assert(f32_cmp_type(p1.w) == 0.0f);

		p1 = yggr::geometry::geo_bezier_curve_value(arr, i, icount);
		p2 = yggr::geometry::geo_bezier_curve_value(arr, static_cast<yggr::f32>(i) / icount);
		std::cout << p1 << std::endl;
		std::cout << p2 << std::endl;
		yggr_test_assert(cmp_type(p1) == p2);
		yggr_test_assert(f32_cmp_type(p1.w) == 1.0f);

		p1 = yggr::geometry::geo_bezier_curve_derivation(arr, i, icount);
		p2 = yggr::geometry::geo_bezier_curve_derivation(arr, static_cast<yggr::f32>(i) / icount);
		std::cout << p1 << std::endl;
		std::cout << p2 << std::endl;
		yggr_test_assert(cmp_type(p1) == p2);
		yggr_test_assert(f32_cmp_type(p1.w) == 0.0f);
	}
}

template<typename T>
void test_3d_zwei(void)
{
	typedef yggr::math::vector3d<T> point_type;
	typedef yggr::geometry::geo_bezier_curve<point_type, 3> bezier_curve_type;
	typedef typename bezier_curve_type::point_array_type point_array_type;
	typedef typename bezier_curve_type::result_point_type result_point_type;
	typedef yggr::math::flaw_comparer<result_point_type> cmp_type;

	point_array_type arr = {point_type(0, 240, 100), point_type(210, 5, 100), point_type(420, 475, 100), point_type(640, 240, 100)};
	bezier_curve_type bezier(arr);
	bezier_curve_type bezier_zwei(point_type(0, 240, 100), point_type(210, 5, 100), point_type(420, 475, 100), point_type(640, 240, 100));

	yggr_test_assert(bezier.equal_to(bezier_zwei));
	yggr_test_assert(bezier == bezier_zwei);
	yggr_test_assert(~bezier == ~bezier_zwei);
	yggr_test_assert(~bezier != bezier_zwei);

	bezier_curve_type bezier_tmp1;
	bezier_curve_type bezier_tmp2(boost::begin(arr), boost::end(arr));
	bezier_curve_type bezier_tmp3(boost::move(bezier_tmp2));
	bezier_curve_type bezier_tmp4(bezier_tmp3);

	result_point_type p1, p2;
	for(int i = 0, isize = 11, icount = 10; i != isize; ++i)
	{
		p1 = bezier[std::make_pair(i, icount)];
		p2 = bezier_zwei[std::make_pair(i, icount)];
		std::cout << p1 << std::endl;
		std::cout << p2 << std::endl;
		yggr_test_assert(cmp_type(p1) == p2);

		p1 = bezier[static_cast<yggr::f32>(i) / icount];
		p2 = bezier_zwei[static_cast<yggr::f32>(i) / icount];
		std::cout << p1 << std::endl;
		std::cout << p2 << std::endl;
		yggr_test_assert(cmp_type(p1) == p2);

		p1 = bezier(std::make_pair(i, icount));
		p2 = bezier_zwei(std::make_pair(i, icount));
		std::cout << p1 << std::endl;
		std::cout << p2 << std::endl;
		yggr_test_assert(cmp_type(p1) == p2);

		p1 = bezier(static_cast<yggr::f32>(i) / icount);
		p2 = bezier_zwei(static_cast<yggr::f32>(i) / icount);
		std::cout << p1 << std::endl;
		std::cout << p2 << std::endl;
		yggr_test_assert(cmp_type(p1) == p2);
	}
}

void test_serialize(void)
{
	typedef yggr::packet::packet<yggr::archive::archive_partner::xml_oarchive_partner> opak_type;
	typedef yggr::packet::packet<yggr::archive::archive_partner::xml_iarchive_partner> ipak_type;

	typedef yggr::math::vector3d<yggr::f32> point_type;
	typedef yggr::geometry::geo_bezier_curve<point_type, 3> bezier_curve_type;
	typedef bezier_curve_type::point_array_type point_array_type;
	typedef bezier_curve_type::result_point_type result_point_type;
	typedef yggr::math::flaw_comparer<result_point_type> cmp_type;

	point_array_type arr = {point_type(0, 240, 100), point_type(210, 5, 100), point_type(420, 475, 100), point_type(640, 240, 100)};
	bezier_curve_type bezier_temp(arr);

	opak_type opak;
	opak.save(YGGR_SERIALIZATION_NAME_NVP("bezier_curve", bezier_temp));
	std::cout << opak.val_buf() << std::endl;


	ipak_type ipak(boost::move(opak));
	bezier_curve_type bezier;
	ipak.load(YGGR_SERIALIZATION_NAME_NVP("bezier_curve", bezier));

	bezier_curve_type bezier_zwei(point_type(0, 240, 100), point_type(210, 5, 100), point_type(420, 475, 100), point_type(640, 240, 100));

	bezier_curve_type bezier_tmp1;
	bezier_curve_type bezier_tmp2(boost::begin(arr), boost::end(arr));
	bezier_curve_type bezier_tmp3(boost::move(bezier_tmp2));
	bezier_curve_type bezier_tmp4(bezier_tmp3);

	result_point_type p1, p2;
	for(int i = 0, isize = 11, icount = 10; i != isize; ++i)
	{
		p1 = bezier[std::make_pair(i, icount)];
		p2 = bezier_zwei[std::make_pair(i, icount)];
		std::cout << p1 << std::endl;
		std::cout << p2 << std::endl;
		yggr_test_assert(cmp_type(p1) == p2);

		p1 = bezier[static_cast<yggr::f32>(i) / icount];
		p2 = bezier_zwei[static_cast<yggr::f32>(i) / icount];
		std::cout << p1 << std::endl;
		std::cout << p2 << std::endl;
		yggr_test_assert(cmp_type(p1) == p2);

		p1 = bezier(std::make_pair(i, icount));
		p2 = bezier_zwei(std::make_pair(i, icount));
		std::cout << p1 << std::endl;
		std::cout << p2 << std::endl;
		yggr_test_assert(cmp_type(p1) == p2);

		p1 = bezier(static_cast<yggr::f32>(i) / icount);
		p2 = bezier_zwei(static_cast<yggr::f32>(i) / icount);
		std::cout << p1 << std::endl;
		std::cout << p2 << std::endl;
		yggr_test_assert(cmp_type(p1) == p2);
	}
}


//template<typename T>
//void test_bug(void)
//{
//	typedef yggr::math::vector3d<T> point_type;
//	typedef yggr::geometry::geo_bezier_curve<point_type, 3> bezier_curve_type;
//	typedef bezier_curve_type::point_array_type point_array_type;
//	typedef bezier_curve_type::result_point_type result_point_type;
//	typedef yggr::math::flaw_comparer<result_point_type> cmp_type;
//
//	point_array_type arr = {point_type(0, 240, 100), point_type(210, 5, 100), point_type(420, 475, 100), point_type(640, 240, 100)};
//	bezier_curve_type bezier(arr);
//	bezier_curve_type bezier_zwei(point_type(0, 240, 100), point_type(210, 5, 100), point_type(420, 475, 100), point_type(640, 240, 100));
//
//	//yggr_test_assert(bezier.equal_to(bezier_zwei));
//	//yggr_test_assert(bezier == bezier_zwei);
//	yggr_test_assert(~bezier == ~bezier_zwei);
//	//yggr_test_assert(~bezier != bezier_zwei);
//}

int main(int argc, char* argv[])
{
	test_2d<yggr::f32>();
	test_2d<yggr::s32>();

	test_2d_zwei<yggr::f32>();
	test_2d_zwei<yggr::s32>();

	test_3d<yggr::f32>();
	test_3d<yggr::s32>();

	test_3d_zwei<yggr::f32>();
	test_3d_zwei<yggr::s32>();

	test_serialize();

	//test_bug<yggr::s32>();

	wait_any_key(argc, argv);
	return 0;
}
