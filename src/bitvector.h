#ifndef ASTL_BITVECTOR_H
#define ASTL_BITVECTOR_H
#include "vector.h"

namespace astl
{
    
template<class T>
struct BitBlock
{
    static constexpr T BITS_IN_BLOCK = sizeof(T)*8;
    static constexpr T UNIT_BLOCK = 1;
    static constexpr T FULL_BYTE = ~static_cast<T>(0);    
};


template<class T, class Allocator = HeapAllocator<T>, AllocationPolicyFunc allocPolicy = allocationPolicy2> class BitVector;

template<class T> class BitVectorIterator;
template<class T> class ConstBitVectorIterator;


template<class T>
class BitReference
{
    T* byte_ref_;
    const size_t bit_id_;

public:

    
    operator bool () const {return ((*byte_ref_) & (BitBlock<T>::UNIT_BLOCK << bit_id_)) > 0;};
    bool operator ~() const {return !(*this);};
    
    BitReference& operator=(bool b) 
    {
        if (b)
        {
            *byte_ref_ = (*byte_ref_) | (BitBlock<T>::UNIT_BLOCK << bit_id_);
        }
        else
        {
            *byte_ref_ = (*byte_ref_) & ~(BitBlock<T>::UNIT_BLOCK << bit_id_);    
        }
        return *this;
    }
	BitReference& operator=(const BitReference& b)
	{
		return this->operator=(static_cast<bool>(b));
	}

    BitReference(const BitReference& b) = default;
    BitReference(T *byte_ref, size_t bit_id)
        :byte_ref_(byte_ref), bit_id_(bit_id){};
    
};

template<class T>
class ConstBitReference
{
    const T* const byte_ref_;
    const size_t bit_id_;

public:

    
    operator bool () const {return ((*byte_ref_) & (BitBlock<T>::UNIT_BLOCK << bit_id_)) > 0;};
    bool operator ~() const {return !(*this);};
       
    ConstBitReference(const ConstBitReference& b) = default;
    ConstBitReference(const T * const byte_ref, size_t bit_id)
        :byte_ref_(byte_ref), bit_id_(bit_id){};
    
};


template<class T> bool operator==(const BitReference<T>& a, const BitReference<T>& b) { return static_cast<bool>(a) == static_cast<bool>(b); };
template<class T> bool operator!=(const BitReference<T>& a, const BitReference<T>& b) { return static_cast<bool>(a) != static_cast<bool>(b); };
template<class T> bool operator==(const ConstBitReference<T>& a, const BitReference<T>& b) { return static_cast<bool>(a) == static_cast<bool>(b); };
template<class T> bool operator==(const BitReference<T>& a, const ConstBitReference<T>& b) { return static_cast<bool>(a) == static_cast<bool>(b); };
template<class T> bool operator!=(const ConstBitReference<T>& a, const BitReference<T>& b) { return static_cast<bool>(a) != static_cast<bool>(b); };
template<class T> bool operator!=(const BitReference<T>& a, const ConstBitReference<T>& b) { return static_cast<bool>(a) != static_cast<bool>(b); };
template<class T> bool operator==(const ConstBitReference<T>& a, const ConstBitReference<T>& b) { return static_cast<bool>(a) == static_cast<bool>(b); };
template<class T> bool operator!=(const ConstBitReference<T>& a, const ConstBitReference<T>& b) { return static_cast<bool>(a) != static_cast<bool>(b); };

template<class T> 
class BitVectorIterator
{
    T* byte_ref_;
    size_t bit_id_;
   
public:
  
    BitVectorIterator& operator++ ()
    {
       bit_id_++;
       
       if (bit_id_ == BitBlock<T>::BITS_IN_BLOCK)
       {
           bit_id_ = 0;
           byte_ref_++;
       }
       
       return *this;
    }

    BitVectorIterator operator++ (int)
    {
       BitVectorIterator old = *this;
       ++(*this);
       return old;
    }

    BitVectorIterator& operator-- ()
    {  
       if (bit_id_ == 0)
       {
           bit_id_ = BitBlock<T>::BITS_IN_BLOCK - 1;
           byte_ref_--;
       }
       else
       {
           bit_id_--;
       }
       
       return *this;
    }

    BitVectorIterator operator-- (int)
    {
       BitVectorIterator old = *this;
       --(*this);
       return old;
    }
    
    BitVectorIterator(T* byte_ref, size_t bit_id)
        :byte_ref_(byte_ref), bit_id_(bit_id) {};
    
    friend BitReference operator *(const BitVectorIterator<T> &b)
    {
        return BitReference(b.byte_ref_, b.bit_id_);
    }

    friend bool operator==(const BitVectorIterator& a, const BitVectorIterator& b);
    friend bool operator!=(const BitVectorIterator& a, const BitVectorIterator& b);
    friend bool operator==(const BitVectorIterator& a, const ConstBitVectorIterator& b);  
    friend bool operator==(const ConstBitVectorIterator& a, const BitVectorIterator& b);
    friend bool operator!=(const BitVectorIterator& a, const ConstBitVectorIterator& b);  
    friend bool operator!=(const ConstBitVectorIterator& a, const BitVectorIterator& b);    
};

template<class T> 
class ConstBitVectorIterator
{
    const T* byte_ref_;
    size_t bit_id_;
    
public:  
    ConstBitVectorIterator& operator++ ()
    {
       bit_id_++;
       
       if (bit_id_ == BitBlock<T>::BITS_IN_BLOCK)
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
           bit_id_ = BitBlock<T>::BITS_IN_BLOCK - 1;
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
    
    ConstBitVectorIterator(T* const byte_ref, T bit_id)
        :byte_ref_(byte_ref), bit_id_(bit_id) {};
    
    friend ConstBitReference operator *(const ConstBitVectorIterator<T> &b)
    {
        return ConstBitReference(b.byte_ref_, b.bit_id_);
    }
    friend bool operator==(const ConstBitVectorIterator& a, const ConstBitVectorIterator& b);
    friend bool operator!=(const ConstBitVectorIterator& a, const ConstBitVectorIterator& b);
    friend bool operator==(const BitVectorIterator& a, const ConstBitVectorIterator& b);  
    friend bool operator==(const ConstBitVectorIterator& a, const BitVectorIterator& b);
    friend bool operator!=(const BitVectorIterator& a, const ConstBitVectorIterator& b);  
    friend bool operator!=(const ConstBitVectorIterator& a, const BitVectorIterator& b);       
};


template<class T> bool operator==(const BitVectorIterator<T>& a, const BitVectorIterator<T>& b) { return a.byte_ref_ == b.byte_ref_ && a.bit_id_ == b.bit_id_; };
template<class T> bool operator==(const BitVectorIterator<T>& a, const ConstBitVectorIterator<T>& b) { return a.byte_ref_ == b.byte_ref_ && a.bit_id_ == b.bit_id_; };
template<class T> bool operator==(const ConstBitVectorIterator<T>& a, const BitVectorIterator<T>& b) { return a.byte_ref_ == b.byte_ref_ && a.bit_id_ == b.bit_id_; };
template<class T> bool operator==(const ConstBitVectorIterator<T>& a, const ConstBitVectorIterator<T>& b) { return a.byte_ref_ == b.byte_ref_ && a.bit_id_ == b.bit_id_; };
template<class T> bool operator!=(const ConstBitVectorIterator<T>& a, const BitVectorIterator<T>& b) { return a.byte_ref_ != b.byte_ref_ || a.bit_id_ != b.bit_id_; };
template<class T> bool operator!=(const BitVectorIterator<T>& a, const ConstBitVectorIterator<T>& b) { return a.byte_ref_ != b.byte_ref_ || a.bit_id_ != b.bit_id_; };
template<class T> bool operator!=(const ConstBitVectorIterator<T>& a, const ConstBitVectorIterator<T>& b) { return a.byte_ref_ != b.byte_ref_ || a.bit_id_ != b.bit_id_; };
template<class T> bool operator!=(const BitVectorIterator<T>& a, const BitVectorIterator<T>& b) { return a.byte_ref_ != b.byte_ref_ || a.bit_id_ != b.bit_id_; };


template<class T, class Allocator, AllocationPolicyFunc allocPolicy> 
class BitVector
{
    T* data_;
    size_t size_;
    size_t capacity_;
    Allocator allocator_;
    
    static size_t getNumBytes(size_t num_bits)
    {
        return (num_bits + BitBlock<T>::BITS_IN_BLOCK - 1u)/ BitBlock<T>::BITS_IN_BLOCK;
    }
    
    
    void writeByteArray(size_t pos, const T* buffer, size_t len)
    {  
        size_t last_byte = pos / BitBlock<T>::BITS_IN_BLOCK;
        size_t res_bits = pos % BitBlock<T>::BITS_IN_BLOCK;
   
        if (res_bits == 0)
        {//just append the array
            for (size_t i = 0; i < len; i++)
            {
                data_[last_byte + i] =  buffer[i];
            }   
            return;    
        }
        
        size_t compl_bits = BitBlock<T>::BITS_IN_BLOCK - res_bits;
        T prev_byte = data_[last_byte] & (BitBlock<T>::FULL_BYTE >> compl_bits);
        
        for (size_t i = 0; i < len; i++)
        {
            T new_prev_byte = buffer[i] >> compl_bits;
            data_[last_byte + i] = prev_byte | (buffer[i] << res_bits);
            prev_byte = new_prev_byte;
        }
        data_[last_byte + len] = (data_[last_byte + len] & (BitBlock<T>::FULL_BYTE << res_bits)) | prev_byte;
    }
    
    
    void writeBitArray(size_t pos, const T* buffer, size_t len)
    {
		size_t bytes_in_buffer = len/BitBlock<T>::BITS_IN_BLOCK;
		writeByteArray(pos, buffer, bytes_in_buffer);
        size_t res_bits = len % BitBlock<T>::BITS_IN_BLOCK;
		if (res_bits == 0)
		{
			return;
		}
		
		size_t pos_res_bits = pos % BitBlock<T>::BITS_IN_BLOCK;
		size_t pos_byte = pos / BitBlock<T>::BITS_IN_BLOCK;
		T res_byte = buffer[bytes_in_buffer] & ~(BitBlock<T>::FULL_BYTE << res_bits);
		size_t total_res_bits = pos_res_bits + res_bits;
		

		if (total_res_bits == BitBlock<T>::BITS_IN_BLOCK)
		{
			data_[pos_byte + bytes_in_buffer] =  data_[pos_byte + bytes_in_buffer] & ~(BitBlock<T>::FULL_BYTE << pos_res_bits);
			data_[pos_byte + bytes_in_buffer] =  data_[pos_byte + bytes_in_buffer] | (res_byte << pos_res_bits);
		}
		else if (total_res_bits < BitBlock<T>::BITS_IN_BLOCK)
		{
			data_[pos_byte + bytes_in_buffer] =  data_[pos_byte + bytes_in_buffer] & (~(BitBlock<T>::FULL_BYTE << pos_res_bits) | (BitBlock<T>::FULL_BYTE << total_res_bits));
			data_[pos_byte + bytes_in_buffer] =  data_[pos_byte + bytes_in_buffer] | (res_byte << pos_res_bits);
		}
		else
		{
			data_[pos_byte + bytes_in_buffer] =  data_[pos_byte + bytes_in_buffer] & ~(BitBlock<T>::FULL_BYTE << pos_res_bits);
			data_[pos_byte + bytes_in_buffer] =  data_[pos_byte + bytes_in_buffer] | (res_byte << pos_res_bits);
			
			data_[pos_byte + bytes_in_buffer + 1] = data_[pos_byte + bytes_in_buffer + 1] & (BitBlock<T>::FULL_BYTE << (total_res_bits - BitBlock<T>::BITS_IN_BLOCK));
			data_[pos_byte + bytes_in_buffer + 1] = data_[pos_byte + bytes_in_buffer + 1] | (res_byte >> (BitBlock<T>::BITS_IN_BLOCK - pos_res_bits));
		}      
    }
       
public:
    
    T* data() {return data_;};
    const T* data() const {return data_;};
    
    size_t size() const {return size_;};
    size_t sizeBytes() const {return getNumBytes(size_);};
    size_t capacity() const {return capacity_*BitBlock<T>::BITS_IN_BLOCK;};
    
    bool operator[](size_t pos) const { return data_[pos/BitBlock<T>::BITS_IN_BLOCK] & (BitBlock<T>::UNIT_BLOCK << (pos % BitBlock<T>::BITS_IN_BLOCK));};
    BitReference<T> operator[](size_t pos) { return BitReference<T>(&data_[pos/BitBlock<T>::BITS_IN_BLOCK], (pos % BitBlock<T>::BITS_IN_BLOCK));};
    
    size_t count() const
    {
        size_t ct = 0;
        const size_t size_bytes = sizeBytes();
        for (size_t i = 0; i < size_bytes; i++)
        {
            T byte_i = data_[i];
            while (byte_i != 0)
            {
                ct += (byte_i & BitBlock<T>::UNIT_BLOCK);
                byte_i >> 1;
            }
        }    
        return ct;
    }
    
    
    bool all() const
    {
        const size_t size_bytes = sizeBytes();
        if (size_bytes == 0)
        {
            return true;
        }
        
        for (size_t i = 0; i < size_bytes - 1; i++)
        {
            if (data_[i] != BitBlock<T>::FULL_BYTE)
            {
                return false;
            }
        }
        
        T last_byte = data_.back();
        for (size_t i = BitBlock<T>::BITS_IN_BLOCK*(size_in_bytes - 1); i < size_; i++)
        {
            if (!(last_byte & BitBlock<T>::UNIT_BLOCK))
            {
                return false;
            }
            last_byte >> 1;
        }
        return true;   
    }
     
    
    bool none() const
    {
        const size_t size_bytes = sizeBytes();
        if (size_bytes == 0)
        {
            return true;
        }
        
        for (size_t i = 0; i < size_bytes - 1; i++)
        {
            if (data_[i] != 0)
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
        const size_t size_bytes = sizeBytes();
        if (size_bytes == 0)
        {
            return false;
        }
        
        for (size_t i = 0; i < size_bytes - 1; i++)
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
    
    
	bool reserve(size_t new_capacity)
	{
        new_capacity = getNumBytes(new_capacity);
		if (getNumBytes(capacity_) >= new_capacity)
		{
			return true;
		}

		T* new_data = allocator_.allocate(new_capacity);
		if (new_data == nullptr)
		{
			return false;
		}

		memmove(new_data, data_, getNumBytes(size_));
		allocator_.deallocate(data_, capacity_);
		capacity_ = new_capacity;
        data_ = new_data;
		return true;
	}


	bool resize(size_t new_size, bool val = false)
	{
        size_t old_bytes = getNumBytes(size_);
        size_t new_bytes = getNumBytes(new_size);
        size_t res_bits = size_ % BitBlock<T>::BITS_IN_BLOCK;
        size_t new_res_bits = new_size % BitBlock<T>::BITS_IN_BLOCK;
        
		if (new_bytes > old_bytes)
		{
            if (!reserve(new_size))
            {
                return false;
            }
            
            for (size_t i = old_bytes; i < new_bytes; i++)
            {
                data_[i] = val ? BitBlock<T>::FULL_BYTE : 0;
            }
			
			if (val && new_res_bits!=0)
			{
				data_[new_bytes - 1] = ~(BitBlock<T>::FULL_BYTE << new_res_bits);
			}
            
            if (val && res_bits!=0)
            {
                data_[old_bytes - 1] = data_[old_bytes - 1] | (BitBlock<T>::FULL_BYTE << res_bits);
            }                 
        }
		else if (val)
		{
			data_[old_bytes - 1] = data_[old_bytes - 1] | ((BitBlock<T>::FULL_BYTE << res_bits) & ~(BitBlock<T>::FULL_BYTE << new_res_bits));
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
    
    
    void copyToBuffer(T* buffer) const
    {
        size_t size_bytes = getNumBytes(size_);
        for (size_t i = 0; i< size_bytes; i++)
        {
            buffer[i] = data_[i];
        }
    }



    bool copyFromBuffer(const bool* buffer, size_t len)
    {
        if (len == 0)
        {
            clear();
            return;
        }
        
        size_t len_bytes = getNumBytes(len);
        T* new_data = allocator_.allocate(len_bytes);
        if (new_data == nullptr)
        {
            return false;
        }
        allocator_.deallocate(data_);
        size_ = len;
        data_ = new_data;
        data_[len_bytes - 1] = 0;

        for (size_t i = 0; i< size_; i++)
        {
            this->operator[](i) = buffer[i];
        }
        return true;
    }
    
    
    bool copyFromBuffer(const T* buffer, size_t len)
    {
        if (len == 0)
        {
            clear();
            return;
        }
        T* new_data = allocator_.allocate(len);
        if (new_data == nullptr)
        {
            return false;
        }
        allocator_.deallocate(data_);
        size_ = len;
        data_ = new_data;

        memcopy(data_, buffer, len);
        return true;
    }
    
    
    bool pushBack(bool x) 
    {
        if (!reserve(allocPolicy(size_ + 1)))
        {
            return false;
        }
        this->operator[](size_) = x;
        size_++;
        return true;
    }


    template <class Allocator2, AllocationPolicyFunc allocPolicy2>
    bool pushBack(const BitVector<T, Allocator2, allocPolicy2>& x) 
    {
        if (x.size() == 0)
        {
            return true;
        }

        if (!reserve(size_ + x.size()))
        {
            return false;
        }
        writeBitArray(size_, x.data(), x.size());   
        size_+= x.size();
    }


    bool insert(size_t pos, bool  x)
    {
        if (pos > size_)
        {
            return false;
        }
        if (!reserve(size_ + 1))
        {
            return false;
        }

        size_t num_bytes = getNumBytes(size_);
        T insert_byte = pos / BitBlock<T>::BITS_IN_BLOCK;
        size_t insert_bit = pos % BitBlock<T>::BITS_IN_BLOCK;

        T byte_left = (data[insert_byte] >> insert_bit);
       
        size_t bits_left = size_ - pos;
        size_t bits_to_write = BitBlock<T>::BITS_IN_BLOCK - insert_bit;

        if (bits_left > bits_to_write)
        {
            size_t bits_written = insert_byte*BitBlock<T>::BITS_IN_BLOCK;
            writeBitArray(pos+1 + bits_to_write, &data_[insert_byte+1], size_ - bits_written);
            writeBitArray(pos+1, &byte_left, bits_to_write);
        }
        else
        {
             writeBitArray(pos+1, &byte_left, bits_left);
        }
    
        this->operator[](pos) = x;
        size_++;               
        return true;
    }


    template <class Allocator2, AllocationPolicyFunc allocPolicy2>
    bool insert(size_t pos, const BitVector<T, Allocator2, allocPolicy2>& x)
    {
        if (x.size() == 0)
        {
            return 0;
        }
        if (pos > size_)
        {
            return false;
        }
        
        if (!reserve(size_+x.size()))
        {
            return false;
        }


		if (static_cast<void*>(this) == static_cast<const void*>(&x))
		{
			auto y = x;
			return insert(pos, y);
		}

        size_t insert_byte = pos / BitBlock<T>::BITS_IN_BLOCK;
        size_t insert_bit = pos % BitBlock<T>::BITS_IN_BLOCK;
        T byte_left = (data_[insert_byte] >> insert_bit);
       
        size_t bits_left = size_ - pos;
        size_t bits_to_write = BitBlock<T>::BITS_IN_BLOCK - insert_bit;

        if (bits_left > bits_to_write)
        {
            size_t bits_written = insert_byte*BitBlock<T>::BITS_IN_BLOCK;
            writeBitArray(pos+x.size() + bits_to_write, &data_[insert_byte+1], size_ - bits_written);
            writeBitArray(pos+x.size(), &byte_left, bits_to_write);
        }
        else
        {
             writeBitArray(pos+1, &byte_left, bits_left);
        }
        writeBitArray(pos, x.data(), x.size());
        size_ += x.size();    
        return true;        
    }


    bool erase(size_t pos) 
    {
        if (pos >= size_)
        {
            return false;
        }
        if (size_ != pos +1)
        {
            size_t erase_byte = pos / BitBlock<T>::BITS_IN_BLOCK;
            size_t erase_bit = pos % BitBlock<T>::BITS_IN_BLOCK;
            T byte_left = data_[erase_byte] >> (erase_bit +1);

            size_t bits_left = size_ - pos - 1;
            size_t bits_to_write = BitBlock<T>::BITS_IN_BLOCK - erase_bit - 1;
            if (bits_left > bits_to_write)
            {
                size_t bits_written = (erase_byte+1)*BitBlock<T>::BITS_IN_BLOCK;
                writeBitArray(pos + bits_to_write, &data_[erase_byte+1], size_ - bits_written);
                writeBitArray(pos, &byte_left, bits_to_write);
            }
            else
            {
                writeBitArray(pos, &byte_left, bits_left);
            }
        }
        this->operator[](size_ - 1) = false;
        size_--;
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
        
        if (size_ != end)
        {
            size_t end_byte = end / BitBlock<T>::BITS_IN_BLOCK;
            size_t end_bit = end % BitBlock<T>::BITS_IN_BLOCK;
            T byte_left = data_[end_byte] >> (end_bit);

            size_t bits_left = size_ - end;
            size_t bits_to_write = BitBlock<T>::BITS_IN_BLOCK - end_bit;
            if (bits_left > bits_to_write)
            {
                size_t bits_written = (end_byte+1)*BitBlock<T>::BITS_IN_BLOCK;
                writeBitArray(start + bits_to_write, &data_[end_byte+1], size_ - bits_written);
                writeBitArray(start, &byte_left, bits_to_write);
            }
            else
            {
                writeBitArray(start, &byte_left, bits_left);
            }
        }
        size_ -= (end - start);
        size_t res_bits = size_ % BitBlock<T>::BITS_IN_BLOCK;
        if (res_bits != 0)
        {
            size_t last_byte = size_ / BitBlock<T>::BITS_IN_BLOCK;
            data_[last_byte] = data_[last_byte] & ~(BitBlock<T>::FULL_BYTE << res_bits);
        }
		return true;
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
	bool pushFront(const BitVector<Allocator2>& x)
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


	bool shrinkToFit()
	{
		size_t size_bytes = getNumBytes(size_);
		if (size_bytes == capacity_*BitBlock<T>::BITS_IN_BLOCK)
		{
			return true;
		}
		T* new_data = allocator_.allocate(size_);
		if (new_data == nullptr)
		{
			return false;
		}
		memmove(new_data, data_, size_bytes);
		allocator_.deallocate(data_, capacity_);
		capacity_ = size_bytes;
		data_ = new_data;
		return true;
	}
    
    BitVector()
        :size_(0), capacity_(0), data_(nullptr){};

    BitVector(size_t len, bool x = false)
        :BitVector()
    {
        resize(len, x);
    }
    
    
    BitVector(const BitVector& x)
        :BitVector()
    {
        if (resize(x.size()))
        {
            writeBitArray(0, x.data(), x.size());
        }
    }


    BitVector(BitVector&& x)
        :BitVector()
    {
        if (allocator_.is_movable)
        {
            data_ = x.data_;
            size_ x.size_;
            capacity_ = x.capacity_;
            x.size_ =0;
            x.capacity_ = 0;
            x.data_ = nullptr;
        }
        else if (resize(x.size()))
        {
            writeBitArray(0, x.data(), x.size());
            x.clear();
            x.shrinkToFit();
        }
    }

    template<class Allocator2, AllocationPolicyFunc allocPolicy2>
    BitVector(const BitVector<T, Allocator2, allocPolicy2>& x)
        :BitVector()
    {
        if (resize(x.size()))
        {
            writeBitArray(0, x.data(), x.size());
        }
    }


    BitVector(const std::initializer_list<bool> l)
        :BitVector()
    {
        if (resize(l.size()))
        {
            auto it = l.begin();
            for (size_t i = 0; i < size_; i++)
            {
                this->operator[](i) = *(it++);
            }
        }
    }


    BitVector& operator=(const BitVector& x)
    {
        if (this != &x)
        {
            if (!reserve(x.size()))
            {
                return this;
            }
            writeByteArray(0, x.data(), getNumBytes(x.size()));
            size_ = x.size();
        }
        return this;
    }


    BitVector& operator=(BitVector&& x)
    {
        if (this != &x)
        {
            if (allocator_.is_movable)
            {
                allocator_.deallocate(data_, capacity_);
                data_ = x.data_;
                size_ =  x.size_;
                capacity_ = x.capacity_;
                x.size_ =0;
                x.capacity_ = 0;
                x.data_ = nullptr;
            }
            else if (resize(x.size()))
            {
                writeBitArray(0, x.data(), x.size());
                x.clear();
                x.shrinkToFit();
            }   
        }
        return *this;
    }


    template<class Allocator2, AllocationPolicyFunc allocPolicy2>
    BitVector& operator=(const BitVector<T, Allocator2, allocPolicy2>& x)
    {
        if (!reserve(x.size()))
        {
            return this;
        }
        writeByteArray(0, x.data(), getNumBytes(x.size()));
        size_ = x.size();
    }


    BitVector& operator=(std::initializer_list<bool> l)
    {
        if (!resize(l.size()))
        {
            return this;
        }

        auto it = l.begin();
        for (size_t i = 0; i < size_; i++)
        {
            this->operator[](i) = *(it++);
        }
        return this;
    }

    template<class Allocator2, AllocationPolicyFunc allocPolicy2>
    bool operator==(const BitVector<T, Allocator2, allocPolicy2>& x)
    {
        if (size_ != x.size())
        {
            return false;
        }

        size_t num_bytes = getNumBytes(size_);
        T* x_data x.data();
        for (size_t i = 0; i< num_bytes; i++)
        {
            if (data_[i]!=x_data[i])
            {
                return false;
            }
        }
        return true;
    }


    template<class Allocator2, AllocationPolicyFunc allocPolicy2>
    bool operator!=(const BitVector<T, Allocator2, allocPolicy2>& x)
    {
        return !((*this) == x);
    }

    ~BitVector()
    {
        allocator_.deallocate(data_, capacity_);
        size_ = 0;
        capacity_ = 0;
    }

     
    bool  front() const {return this->operator[](0);};
    BitReference<T> front(size_t i) {return this->operator[](0);};

    bool  back() const {return this->operator[](size_ - 1);};
    BitReference<T> back(size_t i) {return this->operator[](size_ - 1);};
    
    typedef BitVectorIterator<T> iterator;
    typedef ConstBitVectorIterator<T> const_iterator;
    
    iterator getIterator(size_t i) {return iterator(&data_[i/BitBlock<T>::BITS_IN_BLOCK], (i % BitBlock<T>::BITS_IN_BLOCK));};
    const iterator getIterator(size_t i) const {return const_iterator(&data_[i/BitBlock<T>::BITS_IN_BLOCK], (i % BitBlock<T>::BITS_IN_BLOCK));};
    
    iterator begin() {return iterator(0);};
    iterator end() {return const_iterator(0);}
    const_iterator begin() const {return iterator(size_);};
    const_iterator end() const {return const_iterator(size);}
};
    
    
template<class T, size_t N>
using StaticBitVector = BitVector<T, FixedSizeAllocator<T, (N + BitBlock<T>::BITS_IN_BLOCK - 1)/BitBlock<T>::BITS_IN_BLOCK>, allocationPolicyFixed>;

template<class T, size_t M>
using ChunkBitVector = BitVector<T, HeapAllocator<T>, allocationPolicyChunk<M>>;


}


#endif