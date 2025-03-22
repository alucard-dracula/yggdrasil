//xml_packet_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/base/yggrdef.h>
#include <yggr/archive/xml_archive_partner.hpp>
#include <yggr/packet/packet.hpp>
#include <yggr/serialization/nvp.hpp>

#include <yggr/serialization/shared_ptr.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>

// warning: mingw use libboost_serialization-mgw47-mt-1_53.a //fix xml_gamma_error

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#   include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

typedef yggr::packet::packet<yggr::archive::archive_partner::xml_oarchive_partner> opak;
typedef yggr::packet::packet<yggr::archive::archive_partner::xml_iarchive_partner> ipak;


struct A
{
	//A(void) {}
	virtual ~A(void) {}

	virtual int get(void) const = 0;

private:
	friend class boost::serialization::access;
	template<typename Archive>
	void serialize(Archive& ar, const yggr::u32 version)
	{
	}
};

struct B
	: public A
{
	typedef A base_type;

	B(void)
	{
	}

	B(int n)
		: n1(n)
	{
	}

	virtual ~B(void)
	{
	}

	virtual int get(void) const
	{
		return n1;
	}
private:
	friend class boost::serialization::access;
	template<typename Archive>
	void serialize(Archive& ar, const yggr::u32 version)
	{
		ar & YGGR_SERIALIZATION_NAME_NVP("info", boost::serialization::base_object< base_type >(*this));
		ar & YGGR_SERIALIZATION_NVP(n1);
	}

	int n1;
};

BOOST_CLASS_EXPORT_GUID(B, (typeid(B).name()))

//namespace boost 
//{
//namespace serialization 
//{
//
//template<>                                    
//struct guid_defined< B > 
//	: boost::mpl::true_ {};
//
//namespace ext 
//{
//    template< >
//    struct guid_impl< B >
//    {
//        static inline const char * call()
//        {
//            return typeid(B).name();
//        }
//    };
//} // namespace ext
//
//} // namespace serialization
//} // namespace boost
//
//               
//namespace boost 
//{
//namespace archive 
//{
//namespace detail 
//{
//namespace extra_detail 
//{
//
//template<>                                               
//struct init_guid< B > 
//{
//	typedef guid_initializer< B > guid_initializer_type;
//
//	static guid_initializer_type const & g;              
//};
//
//init_guid<  B >::guid_initializer_type
//	const & init_guid< B >::g =        
//		::boost::serialization::singleton< 
//				init_guid<  B >::guid_initializer_type                          
//			>::get_mutable_instance().export_guid();             
//
//} // namespace extra_detail
//} // namespace detail
//} // namespace archive
//} // namespace boost

typedef yggr::shared_ptr<A> A_ptr_type;

void test_foo1(void)
{
	A_ptr_type p1(new B(10));
	opak o;

	o.save(YGGR_SERIALIZATION_NAME_NVP("ptr", p1));

	std::cout << o.val_buf() << std::endl;

	ipak i(o);
	A_ptr_type p2;

	i.load(YGGR_SERIALIZATION_NAME_NVP("ptr", p2));

	std::cout << p2->get() << std::endl;

	yggr_test_assert(p1->get() == p2->get());

}

int main(int argc, char* argv[])
{
	test_foo1();

	wait_any_key(argc, argv);
	return 0;
}
