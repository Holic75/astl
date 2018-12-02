#ifndef ASTL_MEMORY_OPERATIONS_H
#define ASTL_MEMORY_OPERATIONS_H

#if defined(ARDUINO)
#include <Arduino.h>
typedef uint16_t size_t;
void *operator new(size_t size, void *ptr);
void operator delete(void *obj, void *alloc);
#else
#include <cstdint>
#include <new>
#include <utility>
#endif

#include <type_traits.h>




namespace astl
{
    
template<class T>
void memcopy(T* dest, const T* src, size_t len)
{
	if (src == dest)
	{
		return;
	}
	if (std::is_trivially_copyable<T>::value && std::is_trivially_destructible<T>::value)
	{
		::memmove(dest, src, len * sizeof(T));
		return;
	}

	if (src >= dest)
	{
		for (size_t i = 0; i < len; i++)
		{
			dest[i] = src[i];
		}
	}
	else
	{
		T* dest1 = dest - 1;
		const T* src1 = src - 1;
		for (size_t i = len; i > 0; i--)
		{
			dest1[i] = src1[i];
		}
	}
}


template<class T>
void memmove(T* dest, const T* src, size_t len)
{
	if (src == dest)
	{
		return;
	}
	if (std::is_trivially_copyable<T>::value && std::is_trivially_destructible<T>::value)
	{
		::memmove(dest, src, len * sizeof(T));
		return;
	}

	if (src >= dest)
	{
		for (size_t i = 0; i < len; i++)
		{
			dest[i] = std::move(src[i]);
			src[i].~T();
		}
	}
	else
	{
		T* dest1 = dest - 1;
		const T* src1 = src - 1;
		for (size_t i = len; i > 0; i--)
		{
			dest1[i] = std::move(src1[i]);
			src[i].~T();
		}
	}
}


template<class T, class X>
void memcopy(T* dest, const X* src, size_t len)
{   
	if (src == dest)
	{
		return;
	}
	if (src >= dest)
	{
		for (size_t i = 0; i < len; i++)
		{
			dest[i] = src[i];
		}
	}
	else
	{
		T* dest1 = dest - 1;
		const T* src1 = src - 1;
		for (size_t i = len; i > 0; i--)
		{
			dest1[i] = src1[i];
		}
	}
}


template<class T, class X>
void memmove(T* dest, const X* src, size_t len)
{
	if (src == dest)
	{
		return;
	}
	if (src >= dest)
	{
		for (size_t i = 0; i < len; i++)
		{
			dest[i] = src[i];
			src[i].~T();
		}
	}
	else
	{
		T* dest1 = dest - 1;
		const T* src1 = src - 1;
		for (size_t i = len; i > 0; i--)
		{
			dest1[i] = src1[i];
			src[i].~T();
		}
	}
}


template<class T>
void memclear(T* src, size_t len)
{
    if (std::is_trivially_destructible<T>::value)
    {
        return;
    }
    
    for (size_t i = 0; i< len; i++)
    {
        src[i].~T();
    }
}


template<class T, class ...Args>
void mem_init(T* src, size_t len, Args&&... args)
{    

	if (std::is_trivially_constructible<T>::value && sizeof...(Args) == 0)
	{
		return;
	}

    for (size_t i = 0; i < len; i++)
    {
       T* new_obj =  new (src + i) T(args...);
    }
}



    
}

#endif