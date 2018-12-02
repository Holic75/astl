#ifndef ASTL_UNORDERED_MAP_H
#define ASTL_UNORDERED_MAP_H

#include "list.h"
#include "pair.h"


namespace astl 
{
    
typedef uint8_t hash_type;

template class<T>
hash_type hash(const T& t)
{
    hash_type* ptr =  reinterpret_cast<uint8_t* const>(&t);
    
    hash_type hash = 0;
    for (size_t i = 0; i < sizeof(T); i++)
    {
        hash = hash ^ ptr[i];
    }
    
    return hash;
}

template<class Key, class T, class Allocator = HeapAllocator2<List<Pair<Key,T>>::iterator>, class Arena = HeapArena<ListNode<Pair<Key,T>>>, AllocationPolicyFunc allocPolicy = allocationPolicy2>
class UnorderedMap
{
    List<Pair<Key, T>, Arena> data_;
    Vector<List<Pair<Key, T>, Arena>, Allocator, allocPolicy> bins_;//keeps first iterator of next bin
    
    static const MIN_HASH_BINS_ = 10;
    
    size_t getBin(const Key& key)
    {
        return hash(key) % (bins_.size() - 1);
    }
    
    
public:
    typedef List<Pair<Key, T>, Arena>::iterator iterator;
    typedef List<Pair<Key, T>, Arena>::const_iterator const_iterator;
    
    size_t size() {return data_.size();};
    
    UnorderedMap()
        :bins_(MIN_HASH_BINS_ + 1)
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
            if (it->key == key)
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
    iterator emplace(const Key& key, Args... args)
    {
        auto it = find(key);
        if (it == data_.end())
        {
            size_t bin = getBin(key);
            it = data_.emplace(bins_[bin], args);  
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
            it->value = T(args);
        }
        
        return it;
    }
    
    iterator insert(const Key& key, const T& value)
    {
        return emplace(key, value);
    }
    
    
    iterator remove(const Key& key)
    {
        auto it = find(key);
        if (it != data_.end())
        {
            size_t bin = getBin(key);
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
    
    
    T& operator[] (const Key& key)
    {
        auto it = find(key);
        if (it == data_.end())
        {
            return  emplace(key)->second;
        }
        else
        {
            return it->second();
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
        :UnorderedMap()
    {
        for (auto it = m.begin(); it != m.end(); it++)
        {
            insert(m->first, m->second);
        }
    };
    
    template<class Key2, class T2, class Allocator2, class Arena2, AllocationPolicyFunc allocPolicy2>
    UnorderedMap(const UnorderedMap<Key2, T2, Allocator2, Arena2, allocPolicy2>& m)
        :UnorderedMap()
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
            insert(l->first, l->second);
        }
    };
    
   
    UnorderedMap& operator=(const UnorderedMap& m)
        :UnorderedMap()
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
    
    
    template<class Key2, class T2, class Allocator2, class Arena2, AllocationPolicyFunc allocPolicy2>
    UnorderedMap& operator=(const UnorderedMap<Key2, T2, Allocator2, Arena2, allocPolicy2>& m)
        :UnorderedMap()
    {
        clear();
        for (auto it = m.begin(); it != m.end(); it++)
        {
            insert(m->first, m->second);
        }
        return *this;
    };
            

template<class Key, class T, size_t N>
using StaticUnorderedMap = UnorderedMap<Key, T, StaticAllocator<List<Pair<Key,T>>::iterator, N>, StaticArena<ListNode<Pair<Key,T>>, N>, allocationPolicyFixed>;
    
}

}

#endif