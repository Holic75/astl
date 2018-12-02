#ifndef ASTL_TYPE_TRAITS_H
#define ASTL_TYPE_TRAITS_H

#if defined(ARDUINO)

namespace std
{

// integral constant
template<class _Ty,
	_Ty _Val>
	struct integral_constant
{	// convenient template for integral constant types
	static constexpr _Ty value = _Val;

	typedef _Ty value_type;
	typedef integral_constant<_Ty, _Val> type;

	constexpr operator value_type() const _NOEXCEPT
	{	// return stored value
		return (value);
	}

	constexpr value_type operator()() const _NOEXCEPT
	{	// return stored value
		return (value);
	}
};

typedef integral_constant<bool, true> true_type;
typedef integral_constant<bool, false> false_type;

// TEMPLATE CLASS is_lvalue_reference
template<class _Ty>
struct is_lvalue_reference
	: false_type
{	// determine whether _Ty is an lvalue reference
};

template<class _Ty>
struct is_lvalue_reference<_Ty&>
	: true_type
{	// determine whether _Ty is an lvalue reference
};

// TEMPLATE CLASS is_rvalue_reference
template<class _Ty>
struct is_rvalue_reference
	: false_type
{	// determine whether _Ty is an rvalue reference
};

template<class _Ty>
struct is_rvalue_reference<_Ty&&>
	: true_type
{	// determine whether _Ty is an rvalue reference
};


//memory props
template<class T>
struct is_trivially_constructible
{
    static const bool value = __has_trivial_constructor(T);
};


template<class T>
struct is_trivially_copyable
{
    static const bool value = __has_trivial_copy(T);
};


template<class T>
struct is_trivially_destructible
{
    static const bool value = __has_trivial_destructor(T);
};

//remove_reference
template< class T > struct remove_reference { typedef T type; };
template< class T > struct remove_reference<T&> { typedef T type; };
template< class T > struct remove_reference<T&&> { typedef T type; };
template< class T >

//std::move
constexpr typename std::remove_reference<T>::type&& move(T&& t);

//forward
template<class _Ty> inline
constexpr _Ty&& forward(
	typename remove_reference<_Ty>::type& _Arg) _NOEXCEPT
{	// forward an lvalue as either an lvalue or an rvalue
	return (static_cast<_Ty&&>(_Arg));
}

template<class _Ty> inline
constexpr _Ty&& forward(
	typename remove_reference<_Ty>::type&& _Arg) _NOEXCEPT
{	// forward an rvalue as an rvalue
	static_assert(!is_lvalue_reference<_Ty>::value, "bad forward call");
	return (static_cast<_Ty&&>(_Arg));
}



#else
#include <type_traits>
#endif

#endif