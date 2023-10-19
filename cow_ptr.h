#pragma once

#include <atomic>
#include <memory>
#include <type_traits>
#include <memory>
namespace nonstd
{

template<typename T>
class cow_ptr;

template<typename T, typename ...Args>
cow_ptr<T> make_cow(Args &&... args)
{
    return cow_ptr<T>(new T(std::forward<Args>(args)...));
}

template<typename T>
class cow_ptr
{
public:
    typedef T element_type;
    typedef std::add_const_t<element_type> const_element_type;
    typedef std::remove_cv_t<element_type> value_type;
    typedef element_type* pointer;
    typedef const_element_type* const_pointer;

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

    template<class U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>, void>>
    cow_ptr(const cow_ptr<U> &other) noexcept: ptr(other.ptr), refCounter(other.refCounter)
    {
        if (refCounter)
        {
            ++*refCounter;
        }
    }

    template<class U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>, void>>
    cow_ptr(cow_ptr<U> &&other) noexcept: ptr(other.ptr), refCounter(other.refCounter)
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
            cow_ptr(other).swap(*this);
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

    template<class U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>, void>>
    cow_ptr& operator=(const cow_ptr<U> &other) noexcept
    {
        if (this != &other)
        {
            swap(cow_ptr(other));
        }
        return *this;
    }

    template<class U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>, void>>
    cow_ptr& operator=(cow_ptr<U> &&other) noexcept
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
    const_element_type& operator*() const { return *ptr; }
    const_pointer operator->() const noexcept { return ptr; }
    const_pointer get() const noexcept { return ptr; }
    template<typename U=T>
    std::enable_if_t<std::is_copy_constructible_v<U>, pointer> getMutable() noexcept { return operator->(); }

    template<typename U=T>
    std::enable_if_t<std::is_copy_constructible_v<U>, T &> operator*() { return *operator->(); }

    template<typename U=T>
    std::enable_if_t<std::is_copy_constructible_v<U>, pointer> operator->()
    {
        if (refCounter && *refCounter > 1)
        {
            make_cow<T>(*ptr).swap(*this);
        }
        return ptr;
    }

    template<typename U=T>
    std::enable_if_t<std::is_convertible_v<U*, T*>, bool>
            operator==(const cow_ptr<U> &other) const noexcept { return ptr == other.ptr; }

    template<typename U=T>
    std::enable_if_t<std::is_convertible_v<U*, T*>, bool>
            operator!=(const cow_ptr<U> &other) const noexcept { return ptr != other.ptr; }

    void reset(pointer newValue = nullptr) noexcept
    {
        cow_ptr(newValue).swap(*this);
    }

    template<typename U=T>
    std::enable_if_t<std::is_convertible_v<U*, T*>, void>
    reset(U *newValue)
    {
        destroy();
        ptr = newValue;
        refCounter = (ptr != nullptr) ? createRefCounter() : nullptr;
    }

    void swap(cow_ptr &other) noexcept
    {
        std::swap(ptr, other.ptr);
        std::swap(refCounter, other.refCounter);
    }

private:
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

    static auto createRefCounter()
    {
        return new std::remove_pointer_t<decltype(refCounter)>(1);
    }

    pointer ptr;
    std::atomic<int>* refCounter;
};

} // namespace nonstd
