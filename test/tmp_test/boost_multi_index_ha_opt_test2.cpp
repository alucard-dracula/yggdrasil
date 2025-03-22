// boost_multi_index_ha_opt_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

//#define YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#ifdef _DEBUG
#	define BOOST_MULTI_INDEX_ENABLE_SAFE_MODE
#	define BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING
#endif // _DEBUG

#include <boost/bind.hpp>

#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/member.hpp>

#include <yggr/move/move.hpp>
#include <yggr/container/vector.hpp>

#include <yggr/safe_container/detail/boost_midx_random_access_index_opt.hpp>
#include <yggr/safe_container/detail/boost_midx_sequenced_index_opt.hpp>
#include <yggr/safe_container/detail/boost_midx_ordered_index_opt.hpp>
#include <yggr/safe_container/detail/boost_midx_hashed_index_opt.hpp>
#include <yggr/safe_container/detail/boost_midx_container_opt.hpp>

//#include <yggr/safe_container/detail/boost_midx_keyinfo_helper.hpp>

//#include <yggr/ppex/pp_debug.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER


#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

struct A
{
	A(void)
		: a(), b(), c()
	{
	}

	A(int na, int nb, int nc)
		: a(na), b(nb), c(nc)
	{
	}

	struct tag_1{};
	struct tag_2{};
	struct tag_3{};
	struct tag_4{};
	struct tag_5{};

	float a;
	float b;
	float c;
};

typedef 
	boost::multi_index::multi_index_container
	<
		A,
		boost::multi_index::indexed_by<
			//boost::multi_index::hashed_unique<
			//	boost::multi_index::tag<A::tag_1>,
			//	BOOST_MULTI_INDEX_MEMBER(A, float, a) >//,

			boost::multi_index::hashed_non_unique<
				boost::multi_index::tag<A::tag_2>,
				BOOST_MULTI_INDEX_MEMBER(A, float, b)>//,

			//boost::multi_index::ordered_unique<
			//	boost::multi_index::tag<A::tag_3>,
			//	BOOST_MULTI_INDEX_MEMBER(A, float, a) >,

			//boost::multi_index::ordered_non_unique<
			//	boost::multi_index::tag<A::tag_4>,
			//	BOOST_MULTI_INDEX_MEMBER(A, float, b)>,

			//boost::multi_index::sequenced<>,
			//boost::multi_index::random_access<>
		>
	> org_type;

typedef std::vector<A> val_vt_type;
typedef yggr::safe_container::detail::cast_to_midx_container_opt<org_type>::type opt_type;
typedef opt_type::node_ptr_type node_ptr_type;
typedef opt_type::destroy_holder_type destroy_holder_type;
typedef opt_type::multi_destroy_holder_type multi_destroy_holder_type;

typedef std::vector<node_ptr_type> node_ptr_vt_type;


void test_basic(void)
{
}

template<typename Val, typename ModVal>
void modifier(Val& left, const ModVal& right)
{
	left = right;
	int nnn = 0;
}

template<typename Val, typename ModeVal>
void modifier2(Val& left, const ModeVal& right, Val& tmp)
{
	tmp = boost::move(left);
	left = right;
}

template<typename Val>
void rollback(Val& left, const Val& tmp)
{
	left = tmp;
}

void test(void)
{
	yggr_test_assert((sizeof(org_type) == sizeof(opt_type)));

	{
		org_type org_cont;


		//org_cont.insert(A(1, 1, 1));
		//org_cont.insert(A(2, 2, 2));

		//A val(3, 3, 3);
		//org_cont.insert(val);

		opt_type& opt_cont = reinterpret_cast<opt_type&>(org_cont);

		org_cont.clear();

		//{
		//	org_cont.insert(A(5, 5, 5));
		//	yggr_test_assert(org_cont.size() == 1);

		//	{
		//		destroy_holder_type holder;
		//		opt_cont.insert(holder, 
		//						opt_cont.s_gen_opt_keyinfo_params(org_cont, A(4, 4, 4)), 
		//						opt_type::create_node(org_cont.get_allocator(), A(4, 4, 4)));
		//		yggr_test_assert(org_cont.size() == 2);
		//	}

		//	yggr_test_assert(org_cont.equal_range(5) == opt_cont.equal_range(opt_cont.s_hash(org_cont.hash_function(), 5), 5));

		//	yggr_test_assert(org_cont.find(5) == org_cont.find(5));
		//	org_cont.clear();

		//	org_cont.insert(A(4, 5, 6));
		//	org_cont.insert(A(4, 5, 6));

		//	// nth_layer_cast test

		//	yggr_test_assert(org_cont.find(4) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 4), 4));
		//	yggr_test_assert(org_cont.get<A::tag_1>().find(4)
		//			== opt_cont.get<A::tag_1>()
		//				.find(opt_cont.get<A::tag_1>().s_hash(
		//						org_cont.get<A::tag_1>().hash_function(), 4), 4));

		//	yggr_test_assert(opt_cont.org_get<A::tag_1>().find(4)
		//			== opt_cont.get<A::tag_1>()
		//				.find(opt_cont.get<A::tag_1>().s_hash(
		//						org_cont.get<A::tag_1>().hash_function(), 4), 4));

		//	yggr_test_assert(org_cont.get<A::tag_2>().find(5)
		//			== opt_cont.get<A::tag_2>()
		//				.find(opt_cont.get<A::tag_2>().s_hash(
		//						org_cont.get<A::tag_2>().hash_function(), 5), 5));

		//	yggr_test_assert(opt_cont.org_get<1>().find(5)
		//			== opt_cont.get<1>()
		//				.find(opt_cont.get<1>().s_hash(
		//						org_cont.get<1>().hash_function(), 5), 5));

		//	yggr_test_assert((*opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 4), 4)).a == 4);
		//	yggr_test_assert((*opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 4), 4)).b == 5);
		//	yggr_test_assert((*opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 4), 4)).c == 6);
		//	yggr_test_assert(opt_cont.count(opt_type::s_hash(org_cont.hash_function(), 4), 4) == 1);

		//	yggr_test_assert(org_cont.size() == 1);
		//}

		{
			typedef opt_type::keyinfo_params_type keyinfo_params_type;
			typedef std::vector<keyinfo_params_type> hash_params_type;

			val_vt_type val_vt;
			val_vt.push_back(A(6, 6, 6));
			val_vt.push_back(A(7, 7, 7));
			val_vt.push_back(A(7, 7, 7));

			hash_params_type hash_params;
			destroy_holder_type holder;
			node_ptr_vt_type node_ptr_vt;

#if 0
			org_cont.insert(val_vt.begin(), val_vt.end());
#else
			opt_cont.insert(
				holder,
				opt_cont.s_gen_opt_keyinfo_params_range(
					hash_params, org_cont,
					val_vt.begin(), val_vt.end()),
				opt_type::create_node_range(node_ptr_vt, org_cont.get_allocator(), val_vt.begin(), val_vt.end()));
#endif // 0, 1

			yggr_test_assert(org_cont.find(6) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 6), 6));
			yggr_test_assert((*opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 6), 6)).a == 6);

			yggr_test_assert(org_cont.find(7) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 7), 7));
			yggr_test_assert((*opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 7), 7)).a == 7);
			yggr_test_assert(org_cont.size() == 2);
			//yggr_test_assert(org_cont.size() == 3);
		}

	//	{
	//		org_cont.insert(A(9, 9, 9));

	//		{
	//			destroy_holder_type holder;
	//			opt_cont.insert(holder, 
	//							opt_cont.s_gen_opt_keyinfo_params(org_cont, A(8, 8, 8)), 
	//							opt_type::create_node(org_cont.get_allocator(), A(8, 8, 8)));
	//		}

	//		yggr_test_assert(org_cont.find(8) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 8), 8));
	//		yggr_test_assert((*opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 8), 8)).a == 8);

	//		yggr_test_assert(org_cont.find(9) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 9), 9));
	//		yggr_test_assert((*opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 9), 9)).a == 9);
	//	}

	//	{

	//		destroy_holder_type holder;
	//		opt_cont.erase(holder, opt_type::s_hash(org_cont.hash_function(), 4), 4);

	//		yggr_test_assert(org_cont.find(4) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 4), 4));
	//		yggr_test_assert(opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 4), 4) == org_cont.end());
	//	}

	//	{
	//		yggr_test_assert(org_cont.count(6) == opt_cont.count(opt_type::s_hash(org_cont.hash_function(), 6), 6));
	//		yggr_test_assert(org_cont.count(7) == opt_cont.count(opt_type::s_hash(org_cont.hash_function(), 7), 7));
	//		yggr_test_assert(org_cont.count(100) == opt_cont.count(opt_type::s_hash(org_cont.hash_function(), 100), 100));

	//		yggr_test_assert(org_cont.count(6, org_cont.hash_function(), org_cont.key_eq())
	//				== opt_cont.count(opt_type::s_hash(org_cont.hash_function(), 6), 6, org_cont.key_eq()));
	//		yggr_test_assert(org_cont.count(7, org_cont.hash_function(), org_cont.key_eq())
	//				== opt_cont.count(opt_type::s_hash(org_cont.hash_function(), 7), 7, org_cont.key_eq()));
	//		yggr_test_assert(org_cont.count(100, org_cont.hash_function(), org_cont.key_eq())
	//				== opt_cont.count(opt_type::s_hash(org_cont.hash_function(), 100), 100, org_cont.key_eq()));
	//	}

	//	{
	//		yggr_test_assert(org_cont.bucket(6) == opt_cont.bucket(opt_type::s_hash(org_cont.hash_function(), 6)));
	//		yggr_test_assert(org_cont.bucket(7) == opt_cont.bucket(opt_type::s_hash(org_cont.hash_function(), 7)));
	//		yggr_test_assert(org_cont.bucket(100) == opt_cont.bucket(opt_type::s_hash(org_cont.hash_function(), 100)));
	//	}

	//	{
	//		org_cont.insert(A(100, 100, 100));

	//		bool b = opt_cont.replace(org_cont.find(100), 
	//									opt_cont.s_gen_opt_keyinfo_params(org_cont, A(200, 300, 400)), 
	//									A(200, 300, 400));
	//		yggr_test_assert(b);

	//		yggr_test_assert(org_cont.find(100) == org_cont.end());
	//		yggr_test_assert(org_cont.find(200) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 200), 200));

	//		yggr_test_assert(org_cont.find(100) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 100), 100));
	//		yggr_test_assert(org_cont.find(200) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 200), 200));
	//		yggr_test_assert((*opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 200), 200)).a == 200);
	//		yggr_test_assert((*opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 200), 200)).b == 300);
	//		yggr_test_assert((*opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 200), 200)).c == 400);
	//	}

	//	{
	//		org_cont.insert(A(101, 101, 101));
	//		org_cont.insert(A(102, 102, 102));

	//		{
	//			destroy_holder_type holder;
	//			bool b = opt_cont.modify(holder, org_cont.find(101),
	//										opt_cont.s_gen_opt_keyinfo_params(org_cont, A(201, 301, 401)), 
	//										boost::bind(&modifier<A, A>, _1, A(201, 301, 401)));
	//			yggr_test_assert(b);

	//			yggr_test_assert(org_cont.find(101) == org_cont.end());
	//			yggr_test_assert(org_cont.find(201) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 201), 201));

	//			yggr_test_assert(org_cont.find(101) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 101), 101));
	//			yggr_test_assert(org_cont.find(201) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 201), 201));
	//			yggr_test_assert((*opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 201), 201)).a == 201);
	//			yggr_test_assert((*opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 201), 201)).b == 301);
	//			yggr_test_assert((*opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 201), 201)).c == 401);
	//		}

	//		{
	//			destroy_holder_type holder;
	//			bool b = opt_cont.modify(holder, org_cont.find(102), 
	//										opt_cont.s_gen_opt_keyinfo_params(org_cont, A(201, 301, 401)), 
	//										boost::bind(&modifier<A, A>, _1, A(201, 301, 401)));
	//			yggr_test_assert(!b);

	//			yggr_test_assert(org_cont.find(101) == org_cont.end());
	//			yggr_test_assert(org_cont.find(201) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 201), 201));

	//			yggr_test_assert(org_cont.find(101) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 101), 101));
	//			yggr_test_assert(org_cont.find(201) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 201), 201));
	//			yggr_test_assert((*opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 201), 201)).a == 201);
	//			yggr_test_assert((*opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 201), 201)).b == 301);
	//			yggr_test_assert((*opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 201), 201)).c == 401);
	//		}
	//	}

	//	{
	//		org_cont.insert(A(103, 103, 103));
	//		org_cont.insert(A(104, 104, 104));

	//		A mod_val_tmp(0, 0, 0);

	//		{
	//			destroy_holder_type holder;
	//			bool b = opt_cont.modify(holder, org_cont.find(103), 
	//										opt_cont.s_gen_opt_keyinfo_params(org_cont, A(202, 302, 402)), 
	//										boost::bind(&modifier2<A, A>, _1, A(202, 302, 402), boost::ref(mod_val_tmp)), 
	//										boost::bind(&rollback<A>, _1, boost::cref(mod_val_tmp)) );
	//			yggr_test_assert(b);

	//			yggr_test_assert(org_cont.find(103) == org_cont.end());
	//			yggr_test_assert(org_cont.find(202) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 202), 202));

	//			yggr_test_assert(org_cont.find(103) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 103), 103));
	//			yggr_test_assert(org_cont.find(202) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 202), 202));
	//			yggr_test_assert((*opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 202), 202)).a == 202);
	//			yggr_test_assert((*opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 202), 202)).b == 302);
	//			yggr_test_assert((*opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 202), 202)).c == 402);
	//		}

	//		{
	//			destroy_holder_type holder;
	//			bool b = opt_cont.modify(holder, org_cont.find(104), 
	//										opt_cont.s_gen_opt_keyinfo_params(org_cont, A(202, 302, 402)), 
	//										boost::bind(&modifier2<A, A>, _1, A(202, 302, 402), boost::ref(mod_val_tmp)), 
	//										boost::bind(&rollback<A>, _1, boost::cref(mod_val_tmp)) );
	//			yggr_test_assert(!b);

	//			yggr_test_assert(org_cont.find(103) == org_cont.end());
	//			yggr_test_assert(org_cont.find(202) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 202), 202));

	//			yggr_test_assert(org_cont.find(103) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 103), 103));
	//			yggr_test_assert(org_cont.find(202) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 202), 202));
	//			yggr_test_assert((*opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 202), 202)).a == 202);
	//			yggr_test_assert((*opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 202), 202)).b == 302);
	//			yggr_test_assert((*opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 202), 202)).c == 402);
	//		}
	//	}

	//	{
	//		org_cont.insert(A(105, 105, 105));
	//		org_cont.insert(A(106, 106, 106));

	//		{
	//			destroy_holder_type holder;
	//			bool b = opt_cont.modify_key(holder, org_cont.find(105), 
	//											opt_cont.s_gen_opt_keyinfo_params(org_cont, A(203, 105, 105)), 
	//											boost::bind(&modifier<float, float>, _1, 203));
	//			yggr_test_assert(b);

	//			yggr_test_assert(org_cont.find(105) == org_cont.end());
	//			yggr_test_assert(org_cont.find(203) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 203), 203));

	//			yggr_test_assert(org_cont.find(105) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 105), 105));
	//			yggr_test_assert(org_cont.find(203) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 203), 203));
	//			yggr_test_assert((*opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 203), 203)).a == 203);
	//			yggr_test_assert((*opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 203), 203)).b == 105);
	//			yggr_test_assert((*opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 203), 203)).c == 105);
	//		}

	//		{
	//			destroy_holder_type holder;
	//			bool b = opt_cont.modify_key(holder, org_cont.find(106), 
	//											opt_cont.s_gen_opt_keyinfo_params(org_cont, A(203, 106, 106)), 
	//											boost::bind(&modifier<float, float>, _1, 203));
	//			yggr_test_assert(!b);

	//			yggr_test_assert(org_cont.find(105) == org_cont.end());
	//			yggr_test_assert(org_cont.find(203) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 203), 203));

	//			yggr_test_assert(org_cont.find(105) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 105), 105));
	//			yggr_test_assert(org_cont.find(203) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 203), 203));
	//			yggr_test_assert((*opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 203), 203)).a == 203);
	//			yggr_test_assert((*opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 203), 203)).b == 105);
	//			yggr_test_assert((*opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 203), 203)).c == 105);
	//		}
	//	}

	//	{
	//		org_cont.insert(A(107, 107, 107));
	//		org_cont.insert(A(108, 108, 108));

	//		float mod_val_tmp = 0;

	//		{
	//			destroy_holder_type holder;
	//			bool b = opt_cont.modify_key(holder, org_cont.find(107), 
	//											opt_cont.s_gen_opt_keyinfo_params(org_cont, A(205, 107, 107)), 
	//											boost::bind(&modifier2<float, float>, _1, 205, boost::ref(mod_val_tmp)), 
	//											boost::bind(&rollback<float>, _1, boost::cref(mod_val_tmp)) );
	//			yggr_test_assert(b);

	//			yggr_test_assert(org_cont.find(107) == org_cont.end());
	//			yggr_test_assert(org_cont.find(205) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 205), 205));

	//			yggr_test_assert(org_cont.find(107) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 107), 107));
	//			yggr_test_assert(org_cont.find(205) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 205), 205));
	//			yggr_test_assert((*opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 205), 205)).a == 205);
	//			yggr_test_assert((*opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 205), 205)).b == 107);
	//			yggr_test_assert((*opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 205), 205)).c == 107);
	//		}

	//		{
	//			destroy_holder_type holder;
	//			bool b = opt_cont.modify_key(holder, org_cont.find(108), 
	//											opt_cont.s_gen_opt_keyinfo_params(org_cont, A(205, 108, 108)), 
	//											boost::bind(&modifier2<float, float>, _1, 205, boost::ref(mod_val_tmp)), 
	//											boost::bind(&rollback<float>, _1, boost::cref(mod_val_tmp)) );
	//			yggr_test_assert(!b);

	//			yggr_test_assert(org_cont.find(107) == org_cont.end());
	//			yggr_test_assert(org_cont.find(205) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 205), 205));

	//			yggr_test_assert(org_cont.find(107) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 107), 107));
	//			yggr_test_assert(org_cont.find(205) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 205), 205));
	//			yggr_test_assert((*opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 205), 205)).a == 205);
	//			yggr_test_assert((*opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 205), 205)).b == 107);
	//			yggr_test_assert((*opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 205), 205)).c == 107);
	//		}
	//	}

	//	yggr_test_assert(org_cont.find(1) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 1), 1));
	//	org_cont.erase(1);
	//	
	//	{
	//		std::size_t old_size = org_cont.size();
	//		destroy_holder_type holder;
	//		opt_cont.erase(holder, org_cont.begin());
	//		yggr_test_assert(holder.size() == 1);
	//		yggr_test_assert(old_size - 1 == org_cont.size());
	//	}

	//	{
	//		destroy_holder_type holder;
	//		opt_cont.erase(holder, org_cont.begin(), org_cont.end());
	//		yggr_test_assert(org_cont.empty());
	//	}
	}
}
#else
void test_basic(void)
{
}

void test(void)
{
	std::cout << "!!! not using optimization !!!" << std::endl;
}
#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

int main(int argc, char* argv[])
{
	//test_basic();
	test();

	wait_any_key(argc, argv);
	return 0;
}
