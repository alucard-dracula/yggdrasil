//elo_rating_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>
#include <yggr/math/elo_rating.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

void test_fast(void)
{
	typedef yggr::math::fast_elo_rating rating_type;
	yggr_test_assert( 208 == rating_type::s_rating(1.0f, 200, 0) ); // test_static
	yggr_test_assert( 208 == rating_type()(1.0f, 200, 0)); // test_object

	yggr_test_assert( 24 == rating_type::s_rating(1.0f, 0, 200) ); // test_static
	yggr_test_assert( 24 == rating_type()(1.0f, 0, 200)); // test_object

	yggr_test_assert( 208 == rating_type::s_rating(0.5f, 200, 0) ); // test_static
	yggr_test_assert( 24 == rating_type::s_rating(0.5f, 0, 200) ); // test_static
	std::cout << "test_fast end" << std::endl;
}


void test_normal(void)
{
	typedef yggr::math::elo_rating rating_type;

	yggr_test_assert( 204 == rating_type::s_rating(1.0f, yggr::s32(rating_type::E_profession_k), 200, 0) );
	yggr_test_assert( 196 == rating_type::s_rating(0.5f, yggr::s32(rating_type::E_profession_k), 200, 0) );
	yggr_test_assert( 188 == rating_type::s_rating(0.0f, yggr::s32(rating_type::E_profession_k), 200, 0) );

	yggr_test_assert( 204 == rating_type()(1.0f, yggr::s32(rating_type::E_profession_k), 200, 0) );
	yggr_test_assert( 196 == rating_type()(0.5f, yggr::s32(rating_type::E_profession_k), 200, 0) );
	yggr_test_assert( 188 == rating_type()(0.0f, yggr::s32(rating_type::E_profession_k), 200, 0) );

	yggr_test_assert( 1627 == rating_type::s_rating(1.0f, yggr::s32(rating_type::E_normal_k), 1613, 1573) );
	yggr_test_assert( 1611 == rating_type::s_rating(0.5f, yggr::s32(rating_type::E_normal_k), 1613, 1573) );
	yggr_test_assert( 1595 == rating_type::s_rating(0.0f, yggr::s32(rating_type::E_normal_k), 1613, 1573) );

	yggr_test_assert( 1627 == rating_type()(1.0f, yggr::s32(rating_type::E_normal_k), 1613, 1573) );
	yggr_test_assert( 1611 == rating_type()(0.5f, yggr::s32(rating_type::E_normal_k), 1613, 1573) );
	yggr_test_assert( 1595 == rating_type()(0.0f, yggr::s32(rating_type::E_normal_k), 1613, 1573) );

	std::cout << "test_normal end" << std::endl;
}

int main(int argc, char* argv[])
{
	test_fast();
	test_normal();

	wait_any_key(argc, argv);
	return 0;
}
