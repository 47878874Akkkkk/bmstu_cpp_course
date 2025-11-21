#pragma once

#include <exception>
#include <iostream>
#include <utility>
#include <stdexcept>

namespace bmstu {
template <typename T>
class stack {
public:
    stack() : data_(new T[10]), size_(0), capacity_(10) {}
    
    bool empty() const noexcept { 
        return size_ == 0; 
    }

    size_t size() const noexcept { 
        return size_; 
    }

    ~stack() {
        delete[] data_;
    }

    void push(const T& value) {
        if (size_ >= capacity_) {
            throw std::runtime_error("Stack overflow");
        }
        data_[size_++] = value;
    }

    void push(T&& value) {
        push(value);
    }

    template <typename... Args>
    void emplace(Args&&... args) {
        T temp(std::forward<Args>(args)...);
        push(temp);
    }

    void pop() {
        if (empty()) {
            throw std::underflow_error("Stack underflow");
        }
        --size_;
    }

    T& top() { 
        if (empty()) {
            throw std::underflow_error("Stack is empty");
        }
        return data_[size_ - 1];
    }

    const T& top() const { 
        if (empty()) {
            throw std::underflow_error("Stack is empty");
        }
        return data_[size_ - 1];
    }

    void clear() noexcept {
        size_ = 0;
    }

private:
    T* data_;
    size_t size_;
    size_t capacity_;
};
}  // namespace bmstu