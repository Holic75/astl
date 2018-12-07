#ifndef ASTL_VECTOR_H
#define ASTL_VECTOR_H
#include "allocator.h"
#include "initializer_list.h"

namespace astl
{
template <class T, class Allocator = HeapAllocator<T>, AllocationPolicyFunc allocPolicy = allocationPolicy2>
class Vector
{
    T* data_;
    Allocator allocator_;
    size_t size_;
	size_t capacity_;

	bool reserveShift(size_t new_capacity, size_t old_pos, size_t new_pos)
	{
		T* new_data;
		if (new_capacity <= capacity_)
		{
			new_data = data_;
		}
		else
		{
			new_data = allocator_.allocate(new_capacity);
			if (new_data == nullptr)
			{
				return false;
			}
		}
		if (old_pos < new_pos)
		{
			memmove(new_data, data_, old_pos);
			memmove(new_data + new_pos, data_ + old_pos, size_ - old_pos);
		}

		
		allocator_.deallocate(data_, capacity_);
		capacity_ = new_capacity;
        data_ = new_data;
		return true;
	}

public:
    size_t size() const {return size_;};
	size_t capacity() const { return capacity_; };

	bool reserve(size_t new_capacity)
	{
		if (new_capacity <= capacity_)
		{
			return true;
		}

		T* new_data = allocator_.allocate(new_capacity);
		if (new_data == nullptr)
		{
			return false;
		}
		memmove(new_data, data_, size_);
		allocator_.deallocate(data_, capacity_);
		capacity_ = new_capacity;
        data_ = new_data;
		return true;
	}


	template<class ...Args>
	bool resize(size_t new_size, Args&&... args)
	{
		if (new_size <= capacity_)
		{
			if (new_size < size_)
			{
				memclear(data_ + new_size, size_ - new_size);
			}
			else
			{
				mem_init(data_ + size_, new_size - size_, std::forward<Args>(args)...);
			}
			size_ = new_size;
			return true;
		}

		if (!reserve(new_size))
		{
			return false;
		}
		mem_init(data_ + size_, new_size - size_, std::forward<Args>(args)...);
		size_ = new_size;
		return true;
	}

   
    void copyToBuffer(void* buffer) const
    {
        ::memcpy(buffer, data_, size_*sizeof(T));
    }

	template<class X>
	void copyToBuffer(X* buffer) const
	{
		memcopy(buffer, data_, size_ * sizeof(T));
	}


    bool copyFromBuffer(const void* buffer, size_t len)
    {
        
		if (!resize(len))
		{
			return false;
		}
        ::memcpy(data_, buffer, size_*sizeof(T));
        return true;
    }

	template<class X>
	bool copyFromBuffer(const X* buffer, size_t len)
	{

		if (!resize(len))
		{
			return false;
		}
		memcopy(data_, buffer, size_);
		return true;
	}


    template<class ...Args>
    bool emplaceBack(Args&&... args) 
    {
		if (capacity_ > size_ || reserve(allocPolicy(size_+1)))
		{
			T* new_ptr = new (data_ + size_) T(std::forward<Args>(args)...);
			size_++;
			return true;
		}
		return false;
    }


	bool pushBack(const T& x)
	{
		return emplaceBack(x);
	}


    template <class X, class Allocator2, AllocationPolicyFunc allocPolicy2 >
    bool pushBack(const Vector<X, Allocator2, allocPolicy2>& x)
    {
		if (capacity_ > size_ || reserve(allocPolicy(size_ + x.size())))
		{
			for (size_t i = 0; i < x.size(); i++)
			{
				T* new_ptr = new (data_ + size_ + i) T(x[i]);
			}
			size_+= x.size();
			return true;
		}

        return false;
    }


    template<class ...Args>
    bool emplace(size_t pos, Args&&... args)
    {
		if (!reserveShift(allocPolicy(size_ + 1), pos, pos + 1))
		{
			return false;
		}
		T* new_ptr = new (data_ + pos) T(std::forward<Args>(args)...);
        size_++;
        return true;
    }


	bool insert(size_t pos, const T& x)
	{
		return emplace(pos, x);
	}


    template <class X, class Allocator2, AllocationPolicyFunc allocPolicy2>
    bool insert(size_t pos, const Vector<X, Allocator2, allocPolicy2>& x)
    {
		if (static_cast<void*>(this) == static_cast<const void*>(&x))
		{
			auto y = x;
			insert(pos, y);
		}
		if (!reserveShift(size_ + x.size(), pos, pos + x.size()))
		{
			return false;
		}
		for (size_t i = 0; i < x.size(); i++)
		{
			T* new_ptr = new (data_ + pos + i) T(x[i]);
		}
      
        size_+= x.size();
        return true;
    }


	bool erase(size_t start, size_t end)
	{
		if (end > size_)
		{
			return false;
		}

		if (start >= end)
		{
			return true;
		}

		size_t n_elts_to_erase = end - start;
        size_t n_elts_to_move = size_ - end;
		memmove(data_ + start, data_ + end, n_elts_to_move);
		size_ -= n_elts_to_erase;
		if (n_elts_to_erase > n_elts_to_move)
		{
			memclear(data_ + size_, n_elts_to_erase - n_elts_to_move);
		}
		return true;
	}


    bool erase(size_t pos) 
    {
		return erase(pos, pos + 1);
    }


    bool popBack() 
    {
		if (size_ == 0)
		{
			return false;
		}
		size_--;
		data_[size_].~T();
        return true;
    }

	template<class ...Args>
	bool emplaceFront(Args&&... args)
	{
		return insert(0, std::forward<Args>(args)...);
	}


    bool pushFront(const T& x) 
    {
        return insert(0, x);
    }


    template <class X, class Allocator2, AllocationPolicyFunc allocPolicy2 >
    bool pushFront(const Vector<X, Allocator2, allocPolicy2>& x)
    {
        return insert(0, x);
    }


    bool popFront() 
    {
        return erase(0);
    }



    void clear()
    {
		memclear(data_, size_);
        size_ = 0;
    }


	bool shrinkToFit()
	{
		if (size_ == capacity_)
		{
			return true;
		}

		T* new_data = allocator_.allocate(size_);
		if (new_data == nullptr)
		{
			return false;
		}
		memmove(new_data, data_, size_);
		allocator_.deallocate(data_, capacity_);
		capacity_ = size_;
		data_ = new_data;
		return true;
	}

    const T& operator[](size_t i) const {return data_[i];};
    T& operator[](size_t i) {return data_[i];};

    const T&  front() const {return data_[0];};
    T& front(size_t i) {return data_[0];};

    const T&  back() const {return data_[size_ - 1];};
    T& back(size_t i) {return data_[size_ - 1];};

    T* data() {return data_;};
    const T* data() const {return data_;};

	Vector()
		:allocator_(), capacity_(0), size_(0), data_(nullptr){};

    template<class ...Args>
    Vector(size_t len, Args&&... args)
        :Vector()
    {
		resize(len, std::forward<Args>(args)...);
    }
    
    
    Vector(size_t len)
        : Vector()
	{
		resize(len);
	}

    Vector(const Vector& x)
        :Vector()
    {
        copyFromBuffer(x.data(), x.size());
    }
    
    
     Vector(Vector&& x)
        :Vector()
    {
         if(x.allocator_.is_movable)
        {
            data_ = x.data_;
            size_ = x.size_;
            capacity_ = x.capacity_;
            
            x.data_ = nullptr;
            x.size_ = 0;
            x.capacity_ = 0;
        }
        else
        {
            copyFromBuffer(x.data(), x.size());
            x.clear();
            x.shrinkToFit();
        }
        copyFromBuffer(x.data(), x.size());
    }

    template<class X, class Allocator2, AllocationPolicyFunc allocPolicy2>
    Vector(const Vector<X, Allocator2, allocPolicy2>& x)
		: Vector()
    {
		copyFromBuffer(x.data(), x.size());
    }


	Vector(std::initializer_list<T> l)
		:Vector()
	{
		copyFromBuffer(l.begin(), l.size());
	}


    Vector& operator=(const Vector& x)
    {
        if (this != &x)
        {
            clear();
			copyFromBuffer(x.data(), x.size());
        }
		return *this;
    }
    
    
    Vector& operator=(Vector&& x)
    {
        if (this != &x)
        {
            if(x.allocator_.is_movable)
            {
			    allocator_.deallocate(data_, capacity_);
                data_ = x.data_;
                size_ = x.size_;
                capacity_ = x.capacity_;
                
                x.data_ = nullptr;
                x.size_ = 0;
                x.capacity_ = 0;
            }
            else
            {
                clear();
                copyFromBuffer(x.data(), x.size());
                x.clear();
                x.shrinkToFit();
            }
        }
		return *this;
    }
    

	Vector& operator=(std::initializer_list<T> l)
	{
        clear();
		copyFromBuffer(l.begin(), l.size());		
		return *this;
	}

    template<class X, class Allocator2, AllocationPolicyFunc allocPolicy2>
    Vector& operator=(const Vector<X, Allocator2, allocPolicy2>& x)
    {
        clear();
		copyFromBuffer(x.data(), x.size());
		return *this;
    }


	template<class X, class Allocator2, AllocationPolicyFunc allocPolicy2>
    bool operator==(const Vector<X, Allocator2, allocPolicy2>& x)
    {
		if (size_!= x.size())
		{
			return false;
		}

		auto x_data = x.data();
		for (size_t i = 0; i < size_; i++)
		{
			if (data_[i] != x_data[i])
			{
				return false;
			}
		}
		return true;
    }


	template<class X, class Allocator2, AllocationPolicyFunc allocPolicy2>
    bool operator!=(const Vector<X, Allocator2, allocPolicy2>& x)
	{
		return !((*this)==x);
	}
	

    typedef T* iterator;
    typedef const T* const_iterator;
    iterator begin() {return data_;};
    iterator end() {return data_ + size_;}
    const_iterator begin() const {return data_;};
    const_iterator end() const {return data_ + size_;}
};

template<class T, size_t N>
using StaticVector = Vector<T, FixedSizeAllocator<T, N>, allocationPolicyFixed>;

template<class T, size_t N>
using ChunkVector = Vector<T, HeapAllocator<T>, allocationPolicyChunk<N>>;

}

#endif
