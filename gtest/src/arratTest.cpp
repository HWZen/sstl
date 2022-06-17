//
// Created by HWZen on 2022/6/15.
// Copyright (c) 2022 HWZen All rights reserved.
// MIT License
//

#include <gtest/gtest.h>
#include <array.h>

#include<array>

TEST(arrayInit, initWithoutArg) {
    using namespace sstd;
    array<int, 1024> a;
    array<double, 102400000> b;
}

TEST(arrayInit, initWithInitList) {
    using namespace sstd;
    array<int, 1024> a{{1, 2, 3, 4, 5}};
}

TEST(arrayInit, autoDerivation) {
    using namespace sstd;
    array a{1, 2, 3, 4, 5};
}

TEST(arrayInit, copy) {
    using namespace sstd;
    array<int, 1024> a{{1, 2, 3, 4, 5}};
    array<int, 1024> b(a);
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[4], 5);
    EXPECT_EQ(b[0], 1);
    EXPECT_EQ(b[4], 5);
}

TEST(arrayInit, move) {
    using namespace sstd;
    array<int, 1024> a{{1, 2, 3, 4, 5}};
    array<int, 1024> b(std::move(a));
    EXPECT_EQ(b[0], 1);
    EXPECT_EQ(b[4], 5);
    EXPECT_EQ(a.begin().operator->(), nullptr);
}

TEST(arrayInit, moveItself) {
    using namespace sstd;
    array<int, 1024> a{{1, 2, 3, 4, 5}};
    a = std::move(a);
    EXPECT_EQ(a[0], 1);
}

TEST(arraryFunction, size) {
    using namespace sstd;
    array<int, 1024> a{{1, 2, 3, 4, 5}};
    EXPECT_EQ(a.size(), 1024);
    array<int, 1024> b;
    EXPECT_EQ(b.size(), 1024);
}

TEST(arrayFunction, beginAndEnd) {
    using namespace sstd;
    array<int, 5> a{{1, 2, 3, 4, 5}};
    auto it = a.begin();
    EXPECT_EQ(*it, 1);
}

TEST(arrayFunction, operatorStar) { // operator*
    using namespace sstd;
    array<int, 1024> a{{1, 2, 3, 4, 5}};
    auto it = a.begin();
    EXPECT_EQ(*it, 1);
    *it = 10;
    EXPECT_EQ(*it, 10);
    auto cit = a.cbegin();
    EXPECT_EQ(*cit, 10);
    auto res = std::is_same_v<decltype(*cit), const int&>;
    EXPECT_TRUE(res);
}

TEST(arrayFunction, operatorTo) {// operator->
    using namespace sstd;
    struct S {
        int a;
        int b;
    };
    array<S, 1024> a{{{1, 2}, {3, 4}, {5, 6}}};
    auto it = a.begin();
    EXPECT_EQ(it->a, 1);
    it->a = 10;
    EXPECT_EQ(it->a, 10);
    auto cit = a.cbegin();
    EXPECT_EQ(cit->a, 10);
    auto res = std::is_same_v<decltype(cit.operator->()), S const *>;
    EXPECT_TRUE(res);
}

TEST(arrayFunction, operatorPlusPlus) { // operator++\operator++(int)
    using namespace sstd;
    array<int, 1024> a{{1, 2, 3, 4, 5}};
    auto it = a.begin();
    EXPECT_EQ(*it++, 1);
    EXPECT_EQ(*it, 2);
    EXPECT_EQ(*++it, 3);

    auto cit = a.cbegin();
    EXPECT_EQ(*cit++, 1);
    EXPECT_EQ(*cit, 2);
    EXPECT_EQ(*++cit, 3);
}

TEST(arrayFunction, operatorMinusMinus) { // operator--\operator--(int)
    using namespace sstd;
    array a{1, 2, 3, 4, 5};
    auto it = a.end();
    EXPECT_EQ(*--it, 5);
    EXPECT_EQ(*it--, 5);
    EXPECT_EQ(*it, 4);

    auto cit = a.cend();
    EXPECT_EQ(*--cit, 5);
    EXPECT_EQ(*cit--, 5);
    EXPECT_EQ(*cit, 4);
}

TEST(arrayFunction, operatorEqual){ // operator==
    using namespace sstd;
    array<int, 1024> a{{1, 2, 3, 4, 5}};
    auto it = a.begin();
    auto it2 = a.begin();
    EXPECT_EQ(it, it2);
    ++it2;
    EXPECT_NE(it, it2);

    auto cit = a.cbegin();
    EXPECT_EQ(cit, a.cbegin());
    EXPECT_EQ(cit, it);
    EXPECT_NE(cit, it2);
}

TEST(arrayFunction, frontAndBack) {
    using namespace sstd;
    array a{1, 2, 3, 4, 5};
    EXPECT_EQ(a.front(), 1);
    EXPECT_EQ(a.back(), 5);
    a.front() = 10;
    a.back() = 20;
    EXPECT_EQ(a.front(), 10);
    EXPECT_EQ(a.back(), 20);
}

TEST(arrayFunction, at) { // operator[]
    using namespace sstd;
    array<int, 1024> a{{1, 2, 3, 4, 5}};
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[4], 5);
    a[0] = 10;
    a[4] = 20;
    EXPECT_EQ(a[0], 10);
    EXPECT_EQ(a[4], 20);
}

TEST(arrayFunction, constFrontAndBack) {
    using namespace sstd;
    const array a{1, 2, 3, 4, 5};
    EXPECT_EQ(a.front(), 1);
    EXPECT_EQ(a.back(), 5);
    // check return type
    auto res = std::is_same<decltype(a.front()), const int&>::value;
    EXPECT_TRUE(res);
    res = std::is_same<decltype(a.back()), const int&>::value;
    EXPECT_TRUE(res);
}

TEST(arrayFunction, swap){
    using namespace sstd;
    array a{1, 2, 3, 4, 5};
    array b{6, 7, 8, 9, 10};
    std::swap(a, b);
    EXPECT_EQ(a[0], 6);
    EXPECT_EQ(a[4], 10);
    EXPECT_EQ(b[0], 1);
    EXPECT_EQ(b[4], 5);
    a.swap(b);
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[4], 5);
    EXPECT_EQ(b[0], 6);
    EXPECT_EQ(b[4], 10);
    a.swap(a);
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[4], 5);
}

TEST(arrayFunction, copyAssignment){
    using namespace sstd;
    array a{1, 2, 3, 4, 5};
    array<int, 5> b{};
    b = a;
    EXPECT_EQ(b[0], 1);
    EXPECT_EQ(b[4], 5);
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[4], 5);
    b = b;
    EXPECT_EQ(b[0], 1);
    EXPECT_EQ(b[4], 5);
}

TEST(arrayFunction, moveAssignment){
    using namespace sstd;
    array a{1, 2, 3, 4, 5};
    array<int, 5> b{};
    b = std::move(a);
    EXPECT_EQ(b[0], 1);
    EXPECT_EQ(b[4], 5);
    a = std::move(b);
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[4], 5);

    a = std::move(a);
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[4], 5);
}

TEST(comprehensive_test, test1){
    using namespace sstd;
    array<int, 1024> a{{1, 2, 3, 4, 5}};
    EXPECT_EQ(a.size(), 1024);
    EXPECT_EQ(a[4], 5);
    a[5] = 100;
    EXPECT_EQ(a[5], 100);
    *--a.end() = 200;
    EXPECT_EQ(a.back(), 200);
}




