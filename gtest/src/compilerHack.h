//
// Created by HWZen on 2022/6/18.
// Copyright (c) 2022 HWZen All rights reserved.
// MIT License
// 

#ifndef SSTL_COMPILERHACK_H
#define SSTL_COMPILERHACK_H

#include <gtest/gtest.h>
#include <string>
#include <fast_io/include/fast_io.h>
#include <fast_io/include/fast_io_device.h>
#include <cstdlib>

#define EXPECT_CAN_COMPILE(str) \
fast_io::obf_file obuf_file("temp.cpp"); \
print(obuf_file, str); \
obuf_file.close();              \
system("g++ -std=c++20 -o temp.exe temp.cpp"); \

#endif //SSTL_COMPILERHACK_H
