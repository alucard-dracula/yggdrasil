//sync_system_test.cpp

#include <iostream>
#include <vector>
#include <yggr/sync_system/sync_default_config.hpp>
#include <yggr/sync_system/sync_container_mgr.hpp>
#include <sstream>

#ifdef _WIN32
#   include <vld.h>
#endif //_WIN32

typedef yggr::sync_system::default_params_type params_type;
typedef yggr::sync_system::sync_container_mgr<int, params_type> container_mgr_type;
typedef yggr::sync_system::default_sync_helper_type helper_type;

class A
{
public:
	A(void)
		: num(0)
	{
	}

	A(int n)
		: num(n)
	{
		std::stringstream ss;
		ss << n;
		ss >>str;
	}

	bool operator<<(const helper_type& helper) // helper to var
	{
		num = helper.result().get_val<int>("num");
		str = helper.result().get_val<std::string>("str");
		return true;
	}

	bool operator>>(helper_type& helper) const //var to helper
	{
		helper.result()["num"] = ANY_SET(int, num);
		helper.result()["str"] = ANY_SET(std::string, str);
		return true;
	}

	//bool operator>>(helper_type& helper) //var to helper
	//{
	//	helper.result()["num"] = ANY_SET(int, num);
	//	helper.result()["str"] = ANY_SET(std::string, str);
	//	return true;
	//}

	bool execute(const helper_type& helper, helper_type& out_helper)
	{
		std::cout << "A execute " << num << "," << str << std::endl;
		return true;
	}

	static int type_id(void)
	{
		return 1;
	}

	int num;
	std::string str;
};

class B
{
public:
	B(void)
	{
	}

	B(int n)
	{
		std::stringstream ss;
		ss << n;
		ss >>str;
		num = str;
	}

	bool operator<<(const helper_type& helper) // helper to var
	{
		num = helper.result().get_val<std::string>("num");
		str = helper.result().get_val<std::string>("str");
		return true;
	}

	bool operator>>(helper_type& helper) const //var to helper
	{
		helper.result()["num"] = ANY_SET(std::string, num);
		helper.result()["str"] = ANY_SET(std::string, str);
		return true;
	}

	bool execute(const helper_type& helper, helper_type& out_helper)
	{
		std::cout << "B execute " << num << "," << str << std::endl;
		return true;
	}


	static int type_id(void)
	{
		return 2;
	}

	std::string num;
	std::string str;
};


class C
{
public:

	class eins
	{
	public:
		eins(void)
			: num(), bget(true)
		{
		}

		eins(int n)
			: num(n), bget(false)
		{
		}

		bool operator<<(const helper_type& helper)
		{
			num = helper.result().get_val<int>("num");
			return true;
		}

		bool operator>>(helper_type& helper) const
		{
			helper.condition()["cdt"] = ANY_SET(int, 1);
			if(!bget)
			{
				helper.result()["num"] = ANY_SET(int, num);
			}

			return true;
		}

		bool bget;
		int num;
	};

	class zwei
	{
	public:
		zwei(void)
			: bget(true)
		{
		}
		zwei(const std::vector<int>& nvt)
			: vt(nvt)
		{
		}

		bool operator<<(const helper_type& helper)
		{
			vt = helper.result().get_val<std::vector<int> >("int_vt");
			return true;
		}

		bool operator>>(helper_type& helper) const
		{
			helper.condition()["cdt"] = ANY_SET(int, 2);

			if(!bget)
			{
				helper.result()["int_vt"] = ANY_SET(std::vector<int>, vt);
			}

			return true;
		}

		void out(void)
		{
			for(int i = 0, isize = vt.size(); i != isize; ++i)
			{
				std::cout << vt[i] << std::endl;
			}
		}

		bool bget;
		std::vector<int> vt;
	};

	class drei
	{
	public:
		typedef std::map<int, helper_type> map_type;

		drei(void)
		{
		}

		map_type& map_container(void)
		{
			return _map;
		}

		void out(void) const
		{
			for(map_type::const_iterator i = _map.begin(), isize = _map.end(); i != isize; ++i)
			{
				zwei z;
				z << i->second;

				z.out();
			}
		}

	private:
		map_type _map;
	};

public:
	C(void)
	{
	}

	bool operator<<(const helper_type& helper) // helper to var
	{
		int cdt = helper.condition().get_val<int>("cdt");

		if(cdt == 1)
		{
			vt.push_back(helper.result().get_val<int>("num"));
		}

		if(cdt == 2)
		{
			//vt.push_back(helper.result().get_val<std::vectot>)
			const std::vector<int>& tvt =helper.result().get_val<std::vector<int> >("int_vt");
			int size = vt.size();
			vt.resize(size + tvt.size());
			std::copy(tvt.begin(), tvt.end(), vt.begin() + size);
		}
		return true;
	}

	bool operator>>(helper_type& helper) const
	{
		if( helper.condition().empty())
		{
			helper.result()["int_vt"] = ANY_SET(std::vector<int>, vt);
			return true;
		}

		int cdt = helper.condition().get_val<int>("cdt");

		if(cdt != 2)
		{
			return false;
		}

		helper.result()["int_vt"] = ANY_SET(std::vector<int>, vt);
		return true;
	}

	bool operator>>(helper_type& helper)
	{
		if( helper.condition().empty())
		{
			helper.result()["int_vt"] = ANY_SET(std::vector<int>, vt);
			vt.clear();
			return true;
		}

		int cdt = helper.condition().get_val<int>("cdt");

		if(cdt != 2)
		{
			return false;
		}

		helper.result()["int_vt"] = ANY_SET(std::vector<int>, vt);
		vt.clear();
		return true;
	}

	bool execute(const helper_type& helper, helper_type& out_helper) const
	{
		out();
		return true;
	}

	void out(void) const
	{
		std::cout << "---------------------" << std::endl;
		for(int i=0, isize = vt.size(); i != isize; ++i)
		{
			std::cout << vt[i] << std::endl;
		}

		std::cout << "---------------------" << std::endl;
	}
	static int type_id(void)
	{
		return 3;
	}

	std::vector<int> vt;
};


std::ostream& operator<<(std::ostream& os, const A& na)
{
	os << "A = " << na.num << ", " << na.str;
	return os;
}

std::ostream& operator<<(std::ostream& os, const B& na)
{
	os << "B = " << na.num << ", " << na.str;
	return os;
}


void test1(void)
{

	std::vector<int> vt1;
	std::vector<int> vt2(3, 100);

	vt1.swap(vt2);

	container_mgr_type mgr;
	mgr.reg_container<A>(A::type_id());
	mgr.reg_container<B>(B::type_id());
	mgr.reg_container<C>();

	A a(100);
	mgr.send(A::type_id(), a);

	A a2;
	mgr.recv(A::type_id(), a2);
	std::cout << a2 << std::endl;


	B b(200);
	mgr.send(B::type_id(), b);
	//mgr.send(B::type_id(), a); //assert error

	B b2;
	mgr.recv(B::type_id(), b2);
	std::cout << b2 << std::endl;

	mgr.send(C::type_id(), C::eins(100));
	mgr.send(C::type_id(), C::eins(200));
	mgr.send(C::type_id(), C::eins(300));
	mgr.execute(C::type_id());

	C::zwei zwei;
	mgr.recv(C::type_id(), zwei);
	//mgr.const_recv(C::type_id(), zwei);

	//const container_mgr_type& cref_mgr = mgr;
	//cref_mgr.recv(C::type_id(), zwei);

	zwei.out();

	mgr.execute(C::type_id());

}

void test2(void)
{
	container_mgr_type mgr;
	mgr.reg_container<C>(1);
	mgr.reg_container<C>(2);
	mgr.reg_container<C>(3);

	mgr.send(1, C::eins(100));
	mgr.send(1, C::eins(200));
	mgr.send(1, C::eins(300));

	mgr.send(2, C::eins(200));
	mgr.send(2, C::eins(400));
	mgr.send(2, C::eins(600));

	mgr.send(3, C::eins(300));
	mgr.send(3, C::eins(600));
	mgr.send(3, C::eins(900));

	mgr.send_all(C::eins(10000));
	mgr.send_all(C::eins(20000));
	mgr.send_all(C::eins(30000));

	mgr.execute_all();

	mgr.const_execute_all();

	C::drei drei;
	//mgr.const_recv_all(drei);
	mgr.recv_all(drei);


	drei.out();

	mgr.const_execute_all();

}

int main(int argc, char* argv[])
{

	//test1();
	test2();

	char cc = 0;
	std::cin >> cc;
	return 0;
}
