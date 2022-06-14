//
// Created by 10414 on 2022/6/14.
//

#ifndef SSTL_IO_H
#define SSTL_IO_H
#ifdef _MSC_VER
#include <corecrt_io.h>
#endif
#include "fast_io/include/fast_io.h"
#include <utility>

namespace sstd{
    void Print(auto &&... args){
        print(std::forward<decltype(args)>(args)...);
    }
    void Println(auto &&... args){
        println(std::forward<decltype(args)>(args)...);
    }

    void print(auto &&... args){
        Print(std::forward<decltype(args)>(args)...);
    }

    void println(auto &&... args){
        Println(std::forward<decltype(args)>(args)...);
    }

    class _Out{
        _Out(){}
        _Out(const _Out&) = delete;
        _Out& operator=(const _Out&) = delete;
        _Out(_Out&&) = delete;
        _Out& operator=(_Out&&) = delete;
        ~_Out() = default;
    public:
        static _Out& out(){
            static _Out instance;
            return instance;
        }
        _Out& operator<<(auto&& arg){
            print(std::forward<decltype(arg)>(arg));
            return *this;
        }
    };
}




#endif //SSTL_IO_H
