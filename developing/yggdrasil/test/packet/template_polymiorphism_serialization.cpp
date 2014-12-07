//xml_packet_test.cpp

#error "this test is failed "

#include <iostream>
#include <yggr/base/yggrdef.h>
#include <yggr/archive/xml_archive_partner.hpp>
#include <yggr/packet/packet.hpp>
#include <yggr/serialization/nvp.hpp>

#include <yggr/serialization/shared_ptr.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>

// warning: mingw use libboost_serialization-mgw47-mt-1_53.a //fix xml_gamma_error

#ifdef _MSC_VER
#   include <vld.h>
#endif //_MSC_VER

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

//BOOST_SERIALIZATION_ASSUME_ABSTRACT(A)  
//
//template<typename T>
//struct B;

//namespace boost 
//{
//namespace serialization 
//{
//
//template<typename T>                                    
//struct guid_defined< B<T> > 
//	: boost::mpl::true_ {};
//
//namespace ext 
//{
//    template< typename T >
//    struct guid_impl< B<T> >
//    {
//		typedef B<T> type;
//        static inline const char * call()
//        {
//            return typeid(type).name();
//			//return "aaaaaaaaaaaaaa";
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
//template<typename T >                                               
//struct init_guid< B<T> > 
//{
//	typedef B<T> type;
//	typedef guid_initializer< type > guid_initializer_type;
//
//	static guid_initializer_type const & g;              
//};
//
//template<typename T>
//typename init_guid<  B<T> >::guid_initializer_type
//	const & init_guid< B<T> >::g =        
//		::boost::serialization::singleton< 
//				typename init_guid< B<T> >::guid_initializer_type                          
//			>::get_mutable_instance().export_guid();             
//
//} // namespace extra_detail
//} // namespace detail
//} // namespace archive
//} // namespace boost

template<typename T>
struct B
	: public A
{
	typedef T inner_type;
	typedef A base_type;

	B(void)
	{
	}

	B(inner_type n)
		: n1(n)
	{
	}

	virtual ~B(void)
	{
	}

	virtual inner_type get(void) const
	{
		return n1;
	}
private:
	friend class boost::serialization::access;
	template<typename Archive>
	void serialize(Archive& ar, const yggr::u32 version)
	{
		ar & YGGR_SERIALIZE_NAME_NVP("info", boost::serialization::base_object< base_type >(*this));
		ar & YGGR_SERIALIZE_NVP(n1);
	}

	inner_type n1;
};

//BOOST_CLASS_EXPORT_GUID(B, (typeid(B).name()))

//namespace boost 
//{
//namespace serialization 
//{
//
//template<typename T>                                    
//struct guid_defined< B<T> > 
//	: boost::mpl::true_ {};
//
//namespace ext 
//{
//    template< typename T >
//    struct guid_impl< B<T> >
//    {
//		typedef B<T> type;
//        static inline const char * call()
//        {
//            return typeid(type).name();
//			//return "aaaaaaaaaaaaaa";
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
//template<typename T >                                               
//struct init_guid< B<T> > 
//{
//	typedef B<T> type;
//	typedef guid_initializer< type > guid_initializer_type;
//
//	static guid_initializer_type const & g;              
//};
//
//template<typename T>
//typename init_guid<  B<T> >::guid_initializer_type
//	const & init_guid< B<T> >::g =        
//		::boost::serialization::singleton< 
//				typename init_guid< B<T> >::guid_initializer_type                          
//			>::get_mutable_instance().export_guid();             
//
//} // namespace extra_detail
//} // namespace detail
//} // namespace archive
//} // namespace boost

//namespace boost 
//{
//namespace serialization 
//{
//
//template<typename T, template<typename T> class Wrap>                                    
//struct guid_defined< Wrap<T> > 
//	: boost::mpl::true_ {};
//
//namespace ext 
//{
//    template< typename T, template<typename T> class Wrap >
//    struct guid_impl< Wrap<T> >
//    {
//		typedef Wrap<T> type;
//        static inline const char * call()
//        {
//            return typeid(type).name();
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
//template<typename T >                                               
//struct init_guid< B<T> > 
//{
//	typedef B<T> type;
//	typedef guid_initializer< type > guid_initializer_type;
//
//	static guid_initializer_type const & g;              
//};
//
//template<typename T>
//typename init_guid<  B< T > >::guid_initializer_type
//	const & init_guid< B<T> >::g =        
//		::boost::serialization::singleton< 
//				typename init_guid< B<T> >::guid_initializer_type                          
//			>::get_mutable_instance().export_guid();             
//
//} // namespace extra_detail
//} // namespace detail
//} // namespace archive
//} // namespace boost

typedef boost::shared_ptr<A> A_ptr_type;

void test_foo1(void)
{
	std::cout << (boost::serialization::ext::guid_impl< B<int> >::call()) << std::endl;

	//A_ptr_type p1(new B<int>(10));
	A* p1 = new B<int>(10);
	opak o;

	boost::serialization::nvp<A*> nvps("ptr", p1);
	o.save(nvps);

	std::cout << o.val_buf() << std::endl;

	//ipak i(o);
	//A_ptr_type p2;

	//boost::serialization::nvp<A_ptr_type> nvpl("ptr", p2);
	//i.load(nvpl);

	//std::cout << p2->get() << std::endl;
}



int main(int argc, char* argv[])
{
	test_foo1();

	char cc = 0;
	std::cin >> cc;
	return 0;
}
