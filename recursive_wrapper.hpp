#ifndef MAPBOX_UTIL_RECURSIVE_WRAPPER_HPP
#define MAPBOX_UTIL_RECURSIVE_WRAPPER_HPP

// Based on variant/recursive_wrapper.hpp from boost.
//
// Original license:
//
// Copyright (c) 2002-2003
// Eric Friedman, Itay Maman
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <cassert>
#include <utility>

namespace mapbox { namespace util {

template <typename T>
struct recursive_wrapper_traits
{
    static constexpr bool is_nothrow_destructible = true;
};


template <typename T>
class recursive_wrapper
{

    T* p_;

    void assign(T const& rhs)
    {
        this->get() = rhs;
    }

public:

    using type = T;

    /**
     * Default constructor default initializes the internally stored value.
     * For POD types this means nothing is done and the storage is
     * uninitialized.
     *
     * @throws std::bad_alloc if there is insufficient memory for an object
     *         of type T.
     * @throws any exception thrown by the default constructur of T.
     */
    recursive_wrapper() : p_(new T) {};

    /*
     * we cannot use `noexcept(std::is_nothrow_destructible<T>::value)`
     *  here becase if T contains a variant with recursive_wrapper<T>,
     *  there will be infinite recursion in ~variant() noexcept-specifier
     * https://github.com/mapbox/variant/issues/86#issuecomment-178001768
     *
     * we cannot say `noexcept(true)`, because what if ~T() throws?
     *
     * and we cannot say `noexcept(false)` either, because that would
     *  prevent having a member variable containing a recursive_wrapper
     *  in a class with inherited noexcept(true) virtual destructor
     *
     * IMPORTANT: it it thus the user's responsibility to ensure that
     *            recursive_wrapper_traits<T>::is_nothrow_destructible
     *            matches the noexcept-specifier on ~T()
     */
    ~recursive_wrapper()
        noexcept(recursive_wrapper_traits<T>::is_nothrow_destructible)
    {
        delete p_;
    }

    recursive_wrapper(recursive_wrapper const& operand)
        : p_(new T(operand.get())) {}

    recursive_wrapper(T const& operand)
        : p_(new T(operand)) {}

    recursive_wrapper(recursive_wrapper && operand)
        : p_(new T(std::move(operand.get()))) {}

    recursive_wrapper(T && operand)
        : p_(new T(std::move(operand))) {}

    inline recursive_wrapper & operator=(recursive_wrapper const& rhs)
        noexcept(std::is_nothrow_copy_assignable<T>::value)
    {
        assign(rhs.get());
        return *this;
    }

    inline recursive_wrapper & operator=(T const& rhs)
        noexcept(std::is_nothrow_copy_assignable<T>::value)
    {
        assign(rhs);
        return *this;
    }

    inline void swap(recursive_wrapper & operand) noexcept
    {
        T* temp = operand.p_;
        operand.p_ = p_;
        p_ = temp;
    }

    recursive_wrapper & operator=(recursive_wrapper && rhs) noexcept
    {
        swap(rhs);
        return *this;
    }

    recursive_wrapper & operator=(T && rhs)
        noexcept(std::is_nothrow_move_assignable<T>::value)
    {
        get() = std::move(rhs);
        return *this;
    }

    T & get()
    {
        assert(p_);
        return *get_pointer();
    }

    T const& get() const
    {
        assert(p_);
        return *get_pointer();
    }

    T* get_pointer() { return p_; }

    const T* get_pointer() const { return p_; }

    operator T const&() const { return this->get(); }

    operator T &() { return this->get(); }

}; // class recursive_wrapper

template <typename T>
inline void swap(recursive_wrapper<T> & lhs, recursive_wrapper<T> & rhs) noexcept
{
    lhs.swap(rhs);
}

}}

#endif // MAPBOX_UTIL_RECURSIVE_WRAPPER_HPP
