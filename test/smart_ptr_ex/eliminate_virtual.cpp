// eliminate_virtual.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/container/vector.hpp>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/smart_ptr_ex/derived_deleter.hpp>

#include <iterator>
#include <typeinfo>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

struct base_t
{
protected:
	typedef base_t this_type;

	typedef void (*vfoo_type)(const this_type*, int n);
public:
	base_t(vfoo_type pfoo)
		: _pfoo(pfoo)
	{
	}

	/*virtual*/ // eliminate eliminate destruct virtual
	~base_t(void) 
	{
	}

public:
	/*virtual*/ // eliminate virtual function
	inline void vfoo(int n) const
	{
		yggr_test_assert(_pfoo);
		if(!_pfoo)
		{
			return;
		}

		(*_pfoo)(this, n);
	}

protected:
	vfoo_type _pfoo;
};

typedef yggr::shared_ptr<base_t> base_ptr_type;

template<typename T>
struct derived
	: public base_t
{
public:
	typedef base_t base_type;
	typedef T value_type;

private:
	typedef derived this_type;

public:
	derived(const value_type& val)
		: base_type(&this_type::pro_s_vfoo),
			_val(val)
	{
	}

	/*virtual*/ // eliminate eliminate destruct virtual
	~derived(void)
	{
	}

protected:
	inline void pro_vfoo(int n) const
	{
		std::cout << typeid(value_type).name() << ", " << _val << ", " << n << std::endl;
	}

protected:
	inline static void pro_s_vfoo(const base_type* pbase, int n)
	{
		yggr_test_assert(pbase);
		const this_type* pthis = reinterpret_cast<const this_type*>(pbase);
		yggr_test_assert(pthis);

		if(pthis)
		{
			pthis->pro_vfoo(n);
		}
	}

private:
	value_type _val;
};

template<typename C, typename T>
C& push_data(C& c, const T& val)
{
	typedef derived<T> derived_type;
	typedef yggr::smart_ptr_ex::derived_deleter<derived_type> derived_deleter_type;

	try
	{
		c.push_back(base_ptr_type(new derived_type(val), derived_deleter_type()));
	}
	catch(...)
	{
		return c;
	}

	return c;
}

template<typename C>
const C& out(const C& cont)
{
	typedef C cont_type;
	typedef typename cont_type::const_iterator citer_type;

	for(citer_type i = cont.begin(), isize = cont.end(); i != isize; ++i)
	{
		if(*i)
		{
			(*i)->vfoo(std::distance(cont.begin(), i));
		}
	}

	return cont;
}

int main(int argc, char* argv[])
{
	typedef yggr::vector<base_ptr_type> base_ptr_vt_type;

	{
		base_ptr_vt_type vt;

		push_data(vt, 10);
		push_data(vt, 20.5f);
		push_data(vt, 30.555);

		out(vt);

		vt.clear();
	}

	wait_any_key(argc, argv);
	return 0;
};