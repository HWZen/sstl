//
// Created by HWZ on 2022/9/13.
//
#include "sstl/sstdio.h"
#include "sstl/hash.h"

int main(){
    size_t hashCode{ 0};
    auto t0{sstd::get_time()};
    sstd::hash_int hash_int;
    for(int i{}; i < 100'000'000; ++i)
        hashCode ^= hash_int(i);
    sstd::Println(hashCode, "hash_int time: ", sstd::get_time() - t0);

    t0 = sstd::get_time();
    sstd::hash_double hash_double;
    for (double d{0}; d < 10'000'000; d += 0.1)
        hashCode ^= hash_double(d);
    sstd::Println(hashCode, "hash_double time: ", sstd::get_time() - t0);

    t0 = sstd::get_time();
    std::hash<int> std_hash_int;
    for(int i{}; i < 100'000'000; ++i)
        hashCode ^= std_hash_int(i);
    sstd::Println(hashCode, "std::hash_int time: ", sstd::get_time() - t0);

    t0 = sstd::get_time();
    std::hash<double> std_hash_double;
    for (double d{0}; d < 10'000'000; d += 0.1)
        hashCode ^= std_hash_double(d);
    sstd::Println(hashCode, "std::hash_double time: ", sstd::get_time() - t0);

    uint64_t data{0};
    hashCode = 0;
    t0 = sstd::get_time();
    for(; data < 1'000'000'000; ++data)
        hashCode ^= sstd::FNVHash((sstd::byte *)&data, sizeof(uint64_t));
    sstd::Println(hashCode, "FNVHash time: ", sstd::get_time() - t0);

    data = 0;
    hashCode = 0;
    t0 = sstd::get_time();
    for(; data < 1'000'000'000; ++data)
        hashCode ^= sstd::MurMurHash((sstd::byte *)&data, sizeof(uint64_t));
    sstd::Println(hashCode, "MurMurHash time: ", sstd::get_time() - t0);

    data = 0;
    hashCode = 0;
    t0 = sstd::get_time();
    for(; data < 1'000'000'000; ++data)
        hashCode ^= sstd::stdHash((sstd::byte *)&data, sizeof(uint64_t));
    sstd::Println(hashCode, "stdHash time: ", sstd::get_time() - t0);

#pragma pack(1)
    struct MyStruct{
        int i{0};
        char ch{0};
    } myStruct;
#pragma pack()
    hashCode = 0;
    sstd::hash<MyStruct> hashMyStruct;
    t0 = sstd::get_time();
    for (;myStruct.i < 100'000'000; ++myStruct.i)
        hashCode ^= hashMyStruct(myStruct);
    sstd::Println(hashCode, "hashMyStruct time: ", sstd::get_time() - t0);

    hashCode = 0;
    myStruct.i = 0;
    for(;myStruct.i < 100'000'000; ++myStruct.i)
        hashCode ^= sstd::stdHash((sstd::byte *)&myStruct, sizeof(MyStruct));
    sstd::Println(hashCode, "MyStruct stdHash time: ", sstd::get_time() - t0);

}
