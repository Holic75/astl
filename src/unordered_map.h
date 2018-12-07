#ifndef ASTL_UNORDERED_MAP_H
#define ASTL_UNORDERED_MAP_H

#include "list.h"
#include "vector.h"
#include "pair.h"


namespace astl 
{
    
typedef size_t hash_type;

template <class T>
hash_type hash(const T& t)
{
    const uint8_t* ptr =  reinterpret_cast<const uint8_t* >(&t);
    
    hash_type hash = 0x811c9dc5u;
    for (size_t i = 0; i < sizeof(T); i++)
    {
        hash = (hash ^ ptr[i]) * 0x01000193u;
    }
    
    return hash;
}


template<class Key, class T, class Arena = HeapArena<ListNode<Pair<Key,T>>>, 
          class Allocator = HeapAllocator< typename List<Pair<Key,T>, Arena>::iterator>, 
          AllocationPolicyFunc allocPolicy = allocationPolicy2>
class UnorderedMap
{
    List<Pair<Key, T>, Arena> data_;
    Vector<typename List<Pair<Key, T>, Arena>::iterator, Allocator, allocPolicy> bins_;//keeps first iterator of next bin
    
    static const size_t MIN_HASH_BINS_ = 10;
    
    size_t getBin(const Key& key)
    {
        return hash(key) % (bins_.size() - 1);
    }
    
    
public:
    typedef typename List<Pair<Key, T>, Arena>::iterator iterator;
    typedef typename List<Pair<Key, T>, Arena>::const_iterator const_iterator;
    
    size_t size() {return data_.size();};
    size_t numBins() { return bins_.size() - 1;};

    UnorderedMap(size_t min_hash_bins = 10)
        :bins_(min_hash_bins + 1)
    {
        for (size_t i =0; i<bins_.size();i++)
        {
            bins_[i] = data_.end();
        }
    };
    
    
    iterator find(const Key& key)
    {
        size_t bin = getBin(key); 
        
        if (bins_[bin] == data_.end())
        {
            return data_.end();
        }
        
      
        for (auto it = bins_[bin]; it != bins_[bin+1]; it++)
        {
            if (it->first == key)
            {
                return it;
            }     
        }     
        return data_.end();           
    }
    
    
    const_iterator find(const Key& key) const
    {
        size_t bin = getBin(key); 
        
        if (bins_[bin] == data_.end())
        {
            return data_.end();
        }
        
      
        for (auto it = bins_[bin]; it != bins_[bin+1]; it++)
        {
            if (it->key == key)
            {
                return it;
            }     
        }     
        return data_.end();           
    }
        
        
    template <class ...Args>
    iterator emplace(const Key& key, Args&&... args)
    {
        auto it = find(key);
        if (it == data_.end())
        {
            size_t bin = getBin(key);
			it = data_.emplace(bins_[bin], astl::Pair<Key,T>(key, std::forward<Args>(args)... ));
            if (it == end())
            {// failed to insert into the list
                return it;
            }
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
        }
        else
        {
            it->second = T(std::forward<Args>(args)...);
        }
        
        return it;
    }
    
    iterator insert(const Key& key, const T& value)
    {
        return emplace(key, value);
    }
    

	iterator erase(iterator it)
	{
		if (it != data_.end())
		{
			size_t bin = getBin(it->first);
			auto old_it = it;
			it = data_.erase(it);
			if (bins_[bin] == it)
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
    
    
    T& operator[] (const Key& key)
    {
        auto it = find(key);
        if (it == data_.end())
        {
            return  emplace(key, T())->second;
        }
        else
        {
            return it->second;
        }
    }
    
    
    void clear()
    {
        data_.clear();
        for (size_t i = 0; i < bins_.size(); i++)
        {
            bins_[i] = data_.end();
        }
    }
    
    
    iterator begin() {return data_.begin();};
    iterator end() {return data_.end();};
    
    const_iterator begin() const {return data_.begin();};
    const_iterator end() const {return data_.end();};
    
    
    UnorderedMap(const UnorderedMap& m)
        :UnorderedMap(m.numBins())
    {
        for (auto it = m.begin(); it != m.end(); it++)
        {
            insert(m->first, m->second);
        }
    };
    
    template<class Key2, class T2, class Arena2, class Allocator2, AllocationPolicyFunc allocPolicy2>
    UnorderedMap(const UnorderedMap<Key2, T2, Arena2, Allocator2, allocPolicy2>& m)
        :UnorderedMap(m.numBins())
    {
        for (auto it = m.begin(); it != m.end(); it++)
        {
            insert(m->first, m->second);
        }
    };
    
    UnorderedMap(std::initializer_list<Pair<Key, T>> l)
        :UnorderedMap()
    {
        for (auto it = l.begin(); it != l.end(); it++)
        {
            insert(it->first, it->second);
        }
    };
    
   
    UnorderedMap& operator=(const UnorderedMap& m)
    {
        if (this != &m)
        {
            clear();
            for (auto it = m.begin(); it != m.end(); it++)
            {
                insert(m->first, m->second);
            }
        }
        return *this;
    };



	UnorderedMap& operator=(std::initializer_list<Pair<Key, T>> l)
	{
		clear();
		for (auto it = l.begin(); it != l.end(); it++)
		{
			insert(it->first, it->second);
		}
	};
    
    
    template<class Key2, class T2, class Arena2, class Allocator2, AllocationPolicyFunc allocPolicy2>
    UnorderedMap& operator=(const UnorderedMap<Key2, T2, Allocator2, Arena2, allocPolicy2>& m)
    {
        clear();
        for (auto it = m.begin(); it != m.end(); it++)
        {
            insert(m->first, m->second);
        }
        return *this;
    };
            
};

template<class Key, class T, size_t N, size_t Bins>
using StaticUnorderedMap = UnorderedMap<Key, T, StaticArena<ListNode<Pair<Key,T>>, N + 2>, 
                            FixedSizeAllocator<typename StaticList<Pair<Key,T>, N>::iterator, Bins + 1>, 
                            allocationPolicyFixed>;
    
}


#endif