/*
 * Copyright (c) 2021-2025, RTduino Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * https://github.com/RTduino/RTduino
 * https://gitee.com/rtduino/RTduino
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-01-04     Meco Man     first version
 */

#include <RTduino.h>
#include <WString.h>
#include <utest.h>

#include "catch_compatible.h"

static void TC_characterAccessFunc(void)
{
    #include "WString/test_characterAccessFunc.cpp"
}

static void TC_compareTo(void)
{
    #include "WString/test_compareTo.cpp"
}

static void TC_comparisonFunc(void)
{
    #include "WString/test_comparisonFunc.cpp"
}

static void TC_concat(void)
{
    #include "WString/test_concat.cpp"
}

static void TC_indexOf(void)
{
    #include "WString/test_indexOf.cpp"
}

static void TC_isEmpty(void)
{
    #include "WString/test_isEmpty.cpp"
}

static void TC_lastIndexOf(void)
{
    #include "WString/test_lastIndexOf.cpp"
}

static void TC_length(void)
{
    #include "WString/test_length.cpp"
}

static void TC_move(void)
{
    #include "WString/test_move.cpp"
}

static void TC_operators(void)
{
    #include "WString/test_operators.cpp"
}

static void TC_remove(void)
{
    #include "WString/test_remove.cpp"
}

static void TC_replace(void)
{
    #include "WString/test_replace.cpp"
}

static void TC_String(void)
{
    #include "WString/test_String.cpp"
}

static void TC_substring(void)
{
    #include "WString/test_substring.cpp"
}

static void TC_toDouble(void)
{
    #include "WString/test_toDouble.cpp"
}

static void TC_toFloat(void)
{
    #include "WString/test_toFloat.cpp"
}

static void TC_toInt(void)
{
    #include "WString/test_toInt.cpp"
}

static void TC_toLowerCase(void)
{
    #include "WString/test_toLowerCase.cpp"
}

static void TC_toUpperCase(void)
{
    #include "WString/test_toUpperCase.cpp"
}

static void TC_trim(void)
{
    #include "WString/test_trim.cpp"
}

/* Utest function to run all test cases */
static void utest_do_tc(void)
{
    UTEST_UNIT_RUN(TC_characterAccessFunc);
    UTEST_UNIT_RUN(TC_compareTo);
    UTEST_UNIT_RUN(TC_comparisonFunc);
    UTEST_UNIT_RUN(TC_concat);
    UTEST_UNIT_RUN(TC_indexOf);
    UTEST_UNIT_RUN(TC_isEmpty);
    UTEST_UNIT_RUN(TC_lastIndexOf);
    UTEST_UNIT_RUN(TC_length);
    UTEST_UNIT_RUN(TC_move);
    UTEST_UNIT_RUN(TC_operators);
    UTEST_UNIT_RUN(TC_remove);
    UTEST_UNIT_RUN(TC_replace);
    UTEST_UNIT_RUN(TC_String);
    UTEST_UNIT_RUN(TC_substring);
    UTEST_UNIT_RUN(TC_toDouble);
    UTEST_UNIT_RUN(TC_toFloat);
    UTEST_UNIT_RUN(TC_toInt);
    UTEST_UNIT_RUN(TC_toLowerCase);
    UTEST_UNIT_RUN(TC_toUpperCase);
    UTEST_UNIT_RUN(TC_trim);
}

UTEST_TC_EXPORT(utest_do_tc, "RTduino.core.WString", RT_NULL, RT_NULL, 1000);
