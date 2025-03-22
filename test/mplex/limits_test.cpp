// limits_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/mplex/limits.hpp>

#include <boost/limits.hpp>

#include <cassert>
#include <typeinfo>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

template<typename R, typename T>
const R& native_cast(const T& t)
{
	return *reinterpret_cast<const R*>(&t);
}

template<typename T>
void test_numeric_limits(void)
{
	typedef std::numeric_limits<T> std_lim_type;
	typedef yggr::mplex::numeric_limits<T> yggr_lim_type;

	yggr_test_assert(std_lim_type::is_specialized == yggr_lim_type::is_specialized_type::type::value);

	yggr_test_assert(std_lim_type::digits == yggr_lim_type::digits_type::type::value);
	yggr_test_assert(std_lim_type::digits10 == yggr_lim_type::digits10_type::type::value);
	yggr_test_assert(std_lim_type::is_signed == yggr_lim_type::is_signed_type::type::value);
	yggr_test_assert(std_lim_type::is_integer == yggr_lim_type::is_integer_type::type::value);
	yggr_test_assert(std_lim_type::is_exact == yggr_lim_type::is_exact_type::type::value);
	yggr_test_assert(std_lim_type::radix == yggr_lim_type::radix_type::type::value);

	yggr_test_assert(std_lim_type::min_exponent == yggr_lim_type::min_exponent_type::type::value);
	yggr_test_assert(std_lim_type::min_exponent10 == yggr_lim_type::min_exponent10_type::type::value);
	yggr_test_assert(std_lim_type::max_exponent == yggr_lim_type::max_exponent_type::type::value);
	yggr_test_assert(std_lim_type::max_exponent10 == yggr_lim_type::max_exponent10_type::type::value);

	yggr_test_assert(std_lim_type::has_infinity == yggr_lim_type::has_infinity_type::type::value);
	yggr_test_assert(std_lim_type::has_quiet_NaN == yggr_lim_type::has_quiet_NaN_type::type::value);
	yggr_test_assert(std_lim_type::has_signaling_NaN == yggr_lim_type::has_signaling_NaN_type::type::value);
	yggr_test_assert(std_lim_type::has_denorm == yggr_lim_type::has_denorm_type::type::value);
	yggr_test_assert(std_lim_type::has_denorm_loss == yggr_lim_type::has_denorm_loss_type::type::value);

	yggr_test_assert(std_lim_type::is_iec559 == yggr_lim_type::is_iec559_type::type::value);
	yggr_test_assert(std_lim_type::is_bounded == yggr_lim_type::is_bounded_type::type::value);
	yggr_test_assert(std_lim_type::is_modulo == yggr_lim_type::is_modulo_type::type::value);

	yggr_test_assert(std_lim_type::traps == yggr_lim_type::traps_type::type::value);
	yggr_test_assert(std_lim_type::tinyness_before == yggr_lim_type::tinyness_before_type::type::value);
	yggr_test_assert(std_lim_type::round_style == yggr_lim_type::round_style_type::type::value);

	yggr_test_assert((std_lim_type::min)() == native_cast<T>(yggr_lim_type::min_type::type::value));
	yggr_test_assert((std_lim_type::max)() == native_cast<T>(yggr_lim_type::max_type::type::value));
#if defined(_MSC_VER) || (YGGR_CPP_VER_11 <= YGGR_CPP_VERSION)
	yggr_test_assert(std_lim_type::lowest() == native_cast<T>(yggr_lim_type::lowest_type::type::value));
#endif // #if defined(_MSC_VER) || (YGGR_CPP_VER_11 <= YGGR_CPP_VERSION)

	yggr_test_assert(std_lim_type::epsilon() == native_cast<T>(yggr_lim_type::epsilon_type::type::value));
	yggr_test_assert(std_lim_type::round_error() == native_cast<T>(yggr_lim_type::round_error_type::type::value));

	std::cout << std_lim_type::infinity() << ",\t"
				<< yggr::mplex::get_word<T>(typename yggr_lim_type::infinity_type::type()) << std::endl;

	std::cout << std_lim_type::quiet_NaN() << ",\t"
				<< yggr::mplex::get_word<T>(typename yggr_lim_type::quiet_NaN_type::type()) << std::endl;

	std::cout << std_lim_type::signaling_NaN() << ",\t"
				<< yggr::mplex::get_word<T>(typename yggr_lim_type::signaling_NaN_type::type()) << std::endl;

	std::cout << std_lim_type::denorm_min() << ",\t"
				<< native_cast<T>(yggr_lim_type::min_type::type::value) << ",\t"
				<< native_cast<T>(yggr_lim_type::denorm_min_type::type::value) << std::endl;

	std::cout << "-----------test " << typeid(T).name() << " succeed-------------" << std::endl;
}

#ifndef YGGR_NO_CWCHAR

template<typename T>
void test_numeric_limits_wchar_t(void)
{
	typedef std::numeric_limits<T> std_lim_type;
	typedef yggr::mplex::numeric_limits<T> yggr_lim_type;

	yggr_test_assert(std_lim_type::is_specialized == yggr_lim_type::is_specialized_type::type::value);

	yggr_test_assert(std_lim_type::digits == yggr_lim_type::digits_type::type::value);
	yggr_test_assert(std_lim_type::digits10 == yggr_lim_type::digits10_type::type::value);
	yggr_test_assert(std_lim_type::is_signed == yggr_lim_type::is_signed_type::type::value);
	yggr_test_assert(std_lim_type::is_integer == yggr_lim_type::is_integer_type::type::value);
	yggr_test_assert(std_lim_type::is_exact == yggr_lim_type::is_exact_type::type::value);
	yggr_test_assert(std_lim_type::radix == yggr_lim_type::radix_type::type::value);

	yggr_test_assert(std_lim_type::min_exponent == yggr_lim_type::min_exponent_type::type::value);
	yggr_test_assert(std_lim_type::min_exponent10 == yggr_lim_type::min_exponent10_type::type::value);
	yggr_test_assert(std_lim_type::max_exponent == yggr_lim_type::max_exponent_type::type::value);
	yggr_test_assert(std_lim_type::max_exponent10 == yggr_lim_type::max_exponent10_type::type::value);

	yggr_test_assert(std_lim_type::has_infinity == yggr_lim_type::has_infinity_type::type::value);
	yggr_test_assert(std_lim_type::has_quiet_NaN == yggr_lim_type::has_quiet_NaN_type::type::value);
	yggr_test_assert(std_lim_type::has_signaling_NaN == yggr_lim_type::has_signaling_NaN_type::type::value);
	yggr_test_assert(std_lim_type::has_denorm == yggr_lim_type::has_denorm_type::type::value);
	yggr_test_assert(std_lim_type::has_denorm_loss == yggr_lim_type::has_denorm_loss_type::type::value);

	yggr_test_assert(std_lim_type::is_iec559 == yggr_lim_type::is_iec559_type::type::value);
	yggr_test_assert(std_lim_type::is_bounded == yggr_lim_type::is_bounded_type::type::value);
	yggr_test_assert(std_lim_type::is_modulo == yggr_lim_type::is_modulo_type::type::value);

	yggr_test_assert(std_lim_type::traps == yggr_lim_type::traps_type::type::value);
	yggr_test_assert(std_lim_type::tinyness_before == yggr_lim_type::tinyness_before_type::type::value);
	yggr_test_assert(std_lim_type::round_style == yggr_lim_type::round_style_type::type::value);

	yggr_test_assert((std_lim_type::min)() == native_cast<T>(yggr_lim_type::min_type::type::value));
	yggr_test_assert((std_lim_type::max)() == native_cast<T>(yggr_lim_type::max_type::type::value));
#if defined(_MSC_VER) || (YGGR_CPP_VER_11 <= YGGR_CPP_VERSION)
	yggr_test_assert(std_lim_type::lowest() == native_cast<T>(yggr_lim_type::lowest_type::type::value));
#endif // #if defined(_MSC_VER) || (YGGR_CPP_VER_11 <= YGGR_CPP_VERSION)

	yggr_test_assert(std_lim_type::epsilon() == native_cast<T>(yggr_lim_type::epsilon_type::type::value));
	yggr_test_assert(std_lim_type::round_error() == native_cast<T>(yggr_lim_type::round_error_type::type::value));

	std::wcout << std_lim_type::infinity() << ",\t"
				<< yggr::mplex::get_word<T>(typename yggr_lim_type::infinity_type::type()) << std::endl;

	std::wcout << std_lim_type::quiet_NaN() << ",\t"
				<< yggr::mplex::get_word<T>(typename yggr_lim_type::quiet_NaN_type::type()) << std::endl;

	std::wcout << std_lim_type::signaling_NaN() << ",\t"
				<< yggr::mplex::get_word<T>(typename yggr_lim_type::signaling_NaN_type::type()) << std::endl;

	std::wcout << std_lim_type::denorm_min() << ",\t"
				<< native_cast<T>(yggr_lim_type::min_type::type::value) << ",\t"
				<< native_cast<T>(yggr_lim_type::denorm_min_type::type::value) << std::endl;

	std::wcout << "-----------test " << typeid(T).name() << " succeed-------------" << std::endl;
}

#endif // YGGR_NO_CWCHAR

int main(int argc, char* argv[])
{
    TEST_PP_WIO_IMBUE("chs");

	test_numeric_limits<char>();
	test_numeric_limits<yggr::s8>();
	test_numeric_limits<yggr::u8>();

#ifndef YGGR_NO_CWCHAR
	test_numeric_limits_wchar_t<wchar_t>();
#endif // YGGR_NO_CWCHAR

	test_numeric_limits<yggr::s16>();
	test_numeric_limits<yggr::u16>();

	test_numeric_limits<yggr::sint>();
	test_numeric_limits<yggr::uint>();

	test_numeric_limits<yggr::s32>();
	test_numeric_limits<yggr::u32>();

	test_numeric_limits<yggr::s64>();
	test_numeric_limits<yggr::u64>();

	test_numeric_limits<yggr::f32>();
	test_numeric_limits<yggr::f64>();

	wait_any_key(argc, argv);
	return 0;
}
