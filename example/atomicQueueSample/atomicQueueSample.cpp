//
// Created by HWZen on 2022/8/3.
// Copyright (c) 2022 HWZen All rights reserved.
// MIT License
//

#include <atomic_queue.h>
#include <sstdio.h>
#include <random>
#include <thread>

int main() {
    std::mt19937_64 gen64(std::random_device{}());
    sstd::atomic_queue<uint64_t> q;
    std::counting_semaphore cnt(0);
    auto t0{sstd::get_time()};
    for (size_t i{}; i < std::thread::hardware_concurrency(); i++) {

        std::thread([&q, &gen64, &cnt]() {
            for (size_t i{}; i < 100'000'0; ++i) {
                auto num = gen64();
                switch (num % 4) {
                    case 0:
                        q.push(num);
                        break;
                    case 1:
                        q.try_pop();
                        break;
                    case 2:
                        q.try_front();
                        break;
                    case 3:
                        q.try_back();
                        break;
                    default:
                        sstd::Println("unknown num: ", num % 3);
                        exit(1);
                }
            }
            cnt.release();
        }).detach();
    }

    for (size_t i{}; i < std::thread::hardware_concurrency(); ++i)
        cnt.acquire();
    sstd::Println("time: ", sstd::get_time() - t0);

}

