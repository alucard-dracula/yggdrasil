//vector_hash_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <boost/unordered_set.hpp>

#include <yggr/math/vector2d.hpp>
#include <yggr/math/vector3d.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

void test_vector2d_hash(void)
{
	typedef yggr::math::vector2d<float> vec_type;
	typedef boost::unordered_set<vec_type> vec_set_type;

	vec_type v1(1, 2);
	vec_type v2(3, 4);

	vec_set_type vec_set;

	bool bop = false;
	bop = vec_set.insert(v1).second;
	yggr_test_assert(bop);
	bop = vec_set.insert(v2).second;
	yggr_test_assert(bop);

	bop = vec_set.insert(v1).second;
	yggr_test_assert(!bop);

	yggr_test_assert(vec_set.size() == 2);

	std::cout << "test vector2d hash completed" << std::endl;
}

void test_vector3d_hash(void)
{
	typedef yggr::math::vector3d<float> vec_type;
	typedef boost::unordered_set<vec_type> vec_set_type;

	vec_type v1(1, 2, 3);
	vec_type v2(3, 4, 5);

	vec_set_type vec_set;

	bool bop = false;
	bop = vec_set.insert(v1).second;
	yggr_test_assert(bop);
	bop = vec_set.insert(v2).second;
	yggr_test_assert(bop);

	bop = vec_set.insert(v1).second;
	yggr_test_assert(!bop);

	yggr_test_assert(vec_set.size() == 2);

	std::cout << "test vector3d hash completed" << std::endl;
}

int main(int argc, char* argv[])
{
	test_vector2d_hash();
	test_vector3d_hash();

	wait_any_key(argc, argv);
	return 0;
}