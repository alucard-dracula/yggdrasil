// proxy_task_creator.hpp

#ifndef __YGGR_PROXY_PROXY_TASK_CREATOR_HPP__
#define __YGGR_PROXY_PROXY_TASK_CREATOR_HPP__

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <yggr/ppex/foo_params.hpp>

namespace yggr
{
namespace proxy
{

template<typename Task>
class proxy_task_creator
{
public:
	typedef Task task_type;

private:
	typedef proxy_task_creator this_type;

public:
	proxy_task_creator(void)
	{
	}

	~proxy_task_creator(void)
	{
	}

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template<YGGR_PP_FOO_TYPES_DEF(__n__) \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				typename Obj > \
	inline task_type& operator()(YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_REAL_CREF_PARAMS ) \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) Obj& obj) const { \
		BOOST_MPL_ASSERT((boost::is_same<task_type, Obj>)); \
		return obj; }

#define YGGR_PP_FOO_ARG_NAME(  ) init_arg
#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE(  )
#undef YGGR_PP_FOO_ARG_NAME

	inline task_type& operator()(task_type& task)
	{
		return task;
	}

};

} // namespace proxy
} // namespace yggr

#endif // __YGGR_PROXY_PROXY_TASK_CREATOR_HPP__