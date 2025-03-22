// multi_index_node_handler_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <boost/multi_index/member.hpp>

#include <yggr/base/yggrdef.h>

#include <yggr/container/multi_index_container.hpp>
#include <yggr/container/multi_index_ordered_index.hpp>
#include <yggr/container/multi_index_hashed_index.hpp>
#include <yggr/container/multi_index_sequenced_index.hpp>
#include <yggr/container/multi_index_random_access_index.hpp>

#if defined(YGGR_BOOST_MULTI_INDEX_HAS_RANKED_INDEX)
#	include <yggr/container/multi_index_ranked_index.hpp>
#endif // YGGR_BOOST_MULTI_INDEX_HAS_RANKED_INDEX

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

struct A
{
	struct tag_prm1{};
	struct tag_prm2{};

	struct tag_prm3{};
	struct tag_prm4{};
	struct tag_prm5{};

	A(void)
		: prm1(), prm2()
	{
	}

	A(int a, int b, const std::string& c)
		: prm1(a), prm2(b), prm3(c)
	{
	}

	A(const A& right)
		: prm1(right.prm1), prm2(right.prm2), prm3(right.prm3)
	{
	}

	~A(void)
	{
	}

	operator const int&(void) const
	{
		return prm1;
	}

	A& operator=(const std::string& str)
	{
		prm3 = str;
		return *this;
	}

	A& operator=(const A& right)
	{
		if(this == &right)
		{
			return *this;
		}

		prm1 = right.prm1;
		prm2 = right.prm2;
		prm3 = right.prm3;
		
		return *this;
	}

	int prm1;
	int prm2;
	std::string prm3;
};

bool operator==(const A& l, const A& r)
{
	return l.prm1 == r.prm1 && l.prm2 == r.prm2 && l.prm3 == r.prm3;
}

bool operator<(const A& l, const A& r)
{
	return l.prm1 < r.prm1;
}

void test_ord(void)
{
	typedef
		boost::multi_index_container
		<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::ordered_unique<
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::ordered_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>
			>
		> mi_cont_type;

	typedef yggr::container::node_handle_traits<mi_cont_type> node_handle_traits_type;
	typedef node_handle_traits_type::node_type mi_node_type;
	typedef node_handle_traits_type::insert_return_type mi_insert_return_type;

	mi_cont_type mi_cont;

	mi_cont.insert(A(10, 10, "s10"));
	mi_cont.insert(A(11, 10, "s11"));
	mi_cont.insert(A(20, 20, "s20"));
	mi_cont.insert(A(21, 20, "s21"));

	yggr_test_assert(mi_cont.size() == 4);

#if (BOOST_VERSION < 107400)

	typedef yggr::container::detail::cast_to_midx_container_hacking<mi_cont_type>::type hacking_mi_cont_type;

	{
		hacking_mi_cont_type& hacking_cont = reinterpret_cast<hacking_mi_cont_type&>(mi_cont);

		hacking_mi_cont_type::node_type node = hacking_cont.extract(10);

		std::cout << node.value().prm3 << std::endl;
		yggr_test_assert(mi_cont.size() == 3);

		hacking_mi_cont_type::insert_return_type ins_ret = hacking_cont.insert(boost::move(node));
		yggr_test_assert(ins_ret.inserted);
		yggr_test_assert(ins_ret.position != mi_cont.end());
		yggr_test_assert(!ins_ret.node);
		yggr_test_assert(mi_cont.size() == 4);

		yggr_test_assert(mi_cont.find(10) != mi_cont.get<0>().end());
	}

	{
		hacking_mi_cont_type& hacking_cont = reinterpret_cast<hacking_mi_cont_type&>(mi_cont);

		hacking_mi_cont_type::node_type node = hacking_cont.extract(mi_cont.find(11));
		std::cout << node.value().prm3 << std::endl;
		yggr_test_assert(mi_cont.size() == 3);

		hacking_mi_cont_type::iterator ins_ret = hacking_cont.insert(mi_cont.end(), boost::move(node));

		yggr_test_assert(ins_ret != mi_cont.end());
		yggr_test_assert(!node);
		yggr_test_assert(mi_cont.size() == 4);

		yggr_test_assert(mi_cont.find(11) != mi_cont.get<0>().end());
	}

#endif // (BOOST_VERSION < 107400)

	{
		mi_node_type node = yggr::container::extract(mi_cont, 10);

		std::cout << node.value().prm3 << std::endl;
		yggr_test_assert(mi_cont.size() == 3);

		mi_insert_return_type ins_ret = yggr::container::insert(mi_cont.get<0>(), boost::move(node));

		yggr_test_assert(ins_ret.inserted);
		yggr_test_assert(ins_ret.position != mi_cont.end());
		yggr_test_assert(!ins_ret.node);
		yggr_test_assert(mi_cont.size() == 4);

		yggr_test_assert(mi_cont.find(10) != mi_cont.get<0>().end());

	}

	{
		mi_node_type node = yggr::container::extract(mi_cont, mi_cont.find(11));

		std::cout << node.value().prm3 << std::endl;
		yggr_test_assert(mi_cont.size() == 3);

		mi_cont_type::iterator ins_ret = yggr::container::insert(mi_cont, mi_cont.end(), boost::move(node));

		yggr_test_assert(ins_ret != mi_cont.end());
		yggr_test_assert(!node);
		yggr_test_assert(mi_cont.size() == 4);

		yggr_test_assert(mi_cont.find(11) != mi_cont.get<0>().end());
	}
}

void test_hash(void)
{
	typedef
		boost::multi_index_container
		<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::hashed_unique<
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::hashed_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>
			>
		> mi_cont_type;

	typedef yggr::container::node_handle_traits<mi_cont_type> node_handle_traits_type;
	typedef node_handle_traits_type::node_type mi_node_type;
	typedef node_handle_traits_type::insert_return_type mi_insert_return_type;

	mi_cont_type mi_cont;

	mi_cont.insert(A(10, 10, "s10"));
	mi_cont.insert(A(11, 10, "s11"));
	mi_cont.insert(A(20, 20, "s20"));
	mi_cont.insert(A(21, 20, "s21"));

	yggr_test_assert(mi_cont.size() == 4);

#if (BOOST_VERSION < 107400)

	typedef yggr::container::detail::cast_to_midx_container_hacking<mi_cont_type>::type hacking_mi_cont_type;

	{
		hacking_mi_cont_type& hacking_cont = reinterpret_cast<hacking_mi_cont_type&>(mi_cont);

		hacking_mi_cont_type::node_type node = hacking_cont.extract(10);

		std::cout << node.value().prm3 << std::endl;
		yggr_test_assert(mi_cont.size() == 3);

		hacking_mi_cont_type::insert_return_type ins_ret = hacking_cont.insert(boost::move(node));
		yggr_test_assert(ins_ret.inserted);
		yggr_test_assert(ins_ret.position != mi_cont.end());
		yggr_test_assert(!ins_ret.node);
		yggr_test_assert(mi_cont.size() == 4);

		yggr_test_assert(mi_cont.find(10) != mi_cont.get<0>().end());
	}

	{
		hacking_mi_cont_type& hacking_cont = reinterpret_cast<hacking_mi_cont_type&>(mi_cont);

		hacking_mi_cont_type::node_type node = hacking_cont.extract(mi_cont.find(11));
		std::cout << node.value().prm3 << std::endl;
		yggr_test_assert(mi_cont.size() == 3);

		hacking_mi_cont_type::iterator ins_ret = hacking_cont.insert(mi_cont.end(), boost::move(node));

		yggr_test_assert(ins_ret != mi_cont.end());
		yggr_test_assert(!node);
		yggr_test_assert(mi_cont.size() == 4);

		yggr_test_assert(mi_cont.find(11) != mi_cont.get<0>().end());
	}

#endif // (BOOST_VERSION < 107400)

	{
		mi_node_type node = yggr::container::extract(mi_cont, 10);

		std::cout << node.value().prm3 << std::endl;
		yggr_test_assert(mi_cont.size() == 3);

		mi_insert_return_type ins_ret = yggr::container::insert(mi_cont.get<0>(), boost::move(node));

		yggr_test_assert(ins_ret.inserted);
		yggr_test_assert(ins_ret.position != mi_cont.end());
		yggr_test_assert(!ins_ret.node);
		yggr_test_assert(mi_cont.size() == 4);

		yggr_test_assert(mi_cont.find(10) != mi_cont.get<0>().end());

	}

	{
		mi_node_type node = yggr::container::extract(mi_cont, mi_cont.find(11));

		std::cout << node.value().prm3 << std::endl;
		yggr_test_assert(mi_cont.size() == 3);

		mi_cont_type::iterator ins_ret = yggr::container::insert(mi_cont, mi_cont.end(), boost::move(node));

		yggr_test_assert(ins_ret != mi_cont.end());
		yggr_test_assert(!node);
		yggr_test_assert(mi_cont.size() == 4);

		yggr_test_assert(mi_cont.find(11) != mi_cont.get<0>().end());
	}
}

void test_sequenced(void)
{
	typedef
		boost::multi_index_container
		<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::sequenced<>
			>
		> mi_cont_type;

	typedef yggr::container::node_handle_traits<mi_cont_type> node_handle_traits_type;
	typedef node_handle_traits_type::node_type mi_node_type;
	typedef node_handle_traits_type::insert_return_type mi_insert_return_type;

	mi_cont_type mi_cont;

	mi_cont.push_back(A(10, 10, "s10"));
	mi_cont.push_back(A(11, 10, "s11"));
	mi_cont.push_back(A(20, 20, "s20"));
	mi_cont.push_back(A(21, 20, "s21"));

	yggr_test_assert(mi_cont.size() == 4);

#if (BOOST_VERSION < 107400)

	typedef yggr::container::detail::cast_to_midx_container_hacking<mi_cont_type>::type hacking_mi_cont_type;

	{
		hacking_mi_cont_type& hacking_cont = reinterpret_cast<hacking_mi_cont_type&>(mi_cont);

		hacking_mi_cont_type::node_type node = hacking_cont.extract(mi_cont.begin());

		int val = node.value().prm1;
		std::cout << node.value().prm3 << std::endl;
		yggr_test_assert(mi_cont.size() == 3);

		mi_insert_return_type ins_ret = hacking_cont.insert(mi_cont.end(), boost::move(node));
		yggr_test_assert(ins_ret.inserted);
		yggr_test_assert(ins_ret.position != mi_cont.end());
		yggr_test_assert(!ins_ret.node);
		yggr_test_assert(mi_cont.size() == 4);

		yggr_test_assert(mi_cont.back().prm1 == val);
	}

#endif // (BOOST_VERSION < 107400)
	{
		mi_node_type node = yggr::container::extract(mi_cont, mi_cont.begin());

		int val = node.value().prm1;
		std::cout << node.value().prm3 << std::endl;
		yggr_test_assert(mi_cont.size() == 3);

		mi_insert_return_type ins_ret = yggr::container::insert(mi_cont, mi_cont.end(), boost::move(node));

		yggr_test_assert(ins_ret.inserted);
		yggr_test_assert(ins_ret.position != mi_cont.end());
		yggr_test_assert(!ins_ret.node);
		yggr_test_assert(mi_cont.size() == 4);

		yggr_test_assert(mi_cont.back().prm1 == val);
	}
}

void test_random_access(void)
{
	typedef
		boost::multi_index_container
		<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::random_access<>
			>
		> mi_cont_type;

	typedef yggr::container::node_handle_traits<mi_cont_type> node_handle_traits_type;
	typedef node_handle_traits_type::node_type mi_node_type;
	typedef node_handle_traits_type::insert_return_type mi_insert_return_type;

	mi_cont_type mi_cont;

	mi_cont.push_back(A(10, 10, "s10"));
	mi_cont.push_back(A(11, 10, "s11"));
	mi_cont.push_back(A(20, 20, "s20"));
	mi_cont.push_back(A(21, 20, "s21"));

	yggr_test_assert(mi_cont.size() == 4);

#if (BOOST_VERSION < 107400)

	typedef yggr::container::detail::cast_to_midx_container_hacking<mi_cont_type>::type hacking_mi_cont_type;

	{
		hacking_mi_cont_type& hacking_cont = reinterpret_cast<hacking_mi_cont_type&>(mi_cont);

		hacking_mi_cont_type::node_type node = hacking_cont.extract(mi_cont.begin());

		int val = node.value().prm1;
		std::cout << node.value().prm3 << std::endl;
		yggr_test_assert(mi_cont.size() == 3);

		mi_insert_return_type ins_ret = hacking_cont.insert(mi_cont.end(), boost::move(node));
		yggr_test_assert(ins_ret.inserted);
		yggr_test_assert(ins_ret.position != mi_cont.end());
		yggr_test_assert(!ins_ret.node);
		yggr_test_assert(mi_cont.size() == 4);

		yggr_test_assert(mi_cont.back().prm1 == val);
	}

#endif // (BOOST_VERSION < 107400)

	{
		mi_node_type node = yggr::container::extract(mi_cont, mi_cont.begin());

		int val = node.value().prm1;
		std::cout << node.value().prm3 << std::endl;
		yggr_test_assert(mi_cont.size() == 3);

		mi_insert_return_type ins_ret = yggr::container::insert(mi_cont, mi_cont.end(), boost::move(node));

		yggr_test_assert(ins_ret.inserted);
		yggr_test_assert(ins_ret.position != mi_cont.end());
		yggr_test_assert(!ins_ret.node);
		yggr_test_assert(mi_cont.size() == 4);

		yggr_test_assert(mi_cont.back().prm1 == val);
	}
}

void test_random_access2(void)
{
	typedef 
		boost::multi_index::multi_index_container
		<
			A,
			boost::multi_index::indexed_by
			<
				boost::multi_index::random_access< boost::multi_index::tag<A::tag_prm1> >,
				boost::multi_index::random_access< boost::multi_index::tag<A::tag_prm2> >
			>
		> mi_cont_type;

	typedef yggr::container::node_handle_traits<mi_cont_type> node_handle_traits_type;
	typedef node_handle_traits_type::node_type mi_node_type;
	typedef node_handle_traits_type::insert_return_type mi_insert_return_type;

	

	typedef boost::multi_index::nth_index<mi_cont_type, 0>::type index0_type;
	typedef index0_type::iterator iterator0_type;
	typedef yggr::container::node_handle_traits<index0_type> node_handle_traits0_type;
	typedef node_handle_traits0_type::node_type node0_type;
	typedef node_handle_traits0_type::insert_return_type insert_return0_type;

	typedef boost::multi_index::nth_index<mi_cont_type, 1>::type index1_type;
	typedef index1_type::iterator iterator1_type;
	typedef yggr::container::node_handle_traits<index1_type> node_handle_traits1_type;
	typedef node_handle_traits1_type::node_type node1_type;
	typedef node_handle_traits1_type::insert_return_type insert_return1_type;

	A arr[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};

	mi_cont_type cont;
	cont.push_back(arr[0]);
	cont.push_back(arr[1]);
	cont.push_back(arr[2]);

	//cont.erase(cont.begin());

	{
		node0_type node = yggr::container::extract(cont, cont.begin());
		insert_return0_type ins = yggr::container::insert(cont, cont.end(), boost::move(node));
		yggr_test_assert(ins.inserted);
	}

	{
		node0_type node = yggr::container::extract(cont, cont.begin());
		insert_return0_type ins = yggr::container::insert(cont, cont.end(), boost::move(node));
		yggr_test_assert(ins.inserted);
	}

	{
		node0_type node = yggr::container::extract(cont, cont.begin());
		insert_return0_type ins = yggr::container::insert(cont, cont.end(), boost::move(node));
		yggr_test_assert(ins.inserted);
	}

	{
		node0_type node = yggr::container::extract(cont.get<0>(), cont.get<0>().begin());
		insert_return1_type ins = yggr::container::insert(cont.get<1>(), cont.get<1>().end(), boost::move(node));
		yggr_test_assert(ins.inserted);
	}

	{
		node0_type node = yggr::container::extract(cont.get<0>(), cont.get<0>().begin());
		insert_return1_type ins = yggr::container::insert(cont.get<1>(), cont.get<1>().end(), boost::move(node));
		yggr_test_assert(ins.inserted);
	}

	{
		node0_type node = yggr::container::extract(cont.get<0>(), cont.get<0>().begin());
		insert_return1_type ins = yggr::container::insert(cont.get<1>(), cont.get<1>().end(), boost::move(node));
		yggr_test_assert(ins.inserted);
	}
	
	{
		node1_type node = yggr::container::extract(cont.get<1>(), cont.get<1>().begin());
		insert_return1_type ins = yggr::container::insert(cont.get<1>(), cont.get<1>().end(), boost::move(node));
		yggr_test_assert(ins.inserted);
	}

	{
		node1_type node = yggr::container::extract(cont.get<1>(), cont.get<1>().begin());
		insert_return1_type ins = yggr::container::insert(cont.get<1>(), cont.get<1>().end(), boost::move(node));
		yggr_test_assert(ins.inserted);
	}

	{
		node1_type node = yggr::container::extract(cont.get<1>(), cont.get<1>().begin());
		insert_return1_type ins = yggr::container::insert(cont.get<1>(), cont.get<1>().end(), boost::move(node));
		yggr_test_assert(ins.inserted);
	}
}


#if defined(YGGR_BOOST_MULTI_INDEX_HAS_RANKED_INDEX)

void test_rank(void)
{
	typedef
		boost::multi_index_container
		<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::ranked_unique<
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::ranked_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>
			>
		> mi_cont_type;

	typedef yggr::container::node_handle_traits<mi_cont_type> node_handle_traits_type;
	
	typedef node_handle_traits_type::node_type mi_node_type;
	typedef node_handle_traits_type::insert_return_type mi_insert_return_type;

	mi_cont_type mi_cont;

	mi_cont.insert(A(10, 10, "s10"));
	mi_cont.insert(A(11, 10, "s11"));
	mi_cont.insert(A(20, 20, "s20"));
	mi_cont.insert(A(21, 20, "s21"));

	yggr_test_assert(mi_cont.size() == 4);

#if (BOOST_VERSION < 107400)

	typedef yggr::container::detail::cast_to_midx_container_hacking<mi_cont_type>::type hacking_mi_cont_type;

	{
		hacking_mi_cont_type& hacking_cont = reinterpret_cast<hacking_mi_cont_type&>(mi_cont);

		hacking_mi_cont_type::node_type node = hacking_cont.extract(10);

		std::cout << node.value().prm3 << std::endl;
		yggr_test_assert(mi_cont.size() == 3);

		hacking_mi_cont_type::insert_return_type ins_ret = hacking_cont.insert(boost::move(node));
		yggr_test_assert(ins_ret.inserted);
		yggr_test_assert(ins_ret.position != mi_cont.end());
		yggr_test_assert(!ins_ret.node);
		yggr_test_assert(mi_cont.size() == 4);

		yggr_test_assert(mi_cont.find(10) != mi_cont.get<0>().end());
	}

	{
		hacking_mi_cont_type& hacking_cont = reinterpret_cast<hacking_mi_cont_type&>(mi_cont);

		hacking_mi_cont_type::node_type node = hacking_cont.extract(mi_cont.find(11));
		std::cout << node.value().prm3 << std::endl;
		yggr_test_assert(mi_cont.size() == 3);

		hacking_mi_cont_type::iterator ins_ret = hacking_cont.insert(mi_cont.end(), boost::move(node));

		yggr_test_assert(ins_ret != mi_cont.end());
		yggr_test_assert(!node);
		yggr_test_assert(mi_cont.size() == 4);

		yggr_test_assert(mi_cont.find(11) != mi_cont.get<0>().end());
	}

#endif // (BOOST_VERSION < 107400)

	{
		mi_node_type node = yggr::container::extract(mi_cont, 10);

		std::cout << node.value().prm3 << std::endl;
		yggr_test_assert(mi_cont.size() == 3);

		mi_insert_return_type ins_ret = yggr::container::insert(mi_cont.get<0>(), boost::move(node));

		yggr_test_assert(ins_ret.inserted);
		yggr_test_assert(ins_ret.position != mi_cont.end());
		yggr_test_assert(!ins_ret.node);
		yggr_test_assert(mi_cont.size() == 4);

		yggr_test_assert(mi_cont.find(10) != mi_cont.get<0>().end());

	}

	{
		mi_node_type node = yggr::container::extract(mi_cont, mi_cont.find(11));

		std::cout << node.value().prm3 << std::endl;
		yggr_test_assert(mi_cont.size() == 3);

		mi_cont_type::iterator ins_ret = yggr::container::insert(mi_cont, mi_cont.end(), boost::move(node));

		yggr_test_assert(ins_ret != mi_cont.end());
		yggr_test_assert(!node);
		yggr_test_assert(mi_cont.size() == 4);

		yggr_test_assert(mi_cont.find(11) != mi_cont.get<0>().end());
	}
}
#endif // YGGR_BOOST_MULTI_INDEX_HAS_RANKED_INDEX

int main(int argc, char* argv[])
{
	test_ord();
	test_hash();
	test_sequenced();
	test_random_access();
	test_random_access2();

#if defined(YGGR_BOOST_MULTI_INDEX_HAS_RANKED_INDEX)
	test_rank();
#endif // YGGR_BOOST_MULTI_INDEX_HAS_RANKED_INDEX

	wait_any_key(argc, argv);
	return 0;
}