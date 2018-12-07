#ifndef ASTL_LIST_H
#define ASTL_LIST_H

#include "arena.h"
#include "initializer_list.h"
#include "type_traits.h"

namespace astl
{
    
template <class T> class ListNode;
template <class T> class ConstListNodeIterator;
template <class T> class ListNodeIterator;
template<class T, class Arena = HeapArena<ListNode<T>>> class List;


template <class T>
class ListNode
{
private:
    ListNode* prev;
    ListNode* next;
public:
     T   value;

   ListNode(ListNode* prv, ListNode* nxt, const T& val)
        : value(val), prev(prv), next(nxt){};
    ListNode(const T& val)
        :value(val), prev(nullptr), next(nullptr){};
    ListNode()
        :prev(nullptr), next(nullptr){};
template<class ...Args>
    ListNode(ListNode* prv, ListNode* nxt, Args... args)
        : value(std::forward<Args>(args)...), prev(prev), next(nxt){};
        
   template<class, class>  friend  class List;
   template<class>  friend  class ListNodeIterator;
   template<class>  friend  class ConstListNodeIterator;
};


template <class T>
class ListNodeIterator
{
   ListNode<T>* node_; 
public:
   T* operator->() {return &(node_->value);};
   friend const T& operator *(const ListNodeIterator &it)
   {
       return it.node_->value;
   }
   
   friend T& operator *(ListNodeIterator &it)
   {
       return it.node_->value;
   }
   
   ListNodeIterator& operator++ ()
   {
       node_ = (node_ == nullptr) ? nullptr : node_->next;
       return *this;
   }
   
   ListNodeIterator operator++ (int)
   {
       ListNodeIterator old = *this;
       ++(*this);
       return old;
   }
   
   ListNodeIterator& operator-- ()
   {
       node_ = (node_ == nullptr) == nullptr ? nullptr : node_->prev;
       return *this;
   }
   
   ListNodeIterator operator-- (int)
   {
       ListNodeIterator old = *this;
       --(*this);
       return old;
   }
   
   bool operator==(const ListNodeIterator<T>& b) { return node_ == b.node_; };
   bool operator!=(const ListNodeIterator<T>& b) { return node_ != b.node_; };
   bool operator==(const ConstListNodeIterator<T>& b) { return node_ == b.node_; };
   bool operator!=(const ConstListNodeIterator<T>& b) { return node_ != b.node_; };

   ListNodeIterator(const ListNodeIterator& node) = default;
   ListNodeIterator()
    :node_(nullptr) {};

   template<class, class> friend class List;    
   template<class> friend class ConstListNodeIterator;
private:
   ListNodeIterator(ListNode<T>* node)
      :node_(node){};
      
};



template <class T>
class ConstListNodeIterator
{
   ListNode<T>* node_; 
public:
   const T*  operator->() {return &(node_->value);};
   friend const T& operator *(const ConstListNodeIterator &it)
   {
       return it->value;
   }
   
   
   ConstListNodeIterator& operator++ ()
   {
       node_ = (node_ == nullptr) ? nullptr : node_->next;
       return *this;
   }
   
   ConstListNodeIterator operator++ (int)
   {
       ConstListNodeIterator old = *this;
       ++(*this);
       return old;
   }
   
   ConstListNodeIterator& operator-- ()
   {
       node_ = (node_ == nullptr) == nullptr ? nullptr : node_->prev;
       return *this;
   }
   
   ConstListNodeIterator operator-- (int)
   {
       ConstListNodeIterator old = *this;
       --(*this);
       return old;
   }
   

   bool operator==(const ListNodeIterator<T>& b) { return node_ == b.node_; };
   bool operator!=(const ListNodeIterator<T>& b) { return node_ != b.node_; };
   bool operator==(const ConstListNodeIterator<T>& b) { return node_ == b.node_; };
   bool operator!=(const ConstListNodeIterator<T>& b) { return node_ != b.node_; };


   ConstListNodeIterator(const ConstListNodeIterator& node) = default;

   ConstListNodeIterator(const ListNodeIterator<T>& node)
	   :node_(node.node_) {};

   ConstListNodeIterator()
    :node_(nullptr) {};
   template<class, class> friend class List;    
   template<class> friend class ListNodeIterator;
private:
   ConstListNodeIterator(ListNode<T>* node)
      :node_(node){};
      
};


template<class T> bool operator==(const ListNodeIterator<T>& a, const ListNodeIterator<T>& b) { return a.node_ == b.node_; };
template<class T> bool operator==(const ListNodeIterator<T>& a, const ConstListNodeIterator<T>& b) { return a.node_ == b.node_; };
template<class T> bool operator==(const ConstListNodeIterator<T>& a, const ListNodeIterator<T>& b) { return a.node_ == b.node_; };
template<class T> bool operator==(const ConstListNodeIterator<T>& a, const ConstListNodeIterator<T>& b) { return a.node_ == b.node_; };
template<class T> bool operator!=(const ConstListNodeIterator<T>& a, const ListNodeIterator<T>& b) { return a.node_ != b.node_; };
template<class T> bool operator!=(const ListNodeIterator<T>& a, const ConstListNodeIterator<T>& b) { return a.node_ != b.node_; };
template<class T> bool operator!=(const ConstListNodeIterator<T>& a, const ConstListNodeIterator<T>& b) { return a.node_ != b.node_; };
template<class T> bool operator!=(const ListNodeIterator<T>& a, const ListNodeIterator<T>& b) { return a.node_ != b.node_; };

template <class T, class Arena>
class List
{
    Arena arena_;
    ListNode<T>* head_;
    ListNode<T>* end_;
    size_t size_;

	void initialize()
	{
		end_ = arena_.create();
		head_ = arena_.create();
		head_->next = end_;
		end_->prev = head_;
		size_ = 0;
	}
    
public:
    
	size_t size() const { return size_; };
    List()
        :arena_(), size_(0)
    {
		initialize();
    }

    typedef ListNodeIterator<T> iterator;
    typedef ConstListNodeIterator<T> const_iterator;
    
    iterator begin() {return head_->next;};
    iterator end() {return end_;};
    const_iterator begin() const {return head_->next;};
    const_iterator end() const {return end_;};
    
    
    template<class ...Args>
    iterator emplace(const_iterator it, Args&&... args)
    {

        ListNode<T>* new_node = arena_.create(it.node_->prev , it.node_, std::forward<Args>(args)...);
        if (new_node == nullptr)
        {
            return end_;
        }
        new_node->prev->next = new_node;
        new_node->next->prev = new_node;
        size_++;
		return new_node;
    }
    
    iterator insert(const_iterator it, const T& val)
    {
        return emplace(it, val);
    }
  
  
    template<class X, class Arena2>
    iterator insert(const_iterator it, const List<X, Arena2>& l)
    {  
        for (auto itl = l.begin(); itl != itl.end(); itl++)
        {
            insert(it, *itl);
        }

		return it;
    }
    
    
    template<class ...Args>
    iterator emplaceBack(Args&&... args)
    {        
        return emplace(end_, std::forward<Args>(args)...);
    }
    
    iterator pushBack(const T& val)
    {
        return emplaceBack(val);
    }
    
    template<class ...Args>
    iterator emplaceFront(Args&&... args)
    {        
        return emplace(head_->next, std::forward<Args>(args)...);
    }
    
    iterator pushFront(const T& val)
    {
        return emplaceFront(val);
    }
    
    iterator erase(const_iterator it)
    {
        if (size_ == 0 || it.node_ == end_)
        {
            return end_;
        }
        iterator next_it = it.node_->next;
        it.node_->prev->next = it.node_->next;
        it.node_->next->prev = it.node_->prev;
        
        arena_.destroy(it.node_);
        size_--;
        return next_it;
    }
    
    iterator popBack()
    {
        return erase(end_->prev);
    }
    
    iterator popFront()
    {
        return erase(head_->next);
    }
    
    void clear()
    {

        auto node_ptr = head_->next;
        while(node_ptr != end_)
        {
            node_ptr = node_ptr->next;
            arena_.destroy(node_ptr->prev);
        }       
        head_->next = end_;
        end_->prev = head_;
        size_ = 0;
    }
    
      
    List(const List& l)
        :List()
    {
        insert(end_, l);
    }
    
    template<class X, class Arena2>
    List(const List<X, Arena2>& l)
        :List()
    {
        insert(end_, l);
    }


	List(List&& l)
	{
		if (arena_.is_movable)
		{
			head_ = l.head_;
			end_ = l.end_;
			size_ = l.size_;
			l.initialize();
		}
		else
		{
			initialize();
            auto it = l.begin();
            while( it != l.end())
            {
                insert(end_, *it);
                it = l.erase(it);
            }          
		}
	}
    
    
      
    List(std::initializer_list<T> l)
        :List()
    {
		for (auto it = l.begin(); it != l.end(); it++)
        {
            pushBack(*it);
        }
    }
    

    List& operator=(const List& l)
    {
        if (this != l)
        {
            clear();
            insert(end_, l);
        }
        return this;
    }


	List& operator=(List&& l)
	{
		if (this == l)
		{
			return this;
		}

		if (arena_.is_movable)
		{
			head_ = l.head_;
			end_ = l.end_;
			size_ = l.size_;
			l.initialize();
            return this;
		}

		if (this != l)
		{
			clear();
            auto it = l.begin();
            while( it != l.end())
            {
                insert(end_, *it);
                it = l.erase(it);
            }
		}
		return this;
	}



    
    template<class X, class Arena2>
    List& operator=(const List<X, Arena2>& l)
    {
        clear();
        insert(end_, l);
        return this;
    }


	List& operator=(std::initializer_list<T> l)
	{
		clear();
		for (auto it = l.begin(); it != l.end(); it++)
		{
			pushBack(*it);
		}
	}
    
    
    ~List()
    {
        clear();
        arena_.destroy(head_);
        arena_.destroy(end_);
    }
    
};

template<class T, size_t N>
using StaticList = List<T, StaticArena<ListNode<T>, N + 2>>;


}




#endif