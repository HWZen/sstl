//
// Created by HWZen on 2022/6/17.
// Copyright (c) 2022 HWZen All rights reserved.
// MIT License
// 

#ifndef SSTL_PAIR_H
#define SSTL_PAIR_H

namespace sstd {

    template<typename T1, typename T2>
    class pair {
    public:
        using first_type = T1;
        using second_type = T2;

        T1 first;
        T2 second;
    };
}


#endif //SSTL_PAIR_H
