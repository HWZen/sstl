//
// Created by HWZen on 2022/6/18.
// Copyright (c) 2022 HWZen All rights reserved.
// MIT License
//

#include "RefBase.h"
#include "sstdio.h"

class Integer : private sstd::RefBase {
private:
    int *m_data{nullptr};
public:
    Integer(int data) : m_data(new int(data)) {}

    ~Integer() override {
        decCount();
    }

    int& operator*() {
        return *m_data;
    }

private:
    void zeroCount() override {
        sstd::Println("release resource: ", *m_data);
        delete m_data;
    }

};

int main(){
    Integer i1(114515);
    Integer i2(i1);
    --*i2;
    sstd::Println(*i1);

}