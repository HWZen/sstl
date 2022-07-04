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
    void qsort(Ty first, Ty last, auto &&comp ){

        if(first >= last)
            return;
        auto t = *first;
        auto f = first;
        auto l = last - 1;

        while(f < l){
            while(f < l && !comp(*l, t))
                --l;
            *f = *l;
            while(f < l && comp(*f, t))
                ++f;
            *l = *f;
        }
        *f = t;

        if(first < f)
            qsort(first, f, comp);
        if(last > l)
            qsort(l + 1, last, comp);
    }

    template<sortable Ty>
    void qsort(Ty first, Ty last){
        qsort(first, last, [](auto &&a, auto &&b){
            return a < b;
        });
    }

    void qsort(is_vector_like auto &&ty){
        qsort(ty.begin(), ty.end());
    }

#define Type 1


#if Type == 1
    template<sortable Ty>
    void parallel_qsort(Ty first, Ty last, auto &&comp){

        class parallel_qsort_thread_poll{
        public:
            size_t m_threadNums;
            std::thread *m_ths;
            std::queue<std::function<void()>> m_taskQueue;
            std::mutex m_taskQueueLock;
            std::atomic_bool m_stopSignal{false};
            parallel_qsort_thread_poll(size_t threadNums) : m_threadNums(threadNums) {
                m_ths = new std::thread[m_threadNums];
                for (size_t i{}; i < m_threadNums; ++i) {
                    m_ths[i] = std::thread([&] {
                        while (!m_stopSignal) {
                            if(m_taskQueueLock.try_lock()){
                                if(!m_taskQueue.empty()){
                                    auto task = m_taskQueue.front();
                                    m_taskQueue.pop();
                                    m_taskQueueLock.unlock();
                                    task();
                                }
                                else{
                                    m_taskQueueLock.unlock();
                                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                                }
                            }
                            else{
                                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                            }
                        }
                    });
                }
            }

            void stop(){
                for(size_t i{}; i < m_threadNums; ++i){
                    if(m_ths[i].joinable())
                        m_ths[i].join();
                }
            }

            ~parallel_qsort_thread_poll(){
                stop();
                delete[] m_ths;
            }
        };

        std::atomic_size_t cnt{0};

        // Max thread nums
//        std::counting_semaphore thread_count(std::thread::hardware_concurrency());

        parallel_qsort_thread_poll threadPoll(std::thread::hardware_concurrency());


        // callback: when function finished, invoke callback
        std::function<void(Ty,Ty)> core = [&](Ty first, Ty last){
            if(last < first)
                throw std::runtime_error("parallel_qsort: first > last");

            if(last - first <= 1){
                if(last - first == 1)
                ++cnt;
                return;
            }


            // if range is small, use traditional qsort
            if(last - first < 10000){
                sstd::qsort(first, last, comp);
                cnt += last - first;
                return;
            }

            // acquire a thread to avoid too many threads created
//            thread_count.acquire();

            auto t = *first;
            auto f = first;
            auto l = last - 1;

            while(f < l){
                while(f < l && !comp(*l, t))
                    --l;
                *f = *l;
                while(f < l && comp(*f, t))
                    ++f;
                *l = *f;
            }
            *f = t;

            ++cnt;

            if(first < f){
                threadPoll.m_taskQueueLock.lock();
                threadPoll.m_taskQueue.push([=](){
                    core(first, f);
                });
                threadPoll.m_taskQueueLock.unlock();
//                std::thread{core, first, f, [=]() {// set callback: when finished, release thread_count, and if other state is finished, means this core's work is down. invoke callback captured in this core.
//                    state->release();
//                }}.detach();
            }


            if(last > l){
                threadPoll.m_taskQueueLock.lock();
                threadPoll.m_taskQueue.push([=](){
                    core(l + 1, last);
                });
                threadPoll.m_taskQueueLock.unlock();
            }

//            thread_count.release();
        };


        threadPoll.m_taskQueueLock.lock();
        threadPoll.m_taskQueue.push([&](){
            core(first, last);
        });
        threadPoll.m_taskQueueLock.unlock();
        while (cnt < last - first)
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        threadPoll.m_stopSignal = true;
    }
#elif Type == 2
    template<sortable Ty>
    void parallel_qsort(Ty first, Ty last, auto &&comp){

        std::counting_semaphore thread_count(std::thread::hardware_concurrency());

        std::function<void(Ty, Ty)> core = [&comp, &core, &thread_count](Ty _first, Ty _last) {

            auto _sort = [&comp](auto &&t, auto &&f, auto &&l){
                while (f < l) {
                    while (f < l && !comp(*l, t))
                        --l;
                    *f = *l;
                    while (f < l && comp(*f, t))
                        ++f;
                    *l = *f;
                }
                *f = t;
                return true;
            };

            while(true){
                if (_last - _first <= 1)
                    return;

                auto t = *_first;
                auto f = _first;
                auto l = _last - 1;
                if(!_sort(t,f,l))
                    return;

                std::thread th[2];
                if(thread_count.try_acquire())
                    th[0] = std::thread(core, _first, f);
                else
                    core(_first, f);

                if((thread_count.try_acquire()))
                    th[1] = std::thread(core, f + 1, _last);
                else
                    core(f + 1, _last);

                if(th[0].joinable()){
                    th[0].join();
                    thread_count.release();
                }
                if(th[1].joinable()){
                    th[1].join();
                    thread_count.release();
                }
            }


        };
        core(first, last);
    }
#endif // Type

    template<sortable Ty>
    void parallel_qsort(Ty first, Ty last){
        parallel_qsort(first, last, [](auto &&a, auto &&b){
            return a < b;
        });
    }

    void parallel_qsort(is_vector_like auto &&ty){
        parallel_qsort(ty.begin(), ty.end());
    }

}

#endif //SSTL_ALGORITHM_H
