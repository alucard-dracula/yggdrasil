//s_t.h

#ifndef __S_T_H__
#define __S_T_H__

class s_t
{
public:
    s_t(void)
        : i(100)
    {
    }

    s_t(int ni)
        : i(ni)
    {
    }

    s_t(const s_t& st)
        : i(st.i)
    {
    }

    ~s_t(void)
    {
    }

    s_t& operator=(const s_t& st)
    {
        i = st.i;
        return *this;
    }

    int i;
};



#endif //__S_T_H__
