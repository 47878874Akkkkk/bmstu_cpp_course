#pragma once

#include <exception>
#include <iostream>
#include <utility>
#include <stdexcept>
#include <memory>

namespace bmstu
{
template <typename T>
class stack
{
private:
    T* d;
    size_t s;
    size_t c;

    void r(size_t n) {
        if (n <= c) return;
        
        T* nd = static_cast<T*>(::operator new(n * sizeof(T)));
        for (size_t i = 0; i < s; ++i) {
            new (&nd[i]) T(std::move(d[i]));
            d[i].~T();
        }
        ::operator delete(d);
        d = nd;
        c = n;
    }

public:
    stack() : d(nullptr), s(0), c(0) {}

    ~stack() {
        for (size_t i = 0; i < s; ++i) {
            d[i].~T();
        }
        ::operator delete(d);
    }

    stack(const stack& o) : s(o.s), c(o.c) {
        d = static_cast<T*>(::operator new(c * sizeof(T)));
        for (size_t i = 0; i < s; ++i) {
            new (&d[i]) T(o.d[i]);
        }
    }

    stack& operator=(const stack& o) {
        if (this != &o) {
            stack t(o);
            swap(t);
        }
        return *this;
    }

    stack(stack&& o) noexcept 
        : d(o.d)
        , s(o.s)
        , c(o.c) {
        o.d = nullptr;
        o.s = 0;
        o.c = 0;
    }

    stack& operator=(stack&& o) noexcept {
        if (this != &o) {
            for (size_t i = 0; i < s; ++i) {
                d[i].~T();
            }
            ::operator delete(d);
            
            d = o.d;
            s = o.s;
            c = o.c;
            
            o.d = nullptr;
            o.s = 0;
            o.c = 0;
        }
        return *this;
    }

    void swap(stack& o) noexcept {
        std::swap(d, o.d);
        std::swap(s, o.s);
        std::swap(c, o.c);
    }

    bool empty() const noexcept { 
        return s == 0; 
    }

    size_t size() const noexcept { 
        return s; 
    }

    template <typename... Args>
    void emplace(Args&&... a) {
        if (s >= c) {
            r(c == 0 ? 4 : c * 2);
        }
        new (&d[s]) T(std::forward<Args>(a)...);
        ++s;
    }

    void push(const T& v) {
        if (s >= c) {
            r(c == 0 ? 4 : c * 2);
        }
        new (&d[s]) T(v);
        ++s;
    }

    void push(T&& v) {
        if (s >= c) {
            r(c == 0 ? 4 : c * 2);
        }
        new (&d[s]) T(std::move(v));
        ++s;
    }

    void pop() {
        if (empty()) {
            throw std::underflow_error("Stack is empty");
        }
        --s;
        d[s].~T();
    }

    T& top() {
        if (empty()) {
            throw std::underflow_error("Stack is empty");
        }
        return d[s - 1];
    }

    const T& top() const {
        if (empty()) {
            throw std::underflow_error("Stack is empty");
        }
        return d[s - 1];
    }

    void clear() noexcept {
        for (size_t i = 0; i < s; ++i) {
            d[i].~T();
        }
        s = 0;
    }
};
}