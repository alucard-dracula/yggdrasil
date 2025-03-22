// Copyright Daniel Wallin 2008. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


# ifndef LUABIND_DEDUCE_SIGNATURE_080911_HPP
#  define LUABIND_DEDUCE_SIGNATURE_080911_HPP

#  include <luabind/detail/most_derived.hpp>

#  if LUABIND_MAX_ARITY <= 8
#   include <boost/mpl/vector/vector10.hpp>
#  else
#   include <boost/mpl/vector/vector50.hpp>
#  endif
#  include <boost/preprocessor/cat.hpp>
#  include <boost/preprocessor/iteration/iterate.hpp>
#  include <boost/preprocessor/repetition/enum_params.hpp>


#include <luabind/detail/deduce_signature_oldschool.hpp>
#if 0
#ifdef BOOST_NO_VARIADIC_TEMPLATES
    #include <luabind/detail/deduce_signature_oldschool.hpp>
#else
    #include <luabind/detail/deduce_signature_variadic.hpp>
#endif
#endif



#endif //LUABIND_DEDUCE_SIGNATURE_080911_HPP

