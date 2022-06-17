//
// Created by 10414 on 2022/6/13.
//
#include "io.h"
#include "list.h"
using namespace sstd;
int main(){

    listInt l1({1,2,3,4,5});
    Println("l1.size() = ", l1.size());

    for(auto &it:l1)
        Print(it, " ");

    listInt l2(std::move(l1));
    Println(l2.size());
    Println("l2.front() = ", l2.front());
    Println("l2.back() = ", l2.back());
    l2.push_back(100);
    Println("l2.back() = ", l2.back());
    auto it = l2.begin();
    l2.pop(it);
    Println("l2.size() = ", l2.size());
    Println("l2.front() = ", l2.front());

}