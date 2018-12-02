#ifndef ASTL_BITSET_H
#define ASTL_BITSET_H
#include "vector.h"

namespace astl
{
template<class Allocator = HeapAllocator<uint8_t>, AllocationPolicyFunc allocPolicy = allocationPolicy2> class BasicBitVector;

class BasicBitVectorIterator;

class BitReference
{
    uint8_t* byte_ref_;
    const uint8_t bit_id_;

public:
    static constexpr uint8_t UNIT_BYTE_ = 1;
    static constexpr uint8_t FULL_BYTE_ = 255;
    
    operator bool () const {return (*byte_ref_) & (UNIT_BYTE_ << bit_id_);};
    bool operator ~() const {return !(*this);};
    
    BitReference& operator=(bool b) 
    {
        if (b)
        {
            *byte_ref_ = (*byte_ref_) | (UNIT_BYTE_ << bit_id_);
        }
        else
        {
            *byte_ref_ = (*byte_ref_) & ~(UNIT_BYTE_ << bit_id_);    
        }
        return *this;
    }
	BitReference& operator=(const BitReference& b)
	{
		return this->operator=(static_cast<bool>(b));
	}

    BitReference(const BitReference& b) = default;
    BitReference(uint8_t *byte_ref, uint8_t bit_id)
        :byte_ref_(byte_ref), bit_id_(bit_id){};
    
};


class ConstBitReference
{
    const uint8_t* const byte_ref_;
    const uint8_t bit_id_;

public:

    
    operator bool () const {return (*byte_ref_) & (BitReference::UNIT_BYTE_ << bit_id_);};
    bool operator ~() const {return !(*this);};
       
    ConstBitReference(const ConstBitReference& b) = default;
    ConstBitReference(const uint8_t * const byte_ref, uint8_t bit_id)
        :byte_ref_(byte_ref), bit_id_(bit_id){};
    
};


class BasicBitVectorIterator
{
    uint8_t* byte_ref_;
    uint8_t bit_id_;
   
public:
  
    BasicBitVectorIterator& operator++ ()
    {
       bit_id_++;
       
       if (bit_id_ == sizeof(uint8_t))
       {
           bit_id_ = 0;
           byte_ref_++;
       }
       
       return *this;
    }

    BasicBitVectorIterator operator++ (int)
    {
       BasicBitVectorIterator old = *this;
       ++(*this);
       return old;
    }

    BasicBitVectorIterator& operator-- ()
    {  
       if (bit_id_ == 0)
       {
           bit_id_ = sizeof(uint8_t) - 1;
           byte_ref_--;
       }
       else
       {
           bit_id_--;
       }
       
       return *this;
    }

    BasicBitVectorIterator operator-- (int)
    {
       BasicBitVectorIterator old = *this;
       --(*this);
       return old;
    }
    
    BasicBitVectorIterator(uint8_t* byte_ref, uint8_t bit_id)
        :byte_ref_(byte_ref), bit_id_(bit_id) {};
    
    friend BitReference operator *(const BasicBitVectorIterator &b)
    {
        return BitReference(b.byte_ref_, b.bit_id_);
    }
    
};


class ConstBitVectorIterator
{
    const uint8_t* byte_ref_;
    uint8_t bit_id_;
    
public:  
    ConstBitVectorIterator& operator++ ()
    {
       bit_id_++;
       
       if (bit_id_ == sizeof(uint8_t))
       {
           bit_id_ = 0;
           byte_ref_++;
       }
       
       return *this;
    }

    ConstBitVectorIterator operator++ (int)
    {
       ConstBitVectorIterator old = *this;
       ++(*this);
       return old;
    }

    ConstBitVectorIterator& operator-- ()
    {  
       if (bit_id_ == 0)
       {
           bit_id_ = sizeof(uint8_t) - 1;
           byte_ref_--;
       }
       else
       {
           bit_id_--;
       }
       
       return *this;
    }

    ConstBitVectorIterator operator-- (int)
    {
       ConstBitVectorIterator old = *this;
       --(*this);
       return old;
    }
    
    ConstBitVectorIterator(uint8_t* const byte_ref, uint8_t bit_id)
        :byte_ref_(byte_ref), bit_id_(bit_id) {};
    
    friend ConstBitReference operator *(const ConstBitVectorIterator &b)
    {
        return ConstBitReference(b.byte_ref_, b.bit_id_);
    }
    
};

template<class Allocator, AllocationPolicyFunc allocPolicy> 
class BasicBitVector
{
    Vector<uint8_t, Allocator, allocPolicy> data_;
    size_t size_;
    

       
public:
    
    Vector<uint8_t, Allocator, allocPolicy>& data() {return data_;};
    const Vector<uint8_t, Allocator, allocPolicy>& data() const {return data_;};
    
    size_t size() const {return size_;};
    BitReference getBitReference(size_t pos) const {}
    bool operator[](size_t pos) const { return data_[pos/sizeof(uint8_t)] & (BitReference::UNIT_BYTE_ << (pos % sizeof(uint8_t)));};
    BitReference operator[](size_t pos) { return BitReference(&data_[pos/sizeof(uint8_t)], (pos % sizeof(uint8_t)));};
    
    size_t count() const
    {
        size_t ct = 0;
        for (size_t i = 0; i < data_.size(); i++)
        {
            uint8_t byte_i = data_[i];
            while (byte_i != 0)
            {
                ct += (byte_i & BitReference::UNIT_BYTE_);
                byte_i >> 1;
            }
        }
        
        return ct;
    }
    
    
    bool all() const
    {
        if (data_.size() == 0)
        {
            return true;
        }
        
        for (size_t i = 0; i < data_.size() - 1; i++)
        {
            if (data_.size[i] != BitReference::FULL_BYTE_)
            {
                return false;
            }
        }
        
        uint8_t last_byte = data_.back();
        for (size_t i = sizeof(uint8_t)*(data.size_() - 1); i < size_; i++)
        {
            if (!(last_byte & BitReference::UNIT_BYTE_))
            {
                return false;
            }
            last_byte >> 1;
        }
        return true;   
    }
     

    bool none() const
    {
        if (data_.size() == 0)
        {
            return true;
        }
        
        for (size_t i = 0; i < data_.size() - 1; i++)
        {
            if (data_.size[i] != 0)
            {
                return false;
            }
        }
        
        if (data_.back() != 0)
        {
            return false;
        }
        return true;     
    }

    bool any() const
    {
        if (data_.size() == 0)
        {
            return false;
        }
        
        for (size_t i = 0; i < data_.size() - 1; i++)
        {
            if (data_[i] != 0)
            {
                return true;
            }
        }
        
        if (data_.back() != 0)
        {
            return true;
        }
        return false; 
    }    
    
    
    bool resize(size_t new_size)
    {
        if (new_size/sizeof(uint8_t) != size_/sizeof(uint8_t))
        {
            if (!data_.resize((new_size + sizeof(uint8_t) - 1)/sizeof(uint8_t)))
            {
                return false;
            }
        }
        
        for (size_t i = new_size; i < data_.size()*sizeof(uint8_t); i++)
        {
            this->operator[](i) = false;
        }
        size_ = new_size;
        return true;
    }
    
    
    void copyToBuffer(bool* buffer) const
    {
        for (size_t i = 0; i< size_; i++)
        {
            buffer[i] = this->operator[](i);
        }
    }


    bool copyFromBuffer(const bool* buffer, size_t len)
    {
        if (resize(len))
        {
            return false;
        }

        for (size_t i = 0; i< size_; i++)
        {
            this->operator[](i) = buffer[i];
        }
        return true;
    }
    
    
    bool pushBack(bool x) 
    {
        if (!resize(size_ + 1))
        {
            return false;
        }
        this->operator[](size_- 1) = x;
        return true;
    }


    template <class Allocator2, AllocationPolicyFunc allocPolicy2>
    bool pushBack(const BasicBitVector< Allocator2, allocPolicy2>& x) 
    {
        size_t old_size = size_;

        if (old_size % sizeof(uint8_t) == 0)
        {
            if (data_.pushBack(x.data()))
            {
                size_ = old_size + x.size();
                return true;
            }
        }
        else if (resize(old_size + x.size()))
        {
            for (size_t i = 0; i < x.size(); i++)
            {
                this->operator[](i + old_size) = x[i];
            }
            return true;
        }
        return false;
    }


    bool insert(size_t pos, bool  x)
    {
        if (pos > size_)
        {
            return false;
        }
        if (!resize(size_ + 1))
        {
            return false;
        }
        
        for (size_t i  = size_ - 1; i> pos; i--)
        {
            this->operator[](i) = this->operator[](i - 1);
        }
        this->operator[](pos) = x;
        
        return true;
    }


    template <class Allocator2, AllocationPolicyFunc allocPolicy2>
    bool insert(size_t pos, const BasicBitVector<Allocator2, allocPolicy2>& x)
    {
        if (pos > size_)
        {
            return false;
        }
        
        size_t old_size = size_;
        if (pos % sizeof(uint8_t) == 0 && x.size() % sizeof(uint8_t) == 0 && data_.insert(pos*sizeof(uint8_t), x.data()))
        {
            size_ += x.size();
            return true;
        }
        else if (resize(size_ + x.size()))
        {
            const size_t size_x = x.size();
            for (size_t i  = size_ - size_x; i> pos; i++)
            {
				uint8_t di = i - 1;
                this->operator[](di+size_x) = this->operator[](di);
            }
            for (size_t i  = 0; i< size_x; i--)
            {
                this->operator[](i+pos) = x[i];
            } 
            return true;            
        }
     
        return false;
    }


    bool erase(size_t pos) 
    {
        if (pos >= size_)
        {
            return false;
        }

        for (size_t i = pos; i < size_; i++)
        {
            this->operator[](i) = this->operator[](i + 1);
        }
        resize(size_ - 1);
        return true;
    }


    bool erase(size_t start, size_t end) 
    {
        if (end >= size_)
        {
            return false;
        }

        if (start >= end)
        {
            return true;
        }
        
        if (start % sizeof(uint8_t) == 0 && end % sizeof(uint8_t) == 0)
        {
            data_.erase(start/sizeof(uint8_t), end/sizeof(uint8_t));
            size_ -= (end - start)/sizeof(uint8_t);
            return true;
        }
        else
        {
            size_t n_elts_to_erase = end - start;
            for (size_t i = 0; i < size_ - end; i++)
            {
                this->operator[](i + start) = this->operator[](i + end);
            }    
            resize(size_ - n_elts_to_erase);
            return true;
        }
        return false;
    }


    bool popBack() 
    {
        if (size_ == 0)
        {
            return false;
        }
        resize(size_ - 1);
        return true;
    }



    bool pushFront(bool x) 
    {
        return insert(0, x);
    }

	template <class Allocator2>
	bool pushFront(const BasicBitVector<Allocator2>& x)
	{
		return insert(0, x);
	}

    
    bool popFront() 
    {
        return erase(0);
    }
    
    
    bool clear() 
    {
        return resize(0);
    }
    

    BasicBitVector(size_t len, bool x)
        :data_((len + sizeof(uint8_t) - 1)/sizeof(uint8_t), x ? BitReference::FULL_BYTE_ : 0)
    {
        size_t current_size = data_.size() * sizeof(uint8_t);
        size_ = len > current_size ? current_size: len;
        
        if (x)
        {
            for (size_t i  = size_; i < data_.size() * sizeof(uint8_t); i++)
            {
                this->operator[](i) = false;
            }
        }
    }
    
    
    BasicBitVector(size_t len)
        :data_(), size_(0)
    {
        resize(len);
    }


    BasicBitVector()
        :size_(0), data_(){};
        

    BasicBitVector(const BasicBitVector<Allocator, allocPolicy>& x)
        :data_(x.data())
    {
        size_t current_size = data_.size() * sizeof(uint8_t);
        size_ = x.size() > current_size ? current_size: x.size();
    }

    template<class Allocator2, AllocationPolicyFunc allocPolicy2>
    BasicBitVector(const BasicBitVector<Allocator2, allocPolicy2>& x)
        :data_(x.data())
    {
        size_t current_size = data_.size() * sizeof(uint8_t);
        size_ = x.size() > current_size ? current_size: x.size();
    }

 


    BasicBitVector& operator=(const BasicBitVector<Allocator>& x)
    {
        if (this != &x)
        {
            data_ = x.data();
            size_t current_size = data_.size() * sizeof(uint8_t);
            size_ = x.size() > current_size ? current_size: x.size();
        }
    }

    template<class Allocator2, AllocationPolicyFunc allocPolicy2>
    BasicBitVector& operator=(const BasicBitVector<Allocator2, allocPolicy2>& x)
    {
        data_ = x.data();
        size_t current_size = data_.size() * sizeof(uint8_t);
        size_ = x.size() > current_size ? current_size: x.size();
    }


    BasicBitVector(const std::initializer_list<bool> l)
        :BasicBitVector(l.size())
    {
		auto it = l.begin();
		for (size_t i = 0; i < size_; i++)
		{
			this->operator[](i) = *(it++);
		}
    }
    
    
    bool  front() const {return this->operator[](0);};
    BitReference front(size_t i) {return this->operator[](0);};

    bool  back() const {return this->operator[](size_ - 1);};
    BitReference back(size_t i) {return this->operator[](size_ - 1);};
    
    typedef BasicBitVectorIterator iterator;
    typedef ConstBitVectorIterator const_iterator;
    
    iterator getIterator(size_t i) {return iterator(&data_[i/sizeof(uint8_t)], (i % sizeof(uint8_t)));};
    const iterator getIterator(size_t i) const {return const_iterator(&data_[i/sizeof(uint8_t)], (i % sizeof(uint8_t)));};
    
    iterator begin() {return iterator(0);};
    iterator end() {return const_iterator(0);}
    const_iterator begin() const {return iterator(size_);};
    const_iterator end() const {return const_iterator(size);}
};
    
    
template<size_t N>
using StaticBitVector = BasicBitVector<FixedSizeAllocator<uint8_t, (N + sizeof(uint8_t) - 1)/sizeof(uint8_t)>, allocationPolicyFixed>;

template<size_t N, size_t M>
using ChunkBitVector = BasicBitVector<FixedSizeChunkHeapAllocator<uint8_t, N>, allocationPolicyChunk<M>>;

typedef BasicBitVector<> BitVector;
}


#endif