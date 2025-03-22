//safe_buffered_object_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <vector>
#include <boost/bind.hpp>
#include <yggr/safe_container/safe_buffered_object.hpp>

#ifdef _MSC_VER
#   include <vld.h>
#endif //_MSC_VER

typedef std::vector<int> int_vt_type;
class buf_vector
{
public:
	buf_vector(void)
	{
	}

	~buf_vector(void)
	{
		//_int_vt.clear();
	}

	inline void clear(void)
	{
		_int_vt.clear();
	}

	inline void insert(int n)
	{
		_int_vt.push_back(n);
	}

	void splice_to(buf_vector& right)
	{
#	if 0

		for(std::size_t i = 0, isize = right._int_vt.size(); i != isize; ++i)
		{
			_int_vt.insert(_int_vt.begin() + i, right._int_vt[i]);
		}
		_int_vt.swap(right._int_vt);

#	else

		right._int_vt.reserve(right._int_vt.size() + _int_vt.size());
		right._int_vt.insert(right._int_vt.end(), _int_vt.begin(), _int_vt.end());

#	endif // 0, 1
	}

	void out_it(std::size_t size)
	{
		std::size_t isize = size < _int_vt.size()? size : _int_vt.size();
		for(std::size_t i = 0; i != isize; ++i)
		{
			std::cout << _int_vt[i] << std::endl;
		}

		_int_vt.erase(_int_vt.begin(), _int_vt.begin() + isize);
	}

	inline bool empty(void) const
	{
		return _int_vt.empty();
	}

	inline std::size_t size(void) const
	{
		return _int_vt.size();
	}

private:
	int_vt_type _int_vt;
};

typedef yggr::safe_container::safe_buffered_object<buf_vector> safe_buffered_buf_vector_type;

void out_it(buf_vector& bv, std::size_t size)
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

	sbv.get(boost::bind(&out_it, _1, 30));
	std::cout << "-------------------------3" << std::endl;

	wait_any_key(argc, argv);
	return 0;
}
