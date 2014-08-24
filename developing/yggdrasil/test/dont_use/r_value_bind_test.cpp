//r_value_bind_test.cpp

#error "test result need mend bind's  list 0-9 def and _bi::value. "

#include <iostream>
#include <boost/move/move.hpp>
#include <boost/bind.hpp>
#include <string>

namespace test
{
template<typename T>
class r_reference_wrapper
{ 
public:
    typedef T type;

#if defined( BOOST_MSVC ) && BOOST_WORKAROUND( BOOST_MSVC, < 1300 )

    explicit r_reference_wrapper(BOOST_RV_REF(T) t): t_(&t) {}

#else

    explicit r_reference_wrapper(BOOST_RV_REF(T) t): t_(boost::addressof(t)) {}

#endif

    operator BOOST_RV_REF(T) () const { return boost::forward<T>(*t_); }

    BOOST_RV_REF(T) get() const { return boost::forward<T>(*t_); }

    T* get_pointer() const { return t_; }

private:

    T* t_;
};

template<class T> 
inline r_reference_wrapper<T> const r_ref(BOOST_RV_REF(T) t)
{ 
    return r_reference_wrapper<T>(boost::forward<T>(t));
}

template<class T> class reference_wrapper
{ 
public:
    typedef T type;

#if defined( BOOST_MSVC ) && BOOST_WORKAROUND( BOOST_MSVC, < 1300 )

    explicit reference_wrapper(T& t): t_(&t) {}

#else

    explicit reference_wrapper(T& t): t_(boost::addressof(t)) {}

#endif

    operator T& () const { return *t_; }

    T& get() const { return *t_; }

    T* get_pointer() const { return t_; }

private:

    T* t_;
};

template<class T> inline reference_wrapper<T> const ref(T & t)
{ 
    return reference_wrapper<T>(t);
}

template<class T> inline reference_wrapper<T const> const cref(T const & t)
{
    return reference_wrapper<T const>(t);
}

}// namespace test



void test_move1(BOOST_RV_REF(std::string) str1)
{
	std::cout << boost::forward<std::string>(str1) << std::endl;
}

void test_move3(const std::string& str3)
{
	std::cout << str3 << std::endl;
}

void test_move2(BOOST_RV_REF(std::string) str2)
{
	//test_move1(str2); // error
	test_move1(boost::forward<std::string>(str2));
	test_move3(str2);
}

void test_move(void)
{
	std::string str = "abc";
	test_move2(boost::move(str));
	std::cout << "str = " << std::endl;
}

void test_bind(void)
{
	std::string str1 = "abc";
	test_move1(test::r_ref(boost::move(str1)));
	//boost::bind(&test_move1, test::r_ref(boost::move(str1)))();

	std::string str2 = "def";
	test_move3(test::cref(str2));
	boost::bind(&test_move3, test::cref(str2))();
}
int main(int argc, char* argv[])
{
	test_bind();

	char cc = 0;
	std::cin >> cc;
}
