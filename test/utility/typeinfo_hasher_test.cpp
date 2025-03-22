// typeinfo_hasher_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <yggr/utility/typeinfo.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

template<typename TypeInfo>
std::size_t hash_foo(const TypeInfo& val)
{
	typedef boost::hash<TypeInfo> hash_type;
	hash_type h;
	return h(val);
}

template<typename TypeInfo>
std::size_t hash_ntob_foo(const TypeInfo& val)
{
	typedef boost::hash<TypeInfo> hash_type;
	hash_type h;
	return h(boost::cref(val));
}

template<typename TypeInfo>
std::size_t hash_bton_foo(const TypeInfo& val)
{
	typedef boost::hash<TypeInfo> hash_type;
	hash_type h;
	return h(*val.get_pointer());
}

template<typename TypeInfo>
std::size_t hash_boost_cref_foo(const TypeInfo& val)
{
	return hash_foo(boost::cref(val));
}

template<typename TypeInfo>
std::size_t hash_boost_bton_foo(const TypeInfo& val)
{
	return hash_bton_foo(boost::cref(val));
}


template<typename T>
struct A
{
};

void test_native_cref(void)
{
	yggr_test_assert(hash_foo(typeid(int)));
	yggr_test_assert(hash_foo(typeid(float)));

	int a = 10;
	float b = 10;

	yggr_test_assert(hash_foo(typeid(int)) == hash_foo(typeid(a)));
	yggr_test_assert(hash_foo(typeid(int)) != hash_foo(typeid(b)));

	yggr_test_assert(hash_foo(typeid(float)) != hash_foo(typeid(a)));
	yggr_test_assert(hash_foo(typeid(float)) == hash_foo(typeid(b)));


	yggr_test_assert(hash_foo(typeid(A<int>)) == hash_foo(typeid(A<int>)));
	yggr_test_assert(hash_foo(typeid(A<float>)) == hash_foo(typeid(A<float>)));
	yggr_test_assert(hash_foo(typeid(A<int>)) != hash_foo(typeid(A<float>)));
}

void test_native_pointer(void)
{
	yggr_test_assert(hash_foo(&typeid(int)));
	yggr_test_assert(hash_foo(&typeid(float)));

	int a = 10;
	float b = 10;

	yggr_test_assert(hash_foo(&typeid(int)) == hash_foo(&typeid(a)));
	yggr_test_assert(hash_foo(&typeid(int)) != hash_foo(&typeid(b)));

	yggr_test_assert(hash_foo(&typeid(float)) != hash_foo(&typeid(a)));
	yggr_test_assert(hash_foo(&typeid(float)) == hash_foo(&typeid(b)));


	yggr_test_assert(hash_foo(&typeid(A<int>)) == hash_foo(&typeid(A<int>)));
	yggr_test_assert(hash_foo(&typeid(A<float>)) == hash_foo(&typeid(A<float>)));
	yggr_test_assert(hash_foo(&typeid(A<int>)) != hash_foo(&typeid(A<float>)));
}

void test_boost_cref(void)
{
	yggr_test_assert(hash_boost_cref_foo(typeid(int)));
	yggr_test_assert(hash_boost_cref_foo(typeid(float)));

	int a = 10;
	float b = 10;

	yggr_test_assert(hash_boost_cref_foo(typeid(int)) == hash_boost_cref_foo(typeid(a)));
	yggr_test_assert(hash_boost_cref_foo(typeid(int)) != hash_boost_cref_foo(typeid(b)));

	yggr_test_assert(hash_boost_cref_foo(typeid(float)) != hash_boost_cref_foo(typeid(a)));
	yggr_test_assert(hash_boost_cref_foo(typeid(float)) == hash_boost_cref_foo(typeid(b)));


	yggr_test_assert(hash_boost_cref_foo(typeid(A<int>)) == hash_boost_cref_foo(typeid(A<int>)));
	yggr_test_assert(hash_boost_cref_foo(typeid(A<float>)) == hash_boost_cref_foo(typeid(A<float>)));
	yggr_test_assert(hash_boost_cref_foo(typeid(A<int>)) != hash_boost_cref_foo(typeid(A<float>)));
}

void test_ntob_cref(void)
{
	yggr_test_assert(hash_ntob_foo(typeid(int)));
	yggr_test_assert(hash_ntob_foo(typeid(float)));

	int a = 10;
	float b = 10;

	yggr_test_assert(hash_ntob_foo(typeid(int)) == hash_ntob_foo(typeid(a)));
	yggr_test_assert(hash_ntob_foo(typeid(int)) != hash_ntob_foo(typeid(b)));

	yggr_test_assert(hash_ntob_foo(typeid(float)) != hash_ntob_foo(typeid(a)));
	yggr_test_assert(hash_ntob_foo(typeid(float)) == hash_ntob_foo(typeid(b)));


	yggr_test_assert(hash_ntob_foo(typeid(A<int>)) == hash_ntob_foo(typeid(A<int>)));
	yggr_test_assert(hash_ntob_foo(typeid(A<float>)) == hash_ntob_foo(typeid(A<float>)));
	yggr_test_assert(hash_ntob_foo(typeid(A<int>)) != hash_ntob_foo(typeid(A<float>)));
}

void test_bton_cref(void)
{
	yggr_test_assert(hash_boost_bton_foo(typeid(int)));
	yggr_test_assert(hash_boost_bton_foo(typeid(float)));

	int a = 10;
	float b = 10;

	yggr_test_assert(hash_boost_bton_foo(typeid(int)) == hash_boost_bton_foo(typeid(a)));
	yggr_test_assert(hash_boost_bton_foo(typeid(int)) != hash_boost_bton_foo(typeid(b)));

	yggr_test_assert(hash_boost_bton_foo(typeid(float)) != hash_boost_bton_foo(typeid(a)));
	yggr_test_assert(hash_boost_bton_foo(typeid(float)) == hash_boost_bton_foo(typeid(b)));


	yggr_test_assert(hash_boost_bton_foo(typeid(A<int>)) == hash_boost_bton_foo(typeid(A<int>)));
	yggr_test_assert(hash_boost_bton_foo(typeid(A<float>)) == hash_boost_bton_foo(typeid(A<float>)));
	yggr_test_assert(hash_boost_bton_foo(typeid(A<int>)) != hash_boost_bton_foo(typeid(A<float>)));
}

int main(int argc, char* argv[])
{
	test_native_cref();
	test_native_pointer();

	test_boost_cref();

	test_ntob_cref();
	test_bton_cref();

	wait_any_key(argc, argv);
	return 0;
}