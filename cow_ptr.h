#pragma once

#include <atomic>
#include <type_traits>

namespace nonstd
{
template<typename T>
class cow_ptr
{
public:
    typedef T element_type;
    typedef std::remove_cv_t<T> value_type;
    typedef T* pointer;

    cow_ptr() noexcept: ptr(nullptr), refCounter(nullptr) {}

    explicit cow_ptr(pointer ptr) : ptr(ptr), refCounter(createRefCounter()) {}

    cow_ptr(const cow_ptr &other) noexcept: ptr(other.ptr), refCounter(other.refCounter)
    {
        if (refCounter)
        {
            ++*refCounter;
        }
    }

    cow_ptr(cow_ptr &&other) noexcept: ptr(other.ptr), refCounter(other.refCounter)
    {
        other.nullify();
    }

    ~cow_ptr()
    {
        destroy();
    }

    cow_ptr& operator=(const cow_ptr &other) noexcept
    {
        if (this != &other)
        {
            destroy();
            assign(other);
            if (refCounter)
            {
                ++*refCounter;
            }
        }
        return *this;
    }

    cow_ptr& operator=(cow_ptr &&other) noexcept
    {
        if (this != &other)
        {
            destroy();
            assign(other);
            other.nullify();
        }
        return *this;
    }

    explicit operator bool() const noexcept { return ptr != nullptr; }

    std::add_const_t<T>& operator*() const { return *ptr; }

    std::add_const_t<T>* operator->() const noexcept { return ptr; }

    std::add_const_t<T>* get() const noexcept { return ptr; }

    pointer getMutable() noexcept { return operator->(); }

    template<typename U=T>
    std::enable_if_t<std::is_copy_constructible_v<U>, T &> operator*() { return *operator->(); }

    template<typename U=T>
    std::enable_if_t<std::is_copy_constructible_v<U>, pointer> operator->()
    {
        if (refCounter && *refCounter > 1)
        {
            T* oldPtr = ptr;
            ptr = new T(*oldPtr);
            if (--*refCounter == 0)
            {
                // a very uncommon case when the refCounter was changed by another thread after the previous check
                delete oldPtr;
                *refCounter = 1;
            }
            else
            {
                refCounter = createRefCounter();
            }
        }
        return ptr;
    }

    bool operator==(const cow_ptr &other) const noexcept { return ptr == other.ptr; }

    bool operator!=(const cow_ptr &other) const noexcept { return ptr != other.ptr; }

    void reset(pointer newValue = nullptr) noexcept
    {
        destroy();
        ptr = newValue;
        refCounter = (ptr != nullptr) ? createRefCounter() : nullptr;
    }

private:
    auto createRefCounter()
    {
        return new std::remove_pointer_t<decltype(refCounter)>(1);
    }

    void nullify()
    {
        ptr = nullptr;
        refCounter = nullptr;
    }

    void assign(const cow_ptr &other)
    {
        ptr = other.ptr;
        refCounter = other.refCounter;
    }

    void destroy()
    {
        if (refCounter && --*refCounter == 0)
        {
            delete ptr;
            delete refCounter;
        }
    }

    pointer ptr;
    std::atomic<int>* refCounter;
};

template<typename T, typename ...Args>
cow_ptr<T> make_cow(Args &&... args)
{
    return cow_ptr<T>(new T(std::forward<Args>(args)...));
}

} // namespace nonstd
