// safe_multi_index_range_and_modify_test.cpp

#include <yggr/base/yggrdef.h>
#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/base/static_constant.hpp>

#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/ids/uuid.hpp>
#include <yggr/charset/utf8_string.hpp>

#include <yggr/safe_container/safe_multi_index_container.hpp>
#include <yggr/safe_container/safe_multi_index_hashed_index.hpp>
#include <yggr/safe_container/safe_multi_index_ordered_index.hpp>

#include <yggr/thread/boost_thread.hpp>
#include <yggr/thread/boost_thread_config.hpp>
#include <yggr/thread/this_thread.hpp>

#include <boost/atomic.hpp>
#include <boost/multi_index/mem_fun.hpp>

#include <sstream>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

typedef yggr::ids::uuid uuid_type;
typedef
	yggr::ids::id_generator
	<
		yggr::ids::uuid,
		yggr::ids::parse_uuid_genner::lagged_fibonacci607_type
	> uuid_gen_type;


struct state_def
{
public:
	YGGR_STATIC_CONSTANT(yggr::u32, sat_null = 0);
	YGGR_STATIC_CONSTANT(yggr::u32, sat_enis = 1);
	YGGR_STATIC_CONSTANT(yggr::u32, sat_zwei = 2);

private:
	typedef state_def this_type;

public:
	inline static yggr::u32 state_down(yggr::u32& sat)
	{
		switch(sat)
		{
		case this_type::sat_null:
			return sat;
		case this_type::sat_enis:
		case this_type::sat_zwei:
			return sat--;
		default:
			yggr_test_assert(false);
			return sat;
		}
	}

	inline static yggr::u32 state_down_zwei(yggr::u32& sat)
	{
		yggr::u32 tmp = sat;

		switch(tmp)
		{
		case this_type::sat_null:
		case this_type::sat_enis:
			return tmp;
		case this_type::sat_zwei:
			sat = tmp - 2;
			return tmp;
		default:
			yggr_test_assert(false);
			return tmp;
		}
	}

	inline static yggr::u32 state_up(yggr::u32& sat)
	{
		switch(sat)
		{
		case this_type::sat_null:
			sat = this_type::sat_zwei;
		case this_type::sat_enis:
		case this_type::sat_zwei:
			return sat;
		default:
			yggr_test_assert(false);
			return sat;
		}
	}
};

typedef state_def state_def_type;

struct midx_data
{
public:
	typedef yggr::utf8_string id_type;
	typedef yggr::u32 state_type;

private:
	typedef midx_data this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	midx_data(void)
		: _sat(0)
	{
	}

	midx_data(const uuid_type& id, state_type sat = 0)
		: _sat(sat)
	{
		std::stringstream ss;
		ss << id;
		ss >> _id;
	}

	midx_data(const id_type& id, state_type sat = 0)
		: _id(id), _sat(sat)
	{
	}

	midx_data(BOOST_RV_REF(this_type) right)
		: _id(boost::move(right._id)),
			_sat(boost::move(right._sat))
	{
	}

	midx_data(const this_type& right)
		: _id(right._id),
			_sat(right._sat)
	{
	}

	~midx_data(void)
	{
	}

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		if(this == &right)
		{
			return *this;
		}

		yggr::copy_or_move_or_swap(_id, boost::move(right_ref._id));
		_sat = boost::move(right._sat);
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		_id = right._id;
		_sat = right._sat;
		return *this;
	}

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}

		yggr::swap(_id, right._id);
		yggr::swap(_sat, right._sat);
	}

public:
	inline id_type& id(void)
	{
		return _id;
	}

	inline const id_type& id(void) const
	{
		return _id;
	}

	inline const id_type& index_id(void) const
	{
		return _id;
	}

	inline yggr::u32& state(void)
	{
		return _sat;
	}

	inline yggr::u32 state(void) const
	{
		return _sat;
	}

	inline yggr::u32 index_state(void) const
	{
		return _sat;
	}

private:
	id_type _id;
	state_type _sat;
};

typedef midx_data midx_data_type;

struct tag_id { typedef yggr::utf8_string type; };
struct tag_sat { typedef  yggr::u32 type; };

typedef
	yggr::safe_container::safe_multi_index_container
	<
		midx_data_type,
		boost::multi_index::indexed_by
		<
			boost::multi_index::hashed_unique
			<
				boost::multi_index::tag<tag_id>,
				BOOST_MULTI_INDEX_CONST_MEM_FUN(midx_data_type, const tag_id::type&, index_id)
			>,

			boost::multi_index::ordered_non_unique
			<
				boost::multi_index::tag<tag_sat>,
				BOOST_MULTI_INDEX_CONST_MEM_FUN(midx_data_type, tag_sat::type, index_state)
			>
		>
	> midx_type;

typedef boost::multi_index::index<midx_type, tag_id>::type safe_id_index_type;
typedef safe_id_index_type::index_id_type safe_id_index_id_type;
typedef safe_id_index_type::iterator safe_id_index_iter_type;
typedef safe_id_index_type::const_iterator safe_id_index_citer_type;

typedef boost::multi_index::index<midx_type, tag_sat>::type safe_sat_index_type;
typedef safe_sat_index_type::index_id_type safe_sat_index_id_type;
typedef safe_sat_index_type::iterator safe_sat_index_iter_type;
typedef safe_sat_index_type::const_iterator safe_sat_index_citer_type;

typedef midx_type::base_type unsafe_midx_type;


boost::atomic<bool> g_is_run;
midx_type g_midx;



void init(void)
{
	uuid_gen_type gen;
	g_midx.insert(midx_data_type(gen(), state_def_type::sat_zwei));
	g_midx.insert(midx_data_type(gen(), state_def_type::sat_zwei));
	g_midx.insert(midx_data_type(gen(), state_def_type::sat_zwei));
}

void handler_modify_down(midx_data_type& val, yggr::u32& sat_bak)
{
	sat_bak = val.state();
	state_def_type::state_down(val.state());
}

void handler_modify_down_zwei(midx_data_type& val, yggr::u32& sat_bak)
{
	sat_bak = val.state();
	state_def_type::state_down_zwei(val.state());
}

void handler_modify_up(midx_data_type& val, yggr::u32& sat_bak)
{
	sat_bak = val.state();
	state_def_type::state_up(val.state());
}


void handler_modify_rollback(midx_data_type& val, const yggr::u32& sat_bak)
{
	val.state() = sat_bak;
}

void handler_range_modify_org(unsafe_midx_type& final,
								const
									boost::fusion::pair
									<
										safe_sat_index_id_type,
										std::pair
										<
											safe_sat_index_iter_type,
											safe_sat_index_iter_type
										>
									>& rst)
{
	typedef yggr::vector<safe_id_index_iter_type> iter_vt_type;

	yggr::u32 sat_bak = 0;
	yggr::u32 idx = 0;
	yggr::u32 op = 0;

	yggr_test_assert(std::distance(rst.second.first, rst.second.second) <= 3);
	yggr::u32 sat_arr[3] = {0};

	iter_vt_type iter_vt;

	{
		for(safe_sat_index_iter_type i = rst.second.first, isize = rst.second.second; i != isize; ++i)
		{
			sat_arr[idx++] = i->state();
			iter_vt.push_back(final.project<tag_id>(i));
		}
	}

	idx = 0;

	{
		for(iter_vt_type::const_iterator i = iter_vt.begin(), isize = iter_vt.end(); i != isize; ++i, ++idx)
		{
			op = rand() % 3;

			if(sat_arr[idx] != (*i)->state())
			{
				int nnn = 0;
				assert(!nnn);
			}

			switch(op)
			{
			case 0:
				std::cout << "modify_up start" << std::endl;
				final.get<safe_id_index_id_type::first::value>().modify(
					*i,
					boost::bind(&handler_modify_up, _1, boost::ref(sat_bak)),
					boost::bind(&handler_modify_rollback, _1, boost::cref(sat_bak)));
				std::cout << "modify_up end" << std::endl;
				break;
			case 1:
				std::cout << "modify_down start" << std::endl;
				final.get<safe_id_index_id_type::first::value>().modify(
					*i,
					boost::bind(&handler_modify_down, _1, boost::ref(sat_bak)),
					boost::bind(&handler_modify_rollback, _1, boost::cref(sat_bak)));
				std::cout << "modify_down end" << std::endl;
				break;
			case 2:
				std::cout << "modify_zwei start" << std::endl;
				final.get<safe_id_index_id_type::first::value>().modify(
					*i,
					boost::bind(&handler_modify_down_zwei, _1, boost::ref(sat_bak)),
					boost::bind(&handler_modify_rollback, _1, boost::cref(sat_bak)));
				std::cout << "modify_zwei end" << std::endl;
				break;
			}
		}
	}
}

void range_modify_org(void)
{
	for(;g_is_run.load();)
	{
		g_midx.get<tag_sat>().range(
			boost::bind(g_midx.get<tag_sat>().key_comp(), state_def_type::sat_null, _1),
			boost::bind(g_midx.get<tag_sat>().key_comp(), _1, state_def_type::sat_zwei + 1),
			boost::bind(&handler_range_modify_org, _1, _2));

		g_midx.get<tag_sat>().equal_range(
			state_def_type::sat_null,
			boost::bind(&handler_range_modify_org, _1, _2));
	}
}


int main(int argc, char* argv[])
{
	typedef yggr::thread::this_thread<boost::thread> this_thread_type;
	typedef yggr::thread::boost_thread_config_type thread_config_type;

	init();

	boost::thread_group trds;

	g_is_run = true;

	trds.create_thread(boost::bind(&range_modify_org));

	if(!detail::wait_any_key_detail(argc, argv))
	{
		this_thread_type::sleep(thread_config_type::time_type(10, 0));
	}

	g_is_run = false;

	trds.join_all();

	wait_any_key(argc, argv);
	return 0;
}
