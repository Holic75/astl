#ifndef ASTL_PAIR_H
#define ASTL_PAIR_H


namespace astl
{
template <class X, class Y>
struct Pair
{
    X first;
    Y second;
    Pair(const X& x, const Y& y)
        :first(x), second(y) {};
    Pair(){};
};

template<class X, class Y>
Pair<X,Y> makePair(const X& x, const Y& y)
{
    return Pair<X,Y>(x, y);
}

}


#endif