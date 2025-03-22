//template_friend_test.cpp

#include <iostream>

template<typename T>
struct A;

template<typename T>
struct B;

template<typename T>
struct B
{
public:
  
	template<typename Val>
	void access(const A<Val>& na)
	{
		std::cout << na.n << std::endl;
	}
};

template<typename T>
struct A
{
	 template <typename T> 
	 friend class B;
	A() : n(100) {};
private:
	T n;
};


int main(int argc, char* argv[])
{
	A<int> an;

	B <float > b;
	b.access(an);
	char cc = 0;
	std::cin >> cc;
	return 0;
}
