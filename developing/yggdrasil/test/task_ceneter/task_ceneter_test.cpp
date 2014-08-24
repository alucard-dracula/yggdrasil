//task_center_test.cpp

#include <iostream>

#include <yggr/task_center/task.hpp>
#include <yggr/task_center/task_creator.hpp>
#include <yggr/task_center/task_center.hpp>
#include <yggr/task_center/task_store_op.hpp>
#include <yggr/task_center/task_saver.hpp>
#include <yggr/task_center/support/task_import.hpp>
#include <yggr/task_center/recver_handler_mgr.hpp>

#include <yggr/adapter/adapter_mgr.hpp>
#include <yggr/adapter/adapter_parser_helper.hpp>


#include <yggr/ids/uuid.hpp>

#include <yggr/safe_container/safe_hash_map_queue.hpp>

#include <yggr/adapter/adapter_mgr.hpp>

#include <yggr/charset/string.hpp>

#ifdef _MSC_VER
#include <vld.h>
#endif // _MSC_VER

struct A
{
	typedef yggr::task_center::default_task_data_info_type task_data_info_type;

	enum
	{
		E_pak_id_start = 0x00008000,
		E_pak_id = E_pak_id_start + 1,

		E_id_compile_u32 = 0xffffffff
	};

	enum
	{
		E_version_start = 0x0000000,
		E_now_ver,

		E_ver_compile_u32 = 0xffffffff
	};

public:
	A(void)
		: n(0)
	{
	}

	explicit A(int nn)
		: n(nn)
	{
	}

	A(const A& right)
		: n(right.n)
	{
	}

	virtual ~A(void)
	{
	}

	A& operator=(const A& right)
	{
		n = right.n;
		return *this;
	}

	yggr::u64 user_id(void) const
	{
		return 1;
	}

	YGGR_FAST_CAL_TASK_IMPORT(task_data_info_type, A, E_pak_id, E_now_ver)

public:
	int n;
};

struct B
{
	typedef yggr::task_center::default_task_data_info_type task_data_info_type;
	enum
	{
		E_pak_id_start = 0x00008000,
		E_pak_id = E_pak_id_start + 2,

		E_id_compile_u32 = 0xffffffff
	};

	enum
	{
		E_version_start = 0x0000000,
		E_now_ver,

		E_ver_compile_u32 = 0xffffffff
	};

public:
	B(void)
		: n(0)
	{
	}

	explicit B(int nn)
		: n(nn)
	{
	}

	B(const B& right)
		: n(right.n)
	{
	}

	virtual ~B(void)
	{
	}

	B& operator=(const B& right)
	{
		n = right.n;
		return *this;
	}

	yggr::u64 user_id(void) const
	{
		return 1;
	}

	YGGR_FAST_CAL_TASK_IMPORT(task_data_info_type, B, E_pak_id, E_now_ver)

public:
	int n;
};

typedef yggr::ids::id_generator<yggr::ids::id64_type> task_id_gen_type;

typedef yggr::task_center::task<yggr::ids::id64_type, yggr::u64,
									yggr::task_center::default_task_data_info_type> task_type;

typedef yggr::safe_container::safe_hash_map_queue<task_type::owner_id_type, task_type> task_container_type;

typedef yggr::task_center::key_value_store<task_container_type::value_type, task_type::owner_id_getter> task_store_type;

typedef yggr::task_center::task_saver<yggr::task_center::type_traits::mark_saver_condition, task_type,
										task_container_type, task_store_type> cdt_saver_type;

typedef yggr::task_center::recver_handler_mgr<yggr::task_center::type_traits::mark_saver_result, task_type> rst_saver_type;

typedef yggr::task_center::task_creator<task_id_gen_type, task_type> task_creator_type;
typedef yggr::task_center::task_center<task_creator_type, cdt_saver_type, rst_saver_type> task_center_type;

typedef boost::shared_ptr<task_center_type> task_center_ptr_type;

typedef yggr::adapter::adaper_id_parser_def
		<
			yggr::task_center::task_data_send_typeid_getter
			<
				yggr::task_center::default_task_data_info_type
			>,

			yggr::task_center::task_data_recv_typeid_getter
			<
				yggr::task_center::default_task_data_info_type
			>,

			YGGR_IDS_PARSER_CONST_MEM_FOO(A,
											yggr
												::task_center
													::default_task_data_info_type
														::data_info_type,
											data_info),

			YGGR_IDS_PARSER_CONST_MEM_FOO(task_type,
											const yggr
													::task_center
														::default_task_data_info_type
															::data_info_type&,
											data_info),

			YGGR_IDS_PARSER_CONST_MEM_FOO(task_type,
											yggr
												::task_center
													::default_task_data_info_type
														::class_name_type,
											class_name),
			YGGR_IDS_PARSER_USE_TYPEID_NAME_CONV_RET(std::string)
		> adaper_id_parser_def_type;

template<typename Send_Data, typename Task, typename Recv_Data>
struct adapter_converter
{
public:
	typedef Send_Data send_data_type;
	typedef Task task_type;
	typedef Recv_Data recv_data_type;

	typedef bool conv_to_send_result_type;
	typedef const send_data_type& conv_to_send_arg_data_type;

	typedef recv_data_type conv_from_recv_result_type;
	typedef const task_type& conv_from_recv_arg_data_type;

private:
	typedef adapter_converter this_type;

public:

	template<typename Real_Data_Type, typename Src>
	static bool conv_to_send(Src& src, const send_data_type& data)
	{
		src.template send<cdt_saver_type::mark_type>(data.user_id(), data);
		return true;
	}

	template<typename Real_Data_Type>
	static recv_data_type conv_from_recv(const task_type& data)
	{
		const Real_Data_Type *pval = data.template get<Real_Data_Type>();
		assert(pval);
		if(pval)
		{
			return recv_data_type(pval->n);
		}

		std::cout << typeid(Real_Data_Type).name() << std::endl;
		return recv_data_type();
	}
};

 typedef yggr::adapter::adapter_mgr<task_center_type, A, B, adaper_id_parser_def_type, adapter_converter> adapter_mgr_type;

 typedef boost::shared_ptr<adapter_mgr_type> adapter_mgr_ptr_type;

void handler_recv(const B& b)
{
	std::cout << "recv " << b.n << std::endl;
}

template<typename Src>
void handler_fix_task(Src& src, const task_type& tk)
{
	const A* pa = tk.get<A>();
	assert(pa);

	src.template send<rst_saver_type::mark_type>(tk.owner_info(), B(pa->n + 1));
}

void test1(void)
{
	task_center_ptr_type ptc(new task_center_type());
	task_center_type& task_center = *ptc;

	adapter_mgr_ptr_type padp(new adapter_mgr_type(task_center));
	adapter_mgr_type& adp_mgr = *padp;

	//assert(adp_mgr.register_adapter<A>());
	//assert(adp_mgr.register_adapter<B>());
	adp_mgr.register_adapter<A>();
	adp_mgr.register_adapter<B>();

	yggr::ids::id64_type id = 1;
	adp_mgr.send(A(100)); // use adapter send cdt_data to task_center
	adp_mgr.send(A(200));

	adp_mgr.hold_recv(id, boost::bind(&handler_recv, _1));

	task_center.get_task<cdt_saver_type::mark_type>
		( boost::bind(&handler_fix_task<task_center_type>,
						boost::ref(task_center), _1) );

	task_center.get_task<cdt_saver_type::mark_type>
		( boost::bind(&handler_fix_task<task_center_type>,
						boost::ref(task_center), _1) );

	task_center.get_task<cdt_saver_type::mark_type>
		( boost::bind(&handler_fix_task<task_center_type>,
						boost::ref(task_center), _1) ); // empty_task_center_test

	task_center.drop_all();
}

int main(int argc, char* argv[])
{
	test1();

	char cc = 0;
	std::cin >> cc;
	return 0;
}
