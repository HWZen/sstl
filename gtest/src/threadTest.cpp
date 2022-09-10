//
// Created by HWZen on 2022/8/15.
// Copyright (c) 2022 HWZen All rights reserved.
// MIT License
//
#include <gtest/gtest.h>
#include "thread.h"
#include "sstdio.h"

static void foo1() noexcept{
}

static void foo2(int i) noexcept{
    ++i;
}

static int foo3(int l, int r) noexcept{
    return l + r;
}

TEST(threadCStyleTest, createNativeFd)
{

    auto fd = sstd::createNativeFd(sstd::g_stackSize, &foo1);
    sstd::join(fd);
    fd = sstd::createNativeFd(sstd::g_stackSize, &foo2, 100);
    sstd::join(fd);
    fd = sstd::createNativeFd(sstd::g_stackSize, &foo3, 170001, 114514);
    sstd::join(fd);
    fd = sstd::createNativeFd(sstd::g_stackSize, [&](int i){
        ++i;
        }, 100);
    sstd::join(fd);
}

TEST(threadCStyleTest, isNullFd)
{
    sstd::threadFd fd = sstd::nullThreadFd;
    EXPECT_TRUE(sstd::isNullFd(fd));
    fd = sstd::createNativeFd(sstd::g_stackSize, &foo1);
    EXPECT_FALSE(sstd::isNullFd(fd));
    sstd::join(fd);
}

TEST(threadCStyleTest, join)
{
    sstd::threadFd fd = sstd::nullThreadFd;
    sstd::join(fd);
    fd = sstd::createNativeFd(sstd::g_stackSize, &foo1);
    sstd::join(fd);
}

TEST(threadCStyleTest, detach)
{
    sstd::threadFd fd = sstd::nullThreadFd;
    sstd::detach(fd);
    fd = sstd::createNativeFd(sstd::g_stackSize, &foo1);
    sstd::detach(fd);
}

#ifndef DISABLE_THREAD_TERMINATION_TEST
TEST(threadCStyleTest, terminate)
{

    int sign{0};
    auto fd = sstd::createNativeFd(sstd::g_stackSize, [&]() {
        sign = 1;
        auto t0{sstd::get_time()};
        while((sstd::get_time() - t0).subseconds < 350000000000000000){
            sstd::ThisThread::yield();
        }
        sign = 2;
    });
    sstd::ThisThread::sleep(5);
    sstd::terminate(fd);
    sstd::ThisThread::sleep(50);
    EXPECT_EQ(sign, 1);
}
#endif // DISABLE_THREAD_TERMINATION_TEST

TEST(threadCStyleTest, getCpuNums)
{
    sstd::Println(__LINE__,":",__FUNCTION__,":",sstd::getCpuNums());
    EXPECT_EQ(sstd::getCpuNums(), sstd::getCpuNums());
}



TEST(thisThreadTest, getThreadFd)
{
    sstd::threadFd fd = sstd::ThisThread::getThreadFd();
    EXPECT_FALSE(sstd::isNullFd(fd));
}

TEST(thisThreadTest, yield)
{
    sstd::ThisThread::yield();
}

TEST(thisThreadTest, sleep)
{
    auto t0{sstd::get_time()};
    sstd::ThisThread::sleep(50);
    auto t1{sstd::get_time()};
    EXPECT_LE((t1 - t0).subseconds, 700000000000000000);
    EXPECT_GT((t1 - t0).subseconds, 450000000000000000);
}

TEST(thisThreadTest, yieldAndExit)
{
    int sign{1};
    auto fd = sstd::createNativeFd(sstd::g_stackSize, [&](){
        while(sign){
            sstd::ThisThread::yield();
        }
        sign = 2;
        sstd::ThisThread::exit();
        sign = 3;
    });
    sstd::ThisThread::sleep(5);
    sign = 0;
    sstd::join(fd);
    EXPECT_EQ(sign, 2);
}

TEST(threadClassTest, init)
{
    sstd::thread th1(&foo1);
    th1.join();
    sstd::thread th2(&foo2, 100);
    th2.join();
    sstd::thread th3(foo3, 170001, 114514);
    th3.join();
    sstd::thread th4([&](int i){
        ++i;
        }, 100);
    th4.join();
}

TEST(threadClassTest, detach)
{
    sstd::thread th1(&foo1);
    th1.detach();
    sstd::thread th2(&foo2, 100);
    th2.detach();
    sstd::thread th3(foo3, 170001, 114514);
    th3.detach();
    sstd::thread th4([&](int i){
        ++i;
        }, 100);
    th4.detach();
    sstd::ThisThread::sleep(5);
}

#ifndef DISABLE_THREAD_TERMINATION_TEST
TEST(threadClassTest, terminate)
{
    int sign{0};
    sstd::thread th1([&](){
        sign = 1;
        auto t0{sstd::get_time()};
        while((sstd::get_time() - t0).subseconds < 350000000000000000){
            sstd::ThisThread::yield();
        }
        sign = 2;
    });
    sstd::ThisThread::sleep(5);
    th1.terminate();
    sstd::ThisThread::sleep(50);
    EXPECT_EQ(sign, 1);
}
#endif // DISABLE_THREAD_TERMINATION_TEST

TEST(threadClassTest, status)
{
    sstd::thread<> th1;
    EXPECT_EQ(sstd::threadStatus::UNSTART, th1.getStatus());
    th1.start([](){
        sstd::ThisThread::sleep(50);
    });
    sstd::ThisThread::sleep(10);
    EXPECT_EQ(sstd::threadStatus::RUNNING, th1.getStatus());
    th1.join();
    EXPECT_EQ(sstd::threadStatus::FINISHED, th1.getStatus());

    sstd::thread<> th2([](){
        throw std::runtime_error("an exception");
    });
    th2.join();
    EXPECT_EQ(sstd::threadStatus::EXCEPTED, th2.getStatus());

    sstd::thread<> th3([](){
        sstd::ThisThread::sleep(5);
    });
    th3.detach();
    EXPECT_EQ(sstd::threadStatus::DETACHED, th3.getStatus());
    sstd::ThisThread::sleep(30);
    EXPECT_EQ(sstd::threadStatus::FINISHED, th3.getStatus());
}

TEST(threadClassTest, getThreadFd)
{
    int sign{0};
    sstd::thread th1([&](){
        sign = 1;
        auto t0{sstd::get_time()};
        while((sstd::get_time() - t0).subseconds < 350000000000000000){
            sstd::ThisThread::yield();
        }
        sign = 2;
    });
    auto fd = th1.getRawFd();
    EXPECT_FALSE(sstd::isNullFd(fd));
#ifndef DISABLE_THREAD_TERMINATION_TEST
    sstd::ThisThread::sleep(5);
    sstd::terminate(fd);
    sstd::ThisThread::sleep(50);
    EXPECT_EQ(sign, 1);
#endif // DISABLE_THREAD_TERMINATION_TEST
}

TEST(threadClassTest, getResult)
{
    sstd::thread th1(foo3, 170001, 114514);
    th1.join();
    EXPECT_EQ(th1.getStatus(), sstd::threadStatus::FINISHED);
    EXPECT_EQ(*th1.getResult()->val, 170001 + 114514);

    sstd::thread th2([](unsigned int i){
        uint64_t res{1};
        while (i){
            res *= i--;
        }
        return res;
    }, 15);
    th2.join();
    EXPECT_EQ(th2.getStatus(), sstd::threadStatus::FINISHED);
    EXPECT_EQ(*th2.getResult()->val, 1307674368000);


}


