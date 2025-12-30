#pragma once

#include <exception>
#include <utility>
#include <stdexcept>

namespace bmstu {
template <typename T>
class stack
{
private:
    T* data_;
    size_t size_;

public:
    stack() : data_(nullptr), size_(0) {}
    
    ~stack() {
        clear();
        operator delete(data_);
    }
    
    stack(const stack& other) : size_(other.size_) {
        if (size_ > 0) {
            data_ = static_cast<T*>(operator new(size_ * sizeof(T)));
            for (size_t i = 0; i < size_; ++i) {
                new (data_ + i) T(other.data_[i]);
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
        , size_(other.size_) {
        other.data_ = nullptr;
        other.size_ = 0;
    }
    
    stack& operator=(stack&& other) noexcept {
        if (this != &other) {
            clear();
            operator delete(data_);
            
            data_ = other.data_;
            size_ = other.size_;
            
            other.data_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }
        
    void swap(stack& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
    }
    
    bool empty() const noexcept { 
        return size_ == 0; 
    }
    
    size_t size() const noexcept { 
        return size_; 
    }
    
    template <typename... Args>
    void emplace(Args&&... args) {
        T* new_data = static_cast<T*>(operator new((size_ + 1) * sizeof(T)));
        
        for (size_t i = 0; i < size_; ++i) {
            new (new_data + i) T(std::move(data_[i]));
            data_[i].~T();
        }
        
        new (new_data + size_) T(std::forward<Args>(args)...);
        
        operator delete(data_);
        data_ = new_data;
        ++size_;
    }
    
    void push(const T& value) {
        T* new_data = static_cast<T*>(operator new((size_ + 1) * sizeof(T)));
        
        for (size_t i = 0; i < size_; ++i) {
            new (new_data + i) T(std::move(data_[i]));
            data_[i].~T();
        }
        
        new (new_data + size_) T(value);
        
        operator delete(data_);
        data_ = new_data;
        ++size_;
    }
    
    void push(T&& value) {
        T* new_data = static_cast<T*>(operator new((size_ + 1) * sizeof(T)));
        
        for (size_t i = 0; i < size_; ++i) {
            new (new_data + i) T(std::move(data_[i]));
            data_[i].~T();
        }
        
        new (new_data + size_) T(std::move(value));
        
        operator delete(data_);
        data_ = new_data;
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