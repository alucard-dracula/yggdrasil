//safe_buffered_object_test.cpp

#ifdef _MSC_VER
#   include <vld.h>
#endif //_MSC_VER
#include <iostream>
#include <vector>
#include <yggr/safe_container/safe_buffered_object.hpp>
#include <boost/bind.hpp>

typedef std::vector<int> int_vt_type;
class buf_vector
{
public:
	buf_vector(void)
	{
	}

	~buf_vector(void)
	{
		_int_vt.clear();
	}

	void clear(void)
	{
		_int_vt.clear();
	}

	void insert(int n)
	{
		_int_vt.push_back(n);
	}

	void splice_to(buf_vector& right)
	{
		for(int i = 0, isize = right._int_vt.size(); i != isize; ++i)
		{
			_int_vt.insert(_int_vt.begin() + i, right._int_vt[i]);
		}
	}

	void out_it(int size)
	{
		int isize = size < _int_vt.size()? size : _int_vt.size();
		for(int i = 0; i != isize; ++i)
		{
			std::cout << _int_vt.front() << std::endl;
			_int_vt.erase(_int_vt.begin());
		}
	}

	bool empty(void) const
	{
		return _int_vt.empty();
	}

	int size(void) const
	{
		return _int_vt.size();
	}

private:
	int_vt_type _int_vt;
};

typedef yggr::safe_container::safe_buffered_object<buf_vector> safe_buffered_buf_vector_type;

void out_it(buf_vector& bv, int size)
{
	bv.out_it(size);
}



int main(int argc, char* argv[])
{

	safe_buffered_buf_vector_type sbv(5);

	for(int i = 0; i < 10; ++i)
	{
		sbv.insert(i + 1);
	}

	sbv.get(boost::bind(&out_it, _1, 3));

	std::cout << "-------------------------1" << std::endl;

	for(int i = 0; i < 10; ++i)
	{
		sbv.insert(i + 1);
	}

	sbv.get(boost::bind(&out_it, _1, 12));
	std::cout << "-------------------------2" << std::endl;

	for(int i = 0; i < 10; ++i)
	{
		sbv.insert(i + 1);
	}

	sbv.get(boost::bind(&out_it, _1, 20));
	std::cout << "-------------------------3" << std::endl;

	char cc = 0;
	std::cin >> cc;
	return 0;
}
