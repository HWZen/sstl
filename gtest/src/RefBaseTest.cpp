//
// Created by HWZen on 2022/6/18.
// Copyright (c) 2022 HWZen All rights reserved.
// MIT License
//
#include <gtest/gtest.h>
#include "RefBase.h"

using namespace sstd;
class testClass: public RefBase {
public:
    using RefBase::RefBase;
    int &state;
    testClass(int &state) : state(state), RefBase() {}
    testClass(testClass &&other) noexcept : state(other.state), RefBase(std::move(other)) {}
    testClass(const testClass &other) : state(other.state), RefBase(other) {}
    ~testClass() override {
        decCount();
    }
private:
    void zeroCount() override {
        state = -1;
    }
};

TEST(RefBaseTest, test1) {
    int state = 0;
    testClass t(state);
    EXPECT_EQ(state, 0);
    EXPECT_EQ(t.getCount(), 1);
    auto t2{t};
    EXPECT_EQ(t.getCount(), 2);
    auto t3{std::move(t2)};
    EXPECT_EQ(t.getCount(), 2);
}

TEST(RefBaseTest, test2){
    int state = 0;
    auto t = new testClass(state);
    EXPECT_EQ(state, 0);
    EXPECT_EQ(t->getCount(), 1);
    auto t2 = new testClass(*t);
    EXPECT_EQ(t->getCount(), 2);
    auto t3 = new testClass(std::move(*t2));
    EXPECT_EQ(t->getCount(), 2);
    delete t;
    EXPECT_EQ(t3->getCount(), 1);
    delete t3;
    EXPECT_EQ(state, -1);
    EXPECT_EQ(t2->getCount(), 1);
    delete t2;
}

