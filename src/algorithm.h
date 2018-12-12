#ifndef ASTL_ALGORITHM_H
#define ASTL_ALGORITHM_H

namespace astl 
{
    template<class T, class X>
    T bound(const T& x, const X& bnd) 
    {
        if (x > bnd)
        {
            return bnd;
        }
        else if (x < -bnd)
        {
            return -bnd;
        }
        return x;
    }
    
    
    template<class T, class X, class Y>
    T bound(const T& x, const X& left_bnd, const Y& right_bnd) 
    {
        if (x > right_bnd)
        {
            return right_bnd;
        }
        else if (x < left_bnd)
        {
            return left_bnd;
        }
        return x;
    }
    
}


#endif