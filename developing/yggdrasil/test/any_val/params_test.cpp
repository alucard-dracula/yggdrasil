//params_test.cpp

#include <iostream>

#ifdef _WIN32
#   include <vld.h>
#endif //_WIN32
//#pragma comment(lib, "vld.lib")

#include <yggr/any_val/base_any_type_operator.hpp>
#include <yggr/any_val/any_val_operator.hpp>
#include <yggr/any_val/params.hpp>
#include <yggr/charset/utf8_string.hpp>
#include <boost/bind.hpp>
#include <string>

typedef yggr::any_val::any_val_operator<yggr::any_val::base_any_type_operator> any_val_op_type;
typedef yggr::any_val::params<std::string, any_val_op_type> params_type;

class A
{
public:

	A(void)
		: n(0)
	{
	}

	void from_params(const params_type& params)
	{
		n = params.get_val<yggr::u32>("n");
		str = params.get_val<std::string>("str");
	}

	params_type to_params(void) const
	{
		params_type rst;
		rst["n"] = ANY_SET(yggr::u32, n);
		rst["str"] = ANY_SET(std::string, str);
		return rst;
	}

	yggr::u32 n;
	std::string str;
};


const params_type& set_val(params_type& param, const std::string& name, yggr::u32 val)
{
	param[name] = ANY_SET(yggr::u32, val);
	return param;
}


int main(int argc, char* argv[])
{
	params_type param1, param2, param3, param4;
	param1["aaa"] = ANY_SET(yggr::u32, 2);
	param1["bbb"] = ANY_SET(std::string, "Hello");


	param2["aaa"] = ANY_SET(yggr::u32, 2);
	param2["bbb"] = ANY_SET(std::string, "200");

	param3.copy_from(param1 + param2);
	param3.copy_from(param1 += param2);

	param3.copy_from(param1 * param2);
	param3.copy_from(param1 *= param2);

	param3.copy_from(param1 / param2);
	param3.copy_from(param1 /= param2);

	param3.copy_from(param1 % param2);
	param3.copy_from(param1 %= param2);

	param3.copy_from(param1 & param2);
	param3.copy_from(param1 &= param2);

	param3.copy_from(param1 | param2);
	param3.copy_from(param1 |= param2);

	param3.copy_from(param1 ^ param2);
	param3.copy_from(param1 ^= param2);

	param3.copy_from(param1 << param2);
	param3.copy_from(param1 <<= param2);

	param3.copy_from(param1 >> param2);
	param3.copy_from(param1 >>= param2);

	param4["ccc"] = ANY_SET(yggr::f32, 12.5f);

	std::cout << param3["aaa"].type().name() << std::endl;
	std::cout << param3["bbb"].type().name() << std::endl;

	const yggr::u32 *pu32 = 0;
	const std::string* pstr = 0;
	const yggr::f32 *pf32 = 0;
	//pu32 = param3.get_val_ptr<yggr::u32>("aaa");
	std::cout << ((pu32 = param3.get_val_ptr<yggr::u32>("aaa"))? (*pu32) : 0) << ","
			<< ((pstr = param3.get_val_ptr<std::string>("bbb"))? (*pstr) : "")<< std::endl;

	param3.merge_from(param4);

	//std::cout << (param4["ccc"]).type().name() << std::endl;

	//std::cout << (param3["ccc"]).type().name() << std::endl;
	std::cout << ((pu32 = param3.get_val_ptr<yggr::u32>("aaa"))? (*pu32) : 0) << ","
				<< ((pstr = param3.get_val_ptr<std::string>("bbb"))? (*pstr) : "") << ","
				<< ((pf32 = param3.get_val_ptr<yggr::f32>("ccc"))? (*pf32) : 0.0f)<< std::endl;

	params_type param5;
	param5["aaa"] = ANY_SET(yggr::u32, 100);
	param5["aaa"] = ANY_SET(std::string, "Hello");

	//std::cout << param5["aaa"].type().name() << std::endl;

	std::cout << boost::any_cast<std::string>(param5["aaa"]) << std::endl;

	A a1;
	a1.n = 100;
	a1.str = "params";

	params_type params_a = a1.to_params();

	A a2;
	a2.from_params(params_a);

	std::cout << a2.n << "," << a2.str << std::endl;

	params_type param10, param11;
	param10["aaa"] = ANY_SET(yggr::u32, 100);

	param11.copy_from(param10(boost::bind(&set_val, _1, "aaa", 2000)));

	std::cout << param10.get_val<yggr::u32>("aaa") << std::endl;
	std::cout << param11.get_val<yggr::u32>("aaa") << std::endl;


	params_type param6;
	param6["name"] = ANY_SET(yggr::charset::utf8_string, "ÄãºÃ");

	std::cout << boost::any_cast<yggr::charset::utf8_string>(param6["name"]) << std::endl;

	boost::any any(std::string("12345"));
	boost::any_cast<std::string&>(any) = std::string("abcdefg");

	std::cout << boost::any_cast<const std::string&>(any) << std::endl;

	char cc = 0;
	std::cin >> cc;
	return 0;
}
