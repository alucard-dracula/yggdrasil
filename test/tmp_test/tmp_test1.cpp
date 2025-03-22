
#include <iostream>
#include <list>

#include <boost/preprocessor/arithmetic/sub.hpp>
#include <yggr/ppex/typedef.hpp>
#include <yggr/ppex/pp_debug.hpp>

#define YGGR_SERIALIZATION_BOOL_LINER_CONTINER_SAVE_LOAD( __template_n__, __type__ ) \
	template<typename Archive, YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB(__template_n__, 1), typename T)> \
	inline void save(Archive& ar, \
						const __type__<bool, YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB(__template_n__, 1), T)>& t, \
						const unsigned int) { \
		yggr::serialization::collection_size_type count(t.size()); \
		ar << BOOST_SERIALIZATION_NVP(count); \
		BOOST_DEDUCED_TYPENAME __type__< \
			bool, YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB(__template_n__, 1), T)>::const_iterator it = t.begin(); \
		while(count-- > 0) { \
			bool tb = *it++; \
			ar << boost::serialization::make_nvp("item", tb); } } \
	\
	template<typename Archive, YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB(__template_n__, 1), typename T)> \
	inline void load(Archive& ar, \
						__type__<bool, YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB(__template_n__, 1), T)>& t, \
						const unsigned int) { \
		yggr::serialization::collection_size_type count; \
		ar >> BOOST_SERIALIZATION_NVP(count); \
		t.clear(); \
		while(count-- > 0) { \
			bool i; \
			ar >> boost::serialization::make_nvp("item", i); \
			t.push_back(i); } } \
	\
	template<typename Archive, YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB(__template_n__, 1), typename T)> \
	inline void serialize(Archive& ar, \
							__type__<bool, YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB(__template_n__, 1), T)> & t, \
							const unsigned int file_version) { \
		boost::serialization::split_free(ar, t, file_version); }


template<typename Archive, typename T0> 
inline void save(Archive& ar, const std::list<bool, T0>& t, const unsigned int) 
{ 
	yggr::serialization::collection_size_type count(t.size()); 
	ar << BOOST_SERIALIZATION_NVP(count); 
	typename std::list<bool, T0>::const_iterator it = t.begin(); 
	while(count-- > 0) 
	{ 
		bool tb = *it++;
		ar << boost::serialization::make_nvp("item", tb); 
	} 
} 
template<typename Archive, typename T0> 
inline void load(Archive& ar, std::list<bool, T0>& t, const unsigned int) 
{ 
	yggr::serialization::collection_size_type count; 
	ar >> BOOST_SERIALIZATION_NVP(count); 
	t.clear(); 
	while(count-- > 0) 
	{ 
		bool i; ar >> boost::serialization::make_nvp("item", i); 
		t.push_back(i); 
	} 
} 

template<typename Archive, typename T0> 
inline void serialize(Archive& ar, std::list<bool, T0> & t, const unsigned int file_version) 
{ 
	boost::serialization::split_free(ar, t, file_version); 
}


int main(void)
{

	std::cout << YGGR_PP_DEBUG(YGGR_SERIALIZATION_BOOL_LINER_CONTINER_SAVE_LOAD(2, std::list)) << std::endl;

	char cc = 0;
	std::cin >> cc;
	return 0;
}