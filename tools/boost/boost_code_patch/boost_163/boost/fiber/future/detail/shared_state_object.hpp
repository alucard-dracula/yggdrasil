
//          Copyright Oliver Kowalke 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBERS_DETAIL_SHARED_STATE_OBJECT_H
#define BOOST_FIBERS_DETAIL_SHARED_STATE_OBJECT_H

#include <boost/config.hpp>

#include <memory>

//#include <boost/config.hpp>

#include <boost/fiber/detail/config.hpp>
#include <boost/fiber/future/detail/shared_state.hpp>

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace boost {
namespace fibers {
namespace detail {

template< typename R, typename Allocator >
class shared_state_object : public shared_state< R > {
public:
    typedef typename std::allocator_traits< Allocator >::template rebind_alloc< 
        shared_state_object
    >                                           allocator_t;

    shared_state_object( allocator_t const& alloc) :
        shared_state< R >(), alloc_( alloc) {
    }

protected:
    void deallocate_future() noexcept override final {
        destroy_( alloc_, this);
    }

private:
    allocator_t             alloc_;

    static void destroy_( allocator_t const& alloc, shared_state_object * p) noexcept {
        allocator_t a{ alloc };
        a.destroy( p);
        a.deallocate( p, 1);
    }
};

}}}

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif // BOOST_FIBERS_DETAIL_SHARED_STATE_OBJECT_H
