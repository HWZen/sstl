//
// Created by HWZen on 2022/6/18.
// Copyright (c) 2022 HWZen All rights reserved.
// MIT License
//

#include "sstl/algorithm.h"
#include <algorithm>
#include "sstl/array.h"
#include "sstl/sstdio.h"

#include <vector>

const size_t array_size = 100'000'000;

int main()
{

    sstd::array<int, array_size> a;
    // random for a
    auto t0{sstd::get_time()};
    for (auto &i : a)
        i = rand();
    sstd::Println("general random array time: ", sstd::get_time() - t0);

    t0 = sstd::get_time();
    auto a2{a};
    sstd::Println("copy array time: ", sstd::get_time() - t0);

    t0 = sstd::get_time();
    sstd::parallel_qsort(a);
    sstd::Println("sstd::parallel_qsort(array) time: ", sstd::get_time() - t0);

    t0 = sstd::get_time();
    std::vector<int> v(a2.begin(), a2.end());
    sstd::Println("sstd::array copy to std::vector time: ", sstd::get_time() - t0);

    t0 = sstd::get_time();
    auto v2{v};
    sstd::Println("std::vector copy time: ", sstd::get_time() - t0);

    t0 = sstd::get_time();
    std::sort(v.begin(), v.end());
    sstd::Println("std::sort(vector) time: ", sstd::get_time() - t0);

    //check v == a
    t0 = sstd::get_time();
    for(size_t i{}; i < a.size(); ++i)
        if(v[i] != a[i]) [[unlikely]]
        {
            sstd::Println("error");
            break;
        }
    sstd::Println("check time: ", sstd::get_time() - t0);

    //check v2 == a2
    t0 = sstd::get_time();
    for(size_t i{}; i < a2.size(); ++i)
        if(v2[i] != a2[i]) [[unlikely]]
        {
            sstd::Println("error");
            break;
        }

    t0 = sstd::get_time();
    sstd::parallel_qsort(v2);
    sstd::Println("sstd::parallel_qsort(vector) time: ", sstd::get_time() - t0);

    t0 = sstd::get_time();
    std::sort(a2.begin(), a2.end());
    sstd::Println("std::sort(array) time: ", sstd::get_time() - t0);

}

