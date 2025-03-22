//any_operator_mgr_basic_type_mix_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <yggr/any_val/any_operator_mgr_basic_t_reg.hpp>
#include <yggr/any_val/any_operator_mgr_boost_basic_t_reg.hpp>

#include <yggr/any_val/any_set.hpp>
#include <yggr/charset/utf8_string.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#include YGGR_PP_LINK_LIB(any_val)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

#if defined(__GNUC__) && (!defined(__x86_64__) || (__x86_64__ == 0))

#define TEST_YGGR_ANY
//#define TEST_BOOST_ANY // 32bit ld out of memory fix

#else

#define TEST_YGGR_ANY
#define TEST_BOOST_ANY

#endif // #if defined(__GNUC__) && (!defined(__x86_64__) || (__x86_64__ == 0))

#if defined(TEST_YGGR_ANY)

void test_yggr_any_plus(void)
{
	std::cout << "----------test_yggr_plus S---------------" << std::endl;
	yggr::any a1(ANY_SET(yggr::s32, 10));
	yggr::any a2(ANY_SET(yggr::s32, 20));

	yggr::any a3 = a1 + a2;
	std::cout << yggr::any_cast<yggr::s32>(a3) << std::endl;

	yggr_test_assert(yggr::any_cast<yggr::s32>(a3) == 10 + 20);
	std::cout << "----------test_yggr_plus E---------------" << std::endl;
}

#endif // TEST_YGGR_ANY

#if defined(TEST_BOOST_ANY)

void test_boost_any_plus(void)
{
	std::cout << "----------test_boost_plus S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 10));
	boost::any a2(ANY_SET(yggr::s32, 20));

	boost::any a3 = a1 + a2;
	std::cout << yggr::any_cast<yggr::s32>(a3) << std::endl;

	yggr_test_assert(yggr::any_cast<yggr::s32>(a3) == 10 + 20);
	std::cout << "----------test_boost_plus E---------------" << std::endl;
}

#endif // TEST_BOOST_ANY

int main(int argc, char* argv[])
{
	std::cout << "start init ..." << std::endl;

#if defined(TEST_YGGR_ANY)
    bool bchk_yggr = yggr::any_val::any_operator_mgr_basic_t_reg::s_init(true);
	yggr_test_assert(bchk_yggr);
#endif // TEST_YGGR_ANY

#if defined(TEST_BOOST_ANY)
    bool bchk_boost = yggr::any_val::any_operator_mgr_boost_basic_t_reg::s_init(true);
	yggr_test_assert(bchk_boost);
#endif // TEST_BOOST_ANY


	std::cout << "end init" << std::endl;

#if defined(TEST_YGGR_ANY)
	test_yggr_any_plus();
#endif // TEST_YGGR_ANY

#if defined(TEST_BOOST_ANY)
	test_boost_any_plus();
#endif // TEST_BOOST_ANY

	wait_any_key(argc, argv);
	return 0;
}
