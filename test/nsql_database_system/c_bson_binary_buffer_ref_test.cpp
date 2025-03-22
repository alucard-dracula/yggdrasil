//c_bson_binary_buffer_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <typeinfo>
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

template<typename BufCont> inline
void test_construct_detail(BufCont& init_data, const BufCont& chk_data)
{
	typedef BufCont buf_cont_type;
	typedef yggr::nsql_database_system::c_bson_binary_buffer_ref<0, buf_cont_type> bson_buffer_ref_type;

	bson_buffer_ref_type data(init_data);

	std::cout << data << std::endl;

	yggr_test_assert(data == chk_data);

	bson_buffer_ref_type data2 = data;
	yggr_test_assert(data == data2);
}


void test_construct(void)
{
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;

	{
		typedef int cont_type[3];

		cont_type cont = {1, 2, 3};
		test_construct_detail(cont, cont);
	}

	{
		typedef boost::array<int, 3> cont_type;

		cont_type cont = {1, 2, 3};
		test_construct_detail(cont, cont);
	}

	// vetor
	{
		{
			typedef std::vector<int> cont_type;

			int org_cont[] = {1, 2, 3};
			cont_type cont(org_cont, org_cont + 3);
			test_construct_detail(cont, cont);
		}

		{
			typedef boost::container::vector<int> cont_type;

			int org_cont[] = {1, 2, 3};
			cont_type cont(org_cont, org_cont + 3);
			test_construct_detail(cont, cont);
		}

		{
			typedef yggr::vector<int> cont_type;

			int org_cont[] = {1, 2, 3};
			cont_type cont(org_cont, org_cont + 3);
			test_construct_detail(cont, cont);
		}
	}

	// list
	{
		{
			typedef std::list<int> cont_type;

			int org_cont[] = {1, 2, 3};
			cont_type cont(org_cont, org_cont + 3);
			test_construct_detail(cont, cont);
		}

		{
			typedef boost::container::list<int> cont_type;

			int org_cont[] = {1, 2, 3};
			cont_type cont(org_cont, org_cont + 3);
			test_construct_detail(cont, cont);
		}

		{
			typedef yggr::list<int> cont_type;

			int org_cont[] = {1, 2, 3};
			cont_type cont(org_cont, org_cont + 3);
			test_construct_detail(cont, cont);
		}
	}

	// set
	{
		{
			typedef std::set<int> cont_type;

			int org_cont[] = {1, 2, 3};
			cont_type cont(org_cont, org_cont + 3);
			test_construct_detail(cont, cont);
		}

		{
			typedef boost::container::set<int> cont_type;

			int org_cont[] = {1, 2, 3};
			cont_type cont(org_cont, org_cont + 3);
			test_construct_detail(cont, cont);
		}

		{
			typedef yggr::set<int> cont_type;

			int org_cont[] = {1, 2, 3};
			cont_type cont(org_cont, org_cont + 3);
			test_construct_detail(cont, cont);
		}
	}

	// multiset
	{
		{
			typedef std::multiset<int> cont_type;

			int org_cont[] = {1, 2, 3};
			cont_type cont(org_cont, org_cont + 3);
			test_construct_detail(cont, cont);
		}

		{
			typedef boost::container::multiset<int> cont_type;

			int org_cont[] = {1, 2, 3};
			cont_type cont(org_cont, org_cont + 3);
			test_construct_detail(cont, cont);
		}

		{
			typedef yggr::multiset<int> cont_type;

			int org_cont[] = {1, 2, 3};
			cont_type cont(org_cont, org_cont + 3);
			test_construct_detail(cont, cont);
		}
	}

	// unordered_set
	{
#		if !defined(YGGR_NO_CXX11_HDR_UNORDERED_SET)
		{
			typedef std::unordered_set<int> cont_type;

			int org_cont[] = {1, 2, 3};
			cont_type cont(org_cont, org_cont + 3);
			test_construct_detail(cont, cont);
		}
#		endif // YGGR_NO_CXX11_HDR_UNORDERED_SET

		{
			typedef boost::unordered_set<int> cont_type;

			int org_cont[] = {1, 2, 3};
			cont_type cont(org_cont, org_cont + 3);
			test_construct_detail(cont, cont);
		}

		{
			typedef yggr::unordered_set<int> cont_type;

			int org_cont[] = {1, 2, 3};
			cont_type cont(org_cont, org_cont + 3);
			test_construct_detail(cont, cont);
		}
	}

	// unordered_multiset
	{
#		if !defined(YGGR_NO_CXX11_HDR_UNORDERED_SET)
		{
			typedef std::unordered_multiset<int> cont_type;

			int org_cont[] = {1, 2, 3};
			cont_type cont(org_cont, org_cont + 3);
			test_construct_detail(cont, cont);
		}
#		endif // YGGR_NO_CXX11_HDR_UNORDERED_SET

		{
			typedef boost::unordered_multiset<int> cont_type;

			int org_cont[] = {1, 2, 3};
			cont_type cont(org_cont, org_cont + 3);
			test_construct_detail(cont, cont);
		}

		{
			typedef yggr::unordered_multiset<int> cont_type;

			int org_cont[] = {1, 2, 3};
			cont_type cont(org_cont, org_cont + 3);
			test_construct_detail(cont, cont);
		}
	}

	std::cout << "-------------test_construct end---------------" << std::endl;
}


template<typename BufCont, typename InputCont> inline
void test_operator_set_detail(BufCont& init_data, const InputCont& input)
{
	typedef BufCont buf_cont_type;
	typedef InputCont input_cont_type;
	typedef yggr::nsql_database_system::c_bson_binary_buffer_ref<0, buf_cont_type> bson_buffer_ref_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;

	{
		bson_buffer_ref_type data(init_data);
		data = input;

		std::cout << data << std::endl;

		yggr_test_assert(data == input);
	}

	{
		typedef yggr::nsql_database_system::c_bson_binary_buffer_ref<0, input_cont_type> input_bson_buffer_ref_type;
		input_bson_buffer_ref_type org_data(const_cast<input_cont_type&>(input));
		bson_value_type org_val(org_data);
		bson_buffer_ref_type data(init_data);
		data = org_val;

		std::cout << data << std::endl;

		yggr_test_assert(data == input);
	}
}

// more detailed see c_bson_binary_buffer_ref_operator_set_test_xxx.cpp series
void test_operator_set(void)
{
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;

	{
		typedef int cont_type[3];
		cont_type init_cont = {0};

		{
			typedef int input_type[3];
			input_type input = {1, 2, 3};

			test_operator_set_detail(init_cont, input);
		}

		// array
		{
			typedef boost::array<int, 3> input_type;
			input_type input = {1, 2, 3};
			test_operator_set_detail(init_cont, input);
		}

		// vector
		{
			{
				typedef std::vector<int> input_type;

				int org_input[] = {1, 2, 3};
				input_type input(org_input, org_input + 3);
				test_operator_set_detail(init_cont, input);
			}

			{
				typedef boost::container::vector<int> input_type;

				int org_input[] = {1, 2, 3};
				input_type input(org_input, org_input + 3);
				test_operator_set_detail(init_cont, input);
			}

			{
				typedef yggr::vector<int> input_type;

				int org_input[] = {1, 2, 3};
				input_type input(org_input, org_input + 3);
				test_operator_set_detail(init_cont, input);
			}
		}

		// list
		{
			{
				typedef std::list<int> input_type;

				int org_input[] = {1, 2, 3};
				input_type input(org_input, org_input + 3);
				test_operator_set_detail(init_cont, input);
			}

			{
				typedef boost::container::list<int> input_type;

				int org_input[] = {1, 2, 3};
				input_type input(org_input, org_input + 3);
				test_operator_set_detail(init_cont, input);
			}

			{
				typedef yggr::list<int> input_type;

				int org_input[] = {1, 2, 3};
				input_type input(org_input, org_input + 3);
				test_operator_set_detail(init_cont, input);
			}
		}

		// set
		{
			{
				typedef std::set<int> input_type;

				int org_input[] = {1, 2, 3};
				input_type input(org_input, org_input + 3);
				test_operator_set_detail(init_cont, input);
			}

			{
				typedef boost::container::set<int> input_type;

				int org_input[] = {1, 2, 3};
				input_type input(org_input, org_input + 3);
				test_operator_set_detail(init_cont, input);
			}

			{
				typedef yggr::set<int> input_type;

				int org_input[] = {1, 2, 3};
				input_type input(org_input, org_input + 3);
				test_operator_set_detail(init_cont, input);
			}
		}

		// multiset
		{
			{
				typedef std::multiset<int> input_type;

				int org_input[] = {1, 2, 3};
				input_type input(org_input, org_input + 3);
				test_operator_set_detail(init_cont, input);
			}

			{
				typedef boost::container::multiset<int> input_type;

				int org_input[] = {1, 2, 3};
				input_type input(org_input, org_input + 3);
				test_operator_set_detail(init_cont, input);
			}

			{
				typedef yggr::multiset<int> input_type;

				int org_input[] = {1, 2, 3};
				input_type input(org_input, org_input + 3);
				test_operator_set_detail(init_cont, input);
			}
		}

		// unordered_set
		{

#		if !defined(YGGR_NO_CXX11_HDR_UNORDERED_SET)
			{
				typedef std::unordered_set<int> input_type;

				int org_input[] = {1, 2, 3};
				input_type input(org_input, org_input + 3);
				test_operator_set_detail(init_cont, input);
			}
#		endif // YGGR_NO_CXX11_HDR_UNORDERED_SET

			{
				typedef boost::unordered_set<int> input_type;

				int org_input[] = {1, 2, 3};
				input_type input(org_input, org_input + 3);
				test_operator_set_detail(init_cont, input);
			}

			{
				typedef yggr::unordered_set<int> input_type;

				int org_input[] = {1, 2, 3};
				input_type input(org_input, org_input + 3);
				test_operator_set_detail(init_cont, input);
			}
		}

		// unordered_multiset
		{

#		if !defined(YGGR_NO_CXX11_HDR_UNORDERED_SET)
			{
				typedef std::unordered_multiset<int> input_type;

				int org_input[] = {1, 2, 3};
				input_type input(org_input, org_input + 3);
				test_operator_set_detail(init_cont, input);
			}
#		endif // YGGR_NO_CXX11_HDR_UNORDERED_SET

			{
				typedef boost::unordered_multiset<int> input_type;

				int org_input[] = {1, 2, 3};
				input_type input(org_input, org_input + 3);
				test_operator_set_detail(init_cont, input);
			}

			{
				typedef yggr::unordered_multiset<int> input_type;

				int org_input[] = {1, 2, 3};
				input_type input(org_input, org_input + 3);
				test_operator_set_detail(init_cont, input);
			}
		}
	}

	std::cout << "-------------test_operator_set end---------------" << std::endl;
}

template<typename BufCont> inline
void test_swap_detail(BufCont& init_data1, BufCont& init_data2,
						const BufCont& chk_data1, const BufCont& chk_data2)
{
	typedef BufCont buf_cont_type;
	typedef yggr::nsql_database_system::c_bson_binary_buffer_ref<0, buf_cont_type> bson_buffer_ref_type;

	{
		bson_buffer_ref_type data1(init_data1);
		bson_buffer_ref_type data2(init_data2);
		data1.swap(data2.buffer());

		yggr_test_assert(data2 == chk_data1);
		yggr_test_assert(data1 == chk_data2);
	}

	{
		bson_buffer_ref_type data1(init_data1);
		bson_buffer_ref_type data2(init_data2);
		data1.swap(boost::move(data2));

		yggr_test_assert(data2 == chk_data1);
		yggr_test_assert(data1 == chk_data2);
	}

	{
		bson_buffer_ref_type data1(init_data1);
		bson_buffer_ref_type data2(init_data2);
		data1.swap(data2);

		yggr_test_assert(data2 == chk_data1);
		yggr_test_assert(data1 == chk_data2);
	}

	{
		bson_buffer_ref_type data1(init_data1);
		bson_buffer_ref_type data2(init_data2);

		std::swap(data1, data2.buffer());
		yggr_test_assert(data2 == chk_data1);
		yggr_test_assert(data1 == chk_data2);

		std::swap(data1.buffer(), data2);
		yggr_test_assert(data1 == chk_data1);
		yggr_test_assert(data2 == chk_data2);

		boost::swap(data1, data2.buffer());
		yggr_test_assert(data2 == chk_data1);
		yggr_test_assert(data1 == chk_data2);

		boost::swap(data1.buffer(), data2);
		yggr_test_assert(data1 == chk_data1);
		yggr_test_assert(data2 == chk_data2);

		yggr::swap(data1, data2.buffer());
		yggr_test_assert(data2 == chk_data1);
		yggr_test_assert(data1 == chk_data2);

		yggr::swap(data1.buffer(), data2);
		yggr_test_assert(data1 == chk_data1);
		yggr_test_assert(data2 == chk_data2);
	}

	{
		bson_buffer_ref_type data1(init_data1);
		bson_buffer_ref_type data2(init_data2);

		std::swap(data1, boost::move(data2));
		yggr_test_assert(data2 == chk_data1);
		yggr_test_assert(data1 == chk_data2);

		std::swap(boost::move(data1), data2);
		yggr_test_assert(data1 == chk_data1);
		yggr_test_assert(data2 == chk_data2);

		boost::swap(data1, boost::move(data2));
		yggr_test_assert(data2 == chk_data1);
		yggr_test_assert(data1 == chk_data2);

		boost::swap(boost::move(data1), data2);
		yggr_test_assert(data1 == chk_data1);
		yggr_test_assert(data2 == chk_data2);

		yggr::swap(data1, boost::move(data2));
		yggr_test_assert(data2 == chk_data1);
		yggr_test_assert(data1 == chk_data2);

		yggr::swap(boost::move(data1), data2);
		yggr_test_assert(data1 == chk_data1);
		yggr_test_assert(data2 == chk_data2);
	}

	{
		bson_buffer_ref_type data1(init_data1);
		bson_buffer_ref_type data2(init_data2);

		std::swap(data1, data2);
		yggr_test_assert(data2 == chk_data1);
		yggr_test_assert(data1 == chk_data2);

		boost::swap(data1, data2);
		yggr_test_assert(data1 == chk_data1);
		yggr_test_assert(data2 == chk_data2);

		yggr::swap(data1, data2);
		yggr_test_assert(data2 == chk_data1);
		yggr_test_assert(data1 == chk_data2);
	}
}

template<typename BufCont> inline
typename boost::enable_if<boost::is_array<BufCont>, void>::type
	test_swap_detail_sel(BufCont& init_data1, BufCont& init_data2)
{
	typedef BufCont buf_cont_type;
	typedef yggr::nsql_database_system::c_bson_binary_buffer_ref<0, buf_cont_type> bson_buffer_ref_type;

	buf_cont_type chk_data1 = {0};
	buf_cont_type chk_data2 = {0};

	memcpy(boost::addressof(chk_data1[0]), boost::addressof(init_data1[0]), sizeof(buf_cont_type));
	memcpy(boost::addressof(chk_data2[0]), boost::addressof(init_data2[0]), sizeof(buf_cont_type));

	test_swap_detail(init_data1, init_data2, chk_data1, chk_data2);
}

template<typename BufCont> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			yggr::is_static_container<BufCont>,
			boost::mpl::not_<boost::is_array<BufCont> >
		>,
		void
	>::type
	test_swap_detail_sel(BufCont& init_data1, BufCont& init_data2)
{
	typedef BufCont buf_cont_type;
	typedef yggr::nsql_database_system::c_bson_binary_buffer_ref<0, buf_cont_type> bson_buffer_ref_type;

	buf_cont_type chk_data1 = {0};
	buf_cont_type chk_data2 = {0};

	memcpy(boost::addressof(chk_data1[0]), boost::addressof(init_data1[0]), yggr::byte_size(init_data1));
	memcpy(boost::addressof(chk_data2[0]), boost::addressof(init_data2[0]), yggr::byte_size(init_data2));

	test_swap_detail(init_data1, init_data2, chk_data1, chk_data2);

	memcpy(boost::addressof(init_data1[0]), boost::addressof(chk_data1[0]), yggr::byte_size(chk_data1));
	memcpy(boost::addressof(init_data2[0]), boost::addressof(chk_data2[0]), yggr::byte_size(chk_data2));

	{
		bson_buffer_ref_type data1(init_data1);
		bson_buffer_ref_type data2(init_data2);
		data1.swap(boost::move(data2.buffer()));

		yggr_test_assert(data2 == chk_data1);
		yggr_test_assert(data1 == chk_data2);
	}
}

template<typename BufCont> inline
typename boost::disable_if<yggr::is_static_container<BufCont>, void>::type
	test_swap_detail_sel(BufCont& init_data1, BufCont& init_data2)
{
	typedef BufCont buf_cont_type;
	typedef yggr::nsql_database_system::c_bson_binary_buffer_ref<0, buf_cont_type> bson_buffer_ref_type;

	buf_cont_type chk_data1(init_data1);
	buf_cont_type chk_data2(init_data2);

	test_swap_detail(init_data1, init_data2, chk_data1, chk_data2);

	init_data1 = chk_data1;
	init_data2 = chk_data2;

	{
		bson_buffer_ref_type data1(init_data1);
		bson_buffer_ref_type data2(init_data2);
		data1.swap(boost::move(data2.buffer()));

		yggr_test_assert(data2 == chk_data1);
		yggr_test_assert(data1 == chk_data2);
	}
}

void test_swap(void)
{
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;
	typedef yggr::nsql_database_system::c_bson_binary_buffer<0> bson_buffer_type;

	{
		typedef int buf_type[3];
		buf_type buf1 = {1, 2, 3};
		buf_type buf2 = {4, 5, 6};

		test_swap_detail_sel(buf1, buf2);
	}

	{
		typedef boost::array<int, 3> buf_type;
		buf_type buf1 = {1, 2, 3};
		buf_type buf2 = {4, 5, 6};

		test_swap_detail_sel(buf1, buf2);
	}

	// vector
	{
		{
			typedef std::vector<int> buf_type;
			int org_buf1[] = {1, 2, 3};
			int org_buf2[] = {4, 5, 6};

			buf_type buf1(org_buf1, org_buf1 + 3);
			buf_type buf2(org_buf2, org_buf2 + 3);

			test_swap_detail_sel(buf1, buf2);
		}

		{
			typedef boost::container::vector<int> buf_type;
			int org_buf1[] = {1, 2, 3};
			int org_buf2[] = {4, 5, 6};

			buf_type buf1(org_buf1, org_buf1 + 3);
			buf_type buf2(org_buf2, org_buf2 + 3);

			test_swap_detail_sel(buf1, buf2);
		}

		{
			typedef yggr::vector<int> buf_type;
			int org_buf1[] = {1, 2, 3};
			int org_buf2[] = {4, 5, 6};

			buf_type buf1(org_buf1, org_buf1 + 3);
			buf_type buf2(org_buf2, org_buf2 + 3);

			test_swap_detail_sel(buf1, buf2);
		}
	}

	// list
	{
		{
			typedef std::list<int> buf_type;
			int org_buf1[] = {1, 2, 3};
			int org_buf2[] = {4, 5, 6};

			buf_type buf1(org_buf1, org_buf1 + 3);
			buf_type buf2(org_buf2, org_buf2 + 3);

			test_swap_detail_sel(buf1, buf2);
		}

		{
			typedef boost::container::list<int> buf_type;
			int org_buf1[] = {1, 2, 3};
			int org_buf2[] = {4, 5, 6};

			buf_type buf1(org_buf1, org_buf1 + 3);
			buf_type buf2(org_buf2, org_buf2 + 3);

			test_swap_detail_sel(buf1, buf2);
		}

		{
			typedef yggr::list<int> buf_type;
			int org_buf1[] = {1, 2, 3};
			int org_buf2[] = {4, 5, 6};

			buf_type buf1(org_buf1, org_buf1 + 3);
			buf_type buf2(org_buf2, org_buf2 + 3);

			test_swap_detail_sel(buf1, buf2);
		}
	}

	// set
	{
		{
			typedef std::set<int> buf_type;
			int org_buf1[] = {1, 2, 3};
			int org_buf2[] = {4, 5, 6};

			buf_type buf1(org_buf1, org_buf1 + 3);
			buf_type buf2(org_buf2, org_buf2 + 3);

			test_swap_detail_sel(buf1, buf2);
		}

		{
			typedef boost::container::set<int> buf_type;
			int org_buf1[] = {1, 2, 3};
			int org_buf2[] = {4, 5, 6};

			buf_type buf1(org_buf1, org_buf1 + 3);
			buf_type buf2(org_buf2, org_buf2 + 3);

			test_swap_detail_sel(buf1, buf2);
		}

		{
			typedef yggr::set<int> buf_type;
			int org_buf1[] = {1, 2, 3};
			int org_buf2[] = {4, 5, 6};

			buf_type buf1(org_buf1, org_buf1 + 3);
			buf_type buf2(org_buf2, org_buf2 + 3);

			test_swap_detail_sel(buf1, buf2);
		}
	}

	// multiset
	{
		{
			typedef std::multiset<int> buf_type;
			int org_buf1[] = {1, 2, 3};
			int org_buf2[] = {4, 5, 6};

			buf_type buf1(org_buf1, org_buf1 + 3);
			buf_type buf2(org_buf2, org_buf2 + 3);

			test_swap_detail_sel(buf1, buf2);
		}

		{
			typedef boost::container::multiset<int> buf_type;
			int org_buf1[] = {1, 2, 3};
			int org_buf2[] = {4, 5, 6};

			buf_type buf1(org_buf1, org_buf1 + 3);
			buf_type buf2(org_buf2, org_buf2 + 3);

			test_swap_detail_sel(buf1, buf2);
		}

		{
			typedef yggr::multiset<int> buf_type;
			int org_buf1[] = {1, 2, 3};
			int org_buf2[] = {4, 5, 6};

			buf_type buf1(org_buf1, org_buf1 + 3);
			buf_type buf2(org_buf2, org_buf2 + 3);

			test_swap_detail_sel(buf1, buf2);
		}
	}

	// unordered_set
	{
#	if !defined(YGGR_NO_CXX11_HDR_UNORDERED_SET)
		{
			typedef std::unordered_set<int> buf_type;
			int org_buf1[] = {1, 2, 3};
			int org_buf2[] = {4, 5, 6};

			buf_type buf1(org_buf1, org_buf1 + 3);
			buf_type buf2(org_buf2, org_buf2 + 3);

			test_swap_detail_sel(buf1, buf2);
		}
#	endif // YGGR_NO_CXX11_HDR_UNORDERED_SET

		{
			typedef boost::unordered_set<int> buf_type;
			int org_buf1[] = {1, 2, 3};
			int org_buf2[] = {4, 5, 6};

			buf_type buf1(org_buf1, org_buf1 + 3);
			buf_type buf2(org_buf2, org_buf2 + 3);

			test_swap_detail_sel(buf1, buf2);
		}

		{
			typedef yggr::unordered_set<int> buf_type;
			int org_buf1[] = {1, 2, 3};
			int org_buf2[] = {4, 5, 6};

			buf_type buf1(org_buf1, org_buf1 + 3);
			buf_type buf2(org_buf2, org_buf2 + 3);

			test_swap_detail_sel(buf1, buf2);
		}
	}

	// unordered_multiset
	{
#	if !defined(YGGR_NO_CXX11_HDR_UNORDERED_SET)
		{
			typedef std::unordered_multiset<int> buf_type;
			int org_buf1[] = {1, 2, 3};
			int org_buf2[] = {4, 5, 6};

			buf_type buf1(org_buf1, org_buf1 + 3);
			buf_type buf2(org_buf2, org_buf2 + 3);

			test_swap_detail_sel(buf1, buf2);
		}
#	endif // YGGR_NO_CXX11_HDR_UNORDERED_SET

		{
			typedef boost::unordered_multiset<int> buf_type;
			int org_buf1[] = {1, 2, 3};
			int org_buf2[] = {4, 5, 6};

			buf_type buf1(org_buf1, org_buf1 + 3);
			buf_type buf2(org_buf2, org_buf2 + 3);

			test_swap_detail_sel(buf1, buf2);
		}

		{
			typedef yggr::unordered_multiset<int> buf_type;
			int org_buf1[] = {1, 2, 3};
			int org_buf2[] = {4, 5, 6};

			buf_type buf1(org_buf1, org_buf1 + 3);
			buf_type buf2(org_buf2, org_buf2 + 3);

			test_swap_detail_sel(buf1, buf2);
		}
	}

	std::cout << "-------------test_swap end---------------" << std::endl;
}

template<typename BufCont>
void test_member_foo_detail_some(BufCont& init_data)
{
	typedef BufCont buf_cont_type;
	typedef yggr::nsql_database_system::c_bson_binary_buffer_ref<0, buf_cont_type> bson_buffer_ref_type;

	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;

	bson_buffer_ref_type data(init_data);

	{
		bson_buffer_ref_type data(init_data);

		yggr_test_assert(data.size() == yggr::byte_size(init_data) );
		yggr_test_assert(data.empty() == yggr::container::empty(init_data) );
		yggr_test_assert(std::equal(boost::begin(init_data), boost::end(init_data), boost::begin(data.buffer())));
		yggr_test_assert(boost::addressof(init_data) == boost::addressof(data.buffer()));
	}

	// assign
	{
		{
			bson_buffer_ref_type tmp_data(init_data);
			bson_value_type bs_val(tmp_data);

			buf_cont_type tmp_cont;
			bson_buffer_ref_type data(tmp_cont);
			data.assign(bs_val);

			yggr_test_assert(data == init_data);
		}

		{
			buf_cont_type tmp_cont;
			bson_buffer_ref_type data(tmp_cont);
			data.assign(init_data);

			yggr_test_assert(data == init_data);
		}

		{
			buf_cont_type tmp_cont;
			bson_buffer_ref_type data(tmp_cont);
			data.assign(boost::begin(init_data), boost::end(init_data));

			yggr_test_assert(data == init_data);
		}

		{
			bson_buffer_ref_type tmp_data(init_data);

			buf_cont_type tmp_cont;
			bson_buffer_ref_type data(tmp_cont);
			data.assign(tmp_data);

			yggr_test_assert(data == init_data);
		}
	}

	// save load
	{
		bson_type bs;
		bs.save("c_bson_binary_buffer_ref", data);

		std::cout << bs << std::endl;

		buf_cont_type ld_cont;
		bson_buffer_ref_type ld_data(ld_cont);
		bson_type::iterator bs_iter(bs, "c_bson_binary_buffer_ref");
		bs_iter.load("c_bson_binary_buffer_ref", ld_data);

		yggr_test_assert(ld_data == data);
		yggr_test_assert(ld_data == init_data);
	}

}

template<typename Val>
void test_member_foo_detail_combe(void)
{
	typedef Val val_type;
	{
		typedef val_type buf_type[3];
		buf_type buf = {1, 2, 3};

		test_member_foo_detail_some(buf);
	}

	// array
	{
		typedef boost::array<val_type, 3> buf_type;
		buf_type buf = {1, 2, 3};

		test_member_foo_detail_some(buf);
	}

	// vector
	{
		{
			typedef std::vector<val_type> buf_type;
			val_type org_buf[] = {1, 2, 3};
			buf_type buf(org_buf, org_buf + 3);

			test_member_foo_detail_some(buf);
		}

		{
			typedef boost::container::vector<val_type> buf_type;
			val_type org_buf[] = {1, 2, 3};
			buf_type buf(org_buf, org_buf + 3);

			test_member_foo_detail_some(buf);
		}

		{
			typedef yggr::vector<val_type> buf_type;
			val_type org_buf[] = {1, 2, 3};
			buf_type buf(org_buf, org_buf + 3);

			test_member_foo_detail_some(buf);
		}
	}

	// list
	{
		{
			typedef std::list<val_type> buf_type;
			val_type org_buf[] = {1, 2, 3};
			buf_type buf(org_buf, org_buf + 3);

			test_member_foo_detail_some(buf);
		}

		{
			typedef boost::container::list<val_type> buf_type;
			val_type org_buf[] = {1, 2, 3};
			buf_type buf(org_buf, org_buf + 3);

			test_member_foo_detail_some(buf);
		}

		{
			typedef yggr::list<val_type> buf_type;
			val_type org_buf[] = {1, 2, 3};
			buf_type buf(org_buf, org_buf + 3);

			test_member_foo_detail_some(buf);
		}
	}

	// set
	{
		{
			typedef std::set<val_type> buf_type;
			val_type org_buf[] = {1, 2, 3};
			buf_type buf(org_buf, org_buf + 3);

			test_member_foo_detail_some(buf);
		}

		{
			typedef boost::container::set<val_type> buf_type;
			val_type org_buf[] = {1, 2, 3};
			buf_type buf(org_buf, org_buf + 3);

			test_member_foo_detail_some(buf);
		}

		{
			typedef yggr::set<val_type> buf_type;
			val_type org_buf[] = {1, 2, 3};
			buf_type buf(org_buf, org_buf + 3);

			test_member_foo_detail_some(buf);
		}
	}

	// multiset
	{
		{
			typedef std::multiset<val_type> buf_type;
			val_type org_buf[] = {1, 2, 3};
			buf_type buf(org_buf, org_buf + 3);

			test_member_foo_detail_some(buf);
		}

		{
			typedef boost::container::multiset<val_type> buf_type;
			val_type org_buf[] = {1, 2, 3};
			buf_type buf(org_buf, org_buf + 3);

			test_member_foo_detail_some(buf);
		}

		{
			typedef yggr::multiset<val_type> buf_type;
			val_type org_buf[] = {1, 2, 3};
			buf_type buf(org_buf, org_buf + 3);

			test_member_foo_detail_some(buf);
		}
	}

	// unordered_set
	{
#	if !defined(YGGR_NO_CXX11_HDR_UNORDERED_SET)
		{
			typedef std::unordered_set<val_type> buf_type;
			val_type org_buf[] = {1, 2, 3};
			buf_type buf(org_buf, org_buf + 3);

			test_member_foo_detail_some(buf);
		}
#	endif // YGGR_NO_CXX11_HDR_UNORDERED_SET

		{
			typedef boost::unordered_set<val_type> buf_type;
			val_type org_buf[] = {1, 2, 3};
			buf_type buf(org_buf, org_buf + 3);

			test_member_foo_detail_some(buf);
		}

		{
			typedef yggr::unordered_set<val_type> buf_type;
			val_type org_buf[] = {1, 2, 3};
			buf_type buf(org_buf, org_buf + 3);

			test_member_foo_detail_some(buf);
		}
	}

	// unordered_multiset
	{
#	if !defined(YGGR_NO_CXX11_HDR_UNORDERED_SET)
		{
			typedef std::unordered_multiset<val_type> buf_type;
			val_type org_buf[] = {1, 2, 3};
			buf_type buf(org_buf, org_buf + 3);

			test_member_foo_detail_some(buf);
		}
#	endif // YGGR_NO_CXX11_HDR_UNORDERED_SET

		{
			typedef boost::unordered_multiset<val_type> buf_type;
			val_type org_buf[] = {1, 2, 3};
			buf_type buf(org_buf, org_buf + 3);

			test_member_foo_detail_some(buf);
		}

		{
			typedef yggr::unordered_multiset<val_type> buf_type;
			val_type org_buf[] = {1, 2, 3};
			buf_type buf(org_buf, org_buf + 3);

			test_member_foo_detail_some(buf);
		}
	}
}

void test_member_foo(void)
{
	test_member_foo_detail_combe<yggr::u8>();
	test_member_foo_detail_combe<yggr::u16>();

	std::cout << "-------------test_member_foo end---------------" << std::endl;
}

void test_binart_buffer_ref(void)
{
	typedef yggr::nsql_database_system::c_bson bson_type;
	{
		typedef int cont_native_array_type[3];
		typedef boost::array<int, 3> cont_array_type;
		typedef std::vector<int> cont_vt_type;
		typedef std::list<int> cont_list_type;
		typedef std::set<int> cont_set_type;

		typedef cont_native_array_type inner_cont_type;
		typedef yggr::nsql_database_system::c_bson_binary_buffer_ref<0, inner_cont_type> buffer_type;


		cont_native_array_type cont_narr = {1, 2, 3};
		cont_array_type cont_arr = {1, 2, 3};
		cont_vt_type cont_vt(boost::begin(cont_narr), boost::end(cont_narr));
		cont_list_type cont_list(boost::begin(cont_narr), boost::end(cont_narr));
		cont_set_type cont_set(boost::begin(cont_narr), boost::end(cont_narr));

		inner_cont_type& inner_cont = cont_narr;

		bson_type bs;
		buffer_type buf(inner_cont);
		std::cout << buf << std::endl;
		bs.save("buf_name", buf);
		std::cout << bs << std::endl;

		inner_cont_type out_cont = {0};
		buffer_type lbuf(out_cont);
		bs.begin().load("buf_name", lbuf);

		yggr_test_assert(buf == lbuf);
		yggr_test_assert(!(buf != lbuf));

		yggr_test_assert(buf == out_cont);
		yggr_test_assert(!(buf != out_cont));
		yggr_test_assert(out_cont == buf);
		yggr_test_assert(!(out_cont != buf));

		yggr_test_assert(lbuf == cont_narr);
		yggr_test_assert(!(lbuf != cont_narr));
		yggr_test_assert(cont_narr == lbuf);
		yggr_test_assert(!(cont_narr != lbuf));

		yggr_test_assert(buf == cont_arr);
		yggr_test_assert(cont_arr == buf);
		yggr_test_assert(!(buf != cont_arr));
		yggr_test_assert(!(cont_arr != buf));

		yggr_test_assert(buf == cont_vt);
		yggr_test_assert(cont_vt == buf);
		yggr_test_assert(!(buf != cont_vt));
		yggr_test_assert(!(cont_vt != buf));

		yggr_test_assert(buf == cont_list);
		yggr_test_assert(cont_list == buf);
		yggr_test_assert(!(buf != cont_list));
		yggr_test_assert(!(cont_list != buf));

		yggr_test_assert(buf == cont_set);
		yggr_test_assert(cont_set == buf);
		yggr_test_assert(!(buf != cont_set));
		yggr_test_assert(!(cont_set != buf));
	}

	{
		typedef int cont_native_array_type[3];
		typedef boost::array<int, 3> cont_array_type;
		typedef std::vector<int> cont_vt_type;
		typedef std::list<int> cont_list_type;
		typedef std::set<int> cont_set_type;

		typedef cont_array_type inner_cont_type;
		typedef yggr::nsql_database_system::c_bson_binary_buffer_ref<0, inner_cont_type> buffer_type;


		cont_native_array_type cont_narr = {1, 2, 3};
		cont_array_type cont_arr = {1, 2, 3};
		cont_vt_type cont_vt(boost::begin(cont_narr), boost::end(cont_narr));
		cont_list_type cont_list(boost::begin(cont_narr), boost::end(cont_narr));
		cont_set_type cont_set(boost::begin(cont_narr), boost::end(cont_narr));

		inner_cont_type& inner_cont = cont_arr;

		bson_type bs;
		buffer_type buf(inner_cont);
		std::cout << buf << std::endl;
		bs.save("buf_name", buf);
		std::cout << bs << std::endl;

		inner_cont_type out_cont = {0};
		buffer_type lbuf(out_cont);
		bs.begin().load("buf_name", lbuf);

		yggr_test_assert(buf == lbuf);
		yggr_test_assert(!(buf != lbuf));

		yggr_test_assert(buf == out_cont);
		yggr_test_assert(!(buf != out_cont));
		yggr_test_assert(out_cont == buf);
		yggr_test_assert(!(out_cont != buf));

		yggr_test_assert(lbuf == cont_narr);
		yggr_test_assert(!(lbuf != cont_narr));
		yggr_test_assert(cont_narr == lbuf);
		yggr_test_assert(!(cont_narr != lbuf));

		yggr_test_assert(buf == cont_arr);
		yggr_test_assert(cont_arr == buf);
		yggr_test_assert(!(buf != cont_arr));
		yggr_test_assert(!(cont_arr != buf));

		yggr_test_assert(buf == cont_vt);
		yggr_test_assert(cont_vt == buf);
		yggr_test_assert(!(buf != cont_vt));
		yggr_test_assert(!(cont_vt != buf));

		yggr_test_assert(buf == cont_list);
		yggr_test_assert(cont_list == buf);
		yggr_test_assert(!(buf != cont_list));
		yggr_test_assert(!(cont_list != buf));

		yggr_test_assert(buf == cont_set);
		yggr_test_assert(cont_set == buf);
		yggr_test_assert(!(buf != cont_set));
		yggr_test_assert(!(cont_set != buf));

	}

	{
		typedef int cont_native_array_type[3];
		typedef boost::array<int, 3> cont_array_type;
		typedef std::vector<int> cont_vt_type;
		typedef std::list<int> cont_list_type;
		typedef std::set<int> cont_set_type;

		typedef cont_vt_type inner_cont_type;
		typedef yggr::nsql_database_system::c_bson_binary_buffer_ref<0, inner_cont_type> buffer_type;


		cont_native_array_type cont_narr = {1, 2, 3};
		cont_array_type cont_arr = {1, 2, 3};
		cont_vt_type cont_vt(boost::begin(cont_narr), boost::end(cont_narr));
		cont_list_type cont_list(boost::begin(cont_narr), boost::end(cont_narr));
		cont_set_type cont_set(boost::begin(cont_narr), boost::end(cont_narr));

		inner_cont_type& inner_cont = cont_vt;

		bson_type bs;
		buffer_type buf(inner_cont);
		std::cout << buf << std::endl;
		bs.save("buf_name", buf);
		std::cout << bs << std::endl;

		inner_cont_type out_cont;
		buffer_type lbuf(out_cont);
		bs.begin().load("buf_name", lbuf);

		yggr_test_assert(buf == lbuf);
		yggr_test_assert(!(buf != lbuf));

		yggr_test_assert(buf == out_cont);
		yggr_test_assert(!(buf != out_cont));
		yggr_test_assert(out_cont == buf);
		yggr_test_assert(!(out_cont != buf));

		yggr_test_assert(lbuf == cont_narr);
		yggr_test_assert(!(lbuf != cont_narr));
		yggr_test_assert(cont_narr == lbuf);
		yggr_test_assert(!(cont_narr != lbuf));

		yggr_test_assert(buf == cont_arr);
		yggr_test_assert(cont_arr == buf);
		yggr_test_assert(!(buf != cont_arr));
		yggr_test_assert(!(cont_arr != buf));

		yggr_test_assert(buf == cont_vt);
		yggr_test_assert(cont_vt == buf);
		yggr_test_assert(!(buf != cont_vt));
		yggr_test_assert(!(cont_vt != buf));

		yggr_test_assert(buf == cont_list);
		yggr_test_assert(cont_list == buf);
		yggr_test_assert(!(buf != cont_list));
		yggr_test_assert(!(cont_list != buf));

		yggr_test_assert(buf == cont_set);
		yggr_test_assert(cont_set == buf);
		yggr_test_assert(!(buf != cont_set));
		yggr_test_assert(!(cont_set != buf));
	}

	{
		typedef int cont_native_array_type[3];
		typedef boost::array<int, 3> cont_array_type;
		typedef std::vector<int> cont_vt_type;
		typedef std::list<int> cont_list_type;
		typedef std::set<int> cont_set_type;

		typedef cont_list_type inner_cont_type;
		typedef yggr::nsql_database_system::c_bson_binary_buffer_ref<0, inner_cont_type> buffer_type;


		cont_native_array_type cont_narr = {1, 2, 3};
		cont_array_type cont_arr = {1, 2, 3};
		cont_vt_type cont_vt(boost::begin(cont_narr), boost::end(cont_narr));
		cont_list_type cont_list(boost::begin(cont_narr), boost::end(cont_narr));
		cont_set_type cont_set(boost::begin(cont_narr), boost::end(cont_narr));

		inner_cont_type& inner_cont = cont_list;

		bson_type bs;
		buffer_type buf(inner_cont);
		std::cout << buf << std::endl;
		bs.save("buf_name", buf);
		std::cout << bs << std::endl;

		inner_cont_type out_cont;
		buffer_type lbuf(out_cont);
		bs.begin().load("buf_name", lbuf);

		yggr_test_assert(buf == lbuf);
		yggr_test_assert(!(buf != lbuf));

		yggr_test_assert(buf == out_cont);
		yggr_test_assert(!(buf != out_cont));
		yggr_test_assert(out_cont == buf);
		yggr_test_assert(!(out_cont != buf));

		yggr_test_assert(lbuf == cont_narr);
		yggr_test_assert(!(lbuf != cont_narr));
		yggr_test_assert(cont_narr == lbuf);
		yggr_test_assert(!(cont_narr != lbuf));

		yggr_test_assert(buf == cont_arr);
		yggr_test_assert(cont_arr == buf);
		yggr_test_assert(!(buf != cont_arr));
		yggr_test_assert(!(cont_arr != buf));

		yggr_test_assert(buf == cont_vt);
		yggr_test_assert(cont_vt == buf);
		yggr_test_assert(!(buf != cont_vt));
		yggr_test_assert(!(cont_vt != buf));

		yggr_test_assert(buf == cont_list);
		yggr_test_assert(cont_list == buf);
		yggr_test_assert(!(buf != cont_list));
		yggr_test_assert(!(cont_list != buf));

		yggr_test_assert(buf == cont_set);
		yggr_test_assert(cont_set == buf);
		yggr_test_assert(!(buf != cont_set));
		yggr_test_assert(!(cont_set != buf));

	}

	{
		typedef int cont_native_array_type[3];
		typedef boost::array<int, 3> cont_array_type;
		typedef std::vector<int> cont_vt_type;
		typedef std::list<int> cont_list_type;
		typedef std::set<int> cont_set_type;

		typedef cont_set_type inner_cont_type;
		typedef yggr::nsql_database_system::c_bson_binary_buffer_ref<0, inner_cont_type> buffer_type;


		cont_native_array_type cont_narr = {1, 2, 3};
		cont_array_type cont_arr = {1, 2, 3};
		cont_vt_type cont_vt(boost::begin(cont_narr), boost::end(cont_narr));
		cont_list_type cont_list(boost::begin(cont_narr), boost::end(cont_narr));
		cont_set_type cont_set(boost::begin(cont_narr), boost::end(cont_narr));

		inner_cont_type& inner_cont = cont_set;

		bson_type bs;
		buffer_type buf(inner_cont);
		std::cout << buf << std::endl;
		bs.save("buf_name", buf);
		std::cout << bs << std::endl;

		inner_cont_type out_cont;
		buffer_type lbuf(out_cont);
		bs.begin().load("buf_name", lbuf);

		yggr_test_assert(buf == lbuf);
		yggr_test_assert(!(buf != lbuf));

		yggr_test_assert(buf == out_cont);
		yggr_test_assert(!(buf != out_cont));
		yggr_test_assert(out_cont == buf);
		yggr_test_assert(!(out_cont != buf));

		yggr_test_assert(lbuf == cont_narr);
		yggr_test_assert(!(lbuf != cont_narr));
		yggr_test_assert(cont_narr == lbuf);
		yggr_test_assert(!(cont_narr != lbuf));

		yggr_test_assert(buf == cont_arr);
		yggr_test_assert(cont_arr == buf);
		yggr_test_assert(!(buf != cont_arr));
		yggr_test_assert(!(cont_arr != buf));

		yggr_test_assert(buf == cont_vt);
		yggr_test_assert(cont_vt == buf);
		yggr_test_assert(!(buf != cont_vt));
		yggr_test_assert(!(cont_vt != buf));

		yggr_test_assert(buf == cont_list);
		yggr_test_assert(cont_list == buf);
		yggr_test_assert(!(buf != cont_list));
		yggr_test_assert(!(cont_list != buf));

		yggr_test_assert(buf == cont_set);
		yggr_test_assert(cont_set == buf);
		yggr_test_assert(!(buf != cont_set));
		yggr_test_assert(!(cont_set != buf));

	}

	{
		typedef int cont_native_array_type[3];
		typedef boost::array<int, 3> cont_array_type;
		typedef std::vector<int> cont_vt_type;
		typedef std::list<int> cont_list_type;
		typedef std::set<int> cont_set_type;
		typedef boost::unordered_set<int> cont_uset_type;

		typedef cont_uset_type inner_cont_type;
		typedef yggr::nsql_database_system::c_bson_binary_buffer_ref<0, inner_cont_type> buffer_type;


		cont_native_array_type cont_narr = {1, 2, 3};
		//cont_array_type cont_arr = {1, 2, 3};
		cont_vt_type cont_vt(boost::begin(cont_narr), boost::end(cont_narr));
		cont_list_type cont_list(boost::begin(cont_narr), boost::end(cont_narr));
		cont_set_type cont_set(boost::begin(cont_narr), boost::end(cont_narr));
		cont_uset_type cont_uset(boost::begin(cont_narr), boost::end(cont_narr));

		inner_cont_type& inner_cont = cont_uset;

		bson_type bs;
		buffer_type buf(inner_cont);
		std::cout << buf << std::endl;
		bs.save("buf_name", buf);
		std::cout << bs << std::endl;

		inner_cont_type out_cont;
		buffer_type lbuf(out_cont);
		bs.begin().load("buf_name", lbuf);

		yggr_test_assert(buf == lbuf);
		yggr_test_assert(!(buf != lbuf));

		yggr_test_assert(buf == out_cont);
		yggr_test_assert(!(buf != out_cont));
		yggr_test_assert(out_cont == buf);
		yggr_test_assert(!(out_cont != buf));

		// don't comarer unorderd container and not unordered container;
		//yggr_test_assert(buf == cont_vt);
		//yggr_test_assert(!(buf != cont_vt));
		//yggr_test_assert(cont_vt == buf);
		//yggr_test_assert(!(cont_vt != buf));

		yggr_test_assert(buf == cont_uset);
		yggr_test_assert(cont_uset == buf);
		yggr_test_assert(!(buf != cont_uset));
		yggr_test_assert(!(cont_uset != buf));
	}

	{
		typedef int cont_native_array_type[3];
		typedef boost::array<int, 3> cont_array_type;
		typedef std::vector<int> cont_vt_type;
		typedef std::list<int> cont_list_type;


		typedef cont_vt_type inner_cont_type;
		typedef yggr::nsql_database_system::c_bson_binary_buffer_ref<0, inner_cont_type> buffer_type;

		cont_native_array_type diff_cont_narr = {3, 2, 1};

		cont_native_array_type cont_narr = {1, 2, 3};
		cont_array_type cont_arr = {1, 2, 3};
		cont_vt_type diff_cont_vt(boost::begin(diff_cont_narr), boost::end(diff_cont_narr));
		cont_vt_type cont_vt(boost::begin(cont_narr), boost::end(cont_narr));
		cont_list_type cont_list(boost::begin(cont_narr), boost::end(cont_narr));

		inner_cont_type& inner_cont = diff_cont_vt;

		bson_type bs;
		buffer_type buf(inner_cont);
		buf = cont_vt; // test operator=
		std::cout << buf << std::endl;
		bs.save("buf_name", buf);
		std::cout << bs << std::endl;

		inner_cont_type out_cont;
		buffer_type lbuf(out_cont);
		bs.begin().load("buf_name", lbuf);

		yggr_test_assert(buf == lbuf);
		yggr_test_assert(!(buf != lbuf));

		yggr_test_assert(buf == out_cont);
		yggr_test_assert(!(buf != out_cont));
		yggr_test_assert(out_cont == buf);
		yggr_test_assert(!(out_cont != buf));

		yggr_test_assert(lbuf == cont_narr);
		yggr_test_assert(!(lbuf != cont_narr));
		yggr_test_assert(cont_narr == lbuf);
		yggr_test_assert(!(cont_narr != lbuf));

		yggr_test_assert(buf == cont_arr);
		yggr_test_assert(cont_arr == buf);
		yggr_test_assert(!(buf != cont_arr));
		yggr_test_assert(!(cont_arr != buf));

		yggr_test_assert(buf == cont_vt);
		yggr_test_assert(cont_vt == buf);
		yggr_test_assert(!(buf != cont_vt));
		yggr_test_assert(!(cont_vt != buf));

		yggr_test_assert(buf == cont_list);
		yggr_test_assert(cont_list == buf);
		yggr_test_assert(!(buf != cont_list));
		yggr_test_assert(!(cont_list != buf));
	}

	std::cout << "test_binart_buffer_ref success" << std::endl;
}

int main(int argc, char* argv[])
{
	test_construct();
	test_operator_set(); // more detailed see c_bson_binary_buffer_ref_operator_set_test_xxx.cpp series
	test_swap();
	test_member_foo();

	test_binart_buffer_ref();

	wait_any_key(argc, argv);
	return 0;
}
