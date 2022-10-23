//
// Created by HWZ on 2022/9/7.
//
#include "sstl/hash.h"
#include <array>
#include <gtest/gtest.h>
#include <variant>

TEST(getHashTest, integer)
{
    int i{114514};
    auto hashCode = sstd::SHA256Hash((sstd::byte*)&i, sizeof(int));
    auto hashCodeStr = sstd::SHA256Hash2String(hashCode);
    EXPECT_EQ(hashCodeStr, "278e346209efaac962490150066e4d58e67a5fb2e475f6337f6d55e5a9c4e014");
}

TEST(getHashTest, doubleFloat)
{
    double d{3.1415926};
    auto hashCode = sstd::SHA256Hash((sstd::byte*)&d, sizeof(double));
    auto hashCodeStr = sstd::SHA256Hash2String(hashCode);
    EXPECT_EQ(hashCodeStr, "27b52604d021828deb07b974ed321e17db0f9a7818db06ef7f4c4c9e4bb959b1");
}

TEST(getHashTest, string)
{
    std::string str{"Hello, world!"};
    auto hashCode = sstd::SHA256Hash((sstd::byte*)str.data(), str.size());
    auto hashCodeStr = sstd::SHA256Hash2String(hashCode);
    EXPECT_EQ(hashCodeStr, "315f5bdb76d078c43b8ac0064e4a0164612b1fce77c869345bfc94c75894edd3");
}

TEST(hashTypeTest, trivalType)
{
    int i{114514};
    sstd::hash_int hashInt;
    auto hashCode = hashInt(i);
    EXPECT_EQ(hashCode, i);

    double d{3.1415926};
    sstd::hash_double hashDouble;
    hashCode = hashDouble(d);
    EXPECT_TRUE(memcmp(&hashCode, &d, sizeof(double)) == 0);


    bool b{true};
    sstd::hash_bool hashBool;
    hashCode = hashBool(b);
    EXPECT_EQ(hashCode, static_cast<size_t>(b));

    void *p{nullptr};
    sstd::hash_ptr hashPtr;
    hashCode = hashPtr(p);
    EXPECT_EQ(hashCode, 0);
}

TEST(hashTypeTest, string)
{
    std::string str{"Hello, world!"};
    sstd::hash_string hashString;
    auto hashCode = hashString(str);
    EXPECT_EQ(hashCode, std::hash<std::string>{}(str));
}

TEST(hashTypeTest, array)
{
    std::array<char, 13> arr{'H', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd', '!'};
    sstd::hash<decltype(arr)> hash;
    auto hashCode = hash(arr);
    EXPECT_EQ(hashCode, sstd::stdHash(arr.data(), arr.size()));

    char cStr[] = "Hello, world!";
    sstd::hash<decltype(cStr), sizeof(cStr) - 1 > hash2;
    hashCode = hash2(cStr);
    EXPECT_EQ(hashCode, sstd::stdHash(cStr, sizeof(cStr) - 1));
}



TEST(hashTypeTest, customStruct)
{
#pragma pack(1)
    struct TestStruct
    {
        int i{};
        double d{};
        bool b{};
        size_t sz{};
    };
#pragma pack()

    TestStruct ts{114514, 3.1415926, true, 0};
    TestStruct ts2{114514, 3.1415926, true, 0};
    TestStruct ts3{114514, 3.1415926, true, 0};
    auto ts4{new TestStruct{114514, 3.1415926, true, 0}};

    sstd::hash<TestStruct> hash;
    auto hashCode = hash(ts);
    EXPECT_EQ(hashCode, hash(ts2));
    EXPECT_EQ(hashCode, hash(ts3));
    EXPECT_EQ(hashCode, hash(*ts4));

    TestStruct ts5{114514, 3.1415926, true, 1};
    EXPECT_NE(hashCode, hash(ts5));

    delete ts4;
}

TEST(getHashTest, constexprTest)
{
    auto hashCode = []()consteval {
        constexpr sstd::byte bt[] = { static_cast<sstd::byte>('H'), static_cast<sstd::byte>('e'), static_cast<sstd::byte>('l'),
                                      static_cast<sstd::byte>('l'), static_cast<sstd::byte>('o'), static_cast<sstd::byte>(','),
                                      static_cast<sstd::byte>(' '), static_cast<sstd::byte>('w'), static_cast<sstd::byte>('o'),
                                      static_cast<sstd::byte>('r'), static_cast<sstd::byte>('l'), static_cast<sstd::byte>('d'),
                                      static_cast<sstd::byte>('!') };
        return sstd::SHA256Hash(bt, 13);
    }();
    auto hashCodeStr = sstd::SHA256Hash2String(hashCode);
    EXPECT_EQ(hashCodeStr, "315f5bdb76d078c43b8ac0064e4a0164612b1fce77c869345bfc94c75894edd3");

    hashCode = []()consteval {
        constexpr char str[] = "Hello, world!";
        return sstd::SHA256Hash(str, sizeof(str) - 1);

    }();
    hashCodeStr = sstd::SHA256Hash2String(hashCode);
    EXPECT_EQ(hashCodeStr, "315f5bdb76d078c43b8ac0064e4a0164612b1fce77c869345bfc94c75894edd3");
}

TEST(hashTypeTest, constexprTest)
{
    auto hashCode = []()consteval {
        constexpr sstd::byte bt[] = { static_cast<sstd::byte>('H'), static_cast<sstd::byte>('e'), static_cast<sstd::byte>('l'),
                                      static_cast<sstd::byte>('l'), static_cast<sstd::byte>('o'), static_cast<sstd::byte>(','),
                                      static_cast<sstd::byte>(' '), static_cast<sstd::byte>('w'), static_cast<sstd::byte>('o'),
                                      static_cast<sstd::byte>('r'), static_cast<sstd::byte>('l'), static_cast<sstd::byte>('d'),
                                      static_cast<sstd::byte>('!') };
        sstd::hash<decltype(bt)> hash;
        return hash(bt);
    }();
    EXPECT_EQ(hashCode, std::hash<std::string>{}("Hello, world!"));

    auto hashCode2 = []()consteval {
        constexpr unsigned int i{114514};
        sstd::hash_uint hashUInt;
        return hashUInt(i);
    }();
    EXPECT_EQ(hashCode2, 114514);

    auto hashCode3 = []()consteval {
        constexpr double d{3.1415926};
        sstd::hash_double hashDouble;
        return hashDouble(d);
    }();
    double d{3.1415926};
    EXPECT_TRUE(memcmp(&hashCode3, &d, sizeof(double)) == 0);

    auto hashCode4 = []()consteval {
        constexpr bool b{true};
        sstd::hash_bool hashBool;
        return hashBool(b);
    }();
    EXPECT_EQ(hashCode4, static_cast<size_t>(true));

    auto hashCode5 = []()consteval {
        constexpr float f{3.1415926f};
        sstd::hash_float hashFloat;
        return hashFloat(f);
    }();
    float f{3.1415926f};
    EXPECT_EQ(std::bit_cast<double>(hashCode5), (double )f);

}


TEST(getHashTest, SHA256HashTest)
{
    std::string str(60, 'a');
    auto hashCode = sstd::SHA256Hash((sstd::byte*)str.data(), str.size());
    auto hashCodeStr = sstd::SHA256Hash2String(hashCode);
    EXPECT_EQ(hashCodeStr, "11ee391211c6256460b6ed375957fadd8061cafbb31daf967db875aebd5aaad4");

    str = "Hello, world!";
    hashCode = sstd::SHA256Hash((sstd::byte *)str.data(), str.size());
    hashCodeStr = sstd::SHA256Hash2String(hashCode);
    EXPECT_EQ(hashCodeStr, "315f5bdb76d078c43b8ac0064e4a0164612b1fce77c869345bfc94c75894edd3");

    hashCode = sstd::SHA256Hash(str.data(), str.size());
    hashCodeStr = sstd::SHA256Hash2String(hashCode);
    EXPECT_EQ(hashCodeStr, "315f5bdb76d078c43b8ac0064e4a0164612b1fce77c869345bfc94c75894edd3");
}

TEST(getHashTest, stdHashTest)
{
    double d{3.14159};
    EXPECT_EQ(std::hash<double>{}(d), sstd::stdHash(&d, sizeof(d)));

    std::string str(60, 'a');
    EXPECT_EQ(std::hash<std::string>{}(str), sstd::stdHash(str.data(), str.size()));

}

TEST(getHashTest, FNVHashOrMurMurHashTest)
{
    double d{3.14159};
#ifdef _MSC_VER
    EXPECT_EQ(sstd::FNVHash((sstd::byte*)&d, sizeof(d)), sstd::stdHash(&d, sizeof(d)));
#elif defined(__GNUC__)
    EXPECT_EQ(sstd::MurMurHash((sstd::byte*)&d, sizeof(d)), sstd::stdHash(&d, sizeof(d)));
#endif

}
