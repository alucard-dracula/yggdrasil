// test_packet_info_caster.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/base/yggrdef.h>

#include <yggr/packet/packet_info.hpp>
#include <yggr/packet/packet_info_caster.hpp>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <typeinfo>

#if defined(_MSC_VER)
#   include <vld.h>
#endif // _MSC_VER

int main(int argc, char* argv[])
{
    {
        typedef boost::mpl::vector<int>::type vt_type;
        typedef yggr::packet::packet_info<int> chk_type;

		typedef yggr::packet::packet_info_caster<vt_type> caster_type;
		typedef caster_type::type packet_info_type;
		
		std::cout << typeid(vt_type).name() << std::endl;
		std::cout << typeid(packet_info_type).name() << std::endl;
		std::cout << typeid(chk_type).name() << std::endl;

        BOOST_MPL_ASSERT((boost::is_same<chk_type, packet_info_type>));
    }

	{
		 typedef boost::mpl::vector<>::type vt_type;
		 std::cout << typeid(vt_type).name() << std::endl;
	}

	{
		 typedef boost::mpl::vector<>::type::type vt_type;
		 std::cout << typeid(vt_type).name() << std::endl;
	}


    std::cout << "test end" << std::endl;

	wait_any_key(argc, argv);
    return 0;
}
