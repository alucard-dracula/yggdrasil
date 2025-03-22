// patch_aligned_storage.hpp

#ifndef BOOST_CONTAINER_DETAIL_PATCH_ALIGNED_STORAGE_HPP
#define BOOST_CONTAINER_DETAIL_PATCH_ALIGNED_STORAGE_HPP

#ifndef BOOST_CONFIG_HPP
#  include <boost/config.hpp>
#endif // BOOST_CONFIG_HPP

#if defined(BOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif // BOOST_HAS_PRAGMA_ONCE

#include <boost/mpl/bool.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/container/detail/workaround.hpp>

namespace boost
{
namespace container 
{
namespace dtl 
{
namespace patch
{

template<typename Storage>
struct has_address_impl
{
private:
	typedef Storage storage_type; 
	typedef char true_type;
	typedef int false_type;

	template<typename T, void* (T::*)() const, typename Nil = void > struct check_t;
		
	template<typename T> 
	static true_type chk_foo(const check_t<T, &T::address, void>* ); 
		
	template<typename T> 
	static false_type chk_foo(...); 
		
	public: 
		typedef boost::mpl::bool_<(sizeof(true_type) == sizeof(chk_foo<storage_type>(0)))> type;
};

template<typename Storage>
struct has_address
	: public has_address_impl<Storage>::type
{
};

template<typename Value, typename Storage> BOOST_CONTAINER_FORCEINLINE
typename boost::enable_if<has_address<Storage>, Value*>::type
	aligned_storage_value_pointer(Storage& stor)
{
   typedef Value value_type;
   return reinterpret_cast<value_type*>(stor.address());
}

template<typename Value, typename Storage> BOOST_CONTAINER_FORCEINLINE
typename boost::disable_if<has_address<Storage>, Value*>::type
	aligned_storage_value_pointer(Storage& stor)
{
   typedef Value value_type;
   return reinterpret_cast<value_type*>(stor.data);
}

} // namespace patch
} // namespace dtl
} // namespace container
} // namespace boost

#endif // BOOST_CONTAINER_DETAIL_PATCH_ALIGNED_STORAGE_HPP