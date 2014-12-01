// static_index_mapping.hpp

#ifndef __YGGR_MAPPING_STATIC_INDEX_MAPPING_HPP__
#define __YGGR_MAPPING_STATIC_INDEX_MAPPING_HPP__

#include <cassert>
#include <iterator>

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/preprocessor/cat.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/ppex/symbols.hpp>

namespace yggr
{
namespace mapping
{

template<typename Tag>
struct static_index_mapping;

template<typename Tag>
struct mapping_indexes;

template<typename Tag>
struct mapping_values;

} // namespace mapping
} // namespace yggr

// indexes def
#define YGGR_PP_STATIC_MAPPING_INDEXES_BEGIN( __tag__ ) \
	namespace yggr { namespace mapping { \
	template<> struct mapping_indexes< __tag__ > { \
	public: \
		typedef __tag__ tag_type; \
		enum { 

#define YGGR_PP_STATIC_MAPPING_INDEX( __name__ ) \
	BOOST_PP_CAT( E_, __name__ ) YGGR_PP_SYMBOL_COMMA() 

#define YGGR_PP_STATIC_MAPPING_INDEXES_END() E_size YGGR_PP_SYMBOL_COMMA() E_compile_u32 = 0xffffffff }; }; } }

#define YGGR_PP_GET_STATIC_MAPPING_INDEXES( __tag__ ) \
	yggr::mapping::mapping_indexes< __tag__ >

// values def
#define YGGR_PP_STATIC_MAPPING_VALUES_DECLEAR( __tag__, __val_type__ ) \
	namespace yggr { namespace mapping { \
	template<> struct mapping_values < __tag__ > { \
	public: \
		typedef __tag__ tag_type; \
		typedef __val_type__ value_type; \
	public: \
		static const value_type values[]; };

#define YGGR_PP_STATIC_MAPPING_VALUES_BEGIN_IMPL(__tag__) \
	namespace yggr { namespace mapping { \
	const mapping_values< __tag__ >::value_type mapping_values< __tag__ >::values[] = {

#define YGGR_PP_STATIC_MAPPING_VALUES_BEGIN( __tag__, __val_type__ ) \
	namespace yggr { namespace mapping { \
	template<> struct mapping_values < __tag__ > { \
	public: \
		typedef __tag__ tag_type; \
		typedef __val_type__ value_type; \
	public: \
		static const value_type values[]; }; \
	const mapping_values< __tag__ >::value_type mapping_values< __tag__ >::values[] = { \

#define YGGR_PP_STATIC_MAPPING_VALUE( __val__ ) __val__ YGGR_PP_SYMBOL_COMMA()

#define YGGR_PP_STATIC_MAPPING_VALUES_END() }; } }

#define YGGR_PP_GET_STATIC_MAPPING_VALUES( __tag__ ) \
	yggr::mapping::mapping_values< __tag__ >

// static_index_mapping def
#define YGGR_PP_STATIC_MAPPING_MAKE( __indexes__, __values__ ) \
	namespace yggr { namespace mapping { \
	template<> struct static_index_mapping< __indexes__::tag_type >{ \
	public: \
		typedef __indexes__ indexes_type; \
		typedef __values__ values_type; \
		typedef values_type::value_type value_type; \
		typedef value_type& reference; \
		typedef const value_type& const_reference; \
		typedef value_type* pointer; \
		typedef const value_type* const_pointer; \
		typedef const_pointer iterator; \
		typedef iterator const_iterator; \
		typedef std::reverse_iterator<iterator> reverse_iterator; \
		typedef std::reverse_iterator<const_iterator> const_reverse_iterator; \
		typedef std::size_t difference_type; \
		typedef std::size_t size_type; \
	private: \
		typedef indexes_type::tag_type idx_tag_type; \
		typedef values_type::tag_type val_tag_type; \
		BOOST_MPL_ASSERT((boost::is_same<idx_tag_type, val_tag_type>)); \
	private: \
		typedef static_index_mapping this_type; \
	public: \
		inline static const const_iterator begin(void) { \
			return  values_type::values; } \
		\
		inline static const const_iterator end(void) { \
			return (values_type::values + static_cast<yggr::u32>(indexes_type::E_size)); } \
		\
		inline static const const_reverse_iterator rbegin(void) { \
			return const_reverse_iterator(values_type::values + static_cast<yggr::u32>(indexes_type::E_size)); } \
		\
		inline static const const_reverse_iterator rend(void) { \
			return const_reverse_iterator(values_type::values); } \
		\
		inline static size_type size(void) { \
			return indexes_type::E_size; } \
		\
		inline static bool empty(void) { \
			return !indexes_type::E_size; } \
		\
		inline static const value_type& at(yggr::u32 idx) { \
			assert(idx < indexes_type::E_size); \
			return values_type::values[idx]; } \
		\
		inline static const value_type& front(void) { \
			assert(indexes_type::E_size); \
			return values_type::values[0]; } \
		\
		inline static const value_type& back(void) { \
			assert(indexes_type::E_size); \
			return values_type::values[indexes_type::E_size - 1]; } }; } }

#define YGGR_PP_GET_STATIC_MAPPING( __tag__ ) \
	yggr::mapping::static_index_mapping< __tag__ > 

#define YGGR_PP_GET_STATIC_MAPPING_INDEX( __tag__, __name__ ) \
	YGGR_PP_GET_STATIC_MAPPING( __tag__ )::indexes_type::BOOST_PP_CAT( E_, __name__ )

#define YGGR_PP_GET_STATIC_MAPPING_SIZE( __tag__ ) \
	YGGR_PP_GET_STATIC_MAPPING( __tag__ )::indexes_type::E_size

#define YGGR_PP_GET_STATIC_MAPPING_VALUE( __tag__, __name__) \
	YGGR_PP_GET_STATIC_MAPPING( __tag__ )::values_type::values[YGGR_PP_GET_STATIC_MAPPING_INDEX(__tag__, __name__)]


#endif // __YGGR_MAPPING_STATIC_INDEX_MAPPING_HPP__