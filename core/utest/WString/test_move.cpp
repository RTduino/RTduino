/*
 * Copyright (c) 2020 Arduino.  All rights reserved.
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#define move(x) static_cast<String&&>(x)

/**************************************************************************************
 * TEST CODE
 **************************************************************************************/

TEST_CASE("Testing String move constructor", "[String-move-01]")
{
    String a("src");
    char const* const a_str = a.c_str();
    String b(move(a));
    REQUIRE(a.length() == 0);
    REQUIRE(a.c_str() == nullptr);
    REQUIRE(b.c_str() == a_str);
    REQUIRE(b.length() == 3);
}

TEST_CASE("Testing String move assignment", "[String-move-02]")
{
    String a("src");
    char const* const a_str = a.c_str();
    String b;
    b = move(a);
    REQUIRE(a.length() == 0);
    REQUIRE(a.c_str() == nullptr);
    REQUIRE(b == String("src"));
    REQUIRE(b.c_str() == a_str);
}

TEST_CASE("Testing String move self assignment", "[String-move-03]")
{
#if (defined(GCC_VERSION) && GCC_VERSION >= 13) || (defined(__clang_major__) && __clang_major__ >= 14)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wself-move"
#endif
    String a("src");
    a = move(a);
    REQUIRE(a == "src");
#if defined(GCC_VERSION) && GCC_VERSION >= 13
#pragma GCC diagnostic pop
#endif
}
