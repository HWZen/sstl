//
// Created by HWZen on 2022/6/15.
// Copyright (c) 2022 HWZen All rights reserved.
// MIT License
// 

#ifndef MYSTL_FUNCTIONAL_H
#define MYSTL_FUNCTIONAL_H

#include <type_traits>

// static_warning
#if defined(__GNUC__)
#define static_warning(foo, msg) foo __attribute__((deprecated(msg)))
#elif defined(_MSC_VER)
#define static_warning(foo, msg) __declspec(deprecated(msg)) foo
#else
#pragma message("static_warning is not implemented for this compiler, define it as static_assert")
#define static_warning static_assert
#endif

// concept: judge T1 is same T2 or T2's subclass
template<typename T1, typename T2>
concept  same_or_subclass = std::is_base_of<std::remove_cvref_t<T2>, std::remove_cvref_t<T1>>::value;

template<typename T1, typename T2>
concept weak_same_type = std::is_same_v<std::remove_cvref_t<T2>, std::remove_cvref_t<T1>>;

template<typename Ty>
concept sortable = requires(Ty t) {
    *t;
    t++;
    t--;
    --t;
    ++t;
    t < t;
    t > t;
};

template<typename Ty>
concept is_vector_like = requires(Ty t) {
    t.begin();
    t.end();
    *t.begin();

};



#endif //MYSTL_FUNCTIONAL_H
