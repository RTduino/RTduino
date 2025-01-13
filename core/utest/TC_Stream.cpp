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

#include "catch_compatible.h"

#include <RTduino.h>
#include <Stream.h>

#include <utest.h>

#include "Stream/mock/StreamMock.cpp"

static void TC_find(void)
{
    #include "Stream/test_find.cpp"
}

static void TC_findUntil(void)
{
    #include "Stream/test_findUntil.cpp"
}

static void TC_getTimeout(void)
{
    #include "Stream/test_getTimeout.cpp"
}

static void TC_parseFloat(void)
{
    #include "Stream/test_parseFloat.cpp"
}

static void TC_parseInt(void)
{
    #include "Stream/test_parseInt.cpp"
}

static void TC_readBytes(void)
{
    #include "Stream/test_readBytes.cpp"
}

static void TC_readBytesUntil(void)
{
    #include "Stream/test_readBytesUntil.cpp"
}

static void TC_readString(void)
{
    #include "Stream/test_readString.cpp"
}

static void TC_readStringUntil(void)
{
    #include "Stream/test_readStringUntil.cpp"
}

static void TC_setTimeout(void)
{
    #include "Stream/test_setTimeout.cpp"
}

/* Utest function to run all test cases */
static void utest_do_tc(void)
{
    UTEST_UNIT_RUN(TC_find);
    UTEST_UNIT_RUN(TC_findUntil);
    UTEST_UNIT_RUN(TC_getTimeout);
    UTEST_UNIT_RUN(TC_parseFloat);
    UTEST_UNIT_RUN(TC_parseInt);
    UTEST_UNIT_RUN(TC_readBytes);
    UTEST_UNIT_RUN(TC_readBytesUntil);
    UTEST_UNIT_RUN(TC_readString);
    UTEST_UNIT_RUN(TC_readStringUntil);
    UTEST_UNIT_RUN(TC_setTimeout);
}

UTEST_TC_EXPORT(utest_do_tc, "RTduino.core.Stream", RT_NULL, RT_NULL, 1000);
