//chg_ref.hpp

#ifndef __YGGR_CHG_REF_HPP__
#define __YGGR_CHG_REF_HPP__

#include <base/non_create.hpp>

namespace yggr
{

template<typename T>
class chg_ref : private edit_rule::non_create
{
public:
    typedef T val_type;
public:
    static val_type& ref(val_type& val)
    {
        return val;
    }

    static const val_type& c_ref(const val_type& val)
    {
        return val;
    }
};

} // namespace yggr

#endif //__YGGR_CHG_REF_HPP__
