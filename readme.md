# sstl: Specialized Standard Library

## Introduction
sstl is a basic std library written in c++20.<br>
This lib is not a full-fledged library. It only implements some useful functions and classes 
which very huge and redundancy in stl.

sstl chases small, simple and performance.<br>
sstl should only be used in general environments. So if you want multithreading support 
, high compatibility high reliability or powerful functions, look for other libraries.<br>
In general, sstl assumes you are running in the following environment:
* amd64 architecture
* Windows 10 or above / Linux
* use host compiler

Author does not guarantee that sstl will still work properly in special environments (such as embedded development, big endian cpu, stand-alone compiler)

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
* sstd::thread
* sstd::qsort()
* sstd::parallel_qsort()
* sstd::print()
* sstd::println()
* sstd::out()
* sstd::hash

> IO lib ([io.h](include/sstdio.h)) power by [fast_io](https://gitee.com/qabeowjbtkwb/fast_io)

## features
* header-only
* easy to use
* good code style
* exhaustive comments
* fast, simple and performance
* no binary bloat
* compatible with stl
* write by c++20

## implements in future


## platforms passed by gtest
* Linux (ubuntu 20.04 for wsl) with gcc 13.0.0
* Windows 10 with MSVC 19.32.31329.0 (Visual Studio 2022)
