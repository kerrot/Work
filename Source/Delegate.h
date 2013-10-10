#pragma once

class Callback
{
public:
    virtual void invoke() = 0;
};

template <typename T, typename Method>
class MethodCallback : public Callback
{
private:
    void *object_;
    Method method_;

public:
    MethodCallback(void *object, Method method)
        : object_(object)
        , method_(method)
    {}

    virtual void invoke()
    {
        T *obj = static_cast<T *>(object_);
        (obj->*method_)();
    }
};

class Delegate
{
private:
    Callback *callback_;

public:
    Delegate()
    :callback_(0)
    {}

    template <typename T, typename Method>
    Delegate(T *object, Method method)
        :callback_(new MethodCallback<T, Method>(object, method))
    {}

    ~Delegate(void) 
    { 
        if (callback_)
        {
            delete callback_; 
        }
    }

    void operator()()
    {
        if (callback_)
        {
            callback_->invoke();
        }
    }

    template <typename T, typename Method>
    void AddCallBack(T *object, Method method)
    {
        if (callback_)
        {
            delete callback_;
        }
        callback_ = new MethodCallback<T, Method>(object, method);
    }
};