//
// Created by HWZen on 2022/6/17.
// Copyright (c) 2022 HWZen All rights reserved.
// MIT License
// 

#ifndef SSTL_REFBASE_H
#define SSTL_REFBASE_H

#include <cstddef>
#include <type_traits>

namespace sstd {
    class RefBase {
    protected:
        size_t *m_refCount;
    public:
        RefBase() : m_refCount(new size_t(1)) {}

        RefBase(const RefBase& other) : m_refCount(other.m_refCount) {
            ++(*m_refCount);
        }

        RefBase(RefBase&& other) noexcept : RefBase() {
            swap(other);
        }

        RefBase& operator=(const RefBase& other){
            if(this == &other)
                return *this;
            m_refCount = other.m_refCount;
            incCount();
        }

        RefBase& operator=(RefBase&& other) noexcept{
            swap(other);
            return *this;
        }

        void swap(RefBase& other) noexcept {
            std::swap(m_refCount, other.m_refCount);
        }

        void incCount() {
            ++(*m_refCount);
        }

        void decCount(){
            if (--(*m_refCount) == 0) {
                zeroCount();
            }
        }

        virtual ~RefBase() {
            if( *m_refCount == 0 ){
                delete m_refCount;
            }
        }

        size_t getCount() const {
            return *m_refCount;
        }

    private:
        virtual void zeroCount() = 0;
    };
}

#endif //SSTL_REFBASE_H
