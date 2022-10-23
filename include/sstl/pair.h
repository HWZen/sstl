//
// Created by HWZen on 2022/6/17.
// Copyright (c) 2022 HWZen All rights reserved.
// MIT License
// 

#ifndef SSTL_PAIR_H
#define SSTL_PAIR_H

namespace sstd {

#pragma pack(1)
    template<typename T1, typename T2>
    class pair {
    public:
        using first_type = T1;
        using second_type = T2;

        bool operator==(const pair<T1, T2> &other) const {
            return first == other.first && second == other.second;
        }

        first_type first;
        second_type second;
    };
#pragma pack()
}


#endif //SSTL_PAIR_H
