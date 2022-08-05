//
// Created by HWZ on 2022/8/4.
//
#include <list>
#include <list.h>
#include <sstdio.h>
#include <random>

const size_t g_stepTimes = 100000000;

int main()
{
    sstd::list<int> ssList;
    std::list<int> sList;

    std::mt19937 gen(std::random_device{}());

    auto pArray = new int[g_stepTimes];
    for (size_t i{}; i < g_stepTimes; ++i)
        *(uint32_t*)&pArray[i] = gen();

    auto t0{ sstd::get_time() };
    for (size_t i{}; i < g_stepTimes; ++i)
        sList.push_back(pArray[i]);
    sstd::Println("std::list insert ", g_stepTimes, " time: ", sstd::get_time() - t0);

    t0 = sstd::get_time();
    for (size_t i{}; i < g_stepTimes; ++i)
        ssList.push_back(pArray[i]);
    sstd::Println("sstd::list insert ", g_stepTimes, " time: ", sstd::get_time() - t0);

    t0 = sstd::get_time();
    int64_t sum{};
    for (auto& it: sList)
        sum = (sum + it) % INT32_MAX;
    sstd::Println("std::list range for traverse time: ", sstd::get_time() - t0);

    t0 = sstd::get_time();
    sum = 0;
    for (auto& it: ssList)
        sum = (sum + it) % INT32_MAX;
    sstd::Println("sstd::list range for traverse time: ", sstd::get_time() - t0);

    t0 = sstd::get_time();
    for (size_t i{}; i < g_stepTimes; ++i)
    {
        auto it{ sList.begin() };
        switch (pArray[i] % 10)
        {
        case 0:
        case 1:
        case 2:
        case 3:
            if (it != sList.end())
                ++it;
            else
                --it;
            break;
        case 4:
        case 5:
            if (it != sList.begin())
                --it;
            else
                ++it;
            break;
        case 6:
            it = sList.insert(it, pArray[i]);
            break;
        case 7:
            if (it != sList.end())
                it = sList.erase(it);
            else
                --it;
            break;
        default:
            if (it != sList.end())
                *it = pArray[i];
        }
    }
    sstd::Println("std::list random operation ", g_stepTimes, " time: ", sstd::get_time() - t0);

    t0 = sstd::get_time();
    for (size_t i{}; i < g_stepTimes; ++i)
    {
        auto it{ ssList.begin() };
        switch (pArray[i] % 10)
        {
        case 0:
        case 1:
        case 2:
        case 3:
            if (it != ssList.end())
                ++it;
            else
                --it;
            break;
        case 4:
        case 5:
            if (it != ssList.begin())
                --it;
            else
                ++it;
            break;
        case 6:
            ssList.push(pArray[i], it);
            break;
        case 7:
            if (it != ssList.end())
                ssList.pop(it--);
            else
                --it;
            break;
        default:
            if (it != ssList.end())
                *it = pArray[i];
        }
    }
    sstd::Println("sstd::list random operation ", g_stepTimes, " time: ", sstd::get_time() - t0);

    delete[] pArray;
}
