// the__if_exists_statement.cpp  
// compile with: /EHsc  
 
#include <iostream>

#ifdef _MSC_VER

#include <boost/typeof/typeof.hpp>
#include <iostream>
#include <typeinfo>
#include <boost/mpl/int.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/assert.hpp>

template<typename T>
struct non_used_obj
{
	BOOST_MPL_ASSERT((boost::mpl::false_));
};

template<int N> struct my_the_counter;

template<typename T,int N = 5/*for similarity*/>
struct my_encode_counter
{
    __if_exists(my_the_counter<N + 256>)
    {
		enum
		{
			count = my_encode_counter<T, N + 257>::count
		};
    }
    __if_not_exists(my_the_counter<N + 256>)
    {
        __if_exists(my_the_counter<N + 64>)
        {
			enum
			{
				count = my_encode_counter<T, N + 65>::count
			};
        }
        __if_not_exists(my_the_counter<N + 64>)
        {
            __if_exists(my_the_counter<N + 16>)
            {
				enum
				{
					count = my_encode_counter<T, N + 17>::count
				};
            }
            __if_not_exists(my_the_counter<N + 16>)
            {
                __if_exists(my_the_counter<N + 4>)
                {
					enum
					{
						count = my_encode_counter<T, N + 5>::count
					};
                }
                __if_not_exists(my_the_counter<N + 4>)
                {
                    __if_exists(my_the_counter<N>)
                    {
						enum
						{
							count = my_encode_counter<T, N + 1>::count
						};
                    }
                    __if_not_exists(my_the_counter<N>)
                    {
						enum
						{
							count = N
						};
                        typedef my_the_counter<N> type;
                    }
                }
            }
        }
    }
};


template<typename T, typename ID>
struct my_register_type;

 struct my_extract_type_default_param 
 {
 };

template<typename ID, typename T = my_extract_type_default_param>
struct my_extract_type;

template<typename ID>
struct my_extract_type<ID, my_extract_type_default_param>
{
	//BOOST_MPL_ASSERT((boost::mpl::false_));
    template<bool>
    struct id2type_impl;

    typedef id2type_impl<true> id2type;
};

// µ›πÈ‘⁄’‚¿Ô
template<typename ID, typename T>
struct my_extract_type : my_extract_type<ID, my_extract_type_default_param>
{
    template<>
    struct id2type_impl<true>  //VC8.0 specific bugfeature
    {
        typedef T type;
    };

	template<bool>
    struct id2type_impl;

    typedef id2type_impl<true> id2type;
};



// my_typeid_wrapper
template<int ID>
struct my_typeid_wrapper 
{
	typedef my_extract_type<boost::mpl::int_<ID> > debug_met_type;
	typedef typename debug_met_type::id2type id2type;
    typedef typename id2type::type type;

};

template<>
struct my_typeid_wrapper<1> 
{
    typedef my_typeid_wrapper<1> type;
};

template<>
struct my_typeid_wrapper<4> 
{
    typedef my_typeid_wrapper<4> type;
};


template<typename T, typename ID>
struct my_register_type : my_extract_type<ID, T>
{
};

template<typename T>
struct my_encode_type
{
	enum
	{
		value = my_encode_counter<T>::count,
		next = value + 1
	};

	typedef typename my_register_type<T, boost::mpl::int_<value> >::id2type type;
};

template<class T>
struct my_sizer
{
    typedef char(*type)[my_encode_type<T>::value];
};

template<typename T>
typename my_sizer<T>::type my_encode_start(const T&);

struct A
{
	int arr[3];
};

A foo(void)
{
	return A();
}

int main() 
{

	int n = 10;
	float f = 10;
	A a = {};

	my_typeid_wrapper<sizeof(*my_encode_start(n))>::type k = 10;
	my_typeid_wrapper<sizeof(*my_encode_start(a))>::type k2;

	std::cout << typeid(my_typeid_wrapper<sizeof(*my_encode_start(n))>::type).name() << std::endl;
	std::cout << typeid(my_typeid_wrapper<sizeof(*my_encode_start(f))>::type).name() << std::endl;
	std::cout << typeid(my_typeid_wrapper<sizeof(*my_encode_start(a))>::type).name() << std::endl;
	std::cout << typeid(my_typeid_wrapper<sizeof(*my_encode_start(foo()))>::type).name() << std::endl;

	char cc = 0;
	std::cin >> cc;

	return 0;  
}  

#else

int main() 
{
	 std::cout << "now test only using in msvc" << std::endl;
}

#endif // 