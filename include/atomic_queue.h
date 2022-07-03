//
// Created by HWZen on 2022/7/3.
// Copyright (c) 2022 HWZen All rights reserved.
// MIT License
// 

#ifndef SSTL_ATOMIC_QUEUE_H
#define SSTL_ATOMIC_QUEUE_H

#include <array.h>
#include <semaphore>
#include <mutex>
#include <optional>

namespace sstd{
    // TODO:add m_size to function, consider m_front and m_back is need mutex.
    template<typename T, size_t queue_size>
    class atomic_queue{
    public:
        using value_type = T;
        using pVal = value_type*;
        using refVal = value_type&;

        constexpr atomic_queue();

        constexpr atomic_queue(const atomic_queue &) = delete;

        constexpr atomic_queue(atomic_queue&&)  noexcept = default;

        ~atomic_queue() = default;

        // warning: Block indefinitely
        constexpr value_type& front();

        // warning: Block indefinitely
        constexpr value_type& back();

        // warning: Block indefinitely
        constexpr void push( weak_same_type<value_type> auto &&data);

        // warning: Block indefinitely
        constexpr value_type pop();

        constexpr std::optional<value_type&> try_front();

        constexpr std::optional<value_type&> try_back();

        constexpr bool try_push(weak_same_type<value_type> auto &&data);


        constexpr std::optional<value_type> try_pop();

        template <class _Rep, class _Period>
        constexpr std::optional<value_type&> try_front_for(const std::chrono::duration<_Rep, _Period>);

        template <class _Rep, class _Period>
        constexpr std::optional<value_type&> try_back_for(const std::chrono::duration<_Rep, _Period>);

        template <class _Rep, class _Period>
        constexpr bool try_push_for(weak_same_type<value_type> auto &&data, const std::chrono::duration<_Rep, _Period>);

        template <class _Rep, class _Period>
        constexpr std::optional<value_type> try_pop_for(const std::chrono::duration<_Rep, _Period>);

        constexpr size_t size() const;

    private:
        array<value_type , queue_size> m_data;
        size_t m_front{0};
        size_t m_back{0};
        size_t m_size{0};
        std::unique_lock<std::mutex> m_producer_lock;
        std::unique_lock<std::mutex> m_consumer_lock;
    };

    template<typename T, size_t queue_size>
    constexpr atomic_queue<T, queue_size>::atomic_queue() {
        m_consumer_lock.lock();
    }

    template<typename T, size_t queue_size>
    constexpr T &atomic_queue<T, queue_size>::front() {
        m_consumer_lock.lock();
        auto &res = m_data[m_front];
        m_consumer_lock.unlock();
        return res;
    }

    template<typename T, size_t queue_size>
    constexpr T &atomic_queue<T, queue_size>::back() {
        m_consumer_lock.lock();
        auto &res = m_data[(m_back - 1) < 0 ? queue_size - 1 : m_back - 1];
        m_consumer_lock.unlock();
        return res;
    }

    template<typename T, size_t queue_size>
    constexpr void atomic_queue<T, queue_size>::push(weak_same_type<value_type> auto &&data) {
        m_producer_lock.lock();
        m_data[m_back] = std::forward(data);
        m_back = (m_back + 1 >= queue_size) ? 0 : m_back + 1;
        if(size() == 1)
            m_consumer_lock.unlock();
        if(size() < queue_size - 1)
            m_producer_lock.unlock();
    }

    template<typename T, size_t queue_size>
    constexpr T atomic_queue<T, queue_size>::pop() {
        m_consumer_lock.lock();
        auto res = std::move(m_data[m_front]);
        m_front = (m_front + 1 >= queue_size) ? 0 : m_front + 1;
        if(size() == queue_size - 1)
        if(size() > 0)
            m_consumer_lock.unlock();
        return res;
    }

    template<typename T, size_t queue_size>
    constexpr std::optional<T &> atomic_queue<T, queue_size>::try_front() {
        if(m_consumer_lock.try_lock()){
            auto &res = m_data[m_front];
            m_consumer_lock.unlock();
            return {res};
        }
        else
            return {};
    }

    template<typename T, size_t queue_size>
    constexpr std::optional<T &> atomic_queue<T, queue_size>::try_back() {
        if(m_consumer_lock.try_lock()){
            auto &res = m_data[(m_back - 1) < 0 ? queue_size - 1 : m_back - 1];
            m_consumer_lock.unlock();
            return {res};
        }
        else
            return {};
    }

    template<typename T, size_t queue_size>
    constexpr bool atomic_queue<T, queue_size>::try_push(weak_same_type<value_type> auto &&data) {
        if(m_producer_lock.try_lock()){
            m_data[m_back] = std::forward(data);
            m_data = (m_data + 1 >= queue_size) ? 0 : m_data + 1;
            m_producer_lock.unlock();
            return true;
        }else
            return false;
    }


}

#endif //SSTL_ATOMIC_QUEUE_H
