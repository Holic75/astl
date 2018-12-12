#ifndef ASTL_FUNCTIONAL_H
#define ASTL_FUNCTIONAL_H

#include <type_traits.h>

namespace astl
{
    
namespace aux
{
    
template<class>
class CallableBase;

template<class R, class... Args>
class CallableBase<R(Args...)>
{
	
public:
    virtual R operator()(Args... args) = 0;
    virtual ~CallableBase(){};
    virtual CallableBase* getCopy() = 0;
};

template<class F, class X>
class Callable;

template<class F, class R, class... Args>
class Callable<F, R(Args...)>: public CallableBase<R(Args...)>
{
	F f_;
public:
    Callable() {};
    Callable(F f)
        :f_(f) {};
    virtual R operator()(Args&&... args) override { return f_(std::forward<Args>(args)...);};
    virtual CallableBase<R(Args...)>* getCopy() override {return new Callable(f_);};
};

}


template<class>
class Function;


template<class R, class... Args>
class Function<R(Args...)> 
{
    aux::CallableBase<R(Args...)>* f_;

	void clear()
	{
		if (f_ != nullptr)
		{
			delete f_;
            f_ = nullptr;
		}
	}
    
public:
    Function() 
		:f_(nullptr)
    {};
    template<class F>
    Function(F f)
        :f_(new aux::Callable<F, R(Args...)>(f)){};

    R operator()(Args&&... args) { return f_->operator()(std::forward<Args>(args)...);};
    

    ~Function() 
	{
		clear();
	};


	Function(const Function& other)
	{
		if (other.f_ != nullptr)
        {
            f_ = other.f_->getCopy();
        }
        else
        {
            f_ = nullptr;
        }
	}
    
    
    Function& operator =(const Function& other)
    {
        if (this != &other)
        {
            clear();
            if (other.f_ != nullptr)
            {
                f_ = other.f_->getCopy();
            }
        }
        return *this;
    }
    
    bool empty() {return f_ == nullptr;};
};   
}








#endif