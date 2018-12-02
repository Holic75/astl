#ifndef ASTL_LIST_H
#define ASTL_LIST_H

#include "arena.h"
#include "initializer_list.h"
namespace astl
{
    
template <class T> class ListNode;
template<class T, class Arena = HeapArena<ListNode<T>>> class List;
template <class T>
class ListNode
{
private:
    Node* prev;
    Node* next;
public:
     T   value;

    Node(Node* prv, Node* nxt, const T& val)
        : value(val), prev(prev), next(nxt){};
    Node(const T& val)
        :value(val), prev(nullptr), next(nullptr){};
    Node()
        :prev(nullptr), next(nullptr){};
template<class ...Args>
    Node(Node* prv, Node* nxt, Args... args)
        : value(args), prev(prev), next(nxt){};
        
    friend template<class, class> List;    

};

template <class T>
class ConstListNodeIterator;
template <class T>
class ListNodeIterator;

bool operator==(const ListNodeIterator& a, const ListNodeIterator& b) { return a.node_ == b.node_; };
bool operator==(const ListNodeIterator& a, const ConstListNodeIterator& b) { return a.node_ == b.node_; };
bool operator==(const ConstListNodeIterator& a, const ListNodeIterator& b) { return a.node_ == b.node_; };
bool operator==(const ConstListNodeIterator& a, const ConstListNodeIterator& b) { return a.node_ == b.node_; };
bool operator!=(const ConstListNodeIterator& a, const ListNodeIterator& b) { return a.node_ != b.node_; };
bool operator!=(const ListNodeIterator& a, const ConstListNodeIterator& b) { return a.node_ != b.node_; };;
bool operator!=(const ConstListNodeIterator& a, const ConstListNodeIterator& b) { return a.node_ != b.node_; };;
bool operator!=(const ListNodeIterator& a, const ListNodeIterator& b) { return a.node_ != b.node_; };;

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
       node_ = (node_ == nullptr || node_->prev) ? nullptr : node_->next;
       return *this;
   }
   
   ListNodeIterator operator++ (int)
   {
       NodeIterator old = *this;
       ++(*this);
       return old;
   }
   
   ListNodeIterator& operator-- ()
   {
       node_ = (node_ == nullptr || node_->prev) == nullptr ? nullptr : node_->prev;
       return *this;
   }
   
   ListNodeIterator operator-- (int)
   {
       NodeIterator old = *this;
       --(*this);
       return old;
   }
   
   friend bool operator==(const ListNodeIterator& a, const ListNodeIterator& b);
   friend bool operator==(const ListNodeIterator& a, const ConstListNodeIterator& b);
   friend bool operator==(const ConstListNodeIterator& a, const ListNodeIterator& b);
   friend bool operator!=(const ConstListNodeIterator& a, const ListNodeIterator& b);
   friend bool operator!=(const ListNodeIterator& a, const ConstListNodeIterator& b);
   friend bool operator!=(const ListNodeIterator& a, const ListNodeIterator& b);

   ListNodeIterator(const ListNodeIterator& node) = default;
   ListNodeIterator(const ListNodeIterator& node)
    :node_(nullptr) {};
   friend template<class, class> List;    
private:
   ListNodeIterator(ListNode<T>* node)
      :node_(node){};
      
};



template <class T>
class ConstListNodeIterator
{
   const ListNode<T>* node_; 
public:
   const T*  operator->() {return &(node_->value);};
   friend const T& operator *(const ConstListNodeIterator &it)
   {
       return it->value;
   }
   
   
   ConstListNodeIterator& operator++ ()
   {
       node_ = (node_ == nullptr || node_->prev) ? nullptr : node_->next;
       return *this;
   }
   
   ConstListNodeIterator operator++ (int)
   {
       NodeIterator old = *this;
       ++(*this);
       return old;
   }
   
   ConstListNodeIterator& operator-- ()
   {
       node_ = (node_ == nullptr || node_->prev) == nullptr ? nullptr : node_->prev;
       return *this;
   }
   
   ConstListNodeIterator operator-- (int)
   {
       NodeIterator old = *this;
       --(*this);
       return old;
   }
   
   friend bool operator==(const ConstListNodeIterator& a, const ConstListNodeIterator& b);
   friend bool operator==(const ListNodeIterator& a, const ConstListNodeIterator& b);
   friend bool operator==(const ConstListNodeIterator& a, const ListNodeIterator& b);
   friend bool operator!=(const ConstListNodeIterator& a, const ListNodeIterator& b);
   friend bool operator!=(const ListNodeIterator& a, const ConstListNodeIterator& b);
   friend bool operator!=(const ConstListNodeIterator& a, const ConstListNodeIterator& b);

   ConstListNodeIterator(const ConstListNodeIterator& node) = default;
   ConstListNodeIterator(const ListNodeIterator& node)
    :node_(nullptr) {};
   friend template<class, class> List;    
private:
   ConstListNodeIterator(const ListNode<T>* node)
      :node_(node){};
      
};


template <class T, class Arena>
class List
{
    Arena arena_;
    Node<T>* head_;
    Node<T>* end_;
    size_t size_;
    
public:
    
    List()
        :arena(), size_(0)
    {
        end_ = arena_.create();
        head_ = arena_.create();
    }

    typedef ListNodeIterator iterator;
    typedef ConstListNodeIterator const_iterator;
    
    iterator begin() {return head_->next;};
    iterator end() {return end_;};
    const_iterator begin() const {return head_->next;};
    const_iterator end() const {return end_;};
    
    
    template<class ...Args>
    iterator emplace(const_iterator it, Args... args)
    {

        Node<T*> new_node = arena_.create(it.node_->prev , it.node_, args);
        if (new_node == nullptr)
        {
            return end_;
        }
        new_node_->prev->next = new_node;
        new_node_->next->prev = new_node;
        size_++;
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
    }
    
    
    template<class ...Args>
    iterator emplaceBack(Args... args)
    {        
        return emplace(end_, args);
    }
    
    iterator pushBack(const T& val)
    {
        return emplaceBack(it, val);
    }
    
    template<class ...Args>
    iterator emplaceFront(Args... args)
    {        
        return emplace(head_->next, args);
    }
    
    iterator pushFront(const T& val)
    {
        return emplaceFront(val);
    }
    
    iterator erase(const_iterator it)
    {
        if (size_ == 0)
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
        end_->prev = next;
        size_ = 0;
    }
    
      
    List(const List<T, Arena>& l)
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
    
    
      
    List(std::initializer_list<T> l)
        :List()
    {
		for (auto it = l.begin(); it != l.end(); it++)
        {
            pushBack(*it);
        }
    }
    
    List& operator=(const List<T, Arena2>& l)
        :List()
    {
        if (this != l)
        {
            clear();
            insert(end_, l);
        }
        return this;
    }
    
    template<class X, class Arena2>
    List& operator=(const List<X, Arena2>& l)
        :List()
    {
        clear();
        insert(end_, l);
        return this;
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