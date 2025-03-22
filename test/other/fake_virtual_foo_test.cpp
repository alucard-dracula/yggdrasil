//fake_virtual_foo_test.cpp

#include <iostream>
#include <typeinfo>

#include <boost/function.hpp>

#include <yggr/time/time.hpp>
#include <yggr/time/runtime_performance_tester.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#include YGGR_PP_LINK_LIB(base)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)


#define TEST_PROFORMACE

// real virtual
class base_real
{
public:
	virtual ~base_real(void)
	{
	}

	virtual void vfoo(int n) = 0;
};

template<typename T>
class child_real
	: public base_real
{
public:
	child_real(void)
		: t(new int())
	{
	}

	virtual ~child_real(void)
	{
		if(t)
		{
			delete t;
		}
	}

	virtual void vfoo(int n)
	{
		if(t)
		{
			*t = n;
		}
	}

private:
	T* t;
};

// fake_virtual
class base
{
protected:
	//typedef void (*vfoo_type)(base&, int n);
	typedef boost::function2<void, base&, int> vfoo_type;

public:
	base(vfoo_type vfoo)
		: _pvfoo(vfoo)
	{
	}

	virtual ~base(void)
	{
	}

	inline void fake_vfoo(int n)
	{
		//if(_pvfoo)
		//{
		//	(*_pvfoo)(*this, n);
		//}
		_pvfoo(*this, n);
	}

private:
	vfoo_type _pvfoo;
};

template<typename T>
class child
	: public base
{
public:
	typedef base base_type;
private:
	typedef child this_type;

protected:
	struct fake_vfoo_wrap_t
	{
		typedef void result_type;
		inline void operator()(base_type& base, int n) const
		{
			this_type& now_this = reinterpret_cast<this_type&>(base);
			now_this.fake_vfoo(n);
		}
	};

public:
	child(void)
		: //base_type(&this_type::fake_vfoo_wrap),
			base_type(vfoo_type(fake_vfoo_wrap_t())),
			t(new T())
	{
	}

	virtual ~child(void)
	{
		if(t)
		{
			delete t;
		}
	}

	void fake_vfoo(int n)
	{
#ifndef TEST_PROFORMACE
		std::cout << "fake child [" << typeid(T).name() << "] vfoo called" << std::endl;
#endif // TEST_PROFORMACE
		if(t)
		{
			*t = n;
		}
	}

protected:
	inline static void fake_vfoo_wrap(base_type& base, int n)
	{
		this_type& now_this = static_cast<this_type&>(base);
		now_this.fake_vfoo(n);
	}

private:
	T* t;
};

void test_ref(void)
{
	typedef child<int> ci_type;
	typedef child<float> cf_type;

	ci_type ci;
	cf_type cf;

	base* arr[] = {&ci, &cf};

	arr[0]->fake_vfoo(7);
	arr[1]->fake_vfoo(7);
}

void test_pointer(void)
{
	typedef child<int> ci_type;
	typedef child<float> cf_type;

	base* arr[] = {new ci_type(), new cf_type()};

	arr[0]->fake_vfoo(7);
	arr[1]->fake_vfoo(7);

	delete arr[0];
	delete arr[1];
}

void test_mix(void)
{
	typedef child<int> ci_type;
	typedef child<float> cf_type;

	ci_type ci;

	base* arr[] = {&ci, new cf_type()};

	arr[0]->fake_vfoo(7);
	arr[1]->fake_vfoo(7);

	delete arr[1];
}



void test_proformance(void)
{
	typedef yggr::time::runtime_performance_tester<yggr::time::time> test_type;
	int test_size = 1000000000;

	typedef child<int> ci_fake_type;
	typedef child_real<int> ci_real_type;

	std::cout << "test real" << std::endl;
	{
		base_real* ptr = new ci_real_type();
		{
			test_type t;
			for(int i = 0, isize = test_size; i != isize; ++i)
			{
				ptr->vfoo(i);
			}
		}
		delete ptr;
	}

	std::cout << "test fake" << std::endl;
	{
		base* ptr = new ci_fake_type();
		{
			test_type t;
			for(int i = 0, isize = test_size; i != isize; ++i)
			{
				ptr->fake_vfoo(i);
			}
		}
		delete ptr;
	}
}

int main(int argc, char* argv[])
{
#ifndef TEST_PROFORMACE
	test_ref();
	test_pointer();
	test_mix();

#else

	test_proformance();
#endif // TEST_PROFORMACE

	std::cout << "all test end" << std::endl;
	char cc = 0;
	std::cin >> cc;
	return 0;
}
