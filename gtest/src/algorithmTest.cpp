//
// Created by HWZen on 2022/6/18.
// Copyright (c) 2022 HWZen All rights reserved.
// MIT License
//

#include <gtest/gtest.h>
#include "sstl/algorithm.h"
#include "sstl/array.h"
#include "array"
#include "vector"

TEST(qsortTest, test1) {
    int a[] = {1,1,4,5,1,4};
    sstd::qsort(a, a+6, [](auto &&a, auto &&b){
        return a < b;
    });

    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[1], 1);
    EXPECT_EQ(a[2], 1);
    EXPECT_EQ(a[3], 4);
    EXPECT_EQ(a[4], 4);
    EXPECT_EQ(a[5], 5);
}

TEST(qsortTest, test2) {
    sstd::array a{1,1,4,5,1,4};
    sstd::qsort(a.begin(), a.end(), [](auto &&a, auto &&b){
        return a < b;
    });

    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[1], 1);
    EXPECT_EQ(a[2], 1);
    EXPECT_EQ(a[3], 4);
    EXPECT_EQ(a[4], 4);
    EXPECT_EQ(a[5], 5);
}

TEST(qsortTest, test3){
    std::array a{1,1,4,5,1,4};
    sstd::qsort(a.begin(), a.end(), [](auto &&a, auto &&b){
        return a < b;
    });

    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[1], 1);
    EXPECT_EQ(a[2], 1);
    EXPECT_EQ(a[3], 4);
    EXPECT_EQ(a[4], 4);
    EXPECT_EQ(a[5], 5);

}

TEST(qsortTest, test4){
std::vector<int> a{1,1,4,5,1,4};
    sstd::qsort(a.begin(), a.end(), [](auto &&a, auto &&b){
        return a < b;
    });

    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[1], 1);
    EXPECT_EQ(a[2], 1);
    EXPECT_EQ(a[3], 4);
    EXPECT_EQ(a[4], 4);
    EXPECT_EQ(a[5], 5);
}

TEST(qsortTest, test5){
    std::array a{1,1,4,5,1,4};
    sstd::qsort(a.begin(), a.end());
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[1], 1);
    EXPECT_EQ(a[2], 1);
    EXPECT_EQ(a[3], 4);
    EXPECT_EQ(a[4], 4);
    EXPECT_EQ(a[5], 5);
    
    std::array a2{1,1,4,5,1,4};
    sstd::qsort(a2);
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[1], 1);
    EXPECT_EQ(a[2], 1);
    EXPECT_EQ(a[3], 4);
    EXPECT_EQ(a[4], 4);
    EXPECT_EQ(a[5], 5);
}


