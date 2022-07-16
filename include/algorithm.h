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
#if _MSC_VER
    [[deprecated("parallel_qsort has performance issue in MSVC")]]
#endif // _MSC_VER
    void parallel_qsort(Ty first, Ty last, auto &&comp){

        class parallel_qsort_thread_poll{
            std::atomic_size_t m_aszTaskCnt{ 0 };
        public:
            size_t m_threadNums;
            std::thread *m_ths;
            sstd::atomic_queue<sstd::pair<Ty, Ty>> m_taskQueue;
            std::function<void(Ty,Ty)> m_task;
            std::atomic_bool m_stopSignal{false};
            explicit parallel_qsort_thread_poll(size_t threadNums) : m_threadNums(threadNums) {
                m_ths = new std::thread[m_threadNums];
                for (size_t i{}; i < m_threadNums; ++i) {
                    m_ths[i] = std::thread([&] {
                        while (!m_stopSignal) {
                            auto args = m_taskQueue.try_pop_for(std::chrono::milliseconds(1));
                            if (args.has_value()) {
                                ++m_aszTaskCnt;
                                m_task(args->first, args->second);
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

        std::counting_semaphore cnt{0};

        // Max thread nums
//        std::counting_semaphore thread_count(std::thread::hardware_concurrency());

        parallel_qsort_thread_poll threadPoll(std::thread::hardware_concurrency());



        // callback: when function finished, invoke callback
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
                for(size_t i{};i<last - first;++i)
                    cnt.release();
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

            cnt.release();

            if(first < f){
                threadPoll.m_taskQueue.push(sstd::pair{first,f});
//                std::thread{core, first, f, [=]() {// set callback: when finished, release thread_count, and if other state is finished, means this core's work is down. invoke callback captured in this core.
//                    state->release();
//                }}.detach();
            }


            if(last > l){
                threadPoll.m_taskQueue.push(sstd::pair{l+1,last});
            }

//            thread_count.release();
        };

        threadPoll.m_task = core;

        threadPoll.m_taskQueue.push(sstd::pair{first, last});
        for(size_t i{};i<last - first;++i)
            cnt.acquire();
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
        parallel_qsort(first, last, [](auto &&a, auto &&b) {
            return a < b;
        });
    }

    void parallel_qsort(is_vector_like auto &&ty){
        parallel_qsort(ty.begin(), ty.end());
    }

}

#endif //SSTL_ALGORITHM_H
