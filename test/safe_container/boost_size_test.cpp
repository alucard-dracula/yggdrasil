//boost_size_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/safe_container/safe_vector.hpp>
#include <yggr/safe_container/safe_deque.hpp>
#include <yggr/safe_container/safe_list.hpp>

#include <yggr/safe_container/safe_stack.hpp>
#include <yggr/safe_container/safe_queue.hpp>

#include <yggr/safe_container/safe_set.hpp>
#include <yggr/safe_container/safe_map.hpp>

#include <yggr/safe_container/safe_options_set.hpp>
#include <yggr/safe_container/safe_options_map.hpp>

#include <yggr/safe_container/safe_unordered_set.hpp>
#include <yggr/safe_container/safe_unordered_map.hpp>

#include <yggr/safe_container/safe_multi_index_container.hpp>
#include <yggr/safe_container/safe_multi_index_hashed_index.hpp>
#include <yggr/safe_container/safe_multi_index_ordered_index.hpp>
#include <yggr/safe_container/safe_multi_index_random_access_index.hpp>

#ifdef YGGR_BOOST_MULTI_INDEX_HAS_RANKED_INDEX
#	include <yggr/safe_container/safe_multi_index_ranked_index.hpp>
#endif // YGGR_BOOST_MULTI_INDEX_HAS_RANKED_INDEX

#include <yggr/safe_container/safe_multi_index_sequenced_index.hpp>

#include <boost/range/functions.hpp>
#include <cassert>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

int g_arr[6] = {1, 2, 3, 4, 5, 6};

std::pair<int, int> g_pair_arr[6] =
{
	std::make_pair(1, 1),
	std::make_pair(2, 2),
	std::make_pair(3, 3),
	std::make_pair(4, 4),
	std::make_pair(5, 5),
	std::make_pair(6, 6)
};

// vector
void test_stl_vector(void)
{
	std::vector<int> cont(g_arr, g_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_stl_vector success" << std::endl;
}

void test_boost_vector(void)
{
	boost::container::vector<int> cont(g_arr, g_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_boost_vector success" << std::endl;
}

void test_safe_vector(void)
{
	yggr::safe_container::safe_vector<int> cont(g_arr, g_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_safe_vector success" << std::endl;
}

// deque
void test_stl_deque(void)
{
	std::deque<int> cont(g_arr, g_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_stl_deque success" << std::endl;
}

void test_boost_deque(void)
{
	boost::container::deque<int> cont(g_arr, g_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_boost_deque success" << std::endl;
}

void test_safe_deque(void)
{
	yggr::safe_container::safe_deque<int> cont(g_arr, g_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_safe_deque success" << std::endl;
}

// list
void test_stl_list(void)
{
	std::list<int> cont(g_arr, g_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_stl_list success" << std::endl;
}

void test_boost_list(void)
{
	boost::container::list<int> cont(g_arr, g_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_boost_list success" << std::endl;
}

void test_safe_list(void)
{
	yggr::safe_container::safe_list<int> cont(g_arr, g_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_safe_list success" << std::endl;
}


// queue
// boost::size not support stack, the reason same stack
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//stack

void test_stl_stack(void)
{
	std::deque<int> tcont(g_arr, g_arr + 6);
	std::stack<int> cont(tcont);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_stl_stack success" << std::endl;
}

void test_yggr_stack(void)
{
	yggr::container::deque<int> tcont(g_arr, g_arr + 6);
	yggr::container::stack<int> cont(tcont);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_yggr_stack success" << std::endl;
}

void test_safe_stack(void)
{
	yggr::container::deque<int> tcont(g_arr, g_arr + 6);
	yggr::safe_container::safe_stack<int> cont(tcont);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_safe_stack success" << std::endl;
}

//queue

void test_stl_queue(void)
{
	std::deque<int> tcont(g_arr, g_arr + 6);
	std::queue<int> cont(tcont);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_stl_queue success" << std::endl;
}

void test_yggr_queue(void)
{
	yggr::container::deque<int> tcont(g_arr, g_arr + 6);
	yggr::container::queue<int> cont(tcont);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_yggr_queue success" << std::endl;
}

void test_safe_queue(void)
{
	yggr::container::deque<int> tcont(g_arr, g_arr + 6);
	yggr::safe_container::safe_queue<int> cont(tcont);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_safe_queue success" << std::endl;
}

// priority_queue

void test_stl_priority_queue(void)
{
	std::vector<int> tcont(g_arr, g_arr + 6);
	std::priority_queue<int> cont(std::less<int>(), tcont);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_stl_priority_queue success" << std::endl;
}

void test_yggr_priority_queue(void)
{
	yggr::container::vector<int> tcont(g_arr, g_arr + 6);
	yggr::container::priority_queue<int> cont(tcont);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_yggr_priority_queue success" << std::endl;
}

void test_safe_priority_queue(void)
{
	yggr::container::vector<int> tcont(g_arr, g_arr + 6);
	yggr::safe_container::safe_priority_queue<int> cont(tcont);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_yggr_priority_queue success" << std::endl;
}

//set
void test_stl_set(void)
{
	std::set<int> cont(g_arr, g_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_stl_set success" << std::endl;
}

void test_boost_set(void)
{
	boost::container::set<int> cont(g_arr, g_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_boost_set success" << std::endl;
}

void test_yggr_set(void)
{
	yggr::container::set<int> cont(g_arr, g_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_yggr_set success" << std::endl;
}

void test_safe_set(void)
{
	yggr::safe_container::safe_set<int> cont(g_arr, g_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_safe_set success" << std::endl;
}

//multiset
void test_stl_multiset(void)
{
	std::multiset<int> cont(g_arr, g_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_stl_multiset success" << std::endl;
}

void test_boost_multiset(void)
{
	boost::container::multiset<int> cont(g_arr, g_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_boost_multiset success" << std::endl;
}

void test_yggr_multiset(void)
{
	yggr::container::multiset<int> cont(g_arr, g_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_yggr_multiset success" << std::endl;
}

void test_safe_multiset(void)
{
	yggr::safe_container::safe_multiset<int> cont(g_arr, g_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_safe_multiset success" << std::endl;
}

// map
void test_stl_map(void)
{
	std::map<int, int> cont(g_pair_arr, g_pair_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_stl_map success" << std::endl;
}

void test_boost_map(void)
{
	boost::container::map<int, int> cont(g_pair_arr, g_pair_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_boost_map success" << std::endl;
}

void test_yggr_map(void)
{
	yggr::container::map<int, int> cont(g_pair_arr, g_pair_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_yggr_map success" << std::endl;
}

void test_safe_map(void)
{
	yggr::safe_container::safe_map<int, int> cont(g_pair_arr, g_pair_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_safe_map success" << std::endl;
}

// multimap
void test_stl_multimap(void)
{
	std::multimap<int, int> cont(g_pair_arr, g_pair_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_stl_multimap success" << std::endl;
}

void test_boost_multimap(void)
{
	boost::container::multimap<int, int> cont(g_pair_arr, g_pair_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_boost_multimap success" << std::endl;
}

void test_yggr_multimap(void)
{
	yggr::container::multimap<int, int> cont(g_pair_arr, g_pair_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_yggr_multimap success" << std::endl;
}

void test_safe_multimap(void)
{
	yggr::safe_container::safe_multimap<int, int> cont(g_pair_arr, g_pair_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_safe_multimap success" << std::endl;
}

// options_set
void test_yggr_options_set(void)
{
	yggr::container::options_set<int> cont(g_arr, g_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_yggr_options_set success" << std::endl;
}

void test_safe_options_set(void)
{
	yggr::safe_container::safe_options_set<int> cont(g_arr, g_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_safe_options_set success" << std::endl;
}

// options_multiset
void test_yggr_options_multiset(void)
{
	yggr::container::options_multiset<int> cont(g_arr, g_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_yggr_options_multiset success" << std::endl;
}

void test_safe_options_multiset(void)
{
	yggr::safe_container::safe_options_multiset<int> cont(g_arr, g_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_safe_options_multiset success" << std::endl;
}

// options_map
void test_yggr_options_map(void)
{
	yggr::container::options_map<int, int> cont(g_pair_arr, g_pair_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_yggr_options_map success" << std::endl;
}

void test_safe_options_map(void)
{
	yggr::safe_container::safe_options_map<int, int> cont(g_pair_arr, g_pair_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_safe_options_map success" << std::endl;
}

// options_multimap
void test_yggr_options_multimap(void)
{
	yggr::container::options_multimap<int, int> cont(g_pair_arr, g_pair_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_yggr_options_multimap success" << std::endl;
}

void test_safe_options_multimap(void)
{
	yggr::safe_container::safe_options_multimap<int, int> cont(g_pair_arr, g_pair_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_safe_options_multimap success" << std::endl;
}

// unordered_set
void test_stl_unordered_set(void)
{
#ifdef YGGR_USE_CXX11
	std::unordered_set<int> cont(g_arr, g_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());
#endif // YGGR_USE_CXX11

	std::cout << "test_stl_unordered_set success" << std::endl;
}

void test_boost_unordered_set(void)
{
	boost::unordered_set<int> cont(g_arr, g_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_boost_unordered_set success" << std::endl;
}

void test_yggr_unordered_set(void)
{
	yggr::unordered_set<int> cont(g_arr, g_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_yggr_unordered_set success" << std::endl;
}

void test_safe_unordered_set(void)
{
	yggr::safe_container::safe_unordered_set<int> cont(g_arr, g_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_safe_unordered_set success" << std::endl;
}

// unordered_multiset
void test_stl_unordered_multiset(void)
{
#ifdef YGGR_USE_CXX11
	std::unordered_multiset<int> cont(g_arr, g_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());
#endif // YGGR_USE_CXX11

	std::cout << "test_stl_unordered_multiset success" << std::endl;
}

void test_boost_unordered_multiset(void)
{
	boost::unordered_multiset<int> cont(g_arr, g_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_boost_unordered_multiset success" << std::endl;
}

void test_yggr_unordered_multiset(void)
{
	yggr::unordered_multiset<int> cont(g_arr, g_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_yggr_unordered_multiset success" << std::endl;
}

void test_safe_unordered_multiset(void)
{
	yggr::safe_container::safe_unordered_multiset<int> cont(g_arr, g_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_safe_unordered_multiset success" << std::endl;
}

// unordered_map
void test_stl_unordered_map(void)
{
#ifdef YGGR_USE_CXX11
	std::unordered_map<int, int> cont(g_pair_arr, g_pair_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());
#endif // YGGR_USE_CXX11

	std::cout << "test_stl_unordered_map success" << std::endl;
}

void test_boost_unordered_map(void)
{
	boost::unordered_map<int, int> cont(g_pair_arr, g_pair_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_boost_unordered_map success" << std::endl;
}

void test_yggr_unordered_map(void)
{
	yggr::unordered_map<int, int> cont(g_pair_arr, g_pair_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_yggr_unordered_map success" << std::endl;
}

void test_safe_unordered_map(void)
{
	yggr::safe_container::safe_unordered_map<int, int> cont(g_pair_arr, g_pair_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_safe_unordered_map success" << std::endl;
}

// unordered_multimap
void test_stl_unordered_multimap(void)
{
#ifdef YGGR_USE_CXX11
	std::unordered_multimap<int, int> cont(g_pair_arr, g_pair_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());
#endif // YGGR_USE_CXX11

	std::cout << "test_stl_unordered_multimap success" << std::endl;
}

void test_boost_unordered_multimap(void)
{
	boost::unordered_multimap<int, int> cont(g_pair_arr, g_pair_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_boost_unordered_multimap success" << std::endl;
}

void test_yggr_unordered_multimap(void)
{
	yggr::unordered_multimap<int, int> cont(g_pair_arr, g_pair_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_yggr_unordered_multimap success" << std::endl;
}

void test_safe_unordered_multimap(void)
{
	yggr::safe_container::safe_unordered_multimap<int, int> cont(g_pair_arr, g_pair_arr + 6);
	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_safe_unordered_multimap success" << std::endl;
}

struct tag0{}; // hashed_unique
struct tag1{}; // hashed_non_unique
struct tag2{}; // ordered_unique
struct tag3{}; // ordered_non_unique
struct tag4{}; // sequence
struct tag5{}; // random_access
struct tag6{}; // ranked_unique
struct tag7{}; // ranked_non_unique

// multi_index_container
void test_boost_multi_index_container(void)
{
//    struct tag0{}; // hashed_unique
//    struct tag1{}; // hashed_non_unique
//    struct tag2{}; // ordered_unique
//    struct tag3{}; // ordered_non_unique
//    struct tag4{}; // sequence
//    struct tag5{}; // random_access
//    struct tag6{}; // ranked_unique
//    struct tag7{}; // ranked_non_unique

	typedef
		boost::multi_index::multi_index_container
		<
			int,
			boost::multi_index::indexed_by
			<
				boost::multi_index::hashed_unique
				<
					boost::multi_index::tag<tag0>,
					boost::multi_index::identity< int >
				>
				,boost::multi_index::hashed_non_unique
				<
					boost::multi_index::tag<tag1>,
					boost::multi_index::identity< int >
				>
				,boost::multi_index::ordered_unique
				<
					boost::multi_index::tag<tag2>,
					boost::multi_index::identity< int >
				>
				,boost::multi_index::ordered_non_unique
				<
					boost::multi_index::tag<tag3>,
					boost::multi_index::identity< int >
				>
				,boost::multi_index::sequenced
				<
					boost::multi_index::tag<tag4>
				>
				,boost::multi_index::random_access
				<
					boost::multi_index::tag<tag5>
				>

#ifdef YGGR_BOOST_MULTI_INDEX_HAS_RANKED_INDEX
				,boost::multi_index::ranked_unique
				<
					boost::multi_index::tag<tag6>,
					boost::multi_index::identity< int >
				>
				,boost::multi_index::ranked_non_unique
				<
					boost::multi_index::tag<tag7>,
					boost::multi_index::identity< int >
				>
#endif // YGGR_BOOST_MULTI_INDEX_HAS_RANKED_INDEX
			>
		> midx_type;

	midx_type cont(g_arr, g_arr + 6);

	yggr_test_assert(boost::size(cont.get<0>()) == cont.size());
	yggr_test_assert(boost::size(cont.get<1>()) == cont.size());
	yggr_test_assert(boost::size(cont.get<2>()) == cont.size());
	yggr_test_assert(boost::size(cont.get<3>()) == cont.size());
	yggr_test_assert(boost::size(cont.get<4>()) == cont.size());
	yggr_test_assert(boost::size(cont.get<5>()) == cont.size());

#ifdef YGGR_BOOST_MULTI_INDEX_HAS_RANKED_INDEX
	yggr_test_assert(boost::size(cont.get<6>()) == cont.size());
	yggr_test_assert(boost::size(cont.get<7>()) == cont.size());
#endif // YGGR_BOOST_MULTI_INDEX_HAS_RANKED_INDEX

	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_boost_multi_index_container success" << std::endl;

}

void test_safe_multi_index_container(void)
{
	//struct tag0{}; // hashed_unique
	//struct tag1{}; // hashed_non_unique
	//struct tag2{}; // ordered_unique
	//struct tag3{}; // ordered_non_unique
	//struct tag4{}; // sequence
	//struct tag5{}; // random_access
	//struct tag6{}; // ranked_unique
	//struct tag7{}; // ranked_non_unique

	typedef
		yggr::safe_container::safe_multi_index_container
		<
			int,
			boost::multi_index::indexed_by
			<
				boost::multi_index::hashed_unique
				<
					boost::multi_index::tag<tag0>,
					boost::multi_index::identity< int >
				>
				,boost::multi_index::hashed_non_unique
				<
					boost::multi_index::tag<tag1>,
					boost::multi_index::identity< int >
				>
				,boost::multi_index::ordered_unique
				<
					boost::multi_index::tag<tag2>,
					boost::multi_index::identity< int >
				>
				,boost::multi_index::ordered_non_unique
				<
					boost::multi_index::tag<tag3>,
					boost::multi_index::identity< int >
				>
				,boost::multi_index::sequenced
				<
					boost::multi_index::tag<tag4>
				>,
				boost::multi_index::random_access
				<
					boost::multi_index::tag<tag5>
				>

#ifdef YGGR_BOOST_MULTI_INDEX_HAS_RANKED_INDEX
				,boost::multi_index::ranked_unique
				<
					boost::multi_index::tag<tag6>,
					boost::multi_index::identity< int >
				>
				,boost::multi_index::ranked_non_unique
				<
					boost::multi_index::tag<tag7>,
					boost::multi_index::identity< int >
				>
#endif // YGGR_BOOST_MULTI_INDEX_HAS_RANKED_INDEX
			>
		> midx_type;

	midx_type cont(g_arr, g_arr + 6);

	yggr_test_assert(boost::size(cont.get<0>()) == cont.size());
	yggr_test_assert(boost::size(cont.get<1>()) == cont.size());
	yggr_test_assert(boost::size(cont.get<2>()) == cont.size());
	yggr_test_assert(boost::size(cont.get<3>()) == cont.size());
	yggr_test_assert(boost::size(cont.get<4>()) == cont.size());
	yggr_test_assert(boost::size(cont.get<5>()) == cont.size());

#ifdef YGGR_BOOST_MULTI_INDEX_HAS_RANKED_INDEX
	yggr_test_assert(boost::size(cont.get<6>()) == cont.size());
	yggr_test_assert(boost::size(cont.get<7>()) == cont.size());
#endif // YGGR_BOOST_MULTI_INDEX_HAS_RANKED_INDEX

	yggr_test_assert(boost::size(cont) == cont.size());

	std::cout << "test_safe_multi_index_container success" << std::endl;
}

int main(int argc, char* argv[])
{
	// vector
	test_stl_vector();
	test_boost_vector();
	test_safe_vector();

	std::cout << "-----------------------" << std::endl;

	// deque
	test_stl_deque();
	test_boost_deque();
	test_safe_deque();

	std::cout << "-----------------------" << std::endl;

	// stack
	test_stl_stack();
	test_yggr_stack();
	test_safe_stack();

	std::cout << "-----------------------" << std::endl;

	// queue
	test_stl_queue();
	test_yggr_queue();
	test_safe_queue();

	std::cout << "-----------------------" << std::endl;

	// priority_queue
	test_stl_priority_queue();
	test_yggr_priority_queue();
	test_safe_priority_queue();

	std::cout << "-----------------------" << std::endl;

	// list
	test_stl_list();
	test_boost_list();
	test_safe_list();

	std::cout << "-----------------------" << std::endl;

	// set
	test_stl_set();
	test_boost_set();
	test_yggr_set();
	test_safe_set();

	std::cout << "-----------------------" << std::endl;

	// multiset
	test_stl_multiset();
	test_boost_multiset();
	test_yggr_multiset();
	test_safe_multiset();

	std::cout << "-----------------------" << std::endl;

	// map
	test_stl_map();
	test_boost_map();
	test_yggr_map();
	test_safe_map();

	std::cout << "-----------------------" << std::endl;

	// multimap
	test_stl_multimap();
	test_boost_multimap();
	test_yggr_multimap();
	test_safe_multimap();

	std::cout << "-----------------------" << std::endl;

	// options_set
	test_yggr_options_set();
	test_safe_options_set();

	std::cout << "-----------------------" << std::endl;

	// options_multiset
	test_yggr_options_multiset();
	test_safe_options_multiset();

	std::cout << "-----------------------" << std::endl;

	// options_map
	test_yggr_options_map();
	test_safe_options_map();

	std::cout << "-----------------------" << std::endl;

	// options_multimap
	test_yggr_options_multimap();
	test_safe_options_multimap();

	std::cout << "-----------------------" << std::endl;

	// unordered_set
	test_stl_unordered_set();
	test_boost_unordered_set();
	test_yggr_unordered_set();
	test_safe_unordered_set();

	std::cout << "-----------------------" << std::endl;

	// unordered_multiset
	test_stl_unordered_multiset();
	test_boost_unordered_multiset();
	test_yggr_unordered_multiset();
	test_safe_unordered_multiset();

	std::cout << "-----------------------" << std::endl;

	// unordered_map
	test_stl_unordered_map();
	test_boost_unordered_map();
	test_yggr_unordered_map();
	test_safe_unordered_map();

	std::cout << "-----------------------" << std::endl;

	// unordered_map
	test_stl_unordered_multimap();
	test_boost_unordered_multimap();
	test_yggr_unordered_multimap();
	test_safe_unordered_multimap();

	std::cout << "-----------------------" << std::endl;

	// multi_index
	test_boost_multi_index_container();
	test_safe_multi_index_container();

	std::cout << "-----------------------" << std::endl;

	wait_any_key(argc, argv);
	return 0;
}
