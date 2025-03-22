// params_test_of_yggr_any.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <string>

#include <boost/bind.hpp>

#include <yggr/any_val/params.hpp>
#include <yggr/any_val/any_operator_mgr_basic_t_reg.hpp>
#include <yggr/charset/utf8_string.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(any_val)
#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

typedef yggr::any_val::params<std::string> params_type;

class A
{
public:

	A(void)
		: n(0)
	{
	}

	void from_params(const params_type& params)
	{
		try
		{
			n = params.get_value<yggr::u32>("n");
			str = params.get_value<std::string>("str");
		}
		catch(const params_type::error_type&)
		{
			yggr_test_assert(false);
		}
	}

	params_type to_params(void) const
	{
		params_type rst;
		rst["n"] = ANY_SET(yggr::u32, n);
		rst["str"] = ANY_SET(std::string, str);

		return rst;
	}

	bool operator==(const A& right) const
	{
		return n == right.n && str == right.str;
	}

	yggr::u32 n;
	std::string str;
};


const params_type& set_value(params_type& param, const std::string& name, yggr::u32 val)
{
	param[name] = ANY_SET(yggr::u32, val);
	return param;
}

void test_yggr_params_init(void)
{
	std::cout << "-------test_yggr_params_init S---------" << std::endl;

	params_type param1, param2, param3, param4;
	param1["aaa"] = ANY_SET(yggr::u32, 2);
	param1["bbb"] = ANY_SET(std::string, "Hello ");

	param2["aaa"] = ANY_SET(yggr::u32, 3);
	param2["bbb"] = ANY_SET(std::string, "World");

	yggr_test_assert(!param1.empty_op());
	yggr_test_assert(!param2.empty_op());

	yggr_test_assert(param1.get_value<yggr::u32>("aaa") == 2);
	yggr_test_assert(param1.get_value<std::string>("bbb") == std::string("Hello "));

	yggr_test_assert(param2.get_value<yggr::u32>("aaa") == 3);
	yggr_test_assert(param2.get_value<std::string>("bbb") == std::string("World"));

	std::cout << "-------test_yggr_params_init E---------" << std::endl;
}

void test_yggr_params_plus(void)
{
	std::cout << "-------test_yggr_params_plus S---------" << std::endl;

	{
		params_type param1, param2, param3, param4;
		param1["aaa"] = ANY_SET(yggr::u32, 2);
		param1["bbb"] = ANY_SET(std::string, "Hello ");

		param2["aaa"] = ANY_SET(yggr::u32, 3);
		param2["bbb"] = ANY_SET(std::string, "World");

		param3.copy_from(param1 + param2);
		yggr_test_assert(param3.get_value<yggr::u32>("aaa") == (2 + 3));
		yggr_test_assert(param3.get_value<std::string>("bbb") == std::string("Hello ") + std::string("World"));

		std::cout << param3.get_value<yggr::u32>("aaa") << std::endl;
		std::cout << param3.get_value<std::string>("bbb") << std::endl;
	}

	{
		params_type param1, param2, param3, param4;
		param1["aaa"] = ANY_SET(yggr::u32, 2);
		param1["bbb"] = ANY_SET(std::string, "Hello ");

		param2["bbb"] = ANY_SET(std::string, "World");
		param2["ccc"] = ANY_SET(yggr::u32, 3);

		param3.copy_from(param1 + param2);

		yggr_test_assert(param3.empty("aaa"));
		yggr_test_assert(param3.get_value<std::string>("bbb") == std::string("Hello ") + std::string("World"));
		yggr_test_assert(param3.empty("ccc"));

		std::cout << param3.get_value<std::string>("bbb") << std::endl;
	}

	std::cout << "-------test_yggr_params_plus E---------" << std::endl;
}

void test_yggr_params_plus_equal(void)
{
	std::cout << "-------test_yggr_params_plus_equal S---------" << std::endl;

	{
		params_type param1, param2, param3, param4;
		param1["aaa"] = ANY_SET(yggr::u32, 2);
		param1["bbb"] = ANY_SET(std::string, "Hello ");

		param2["aaa"] = ANY_SET(yggr::u32, 3);
		param2["bbb"] = ANY_SET(std::string, "World");

		param3.copy_from(param1 += param2);
		yggr_test_assert(param3.get_value<yggr::u32>("aaa") == (2 + 3));
		yggr_test_assert(param3.get_value<std::string>("bbb") == std::string("Hello ") + std::string("World"));

		std::cout << param3.get_value<yggr::u32>("aaa") << std::endl;
		std::cout << param3.get_value<std::string>("bbb") << std::endl;

		yggr_test_assert(param3 == param1);
	}

	{
		params_type param1, param2, param3, param4;
		param1["aaa"] = ANY_SET(yggr::u32, 2);
		param1["bbb"] = ANY_SET(std::string, "Hello ");

		param2["bbb"] = ANY_SET(std::string, "World");
		param2["ccc"] = ANY_SET(yggr::u32, 3);
		
		param3.copy_from(param1 += param2);
		yggr_test_assert(param3.get_value<yggr::u32>("aaa") == 2);
		yggr_test_assert(param3.get_value<std::string>("bbb") == std::string("Hello ") + std::string("World"));
		yggr_test_assert(param3.empty("ccc"));

		std::cout << param3.get_value<yggr::u32>("aaa") << std::endl;
		std::cout << param3.get_value<std::string>("bbb") << std::endl;

		yggr_test_assert(param3 == param1);
	}

	std::cout << "-------test_yggr_params_plus_equal E---------" << std::endl;
}

void test_yggr_params_mul(void)
{
	std::cout << "-------test_yggr_params_mul S---------" << std::endl;
	params_type param1, param2, param3, param4;
	param1["aaa"] = ANY_SET(yggr::u32, 2);
	param1["bbb"] = ANY_SET(std::string, "Hello ");

	param2["aaa"] = ANY_SET(yggr::u32, 3);
	param2["bbb"] = ANY_SET(std::string, "World");

	param3.copy_from(param1 * param2);
	yggr_test_assert(param3.get_value<yggr::u32>("aaa") == (2 * 3));
	yggr_test_assert(param3.empty("bbb"));

	std::cout << param3.get_value<yggr::u32>("aaa") << std::endl;
	std::cout << param3.type_name("bbb") << std::endl;

	std::cout << "-------test_yggr_params_mul E---------" << std::endl;
}

void test_yggr_params_mul_equal(void)
{
	std::cout << "-------test_yggr_params_mul_equal S---------" << std::endl;
	params_type param1, param2, param3, param4;
	param1["aaa"] = ANY_SET(yggr::u32, 2);
	param1["bbb"] = ANY_SET(std::string, "Hello ");

	param2["aaa"] = ANY_SET(yggr::u32, 3);
	param2["bbb"] = ANY_SET(std::string, "World");

	param3.copy_from(param1 *= param2);
	yggr_test_assert(param3.get_value<yggr::u32>("aaa") == (2 * 3));
	yggr_test_assert(param3.empty("bbb"));

	std::cout << param3.get_value<yggr::u32>("aaa") << std::endl;
	std::cout << param3.type_name("bbb") << std::endl;

	yggr_test_assert(param3 == param1);

	std::cout << "-------test_yggr_params_mul_equal E---------" << std::endl;
}

void test_yggr_params_div(void)
{
	std::cout << "-------test_yggr_params_div S---------" << std::endl;
	params_type param1, param2, param3, param4;
	param1["aaa"] = ANY_SET(yggr::u32, 2);
	param1["bbb"] = ANY_SET(std::string, "Hello ");

	param2["aaa"] = ANY_SET(yggr::u32, 3);
	param2["bbb"] = ANY_SET(std::string, "World");

	param3.copy_from(param1 / param2);
	yggr_test_assert(param3.get_value<yggr::u32>("aaa") == (2 / 3));
	yggr_test_assert(param3.empty("bbb"));

	std::cout << param3.get_value<yggr::u32>("aaa") << std::endl;
	std::cout << param3.type_name("bbb") << std::endl;

	std::cout << "-------test_yggr_params_div E---------" << std::endl;
}

void test_yggr_params_div_equal(void)
{
	std::cout << "-------test_yggr_params_div_equal S---------" << std::endl;
	params_type param1, param2, param3, param4;
	param1["aaa"] = ANY_SET(yggr::u32, 2);
	param1["bbb"] = ANY_SET(std::string, "Hello ");

	param2["aaa"] = ANY_SET(yggr::u32, 3);
	param2["bbb"] = ANY_SET(std::string, "World");

	param3.copy_from(param1 /= param2);
	yggr_test_assert(param3.get_value<yggr::u32>("aaa") == (2 / 3));
	yggr_test_assert(param3.empty("bbb"));

	std::cout << param3.get_value<yggr::u32>("aaa") << std::endl;
	std::cout << param3.type_name("bbb") << std::endl;

	yggr_test_assert(param3 == param1);

	std::cout << "-------test_yggr_params_div_equal E---------" << std::endl;
}

void test_yggr_params_mod(void)
{
	std::cout << "-------test_yggr_params_mod S---------" << std::endl;
	params_type param1, param2, param3, param4;
	param1["aaa"] = ANY_SET(yggr::u32, 2);
	param1["bbb"] = ANY_SET(std::string, "Hello ");

	param2["aaa"] = ANY_SET(yggr::u32, 3);
	param2["bbb"] = ANY_SET(std::string, "World");

	param3.copy_from(param1 % param2);
	yggr_test_assert(param3.get_value<yggr::u32>("aaa") == (2 % 3));
	yggr_test_assert(param3.empty("bbb"));

	std::cout << param3.get_value<yggr::u32>("aaa") << std::endl;
	std::cout << param3.type_name("bbb") << std::endl;

	std::cout << "-------test_yggr_params_mod E---------" << std::endl;
}

void test_yggr_params_mod_equal(void)
{
	std::cout << "-------test_yggr_params_mod_equal S---------" << std::endl;
	params_type param1, param2, param3, param4;
	param1["aaa"] = ANY_SET(yggr::u32, 2);
	param1["bbb"] = ANY_SET(std::string, "Hello ");

	param2["aaa"] = ANY_SET(yggr::u32, 3);
	param2["bbb"] = ANY_SET(std::string, "World");

	param3.copy_from(param1 %= param2);
	yggr_test_assert(param3.get_value<yggr::u32>("aaa") == (2 % 3));
	yggr_test_assert(param3.empty("bbb"));

	std::cout << param3.get_value<yggr::u32>("aaa") << std::endl;
	std::cout << param3.type_name("bbb") << std::endl;

	yggr_test_assert(param3 == param1);

	std::cout << "-------test_yggr_params_mod_equal E---------" << std::endl;
}

void test_yggr_params_bitwise_and(void)
{
	std::cout << "-------test_yggr_params_bitwise_and S---------" << std::endl;
	params_type param1, param2, param3, param4;
	param1["aaa"] = ANY_SET(yggr::u32, 2);
	param1["bbb"] = ANY_SET(std::string, "Hello ");

	param2["aaa"] = ANY_SET(yggr::u32, 3);
	param2["bbb"] = ANY_SET(std::string, "World");

	param3.copy_from(param1 & param2);
	yggr_test_assert(param3.get_value<yggr::u32>("aaa") == (2 & 3));
	yggr_test_assert(param3.empty("bbb"));

	std::cout << param3.get_value<yggr::u32>("aaa") << std::endl;
	std::cout << param3.type_name("bbb") << std::endl;

	std::cout << "-------test_yggr_params_bitwise_and E---------" << std::endl;
}

void test_yggr_params_bitwise_and_equal(void)
{
	std::cout << "-------test_yggr_params_bitwise_and_equal S---------" << std::endl;
	params_type param1, param2, param3, param4;
	param1["aaa"] = ANY_SET(yggr::u32, 2);
	param1["bbb"] = ANY_SET(std::string, "Hello ");

	param2["aaa"] = ANY_SET(yggr::u32, 3);
	param2["bbb"] = ANY_SET(std::string, "World");

	param3.copy_from(param1 &= param2);
	yggr_test_assert(param3.get_value<yggr::u32>("aaa") == (2 & 3));
	yggr_test_assert(param3.empty("bbb"));

	std::cout << param3.get_value<yggr::u32>("aaa") << std::endl;
	std::cout << param3.type_name("bbb") << std::endl;

	yggr_test_assert(param3 == param1);

	std::cout << "-------test_yggr_params_bitwise_and_equal E---------" << std::endl;
}

void test_yggr_params_bitwise_or(void)
{
	std::cout << "-------test_yggr_params_bitwise_or S---------" << std::endl;
	params_type param1, param2, param3, param4;
	param1["aaa"] = ANY_SET(yggr::u32, 2);
	param1["bbb"] = ANY_SET(std::string, "Hello ");

	param2["aaa"] = ANY_SET(yggr::u32, 3);
	param2["bbb"] = ANY_SET(std::string, "World");

	param3.copy_from(param1 | param2);
	yggr_test_assert(param3.get_value<yggr::u32>("aaa") == (2 | 3));
	yggr_test_assert(param3.empty("bbb"));

	std::cout << param3.get_value<yggr::u32>("aaa") << std::endl;
	std::cout << param3.type_name("bbb") << std::endl;

	std::cout << "-------test_yggr_params_bitwise_or E---------" << std::endl;
}

void test_yggr_params_bitwise_or_equal(void)
{
	std::cout << "-------test_yggr_params_bitwise_or_equal S---------" << std::endl;
	params_type param1, param2, param3, param4;
	param1["aaa"] = ANY_SET(yggr::u32, 2);
	param1["bbb"] = ANY_SET(std::string, "Hello ");

	param2["aaa"] = ANY_SET(yggr::u32, 3);
	param2["bbb"] = ANY_SET(std::string, "World");

	param3.copy_from(param1 |= param2);
	yggr_test_assert(param3.get_value<yggr::u32>("aaa") == (2 | 3));
	yggr_test_assert(param3.empty("bbb"));

	std::cout << param3.get_value<yggr::u32>("aaa") << std::endl;
	std::cout << param3.type_name("bbb") << std::endl;

	yggr_test_assert(param3 == param1);

	std::cout << "-------test_yggr_params_bitwise_or_equal E---------" << std::endl;
}

void test_yggr_params_bitwise_xor(void)
{
	std::cout << "-------test_yggr_params_bitwise_xor S---------" << std::endl;
	params_type param1, param2, param3, param4;
	param1["aaa"] = ANY_SET(yggr::u32, 2);
	param1["bbb"] = ANY_SET(std::string, "Hello ");

	param2["aaa"] = ANY_SET(yggr::u32, 3);
	param2["bbb"] = ANY_SET(std::string, "World");

	param3.copy_from(param1 ^ param2);
	yggr_test_assert(param3.get_value<yggr::u32>("aaa") == (2 ^ 3));
	yggr_test_assert(param3.empty("bbb"));

	std::cout << param3.get_value<yggr::u32>("aaa") << std::endl;
	std::cout << param3.type_name("bbb") << std::endl;

	std::cout << "-------test_yggr_params_bitwise_xor E---------" << std::endl;
}

void test_yggr_params_bitwise_xor_equal(void)
{
	std::cout << "-------test_yggr_params_bitwise_xor_equal S---------" << std::endl;
	params_type param1, param2, param3, param4;
	param1["aaa"] = ANY_SET(yggr::u32, 2);
	param1["bbb"] = ANY_SET(std::string, "Hello ");

	param2["aaa"] = ANY_SET(yggr::u32, 3);
	param2["bbb"] = ANY_SET(std::string, "World");

	param3.copy_from(param1 ^= param2);
	yggr_test_assert(param3.get_value<yggr::u32>("aaa") == (2 ^ 3));
	yggr_test_assert(param3.empty("bbb"));

	std::cout << param3.get_value<yggr::u32>("aaa") << std::endl;
	std::cout << param3.type_name("bbb") << std::endl;

	yggr_test_assert(param3 == param1);

	std::cout << "-------test_yggr_params_bitwise_xor_equal E---------" << std::endl;
}

void test_yggr_params_left_trans(void)
{
	std::cout << "-------test_yggr_params_left_trans S---------" << std::endl;
	params_type param1, param2, param3, param4;
	param1["aaa"] = ANY_SET(yggr::u32, 2);
	param1["bbb"] = ANY_SET(std::string, "Hello ");

	param2["aaa"] = ANY_SET(yggr::u32, 3);
	param2["bbb"] = ANY_SET(std::string, "World");

	param3.copy_from(param1 << param2);
	yggr_test_assert(param3.get_value<yggr::u32>("aaa") == (2 << 3));
	yggr_test_assert(param3.empty("bbb"));

	std::cout << param3.get_value<yggr::u32>("aaa") << std::endl;
	std::cout << param3.type_name("bbb") << std::endl;

	std::cout << "-------test_yggr_params_left_trans E---------" << std::endl;
}

void test_yggr_params_left_trans_equal(void)
{
	std::cout << "-------test_yggr_params_left_trans_equal S---------" << std::endl;
	params_type param1, param2, param3, param4;
	param1["aaa"] = ANY_SET(yggr::u32, 2);
	param1["bbb"] = ANY_SET(std::string, "Hello ");

	param2["aaa"] = ANY_SET(yggr::u32, 3);
	param2["bbb"] = ANY_SET(std::string, "World");

	param3.copy_from(param1 <<= param2);
	yggr_test_assert(param3.get_value<yggr::u32>("aaa") == (2 << 3));
	yggr_test_assert(param3.empty("bbb"));

	std::cout << param3.get_value<yggr::u32>("aaa") << std::endl;
	std::cout << param3.type_name("bbb") << std::endl;

	yggr_test_assert(param3 == param1);

	std::cout << "-------test_yggr_params_left_trans_equal E---------" << std::endl;
}

void test_yggr_params_right_trans(void)
{
	std::cout << "-------test_yggr_params_right_trans S---------" << std::endl;
	params_type param1, param2, param3, param4;
	param1["aaa"] = ANY_SET(yggr::u32, 2);
	param1["bbb"] = ANY_SET(std::string, "Hello ");

	param2["aaa"] = ANY_SET(yggr::u32, 3);
	param2["bbb"] = ANY_SET(std::string, "World");

	param3.copy_from(param1 >> param2);
	yggr_test_assert(param3.get_value<yggr::u32>("aaa") == (2 >> 3));
	yggr_test_assert(param3.empty("bbb"));

	std::cout << param3.get_value<yggr::u32>("aaa") << std::endl;
	std::cout << param3.type_name("bbb") << std::endl;

	std::cout << "-------test_yggr_params_right_trans E---------" << std::endl;
}

void test_yggr_params_right_trans_equal(void)
{
	std::cout << "-------test_yggr_params_right_trans_equal S---------" << std::endl;
	params_type param1, param2, param3, param4;
	param1["aaa"] = ANY_SET(yggr::u32, 2);
	param1["bbb"] = ANY_SET(std::string, "Hello ");

	param2["aaa"] = ANY_SET(yggr::u32, 3);
	param2["bbb"] = ANY_SET(std::string, "World");

	param3.copy_from(param1 >>= param2);
	yggr_test_assert(param3.get_value<yggr::u32>("aaa") == (2 >> 3));
	yggr_test_assert(param3.empty("bbb"));

	std::cout << param3.get_value<yggr::u32>("aaa") << std::endl;
	std::cout << param3.type_name("bbb") << std::endl;

	yggr_test_assert(param3 == param1);

	std::cout << "-------test_yggr_params_right_trans_equal E---------" << std::endl;
}

void test_yggr_params_merge_datas(void)
{
	std::cout << "-------test_yggr_params_merge_datas S---------" << std::endl;
	params_type param1, param2, param3, param4;
	param1["aaa"] = ANY_SET(yggr::u32, 2);
	param1["bbb"] = ANY_SET(std::string, "Hello ");

	param1["aaa"] = ANY_SET(yggr::u32, 3);
	param2["ccc"] = ANY_SET(yggr::f32, 12.5f);

	param1.merge_datas_from(param2);

	yggr_test_assert(param1.get_value<yggr::u32>("aaa") == 3);
	yggr_test_assert(param1.get_value<std::string>("bbb") == std::string("Hello "));
	yggr_test_assert(param1.get_value<yggr::f32>("ccc") == 12.5f);

	std::cout << "-------test_yggr_params_merge_datas E---------" << std::endl;
}

void test_yggr_params_serialize_like(void)
{
	std::cout << "-------test_yggr_params_exchg S---------" << std::endl;
	A a1;
	a1.n = 100;
	a1.str = "params";

	params_type params_a = a1.to_params();

	A a2;
	a2.from_params(params_a);

	std::cout << a2.n << "," << a2.str << std::endl;

	yggr_test_assert(a1 == a2);

	std::cout << "-------test_yggr_params_exchg E---------" << std::endl;
}

void test_yggr_params_swap(void)
{
	std::cout << "-------test_yggr_params_swap S---------" << std::endl;
	params_type param1, param2, param3, param4;
	param1["aaa"] = ANY_SET(yggr::u32, 2);
	param1["bbb"] = ANY_SET(std::string, "Hello ");

	param2["aaa"] = ANY_SET(yggr::u32, 3);
	param2["bbb"] = ANY_SET(std::string, "World");


	param3.copy_from(param1);
	param4.copy_from(param2);

	param3.swap(param4);
	yggr_test_assert((param3 == param2 && param4 == param1));

	std::swap(param3, param4);
	yggr_test_assert((param3 == param1 && param4 == param2));

	boost::swap(param3, param4);
	yggr_test_assert((param3 == param2 && param4 == param1));

	std::cout << "-------test_yggr_params_swap E---------" << std::endl;
}

void test_yggr(void)
{
	bool bchk = yggr::any_val::any_operator_mgr_basic_t_reg::s_init(true);
	yggr_test_assert(bchk);

	test_yggr_params_init();

	test_yggr_params_plus();
	test_yggr_params_plus_equal();

	test_yggr_params_mul();
	test_yggr_params_mul_equal();

	test_yggr_params_div();
	test_yggr_params_div_equal();

	test_yggr_params_mod();
	test_yggr_params_mod_equal();

	test_yggr_params_bitwise_and();
	test_yggr_params_bitwise_and_equal();

	test_yggr_params_bitwise_or();
	test_yggr_params_bitwise_or_equal();

	test_yggr_params_bitwise_xor();
	test_yggr_params_bitwise_xor_equal();

	test_yggr_params_left_trans();
	test_yggr_params_left_trans_equal();

	test_yggr_params_right_trans();
	test_yggr_params_right_trans_equal();

	test_yggr_params_merge_datas();

	test_yggr_params_serialize_like();

	test_yggr_params_swap();
}

int main(int argc, char* argv[])
{
	test_yggr();

	wait_any_key(argc, argv);
	return 0;
}
