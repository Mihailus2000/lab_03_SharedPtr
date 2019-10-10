//
// Created by mihailus on 05.10.2019.
//

#ifndef MY_REALIZATION_SHARED_PTR_SHAREDPTR_HPP
#define MY_REALIZATION_SHARED_PTR_SHAREDPTR_HPP


#include <atomic>
#include <cstdio>

template <typename T>
class SharedPtr {
    using pointer = T*;

public:
    SharedPtr() noexcept
        : _pointerCounter(nullptr), _pointer(nullptr) {}

    explicit SharedPtr(pointer ptr) noexcept
        : _pointerCounter(new std::atomic_uint{1}), _pointer(ptr){}

    SharedPtr(const SharedPtr& shared) noexcept {
         *this = shared;
    }

    SharedPtr(SharedPtr&& shared) noexcept {
        *this = std::move(shared);
    }

    ~SharedPtr(){
        if(_pointerCounter == nullptr){
            return;
        }

        if(*_pointerCounter == 1)
        {
            (*_pointerCounter)--;
            assert(_pointerCounter != nullptr);
            delete _pointerCounter;
            _pointerCounter = nullptr;
            assert(_pointer != nullptr);
            delete  _pointer;
            _pointer = nullptr;
        }
    }

    SharedPtr& operator=(const SharedPtr& shared){
        if(&shared == this)
            return *this;

       this->~SharedPtr();

        _pointer = shared._pointer;
        _pointerCounter = shared._pointerCounter;
        (*_pointerCounter)++;

        return *this;
    }

    SharedPtr& operator=(SharedPtr&& shared) noexcept {
        if(&shared == this)
            return *this;

        this->~SharedPtr();

        _pointer = shared._pointer;
        shared._pointer = nullptr;
        _pointerCounter = shared._pointerCounter;
        shared._pointerCounter = nullptr;

        return *this;

    }

    // проверяет, указывает ли указатель на объект
    operator bool() const
    {
        return _pointer != nullptr;
    }

    T& operator*() const
    {
        return *_pointer;
    }

    pointer operator->() const {
        return _pointer;
    }

    pointer get() {
        return _pointer;
    }

    void reset() noexcept {
        *this = SharedPtr();
    }
    void reset(pointer ptr){
        *this = SharedPtr(ptr);
    }

    void swap(SharedPtr& shared){
        if(&shared == this )
            return;

        std::swap(_pointer, shared._pointer);
        std::swap(_pointerCounter, shared._pointerCounter);
    }

    // возвращает количество объектов SharedPtr, которые ссылаются на тот же управляемый объект
    [[nodiscard]] size_t use_count() const noexcept {
        return *_pointerCounter;
    }

private:
    std::atomic_uint *_pointerCounter = nullptr;
    pointer _pointer = nullptr;
};



#endif //MY_REALIZATION_SHARED_PTR_SHAREDPTR_HPP
