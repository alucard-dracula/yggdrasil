// string_view_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <yggr/charset/string_view.hpp>
#include <yggr/iterator_ex/iterator.hpp>
#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

void test_construct(void)
{

	std::cout << "--------------test_construct begin-----------------" << std::endl;

	{
		yggr::charset::string_view nsv;
		std::cout << nsv << std::endl;
		yggr_test_assert(nsv.empty());
	}

	{
		yggr::charset::string_view nsv("abcde");
		std::cout << nsv << std::endl;
		yggr_test_assert(0 == memcmp("abcde", nsv.data(), nsv.size()));
	}

	{
		yggr::charset::string_view nsv("abcde", 3);
		std::cout << nsv << std::endl;
		yggr_test_assert(0 == memcmp("abc", nsv.data(), nsv.size()));
	}

#ifndef YGGR_NO_CWCHAR
	{
		yggr::charset::wstring_view nsv(L"abcde", 3);
		std::cout << nsv << std::endl;
		yggr_test_assert(0 == memcmp(L"abc", nsv.data(), nsv.size() * sizeof(wchar_t)));
	}
#endif // YGGR_NO_CWCHAR

	{
		char str[] = "abcde";
		yggr::charset::string_view nsv(boost::begin(str), boost::begin(str) + sizeof(str) - 1);
		std::cout << nsv << std::endl;
		yggr_test_assert(0 == memcmp("abcde", nsv.data(), nsv.size()));
	}

	{
		yggr::charset::string_view tmp_nsv("abcde", 3);
		yggr::charset::string_view nsv(boost::move(tmp_nsv));
		std::cout << nsv << std::endl;
		yggr_test_assert(0 == memcmp("abc", nsv.data(), nsv.size()));
	}

	{
		yggr::charset::string_view tmp_nsv("abcde", 3);
		yggr::charset::string_view nsv(tmp_nsv);
		std::cout << nsv << std::endl;
		yggr_test_assert(0 == memcmp("abc", nsv.data(), nsv.size()));
	}

	std::cout << "--------------test_construct end-----------------" << std::endl;
}

void test_set_and_swap(void)
{
	std::cout << "--------------test_set_and_swap begin-----------------" << std::endl;

	// operator=
	{
		yggr::charset::string_view tmp_nsv("abcde", 3);
		yggr::charset::string_view nsv;
		nsv = boost::move(tmp_nsv);
		std::cout << nsv << std::endl;
		yggr_test_assert(0 == memcmp("abc", nsv.data(), nsv.size()));
	}

	{
		yggr::charset::string_view tmp_nsv("abcde", 3);
		yggr::charset::string_view nsv;
		nsv = tmp_nsv;
		std::cout << nsv << std::endl;
		yggr_test_assert(0 == memcmp("abc", nsv.data(), nsv.size()));
	}

	// swap
	{
		yggr::charset::string_view tmp_nsv("abcde", 3);
		yggr::charset::string_view nsv;
		nsv.swap(boost::move(tmp_nsv));
		std::cout << nsv << std::endl;
		yggr_test_assert(0 == memcmp("abc", nsv.data(), nsv.size()));
		yggr_test_assert(tmp_nsv.empty());
	}

	{
		yggr::charset::string_view tmp_nsv("abcde", 3);
		yggr::charset::string_view nsv;
		nsv.swap(tmp_nsv);
		std::cout << nsv << std::endl;
		yggr_test_assert(0 == memcmp("abc", nsv.data(), nsv.size()));
		yggr_test_assert(tmp_nsv.empty());
	}

	{
		yggr::charset::string_view tmp_nsv("abcde", 3);
		yggr::charset::string_view nsv;
		yggr::swap(nsv, boost::move(tmp_nsv));
		std::cout << nsv << std::endl;
		yggr_test_assert(0 == memcmp("abc", nsv.data(), nsv.size()));
		yggr_test_assert(tmp_nsv.empty());
	}

	{
		yggr::charset::string_view tmp_nsv("abcde", 3);
		yggr::charset::string_view nsv;
		yggr::swap(nsv, tmp_nsv);
		std::cout << nsv << std::endl;
		yggr_test_assert(0 == memcmp("abc", nsv.data(), nsv.size()));
		yggr_test_assert(tmp_nsv.empty());
	}

	std::cout << "--------------test_set_and_swap end-----------------" << std::endl;
}

void test_iterators(void)
{
	std::cout << "--------------test_iterators begin-----------------" << std::endl;

	{
		yggr::charset::string_view nsv("abcde");
		std::cout << nsv << std::endl;
		yggr_test_assert(*boost::begin(nsv) == 'a');
		yggr_test_assert(*nsv.begin() == 'a');
		yggr_test_assert(std::distance(nsv.begin(), nsv.end()) == nsv.size());

		yggr_test_assert(*nsv.cbegin() == 'a');
		yggr_test_assert(std::distance(nsv.cbegin(), nsv.cend()) == nsv.size());

		yggr_test_assert(*nsv.rbegin() == 'e');
		yggr_test_assert(std::distance(nsv.rbegin(), nsv.rend()) == nsv.size());

		yggr_test_assert(*nsv.crbegin() == 'e');
		yggr_test_assert(std::distance(nsv.crbegin(), nsv.crend()) == nsv.size());
		
	}

	std::cout << "--------------test_iterators end-----------------" << std::endl;
}

void test_element_access(void)
{
	std::cout << "--------------test_element_access begin-----------------" << std::endl;

	{
		yggr::charset::string_view nsv("abcde");
		std::cout << nsv << std::endl;

		yggr_test_assert(nsv[0] == 'a');
		yggr_test_assert(nsv[1] == 'b');

		yggr_test_assert(nsv.at(0) == 'a');
		yggr_test_assert(nsv.at(1) == 'b');

		try
		{
			nsv.at(nsv.size());
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

		yggr_test_assert(nsv[0] == nsv.front());
		yggr_test_assert(*nsv.rbegin() == nsv.back());

		yggr_test_assert(*nsv.data() == nsv.front());
	}

	std::cout << "--------------test_element_access end-----------------" << std::endl;
}

void test_capacity(void)
{
	std::cout << "--------------test_capacity begin-----------------" << std::endl;

	{
		yggr::charset::string_view nsv("abcde");
		std::cout << nsv << std::endl;

		yggr_test_assert(nsv.size() == 5);
		yggr_test_assert(nsv.length() == 5);

		//yggr::charset::string_view::size_type a = nsv.max_size();
		//yggr::charset::string_view::size_type b = yggr::mplex::numeric_limits<yggr::charset::string_view::size_type>::max_type::value;
		//yggr::charset::string_view::size_type c = yggr::charset::string_view::npos;
		yggr_test_assert(nsv.max_size() ==
				(std::min)(
					yggr::mplex::numeric_limits<yggr::charset::string_view::size_type>::max_type::value / sizeof(char),
					static_cast<yggr::charset::string_view::size_type>(
						yggr::mplex::numeric_limits<yggr::charset::string_view::difference_type>::max_type::value)));

		yggr_test_assert(!nsv.empty());
		
	}

	std::cout << "--------------test_capacity end-----------------" << std::endl;
}

void test_operations_copy(void)
{
	std::cout << "--------------test_operations_copy begin-----------------" << std::endl;

	{
		char str[1024] = {0};
		yggr::charset::string_view::size_type ret = 0;
		yggr::charset::string_view nsv("abcde");
		std::cout << nsv << std::endl;

		ret = nsv.copy(str, yggr::charset::string_view::npos);
		std::cout << str << std::endl;
		yggr_test_assert(ret == nsv.size());
		yggr_test_assert(0 == memcmp(str, nsv.data(), nsv.size()));
	}

	{
		char str[1024] = {0};
		yggr::charset::string_view::size_type ret = 0;
		yggr::charset::string_view nsv("abcde");
		std::cout << nsv << std::endl;

		ret = nsv.copy(str, -1, 1);
		std::cout << str << std::endl;
		yggr_test_assert(ret == nsv.size() - 1);
		yggr_test_assert(0 == memcmp(str, nsv.data() + 1, nsv.size() - 1));
	}

	{
		char str[1024] = {0};
		yggr::charset::string_view::size_type ret = 0;
		yggr::charset::string_view nsv("abcde");
		std::cout << nsv << std::endl;

		try
		{
			ret = nsv.copy(str, -1, nsv.size());
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	std::cout << "--------------test_operations_copy end-----------------" << std::endl;
}

void test_operations_substr(void)
{
	std::cout << "--------------test_operations_substr begin-----------------" << std::endl;

	{
		yggr::charset::string_view nsv("abcde");
		std::cout << nsv << std::endl;

		yggr::charset::string_view subnsv = nsv.substr();
		std::cout << subnsv << std::endl;
		yggr_test_assert(subnsv == nsv);
	}

	{
		yggr::charset::string_view nsv("abcde");
		std::cout << nsv << std::endl;

		yggr::charset::string_view subnsv = nsv.substr(1, 3);
		std::cout << subnsv << std::endl;
		yggr_test_assert(0 == memcmp("bcd", subnsv.data(), subnsv.size()));
	}

	{
		yggr::charset::string_view nsv("abcde");
		std::cout << nsv << std::endl;

		yggr::charset::string_view subnsv;

		try
		{
			subnsv = nsv.substr(nsv.size(), 1);
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	std::cout << "--------------test_operations_substr end-----------------" << std::endl;
}

void test_operations_compare(void)
{
	std::cout << "--------------test_operations_compare begin-----------------" << std::endl;

	{
		yggr::charset::string_view nsv("abcde");
		std::cout << nsv << std::endl;

		// inline bool compare_eq(const this_type& right) const YGGR_NOEXCEPT_OR_NOTHROW
		yggr_test_assert((yggr::charset::string_view()).compare_eq(yggr::charset::string_view()));
		yggr_test_assert(nsv.compare_eq(nsv.substr()));
		yggr_test_assert(!nsv.compare_eq(nsv.substr(1, 3)));

		// inline int compare(const this_type& right) const YGGR_NOEXCEPT_OR_NOTHROW
		yggr_test_assert(0 < nsv.compare(yggr::charset::string_view()));
		yggr_test_assert(0 == nsv.compare(nsv.substr()));
		yggr_test_assert(nsv.compare(nsv.substr(1)) < 0);
		yggr_test_assert(0 < nsv.substr(2).compare(nsv.substr(1)));
		yggr_test_assert(0 == nsv.substr(2).compare(nsv.substr(2)));

		// inline int compare(size_type pos1, size_type n1, const this_type& right) const
		yggr_test_assert(0 == nsv.compare(0, -1, nsv.substr()));
		yggr_test_assert(nsv.compare(0, -1, nsv.substr(1)) < 0);
		yggr_test_assert(0 < nsv.compare(2, -1, nsv.substr(1)));
		yggr_test_assert(0 == nsv.compare(2, -1, nsv.substr(2)));

		try
		{
			nsv.compare(nsv.size(), -1, nsv.substr(2));
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

		// inline int compare(size_type pos1, size_type n1,
		// 						const this_type& right, size_type pos2, size_type n2) const
		yggr_test_assert(0 == nsv.compare(0, -1, nsv.substr(), 0, -1));
		yggr_test_assert(nsv.compare(0, -1, nsv.substr(1), 0, 1) < 0);
		yggr_test_assert(0 < nsv.compare(2, -1, nsv.substr(1), 0, -1));
		yggr_test_assert(0 == nsv.compare(2, -1, nsv.substr(2), 0, -1));

		try
		{
			nsv.compare(nsv.size(), -1, nsv.substr(2), 0, -1);
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

		try
		{
			nsv.compare(0, -1, nsv.substr(2), nsv.size(), -1);
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

		//inline int compare(const_pointer s) const
		yggr_test_assert(0 < nsv.compare(0));
		yggr_test_assert(0 == nsv.compare(nsv.substr().data()));
		yggr_test_assert(nsv.compare(nsv.substr(1).data()) < 0);
		yggr_test_assert(0 < nsv.substr(2).compare(nsv.substr(1).data()));
		yggr_test_assert(0 == nsv.substr(2).compare(nsv.substr(2).data()));

		// inline int compare(size_type pos1, size_type n1, const_pointer s) const
		yggr_test_assert(0 < nsv.compare(0, -1, 0));
		yggr_test_assert(0 == nsv.compare(0, -1, nsv.substr().data()));
		yggr_test_assert(nsv.compare(0, -1, nsv.substr(1).data()) < 0);
		yggr_test_assert(0 < nsv.compare(2, -1, nsv.substr(1).data()));
		yggr_test_assert(0 == nsv.compare(2, -1, nsv.substr(2).data()));

		try
		{
			nsv.compare(nsv.size(), -1, nsv.substr(2).data());
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

		//inline int compare(size_type pos1, size_type n1,
		//                     const_pointer s, size_type n2) const
		yggr_test_assert(0 < nsv.compare(0, -1, 0, -1));
		yggr_test_assert(0 == nsv.compare(0, -1, nsv.substr().data(), -1));
		yggr_test_assert(nsv.compare(0, -1, nsv.substr(1).data(), -1) < 0);
		yggr_test_assert(0 < nsv.compare(2, -1, nsv.substr(1).data(), -1));
		yggr_test_assert(0 == nsv.compare(2, -1, nsv.substr(2).data(), -1));

		try
		{
			nsv.compare(nsv.size(), -1, nsv.substr(2).data(), -1);
			yggr_test_assert(false);
		}
		catch(const std::out_of_range& e)
		{
			std::cerr << e.what() << std::endl;
		}

		// operator<=>
		yggr_test_assert(yggr::charset::string_view() == yggr::charset::string_view());
		yggr_test_assert(nsv == nsv.substr());
		yggr_test_assert(nsv != nsv.substr(1));

		yggr_test_assert(nsv < nsv.substr(1));

		yggr_test_assert(nsv <= nsv.substr(1));
		yggr_test_assert(nsv <= nsv.substr());

		yggr_test_assert(nsv.substr(1) > nsv);

		yggr_test_assert(nsv.substr(1) >= nsv);
		yggr_test_assert(nsv.substr() >= nsv);
	}

	std::cout << "--------------test_operations_compare end-----------------" << std::endl;
}

void test_operations_starts_ends_with(void)
{
	std::cout << "--------------test_operations_starts_ends_with begin-----------------" << std::endl;
	{
		yggr::charset::string_view nsv("abcde");
		std::cout << nsv << std::endl;

		yggr_test_assert(nsv.starts_with(nsv.substr()));
		yggr_test_assert(!nsv.starts_with(nsv.substr(1)));

		yggr_test_assert(nsv.starts_with(nsv.front()));
		yggr_test_assert(!nsv.starts_with(nsv.back()));

		yggr_test_assert(nsv.starts_with(nsv.substr().data()));
		yggr_test_assert(!nsv.starts_with(nsv.substr(1).data()));
	}

	{
		yggr::charset::string_view nsv("abcde");
		std::cout << nsv << std::endl;

		yggr_test_assert(nsv.ends_with(nsv.substr()));
		yggr_test_assert(!nsv.ends_with(nsv.substr(1, 3)));

		yggr_test_assert(nsv.ends_with(nsv.back()));
		yggr_test_assert(!nsv.ends_with(nsv.front()));

		yggr_test_assert(nsv.ends_with(nsv.substr().data()));
		yggr_test_assert(nsv.ends_with(nsv.substr(1).data()));
		yggr_test_assert(!nsv.ends_with("edcba"));
	}

	std::cout << "--------------test_operations_starts_ends_with end-----------------" << std::endl;
}

void test_operations_contains(void)
{
	std::cout << "--------------test_operations_contains begin-----------------" << std::endl;
	{
		yggr::charset::string_view nsv("abcde");
		std::cout << nsv << std::endl;

		yggr_test_assert(nsv.contains(nsv.substr()));
		yggr_test_assert(nsv.contains(nsv.substr(1)));
		yggr_test_assert(nsv.contains(nsv.substr(2)));

		yggr_test_assert(nsv.contains(nsv.substr(1, 2)));
		yggr_test_assert(nsv.contains(nsv.substr(2, 2)));

		yggr_test_assert(!nsv.contains("dcb"));
	}

	std::cout << "--------------test_operations_contains end-----------------" << std::endl;
}

void test_operations_find(void)
{
	std::cout << "--------------test_operations_find begin-----------------" << std::endl;
	{
		yggr::charset::string_view nsv("abcde");
		std::cout << nsv << std::endl;

		// inline size_type find(const this_type& right, size_type pos = 0) const YGGR_NOEXCEPT_OR_NOTHROW
		yggr_test_assert(0 == nsv.find(nsv.substr()));
		yggr_test_assert(1 == nsv.find(nsv.substr(1)));

		yggr_test_assert(yggr::charset::string_view::npos == nsv.find(nsv.substr(), 1));
		yggr_test_assert(1 == nsv.find(nsv.substr(1), 1));

		// inline size_type find(value_type c, size_type pos = 0) const YGGR_NOEXCEPT_OR_NOTHROW
		yggr_test_assert(0 == nsv.find(nsv.front()));
		yggr_test_assert(nsv.size() - 1 == nsv.find(nsv.back()));

		yggr_test_assert(yggr::charset::string_view::npos == nsv.find(nsv.front(), 1));
		yggr_test_assert(nsv.size() - 1 == nsv.find(nsv.back(), 1));

		// inline size_type find(const_pointer s, size_type pos, size_type n) const
		yggr_test_assert(0 == nsv.find(nsv.substr().data(), 0, nsv.substr().size()));
		yggr_test_assert(1 == nsv.find(nsv.substr(1).data(), 0, nsv.substr(1).size()));

		yggr_test_assert(yggr::charset::string_view::npos == nsv.find(nsv.substr().data(), 1, nsv.substr().size()));
		yggr_test_assert(1 == nsv.find(nsv.substr(1).data(), 1, nsv.substr(1).size()));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find("z", 0, 1));
		yggr_test_assert(2 == nsv.find("c", 0, 1));
		yggr_test_assert(0 == nsv.find("", 0, 0));

		// inline size_type find(const_pointer s, size_type pos = 0) const
		yggr_test_assert(0 == nsv.find(nsv.substr().data(), 0));
		yggr_test_assert(1 == nsv.find(nsv.substr(1).data(), 0));

		yggr_test_assert(yggr::charset::string_view::npos == nsv.find(nsv.substr().data(), 1));
		yggr_test_assert(1 == nsv.find(nsv.substr(1).data(), 1));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find("z", 0));
		yggr_test_assert(2 == nsv.find("c", 0));

	}

	std::cout << "--------------test_operations_find end-----------------" << std::endl;
}

void test_operations_rfind(void)
{
	std::cout << "--------------test_operations_rfind begin-----------------" << std::endl;

	yggr::charset::string_view nsv("abcde");
	std::cout << nsv << std::endl;

	// inline size_type rfind(const this_type& right, size_type pos = this_type::npos) const YGGR_NOEXCEPT_OR_NOTHROW
	yggr_test_assert(0 == nsv.rfind(nsv.substr()));
	yggr_test_assert(1 == nsv.rfind(nsv.substr(1)));

	yggr_test_assert(0 == nsv.rfind(nsv.substr(), 1));
	yggr_test_assert(1 == nsv.rfind(nsv.substr(1), 1));

	// inline size_type rfind(value_type c, size_type pos = this_type::npos) const YGGR_NOEXCEPT_OR_NOTHROW
	yggr_test_assert(0 == nsv.rfind(nsv.front()));
	yggr_test_assert(nsv.size() - 1 == nsv.rfind(nsv.back()));

	yggr_test_assert(0 == nsv.rfind(nsv.front(), 1));
	yggr_test_assert(yggr::charset::string_view::npos == nsv.rfind(nsv.back(), 1));

	// inline size_type rfind(const_pointer s, size_type pos, size_type n) const
	yggr_test_assert(0 == nsv.rfind(nsv.substr().data(), 0, nsv.substr().size()));
	yggr_test_assert(yggr::charset::string_view::npos == nsv.rfind(nsv.substr(1).data(), 0, nsv.substr(1).size()));

	yggr_test_assert(0 == nsv.rfind(nsv.substr().data(), 1, nsv.substr().size()));
	yggr_test_assert(1 == nsv.rfind(nsv.substr(1).data(), 1, nsv.substr(1).size()));
	yggr_test_assert(yggr::charset::string_view::npos == nsv.rfind("z", 0, 1));
	yggr_test_assert(2 == nsv.find("c", 0, 1));
	yggr_test_assert(0 == nsv.rfind("", 0, 0));

	// inline size_type rfind(const_pointer s, size_type pos = 0) const
	yggr_test_assert(0 == nsv.rfind(nsv.substr().data(), 0));
	yggr_test_assert(yggr::charset::string_view::npos == nsv.rfind(nsv.substr(1).data(), 0));

	yggr_test_assert(0 == nsv.rfind(nsv.substr().data(), 1));
	yggr_test_assert(1 == nsv.rfind(nsv.substr(1).data(), 1));
	yggr_test_assert(yggr::charset::string_view::npos == nsv.rfind("z", 0));
	yggr_test_assert(yggr::charset::string_view::npos == nsv.rfind("c", 0));

	std::cout << "--------------test_operations_rfind end-----------------" << std::endl;
}

void test_operations_find_first_of(void)
{
	std::cout << "--------------test_operations_find_first_of begin-----------------" << std::endl;
	{
		yggr::charset::string_view nsv("abcde");
		std::cout << nsv << std::endl;

		// inline size_type find_first_of(const this_type& right, size_type pos = 0) const YGGR_NOEXCEPT_OR_NOTHROW
		yggr_test_assert(0 == nsv.find_first_of("ace"));
		yggr_test_assert(2 == nsv.find_first_of("ce"));

		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_first_of("fgh"));
		yggr_test_assert(2 == nsv.find_first_of("ce", 1));

		// inline size_type find_first_of(value_type c, size_type pos = 0) const YGGR_NOEXCEPT_OR_NOTHROW
		yggr_test_assert(0 == nsv.find_first_of(nsv.front()));
		yggr_test_assert(nsv.size() - 1 == nsv.find_first_of(nsv.back()));

		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_first_of(nsv.front(), 1));
		yggr_test_assert(nsv.size() - 1 == nsv.find_first_of(nsv.back(), 1));

		// inline size_type find_first_of(const_pointer s, size_type pos, size_type n) const
		yggr_test_assert(0 == nsv.find_first_of(nsv.substr().data(), 0, nsv.substr().size()));
		yggr_test_assert(1 == nsv.find_first_of(nsv.substr(1).data(), 0, nsv.substr(1).size()));

		yggr_test_assert(1 == nsv.find_first_of(nsv.substr().data(), 1, nsv.substr().size()));
		yggr_test_assert(1 == nsv.find_first_of(nsv.substr(1).data(), 1, nsv.substr(1).size()));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_first_of("z", 0, 1));
		yggr_test_assert(2 == nsv.find_first_of("c", 0, 1));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_first_of("", 0, 0));

		// inline size_type find_first_of(const_pointer s, size_type pos = 0) const
		yggr_test_assert(0 == nsv.find_first_of(nsv.substr().data(), 0));
		yggr_test_assert(1 == nsv.find_first_of(nsv.substr(1).data(), 0));

		yggr_test_assert(1 == nsv.find_first_of(nsv.substr().data(), 1));
		yggr_test_assert(1 == nsv.find_first_of(nsv.substr(1).data(), 1));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_first_of("z", 0));
		yggr_test_assert(2 == nsv.find_first_of("c", 0));

	}

	std::cout << "--------------test_operations_find_first_of end-----------------" << std::endl;
}

void test_operations_find_last_of(void)
{
	std::cout << "--------------test_operations_find_last_of begin-----------------" << std::endl;
	{
		yggr::charset::string_view nsv("abcde");
		std::cout << nsv << std::endl;

		// inline size_type find_last_of(const this_type& right, size_type pos = 0) const YGGR_NOEXCEPT_OR_NOTHROW
		yggr_test_assert(4 == nsv.find_last_of("ace"));
		yggr_test_assert(4 == nsv.find_last_of("ce"));

		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of("fgh"));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of("ce", 1));

		// inline size_type find_last_of(value_type c, size_type pos = 0) const YGGR_NOEXCEPT_OR_NOTHROW
		yggr_test_assert(0 == nsv.find_last_of(nsv.front()));
		yggr_test_assert(nsv.size() - 1 == nsv.find_last_of(nsv.back()));

		yggr_test_assert(0 == nsv.find_last_of(nsv.front(), 1));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of(nsv.back(), 1));

		// inline size_type find_last_of(const_pointer s, size_type pos, size_type n) const
		yggr_test_assert(0 == nsv.find_last_of(nsv.substr().data(), 0, nsv.substr().size()));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of(nsv.substr(1).data(), 0, nsv.substr(1).size()));

		yggr_test_assert(1 == nsv.find_last_of(nsv.substr().data(), 1, nsv.substr().size()));
		yggr_test_assert(1 == nsv.find_last_of(nsv.substr(1).data(), 1, nsv.substr(1).size()));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of("z", 0, 1));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of("c", 0, 1));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of("", 0, 0));

		// inline size_type find_last_of(const_pointer s, size_type pos = 0) const
		yggr_test_assert(0 == nsv.find_last_of(nsv.substr().data(), 0));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of(nsv.substr(1).data(), 0));

		yggr_test_assert(1 == nsv.find_last_of(nsv.substr().data(), 1));
		yggr_test_assert(1 == nsv.find_last_of(nsv.substr(1).data(), 1));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of("z", 0));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_of("c", 0));
	}

	std::cout << "--------------test_operations_find_last_of end-----------------" << std::endl;
}

void test_operations_find_first_not_of(void)
{
	std::cout << "--------------test_operations_find_first_not_of begin-----------------" << std::endl;
	{
		yggr::charset::string_view nsv("abcde");
		std::cout << nsv << std::endl;

		// inline size_type find_first_not_of(const this_type& right, size_type pos = 0) const YGGR_NOEXCEPT_OR_NOTHROW
		yggr_test_assert(1 == nsv.find_first_not_of("ace"));
		yggr_test_assert(0 == nsv.find_first_not_of("ce"));

		yggr_test_assert(0 == nsv.find_first_not_of("fgh"));
		yggr_test_assert(1 == nsv.find_first_not_of("ce", 1));

		// inline size_type find_first_not_of(value_type c, size_type pos = 0) const YGGR_NOEXCEPT_OR_NOTHROW
		yggr_test_assert(1 == nsv.find_first_not_of(nsv.front()));
		yggr_test_assert(0 == nsv.find_first_not_of(nsv.back()));

		yggr_test_assert(1 == nsv.find_first_not_of(nsv.front(), 1));
		yggr_test_assert(1 == nsv.find_first_not_of(nsv.back(), 1));

		// inline size_type find_first_not_of(const_pointer s, size_type pos, size_type n) const
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_first_not_of(nsv.substr().data(), 0, nsv.substr().size()));
		yggr_test_assert(0 == nsv.find_first_not_of(nsv.substr(1).data(), 0, nsv.substr(1).size()));

		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_first_not_of(nsv.substr().data(), 1, nsv.substr().size()));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_first_not_of(nsv.substr(1).data(), 1, nsv.substr(1).size()));
		yggr_test_assert(0 == nsv.find_first_not_of("z", 0, 1));
		yggr_test_assert(0 == nsv.find_first_not_of("c", 0, 1));
		yggr_test_assert(0 == nsv.find_first_not_of("", 0, 0));

		// inline size_type find_first_not_of(const_pointer s, size_type pos = 0) const
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_first_not_of(nsv.substr().data(), 0));
		yggr_test_assert(0 == nsv.find_first_not_of(nsv.substr(1).data(), 0));

		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_first_not_of(nsv.substr().data(), 1));
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_first_not_of(nsv.substr(1).data(), 1));
		yggr_test_assert(0 == nsv.find_first_not_of("z", 0));
		yggr_test_assert(0 == nsv.find_first_not_of("c", 0));
	}

	std::cout << "--------------test_operations_find_first_not_of end-----------------" << std::endl;
}

void test_operations_find_last_not_of(void)
{
	std::cout << "--------------test_operations_find_last_not_of begin-----------------" << std::endl;
	{
		yggr::charset::string_view nsv("abcde");
		std::cout << nsv << std::endl;

		// inline size_type find_last_not_of(const this_type& right, size_type pos = 0) const YGGR_NOEXCEPT_OR_NOTHROW
		yggr_test_assert(3 == nsv.find_last_not_of("ace"));
		yggr_test_assert(3 == nsv.find_last_not_of("ce"));

		yggr_test_assert(nsv.size() - 1 == nsv.find_last_not_of("fgh"));
		yggr_test_assert(1 == nsv.find_last_not_of("ce", 1));

		// inline size_type find_last_not_of(value_type c, size_type pos = 0) const YGGR_NOEXCEPT_OR_NOTHROW
		yggr_test_assert(nsv.size() - 1 == nsv.find_last_not_of(nsv.front()));
		yggr_test_assert(nsv.size() - 2 == nsv.find_last_not_of(nsv.back()));

		yggr_test_assert(1 == nsv.find_last_not_of(nsv.front(), 1));
		yggr_test_assert(1 == nsv.find_last_not_of(nsv.back(), 1));

		// inline size_type find_last_not_of(const_pointer s, size_type pos, size_type n) const
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_not_of(nsv.substr().data(), 0, nsv.substr().size()));
		yggr_test_assert(0 == nsv.find_last_not_of(nsv.substr(1).data(), 0, nsv.substr(1).size()));

		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_not_of(nsv.substr().data(), 1, nsv.substr().size()));
		yggr_test_assert(0 == nsv.find_last_not_of(nsv.substr(1).data(), 1, nsv.substr(1).size()));
		yggr_test_assert(0 == nsv.find_last_not_of("z", 0, 1));
		yggr_test_assert(0 == nsv.find_last_not_of("c", 0, 1));
		yggr_test_assert(0 == nsv.find_last_not_of("", 0, 0));

		// inline size_type find_last_not_of(const_pointer s, size_type pos = 0) const
		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_not_of(nsv.substr().data(), 0));
		yggr_test_assert(0 == nsv.find_last_not_of(nsv.substr(1).data(), 0));

		yggr_test_assert(yggr::charset::string_view::npos == nsv.find_last_not_of(nsv.substr().data(), 1));
		yggr_test_assert(0 == nsv.find_last_not_of(nsv.substr(1).data(), 1));
		yggr_test_assert(0 == nsv.find_last_not_of("z", 0));
		yggr_test_assert(0 == nsv.find_last_not_of("c", 0));
	}

	std::cout << "--------------test_operations_find_last_not_of end-----------------" << std::endl;
}

void test_operations_append_to(void)
{
	std::cout << "--------------test_operations_append_to begin-----------------" << std::endl;
	// +=
	{
		{
			std::string str = "abc";
			str += yggr::string_view("def");
			yggr_test_assert(str == std::string("abcdef"));
		}
		
		{
			boost::container::string str("abc");
			str += yggr::string_view("def");
			yggr_test_assert(str == boost::container::string("abcdef"));
		}

		{
			yggr::string str = "abc";
			str += yggr::string_view("def");
			yggr_test_assert(str == yggr::string("abcdef"));
		}
	}

#ifndef YGGR_NO_CWCHAR
	{
		{
			std::wstring str = L"abc";
			str += yggr::wstring_view(L"def");
			yggr_test_assert(str == std::wstring(L"abcdef"));
		}
		
		{
			boost::container::wstring str(L"abc");
			str += yggr::wstring_view(L"def");
			yggr_test_assert(str == boost::container::wstring(L"abcdef"));
		}

		{
			yggr::wstring str = L"abc";
			str += yggr::wstring_view(L"def");
			yggr_test_assert(str == yggr::wstring(L"abcdef"));
		}
	}
#endif // YGGR_NO_CWCHAR

	std::cout << "--------------test_operations_append_to end-----------------" << std::endl;
}

void test_operations_append(void)
{
	std::cout << "--------------test_operations_append begin-----------------" << std::endl;
	// operator+
	{
		yggr_test_assert(std::string("abc") + yggr::string_view("def") == std::string("abcdef"));
		yggr_test_assert(yggr::string_view("abc") + std::string("def") == std::string("abcdef"));

		yggr_test_assert(boost::container::string("abc") + yggr::string_view("def") == boost::container::string("abcdef"));
		yggr_test_assert(yggr::string_view("abc") + boost::container::string("def") == boost::container::string("abcdef"));

		yggr_test_assert(yggr::string("abc") + yggr::string_view("def") == yggr::string("abcdef"));
		yggr_test_assert(yggr::string_view("abc") + yggr::string("def") == yggr::string("abcdef"));
	}

#ifndef YGGR_NO_CWCHAR
	{
		yggr_test_assert(std::wstring(L"abc") + yggr::wstring_view(L"def") == std::wstring(L"abcdef"));
		yggr_test_assert(yggr::wstring_view(L"abc") + std::wstring(L"def") == std::wstring(L"abcdef"));

		yggr_test_assert(boost::container::wstring(L"abc") + yggr::wstring_view(L"def") == boost::container::wstring(L"abcdef"));
		yggr_test_assert(yggr::wstring_view(L"abc") + boost::container::wstring(L"def") == boost::container::wstring(L"abcdef"));

		yggr_test_assert(yggr::wstring(L"abc") + yggr::wstring_view(L"def") == yggr::wstring(L"abcdef"));
		yggr_test_assert(yggr::wstring_view(L"abc") + yggr::wstring(L"def") == yggr::wstring(L"abcdef"));
	}
#endif // YGGR_NO_CWCHAR

	std::cout << "--------------test_operations_append end-----------------" << std::endl;
}

void test_operations_op_compare(void)
{
	std::cout << "--------------test_operations_op_compare begin-----------------" << std::endl;

	// ==
	yggr_test_assert(yggr::string_view("abc") == "abc");
	yggr_test_assert("abc" == yggr::string_view("abc"));

	yggr_test_assert(yggr::string_view("abc") == std::string("abc"));
	yggr_test_assert(std::string("abc") == yggr::string_view("abc"));

	yggr_test_assert(yggr::string_view("abc") == boost::container::string("abc"));
	yggr_test_assert(boost::container::string("abc") == yggr::string_view("abc"));

	yggr_test_assert(yggr::string_view("abc") == yggr::string("abc"));
	yggr_test_assert(yggr::string("abc") == yggr::string_view("abc"));

	yggr_test_assert(yggr::string_view("abc") == yggr::string_view("abc"));

	// !=
	yggr_test_assert(yggr::string_view("abc") != "dabc");
	yggr_test_assert("abc" != yggr::string_view("dabc"));

	yggr_test_assert(yggr::string_view("abc") != std::string("dabc"));
	yggr_test_assert(std::string("abc") != yggr::string_view("dabc"));

	yggr_test_assert(yggr::string_view("abc") != boost::container::string("dabc"));
	yggr_test_assert(boost::container::string("abc") != yggr::string_view("dabc"));

	yggr_test_assert(yggr::string_view("abc") != yggr::string("dabc"));
	yggr_test_assert(yggr::string("abc") != yggr::string_view("dabc"));

	yggr_test_assert(yggr::string_view("abc") != yggr::string_view("dabc"));

	// <
	yggr_test_assert(yggr::string_view("abc") < "dabc");
	yggr_test_assert("abc" < yggr::string_view("dabc"));

	yggr_test_assert(yggr::string_view("abc") < std::string("dabc"));
	yggr_test_assert(std::string("abc") < yggr::string_view("dabc"));

	yggr_test_assert(yggr::string_view("abc") < boost::container::string("dabc"));
	yggr_test_assert(boost::container::string("abc") < yggr::string_view("dabc"));

	yggr_test_assert(yggr::string_view("abc") < yggr::string("dabc"));
	yggr_test_assert(yggr::string("abc") < yggr::string_view("dabc"));

	yggr_test_assert(yggr::string_view("abc") < yggr::string_view("dabc"));

	// <=
	yggr_test_assert(yggr::string_view("abc") <= "abc");
	yggr_test_assert("abc" <= yggr::string_view("dabc"));

	yggr_test_assert(yggr::string_view("abc") <= std::string("abc"));
	yggr_test_assert(std::string("abc") <= yggr::string_view("dabc"));

	yggr_test_assert(yggr::string_view("abc") <= boost::container::string("abc"));
	yggr_test_assert(boost::container::string("abc") <= yggr::string_view("dabc"));

	yggr_test_assert(yggr::string_view("abc") <= yggr::string("abc"));
	yggr_test_assert(yggr::string("abc") <= yggr::string_view("dabc"));

	yggr_test_assert(yggr::string_view("abc") <= yggr::string_view("abc"));

	// >
	yggr_test_assert(yggr::string_view("dabc") > "abc");
	yggr_test_assert("dabc" > yggr::string_view("abc"));

	yggr_test_assert(yggr::string_view("dabc") > std::string("abc"));
	yggr_test_assert(std::string("dabc") > yggr::string_view("abc"));

	yggr_test_assert(yggr::string_view("dabc") > boost::container::string("abc"));
	yggr_test_assert(boost::container::string("dabc") > yggr::string_view("abc"));

	yggr_test_assert(yggr::string_view("dabc") > yggr::string("abc"));
	yggr_test_assert(yggr::string("dabc") > yggr::string_view("abc"));

	yggr_test_assert(yggr::string_view("dabc") > yggr::string_view("abc"));

	// >=
	yggr_test_assert(yggr::string_view("abc") >= "abc");
	yggr_test_assert("dabc" >= yggr::string_view("abc"));

	yggr_test_assert(yggr::string_view("abc") >= std::string("abc"));
	yggr_test_assert(std::string("dabc") >= yggr::string_view("abc"));

	yggr_test_assert(yggr::string_view("abc") >= boost::container::string("abc"));
	yggr_test_assert(boost::container::string("dabc") >= yggr::string_view("abc"));

	yggr_test_assert(yggr::string_view("abc") >= yggr::string("abc"));
	yggr_test_assert(yggr::string("dabc") >= yggr::string_view("abc"));

	yggr_test_assert(yggr::string_view("dabc") >= yggr::string_view("abc"));

#ifndef YGGR_NO_CWCHAR

	// ==
	yggr_test_assert(yggr::wstring_view(L"abc") == L"abc");
	yggr_test_assert(L"abc" == yggr::wstring_view(L"abc"));

	yggr_test_assert(yggr::wstring_view(L"abc") == std::wstring(L"abc"));
	yggr_test_assert(std::wstring(L"abc") == yggr::wstring_view(L"abc"));

	yggr_test_assert(yggr::wstring_view(L"abc") == boost::container::wstring(L"abc"));
	yggr_test_assert(boost::container::wstring(L"abc") == yggr::wstring_view(L"abc"));

	yggr_test_assert(yggr::wstring_view(L"abc") == yggr::wstring(L"abc"));
	yggr_test_assert(yggr::wstring(L"abc") == yggr::wstring_view(L"abc"));

	yggr_test_assert(yggr::wstring_view(L"abc") == yggr::wstring_view(L"abc"));

	// !=
	yggr_test_assert(yggr::wstring_view(L"abc") != L"dabc");
	yggr_test_assert(L"abc" != yggr::wstring_view(L"dabc"));

	yggr_test_assert(yggr::wstring_view(L"abc") != std::wstring(L"dabc"));
	yggr_test_assert(std::wstring(L"abc") != yggr::wstring_view(L"dabc"));

	yggr_test_assert(yggr::wstring_view(L"abc") != boost::container::wstring(L"dabc"));
	yggr_test_assert(boost::container::wstring(L"abc") != yggr::wstring_view(L"dabc"));

	yggr_test_assert(yggr::wstring_view(L"abc") != yggr::wstring(L"dabc"));
	yggr_test_assert(yggr::wstring(L"abc") != yggr::wstring_view(L"dabc"));

	yggr_test_assert(yggr::wstring_view(L"abc") != yggr::wstring_view(L"dabc"));

	// <
	yggr_test_assert(yggr::wstring_view(L"abc") < L"dabc");
	yggr_test_assert(L"abc" < yggr::wstring_view(L"dabc"));

	yggr_test_assert(yggr::wstring_view(L"abc") < std::wstring(L"dabc"));
	yggr_test_assert(std::wstring(L"abc") < yggr::wstring_view(L"dabc"));

	yggr_test_assert(yggr::wstring_view(L"abc") < boost::container::wstring(L"dabc"));
	yggr_test_assert(boost::container::wstring(L"abc") < yggr::wstring_view(L"dabc"));

	yggr_test_assert(yggr::wstring_view(L"abc") < yggr::wstring(L"dabc"));
	yggr_test_assert(yggr::wstring(L"abc") < yggr::wstring_view(L"dabc"));

	yggr_test_assert(yggr::wstring_view(L"abc") < yggr::wstring_view(L"dabc"));

	// >
	yggr_test_assert(yggr::wstring_view(L"dabc") > L"abc");
	yggr_test_assert(L"dabc" > yggr::wstring_view(L"abc"));

	yggr_test_assert(yggr::wstring_view(L"dabc") > std::wstring(L"abc"));
	yggr_test_assert(std::wstring(L"dabc") > yggr::wstring_view(L"abc"));

	yggr_test_assert(yggr::wstring_view(L"dabc") > boost::container::wstring(L"abc"));
	yggr_test_assert(boost::container::wstring(L"dabc") > yggr::wstring_view(L"abc"));

	yggr_test_assert(yggr::wstring_view(L"dabc") > yggr::wstring(L"abc"));
	yggr_test_assert(yggr::wstring(L"dabc") > yggr::wstring_view(L"abc"));

	yggr_test_assert(yggr::wstring_view(L"dabc") > yggr::wstring_view(L"abc"));

	// >=
	yggr_test_assert(yggr::wstring_view(L"abc") >= L"abc");
	yggr_test_assert(L"dabc" >= yggr::wstring_view(L"abc"));

	yggr_test_assert(yggr::wstring_view(L"abc") >= std::wstring(L"abc"));
	yggr_test_assert(std::wstring(L"dabc") >= yggr::wstring_view(L"abc"));

	yggr_test_assert(yggr::wstring_view(L"abc") >= boost::container::wstring(L"abc"));
	yggr_test_assert(boost::container::wstring(L"dabc") >= yggr::wstring_view(L"abc"));

	yggr_test_assert(yggr::wstring_view(L"abc") >= yggr::wstring(L"abc"));
	yggr_test_assert(yggr::wstring(L"dabc") >= yggr::wstring_view(L"abc"));

	yggr_test_assert(yggr::wstring_view(L"dabc") >= yggr::wstring_view(L"abc"));

#endif // YGGR_NO_CWCHAR

	std::cout << "--------------test_operations_op_compare end-----------------" << std::endl;
}

void test_operations(void)
{
	std::cout << "--------------test_operations begin-----------------" << std::endl;

	test_operations_copy();
	test_operations_substr();
	test_operations_compare();
	test_operations_starts_ends_with();
	test_operations_contains();
	test_operations_find();
	test_operations_rfind();
	test_operations_find_first_of();
	test_operations_find_last_of();
	test_operations_find_first_not_of();
	test_operations_find_last_not_of();

	test_operations_append_to();
	test_operations_append();

	test_operations_op_compare();

	std::cout << "--------------test_operations end-----------------" << std::endl;
}


void test_hash(void)
{
	std::cout << "--------------test_hash begin-----------------" << std::endl;

	{
		yggr::charset::string_view nsv;
		std::cout << nsv.hash() << std::endl;
	}

	{
		yggr::charset::string_view nsv("abcde");
		std::cout << nsv.hash() << std::endl;
	}

	std::cout << "--------------test_hash end-----------------" << std::endl;
}

int main(int argc, char* argv[])
{
	test_construct();
	test_set_and_swap();
	test_iterators();
	test_element_access();
	test_capacity();
	test_operations();
	test_hash();

	wait_any_key(argc, argv);
	return 0;
}