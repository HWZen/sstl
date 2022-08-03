//
// Created by HWZen on 2022/6/17.
// Copyright (c) 2022 HWZen All rights reserved.
// MIT License
//

#include "ref_ptr.h"
#include "sstdio.h"
#include <string>

class Base {
public:
    Base(std::string str) : m_str(std::move(str)) {
        sstd::Print("Base::Base(",m_str,")\n");
    }
    virtual ~Base() {
        sstd::Print("Base::~Base(",m_str,")\n");
    }

    std::string m_str;

};
class Derived : public Base {
public:
    Derived(std::string str) : Base(std::move(str)) {
        sstd::Print("Derived::Derived(",m_str,")\n");
    }
    ~Derived() override {
        sstd::Print("Derived::~Derived(",m_str,")\n");
    }
};

int main() {
    using namespace sstd;

    ref_ptr<Base> a{new Base{"a"}};
    ref_ptr b{new Derived{"b"}};

    Print("\n");
    Println("============================================================");
    // ref_ptr<Base>, but ref to Derived
    ref_ptr<Base> c{new Derived{"c"}};

    Print("\n");
    Println("============================================================");
    // ref_ptr<Base> re-ref to Derived
    a = b;
    Print("\n");
    Println("============================================================");
    // ref_ptr<Base> re-ref to Base (in fact, ref to Derived)
    c = a;
    Print("\n");
    Println("============================================================");
    // b = a; // error: cannot assign to 'b' because it is a ref_ptr<Derived>, ref_ptr can not dynamic_cast.

}

