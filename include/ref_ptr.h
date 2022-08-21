//
// Created by HWZen on 2022/6/17.
// Copyright (c) 2022 HWZen All rights reserved.
// MIT License
// 

#ifndef SSTL_REF_PTR_H
#define SSTL_REF_PTR_H
#include <type_traits>
#include <cstddef>
#include "functional.h"

namespace sstd {


    template<typename T>
    class ref_ptr {
        public:
        using value_type = T;
        using pVal = value_type*;
        using refVal = value_type&;

        template<class other>
        friend class ref_ptr;

        constexpr ref_ptr():m_refCount(new size_t(1)), m_ptr(new value_type) {}

        constexpr ref_ptr(const pVal ptr) : m_refCount(new size_t(1)), m_ptr(ptr) {}

        template<same_or_subclass<value_type> Ty>
        constexpr ref_ptr(ref_ptr<Ty>&& other) noexcept : ref_ptr() {
            swap(other);
        }

        constexpr ref_ptr(const ref_ptr& other) : m_refCount(other.m_refCount), m_ptr(other.m_ptr) {
            ++(*m_refCount);
        }

        template<same_or_subclass<value_type> Ty>
        constexpr ref_ptr(const ref_ptr<Ty>& other) : m_ptr(other.m_ptr) , m_refCount(other.m_refCount) {
            ++(*m_refCount);
        }

        ~ref_ptr() {
            if (--(*m_refCount) == 0) {
                delete m_refCount;
                delete m_ptr;
            }
        }

        template<typename Ty1, same_or_subclass<Ty1> Ty2>
        friend constexpr inline bool operator==(const ref_ptr<Ty1>& lhs, const ref_ptr<Ty2>& rhs);

        template<same_or_subclass<value_type> Ty>
        constexpr void swap(ref_ptr<Ty>& other) noexcept {
            std::swap(m_refCount, other.m_refCount);
            std::swap(m_ptr,  reinterpret_cast<pVal&>(other.m_ptr));
        }

        template<same_or_subclass<value_type> Ty>
        constexpr ref_ptr& operator=(ref_ptr<Ty> &&other) {
            swap(other);
            return *this;
        }

        constexpr ref_ptr& operator=(const ref_ptr& other);

        template<same_or_subclass<typename ref_ptr<T>::value_type> Ty>
        constexpr ref_ptr& operator=(const ref_ptr<Ty>& other);

        constexpr pVal operator->() const {
            return m_ptr;
        }

        constexpr refVal operator*() const {
            return *m_ptr;
        }

        constexpr operator pVal() const {
            return m_ptr;
        }

        constexpr pVal get() const {
            return m_ptr;
        }

        constexpr bool checkValid() const {
            return m_ptr != nullptr && m_refCount != nullptr;
        }

        private:
        pVal m_ptr;
        size_t *m_refCount;
    };

    template<typename T>
    constexpr ref_ptr<T>& ref_ptr<T>::operator=(const ref_ptr& other) {
        if (*this == other) {
            return *this;
        }
        --(*m_refCount);
        if (*m_refCount == 0) {
            delete m_refCount;
            delete m_ptr;
        }

        m_ptr = other.m_ptr;
        m_refCount = other.m_refCount;
        ++(*m_refCount);
        return *this;
    }

    template<typename T>
    template<same_or_subclass<typename ref_ptr<T>::value_type> Ty>
    constexpr ref_ptr<T> &ref_ptr<T>::operator=(const ref_ptr<Ty> &other) {
        if (*this == other) {
            return *this;
        }
        --(*m_refCount);
        if (*m_refCount == 0) {
            delete m_refCount;
            delete m_ptr;
        }

        m_ptr = static_cast<pVal>(other.m_ptr);
        m_refCount = other.m_refCount;
        ++(*m_refCount);
        return *this;
    }

    template<typename Ty1, same_or_subclass<Ty1> Ty2>
    constexpr bool operator==(const ref_ptr<Ty1> &lhs, const ref_ptr<Ty2> &rhs) {
        return lhs.m_ptr == rhs.m_ptr;
    }

}

#endif //SSTL_REF_PTR_H
