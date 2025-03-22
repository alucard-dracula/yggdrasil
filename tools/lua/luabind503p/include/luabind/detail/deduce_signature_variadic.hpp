// Copyright Daniel Wallin 2008. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


# ifndef LUABIND_DEDUCE_SIGNATURE_VARIADIC_080911_HPP
#  define LUABIND_DEDUCE_SIGNATURE_VARIADIC_080911_HPP

#  include <luabind/detail/most_derived.hpp>
#  include <luabind/detail/variadic_to_mpl.hpp>

#  if LUABIND_MAX_ARITY <= 8
#   include <boost/mpl/vector/vector10.hpp>
#  else
#   include <boost/mpl/vector/vector50.hpp>
#  endif
#  include <boost/preprocessor/cat.hpp>
#  include <boost/preprocessor/iteration/iterate.hpp>
#  include <boost/preprocessor/repetition/enum_params.hpp>

namespace luabind { namespace detail {

namespace mpl = boost::mpl;


//no matching function for call to 'deduce_signature(int (CDialogHelper::* const&)()const, CDialogHelper*)'

// aton besoin des ",..." ici ?
// C function pointer
template <class R,class... Args>
typename mplVectorFromVariadic<R,Args...>::type
deduce_signature( R(*)(Args...), ... )
{
    //signature();
    return typename mplVectorFromVariadic<R,Args...>::type();
}

// Member function pointer
template <class R, class T, class... Args>
typename mplVectorFromVariadic<R,T&,Args...>::type
deduce_signature( R(T::*)( Args...) )
{
    return typename mplVectorFromVariadic<R,T&,Args...>::type();
}


// Member function const version
template <class R, class T, class... Args>
typename mplVectorFromVariadic<R,T const&, Args...>::type
deduce_signature(R(T::*)(Args...) const)
{
    return typename mplVectorFromVariadic<R,T const&, Args...>::type();
}


// With a wrapper type ?
// passer wrapped avant

template <class R, class T, class... Args, class Wrapped>
typename mplVectorFromVariadic<R, typename most_derived<T,Wrapped>::type&, Args...>::type
deduce_signature(R(T::*)( Args... ), Wrapped*)
{
    return typename mplVectorFromVariadic<R,typename most_derived<T,Wrapped>::type&, Args...>::type();
}

template <class R, class T, class Wrapped, class... Args>
typename mplVectorFromVariadic<R, typename most_derived<T,Wrapped>::type const&, Args...>::type
deduce_signature(R(T::*)(Args...) const, Wrapped*)
{
    return typename mplVectorFromVariadic<R,typename most_derived<T,Wrapped>::type const&, Args...>::type();
}



#undef LUABIND_GENERATE_SIGNATURE_FROM_VARIADIC



}} // namespace luabind::detail

# endif // LUABIND_DEDUCE_SIGNATURE_VARIADIC_080911_HPP
