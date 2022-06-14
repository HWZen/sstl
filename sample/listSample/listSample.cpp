//
// Created by 10414 on 2022/6/13.
//
#include "io.h"
#include "list.h"
int main(){
    using namespace sstd;
    listInt l1({1,2,3,4,5});
    listInt l2(std::move(l1));
    print(l2.size());
}