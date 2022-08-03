# sstl: Specialized Standard Library

## Introduction
sstl is a basic stl library.<br>
This lib is not a full-fledged library. It only implements some useful functions and classes 
which very huge and redundancy in stl.

sstl chases small, simple and performance, So if you want multithreading support, 
look for other libraries.<br>
In general, sstl do not guarantee thread safety.

## Requirements
* c++ compiler which supports [c++20](https://en.cppreference.com/w/cpp/20)
* if you want to build googletest and examples, you need:
* - googletest
* - cmake

## Usage
* sstl is a header-only lib, add [include](include) dir to your ```include_directories``` that you can use it.

## implements
* sstd::list
* sstd::array
* sstd::pair
* sstd::ref_ptr
* sstd::RefBase
* sstd::atomic_queue
* sstd::qsort()
* sstd::parallel_qsort()
* sstd::print()
* sstd::println()
* sstd::out()

> IO lib ([io.h](include/sstdio.h)) power by [fast_io](https://gitee.com/qabeowjbtkwb/fast_io)

## features
* header-only
* easy to use
* fast, simple and performance
* no binary bloat
* compatible with stl
* write by c++20

## implements in future


## platforms passed by gtest
* Linux (ubuntu 20.04 for wsl) with gcc 13.0.0
* Windows 10 with MSVC 19.32.31329.0 (Visual Studio 2022)
