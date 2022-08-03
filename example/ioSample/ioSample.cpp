//
// Created by HWZen on 2022/6/16.
// Copyright (c) 2022 HWZen All rights reserved.
// MIT License
//

#include <sstdio.h>

int main(){
    using namespace sstd;
    Print("Hello, World!\n");
    Println("Hello, World!");
    Println("Hello, Now local time:  ", get_local_time());
    Println("Hello, Now utc time:  ", get_utc_time());
    Println("Hello, Now ", get_time(), "secs has passed since 1990-01-01 00:00:00");
    Println("Hello, Your timezone: ", TIMEZONE);
    println(1," ", 2, " ", 3, " ", 4, " ", 5, " ", 3.14159, " Hello world ", 0xfffffffff);
    out << "Hello, World!" << 1 << 2 << 3 << 4 << 5 << 3.14159 << 0xfffffffff << "\n";
}

