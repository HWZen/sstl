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
            std::thread *m_ths;
            std::counting_semaphore<std::__semaphore_impl::_S_max> m_taskCnt;
            std::queue<std::function<void()>> m_taskQueue;
            std::atomic_bool m_stopSignal{false};
        public:
            parallel_qsort_thread_poll() {
                m_ths = new std::thread[std::thread::hardware_concurrency()];
                for (size_t i{}; i < std::thread::hardware_concurrency(); ++i) {
                    m_ths[i] = std::thread([&] {
                        while (!m_stopSignal) {
                            if (m_taskCnt.template try_acquire_for(std::chrono::milliseconds(1))) {

                            }

                        }

                    });
                }
            }


        };

        // Max thread nums
        std::counting_semaphore thread_count(std::thread::hardware_concurrency());

        // callback: when function finished, invoke callback
        std::function<void(Ty,Ty, std::function<void()>)> core = [&](Ty first, Ty last, std::function<void()> callback){
            if(last < first)
                throw std::runtime_error("parallel_qsort: first > last");

            if(last - first <= 1){
                callback();
                return;
            }

            // if range is small, use traditional qsort
//            if(last - first < 50000){
//                sstd::qsort(first, last, comp);
//                callback();
//                return;
//            }

            // acquire a thread to avoid too many threads created
            thread_count.acquire();

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

            auto state = new std::binary_semaphore{0};
            if(first < f){
                std::thread{core, first, f, [=]() {// set callback: when finished, release thread_count, and if other state is finished, means this core's work is down. invoke callback captured in this core.
                    state->release();
                }}.detach();
            }
            else
                state->release();

            std::function foo = [=]{
                state->acquire();
                delete state;
                callback();
            };
            if(last > l){
                std::thread{core, l + 1, last, foo}.detach();
            }
            else
                std::thread(foo).detach();

            thread_count.release();
        };

        std::binary_semaphore state{0};
        core(first, last, [&](){
            state.release();
        });
        state.acquire();

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
