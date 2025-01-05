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
 * 2025-01-05     Meco Man     first version
 */

#include <RTduino.h>
#include <Ringbuffer.h>
#include <utest.h>

#include "catch_compatible.h"


static void TC_available(void)
{
    #include "Ringbuffer/test_available.cpp"
}

static void TC_availableForStore(void)
{
    #include "Ringbuffer/test_availableForStore.cpp"
}

static void TC_clear(void)
{
    #include "Ringbuffer/test_clear.cpp"
}

static void TC_isFull(void)
{
    #include "Ringbuffer/test_isFull.cpp"
}

static void TC_peek(void)
{
    #include "Ringbuffer/test_peek.cpp"
}

static void TC_read_char(void)
{
    #include "Ringbuffer/test_read_char.cpp"
}

static void TC_store_char(void)
{
    #include "Ringbuffer/test_store_char.cpp"
}

/* Utest function to run all test cases */
static void utest_do_tc(void)
{
    UTEST_UNIT_RUN(TC_available);
    UTEST_UNIT_RUN(TC_availableForStore);
    UTEST_UNIT_RUN(TC_clear);
    UTEST_UNIT_RUN(TC_isFull);
    UTEST_UNIT_RUN(TC_peek);
    UTEST_UNIT_RUN(TC_read_char);
    UTEST_UNIT_RUN(TC_store_char);
}

UTEST_TC_EXPORT(utest_do_tc, "RTduino.core.Ringbuffer", RT_NULL, RT_NULL, 1000);
