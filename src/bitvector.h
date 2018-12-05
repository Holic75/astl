#ifndef ASTL_BITVECTOR_H
#define ASTL_BITVECTOR_H
#include "vector.h"

namespace astl
{
constexpr uint8_t BITS_IN_BYTE = sizeof(uint8_t)*8;
constexpr uint8_t UNIT_BYTE = 1;
constexpr uint8_t FULL_BYTE = ~static_cast<uint8_t>(0);
template<class Allocator = HeapAllocator<uint8_t>, AllocationPolicyFunc allocPolicy = allocationPolicy2> class BasicBitVector;


class BitVectorIterator;
class ConstBitVectorIterator;



class BitReference
{
    uint8_t* byte_ref_;
    const uint8_t bit_id_;

public:

    
    operator bool () const {return ((*byte_ref_) & (UNIT_BYTE << bit_id_)) > 0;};
    bool operator ~() const {return !(*this);};
    
    BitReference& operator=(bool b) 
    {
        if (b)
        {
            *byte_ref_ = (*byte_ref_) | (UNIT_BYTE << bit_id_);
        }
        else
        {
            *byte_ref_ = (*byte_ref_) & ~(UNIT_BYTE << bit_id_);    
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

    
    operator bool () const {return ((*byte_ref_) & (UNIT_BYTE << bit_id_)) > 0;};
    bool operator ~() const {return !(*this);};
       
    ConstBitReference(const ConstBitReference& b) = default;
    ConstBitReference(const uint8_t * const byte_ref, uint8_t bit_id)
        :byte_ref_(byte_ref), bit_id_(bit_id){};
    
};


bool operator==(const BitReference& a, const BitReference& b) { return static_cast<bool>(a) == static_cast<bool>(b); };
bool operator!=(const BitReference& a, const BitReference& b) { return static_cast<bool>(a) != static_cast<bool>(b); };
bool operator==(const ConstBitReference& a, const BitReference& b) { return static_cast<bool>(a) == static_cast<bool>(b); };
bool operator==(const BitReference& a, const ConstBitReference& b) { return static_cast<bool>(a) == static_cast<bool>(b); };
bool operator!=(const ConstBitReference& a, const BitReference& b) { return static_cast<bool>(a) != static_cast<bool>(b); };
bool operator!=(const BitReference& a, const ConstBitReference& b) { return static_cast<bool>(a) != static_cast<bool>(b); };
bool operator==(const ConstBitReference& a, const ConstBitReference& b) { return static_cast<bool>(a) == static_cast<bool>(b); };
bool operator!=(const ConstBitReference& a, const ConstBitReference& b) { return static_cast<bool>(a) != static_cast<bool>(b); };

class BitVectorIterator
{
    uint8_t* byte_ref_;
    uint8_t bit_id_;
   
public:
  
    BitVectorIterator& operator++ ()
    {
       bit_id_++;
       
       if (bit_id_ == BITS_IN_BYTE)
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
           bit_id_ = BITS_IN_BYTE - 1;
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
    
    BitVectorIterator(uint8_t* byte_ref, uint8_t bit_id)
        :byte_ref_(byte_ref), bit_id_(bit_id) {};
    
    friend BitReference operator *(const BitVectorIterator &b)
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


class ConstBitVectorIterator
{
    const uint8_t* byte_ref_;
    uint8_t bit_id_;
    
public:  
    ConstBitVectorIterator& operator++ ()
    {
       bit_id_++;
       
       if (bit_id_ == BITS_IN_BYTE)
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
           bit_id_ = BITS_IN_BYTE - 1;
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
    friend bool operator==(const ConstBitVectorIterator& a, const ConstBitVectorIterator& b);
    friend bool operator!=(const ConstBitVectorIterator& a, const ConstBitVectorIterator& b);
    friend bool operator==(const BitVectorIterator& a, const ConstBitVectorIterator& b);  
    friend bool operator==(const ConstBitVectorIterator& a, const BitVectorIterator& b);
    friend bool operator!=(const BitVectorIterator& a, const ConstBitVectorIterator& b);  
    friend bool operator!=(const ConstBitVectorIterator& a, const BitVectorIterator& b);       
};


bool operator==(const BitVectorIterator& a, const BitVectorIterator& b) { return a.byte_ref_ == b.byte_ref_ && a.bit_id_ == b.bit_id_; };
bool operator==(const BitVectorIterator& a, const ConstBitVectorIterator& b) { return a.byte_ref_ == b.byte_ref_ && a.bit_id_ == b.bit_id_; };
bool operator==(const ConstBitVectorIterator& a, const BitVectorIterator& b) { return a.byte_ref_ == b.byte_ref_ && a.bit_id_ == b.bit_id_; };
bool operator==(const ConstBitVectorIterator& a, const ConstBitVectorIterator& b) { return a.byte_ref_ == b.byte_ref_ && a.bit_id_ == b.bit_id_; };
bool operator!=(const ConstBitVectorIterator& a, const BitVectorIterator& b) { return a.byte_ref_ != b.byte_ref_ || a.bit_id_ != b.bit_id_; };
bool operator!=(const BitVectorIterator& a, const ConstBitVectorIterator& b) { return a.byte_ref_ != b.byte_ref_ || a.bit_id_ != b.bit_id_; };
bool operator!=(const ConstBitVectorIterator& a, const ConstBitVectorIterator& b) { return a.byte_ref_ != b.byte_ref_ || a.bit_id_ != b.bit_id_; };
bool operator!=(const BitVectorIterator& a, const BitVectorIterator& b) { return a.byte_ref_ != b.byte_ref_ || a.bit_id_ != b.bit_id_; };


template<class Allocator, AllocationPolicyFunc allocPolicy> 
class BasicBitVector
{
    uint8_t* data_;
    size_t size_;
    size_t capacity_;
    Allocator allocator_;
    
    static size_t getNumBytes(size_t num_bits)
    {
        return (num_bits + BITS_IN_BYTE - 1u)/ BITS_IN_BYTE;
    }
    
    
    void writeByteArray(size_t pos, const uint8_t* buffer, size_t len)
    {  
        size_t last_byte = pos / BITS_IN_BYTE;
        uint8_t res_bits = pos % BITS_IN_BYTE;
   
        if (res_bits == 0)
        {//just append the array
            for (size_t i = 0; i < len; i++)
            {
                data_[last_byte + i] =  buffer[i];
            }   
            return;    
        }
        
        uint8_t compl_bits = BITS_IN_BYTE - res_bits;
        uint8_t prev_byte = data_[last_byte] & (FULL_BYTE >> compl_bits);
        
        for (size_t i = 0; i < len; i++)
        {
            uint8_t new_prev_byte = buffer[i] >> compl_bits;
            data_[last_byte + i] = prev_byte | (buffer[i] << res_bits);
            prev_byte = new_prev_byte;
        }
        data_[last_byte + len] = (data_[last_byte + len] & (FULL_BYTE << res_bits)) | prev_byte;
    }
    
    
    void writeBitArray(size_t pos, const uint8_t* buffer, size_t len)
    {
		size_t bytes_in_buffer = len/BITS_IN_BYTE;
		writeByteArray(pos, buffer, bytes_in_buffer);
        uint8_t res_bits = len % BITS_IN_BYTE;
		if (res_bits == 0)
		{
			return;
		}
		
		uint8_t pos_res_bits = pos % BITS_IN_BYTE;
		size_t pos_byte = pos / BITS_IN_BYTE;
		uint8_t res_byte = buffer[bytes_in_buffer] & ~(FULL_BYTE << res_bits);
		uint8_t total_res_bits = pos_res_bits + res_bits;
		

		if (total_res_bits == BITS_IN_BYTE)
		{
			data_[pos_byte + bytes_in_buffer] =  data_[pos_byte + bytes_in_buffer] & ~(FULL_BYTE << pos_res_bits);
			data_[pos_byte + bytes_in_buffer] =  data_[pos_byte + bytes_in_buffer] | (res_byte << pos_res_bits);
		}
		else if (total_res_bits < BITS_IN_BYTE)
		{
			data_[pos_byte + bytes_in_buffer] =  data_[pos_byte + bytes_in_buffer] & (~(FULL_BYTE << pos_res_bits) | (FULL_BYTE << total_res_bits));
			data_[pos_byte + bytes_in_buffer] =  data_[pos_byte + bytes_in_buffer] | (res_byte << pos_res_bits);
		}
		else
		{
			data_[pos_byte + bytes_in_buffer] =  data_[pos_byte + bytes_in_buffer] & ~(FULL_BYTE << pos_res_bits);
			data_[pos_byte + bytes_in_buffer] =  data_[pos_byte + bytes_in_buffer] | (res_byte << pos_res_bits);
			
			data_[pos_byte + bytes_in_buffer + 1] = data_[pos_byte + bytes_in_buffer + 1] & (FULL_BYTE << (total_res_bits - BITS_IN_BYTE));
			data_[pos_byte + bytes_in_buffer + 1] = data_[pos_byte + bytes_in_buffer + 1] | (res_byte >> (BITS_IN_BYTE - pos_res_bits));
		}      
    }
       
public:
    
    uint8_t* data() {return data_;};
    const uint8_t* data() const {return data_;};
    
    size_t size() const {return size_;};
    size_t sizeBytes() const {return getNumBytes(size_);};
    size_t capacity() const {return capacity_*BITS_IN_BYTE;};
    
    bool operator[](size_t pos) const { return data_[pos/BITS_IN_BYTE] & (UNIT_BYTE << (pos % BITS_IN_BYTE));};
    BitReference operator[](size_t pos) { return BitReference(&data_[pos/BITS_IN_BYTE], (pos % BITS_IN_BYTE));};
    
    size_t count() const
    {
        size_t ct = 0;
        const size_t size_bytes = sizeBytes();
        for (size_t i = 0; i < size_bytes; i++)
        {
            uint8_t byte_i = data_[i];
            while (byte_i != 0)
            {
                ct += (byte_i & UNIT_BYTE);
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
            if (data_[i] != FULL_BYTE)
            {
                return false;
            }
        }
        
        uint8_t last_byte = data_.back();
        for (size_t i = BITS_IN_BYTE*(size_in_bytes - 1); i < size_; i++)
        {
            if (!(last_byte & UNIT_BYTE))
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

		uint8_t* new_data = allocator_.allocate(new_capacity);
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
        size_t res_bits = size_ % BITS_IN_BYTE;
        size_t new_res_bits = new_size % BITS_IN_BYTE;
        
		if (new_bytes > old_bytes)
		{
            if (!reserve(new_size))
            {
                return false;
            }
            
            for (size_t i = old_bytes; i < new_bytes; i++)
            {
                data_[i] = val ? FULL_BYTE : 0;
            }
			
			if (val && new_res_bits!=0)
			{
				data_[new_bytes - 1] = ~(FULL_BYTE << new_res_bits);
			}
            
            if (val && res_bits!=0)
            {
                data_[old_bytes - 1] = data_[old_bytes - 1] | (FULL_BYTE << res_bits);
            }                 
        }
		else if (val)
		{
			data_[old_bytes - 1] = data_[old_bytes - 1] | ((FULL_BYTE << res_bits) & ~(FULL_BYTE << new_res_bits));
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
    
    
    void copyToBuffer(uint8_t* buffer) const
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
        uint8_t* new_data = allocator_.allocate(len_bytes);
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
    
    
    bool copyFromBuffer(const uint8_t* buffer, size_t len)
    {
        if (len == 0)
        {
            clear();
            return;
        }
        uint8_t* new_data = allocator_.allocate(len);
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
    bool pushBack(const BasicBitVector< Allocator2, allocPolicy2>& x) 
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
        uint8_t insert_byte = pos / BITS_IN_BYTE;
        uint8_t insert_bit = pos % BITS_IN_BYTE;

        uint8_t byte_left = (data[insert_byte] >> insert_bit);
       
        uint8_t bits_left = size_ - pos;
        uint8_t bits_to_write = BITS_IN_BYTE - insert_bit;

        if (bits_left > bits_to_write)
        {
            size_t bits_written = insert_byte*BITS_IN_BYTE;
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
    bool insert(size_t pos, const BasicBitVector<Allocator2, allocPolicy2>& x)
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

        size_t insert_byte = pos / BITS_IN_BYTE;
        uint8_t insert_bit = pos % BITS_IN_BYTE;
        uint8_t byte_left = (data_[insert_byte] >> insert_bit);
       
        size_t bits_left = size_ - pos;
        uint8_t bits_to_write = BITS_IN_BYTE - insert_bit;

        if (bits_left > bits_to_write)
        {
            size_t bits_written = insert_byte*BITS_IN_BYTE;
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
            size_t erase_byte = pos / BITS_IN_BYTE;
            uint8_t erase_bit = pos % BITS_IN_BYTE;
            uint8_t byte_left = data_[erase_byte] >> (erase_bit +1);

            size_t bits_left = size_ - pos - 1;
            uint8_t bits_to_write = BITS_IN_BYTE - erase_bit - 1;
            if (bits_left > bits_to_write)
            {
                size_t bits_written = (erase_byte+1)*BITS_IN_BYTE;
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
            size_t end_byte = end / BITS_IN_BYTE;
            uint8_t end_bit = end % BITS_IN_BYTE;
            uint8_t byte_left = data_[end_byte] >> (end_bit);

            size_t bits_left = size_ - end;
            uint8_t bits_to_write = BITS_IN_BYTE - end_bit;
            if (bits_left > bits_to_write)
            {
                size_t bits_written = (end_byte+1)*BITS_IN_BYTE;
                writeBitArray(start + bits_to_write, &data_[end_byte+1], size_ - bits_written);
                writeBitArray(start, &byte_left, bits_to_write);
            }
            else
            {
                writeBitArray(start, &byte_left, bits_left);
            }
        }
        size_ -= (end - start);
        uint8_t res_bits = size_ % BITS_IN_BYTE;
        if (res_bits != 0)
        {
            size_t last_byte = size_ / BITS_IN_BYTE;
            data_[last_byte] = data_[last_byte] & ~(FULL_BYTE << res_bits);
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


	bool shrinkToFit()
	{
		size_t size_bytes = getNumBytes(size_);
		if (size_bytes == capacity_*BITS_IN_BYTE)
		{
			return true;
		}
		uint8_t* new_data = allocator_.allocate(size_);
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
    
    BasicBitVector()
        :size_(0), capacity_(0), data_(nullptr){};

    BasicBitVector(size_t len, bool x = false)
        :BasicBitVector()
    {
        resize(len, x);
    }
    
    
    BasicBitVector(const BasicBitVector& x)
        :BasicBitVector()
    {
        if (resize(x.size()))
        {
            writeBitArray(0, x.data(), x.size());
        }
    }


    BasicBitVector(BasicBitVector&& x)
        :BasicBitVector()
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
    BasicBitVector(const BasicBitVector<Allocator2, allocPolicy2>& x)
        :BasicBitVector()
    {
        if (resize(x.size()))
        {
            writeBitArray(0, x.data(), x.size());
        }
    }


    BasicBitVector(const std::initializer_list<bool> l)
        :BasicBitVector()
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


    BasicBitVector& operator=(const BasicBitVector& x)
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


    BasicBitVector& operator=(BasicBitVector&& x)
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
    BasicBitVector& operator=(const BasicBitVector<Allocator2, allocPolicy2>& x)
    {
        if (!reserve(x.size()))
        {
            return this;
        }
        writeByteArray(0, x.data(), getNumBytes(x.size()));
        size_ = x.size();
    }


    BasicBitVector& operator=(std::initializer_list<bool> l)
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
    bool operator==(const BasicBitVector<Allocator2, allocPolicy2>& x)
    {
        if (size_ != x.size())
        {
            return false;
        }

        size_t num_bytes = getNumBytes(size_);
        uint8_t* x_data x.data();
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
    bool operator!=(const BasicBitVector<Allocator2, allocPolicy2>& x)
    {
        return !((*this) == x);
    }

    ~BasicBitVector()
    {
        allocator_.deallocate(data_, capacity_);
        size_ = 0;
        capacity_ = 0;
    }

     
    bool  front() const {return this->operator[](0);};
    BitReference front(size_t i) {return this->operator[](0);};

    bool  back() const {return this->operator[](size_ - 1);};
    BitReference back(size_t i) {return this->operator[](size_ - 1);};
    
    typedef BitVectorIterator iterator;
    typedef ConstBitVectorIterator const_iterator;
    
    iterator getIterator(size_t i) {return iterator(&data_[i/BITS_IN_BYTE], (i % BITS_IN_BYTE));};
    const iterator getIterator(size_t i) const {return const_iterator(&data_[i/BITS_IN_BYTE], (i % BITS_IN_BYTE));};
    
    iterator begin() {return iterator(0);};
    iterator end() {return const_iterator(0);}
    const_iterator begin() const {return iterator(size_);};
    const_iterator end() const {return const_iterator(size);}
};
    
    
template<size_t N>
using StaticBitVector = BasicBitVector<FixedSizeAllocator<uint8_t, (N + BITS_IN_BYTE - 1)/BITS_IN_BYTE>, allocationPolicyFixed>;

template<size_t M>
using ChunkBitVector = BasicBitVector<HeapAllocator<uint8_t>, allocationPolicyChunk<M>>;

typedef BasicBitVector<> BitVector;
}


#endif