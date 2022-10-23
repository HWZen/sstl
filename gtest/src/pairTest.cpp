//
// Created by HWZen on 2022/6/17.
// Copyright (c) 2022 HWZen All rights reserved.
// MIT License
//

#include "sstl/pair.h"
#include <gtest/gtest.h>

TEST(pairTest, initWithoutArg) {
    using namespace sstd;
    pair<int, int> a{};
}

TEST(pairTest, aggregateInitialization){
    using namespace sstd;
    pair<int, int> a{1, 2};
    EXPECT_EQ(a.first, 1);
    EXPECT_EQ(a.second, 2);
}

TEST(pairTest, move){
    using namespace sstd;
    struct A{
        int a;
        int b;
        A(A&& other) noexcept{
            a = other.a;
            b = other.b;
            other.a = 0;
            other.b = 0;
        }
        A(int a, int b){
            this->a = a;
            this->b = b;
        }
    };

    pair p{A{1, 2}, A{3, 4}};
    pair p2(std::move(p));
    EXPECT_EQ(p2.first.a, 1);
    EXPECT_EQ(p2.first.b, 2);
    EXPECT_EQ(p2.second.a, 3);
    EXPECT_EQ(p2.second.b, 4);
    EXPECT_EQ(p.first.a, 0);
    EXPECT_EQ(p.first.b, 0);
    EXPECT_EQ(p.second.a, 0);
    EXPECT_EQ(p.second.b, 0);
}

TEST(pairTest, autoDerivation){
    using namespace sstd;
    pair a{1, 2};
    EXPECT_EQ(a.first, 1);
    EXPECT_EQ(a.second, 2);

    struct A{
        int a;
        int b;
    };

    pair b{12.235, A{12,77}};
    EXPECT_EQ(b.first, 12.235);
    EXPECT_EQ(b.second.a, 12);
    EXPECT_EQ(b.second.b, 77);
}
