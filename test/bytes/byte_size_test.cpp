//byte_size.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <iterator>

#include <yggr/base/yggrdef.h>
#include <yggr/bytes/byte_size.hpp>
#include <yggr/time/time.hpp>
#include <yggr/time/runtime_performance_tester.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

typedef yggr::time::runtime_performance_tester<yggr::time::time> test_helper_type;

class A
{
public:
    A(void) {}
    ~A(void) {}
    yggr::u64 a;
};

struct B
{
    yggr::u32 b;
};

struct C
{
    inline yggr::u64 byte_size(void) const
    {
        return 10;
    }

    yggr::u32 b;
};

struct D
{
    inline static yggr::u64 byte_size(void)
    {
        return 20;
    }

    yggr::u32 b;
};

struct E
{
    inline static yggr::u64 byte_size(const E&)
    {
        return 30;
    }

    yggr::u32 b;
};

struct F
{
    inline static yggr::u64 byte_size(void)
    {
        return 40;
    }

    inline static yggr::u64 byte_size(const F&)
    {
        return 50;
    }

    yggr::u32 b;
};

void test_base(void)
{
    // gcc cpp03 not support inner class struct
//	class A
//	{
//	public:
//		A(void) {}
//		~A(void) {}
//		yggr::u64 a;
//	};
//
//	struct B
//	{
//		yggr::u32 b;
//	};
//
//	struct C
//	{
//		inline yggr::u64 byte_size(void) const
//		{
//			return 10;
//		}
//
//		yggr::u32 b;
//	};
//
//	struct D
//	{
//		inline static yggr::u64 byte_size(void)
//		{
//			return 20;
//		}
//
//		yggr::u32 b;
//	};
//
//	struct E
//	{
//		inline static yggr::u64 byte_size(const E&)
//		{
//			return 30;
//		}
//
//		yggr::u32 b;
//	};
//
//	struct F
//	{
//		inline static yggr::u64 byte_size(void)
//		{
//			return 40;
//		}
//
//		inline static yggr::u64 byte_size(const F&)
//		{
//			return 50;
//		}
//
//		yggr::u32 b;
//	};

	yggr_test_assert(yggr::byte_size(char(0)) == 1);
	yggr_test_assert(yggr::byte_size(yggr::s8(0)) == 1);
	yggr_test_assert(yggr::byte_size(yggr::u8(0)) == 1);

	yggr_test_assert(yggr::byte_size(wchar_t(0)) == sizeof(wchar_t));
	yggr_test_assert(yggr::byte_size(yggr::s16(0)) == 2);
	yggr_test_assert(yggr::byte_size(yggr::u16(0)) == 2);

	yggr_test_assert(yggr::byte_size(yggr::s32(0)) == 4);
	yggr_test_assert(yggr::byte_size(yggr::u32(0)) == 4);
	yggr_test_assert(yggr::byte_size(yggr::f32(0)) == 4);

	yggr_test_assert(yggr::byte_size(yggr::s64(0)) == 8);
	yggr_test_assert(yggr::byte_size(yggr::u64(0)) == 8);
	yggr_test_assert(yggr::byte_size(yggr::f64(0)) == 8);

	yggr_test_assert((yggr::byte_size(static_cast<void*>(0))) == sizeof(void*));
	yggr_test_assert((yggr::byte_size(static_cast<int*>(0))) == sizeof(int*));

	yggr_test_assert(yggr::byte_size(A()) == sizeof(A));
	yggr_test_assert(yggr::byte_size(B()) == sizeof(B));

	yggr_test_assert(yggr::byte_size(C()) == 10);
	yggr_test_assert(yggr::byte_size(D()) == 20);
	yggr_test_assert(yggr::byte_size(E()) == 30);
	yggr_test_assert(yggr::byte_size(F()) == 50);

	std::cout << "test_base success" << std::endl;
};

void test_native_array(void)
{
//	class A
//	{
//	public:
//		A(void) {}
//		~A(void) {}
//		yggr::u64 a;
//	};
//
//	struct B
//	{
//		yggr::u32 b;
//	};
//
//	struct C
//	{
//		inline yggr::u64 byte_size(void) const
//		{
//			return 10;
//		}
//
//		yggr::u32 b;
//	};
//
//	struct D
//	{
//		inline static yggr::u64 byte_size(void)
//		{
//			return 20;
//		}
//
//		yggr::u32 b;
//	};
//
//	struct E
//	{
//		inline static yggr::u64 byte_size(const E&)
//		{
//			return 30;
//		}
//
//		yggr::u32 b;
//	};
//
//	struct F
//	{
//		inline static yggr::u64 byte_size(void)
//		{
//			return 40;
//		}
//
//		inline static yggr::u64 byte_size(const F&)
//		{
//			return 50;
//		}
//
//		yggr::u32 b;
//	};

	{
		int arr[3] = {0};
		yggr_test_assert(yggr::byte_size(arr) == sizeof(arr));
		yggr_test_assert(yggr::byte_size(arr + 0, arr + 3) == sizeof(arr));
		yggr_test_assert(yggr::byte_size(std::reverse_iterator<const int*>(arr + 3),
								std::reverse_iterator<const int*>(arr)) == sizeof(arr));
	}

	{
		A arr[3] = {A()};
		yggr_test_assert(yggr::byte_size(arr) == sizeof(arr));
		yggr_test_assert(yggr::byte_size(arr + 0, arr + 3) == sizeof(arr));
		yggr_test_assert(yggr::byte_size(std::reverse_iterator<const A*>(arr + 3),
								std::reverse_iterator<const A*>(arr)) == sizeof(arr));
	}

	{
		B arr[3] = {B()};
		yggr_test_assert(yggr::byte_size(arr) == sizeof(arr));
		yggr_test_assert(yggr::byte_size(arr + 0, arr + 3) == sizeof(arr));
		yggr_test_assert(yggr::byte_size(std::reverse_iterator<const B*>(arr + 3),
								std::reverse_iterator<const B*>(arr)) == sizeof(arr));
	}

	{
		C arr[3] = {C()};
		yggr_test_assert(yggr::byte_size(arr) == 10 * 3);
		yggr_test_assert(yggr::byte_size(arr + 0, arr + 3) == 10 * 3);
		yggr_test_assert(yggr::byte_size(std::reverse_iterator<const C*>(arr + 3),
								std::reverse_iterator<const C*>(arr)) == 10 * 3);
	}

	{
		D arr[3] = {D()};
		yggr_test_assert(yggr::byte_size(arr) == 20 * 3);
		yggr_test_assert(yggr::byte_size(arr + 0, arr + 3) == 20 * 3);
		yggr_test_assert(yggr::byte_size(std::reverse_iterator<const D*>(arr + 3),
								std::reverse_iterator<const D*>(arr)) == 20 * 3);
	}

	{
		E arr[3] = {E()};
		yggr_test_assert(yggr::byte_size(arr) == 30 * 3);
		yggr_test_assert(yggr::byte_size(arr + 0, arr + 3) == 30 * 3);
		yggr_test_assert(yggr::byte_size(std::reverse_iterator<const E*>(arr + 3),
								std::reverse_iterator<const E*>(arr)) == 30 * 3);
	}

	{
		F arr[3] = {F()};
		yggr_test_assert(yggr::byte_size(arr) == 40 * 3);
		yggr_test_assert(yggr::byte_size(arr + 0, arr + 3) == 50 * 3);
		yggr_test_assert(yggr::byte_size(std::reverse_iterator<const F*>(arr + 3),
								std::reverse_iterator<const F*>(arr)) == 50 * 3);
	}

	std::cout << "test_org_array success" << std::endl;
}

#include <yggr/container/array.hpp>

void test_array(void)
{
//	class A
//	{
//	public:
//		A(void) {}
//		~A(void) {}
//		yggr::u64 a;
//	};
//
//	struct B
//	{
//		yggr::u32 b;
//	};
//
//	struct F
//	{
//		inline static yggr::u64 byte_size(void)
//		{
//			return 40;
//		}
//
//		inline static yggr::u64 byte_size(const F&)
//		{
//			return 50;
//		}
//
//		yggr::u32 b;
//	};

	{
		boost::array<int, 3> arr = {0};
		yggr_test_assert(yggr::byte_size(arr) == sizeof(int) * 3);
		yggr_test_assert(yggr::byte_size(arr.begin(), arr.end()) == sizeof(int) * 3);
		yggr_test_assert(yggr::byte_size(arr.rbegin(), arr.rend()) == sizeof(int) * 3);
	}

	{
		boost::array<A, 3> arr = {A()};
		yggr_test_assert(yggr::byte_size(arr) == sizeof(A) * 3);
		yggr_test_assert(yggr::byte_size(arr.begin(), arr.end()) == sizeof(A) * 3);
		yggr_test_assert(yggr::byte_size(arr.rbegin(), arr.rend()) == sizeof(A) * 3);
	}

	{
		boost::array<B, 3> arr = {B()};
		yggr_test_assert(yggr::byte_size(arr) == sizeof(B) * 3);
		yggr_test_assert(yggr::byte_size(arr.begin(), arr.end()) == sizeof(B) * 3);
		yggr_test_assert(yggr::byte_size(arr.rbegin(), arr.rend()) == sizeof(B) * 3);
	}

	{
		boost::array<F, 3> arr = {F()};
		yggr_test_assert(yggr::byte_size(arr) == 40 * 3);
		yggr_test_assert(yggr::byte_size(arr.begin(), arr.end()) == 50 * 3);
		yggr_test_assert(yggr::byte_size(arr.rbegin(), arr.rend()) == 50 * 3);
	}

#ifndef BOOST_NO_CXX11_HDR_ARRAY
	{
		std::array<int, 3> arr = {0};
		yggr_test_assert(yggr::byte_size(arr) == sizeof(int) * 3);
		yggr_test_assert(yggr::byte_size(arr.begin(), arr.end()) == sizeof(int) * 3);
		yggr_test_assert(yggr::byte_size(arr.rbegin(), arr.rend()) == sizeof(int) * 3);
	}

	{
		std::array<A, 3> arr = {A()};
		yggr_test_assert(yggr::byte_size(arr) == sizeof(A) * 3);
		yggr_test_assert(yggr::byte_size(arr.begin(), arr.end()) == sizeof(A) * 3);
		yggr_test_assert(yggr::byte_size(arr.rbegin(), arr.rend()) == sizeof(A) * 3);
	}

	{
		std::array<B, 3> arr = {B()};
		yggr_test_assert(yggr::byte_size(arr) == sizeof(B) * 3);
		yggr_test_assert(yggr::byte_size(arr.begin(), arr.end()) == sizeof(B) * 3);
		yggr_test_assert(yggr::byte_size(arr.rbegin(), arr.rend()) == sizeof(B) * 3);
	}

	{
		std::array<F, 3> arr = {F()};
		yggr_test_assert(yggr::byte_size(arr) == 40 * 3);
		yggr_test_assert(yggr::byte_size(arr.begin(), arr.end()) == 50 * 3);
		yggr_test_assert(yggr::byte_size(arr.rbegin(), arr.rend()) == 50 * 3);
	}
#endif // BOOST_NO_CXX11_HDR_ARRAY

	std::cout << "test_array success" << std::endl;
}

void test_pair(void)
{
//	class A
//	{
//	public:
//		A(void) {}
//		~A(void) {}
//		yggr::u64 a;
//	};
//
//	struct B
//	{
//		yggr::u32 b;
//	};

	{
		std::pair<int, double> pair(0, 1);
		yggr_test_assert(yggr::byte_size(pair) == sizeof(int) + sizeof(double));
	}

	{
		std::pair<A, B> pair = std::make_pair(A(), B());
		yggr_test_assert(yggr::byte_size(pair) == sizeof(A) + sizeof(B));
	}

	{
		std::pair<A, int> pair = std::make_pair(A(), 10);
		yggr_test_assert(yggr::byte_size(pair) == sizeof(A) + sizeof(int));
	}

	std::cout << "test_pair success" << std::endl;
}

#include <yggr/container/vector.hpp>
#include <yggr/container/list.hpp>

void test_vector_list(void)
{
//	class A
//	{
//	public:
//		A(void) {}
//		~A(void) {}
//		yggr::u64 a;
//	};
//
//	struct B
//	{
//		yggr::u32 b;
//	};
//
//	struct F
//	{
//		inline static yggr::u64 byte_size(void)
//		{
//			return 40;
//		}
//
//		inline static yggr::u64 byte_size(const F&)
//		{
//			return 50;
//		}
//
//		yggr::u32 b;
//	};

	{
		int arr[3] = {1, 2, 3};

		std::vector<int> vt(boost::begin(arr), boost::end(arr));
		yggr_test_assert(yggr::byte_size(vt) == yggr::byte_size(arr));
		yggr_test_assert(yggr::byte_size(vt.begin(), vt.end()) == yggr::byte_size(arr));
		yggr_test_assert(yggr::byte_size(vt.rbegin(), vt.rend()) == yggr::byte_size(arr));

		boost::container::list<int> list(boost::begin(arr), boost::end(arr));
		yggr_test_assert(yggr::byte_size(list) == yggr::byte_size(arr));
		yggr_test_assert(yggr::byte_size(list.begin(), list.end()) == yggr::byte_size(arr));
		yggr_test_assert(yggr::byte_size(list.rbegin(), list.rend()) == yggr::byte_size(arr));
	}

	{
		A arr[3] = {A()};

		std::vector<A> vt(boost::begin(arr), boost::end(arr));
		yggr_test_assert(yggr::byte_size(vt) == yggr::byte_size(arr));
		yggr_test_assert(yggr::byte_size(vt.begin(), vt.end()) == yggr::byte_size(arr));
		yggr_test_assert(yggr::byte_size(vt.rbegin(), vt.rend()) == yggr::byte_size(arr));

		boost::container::list<A> list(boost::begin(arr), boost::end(arr));
		yggr_test_assert(yggr::byte_size(list) == yggr::byte_size(arr));
		yggr_test_assert(yggr::byte_size(list.begin(), list.end()) == yggr::byte_size(arr));
		yggr_test_assert(yggr::byte_size(list.rbegin(), list.rend()) == yggr::byte_size(arr));
	}

	{
		B arr[3] = {B()};

		std::vector<B> vt(boost::begin(arr), boost::end(arr));
		yggr_test_assert(yggr::byte_size(vt) == yggr::byte_size(arr));
		yggr_test_assert(yggr::byte_size(vt.begin(), vt.end()) == yggr::byte_size(arr));
		yggr_test_assert(yggr::byte_size(vt.rbegin(), vt.rend()) == yggr::byte_size(arr));

		boost::container::list<B> list(boost::begin(arr), boost::end(arr));
		yggr_test_assert(yggr::byte_size(list) == yggr::byte_size(arr));
		yggr_test_assert(yggr::byte_size(list.begin(), list.end()) == yggr::byte_size(arr));
		yggr_test_assert(yggr::byte_size(list.rbegin(), list.rend()) == yggr::byte_size(arr));
	}

	{
		F arr[3] = {F()};

		std::vector<F> vt(boost::begin(arr), boost::end(arr));
		yggr_test_assert(yggr::byte_size(vt) == 40 * 3);
		yggr_test_assert(yggr::byte_size(vt.begin(), vt.end()) == 50 * 3);
		yggr_test_assert(yggr::byte_size(vt.rbegin(), vt.rend()) == 50 * 3);

		boost::container::list<F> list(boost::begin(arr), boost::end(arr));
		yggr_test_assert(yggr::byte_size(list) == 40 * 3);
		yggr_test_assert(yggr::byte_size(list.begin(), list.end()) == 50 * 3);
		yggr_test_assert(yggr::byte_size(list.rbegin(), list.rend()) == 50 * 3);
	}

	{
		std::pair<int, int> arr[3] = {std::make_pair(1, 1)};

		std::vector< std::pair<int, int> > vt(boost::begin(arr), boost::end(arr));
		yggr_test_assert(yggr::byte_size(vt) == yggr::byte_size(arr));
		yggr_test_assert(yggr::byte_size(vt) == sizeof(int) * 2 * 3);
		yggr_test_assert(yggr::byte_size(vt.begin(), vt.end()) == yggr::byte_size(arr));
		yggr_test_assert(yggr::byte_size(vt.rbegin(), vt.rend()) == sizeof(int) * 2 * 3);

		boost::container::list<std::pair< int, int> > list(boost::begin(arr), boost::end(arr));
		yggr_test_assert(yggr::byte_size(list) == yggr::byte_size(arr));
		yggr_test_assert(yggr::byte_size(list.begin(), list.end()) == yggr::byte_size(arr));
		yggr_test_assert(yggr::byte_size(list.rbegin(), list.rend()) == yggr::byte_size(arr));
	}

	std::cout << "test_vector_list success" << std::endl;
}

#include <yggr/safe_container/safe_stack.hpp>

void test_stack(void)
{
	{
		std::stack<int> stack;

		stack.push(10);
		stack.push(20);

		yggr_test_assert(yggr::byte_size(stack) == sizeof(int) * 2);
	}

	{
		yggr::container::stack<int> stack;

		stack.push(10);
		stack.push(20);

		yggr_test_assert(yggr::byte_size(stack) == sizeof(int) * 2);
	}

	{
		yggr::safe_container::safe_stack<int> stack;

		stack.push(10);
		stack.push(20);

		yggr_test_assert(yggr::byte_size(stack) == sizeof(int) * 2);
	}

	std::cout << "test_stack success" << std::endl;
}

#include <yggr/safe_container/safe_queue.hpp>

void test_queue(void)
{
	{
		std::queue<int> queue;

		queue.push(10);
		queue.push(20);

		yggr_test_assert(yggr::byte_size(queue) == sizeof(int) * 2);
	}

	{
		yggr::container::queue<int> queue;

		queue.push(10);
		queue.push(20);

		yggr_test_assert(yggr::byte_size(queue) == sizeof(int) * 2);
	}

	{
		yggr::safe_container::safe_queue<int> queue;

		queue.push(10);
		queue.push(20);

		yggr_test_assert(yggr::byte_size(queue) == sizeof(int) * 2);
	}

	{
		std::priority_queue<int> queue;

		queue.push(10);
		queue.push(20);

		yggr_test_assert(yggr::byte_size(queue) == sizeof(int) * 2);
	}

	{
		yggr::container::priority_queue<int> queue;

		queue.push(10);
		queue.push(20);

		yggr_test_assert(yggr::byte_size(queue) == sizeof(int) * 2);
	}

	{
		yggr::safe_container::safe_priority_queue<int> queue;

		queue.push(10);
		queue.push(20);

		yggr_test_assert(yggr::byte_size(queue) == sizeof(int) * 2);
	}

	std::cout << "test_queue success" << std::endl;
}


#include <yggr/charset/utf8_string.hpp>
void test_strings(void)
{
	const char str_data[] = "abc";


	{
		const char str[] = "abc";
		yggr_test_assert(yggr::byte_size(str) == sizeof(str_data));
	}

	{
		std::string str(str_data);
		yggr_test_assert(yggr::byte_size(str) == sizeof(str_data) - sizeof(char));
		yggr_test_assert(yggr::byte_size(str) == str.size() * sizeof(char));
		yggr_test_assert(yggr::byte_size(str.begin(), str.end()) == str.size() * sizeof(char));
		yggr_test_assert(yggr::byte_size(str.rbegin(), str.rend()) == str.size() * sizeof(char));
	}

	{
		yggr::utf8_char ch('a');
		yggr_test_assert(yggr::byte_size(ch) == ch.size() * sizeof(char));
		yggr_test_assert(yggr::byte_size(ch.begin(), ch.end()) == ch.size() * sizeof(char));

		yggr_test_assert(yggr::byte_size(ch.view()) == ch.size() * sizeof(char));
		yggr_test_assert(yggr::byte_size(ch.view().begin(), ch.view().end()) == ch.size() * sizeof(char));


        yggr_test_assert(yggr::byte_size(yggr::utf8_char::char_reference_type(ch)) == ch.size() * sizeof(char));
		yggr_test_assert(yggr::byte_size(yggr::utf8_char::char_reference_type(ch).begin(),
								yggr::utf8_char::char_reference_type(ch).end()) == ch.size() * sizeof(char));

		yggr_test_assert(yggr::byte_size(yggr::utf8_char::char_const_reference_type(ch)) == ch.size() * sizeof(char));
		yggr_test_assert(yggr::byte_size(yggr::utf8_char::char_const_reference_type(ch).begin(),
								yggr::utf8_char::char_const_reference_type(ch).end()) == ch.size() * sizeof(char));

	}

	{
		yggr::utf8_string str(str_data);
		yggr_test_assert(yggr::byte_size(str) == str.size() * sizeof(char));
		yggr_test_assert(yggr::byte_size(yggr::utf8_string(str_data)) == str.size() * sizeof(char));

		yggr_test_assert(yggr::byte_size(str.begin(), str.end()) == str.size() * sizeof(char));
		yggr_test_assert(yggr::byte_size(str.rbegin(), str.rend()) == str.size() * sizeof(char));

		yggr_test_assert(yggr::byte_size(str.cbegin(), str.cend()) == str.size() * sizeof(char));
		yggr_test_assert(yggr::byte_size(str.crbegin(), str.crend()) == str.size() * sizeof(char));

		yggr_test_assert(yggr::byte_size(str.ncbegin(), str.ncend()) == str.size() * sizeof(char));
		yggr_test_assert(yggr::byte_size(str.ncrbegin(), str.ncrend()) == str.size() * sizeof(char));
	}

	{
		std::pair<std::string, std::string> pair = std::make_pair("aaa", "aaa"); // size = 6
		yggr_test_assert(yggr::byte_size(pair) == 6);
	}

#ifndef YGGR_NO_CWCHAR

	const wchar_t wstr_data[] = L"abcdefg";

	{
		std::wstring str(wstr_data);
		yggr_test_assert(yggr::byte_size(str) == sizeof(wstr_data) - sizeof(wchar_t));
		yggr_test_assert(yggr::byte_size(str) == str.size() * sizeof(wchar_t));

		yggr_test_assert(yggr::byte_size(str.begin(), str.end()) == str.size() * sizeof(wchar_t));
		yggr_test_assert(yggr::byte_size(str.rbegin(), str.rend()) == str.size() * sizeof(wchar_t));
	}

	{
		yggr::utf8_string str(wstr_data);
		yggr_test_assert(yggr::byte_size(str) == str.size() * sizeof(char));
		yggr_test_assert(yggr::byte_size(str) != sizeof(wstr_data) - sizeof(wchar_t));

		yggr_test_assert(yggr::byte_size(str.begin(), str.end()) == str.size() * sizeof(char));
		yggr_test_assert(yggr::byte_size(str.rbegin(), str.rend()) == str.size() * sizeof(char));

		yggr_test_assert(yggr::byte_size(str.cbegin(), str.cend()) == str.size() * sizeof(char));
		yggr_test_assert(yggr::byte_size(str.crbegin(), str.crend()) == str.size() * sizeof(char));

		yggr_test_assert(yggr::byte_size(str.ncbegin(), str.ncend()) == str.size() * sizeof(char));
		yggr_test_assert(yggr::byte_size(str.ncrbegin(), str.ncrend()) == str.size() * sizeof(char));
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_strings success" << std::endl;
}


#include <yggr/container/set.hpp>

void test_set(void)
{

	{
		int arr[3] = {1, 2, 3};
		std::set<int> set(boost::begin(arr), boost::end(arr));
		yggr_test_assert(yggr::byte_size(set) == sizeof(arr));
	}

	{
		const char* arr[3] = {"aaa", "bbbb", "ccccc"}; // size = 12
		std::set<std::string> set(boost::begin(arr), boost::end(arr));
		yggr_test_assert(yggr::byte_size(set) == 12);
	}

	std::cout << "test_set success" << std::endl;
}

#include <map>

void test_map(void)
{
	{
		std::pair<int, int> arr[3] =
		{
			std::make_pair(1, 1),
			std::make_pair(2, 1),
			std::make_pair(3, 1)
		}; // size = 3 * 8

		std::map<int, int> map(boost::begin(arr), boost::end(arr));
		yggr_test_assert(yggr::byte_size(map) == 24);
	}

	{
		std::pair<std::string, std::string> arr[3] =
		{
			std::make_pair(std::string("aaa"), std::string("aaa")), // 6
			std::make_pair(std::string("bbbb"), std::string("bbbb")), // 8
			std::make_pair(std::string("ccccc"), std::string("ccccc")) // 10
		}; // size = 24

		std::map<std::string, std::string> map(boost::begin(arr), boost::end(arr));
		yggr_test_assert(yggr::byte_size(map) == 24);
	}

	{
		std::pair<std::string, int> arr[3] =
		{
			std::make_pair(std::string("aaa"), 1), // 7
			std::make_pair(std::string("bbbb"), 2), // 8
			std::make_pair(std::string("ccccc"), 3) // 9
		}; // size = 24

		std::map<std::string, int> map(boost::begin(arr), boost::end(arr));
		yggr_test_assert(yggr::byte_size(map) == 24);
	}

	std::cout << "test_map success" << std::endl;
}

#include <boost/unordered_set.hpp>
void test_unordered_set(void)
{

	{
		int arr[3] = {1, 2, 3};
		boost::unordered_set<int> set(boost::begin(arr), boost::end(arr));
		yggr_test_assert(yggr::byte_size(set) == sizeof(arr));
	}

	{
		const char* arr[3] = {"aaa", "bbbb", "ccccc"}; // size = 12
		boost::unordered_set<std::string> set(boost::begin(arr), boost::end(arr));
		yggr_test_assert(yggr::byte_size(set) == 12);
	}

	std::cout << "test_unordered_set success" << std::endl;
}

#include <boost/unordered_map.hpp>

void test_unordered_map(void)
{
	{
		std::pair<int, int> arr[3] =
		{
			std::make_pair(1, 1),
			std::make_pair(2, 1),
			std::make_pair(3, 1)
		}; // size = 3 * 8

		boost::unordered_map<int, int> map(boost::begin(arr), boost::end(arr));
		yggr_test_assert(yggr::byte_size(map) == 24);
	}

	{
		std::pair<std::string, std::string> arr[3] =
		{
			std::make_pair(std::string("aaa"), std::string("aaa")), // 6
			std::make_pair(std::string("bbbb"), std::string("bbbb")), // 8
			std::make_pair(std::string("ccccc"), std::string("ccccc")) // 10
		}; // size = 24

		boost::unordered_map<std::string, std::string> map(boost::begin(arr), boost::end(arr));
		yggr_test_assert(yggr::byte_size(map) == 24);
	}

	{
		std::pair<std::string, int> arr[3] =
		{
			std::make_pair(std::string("aaa"), 1), // 7
			std::make_pair(std::string("bbbb"), 2), // 8
			std::make_pair(std::string("ccccc"), 3) // 9
		}; // size = 24

		boost::unordered_map<std::string, int> map(boost::begin(arr), boost::end(arr));
		yggr_test_assert(yggr::byte_size(map) == 24);
	}

	std::cout << "test_unordered_map success" << std::endl;
}

#include <yggr/safe_container/safe_vector.hpp>
#include <yggr/safe_container/safe_list.hpp>

void test_safe_vector_list(void)
{
//	class A
//	{
//	public:
//		A(void) {}
//		~A(void) {}
//		yggr::u64 a;
//	};
//
//	struct B
//	{
//		yggr::u32 b;
//	};

	{
		int arr[3] = {1, 2, 3};

		yggr::safe_container::safe_vector<int> vt(boost::begin(arr), boost::end(arr));
		yggr_test_assert(yggr::byte_size(vt) == yggr::byte_size(arr));

		yggr::safe_container::safe_list<int> list(boost::begin(arr), boost::end(arr));
		yggr_test_assert(yggr::byte_size(list) == yggr::byte_size(arr));
	}

	{
		A arr[3] = {A()};

		yggr::safe_container::safe_vector<A> vt(boost::begin(arr), boost::end(arr));
		yggr_test_assert(yggr::byte_size(vt) == yggr::byte_size(arr));

		yggr::safe_container::safe_list<A> list(boost::begin(arr), boost::end(arr));
		yggr_test_assert(yggr::byte_size(list) == yggr::byte_size(arr));
	}

	{
		B arr[3] = { B() };

		yggr::safe_container::safe_vector<B> vt(boost::begin(arr), boost::end(arr));
		yggr_test_assert(yggr::byte_size(vt) == yggr::byte_size(arr));

		yggr::safe_container::safe_list<B> list(boost::begin(arr), boost::end(arr));
		yggr_test_assert(yggr::byte_size(list) == yggr::byte_size(arr));
	}

	std::cout << "test_safe_vector_list success" << std::endl;
}

#include <yggr/safe_container/safe_stack.hpp>

void test_safe_stack(void)
{
	{
		int arr[3] = {1, 2, 3};
		typedef std::vector<int> vt_type;
		typedef yggr::safe_container::safe_stack<vt_type> stack_type;

		stack_type stack;

		stack.push(vt_type(arr + 0, arr + 3));
		stack.push(vt_type(arr + 0, arr + 3));

		yggr_test_assert(yggr::byte_size(stack) == sizeof(arr) * 2);
	}

	std::cout << "test_safe_stack success" << std::endl;
}

#include <yggr/safe_container/safe_set.hpp>
void test_safe_set(void)
{

	{
		int arr[3] = {1, 2, 3};
		yggr::safe_container::safe_set<int> set(boost::begin(arr), boost::end(arr));
		yggr_test_assert(yggr::byte_size(set) == sizeof(arr));
	}

	{
		const char* arr[3] = {"aaa", "bbbb", "ccccc"}; // size = 12
		yggr::safe_container::safe_set<std::string> set(boost::begin(arr), boost::end(arr));
		yggr_test_assert(yggr::byte_size(set) == 12);
	}

	std::cout << "test_safe_set success" << std::endl;
}

#include <yggr/safe_container/safe_map.hpp>
void test_safe_map(void)
{
	{
		std::pair<int, int> arr[3] =
		{
			std::make_pair(1, 1),
			std::make_pair(2, 1),
			std::make_pair(3, 1)
		}; // size = 3 * 8

		yggr::safe_container::safe_map<int, int> map(boost::begin(arr), boost::end(arr));
		yggr_test_assert(yggr::byte_size(map) == 24);
	}

	{
		std::pair<std::string, std::string> arr[3] =
		{
			std::make_pair(std::string("aaa"), std::string("aaa")), // 6
			std::make_pair(std::string("bbbb"), std::string("bbbb")), // 8
			std::make_pair(std::string("ccccc"), std::string("ccccc")) // 10
		}; // size = 24

		yggr::safe_container::safe_map<std::string, std::string> map(boost::begin(arr), boost::end(arr));
		yggr_test_assert(yggr::byte_size(map) == 24);
	}

	{
		std::pair<std::string, int> arr[3] =
		{
			std::make_pair(std::string("aaa"), 1), // 7
			std::make_pair(std::string("bbbb"), 2), // 8
			std::make_pair(std::string("ccccc"), 3) // 9
		}; // size = 24

		yggr::safe_container::safe_map<std::string, int> map(boost::begin(arr), boost::end(arr));
		yggr_test_assert(yggr::byte_size(map) == 24);
	}

	std::cout << "test_safe_map success" << std::endl;
}

#include <yggr/safe_container/safe_unordered_set.hpp>
void test_safe_unordered_set(void)
{

	{
		int arr[3] = {1, 2, 3};
		yggr::safe_container::safe_unordered_set<int> set(boost::begin(arr), boost::end(arr));
		yggr_test_assert(yggr::byte_size(set) == sizeof(arr));
	}

	{
		const char* arr[3] = {"aaa", "bbbb", "ccccc"}; // size = 12
		yggr::safe_container::safe_unordered_set<std::string> set(boost::begin(arr), boost::end(arr));
		yggr_test_assert(yggr::byte_size(set) == 12);
	}

	std::cout << "test_safe_unordered_set success" << std::endl;
}

#include <yggr/safe_container/safe_unordered_map.hpp>
void test_safe_unordered_map(void)
{
	{
		std::pair<int, int> arr[3] =
		{
			std::make_pair(1, 1),
			std::make_pair(2, 1),
			std::make_pair(3, 1)
		}; // size = 3 * 8

		yggr::safe_container::safe_unordered_map<int, int> map(boost::begin(arr), boost::end(arr));
		yggr_test_assert(yggr::byte_size(map) == 24);
	}

	{
		std::pair<std::string, std::string> arr[3] =
		{
			std::make_pair(std::string("aaa"), std::string("aaa")), // 6
			std::make_pair(std::string("bbbb"), std::string("bbbb")), // 8
			std::make_pair(std::string("ccccc"), std::string("ccccc")) // 10
		}; // size = 24

		yggr::safe_container::safe_unordered_map<std::string, std::string> map(boost::begin(arr), boost::end(arr));
		yggr_test_assert(yggr::byte_size(map) == 24);
	}

	{
		std::pair<std::string, int> arr[3] =
		{
			std::make_pair(std::string("aaa"), 1), // 7
			std::make_pair(std::string("bbbb"), 2), // 8
			std::make_pair(std::string("ccccc"), 3) // 9
		}; // size = 24

		yggr::safe_container::safe_unordered_map<std::string, int> map(boost::begin(arr), boost::end(arr));
		yggr_test_assert(yggr::byte_size(map) == 24);
	}

	std::cout << "test_safe_unordered_map success" << std::endl;
}

#include <yggr/safe_container/safe_multi_index_hashed_index.hpp>
#include <yggr/safe_container/safe_multi_index_ordered_index.hpp>
#include <yggr/safe_container/safe_multi_index_random_access_index.hpp>
#include <yggr/safe_container/safe_multi_index_sequenced_index.hpp>
#include <yggr/safe_container/safe_multi_index_container.hpp>

void test_multi_index_container(void)
{
	{
		typedef boost::multi_index::multi_index_container<int> mi_cont_type;
		int arr[3] = {1, 2, 3};

		mi_cont_type midx(boost::begin(arr), boost::end(arr));
		yggr_test_assert(yggr::byte_size(midx) == sizeof(arr));
		yggr_test_assert(yggr::byte_size(midx.begin(), midx.end()) == sizeof(arr));
		yggr_test_assert(yggr::byte_size(midx.rbegin(), midx.rend()) == sizeof(arr));
	}

	{
		typedef boost::multi_index::multi_index_container<std::string> mi_cont_type;
		const char* arr[3] = {"aaa", "bbbb", "ccccc"}; // size = 12

		mi_cont_type midx(boost::begin(arr), boost::end(arr));
		yggr_test_assert(yggr::byte_size(midx) == 12);
		yggr_test_assert(yggr::byte_size(midx.begin(), midx.end()) == 12);
		yggr_test_assert(yggr::byte_size(midx.rbegin(), midx.rend()) == 12);

	}

	std::cout << "test_multi_index_container success" << std::endl;
}

void test_safe_multi_index_container(void)
{
	{
		typedef yggr::safe_container::safe_multi_index_container<int> mi_cont_type;
		int arr[3] = {1, 2, 3};

		mi_cont_type midx(boost::begin(arr), boost::end(arr));
		yggr_test_assert(yggr::byte_size(midx) == sizeof(arr));
	}

	{
		typedef yggr::safe_container::safe_multi_index_container<std::string> mi_cont_type;
		const char* arr[3] = {"aaa", "bbbb", "ccccc"}; // size = 12

		mi_cont_type midx(boost::begin(arr), boost::end(arr));
		yggr_test_assert(yggr::byte_size(midx) == 12);

	}

	std::cout << "test_safe_multi_index_container success" << std::endl;
}

void test_mix_container(void)
{
	typedef std::vector<int> vt_type;
	typedef std::map<int, vt_type> map_type;
	typedef std::list<map_type> list_type;

	typedef boost::unordered_map<std::string, list_type> mix_cont_type;

	const char* arr_str[3] =
	{
		"aaa", // 3
		"bbb", // 3
		"ccc"  // 3
	}; // 9

	int arr_int[10] = {1, 2, 3}; // 40

	vt_type vt(boost::begin(arr_int), boost::end(arr_int)); // 40

	yggr_test_assert(yggr::byte_size(vt) == 40);

	map_type map; // 132
	map[arr_int[0]] = vt; // 40 + 4 = 44
	map[arr_int[1]] = vt; // 40 + 4 = 44
	map[arr_int[2]] = vt; // 40 + 4 = 44

	yggr_test_assert(yggr::byte_size(map) == 132);

	list_type list; // 396
	list.push_back(map); // 132
	list.push_back(map); // 132
	list.push_back(map); // 132

	yggr_test_assert(yggr::byte_size(list) == 396);

	mix_cont_type cont; //  1197
	cont[arr_str[0]] = list; // 396 + 3 = 399
	cont[arr_str[1]] = list; // 396 + 3
	cont[arr_str[2]] = list; // 396 + 3

	{
		test_helper_type test;
		//std::cout << yggr::byte_size(cont) << std::endl;
		yggr_test_assert(yggr::byte_size(cont) == 1197);
	}

	std::cout << "test_mix_container success" << std::endl;
}

int main(int argc, char* argv[])
{
	test_base();
	test_native_array();
	test_array();
	test_pair();

	test_vector_list();
	test_stack();
	test_queue();
	test_strings();
	test_set();
	test_map();
	test_unordered_set();
	test_unordered_map();

	test_safe_vector_list();
	test_safe_stack();
	test_safe_set();
	test_safe_map();
	test_safe_unordered_set();
	test_safe_unordered_map();

	test_multi_index_container();
	test_safe_multi_index_container();

	test_mix_container();

	std::cout << "------------all_test_end----------" << std::endl;

	wait_any_key(argc, argv);
	return 0;
}
