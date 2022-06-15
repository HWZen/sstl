//
// Created by HWZen on 2022/6/15.
// Copyright (c) 2022 HWZen All rights reserved.
// MIT License
//

#include <gtest/gtest.h>
#include <array.h>

TEST(arrayInit, initWithoutArg) {
    using namespace sstd;
    array<int, 1024> a;
    array<double, 102400000> b;
}

TEST(arrayInit, initWithInitList) {
    using namespace sstd;
    array<int, 1024> a{{1,2,3,4,5}};
}

TEST(arrayInit, copy){
    using namespace sstd;
    array<int, 1024> a{{1,2,3,4,5}};
    array<int, 1024> b(a);
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[4], 5);
    EXPECT_EQ(b[0], 1);
    EXPECT_EQ(b[4], 5);
}

TEST(arrayInit, move){
    using namespace sstd;
    array<int, 1024> a{{1,2,3,4,5}};
    array<int, 1024> b(std::move(a));
    EXPECT_EQ(b[0], 1);
    EXPECT_EQ(b[4], 5);
    EXPECT_EQ(a.begin().operator->(), nullptr);
}

TEST(arrayInit, moveItself){
    using namespace sstd;
    array<int, 1024> a{{1,2,3,4,5}};
    a = std::move(a);
    EXPECT_EQ(a[0], 1);
}

TEST(arrayFunction, beginAndEnd){
    using namespace sstd;
    array<int, 5> a{{1,2,3,4,5}};
    auto it = a.begin();
    EXPECT_EQ(*it, 1);
}


