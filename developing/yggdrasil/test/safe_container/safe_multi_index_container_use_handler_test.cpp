//safe_multi_index_container_use_handler_test.cpp

#if defined(_MSC_VER) && defined(_DEBUG)
#	include <vld.h>
#endif // _MSC_VER && _DEBUG
#include <iostream>
#include <string>
#include <yggr/safe_container/safe_multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/bind.hpp>

#include <assert.h>

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



#ifdef _DEBUG

//--------------------hash------------------------------------------

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


void test_use_handler_of_hash(void)
{
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
		> mi_cont_type;



	A arr[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 4, "ddd")};
	mi_cont_type::base_type base_cont(arr, arr + 10);
	assert(base_cont.size() == 5);
	mi_cont_type cont;
	mi_cont_type& ref_cont = cont;
	const mi_cont_type& cref_cont = cont;

	ref_cont.use_handler(boost::bind(&foos_of_hash<mi_cont_type>::handler_insert, _1, boost::cref(base_cont)));
	cref_cont.use_handler(boost::bind(&foos_of_hash<mi_cont_type>::handler_check, _1, boost::cref(base_cont)));

	int n = 0;
	foos_of_hash<mi_cont_type>::handler_insert_of_ret_helper(cont, base_cont,
																boost::bind(&foos_of_hash<mi_cont_type>::handler_of_ret, boost::ref(n)));
	assert(n == 1);

	n = 0;

	foos_of_hash<mi_cont_type>::handler_check_of_ret_helper(cont, base_cont,
																boost::bind(&foos_of_hash<mi_cont_type>::handler_of_ret, boost::ref(n)));
	assert(n == 1);


	std::cout << "-----------------test_use_handler_of_hash end---------------" << std::endl;
}

//------------------------------ordered--------------------------------

template<typename Container>
struct foos_of_ordered
{
	typedef Container mi_cont_type;
	typedef foos_of_ordered this_type;

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


void test_use_handler_of_ordered(void)
{
	typedef yggr::safe_container::safe_multi_index_container<
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



	A arr[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 4, "ddd")};
	mi_cont_type::base_type base_cont(arr, arr + 10);
	assert(base_cont.size() == 5);
	mi_cont_type cont;
	mi_cont_type& ref_cont = cont;
	const mi_cont_type& cref_cont = cont;

	ref_cont.use_handler(boost::bind(&foos_of_ordered<mi_cont_type>::handler_insert, _1, boost::cref(base_cont)));
	cref_cont.use_handler(boost::bind(&foos_of_ordered<mi_cont_type>::handler_check, _1, boost::cref(base_cont)));

	int n = 0;
	foos_of_ordered<mi_cont_type>::handler_insert_of_ret_helper(cont, base_cont,
																boost::bind(&foos_of_ordered<mi_cont_type>::handler_of_ret, boost::ref(n)));
	assert(n == 1);

	n = 0;

	foos_of_ordered<mi_cont_type>::handler_check_of_ret_helper(cont, base_cont,
																boost::bind(&foos_of_ordered<mi_cont_type>::handler_of_ret, boost::ref(n)));
	assert(n == 1);


	std::cout << "-----------------test_use_handler_of_ordered end---------------" << std::endl;
}

//---------------------sequenced------------------------------------

template<typename Container>
struct foos_of_sequenced
{
	typedef Container mi_cont_type;
	typedef foos_of_sequenced this_type;

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
			lref.push_back(*i);
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
			assert(std::find(rcref.begin(), rcref.end(), i->prm1) != rcref.end());
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


void test_use_handler_of_sequenced(void)
{
	typedef yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::sequenced< boost::multi_index::tag<A::tag_prm1> >,
				boost::multi_index::sequenced< boost::multi_index::tag<A::tag_prm2> >
			>
		> mi_cont_type;

	A arr[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 4, "ddd")};
	mi_cont_type::base_type base_cont(arr, arr + 10);
	assert(base_cont.size() == 10);
	mi_cont_type cont;
	mi_cont_type& ref_cont = cont;
	const mi_cont_type& cref_cont = cont;

	ref_cont.use_handler(boost::bind(&foos_of_sequenced<mi_cont_type>::handler_insert, _1, boost::cref(base_cont)));
	cref_cont.use_handler(boost::bind(&foos_of_sequenced<mi_cont_type>::handler_check, _1, boost::cref(base_cont)));

	int n = 0;
	foos_of_sequenced<mi_cont_type>::handler_insert_of_ret_helper(cont, base_cont,
																boost::bind(&foos_of_sequenced<mi_cont_type>::handler_of_ret, boost::ref(n)));
	assert(n == 1);

	n = 0;

	foos_of_sequenced<mi_cont_type>::handler_check_of_ret_helper(cont, base_cont,
																boost::bind(&foos_of_sequenced<mi_cont_type>::handler_of_ret, boost::ref(n)));
	assert(n == 1);


	std::cout << "-----------------test_use_handler_of_sequenced end---------------" << std::endl;
}

// -------------------------------random access----------------------------------

template<typename Container>
struct foos_of_random_access
{
	typedef Container mi_cont_type;
	typedef foos_of_random_access this_type;

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
			lref.push_back(*i);
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
			assert(std::find(rcref.begin(), rcref.end(), i->prm1) != rcref.end());
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


void test_use_handler_of_random_access(void)
{
	typedef yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::random_access< boost::multi_index::tag<A::tag_prm1> >,
				boost::multi_index::random_access< boost::multi_index::tag<A::tag_prm2> >
			>
		> mi_cont_type;

	A arr[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 4, "ddd")};
	mi_cont_type::base_type base_cont(arr, arr + 10);
	assert(base_cont.size() == 10);
	mi_cont_type cont;
	mi_cont_type& ref_cont = cont;
	const mi_cont_type& cref_cont = cont;

	ref_cont.use_handler(boost::bind(&foos_of_random_access<mi_cont_type>::handler_insert, _1, boost::cref(base_cont)));
	cref_cont.use_handler(boost::bind(&foos_of_random_access<mi_cont_type>::handler_check, _1, boost::cref(base_cont)));

	int n = 0;
	foos_of_random_access<mi_cont_type>::handler_insert_of_ret_helper(cont, base_cont,
																boost::bind(&foos_of_random_access<mi_cont_type>::handler_of_ret, boost::ref(n)));
	assert(n == 1);

	n = 0;

	foos_of_random_access<mi_cont_type>::handler_check_of_ret_helper(cont, base_cont,
																boost::bind(&foos_of_random_access<mi_cont_type>::handler_of_ret, boost::ref(n)));
	assert(n == 1);


	std::cout << "-----------------test_use_handler_of_sequenced end---------------" << std::endl;
}


#endif // _DEBUG

void debug_test(void)
{
#ifdef _DEBUG
	test_use_handler_of_hash();
	test_use_handler_of_ordered();
	test_use_handler_of_sequenced();
	test_use_handler_of_random_access();

#endif // _DEBUG
}

int main(int argc, char* argv[])
{
	debug_test();
	char cc = 0;
	std::cin >> cc;
	return 0;
}
