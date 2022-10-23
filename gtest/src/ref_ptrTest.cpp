//
// Created by HWZen on 2022/6/17.
// Copyright (c) 2022 HWZen All rights reserved.
// MIT License
//
#include "sstl/ref_ptr.h"
#include <gtest/gtest.h>

TEST(ref_ptrInit, initWithoutArg){
    using namespace sstd;
    struct Base{};
    struct Derived : public Base{};
    ref_ptr<Base> a{};
    ref_ptr<Derived> b{};
}

TEST(ref_ptrInit, initWithArg){
    using namespace sstd;
    struct Base{};
    struct Derived : public Base{};
    ref_ptr<Base> a{new Base{}};
    ref_ptr<Derived> b{new Derived{}};
    ref_ptr<Base> c{new Derived{}};
}

TEST(ref_ptrInit, autoDerivation){
    using namespace sstd;
    struct Base{};
    struct Derived : public Base{};
    ref_ptr a{new Base{}};
    ref_ptr b{new Derived{}};
}

TEST(ref_ptrInit, copy){
    using namespace sstd;
    struct Base{};
    struct Derived : public Base{};
    ref_ptr<Base> a{new Base{}};
    ref_ptr<Derived> b{new Derived{}};
    ref_ptr<Base> c{new Derived{}};
    ref_ptr<Base> d{a};
    ref_ptr<Derived> e{b};
    ref_ptr<Base> f{c};
    ref_ptr a2{a};
    ref_ptr b2{b};
}

TEST(ref_ptrInit, move) {
    using namespace sstd;
    struct Base {
        int a;
        int b;
    };
    struct Derived : public Base {
        int c;
        int d;
    };
    ref_ptr<Base> a{new Base{1, 2}};
    ref_ptr<Derived> b{new Derived{1, 2, 3, 4}};
    ref_ptr<Base> c{std::move(a)};
    ref_ptr<Base> d{std::move(b)};
    EXPECT_EQ(c->a, 1);
    EXPECT_EQ(c->b, 2);
    auto d2 = (Derived *)(Base *)d;
    EXPECT_EQ(d2->c, 3);
    EXPECT_EQ(d2->d, 4);
}

TEST(ref_ptrFunction, swap){
    using namespace sstd;
    struct Base{};

    struct Derived : public Base{};
    ref_ptr<Base> a{new Derived{}};
    ref_ptr<Derived> b{new Derived{}};
    a.swap(b);
}

TEST(ref_ptrFunction, operatorStar) { // operator*
    using namespace sstd;
    ref_ptr ref = new int{1};
    EXPECT_EQ(*ref, 1);
    *ref = 114514;
    EXPECT_EQ(*ref, 114514);
}

TEST(ref_ptrFunction, operatorArrow) { // operator->
    using namespace sstd;
    struct Base {
        int a;
        int b;
    };
    ref_ptr ref = new Base{1, 2};
    EXPECT_EQ(ref->a, 1);
    EXPECT_EQ(ref->b, 2);
    ref->a = 114514;
    EXPECT_EQ(ref->a, 114514);
}

TEST(ref_ptrFunction, operatorEqual) { // operator==
    using namespace sstd;
    ref_ptr ref1{new int{1}};
    ref_ptr ref2{ref1};
    EXPECT_EQ(ref1, ref2);

    struct Base{};
    struct Derived : public Base{};
    ref_ptr ref3{new Derived{}};
    ref_ptr<Base> ref4{ref3};
    EXPECT_EQ(ref4, ref3);
    EXPECT_EQ(ref3, ref4);

    ref3 = {new Derived{}};
    EXPECT_NE(ref3, ref4);
    EXPECT_NE(ref4, ref3);
}

TEST(ref_ptrFunction, assignEqual) { // operator=
    using namespace sstd;
    struct Base{};
    struct Derived : public Base{};
    ref_ptr ref1{new Derived{}};
    ref_ptr ref2{new Derived{}};
    EXPECT_NE(ref1, ref2);
    ref2 = ref1;
    EXPECT_EQ(ref1, ref2);

    ref_ptr<Base> ref3{new Derived{}};
    EXPECT_NE(ref3, ref1);
    ref3 = ref1;
    EXPECT_EQ(ref3, ref1);
}

TEST(ref_ptrFunction, operatorPVal){
    using namespace sstd;
    auto foo = [](int *){};
    ref_ptr ref{new int{1}};
    foo(ref);
}

TEST(ref_ptrFunction, get){
    using namespace sstd;
    auto p = new int{1};
    ref_ptr ref{p};
    EXPECT_EQ(ref.get(), p);
}
