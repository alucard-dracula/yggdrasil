//matrix_test.cpp

#include <iostream>

#include <yggr/math/array_indexes_permutation_parity.hpp>

#ifdef _MSC_VER
#include <vld.h>
#endif // _MSC_VER

void test_array_indexes_permutation_parity(void)
{
	std::cout << (yggr::math::array_indexes_permutation_parity<0, 3>::value) << std::endl; // 0
	std::cout << (yggr::math::array_indexes_permutation_parity<1, 3>::value) << std::endl; // 2
	std::cout << (yggr::math::array_indexes_permutation_parity<2, 3>::value) << std::endl; // 2

	std::cout << "------------------------------------------------------------" << std::endl;

	std::cout << (yggr::math::array_indexes_permutation_parity<2, 3, false>::value) << std::endl; // 0
	std::cout << (yggr::math::array_indexes_permutation_parity<1, 3, false>::value) << std::endl; // 2
	std::cout << (yggr::math::array_indexes_permutation_parity<0, 3, false>::value) << std::endl; // 2

	std::cout << "------------------------------------------------------------" << std::endl;

	std::cout << (yggr::math::array_indexes_permutation_parity<0, 4>::value) << std::endl; // 0
	std::cout << (yggr::math::array_indexes_permutation_parity<1, 4>::value) << std::endl; // 3
	std::cout << (yggr::math::array_indexes_permutation_parity<2, 4>::value) << std::endl; // 4
	std::cout << (yggr::math::array_indexes_permutation_parity<3, 4>::value) << std::endl; // 3

	std::cout << "------------------------------------------------------------" << std::endl;

	std::cout << (yggr::math::array_indexes_permutation_parity<3, 4, false>::value) << std::endl; // 0
	std::cout << (yggr::math::array_indexes_permutation_parity<2, 4, false>::value) << std::endl; // 3
	std::cout << (yggr::math::array_indexes_permutation_parity<1, 4, false>::value) << std::endl; // 4
	std::cout << (yggr::math::array_indexes_permutation_parity<0, 4, false>::value) << std::endl; // 3

	std::cout << "------------------------------------------------------------" << std::endl;

	std::cout << (yggr::math::array_indexes_permutation_parity<0, 5>::value) << std::endl;
	std::cout << (yggr::math::array_indexes_permutation_parity<1, 5>::value) << std::endl;
	std::cout << (yggr::math::array_indexes_permutation_parity<2, 5>::value) << std::endl;
	std::cout << (yggr::math::array_indexes_permutation_parity<3, 5>::value) << std::endl;
	std::cout << (yggr::math::array_indexes_permutation_parity<4, 5>::value) << std::endl;

	std::cout << "------------------------------------------------------------" << std::endl;

	std::cout << (yggr::math::array_indexes_permutation_parity<4, 5>::value) << std::endl;
	std::cout << (yggr::math::array_indexes_permutation_parity<3, 5>::value) << std::endl;
	std::cout << (yggr::math::array_indexes_permutation_parity<2, 5>::value) << std::endl;
	std::cout << (yggr::math::array_indexes_permutation_parity<1, 5>::value) << std::endl;
	std::cout << (yggr::math::array_indexes_permutation_parity<0, 5>::value) << std::endl;

	std::cout << "------------------------------------------------------------" << std::endl;

	std::cout << (yggr::math::array_indexes_permutation_parity<7, 3>::value) << std::endl; // 0
 	std::cout << (yggr::math::array_indexes_permutation_parity<8, 3>::value) << std::endl; // 0
	std::cout << (yggr::math::array_indexes_permutation_parity<9, 3>::value) << std::endl; // 0
}

#include <yggr/math/determinant.hpp>
#include <boost/array.hpp>
void test_determinant(void)
{
	{
		yggr::math::determinant<yggr::s32, 2, 2> d;
		d[0][0] = 1;
		d[1][1] = 1;
		std::cout << d.det() << std::endl;
		//d.zero();
		std::cout << d.is_zero() << std::endl;
	}

	{
		//yggr::s32 t[3][3] = {	{2, 6, 3},{1, 0, 2},{5, 8, 4}};
		boost::array<yggr::s32, 9> arr = {
											2, 6, 3,
											1, 0, 2,
											5, 8, 4
										};
		yggr::math::determinant<yggr::s32, 3, 3> d = arr;

		std::cout << d.det() << std::endl;
		//d.zero();
		std::cout << d.is_zero() << std::endl;
	}

	{
		yggr::math::determinant<yggr::s32, 4, 4> d;
		d[0][0] = 1;
		d[1][1] = 1;
		d[2][2] = 1;
		d[3][3] = 1;
		std::cout << d.det() << std::endl;
		//d.zero();
		std::cout << d.is_zero() << std::endl;
	}

	//compile err
	//{ yggr::math::determinant<yggr::s32, 0, 0> determinant_type d; }
	//{ yggr::math::determinant<yggr::s32, 0, 1> determinant_type d; }
	//{ yggr::math::determinant<yggr::s32, 1, 0> determinant_type d; }
	//{ yggr::math::determinant<yggr::s32, 1, 1> determinant_type d; }
	//{ yggr::math::determinant<yggr::s32, 0, 2> determinant_type d; }
	//{ yggr::math::determinant<yggr::s32, 2, 0> determinant_type d; }
	//{ yggr::math::determinant<yggr::s32, 1, 2> determinant_type d; }
	//{ yggr::math::determinant<yggr::s32, 2, 1> determinant_type d; }
}

#include <yggr/math/matrix2d.hpp>
#include <yggr/math/vector2d.hpp>

void matrix2d_test(void)
{
	typedef yggr::math::matrix2d<yggr::s32> mat_type;
	/*boost::array<yggr::s32, 3 * 3> arr = {	1, 2, -1,
											3, 4, -2,
											5, -4, 1
										};*/

	//inverse
	//{
	//	-2,	  1,  0
	//	-6.5, 5, -0.5
	// -16,   7, -1
	//}

	boost::array<yggr::s32, 3 * 3> arr = {	1, 2, -1,
											0, 0, 0,
											5, -4, 1
										}; // non_inverse



	typedef yggr::math::vector2d<yggr::s32> pos_type;

	yggr::math::matrix2d<yggr::s32> mat(arr), mat2(0), mat3;


	std::cout << mat << std::endl;
	std::cout << mat.transpose() << std::endl;
	std::cout << "-----------------------" << std::endl;
	std::cout << mat.adjugate() << std::endl;
	std::cout << "-----------------------" << std::endl;
	std::cout << mat.inverse() << std::endl;
	std::cout << "-----------------------" << std::endl;
	std::cout << mat * mat2 << std::endl;
	std::cout << "-----------------------" << std::endl;
	std::cout << mat * mat3 << std::endl;
	std::cout << "-----------------------" << std::endl;
	std::cout << mat * mat.inverse() << std::endl;
	std::cout << "-----------------------" << std::endl;
	std::cout << mat2 * mat.inverse() << std::endl;
	std::cout << "-----------------------" << std::endl;
	std::cout << mat3 * mat.inverse() << std::endl;
	std::cout << "-----------------------" << std::endl;

	pos_type pos(1, 0);
	//mat = mat_type::make_reflect(pos_type(0, -1000), pos_type(0, 1000));
	mat = mat_type::make_translate(10, 20);
	std::cout << mat << std::endl;
	std::cout << pos * mat << std::endl;

	std::cout << mat_type::make_reflect(pos_type(-10, 0), pos_type(10, 0)) << std::endl;
	std::cout << mat_type::make_reflect(pos_type(1, 1), pos_type(2, 2)) << std::endl;
}

#include <yggr/math/matrix3d.hpp>
#include <yggr/math/vector3d.hpp>
void matrix3d_test(void)
{
	typedef yggr::math::matrix3d<yggr::s32> mat_type;
	boost::array<yggr::s32, 4 * 4> arr = {	1, -2, -1, -2,
											2, -1, 4, 5,
											4, 1, 2, 1,
											1, -1, 1, 1
										}; // non_inverse


	typedef yggr::math::vector3d<yggr::s32> pos_type;

	yggr::math::matrix3d<yggr::s32> mat(arr), mat2(0), mat3;


	std::cout << mat << std::endl;
	std::cout << mat.transpose() << std::endl;
	std::cout << mat * mat3 << std::endl;
	std::cout << "-----------------------" << std::endl;
	std::cout << mat.adjugate() << std::endl;
	std::cout << "-----------------------" << std::endl;
	std::cout << mat.inverse() << std::endl;

	std::cout << "-----------------------" << std::endl;
	std::cout << mat * mat2 << std::endl;
	std::cout << "-----------------------" << std::endl;
	std::cout << mat * mat3 << std::endl;
	std::cout << "-----------------------" << std::endl;
	std::cout << mat.inverse() * mat << std::endl;
	std::cout << "-----------------------" << std::endl;
	std::cout << mat.inverse() * mat2 << std::endl;
	std::cout << "-----------------------" << std::endl;
	std::cout << mat.inverse() * mat3 << std::endl;
	std::cout << "-----------------------" << std::endl;

	pos_type pos(1, 0, 0);
	//mat = mat_type::make_reflect(pos_type(0, 0, 0), pos_type(0, 1, 0), pos_type(0, 0, 1));
	mat = mat_type::make_translate(10, 20, 30);
	std::cout << mat << std::endl;
	std::cout << pos * mat << std::endl;

	std::cout << mat_type::make_reflect(pos_type(0, 0, 0), pos_type(0, 1, 0), pos_type(0, 2, 0)) << std::endl; //pyz
	std::cout << mat_type::make_reflect(pos_type(-10, 0, 0), pos_type(10, 0, 0), pos_type(0, 0, 1000)) << std::endl; // pxz
	std::cout << mat_type::make_reflect(pos_type(0, 0, 0), pos_type(1, 1, 1), pos_type(1, 0, 1)) << std::endl;

#ifndef __MINGW32__
	std::wcout.imbue(std::locale("chs"));
	std::wcout << mat_type::make_reflect(pos_type(0, 0, 0), pos_type(1, 1, 1), pos_type(1, 0, 1)) << std::endl;
#endif // __MINGE32__
}

int main(int argc, char* argv[])
{
	test_array_indexes_permutation_parity();
	test_determinant();

	matrix2d_test();
	matrix3d_test();

	char cc = 0;
	std::cin >> cc;

	return 0;
}
