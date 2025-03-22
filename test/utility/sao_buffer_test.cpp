// sao_buffer_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/utility/sao_buffer.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

template<typename T>
void sao_buffer_test_min(void)
{
	typedef yggr::sao_buffer<T/*, 1024*/> buf_type;
	typedef typename buf_type::iterator iter_type;
	typedef typename buf_type::const_iterator citer_type;
	typedef typename buf_type::reverse_iterator riter_type;
	typedef typename buf_type::const_reverse_iterator criter_type;
	typedef typename buf_type::pointer pointer_type;

	buf_type buf(1000);
	std::fill(buf.begin(), buf.end(), T());

	yggr_test_assert(*buf.begin() == T());
	yggr_test_assert(*buf.rbegin() == T());
	yggr_test_assert(*buf.begin() == *buf.rbegin());

	yggr_test_assert(buf.is_optimized());
}

template<typename T>
void sao_buffer_test(void)
{
	typedef yggr::sao_buffer<T/*, 1024*/> buf_type;
	typedef typename buf_type::iterator iter_type;
	typedef typename buf_type::const_iterator citer_type;
	typedef typename buf_type::reverse_iterator riter_type;
	typedef typename buf_type::const_reverse_iterator criter_type;
	typedef typename buf_type::pointer pointer_type;

	buf_type buf(2000);
	std::fill(buf.begin(), buf.end(), T());

	yggr_test_assert(*buf.begin() == T());
	yggr_test_assert(*buf.rbegin() == T());
	yggr_test_assert(*buf.begin() == *buf.rbegin());

	yggr_test_assert(!buf.is_optimized());

}


int main(int argc, char* argv[])
{

	sao_buffer_test_min<char>();
	sao_buffer_test_min<int>();
	sao_buffer_test_min<float>();

	sao_buffer_test<char>();
	sao_buffer_test<int>();
	sao_buffer_test<float>();

	wait_any_key(argc, argv);
	return 0;
}
