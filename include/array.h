//
// Created by HWZen on 2022/6/15.
// Copyright (c) 2022 HWZen All rights reserved.
// MIT License
// 

#ifndef MYSTL_ARRAY_H
#define MYSTL_ARRAY_H
#include "functional.h"
#include <cstddef>
#ifdef _WIN32
const size_t MAX_STACK_SIZE = 0x100000;
#elif defined(__linux__)
const size_t MAX_STACK_SIZE = 0x400000;
#elif defined(__APPLE__)
const size_t MAX_STACK_SIZE = 0x400000;
#endif

namespace sstd {
    template<typename T, size_t _size> requires (_size > 0)
    class array {
    public:
        using value_type = T;
        using pVal = value_type*;
        using refVal = value_type&;
    private:
        pVal m_data;

    public:

        [[deprecated("array too small, suggest to use std::array to use stack memory")]]
        constexpr array() requires(_size * sizeof(T) < MAX_STACK_SIZE)  : m_data(new value_type[_size]) {
        }

        constexpr array() : m_data(new value_type[_size]) {
        }
        constexpr array(const array& other);

        constexpr array(array&& other) noexcept;

        constexpr array(std::initializer_list<value_type> il);

        ~array();

        constexpr array& operator=(const array& other);

        constexpr array& operator=(array&& other) noexcept;


        constexpr refVal operator[](size_t index) {
            return m_data[index];
        }

        constexpr const refVal operator[](size_t index) const {
            return m_data[index];
        }

        class const_iterator {
        protected:
            pVal m_ptr;
        public:
            const_iterator() = delete;
            constexpr const_iterator(pVal ptr) : m_ptr(ptr) {}
            constexpr const_iterator(const const_iterator&) = default;
            constexpr const_iterator(const_iterator&&) = default;
            constexpr const_iterator& operator=(const const_iterator&) = default;
            constexpr const_iterator& operator=(const_iterator&&) = default;


            constexpr const_iterator& operator++() {
                m_ptr++;
                return *this;
            }

            constexpr const_iterator operator++(int) {
                const_iterator tmp = *this;
                ++*this;
                return tmp;
            }

            constexpr const_iterator& operator--() {
                m_ptr--;
                return *this;
            }

            constexpr const_iterator operator--(int) {
                const_iterator tmp = *this;
                --*this;
                return tmp;
            }

            constexpr bool operator==(const const_iterator& other) const {
                return m_ptr == other.m_ptr;
            }

            constexpr const value_type &operator*() const {
                return *m_ptr;
            }

            constexpr const value_type *operator->() const {
                return m_ptr;
            }

            bool checkValid() const {
                return m_ptr != nullptr;
            }
        };

        class iterator : public const_iterator {
        public:
            using const_iterator::const_iterator;
            iterator() = delete;

            constexpr iterator& operator++() {
                const_iterator::operator++();
                return *this;
            }

            constexpr iterator operator++(int) {
                iterator tmp = *this;
                ++*this;
                return tmp;
            }

            constexpr iterator operator--() {
                const_iterator::operator--();
                return *this;
            }

            constexpr iterator operator--(int) {
                iterator tmp = *this;
                --*this;
                return tmp;
            }

            constexpr refVal operator*() const {
                return *this->m_ptr;
            }

            constexpr pVal operator->() const {
                return this->m_ptr;
            }
        };

        constexpr auto begin() {
            return iterator(m_data);
        }

        constexpr auto end() {
            return iterator(m_data + _size);
        }

        constexpr auto cbegin() const {
            return const_iterator(m_data);
        }

        constexpr auto cend() const {
            return const_iterator(m_data + _size);
        }

        constexpr void swap(array& other);


        consteval size_t size() const {
            return _size;
        }

        constexpr value_type& front() {
            return m_data[0];
        }

        constexpr value_type& back() {
            return m_data[_size - 1];
        }

        constexpr const value_type& front() const {
            return m_data[0];
        }

        constexpr const value_type& back() const {
            return m_data[_size - 1];
        }


    };

    template<typename T, typename ...U>
    array(T, U...) -> array<T, 1 + sizeof...(U)>;

    template<typename T, size_t _size> requires (_size > 0)
    constexpr array<T, _size>::array(const array &other): array() {
        for(int i{}; i < _size; ++i){
            m_data[i] = other.m_data[i];
        }
    }

    template<typename T, size_t _size> requires (_size > 0)
    constexpr array<T, _size>::array(array &&other) noexcept : m_data(nullptr) {
        swap(other);
    }

    template<typename T, size_t _size> requires (_size > 0)
    constexpr void array<T, _size>::swap(array &other) {
        std::swap(m_data, other.m_data);
    }

    template<typename T, size_t _size> requires (_size > 0)
    constexpr array<T, _size>::array(std::initializer_list<value_type> il) :array() {
        auto size = il.size();
        if (size > _size) {
            throw std::out_of_range("initializer_list size is too large");
        }
        std::move(il.begin(), il.end(), m_data);
    }

    template<typename T, size_t _size> requires (_size > 0)
    array<T, _size>::~array() {
        delete[] m_data;
    }

    template<typename T, size_t _size> requires (_size > 0)
    constexpr array<T, _size>& array<T, _size>::operator=(const array<T, _size> &other) {
        if (this == &other) {
            return *this;
        }
        delete[] m_data;
        m_data = new value_type[_size];
        std::copy(other.m_data, other.m_data + _size, m_data);
        return *this;
    }

    template<typename T, size_t _size> requires (_size > 0)
    constexpr array<T, _size> &array<T, _size>::operator=(array<T, _size> &&other) noexcept {
        swap(other);
        return *this;
    }

}

#endif //MYSTL_ARRAY_H
