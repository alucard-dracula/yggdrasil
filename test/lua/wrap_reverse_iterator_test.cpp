// wrap_reverse_iterator_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/container/list.hpp>
#include <yggr/container/vector.hpp>

#include <yggr/lua_wrap/wrap_reverse_iterator.hpp>
#include <yggr/lua_wrap/wrap_random_access_iterator.hpp>
#include <yggr/lua_wrap/wrap_bidirectional_iterator.hpp>

#include <typeinfo>
#include <cassert>

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
	typedef typename cont_type::reverse_iterator riter_type;
	typedef typename cont_type::const_reverse_iterator criter_type;

	typedef yggr::lua_wrap::wrap_random_access_iterator<iter_type> wrap_iter_type;
	typedef yggr::lua_wrap::wrap_random_access_const_iterator<citer_type> wrap_citer_type;
	typedef yggr::lua_wrap::wrap_reverse_iterator<riter_type> wrap_riter_type;
	typedef yggr::lua_wrap::wrap_reverse_iterator<criter_type> wrap_criter_type;

	value_type arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));

	// wrap_riter_type
	{
		for(wrap_riter_type i = cont.rbegin(), isize = cont.rend(); i != isize; ++i)
		{
			std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
		}
	}

	{
		for(wrap_criter_type i = cont.rbegin(), isize = cont.rend(); i != isize; ++i)
		{
			std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
		}
	}

	{
		iter_type beg = cont.begin(), end = cont.end();
		for(wrap_riter_type i(end), isize(beg); i != isize; ++i)
		{
			std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
		}
	}

	{
		citer_type beg = cont.begin(), end = cont.end();
		for(wrap_criter_type i(end), isize(beg); i != isize; ++i)
		{
			std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
		}
	}

#if 0
	{
		iter_type beg = cont.begin(), end = cont.end();
		wrap_riter_type i(wrap_iter_type(end)); // cl.exe optimization this code, so C2105 C2171
		++i;
	}
#endif // 0, 1

	{
		wrap_iter_type beg = cont.begin(), end = cont.end();
		for(wrap_riter_type i(end), isize(beg); i != isize; ++i)
		{
			std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
		}
	}

	{
		wrap_iter_type beg = cont.begin(), end = cont.end();
		for(wrap_criter_type i(end), isize(beg); i != isize; ++i)
		{
			std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
		}
	}

	{
		wrap_citer_type beg = cont.begin(), end = cont.end();
		for(wrap_criter_type i(end), isize(beg); i != isize; ++i)
		{
			std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
		}
	}

	{
		riter_type beg = cont.rbegin(), end = cont.rend();
		for(wrap_riter_type i = beg, isize = end; i != isize; ++i)
		{
			std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
		}
	}

	{
		riter_type beg = cont.rbegin(), end = cont.rend();
		for(wrap_criter_type i = beg, isize = end; i != isize; ++i)
		{
			std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
		}
	}

	{
		criter_type beg = cont.rbegin(), end = cont.rend();
		for(wrap_criter_type i = beg, isize = end; i != isize; ++i)
		{
			std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
		}
	}

	{
		wrap_riter_type beg = cont.rbegin(), end = cont.rend();
		for(wrap_riter_type i = beg, isize = end; i != isize; ++i)
		{
			std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
		}
	}

	{
		wrap_riter_type beg = cont.rbegin(), end = cont.rend();
		for(wrap_criter_type i = beg, isize = end; i != isize; ++i)
		{
			std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
		}
	}

	{
		wrap_criter_type beg = cont.rbegin(), end = cont.rend();
		for(wrap_criter_type i = beg, isize = end; i != isize; ++i)
		{
			std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
		}
	}

	std::cout << "------test_construct [" << typeid(wrap_iter_type).name() << "] end------" << std::endl;
}

template<typename T>
void test_construct_bi(void)
{
	typedef T cont_type;
	typedef typename cont_type::value_type value_type;
	typedef typename cont_type::iterator iter_type;
	typedef typename cont_type::const_iterator citer_type;
	typedef typename cont_type::reverse_iterator riter_type;
	typedef typename cont_type::const_reverse_iterator criter_type;

	typedef yggr::lua_wrap::wrap_bidirectional_iterator<iter_type> wrap_iter_type;
	typedef yggr::lua_wrap::wrap_bidirectional_const_iterator<citer_type> wrap_citer_type;
	typedef yggr::lua_wrap::wrap_reverse_iterator<riter_type> wrap_riter_type;
	typedef yggr::lua_wrap::wrap_reverse_iterator<criter_type> wrap_criter_type;

	value_type arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));

	// wrap_riter_type
	{
		for(wrap_riter_type i = cont.rbegin(), isize = cont.rend(); i != isize; ++i)
		{
			std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
		}
	}

	{
		for(wrap_criter_type i = cont.rbegin(), isize = cont.rend(); i != isize; ++i)
		{
			std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
		}
	}

	{
		iter_type beg = cont.begin(), end = cont.end();
		for(wrap_riter_type i(end), isize(beg); i != isize; ++i)
		{
			std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
		}
	}

	{
		citer_type beg = cont.begin(), end = cont.end();
		for(wrap_criter_type i(end), isize(beg); i != isize; ++i)
		{
			std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
		}
	}

#if 0
	{
		iter_type beg = cont.begin(), end = cont.end();
		wrap_riter_type i(wrap_iter_type(end)); // cl.exe optimization this code, so C2105 C2171
		++i;
	}
#endif // 0, 1

	{
		wrap_iter_type beg = cont.begin(), end = cont.end();
		for(wrap_riter_type i(end), isize(beg); i != isize; ++i)
		{
			std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
		}
	}

	{
		wrap_iter_type beg = cont.begin(), end = cont.end();
		for(wrap_criter_type i(end), isize(beg); i != isize; ++i)
		{
			std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
		}
	}

	{
		wrap_citer_type beg = cont.begin(), end = cont.end();
		for(wrap_criter_type i(end), isize(beg); i != isize; ++i)
		{
			std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
		}
	}

	{
		riter_type beg = cont.rbegin(), end = cont.rend();
		for(wrap_riter_type i = beg, isize = end; i != isize; ++i)
		{
			std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
		}
	}

	{
		riter_type beg = cont.rbegin(), end = cont.rend();
		for(wrap_criter_type i = beg, isize = end; i != isize; ++i)
		{
			std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
		}
	}

	{
		criter_type beg = cont.rbegin(), end = cont.rend();
		for(wrap_criter_type i = beg, isize = end; i != isize; ++i)
		{
			std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
		}
	}

	{
		wrap_riter_type beg = cont.rbegin(), end = cont.rend();
		for(wrap_riter_type i = beg, isize = end; i != isize; ++i)
		{
			std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
		}
	}

	{
		wrap_riter_type beg = cont.rbegin(), end = cont.rend();
		for(wrap_criter_type i = beg, isize = end; i != isize; ++i)
		{
			std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
		}
	}

	{
		wrap_criter_type beg = cont.rbegin(), end = cont.rend();
		for(wrap_criter_type i = beg, isize = end; i != isize; ++i)
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
	typedef typename cont_type::reverse_iterator riter_type;
	typedef typename cont_type::const_reverse_iterator criter_type;

	typedef yggr::lua_wrap::wrap_random_access_iterator<iter_type> wrap_iter_type;
	typedef yggr::lua_wrap::wrap_random_access_const_iterator<citer_type> wrap_citer_type;
	typedef yggr::lua_wrap::wrap_reverse_iterator<riter_type> wrap_riter_type;
	typedef yggr::lua_wrap::wrap_reverse_iterator<criter_type> wrap_criter_type;

	value_type arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	
	// wrap_riter_type
	{
		wrap_riter_type iter;
		iter = cont.rbegin();
		yggr_test_assert(iter == cont.rbegin());

		wrap_riter_type iter2;
		iter2 = iter;
		yggr_test_assert(iter2 == cont.rbegin());
	}

	{
		riter_type iterl = cont.rbegin(), iterr = cont.rend();
		wrap_riter_type witerl = iterl, witerr = iterr;

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

	// wrap_criter_type
	{
		riter_type irbeg = cont.rbegin();
		criter_type cirbeg = irbeg;
		wrap_riter_type wirbeg = irbeg;

		wrap_criter_type riter;
		riter = irbeg;
		yggr_test_assert(riter == cont.rbegin());
		riter = cirbeg;
		yggr_test_assert(riter == cont.rbegin());
		riter = wirbeg;
		yggr_test_assert(riter == cont.rbegin());

		wrap_criter_type riter2;
		riter2 = riter;
		yggr_test_assert(riter2 == cont.rbegin());
	}

	{
		criter_type riterl = cont.rbegin(), riterr = cont.rend();
		wrap_criter_type writerl = riterl, writerr = riterr;

		yggr_test_assert(writerl != writerr);
		writerl.swap(riterr);
		yggr_test_assert(writerl == writerr);
		yggr::swap(writerl, riterr);

		yggr_test_assert(writerl != writerr);
		writerl.swap(writerr);
		yggr_test_assert(writerl != writerr);
		yggr::swap(writerl, writerr);

		yggr_test_assert(riterl == writerl);
		yggr_test_assert(riterr == writerr);
		yggr::swap(riterr, writerl);
		yggr_test_assert(riterl != writerl);
		yggr_test_assert(riterr != writerr);
		yggr::swap(riterr, writerl);
		yggr_test_assert(riterl == writerl);
		yggr_test_assert(riterr == writerr);
	}

	std::cout << "------test_construct [" << typeid(wrap_iter_type).name() << "] end------" << std::endl;
}

template<typename T>
void test_set_and_swap_bi(void)
{
	typedef T cont_type;
	typedef typename cont_type::value_type value_type;
	typedef typename cont_type::iterator iter_type;
	typedef typename cont_type::const_iterator citer_type;
	typedef typename cont_type::reverse_iterator riter_type;
	typedef typename cont_type::const_reverse_iterator criter_type;

	typedef yggr::lua_wrap::wrap_bidirectional_iterator<iter_type> wrap_iter_type;
	typedef yggr::lua_wrap::wrap_bidirectional_const_iterator<citer_type> wrap_citer_type;
	typedef yggr::lua_wrap::wrap_reverse_iterator<riter_type> wrap_riter_type;
	typedef yggr::lua_wrap::wrap_reverse_iterator<criter_type> wrap_criter_type;

	value_type arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
	
	// wrap_riter_type
	{
		wrap_riter_type iter;
		iter = cont.rbegin();
		yggr_test_assert(iter == cont.rbegin());

		wrap_riter_type iter2;
		iter2 = iter;
		yggr_test_assert(iter2 == cont.rbegin());
	}

	{
		riter_type iterl = cont.rbegin(), iterr = cont.rend();
		wrap_riter_type witerl = iterl, witerr = iterr;

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

	// wrap_criter_type
	{
		riter_type irbeg = cont.rbegin();
		criter_type cirbeg = irbeg;
		wrap_riter_type wirbeg = irbeg;

		wrap_criter_type riter;
		riter = irbeg;
		yggr_test_assert(riter == cont.rbegin());
		riter = cirbeg;
		yggr_test_assert(riter == cont.rbegin());
		riter = wirbeg;
		yggr_test_assert(riter == cont.rbegin());

		wrap_criter_type riter2;
		riter2 = riter;
		yggr_test_assert(riter2 == cont.rbegin());
	}

	{
		criter_type riterl = cont.rbegin(), riterr = cont.rend();
		wrap_criter_type writerl = riterl, writerr = riterr;

		yggr_test_assert(writerl != writerr);
		writerl.swap(riterr);
		yggr_test_assert(writerl == writerr);
		yggr::swap(writerl, riterr);

		yggr_test_assert(writerl != writerr);
		writerl.swap(writerr);
		yggr_test_assert(writerl != writerr);
		yggr::swap(writerl, writerr);

		yggr_test_assert(riterl == writerl);
		yggr_test_assert(riterr == writerr);
		yggr::swap(riterr, writerl);
		yggr_test_assert(riterl != writerl);
		yggr_test_assert(riterr != writerr);
		yggr::swap(riterr, writerl);
		yggr_test_assert(riterl == writerl);
		yggr_test_assert(riterr == writerr);
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
	typedef typename cont_type::reverse_iterator riter_type;
	typedef typename cont_type::const_reverse_iterator criter_type;

	typedef yggr::lua_wrap::wrap_random_access_iterator<iter_type> wrap_iter_type;
	typedef yggr::lua_wrap::wrap_random_access_const_iterator<citer_type> wrap_citer_type;
	typedef yggr::lua_wrap::wrap_reverse_iterator<riter_type> wrap_riter_type;
	typedef yggr::lua_wrap::wrap_reverse_iterator<criter_type> wrap_criter_type;

	value_type arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));

	// wrap_riter_type
	{
		wrap_riter_type witer_rbeg = cont.rbegin(), witer_rend = cont.rend();

		riter_type iter_rbeg, iter_rend;
		iter_rbeg = witer_rbeg;
		iter_rend = witer_rend;

		yggr_test_assert(witer_rbeg == iter_rbeg);
		yggr_test_assert(witer_rend == iter_rend);
	}

	// wrap_criter_type
	{
		wrap_criter_type witer_rbeg = cont.rbegin(), witer_rend = cont.rend();

		criter_type iter_rbeg, iter_rend;
		iter_rbeg = witer_rbeg;
		iter_rend = witer_rend;

		yggr_test_assert(witer_rbeg == iter_rbeg);
		yggr_test_assert(witer_rend == iter_rend);
	}

	std::cout << "------test_operator_val [" << typeid(wrap_iter_type).name() << "] end------" << std::endl;
}

template<typename T>
void test_operator_val_bi(void)
{
	typedef T cont_type;
	typedef typename cont_type::value_type value_type;
	typedef typename cont_type::iterator iter_type;
	typedef typename cont_type::const_iterator citer_type;
	typedef typename cont_type::reverse_iterator riter_type;
	typedef typename cont_type::const_reverse_iterator criter_type;

	typedef yggr::lua_wrap::wrap_bidirectional_iterator<iter_type> wrap_iter_type;
	typedef yggr::lua_wrap::wrap_bidirectional_const_iterator<citer_type> wrap_citer_type;
	typedef yggr::lua_wrap::wrap_reverse_iterator<riter_type> wrap_riter_type;
	typedef yggr::lua_wrap::wrap_reverse_iterator<criter_type> wrap_criter_type;

	value_type arr[] = {1, 2, 3, 4, 5};

	cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));

	// wrap_riter_type
	{
		wrap_riter_type witer_rbeg = cont.rbegin(), witer_rend = cont.rend();

		riter_type iter_rbeg, iter_rend;
		iter_rbeg = witer_rbeg;
		iter_rend = witer_rend;

		yggr_test_assert(witer_rbeg == iter_rbeg);
		yggr_test_assert(witer_rend == iter_rend);
	}

	// wrap_criter_type
	{
		wrap_criter_type witer_rbeg = cont.rbegin(), witer_rend = cont.rend();

		criter_type iter_rbeg, iter_rend;
		iter_rbeg = witer_rbeg;
		iter_rend = witer_rend;

		yggr_test_assert(witer_rbeg == iter_rbeg);
		yggr_test_assert(witer_rend == iter_rend);
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
	typedef typename cont_type::reverse_iterator riter_type;
	typedef typename cont_type::const_reverse_iterator criter_type;

	typedef yggr::lua_wrap::wrap_random_access_iterator<iter_type> wrap_iter_type;
	typedef yggr::lua_wrap::wrap_random_access_const_iterator<citer_type> wrap_citer_type;
	typedef yggr::lua_wrap::wrap_reverse_iterator<riter_type> wrap_riter_type;
	typedef yggr::lua_wrap::wrap_reverse_iterator<criter_type> wrap_criter_type;

	value_type arr[] = {1, 2, 3, 4, 5};

	// at
	{
		cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
		//wrap_riter_type
		{
			wrap_riter_type beg = cont.rbegin();
			for(std::size_t i = 0, isize = cont.size(); i != isize; ++i)
			{
				std::cout << beg.at(i) << std::endl;
				beg.at(i) *= 10;
				std::cout << "[ " << beg[i] << ", " << beg.at(i) << " ]" << std::endl;
			}
		}

		//wrap_criter_type
		{
			wrap_criter_type beg = cont.rbegin();
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
		//wrap_riter_type
		{
			{
				for(wrap_riter_type i = cont.rbegin(), isize = cont.rend(); i != isize; ++i)
				{
					std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
				}
			}

			{
				for(wrap_riter_type i = cont.rbegin(), isize = cont.rend(); i != isize; i++)
				{
					std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
				}
			}

			wrap_riter_type rbeg = cont.rbegin();
			wrap_riter_type i = rbeg++;
			yggr_test_assert(*i == *cont.rbegin());
			i = ++rbeg;
			yggr_test_assert(*i == cont[sizeof(arr) / sizeof(arr[0]) - 3 /*-2 -1, rbeg is cont[size - 1]*/]);
		}

		// wrap_criter_type
		{
			{
				for(wrap_criter_type i = cont.rbegin(), isize = cont.rend(); i != isize; ++i)
				{
					std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
				}
			}

			{
				for(wrap_criter_type i = cont.rbegin(), isize = cont.rend(); i != isize; i++)
				{
					std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
				}
			}

			wrap_criter_type rbeg = cont.rbegin();
			wrap_criter_type i = rbeg++;
			yggr_test_assert(*i == *cont.rbegin());
			i = ++rbeg;
			yggr_test_assert(*i == cont[sizeof(arr) / sizeof(arr[0]) - 3]);
		}
	}

	// head_decrement, tail_decrement
	{
		cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));

		//wrap_riter_type
		{
			{
				for(wrap_riter_type i = cont.rend() - 1, isize = cont.rbegin(); i != isize; --i)
				{
					std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
				}
			}

			{
				for(wrap_riter_type i = cont.rend() - 1, isize = cont.rbegin(); i != isize; i--)
				{
					std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
				}
			}

			wrap_riter_type rend = cont.rend() - 1;
			wrap_riter_type i = rend--;
			yggr_test_assert(*i == *cont.begin());
			i = --rend;
			yggr_test_assert(*i == cont[sizeof(arr) / sizeof(arr[0]) - 3]);
		}

		//wrap_criter_type
		{
			{
				for(wrap_criter_type i = cont.rend() - 1, isize = cont.rbegin(); i != isize; --i)
				{
					std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
				}
			}

			{
				for(wrap_criter_type i = cont.rend() - 1, isize = cont.rbegin(); i != isize; i--)
				{
					std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
				}
			}

			wrap_criter_type rend = cont.rend() - 1;
			wrap_criter_type i = rend--;
			yggr_test_assert(*i == *cont.begin());
			i = --rend;
			yggr_test_assert(*i == cont[sizeof(arr) / sizeof(arr[0]) - 3]);
		}
	}

	// plus
	{
		cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));

		// wrap_riter_type
		{
			wrap_riter_type rbeg = cont.rbegin();
			yggr_test_assert(cont[sizeof(arr) / sizeof(arr[0]) - 2] == *(rbeg + 1));
			yggr_test_assert(cont[sizeof(arr) / sizeof(arr[0]) - 2] == *(rbeg + yggr::lua_wrap::wrap_base_t<yggr::s64>(1)));
			yggr_test_assert(cont[sizeof(arr) / sizeof(arr[0]) - 2] == *(rbeg + yggr::lua_wrap::wrap_base_t<yggr::u64>(1)));

			yggr_test_assert(cont[sizeof(arr) / sizeof(arr[0]) - 2] == *(1 + rbeg));
			yggr_test_assert(cont[sizeof(arr) / sizeof(arr[0]) - 2] == *(yggr::lua_wrap::wrap_base_t<yggr::u64>(1) + rbeg));
			yggr_test_assert(cont[sizeof(arr) / sizeof(arr[0]) - 2] == *(yggr::lua_wrap::wrap_base_t<yggr::s64>(1) + rbeg));

			yggr_test_assert(cont[sizeof(arr) / sizeof(arr[0]) - 3] == *(1 + rbeg + 1));
			yggr_test_assert(cont[sizeof(arr) / sizeof(arr[0]) - 3] == *(yggr::lua_wrap::wrap_base_t<yggr::u64>(1) + rbeg + yggr::lua_wrap::wrap_base_t<yggr::s64>(1)));
			yggr_test_assert(cont[sizeof(arr) / sizeof(arr[0]) - 3] == *(yggr::lua_wrap::wrap_base_t<yggr::s64>(1) + rbeg + yggr::lua_wrap::wrap_base_t<yggr::u64>(1)));

			yggr_test_assert(cont[sizeof(arr) / sizeof(arr[0]) - 2] == *(rbeg += 1));
			yggr_test_assert(cont[sizeof(arr) / sizeof(arr[0]) - 3] == *(rbeg += yggr::lua_wrap::wrap_base_t<yggr::s64>(1)));
			yggr_test_assert(cont[sizeof(arr) / sizeof(arr[0]) - 4] == *(rbeg += yggr::lua_wrap::wrap_base_t<yggr::u64>(1)));
		}

		// wrap_criter_type
		{
			wrap_criter_type rbeg = cont.rbegin();
			yggr_test_assert(cont[sizeof(arr) / sizeof(arr[0]) - 2] == *(rbeg + 1));
			yggr_test_assert(cont[sizeof(arr) / sizeof(arr[0]) - 2] == *(rbeg + yggr::lua_wrap::wrap_base_t<yggr::s64>(1)));
			yggr_test_assert(cont[sizeof(arr) / sizeof(arr[0]) - 2] == *(rbeg + yggr::lua_wrap::wrap_base_t<yggr::u64>(1)));

			yggr_test_assert(cont[sizeof(arr) / sizeof(arr[0]) - 2] == *(1 + rbeg));
			yggr_test_assert(cont[sizeof(arr) / sizeof(arr[0]) - 2] == *(yggr::lua_wrap::wrap_base_t<yggr::u64>(1) + rbeg));
			yggr_test_assert(cont[sizeof(arr) / sizeof(arr[0]) - 2] == *(yggr::lua_wrap::wrap_base_t<yggr::s64>(1) + rbeg));

			yggr_test_assert(cont[sizeof(arr) / sizeof(arr[0]) - 3] == *(1 + rbeg + 1));
			yggr_test_assert(cont[sizeof(arr) / sizeof(arr[0]) - 3] == *(yggr::lua_wrap::wrap_base_t<yggr::u64>(1) + rbeg + yggr::lua_wrap::wrap_base_t<yggr::s64>(1)));
			yggr_test_assert(cont[sizeof(arr) / sizeof(arr[0]) - 3] == *(yggr::lua_wrap::wrap_base_t<yggr::s64>(1) + rbeg + yggr::lua_wrap::wrap_base_t<yggr::u64>(1)));

			yggr_test_assert(cont[sizeof(arr) / sizeof(arr[0]) - 2] == *(rbeg += 1));
			yggr_test_assert(cont[sizeof(arr) / sizeof(arr[0]) - 3] == *(rbeg += yggr::lua_wrap::wrap_base_t<yggr::s64>(1)));
			yggr_test_assert(cont[sizeof(arr) / sizeof(arr[0]) - 4] == *(rbeg += yggr::lua_wrap::wrap_base_t<yggr::u64>(1)));
		}
	}

	// minus
	{
		cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));

		// wrap_riter_type
		{
			wrap_riter_type rend = cont.rend() - 1;
			yggr_test_assert(cont[1] == *(rend - 1));
			yggr_test_assert(cont[1] == *(rend - yggr::lua_wrap::wrap_base_t<yggr::s64>(1)));
			yggr_test_assert(cont[1] == *(rend - yggr::lua_wrap::wrap_base_t<yggr::u64>(1)));

			yggr_test_assert(cont[1] == *(rend -= 1));
			yggr_test_assert(cont[2] == *(rend -= yggr::lua_wrap::wrap_base_t<yggr::s64>(1)));
			yggr_test_assert(cont[3] == *(rend -= yggr::lua_wrap::wrap_base_t<yggr::u64>(1)));

			yggr_test_assert(-5 == (cont.rbegin()) - wrap_riter_type(cont.rend()));
			yggr_test_assert(5 == (cont.rend()) - wrap_riter_type(cont.rbegin()));

			yggr_test_assert(-5 == wrap_riter_type(cont.rbegin()) - (cont.rend()));
			yggr_test_assert(5 == wrap_riter_type(cont.rend()) - (cont.rbegin()));

			yggr_test_assert(-5 == wrap_riter_type(cont.rbegin()) - wrap_riter_type(cont.rend()));
			yggr_test_assert(5 == wrap_riter_type(cont.rend()) - wrap_riter_type(cont.rbegin()));

			yggr_test_assert(-5 == wrap_criter_type(cont.rbegin()) - wrap_riter_type(cont.rend()));
			yggr_test_assert(5 == wrap_riter_type(cont.rend()) - wrap_criter_type(cont.rbegin()));
		}

		// wrap_criter_type
		{
			wrap_criter_type rend = cont.rend() - 1;
			yggr_test_assert(cont[1] == *(rend - 1));
			yggr_test_assert(cont[1] == *(rend - yggr::lua_wrap::wrap_base_t<yggr::s64>(1)));
			yggr_test_assert(cont[1] == *(rend - yggr::lua_wrap::wrap_base_t<yggr::u64>(1)));

			yggr_test_assert(cont[1] == *(rend -= 1));
			yggr_test_assert(cont[2] == *(rend -= yggr::lua_wrap::wrap_base_t<yggr::s64>(1)));
			yggr_test_assert(cont[3] == *(rend -= yggr::lua_wrap::wrap_base_t<yggr::u64>(1)));

			yggr_test_assert(-5 == (cont.rbegin()) - wrap_criter_type(cont.rend()));
			yggr_test_assert(5 == (cont.rend()) - wrap_criter_type(cont.rbegin()));

			yggr_test_assert(-5 == wrap_criter_type(cont.rbegin()) - (cont.rend()));
			yggr_test_assert(5 == wrap_criter_type(cont.rend()) - (cont.rbegin()));

			yggr_test_assert(-5 == wrap_criter_type(cont.rbegin()) - wrap_criter_type(cont.rend()));
			yggr_test_assert(5 == wrap_criter_type(cont.rend()) - wrap_criter_type(cont.rbegin()));

			yggr_test_assert(-5 == wrap_criter_type(cont.rbegin()) - wrap_riter_type(cont.rend()));
			yggr_test_assert(5 == wrap_riter_type(cont.rend()) - wrap_criter_type(cont.rbegin()));
		}
	}

	// compare
	{
		cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));

		// wrap_riter_type
		{
			yggr_test_assert((cont.rbegin()) == wrap_riter_type(cont.rbegin()));
			yggr_test_assert(wrap_riter_type(cont.rbegin()) == (cont.rbegin()));
			yggr_test_assert(wrap_riter_type(cont.rbegin()) == wrap_riter_type(cont.rbegin()));
			yggr_test_assert(wrap_riter_type(cont.rbegin()) == wrap_criter_type(cont.rbegin()));

			yggr_test_assert((cont.rbegin()) != wrap_riter_type(cont.rend()));
			yggr_test_assert(wrap_riter_type(cont.rbegin()) != (cont.rend()));
			yggr_test_assert(wrap_riter_type(cont.rbegin()) != wrap_riter_type(cont.rend()));
			yggr_test_assert(wrap_riter_type(cont.rbegin()) != wrap_criter_type(cont.rend()));

			yggr_test_assert((cont.rbegin()) < wrap_riter_type(cont.rend()));
			yggr_test_assert(wrap_riter_type(cont.rbegin()) < (cont.rend()));
			yggr_test_assert(wrap_riter_type(cont.rbegin()) < wrap_riter_type(cont.rend()));
			yggr_test_assert(wrap_riter_type(cont.rbegin()) < wrap_criter_type(cont.rend()));

			yggr_test_assert((cont.rbegin()) <= wrap_riter_type(cont.rend()));
			yggr_test_assert(wrap_riter_type(cont.rbegin()) <= (cont.rend()));
			yggr_test_assert(wrap_riter_type(cont.rbegin()) <= wrap_riter_type(cont.rend()));
			yggr_test_assert(wrap_riter_type(cont.rbegin()) <= wrap_criter_type(cont.rend()));

			yggr_test_assert((cont.rbegin()) <= wrap_riter_type(cont.rbegin()));
			yggr_test_assert(wrap_riter_type(cont.rbegin()) <= (cont.rbegin()));
			yggr_test_assert(wrap_riter_type(cont.rbegin()) <= wrap_riter_type(cont.rbegin()));
			yggr_test_assert(wrap_riter_type(cont.rbegin()) <= wrap_criter_type(cont.rbegin()));

			yggr_test_assert((cont.rend()) > wrap_riter_type(cont.rbegin()));
			yggr_test_assert(wrap_riter_type(cont.rend()) > (cont.rbegin()));
			yggr_test_assert(wrap_riter_type(cont.rend()) > wrap_riter_type(cont.rbegin()));
			yggr_test_assert(wrap_riter_type(cont.rend()) > wrap_criter_type(cont.rbegin()));

			yggr_test_assert((cont.rend()) >= wrap_riter_type(cont.rend()));
			yggr_test_assert(wrap_riter_type(cont.rend()) >= (cont.rend()));
			yggr_test_assert(wrap_riter_type(cont.rend()) >= wrap_riter_type(cont.rend()));
			yggr_test_assert(wrap_riter_type(cont.rend()) >= wrap_criter_type(cont.rend()));

			yggr_test_assert((cont.rend()) >= wrap_riter_type(cont.rbegin()));
			yggr_test_assert(wrap_riter_type(cont.rend()) >= (cont.rbegin()));
			yggr_test_assert(wrap_riter_type(cont.rend()) >= wrap_riter_type(cont.rbegin()));
			yggr_test_assert(wrap_riter_type(cont.rend()) >= wrap_criter_type(cont.rbegin()));
		}

		// wrap_criter_type
		{
			yggr_test_assert((cont.rbegin()) == wrap_criter_type(cont.rbegin()));
			yggr_test_assert(wrap_criter_type(cont.rbegin()) == (cont.rbegin()));
			yggr_test_assert(wrap_criter_type(cont.rbegin()) == wrap_criter_type(cont.rbegin()));
			yggr_test_assert(wrap_criter_type(cont.rbegin()) == wrap_riter_type(cont.rbegin()));

			yggr_test_assert((cont.rbegin()) != wrap_criter_type(cont.rend()));
			yggr_test_assert(wrap_criter_type(cont.rbegin()) != (cont.rend()));
			yggr_test_assert(wrap_criter_type(cont.rbegin()) != wrap_criter_type(cont.rend()));
			yggr_test_assert(wrap_criter_type(cont.rbegin()) != wrap_riter_type(cont.rend()));

			yggr_test_assert((cont.rbegin()) < wrap_criter_type(cont.rend()));
			yggr_test_assert(wrap_criter_type(cont.rbegin()) < (cont.rend()));
			yggr_test_assert(wrap_criter_type(cont.rbegin()) < wrap_criter_type(cont.rend()));
			yggr_test_assert(wrap_criter_type(cont.rbegin()) < wrap_riter_type(cont.rend()));

			yggr_test_assert((cont.rbegin()) <= wrap_criter_type(cont.rend()));
			yggr_test_assert(wrap_criter_type(cont.rbegin()) <= (cont.rend()));
			yggr_test_assert(wrap_criter_type(cont.rbegin()) <= wrap_criter_type(cont.rend()));
			yggr_test_assert(wrap_criter_type(cont.rbegin()) <= wrap_riter_type(cont.rend()));

			yggr_test_assert((cont.rbegin()) <= wrap_criter_type(cont.rbegin()));
			yggr_test_assert(wrap_criter_type(cont.rbegin()) <= (cont.rbegin()));
			yggr_test_assert(wrap_criter_type(cont.rbegin()) <= wrap_criter_type(cont.rbegin()));
			yggr_test_assert(wrap_criter_type(cont.rbegin()) <= wrap_riter_type(cont.rbegin()));

			yggr_test_assert((cont.rend()) > wrap_criter_type(cont.rbegin()));
			yggr_test_assert(wrap_criter_type(cont.rend()) > (cont.rbegin()));
			yggr_test_assert(wrap_criter_type(cont.rend()) > wrap_criter_type(cont.rbegin()));
			yggr_test_assert(wrap_criter_type(cont.rend()) > wrap_riter_type(cont.rbegin()));

			yggr_test_assert((cont.rend()) >= wrap_criter_type(cont.rend()));
			yggr_test_assert(wrap_criter_type(cont.rend()) >= (cont.rend()));
			yggr_test_assert(wrap_criter_type(cont.rend()) >= wrap_criter_type(cont.rend()));
			yggr_test_assert(wrap_criter_type(cont.rend()) >= wrap_riter_type(cont.rend()));

			yggr_test_assert((cont.rend()) >= wrap_criter_type(cont.rbegin()));
			yggr_test_assert(wrap_criter_type(cont.rend()) >= (cont.rbegin()));
			yggr_test_assert(wrap_criter_type(cont.rend()) >= wrap_criter_type(cont.rbegin()));
			yggr_test_assert(wrap_criter_type(cont.rend()) >= wrap_riter_type(cont.rbegin()));
		}
	}

	std::cout << "------test_operator_op [" << typeid(wrap_iter_type).name() << "] end------" << std::endl;
}

template<typename T>
void test_operator_op_bi(void)
{
	typedef T cont_type;
	typedef typename cont_type::value_type value_type;
	typedef typename cont_type::iterator iter_type;
	typedef typename cont_type::const_iterator citer_type;
	typedef typename cont_type::reverse_iterator riter_type;
	typedef typename cont_type::const_reverse_iterator criter_type;

	typedef yggr::lua_wrap::wrap_bidirectional_iterator<iter_type> wrap_iter_type;
	typedef yggr::lua_wrap::wrap_bidirectional_iterator<citer_type> wrap_citer_type;
	typedef yggr::lua_wrap::wrap_reverse_iterator<riter_type> wrap_riter_type;
	typedef yggr::lua_wrap::wrap_reverse_iterator<criter_type> wrap_criter_type;

	value_type arr[] = {1, 2, 3, 4, 5};

	// head_increment, tail_increment
	{
		cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));
		//wrap_riter_type
		{
			{
				for(wrap_riter_type i = cont.rbegin(), isize = cont.rend(); i != isize; ++i)
				{
					std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
				}
			}

			{
				for(wrap_riter_type i = cont.rbegin(), isize = cont.rend(); i != isize; i++)
				{
					std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
				}
			}

			wrap_riter_type rbeg = cont.rbegin();
			wrap_riter_type i = rbeg++;
			yggr_test_assert(*i == *cont.rbegin());
			i = ++rbeg;
			yggr_test_assert(*i == arr[sizeof(arr) / sizeof(arr[0]) - 3 /*-2 -1, rbeg is cont[size - 1]*/]);
		}

		// wrap_criter_type
		{
			{
				for(wrap_criter_type i = cont.rbegin(), isize = cont.rend(); i != isize; ++i)
				{
					std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
				}
			}

			{
				for(wrap_criter_type i = cont.rbegin(), isize = cont.rend(); i != isize; i++)
				{
					std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
				}
			}

			wrap_criter_type rbeg = cont.rbegin();
			wrap_criter_type i = rbeg++;
			yggr_test_assert(*i == *cont.rbegin());
			i = ++rbeg;
			yggr_test_assert(*i == arr[sizeof(arr) / sizeof(arr[0]) - 3]);
		}
	}

	// head_decrement, tail_decrement
	{
		cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));

		//wrap_riter_type
		{
			{
				for(wrap_riter_type i = yggr::advance_copy(cont.rend(), -1), isize = cont.rbegin(); i != isize; --i)
				{
					std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
				}
			}

			{
				for(wrap_riter_type i = yggr::advance_copy(cont.rend(), -1), isize = cont.rbegin(); i != isize; i--)
				{
					std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
				}
			}

			wrap_riter_type rend = yggr::advance_copy(cont.rend(), -1);
			wrap_riter_type i = rend--;
			yggr_test_assert(*i == *cont.begin());
			i = --rend;
			yggr_test_assert(*i == arr[sizeof(arr) / sizeof(arr[0]) - 3]);
		}

		//wrap_criter_type
		{
			{
				for(wrap_criter_type i = yggr::advance_copy(cont.rend(), -1), isize = cont.rbegin(); i != isize; --i)
				{
					std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
				}
			}

			{
				for(wrap_criter_type i = yggr::advance_copy(cont.rend(), -1), isize = cont.rbegin(); i != isize; i--)
				{
					std::cout << "[ " << i << ", " << *i << " ]" << std::endl;
				}
			}

			wrap_criter_type rend = yggr::advance_copy(cont.rend(), -1);
			wrap_criter_type i = rend--;
			yggr_test_assert(*i == *cont.begin());
			i = --rend;
			yggr_test_assert(*i == arr[sizeof(arr) / sizeof(arr[0]) - 3]);
		}
	}

	// compare
	{
		cont_type cont(arr + 0, arr + sizeof(arr) / sizeof(arr[0]));

		// wrap_riter_type
		{
			yggr_test_assert((cont.rbegin()) == wrap_riter_type(cont.rbegin()));
			yggr_test_assert(wrap_riter_type(cont.rbegin()) == (cont.rbegin()));
			yggr_test_assert(wrap_riter_type(cont.rbegin()) == wrap_riter_type(cont.rbegin()));
			yggr_test_assert(wrap_riter_type(cont.rbegin()) == wrap_criter_type(cont.rbegin()));

			yggr_test_assert((cont.rbegin()) != wrap_riter_type(cont.rend()));
			yggr_test_assert(wrap_riter_type(cont.rbegin()) != (cont.rend()));
			yggr_test_assert(wrap_riter_type(cont.rbegin()) != wrap_riter_type(cont.rend()));
			yggr_test_assert(wrap_riter_type(cont.rbegin()) != wrap_criter_type(cont.rend()));
		}

		// wrap_criter_type
		{
			yggr_test_assert((cont.rbegin()) == wrap_criter_type(cont.rbegin()));
			yggr_test_assert(wrap_criter_type(cont.rbegin()) == (cont.rbegin()));
			yggr_test_assert(wrap_criter_type(cont.rbegin()) == wrap_criter_type(cont.rbegin()));
			yggr_test_assert(wrap_criter_type(cont.rbegin()) == wrap_riter_type(cont.rbegin()));

			yggr_test_assert((cont.rbegin()) != wrap_criter_type(cont.rend()));
			yggr_test_assert(wrap_criter_type(cont.rbegin()) != (cont.rend()));
			yggr_test_assert(wrap_criter_type(cont.rbegin()) != wrap_criter_type(cont.rend()));
			yggr_test_assert(wrap_criter_type(cont.rbegin()) != wrap_riter_type(cont.rend()));
		}
	}

	std::cout << "------test_operator_op [" << typeid(wrap_iter_type).name() << "] end------" << std::endl;
}

int main(int argc, char* argv[])
{
	// randam_accesser
	{
		test_construct< yggr::vector<yggr::s64> >();
		test_construct< yggr::vector<yggr::u64> >();

		test_set_and_swap< yggr::vector<yggr::s64> >();
		test_set_and_swap< yggr::vector<yggr::u64> >();

		test_operator_val< yggr::vector<yggr::s64> >();
		test_operator_val< yggr::vector<yggr::u64> >();

		test_operator_op< yggr::vector<yggr::s64> >();
		test_operator_op< yggr::vector<yggr::u64> >();
	}

	{
		test_construct_bi< yggr::list<yggr::s64> >();
		test_construct_bi< yggr::list<yggr::u64> >();

		test_set_and_swap_bi< yggr::list<yggr::s64> >();
		test_set_and_swap_bi< yggr::list<yggr::u64> >();

		test_operator_val_bi< yggr::list<yggr::s64> >();
		test_operator_val_bi< yggr::list<yggr::u64> >();

		test_operator_op_bi< yggr::list<yggr::s64> >();
		test_operator_op_bi< yggr::list<yggr::u64> >();
	}

	wait_any_key(argc, argv);
	return 0;
}