//elo_rating_test.cpp

#include <iostream>
#include <cassert>
#include <yggr/math/elo_rating.hpp>

void test_fast(void)
{
	typedef yggr::math::fast_elo_rating rating_type;
	assert( 208 == rating_type::s_rating(1.0f, 200, 0) ); // test_static
	assert( 208 == rating_type()(1.0f, 200, 0)); // test_object

	assert( 24 == rating_type::s_rating(1.0f, 0, 200) ); // test_static
	assert( 24 == rating_type()(1.0f, 0, 200)); // test_object

	assert( 208 == rating_type::s_rating(0.5f, 200, 0) ); // test_static
	assert( 24 == rating_type::s_rating(0.5f, 0, 200) ); // test_static
	std::cout << "test_fast end" << std::endl;
}


void test_normal(void)
{
	typedef yggr::math::elo_rating rating_type;

	assert( 204 == rating_type::s_rating(1.0f, yggr::s32(rating_type::E_profession_k), 200, 0) );
	assert( 196 == rating_type::s_rating(0.5f, yggr::s32(rating_type::E_profession_k), 200, 0) );
	assert( 188 == rating_type::s_rating(0.0f, yggr::s32(rating_type::E_profession_k), 200, 0) );

	assert( 204 == rating_type()(1.0f, yggr::s32(rating_type::E_profession_k), 200, 0) );
	assert( 196 == rating_type()(0.5f, yggr::s32(rating_type::E_profession_k), 200, 0) );
	assert( 188 == rating_type()(0.0f, yggr::s32(rating_type::E_profession_k), 200, 0) );

	assert( 1627 == rating_type::s_rating(1.0f, yggr::s32(rating_type::E_normal_k), 1613, 1573) );
	assert( 1611 == rating_type::s_rating(0.5f, yggr::s32(rating_type::E_normal_k), 1613, 1573) );
	assert( 1595 == rating_type::s_rating(0.0f, yggr::s32(rating_type::E_normal_k), 1613, 1573) );

	assert( 1627 == rating_type()(1.0f, yggr::s32(rating_type::E_normal_k), 1613, 1573) );
	assert( 1611 == rating_type()(0.5f, yggr::s32(rating_type::E_normal_k), 1613, 1573) );
	assert( 1595 == rating_type()(0.0f, yggr::s32(rating_type::E_normal_k), 1613, 1573) );

	std::cout << "test_normal end" << std::endl;
}

int main(int argc, char* argv[])
{
	test_fast();
	test_normal();

	char cc = 0;
	std::cin >> cc;
	return 0;
}
