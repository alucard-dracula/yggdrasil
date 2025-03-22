//string_serialization_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/nsql_database_system/bson_native_ex.hpp>

#include <boost/type_traits/alignment_of.hpp>
#include <boost/type_traits/aligned_storage.hpp>

#if defined(_MSC_VER)
#	include <vld.h>
#endif // _MSC_VER

BSON_ALIGNED_BEGIN (128)
typedef struct
{
	bson_flags_t		flags;            /* flags describing the bson_t */
	uint32_t			len;              /* length of bson document in bytes */
	bson_t*				parent;           /* parent bson if a child */
	uint32_t			depth;            /* Subdocument depth. */
	uint8_t**			buf;              /* pointer to buffer pointer */
	size_t*				buflen;           /* pointer to buffer length */
	size_t				offset;           /* our offset inside *buf  */
	uint8_t*			alloc;            /* buffer that we own. */
	size_t				alloclen;         /* length of buffer that we own. */
	bson_realloc_func	realloc;          /* our realloc implementation */
	void*				realloc_func_ctx; /* context for our realloc func */
	bson_t*				pchild;			  // append pchild pointer
} inner_bson_impl_alloc_t BSON_ALIGNED_END (128);

//BSON_ALIGNED_BEGIN (4)
typedef struct {char a; uint32_t b;} test_alloc_t
/*BSON_ALIGNED_END (4)*/;

int main(int argc, char* argv[])
{

    std::cout
        << sizeof(bson_impl_alloc_t)
        << "," << (boost::alignment_of<bson_impl_alloc_t>::value)
        << "," << sizeof(boost::aligned_storage<sizeof(bson_impl_alloc_t), boost::alignment_of<bson_impl_alloc_t>::value>::type)
        << std::endl;
	std::cout
        << sizeof(inner_bson_impl_alloc_t)
        << "," << (boost::alignment_of<inner_bson_impl_alloc_t>::value)
        << "," << sizeof(boost::aligned_storage<sizeof(inner_bson_impl_alloc_t), boost::alignment_of<inner_bson_impl_alloc_t>::value>)
        << std::endl;
	std::cout
        << sizeof(bson_t)
        << "," << (boost::alignment_of<bson_t>::value)
        << "," << sizeof(boost::aligned_storage<sizeof(bson_t), boost::alignment_of<bson_t>::value>)
        << std::endl;

    std::cout
        << sizeof(test_alloc_t)
        << "," << (boost::alignment_of<test_alloc_t>::value)
        << "," << sizeof(boost::aligned_storage<sizeof(test_alloc_t), boost::alignment_of<test_alloc_t>::value>)
        << std::endl;

	{
		bson_impl_alloc_t t = {static_cast<bson_flags_t>(0)};
	    std::cout << sizeof(t) << std::endl;
	}

	{
	    bson_impl_alloc_t t = {static_cast<bson_flags_t>(0)};
	    bson_impl_alloc_t& tref = t;
	    std::cout << sizeof(tref) << std::endl;
	}

    bson_t* porg = new bson_t();
    std::cout << "porg is: " << porg << std::endl;
    inner_bson_impl_alloc_t* pdst = reinterpret_cast<inner_bson_impl_alloc_t*>(porg);
    memset(porg, 0, sizeof(boost::aligned_storage<sizeof(bson_t), boost::alignment_of<bson_t>::value>));

    pdst->pchild = reinterpret_cast<bson_t*>(porg);

    std::cout << "pchild is: " << pdst->pchild << std::endl;

    delete porg;

	wait_any_key(argc, argv);
	return 0;
}
