// operator_result_test.cpp

#include <cassert>
#include <test/wait_any_key/wait_any_key.hpp>
#include <typeinfo>

//#include <yggr/ppex/pp_debug.hpp>
#include <yggr/type_traits/operator_result.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

//---------------math--------------------------
void test_plus_operator_result(void)
{
	int a = 10, b =20;
	yggr::plus_operator_result<int, int>::type rst = a + b;

	yggr_test_assert(typeid(yggr::plus_operator_result<int, int>::type) == typeid(a + b));
	yggr_test_assert(rst == a + b);

	std::cout << "test_plus_operator_result completed" << std::endl;
}

void test_minus_operator_result(void)
{
	int a = 10, b =20;
	yggr::minus_operator_result<int, int>::type rst = a - b;

	yggr_test_assert(typeid(yggr::minus_operator_result<int, int>::type) == typeid(a - b));
	yggr_test_assert(rst == a - b);

	std::cout << "test_minus_operator_result completed" << std::endl;
}

void test_multiplies_operator_result(void)
{
	int a = 10;
	float b = 20.0f;
	yggr::multiplies_operator_result<int, float>::type rst = a * b;

	yggr_test_assert(typeid(yggr::multiplies_operator_result<int, float>::type) == typeid(a * b));
	yggr_test_assert(rst == a * b);

	std::cout << "test_multiplies_operator_result completed" << std::endl;
}

void test_divides_operator_result(void)
{
	int a = 10;
	float b = 20.0f;
	yggr::divides_operator_result<int, float>::type rst = a / b;

	yggr_test_assert(typeid(yggr::divides_operator_result<int, float>::type) == typeid(a / b));
	yggr_test_assert(rst == a / b);

	std::cout << "test_divides_operator_result completed" << std::endl;
}

void test_modulus_operator_result(void)
{
	int a = 10;
	int b = 20;
	yggr::modulus_operator_result<int, int>::type rst = a % b;

	yggr_test_assert(typeid(yggr::modulus_operator_result<int, int>::type) == typeid(a % b));
	yggr_test_assert(rst == a % b);

	std::cout << "test_modulus_operator_result completed" << std::endl;
}

void test_negate_operator_result(void)
{
	int a = 10;
	yggr::negate_operator_result<int>::type rst = -a;

	yggr_test_assert(typeid(yggr::negate_operator_result<int>::type) == typeid(-a));
	yggr_test_assert(rst == (-a));

	std::cout << "test_negate_operator_result completed" << std::endl;
}

//------------------bit--------------------

void test_bitwise_and_operator_result(void)
{
	int a = 10;
	int b = 20;
	yggr::bitwise_and_operator_result<int, int>::type rst = a & b;

	yggr_test_assert(typeid(yggr::bitwise_and_operator_result<int, int>::type) == typeid(a & b));
	yggr_test_assert(rst == (a & b));

	std::cout << "test_bitwise_and_operator_result completed" << std::endl;
}

void test_bitwise_or_operator_result(void)
{
	int a = 10;
	int b = 20;
	yggr::bitwise_or_operator_result<int, int>::type rst = a | b;

	yggr_test_assert(typeid(yggr::bitwise_or_operator_result<int, int>::type) == typeid(a | b));
	yggr_test_assert(rst == (a | b));

	std::cout << "test_bitwise_or_operator_result completed" << std::endl;
}

void test_bitwise_xor_operator_result(void)
{
	int a = 10;
	int b = 20;
	yggr::bitwise_xor_operator_result<int, int>::type rst = a ^ b;

	yggr_test_assert(typeid(yggr::bitwise_xor_operator_result<int, int>::type) == typeid(a ^ b));
	yggr_test_assert(rst == (a ^ b));

	std::cout << "test_bitwise_xor_operator_result completed" << std::endl;
}

void test_bitwise_left_shift_operator_result(void)
{
	int a = 10;
	int b = 2;
	yggr::bitwise_left_shift_operator_result<int, int>::type rst = a << b;

	yggr_test_assert(typeid(yggr::bitwise_left_shift_operator_result<int, int>::type) == typeid(a << b));
	yggr_test_assert(rst == (a << b));

	std::cout << "test_bitwise_left_shift_operator_result completed" << std::endl;
}

void test_bitwise_right_shift_operator_result(void)
{
	int a = 10;
	int b = 2;
	yggr::bitwise_right_shift_operator_result<int, int>::type rst = a >> b;

	yggr_test_assert(typeid(yggr::bitwise_left_shift_operator_result<int, int>::type) == typeid(a >> b));
	yggr_test_assert(rst == (a >> b));

	std::cout << "test_bitwise_right_shift_operator_result completed" << std::endl;
}

void test_bitwise_not_operator_result(void)
{
	int a = 10;
	yggr::bitwise_not_operator_result<int>::type rst = ~a;

	yggr_test_assert(typeid(yggr::bitwise_not_operator_result<int>::type) == typeid(~a));
	yggr_test_assert(rst == (~a));

	std::cout << "test_bitwise_not_operator_result completed" << std::endl;
}

int main(int argc, char* argv[])
{
	test_plus_operator_result();
	test_minus_operator_result();
	test_multiplies_operator_result();
	test_divides_operator_result();
	test_modulus_operator_result();
	test_negate_operator_result();

	test_bitwise_and_operator_result();
	test_bitwise_or_operator_result();
	test_bitwise_xor_operator_result();
	test_bitwise_left_shift_operator_result();
	test_bitwise_right_shift_operator_result();
	test_bitwise_not_operator_result();

	wait_any_key(argc, argv);
	return 0;
}
