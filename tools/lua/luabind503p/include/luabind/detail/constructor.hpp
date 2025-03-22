// Copyright Daniel Wallin 2008. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !BOOST_PP_IS_ITERATING

# ifndef LUABIND_DETAIL_CONSTRUCTOR_081018_HPP
#  define LUABIND_DETAIL_CONSTRUCTOR_081018_HPP

#  include <luabind/get_main_thread.hpp>
#  include <luabind/object.hpp>
#  include <luabind/wrapper_base.hpp>
#  include <luabind/detail/inheritance.hpp>
#  include <luabind/detail/instance_holder.hpp>

#  include <boost/preprocessor/iteration/iterate.hpp>
#  include <boost/preprocessor/iteration/local.hpp>
#  include <boost/preprocessor/repetition/enum_params.hpp>
#  include <boost/preprocessor/repetition/enum_binary_params.hpp>

#  include <boost/scoped_ptr.hpp> 

namespace luabind { namespace detail {

inline void inject_backref(lua_State*, void*, void*)
{}

template <class T>
void inject_backref(lua_State* L, T* p, wrap_base*)
{
    weak_ref(get_main_thread(L), L, 1).swap(wrap_access::ref(*p));
}


/** \brief  TODO
 * \todo
 * \tparam Arity Number of arguments required by constructor
 * \tparam T Class of which we define constructor
 * \tparam Pointer
 * \tparam Signature Should be a mpl::vector
 * \return
 *
 */
template <std::size_t Arity, class T, class Pointer, class Signature>
struct construct_aux;



// Arity > 0
template <class T, class Pointer, class Signature>
struct construct
  : construct_aux<mpl::size<Signature>::value - 2, T, Pointer, Signature>
{};



// Specialization if empty constructor ?
template <class T, class Pointer, class Signature>
struct construct_aux<0, T, Pointer, Signature>
{
    typedef pointer_holder<Pointer, T> holder_type;

    void operator()(argument const& self_) const
    {
        object_rep* self = touserdata<object_rep>(self_);
        class_rep* cls = self->crep();

//#if defined(BOOST_NO_CXX11_SMART_PTR)
//        std::auto_ptr<T> instance(new T);
//#else
//		std::unique_ptr<T> instance(new T);
//#endif // BOOST_NO_CXX11_SMART_PTR
		boost::scoped_ptr<T> instance(new T);

        inject_backref(self_.interpreter(), instance.get(), instance.get());

        void* naked_ptr = instance.get();
        //Pointer ptr(instance.release());
		Pointer ptr(release_ownership_trans(instance));

        void* storage = self->allocate(sizeof(holder_type));

        // Calls constructor
        self->set_instance(new (storage) holder_type(
            ptr, registered_class<T>::id, naked_ptr, cls));
    }
};

// Cf doc here
// http://www.boost.org/doc/libs/1_46_1/libs/preprocessor/doc/topics/file_iteration.html
#  define BOOST_PP_ITERATION_PARAMS_1 \
    (3, (1, LUABIND_MAX_ARITY, <luabind/detail/constructor.hpp>))
#  include BOOST_PP_ITERATE()

}} // namespace luabind::detail

# endif // LUABIND_DETAIL_CONSTRUCTOR_081018_HPP

// If BOOST_PP_IS_ITERATING
#else

// Current iteration
# define N BOOST_PP_ITERATION()


// Specialization of construct_aux for a constructer with N arguments
template <class T, class Pointer, class Signature>
struct construct_aux<N, T, Pointer, Signature>
{
    typedef typename mpl::begin<Signature>::type first;
    typedef typename mpl::next<first>::type iter0;

    // Produce iterators  iter1, ..., iterN and then typedef iter1::type to a1 etc....
    //http://www.boost.org/doc/libs/1_46_1/libs/preprocessor/doc/ref/local_macro.html
    //BOOST_PP_DEC(n) => n The value to be decremented.  Valid values range from 0 to BOOST_PP_LIMIT_MAG.
# define BOOST_PP_LOCAL_MACRO(n) \
    typedef typename mpl::next< \
        BOOST_PP_CAT(iter,BOOST_PP_DEC(n))>::type BOOST_PP_CAT(iter,n); \
    typedef typename BOOST_PP_CAT(iter,n)::type BOOST_PP_CAT(a,BOOST_PP_DEC(n));

# define BOOST_PP_LOCAL_LIMITS (1,N)
# include BOOST_PP_LOCAL_ITERATE()

    typedef pointer_holder<Pointer, T> holder_type;


    // BOOST_PP_ENUM_BINARY_PARAMS(N,a,_) => expands to a1 1, a2 2,... aN N ?
    // http://www.boost.org/doc/libs/1_46_1/libs/preprocessor/doc/ref/enum_binary_params.html
    void operator()(argument const& self_, BOOST_PP_ENUM_BINARY_PARAMS(N,a,_)) const
    {
        object_rep* self = touserdata<object_rep>(self_);
		if(!self)
		{
			return;
		}
        class_rep* cls = self->crep();
		if(!cls)
		{
			return;
		}

        // Use parameters of the function
//#if defined(BOOST_NO_CXX11_SMART_PTR)
//        std::auto_ptr<T> instance(new T(BOOST_PP_ENUM_PARAMS(N,_)));
//#else
//		 std::unique_ptr<T> instance(new T(BOOST_PP_ENUM_PARAMS(N,_)));
//#endif // BOOST_NO_CXX11_SMART_PTR
		boost::scoped_ptr<T> instance(new T(BOOST_PP_ENUM_PARAMS(N,_)));
        inject_backref(self_.interpreter(), instance.get(), instance.get());

        void* naked_ptr = instance.get();
        // auto_ptr.release() -> Sets the auto_ptr internal pointer to null pointer without destructing the object currently pointed by the auto_ptr
        // pointer to the element pointed by the auto_ptr object before the call
        //Pointer ptr(instance.release());
		Pointer ptr(release_ownership_trans(instance));

        void* storage = self->allocate(sizeof(holder_type));

        self->set_instance(new (storage) holder_type(
            ptr, registered_class<T>::id, naked_ptr, cls));
    }
};

# undef N

#endif

