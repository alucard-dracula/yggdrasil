// boost_multi_index_random_access_index_a_opt_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <vector>
#include <cassert>

//#define YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#ifdef _DEBUG
#	define BOOST_MULTI_INDEX_ENABLE_SAFE_MODE
#	define BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING
#endif // _DEBUG

#include <boost/bind.hpp>

//#include <boost/multi_index/hashed_index.hpp>
//#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/random_access_index.hpp>

#include <yggr/move/move.hpp>

#include <yggr/safe_container/detail/boost_midx_random_access_index_opt.hpp>
#include <yggr/safe_container/detail/boost_midx_container_opt.hpp>

//#include <yggr/safe_container/detail/boost_midx_keyhash_helper.hpp>

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

	float a;
	float b;
	float c;
};

bool operator==(const A& l, const A& r)
{
	return 0 == memcmp(&l, &r, sizeof(A));
}

typedef boost::multi_index::multi_index_container<
		A,
		boost::multi_index::indexed_by<
			boost::multi_index::random_access<>
		>
	> org_type;

typedef std::vector<A> val_vt_type;
typedef yggr::safe_container::detail::cast_to_midx_container_opt<org_type>::type opt_type;
typedef opt_type::node_ptr_type node_ptr_type;
typedef std::vector<node_ptr_type> node_ptr_vt_type;
typedef opt_type::destroy_holder_type destroy_holder_type;
typedef opt_type::multi_destroy_holder_type multi_destroy_holder_type;


void test_basic(void)
{
}

template<typename Val, typename ModVal>
void modifier(Val& left, const ModVal& right)
{
	left = right;
	//int nnn = 0;
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

		org_cont.push_back(A(1, 1, 1));
		org_cont.push_back(A(2, 2, 2));

		A val(3, 3, 3);
		org_cont.push_back(val);

		//opt_type* ptr = 0;

		opt_type& opt_cont = reinterpret_cast<opt_type&>(org_cont);

		org_cont.clear();

		{
			org_cont.push_back(A(5, 5, 5));
			yggr_test_assert(org_cont.size() == 1);

			{
				destroy_holder_type holder;
				opt_cont.push_back(holder,
									opt_cont.s_gen_opt_keyinfo_params(org_cont, A(4, 4, 4)),
									opt_type::create_node(org_cont.get_allocator(), A(4, 4, 4)));
				yggr_test_assert(org_cont.size() == 2);
				yggr_test_assert(holder.size() == 0);
			}

			{
				destroy_holder_type holder;
				opt_cont.push_front(holder,
									opt_cont.s_gen_opt_keyinfo_params(org_cont, A(4, 4, 4)),
									opt_type::create_node(org_cont.get_allocator(), A(4, 4, 4)));
				yggr_test_assert(org_cont.size() == 3);
				yggr_test_assert(holder.size() == 0);
			}

			org_cont.clear();

			destroy_holder_type holder;

			opt_cont.insert(holder, org_cont.begin(),
							opt_cont.s_gen_opt_keyinfo_params(org_cont, A(4, 5, 6)),
							opt_type::create_node(org_cont.get_allocator(), A(4, 5, 6)));

			opt_cont.insert(holder, org_cont.end(),
								opt_cont.s_gen_opt_keyinfo_params(org_cont, A(4, 5, 6)),
								opt_type::create_node(org_cont.get_allocator(), A(4, 5, 6)));

			yggr_test_assert(org_cont.size() == 2);
			yggr_test_assert(holder.size() == 0);
		}

		{
			typedef opt_type::keyinfo_params_type keyinfo_params_type;
			typedef std::vector<keyinfo_params_type> keyinfo_params_vt_type;

			val_vt_type val_vt;
			val_vt.push_back(A(6, 6, 6));
			val_vt.push_back(A(7, 7, 7));
			val_vt.push_back(A(7, 7, 7));

			multi_destroy_holder_type holder;
			keyinfo_params_vt_type key_info_params_vt;
			node_ptr_vt_type node_ptr_vt;


			opt_cont.insert(holder, org_cont.begin(),
				opt_cont.s_gen_opt_keyinfo_params_range(
					key_info_params_vt, org_cont,
					val_vt.begin(), val_vt.end()),
				opt_type::create_node_range(
					node_ptr_vt, org_cont.get_allocator(),
					val_vt.begin(), val_vt.end()));

			opt_cont.insert(holder, org_cont.end(),
				opt_cont.s_gen_opt_keyinfo_params_range(
					key_info_params_vt, org_cont,
					val_vt.begin(), val_vt.end()),
				opt_type::create_node_range(
					node_ptr_vt, org_cont.get_allocator(),
					val_vt.begin(), val_vt.end()));

			yggr_test_assert(org_cont.size() == 8);
			yggr_test_assert(org_cont.front().a == 6);
			yggr_test_assert(org_cont.back().a == 7);

			yggr_test_assert(holder.size() == 0);

		}

		{
			org_cont.push_back(A(9, 9, 9));

			{
				destroy_holder_type holder;
				opt_cont.push_back(
					holder,
					opt_type::s_gen_opt_keyinfo_params(org_cont, A(8, 8, 8)),
					opt_type::create_node(org_cont.get_allocator(), A(8, 8, 8)));
			}

			yggr_test_assert(std::find(org_cont.begin(), org_cont.end(), A(8, 8, 8)) != org_cont.end());
			yggr_test_assert(org_cont.back().a == 8);

			yggr_test_assert(std::find(org_cont.begin(), org_cont.end(), A(9, 9, 9)) != org_cont.end());
			yggr_test_assert((*(++org_cont.rbegin())).a == 9);
		}

		{
			std::size_t old_size = org_cont.size();
			destroy_holder_type holder;
			opt_cont.erase(holder, org_cont.begin());
			yggr_test_assert((old_size - 1 == org_cont.size()));
			yggr_test_assert(holder.size() == 1);
		}


		{
			org_cont.push_front(A(100, 100, 100));

			bool b = opt_cont.replace(org_cont.begin(),
										opt_cont.s_gen_opt_keyinfo_params(org_cont, A(200, 300, 400)),
										A(200, 300, 400));
			yggr_test_assert(b);

			yggr_test_assert(org_cont.front() == A(200, 300, 400));
		}

		{
			org_type org_cont;
			org_cont.push_back(A(101, 101, 101));
			org_cont.push_back(A(102, 102, 102));

			opt_type& opt_cont = reinterpret_cast<opt_type&>(org_cont);

			{
				destroy_holder_type holder;
				bool b = opt_cont.modify(holder, --org_cont.end(),
											opt_cont.s_gen_opt_keyinfo_params(org_cont, A(201, 301, 401)),
											boost::bind(&modifier<A, A>, _1, A(201, 301, 401)));
				yggr_test_assert(b);

				yggr_test_assert((org_cont.back()).a == 201);
				yggr_test_assert((org_cont.back()).b == 301);
				yggr_test_assert((org_cont.back()).c == 401);
			}
		}

		{
			org_type org_cont;
			org_cont.push_back(A(101, 101, 101));
			org_cont.push_back(A(102, 102, 102));

			opt_type& opt_cont = reinterpret_cast<opt_type&>(org_cont);

			{
				destroy_holder_type holder;
				bool b = opt_cont.modify(holder, --org_cont.end(),
											boost::bind(&modifier<A, A>, _1, A(201, 301, 401)));
				yggr_test_assert(b);
				yggr_test_assert(holder.size() == 0);

				yggr_test_assert((org_cont.back()).a == 201);
				yggr_test_assert((org_cont.back()).b == 301);
				yggr_test_assert((org_cont.back()).c == 401);
			}
		}

		{
			org_cont.push_back(A(101, 101, 101));
			org_cont.push_back(A(102, 102, 102));

			{
				destroy_holder_type holder;
				bool b = opt_cont.modify(holder, --org_cont.end(),
											opt_cont.s_gen_opt_keyinfo_params(org_cont, A(201, 301, 401)),
											boost::bind(&modifier<A, A>, _1, A(201, 301, 401)));
				yggr_test_assert(b);
				yggr_test_assert(holder.size() == 0);

				yggr_test_assert((org_cont.back()).a == 201);
				yggr_test_assert((org_cont.back()).b == 301);
				yggr_test_assert((org_cont.back()).c == 401);
			}
		}

		{
			org_cont.push_back(A(103, 103, 103));
			org_cont.push_back(A(104, 104, 104));

			A mod_val_tmp(0, 0, 0);

			{
				destroy_holder_type holder;
				bool b = opt_cont.modify(holder, --org_cont.end(),
											opt_cont.s_gen_opt_keyinfo_params(org_cont, A(202, 302, 402)),
											boost::bind(&modifier2<A, A>, _1, A(202, 302, 402), boost::ref(mod_val_tmp)),
											boost::bind(&rollback<A>, _1, boost::cref(mod_val_tmp)) );
				yggr_test_assert(b);

				yggr_test_assert((org_cont.back()).a == 202);
				yggr_test_assert((org_cont.back()).b == 302);
				yggr_test_assert((org_cont.back()).c == 402);
			}
		}

		{
			std::size_t old_size = org_cont.size();
			destroy_holder_type holder;
			opt_cont.remove(holder, A(101, 101, 101));

			yggr_test_assert((old_size - 1 == org_cont.size()));
			yggr_test_assert(holder.size() == 1);
		}

		{

#	if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
			std::size_t old_size = org_cont.size();
			destroy_holder_type holder;
			opt_cont.remove_if(holder,
								std::bind2nd(std::equal_to<A>(), A(103, 103, 103)));

			yggr_test_assert((old_size - 1 == org_cont.size()));
			yggr_test_assert(holder.size() == 1);
#	else
			std::size_t old_size = org_cont.size();
			destroy_holder_type holder;
			opt_cont.remove_if(holder,
								boost::bind(std::equal_to<A>(), _1, A(103, 103, 103)));

			yggr_test_assert((old_size - 1 == org_cont.size()));
			yggr_test_assert(holder.size() == 1);
#	endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		}

		{
			std::size_t old_size = org_cont.size();
			destroy_holder_type holder;
			opt_cont.erase(holder, org_cont.begin(), org_cont.end());

			yggr_test_assert(holder.size() == old_size);
		}

		org_cont.clear();
		yggr_test_assert(org_cont.empty());
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
	test_basic();
	test();

	wait_any_key(argc, argv);
	return 0;
}
