//c_bson_binary_buffer_test.cpp.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/container/vector.hpp>
#include <yggr/container/list.hpp>
#include <yggr/container/set.hpp>
#include <yggr/container/array.hpp>
#include <yggr/container/unordered_set.hpp>

#include <yggr/nsql_database_system/c_bson.hpp>
#include <yggr/nsql_database_system/c_bson_value.hpp>
#include <yggr/nsql_database_system/c_bson_binary_buffer.hpp>
#include <yggr/nsql_database_system/c_bson_binary_buffer_ref.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

void test_construct(void)
{
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;
	typedef yggr::nsql_database_system::c_bson_binary_buffer<0> bson_buffer_type;
	
	{
		bson_buffer_type data;
		std::cout << data << std::endl;
		yggr_test_assert(data.empty());
	}

	{
		int arr[] = {1, 2, 3};
		bson_buffer_type org_data(arr);
		bson_value_type org_val(org_data);
		bson_buffer_type data(org_val);

		std::cout << data << std::endl;

		yggr_test_assert(data == org_data);
	}

	{
		int arr[] = {1, 2, 3};
		bson_buffer_type org_data(arr);
		bson_buffer_type data(boost::move(org_data.buffer()));

		std::cout << data << std::endl;

		//yggr_test_assert(data != org_data);
		//yggr_test_assert(org_data.empty());
	}

	{
		int arr[] = {1, 2, 3};
		bson_buffer_type org_data(arr);
		bson_buffer_type data(org_data.buffer());

		std::cout << data << std::endl;

		yggr_test_assert(data == org_data);
	}

	{
		int arr[] = {1, 2, 3};
		bson_buffer_type org_data(arr);
		bson_buffer_type data(org_data.data(), org_data.size());

		std::cout << data << std::endl;

		yggr_test_assert(data == org_data);
	}

	{
		int arr[] = {1, 2, 3};
		std::vector<int> org_cont1(arr + 0, arr + 3);
		std::list<int> org_cont2(arr + 0, arr + 3);
		bson_buffer_type data1(org_cont1);
		bson_buffer_type data2(org_cont2);

		std::cout << data1 << std::endl;
		std::cout << data2 << std::endl;

		yggr_test_assert(data1 ==  data2);
	}

	{
		int arr[] = {1, 2, 3};
		bson_buffer_type data(arr + 0, arr + 3);

		std::cout << data << std::endl;

		yggr_test_assert(data == arr);
	}

	{
		int arr[] = {1, 2, 3};
		bson_buffer_type org_data(arr);
		bson_buffer_type data(boost::move(org_data));

		std::cout << data << std::endl;

		//yggr_test_assert(data != org_data);
		//yggr_test_assert(org_data.empty());
	}

	{
		int arr[] = {1, 2, 3};
		bson_buffer_type org_data(arr);
		bson_buffer_type data(org_data);

		std::cout << data << std::endl;

		yggr_test_assert(data == org_data);
	}

	std::cout << "-------------test_construct end---------------" << std::endl;
}

void test_operator_set(void)
{
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;
	typedef yggr::nsql_database_system::c_bson_binary_buffer<0> bson_buffer_type;

	{
		int arr[] = {1, 2, 3};
		bson_buffer_type org_data(arr);
		bson_value_type org_val(org_data);
		bson_buffer_type data;
		data = org_val;

		std::cout << data << std::endl;

		yggr_test_assert(data == org_data);
	}

	{
		int arr[] = {1, 2, 3};
		bson_buffer_type org_data(arr);
		bson_buffer_type data;
		data = (boost::move(org_data.buffer()));

		std::cout << data << std::endl;

		//yggr_test_assert(data != org_data);
		//yggr_test_assert(org_data.empty());
	}

	{
		int arr[] = {1, 2, 3};
		bson_buffer_type org_data(arr);
		bson_buffer_type data;
		data = (org_data.buffer());

		std::cout << data << std::endl;

		yggr_test_assert(data == org_data);
	}

	{
		int arr[] = {1, 2, 3};
		std::vector<int> org_cont1(arr + 0, arr + 3);
		std::list<int> org_cont2(arr + 0, arr + 3);
		bson_buffer_type data1;
		data1 = (org_cont1);
		bson_buffer_type data2;
		data2 = (org_cont2);

		std::cout << data1 << std::endl;
		std::cout << data2 << std::endl;

		yggr_test_assert(data1 ==  data2);
	}

	{
		int arr[] = {1, 2, 3};
		bson_buffer_type org_data(arr);
		bson_buffer_type data;
		data = (boost::move(org_data));

		std::cout << data << std::endl;

		//yggr_test_assert(data != org_data);
		//yggr_test_assert(org_data.empty());
	}

	{
		int arr[] = {1, 2, 3};
		bson_buffer_type org_data(arr);
		bson_buffer_type data;
		data = (org_data);

		std::cout << data << std::endl;

		yggr_test_assert(data == org_data);
	}

	std::cout << "-------------test_operator_set end---------------" << std::endl;
}

void test_swap(void)
{
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;
	typedef yggr::nsql_database_system::c_bson_binary_buffer<0> bson_buffer_type;

	{
		int arr1[] = {1, 2, 3};
		int arr2[] = {4, 5, 6};

		bson_buffer_type data1(arr1);
		bson_buffer_type data2(arr2);

		data1.swap(boost::move(data2.buffer()));

		yggr_test_assert(data1 == arr2);
		yggr_test_assert(data2 == arr1);

		data1.swap(data2.buffer());

		yggr_test_assert(data1 == arr1);
		yggr_test_assert(data2 == arr2);

		std::swap(data1, data2.buffer());

		yggr_test_assert(data1 == arr2);
		yggr_test_assert(data2 == arr1);

		boost::swap(data1.buffer(), data2);

		yggr_test_assert(data1 == arr1);
		yggr_test_assert(data2 == arr2);

		yggr::swap(data1, data2);

		yggr_test_assert(data1 == arr2);
		yggr_test_assert(data2 == arr1);
	}

	{
		int arr1[] = {1, 2, 3};
		int arr2[] = {4, 5, 6};

		bson_buffer_type data1(arr1);
		bson_buffer_type data2(arr2);

		data1.swap(boost::move(data2));

		yggr_test_assert(data1 == arr2);
		yggr_test_assert(data2 == arr1);

		data1.swap(data2);

		yggr_test_assert(data1 == arr1);
		yggr_test_assert(data2 == arr2);

		std::swap(data1, data2);

		yggr_test_assert(data1 == arr2);
		yggr_test_assert(data2 == arr1);

		boost::swap(data1, data2);

		yggr_test_assert(data1 == arr1);
		yggr_test_assert(data2 == arr2);

		yggr::swap(data1, data2);

		yggr_test_assert(data1 == arr2);
		yggr_test_assert(data2 == arr1);
	}

	std::cout << "-------------test_swap end---------------" << std::endl;
}

void test_member_foo(void)
{
	typedef yggr::nsql_database_system::bson_json_mode_def bson_json_mode_def_type;

	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;
	typedef yggr::nsql_database_system::c_bson_binary_buffer<0> bson_buffer_type;

	// value_typeid
	{
		bson_buffer_type data;

		yggr_test_assert(bson_buffer_type::value_typeid() == yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_BINARY);
		yggr_test_assert(bson_buffer_type::s_value_typeid() == yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_BINARY);
		yggr_test_assert(data.value_typeid() == yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_BINARY);
		yggr_test_assert(data.s_value_typeid() == yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_BINARY);

		yggr_test_assert(data.buffer_typeid() == 0);
		yggr_test_assert(data.s_buffer_typeid() == 0);
	}

	{
		int arr[] = {1, 2, 3};

		bson_buffer_type data(arr);
	
		yggr_test_assert(data.size() == sizeof(arr));
		yggr_test_assert(!data.empty());

		data.resize(16);
		std::cout << data << std::endl;

		yggr_test_assert(data.buffer().size() == data.size());

		yggr_test_assert(data.data());

		data.clear();

		yggr_test_assert(data.size() == 0);
		yggr_test_assert(data.empty());
		yggr_test_assert(!data.data());
	}

	// == !=
	{
		int arr_same[] = {1, 2, 3};
		int arr_diff[] = {4, 5, 6};

		bson_buffer_type data(arr_same);
		bson_buffer_type data_same(data);
		bson_buffer_type data_diff(arr_diff);

		bson_value_type bs_val_same(data_same);
		bson_value_type bs_val_diff(data_diff);

		bson_value_type::base_type& base_bs_val_same(bs_val_same);
		bson_value_type::base_type& base_bs_val_diff(bs_val_diff);

		std::cout << base_bs_val_same << std::endl;

		// ==
		yggr_test_assert(data == arr_same);
		yggr_test_assert(arr_same == data);
		yggr_test_assert(!(data == arr_diff));
		yggr_test_assert(!(arr_diff == data));

		yggr_test_assert(data == yggr::vector<int>(arr_same, arr_same + 3));
		yggr_test_assert(yggr::list<int>(arr_same, arr_same + 3) == data);
		yggr_test_assert(!(data == yggr::vector<int>(arr_diff, arr_diff + 3)));
		yggr_test_assert(!(yggr::list<int>(arr_diff, arr_diff + 3) == data));

		yggr_test_assert((data == data));
		yggr_test_assert((data == data_same));
		yggr_test_assert(!(data == data_diff));

		yggr_test_assert((data == bs_val_same));
		yggr_test_assert((bs_val_same == data));
		yggr_test_assert(!(data == bs_val_diff));
		yggr_test_assert(!(bs_val_diff == data));

		yggr_test_assert((data == base_bs_val_same));
		yggr_test_assert((base_bs_val_same == data));
		yggr_test_assert(!(data == base_bs_val_diff));
		yggr_test_assert(!(base_bs_val_diff == data));

		// !=
		yggr_test_assert(data != arr_diff);
		yggr_test_assert(arr_diff != data);
		yggr_test_assert(!(data != arr_same));
		yggr_test_assert(!(arr_same != data));

		yggr_test_assert(data != yggr::vector<int>(arr_diff, arr_diff + 3));
		yggr_test_assert(yggr::list<int>(arr_diff, arr_diff + 3) != data);
		yggr_test_assert(!(data != yggr::vector<int>(arr_same, arr_same + 3)));
		yggr_test_assert(!(yggr::list<int>(arr_same, arr_same + 3) != data));

		yggr_test_assert(!(data != data));
		yggr_test_assert(!(data != data_same));
		yggr_test_assert((data != data_diff));

		yggr_test_assert(!(data != bs_val_same));
		yggr_test_assert(!(bs_val_same != data));
		yggr_test_assert((data != bs_val_diff));
		yggr_test_assert((bs_val_diff != data));

		yggr_test_assert(!(data != base_bs_val_same));
		yggr_test_assert(!(base_bs_val_same != data));
		yggr_test_assert((data != base_bs_val_diff));
		yggr_test_assert((base_bs_val_diff != data));
	}

	// out_put
	{
		int arr[] = {1, 2, 3};
		bson_buffer_type data(arr);

		std::cout << data << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(data) << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(data) << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(data) << std::endl;
	}

	{
		int arr[] = {1, 2, 3};
		bson_buffer_type data(arr);

		std::string chk_str1 = "{ \"$binary\" : \"AQAAAAIAAAADAAAA\", \"$type\" : \"00\" }";
		std::string chk_str2 = "{ \"$binary\" : { \"base64\" : \"AQAAAAIAAAADAAAA\", \"subType\" : \"00\" } }";

		{
			std::stringstream ss;
			ss << data;
			yggr_test_assert(ss.str() == chk_str1);
		}

		{
			std::stringstream ss;
			ss << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(data);
			yggr_test_assert(ss.str() == chk_str1);
		}

		{
			std::stringstream ss;
			ss << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(data);
			yggr_test_assert(ss.str() == chk_str2);
		}

		{
			std::stringstream ss;
			ss << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(data);
			yggr_test_assert(ss.str() == chk_str2);
		}
	}

	{
		int arr[] = {1, 2, 3};
		bson_buffer_type data(arr);

		bson_type bs;
		bs.save("c_bson_binary_buffer", data);

		std::cout << bs << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(bs) << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(bs) << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(bs) << std::endl;

		bson_buffer_type ld_data;
		bson_type::iterator bs_iter(bs, "c_bson_binary_buffer");
		bs_iter.load("c_bson_binary_buffer", ld_data);

		yggr_test_assert(ld_data == data);
		yggr_test_assert(ld_data == arr);
	}
	
	std::cout << "-------------test_member_foo end---------------" << std::endl;
}

void test_binay_buffer(void)
{
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_binary_buffer<0> buffer_type;

	{
		bson_type bs;
		buffer_type buf;
		std::cout << buf << std::endl;
		bs.save("buf_name", buf);
		std::cout << bs << std::endl;

		buffer_type lbuf;
		bs.begin().load("buf_name", lbuf);

		yggr_test_assert(buf == lbuf);
		yggr_test_assert(!(buf != lbuf));
	}

	{
		int arr[] = {1, 2, 3};
		bson_type bs;
		buffer_type buf(arr);
		buf = arr;
		std::cout << buf << std::endl;
		bs.save("buf_name", buf);
		std::cout << bs << std::endl;

		buffer_type lbuf;
		bs.begin().load("buf_name", lbuf);

		yggr_test_assert(buf == lbuf);
		yggr_test_assert(!(buf != lbuf));
		yggr_test_assert(lbuf == arr);
		yggr_test_assert(!(lbuf != arr));
		yggr_test_assert(arr == lbuf);
		yggr_test_assert(!(arr != lbuf));
	}

	{
		int arr[] = {1, 2, 3};
		std::vector<int> cont(boost::begin(arr), boost::end(arr));
		bson_type bs;
		buffer_type buf(cont);
		buf = cont;
		std::cout << buf << std::endl;
		bs.save("buf_name", buf);
		std::cout << bs << std::endl;

		buffer_type lbuf;
		bs.begin().load("buf_name", lbuf);

		yggr_test_assert(buf == lbuf);
		yggr_test_assert(!(buf != lbuf));
		yggr_test_assert(lbuf == cont);
		yggr_test_assert(lbuf == arr);
		yggr_test_assert(!(lbuf != cont));
		yggr_test_assert(cont == lbuf);
		yggr_test_assert(arr == lbuf);
		yggr_test_assert(!(cont != lbuf));
	}

	{
		int arr[] = {1, 2, 3};
		std::list<int> cont(boost::begin(arr), boost::end(arr));
		bson_type bs;
		buffer_type buf(cont);
		buf = cont;
		std::cout << buf << std::endl;
		bs.save("buf_name", buf);
		std::cout << bs << std::endl;

		buffer_type lbuf;
		bs.begin().load("buf_name", lbuf);

		yggr_test_assert(buf == lbuf);
		yggr_test_assert(!(buf != lbuf));
		yggr_test_assert(lbuf == cont);
		yggr_test_assert(lbuf == arr);
		yggr_test_assert(!(lbuf != cont));
		yggr_test_assert(cont == lbuf);
		yggr_test_assert(arr == lbuf);
		yggr_test_assert(!(cont != lbuf));
	}

	{
		int arr[] = {1, 2, 3};
		std::set<int> cont(boost::begin(arr), boost::end(arr));
		bson_type bs;
		buffer_type buf(cont);
		buf = cont;
		std::cout << buf << std::endl;
		bs.save("buf_name", buf);
		std::cout << bs << std::endl;

		buffer_type lbuf;
		bs.begin().load("buf_name", lbuf);

		yggr_test_assert(buf == lbuf);
		yggr_test_assert(!(buf != lbuf));
		yggr_test_assert(lbuf == cont);
		yggr_test_assert(lbuf == arr);
		yggr_test_assert(!(lbuf != cont));
		yggr_test_assert(cont == lbuf);
		yggr_test_assert(arr == lbuf);
		yggr_test_assert(!(cont != lbuf));
	}

	{
		int arr[] = {1, 2, 3};
		boost::array<int, 3> cont = {1, 2, 3};
		bson_type bs;
		buffer_type buf(cont);
		buf = cont;
		std::cout << buf << std::endl;
		bs.save("buf_name", buf);
		std::cout << bs << std::endl;

		buffer_type lbuf;
		bs.begin().load("buf_name", lbuf);

		yggr_test_assert(buf == lbuf);
		yggr_test_assert(!(buf != lbuf));
		yggr_test_assert(lbuf == cont);
		yggr_test_assert(lbuf == arr);
		yggr_test_assert(!(lbuf != cont));
		yggr_test_assert(cont == lbuf);
		yggr_test_assert(arr == lbuf);
		yggr_test_assert(!(cont != lbuf));
	}

	{
		int arr[] = {1, 2, 3};
		boost::unordered_set<int> cont(boost::begin(arr), boost::end(arr));
		bson_type bs;
		buffer_type buf(cont);
		buf = cont;
		std::cout << buf << std::endl;
		bs.save("buf_name", buf);
		std::cout << bs << std::endl;

		buffer_type lbuf;
		bs.begin().load("buf_name", lbuf);

		yggr_test_assert(buf == lbuf);
		yggr_test_assert(!(buf != lbuf));
		yggr_test_assert(lbuf == cont);
#if (BOOST_VERSION < 108000)
		yggr_test_assert(lbuf != arr); // ub
#else
		yggr_test_assert(lbuf == arr); // ub hash_function changed
#endif // (BOOST_VERSION < 108000)
		yggr_test_assert(!(lbuf != cont));
		yggr_test_assert(cont == lbuf);

#if (BOOST_VERSION < 108000)
		yggr_test_assert(arr != lbuf); // ub
#else
		yggr_test_assert(arr == lbuf); // ub hash_function changed
#endif // (BOOST_VERSION < 108000)
		yggr_test_assert(!(cont != lbuf));
	}

	std::cout << "---------------test_binay_buffer success------------------" << std::endl;
}

int main(int argc, char* argv[])
{
	test_construct();
	test_operator_set();
	test_swap();
	test_member_foo();

	test_binay_buffer();

	wait_any_key(argc, argv);
	return 0;
}