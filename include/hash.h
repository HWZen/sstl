//
// Created by HWZ on 2022/9/7.
//

#ifndef SSTL_HASH_H
#define SSTL_HASH_H

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <array>
#include <bit>

namespace sstd {
    using std::byte;


    /****************************
     * C-Style API
     ***************************/

    /**
     * @brief invoke std hash
     * @param data data
     * @param size data size
     * @return size_t hash code
     */
    size_t stdHash(const void* data, size_t size) noexcept;

    /**
     * @brief FNV hash implementation
     * @tparam Ty data type, required: size of Ty is 1
     * @param data data
     * @param size data size
     * @return hash code
     */
    template<typename Ty> requires( sizeof(Ty) == 1)
    constexpr size_t FNVHash(const Ty* data, size_t size) noexcept;

    /**
     * @brief MurMur hash implementation
     * @tparam Ty data type. required: size of Ty is 1
     * @param data data
     * @param size data size
     * @return hash code
     */
    template<typename Ty> requires( sizeof(Ty) == 1)
    constexpr size_t MurMurHash(const Ty* data, size_t size) noexcept;



    /**
     * @brief SHA-256 hash implementation
     * @param data data
     * @param size data size
     * @return digest
     */
    template<typename Ty> requires( sizeof(Ty) == 1)
    constexpr std::array<uint32_t, 8> SHA256Hash(const Ty* data, size_t size) noexcept;

    /**
     * @brief SHA-256 hash to string
     * @param hash
     * @return hash string
     */
    constexpr std::string SHA256Hash2String(const std::array<uint32_t, 8> &hash);



    /****************************
     * C++ OOP
     ***************************/

    /**
     * @brief hash type
     * @tparam Ty
     * @tparam TypeSize
     */
    template<typename Ty, size_t TypeSize = sizeof(Ty)>
    struct hash {
        constexpr size_t operator()(const Ty &data) const noexcept;
    };

    template<>
    struct hash<float> {
        constexpr size_t operator()(const float &data) const noexcept;
    };

    template<>
    struct hash<double> {
        constexpr size_t operator()(const double &data) const noexcept;
    };

    template<>
    struct hash<long double>{
        size_t operator()(const long double &data) const noexcept;
    };

    template<>
    struct hash<std::string, 0> {
        constexpr size_t operator()(const std::string &str) const noexcept;
    };


    using hash_int = hash<int>;
    using hash_uint = hash<unsigned int>;
    using hash_long = hash<long>;
    using hash_ulong = hash<unsigned long>;
    using hash_llong = hash<long long>;
    using hash_ullong = hash<unsigned long long>;
    using hash_float = hash<float>;
    using hash_double = hash<double>;
    using hash_ldouble = hash<long double>;
    using hash_char = hash<char>;
    using hash_uchar = hash<unsigned char>;
    using hash_schar = hash<signed char>;
    using hash_wchar = hash<wchar_t>;
    using hash_char16 = hash<char16_t>;
    using hash_char32 = hash<char32_t>;
    using hash_bool = hash<bool>;
    using hash_size = hash<size_t>;
    using hash_ptr = hash<void *>;
    using hash_cptr = hash<const void *>;

    using hash_string = hash<std::string, 0>;

    /**********************
     * Implementation
     *********************/

    size_t stdHash(const void *data, size_t size) noexcept {
#ifdef _MSC_VER
        return std::_Fnv1a_append_bytes(std::_FNV_offset_basis, static_cast<const uint8_t*>(data), size);
#elif defined(__GNUC__)
        return  std::_Hash_bytes(data, size, static_cast<size_t>(0xc70f6907UL));
#endif

    }



    template<typename Ty> requires( sizeof(Ty) == 1)
    inline constexpr std::array<uint32_t, 8> SHA256Hash(const Ty* data, size_t size) noexcept {
        auto padding = [](const Ty *data, size_t size, Ty (&newBuf)[128]) constexpr noexcept {
            for(size_t i{0}; i < size % 64; ++i)
                newBuf[i] = data[size - (size % 64) + i];
            newBuf[size % 64] = static_cast<Ty>(0x80);

            if (size % 64 > 55) {
                for (size_t i{size % 64 + 1}; i < 128 - 8; ++i)
                    newBuf[i] = static_cast<Ty>(0);
                uint64_t bitSize = size * 8;
                for (int i = 1; i <= 8; ++i)
                    newBuf[119 + i] =
                            std::bit_cast<Ty>(static_cast<byte>((bitSize >> (64 - i * 8)) & 0xff));
                return true;

            } else {
                for (size_t i{size % 64 + 1}; i < 128 - 8; ++i)
                    newBuf[i] = static_cast<Ty>(0);
                uint64_t bitSize = size * 8;
                for (int i = 1; i <= 8; ++i)
                    newBuf[55 + i] =
                            std::bit_cast<Ty>(static_cast<byte>((bitSize >> (64 - i * 8)) & 0xff));
                return false;
            }
        };

        auto ch = [](uint32_t x, uint32_t y, uint32_t z) constexpr noexcept {
            return (x & y) ^ ((~x) & z);
        };

        auto maj = [](uint32_t x, uint32_t y, uint32_t z) constexpr noexcept {
            return (x & y) ^ (x & z) ^ (y & z);
        };

        auto big_sigma0 = [](uint32_t x) constexpr noexcept {
            return (x >> 2 | x << 30) ^ (x >> 13 | x << 19) ^ (x >> 22 | x << 10);
        };

        auto big_sigma1 = [](uint32_t x) constexpr noexcept {
            return (x >> 6 | x << 26) ^ (x >> 11 | x << 21) ^ (x >> 25 | x << 7);
        };

        auto small_sigma0 = [](uint32_t x) constexpr noexcept {
            return (x >> 7 | x << 25) ^ (x >> 18 | x << 14) ^ (x >> 3);
        };

        auto small_sigma1 = [](uint32_t x) constexpr noexcept {
            return (x >> 17 | x << 15) ^ (x >> 19 | x << 13) ^ (x >> 10);
        };

        auto structWords = [&](const Ty *chunk, uint32_t(&W)[64]) constexpr noexcept {
            for (int i = 0; i < 16; ++i) {
                W[i] = (static_cast<uint32_t>(static_cast<byte>(chunk[i * 4])) << 24) |
                       (static_cast<uint32_t>(static_cast<byte>(chunk[i * 4 + 1])) << 16) |
                       (static_cast<uint32_t>(static_cast<byte>(chunk[i * 4 + 2])) << 8) |
                       static_cast<uint32_t>(static_cast<byte>(chunk[i * 4 + 3]));
            }
            for (int i = 16; i < 64; ++i) {
                W[i] = small_sigma1(W[i - 2]) + W[i - 7] + small_sigma0(W[i - 15]) + W[i - 16];
            }
        };

        auto transform = [&](const uint32_t (&W)[64], std::array<uint32_t, 8> &h) constexpr noexcept {

            constexpr uint32_t K[] = {
                    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b,
                    0x59f111f1, 0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01,
                    0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7,
                    0xc19bf174, 0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
                    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da, 0x983e5152,
                    0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
                    0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc,
                    0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
                    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819,
                    0xd6990624, 0xf40e3585, 0x106aa070, 0x19a4c116, 0x1e376c08,
                    0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f,
                    0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
                    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2 };

            auto H = h;

            for (int i{}; i < 64; ++i)
            {
                uint32_t t1 =
                        h[7] + big_sigma1(h[4]) + ch(h[4], h[5], h[6]) + K[i] + W[i];
                uint32_t t2 = big_sigma0(h[0]) + maj(h[0], h[1], h[2]);
                h[7] = h[6];
                h[6] = h[5];
                h[5] = h[4];
                h[4] = h[3] + t1;
                h[3] = h[2];
                h[2] = h[1];
                h[1] = h[0];
                h[0] = t1 + t2;
            }

            for (int i{}; i < 8; ++i)
            {
                h[i] += H[i];
            }
        };

        Ty paddingBuf[128];
        auto twoChunk = padding(data, size, paddingBuf);

        uint32_t W[64];
        std::array<uint32_t, 8> digest = {0x6a09e667, 0xbb67ae85, 0x3c6ef372,
                                        0xa54ff53a, 0x510e527f, 0x9b05688c,
                                        0x1f83d9ab, 0x5be0cd19};

        for(size_t it{0};it < size / 64; ++it){
            auto pChunk = data + it * 64;
            structWords(pChunk, W);
            transform(W, digest);
        }
        structWords(paddingBuf, W);
        transform(W, digest);
        if (twoChunk) {
            structWords(paddingBuf + 64, W);
            transform(W, digest);
        }
        return digest;
    }

    template<typename Ty> requires( sizeof(Ty) == 1)
    inline constexpr size_t FNVHash(const Ty* data, size_t size) noexcept
    {
        size_t result = 14695981039346656037ULL;
        while(size--){
            result ^= static_cast<size_t>(*data++);
            result *= static_cast<size_t>(1099511628211ULL);
        }
        return result;
    }

    template<typename Ty> requires( sizeof(Ty) == 1)
    inline constexpr size_t MurMurHash(const Ty* data, size_t size) noexcept
    {
        auto unaligned_load = [](const Ty* data) constexpr noexcept {
            size_t result{0};
            for(size_t i = 0; i < sizeof(size_t); ++i)
                result |= static_cast<size_t>(data[i]) << (i * 8);
            return result;
        };

        auto shift_mix = [](size_t v) constexpr noexcept {
            return v ^ (v >> 47);
        };

        auto load_bytes = [](const Ty* p, int n) constexpr noexcept {
            std::size_t result = 0;
            --n;
            do
                result = (result << 8) + static_cast<size_t>(p[n]);
            while (--n >= 0);
            return result;
        };

        constexpr size_t mul = (((size_t) 0xc6a4a793UL) << 32UL)
        + (size_t) 0x5bd1e995UL;
        const Ty* const buf = data;

        // Remove the bytes not divisible by the sizeof(size_t).  This
        // allows the main loop to process the data as 64-bit integers.
        const size_t len_aligned = size & ~(size_t)0x7;
        const Ty* const end = buf + len_aligned;
        size_t hash = 0xc70f6907UL ^ (size * mul);
        for (const Ty* p = buf; p != end; p += 8)
        {
            const size_t _data = shift_mix(unaligned_load(p) * mul) * mul;
            hash ^= _data;
            hash *= mul;
        }
        if ((size & 0x7) != 0)
        {
            const size_t _data = load_bytes(end, size & 0x7);
            hash ^= _data;
            hash *= mul;
        }
        hash = shift_mix(hash) * mul;
        hash = shift_mix(hash);
        return hash;
    }

    inline constexpr std::string SHA256Hash2String(const std::array<uint32_t, 8> &hash) {
        std::string result;
        for (auto i: hash) {
            char buf[9];
            sprintf(buf, "%08x", i);
            buf[8] = 0;
            result += buf;
        }
        return result;
    }

    template<typename Ty, size_t TypeSize>
    constexpr size_t hash<Ty, TypeSize>::operator()(const Ty &data) const noexcept {
        if constexpr (std::is_arithmetic_v<Ty>)
                return static_cast<size_t>(data);

        if constexpr (std::is_array_v<Ty> && sizeof(std::remove_all_extents_t<Ty>) == 1){
#ifdef _MSC_VER
            return FNVHash(data, TypeSize);
#elif defined(__GNUC__)
            return MurMurHash(data, TypeSize);
#endif
        }
            // The following branches cannot be evaluated at compile time
        else {
            if constexpr (TypeSize <= sizeof(size_t))
            {
                size_t res{0};
                memcpy(&res, &data, sizeof(data));
                return res;
            }
            return stdHash(reinterpret_cast<const byte*>(&data), TypeSize);
        }
    }

    constexpr size_t hash<std::string, 0>::operator()(const std::string &str) const noexcept {
        if (str.size() <= sizeof(size_t))
        {
            size_t res{0};
            for (const auto &ch: str)
                res = (res << 8) | static_cast<size_t>(ch);
            return res;
        }
#ifdef _MSC_VER
        return FNVHash(str.data(), str.size());
#elif defined(__GNUC__)
        return MurMurHash(str.data(), str.size());
#endif
    }

    constexpr size_t hash<float>::operator()(const float &data) const noexcept {
        return std::bit_cast<size_t>((double)data);
    }

    constexpr size_t hash<double>::operator()(const double &data) const noexcept {
        return std::bit_cast<size_t>(data);
    }

    size_t hash<long double>::operator()(const long double &data) const noexcept {
#ifdef __GNUC__
        return MurMurHash((const byte*)&data, sizeof(data));
#else
        return stdHash(&data, sizeof(data));
#endif
    }
}

#endif //SSTL_HASH_H
