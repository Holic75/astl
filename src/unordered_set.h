#ifndef ASTL_UNORDERED_SET_H
#define ASTL_UNORDERED_SET_H

#include "list.h"
#include "vector.h"
#include "pair.h"
#include "hash.h"


namespace astl 
{

namespace aux
{
template<class Key, class Arena>
struct UnorderedSetBucket
{
	typename List<const Key, Arena>::iterator it;
	size_t count;
};
}


template<class Key, class Arena = HeapArena<ListNode<const Key>>, 
          class Allocator = HeapAllocator<typename UnorderedSetBucket<Key, Arena>>>
class UnorderedSet
{
    List<const Key, Arena> data_;
    Vector<typename UnorderedSetBucket<Key, Arena>, Allocator, allocationPolicyFixed> bins_;//keeps first iterator of next bin
    float max_load_factor_ = 0.75;
    
    static const size_t MIN_HASH_BINS_ = 10;
    
    size_t getBin(const Key& key)
    {
        return hash(key) % (bins_.size() - 1);
    }
    
    
public:
    typedef typename List<const Key, Arena>::iterator iterator;
    typedef typename List<const Key, Arena>::const_iterator const_iterator;
    
    size_t size() {return data_.size();};
    size_t numBins() { return bins_.size() - 1;};
	size_t binCount(size_t bin_id) const { return bins_[bin_id].count; };

    UnorderedMap(size_t min_hash_bins = 10)
        :bins_(min_hash_bins + 1)
    {
        for (size_t i =0; i<bins_.size();i++)
        {
            bins_[i].it = data_.end();
            bins_[i].count = 0;
        }
    };
    
     
    iterator find(const Key& key)
    {
        size_t bin = getBin(key); 
        
        if (bins_[bin].it == data_.end())
        {
            return data_.end();
        }
        
      
        for (auto it = bins_[bin].it; it != bins_[bin+1].it; it++)
        {
            if (*it == key)
            {
                return it;
            }     
        }     
        return data_.end();           
    }     
    
     
    const_iterator find(const Key& key) const
    {
        size_t bin = getBin(key); 
        
        if (bins_[bin].it == data_.end())
        {
            return data_.end();
        }
        
      
        for (auto it = bins_[bin].it; it != bins_[bin+1].it; it++)
        {
            if (*it == key)
            {
                return it;
            }     
        }     
        return data_.end();           
    }
        
        
    iterator insert(const Key& key)
    {
        auto it = find(key);
        if (it == data_.end())
        {
            size_t bin = getBin(key);
			it = data_.emplace(bins_[bin].it, key));
            if (it == end())
            {// failed to insert into the list
                return it;
            }
            bins_[bin].count++;
            auto old_it = bins_[bin];
            for (size_t i = bin + 1; i > 0; i--)
            {
                if (bins_[i - 1] == old_it)
                {
                    bins_[i - 1] = it;
                }
                else
                {
                    break;
                }
            }      
            if (data_.size() > max_load_factor_*numBins())
            {
                rehash(numBins()*2);
            }
        }
        
        return it;
    }
    

	iterator erase(iterator it)
	{
		if (it != data_.end())
		{
			size_t bin = getBin(*it);
			auto old_it = it;
			it = data_.erase(it);
            bins_[bin].count--;
			if (bins_[bin].it == it)
			{ //if it is first element update all corresponding bins
				for (size_t i = bin + 1; i > 0; i--)
				{
					if (bins_[i - 1] == old_it)
					{
						bins_[i - 1] = it;
					}
					else
					{
						break;
					}
				}

			}
		}
		return it;
	}
    
    iterator remove(const Key& key)
    {
        auto it = find(key);
        if (it != data_.end())
        {
			it = erase(it);
        }
        return it;
    }
    
    
    bool contains(const Key& key) const
    {
        return find(key) != data_.end();
    }
    
    
    void clear()
    {
        data_.clear();
        for (size_t i = 0; i < bins_.size(); i++)
        {
            bins_[i].it = data_.end();
        }
    }
    
    
    iterator begin() {return data_.begin();};
    iterator end() {return data_.end();};
    
    const_iterator begin() const {return data_.begin();};
    const_iterator end() const {return data_.end();};
    
    
    UnorderedSet(const UnorderedSet& m)
        :UnorderedSet(m.numBins())
    {
        for (auto it = m.begin(); it != m.end(); it++)
        {
            insert(*it);
        }
    };
    
    template<class Key2,  class Arena2, class Allocator2>
    UnorderedSet(const UnorderedSet<Key2, T2, Arena2, Allocator2>& m)
        :UnorderedSet(m.numBins())
    {
        for (auto it = m.begin(); it != m.end(); it++)
        {
            insert(*it);
        }
    };
    
    UnorderedSet(std::initializer_list<const Key> l)
        :UnorderedSet()
    {
        for (auto it = l.begin(); it != l.end(); it++)
        {
            insert(*it);
        }
    };
    
   
    UnorderedSet& operator=(const UnorderedSet& m)
    {
        if (this != &m)
        {
            clear();
            for (auto it = m.begin(); it != m.end(); it++)
            {
                insert(*it);
            }
        }
        return *this;
    };



	UnorderedSet& operator=(std::initializer_list<const Key> l)
	{
		clear();
		for (auto it = l.begin(); it != l.end(); it++)
		{
			insert(it->first, it->second);
		}
	};
    
    
    template<class Key2,  class Arena2, class Allocator2>
    UnorderedSet& operator=(const UnorderedSet<Key2, Allocator2, Arena2>& m)
    {
        clear();
        for (auto it = m.begin(); it != m.end(); it++)
        {
            insert(it->first, it->second);
        }
        return *this;
    };
    
    
	bool rehash(size_t bucket_count)
	{
		if (!bins_.resize(bucket_count + 1))
		{
			return false;
		}

		List<const Key, Arena> l = std::move(data_);

		for (int i = 0; i < bins_.size(); i++)
		{
			bins_[i].it = data_.end();
			bins_[i].count = 0;
		}

		auto it = l.begin();
		while (it != l.end())
		{
			insert(it->first, std::move(it->second));
			it = l.erase(it);
		}
		return true;
	}
            
};

template<class Key, size_t N, size_t Bins>
using StaticUnorderedSet = UnorderedMap<Key, StaticArena<ListNode<const Key>, N + 2>, 
                            FixedSizeAllocator<typename UnorderedSetBucket<Key, StaticArena<ListNode<const Key>, N + 2>> , 
                            (Bins > 10 ? Bins : 10) + 1>>;
    
}


#endif