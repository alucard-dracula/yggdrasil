// wrap_random_access_iterator_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/lua_wrap/wrap_random_access_iterator.hpp>
#include <yggr/container/vector.hpp>

#include <cassert>
#include <typeinfo>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

template<typename T>
void test_construct(void)
{
	typedef T cont_type;
	typedef typename cont_type::value_type value_type;
	typedef typename cont_type::iterator iter_type;
	typedef typename cont_type::const_iterator citer_type;
	typedef yggr::lua_wrap::wrap_random_access_iterator<iter_type> wrap_iter_type;
	typedef yggr::lua_wrap::wrap_random_access_const_iterator<citer_type> wrap_citer_type;

	value_type arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));

	// wrap_iter_type
	{
		for(wrap_iter_type i = cont.begin(), isize = cont.end(); i != isize; ++i)
		{
			std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
		}
	}

#if 0
	//error C2440: 'initializing' : cannot convert from 'citer_type' to 'yggr::lua_wrap::wrap_random_access_iterator<TIter>'
	{
		citer_type beg = cont.begin(), end = cont.end();
		for(wrap_iter_type i = beg, isize = end; i != isize; ++i)
		{
			std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
		}
	}
#endif // 0, 1

	{
		wrap_iter_type beg = cont.begin(), end = cont.end();
		for(wrap_iter_type i = beg, isize = end; i != isize; ++i)
		{
			std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
		}
	}

	// wrap_citer_type
	{
		for(wrap_citer_type i = cont.begin(), isize = cont.end(); i != isize; ++i)
		{
			std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
		}
	}

	{
		iter_type beg = cont.begin(), end = cont.end();
		for(wrap_citer_type i = beg, isize = end; i != isize; ++i)
		{
			std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
		}
	}

	{
		citer_type beg = cont.begin(), end = cont.end();
		for(wrap_citer_type i = beg, isize = end; i != isize; ++i)
		{
			std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
		}
	}

	{
		wrap_iter_type beg = cont.begin(), end = cont.end();
		for(wrap_citer_type i = beg, isize = end; i != isize; ++i)
		{
			std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
		}
	}

	{
		wrap_citer_type beg = cont.begin(), end = cont.end();
		for(wrap_citer_type i = beg, isize = end; i != isize; ++i)
		{
			std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
		}
	}

	std::cout << "------test_construct [" << typeid(wrap_iter_type).name() << "] end------" << std::endl;
}

template<typename T>
void test_set_and_swap(void)
{
	typedef T cont_type;
	typedef typename cont_type::value_type value_type;
	typedef typename cont_type::iterator iter_type;
	typedef typename cont_type::const_iterator citer_type;
	typedef yggr::lua_wrap::wrap_random_access_iterator<iter_type> wrap_iter_type;
	typedef yggr::lua_wrap::wrap_random_access_const_iterator<citer_type> wrap_citer_type;

	value_type arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));

	// wrap_iter_type
	{
		wrap_iter_type iter;
		iter = cont.begin();
		yggr_test_assert(iter == cont.begin());

		wrap_iter_type iter2;
		iter2 = boost::move(iter);
		yggr_test_assert(iter2 == cont.begin());

		wrap_iter_type iter3;
		iter3 = iter2;
		yggr_test_assert(iter3 == cont.begin());
	}

	{
		iter_type iterl = cont.begin(), iterr = cont.end();
		wrap_iter_type witerl = iterl, witerr = iterr;

		yggr_test_assert(witerl != witerr);
		witerl.swap(iterr);
		yggr_test_assert(witerl == witerr);
		yggr::swap(witerl, iterr);

		yggr_test_assert(witerl != witerr);
		witerl.swap(witerr);
		yggr_test_assert(witerl != witerr);
		yggr::swap(witerl, witerr);

		yggr_test_assert(iterl == witerl);
		yggr_test_assert(iterr == witerr);
		yggr::swap(iterr, witerl);
		yggr_test_assert(iterl != witerl);
		yggr_test_assert(iterr != witerr);
		yggr::swap(iterr, witerl);
		yggr_test_assert(iterl == witerl);
		yggr_test_assert(iterr == witerr);
	}

	// wrap_icter_type
	{
		iter_type ibeg = cont.begin();
		citer_type cibeg = ibeg;
		wrap_iter_type wibeg = ibeg;

		wrap_citer_type iter;
		iter = ibeg;
		yggr_test_assert(iter == cont.begin());
		iter = cibeg;
		yggr_test_assert(iter == cont.begin());
		iter = wibeg;
		yggr_test_assert(iter == cont.begin());

		wrap_citer_type iter2;
		iter2 = boost::move(iter);
		yggr_test_assert(iter2 == cont.begin());

		wrap_citer_type iter3;
		iter3 = iter2;
		yggr_test_assert(iter3 == cont.begin());
	}

	{
#	if 0
		// error C2668: 'yggr::lua_wrap::swap_support::swap' : ambiguous call to overloaded function
		iter_type iterl = cont.begin(), iterr = cont.end();
#else
		citer_type iterl = cont.begin(), iterr = cont.end();
#endif // 0, 1
		wrap_citer_type witerl = iterl, witerr = iterr;

		yggr_test_assert(witerl != witerr);
		witerl.swap(iterr);
		yggr_test_assert(witerl == witerr);
		yggr::swap(witerl, iterr);

		yggr_test_assert(witerl != witerr);
		witerl.swap(witerr);
		yggr_test_assert(witerl != witerr);
		yggr::swap(witerl, witerr);

		yggr_test_assert(iterl == witerl);
		yggr_test_assert(iterr == witerr);
		yggr::swap(iterr, witerl);
		yggr_test_assert(iterl != witerl);
		yggr_test_assert(iterr != witerr);
		yggr::swap(iterr, witerl);
		yggr_test_assert(iterl == witerl);
		yggr_test_assert(iterr == witerr);
	}

	std::cout << "------test_construct [" << typeid(wrap_iter_type).name() << "] end------" << std::endl;
}

template<typename T>
void test_operator_val(void)
{
	typedef T cont_type;
	typedef typename cont_type::value_type value_type;
	typedef typename cont_type::iterator iter_type;
	typedef typename cont_type::const_iterator citer_type;
	typedef yggr::lua_wrap::wrap_random_access_iterator<iter_type> wrap_iter_type;
	typedef yggr::lua_wrap::wrap_random_access_const_iterator<citer_type> wrap_citer_type;

	value_type arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));

	// wrap_iter_type
	{
		wrap_iter_type witer_beg = cont.begin(), witer_end = cont.end();

		iter_type iter_beg, iter_end;
		iter_beg = witer_beg;
		iter_end = witer_end;

		yggr_test_assert(witer_beg == iter_beg);
		yggr_test_assert(witer_end == iter_end);
	}

	// wrap_citer_type
	{
		wrap_citer_type witer_beg = cont.begin(), witer_end = cont.end();

		citer_type iter_beg, iter_end;
		iter_beg = witer_beg;
		iter_end = witer_end;

		yggr_test_assert(witer_beg == iter_beg);
		yggr_test_assert(witer_end == iter_end);
	}

	std::cout << "------test_operator_val [" << typeid(wrap_iter_type).name() << "] end------" << std::endl;
}

template<typename T>
void test_operator_op(void)
{
	typedef T cont_type;
	typedef typename cont_type::value_type value_type;
	typedef typename cont_type::iterator iter_type;
	typedef typename cont_type::const_iterator citer_type;
	typedef yggr::lua_wrap::wrap_random_access_iterator<iter_type> wrap_iter_type;
	typedef yggr::lua_wrap::wrap_random_access_const_iterator<citer_type> wrap_citer_type;

	value_type arr[] = {1, 2, 3, 4, 5};

	// at
	{
		cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
		//wrap_iter_type
		{
			wrap_iter_type beg = cont.begin();
			for(std::size_t i = 0, isize = cont.size(); i != isize; ++i)
			{
				std::cout << beg.at(i) << std::endl;
				beg.at(i) *= 10;
				std::cout << "[ " << beg[i] << ", " << beg.at(i) << " ]" << std::endl;
			}
		}

		//wrap_citer_type
		{
			wrap_citer_type beg = cont.begin();
			for(std::size_t i = 0, isize = cont.size(); i != isize; ++i)
			{
#			if 0
				//error C3892: 'beg' : you cannot assign to a variable that is const
				beg.at(i) *= 10;
#			endif // 0, 1
				std::cout << "[ " << beg[i] << ", " << beg.at(i) << " ]" << std::endl;
			}
		}
	}

	// head_increment, tail_increment
	{
		cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
		//wrap_iter_type
		{
			{
				for(wrap_iter_type i = cont.begin(), isize = cont.end(); i != isize; ++i)
				{
					std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
				}
			}

			{
				for(wrap_iter_type i = cont.begin(), isize = cont.end(); i != isize; i++)
				{
					std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
				}
			}

			wrap_iter_type beg = cont.begin();
			wrap_iter_type i = beg++;
			yggr_test_assert(*i == *cont.begin());
			i = ++beg;
			yggr_test_assert(*i == cont[2]);
		}

		// wrap_citer_type
		{
			{
				for(wrap_citer_type i = cont.begin(), isize = cont.end(); i != isize; ++i)
				{
					std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
				}
			}

			{
				for(wrap_citer_type i = cont.begin(), isize = cont.end(); i != isize; i++)
				{
					std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
				}
			}

			wrap_citer_type beg = cont.begin();
			wrap_citer_type i = beg++;
			yggr_test_assert(*i == *cont.begin());
			i = ++beg;
			yggr_test_assert(*i == cont[2]);
		}
	}

	// head_decrement, tail_decrement
	{
		cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));

		//wrap_iter_type
		{
			{
				for(wrap_iter_type i = cont.end() - 1, isize = cont.begin(); i != isize; --i)
				{
					std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
				}
			}

			{
				for(wrap_iter_type i = cont.end() - 1, isize = cont.begin(); i != isize; i--)
				{
					std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
				}
			}

			wrap_iter_type end = cont.end() - 1;
			wrap_iter_type i = end--;
			yggr_test_assert(*i == *cont.rbegin());
			i = --end;
			yggr_test_assert(*i == cont[2]);
		}

		//wrap_citer_type
		{
			{
				for(wrap_citer_type i = cont.end() - 1, isize = cont.begin(); i != isize; --i)
				{
					std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
				}
			}

			{
				for(wrap_citer_type i = cont.end() - 1, isize = cont.begin(); i != isize; i--)
				{
					std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
				}
			}

			wrap_citer_type end = cont.end() - 1;
			wrap_citer_type i = end--;
			yggr_test_assert(*i == *cont.rbegin());
			i = --end;
			yggr_test_assert(*i == cont[2]);
		}
	}

	// plus
	{
		cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));

		// wrap_iter_type
		{
			wrap_iter_type beg = cont.begin();
			yggr_test_assert(cont[1] == *(beg + 1));
			yggr_test_assert(cont[1] == *(beg + yggr::lua_wrap::wrap_base_t<yggr::s64>(1)));
			yggr_test_assert(cont[1] == *(beg + yggr::lua_wrap::wrap_base_t<yggr::u64>(1)));

			yggr_test_assert(cont[1] == *(1 + beg));
			yggr_test_assert(cont[1] == *(yggr::lua_wrap::wrap_base_t<yggr::u64>(1) + beg));
			yggr_test_assert(cont[1] == *(yggr::lua_wrap::wrap_base_t<yggr::s64>(1) + beg ));

			yggr_test_assert(cont[2] == *(1 + beg + 1));
			yggr_test_assert(cont[2] == *(yggr::lua_wrap::wrap_base_t<yggr::u64>(1) + beg + yggr::lua_wrap::wrap_base_t<yggr::s64>(1)));
			yggr_test_assert(cont[2] == *(yggr::lua_wrap::wrap_base_t<yggr::s64>(1) + beg + yggr::lua_wrap::wrap_base_t<yggr::u64>(1)));

			yggr_test_assert(cont[1] == *(beg += 1));
			yggr_test_assert(cont[2] == *(beg += yggr::lua_wrap::wrap_base_t<yggr::s64>(1)));
			yggr_test_assert(cont[3] == *(beg += yggr::lua_wrap::wrap_base_t<yggr::u64>(1)));
		}

		// wrap_citer_type
		{
			wrap_citer_type beg = cont.begin();
			yggr_test_assert(cont[1] == *(beg + 1));
			yggr_test_assert(cont[1] == *(beg + yggr::lua_wrap::wrap_base_t<yggr::s64>(1)));
			yggr_test_assert(cont[1] == *(beg + yggr::lua_wrap::wrap_base_t<yggr::u64>(1)));

			yggr_test_assert(cont[1] == *(1 + beg));
			yggr_test_assert(cont[1] == *(yggr::lua_wrap::wrap_base_t<yggr::u64>(1) + beg));
			yggr_test_assert(cont[1] == *(yggr::lua_wrap::wrap_base_t<yggr::s64>(1) + beg ));

			yggr_test_assert(cont[2] == *(1 + beg + 1));
			yggr_test_assert(cont[2] == *(yggr::lua_wrap::wrap_base_t<yggr::u64>(1) + beg + yggr::lua_wrap::wrap_base_t<yggr::s64>(1)));
			yggr_test_assert(cont[2] == *(yggr::lua_wrap::wrap_base_t<yggr::s64>(1) + beg + yggr::lua_wrap::wrap_base_t<yggr::u64>(1)));

			yggr_test_assert(cont[1] == *(beg += 1));
			yggr_test_assert(cont[2] == *(beg += yggr::lua_wrap::wrap_base_t<yggr::s64>(1)));
			yggr_test_assert(cont[3] == *(beg += yggr::lua_wrap::wrap_base_t<yggr::u64>(1)));
		}
	}

	// minus
	{
		cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));

		{
			wrap_iter_type end = cont.end() - 1;
			yggr_test_assert(cont[3] == *(end - 1));
			yggr_test_assert(cont[3] == *(end - yggr::lua_wrap::wrap_base_t<yggr::s64>(1)));
			yggr_test_assert(cont[3] == *(end - yggr::lua_wrap::wrap_base_t<yggr::u64>(1)));

			yggr_test_assert(cont[3] == *(end -= 1));
			yggr_test_assert(cont[2] == *(end -= yggr::lua_wrap::wrap_base_t<yggr::s64>(1)));
			yggr_test_assert(cont[1] == *(end -= yggr::lua_wrap::wrap_base_t<yggr::u64>(1)));

			yggr_test_assert(-5 == (cont.begin()) - wrap_iter_type(cont.end()));
			yggr_test_assert(5 == (cont.end()) - wrap_iter_type(cont.begin()));

			yggr_test_assert(-5 == wrap_iter_type(cont.begin()) - (cont.end()));
			yggr_test_assert(5 == wrap_iter_type(cont.end()) - (cont.begin()));

			yggr_test_assert(-5 == wrap_iter_type(cont.begin()) - wrap_iter_type(cont.end()));
			yggr_test_assert(5 == wrap_iter_type(cont.end()) - wrap_iter_type(cont.begin()));

			yggr_test_assert(-5 == wrap_citer_type(cont.begin()) - wrap_iter_type(cont.end()));
			yggr_test_assert(5 == wrap_iter_type(cont.end()) - wrap_citer_type(cont.begin()));
		}

		{
			wrap_citer_type end = cont.end() - 1;
			yggr_test_assert(cont[3] == *(end - 1));
			yggr_test_assert(cont[3] == *(end - yggr::lua_wrap::wrap_base_t<yggr::s64>(1)));
			yggr_test_assert(cont[3] == *(end - yggr::lua_wrap::wrap_base_t<yggr::u64>(1)));

			yggr_test_assert(cont[3] == *(end -= 1));
			yggr_test_assert(cont[2] == *(end -= yggr::lua_wrap::wrap_base_t<yggr::s64>(1)));
			yggr_test_assert(cont[1] == *(end -= yggr::lua_wrap::wrap_base_t<yggr::u64>(1)));

			yggr_test_assert(-5 == (cont.begin()) - wrap_iter_type(cont.end()));
			yggr_test_assert(5 == (cont.end()) - wrap_iter_type(cont.begin()));

			yggr_test_assert(-5 == wrap_iter_type(cont.begin()) - (cont.end()));
			yggr_test_assert(5 == wrap_iter_type(cont.end()) - (cont.begin()));

			yggr_test_assert(-5 == wrap_iter_type(cont.begin()) - wrap_iter_type(cont.end()));
			yggr_test_assert(5 == wrap_iter_type(cont.end()) - wrap_iter_type(cont.begin()));

			yggr_test_assert(-5 == wrap_citer_type(cont.begin()) - wrap_iter_type(cont.end()));
			yggr_test_assert(5 == wrap_iter_type(cont.end()) - wrap_citer_type(cont.begin()));
		}
	}

	// compare
	{
		cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));

		// wrap_iter_type
		{
			yggr_test_assert((cont.begin()) == wrap_iter_type(cont.begin()));
			yggr_test_assert(wrap_iter_type(cont.begin()) == (cont.begin()));
			yggr_test_assert(wrap_iter_type(cont.begin()) == wrap_iter_type(cont.begin()));
			yggr_test_assert(wrap_iter_type(cont.begin()) == wrap_citer_type(cont.begin()));

			yggr_test_assert((cont.begin()) != wrap_iter_type(cont.end()));
			yggr_test_assert(wrap_iter_type(cont.begin()) != (cont.end()));
			yggr_test_assert(wrap_iter_type(cont.begin()) != wrap_iter_type(cont.end()));
			yggr_test_assert(wrap_iter_type(cont.begin()) != wrap_citer_type(cont.end()));

			yggr_test_assert((cont.begin()) < wrap_iter_type(cont.end()));
			yggr_test_assert(wrap_iter_type(cont.begin()) < (cont.end()));
			yggr_test_assert(wrap_iter_type(cont.begin()) < wrap_iter_type(cont.end()));
			yggr_test_assert(wrap_iter_type(cont.begin()) < wrap_citer_type(cont.end()));

			yggr_test_assert((cont.begin()) <= wrap_iter_type(cont.end()));
			yggr_test_assert(wrap_iter_type(cont.begin()) <= (cont.end()));
			yggr_test_assert(wrap_iter_type(cont.begin()) <= wrap_iter_type(cont.end()));
			yggr_test_assert(wrap_iter_type(cont.begin()) <= wrap_citer_type(cont.end()));

			yggr_test_assert((cont.begin()) <= wrap_iter_type(cont.begin()));
			yggr_test_assert(wrap_iter_type(cont.begin()) <= (cont.begin()));
			yggr_test_assert(wrap_iter_type(cont.begin()) <= wrap_iter_type(cont.begin()));
			yggr_test_assert(wrap_iter_type(cont.begin()) <= wrap_citer_type(cont.begin()));

			yggr_test_assert((cont.end()) > wrap_iter_type(cont.begin()));
			yggr_test_assert(wrap_iter_type(cont.end()) > (cont.begin()));
			yggr_test_assert(wrap_iter_type(cont.end()) > wrap_iter_type(cont.begin()));
			yggr_test_assert(wrap_iter_type(cont.end()) > wrap_citer_type(cont.begin()));

			yggr_test_assert((cont.end()) >= wrap_iter_type(cont.end()));
			yggr_test_assert(wrap_iter_type(cont.end()) >= (cont.end()));
			yggr_test_assert(wrap_iter_type(cont.end()) >= wrap_iter_type(cont.end()));
			yggr_test_assert(wrap_iter_type(cont.end()) >= wrap_citer_type(cont.end()));

			yggr_test_assert((cont.end()) >= wrap_iter_type(cont.begin()));
			yggr_test_assert(wrap_iter_type(cont.end()) >= (cont.begin()));
			yggr_test_assert(wrap_iter_type(cont.end()) >= wrap_iter_type(cont.begin()));
			yggr_test_assert(wrap_iter_type(cont.end()) >= wrap_citer_type(cont.begin()));
		}

		// wrap_citer_type
		{
			yggr_test_assert((cont.begin()) == wrap_citer_type(cont.begin()));
			yggr_test_assert(wrap_citer_type(cont.begin()) == (cont.begin()));
			yggr_test_assert(wrap_citer_type(cont.begin()) == wrap_citer_type(cont.begin()));
			yggr_test_assert(wrap_citer_type(cont.begin()) == wrap_iter_type(cont.begin()));

			yggr_test_assert((cont.begin()) != wrap_citer_type(cont.end()));
			yggr_test_assert(wrap_citer_type(cont.begin()) != (cont.end()));
			yggr_test_assert(wrap_citer_type(cont.begin()) != wrap_citer_type(cont.end()));
			yggr_test_assert(wrap_citer_type(cont.begin()) != wrap_iter_type(cont.end()));

			yggr_test_assert((cont.begin()) < wrap_citer_type(cont.end()));
			yggr_test_assert(wrap_citer_type(cont.begin()) < (cont.end()));
			yggr_test_assert(wrap_citer_type(cont.begin()) < wrap_citer_type(cont.end()));
			yggr_test_assert(wrap_citer_type(cont.begin()) < wrap_iter_type(cont.end()));

			yggr_test_assert((cont.begin()) <= wrap_citer_type(cont.end()));
			yggr_test_assert(wrap_citer_type(cont.begin()) <= (cont.end()));
			yggr_test_assert(wrap_citer_type(cont.begin()) <= wrap_citer_type(cont.end()));
			yggr_test_assert(wrap_citer_type(cont.begin()) <= wrap_iter_type(cont.end()));

			yggr_test_assert((cont.begin()) <= wrap_citer_type(cont.begin()));
			yggr_test_assert(wrap_citer_type(cont.begin()) <= (cont.begin()));
			yggr_test_assert(wrap_citer_type(cont.begin()) <= wrap_citer_type(cont.begin()));
			yggr_test_assert(wrap_citer_type(cont.begin()) <= wrap_iter_type(cont.begin()));

			yggr_test_assert((cont.end()) > wrap_citer_type(cont.begin()));
			yggr_test_assert(wrap_citer_type(cont.end()) > (cont.begin()));
			yggr_test_assert(wrap_citer_type(cont.end()) > wrap_citer_type(cont.begin()));
			yggr_test_assert(wrap_citer_type(cont.end()) > wrap_iter_type(cont.begin()));

			yggr_test_assert((cont.end()) >= wrap_citer_type(cont.end()));
			yggr_test_assert(wrap_citer_type(cont.end()) >= (cont.end()));
			yggr_test_assert(wrap_citer_type(cont.end()) >= wrap_citer_type(cont.end()));
			yggr_test_assert(wrap_citer_type(cont.end()) >= wrap_iter_type(cont.end()));

			yggr_test_assert((cont.end()) >= wrap_citer_type(cont.begin()));
			yggr_test_assert(wrap_citer_type(cont.end()) >= (cont.begin()));
			yggr_test_assert(wrap_citer_type(cont.end()) >= wrap_citer_type(cont.begin()));
			yggr_test_assert(wrap_citer_type(cont.end()) >= wrap_iter_type(cont.begin()));
		}
	}

	std::cout << "------test_operator_op [" << typeid(wrap_iter_type).name() << "] end------" << std::endl;
}

int main(int argc, char* argv[])
{
	test_construct< yggr::vector<yggr::s64> >();
	test_construct< yggr::vector<yggr::u64> >();

	test_set_and_swap< yggr::vector<yggr::s64> >();
	test_set_and_swap< yggr::vector<yggr::u64> >();

//	test_operator_val< yggr::vector<yggr::s64> >();
//	test_operator_val< yggr::vector<yggr::u64> >();
//
//	test_operator_op< yggr::vector<yggr::s64> >();
//	test_operator_op< yggr::vector<yggr::u64> >();

	wait_any_key(argc, argv);
	return 0;
}
