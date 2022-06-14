//
// Created by 10414 on 2022/6/13.
//

#include "list.h"
#include "gtest/gtest.h"
#include <vector>




TEST(init, initWithoutArg){
    using namespace sstd;
    EXPECT_NO_THROW(listInt l);
    EXPECT_NO_THROW(listDouble l);
    EXPECT_NO_THROW(listChar l);
    EXPECT_NO_THROW(listBool l);
    EXPECT_NO_THROW(list<std::vector<int>> l);
}

TEST(init, initWithInitList){
    using namespace sstd;
    EXPECT_NO_THROW(listInt l({1,2,3,4,5}));
}

TEST(init, copy){
    using namespace sstd;
    listInt l({1,2,3,4,5});
    listInt l2(l);
    EXPECT_EQ(l2.size(), 5);
    EXPECT_EQ(l.size(), 5);
}


TEST(init, move){
    using namespace sstd;
    listInt l1({1,2,3,4,5});
    listInt l2(std::move(l1));
    EXPECT_EQ(l2.size(), 5);
}

TEST(listFunction, size){
    using namespace sstd;
    listInt l({1,2,3,4,5});
    EXPECT_EQ(l.size(), 5);
    listInt l2;
    EXPECT_EQ(l2.size(), 0);
}

TEST(listFunction, operatorStar){ // operator*
    using namespace sstd;
    listInt l({1,2,3,4,5});
    auto it = l.begin();
    EXPECT_EQ(*it, 1);
}

TEST(listFunction, operatorTo){// operator->
    using namespace sstd;
    struct S{
        int a;
        int b;
    };
    list<S> l({{1,2},{3,4},{5,6}});
    auto it = l.begin();
    EXPECT_EQ(it->a, 1);
}



TEST(listFunction, operatorPlusPlus){ // operator++\operator++(int)
    using namespace sstd;
    listInt l({1,2,3,4,5});
    auto it = l.begin();
    EXPECT_EQ(*it++, 1);
    EXPECT_EQ(*it, 2);
    EXPECT_EQ(*++it, 3);
}

TEST(listFunction, operatorMinusMinus) { // operator--\operator--(int)
    using namespace sstd;
    listInt l({1,2,3,4,5});
    auto it = l.end();
    EXPECT_EQ(*--it, 5);
    EXPECT_EQ(*it--, 5);
    EXPECT_EQ(*it, 4);
}

TEST(listFunction, constOperatorPlusPlus){ // const operator++\operator++(int)
    using namespace sstd;
    const listInt l({1,2,3,4,5});
    auto it = l.cbegin();
    EXPECT_EQ(*it++, 1);
    EXPECT_EQ(*it, 2);
    EXPECT_EQ(*++it, 3);
    EXPECT_EQ(*it, 3);
}

TEST(listFunction, constOperatorMinusMinus) { // const operator--\operator--(int)
using namespace sstd;
    const listInt l({1,2,3,4,5});
    auto it = --l.cend();
    EXPECT_EQ(*it, 5);
    EXPECT_EQ(*it--, 5);
    EXPECT_EQ(*it, 4);
}

TEST(listFunction, checkValid) {
    using namespace sstd;
    listInt l({1,2,3,4,5});
    auto it = l.begin();
    EXPECT_TRUE(it.checkValid());
    it = l.end();
    EXPECT_FALSE(it.checkValid());
    auto cit = l.cbegin();
    EXPECT_TRUE(cit.checkValid());
    cit = l.cend();
    EXPECT_FALSE(cit.checkValid());
}

TEST(listFunction, beginAndEnd){
    using namespace sstd;
    listInt l;
    EXPECT_EQ(l.begin(), l.end());
    EXPECT_FALSE(l.begin().checkValid());
    l.push_back(1);
    EXPECT_EQ(*l.begin(), 1);
    *l.begin() = 2;
    EXPECT_EQ(*l.begin(), 2);
    EXPECT_FALSE(l.end().checkValid());
}

TEST(listFunction, cbeginAndCend){
    using namespace sstd;
    listInt l({1,2,3,4,5});
    EXPECT_EQ(*l.cbegin(), 1);
    EXPECT_EQ(*--l.cend(), 5);
}

TEST(listFunction, frontAndBack){
    using namespace sstd;
    listInt l({1,2,3,4,5});
    EXPECT_EQ(l.front(), 1);
    l.front() = 2;
    EXPECT_EQ(l.front(), 2);
    EXPECT_EQ(l.back(), 5);
    listInt l2;
    EXPECT_THROW(l2.front(), std::bad_optional_access);
    EXPECT_THROW(l2.back(), std::bad_optional_access);
}

TEST(listFunction, constFrontAndBack){
    using namespace sstd;
    const listInt l({1,2,3,4,5});
    EXPECT_EQ(l.front(), 1);
    EXPECT_EQ(l.back(), 5);
}



TEST(listFunction, push){
    using namespace sstd;
    listInt l;
    auto it = l.begin();
    l.push(1, it);
    EXPECT_EQ(l.front(), 1);
    l.push(2, l.end());
    EXPECT_EQ(l.back(), 2);
    l.push(3, l.begin());
    EXPECT_EQ(l.front(), 3);
    it = ++l.begin();
    l.push(4, it);
    EXPECT_EQ(*++l.begin(), 4);
}

TEST(listFunction, pop){
    using namespace sstd;
    listInt l({1,2,3,4,5});
    auto it = l.begin();
    l.pop(it);
    EXPECT_EQ(l.front(), 2);

    l.pop(l.end());
}

TEST(listFunction, pushBackAndFront){
    using namespace sstd;
    listInt l;
    l.push_back(1);
    EXPECT_EQ(l.front(), 1);
    l.push_back(2);
    EXPECT_EQ(l.back(), 2);
    l.push_front(3);
    EXPECT_EQ(l.front(), 3);
    listInt l2;
    l2.push_front(4);
    EXPECT_EQ(l2.front(), 4);
}

TEST(listFunction, oparatorEq){
    using namespace sstd;
    listInt l1({1,2,3,4,5});
    listInt l2({1});
    l2 = l1;
    EXPECT_EQ(l2.size(), 5);
}

TEST(listFunction, swap){
    using namespace sstd;
    listInt l1({1,2,3,4,5});
    listInt l2({6,7,8,9,10});
    l1.swap(l2);
    EXPECT_EQ(l1.size(), 5);
    EXPECT_EQ(l1.front(), 6);
    EXPECT_EQ(l2.size(), 5);
    EXPECT_EQ(l2.front(), 1);
}

TEST(listFunction, operatorMoveEq){
    using namespace sstd;
    listInt l1({1,2,3,4,5});
    listInt l2({1});
    l2 = std::move(l1);
    EXPECT_EQ(l2.size(), 5);
}






TEST(listIterator, move){
    using namespace sstd;
    listInt l;
    EXPECT_NO_THROW(l.push_back(1));
    EXPECT_NO_THROW(l.push_back(2));
    EXPECT_NO_THROW(l.push_back(3));
    auto it = l.begin();
    EXPECT_EQ(*it, 1);
    EXPECT_EQ(*++it, 2);
    EXPECT_EQ(*++it, 3);
    EXPECT_EQ(++it, l.end());
    auto it2 = --l.end();
    *--it = 4;
    EXPECT_EQ(it, it2);
}




