// Copyright Daniel Wallin 2008. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUABIND_INSTANCE_HOLDER_081024_HPP
# define LUABIND_INSTANCE_HOLDER_081024_HPP

# include <luabind/detail/inheritance.hpp>
# include <luabind/detail/class_rep.hpp> // TODO
# include <luabind/detail/scoped_ptr_helper.hpp>
# include <luabind/get_pointer.hpp>
# include <luabind/typeid.hpp>
# include <boost/utility/enable_if.hpp>
# include <boost/type_traits/is_class.hpp>
# include <boost/type_traits/is_polymorphic.hpp>
# include <stdexcept>

namespace luabind { namespace detail {

class instance_holder
{
public:
    instance_holder(class_rep* cls, bool pointee_const)
      : m_cls(cls)
      , m_pointee_const(pointee_const)
    {}

    virtual ~instance_holder()
    {}

    virtual std::pair<void*, int> get(class_id target) const = 0;

    virtual void release() = 0;

    class_rep* get_class() const
    {
        return m_cls;
    }

    bool pointee_const() const
    {
        return m_pointee_const;
    }

private:
    class_rep* m_cls;
    bool m_pointee_const;
};

namespace mpl = boost::mpl;

inline mpl::false_ check_const_pointer(void*)
{
    return mpl::false_();
}

inline mpl::true_ check_const_pointer(void const*)
{
    return mpl::true_();
}

#if defined(BOOST_NO_CXX11_SMART_PTR)

template <class T> inline
void release_ownership(std::auto_ptr<T>& p)
{
    p.release();
}

#else

template <class T> inline
void release_ownership(std::unique_ptr<T>& p)
{
    p.release();
}

template <class T> inline
void release_ownership(const std::unique_ptr<T>& p)
{
    const_cast<std::unique_ptr<T>&>(p).release();
}

template <class T> inline
T* release_ownership_trans(const std::unique_ptr<T>& p)
{
    return const_cast<std::unique_ptr<T>&>(p).release();
}

#endif // BOOST_NO_CXX11_SMART_PTR

template <class T> inline
void release_ownership(boost::scoped_ptr<T>& p)
{
   scoped_ptr_helper::release_ownership_scoped_ptr(p);
}

template <class T> inline
void release_ownership(const boost::scoped_ptr<T>& p)
{
	scoped_ptr_helper::release_ownership_scoped_ptr(const_cast<boost::scoped_ptr<T>&>(p));
}

template <class T> inline
T* release_ownership_trans(const boost::scoped_ptr<T>& p)
{
    return scoped_ptr_helper::release_ownership_trans_scoped_ptr(p);
}

template <class P> inline
void release_ownership(P const&)
{
    throw std::runtime_error(
        "luabind: smart pointer does not allow ownership transfer");
}

template <class T> inline
class_id static_class_id(T*)
{
    return registered_class<T>::id;
}


/** \brief
 */

template <class P, class Pointee = void const>
class pointer_holder : public instance_holder
{
public:
    pointer_holder(
        P np, class_id dynamic_id, void* dynamic_ptr, class_rep* cls
    )
      : instance_holder(cls, check_const_pointer(false ? get_pointer(np) : 0))
      , p(np)
      , weak(0)
      , dynamic_id(dynamic_id)
      , dynamic_ptr(dynamic_ptr)
    {}

    std::pair<void*, int> get(class_id target) const
    {
        if (target == registered_class<P>::id)
            return std::pair<void*, int>(&this->p, 0);

        void* naked_ptr = const_cast<void*>(static_cast<void const*>(
            weak ? weak : get_pointer(p)));

        if (!naked_ptr)
            return std::pair<void*, int>(static_cast<void*>(0), 0);

        return get_class()->casts().cast(
            naked_ptr
          , static_class_id(false ? get_pointer(p) : 0)
          , target
          , dynamic_id
          , dynamic_ptr
        );
    }

    void release()
    {
        weak = const_cast<void*>(static_cast<void const*>(
            get_pointer(p)));
        release_ownership(p);
    }

private:
    mutable P p;
    // weak will hold a possibly stale pointer to the object owned
    // by p once p has released it's owership. This is a workaround
    // to make adopt() work with virtual function wrapper classes.
    void* weak;
    class_id dynamic_id;
    void* dynamic_ptr;
};

#if !defined(BOOST_NO_CXX11_SMART_PTR)

template <class T, class Pointee>
class pointer_holder <std::unique_ptr<T>, Pointee> 
	: public instance_holder
{
public:
	typedef T value_type;
	typedef T* pointer_type;
	typedef std::unique_ptr<T> unique_ptr_type;

public:
    pointer_holder(
        unique_ptr_type& np, class_id dynamic_id, void* dynamic_ptr, class_rep* cls
    )
      : instance_holder(cls, check_const_pointer(false ? get_pointer(np) : 0))
      , p(release_ownership_trans(np))
      , weak(0)
      , dynamic_id(dynamic_id)
      , dynamic_ptr(dynamic_ptr)
    {}

    std::pair<void*, int> get(class_id target) const
    {
        if (target == registered_class<unique_ptr_type>::id)
            return std::pair<void*, int>(&this->p, 0);

        void* naked_ptr = const_cast<void*>(static_cast<void const*>(
            weak ? weak : get_pointer(p)));

        if (!naked_ptr)
            return std::pair<void*, int>(static_cast<void*>(0), 0);

        return get_class()->casts().cast(
            naked_ptr
          , static_class_id(false ? get_pointer(p) : 0)
          , target
          , dynamic_id
          , dynamic_ptr
        );
    }

    void release()
    {
        weak = const_cast<void*>(static_cast<void const*>(
            get_pointer(p)));
        release_ownership(p);
    }

private:
    mutable unique_ptr_type p;
    // weak will hold a possibly stale pointer to the object owned
    // by p once p has released it's owership. This is a workaround
    // to make adopt() work with virtual function wrapper classes.
    void* weak;
    class_id dynamic_id;
    void* dynamic_ptr;
};

#endif // BOOST_NO_CXX11_SMART_PTR

template <class T, class Pointee>
class pointer_holder <boost::scoped_ptr<T>, Pointee> 
	: public instance_holder
{
public:
	typedef T value_type;
	typedef T* pointer_type;
	typedef boost::scoped_ptr<T> unique_ptr_type;

public:
    pointer_holder(
        unique_ptr_type& np, class_id dynamic_id, void* dynamic_ptr, class_rep* cls
    )
      : instance_holder(cls, check_const_pointer(false ? get_pointer(np) : 0))
      , p(release_ownership_trans(np))
      , weak(0)
      , dynamic_id(dynamic_id)
      , dynamic_ptr(dynamic_ptr)
    {}

    std::pair<void*, int> get(class_id target) const
    {
        if (target == registered_class<unique_ptr_type>::id)
            return std::pair<void*, int>(&this->p, 0);

        void* naked_ptr = const_cast<void*>(static_cast<void const*>(
            weak ? weak : get_pointer(p)));

        if (!naked_ptr)
            return std::pair<void*, int>(static_cast<void*>(0), 0);

        return get_class()->casts().cast(
            naked_ptr
          , static_class_id(false ? get_pointer(p) : 0)
          , target
          , dynamic_id
          , dynamic_ptr
        );
    }

    void release()
    {
        weak = const_cast<void*>(static_cast<void const*>(
            get_pointer(p)));
        release_ownership(p);
    }

private:
    mutable unique_ptr_type p;
    // weak will hold a possibly stale pointer to the object owned
    // by p once p has released it's owership. This is a workaround
    // to make adopt() work with virtual function wrapper classes.
    void* weak;
    class_id dynamic_id;
    void* dynamic_ptr;
};

}} // namespace luabind::detail

#endif // LUABIND_INSTANCE_HOLDER_081024_HPP
