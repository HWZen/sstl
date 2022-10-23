//
// Created by HWZen on 2022/7/4.
// Copyright (c) 2022 HWZen All rights reserved.
// MIT License
//

#include <gtest/gtest.h>
#include "sstl/atomic_queue.h"

TEST(queueInit, initWithoutArgs){
    sstd::atomic_queue<int> q;
}

TEST(queueInit, moveConstruct){
    sstd::atomic_queue<int> q;
    auto q2{std::move(q)};
}

TEST(queueFunc, sizeAndPushAndPop){
    sstd::atomic_queue<int> q;
    EXPECT_EQ(q.size(), 0);
    q.push(128);
    EXPECT_EQ(q.size(), 1);
    EXPECT_EQ(q.front(), 128);
    EXPECT_EQ(q.pop(), 128);
    EXPECT_EQ(q.size(), 0);

    sstd::atomic_queue<std::function<void()>> q2;
    std::function<void()> f = [](){};
    q2.push(f);
    q2.push([](){});
}

TEST(queueFunc, frontAndBack){
    sstd::atomic_queue<int> q;
    q.push(1);
    q.push(2);
    EXPECT_EQ(q.front(), 1);
    EXPECT_EQ(q.back(), 2);
}

TEST(queueFunc, tryFunc){
    sstd::atomic_queue<int> q;
    EXPECT_EQ(q.try_front(), std::nullopt);
    EXPECT_EQ(q.try_back(), std::nullopt);
    EXPECT_EQ(q.try_pop(), std::nullopt);

    EXPECT_EQ(q.try_push(1), true);

    EXPECT_EQ(q.try_front(), 1);
    EXPECT_EQ(q.try_back(), 1);
    EXPECT_EQ(q.try_pop(), 1);

    EXPECT_EQ(q.try_push(1), true);
    EXPECT_EQ(q.try_push(2), true);
    EXPECT_EQ(q.try_push(3), true);

}
