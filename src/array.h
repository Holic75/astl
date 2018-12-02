#ifndef ASTL_ARRAY_H
#define ASTL_ARRAY_H
#include <memory_operations.h>


namespace astl 
{

template <class T, size_t N>
struct Array
{
    T data_[N];
    constexpr size_t size() const {return N;};

    const T& operator[](size_t i) const {return data_[i];};
    T& operator[](size_t i) {return data_[i];};

    const T&  front() const {return data_[0];};
    T& front(size_t i) {return data_[0];};

    const T&  back() const {return data_[N - 1];};
    T& back(size_t i) {return data_[N - 1];};
    
    void copyToBuffer(void* buffer) const
    {
        memcpy(buffer, data_, N*sizeof(T));
    }


    bool copyFromBuffer(const void* buffer)
    {
        memcpy(data_, buffer, N*sizeof(T));
        return true;
    }


    template<class X>
    Array<T, N>& operator=(const Array<X,N>& x)
    {
        for (int i = 0; i< N; i++)
        {
            data_[i] = x[i];
        }  
    }

	T* data() { return &data_[0]; };
	const T* data() const { return &data_[0]; };

    typedef T* iterator;
    typedef const T*  const_iterator;
    iterator begin() {return &data_[0];};
    iterator end() {return &data_[N];}
    const_iterator begin() const {return &data_[0];};
    const_iterator end() const {return &data_[N];}
};


}


#endif