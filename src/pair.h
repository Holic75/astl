#ifndef ASTL_PAIR_H
#define ASTL_PAIR_H


namespace astl
{
template <class X, class Y>
struct Pair
{
    X first;
    Y second;
	template<class U, class V>
    Pair(U&& x, V&& y)
        :first(std::forward<U>(x)), second(std::forward<V>(y)) {};
    Pair()
		:first(), second(){};
};

template<class X, class Y>
Pair<X,Y> makePair(const X& x, const Y& y)
{
    return Pair<X,Y>(x, y);
}

}


#endif