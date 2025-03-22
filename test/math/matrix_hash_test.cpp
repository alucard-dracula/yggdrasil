//vector_hash_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <boost/unordered_set.hpp>

#include <yggr/math/matrix2d.hpp>
#include <yggr/math/matrix3d.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

void test_det2x2_hash(void)
{
	typedef yggr::math::determinant<float, 2, 2> det_type;
	typedef boost::unordered_set<det_type> det_set_type;
	typedef boost::array<float, 2 * 2> init_type;


	init_type v1 = {1, 2, 3, 4};
	init_type v2 = {5, 6, 7, 8};

	det_set_type det_set;

	det_type d1(v1);
	det_type d2(v2);

	bool bop = false;
	bop = det_set.insert(d1).second;
	yggr_test_assert(bop);
	bop = det_set.insert(d2).second;
	yggr_test_assert(bop);

	bop = det_set.insert(d1).second;
	yggr_test_assert(!bop);

	yggr_test_assert(det_set.size() == 2);

	std::cout << "test det2x2 hash completed" << std::endl;
}

void test_det3x3_hash(void)
{
	typedef yggr::math::determinant<float, 3, 3> det_type;
	typedef boost::unordered_set<det_type> det_set_type;
	typedef boost::array<float, 3 * 3> init_type;


	init_type v1 = {1, 2, 3, 4};
	init_type v2 = {5, 6, 7, 8};

	det_set_type det_set;

	det_type d1(v1);
	det_type d2(v2);

	bool bop = false;
	bop = det_set.insert(d1).second;
	yggr_test_assert(bop);
	bop = det_set.insert(d2).second;
	yggr_test_assert(bop);

	bop = det_set.insert(d1).second;
	yggr_test_assert(!bop);

	yggr_test_assert(det_set.size() == 2);

	std::cout << "test det3x3 hash completed" << std::endl;
}

void test_det4x4_hash(void)
{
	typedef yggr::math::determinant<float, 4, 4> det_type;
	typedef boost::unordered_set<det_type> det_set_type;
	typedef boost::array<float, 4 * 4> init_type;


	init_type v1 = {1, 2, 3, 4};
	init_type v2 = {5, 6, 7, 8};

	det_set_type det_set;

	det_type d1(v1);
	det_type d2(v2);

	bool bop = false;
	bop = det_set.insert(d1).second;
	yggr_test_assert(bop);
	bop = det_set.insert(d2).second;
	yggr_test_assert(bop);

	bop = det_set.insert(d1).second;
	yggr_test_assert(!bop);

	yggr_test_assert(det_set.size() == 2);

	std::cout << "test det4x4 hash completed" << std::endl;
}

void test_matrix2d_hash(void)
{
	typedef yggr::math::matrix2d<float> mat_type;
	typedef boost::unordered_set<mat_type> mat_set_type;
	typedef boost::array<float, 3 * 3> init_type;


	init_type v1 = {1, 2, 3, 4};
	init_type v2 = {5, 6, 7, 8};

	mat_set_type mat_set;

	mat_type m1(v1);
	mat_type m2(v2);

	bool bop = false;
	bop = mat_set.insert(m1).second;
	yggr_test_assert(bop);
	bop = mat_set.insert(m2).second;
	yggr_test_assert(bop);

	bop = mat_set.insert(m1).second;
	yggr_test_assert(!bop);

	yggr_test_assert(mat_set.size() == 2);

	std::cout << "test matrix2d hash completed" << std::endl;
}

void test_matrix3d_hash(void)
{
	typedef yggr::math::matrix3d<float> mat_type;
	typedef boost::unordered_set<mat_type> mat_set_type;
	typedef boost::array<float, 4 * 4> init_type;


	init_type v1 = {1, 2, 3, 4};
	init_type v2 = {5, 6, 7, 8};

	mat_set_type mat_set;

	mat_type m1(v1);
	mat_type m2(v2);

	bool bop = false;
	bop = mat_set.insert(m1).second;
	yggr_test_assert(bop);
	bop = mat_set.insert(m2).second;
	yggr_test_assert(bop);

	bop = mat_set.insert(m1).second;
	yggr_test_assert(!bop);

	yggr_test_assert(mat_set.size() == 2);

	std::cout << "test matrix3d hash completed" << std::endl;
}

int main(int argc, char* argv[])
{
	test_det2x2_hash();
	test_det3x3_hash();
	test_det4x4_hash();

	test_matrix2d_hash();
	test_matrix3d_hash();

	wait_any_key(argc, argv);
	return 0;
}