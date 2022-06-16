//
// Created by 10414 on 2022/6/14.
//

#ifndef SSTL_IO_H
#define SSTL_IO_H
#ifdef _MSC_VER
#ifdef ENABLE_CLANGD_FIXED
#ifndef __cpp_concepts
#define __cpp_concepts
#endif
#endif
#include <corecrt_io.h>
#endif

#include <utility>
#include <type_traits>

#include "fast_io/include/fast_io.h"

#define CONSTEXPR inline constexpr

namespace sstd{

    CONSTEXPR void Println(){
        ::print("\n");
    }

    CONSTEXPR void Print(){}

    CONSTEXPR void Print(auto &&val ,auto &&... args){
        if constexpr (std::is_same_v<std::remove_reference_t<decltype((val))>, const char *>){
            ::print(fast_io::mnp::os_c_str(val));
        }
        else
            ::print(std::forward<decltype(val)>(val));
        sstd::Print(args...);
    }

    CONSTEXPR void Println(auto &&val, auto &&... args){
        if constexpr (std::is_same_v<std::remove_reference_t<decltype((val))>, const char *>){
            ::print(fast_io::mnp::os_c_str(val));
        }
        else
            ::print(std::forward<decltype(val)>(val));
        sstd::Println(args...);
    }

    class _Out{
        _Out(){}
        _Out(const _Out&) = delete;
        _Out& operator=(const _Out&) = delete;
        _Out(_Out&&) = delete;
        _Out& operator=(_Out&&) = delete;
        ~_Out() = default;
    public:
        static _Out& Out(){
            static _Out instance;
            return instance;
        }
        const _Out& operator<<(auto&& arg) const{
            print(std::forward<decltype(arg)>(arg));
            return *this;
        }
    };

    inline const _Out& out = _Out::Out();

    inline auto get_time(){
        return fast_io::posix_clock_gettime(fast_io::posix_clock_id::realtime);
    }

    inline auto get_utc_time(){
        return utc(get_time());
    }

    inline auto get_local_time(){
        return local(get_time());
    }

    inline const auto TIMEZONE{[](){
        fast_io::posix_tzset();
        return fast_io::timezone_name();
    }()};

}




#endif //SSTL_IO_H
