//
// Created by HWZen on 2022/6/18.
// Copyright (c) 2022 HWZen All rights reserved.
// MIT License
// 

#ifndef SSTL_ALGORITHM_H
#define SSTL_ALGORITHM_H

#include <functional.h>
#include <semaphore>
#include <thread>
#include <functional>
#include <queue>
#include <atomic>
#include "pair.h"
#include "atomic_queue.h"

namespace sstd{

    template<sortable Ty>
    void qsort(Ty first, Ty last, auto &&cmp ){

        if(first >= last)
            return;
        auto mid = *first;
        auto left = first;
        auto right = last - 1;

        while(left < right){
            while(left < right && !cmp(*right, mid))
                --right;
            *left = *right;
            while(left < right && cmp(*left, mid))
                ++left;
            *right = *left;
        }
        *left = mid;

        if(first < left)
            qsort(first, left, cmp);
        if(last > right)
            qsort(right + 1, last, cmp);
    }

    template<sortable Ty>
    void qsort(Ty first, Ty last){
        qsort(first, last, [](auto &&left, auto &&right){
            return left < right;
        });
    }

    void qsort(is_vector_like auto &&ty){
        qsort(ty.begin(), ty.end());
    }

    template<sortable Ty>
#if _MSC_VER
    [[deprecated("parallel_qsort has performance issue in MSVC")]]
#endif // _MSC_VER
    void parallel_qsort(Ty first, Ty last, auto &&comp){

        class parallel_qsort_thread_poll{
        private:
            std::thread *m_ths;
            size_t m_threadNums;
            std::function<void(Ty,Ty)> m_task;

        public:

            sstd::atomic_queue<sstd::pair<Ty, Ty>> m_taskQueue;

            std::atomic_bool m_stopSignal{false};

            explicit parallel_qsort_thread_poll(size_t threadNums) : m_threadNums(threadNums) {
                auto loopTask = [&] {
                    while (!m_stopSignal) {
                        auto args = m_taskQueue.try_pop_for(std::chrono::milliseconds(1));
                        if (args.has_value())
                            m_task(args->first, args->second);
                    }
                };

                m_ths = new std::thread[m_threadNums];
                for (size_t i{}; i < m_threadNums; ++i)
                    m_ths[i] = std::thread(loopTask);
            }

            void stop(){
                for(size_t i{}; i < m_threadNums; ++i){
                    if(m_ths[i].joinable())
                        m_ths[i].join();
                }
            }

            constexpr void setTask(std::function<void(Ty, Ty)> task){
                m_task = std::move(task);
            }

            ~parallel_qsort_thread_poll(){
                stop();
                delete[] m_ths;
            }
        };

        parallel_qsort_thread_poll threadPoll(std::thread::hardware_concurrency());

        std::counting_semaphore cnt{0};
        std::function<void(Ty,Ty)> core = [&](Ty first, Ty last){
            if(last < first)
                throw std::runtime_error("parallel_qsort: first > last");

            if(last - first <= 1){
                if(last - first == 1)
                    cnt.release();
                return;
            }


            // if range is small, use traditional qsort
            if(last - first < 50000){
                sstd::qsort(first, last, comp);
                for(size_t i{}; i < last - first; ++i)
                    cnt.release();
                return;
            }

            auto mid = *first;
            auto left = first;
            auto right = last - 1;

            while(left < right){
                while(left < right && !comp(*right, mid))
                    --right;
                *left = *right;
                while(left < right && comp(*left, mid))
                    ++left;
                *right = *left;
            }
            *left = mid;

            cnt.release();

            if(first < left)
                threadPoll.m_taskQueue.push(sstd::pair{first, left});

            if(last > right)
                threadPoll.m_taskQueue.push(sstd::pair{right + 1, last});

        };

        threadPoll.setTask(core);

        threadPoll.m_taskQueue.push(sstd::pair{first, last});
        for(size_t i{};i<last - first;++i)
            cnt.acquire();
        threadPoll.m_stopSignal = true;
    }


    template<sortable Ty>
    void parallel_qsort(Ty first, Ty last){
        parallel_qsort(first, last, [](auto &&left, auto &&right) {
            return left < right;
        });
    }

    void parallel_qsort(is_vector_like auto &&ty){
        parallel_qsort(ty.begin(), ty.end());
    }

}

#endif //SSTL_ALGORITHM_H
