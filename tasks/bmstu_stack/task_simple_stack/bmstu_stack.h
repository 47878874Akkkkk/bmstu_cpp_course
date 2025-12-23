#pragma once

#include <exception>
#include <utility>
#include <stdexcept>

namespace bmstu
{
template <typename T>
class stack
{
private:
    T* data_;
    size_t size_;
    size_t capacity_;

    void reserve(size_t new_capacity) {
        if (new_capacity <= capacity_) return;
        
        T* new_data = static_cast<T*>(::operator new(new_capacity * sizeof(T)));
        for (size_t i = 0; i < size_; ++i) {
            ::new (new_data + i) T(std::move(data_[i]));
            data_[i].~T();
        }
        ::operator delete(data_);
        data_ = new_data;
        capacity_ = new_capacity;
    }

public:
    stack() : data_(nullptr), size_(0), capacity_(0) {}

    ~stack() {
        clear();
        ::operator delete(data_);
    }

    stack(const stack& other) : size_(other.size_), capacity_(other.size_) {
        if (size_ > 0) {
            data_ = static_cast<T*>(::operator new(capacity_ * sizeof(T)));
            for (size_t i = 0; i < size_; ++i) {
                ::new (data_ + i) T(other.data_[i]);
            }
        } else {
            data_ = nullptr;
        }
    }

    stack& operator=(const stack& other) {
        if (this != &other) {
            stack temp(other);
            swap(temp);
        }
        return *this;
    }

    stack(stack&& other) noexcept 
        : data_(other.data_)
        , size_(other.size_)
        , capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    stack& operator=(stack&& other) noexcept {
        if (this != &other) {
            clear();
            ::operator delete(data_);
            
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            
            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }

    void swap(stack& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

    bool empty() const noexcept { 
        return size_ == 0; 
    }

    size_t size() const noexcept { 
        return size_; 
    }

    template <typename... Args>
    void emplace(Args&&... args) {
        if (size_ >= capacity_) {
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        ::new (data_ + size_) T(std::forward<Args>(args)...);
        ++size_;
    }

    void push(const T& value) {
        if (size_ >= capacity_) {
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        ::new (data_ + size_) T(value);
        ++size_;
    }

    void push(T&& value) {
        if (size_ >= capacity_) {
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        ::new (data_ + size_) T(std::move(value));
        ++size_;
    }

    void pop() {
        if (empty()) {
            throw std::underflow_error("Stack is empty");
        }
        --size_;
        data_[size_].~T();
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
        for (size_t i = 0; i < size_; ++i) {
            data_[i].~T();
        }
        size_ = 0;
    }
};
}