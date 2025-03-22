//c_bson_binary_buffer_test.cpp

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

#include <typeinfo>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

template<typename BufCont, typename InputCont> inline
void test_operator_set_detail(BufCont& init_data, const InputCont& input)
{
	typedef BufCont buf_cont_type;
	//typedef InputCont input_cont_type;

	//typedef typename yggr::range_ex::range_value_ex<buf_cont_type>::type buf_val_type;
	//typedef typename yggr::range_ex::range_value_ex<input_cont_type>::type input_val_type;

	//typedef yggr::vector<buf_val_type> buf_val_vt_type;
	//typedef yggr::vector<input_val_type> input_val_vt_type;

	typedef yggr::nsql_database_system::c_bson_binary_buffer_ref<0, buf_cont_type> bson_buffer_ref_type;

	bson_buffer_ref_type data(init_data);
	data = input;

	std::cout << data << std::endl;

	yggr_test_assert(data == input);
}


template<typename BufCont, typename InputVal>
typename boost::enable_if<yggr::is_static_container<BufCont>, void>::type
	test_operator_set_combo_cont_t(void)
{
	typedef BufCont buf_cont_type;

	typedef typename yggr::range_ex::range_value_ex<buf_cont_type>::type buf_val_type;
	typedef InputVal input_val_type;

	typedef boost::mpl::size_t<sizeof(buf_val_type)> buf_val_byte_size_type;
	typedef boost::mpl::size_t<sizeof(input_val_type)> input_val_byte_size_type;

	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;

	typedef
		boost::mpl::size_t
		<
			((sizeof(buf_cont_type) + input_val_byte_size_type::value - 1) / input_val_byte_size_type::value)
		> input_array_size_type;

	typedef input_val_type org_input_type[input_array_size_type::value];

//	buf_cont_type* p1 = 0;
//	buf_val_byte_size_type* p2 = 0;
//	input_val_byte_size_type* p3 = 0;


	{
		buf_cont_type init_cont = {0};

		{
			org_input_type input = {0};

			for(std::size_t i = 0, isize = input_array_size_type::value; i != isize; ++i)
			{
				input[i] = static_cast<input_val_type>(i);
			}

			test_operator_set_detail(init_cont, input);
		}

		// array
		{
			typedef boost::array<input_val_type, input_array_size_type::value> input_type;
			input_type input = {0};

			for(std::size_t i = 0, isize = input_array_size_type::value; i != isize; ++i)
			{
				input[i] = static_cast<input_val_type>(i);
			}

			test_operator_set_detail(init_cont, input);
		}

		org_input_type org_input = {0};
		for(std::size_t i = 0, isize = input_array_size_type::value; i != isize; ++i)
		{
			org_input[i] = static_cast<input_val_type>(i);
		}

		// vector
		{
			{
				typedef std::vector<input_val_type> input_type;

				input_type input(org_input, org_input + input_array_size_type::value);
				test_operator_set_detail(init_cont, input);
			}

			{
				typedef boost::container::vector<input_val_type> input_type;

				input_type input(org_input, org_input + input_array_size_type::value);
				test_operator_set_detail(init_cont, input);
			}

			{
				typedef yggr::vector<input_val_type> input_type;

				input_type input(org_input, org_input + input_array_size_type::value);
				test_operator_set_detail(init_cont, input);
			}
		}

		// list
		{
			{
				typedef std::list<input_val_type> input_type;

				input_type input(org_input, org_input + input_array_size_type::value);
				test_operator_set_detail(init_cont, input);
			}

			{
				typedef boost::container::list<input_val_type> input_type;

				input_type input(org_input, org_input + input_array_size_type::value);
				test_operator_set_detail(init_cont, input);
			}

			{
				typedef yggr::list<input_val_type> input_type;

				input_type input(org_input, org_input + input_array_size_type::value);
				test_operator_set_detail(init_cont, input);
			}
		}

//		// set
//		{
//			{
//				typedef std::set<input_val_type> input_type;
//
//				input_type input(org_input, org_input + input_array_size_type::value);
//				test_operator_set_detail(init_cont, input);
//			}
//
//			{
//				typedef boost::container::set<input_val_type> input_type;
//
//				input_type input(org_input, org_input + input_array_size_type::value);
//				test_operator_set_detail(init_cont, input);
//			}
//
//			{
//				typedef yggr::set<input_val_type> input_type;
//
//				input_type input(org_input, org_input + input_array_size_type::value);
//				test_operator_set_detail(init_cont, input);
//			}
//		}
//
//		// multiset
//		{
//			{
//				typedef std::multiset<input_val_type> input_type;
//
//				input_type input(org_input, org_input + input_array_size_type::value);
//				test_operator_set_detail(init_cont, input);
//			}
//
//			{
//				typedef boost::container::multiset<input_val_type> input_type;
//
//				input_type input(org_input, org_input + input_array_size_type::value);
//				test_operator_set_detail(init_cont, input);
//			}
//
//			{
//				typedef yggr::multiset<input_val_type> input_type;
//
//				input_type input(org_input, org_input + input_array_size_type::value);
//				test_operator_set_detail(init_cont, input);
//			}
//		}
//
//		// unordered_set
//		{
//#		if !defined(YGGR_NO_CXX11_HDR_UNORDERED_SET)
//			{
//				typedef std::unordered_set<input_val_type> input_type;
//
//				input_type input(org_input, org_input + input_array_size_type::value);
//				test_operator_set_detail(init_cont, input);
//			}
//#		endif // YGGR_NO_CXX11_HDR_UNORDERED_SET
//
//			{
//				typedef boost::unordered_set<input_val_type> input_type;
//
//				input_type input(org_input, org_input + input_array_size_type::value);
//				test_operator_set_detail(init_cont, input);
//			}
//
//			{
//				typedef yggr::unordered_set<input_val_type> input_type;
//
//				input_type input(org_input, org_input + input_array_size_type::value);
//				test_operator_set_detail(init_cont, input);
//			}
//		}
//
//		// unordered_multiset
//		{
//#		if !defined(YGGR_NO_CXX11_HDR_UNORDERED_SET)
//			{
//				typedef std::unordered_multiset<input_val_type> input_type;
//
//				input_type input(org_input, org_input + input_array_size_type::value);
//				test_operator_set_detail(init_cont, input);
//			}
//#		endif // YGGR_NO_CXX11_HDR_UNORDERED_SET
//
//			{
//				typedef boost::unordered_multiset<input_val_type> input_type;
//
//				input_type input(org_input, org_input + input_array_size_type::value);
//				test_operator_set_detail(init_cont, input);
//			}
//
//			{
//				typedef yggr::unordered_multiset<input_val_type> input_type;
//
//				input_type input(org_input, org_input + input_array_size_type::value);
//				test_operator_set_detail(init_cont, input);
//			}
//		}
	}

	std::cout
		<< "-------------test_operator_set_combo_cont_t [ "
		<< typeid(buf_cont_type).name()
		<< ", "
		<< typeid(input_val_type).name()
		<< " ] end---------------\n"
		<< std::endl;
}

template<typename BufCont, typename InputVal>
typename boost::disable_if<yggr::is_static_container<BufCont>, void>::type
	test_operator_set_combo_cont_t(void)
{
	typedef BufCont buf_cont_type;
	typedef InputVal input_val_type;

	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;

	{
		buf_cont_type init_cont;

		{
			typedef input_val_type input_type[8];
			input_type input = {1, 2, 3, 4, 5 ,6, 7, 8};

			test_operator_set_detail(init_cont, input);
		}

		// array
		{
			typedef boost::array<input_val_type, 8> input_type;
			input_type input = {1, 2, 3, 4, 5 ,6, 7, 8};
			test_operator_set_detail(init_cont, input);
		}

		// vector
		{
			{
				typedef std::vector<input_val_type> input_type;

				input_val_type org_input[] = {1, 2, 3, 4, 5 ,6, 7, 8};
				input_type input(org_input, org_input + 8);
				test_operator_set_detail(init_cont, input);
			}

			{
				typedef boost::container::vector<input_val_type> input_type;

				input_val_type org_input[] = {1, 2, 3, 4, 5 ,6, 7, 8};
				input_type input(org_input, org_input + 8);
				test_operator_set_detail(init_cont, input);
			}

			{
				typedef yggr::vector<input_val_type> input_type;

				input_val_type org_input[] = {1, 2, 3, 4, 5 ,6, 7, 8};
				input_type input(org_input, org_input + 8);
				test_operator_set_detail(init_cont, input);
			}
		}

		// list
		{
			{
				typedef std::list<input_val_type> input_type;

				input_val_type org_input[] = {1, 2, 3, 4, 5 ,6, 7, 8};
				input_type input(org_input, org_input + 8);
				test_operator_set_detail(init_cont, input);
			}

			{
				typedef boost::container::list<input_val_type> input_type;

				input_val_type org_input[] = {1, 2, 3, 4, 5 ,6, 7, 8};
				input_type input(org_input, org_input + 8);
				test_operator_set_detail(init_cont, input);
			}

			{
				typedef yggr::list<input_val_type> input_type;

				input_val_type org_input[] = {1, 2, 3, 4, 5 ,6, 7, 8};
				input_type input(org_input, org_input + 8);
				test_operator_set_detail(init_cont, input);
			}
		}

//		// set
//		{
//			{
//				typedef std::set<input_val_type> input_type;
//
//				input_val_type org_input[] = {1, 2, 3, 4, 5 ,6, 7, 8};
//				input_type input(org_input, org_input + 8);
//				test_operator_set_detail(init_cont, input);
//			}
//
//			{
//				typedef boost::container::set<input_val_type> input_type;
//
//				input_val_type org_input[] = {1, 2, 3, 4, 5 ,6, 7, 8};
//				input_type input(org_input, org_input + 8);
//				test_operator_set_detail(init_cont, input);
//			}
//
//			{
//				typedef yggr::set<input_val_type> input_type;
//
//				input_val_type org_input[] = {1, 2, 3, 4, 5 ,6, 7, 8};
//				input_type input(org_input, org_input + 8);
//				test_operator_set_detail(init_cont, input);
//			}
//		}
//
//		// multiset
//		{
//			{
//				typedef std::multiset<input_val_type> input_type;
//
//				input_val_type org_input[] = {1, 2, 3, 4, 5 ,6, 7, 8};
//				input_type input(org_input, org_input + 8);
//				test_operator_set_detail(init_cont, input);
//			}
//
//			{
//				typedef boost::container::multiset<input_val_type> input_type;
//
//				input_val_type org_input[] = {1, 2, 3, 4, 5 ,6, 7, 8};
//				input_type input(org_input, org_input + 8);
//				test_operator_set_detail(init_cont, input);
//			}
//
//			{
//				typedef yggr::multiset<input_val_type> input_type;
//
//				input_val_type org_input[] = {1, 2, 3, 4, 5 ,6, 7, 8};
//				input_type input(org_input, org_input + 8);
//				test_operator_set_detail(init_cont, input);
//			}
//		}
//
//		// unordered_set
//		{
//
//#		if !defined(YGGR_NO_CXX11_HDR_UNORDERED_SET)
//			{
//				typedef std::unordered_set<input_val_type> input_type;
//
//				input_val_type org_input[] = {1, 2, 3, 4, 5 ,6, 7, 8};
//				input_type input(org_input, org_input + 8);
//				test_operator_set_detail(init_cont, input);
//			}
//#		endif // YGGR_NO_CXX11_HDR_UNORDERED_SET
//
//			{
//				typedef boost::unordered_set<input_val_type> input_type;
//
//				input_val_type org_input[] = {1, 2, 3, 4, 5 ,6, 7, 8};
//				input_type input(org_input, org_input + 8);
//				test_operator_set_detail(init_cont, input);
//			}
//
//			{
//				typedef yggr::unordered_set<input_val_type> input_type;
//
//				input_val_type org_input[] = {1, 2, 3, 4, 5 ,6, 7, 8};
//				input_type input(org_input, org_input + 8);
//				test_operator_set_detail(init_cont, input);
//			}
//		}
//
//		// unordered_multiset
//		{
//
//#		if !defined(YGGR_NO_CXX11_HDR_UNORDERED_SET)
//			{
//				typedef std::unordered_multiset<input_val_type> input_type;
//
//				input_val_type org_input[] = {1, 2, 3, 4, 5 ,6, 7, 8};
//				input_type input(org_input, org_input + 8);
//				test_operator_set_detail(init_cont, input);
//			}
//#		endif // YGGR_NO_CXX11_HDR_UNORDERED_SET
//
//			{
//				typedef boost::unordered_multiset<input_val_type> input_type;
//
//				input_val_type org_input[] = {1, 2, 3, 4, 5 ,6, 7, 8};
//				input_type input(org_input, org_input + 8);
//				test_operator_set_detail(init_cont, input);
//			}
//
//			{
//				typedef yggr::unordered_multiset<input_val_type> input_type;
//
//				input_val_type org_input[] = {1, 2, 3, 4, 5 ,6, 7, 8};
//				input_type input(org_input, org_input + 8);
//				test_operator_set_detail(init_cont, input);
//			}
//		}
	}

	std::cout
		<< "-------------test_operator_set_combo_cont_t [ "
		<< typeid(buf_cont_type).name()
		<< ", "
		<< typeid(input_val_type).name()
		<< " ] end---------------\n"
		<< std::endl;
}


template<typename ContVal, typename InputVal>
void test_operator_set_combe(void)
{
	typedef ContVal cont_val_type;
	typedef InputVal input_val_type;

	// array
	{
		typedef cont_val_type buffer_type[8];
		{
			typedef input_val_type input_value_type;
			test_operator_set_combo_cont_t<buffer_type, input_value_type>();
		}
	}

	// boost::array
	{
		typedef boost::array<cont_val_type, 8> buffer_type;
		{
			typedef input_val_type input_value_type;
			test_operator_set_combo_cont_t<buffer_type, input_value_type>();
		}
	}

	// vector
	{
		{
			typedef std::vector<cont_val_type> buffer_type;
			{
				typedef input_val_type input_value_type;
				test_operator_set_combo_cont_t<buffer_type, input_value_type>();
			}
		}

		{
			typedef boost::container::vector<cont_val_type> buffer_type;
			{
				typedef input_val_type input_value_type;
				test_operator_set_combo_cont_t<buffer_type, input_value_type>();
			}
		}

		{
			typedef yggr::vector<cont_val_type> buffer_type;
			{
				typedef input_val_type input_value_type;
				test_operator_set_combo_cont_t<buffer_type, input_value_type>();
			}
		}
	}

	// list
	{
		{
			typedef std::list<cont_val_type> buffer_type;
			{
				typedef input_val_type input_value_type;
				test_operator_set_combo_cont_t<buffer_type, input_value_type>();
			}
		}

		{
			typedef boost::container::list<cont_val_type> buffer_type;
			{
				typedef input_val_type input_value_type;
				test_operator_set_combo_cont_t<buffer_type, input_value_type>();
			}
		}

		{
			typedef yggr::list<cont_val_type> buffer_type;
			{
				typedef input_val_type input_value_type;
				test_operator_set_combo_cont_t<buffer_type, input_value_type>();
			}
		}
	}

//	// set
//	{
//		{
//			typedef std::set<cont_val_type> buffer_type;
//			{
//				typedef input_val_type input_value_type;
//				test_operator_set_combo_cont_t<buffer_type, input_value_type>();
//			}
//		}
//
//		{
//			typedef boost::container::set<cont_val_type> buffer_type;
//			{
//				typedef input_val_type input_value_type;
//				test_operator_set_combo_cont_t<buffer_type, input_value_type>();
//			}
//		}
//
//		{
//			typedef yggr::set<cont_val_type> buffer_type;
//			{
//				typedef input_val_type input_value_type;
//				test_operator_set_combo_cont_t<buffer_type, input_value_type>();
//			}
//		}
//	}
//
//	// multiset
//	{
//		{
//			typedef std::multiset<cont_val_type> buffer_type;
//			{
//				typedef input_val_type input_value_type;
//				test_operator_set_combo_cont_t<buffer_type, input_value_type>();
//			}
//		}
//
//		{
//			typedef boost::container::multiset<cont_val_type> buffer_type;
//			{
//				typedef input_val_type input_value_type;
//				test_operator_set_combo_cont_t<buffer_type, input_value_type>();
//			}
//		}
//
//		{
//			typedef yggr::multiset<cont_val_type> buffer_type;
//			{
//				typedef input_val_type input_value_type;
//				test_operator_set_combo_cont_t<buffer_type, input_value_type>();
//			}
//		}
//	}
//
//	// unordered_set
//	{
//#		if !defined(YGGR_NO_CXX11_HDR_UNORDERED_SET)
//		{
//			typedef std::unordered_set<cont_val_type> buffer_type;
//			{
//				typedef input_val_type input_value_type;
//				test_operator_set_combo_cont_t<buffer_type, input_value_type>();
//			}
//		}
//#		endif // YGGR_NO_CXX11_HDR_UNORDERED_SET
//
//		{
//			typedef boost::unordered_set<cont_val_type> buffer_type;
//			{
//				typedef input_val_type input_value_type;
//				test_operator_set_combo_cont_t<buffer_type, input_value_type>();
//			}
//		}
//
//		{
//			typedef yggr::unordered_set<cont_val_type> buffer_type;
//			{
//				typedef input_val_type input_value_type;
//				test_operator_set_combo_cont_t<buffer_type, input_value_type>();
//			}
//		}
//	}
//
//	// unordered_multiset
//	{
//#		if !defined(YGGR_NO_CXX11_HDR_UNORDERED_SET)
//		{
//			typedef std::unordered_multiset<cont_val_type> buffer_type;
//			{
//				typedef input_val_type input_value_type;
//				test_operator_set_combo_cont_t<buffer_type, input_value_type>();
//			}
//		}
//#		endif // YGGR_NO_CXX11_HDR_UNORDERED_SET
//
//		{
//			typedef boost::unordered_multiset<cont_val_type> buffer_type;
//			{
//				typedef input_val_type input_value_type;
//				test_operator_set_combo_cont_t<buffer_type, input_value_type>();
//			}
//		}
//
//		{
//			typedef yggr::unordered_multiset<cont_val_type> buffer_type;
//			{
//				typedef input_val_type input_value_type;
//				test_operator_set_combo_cont_t<buffer_type, input_value_type>();
//			}
//		}
//	}

	std::cout
		<< "-------------test_operator_set_combe [ "
		<< typeid(cont_val_type).name()
		<< ", "
		<< typeid(input_val_type).name()
		<< " ] end---------------\n"
		<< std::endl;
}

void test_operator_set(void)
{
	{
		test_operator_set_combe<yggr::u8, yggr::u8>();
		test_operator_set_combe<yggr::u8, yggr::u16>();
		test_operator_set_combe<yggr::u8, yggr::u32>();
	}

	{
		test_operator_set_combe<yggr::u16, yggr::u8>();
		test_operator_set_combe<yggr::u16, yggr::u16>();
		test_operator_set_combe<yggr::u16, yggr::u32>();
	}

	{
		test_operator_set_combe<yggr::u32, yggr::u8>();
		test_operator_set_combe<yggr::u32, yggr::u16>();
		test_operator_set_combe<yggr::u32, yggr::u32>();
	}

	std::cout << "-------------test_operator_set end---------------" << std::endl;
}

int main(int argc, char* argv[])
{
	test_operator_set();

	wait_any_key(argc, argv);
	return 0;
}
