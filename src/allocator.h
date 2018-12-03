#ifndef ASTL_ALLOCATOR_H
#define ASTL_ALLOCATOR_H

#include "memory_operations.h"
#include "allocator.h"

namespace astl
{
    template <class T, size_t N>
    class FixedSizeAllocator
    {

        uint8_t data_[N*sizeof(T)];

    public:
        
        static const bool is_movable = false;
        constexpr size_t maxSize() const {return N;};
        T* allocate(size_t n)
        {
            if (n > N)
            {
                return nullptr;
            }

            return  reinterpret_cast<T*>(&data_[0]);
        }

        bool deallocate(T* ptr, size_t n)
        {
            if (ptr!= reinterpret_cast<T*>(&data_[0]))
            {
                return false;
            }
			return true;
        }

        FixedSizeAllocator() {};

    };


template <class T>
class HeapAllocator
{

public:
    static const bool is_movable = true;
    constexpr size_t maxSize() const {return 32768/sizeof(T);};
    T* allocate(size_t n)
    {
        if (n == 0)
        {
            return nullptr;
        }
        void* ptr = new uint8_t[n *sizeof(T)];
        T* typed_ptr = static_cast<T*>(ptr);

        return typed_ptr;
    }

    bool deallocate(T* ptr, size_t n)
    {
        if (n != 0)
        {
            delete ptr;               
        }

        return true;
    }

    HeapAllocator() {};
};


typedef size_t(*AllocationPolicyFunc)(size_t);

inline size_t allocationPolicy2(size_t n)
{
	if (n == 0)
	{
		return 0;
	}
    size_t out = 1;
    while (n != 0)
    {
        n = n >> 1;
        out = out << 1;
    }
    return out;
}

template<size_t CH>
size_t allocationPolicyChunk(size_t n)
{
    return (n + CH - 1)/CH;
}

inline size_t allocationPolicyFixed(size_t n)
{
	return n;
}


}




#endif