//class_new_delete_test.cpp

#include <iostream>

class A
{
public:
	A(void)
	{
		std::cout <<"A(void)" << std::endl;
	}

	~A(void)
	{
		std::cout << "~A(void)" << std::endl;
	}
};

class B
{
public:
	B(void)
	{
		std::cout <<"B(void)" << std::endl;
	}

	~B(void)
	{
		std::cout << "~B(void)" << std::endl;
	}
};

class C
{
public:
	C(void)
	{
		std::cout <<"C(void)" << std::endl;
	}

	~C(void)
	{
		std::cout << "~C(void)" << std::endl;
	}

	A a;
	B b;
};

class D
{
public:
	D(void)
	{
		std::cout <<"D(void)" << std::endl;
	}

	~D(void)
	{
		std::cout << "~D(void)" << std::endl;
	}
};

class E
{
public:
	E(void)
	{
		std::cout <<"E(void)" << std::endl;
	}

	~E(void)
	{
		std::cout << "~E(void)" << std::endl;
	}
};

class F : public C
{
public:
	F(void)
	{
		std::cout <<"F(void)" << std::endl;
	}

	~F(void)
	{
		std::cout << "~F(void)" << std::endl;
	}

	D d;
	E e;
};


int main(int argc, char* argv[])
{
	F *p = new F();
	std::cout << "-------------" <<std::endl;

	delete p;

	char cc = 0;
	std::cin >> cc;
	return 0;
}