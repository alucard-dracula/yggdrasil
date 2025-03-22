//boost_midx_keyinfo_helper.hpp (container)

/****************************************************************************
Copyright (c) 2010-2024 yggdrasil

author: xu yang

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef __YGGR_CONTAINER_DETAIL_BOOST_MIDX_KEYINFO_HELPER_HPP__
#define __YGGR_CONTAINER_DETAIL_BOOST_MIDX_KEYINFO_HELPER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>
#include <yggr/move/move.hpp>

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES
#	include <yggr/type_traits/integer_sequence.hpp>
#else
#	include <yggr/ppex/typedef.hpp>
#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>

#include <boost/ref.hpp>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/copy_if.hpp>
#include <boost/mpl/back_inserter.hpp>

#include <boost/fusion/container/map/map.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/at_key.hpp>
#include <boost/fusion/include/find_if.hpp>
#include <boost/fusion/include/copy.hpp>
#include <boost/fusion/include/swap.hpp>
#include <boost/fusion/include/size.hpp>

#if (BOOST_VERSION >= 105800) && (defined(_MSC_VER) || (YGGR_CPP_VER_11 <= YGGR_CPP_VERSION))
#	include <boost/fusion/include/move.hpp>
#endif // #if (BOOST_VERSION >= 105800) && (defined(_MSC_VER) || (YGGR_CPP_VER_11 <= YGGR_CPP_VERSION))

#include <utility>

namespace yggr
{
namespace container
{
namespace detail
{

template<typename Tag_List>
struct boost_midx_get_tag;

template<typename Index>
struct boost_midx_keyinfo_meta_pair;

// make_keyhash_meta_map decl
template<typename T, typename IndexSpecifierList>
struct make_boost_midx_keyinfo_meta;

//boost_midx_keyinfo_makers decl
template<typename T, typename Meta_Map>
struct boost_midx_keyinfo_makers;

//boost_midx_keyinfo_params decl
template<typename T, typename Meta_Map>
struct boost_midx_keyinfo_params;

} // namespace detail
} // namespace container
} // namespace yggr

namespace yggr
{
namespace container
{
namespace detail
{

template<typename Tag_List>
struct boost_midx_get_tag
	: public
		boost::mpl::if_c
		<
			!!boost::mpl::size<Tag_List>::value,
			typename boost::mpl::at_c<Tag_List, 0>::type,
			boost::mpl::na
		>
{
};

template<typename Index>
struct boost_midx_keyinfo_meta_pair
	: public boost::mpl::false_
{
private:
	typedef boost::mpl::na tag_type;
	typedef boost::fusion::void_ key_from_value_type;
	typedef boost::fusion::void_ hash_type;
public:
	typedef
		boost::mpl::pair
		<
			boost::fusion::pair<tag_type, std::pair<hash_type, key_from_value_type> >,
			boost::fusion::pair<tag_type, std::size_t >
		> value_type;
};

#define YGGR_PP_CONTAINER_DETAIL_KEYHASH_META_PAIR_DEF( __sp__ ) \
	namespace yggr { namespace container { namespace detail { \
		template<typename A1, typename A2, typename A3, typename A4> \
		struct boost_midx_keyinfo_meta_pair< __sp__<A1, A2, A3, A4> > \
			: public boost::mpl::true_ { \
		private: \
			typedef __sp__<A1, A2, A3, A4> index_type; \
			typedef typename index_type::tag_list_type tag_list_type; \
			typedef typename boost_midx_get_tag<tag_list_type>::type tag_type; \
			typedef typename index_type::key_from_value_type key_from_value_type; \
			typedef typename index_type::hash_type hash_type; \
		public: \
			typedef \
				boost::mpl::pair< \
					boost::fusion::pair<tag_type, std::pair<hash_type, key_from_value_type> >, \
					boost::fusion::pair<tag_type, std::size_t > > value_type;  }; }}}

// make_meta_map_helper
#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES
//std::size_t ...I
template<typename Pair_Vt, typename Indexes>
struct detail_make_meta_map_helper;

template<typename Pair_Vt, std::size_t ...I>
struct detail_make_meta_map_helper<Pair_Vt, ::yggr::index_sequence<I...> >
{
protected:
	typedef Pair_Vt now_pair_vt;

protected:
	typedef
		boost::fusion::map
		<
			typename
				boost_midx_keyinfo_meta_pair
				<
					typename boost::mpl::at_c<now_pair_vt, I>::type
				>::value_type::first...
		> maker_map_type;

	typedef
		boost::fusion::map
		<
			typename
				boost_midx_keyinfo_meta_pair
				<
					typename boost::mpl::at_c<now_pair_vt, I>::type
				>::value_type::second...
		> param_map_type;
};

template<typename Pair_Vt,
			std::size_t N = boost::mpl::size<Pair_Vt>::value >
struct make_meta_map_helper
	: protected detail_make_meta_map_helper<Pair_Vt, ::yggr::make_index_sequence<N> >
{
private:
	typedef detail_make_meta_map_helper<Pair_Vt, ::yggr::make_index_sequence<N> > base_type;

public:
	typedef typename base_type::maker_map_type maker_map_type;
	typedef typename base_type::param_map_type param_map_type;
};

#else

template<typename Pair_Vt,
			std::size_t N = boost::mpl::size<Pair_Vt>::value >
struct make_meta_map_helper;

#	define YGGR_TMP_PP_PAIR_VT_NAME() now_pair_vt

#	define YGGR_TMP_PP_KETHASH_META_HASHER( __z__, __n__, __op__ ) \
				YGGR_PP_SYMBOL_IF( __n__, __op__ ) \
				typename boost_midx_keyinfo_meta_pair< \
					typename boost::mpl::at_c< \
						YGGR_TMP_PP_PAIR_VT_NAME(), __n__ >::type >::value_type::first

#	define YGGR_TMP_PP_KETHASH_META_PARAM( __z__, __n__, __op__ ) \
				YGGR_PP_SYMBOL_IF( __n__, __op__ ) \
				typename boost_midx_keyinfo_meta_pair< \
					typename boost::mpl::at_c< \
						YGGR_TMP_PP_PAIR_VT_NAME(), __n__ >::type >::value_type::second


#	define YGGR_TMP_MAKE_MAP_HELPER( __n__ ) \
		template< typename Pair_Vt > \
		struct make_meta_map_helper<Pair_Vt, __n__ > { \
		private: \
			typedef Pair_Vt YGGR_TMP_PP_PAIR_VT_NAME(); \
		public: \
			typedef boost::fusion::map< \
				YGGR_PP_REPEAT_CUSTOM_OP_NOW(__n__, YGGR_TMP_PP_KETHASH_META_HASHER, YGGR_PP_SYMBOL_COMMA) \
			> maker_map_type; \
			\
			typedef boost::fusion::map< \
				YGGR_PP_REPEAT_CUSTOM_OP_NOW(__n__, YGGR_TMP_PP_KETHASH_META_PARAM, YGGR_PP_SYMBOL_COMMA) \
			> param_map_type; };

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		YGGR_TMP_MAKE_MAP_HELPER( __n__ )

#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_TEMPLATE_PARAMS_LEN() )
#	include BOOST_PP_LOCAL_ITERATE(  )

#	undef YGGR_TMP_MAKE_MAP_HELPER
#	undef YGGR_TMP_PP_KETHASH_META_HASHER
#	undef YGGR_TMP_PP_KETHASH_META_PARAM
#	undef YGGR_TMP_PP_PAIR_VT_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

// handler_make_makers
template<typename Container>
struct handler_make_makers
{
public:
	typedef Container cont_type;

private:
	typedef handler_make_makers this_type;

public:
	handler_make_makers(const cont_type& cont)
		: _cont(cont)
	{
	}

public:
	template<typename MME> inline
	void operator()(MME& mme) const
	{
		typedef MME mme_type;
		typedef typename
			boost::fusion::result_of::first<mme_type>::type tag_type;
		typedef typename
			boost::multi_index::index<cont_type, tag_type>::type now_cont_type;

		const now_cont_type& now_cont = _cont.template get<tag_type>();
		mme.second.first = now_cont.hash_function();
		mme.second.second = now_cont.key_extractor();
	}

private:
	const cont_type& _cont;
};

// make_makers_helper
template<typename MetaMap,
			std::size_t = boost::fusion::result_of::size<MetaMap>::value >
struct make_makers_helper
{
public:
	typedef MetaMap meta_map_type;

private:
	typedef make_makers_helper this_type;

public:
	template<typename Container> inline
	meta_map_type& operator()(meta_map_type& meta_map, const Container& cont) const
	{
		typedef Container cont_type;
		typedef handler_make_makers<cont_type> foo_type;

		foo_type foo(cont);

		boost::fusion::for_each(meta_map, foo);
		return meta_map;
	}
};

// make_boost_midx_keyinfo_meta
template<typename T, typename IndexSpecifierList>
struct make_boost_midx_keyinfo_meta
{
private:
	typedef IndexSpecifierList isl_type;

	typedef typename
		boost::mpl::copy_if
		<
			isl_type,
			boost_midx_keyinfo_meta_pair<boost::mpl::_>,
			boost::mpl::back_inserter< boost::mpl::vector<> >
		>::type pair_vt_type;

	// make_map_helper
	typedef make_meta_map_helper<pair_vt_type> make_meta_map_helper_type;
	typedef typename make_meta_map_helper_type::maker_map_type maker_map_type;
	typedef typename make_meta_map_helper_type::param_map_type param_map_type;

	BOOST_MPL_ASSERT((
		boost::mpl::bool_
		<
			boost::fusion::result_of::size<maker_map_type>::value
				== boost::fusion::result_of::size<param_map_type>::value
		>
	));

public:
	typedef T value_type;
	typedef boost_midx_keyinfo_makers<value_type, maker_map_type> makers_type;
	typedef boost_midx_keyinfo_params<value_type, param_map_type> params_type;
};

// foo_get_helper
template<typename BOOL_T>
struct boost_midx_keyinfo_makers_foo_get_helper;

template<>
struct boost_midx_keyinfo_makers_foo_get_helper<boost::mpl::true_>
{
	template<typename Iter> inline
	boost::fusion::void_ operator()(const Iter&) const
	{
		return boost::fusion::void_();
	}
};

template<>
struct boost_midx_keyinfo_makers_foo_get_helper<boost::mpl::false_>
{
	template<typename Iter> inline
	typename boost::fusion::result_of::value_of_data<Iter>::type&
		operator()(Iter& i)
	{
		return (*i).second;
	}

	template<typename Iter> inline
	const typename boost::fusion::result_of::value_of_data<Iter>::type&
		operator()(Iter& i) const
	{
		return (*i).second;
	}
};

//boost_midx_keyinfo_makers impl
template<typename T, typename Meta_Map>
struct boost_midx_keyinfo_makers
{
private:

	// the meta_map is
	//fusion::map
	//<
	//	boost::fusion::pair<tag_type, std::pair<hash_type, key_from_value_type>,
	//	....
	//>

	typedef Meta_Map meta_map_type;

public:
	typedef T value_type;

public:
	YGGR_STATIC_CONSTANT(std::size_t, E_size = boost::fusion::result_of::size<meta_map_type>::type::value);

private:
	typedef boost_midx_keyinfo_makers this_type;

public:
	template<typename V, typename I, typename A>
	boost_midx_keyinfo_makers(const boost::multi_index::multi_index_container<V, I, A>& cont)
	{
		typedef make_makers_helper<meta_map_type> h_type;
		h_type h;
		h(_meta_map, cont);
	}

	boost_midx_keyinfo_makers(BOOST_RV_REF(this_type) right)
		: _meta_map(boost::move(right._meta_map))
	{
	}

	boost_midx_keyinfo_makers(const this_type& right)
		: _meta_map(right._meta_map)
	{
	}

	~boost_midx_keyinfo_makers(void)
	{
	}

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		if(this == &right_ref)
		{
			return *this;
		}
#if (!defined(YGGR_NO_CXX11_RVALUE_REFERENCES)) && (BOOST_VERSION >= 105800)
		boost::fusion::move(boost::move(right._meta_map), _meta_map);
#else
		boost::fusion::swap(right_ref._meta_map, _meta_map);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

		return *this;
	}

	inline this_type& operator=(const this_type& right)
	{
		if(this == boost::addressof(right))
		{
			return *this;
		}

		boost::fusion::copy(_meta_map, right._meta_map);
		return *this;
	}

public:
	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	inline void swap(this_type& right)
	{
		if(this == boost::addressof(right))
		{
			return;
		}

		boost::fusion::swap(right._meta_map, _meta_map);
	}

public:
	inline static std::size_t size(void)
	{
		return this_type::E_size;
	}

public:
	template<typename Tag> inline
	typename
		boost::mpl::if_
		<
			typename
				boost::is_same
				<
					typename boost::fusion::result_of::find<meta_map_type, Tag>::type,
					typename boost::fusion::result_of::end<meta_map_type>::type
				>::type,
			boost::fusion::void_,
			typename
				boost::fusion::result_of::value_of_data
				<
					typename boost::fusion::result_of::find<meta_map_type, Tag>::type
				>::type
		>::type&
		get(void)
	{
		typedef Tag tag_type;
		typedef typename
			boost::is_same
			<
				typename boost::fusion::result_of::find<meta_map_type, tag_type>::type,
				typename boost::fusion::result_of::end<meta_map_type>::type
			>::type chk_type;
		typedef boost::mpl::bool_<chk_type::value> case_type;
		typedef boost_midx_keyinfo_makers_foo_get_helper<case_type> helper_type;

		helper_type h;
		return h(boost::fusion::find<tag_type>(_meta_map));
	}

	template<typename Tag> inline
	const typename
		boost::mpl::if_
		<
			typename
				boost::is_same
				<
					typename boost::fusion::result_of::find<meta_map_type, Tag>::type,
					typename boost::fusion::result_of::end<meta_map_type>::type
				>::type,
			boost::fusion::void_,
			typename
				boost::fusion::result_of::value_of_data
				<
					typename boost::fusion::result_of::find<meta_map_type, Tag>::type
				>::type
		>::type&
		get(void) const
	{
		typedef Tag tag_type;
		typedef typename
			boost::is_same
			<
				typename boost::fusion::result_of::find<meta_map_type, tag_type>::type,
				typename boost::fusion::result_of::end<meta_map_type>::type
			>::type chk_type;
		typedef boost::mpl::bool_<chk_type::value> case_type;
		typedef boost_midx_keyinfo_makers_foo_get_helper<case_type> helper_type;

		const helper_type h;
		return h(boost::fusion::find<tag_type>(_meta_map));
	}

private:
	meta_map_type _meta_map;
};

template<typename Value, typename Makers>
struct boost_midx_keyinfo_params_init_foo_t
{
public:
	typedef Value value_type;
	typedef Makers makers_type;

private:
	typedef boost_midx_keyinfo_params_init_foo_t this_type;

public:
	boost_midx_keyinfo_params_init_foo_t(const value_type& val,
											const makers_type& makers)
		: _val_ref(val), _makers(makers)
	{
	}

public:
	template<typename MME> inline
	void operator()(MME& mme) const
	{
		typedef MME mme_type;
		typedef typename boost::fusion::result_of::first<mme_type>::type now_tag_type;

		hash_detail(mme, _makers.template get<now_tag_type>());
	}

private:
	template<typename MME, typename Hash_Pair> inline
	void hash_detail(MME& mme, const Hash_Pair& hash_pair) const
	{
		mme.second = hash_pair.first(hash_pair.second(_val_ref));
	}

private:
	const value_type& _val_ref;
	const makers_type& _makers;
};

// boost_midx_keyinfo_params
template<typename BOOL_T>
struct boost_midx_keyinfo_params_foo_get_helper;

template< >
struct boost_midx_keyinfo_params_foo_get_helper<boost::mpl::true_>
{
	template<typename Iter> inline
	boost::fusion::void_ operator()(const Iter&) const
	{
		return boost::fusion::void_();
	}
};

template< >
struct boost_midx_keyinfo_params_foo_get_helper<boost::mpl::false_>
{
	template<typename Iter> inline
	std::size_t operator()(Iter& i) const
	{
		return (*i).second;
	}
};

// boost_midx_keyinfo_params impl
template<typename T, typename Meta_Map>
struct boost_midx_keyinfo_params
{
private:

	//the meta_map is
	//fusion::map
	//<
	//	boost::fusion::pair<tag_type, std::size_t > ,
	//	// ...
	//>

	typedef Meta_Map meta_map_type;

public:
	typedef T value_type;

public:
	YGGR_STATIC_CONSTANT(std::size_t, E_size = boost::fusion::result_of::size<meta_map_type>::type::value);

private:
	typedef boost_midx_keyinfo_params this_type;

public:
	template<typename Makers_Meta_Map> inline
	explicit boost_midx_keyinfo_params(const value_type& val,
										const boost_midx_keyinfo_makers
												<
													value_type,
													Makers_Meta_Map
												>& hashers)
	{
		typedef
			boost_midx_keyinfo_makers
			<
				value_type,
				Makers_Meta_Map
			> makers_type;

		typedef boost_midx_keyinfo_params_init_foo_t<value_type, makers_type> init_foo_type;

		init_foo_type foo(val, hashers);
		boost::fusion::for_each(_meta_map, foo);
	}

	boost_midx_keyinfo_params(BOOST_RV_REF(meta_map_type) right)
		: _meta_map(boost::move(right))
	{
	}

//#if !defined(YGGR_NO_CXX11_RVALUE_REFERENCES)
//	boost_midx_keyinfo_params(BOOST_RV_REF(meta_map_type) right)
//		: _meta_map(right)
//	{
//	}
//#else
//	boost_midx_keyinfo_params(BOOST_RV_REF(meta_map_type) right)
//	{
//		meta_map_type& right_ref = right;
//		boost::fusion::swap(right, _meta_map);
//	}
//#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

	boost_midx_keyinfo_params(const meta_map_type& right)
		: _meta_map(right)
	{
	}

	boost_midx_keyinfo_params(BOOST_RV_REF(this_type) right)
		: _meta_map(boost::move(right._meta_map))
	{
	}

	boost_midx_keyinfo_params(const this_type& right)
		: _meta_map(right._meta_map)
	{
	}

	~boost_midx_keyinfo_params(void)
	{
	}

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		if(this == boost::addressof(right_ref))
		{
			return *this;
		}
#if (!defined(YGGR_NO_CXX11_RVALUE_REFERENCES)) && (BOOST_VERSION >= 105800)
		boost::fusion::move(boost::move(right._meta_map), _meta_map);
#else
		boost::fusion::swap(right_ref._meta_map, _meta_map);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

		return *this;
	}

	inline this_type& operator=(const this_type& right)
	{
		if(this == boost::addressof(right))
		{
			return *this;
		}

		boost::fusion::copy(right._meta_map, _meta_map);
		return *this;
	}

public:
	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	inline void swap(this_type& right)
	{
		if(this == boost::addressof(right))
		{
			return;
		}

		boost::fusion::swap(right._meta_map, _meta_map);
	}

public:
	inline static std::size_t size(void)
	{
		return this_type::E_size;
	}

public:
	template<typename Tag> inline
	typename
		boost::mpl::if_
		<
			typename
				boost::is_same
				<
					typename boost::fusion::result_of::find<meta_map_type, Tag>::type,
					typename boost::fusion::result_of::end<meta_map_type>::type
				>::type,
			boost::fusion::void_,
			typename
				boost::fusion::result_of::value_of_data
				<
					typename boost::fusion::result_of::find<meta_map_type, Tag>::type
				>::type
		>::type
		get(void)
	{
		typedef Tag tag_type;
		typedef typename
			boost::is_same
			<
				typename boost::fusion::result_of::find<meta_map_type, tag_type>::type,
				typename boost::fusion::result_of::end<meta_map_type>::type
			>::type chk_type;
		typedef boost::mpl::bool_<chk_type::value> case_type;
		typedef boost_midx_keyinfo_params_foo_get_helper<case_type> helper_type;

		helper_type h;
		return h(boost::fusion::find<tag_type>(_meta_map));
	}

	template<typename Tag> inline
	typename
		boost::mpl::if_
		<
			typename
				boost::is_same
				<
					typename boost::fusion::result_of::find<meta_map_type, Tag>::type,
					typename boost::fusion::result_of::end<meta_map_type>::type
				>::type,
			boost::fusion::void_,
			typename
				boost::fusion::result_of::value_of_data
				<
					typename boost::fusion::result_of::find<meta_map_type, Tag>::type
				>::type
		>::type
		get(void) const
	{
		typedef Tag tag_type;
		typedef typename
			boost::is_same
			<
				typename boost::fusion::result_of::find<meta_map_type, tag_type>::type,
				typename boost::fusion::result_of::end<meta_map_type>::type
			>::type chk_type;
		typedef boost::mpl::bool_<chk_type::value> case_type;
		typedef boost_midx_keyinfo_params_foo_get_helper<case_type> helper_type;

		helper_type h;
		return h(boost::fusion::find<tag_type>(_meta_map));
	}

private:
	meta_map_type _meta_map;
};

} // namespace detail
} // namespace container
} // namespace yggr

namespace yggr
{
namespace container
{
namespace detail
{
namespace swap_support
{

template<typename T, typename Meta_Map> inline
void swap(boost_midx_keyinfo_makers<T, Meta_Map>& l,
			boost_midx_keyinfo_makers<T, Meta_Map>& r)
{
	l.swap(r);
}

template<typename T, typename Meta_Map> inline
void swap(boost_midx_keyinfo_params<T, Meta_Map>& l,
			boost_midx_keyinfo_params<T, Meta_Map>& r)
{
	l.swap(r);
}

} // namespace swap_support

using swap_support::swap;

} // namespace detail
} // namespace container
} // namespace yggr

namespace std
{
	using ::yggr::container::detail::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::container::detail::swap_support::swap;
} // namespace boost

#endif // __YGGR_CONTAINER_DETAIL_BOOST_MIDX_KEYINFO_HELPER_HPP__
