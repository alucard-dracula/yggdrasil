//safe_multi_index_container_thread_safe_test.cpp

#if defined(_MSC_VER) && defined(_DEBUG)
#	include <vld.h>
#endif // _MSC_VER && _DEBUG
#include <iostream>
#include <string>
#include <yggr/safe_container/safe_multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/bind.hpp>

#include <assert.h>

#include <boost/shared_ptr.hpp>
#include <cstdlib>
#include <boost/thread.hpp>
#include <yggr/safe_container/safe_vector.hpp>

struct A
{
	struct tag_prm1{};
	struct tag_prm2{};
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

	int prm1;
	int prm2;
	std::string prm3;
};

template<typename Container>
struct foos_of_hash
{
	typedef Container mi_cont_type;
	typedef foos_of_hash this_type;

	static void handler_insert(typename mi_cont_type::base_type& l, const typename mi_cont_type::base_type& r)
	{
		typedef typename mi_cont_type::base_type base_type;
		typedef typename base_type::template nth_index<0> index_type;
		typedef typename index_type::type cont_type;

		typedef typename cont_type::const_iterator citer_type;

		cont_type& lref = l.template get<0>();
		const cont_type& rcref = r.template get<0>();

		for(citer_type i = rcref.begin(), isize = rcref.end(); i != isize; ++i)
		{
			lref.insert(*i);
		}
	}

	static void handler_check(const typename mi_cont_type::base_type& l, const typename mi_cont_type::base_type& r)
	{
		typedef typename mi_cont_type::base_type base_type;
		typedef typename base_type::template nth_index<0> index_type;
		typedef typename index_type::type cont_type;

		typedef typename cont_type::const_iterator citer_type;

		const cont_type& lcref = l.template get<0>();
		const cont_type& rcref = r.template get<0>();

		for(citer_type i = lcref.begin(), isize = lcref.end(); i != isize; ++i)
		{
			assert(rcref.find(i->prm1) != rcref.end());
		}
	}

	template<typename Handler>
	static void handler_insert_of_ret(typename mi_cont_type::base_type& l, const typename mi_cont_type::base_type& r, const Handler& ret)
	{
		handler_insert(l, r);
		ret();
	}

	template<typename Handler>
	static void handler_check_of_ret(const typename mi_cont_type::base_type& l, const typename mi_cont_type::base_type& r, const Handler& ret)
	{
		handler_check(l, r);
		ret();
	}

	template<typename Handler>
	static void handler_insert_of_ret_helper(mi_cont_type& cont, const typename mi_cont_type::base_type& base_cont, const Handler& ret)
	{
		typedef Handler handler_type;
		cont.use_handler(boost::bind(&this_type::handler_insert_of_ret<handler_type>, _1, boost::cref(base_cont), _2), ret);
	}

	template<typename Handler>
	static void handler_check_of_ret_helper(const mi_cont_type& cont, const typename mi_cont_type::base_type& base_cont, const Handler& ret)
	{
		typedef Handler handler_type;
		cont.use_handler(boost::bind(&this_type::handler_check_of_ret<handler_type>, _1, boost::cref(base_cont), _2), ret);
	}

	static void handler_of_ret(int& count)
	{
		++count;
	}
};


typedef yggr::safe_container::safe_vector<int, boost::mutex, std::allocator<int>, std::vector> hvt_type;

enum
{
	E_read = 0,
	E_write = 100,
	E_max_elem = 20000
};

template<typename Value>
Value create_val(void)
{
	return Value(std::rand() % E_max_elem, std::rand() % E_max_elem, "aaa");
}


template<typename Container>
void read_op(const Container& cont, hvt_type& hvt)
{
	typedef Container container_type;
	typedef typename container_type::base_type container_base_type;
	typedef typename container_type::val_type val_type;

	if(E_max_elem < hvt.size())
	{
		hvt.clear();
	}
	int op = std::rand() % 8;
	switch(op)
	{
	case 0: // size
		hvt.push_back(0 + E_read);
		cont.template use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(container_type, 0).size());
		break;
	case 1: // max_size
		hvt.push_back(1 + E_read);
		cont.template use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(container_type, 0).max_size());
		break;
	case 2: // empty
		hvt.push_back(2 + E_read);
		cont.template use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(container_type, 0).empty());
		break;
	case 3: // key_eq
		hvt.push_back(3 + E_read);
		cont.template use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(container_type, 0).key_eq());
		break;
	case 4: // hash_function
		hvt.push_back(4 + E_read);
		cont.template use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(container_type, 0).hash_function());
		break;
	case 5: // get_allocator
		hvt.push_back(5 + E_read);
		cont.get_allocator();
		break;
	case 6: // is_exisits
		{
			hvt.push_back(6 + E_read);
			cont.is_exists(std::rand() % E_max_elem);
			cont.is_exists(std::rand() % E_max_elem);
		}
		break;
	case 7:
		hvt.push_back(8 + E_read);
		cont.template use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(container_type, 0).count(std::rand() % E_max_elem));
		break;
	default:
		assert(false);
	}
}

template<typename Container>
void write_op(Container& cont, hvt_type& hvt)
{
	typedef Container container_type;
	typedef typename container_type::base_type container_base_type;
	typedef typename container_type::val_type val_type;
	if(E_max_elem < hvt.size())
	{
		hvt.clear();
	}

	int op = std::rand() % 9;
	switch(op)
	{
	case 0: // operator= &&
		{
			hvt.push_back(0 + E_write);
			container_base_type tmp;
			tmp.insert(create_val<val_type>());
			tmp.insert(create_val<val_type>());
			tmp.insert(create_val<val_type>());
			cont = boost::move(tmp);
		}
		break;
	case 1: // operator = const &
		{
			hvt.push_back(1 + E_write);
			container_base_type tmp;
			tmp.insert(create_val<val_type>());
			tmp.insert(create_val<val_type>());
			tmp.insert(create_val<val_type>());
			cont = tmp;
		}
		break;
	case 2: // insert
		hvt.push_back(2 + E_write);
		cont.template use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(container_type, 0).insert(create_val<val_type>()));
		break;
	case 3: // insert(&&)
		hvt.push_back(3 + E_write);
		{
		    val_type val(create_val<val_type>());
            cont.template use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(container_type, 0).insert(boost::move(val)));
		}
		break;
	case 4: // erase
		{
			hvt.push_back(6 + E_write);
			cont.template use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(container_type, 0).erase(std::rand() % E_max_elem));
		}
		break;
	case 5: // swap(base&&)
		{
			hvt.push_back(7 + E_write);
			container_base_type tmp;
			tmp.insert(create_val<val_type>());
			tmp.insert(create_val<val_type>());
			tmp.insert(create_val<val_type>());
			tmp.insert(create_val<val_type>());
			tmp.insert(create_val<val_type>());
			cont.swap(boost::move(tmp));
		}
		break;
	case 6: // swap(base&)
		{
			hvt.push_back(8 + E_write);
			container_base_type tmp;
			tmp.insert(create_val<val_type>());
			tmp.insert(create_val<val_type>());
			tmp.insert(create_val<val_type>());
			tmp.insert(create_val<val_type>());
			tmp.insert(create_val<val_type>());
			cont.swap(tmp);
		}
		break;
	case 7: // swap(this&)
		{
			hvt.push_back(9 + E_write);
			container_type tmp;
			tmp.template use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(container_type, 0).insert(create_val<val_type>()));
			tmp.template use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(container_type, 0).insert(create_val<val_type>()));
			tmp.template use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(container_type, 0).insert(create_val<val_type>()));
			tmp.template use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(container_type, 0).insert(create_val<val_type>()));
			tmp.template use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(container_type, 0).insert(create_val<val_type>()));
			cont.swap(tmp);
		}
		break;
	case 8: //clear
		hvt.push_back(10 + E_write);
		cont.clear();
		break;
	default:
		assert(false);
	}

	if(cont.size() > E_max_elem)
	{
	    container_base_type tmp;
		cont.swap(tmp);
	}
}

template<typename Container>
void thread_foo_of_mutex(Container& cont, int size, hvt_type& hvt)
{
	boost::thread::id tid = boost::this_thread::get_id();
	for(int i = 0; i != size; ++i)
	{
		if(std::rand() % 2)
		{
			write_op(cont, hvt);
		}
		else
		{
			read_op(cont, hvt);
		}

		//std::cout << "thread " << tid << " run " << i << std::endl;
	}
}

void test_thread_safe_of_mutex(void)
{
	typedef A val_type;
	typedef yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::hashed_unique<
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::hashed_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>
			>
		> container_type;

	int loop_size = 1000;
	int trd_size = 100;
	std::cout << "input loop_size:" << std::endl;
	std::cin >> loop_size;

	std::cout << "input thread_size:" << std::endl;
	std::cin >> trd_size;

	container_type cont;
	hvt_type hvt;

	boost::thread_group trds;

	for(int i = 0; i != trd_size; ++i)
	{
		trds.create_thread(boost::bind(thread_foo_of_mutex<container_type>, boost::ref(cont), loop_size, boost::ref(hvt)));
	}
	trds.join_all();

	std::cout << "test_thread_safe_of_mutex end" << std::endl;
}

template<typename Container>
void thread_foo_of_shard_mutex(Container& cont, int size, hvt_type& hvt, bool mode)
{
	boost::thread::id tid = boost::this_thread::get_id();
	for(int i = 0; i != size; ++i)
	{
		if(mode)
		{
			write_op(cont, hvt);
		}
		else
		{
			read_op(cont, hvt);
		}

		//std::cout << "thread " << tid << " run " << i << std::endl;
	}
}

void thread_safe_test_of_shared_mutex(void)
{
	typedef A val_type;
	typedef yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::hashed_unique<
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::hashed_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>
			>,
			boost::shared_mutex

		> container_type;

	int loop_size = 100;
	int trd_size = 100;
	std::cout << "input loop_size:" << std::endl;
	std::cin >> loop_size;

	std::cout << "input thread_size:" << std::endl;
	std::cin >> trd_size;

	container_type cont;
	hvt_type hvt;

	boost::thread_group trds;

	trds.create_thread(boost::bind(&thread_foo_of_shard_mutex<container_type>, boost::ref(cont), loop_size, boost::ref(hvt), true));

	for(int i = 0, isize = trd_size - 1; i != isize; ++i)
	{
		trds.create_thread(boost::bind(thread_foo_of_shard_mutex<container_type>, boost::ref(cont), loop_size, boost::ref(hvt), false));
	}
	trds.join_all();

	std::cout << "thread_safe_test_of_shared_mutex end" << std::endl;
}

int main(int argc, char* argv[])
{
	test_thread_safe_of_mutex();
	thread_safe_test_of_shared_mutex();
	char cc = 0;
	std::cin >> cc;
	return 0;
}
